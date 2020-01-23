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

#include "colors.hpp"
#include "gui.hpp"

#include <stack>

std::stack<std::unique_ptr<Screen>> screens;

void Gui::mainLoop(u16 hDown, touchPosition touch) {
	screens.top()->Draw();
	screens.top()->Logic(hDown, touch);
}

void Gui::setScreen(std::unique_ptr<Screen> screen)
{
	screens.push(std::move(screen));
}

void Gui::screenBack()
{
	screens.pop();
}

// Basic GUI Stuff.

void Gui::DrawTop(void)
{
	drawRectangle(0, 20, 256, 152, BLUE, true, false);
	drawRectangle(0, 0, 256, 20, DARK_BLUE, true, false);
	drawRectangle(0, 172, 256, 20, DARK_BLUE, true, false);
}

void Gui::DrawBottom(void)
{
	drawRectangle(0, 20, 256, 152, BLUE, false, false);
	drawRectangle(0, 0, 256, 20, DARK_BLUE, false, false);
	drawRectangle(0, 172, 256, 20, DARK_BLUE, false, false);
}

void Gui::clearScreen(bool top, bool layer) {
    drawRectangle(0, 0, 256, 192, CLEAR, top, layer);
}

// Sprites stuff

int Gui::keyboardSpriteID;

void Gui::initSprites(void) {
	keyboardSpriteID = initSprite(false, SpriteSize_32x32);
	prepareSprite(keyboardSpriteID, false, 0, 0, 0);
}
