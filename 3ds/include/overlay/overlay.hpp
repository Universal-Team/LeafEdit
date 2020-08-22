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

#ifndef _LEAFEDIT_OVERLAY_HPP
#define _LEAFEDIT_OVERLAY_HPP

#include "common.hpp"
#include "coreUtils.hpp"

enum class PatternMode {
	/* Main modes. */
	Draw, // Always the default mode.
	Clear,
	Import,
	Export,
	Palette,
	Own,
	Exit,
	ExportInformation,

	/* Pattern changing stuff. TODO. */
	Name,
	CreatorName,
	CreatorID,
	TownName,
	TownID,
	CreatorGender
};

namespace Overlays {
	u16 SelectVillager(u16 oldID, const SaveType st);
	u16 SelectItem(u16 oldID, const SaveType st, const bool blockInv = false);
	std::string SelectFile(const std::vector<std::string> fileType, const std::string initialPath, const std::string Text);
	void SplashOverlay();
	void showCredits();
	
	PatternMode SelectPatternTool();
	void PaletteToolWW(std::shared_ptr<PatternImage> &pImg, C2D_Image &img);
	void PaletteToolNL(std::shared_ptr<PatternImage> &pImg, C2D_Image &img);
	void PaletteTool(std::shared_ptr<PatternImage> &pImg, C2D_Image &img, SaveType ST);

	std::string SelectDestination(std::string Text, std::string initialPath, std::string defaultDest);
	std::string RomfsSDOverlay(std::vector<std::string> extensions, std::string SDPath, std::string romfsPath, std::string Text);

}

#endif