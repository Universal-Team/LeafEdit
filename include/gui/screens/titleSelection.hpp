/*
*   This file is part of LeafEdit
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
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
#ifndef TITLESELECTION_HPP
#define TITLESELECTION_HPP

#include "gui/screens/screen.hpp"
#include "gameLoader.hpp"

class TitleSelection : public SCREEN 
{
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	void TitleDraw(void) const;
    void SelectionLogic(u32 hDown);
    void DrawInformationBox(void) const;

private:
	int selectedTitle = -2;
	bool GameSelected   = false;

    std::shared_ptr<Title> titleFromIndex(int i) const
    {
        if (i == -1)
        {
            return GameLoader::cardTitle;
        }
        else if (i == -2)
        {
            return nullptr;
        }
        else if ((size_t)i < GameLoader::installedTitles.size())
        {
            return GameLoader::installedTitles[i];
        }
        return nullptr;
    }
};

#endif