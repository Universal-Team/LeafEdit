/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2020 Universal-Team
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

#include "common.hpp"
#include "itemManager.hpp"
#include "itemUtils.hpp"
#include "overlay.hpp"
#include "spriteManagement.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);
extern std::vector<std::pair<u16, std::string>> itemDB;

static void Draw(int itemIndex) {
	std::string itemList;
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);
	GFX::DrawFileBrowseBG(true);
	Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, Lang::get("SELECT_ITEM"), 395, 0, font);

	for (int i = (itemIndex < 8) ? 0 : itemIndex - 8; i < (int)itemDB.size() && i < ((itemIndex < 8) ? 9 : itemIndex + 1); i++) {
		itemList += itemDB[i].second + "\n";
	}

	for (uint i = 0; i < ((itemDB.size() < 9) ? 9 - itemDB.size() : 0); i++) {
		itemList += "\n";
	}

	// Selector Logic.
	if (itemIndex < 9) GFX::DrawSelector(true, 24 + (itemIndex * 21));
	else GFX::DrawSelector(true, 24 + (8 * 21));
	Gui::DrawString(5, 25, 0.85f, BLACK, itemList, 360, 0, font);
	Gui::DrawStringCentered(0, 217, 0.9f, WHITE, std::to_string(itemIndex + 1) + " | " + std::to_string(itemDB.size()), 395, 0, font);

	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
	C3D_FrameEnd(0);
}

// Select an Item.
u16 Overlays::SelectItem(u16 oldID, const SaveType st, const bool blockInv) {
	int itemIndex = ItemManager::getIndex(oldID); int keyRepeatDelay = 0;

	while(1) {
		Draw(itemIndex);

		hidScanInput();
		if (keyRepeatDelay) keyRepeatDelay--;

		if (hidKeysHeld() & KEY_DOWN && !keyRepeatDelay) {
			if (itemIndex < (int)itemDB.size()-1) itemIndex++;
			else itemIndex = 0;
			keyRepeatDelay = 6;
		}

		if (hidKeysHeld() & KEY_UP && !keyRepeatDelay) {
			if (itemIndex > 0) itemIndex--;
			else if (itemIndex == 0) itemIndex = (int)itemDB.size()-1;
			keyRepeatDelay = 6;
		}

		if ((hidKeysHeld() & KEY_LEFT && !keyRepeatDelay) || (hidKeysHeld() & KEY_L && !keyRepeatDelay)) {
			if ((itemIndex - 9) < 0) {
				itemIndex = 0;
			} else {
				itemIndex -= 9;
			}
			
			keyRepeatDelay = 6;
		}

		if ((hidKeysHeld() & KEY_RIGHT && !keyRepeatDelay) || (hidKeysHeld() & KEY_R && !keyRepeatDelay)) {
			if ((itemIndex + 9) > (int)itemDB.size()-1) {
				itemIndex = (int)itemDB.size()-1;
			} else {
				itemIndex += 9;
			}
			
			keyRepeatDelay = 6;
		}

		if (hidKeysDown() & KEY_A) {
			if (blockInv && (st == SaveType::NL || st == SaveType::WA)) {
				if (ItemUtils::IsInvWhitelisted(itemDB[itemIndex].first)) {
					return itemDB[itemIndex].first;
				}
			} else {
				return itemDB[itemIndex].first;
			}
		}

		if (hidKeysDown() & KEY_B) {
			return oldID;
		}
	}
}