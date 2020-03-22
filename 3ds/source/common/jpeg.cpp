/*
	MIT License
	This file is part of NLTK
	Copyright (c) 2018-2019 Slattz, Cuyler

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include "gfx.hpp"
#include "jpeg.h"
#include "msg.hpp"
#include "utils.hpp"

#include <3ds.h>
#include <citro2d.h>
#include <string>
#include <turbojpeg.h>

#define TPC_SIZE        0x1400
#define TPC_MAX_WIDTH   64
#define TPC_MAX_HEIGHT  104

extern C2D_SpriteSheet sprites;

static inline u32 Pow2(u32 x)
{
	if (x <= 2)
		return x;

	return 1u << (32 - __builtin_clz(x - 1));
}

JPEGInfo DecompressJPEG(const void * jpegSrc, const u32 jpegSize)
{
	JPEGInfo    jpeg = {0, 0, nullptr};
	int         jpegSubsamp;
	tjhandle    jpegDecompressor = tjInitDecompress();

	// Get the dimensions of the image
	if (tjDecompressHeader2(jpegDecompressor, (u8 *)jpegSrc, jpegSize, (int *)&jpeg.width, (int *)&jpeg.height, &jpegSubsamp))
		Msg::DisplayWarnMsg(std::string("tjDecompressHeader2: error\n") + tjGetErrorStr());

	// Allocate the temporary buffer
	u32 powSize = Pow2(jpeg.width) * Pow2(jpeg.height) * 4;
	jpeg.image = linearAlloc(powSize);

	if (!jpeg.image)
		Msg::DisplayWarnMsg("jpeg.image is null");

	// Clear temporary buffer
	C3D_SyncMemoryFill((u32 *)jpeg.image, 0, (u32 *)((u8 *)jpeg.image + powSize), BIT(0) | GX_FILL_32BIT_DEPTH, nullptr, 0, nullptr, 0);
	GSPGPU_FlushDataCache(jpeg.image, powSize);

	// Decompress the image
	if (tjDecompress2(jpegDecompressor, (u8 *)jpegSrc, jpegSize, (u8 *)jpeg.image, jpeg.width, 0, jpeg.height, TJPF_ABGR, TJFLAG_FASTDCT))
		Msg::DisplayWarnMsg(std::string("tjDecompress2: error\n") + tjGetErrorStr());
	GSPGPU_FlushDataCache(jpeg.image, powSize);

	tjDestroy(jpegDecompressor);

	return jpeg;
}

C2D_Image LoadPlayerPicture(const void *tpcData)
{
	JPEGInfo jpeg = DecompressJPEG(tpcData, TPC_SIZE);

	if (jpeg.width > TPC_MAX_WIDTH || jpeg.height > TPC_MAX_HEIGHT || jpeg.image == nullptr) {
		if (jpeg.image != nullptr)
			linearFree(jpeg.image);
		return C2D_SpriteSheetGetImage(sprites, sprites_noTPC_idx);
	}

	C2D_Image image = C2DUtils::ImageDataToC2DImage((u32*)jpeg.image, jpeg.width, jpeg.height, GPU_RGBA8);
	linearFree(jpeg.image);
	return image;
}
