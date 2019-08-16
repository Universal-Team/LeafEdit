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

#include "gui/screens/screenCommon.hpp"
#include "gui/screens/mainMenu.hpp"
#include "gui/screens/titleSelection.hpp"
#include "gui/keyboard.hpp"
#include "gameLoader.hpp"
#include "townManagement.hpp"
#include "utils.hpp"

extern bool exiting;
extern int fadealpha;
extern bool fadein;


void TitleSelection::Draw(void) const
{
	Gui::ScreenDraw(top);

	// Draw the BG.
	Gui::Draw_Rect(0, 0, 400.0f, 240.0f, C2D_Color32(30, 130, 10, 255));

	// Draw the Top Bar.
	Gui::Draw_Rect(0, 0, 400.0f, 25.0f, C2D_Color32(30, 190, 10, 255));

    // Draw the Two Rectangles for the Gamecard and installed Titles.
	Gui::Draw_Rect(119, 30, 270.0f, 180.0f, C2D_Color32(30, 190, 10, 255));
	Gui::Draw_Rect(20, 30, 80.0f, 180.0f, C2D_Color32(30, 190, 10, 255));


	Gui::DrawString((400-Gui::Draw_GetStringWidth(0.72f,"Select a Title to start."))/2, 2, 0.72f, WHITE, "Select a Title to start.");
	
    // Draw the 3DS Gamecard.
    Gui::sprite(sprites_card_idx, 30, 93);
    // Draw the Available Titles on the Top Screen.
	TitleDraw();

    Gui::DrawString(24, 190, 0.6f, BLACK, "Cartridge");

    Gui::DrawString(190, 190, 0.6f, BLACK, "Installed Titles");

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, fadealpha)); // Fade in/out effect


	Gui::ScreenDraw(bottom);
	Gui::Draw_Rect(0, 0, 320, 30, C2D_Color32(30, 190, 10, 255));
	Gui::Draw_Rect(0, 30, 320, 180, C2D_Color32(30, 130, 10, 255));
	Gui::Draw_Rect(0, 210, 320, 30, C2D_Color32(30, 190, 10, 255));

    // Draw the current Selected Title on the Bottom Screen with informations.
    if (selectedTitle != -2)
    {
        Gui::Draw_Rect(243, 33, 52, 52, C2D_Color32(30, 130, 10, 255));
        DrawInformationBox();
        C2D_DrawImageAt(titleFromIndex(selectedTitle)->icon(), 245, 35, 1.0f);
        Gui::DrawString((320-Gui::Draw_GetStringWidth(0.6f, titleFromIndex(selectedTitle)->name()))/2, 100+5, 0.6f, WHITE, titleFromIndex(selectedTitle)->name());
        Gui::DrawString(100+10, 150+5, 0.6f, WHITE, "ID :");
        Gui::DrawString(100+40, 150+5, 0.6f, WHITE, StringUtils::format("%08X", titleFromIndex(selectedTitle)->lowId()));
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
            if (selectedTitle == (int)GameLoader::installedTitles.size() - 1 || selectedTitle == 3)
            {
                if (GameLoader::cardTitle)
                {
                    selectedTitle = -1;
                }
                else
                {
                    if (GameLoader::installedTitles.size() > 4 && selectedTitle > 3)
                    {
                        if (selectedTitle > 3)
                        {
                            selectedTitle = 4;
                        }
                        else if (selectedTitle == 3)
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
                selectedTitle = GameLoader::installedTitles.size() < 4 ? GameLoader::installedTitles.size() - 1 : 3;
            }
            else if (selectedTitle == 4)
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
                    selectedTitle = GameLoader::installedTitles.size() > 4 ? 3 : (int)GameLoader::installedTitles.size() - 1;
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
		Gui::setScreen(std::make_unique<MainMenu>());
	}
}


// Draw all the Titles. (Game Card and Installed Titles.)
void TitleSelection::TitleDraw(void) const
{
    // Gamecard.
    if (GameLoader::cardTitle != nullptr)
    {
        C2D_DrawImageAt(GameLoader::cardTitle->icon(), 35, 98, 1.0f);
        if (titleFromIndex(selectedTitle) == GameLoader::cardTitle)
        {
            Gui::drawSelector(34, 97);
        }
    }

    // Installed Titles.
    for (size_t i = 0; i < GameLoader::installedTitles.size(); i++)
    {
        int y = GameLoader::installedTitles.size() > 4 ? (i / 4) * 60 + 68 : 98;
        int x = 150 + (4 - (GameLoader::installedTitles.size() % 4 == 0 ? 4 : GameLoader::installedTitles.size() % 4)) * 30 + (i > 3 ? i - 4 : i) * 60;
        ;
        if (GameLoader::installedTitles.size() > 4 && i < 4)
        {
            x = 150 + (i > 3 ? i - 4 : i) * 60;
        }

        C2D_DrawImageAt(GameLoader::installedTitles[i]->icon(), x, y, 1.0f);
        if (titleFromIndex(selectedTitle) == GameLoader::installedTitles[i])
        {
            Gui::drawSelector(x - 1, y - 1);
        }
    }
}

void TitleSelection::DrawInformationBox(void) const
{
    Gui::Draw_Rect(0, 100, 320, 30, GRAY);

    Gui::Draw_Rect(100, 150, 140, 30, GRAY);
}