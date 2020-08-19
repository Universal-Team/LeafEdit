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

#include "coreUtils.hpp"
#include "overlay.hpp"
#include "rawPatternEditor.hpp"
#include "screenCommon.hpp"
#include "stringUtils.hpp"
#include "utils.hpp"

extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);

/* Needed to display Palettes. */
static const u32 WWPaletteColors[] = {
	0xFF0000FF, 0xFF3173FF, 0xFF00ADFF, 0xFF00FFFF, 0xFF00FFAD, 0xFF00FF52, 0xFF00FF00, 0xFF52AD00, 0xFFAD5200, 0xFFFF0000, 0xFFFF0052, 0xFFFF00AD, 0xFFFF00FF, 0xFF000000, 0xFFFFFFFF,
	0xFF7B7BFF, 0xFF7BB5FF, 0xFF7BE7FF, 0xFF7BFFFF, 0xFF7BFFDE, 0xFF7BFFAD, 0xFF7BFF7B, 0xFF84AD52, 0xFFAD8452, 0xFFFF7B7B, 0xFFFF7BB5, 0xFFFF7BE7, 0xFFFF7BFF, 0xFF000000, 0xFFFFFFFF,
	0xFF0000A5, 0xFF0031A5, 0xFF0073A5, 0xFF00A5A5, 0xFF00A573, 0xFF00A531, 0xFF00A500, 0xFF215200, 0xFF522100, 0xFFA50000, 0xFFA50031, 0xFFA50073, 0xFFA500A5, 0xFF000000, 0xFFFFFFFF,
	0xFF009C00, 0xFF6BCE5A, 0xFFDEFFB5, 0xFF6B9C00, 0xFFA5CE52, 0xFFD6FFAD, 0xFFAD5200, 0xFFD68429, 0xFFFFAD5A, 0xFFFF0000, 0xFFFF6B4A, 0xFFDE4A31, 0xFFB52118, 0xFF8C0000, 0xFFFFFFFF,
	0xFF0073AD, 0xFF42ADD6, 0xFF8CDEFF, 0xFF3908FF, 0xFF6B4AFF, 0xFF9C94FF, 0xFFFF00AD, 0xFFFF63D6, 0xFFFFCEFF, 0xFF9CBDFF, 0xFF7394DE, 0xFF4A63BD, 0xFF21399C, 0xFF00107B, 0xFFFFFFFF,
	0xFF0000FF, 0xFF0052FF, 0xFF5AB5FF, 0xFFADEFFF, 0xFF00107B, 0xFF314AA5, 0xFF6B84D6, 0xFF9CBDFF, 0xFFFFAD5A, 0xFFFFC684, 0xFFFFE7AD, 0xFFFFFFD6, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF00FF00, 0xFF42FF42, 0xFF8CFF8C, 0xFFD6FFD6, 0xFFFF0000, 0xFFFF4242, 0xFFFF8C8C, 0xFFFFD6D6, 0xFF0000FF, 0xFF4242FF, 0xFF8C8CFF, 0xFFD6D6FF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF003100, 0xFF426342, 0xFF849C84, 0xFFC6D6C6, 0xFF00107B, 0xFF294AA5, 0xFF5A8CD6, 0xFF8CC6FF, 0xFF00B5D6, 0xFF39CEE7, 0xFF7BDEF7, 0xFFBDF7FF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFFFF0000, 0xFF0000FF, 0xFF00FFFF, 0xFFFF4242, 0xFF4242FF, 0xFF42FFFF, 0xFFFF8C8C, 0xFF8C8CFF, 0xFF8CFFFF, 0xFFFFD6D6, 0xFFD6D6FF, 0xFFD6FFFF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF00FF00, 0xFFFF0000, 0xFFFF00FF, 0xFF42FF42, 0xFFFF4242, 0xFFFF42FF, 0xFF8CFF8C, 0xFFFF8C8C, 0xFFFF8CFF, 0xFFD6FFD6, 0xFFFFD6D6, 0xFFFFD6FF, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF0000FF, 0xFF007BFF, 0xFF00FFFF, 0xFF00FF84, 0xFF00FF00, 0xFF7B8400, 0xFFFF0000, 0xFFFF007B, 0xFFFF94FF, 0xFF00B5D6, 0xFF0010BD, 0xFF00105A, 0xFF6B6B6B, 0xFF000000, 0xFFFFFFFF,
	0xFF639410, 0xFF527B08, 0xFF398C10, 0xFF319C31, 0xFF4AA5CE, 0xFF3994CE, 0xFF4A8CBD, 0xFF318CD6, 0xFF4A73AD, 0xFF315A8C, 0xFF29426B, 0xFFFFEF84, 0xFFEFCE31, 0xFFC6A500, 0xFFFFFFFF,
	0xFFE7DED6, 0xFFDECEB5, 0xFFEFEFE7, 0xFFF7F7F7, 0xFF7B7384, 0xFF6B8C94, 0xFF637B84, 0xFF5A849C, 0xFFB59C73, 0xFF2929FF, 0xFF00FFFF, 0xFFFF2194, 0xFFBD9C00, 0xFF000000, 0xFFFFFFFF,
	0xFFFFFFFF, 0xFFEFEFF7, 0xFFDEDEE7, 0xFFCECED6, 0xFFB5B5C6, 0xFFA5A5B5, 0xFF9494A5, 0xFF84849C, 0xFF6B6B8C, 0xFF5A5A7B, 0xFF4A4A6B, 0xFF31315A, 0xFF21214A, 0xFF101042, 0xFF000031,
	0xFFFFFFFF, 0xFFEFEFEF, 0xFFDEDEDE, 0xFFCECECE, 0xFFB5B5B5, 0xFFA5A5A5, 0xFF949494, 0xFF848484, 0xFF6B6B6B, 0xFF5A5A5A, 0xFF4A4A4A, 0xFF313131, 0xFF212121, 0xFF101010, 0xFF000000,
	0xFF7B8CFF, 0xFF0000FF, 0xFF007BFF, 0xFF00FFFF, 0xFF008400, 0xFF00FF00, 0xFFFF0000, 0xFFFF9C00, 0xFFFF00D6, 0xFFFF6BFF, 0xFF00009C, 0xFF0094FF, 0xFF94BDFF, 0xFF000000, 0xFFFFFFFF
};

static const u32 NLPaletteColors[] = {
	0xFFFFEEFF, 0xFFAA99FF, 0xFF9955EE, 0xFFAA66FF, 0xFF6600FF, 0xFF7744BB, 0xFF5500CC, 0xFF330099, 0xFF332255, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF,
	0xFFCCBBFF, 0xFF7777FF, 0xFF1133DD, 0xFF4455FF, 0xFF0000FF, 0xFF6666CC, 0xFF4444BB, 0xFF0000BB, 0xFF222288, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFEEEEEE,
	0xFFBBCCDD, 0xFF66CCFF, 0xFF2266DD, 0xFF22AAFF, 0xFF0066FF, 0xFF5588BB, 0xFF0044DD, 0xFF0044BB, 0xFF113366, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFDDDDDD,
	0xFFDDEEFF, 0xFFCCDDFF, 0xFFAACCFF, 0xFF88BBFF, 0xFF88AAFF, 0xFF6688DD, 0xFF4466BB, 0xFF335599, 0xFF224488, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFCCCCCC,
	0xFFFFCCFF, 0xFFFF88EE, 0xFFDD66CC, 0xFFCC88BB, 0xFFFF00CC, 0xFF996699, 0xFFAA0088, 0xFF770055, 0xFF440033, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFBBBBBB,
	0xFFFFBBFF, 0xFFFF99FF, 0xFFBB22DD, 0xFFEE55FF, 0xFFCC00FF, 0xFF775588, 0xFF9900BB, 0xFF660088, 0xFF440055, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFAAAAAA,
	0xFF99BBDD, 0xFF77AACC, 0xFF334477, 0xFF4477AA, 0xFF003399, 0xFF223377, 0xFF002255, 0xFF001133, 0xFF001122, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF999999,
	0xFFCCFFFF, 0xFF77FFFF, 0xFF22DDDD, 0xFF00FFFF, 0xFF00DDFF, 0xFF00AACC, 0xFF009999, 0xFF007788, 0xFF005555, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF888888,
	0xFFFFBBDD, 0xFFEE99BB, 0xFFCC3366, 0xFFFF5599, 0xFFFF0066, 0xFF884455, 0xFF990044, 0xFF660022, 0xFF331122, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF777777,
	0xFFFFBBBB, 0xFFFF9988, 0xFFAA3333, 0xFFEE5533, 0xFFFF0000, 0xFF883333, 0xFFAA0000, 0xFF661111, 0xFF220000, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF666666,
	0xFFBBEE99, 0xFF77CC66, 0xFF116622, 0xFF33AA44, 0xFF338800, 0xFF557755, 0xFF005522, 0xFF223311, 0xFF112200, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF555555,
	0xFFBBFFDD, 0xFF88FFCC, 0xFF55AA88, 0xFF88DDAA, 0xFF00FF88, 0xFF99BBAA, 0xFF00BB66, 0xFF009955, 0xFF006633, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF444444,
	0xFFFFDDBB, 0xFFFFCC77, 0xFF995533, 0xFFFF9966, 0xFFFF7711, 0xFFAA7744, 0xFF774422, 0xFF772200, 0xFF441100, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF333333,
	0xFFFFFFAA, 0xFFFFFF55, 0xFFBB8800, 0xFFCCBB55, 0xFFFFCC00, 0xFFAA9944, 0xFF886600, 0xFF554400, 0xFF332200, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF222222,
	0xFFEEFFCC, 0xFFDDEEAA, 0xFFAACC33, 0xFFBBEE55, 0xFFCCFF00, 0xFFAAAA77, 0xFF99AA00, 0xFF778800, 0xFF334400, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFF000000,
	0xFFAAFFAA, 0xFF77FF77, 0xFF44DD66, 0xFF00FF00, 0xFF22DD22, 0xFF55BB55, 0xFF00BB00, 0xFF008800, 0xFF224422, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF
};

/* Return the save name here. */
const std::string RawPatternEditor::getSaveName() const {
	switch(this->savetype) {
		case SaveType::WW:
			return "Wild World";
		case SaveType::NL:
		case SaveType::WA:
			return "New Leaf | Welcome Amiibo";
		case SaveType::UNUSED:
			return "?";
	}

	return "?";
}

/* Return the region name here. */
const std::string RawPatternEditor::getRegionName() const {
	switch(this->saveregion) {
		case WWRegion::JPN_REV0:
		case WWRegion::JPN_REV1:
			return "Japanese";
		case WWRegion::USA_REV0:
		case WWRegion::USA_REV1:
		case WWRegion::EUR_REV1:
			return "Europe | USA";
		case WWRegion::KOR_REV1:
			return "Korean";
		case WWRegion::UNKNOWN:
			return "?";
	}

	return "?";
}

/* Automatically detect Pattern type here. */
void RawPatternEditor::getPattern(const std::string ptrnFile) {
	// Reset to invalid.
	this->saveregion = WWRegion::UNKNOWN;
	this->savetype = SaveType::UNUSED;
	this->patternSize = 0;
	this->isValid = false;

	FILE *file = fopen(ptrnFile.c_str(), "rb");
	if (file) {
		fseek(file, 0, SEEK_END);
		this->patternSize = ftell(file);
		fseek(file, 0, SEEK_SET);
		this->data = std::shared_ptr<u8[]>(new u8[this->patternSize]);
		fread(this->data.get(), 1, this->patternSize, file);
		fclose(file);

		/* Detect format here. */
		switch(this->patternSize) {
			case 0x220:
				this->saveregion = WWRegion::JPN_REV0;
				this->savetype = SaveType::WW;
				this->isValid = true;
				break;

			case 0x228:
				this->saveregion = WWRegion::EUR_REV1; // Is also USA.
				this->savetype = SaveType::WW;
				this->isValid = true;
				break;

			case 0x234:
				this->saveregion = WWRegion::KOR_REV1;
				this->savetype = SaveType::WW;
				this->isValid = true;
				break;

			case 0x26C:
			case 0x870:
				this->saveregion = WWRegion::UNKNOWN;
				this->savetype = SaveType::NL; // Is also AC:WA format, but they're the same.
				this->isValid = true;
				break;
		}
	}
}

/* Load the C2D_Image of the pattern data and such. */
void RawPatternEditor::load(const std::string ptrnFile, bool fromFile) {
	if (fromFile) this->getPattern(ptrnFile);

	if (this->isValid && this->data != nullptr) {
		switch(this->savetype) {
			case SaveType::NL:
				this->pattern = std::make_shared<PatternNL>(this->data, 0);
				break;
			case SaveType::WA:
				this->pattern = std::make_shared<PatternWA>(this->data, 0);
				break;
			case SaveType::WW:
				this->pattern = std::make_shared<PatternWW>(this->data, 0, this->saveregion);
				break;
			case SaveType::UNUSED:
				break;
		}

		C3D_FrameEnd(0);
		this->image = this->pattern->image(0);
		this->patternImage = CoreUtils::patternImage(this->image, this->savetype);
	}
}

/* Load Empty Pattern. */
RawPatternEditor::RawPatternEditor() {
	this->savetype = SaveType::NL;
	this->saveregion = WWRegion::EUR_REV1;
	CoreUtils::generateEmptyPattern(this->savetype, this->saveregion, this->data);
	this->isValid = true;
	this->load("", false);
}

/* Destroy C2D_Image, if exist. */
RawPatternEditor::~RawPatternEditor() {
	if (this->isValid) {
		if (this->patternImage.subtex != nullptr) C2DUtils::C2D_ImageDelete(this->patternImage);
	}
}

void RawPatternEditor::Draw(void) const {
	GFX::DrawTop();
	if (this->isValid) {
		Gui::DrawStringCentered(0, -2 + barOffset, 0.9, WHITE, "LeafEdit - " + Lang::get("PATTERN_EDITOR_RAW"), 395, 0, font);
		Gui::DrawStringCentered(0, 40, 0.7f, BLACK, Lang::get("PATTERN_NAME") + ": " + StringUtils::UTF16toUTF8(this->pattern->name()), 395, 0, font);
		Gui::DrawStringCentered(0, 60, 0.7f, BLACK, Lang::get("PATTERN_CREATOR_NAME") + ": " +  StringUtils::UTF16toUTF8(this->pattern->creatorname()), 395, 0, font);
		Gui::DrawStringCentered(0, 80, 0.7f, BLACK, Lang::get("PATTERN_CREATOR_ID") + ": " + std::to_string(pattern->creatorid()), 395, 0, font);
		Gui::DrawStringCentered(0, 100, 0.7f, BLACK, Lang::get("PATTERN_ORIGIN_NAME") + ": " + StringUtils::UTF16toUTF8(this->pattern->origtownname()), 395, 0, font);
		Gui::DrawStringCentered(0, 120, 0.7f, BLACK, Lang::get("PATTERN_ORIGIN_ID") + ": " + std::to_string(this->pattern->origtownid()), 395, 0, font);
		
		if (this->pattern->creatorGender()) {
			Gui::DrawStringCentered(0, 140, 0.7f, BLACK, Lang::get("GENDER") + ": " + Lang::get("FEMALE"), 395, 0, font);
		} else {
			Gui::DrawStringCentered(0, 140, 0.7f, BLACK, Lang::get("GENDER") + ": " + Lang::get("MALE"), 395, 0, font);
		}

		/* Display Savetype. */
		Gui::DrawStringCentered(0, 160, 0.7f, BLACK, Lang::get("SAVETYPE") + ": " + this->getSaveName(), 395, 0, font);

		/* Display Region, if on AC:WW. */
		if (this->savetype == SaveType::WW) {
			Gui::DrawStringCentered(0, 180, 0.7f, BLACK, Lang::get("REGION") + ": " + this->getRegionName(), 395, 0, font);
		}

		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
		GFX::DrawBottom(true);
		if (this->patternImage.subtex) C2D_DrawImageAt(this->patternImage, 8, 8, 0.5f, nullptr, 7, 7); // 224x224. 224/32 -> 7.

		/* Drawing Palette. */
		if (this->savetype == SaveType::WW) {
			for (int i = 0; i < 15; i++) {
				if (i == this->color) {
					GFX::drawGrid(palettePos[i].x, palettePos[i].y, palettePos[i].w, palettePos[i].h, WWPaletteColors[this->image->getPaletteColor(i)], C2D_Color32(160, 0, 0, 255));
				} else {
					GFX::drawGrid(palettePos[i].x, palettePos[i].y, palettePos[i].w, palettePos[i].h, WWPaletteColors[this->image->getPaletteColor(i)], C2D_Color32(20, 20, 20, 255));
				}
			}
		} else if (this->savetype == SaveType::NL || this->savetype == SaveType::WA) {
			for (int i = 0; i < 15; i++) {
				if (i == this->color) {
					GFX::drawGrid(palettePos[i].x, palettePos[i].y, palettePos[i].w, palettePos[i].h, NLPaletteColors[this->image->getPaletteColor(i)], C2D_Color32(160, 0, 0, 255));
				} else {
					GFX::drawGrid(palettePos[i].x, palettePos[i].y, palettePos[i].w, palettePos[i].h, NLPaletteColors[this->image->getPaletteColor(i)], C2D_Color32(20, 20, 20, 255));
				}
			}
		}

		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

		/* Invalid!! */
	} else {
		Gui::DrawStringCentered(0, -2 + barOffset, 0.9, WHITE, "LeafEdit - " + Lang::get("INVALID_PATTERN"), 395, 0, font);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
		GFX::DrawBottom(true);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	}
}

void RawPatternEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack(true);
	}

	/* Open the Pattern Palette Selection. */
	if (hDown & KEY_Y) {
		Overlays::PaletteTool(this->image, this->patternImage, this->savetype);
	}

	/* Pattern drawing part. */
	if (this->isValid) {
		if (hHeld & KEY_TOUCH) {
			bool didTouch = false;
			for (int x = 0; x < 32; x++) {
				for (int y = 0; y < 32; y++) {
					if (touch.px <= (8 + 7 + x * 7) && touch.px >= (8 + x * 7) && touch.py <= (8 + 7 + y * 7) && touch.py >= (8 + y * 7)) {
						if (this->savetype == SaveType::WW) this->image->setPixel(x, y, this->color + 1);
						else if (this->savetype == SaveType::NL || this->savetype == SaveType::WA) this->image->setPixel(x, y, this->color);
						didTouch = true;
						break;
					}
				}
			}

			/* If we didn't touched the Pattern. */
			if (!didTouch) {
				for (int i = 0; i < 15; i++) {
					if (iconTouch(touch, palettePos[i])) {
						this->color = i;
					}
				}
			}

			if (didTouch) {
				C3D_FrameEnd(0);
				if (this->patternImage.subtex != nullptr) C2DUtils::C2D_ImageDelete(this->patternImage);
				this->patternImage = CoreUtils::patternImage(this->image, this->savetype);
			}
		}
	}
}