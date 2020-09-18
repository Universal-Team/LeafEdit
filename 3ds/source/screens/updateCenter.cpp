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

#include "updateCenter.hpp"

extern std::unique_ptr<Config> config;
extern bool touching(touchPosition touch, ButtonType button);
extern bool changesMade;

UpdateCenter::UpdateCenter() {
	if (checkWifiStatus()) {
		if (Msg::promptMsg(Lang::get("FETCH_UPDATES"))) {
			Msg::DisplayMsg(Lang::get("FETCHING_UPDATES"));
			checkUpdate();
		}
	}
}

void UpdateCenter::checkUpdate() {
	/* Get Latest Release & Nightly. */
	latestRelease = Download::getLatestRelease2();
	latestNightly = Download::getLatestNightly();

	/* Check if Nightly & Release matches. */
	if (config->currentRelease() != latestRelease.Version) ReleaseAvailable = true;
	if (config->currentNightly() != latestNightly.Target) NightlyAvailable = true;
	hasCheckedForUpdate = true;
}

void UpdateCenter::Draw(void) const {
	switch(this->Mode) {
		case 0:
			this->DrawMain();
			break;

		case 1:
			this->DrawExtras();
			break;
	}
}

void UpdateCenter::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	switch(this->Mode) {
		case 0:
			this->MainLogic(hDown, hHeld, touch);
			break;

		case 1:
			this->ExtrasLogic(hDown, hHeld, touch);
			break;
	}
}

void UpdateCenter::DrawMain(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("UPDATE_CENTER"), 400, 0, font);
	GFX::DrawGUI(gui_bottom_bar_idx, 0, 209);

	/* Display some Informations. */
	if (this->Selection == 0) {
		Gui::DrawStringCentered(0, 60, 0.8f, BLACK, Lang::get("LATEST_VERSION") + latestRelease.Version, 395, 0, font);
		Gui::DrawStringCentered(0, 80, 0.8f, BLACK, Lang::get("PUBLISHED_AT") + latestRelease.Published, 395, 90, font);
		Gui::DrawStringCentered(0, 100, 0.8f, BLACK, latestRelease.ReleaseName, 395, 0, font);
		Gui::DrawStringCentered(0, 218, 0.9f, WHITE, Lang::get("CURRENT_VERSION") + config->currentRelease(), 395, 0, font);

	} else if (this->Selection == 1) {
		Gui::DrawStringCentered(0, 40, 0.8f, BLACK, Lang::get("LATEST_VERSION") + latestNightly.Target, 395, 0, font);
		Gui::DrawStringCentered(0, 60, 0.8f, BLACK, Lang::get("COMMITTED_BY") + latestNightly.Committer, 395, 90, font);
		Gui::DrawStringCentered(0, 80, 0.8f, BLACK, Lang::get("AUTHORED_BY") + latestNightly.Author, 395, 90, font);
		Gui::DrawStringCentered(0, 100, 0.8f, BLACK, latestNightly.Message, 395, 90, font);
		Gui::DrawStringCentered(0, 218, 0.9f, WHITE, Lang::get("CURRENT_VERSION") + config->currentNightly(), 395, 0, font);
	}

	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
	GFX::DrawBottom();

	for (int i = 0; i < 3; i++) {
		GFX::DrawButton(mainButtons[i]);
		if (i == Selection)	GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x + 100, mainButtons[i].y + 30);
	}

	if (this->ReleaseAvailable)	GFX::DrawGUI(gui_update_idx, mainButtons[0].x + 124, mainButtons[0].y - 4);
	if (this->NightlyAvailable)	GFX::DrawGUI(gui_update_idx, mainButtons[1].x + 124, mainButtons[1].y - 4);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}


void UpdateCenter::MainLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hDown & KEY_B) {
		Gui::screenBack(doFade);
	}
	
	if (hDown & KEY_X) {
		if (checkWifiStatus()) {
			if (Msg::promptMsg(Lang::get("FETCH_UPDATES"))) {
				Msg::DisplayMsg(Lang::get("FETCHING_UPDATES"));
				checkUpdate();
			}
		}
	}

	if (hDown & KEY_Y) {
		if (this->Selection == 0) {
			if (checkWifiStatus()) {
				changelogShown = Download::showReleaseInfo(latestRelease);
			}
		}
	}

	if (hRepeat & KEY_UP) {
		if (this->Selection > 0) this->Selection --;
	}
	
	if (hRepeat & KEY_DOWN) {
		if (this->Selection < 2) this->Selection++;
	}

	if (hDown & KEY_A) {
		switch(this->Selection) {
			case 0:
				if (checkWifiStatus()) {
					if (this->hasCheckedForUpdate) {
						if (!this->ReleaseAvailable) {
							if (Msg::promptMsg(Lang::get("ON_LATEST_VERSION"))) {
								if (Download::updateApp(false, latestRelease.Version) == 0) {
									this->ReleaseAvailable = false;
								}
							}

						} else {
							if (Download::updateApp(false, latestRelease.Version) == 0) {
								this->ReleaseAvailable = false;
							}
						}

					} else {
						/* Still allow updating even if not checked, but with no version set. */
						Download::updateApp(false, latestRelease.Version);
					}
				}
				break;

			case 1:
				if (checkWifiStatus()) {
					if (this->hasCheckedForUpdate) {
						if (!this->NightlyAvailable) {
							if (Msg::promptMsg(Lang::get("ON_LATEST_VERSION"))) {
								if (Download::updateApp(true, latestNightly.Target) == 0) {
									this->NightlyAvailable = false;
								}
							}

						} else {
							if (Download::updateApp(true, latestNightly.Target) == 0) {
								this->NightlyAvailable = false;
							}
						}

					} else {
						/* Still allow updating even if not checked, but with no version set. */
						if (checkWifiStatus()) {
							Download::updateApp(true, latestNightly.Target);
						}
					}
				}
				break;

			case 2:
				if (checkWifiStatus()) Download::downloadAssets();
				break;
		}
	}


	if (hDown & KEY_TOUCH) {
		if (touching(touch, this->mainButtons[0])) {
			if (checkWifiStatus()) {
				if (this->hasCheckedForUpdate) {
					if (!this->ReleaseAvailable) {
						if (Msg::promptMsg(Lang::get("ON_LATEST_VERSION"))) {
							if (Download::updateApp(false, latestRelease.Version) == 0) {
								this->ReleaseAvailable = false;
							}
						}

					} else {
						if (Download::updateApp(false, latestRelease.Version) == 0) {
							this->ReleaseAvailable = false;
						}
					}

				} else {
					/* Still allow updating even if not checked, but with no version set. */
					if (checkWifiStatus()) {
						Download::updateApp(false, latestRelease.Version);
					}
				}
			}


		} else if (touching(touch, this->mainButtons[1])) {
			if (checkWifiStatus()) {
				if (this->hasCheckedForUpdate) {
					if (!this->NightlyAvailable) {
						if (Msg::promptMsg(Lang::get("ON_LATEST_VERSION"))) {
							if (Download::updateApp(true, latestNightly.Target) == 0) {
								this->NightlyAvailable = false;
							}
						}

					} else {
						if (Download::updateApp(true, latestNightly.Target) == 0) {
							this->NightlyAvailable = false;
						}
					}

				} else {
					/* Still allow updating even if not checked, but with no version set. */
					if (checkWifiStatus()) {
						Download::updateApp(true, latestNightly.Target);
					}
				}
			}


		} else if (touching(touch, this->mainButtons[2])) {
			if (checkWifiStatus()) Download::downloadAssets();
		}
	}

	/* Go to Extras section. */
	if (hDown & KEY_SELECT) {
		this->Selection = 0;
		this->Mode = 1;
	}
}

void UpdateCenter::DrawExtras(void) const {
	GFX::DrawTop();
	Gui::DrawStringCentered(0, -2, 0.9f, WHITE, "LeafEdit - " + Lang::get("UPDATE_CENTER") + " Extras", 400, 0, font);
	GFX::DrawGUI(gui_pattern_editor_idx, 25 + 1, 35 + 1);

	GFX::DrawBottom();
	Gui::DrawStringCentered(0, 35, 0.7f, BLACK, Lang::get("APP") + this->extraNames[this->selectedExtra], 310, 0, font);
	Gui::DrawStringCentered(0, 50, 0.7f, BLACK, Lang::get("AUTHOR") + this->extraAuthorNames[this->selectedExtra], 310, 0, font);

	for (int i = 0; i < 2; i++) {
		GFX::DrawButton(extrasBtns[i]);
		if (i == Selection)	GFX::DrawGUI(gui_pointer_idx, extrasBtns[i].x + 100, extrasBtns[i].y + 30);
	}
}

void UpdateCenter::ExtrasLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	u32 hRepeat = hidKeysDownRepeat();

	if (hRepeat & KEY_DOWN) {
		if (this->Selection < 1) this->Selection++;
	}

	if (hRepeat & KEY_UP) {
		if (this->Selection > 0) this->Selection--;
	}

	if (hDown & KEY_B) {
		this->Selection = 0;
		this->Mode = 0;
	}

	if (hDown & KEY_A) {
		Download::getPatternEditor((bool)this->Selection);
	}
}