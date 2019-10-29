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

#include "common/utils.hpp"

#include "core/management/itemManagement.hpp"

#include "core/save/item.h"
#include "core/save/offsets.h"
#include "core/save/save.h"

#include "gui/gui.hpp"

#include "gui/screens/itemEditor.hpp"

#include <3ds.h>
#include <map>

extern Save* SaveFile;
extern std::map<u16, std::string> g_itemDatabase;
static std::vector<std::pair<std::string, s32>> inventoryData; // TODO: I dislike this. Find someother way of doing. Perhaps an item container class?
Item it;
extern int selectedPassedPlayer;

ItemEditor::ItemEditor() {
	it.LoadItemBins();
}

ItemEditor::~ItemEditor()
{
	it.UnloadItemBins();
}

void ItemEditor::DrawPocket(void) const {
	std::string title = "LeafEdit - ";
	title += Lang::get("ITEM_EDITOR");
	int x = 42;
	int y = 63;
	inventoryData = EditorUtils::load_player_invitems(selectedPassedPlayer);

	Gui::DrawTop();
	Gui::DrawString((400-Gui::GetStringWidth(0.8f, title.c_str()))/2, 2, 0.8f, Config::barText, title.c_str(), 400);

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
	DrawPocket();
}

void ItemEditor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
}