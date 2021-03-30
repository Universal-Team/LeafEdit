/*
*   This file is part of LeafEdit
*   Copyright (C) 2019-2021 Universal-Team
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

#ifndef _LEAFEDIT_UPDATE_CENTER_HPP
#define _LEAFEDIT_UPDATE_CENTER_HPP

#include "common.hpp"
#include "download.hpp"
#include "structs.hpp"

#include <vector>

class UpdateCenter : public Screen {
public:
	void Draw(void) const override;
	void Logic(u32 hDown, u32 hHeld, touchPosition touch) override;
	UpdateCenter();
private:
	int Selection = 0, Mode = 0, selectedExtra = 0;
	void checkUpdate();
	bool NightlyAvailable = false;
	bool ReleaseAvailable = false;
	bool changelogShown = false; // I have no clue if I keep that, lol.
	bool hasCheckedForUpdate = false; // Always false at startup.

	ReleaseFetch latestRelease = { "" };
	NightlyFetch latestNightly = { "" };

	void DrawMain(void) const;
	void MainLogic(u32 hDown, u32 hHeld, touchPosition touch);

	void DrawExtras(void) const;
	void ExtrasLogic(u32 hDown, u32 hHeld, touchPosition touch);

	const std::vector<ButtonType> mainButtons = {
		{ 95, 34, 102, 20, "DOWNLOAD_RELEASE" },
		{ 95, 97, 102, 20, "DOWNLOAD_NIGHTLY" },
		{ 95, 159, 102, 20, "DOWNLOAD_ASSETS" }
	};

	const std::vector<ButtonType> extrasBtns = {
		{ 95, 97, 102, 20, "DOWNLOAD_3DSX" },
		{ 95, 159, 102, 20, "DOWNLOAD_CIA" }
	};

	/* For the extras. */
	const std::vector<std::string> extraNames = { "LeafEdit-Pattern-Editor" };
	const std::vector<std::string> extraAuthorNames = { "SuperSaiyajinStackZ" };
};

#endif