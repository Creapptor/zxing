//
//  Pdf417ResultPoint.h
//  ZXingWidget
//
//  Created by Marton Szabo on 4/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __PDF417_RESULT_POINT_H__
#define __PDF417_RESULT_POINT_H__

#include <zxing/ResultPoint.h>
#include <cmath>

namespace zxing {
	namespace pdf417 {
		
		class Pdf417ResultPoint : public ResultPoint {
		private:
			float posX_;
			float posY_;
			
		public:
			Pdf417ResultPoint(float posX, float posY);
			float getX() const;
			float getY() const;
            void setX(float f);
            void setY(float f);
            
            static inline float distance(Ref<ResultPoint> p1, Ref<ResultPoint> p2) {
                float xDiff = p1->getX() - p2->getX();
                float yDiff = p1->getY() - p2->getY();
                return (float) sqrt((double) (xDiff * xDiff + yDiff * yDiff));
            }
		};
		
	}
}

#endif // __PDF417_RESULT_POINT_H__
