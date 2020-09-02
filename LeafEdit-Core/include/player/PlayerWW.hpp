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

#ifndef _LEAFEDIT_CORE_PLAYER_WW_HPP
#define _LEAFEDIT_CORE_PLAYER_WW_HPP

#include "Item.hpp"
#include "ItemWW.hpp"
#include "Letter.hpp"
#include "LetterWW.hpp"
#include "Pattern.hpp"
#include "PatternWW.hpp"
#include "Player.hpp"
#include "types.hpp"

#include <memory>
#include <vector>

class Item;
class ItemWW;
class Letter;
class LetterWW;
class Pattern;
class PatternWW;
class PlayerWW : public Player {
protected:
	std::shared_ptr<u8[]> data;
	u32 offset;
	WWRegion region;
	int Index;
public:
	virtual ~PlayerWW() {}
	PlayerWW(std::shared_ptr<u8[]> playerData, u32 playerOffset, WWRegion Region, int index) :
			Player(playerData, playerOffset, index), data(playerData), offset(playerOffset), region(Region), Index(index) { }
			
	u32 getPlayerSize() const override {
		switch(this->region) {
			case WWRegion::USA_REV0:
			case WWRegion::USA_REV1:
			case WWRegion::EUR_REV1:
				return 0x228C;
			case WWRegion::JPN_REV0:
			case WWRegion::JPN_REV1:
				return 0x1D10;
			case WWRegion::KOR_REV1:
				return 0x249C;
			case WWRegion::UNKNOWN:
				return 0;
		}
		return 0;
	}


	u8 face() const override;
	void face(u8 v) override;
	u8 gender() const override;
	void gender(u8 v) override;
	u16 tan() const override;
	void tan(u16 v) override;
	u8 hairstyle() const override;
	void hairstyle(u8 v) override;
	u8 haircolor() const override;
	void haircolor(u8 v) override;
	u8 eyecolor() const override;
	void eyecolor(u8 v) override;
	u8 badge(int badge) const override;
	void badge(int badge, u8 v) override;
	u16 playerid() const override;
	void playerid(u16 v) override;
	u16 townid() const override;
	void townid(u16 v) override;
	std::u16string townname() const override;
	void townname(std::u16string v) override;
	bool exist() const override;
	std::u16string name() const override;
	void name(std::u16string v) override;
	u32 wallet() const override;
	void wallet(u32 v) override;
	u32 bank() const override;
	void bank(u32 v) override;
	u32 islandmedals() const override;
	void islandmedals(u32 v) override;
	u32 coupons() const override;
	void coupons(u32 v) override;
	std::unique_ptr<Letter> letter(int slot) const override;
	
	std::unique_ptr<Item> pocket(int slot) const override;
	std::unique_ptr<Item> dresser(int slot) const override;
	std::unique_ptr<Item> islandbox(int slot) const override { return nullptr; };
	std::unique_ptr<Item> storage(int slot) const override { return nullptr; };
	std::unique_ptr<Pattern> pattern(int slot) const override;

	u8 *tpcImage() const override;
	bool hasTPC() const override { return false; }
private:
	u8* playerPointer() const {
		return data.get() + offset;
	}
};

#endif