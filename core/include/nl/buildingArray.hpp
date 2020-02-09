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

#pragma once

#ifndef BUILDINGARRAY_HPP
#define BUILDINGARRAY_HPP

#include "save.hpp"
#include "types.hpp"

#include <string>

class BuildingArray {
public:
	BuildingArray();
	~BuildingArray(void);
	void Write(void);

	bool returnExistState(int pos);
	u8 returnXPos(int pos);
	u8 returnYPos(int pos);
	u16 returnID(int pos);
	std::string GetName(int pos); // Get Building name.
	u8 getBuildCount();

	// Set.
	u8 setXPos(int pos, u8 newPosition);
	u8 setYPos(int pos, u8 newPosition);
	u16 setBuilding(int pos, u16 newID);

	#ifdef _3DS
	void FixInvalidBuildings(void);
	#endif

private:
	bool m_exist[58];
	u8 m_xPos[58];
	u8 m_yPos[58];
	u16 m_ID[58];
	u8 m_Buildings;
	u8 m_BuildingsEvent;
};

#endif