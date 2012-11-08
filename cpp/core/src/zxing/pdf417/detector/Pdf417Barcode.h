//
//  Pdf417Matrix.h
//  ZXingWidget
//
//  Created by Marton Szabo on 5/3/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __PDF417_MATRIX_H__
#define __PDF417_MATRIX_H__

#define PRIM	1
#define GPRIME	929
#define A0	928
#define Ldec	1

#define modbase(x) ((x) % (GPRIME - 1))

#include <zxing/common/Counted.h>
#include <zxing/common/Array.h>
#include <vector>

namespace zxing {
    
	namespace pdf417 {
 
        using namespace std;
        
        class Pdf417Symbol {
		public:
            
			//Pdf417Symbol() : K(-1), codeword(-1) {}
            Pdf417Symbol(int k, int cw) : K(k), codeword(cw) {}
            
            int K;
            int codeword;
            
            static bool getPatternEdges(int *pattern, int* edges);
            static bool checkPatternIsStartLocator(int* pattern);
            static bool checkPatternIsEndLocator(int* pattern);
            static int getPatternSizeInPixels(int* pattern);
            static bool checkPatternBlockLengths(int* pattern);
            static int getCluster(int* edges);
            
            ~Pdf417Symbol() {}
		};
        
        class Pdf417Line {
		public:

			Pdf417Line();
            ~Pdf417Line();
            
            Pdf417Symbol* leftColumnIndicator;
            Pdf417Symbol* rightColumnIndicator;
            std::vector<Pdf417Symbol> symbols;
		};
        
        
        class Pdf417Barcode {
		private:
			int **_matrix;
            int rows;
            int columns;
            int ecLevel;
            int erasures;
            
		public:
			Pdf417Barcode(int r, int c, int ec);
            void set(int x, int y, int codeword);
            void setGuess(int x, int y, int codeword);
            int get(int x, int y);

            
            inline int getRows(){ return this->rows; }
            inline int getColumns(){ return this->columns; }
            inline int getECLevel(){ return this->ecLevel; }
            inline int getErasures(){ return this->erasures; }
            
            ArrayRef<int> getCodewords();
            bool correctErrors(ArrayRef<int> codewords);
            
            ~Pdf417Barcode();
		};

        class Pdf417RSDecoder {
        public:
            int Alpha_to[1024];
            int Index_of[1024];

            Pdf417RSDecoder();
            int correctErrors(ArrayRef<int> data, int *eras_pos, int no_eras,
                            int data_len, int synd_len);
            ~Pdf417RSDecoder(){ }
        };
        
    }
}

#endif // __PDF417_MATRIX_H__