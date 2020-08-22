/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 Universal-Team
*
*   This program is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*   Additional Terms 7.b and 7.c of GPLv3 apply to this file:
*       * Requiring preservation of specified reasonable legal notices or
*         author attributions in that material or in the Appropriate Legal
*         Notices displayed by works containing it.
*       * Prohibiting misrepresentation of the origin of that material,
*         or requiring that modified versions of such material be marked in
*         reasonable ways as different from the original version.
*/

#include "utils.hpp"
#include <regex>

bool Utils::matchPattern(std::string pattern, std::string tested) {
	std::regex patternRegex(pattern);
	return regex_match(tested, patternRegex);
}

static inline u32 Pow2(u32 x) {
	if (x <= 64) return 64;

	return 1u << (32 - __builtin_clz(x - 1));
}

// Convert RAW Data to C2D_Image.
C2D_Image C2DUtils::ImageDataToC2DImage(u32 *imageData, u32 width, u32 height, GPU_TEXCOLOR colorFormat) {

	u32		widthPow2 = Pow2(width);
	u32		heightPow2 = Pow2(height);
	u32*	buffer = (u32 *)linearAlloc(sizeof(u32) * widthPow2 * heightPow2);

	// Clear buffer.
	C3D_SyncMemoryFill(buffer, 0, (u32 *)((u8 *)buffer + (sizeof(u32) * widthPow2 * heightPow2)), BIT(0) | GX_FILL_32BIT_DEPTH, nullptr, 0, nullptr, 0);
	GSPGPU_FlushDataCache(buffer, widthPow2 * heightPow2 * sizeof(u32));

	// Copy Data.
	u32 *dst = buffer;
	u32 *src = imageData;

	for (u32 h = height; h > 0; h--) {
		memcpy(dst, src, width * sizeof(u32));
		dst += widthPow2;
		src += width;
	}

	GSPGPU_FlushDataCache(buffer, widthPow2 * heightPow2 * sizeof(u32));

	C3D_Tex *tex = new C3D_Tex();
	C3D_TexInit(tex, Pow2(width), Pow2(height), colorFormat);

	tex->param = GPU_TEXTURE_MAG_FILTER(GPU_NEAREST) | GPU_TEXTURE_MIN_FILTER(GPU_NEAREST)
		| GPU_TEXTURE_WRAP_S(GPU_CLAMP_TO_BORDER) | GPU_TEXTURE_WRAP_T(GPU_CLAMP_TO_BORDER);
	tex->border = 0xFFFFFFFF;

	C3D_SyncMemoryFill((u32 *)tex->data, 0, (u32 *)((u8 *)tex->data + tex->size), BIT(0) | (tex->fmt << 8), nullptr, 0, nullptr, 0);
	C3D_TexFlush(tex);

	C3D_SyncDisplayTransfer(buffer, GX_BUFFER_DIM(tex->width, tex->height), \
		(u32 *)tex->data, GX_BUFFER_DIM(tex->width, tex->height), TEXTURE_TRANSFER_FLAGS);

	C3D_TexFlush(tex);
	linearFree(buffer);

	Tex3DS_SubTexture *subtex = new Tex3DS_SubTexture();
	subtex->width = width;
	subtex->height = height;
	subtex->left = 0.f;
	subtex->top =  1.f;
	subtex->right = (float)width / (float)tex->width;
	subtex->bottom = 1.f - (float)height / (float)tex->height;

	C2D_Image image;
	image.tex = tex;
	image.subtex = subtex;

	return image;
}

// Delete | free's a C2D_Image.
void C2DUtils::C2D_ImageDelete(C2D_Image image) {
	if (image.tex != nullptr && image.subtex != nullptr) {
		C3D_TexDelete(image.tex);
		delete image.tex;
		delete image.subtex;
	}
}