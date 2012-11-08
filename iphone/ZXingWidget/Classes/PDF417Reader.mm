//
//  PDF417Reader.m
//  ZXingWidget
//
//  Created by Marton Szabo on 4/19/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#import "PDF417Reader.h"
#import <zxing/pdf417/Pdf417Reader.h>
#import "FormatReader.h"

@implementation PDF417Reader

- (id) init {
    zxing::pdf417::Pdf417Reader *reader = new zxing::pdf417::Pdf417Reader();
    return [super initWithReader:reader];
}

@end
