//
//  Detector.h
//  ZXingWidget
//
//  Created by Marton Szabo on 4/18/11.
//  jollyblade@gmail.com
//


#ifndef __PDF417_DETECTOR_H__
#define __PDF417_DETECTOR_H__

#include <zxing/common/Counted.h>
#include <zxing/common/DetectorResult.h>
#include <zxing/common/BitMatrix.h>
#include <zxing/common/PerspectiveTransform.h>
#include <zxing/ResultPointCallback.h>

namespace zxing {
    
    class DecodeHints;
    
	namespace pdf417 {
		
		class Detector : public Counted {
		private:
            static const int MAX_AVG_VARIANCE;
            static const int MAX_INDIVIDUAL_VARIANCE;
            static const int SKEW_THRESHOLD;

            static const int START_PATTERN[];
            static const int START_PATTERN_REVERSE[];
            static const int STOP_PATTERN[];
            static const int STOP_PATTERN_REVERSE[];
                
            Ref<BitMatrix> image_;
            Ref<ResultPointCallback> callback_;
			
		public:
			Detector(Ref<BitMatrix> image);
            Ref<DetectorResult> detect(DecodeHints const& hints);
            Ref<BitMatrix> getImage();
            
            bool findVertices(Ref<BitMatrix> matrix, Ref<ResultPoint> *result);
            bool findVertices180(Ref<BitMatrix> matrix, Ref<ResultPoint> *result);
            void correctCodeWordVertices(Ref<ResultPoint> *result, bool upsideDown);
            float computeModuleWidth(Ref<ResultPoint> *result);
            
            int computeDimension(Ref<ResultPoint> topLeft, Ref<ResultPoint> topRight,
                                 Ref<ResultPoint> bottomLeft, Ref<ResultPoint> bottomRight, float moduleWidth);
            
            bool findGuardPattern(Ref<BitMatrix> matrix, int column, int row, int width, bool whiteFirst, const int* pattern, int arrayLen, int *result);
            
            int patternMatchVariance(ArrayRef<int> counters, const int* pattern, int patternLen, int maxIndividualVariance);
            
            Ref<PerspectiveTransform> createTransform(Ref<ResultPoint> topLeft, Ref<ResultPoint> topRight, 
                                                                Ref <ResultPoint> bottomLeft, Ref <ResultPoint> bottomRight, 
                                                      int dimension);
		};
	}
}

#endif // __PDF417_DETECTOR_H__
