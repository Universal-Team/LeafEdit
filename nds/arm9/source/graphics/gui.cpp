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
#include "input.hpp"

#include <stack>

std::stack<std::unique_ptr<Screen>> screens;
bool toggle = false; // Toggle Pointer sprite.
bool selected = true; // Set this to true to update the Pointer OAM.

void Gui::togglePointer() {
	if (!toggle) {
		toggle = true;
		setSpriteVisibility(Gui::pointerID, false, false);
	} else {
		toggle = false;
		setSpriteVisibility(Gui::pointerID, false, true);
	}
	updateOam();
}

void Gui::hidePointer() {
	setSpriteVisibility(Gui::pointerID, false, false);
	updateOam();
}

void Gui::showPointer() {
	setSpriteVisibility(Gui::pointerID, false, true);
	updateOam();
}

void Gui::updatePointer(int x, int y) {
	if (selected) {
		setSpritePosition(Gui::pointerID, false, x, y);
		updateOam();
		selected = false;
	}
}

void Gui::DrawScreen() {
	clearScreen(false, true);
	clearScreen(true, true);
	screens.top()->Draw();
}

void Gui::mainLoop(u16 hDown, touchPosition touch) {
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
	drawRectangle(0, 20, 256, 152, GREEN, true, false);
	drawRectangle(0, 0, 256, 20, DARK_GREEN, true, false);
	drawRectangle(0, 172, 256, 20, DARK_GREEN, true, false);
}

void Gui::DrawBottom(void)
{
	drawRectangle(0, 20, 256, 152, GREEN, false, false);
	drawRectangle(0, 0, 256, 20, DARK_GREEN, false, false);
	drawRectangle(0, 172, 256, 20, DARK_GREEN, false, false);
}

void Gui::clearScreen(bool top, bool layer) {
	drawRectangle(0, 0, 256, 192, CLEAR, top, layer);
}

// Sprites stuff
int Gui::keyboardSpriteID, Gui::pointerID;

// Initialize Sprite.
void Gui::initSprites(void) {
	keyboardSpriteID = initSprite(false, SpriteSize_32x32);
	prepareSprite(keyboardSpriteID, false, 0, 0, 0);

	pointerID = initSprite(false, SpriteSize_32x32);
	prepareSprite(pointerID, false, 0, 0, 0);
	setSpriteVisibility(pointerID, false, false);
}

// Load sprites.
void Gui::loadSprites(void) {
	Image pointerImg = loadImage("/graphics/pointer.gfx");
	fillSpriteImage(pointerID, false, 32, 0, 0, pointerImg);
}

static void drawList(int screenPos, bool background, const std::vector<std::string> &list) {
	if (background) {
		// Clear screen.
		drawRectangle(0, 0, 256, 192, DARKERER_GRAY, DARKER_GRAY, false, false);
	}
	// Clear text.
	drawRectangle(0, 0, 256, 192, CLEAR, false, true);
	// Print list.
	for (unsigned i=0;i<std::min(9u, list.size()-screenPos);i++) {
		printText(list[screenPos+i], 4, 4+(i*20), false, true);
	}
}

int Gui::selectList(int current, const std::vector<std::string> &list) {
	// Set pointer position
	setSpriteVisibility(Gui::pointerID, false, true);
	setSpritePosition(pointerID, false, 4+getTextWidth(list[current]), -2);
	updateOam();

	drawList(current, true, list);

	int held, pressed, screenPos = current, newSelection = current, entriesPerScreen = 9;
	while(1) {
		do {
			swiWaitForVBlank();
			scanKeys();
			pressed = keysDown();
			held = keysDownRepeat();
		} while(!held);

		if (held & KEY_UP) {
			if (newSelection > 0)	newSelection--;
			else	newSelection = list.size()-1;
		} else if (held & KEY_DOWN) {
			if (newSelection < (int)list.size()-1)	newSelection++;
			else newSelection = 0;
		} else if (held & KEY_LEFT) {
			newSelection -= entriesPerScreen;
			if (newSelection < 0)	newSelection = 0;
		} else if (held & KEY_RIGHT) {
			newSelection += entriesPerScreen;
			if (newSelection > (int)list.size()-1)	newSelection = list.size()-1;
		} else if (pressed & KEY_A) {
			for (unsigned int i=0;i<list.size();i++) {
				hidePointer();
				return newSelection;
			}

		} if (pressed & KEY_B) {
			hidePointer();
			return current;
		}

		// Scroll screen if needed.
		if (newSelection < screenPos) {
			screenPos = newSelection;
			drawList(screenPos, false, list);
		} else if (newSelection > screenPos + entriesPerScreen - 1) {
			screenPos = newSelection - entriesPerScreen + 1;
			drawList(screenPos, false, list);
		}

		// Move pointer.
		setSpritePosition(pointerID, false, 3+getTextWidth(list[newSelection]), (20*(newSelection-screenPos)-2)+10);
		updateOam();
	}
}