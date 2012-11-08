//
//  Decoder.cpp
//  ZXingWidget
//
//  Created by Marton Szabo on 4/19/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#include <zxing/pdf417/decoder/Decoder.h>
#include <zxing/pdf417/decoder/BitMatrixParser.h>
#include <zxing/pdf417/decoder/DecodedBitStreamParser.h>
#include <zxing/pdf417/detector/Pdf417Barcode.h>
#include <zxing/ReaderException.h>
#include <zxing/common/Str.h>

namespace zxing {
    namespace pdf417 {
        
        using namespace std;
        
        const int Decoder::MAX_ERRORS = 3;
        const int Decoder::MAX_EC_CODEWORDS = 512;

        
        Decoder::Decoder() {
            
        }
        
        Ref<DecoderResult> Decoder::decode(Ref<BitMatrix> bits) {
            // Construct a parser to read the data codewords and error-correction level
            BitMatrixParser parser(bits);
            ArrayRef<int> codewords = parser.readCodewords();
            if (codewords->size() == 0) {
                throw ReaderException("No codewords found");
            }
            
            int ecLevel = parser.getECLevel();
            int numECCodewords = 1 << (ecLevel + 1);
            int* erasures = parser.getErasures();
            
            correctErrors(codewords, erasures, parser.getEraseCount(), numECCodewords);
            verifyCodewordCount(codewords, numECCodewords);
            
            DecodedBitStreamParser bsParser;
            Ref<String> text(new String(bsParser.decode(codewords)));
            ArrayRef<unsigned char> rawBytes(1);
            Ref<DecoderResult> result(new DecoderResult(rawBytes, text));
            return result;
        }
        

        
        void Decoder::verifyCodewordCount(ArrayRef<int> codewords, int numECCodewords) {
            if (codewords->size() < 4) {
                // Codeword array size should be at least 4 allowing for
                // Count CW, At least one Data CW, Error Correction CW, Error Correction CW
                throw ReaderException("Codewords array size smaller than 4");
            }
            // The first codeword, the Symbol Length Descriptor, shall always encode the total number of data
            // codewords in the symbol, including the Symbol Length Descriptor itself, data codewords and pad
            // codewords, but excluding the number of error correction codewords.
            int numberOfCodewords = codewords[0];
            if (numberOfCodewords > (int)codewords->size()) {
                throw ReaderException("Invalid size of codewords");
            }
            if (numberOfCodewords == 0) {
                // Reset to the length of the array - 8 (Allow for at least level 3 Error Correction (8 Error Codewords)
                if (numECCodewords < (int)codewords->size()) {
                    codewords[0] = codewords->size() - numECCodewords;
                } else {
                    throw ReaderException("Invalid size of codewords");
                }
            }
        }
        
        int Decoder::correctErrors(ArrayRef<int> codewords, int* erasures, int eraseCount, int numECCodewords) {
            //if ((erasures!=NULL && eraseCount > numECCodewords / 2 + MAX_ERRORS) ||
            if ((erasures!=NULL && eraseCount > numECCodewords -3) ||
                (numECCodewords < 0 || numECCodewords > MAX_EC_CODEWORDS)) {
                // Too many errors or EC Codewords is corrupted
                throw ReaderException("Too many errors or EC codewords corrupted");
            }

            Pdf417RSDecoder decoder;
            int num = decoder.correctErrors(codewords, NULL, 0, codewords->size(), codewords->size() - codewords[0]);
            if (num < 0) {
                throw ReaderException("Could not correct errors");
            }
            else {
                return num;   
            }
        }
        
    }
}