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

#include "updateCenter.hpp"

extern bool touching(touchPosition touch, Structs::ButtonPos button);

UpdateCenter::UpdateCenter() {
	if (checkWifiStatus() == true) {
		if (Msg::promptMsg(Lang::get("CHECK_LEAFEDIT_UPDATES"))) {
			Msg::DisplayMsg(Lang::get("FETCHING_UPDATES"));
			checkUpdate();
		}
	}
}

void UpdateCenter::checkUpdate() {
	// Get Latest Release & Nightly.
	latestRelease = Download::getLatestRelease2();
	latestNightly = Download::getLatestNightly();
	// Check if Nightly & Release matches.
	if (Config::currentRelease != latestRelease.Version)	ReleaseAvailable = true;
	if (Config::currentNightly != latestNightly.Target)		NightlyAvailable = true;
	hasCheckedForUpdate = true;
}


void UpdateCenter::Draw(void) const
{
	GFX::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, "LeafEdit - " + Lang::get("UPDATE_CENTER"), 400);

	// Display some Informations. :)
	if (Selection == 0) {
		Gui::DrawStringCentered(0, 60, 0.8f, WHITE, Lang::get("LATEST_VERSION") + latestRelease.Version, 395);
		Gui::DrawStringCentered(0, 80, 0.8f, WHITE, Lang::get("PUBLISHED_AT") + latestRelease.Published, 395, 90);
		Gui::DrawStringCentered(0, 100, 0.8f, WHITE, latestRelease.ReleaseName, 395);
		Gui::DrawStringCentered(0, 215, 0.8f, WHITE, Lang::get("CURRENT_VERSION") + Config::currentRelease, 395);
	} else if (Selection == 1) {
		Gui::DrawStringCentered(0, 40, 0.8f, WHITE, Lang::get("LATEST_VERSION") + latestNightly.Target, 395);
		Gui::DrawStringCentered(0, 60, 0.8f, WHITE, Lang::get("COMMITTED_BY") + latestNightly.Committer, 395, 90);
		Gui::DrawStringCentered(0, 80, 0.8f, WHITE, Lang::get("AUTHORED_BY") + latestNightly.Author, 395, 90);
		Gui::DrawStringCentered(0, 100, 0.8f, WHITE, latestNightly.Message, 395, 90);
		Gui::DrawStringCentered(0, 215, 0.8f, WHITE, Lang::get("CURRENT_VERSION") + Config::currentNightly, 395);
	}

	GFX::DrawBottom();
	for (int i = 0; i < 3; i++) {
		Gui::Draw_Rect(mainButtons[i].x, mainButtons[i].y, mainButtons[i].w, mainButtons[i].h, UNSELECTED_COLOR);
	}

	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("DOWNLOAD_RELEASE")))/2-80+17.5, 0.8, WHITE, Lang::get("DOWNLOAD_RELEASE"), 130, 25);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("DOWNLOAD_NIGHTLY")))/2-20+17.5, 0.8, WHITE, Lang::get("DOWNLOAD_NIGHTLY"), 130, 25);
	Gui::DrawStringCentered(0, (240-Gui::GetStringHeight(0.8, Lang::get("DOWNLOAD_ASSETS")))/2+75-17.5, 0.8, WHITE, Lang::get("DOWNLOAD_ASSETS"), 130, 25);

	// Draw Dots.
	if (ReleaseAvailable)	GFX::DrawSpriteBlend(sprites_dot_idx, mainButtons[0].x+130, mainButtons[0].y-4, LIGHT_COLOR, 1, 1);
	if (NightlyAvailable)	GFX::DrawSpriteBlend(sprites_dot_idx, mainButtons[1].x+130, mainButtons[1].y-4, LIGHT_COLOR, 1, 1);
	// Draw Pointer.
	GFX::DrawSprite(sprites_pointer_idx, mainButtons[Selection].x+133, mainButtons[Selection].y);
}


void UpdateCenter::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
	
	if (hDown & KEY_X) {
		if (checkWifiStatus() == true) {
			if (Msg::promptMsg(Lang::get("CHECK_LEAFEDIT_UPDATES"))) {
				Msg::DisplayMsg(Lang::get("FETCHING_UPDATES"));
				checkUpdate();
			}
		}
	}

	if (hDown & KEY_Y) {
		if (Selection == 0) {
			changelogShown = Download::showReleaseInfo(latestRelease);
		}
	}
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	} else if (hDown & KEY_DOWN) {
		if(Selection < 2)	Selection++;
	}

	if (hDown & KEY_A) {
		switch(Selection) {
			case 0:
				if (hasCheckedForUpdate) {
					if (ReleaseAvailable == false) {
						if (Msg::promptMsg(Lang::get("SEEM_ON_LATEST"))) {
							if (Download::updateApp(false) == 0) {
								Config::currentRelease = latestRelease.Version;
								ReleaseAvailable = false;
							}
						}
					} else {
						if (Download::updateApp(false) == 0) {
							Config::currentRelease = latestRelease.Version;
							ReleaseAvailable = false;
						}
					}
				} else {
					// Still allow updating even if not checked, but with no version set.
					Download::updateApp(false);
				}
				break;
			case 1:
				if (hasCheckedForUpdate) {
					if (NightlyAvailable == false) {
						if (Msg::promptMsg(Lang::get("SEEM_ON_LATEST"))) {
							if (Download::updateApp(true) == 0) {
								Config::currentNightly = latestNightly.Target;
								NightlyAvailable = false;
							}
						}
					} else {
						if (Download::updateApp(true) == 0) {
							Config::currentNightly = latestNightly.Target;
							NightlyAvailable = false;
						}
					}
				} else {
					// Still allow updating even if not checked, but with no version set.
					Download::updateApp(true);
				}
				break;
			case 2:
				Download::downloadAssets();
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			if (hasCheckedForUpdate) {
				if (ReleaseAvailable == false) {
					if (Msg::promptMsg(Lang::get("SEEM_ON_LATEST"))) {
						if (Download::updateApp(false) == 0) {
							Config::currentRelease = latestRelease.Version;
							ReleaseAvailable = false;
						}
					}
				} else {
					if (Download::updateApp(false) == 0) {
						Config::currentRelease = latestRelease.Version;
						ReleaseAvailable = false;
					}
				}
			} else {
				// Still allow updating even if not checked, but with no version set.
				Download::updateApp(false);
			}
		} else if (touching(touch, mainButtons[1])) {
			if (hasCheckedForUpdate) {
				if (NightlyAvailable == false) {
					if (Msg::promptMsg(Lang::get("SEEM_ON_LATEST"))) {
						if (Download::updateApp(true) == 0) {
							Config::currentNightly = latestNightly.Target;
							NightlyAvailable = false;
						}
					}
				} else {
					if (Download::updateApp(true) == 0) {
						Config::currentNightly = latestNightly.Target;
						NightlyAvailable = false;
					}
				}
			} else {
				// Still allow updating even if not checked, but with no version set.
				Download::updateApp(true);
			}
		} else if (touching(touch, mainButtons[2])) {
			Download::downloadAssets();
		}
	}

	if (hHeld & KEY_SELECT) {
		Msg::HelperBox(Lang::get("A_SELECTION") + "\n" + Lang::get("B_BACK"));
	}
}