//
//  DecodedBitStreamParser.h
//  ZXingWidget
//
//  Created by Marton Szabo on 4/19/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#ifndef __PDF417_DECODED_BIT_STREAM_PARSER_H__
#define __PDF417_DECODED_BIT_STREAM_PARSER_H__

#include <string>
#include <sstream>
#include <zxing/common/BitSource.h>
#include <zxing/common/Counted.h>
#include <zxing/common/Array.h>
#include <zxing/ReaderException.h>

namespace zxing {
    namespace pdf417 {
        
        class DecodedBitStreamParser {
        private:
            static const char *ASCII;
            static const char *ISO88591;
            static const char *UTF8;
            static const char *SHIFT_JIS;
            static const char *EUC_JP;
            
            static const int TEXT_COMPACTION_MODE_LATCH;
            static const int BYTE_COMPACTION_MODE_LATCH;
            static const int NUMERIC_COMPACTION_MODE_LATCH;
            static const int BYTE_COMPACTION_MODE_LATCH_6;
            static const int BEGIN_MACRO_PDF417_CONTROL_BLOCK;
            static const int BEGIN_MACRO_PDF417_OPTIONAL_FIELD;
            static const int MACRO_PDF417_TERMINATOR;
            static const int MODE_SHIFT_TO_BYTE_COMPACTION_MODE;
            static const int MAX_NUMERIC_CODEWORDS;
            
            static const int ALPHA;
            static const int LOWER;
            static const int MIXED;
            static const int PUNCT;
            static const int PUNCT_SHIFT;
            
            static const int PL;
            static const int LL;
            static const int AS;
            static const int ML;
            static const int AL;
            static const int PS;
            static const int PAL;
            
            static const char PUNCT_CHARS[];
            
            static const char MIXED_CHARS[];
            static const char* EXP900[];
            
            int textCompaction(ArrayRef<int> codewords, int codeIndex, std::string &result);
            void decodeTextCompaction(ArrayRef<int> textCompactionData, ArrayRef<int> byteCompactionData, int length, std::string &result);
            int byteCompaction(int mode, ArrayRef<int> codewords, int codeIndex, std::string &result);
            int numericCompaction(ArrayRef<int> codewords, int codeIndex, std::string &result);
            void append(std::string &ost, const unsigned char *bufIn, size_t nIn, const char *src);
            const char *guessEncoding(unsigned char *bytes, int length);
            std::string decodeBase900toBase10(ArrayRef<int> codewords, int count);
            std::string multiply( std::string &value1, int value2);
            std::string add(std::string &value1, std::string &value2);
            
        public:
            std::string decode(ArrayRef<int> codewords);
        };
    }
}

#endif // __PDF417_DECODED_BIT_STREAM_PARSER_H__
