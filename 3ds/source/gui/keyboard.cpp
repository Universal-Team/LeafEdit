#include "gfx.hpp"
#include "gui.hpp"
#include "keyboard.hpp"
#include "screenCommon.hpp"
#include "structs.hpp"
#include "stringUtils.hpp"

#include <string>
#include <stdio.h>
#include <ctype.h>

extern C3D_RenderTarget* Top;
extern C3D_RenderTarget* Bottom;

bool caps = false, enter = false;
int shift = 0;
extern C2D_Font font;

Structs::Key keysQWERTY[] = {
	{"1", 0, 0}, {"2", 25, 0}, {"3", 50, 0}, {"4", 75, 0}, {"5", 100, 0}, {"6", 125, 0}, {"7", 150, 0}, {"8", 175, 0}, {"9", 200, 0}, {"0", 225, 0}, {"-", 250, 0}, {"=", 275, 0},
	{"q", 12, 22}, {"w", 37, 22}, {"e", 62, 22}, {"r", 87, 22}, {"t", 112, 22}, {"y", 137, 22}, {"u", 162, 22}, {"i", 187, 22}, {"o", 212, 22}, {"p", 237, 22}, {"[", 262, 22}, {"]", 287, 22},
	{"a", 25, 45}, {"s", 50, 45}, {"d", 75, 45}, {"f", 100, 45}, {"g", 125, 45}, {"h", 150, 45}, {"j", 175, 45}, {"k", 200, 45}, {"l", 225, 45}, {";", 250, 45}, {"'", 275, 45},
	{"z", 35, 67}, {"x", 60, 67}, {"c", 85, 67}, {"v", 110, 67}, {"b", 135, 67}, {"n", 160, 67}, {"m", 185, 67}, {",", 210, 67}, {".", 235, 67}, {"/", 260, 67}, {"\\", 210, 90},
};
Structs::Key keysQWERTYShift[] = {
	{"!", 0, 0}, {"@", 25, 0}, {"#", 50, 0}, {"$", 75, 0}, {"%", 100, 0}, {"^", 125, 0}, {"&", 150, 0}, {"*", 175, 0}, {"(", 200, 0}, {")", 225, 0}, {"_", 250, 0}, {"+", 275, 0},
	{"Q", 12, 22}, {"W", 37, 22}, {"E", 62, 22}, {"R", 87, 22}, {"T", 112, 22}, {"Y", 137, 22}, {"U", 162, 22}, {"I", 187, 22}, {"O", 212, 22}, {"P", 237, 22}, {"{", 262, 22}, {"}", 287, 22},
	{"A", 25, 45}, {"S", 50, 45}, {"D", 75, 45}, {"F", 100, 45}, {"G", 125, 45}, {"H", 150, 45}, {"J", 175, 45}, {"K", 200, 45}, {"L", 225, 45}, {":", 250, 45}, {"\"", 275, 45},
	{"Z", 35, 67}, {"X", 60, 67}, {"C", 85, 67}, {"V", 110, 67}, {"B", 135, 67}, {"N", 160, 67}, {"M", 185, 67}, {"<,", 210, 67}, {">", 235, 67}, {"?", 260, 67}, {"\\", 210, 90},
};
Structs::Key modifierKeys[] = {
	{"\uE071", 300, 0, 20},	// Backspace
	{"\uE01D",   0, 45, 20},	// Caps Lock
	{"\uE056", 300, 45, 20},	// Enter
	{"\uE01B",   0, 67, 30},	// Left Shift
	{"\uE01B", 285, 67, 35},	// Right Shift
	{" ",     85, 90, 120},	// Space
};
Structs::Key NumpadStruct[] = {
	{"1", 10, 25},
	{"2", 90, 25},
	{"3", 170, 25},

	{"4", 10, 95},
	{"5", 90, 95},
	{"6", 170, 95},

	{"7", 10, 165},
	{"8", 90, 165},
	{"9", 170, 165},

	{"0", 250, 95},

	{"E", 250, 165},

	{"B", 250, 25},
};
Structs::ButtonPos Numbers [] = {
	{10, 25, 60, 50}, // 1
	{90, 25, 60, 50}, // 2
	{170, 25, 60, 50}, // 3

	{10, 95, 60, 50},
	{90, 95, 60, 50},
	{170, 95, 60, 50},

	{10, 165, 60, 50},
	{90, 165, 60, 50},
	{170, 165, 60, 50},

	{250, 95, 60, 50}, // 0.

	{250, 165, 60, 50}, // Enter.

	{250, 25, 60, 50}, // Backspace.
};


Structs::Key HexStruct[] = {
	// Num struct.
	{"1", 70, 50},
	{"2", 110, 50},
	{"3", 150, 50},
	{"4", 190, 50},
	{"5", 230, 50},

	{"6", 70, 90},
	{"7", 110, 90},
	{"8", 150, 90},
	{"9", 190, 90},
	{"0", 230, 90},

	// Letter.
	{"A", 10, 10},
	{"B", 10, 50},
	{"C", 10, 90},
	{"D", 10, 130},
	{"E", 10, 170},
	{"F", 10, 210},
	// Special cases.
	{"x", 110, 150},
	{"B", 150, 150},
	{"E", 190, 150},
};

Structs::ButtonPos Hex [] = {
	{70, 50, 30, 20}, // 1.
	{110, 50, 30, 20}, // 2.
	{150, 50, 30, 20}, // 3.
	{190, 50, 30, 20}, // 4.
	{230, 50, 30, 20}, // 5.

	{70, 90, 30, 20}, // 6.
	{110, 90, 30, 20}, // 7.
	{150, 90, 30, 20}, // 8.
	{190, 90, 30, 20}, // 9.
	{230, 90, 30, 20}, // 0.

	{10, 10, 30, 20}, // A.
	{10, 50, 30, 20}, // B.
	{10, 90, 30, 20}, // C.
	{10, 130, 30, 20}, // D.
	{10, 170, 30, 20}, // E.
	{10, 210, 30, 20}, // F.

	{110, 150, 30, 20}, // x.
	{150, 150, 30, 20}, // Backspace.
	{190, 150, 30, 20}, // Enter.
};

extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);

void Input::DrawHex() {
	for(uint i=0;i<(sizeof(HexStruct)/sizeof(HexStruct[0]));i++) {
		Gui::Draw_Rect(HexStruct[i].x, HexStruct[i].y, 30, 20, DARKER_COLOR);
		char c[2] = {HexStruct[i].character[0]};
		Gui::DrawString(HexStruct[i].x+12, HexStruct[i].y+4, 0.5f, WHITE, c, 50, 0, font);
	}
}

void Input::DrawNumpad() {
	for(uint i=0;i<(sizeof(NumpadStruct)/sizeof(NumpadStruct[0]));i++) {
		Gui::Draw_Rect(NumpadStruct[i].x, NumpadStruct[i].y, 60, 50, DARKER_COLOR);
		char c[2] = {NumpadStruct[i].character[0]};
		Gui::DrawString(NumpadStruct[i].x+25, NumpadStruct[i].y+15, 0.72f, WHITE, c, 50, 0, font);
	}
}

void Input::drawKeyboard() {
	for(uint i=0;i<(sizeof(keysQWERTY)/sizeof(keysQWERTY[0]));i++) {
		C2D_DrawRectSolid(keysQWERTY[i].x, keysQWERTY[i].y+103, 0.5f, 20, 20, DARKER_COLOR & C2D_Color32(255, 255, 255, 200));
		if (shift) {
			char c[2] = {caps ? (char)toupper(keysQWERTYShift[i].character[0]) : keysQWERTYShift[i].character[0]};
			Gui::DrawString(keysQWERTYShift[i].x+(10-(Gui::GetStringWidth(0.50, c)/2)), keysQWERTYShift[i].y+103+(10-(Gui::GetStringHeight(0.50, c)/2)), 0.50, WHITE, c, 0, 0, font);
		} else {
			char c[2] = {caps ? (char)toupper(keysQWERTY[i].character[0]) : keysQWERTY[i].character[0]};
			Gui::DrawString(keysQWERTY[i].x+(10-(Gui::GetStringWidth(0.50, c)/2)), keysQWERTY[i].y+103+(10-(Gui::GetStringHeight(0.50, c)/2)), 0.50, WHITE, c, 0, 0, font);
		}
	}

	for(uint i=0;i<(sizeof(modifierKeys)/sizeof(modifierKeys[0]));i++) {
		std::string enter = modifierKeys[2].character;
		std::string arrowUp = modifierKeys[3].character;
		std::string backSpace = modifierKeys[0].character;
		std::string caps = modifierKeys[1].character;

		C2D_DrawRectSolid(modifierKeys[i].x, modifierKeys[i].y+103, 0.5f, modifierKeys[i].w, 20, DARKER_COLOR & C2D_Color32(255, 255, 255, 200));
		Gui::DrawString(modifierKeys[2].x+5, modifierKeys[2].y+105, 0.50, WHITE, enter, 0, 0, font);
		Gui::DrawString(modifierKeys[3].x+7, modifierKeys[3].y+105, 0.45, WHITE, arrowUp, 0, 0, font);
		Gui::DrawString(modifierKeys[4].x+10, modifierKeys[4].y+105, 0.45, WHITE, arrowUp, 0, 0, font);
		Gui::DrawString(modifierKeys[0].x+5, modifierKeys[0].y+105, 0.45, WHITE, backSpace, 0, 0, font);
		Gui::DrawString(modifierKeys[1].x+5, modifierKeys[1].y+105, 0.45, WHITE, caps, 0, 0, font);
	}
}


std::string Input::getString(std::string Text) { return Input::getString(-1, Text); }
std::string Input::getStringLong(std::string Text) { return Input::getString(-1, Text, 0.7f); } // For Long text.

std::string Input::getString(uint maxLength, std::string Text, float inputTextSize) {
	int hDown;
	touchPosition touch;
	std::string string;
	int keyDownDelay = 10, cursorBlink = 20;
	caps = false, shift = 0, enter = false;
	while(1) {
		do {
			C3D_FrameEnd(0);
			Gui::clearTextBufs();
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			GFX::DrawTop();
			Gui::DrawString((400-Gui::GetStringWidth(0.8f, Text))/2, 2, 0.8f, WHITE, Text, 400, 0, font);
			GFX::DrawBottom();
			drawKeyboard();
			C2D_DrawRectSolid(0, 81, 0.5f, 320, 20, DARKER_COLOR & C2D_Color32(200, 200, 200, 200));
			Gui::DrawString(2, 82, inputTextSize, WHITE, (string+(cursorBlink-- > 0 ? "_" : "")).c_str(), 316, 0, font);
			if (cursorBlink < -20)	cursorBlink = 20;
			scanKeys();
			hDown = keysDown();
			if (keyDownDelay > 0) {
				keyDownDelay--;
			} else if(keyDownDelay == 0) {
				keyDownDelay--;
			}
		} while(!hDown);
		if (keyDownDelay > 0) {
		}
		keyDownDelay = 10;

		if (hDown & KEY_TOUCH) {
			touchRead(&touch);
			if (string.length() < maxLength) {
				// Check if a regular key was pressed
				for(uint i=0;i<(sizeof(keysQWERTY)/sizeof(keysQWERTY[0]));i++) {
					if ((touch.px > keysQWERTY[i].x-2 && touch.px < keysQWERTY[i].x+18) && (touch.py > keysQWERTY[i].y+(103)-2 && touch.py < keysQWERTY[i].y+18+(103))) {
						char c = (shift ? keysQWERTYShift[i] : keysQWERTY[i]).character[0];
						string += (shift || caps ? toupper(c) : c);
						shift = 0;
						break;
					}
				}
			}

			// Check if a modifier key was pressed
			for(uint i=0;i<(sizeof(modifierKeys)/sizeof(modifierKeys[0]));i++) {
				if ((touch.px > modifierKeys[i].x-2 && touch.px < modifierKeys[i].x+modifierKeys[i].w+2) && (touch.py > modifierKeys[i].y+(103)-2 && touch.py < modifierKeys[i].y+18+(103))) {
					if (modifierKeys[i].character == "\uE071") {
						string = string.substr(0, string.length()-1);
					} else if (modifierKeys[i].character == "\uE01D") {
						caps = !caps;
					} else if (modifierKeys[i].character == "\uE056") {
						enter = true;
					} else if(modifierKeys[i].character == "\uE01B") {
						if (shift)	shift = 0;
						else		shift = 1;
						if (shift) {
							keyDownDelay = -1;
						} else {
							keyDownDelay = 0;
						}
					} else if (modifierKeys[i].character == " ") {
						if (string.length() < maxLength) {
							shift = 0;
							string += modifierKeys[5].character[0];
						}
					}
					break;
				}
			}
		} else if (hDown & KEY_B) {
			string = string.substr(0, string.length()-1);
		}

		if (hDown & KEY_START || enter) {
			break;
		}
	}

	return string;
}

int Input::getUint(int max, std::string Text) {
	std::string s = Input::Numpad(3, Text);
	if (s == "" || (atoi(s.c_str()) == 0 && s[0] != '0')) return -1;
	int i = atoi(s.c_str());
	if (i > max)	return 255;
	return i;
}

std::string Input::Numpad(std::string Text) { return Input::Numpad(-1, Text); }

std::string Input::Numpad(uint maxLength, std::string Text) {
	int hDown;
	touchPosition touch;
	std::string string;
	int keyDownDelay = 10, cursorBlink = 20;
	enter = false;
	while(1) {
		do {
			C3D_FrameEnd(0);
			Gui::clearTextBufs();
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(Top, BLACK);
			C2D_TargetClear(Bottom, BLACK);
			GFX::DrawTop();
			Gui::DrawStringCentered(0, 2, 0.7f, WHITE, Text, 400, 0, font);
			Gui::DrawString(180, 217, 0.9, WHITE, (string+(cursorBlink-- > 0 ? "_" : "")).c_str(), 380, 0, font);
			if (cursorBlink < -20)	cursorBlink = 20;
			GFX::DrawBottom();
			DrawNumpad();
			scanKeys();
			hDown = keysDown();
			if (keyDownDelay > 0) {
				keyDownDelay--;
			} else if(keyDownDelay == 0) {
				keyDownDelay--;
			}
		} while(!hDown);
		if (keyDownDelay > 0) {
		}
		keyDownDelay = 10;

		if (hDown & KEY_TOUCH) {
			touchRead(&touch);
			if (string.length() < maxLength) {
				if (iconTouch(touch, Numbers[0])) {
					string += "1";
				} else if (iconTouch(touch, Numbers[1])) {
					string += "2";
				} else if (iconTouch(touch, Numbers[2])) {
					string += "3";
				} else if (iconTouch(touch, Numbers[3])) {
					string += "4";
				} else if (iconTouch(touch, Numbers[4])) {
					string += "5";
				} else if (iconTouch(touch, Numbers[5])) {
					string += "6";
				} else if (iconTouch(touch, Numbers[6])) {
					string += "7";
				} else if (iconTouch(touch, Numbers[7])) {
					string += "8";
				} else if (iconTouch(touch, Numbers[8])) {
					string += "9";
				} else if (iconTouch(touch, Numbers[9])) {
					string += "0";
				}
			}
		}

		if (hDown & KEY_B || iconTouch(touch, Numbers[11])) {
			string = string.substr(0, string.length()-1);
		}

		if (hDown & KEY_START || iconTouch(touch, Numbers[10]) || enter) {
			break;
		}
	}

	return string;
	enter = false;
}


std::string Input::getHex(int max, std::string Text) {
	int hDown;
	touchPosition touch;
	std::string string;
	int keyDownDelay = 10, cursorBlink = 20;
	enter = false;
	while(1) {
		do {
			C3D_FrameEnd(0);
			Gui::clearTextBufs();
			C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
			C2D_TargetClear(Top, BLACK);
			C2D_TargetClear(Bottom, BLACK);
			GFX::DrawTop();
			Gui::DrawStringCentered(0, 2, 0.9f, WHITE, Text, 400, 0, font);
			Gui::DrawString(180, 217, 0.9, WHITE, (string+(cursorBlink-- > 0 ? "_" : "")).c_str(), 380, 0, font);
			if (cursorBlink < -20)	cursorBlink = 20;
			Gui::ScreenDraw(Bottom);
			Gui::Draw_Rect(0, 0, 320, 240, LIGHT_COLOR);
			DrawHex();
			scanKeys();
			hDown = keysDown();
			if (keyDownDelay > 0) {
				keyDownDelay--;
			} else if (keyDownDelay == 0) {
				keyDownDelay--;
			}
		} while(!hDown);
		if (keyDownDelay > 0) {
		}
		keyDownDelay = 10;

		if (hDown & KEY_TOUCH) {
			touchRead(&touch);
			if ((int)string.length() < max) {
				if (iconTouch(touch, Hex[0])) {
					string += "1";
				} else if (iconTouch(touch, Hex[1])) {
					string += "2";
				} else if (iconTouch(touch, Hex[2])) {
					string += "3";
				} else if (iconTouch(touch, Hex[3])) {
					string += "4";
				} else if (iconTouch(touch, Hex[4])) {
					string += "5";
				} else if (iconTouch(touch, Hex[5])) {
					string += "6";
				} else if (iconTouch(touch, Hex[6])) {
					string += "7";
				} else if (iconTouch(touch, Hex[7])) {
					string += "8";
				} else if (iconTouch(touch, Hex[8])) {
					string += "9";
				} else if (iconTouch(touch, Hex[9])) {
					string += "0";
				} else if (iconTouch(touch, Hex[10])) {
					string += "A";
				} else if (iconTouch(touch, Hex[11])) {
					string += "B";
				} else if (iconTouch(touch, Hex[12])) {
					string += "C";
				} else if (iconTouch(touch, Hex[13])) {
					string += "D";
				} else if (iconTouch(touch, Hex[14])) {
					string += "E";
				} else if (iconTouch(touch, Hex[15])) {
					string += "F";
				} else if (iconTouch(touch, Hex[16])) {
					string += "x";
				}
			}
		}

		if (hDown & KEY_B || iconTouch(touch, Hex[17])) {
			string = string.substr(0, string.length()-1);
		}

		if (hDown & KEY_START || iconTouch(touch, Hex[18]) || enter) {
			break;
		}
	}

	return string;
	enter = false;
}

std::uint32_t Input::handleu32(uint maxLength, std::string Text, int maxNum, u32 oldValue) {
	std::string testString = Numpad(maxLength, Text);
	if (testString == "") {
		return oldValue;
	} else {
		u32 value = (u32)std::min(std::stoi(testString), maxNum);
		return value;
	}
}

std::uint16_t Input::handleu16(uint maxLength, std::string Text, int maxNum, u16 oldValue) {
	std::string testString = Numpad(maxLength, Text);
	if (testString == "") {
		return oldValue;
	} else {
		u16 value = (u16)std::min(std::stoi(testString), maxNum);
		return value;
	}
}

std::uint8_t Input::handleu8(uint maxLength, std::string Text, int maxNum, u8 oldValue) {
	std::string testString = Numpad(maxLength, Text);
	if (testString == "") {
		return oldValue;
	} else {
		u8 value = (u8)std::min(std::stoi(testString), maxNum);
		return value;
	}
}

std::u16string Input::handleu16String(uint maxLength, std::string Text, std::u16string oldString) {
	std::string testString = getString(maxLength, Text);
	if (testString == "") {
		return oldString;
	} else {
		std::u16string newString = StringUtils::UTF8toUTF16(testString);
		return newString;
	}
}

std::string Input::handleString(uint maxLength, std::string Text, std::string oldString) {
	std::string testString = getString(maxLength, Text);
	if (testString == "") {
		return oldString;
	} else {
		return testString;
	}
}