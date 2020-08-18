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
		Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - " + Lang::get("PATTERN_EDITOR_RAW"), 395, 0, font);
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

		/* TODO: Grid. */
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));

		/* Invalid!! */
	} else {
		Gui::DrawStringCentered(0, -2, 0.8f, C2D_Color32(255, 255, 255, 255), "LeafEdit - " + Lang::get("INVALID_PATTERN"), 395, 0);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
		GFX::DrawBottom(true);
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	}
}

void RawPatternEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack(true);
	}
}