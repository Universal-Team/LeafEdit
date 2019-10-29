	/*
*   This file is part of LeafEdit
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
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

#include "common/common.hpp"
#include "common/utils.hpp"

#include "core/gameLoader.hpp"

#include "gui/keyboard.hpp"

#include "gui/screens/mainMenu.hpp"
#include "gui/screens/screenCommon.hpp"
#include "gui/screens/settings.hpp"
#include "gui/screens/titleSelection.hpp"

extern bool exiting;
extern int fadealpha;
extern bool fadein;

u64 currentID;
u32 currentLowID;
u32 currentHighID;
u32 currentUniqueID;
FS_MediaType currentMedia;
extern bool touching(touchPosition touch, Structs::ButtonPos button);
bool titleIsLoaded = false;
bool isROMHack = false;

void TitleSelection::Draw(void) const {
	Gui::DrawTop();

	Gui::Draw_Rect(119, 30, 270.0f, 180.0f, Config::Color3);
	Gui::Draw_Rect(20, 30, 80.0f, 180.0f, Config::Color3);

	Gui::DrawString((400-Gui::GetStringWidth(0.72f, Lang::get("SELECT_TITLE")))/2, 2, 0.72f, greentext, Lang::get("SELECT_TITLE"), 400);

	// Draw the 3DS Gamecard.
	Gui::Draw_ImageBlend(0, sprites_card_idx, 30, 93, Config::Color2);
	// Draw the Available Titles on the Top Screen.
	TitleDraw();

	Gui::DrawString(((400-Gui::GetStringWidth(0.6f, Lang::get("CARD")))/2)-100-40, 180, 0.6f, BLACK, Lang::get("CARD"), 80);

	Gui::DrawString(((400-Gui::GetStringWidth(0.6f, Lang::get("INSTALLED")))/2)+189-135, 180, 0.6f, BLACK, Lang::get("INSTALLED"), 270);

	Gui::DrawString(395-Gui::GetStringWidth(FONT_SIZE_18, V_STRING), 216, FONT_SIZE_18, greentext, V_STRING, 400);

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, fadealpha)); // Fade in/out effect


	Gui::DrawBottom();

	Gui::Draw_Rect(0, 100, 320, 30, Config::Color2);
	Gui::Draw_Rect(0, 150, 320, 30, Config::Color2);

	Gui::DrawString((320-Gui::GetStringWidth(0.72f, "Press Y to open Settings."))/2, 2, 0.72f, greentext, "Press Y to open Settings.", 320);
	Gui::Draw_Rect(245, 38, 48, 48, Config::Color2);
	// Draw the current Selected Title on the Bottom Screen with informations.
	if (selectedTitle != -2)
	{
		C2D_DrawImageAt(titleFromIndex(selectedTitle)->icon(), 245, 38, 0.5f);
		displayRegion();
		Gui::DrawString((320-Gui::GetStringWidth(0.7f, titleFromIndex(selectedTitle)->name()))/2, 105, 0.7f, BLACK, titleFromIndex(selectedTitle)->name(), 400);
		std::string IDAndCode = "ID: ";
		IDAndCode += StringUtils::format("%08X", titleFromIndex(selectedTitle)->lowId());
		IDAndCode += ", Prod Code: ";
		IDAndCode += titleFromIndex(selectedTitle)->productCode();
		Gui::DrawString((320-Gui::GetStringWidth(0.6f, IDAndCode.c_str()))/2, 155, 0.6f, BLACK, IDAndCode.c_str(), 320);
	}
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, fadealpha)); // Fade in/out effect
}

void TitleSelection::SelectionLogic(u32 hDown) {
	if (GameLoader::cardUpdate())
	{
		GameSelected  = false;
		selectedTitle = -2;
	}

	if (selectedTitle == -2)
	{
		if (GameLoader::cardTitle == nullptr && !GameLoader::installedTitles.empty())
		{
			selectedTitle = 0;
		}
		else if (GameLoader::cardTitle != nullptr)
		{
			selectedTitle = -1;
		}
	}

	// Scroll with D-Pad Right to the next available Title.
	else if (hDown & KEY_RIGHT)
	{
		if (selectedTitle == (int)GameLoader::installedTitles.size() - 1 || selectedTitle == 7)
		{
			if (GameLoader::cardTitle)
			{
				selectedTitle = -1;
			}
			else
			{
				if (GameLoader::installedTitles.size() > 8 && selectedTitle > 7)
				{
					if (selectedTitle > 7)
					{
						selectedTitle = 8;
					}
					else if (selectedTitle == 7)
					{
						selectedTitle = 0;
					}
				}
				else
				{
					selectedTitle = 0;
				}
			}
		}
		else
		{
			selectedTitle++;
		}
	}

	// Scroll with D-Pad Left to the next available Title.
	else if (hDown & KEY_LEFT)
	{
		if (selectedTitle == -1)
		{
			selectedTitle = GameLoader::installedTitles.size() < 8 ? GameLoader::installedTitles.size() - 1 : 7;
		}
		else if (selectedTitle == 8)
		{
			if (GameLoader::cardTitle)
			{
				selectedTitle = -1;
			}
			else
			{
				selectedTitle = (int)GameLoader::installedTitles.size() - 1;
			}
		}
		else if (selectedTitle == 0)
		{
			if (GameLoader::cardTitle)
			{
				selectedTitle = -1;
			}
			else
			{
				selectedTitle = GameLoader::installedTitles.size() > 8 ? 7 : (int)GameLoader::installedTitles.size() - 1;
			}
		}
		else
		{
			selectedTitle--;
		}
	}
}

void TitleSelection::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	// For the D-Pad Selection.
	SelectionLogic(hDown);

	if (hDown & KEY_START) {
		exiting = true;
	} else if (hDown & KEY_A) {
		if (GameLoader::cardTitle == nullptr && GameLoader::installedTitles.empty()) {
		} else {
			if (titleFromIndex(selectedTitle)->ID() == WelcomeLuxury) {
				isROMHack = true;
			} else {
				isROMHack = false;
			}
			currentID = titleFromIndex(selectedTitle)->ID();
			currentMedia = titleFromIndex(selectedTitle)->mediaType();
			currentLowID = titleFromIndex(selectedTitle)->lowId();
			currentHighID = titleFromIndex(selectedTitle)->highId();
			currentUniqueID = titleFromIndex(selectedTitle)->uniqueId();
			titleIsLoaded = true;
			Gui::setScreen(std::make_unique<MainMenu>());
		}
	} else if (hDown & KEY_TOUCH && touching(touch, icon[0])) {
		if (GameLoader::cardTitle == nullptr && GameLoader::installedTitles.empty()) {
		} else {
			if (titleFromIndex(selectedTitle)->ID() == WelcomeLuxury) {
				isROMHack = true;
			} else {
				isROMHack = false;
			}
			currentID = titleFromIndex(selectedTitle)->ID();
			currentMedia = titleFromIndex(selectedTitle)->mediaType();
			currentLowID = titleFromIndex(selectedTitle)->lowId();
			currentHighID = titleFromIndex(selectedTitle)->highId();
			currentUniqueID = titleFromIndex(selectedTitle)->uniqueId();
			titleIsLoaded = true;
			Gui::setScreen(std::make_unique<MainMenu>());
		}
	} else if (hDown & KEY_X) {
		GameLoader::updateCheck2();
	} else if (hDown & KEY_Y) {
		titleIsLoaded = false;
		Gui::setScreen(std::make_unique<Settings>());
	} else if (hHeld & KEY_SELECT) {
		Msg::HelperBox("Select a Title to load. if you don't have an installed title -> Press Y.\nPress Start, to exit the App or press Home.\n(You can't use the Town Manager without a Title)\nYou can also tap on the Icon, if you have an installed Title or Gamecard.");
	}
}


// Draw all the Titles. (Game Card and Installed Titles.)
void TitleSelection::TitleDraw(void) const
{
	// Display Blank boxes for placeholders.
	u32 box = 8;
	for (u32 y = 0; y < 2; y++) {
		for (u32 x = 0; x < 4; x++, box++) {
			Gui::Draw_Rect(143 + x * 60, y * 55 + 68, 48, 48, Config::Color2);
		}
	}

	// Gamecard.
	if (GameLoader::cardTitle != nullptr)
	{
		C2D_DrawImageAt(GameLoader::cardTitle->icon(), 35, 98, 0.5f);
		if (titleFromIndex(selectedTitle) == GameLoader::cardTitle)
		{
			Gui::drawAnimatedSelector(34, 97, 50, 50, .025f, C2D_Color32(0, 0, 0, 0));
		}
	}

	// Installed Titles.
	for (size_t i = 0; i < GameLoader::installedTitles.size(); i++)
	{
		int y = GameLoader::installedTitles.size() > 3 ? (i / 4) * 55 + 68 : 68;
		int x = 143 + (5 - (GameLoader::installedTitles.size() % 4 == 0 ? 5 : GameLoader::installedTitles.size() % 4)) * 0 + (i > 3 ? i - 4 : i) * 60;
		;
		if (GameLoader::installedTitles.size() > 4 && i < 4)
		{
			x = 143 + (i > 3 ? i - 4 : i) * 60;
		}

		C2D_DrawImageAt(GameLoader::installedTitles[i]->icon(), x, y, 0.5f);
		if (titleFromIndex(selectedTitle) == GameLoader::installedTitles[i])
		{
			Gui::drawAnimatedSelector(x - 1, y - 1, 50, 50, .025f, C2D_Color32(0, 0, 0, 0));
		}
	}
}

void TitleSelection::displayRegion(void) const {
	if (titleFromIndex(selectedTitle)->ID() == OldEUR || titleFromIndex(selectedTitle)->ID() == WelcomeAmiiboEUR) {
		Gui::sprite(0, sprites_europe_idx, 50, 42, 1.5, 1.5);
	} else if (titleFromIndex(selectedTitle)->ID() == OldJPN || titleFromIndex(selectedTitle)->ID() == WelcomeAmiiboJPN) {
		Gui::sprite(0, sprites_japan_idx, 50, 42, 1.5, 1.5);
	} else if (titleFromIndex(selectedTitle)->ID() == OldKOR || titleFromIndex(selectedTitle)->ID() == WelcomeAmiiboKOR) {
		Gui::sprite(0, sprites_korea_idx, 50, 42, 1.5, 1.5);
	} else if (titleFromIndex(selectedTitle)->ID() == OldUSA || titleFromIndex(selectedTitle)->ID() == WelcomeAmiiboUSA) {
		Gui::sprite(0, sprites_usa_idx, 50, 42, 1.5, 1.5);
	}
}