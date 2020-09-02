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

#ifndef _LEAFEDIT_CORE_LETTER_WA_HPP
#define _LEAFEDIT_CORE_LETTER_WA_HPP

#include "Item.hpp"
#include "ItemWA.hpp"
#include "Letter.hpp"

#include <memory>
#include <vector>

class Item;
class ItemWA;
class LetterWA : public Letter {
protected:
	u32 Offset;
	std::shared_ptr<u8[]> data;
public:
	virtual ~LetterWA() {}
	LetterWA(std::shared_ptr<u8[]> letterData, u32 offset) :
		Letter(letterData, offset), Offset(offset), data(letterData) { }

	u16 playerid() const override;
	std::u16string playername() const override;
	u16 townid() const override;
	std::u16string townname() const override;
	u8 playerindex() const override;

	std::u16string intro() const override;
	std::u16string body() const override;
	std::u16string end() const override;

	u8 nameindex() const override;
	u8 paperid() const override;
	u8 flag() const override;
	u8 senderid() const override;
	u8 lettertype() const override;

	std::unique_ptr<Item> item() const override;
private:
	u8* letterPointer() const {
		return data.get() + Offset;
	}
};

#endif