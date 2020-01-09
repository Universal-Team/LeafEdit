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

#include "common/utils.hpp"

#include "core/management/itemManagement.hpp"

#include "core/save/item.h"
#include "core/save/offsets.h"
#include "core/save/player.h"
#include "core/save/save.h"

#include "gui/gui.hpp"
#include "gui/keyboard.hpp"

#include "gui/screens/itemEditor.hpp"

#include <3ds.h>
#include <map>

extern Save* SaveFile;
extern std::map<u16, std::string> g_itemDatabase;
std::vector<std::pair<std::string, s32>> testItemData; // Test. xD
static std::vector<std::pair<std::string, s32>> inventoryData; // TODO: I dislike this. Find someother way of doing. Perhaps an item container class?
static std::vector<std::pair<std::string, s32>> dresserData;
Item it;
extern int selectedPassedPlayer;
#define maxDresser 17 // 17, because it starts with 0, so it would be basically 18.

// Only load item stuff, when accessing this screen and also unload by exit of that screen.
ItemEditor::ItemEditor() {
	it.LoadItemBins();
}

ItemEditor::~ItemEditor()
{
	it.UnloadItemBins();
}

void ItemEditor::DisplayDresser(void) const {
	dresserData = EditorUtils::load_player_dresseritems(selectedPassedPlayer, currentDresser);
	std::string itemName = dresserData[currentDresserItem].first;
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, "LeafEdit - " + Lang::get("ITEM_DRESSER") + std::to_string(currentDresser+1), 400);
	Gui::DrawStringCentered(0, 214, 0.8f, WHITE, itemName, 390);
	Gui::DrawBottom();
	for (int i = 0; i < 10; ++i) {
		for (u32 y = 0; y < 2; y++) {
			for (u32 x = 0; x < 5; x++, i++) {
				Gui::sprite(0, sprites_itemHole_idx, 25 + x * 58 - 16, 75 + y * 58 - 16);
				if (dresserData[i].second > -1)
				{
					ItemManagement::DrawItem(dresserData[i].second, 25 + x * 58, 75 + y * 58, 1 , 1);
				}
			}
		}
	}
}



// Was a Test, to display the first Item on the Pocket of Player 1. Will be used for other needed Item Tests.
void ItemEditor::DisplayItems(void) const {
	Gui::DrawTop();
	Item *items = &Save::Instance()->players[0]->Pockets[0];
	testItemData.push_back(std::make_pair(items->GetName(), items->GetSpritesheetID()));
	ItemManagement::DrawItem(testItemData[0].second, 160, 60, 1 , 1);
	std::string itemName = testItemData[0].first;
	Gui::DrawStringCentered(0, 160, 0.7f, WHITE, itemName, 390);
	Gui::DrawBottom();
}


void ItemEditor::DisplayPocket(void) const {
	int x = 42;
	int y = 63;
	inventoryData = EditorUtils::load_player_invitems(selectedPassedPlayer);
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.8f, WHITE, "LeafEdit - " + Lang::get("ITEM_POCKET"), 400);
	Gui::DrawBottom();
	for (int i = 0; i < 16; ++i)
	{
		if (i == 2)
		{
			x += 38 * 2;
		}
		if (i > 0 && (i == 4 || i % 10 == 0))
		{
			y += 38;
			x = 42;
		}
		Gui::sprite(0, sprites_itemHole_idx, x - 16, y - 16);
		if (inventoryData[i].second > -1)
		{
			ItemManagement::DrawItem(inventoryData[i].second, x, y, 1 , 1);
		}
		x += 38;
	}
}


void ItemEditor::Draw(void) const {
	if (itemMode == 0) {
		DisplayPocket();
	} else if (itemMode == 1) {
		DisplayDresser();
	}
}
void ItemEditor::PocketLogic(u32 hDown) {
	if (hDown & KEY_X) {
		itemMode = 1;
	}
}

void ItemEditor::DresserLogic(u32 hDown) {
	if (hDown & KEY_X) {
		itemMode = 0;
	}

	if (hDown & KEY_R) {
		if(currentDresser < maxDresser)	currentDresser++;
	} else if (hDown & KEY_L) {
		if (currentDresser > 0) currentDresser--;
	}

	if (hDown & KEY_RIGHT) {
		if(currentDresserItem < 9)	currentDresserItem++;
	} else if (hDown & KEY_LEFT) {
		if (currentDresserItem > 0) currentDresserItem--;
	}
}


void ItemEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}

	if (itemMode == 0) {
		PocketLogic(hDown);
	} else if (itemMode == 1) {
		DresserLogic(hDown);
	}
}