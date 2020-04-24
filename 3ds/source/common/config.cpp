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

#include "config.hpp"

	u32 Config::Pattern, Config::Building, Config::MoneyRock, Config::Furniture, Config::Gyroid, Config::Clothes,
	Config::Song, Config::Paper, Config::Trash, Config::Shell,
	Config::Fruit, Config::Turnip, Config::Catchable, Config::Item, Config::WallpaperCarpet, Config::Fossil, 
	Config::Tool, Config::Tree, Config::Weed, Config::Flower, Config::Rock,
	Config::Money, Config::ParchedFlower, Config::WateredFlower, Config::WiltedFlower, Config::Occupied, Config::Invalid;

void Config::initColors() {
	Config::Furniture		= C2D_Color32(99, 226, 90, 255);
	Config::Gyroid			= C2D_Color32(218, 155, 80, 255);
	Config::Clothes			= C2D_Color32(83, 143, 185, 255);
	Config::Song			= C2D_Color32(181, 237, 196, 255);
	Config::Paper			= C2D_Color32(181, 237, 234, 255);
	Config::Trash			= C2D_Color32(119, 136, 89, 255);
	Config::Shell			= C2D_Color32(252, 203, 211, 255);
	Config::Fruit			= C2D_Color32(225, 177, 225, 255);
	Config::Turnip			= C2D_Color32(199, 187, 175, 255);
	Config::Catchable		= C2D_Color32(198, 230, 101, 255);
	Config::Item			= C2D_Color32(252, 181, 52, 255);
	Config::WallpaperCarpet = C2D_Color32(172, 102, 102, 255);
	Config::Fossil			= C2D_Color32(116, 100, 89, 255);
	Config::Tool			= C2D_Color32(153, 153, 153, 255);
	Config::Tree			= C2D_Color32(161, 106, 67, 255);
	Config::Weed			= C2D_Color32(52, 152, 52, 255);
	Config::Flower			= C2D_Color32(237, 133, 196, 255);
	Config::Rock			= C2D_Color32(52, 52, 52, 255);
	Config::MoneyRock		= C2D_Color32(180, 89, 89, 255);
	Config::Money			= C2D_Color32(252, 252, 52, 255);
	Config::Building		= C2D_Color32(119, 119, 119, 255);
	Config::ParchedFlower	= C2D_Color32(180, 133, 52, 255);
	Config::WateredFlower	= C2D_Color32(52, 177, 177, 255);
	Config::Pattern			= C2D_Color32(172, 172, 252, 255);
	Config::WiltedFlower	= C2D_Color32(112, 112, 92, 255);
	Config::Occupied		= C2D_Color32(165, 165, 165, 255);
	Config::Invalid			= C2D_Color32(252, 52, 52, 255);
}