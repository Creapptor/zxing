//
//  Pdf417Reader.cpp
//  ZXingWidget
//
//  Created by Marton Szabo on 4/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <zxing/pdf417/PDF417Reader.h>
#include <zxing/pdf417/detector/Detector.h>
#include <zxing/pdf417/detector/Detector2.h>

#include <iostream>

namespace zxing {
	namespace pdf417 {
		
		using namespace std;
		
		Pdf417Reader::Pdf417Reader() :decoder_() {
            
		}
        
		//TODO: see if any of the other files in the qrcode tree need tryHarder
		Ref<Result> Pdf417Reader::decode(Ref<BinaryBitmap> image, DecodeHints hints) {
//#ifdef DEBUG
//			cout << "decoding image " << image.object_ << ":\n" << flush;
//#endif
//			
//			Detector detector(image->getBlackMatrix());
//			
//#ifdef DEBUG
//			cout << "(1) created detector " << &detector << "\n" << flush;
//#endif
//			
//			Ref<DetectorResult> detectorResult(detector.detect(hints));
//#ifdef DEBUG
//			cout << "(2) detected, have detectorResult " << detectorResult.object_ << "\n" << flush;
//#endif
//			
//			std::vector<Ref<ResultPoint> > points(detectorResult->getPoints());
//			
//			
//#ifdef DEBUG
//			cout << "(3) extracted points " << &points << "\n" << flush;
//			cout << "found " << points.size() << " points:\n";
//			for (size_t i = 0; i < points.size(); i++) {
//				cout << "   " << points[i]->getX() << "," << points[i]->getY() << "\n";
//			}
//			cout << "bits:\n";
//			cout << *(detectorResult->getBits()) << "\n";
//#endif
//			
//			Ref<DecoderResult> decoderResult(decoder_.decode(detectorResult->getBits()));
//#ifdef DEBUG
//			cout << "(4) decoded, have decoderResult " << decoderResult.object_ << "\n" << flush;
//#endif
		
            
            Detector2 detector2(image->getBlackMatrix());
            Ref<DecoderResult> decoderResult(detector2.detect());
            std::vector<Ref<ResultPoint> > points;
            
			Ref<Result> result(
							   new Result(decoderResult->getText(), decoderResult->getRawBytes(), points, BarcodeFormat_PDF417));
#ifdef DEBUG
			cout << "(5) created result " << result.object_ << ", returning\n" << flush;
#endif
			//Rugged and Mobile Ltd, Mersey House,Speke Road, Liverpool, L19 2PH, UK
			return result;
		}
		
		Pdf417Reader::~Pdf417Reader() {
		}
		
	}
}
