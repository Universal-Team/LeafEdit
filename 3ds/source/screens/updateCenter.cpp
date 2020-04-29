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

extern bool touching(touchPosition touch, ButtonType button);
extern bool changesMade;

UpdateCenter::UpdateCenter() {
	if (checkWifiStatus() == true) {
		if (Msg::promptMsg("Do you like to check for updates?")) {
			Msg::DisplayMsg("Fetching latest updates...");
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
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - Update Center", 400);
	GFX::DrawGUI(gui_bottom_bar_idx, 0, 209); // We draw the bottom bar on this screen, cause `Current Version: `.
	// Display some Informations. :)
	if (Selection == 0) {
		Gui::DrawStringCentered(0, 60, 0.8f, BLACK, "Latest Version: " + latestRelease.Version, 395);
		Gui::DrawStringCentered(0, 80, 0.8f, BLACK, "Published at: " + latestRelease.Published, 395, 90);
		Gui::DrawStringCentered(0, 100, 0.8f, BLACK, latestRelease.ReleaseName, 395);
		Gui::DrawStringCentered(0, 217, 0.9f, WHITE, "Current Version: " + Config::currentRelease, 395);
	} else if (Selection == 3) {
		Gui::DrawStringCentered(0, 40, 0.8f, BLACK, "Latest Version: " + latestNightly.Target, 395);
		Gui::DrawStringCentered(0, 60, 0.8f, BLACK, "Committed by: " + latestNightly.Committer, 395, 90);
		Gui::DrawStringCentered(0, 80, 0.8f, BLACK, "Authored by: " + latestNightly.Author, 395, 90);
		Gui::DrawStringCentered(0, 100, 0.8f, BLACK, latestNightly.Message, 395, 90);
		Gui::DrawStringCentered(0, 217, 0.9f, WHITE, "Current Version: " + Config::currentNightly, 395);
	}

	GFX::DrawBottom();
	for (int i = 0; i < 6; i++) {
		GFX::DrawButton(mainButtons[i]);
		if (i == Selection)	GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x+100, mainButtons[i].y+30);
	}

	if (ReleaseAvailable)	GFX::DrawGUI(gui_update_idx, mainButtons[0].x+124, mainButtons[0].y-4);
	if (NightlyAvailable)	GFX::DrawGUI(gui_update_idx, mainButtons[3].x+124, mainButtons[3].y-4);
}


void UpdateCenter::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		Gui::screenBack();
		return;
	}
	
	if (hDown & KEY_X) {
		if (checkWifiStatus() == true) {
			if (Msg::promptMsg("Do you like to check for LeafEdit updates?")) {
				Msg::DisplayMsg("Fetching updates...");
				checkUpdate();
			}
		}
	}

	if (hDown & KEY_Y) {
		if (Selection == 0) {
			if (checkWifiStatus() == true) {
				changelogShown = Download::showReleaseInfo(latestRelease);
			}
		}
	}

	// Navigation.
	if (hDown & KEY_UP) {
		if(Selection > 0)	Selection--;
	}
	if (hDown & KEY_DOWN) {
			if(Selection < 5)	Selection++;
	}
	if (hDown & KEY_RIGHT) {
		if (Selection < 3) {
			Selection += 3;
		}
	}
	if (hDown & KEY_LEFT) {
		if (Selection < 6 && Selection > 2) {
			Selection -= 3;
		}
	}

	if (hDown & KEY_A) {
		switch(Selection) {
			case 0:
				if (checkWifiStatus() == true) {
					if (hasCheckedForUpdate) {
						if (ReleaseAvailable == false) {
							if (Msg::promptMsg("You seem to be on the latest Version.\nDo you still want to update?")) {
								if (Download::updateApp(false, latestRelease.Version) == 0) {
									ReleaseAvailable = false;
								}
							}
						} else {
							if (Download::updateApp(false, latestRelease.Version) == 0) {
								ReleaseAvailable = false;
							}
						}
					} else {
						// Still allow updating even if not checked, but with no version set.
						Download::updateApp(false, latestRelease.Version);
					}
				}
				break;
			case 1:
				if (checkWifiStatus() == true) {
					Download::downloadAssets();
				}
				break;
			case 3:
				if (checkWifiStatus() == true) {
					if (hasCheckedForUpdate) {
						if (NightlyAvailable == false) {
							if (Msg::promptMsg("You seem to be on the latest Version.\nDo you still want to update?")) {
								if (Download::updateApp(true, latestNightly.Target) == 0) {
									NightlyAvailable = false;
								}
							}
						} else {
							if (Download::updateApp(true, latestNightly.Target) == 0) {
								NightlyAvailable = false;
							}
						}
					} else {
						// Still allow updating even if not checked, but with no version set.
						if (checkWifiStatus() == true) {
							Download::updateApp(true, latestNightly.Target);
						}
					}
				}
				break;
			case 4:
				if (checkWifiStatus() == true) {
					Download::downloadScripts();
				}
				break;
		}
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, mainButtons[0])) {
			if (checkWifiStatus() == true) {
				if (hasCheckedForUpdate) {
					if (ReleaseAvailable == false) {
						if (Msg::promptMsg("You seem to be on the latest Version.\nDo you still want to update?")) {
							if (Download::updateApp(false, latestRelease.Version) == 0) {
								ReleaseAvailable = false;
							}
						}
					} else {
						if (Download::updateApp(false, latestRelease.Version) == 0) {
							ReleaseAvailable = false;
						}
					}
				} else {
					// Still allow updating even if not checked, but with no version set.
					if (checkWifiStatus() == true) {
						Download::updateApp(false, latestRelease.Version);
					}
				}
			}
		} else if (touching(touch, mainButtons[1])) {
			if (checkWifiStatus() == true) {
				Download::downloadAssets();
			}
		} else if (touching(touch, mainButtons[3])) {
			if (checkWifiStatus() == true) {
				if (hasCheckedForUpdate) {
					if (NightlyAvailable == false) {
						if (Msg::promptMsg("You seem to be on the latest Version.\nDo you still want to update?")) {
							if (Download::updateApp(true, latestNightly.Target) == 0) {
								NightlyAvailable = false;
							}
						}
					} else {
						if (Download::updateApp(true, latestNightly.Target) == 0) {
							NightlyAvailable = false;
						}
					}
				} else {
					// Still allow updating even if not checked, but with no version set.
					if (checkWifiStatus() == true) {
						Download::updateApp(true, latestNightly.Target);
					}
				}
			}
		} else if (touching(touch, mainButtons[4])) {
			if (checkWifiStatus() == true) {
				Download::downloadScripts();
			}
		}
	}
}