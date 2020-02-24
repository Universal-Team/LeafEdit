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

#include "wwPlayer.hpp"
#include "wwsave.hpp"

WWPlayer::WWPlayer() { }

WWPlayer::~WWPlayer() { }

WWPlayer::WWPlayer(u32 offset, u32 index) {
	this->m_offset = offset;
	this->m_index = index;

	this->Gender = WWSave::Instance()->ReadU16(offset + 0x228A); // Gender.
	this->Bells = WWSave::Instance()->ReadU32(offset + 0x1B40); // Bells from the Wallet.
	this->Name = WWSave::Instance()->ReadString(offset + 0x2282, 8, false);
}

void WWPlayer::Write() {
	WWSave::Instance()->Write(this->m_offset + 0x1B40, this->Bells);
	WWSave::Instance()->Write(this->m_offset + 0x228A, this->Gender);
	WWSave::Instance()->Write(this->m_offset + 0x2282, this->Name, 8, false);
}

bool WWPlayer::Exists() {
	return WWSave::Instance()->ReadU16(this->m_offset + 0x2280) != 0;
}