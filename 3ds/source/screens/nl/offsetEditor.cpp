	/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 DeadPhoenix8091, Epicpkmn11, Flame, RocketRobz, StackZ, TotallyNotGuy
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


#include "encryptedInt32.hpp"
#include "gui.hpp"
#include "keyboard.hpp"
#include "offsetEditor.hpp"
#include "save.hpp"
#include "screenCommon.hpp"
#include "utils.hpp"

#include <3ds.h>

extern Save* SaveFile;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

void OffsetEditor::Draw(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("OFFSET_EDITOR") + " " + (isReadMode ? Lang::get("READ_MODE") : Lang::get("WRITE_MODE")), 400);
	GFX::DrawNPC(npc_copper_idx, 20, 45);
	GFX::DrawNPC(npc_booker_idx, 200, 45);
	GFX::DrawBottom();

	for (int i = 0; i < 6; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, UNSELECTED_COLOR);
		if (Selection == i) {
			GFX::DrawSprite(sprites_pointer_idx, mainButtons[i].x+130, mainButtons[i].y+25);
		}
	}
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, "u8"))/2-80+17.5, 0.8, WHITE, "u8", 130, 25);
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, "u16"))/2-20+17.5, 0.8, WHITE, "u16", 130, 25);
	Gui::DrawStringCentered(-80, (240-Gui::GetStringHeight(0.8, "u32"))/2+75-17.5, 0.8, WHITE, "u32", 130, 25);
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, "u64"))/2-80+17.5, 0.8, WHITE, "u64", 130, 25);
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, "u16String"))/2-20+17.5, 0.8, WHITE, "u16String", 130, 25);
	Gui::DrawStringCentered(80, (240-Gui::GetStringHeight(0.8, "EncryptedInt32"))/2+75-17.5, 0.8, WHITE, "EncryptedInt32", 130, 25);
}

void OffsetEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (hDown & KEY_R) {
		if (isReadMode == true)	isReadMode = false;
	}
	if (hDown & KEY_L) {
		if (isReadMode == false)	isReadMode = true;
	}

	// Selection.
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	}
	if (hDown & KEY_DOWN) {
			if(Selection < 5)	Selection++;
	}
	if (hDown & KEY_RIGHT) {
		if (Selection < 3) {
			Selection += 3;
		}
	}
	if (hDown & KEY_LEFT) {
		if (Selection < 6 && Selection > 2) {
			Selection -= 3;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (isReadMode == false) {
			if (touching(touch, mainButtons[0])) {
				Editu8();
			} else if (touching(touch, mainButtons[1])) {
				Editu16();
			} else if (touching(touch, mainButtons[2])) {
				Editu32();
			} else if (touching(touch, mainButtons[3])) {
				Editu64();
			} else if (touching(touch, mainButtons[4])) {
				Editu16String();
			} else if (touching(touch, mainButtons[5])) {
				EditEncryptedInt32();
			}
		} else {
			if (touching(touch, mainButtons[0])) {
				Readu8();
			} else if (touching(touch, mainButtons[1])) {
				Readu16();
			} else if (touching(touch, mainButtons[2])) {
				Readu32();
			} else if (touching(touch, mainButtons[3])) {
				Readu64();
			} else if (touching(touch, mainButtons[4])) {
				Readu16String();
			} else if (touching(touch, mainButtons[5])) {
				ReadEncryptedInt32();
			}
		}
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK") + "\n" + Lang::get("LR_MODESWITCH"));
	}

	if (hDown & KEY_A) {
		if (isReadMode == false) {
			switch(Selection) {
				case 0:
					Editu8();
					break;
				case 1:
					Editu16();
					break;
				case 2:
					Editu32();
					break;
				case 3:
					Editu64();
					break;
				case 4:
					Editu16String();
					break;
				case 5:
					EditEncryptedInt32();
					break;
			}
		} else {
			switch(Selection) {
				case 0:
					Readu8();
					break;
				case 1:
					Readu16();
					break;
				case 2:
					Readu32();
					break;
				case 3:
					Readu64();
					break;
				case 4:
					Readu16String();
					break;
				case 5:
					ReadEncryptedInt32();
					break;
			}
		}
	}
}

u8 setu8()
{
	char message [100];
	snprintf(message, sizeof(message), Lang::get("ENTER_VALUE").c_str(), "u8");
	std::string input = Input::getHex(20, message);
	return std::stoi(input, 0, 16);
}

u16 setu16()
{
	char message [100];
	snprintf(message, sizeof(message), Lang::get("ENTER_VALUE").c_str(), "u16");
	std::string input = Input::getHex(20, message);
	return std::stoi(input, 0, 16);
}

u32 setu32(bool isOffset)
{
	if (isOffset) {
		std::string input = Input::getHex(20, Lang::get("ENTER_OFFSET"));
		return std::stoi(input, 0, 16);
	} else {
		char message [100];
		snprintf(message, sizeof(message), Lang::get("ENTER_VALUE").c_str(), "u32");
		std::string input = Input::getHex(20, message);
		return std::stoi(input, 0, 16);
	}
}

u64 setu64()
{
	char message [100];
	snprintf(message, sizeof(message), Lang::get("ENTER_VALUE").c_str(), "u64");
	std::string input = Input::getHex(20, message);
	return std::stoi(input, 0, 16);
}

// Edit operations.
void OffsetEditor::Editu8() {
	u32 offset = setu32(true);
	u8 value = setu8();
	SaveFile->Write(offset, value);
}

void OffsetEditor::Editu16() {
	u32 offset = setu32(true);
	u16 value = setu16();
	SaveFile->Write(offset, value);
}

void OffsetEditor::Editu32() {
	u32 offset = setu32(true);
	u32 value = setu32(false);
	SaveFile->Write(offset, value);
}

void OffsetEditor::Editu64() {
	u32 offset = setu32(true);
	u64 value = setu64();
	SaveFile->Write(offset, value);
}

void OffsetEditor::Editu16String() {
	u32 offset = setu32(true);
	std::u16string u16String = Input::handleu16String(40, Lang::get("ENTER_U16_STRING"), u"");
	u32 length = Input::getUint(20, Lang::get("ENTER_LENGTH"));
	SaveFile->Write(offset, u16String, length);
}

void OffsetEditor::EditEncryptedInt32() {
	u32 encryptedInt = 0;
	u32 encryptionData = 0;

	u64 offset = setu64();

	EncryptedInt32 EI32 = EncryptedInt32(Save::Instance()->ReadU64(offset));
	// New Value.
	char message [100];
	snprintf(message, sizeof(message), Lang::get("ENTER_VALUE").c_str(), "EncryptedInt32");
	EI32.value = Input::handleu32(20, message, 99999999, 0);

	// Writing.
	EI32.encrypt(encryptedInt, encryptionData);
	Save::Instance()->Write(offset, encryptedInt);
	Save::Instance()->Write(offset + 0x04, encryptionData);
}

// Read operations.
void OffsetEditor::Readu8() {
	u32 offset = setu32(true);
	u8 value = SaveFile->ReadU8(offset);
	char message [100];
	snprintf(message, sizeof(message), Lang::get("OUTPUT").c_str(), std::to_string(value).c_str());
	Msg::DisplayWaitMsg(message);
}

void OffsetEditor::Readu16() {
	u32 offset = setu32(true);
	u16 value = SaveFile->ReadU16(offset);
	char message [100];
	snprintf(message, sizeof(message), Lang::get("OUTPUT").c_str(), std::to_string(value).c_str());
	Msg::DisplayWaitMsg(message);
}

void OffsetEditor::Readu32() {
	u32 offset = setu32(true);
	u32 value = SaveFile->ReadU32(offset);
	char message [100];
	snprintf(message, sizeof(message), Lang::get("OUTPUT").c_str(), std::to_string(value).c_str());
	Msg::DisplayWaitMsg(message);
}

void OffsetEditor::Readu64() {
	u32 offset = setu32(true);
	u64 value = SaveFile->ReadU64(offset);
	char message [100];
	snprintf(message, sizeof(message), Lang::get("OUTPUT").c_str(), std::to_string(value).c_str());
	Msg::DisplayWaitMsg(message);
}

void OffsetEditor::Readu16String() {
	u32 offset = setu32(true);
	u32 length = Input::getUint(20, Lang::get("ENTER_LENGTH"));
	std::u16string u16String = SaveFile->ReadString(offset, length);
	std::string newString = StringUtils::UTF16toUTF8(u16String);
	char message [100];
	snprintf(message, sizeof(message), Lang::get("OUTPUT").c_str(), newString.c_str());
	Msg::DisplayWaitMsg(message);
}

// Might be wrong?
void OffsetEditor::ReadEncryptedInt32() {
	u32 encryptedInt;
	u32 encryptionData;
	u64 offset = setu64();
	EncryptedInt32 EI32 = EncryptedInt32(Save::Instance()->ReadU64(offset));
	EI32.encrypt(encryptedInt, encryptionData);
	char message [100];
	snprintf(message, sizeof(message), Lang::get("OUTPUT").c_str(), std::to_string(encryptedInt).c_str());
	Msg::DisplayWaitMsg(message);
}