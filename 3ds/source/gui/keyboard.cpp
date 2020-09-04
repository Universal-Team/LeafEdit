#include "common.hpp"
#include "keyboard.hpp"
#include "screenCommon.hpp"
#include "structs.hpp"
#include "stringUtils.hpp"

#include <string>
#include <stdio.h>
#include <ctype.h>

extern C2D_Font font;

/* 3DS Native keyboard. */
std::string Input::setString(uint maxLength, std::string Text) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	GFX::DrawTop(true);
	Gui::DrawStringCentered(0, -2, 0.8f, C2D_Color32(255, 255, 255, 255), Text, 395, 0, font);
	C3D_FrameEnd(0);

	C3D_FrameEnd(0);
	SwkbdState state;
	swkbdInit(&state, SWKBD_TYPE_NORMAL, 2, maxLength);
	char temp[maxLength] = {0};
	swkbdSetHintText(&state, Text.c_str());
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, SWKBD_FILTER_PROFANITY, 0);
	SwkbdButton ret = swkbdInputText(&state, temp, sizeof(temp));
	temp[maxLength-1] = '\0';

	if (ret == SWKBD_BUTTON_CONFIRM) {
		return temp;
	}

	return "";
}

int Input::setInt(int maxValue, std::string Text, int numAmount, int oldVal) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	GFX::DrawTop(true);
	Gui::DrawStringCentered(0, -2, 0.8f, C2D_Color32(255, 255, 255, 255), Text, 395, 0, font);
	C3D_FrameEnd(0);

	SwkbdState state;
	swkbdInit(&state, SWKBD_TYPE_NUMPAD, 2, numAmount);
	swkbdSetFeatures(&state, SWKBD_FIXED_WIDTH);
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, 0, 0);
	char input[numAmount + 1]   = {0};
	SwkbdButton ret = swkbdInputText(&state, input, sizeof(input));
	input[numAmount] = '\0';

	return (ret == SWKBD_BUTTON_CONFIRM ? (int)std::min(std::stoi(input), maxValue) : oldVal);
}