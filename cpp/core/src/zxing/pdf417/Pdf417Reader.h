#ifndef __PDF417_READER_H__
#define __PDF417_READER_H__


//
//  Pdf417Reader.h
//  ZXingWidget
//
//  Created by Marton Szabo on 4/18/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#include <zxing/Reader.h>
#include <zxing/pdf417/decoder/Decoder.h>
#include <zxing/DecodeHints.h>

namespace zxing {
	namespace pdf417 {
		
		class Pdf417Reader : public Reader {
		private:
			Decoder decoder_;
			
		public:
			Pdf417Reader();
			virtual Ref<Result> decode(Ref<BinaryBitmap> image, DecodeHints hints);
			virtual ~Pdf417Reader();
		};
	}
}

#endif // __PDF417_READER_H__ 