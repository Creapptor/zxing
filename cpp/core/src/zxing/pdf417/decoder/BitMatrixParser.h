//
//  BitMatrixParser.h
//  ZXingWidget
//
//  Created by Marton Szabo on 4/19/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#ifndef __PDF417_BIT_MATRIX_PARSER_H__
#define __PDF417_BIT_MATRIX_PARSER_H__

#include <zxing/ReaderException.h>
#include <zxing/common/BitMatrix.h>
#include <zxing/common/Counted.h>
#include <zxing/common/Array.h>

namespace zxing {
    namespace pdf417 {
        
        class BitMatrixParser : public Counted {
        private:
            Ref<BitMatrix> bitMatrix_;
            
            static int MAX_ROW_DIFFERENCE;
            static int MAX_ROWS;
            static int MAX_CW_CAPACITY;
            static int MODULES_IN_SYMBOL;
            
            static const int CODEWORD_TABLE[];
            
            int rows;
            int leftColumnECData;
            int rightColumnECData;
            int eraseCount;
            int* erasures;
            int ecLevel;
            
        public:
            static int SYMBOL_TABLE[];

            BitMatrixParser(Ref<BitMatrix> bitMatrix);
            
            int processRow(ArrayRef<int> rowCounters, int rowCountersSize, int rowNumber, int rowHeight, ArrayRef<int> codewords, int codewordsSize, int next); 
            ArrayRef<int> readCodewords();
            int getCodeword(long symbol);
            int findCodewordIndex(long symbol);
            static int getCodewordByIndex(int idx);
            
            inline int* getErasures() {
                return erasures;
            }
            
            inline int getEraseCount() {
                return eraseCount;
            }

            
            inline int getECLevel() {
                return ecLevel;
            }
            
        private:
            BitMatrixParser(const BitMatrixParser&);
            BitMatrixParser& operator =(const BitMatrixParser&);         
        };
    }
}

#endif // __PDF417_BIT_MATRIX_PARSER_H__
