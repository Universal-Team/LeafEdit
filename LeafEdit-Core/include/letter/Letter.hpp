/*
*   This file is part of LeafEdit-Core
*   Copyright (C) 2020 Universal-Team
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

#ifndef _LEAFEDIT_CORE_LETTER_HPP
#define _LEAFEDIT_CORE_LETTER_HPP

#include "Item.hpp"
#include "types.hpp"

#include <memory>
#include <vector>

class Item;
class Letter {
protected:
	std::shared_ptr<u8[]> data;
	u32 Offset;
public:
	virtual ~Letter() {}
	Letter(std::shared_ptr<u8[]> dt, u32 offset) : data(dt), Offset(offset) { }
	Letter(const Letter& letter) = delete;
	Letter& operator=(const Letter& letter) = delete;

	virtual u16 playerid() const = 0;
	virtual std::u16string playername() const = 0;
	virtual u16 townid() const = 0;
	virtual std::u16string townname() const = 0;
	virtual u8 playerindex() const = 0;

	virtual std::u16string intro() const = 0;
	virtual std::u16string body() const = 0;
	virtual std::u16string end() const = 0;

	virtual u8 nameindex() const = 0;
	virtual u8 paperid() const = 0;
	virtual u8 flag() const = 0;
	virtual u8 senderid() const = 0;
	virtual u8 lettertype() const = 0;

	virtual std::unique_ptr<Item> item() const = 0;
};

#endif