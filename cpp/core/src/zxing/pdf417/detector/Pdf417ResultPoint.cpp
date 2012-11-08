//
//  Pdf417ResultPoint.cpp
//  ZXingWidget
//
//  Created by Marton Szabo on 4/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include <zxing/pdf417/detector/Pdf417ResultPoint.h>

namespace zxing {
    namespace pdf417 {
        
        using namespace std;
        
        Pdf417ResultPoint::Pdf417ResultPoint(float posX, float posY) :
            posX_(posX), posY_(posY) {
        }
        
        float Pdf417ResultPoint::getX() const {
            return posX_;
        }
        
        float Pdf417ResultPoint::getY() const {
            return posY_;
        }
        
        void Pdf417ResultPoint::setX(float f) {
            posX_ = f;
        }
        
        void Pdf417ResultPoint::setY(float f) {
            posY_ = f;
        };
    }
}
