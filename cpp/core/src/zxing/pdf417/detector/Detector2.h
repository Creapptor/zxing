//
//  Detector2.h
//  ZXingWidget
//
//  Created by Marton Szabo on 5/2/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#ifndef __PDF417_DETECTOR2_H__
#define __PDF417_DETECTOR2_H__

#include <zxing/common/Counted.h>
#include <zxing/common/BitMatrix.h>
#include <zxing/pdf417/detector/Pdf417Barcode.h>
#include <zxing/common/DecoderResult.h>

#define PATTERN_NOT_FOUND -1
#define START_PATTERN_FOUND 1
#define END_PATTERN_FOUND 2

namespace zxing {
        
	namespace pdf417 {
		
		class Detector2 : public Counted {
		private:            
            
            static const int START_PATTERN[];
            static const int SYMBOL_TABLE[];
            static const char* EDGE_DISTANCE_TABLE[];
            
            int k0lc;
            int k3lc;
            int k6lc;
            int numOfRows;
            int errorCorrection;
            int numOfColumns;
            
            Ref<BitMatrix> image_;

            bool initialize(Ref<BitMatrix> matrix);
            void buildMatrix();
            bool analyzeRow(int* pattern, int symbolsToFind);

            bool foundPattern(int*  pattern);
            int findCodewordByEdges(int* edges);
            
            void convertSymbol(int pattern, int* edges);
            void printE(int* edges);
            int findCwIndexByEdgeDistance(char * ED);

            void scanBlocks(int y, int * blocks);
            bool scanLineInitial(int y, Pdf417Line* pdfLine);
            void scan(Pdf417Barcode* barcode);
            int findStartStopPattern(int *blocks, int& where);
            bool readSymbol(int* blocks, int from, int& K, int& codeword);
            void extractColumnIndicator(Pdf417Symbol* symbol, bool left);
            int getLineDifference(int k1, int k2);
            int getLineDifferenceSigned(int k1, int k2);
		public:
			Detector2(Ref<BitMatrix> image);
            Ref<DecoderResult> detect();

		};
	}
}

#endif 