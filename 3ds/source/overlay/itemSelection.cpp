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
#include "keyboard.hpp"
#include "overlay.hpp"
#include "spriteManagement.hpp"
#include "stringDB.hpp"

extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);
extern std::vector<std::tuple<u16, std::string, std::string>> itemDB;
extern std::vector<std::string> itemCategories;
extern touchPosition touch;

static std::vector<std::tuple<u16, std::string, std::string>> search(std::vector<std::string> searchCategory = {""}, std::string searchResult = "", bool compare = false) {
	return StringDB::searchTuple(searchResult, searchCategory, itemDB, compare);
}

static void Draw(int itemIndex, std::vector<std::tuple<u16, std::string, std::string>> itmList, bool showCat = true, std::string msg = "") {
	std::string itemList;
	Gui::clearTextBufs();
	C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
	C2D_TargetClear(Top, BLACK);
	C2D_TargetClear(Bottom, BLACK);
	GFX::DrawFileBrowseBG(true);
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, msg, 395, 0, font);

	for (int i = (itemIndex < 8) ? 0 : itemIndex - 8; i < (int)itmList.size() && i < ((itemIndex < 8) ? 9 : itemIndex + 1); i++) {
		showCat ? itemList += std::get<1>(itmList[i]) + " - " + std::get<2>(itmList[i]) + "\n" : itemList += std::get<1>(itmList[i]) + "\n";
	}

	for (uint i = 0; i < ((itmList.size() < 9) ? 9 - itmList.size() : 0); i++) {
		itemList += "\n";
	}

	/* Selector Logic. */
	if (itemIndex < 9) GFX::DrawSelector(true, 24 + (itemIndex * 21));
	else GFX::DrawSelector(true, 24 + (8 * 21));

	Gui::DrawString(5, 25, 0.85f, BLACK, itemList, 360, 0, font);
	Gui::DrawStringCentered(0, 217, 0.9f, WHITE, std::to_string(itemIndex + 1) + " | " + std::to_string(itmList.size()), 395, 0, font);

	GFX::DrawBottom();
	Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, 190));
	GFX::DrawGUI(gui_search_idx, 295, 3);
	C3D_FrameEnd(0);
}

static Structs::ButtonPos searchBtn = {295, 3, 20, 20};

/* Select an Item. */
u16 Overlays::SelectItem(u16 oldID, const SaveType st, const bool blockInv, std::string msg) {
	std::vector<std::tuple<u16, std::string, std::string>> itemList = itemDB;
	int itemIndex = ItemManager::getIndex(oldID);

	while(1) {
		Draw(itemIndex, itemList, true, msg);
		u32 hRepeat = hidKeysDownRepeat();
		hidScanInput();
		hidTouchRead(&touch);

		if (itemList.size() > 0) {
			if (hRepeat & KEY_DOWN) {
				if (itemIndex < (int)itemList.size()-1) itemIndex++;
				else itemIndex = 0;
			}

			if (hRepeat & KEY_UP) {
				if (itemIndex > 0) itemIndex--;
				else if (itemIndex == 0) itemIndex = (int)itemList.size()-1;
			}

			if ((hRepeat & KEY_LEFT) || (hRepeat & KEY_L)) {
				if ((itemIndex - 9) < 0) {
					itemIndex = 0;
				} else {
					itemIndex -= 9;
				}
			}

			if ((hRepeat & KEY_RIGHT) || (hRepeat & KEY_R)) {
				if ((itemIndex + 9) > (int)itemList.size()-1) {
					itemIndex = (int)itemList.size()-1;
				} else {
					itemIndex += 9;
				}
			}

			if (hidKeysDown() & KEY_A) {
				if (blockInv) {
					if (st == SaveType::NL || st == SaveType::WA) {
						if (ItemUtils::IsInvWhitelisted(std::get<0>(itemList[itemIndex]))) {
							return std::get<0>(itemList[itemIndex]);
						}
					}
				} else {
					return std::get<0>(itemList[itemIndex]);
				}
			}
		}

		if ((hidKeysDown() & KEY_X) || (hidKeysDown() & KEY_TOUCH && iconTouch(touch, searchBtn))) {
			itemIndex = 0;
			itemList.clear();
			int category = GFX::ListSelection(0, itemCategories, Lang::get("SELECT_ITEM_CATEGORY"));

			std::vector<std::string> categoryNames;
			categoryNames.push_back({itemCategories[category]});

			const std::string searchResult = Input::setString(-1, Lang::get("ENTER_SEARCH"));
			itemList = search(categoryNames, searchResult);
		}

		if (hidKeysDown() & KEY_Y) {
			itemList = itemDB;
		}

		if (hidKeysDown() & KEY_B) {
			return oldID;
		}
	}
}

/* Select an Item from a category. */
u16 Overlays::SelectItemCategory(u16 oldID, const SaveType st, const bool blockInv, std::vector<int> category, std::string msg) {
	std::vector<std::string> categoryNames;

	for (int i = 0; i < (int)category.size(); i++) {
		categoryNames.push_back({itemCategories[category[i]]});
	}

	std::vector<std::tuple<u16, std::string, std::string>> itemList = search(categoryNames, "", true);
	int itemIndex = 0;

	while(1) {
		Draw(itemIndex, itemList, false, msg);
		u32 hRepeat = hidKeysDownRepeat();
		hidScanInput();
		hidTouchRead(&touch);

		if (itemList.size() > 0) {
			if (hRepeat & KEY_DOWN) {
				if (itemIndex < (int)itemList.size()-1) itemIndex++;
				else itemIndex = 0;
			}

			if (hRepeat & KEY_UP) {
				if (itemIndex > 0) itemIndex--;
				else if (itemIndex == 0) itemIndex = (int)itemList.size()-1;
			}

			if ((hRepeat & KEY_LEFT) || (hRepeat & KEY_L)) {
				if ((itemIndex - 9) < 0) {
					itemIndex = 0;
				} else {
					itemIndex -= 9;
				}
			}

			if ((hRepeat & KEY_RIGHT) || (hRepeat & KEY_R)) {
				if ((itemIndex + 9) > (int)itemList.size()-1) {
					itemIndex = (int)itemList.size()-1;
				} else {
					itemIndex += 9;
				}
			}

			if (hidKeysDown() & KEY_A) {
				if (blockInv) {
					if (st == SaveType::NL || st == SaveType::WA) {
						if (ItemUtils::IsInvWhitelisted(std::get<0>(itemList[itemIndex]))) {
							return std::get<0>(itemList[itemIndex]);
						}
					}
				} else {
					return std::get<0>(itemList[itemIndex]);
				}
			}
		}

		if ((hidKeysDown() & KEY_X) || (hidKeysDown() & KEY_TOUCH && iconTouch(touch, searchBtn))) {
			itemIndex = 0;
			itemList.clear();
			const std::string searchResult = Input::setString(-1, Lang::get("ENTER_SEARCH"));
			itemList = search(categoryNames, searchResult, true);
		}

		if (hidKeysDown() & KEY_Y) {
			itemList = search(categoryNames, "", true);
		}

		if (hidKeysDown() & KEY_B) {
			return oldID;
		}
	}
}

/* Select an Item from a category with index for Wild World. */
u8 Overlays::SelectWWCategory(u8 oldIndex, std::vector<int> category, std::string msg) {
	std::vector<std::string> categoryNames;

	for (int i = 0; i < (int)category.size(); i++) {
		categoryNames.push_back({itemCategories[category[i]]});
	}

	std::vector<std::tuple<u16, std::string, std::string>> itemList = search(categoryNames, "", true);

	int itemIndex = 0;

	while(1) {
		Draw(itemIndex, itemList, false, msg);
		u32 hRepeat = hidKeysDownRepeat();
		hidScanInput();
		hidTouchRead(&touch);

		if (itemList.size() > 0) {
			if (hRepeat & KEY_DOWN) {
				if (itemIndex < (int)itemList.size() - 1) itemIndex++;
				else itemIndex = 0;
			}

			if (hRepeat & KEY_UP) {
				if (itemIndex > 0) itemIndex--;
				else if (itemIndex == 0) itemIndex = (int)itemList.size()-1;
			}

			if ((hRepeat & KEY_LEFT) || (hRepeat & KEY_L)) {
				if ((itemIndex - 9) < 0) {
					itemIndex = 0;
				} else {
					itemIndex -= 9;
				}
			}

			if ((hRepeat & KEY_RIGHT) || (hRepeat & KEY_R)) {
				if ((itemIndex + 9) > (int)itemList.size()-1) {
					itemIndex = (int)itemList.size()-1;
				} else {
					itemIndex += 9;
				}
			}

			if (hidKeysDown() & KEY_A) {
				return itemIndex;
			}
		}

		if ((hidKeysDown() & KEY_X) || (hidKeysDown() & KEY_TOUCH && iconTouch(touch, searchBtn))) {
			itemIndex = 0;
			itemList.clear();
			const std::string searchResult = Input::setString(-1, Lang::get("ENTER_SEARCH"));
			itemList = search(categoryNames, searchResult, true);
		}

		if (hidKeysDown() & KEY_Y) {
			itemList = search(categoryNames, "", true);
		}

		if (hidKeysDown() & KEY_B) {
			return oldIndex;
		}
	}
}