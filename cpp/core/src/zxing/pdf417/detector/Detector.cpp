
#include <zxing/pdf417/detector/Detector.h>
#include <zxing/pdf417/detector/Pdf417ResultPoint.h>
#include <zxing/common/GridSampler.h>
#include <zxing/DecodeHints.h>
#include <zxing/ReaderException.h>
#include <cmath>
#include <sstream>
#include <cstdlib>
#include <iostream>

namespace zxing {
	namespace pdf417 {
		
		using namespace std;
        using zxing::pdf417::Detector;
        using zxing::DetectorResult;
        using zxing::pdf417::Pdf417ResultPoint;
        using zxing::Ref;
        using zxing::BitMatrix;
        using zxing::ResultPointCallback;
        using zxing::PerspectiveTransform;

		
        const int Detector::MAX_AVG_VARIANCE = (int) ((1 << 8) * 0.42f);
        const int Detector::MAX_INDIVIDUAL_VARIANCE = (int) ((1 << 8) * 0.8f);
        const int Detector::SKEW_THRESHOLD = 2;
        
        const int Detector::START_PATTERN[] = {8, 1, 1, 1, 1, 1, 1, 3};
        const int Detector::START_PATTERN_REVERSE[] = {3, 1, 1, 1, 1, 1, 1, 8};
        const int Detector::STOP_PATTERN[] = {7, 1, 1, 3, 1, 1, 1, 2, 1};
        const int Detector::STOP_PATTERN_REVERSE[] = {1, 2, 1, 1, 1, 3, 1, 1, 7};

        
		Detector::Detector(Ref<BitMatrix> image) :
            image_(image) {
        }
        
        Ref<BitMatrix> Detector::getImage() {
            return image_;
        }
        
		Ref<DetectorResult> Detector::detect(DecodeHints const& hints) {
            callback_ = hints.getResultPointCallback();
            
#ifdef DEBUG
			cout << "trying to detect in matrix:\n";
			cout << *(image_) << "\n";
#endif


            
            // Try to find the vertices assuming the image is upright.
            Ref<ResultPoint> vertices[8];
            bool found = false;
            if (!(found = findVertices(image_, vertices))) {
                // Maybe the image is rotated 180 degrees?
                if ((found = findVertices180(image_, vertices))) {
                    correctCodeWordVertices(vertices, true);
                }
            } else {
                correctCodeWordVertices(vertices, false);
            }
            
            if (!found) {
                throw zxing::ReaderException("not found pdf417");
            }
            
            float moduleWidth = computeModuleWidth(vertices);
            if (moduleWidth < 1.0f) {
                throw zxing::ReaderException("bad module size");
            }
            
            int dimension = computeDimension(vertices[4], vertices[6],
                                             vertices[5], vertices[7], moduleWidth);

            if (dimension  < 1) {
                throw zxing::ReaderException("not found pdf417, dimension < 1");
            }
            //tl, tr, bl, br, dim
            Ref<PerspectiveTransform> transform = createTransform(vertices[4], vertices[6], vertices[5], vertices[7], dimension);
            GridSampler &sampler = GridSampler::getInstance();
            
            Ref<BitMatrix> bits(sampler.sampleGrid(image_, dimension, transform));
            
            //Ref<BitMatrix> bits(sampler.scaleGridByNearestNeighbour(image_, moduleWidth));
            
            std::vector<Ref<ResultPoint> > points(4);
            points[0].reset(vertices[4]);
            points[1].reset(vertices[5]);
            points[2].reset(vertices[6]);
            points[3].reset(vertices[7]);

#ifdef DEBUG
            cout << "resampled matrix:\n";
            cout << *(bits) << "\n";
#endif

            // Ref<DetectorResult> result(new DetectorResult(bits, points, transform)); // FIXME?
            Ref<DetectorResult> result(new DetectorResult(bits, points));
            return result;
		}
        
        bool Detector::findVertices(Ref<BitMatrix> matrix, Ref<ResultPoint> *result) {
            int height = matrix->getHeight();
            int width = matrix->getWidth();
            
            bool found = false;
            int loc[2];            
            
            int foundIndexes[4];
            
            // Top Left
            for (int i = 0; i < height; i++) {
                if (findGuardPattern(matrix, 0, i, width, false, START_PATTERN, 8, loc)) {
                    result[0] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[0], i));
                    result[4] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[1], i));
                    found = true;
                    foundIndexes[0] = i;
                    break;
                }
            }

            // Bottom right
            if (found) { // Found the Top right vertex
                found = false;
                for (int i = height - 1; i > 0; i--) {
                    if (findGuardPattern(matrix, 0, i, width, false, STOP_PATTERN, 8, loc)) {
                        result[3] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[1], i));
                        result[7] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[0], i));
                        found = true;
                        foundIndexes[3] = height - i;                        
                        break;
                    }
                }
            }

            // Bottom left
            if (found) { // Found the Top Left vertex
                found = false;
                for (int i = height - 1; i > 0; i--) {
                    if (findGuardPattern(matrix, 0, i, width, false, START_PATTERN, 8, loc)) {
                        result[1] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[0], i));
                        result[5] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[1], i));
                        found = true;
                        foundIndexes[1] = height - i;
                        break;
                    }
                }
            }
            // Top right
            if (found) { // Found the Bottom Left vertex
                found = false;
                for (int i = 0; i < height; i++) {
                    if (findGuardPattern(matrix, 0, i, width, false, STOP_PATTERN, 8, loc)) {
                        result[2] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[1], i));
                        result[6] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[0], i));
                        found = true;
                        foundIndexes[2] = i;
                        break;
                    }
                }
            }
            return found;
        }
        
        bool Detector::findVertices180(Ref<BitMatrix> matrix, Ref<ResultPoint> *result) {
            int height = matrix->getHeight();
            int width = matrix->getWidth();
            int halfWidth = width >> 1;
            
            bool found = false;
            int loc[2];            
            // Top Left
            for (int i = height - 1; i > 0; i--) {
                if (findGuardPattern(matrix, halfWidth, i, halfWidth, true, START_PATTERN_REVERSE, 8, loc)) {
                    result[0] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[1], i));
                    result[4] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[0], i));
                    found = true;
                    break;
                }
            }
            if (found) { // Found the Top Right vertex
                found = false;
                for (int i = 0; i < height; i++) {
                    if (findGuardPattern(matrix, 0, i, halfWidth, false, STOP_PATTERN_REVERSE, 8, loc)) {
                        result[3] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[0], i));
                        result[7] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[1], i));
                        found = true;
                        break;
                    }
                }
            }
            // Bottom Left
            if (found) { // Found the Top Left vertex
                found = false;
                for (int i = 0; i < height; i++) {
                    if (findGuardPattern(matrix, halfWidth, i, halfWidth, true, START_PATTERN_REVERSE, 8, loc)) {
                        result[1] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[1], i));
                        result[5] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[0], i));
                        found = true;
                        break;
                    }
                }
            }
            // Top Right
            if (found) { // Found the Bottom Left vertex
                found = false;
                for (int i = height - 1; i > 0; i--) {
                    if (findGuardPattern(matrix, 0, i, halfWidth, false, STOP_PATTERN_REVERSE, 8, loc)) {
                        result[2] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[0], i));
                        result[6] = Ref<Pdf417ResultPoint>(new Pdf417ResultPoint(loc[1], i));
                        found = true;
                        break;
                    }
                }
            }
            // Bottom Right
            return found;
        }
        
        void Detector::correctCodeWordVertices(Ref<ResultPoint> *vertices, bool upsideDown) {
            float skew = vertices[4]->getY() - vertices[6]->getY();
            if (upsideDown) {
                skew = -skew;
            }
            if (skew > Detector::SKEW_THRESHOLD) {
                // Fix v4
                float length = vertices[4]->getX() - vertices[0]->getX();
                float deltax = vertices[6]->getX() - vertices[0]->getX();
                float deltay = vertices[6]->getY() - vertices[0]->getY();
                float correction = length * deltay / deltax;
                vertices[4]->setY(vertices[4]->getY() + correction);
            } else if (-skew > Detector::SKEW_THRESHOLD) {
                // Fix v6
                float length = vertices[2]->getX() - vertices[6]->getX();
                float deltax = vertices[2]->getX() - vertices[4]->getX();
                float deltay = vertices[2]->getY() - vertices[4]->getY();
                float correction = length * deltay / deltax;
                vertices[6]->setY(vertices[6]->getY() - correction);
            }
            
            skew = vertices[7]->getY() - vertices[5]->getY();
            if (upsideDown) {
                skew = -skew;
            }
            if (skew > Detector::SKEW_THRESHOLD) {
                // Fix v5
                float length = vertices[5]->getX() - vertices[1]->getX();
                float deltax = vertices[7]->getX() - vertices[1]->getX();
                float deltay = vertices[7]->getY() - vertices[1]->getY();
                float correction = length * deltay / deltax;
                vertices[5]->setY(vertices[5]->getY() + correction);
            } else if (-skew > Detector::SKEW_THRESHOLD) {
                // Fix v7
                float length = vertices[3]->getX() - vertices[7]->getX();
                float deltax = vertices[3]->getX() - vertices[5]->getX();
                float deltay = vertices[3]->getY() - vertices[5]->getY();
                float correction = length * deltay / deltax;
                vertices[7]->setY(vertices[7]->getY() - correction);
            }
        }
        
        float Detector::computeModuleWidth(Ref<ResultPoint> *vertices) {
            float pixels1 = Pdf417ResultPoint::distance(vertices[0], vertices[4]);
            float pixels2 = Pdf417ResultPoint::distance(vertices[1], vertices[5]);
            float moduleWidth1 = (pixels1 + pixels2) / (17 * 2.0f);
            float pixels3 = Pdf417ResultPoint::distance(vertices[6], vertices[2]);
            float pixels4 = Pdf417ResultPoint::distance(vertices[7], vertices[3]);
            float moduleWidth2 = (pixels3 + pixels4) / (18 * 2.0f);
            return (moduleWidth1 + moduleWidth2) / 2.0f;
        }
        
        
        int Detector::computeDimension(Ref<ResultPoint> topLeft, Ref<ResultPoint> topRight,
                            Ref<ResultPoint> bottomLeft, Ref<ResultPoint> bottomRight, float moduleWidth) {
            int topRowDimension = round(Pdf417ResultPoint::distance(topLeft, topRight) / moduleWidth);
            int bottomRowDimension = round(Pdf417ResultPoint::distance(bottomLeft, bottomRight) / moduleWidth);
            return ((((topRowDimension + bottomRowDimension) >> 1) + 8) / 17) * 17;
        }
        
        Ref<PerspectiveTransform> Detector::createTransform(Ref<ResultPoint> topLeft, Ref<ResultPoint> topRight, 
                                                            Ref <ResultPoint> bottomLeft, Ref <ResultPoint> bottomRight, 
                                                            int dimension) {
            Ref<PerspectiveTransform> transform(
                        PerspectiveTransform::quadrilateralToQuadrilateral(
                               0.0f, // p1ToX
                               0.0f, // p1ToY
                               dimension, // p2ToX
                               0.0f, // p2ToY
                               dimension, // p3ToX
                               dimension, // p3ToY
                               0.0f, // p4ToX
                               dimension, // p4ToY
                               topLeft->getX(), // p1FromX
                               topLeft->getY(), // p1FromY
                               topRight->getX(), // p2FromX
                               topRight->getY(), // p2FromY
                               bottomRight->getX(), // p3FromX
                               bottomRight->getY(), // p3FromY
                               bottomLeft->getX(), // p4FromX
                               bottomLeft->getY()));
            return transform;

        }
                
        bool Detector::findGuardPattern(Ref<BitMatrix> matrix, int column, int row, int width, bool whiteFirst, const int* pattern, int patternLen, int *result) {

            ArrayRef<int> counters(patternLen);
            for(int i=0; i<patternLen; i++){
                counters[i] = 0;
            }
            bool isWhite = whiteFirst;
            
            int counterPosition = 0;
            int patternStart = column;
            for (int x = column; x < column + width; x++) {
                bool pixel = matrix->get(x, row);
                if (pixel ^ isWhite) {
                    counters[counterPosition]++;
                } else {
                    if (counterPosition == patternLen - 1) {
                        if (patternMatchVariance(counters, pattern, patternLen, Detector::MAX_INDIVIDUAL_VARIANCE) < Detector::MAX_AVG_VARIANCE) {
                            result[0] = patternStart;
                            result[1] = x;
                            return true;
                        }
                        patternStart += counters[0] + counters[1];
                        for (int y = 2; y < patternLen; y++) {
                            counters[y - 2] = counters[y];
                        }
                        counters[patternLen - 2] = 0;
                        counters[patternLen - 1] = 0;
                        counterPosition--;
                    } else {
                        counterPosition++;
                    }
                    counters[counterPosition] = 1;
                    isWhite = !isWhite;
                }
            }
            return false;
        }
		
        
        int Detector::patternMatchVariance(ArrayRef<int> counters, const int* pattern, int arrayLen, int maxIndividualVariance) {
            int numCounters = arrayLen;
            int total = 0;
            int patternLength = 0;
            for (int i = 0; i < numCounters; i++) {
                total += counters[i];
                patternLength += pattern[i];
            }
            if (total < patternLength) {
                return INT_MAX;
            }
            int unitBarWidth = (total << 8) / patternLength;
            maxIndividualVariance = (maxIndividualVariance * unitBarWidth) >> 8;
            
            int totalVariance = 0;
            for (int x = 0; x < numCounters; x++) {
                int counter = counters[x] << 8;
                int scaledPattern = pattern[x] * unitBarWidth;
                int variance = counter > scaledPattern ? counter - scaledPattern : scaledPattern - counter;
                if (variance > maxIndividualVariance) {
                    return INT_MAX;
                }
                totalVariance += variance;
            }
            return totalVariance / total;
        }
	}
}
