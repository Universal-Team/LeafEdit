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

#include "buildingArray.hpp"

#include <map>

/* Building names.
0x00, "Player 1's House"
0x01, "Player 2's House"
0x02, "Player 3's House"
0x03, "Player 4's House"
0x04, "Mailbox Only (Player 1)"
0x05, "Mailbox Only (Player 2)"
0x06, "Mailbox Only (Player 3)"
0x07, "Mailbox Only (Player 4)"
0x08, "Villager 1's House"
0x09, "Villager 2's House"
0x0A, "Villager 3's House"
0x0B, "Villager 4's House"
0x0C, "Villager 5's House"
0x0D, "Villager 6's House"
0x0E, "Villager 7's House"
0x0F, "Villager 8's House"
0x10, "Villager 9's House"
0x11, "Villager 10's House"
0x4C, "Modern Police Station"
0x4D, "Classic Police Station"
0x4E, "Café"
0x4F, "Reset Center"
0x50, "Classic Town Hall"
0x51, "Zen Town Hall"
0x52, "Fairy-Tale Town Hall"
0x53, "Modern Town Hall"
0x54, "Classic Train Station"
0x55, "Zen Train Station"
0x56, "Fairy-Tale Train Station"
0x57, "Modern Train Station"
0x58, "Recycle Shop"
0x59, "Town Plaza"
0x5A, "Railroad Crossing Arms"
0x5B, "Bulletin Board"
0x5C, "Island Storage Bin"
0x5D, "Dock"
0x5E, "Katrina's Tent"
0x5F, "Camper's Tent"
0x60, "Redd's Tent"
0x61, "Chip's Tent"
0x62, "Nat's Tent"
0x63, "Franklin's Table"
0x64, "Pavé's Dance Stage"
0x65, "Countdown Board"
0x66, "Redd's Stand"
0x67, "Dream World Bed"
0x69, "Island Hut"
0x6A, "Lloid"
0x6B, "Face Cutout Standee - New Years Day"
0x6C, "Face Cutout Standee - Japanese Rite"
0x6D, "Face Cutout Standee - Sky Night"
0x6E, "Face Cutout Standee - Groundhog Day"
0x6F, "Face Cutout Standee - Bean Throwing Festival (Setsubun)"
0x70, "Face Cutout Standee - Japanese Wedding"
0x71, "Face Cutout Standee - Shamrock Day"
0x72, "Face Cutout Standee - Garden Kids"
0x73, "Face Cutout Standee - Nature Day"
0x74, "Face Cutout Standee - Carp Banner"
0x75, "Face Cutout Standee - Japanese School"
0x76, "Face Cutout Standee - Summer Solstice"
0x77, "Face Cutout Standee - Starcrossed Day (Tanabata)"
0x78, "Face Cutout Standee - Obon"
0x79, "Face Cutout Standee - Labor Day"
0x7A, "Face Cutout Standee - Tsukimi"
0x7B, "Face Cutout Standee - Autumn Moon"
0x7C, "Face Cutout Standee - Explorer's Day"
0x7D, "Face Cutout Standee - Toy Day"
0x7E, "Face Cutout Standee - Winter Solstice"
0x8E, "Main Street Lamp"
0x8F, "Main Street Bench (Vertical)"
0x90, "Cobblestone Bridge (N-S)"
0x91, "Cobblestone Bridge (SW-NE)"
0x92, "Cobblestone Bridge (NW-SE)"
0x93, "Cobblestone Bridge (W-E)"
0x94, "Suspension Bridge (N-S)"
0x95, "Suspension Bridge (SW-NE)"
0x96, "Suspension Bridge (NW-SE)"
0x97, "Suspension Bridge (W-E)"
0x98, "Brick Bridge (N-S)"
0x99, "Brick Bridge (SW-NE)"
0x9A, "Brick Bridge (NW-SE)"
0x9B, "Brick Bridge (W-E)"
0x9C, "Modern Bridge (N-S)"
0x9D, "Modern Bridge (SW-NE)"
0x9E, "Modern Bridge (NW-SE)"
0x9F, "Modern Bridge (W-E)"
0xA0, "Wooden Bridge (N-S)"
0xA1, "Wooden Bridge (SW-NE)"
0xA2, "Wooden Bridge (NW-SE)"
0xA3, "Wooden Bridge (W-E)"
0xA4, "Fairy-Tale Bridge (N-S)"
0xA5, "Fairy-Tale Bridge (SW-NE)"
0xA6, "Fairy-Tale Bridge (NW-SE)"
0xA7, "Fairy-Tale Bridge (W-E)"
0xA8, "Yellow Bench"
0xA9, "Blue Bench"
0xAA, "Wood Bench"
0xAB, "Metal Bench"
0xAC, "Log Bench"
0xAD, "Modern Bench"
0xAE, "Fairy-Tale Bench"
0xAF, "Zen Bench"
0xB0, "Flower Bed"
0xB1, "Drinking Fountain"
0xB2, "Instrument Shelter"
0xB3, "Sandbox"
0xB4, "Garbage Can"
0xB5, "Pile Of Pipes"
0xB6, "Water Well"
0xB7, "Fountain"
0xB8, "Tire Toy"
0xB9, "Jungle Gym"
0xBA, "Park Clock"
0xBB, "Modern Clock"
0xBC, "Fairy-Tale Clock"
0xBD, "Zen Clock"
0xBE, "Street Lamp"
0xBF, "Round Streetlight"
0xC0, "Streetlight"
0xC1, "Modern Streetlight"
0xC2, "Fairy-Tale Streetlight"
0xC3, "Zen Streetlight"
0xC4, "Balloon Arch"
0xC5, "Flower Arch"
0xC6, "Campsite"
0xC7, "Picnic Blanket"
0xC8, "Hammock"
0xC9, "Fire Pit"
0xCA, "Camping Cot"
0xCB, "Outdoor Chair"
0xCC, "Torch"
0xCD, "Zen Garden"
0xCE, "Hot Spring"
0xCF, "Geyser"
0xD0, "Statue Fountain"
0xD1, "Stone Tablet"
0xD2, "Water Pump"
0xD3, "Wisteria Trellis"
0xD4, "Bell"
0xD5, "Zen Bell"
0xD6, "Scarecrow"
0xD7, "Rack Of Rice"
0xD8, "Fence"
0xD9, "Bus Stop"
0xDA, "Fire Hydrant"
0xDB, "Traffic Signal"
0xDC, "Custom-Design Sign"
0xDD, "Face-Cutout Standee"
0xDE, "Caution Sign"
0xDF, "Do-Not-Enter Sign"
0xE0, "Yield Sign"
0xE1, "Cube Sculpture"
0xE2, "Archway Sculpture"
0xE3, "Chair Sculpture"
0xE4, "Illuminated Heart"
0xE5, "Illuminated Arch"
0xE6, "Illuminated Clock"
0xE7, "Illuminated Tree"
0xE8, "Stadium Light"
0xE9, "Video Screen"
0xEA, "Drilling Rig"
0xEB, "Parabolic Antenna"
0xEC, "Solar Panel"
0xED, "Wind Turbine"
0xEE, "Windmill"
0xEF, "Lighthouse"
0xF0, "Tower"
0xF1, "Stonehenge"
0xF2, "Totem Pole"
0xF3, "Moai Statue"
0xF4, "Pyramid"
0xF5, "Sphinx"
0xF6, "Round Topiary"
0xF7, "Square Topiary"
0xF8, "Tulip Topiary"
0xF9, "Flower Clock"
0xFC, "No Building"
*/

std::map<u16, std::string> g_buildingDatabase; // ID & name.

BuildingArray::BuildingArray(u32 offset) {
	m_offset = offset;
	m_ID = Save::Instance()->ReadU16(m_offset);
	m_xPos = Save::Instance()->ReadU8(m_offset + 2);
	m_yPos = Save::Instance()->ReadU8(m_offset + 3);
	m_exist = ((m_ID < 0xFC) && !(m_ID >= 0x12 && m_ID <= 0x4B)); // Returns true, always.
}

std::string BuildingArray::GetName() {
	for (auto const& entry : g_buildingDatabase) {
		if (entry.first == m_ID) {
			return entry.second;
		}
	}
	return std::string("???");
}

BuildingArray::~BuildingArray() { }

// Return some stuff. ;)
u16 BuildingArray::returnID() {
	return m_ID;
}

bool BuildingArray::returnExistState() {
	return m_exist;
}

u8 BuildingArray::returnXPos() {
	return m_xPos;
}

u8 BuildingArray::returnYPos() {
	return m_yPos;
}

// Set Positions.
u8 BuildingArray::setXPos(u8 newPosition) {
	return m_xPos = newPosition;
}

u8 BuildingArray::setYPos(u8 newPosition) {
	return m_yPos = newPosition;
}

u16 BuildingArray::setBuilding(u16 newID) {
	return m_ID = newID;
}

// TODO?
void BuildingArray::Write() {
	Save::Instance()->Write(m_offset, m_ID);
	Save::Instance()->Write(m_offset + 2, m_xPos);
	Save::Instance()->Write(m_offset + 3, m_yPos);
}