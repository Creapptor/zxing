//
//  Decoder.h
//  ZXingWidget
//
//  Created by Marton Szabo on 4/19/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __PDF417_DECODER_H__
#define __PDF417_DECODER_H__


#include <zxing/common/Counted.h>
#include <zxing/common/Array.h>
#include <zxing/common/DecoderResult.h>
#include <zxing/common/BitMatrix.h>

namespace zxing {
    namespace pdf417 {
        
        class Decoder {
        private:
            static const int MAX_ERRORS;
            static const int MAX_EC_CODEWORDS;

            void verifyCodewordCount(ArrayRef<int> codewords, int numECCodewords);
            int correctErrors(ArrayRef<int> codewords, int* erasures, int eraseCount, int numECCodewords);
            
        public:
            Decoder();
            
            Ref<DecoderResult> decode(Ref<BitMatrix> bits);
        };
    }
}

#endif // __PDF417_DECODER_H__
