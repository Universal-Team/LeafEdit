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
std::string Input::setString(uint maxLength, std::string old, std::string Text) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	GFX::DrawTop(false);
	GFX::DrawBox((240-Gui::GetStringHeight(0.8f, Text, font))/2 - 24, Gui::GetStringHeight(0.8f, Text, font));
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8f, Text, font))/2, 0.8f, WHITE, Text, 385, 100, font);
	C3D_FrameEnd(0);

	SwkbdState state;
	swkbdInit(&state, SWKBD_TYPE_NORMAL, 2, maxLength);
	char temp[maxLength + 1] = {0};
	swkbdSetHintText(&state, Text.c_str());
	swkbdSetValidation(&state, SWKBD_NOTBLANK_NOTEMPTY, SWKBD_FILTER_PROFANITY, 0);
	SwkbdButton ret = swkbdInputText(&state, temp, sizeof(temp));
	temp[maxLength] = '\0';

	return (ret == SWKBD_BUTTON_CONFIRM ? temp : old);
}

int Input::setInt(int maxValue, std::string Text, int numAmount, int oldVal) {
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, C2D_Color32(0, 0, 0, 0));
	GFX::DrawTop(false);
	GFX::DrawBox((240-Gui::GetStringHeight(0.8f, Text, font))/2 - 24, Gui::GetStringHeight(0.8f, Text, font));
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8f, Text, font))/2, 0.8f, WHITE, Text, 385, 100, font);
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