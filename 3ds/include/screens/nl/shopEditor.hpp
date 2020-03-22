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

#ifndef SHOPEDITOR_HPP
#define SHOPEDITOR_HPP

#include "common.hpp"
#include "itemContainer.hpp"
#include "structs.hpp"

#include <vector>

class ItemContainer;
class ShopEditor : public Screen
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	ShopEditor();
	~ShopEditor();
private:
	// Shop Containers.
	std::shared_ptr<ItemContainer> Retail[8];
	std::shared_ptr<ItemContainer> TT[24];
	std::shared_ptr<ItemContainer> Gracie[18];
	std::shared_ptr<ItemContainer> NooksHome[8];
	std::shared_ptr<ItemContainer> Leif[11];
	std::shared_ptr<ItemContainer> AbleSisterClothes[5];
	std::shared_ptr<ItemContainer> AbleSisterAccesories[7];
	std::shared_ptr<ItemContainer> Kicks[6];
	std::shared_ptr<ItemContainer> Redd[4];
	std::shared_ptr<ItemContainer> IslandShop[4];
	std::shared_ptr<ItemContainer> MuseumShop[3];
	std::shared_ptr<ItemContainer> LostFound[16];
	std::shared_ptr<ItemContainer> ClubLol[4];
	std::shared_ptr<ItemContainer> Harvey[2];

	int Selection = 0;
	int shopMode = 0;

	int turnipMode = 0; // 0 -> AM, 1 -> PM.
	void DrawSubMenu(void) const;
	void SubMenuLogic(u32 hDown, u32 hHeld, touchPosition touch);

	void DrawTurnipScreen(void) const;
	void TurnipLogic(u32 hDown, u32 hHeld, touchPosition touch);

	// Sub Pos.
	std::vector<Structs::ButtonPos> mainButtons = {
		{10, 40, 140, 35}, // Turnip Prices.
		{10, 100, 140, 35}, // ?.
		{10, 160, 140, 35}, // ?.
		{170, 40, 140, 35}, // ?.
		{170, 100, 140, 35}, // ?.
		{170, 160, 140, 35}, // ?.
	};
};

#endif