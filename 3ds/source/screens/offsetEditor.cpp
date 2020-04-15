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
#include "keyboard.hpp"
#include "offsetEditor.hpp"
#include "Sav.hpp"
#include "saveUtils.hpp"
#include "screenCommon.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern std::shared_ptr<Sav> save;

// Read.
template <typename t>
t Read() {
	u32 offset;
	std::string input = Input::getHex(20, "Enter Offset.");
	if (input != "" || input != "0" || input != "0x") {
		offset = std::stoi(input, 0, 16);
	} else {
		offset = 0x0; // Offset was not entered properly.
	}
	return SaveUtils::Read<t>(save->savePointer(), offset);
}
// Write.
template <typename t>
void Write() {
	u32 offset;
	t value;
	std::string input = Input::getHex(20, "Enter Offset.");
	if (input != "" || input != "0" || input != "0x") {
		offset = std::stoi(input, 0, 16);
	} else {
		return; // No, do not write.
	}

	std::string input2 = Input::getHex(20, "Enter Hex Data.");
	if (input2 != "" || input2 != "0" || input2 != "0x") {
		value = std::stoi(input, 0, 16);
	} else {
		return; // No, do not write.
	}
	SaveUtils::Write<t>(save->savePointer(), offset, value);
}

// Show value.
template <typename t>
void show(const t value) {
	char message [100];
	snprintf(message, sizeof(message), Lang::get("OUTPUT").c_str(), std::to_string(value).c_str());
	Msg::DisplayWaitMsg(message);
}
// Show String.
void showString() {
	u32 offset;
	std::u16string output;
	std::string input = Input::getHex(20, "Enter Offset.");
	if (input != "" || input != "0" || input != "0x") {
		offset = std::stoi(input, 0, 16);
	} else {
		offset = 0x0; // Offset was not entered properly.
	}
	// String length.
	int length = Input::getUint(35, "Enter String Length.");

	if (save->getType() == SaveType::WW) {
		switch (save->getRegion()) {
			case WWRegion::USA_REV0:
			case WWRegion::USA_REV1:
			case WWRegion::EUR_REV1:
			case WWRegion::JPN_REV0:
			case WWRegion::JPN_REV1:
				output = StringUtils::ReadWWString(save->savePointer(), offset, length, save->getRegion());
				break;
			case WWRegion::KOR_REV1:
				output = StringUtils::ReadNLString(save->savePointer(), offset, length, u'\uFFFF');
				break;
			case WWRegion::UNKNOWN:
				output = StringUtils::UTF8toUTF16("?");
				break;
		}
	} else if (save->getType() == SaveType::NL || save->getType() == SaveType::WA) {
		output = StringUtils::ReadNLString(save->savePointer(), offset, length, u'\uFFFF');
	}

	char message [100];
	snprintf(message, sizeof(message), Lang::get("OUTPUT").c_str(), std::string(StringUtils::UTF16toUTF8(output)).c_str());
	Msg::DisplayWaitMsg(message);
}

u32 ReadEncryptedInt32() {
	u32 offset;
	std::string input = Input::getHex(20, "Enter Offset.");
	if (input != "" || input != "0" || input != "0x") {
		offset = std::stoi(input, 0, 16);
	} else {
		offset = 0x0; // Offset was not entered properly.
	}

	EncryptedInt32 EI32 = EncryptedInt32(SaveUtils::Read<u64>(save->savePointer(), offset));
	return EI32.value;
}

void EditEncryptedInt32() {
	u32 encryptedInt = 0;
	u32 encryptionData = 0;
	u32 offset;
	u32 value;
	std::string input = Input::getHex(20, "Enter Offset.");
	if (input != "" || input != "0" || input != "0x") {
		offset = std::stoi(input, 0, 16);
	} else {
		return; // Do not Write.
	}

	EncryptedInt32 EI32 = EncryptedInt32(SaveUtils::Read<u64>(save->savePointer(), offset));
	// New Value.
	std::string input2 = Input::getHex(20, "Enter Data.");
	if (input2 != "" || input2 != "0" || input2 != "0x") {
		value = std::stoi(input2, 0, 16);
	} else {
		return; // Do not Write.
	}
	EI32.value = value;

	// Writing.
	EI32.encrypt(encryptedInt, encryptionData);
	SaveUtils::Write<u32>(save->savePointer(), offset, encryptedInt);
	SaveUtils::Write<u32>(save->savePointer(), offset + 0x04, encryptionData);
}

void OffsetEditor::Draw(void) const
{
	std::string title = "LeafEdit - Offset Editor - ";
	if (Mode == 0)	title += "Read Mode";
	else			title += "Write Mode";
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9, WHITE, title, 390);
	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(mainButtons[i]);
		if (i == Selection)	GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x+100, mainButtons[i].y+30);
	}
}


void OffsetEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	// Navigation.
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

	if (hDown & KEY_A) {
		// Read.
		if (Mode == 0) {
			if (Selection == 0) {
				u32 offset;
				std::string input = Input::getHex(20, "Enter Offset.");
				if (input != "" || input != "0" || input != "0x") {
					offset = std::stoi(input, 0, 16);
				} else {
					offset = 0x0;
				}
				u8 test = save->savePointer()[offset];
				show(test);
			} else if (Selection == 1) {
				u16 value = Read<u16>();
				show(value);
			} else if (Selection == 2) {
				u32 value = Read<u32>();
				show(value);
			} else if (Selection == 3) {
				u64 value = Read<u64>();
				show(value);
			} else if (Selection == 4) {
				showString();
			}
		} else {
			// Write.
			if (Selection == 0) {
				u32 offset;
				u8 value;
				std::string input = Input::getHex(20, "Enter Offset.");
				if (input != "" || input != "0" || input != "0x") {
					offset = std::stoi(input, 0, 16);
				} else {
					return;
				}
				std::string input2 = Input::getHex(20, "Enter Data.");
				if (input2 != "" || input2 != "0" || input2 != "0x") {
					value = std::stoi(input, 0, 16);
				} else {
					return;
				}
				save->savePointer()[offset] = value;
			} else if (Selection == 1) {
				Write<u16>();
			} else if (Selection == 2) {
				Write<u32>();
			} else if (Selection == 3) {
				Write<u64>();
			}
		}
	}
}