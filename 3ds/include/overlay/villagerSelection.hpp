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

#ifndef _LEAFEDIT_VILLAGER_SELECTION_OVERLAY_HPP
#define _LEAFEDIT_VILLAGER_SELECTION_OVERLAY_HPP

#include "overlay.hpp"
#include "screenCommon.hpp"
#include "Villager.hpp"

#include <memory>

class VillagerSelection : public Overlay {
protected:
	std::shared_ptr<Villager> villager;
	SaveType save;
public:
	VillagerSelection(std::shared_ptr<Villager> v, const SaveType st) : villager(v), save(st) {
		this->isUsed = true;
		if (this->save == SaveType::WA) {
			this->maxSelection = 398;
		} else if (this->save == SaveType::NL) {
			this->maxSelection = 332;
		} else if (this->save == SaveType::WW) {
			this->maxSelection = 149;
		}
	}

	~VillagerSelection() override { }
	void DrawOverlayTop(void) const override;
	void DrawOverlayBottom(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;

private:
	int selection = 0;
	int maxSelection = 0;
};

#endif