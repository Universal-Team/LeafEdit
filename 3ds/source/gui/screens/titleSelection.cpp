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

#include "common/common.hpp"

#include "core/gameLoader.hpp"

#include "gui/screens/mainMenu.hpp"
#include "gui/screens/screenCommon.hpp"
#include "gui/screens/settings.hpp"
#include "gui/screens/titleSelection.hpp"

extern bool exiting;
extern int fadealpha;
extern bool fadein;

extern bool touching(touchPosition touch, Structs::ButtonPos button);

bool isROMHack = false;
bool isACWW = false;
bool isCard = false;

std::vector<u64> wlID = {
	0x00040000004C5700, // Animal Crossing: Welcome Luxury [ROM Hack] https://gitlab.com/Kyusetzu/ACWL
};

std::vector<u64> newIDs = {
	0x0004000000198D00, // JPN.
	0x0004000000198E00, // USA.
	0x0004000000198F00, // EUR.
	0x0004000000199000,  // KOR.
};

std::vector<u64> oldIDs = {
	0x0004000000086200, // JPN.
	0x0004000000086300, // USA.
	0x0004000000086400, // EUR.
	0x0004000000086500, // KOR.
};

void TitleSelection::Draw(void) const {
	if (selectMode == 0) {
		DrawGameSelector();
	} else if (selectMode == 1) {
		DrawVersionSelector();
	} else if (selectMode == 2) {
		DrawRegionSelector();
	}
}

void TitleSelection::Logic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (selectMode == 0) {
		gameLogic(hDown, hHeld, touch);
	} else if (selectMode == 1) {
		versionLogic(hDown, hHeld, touch);
	} else if (selectMode == 2) {
		regionLogic(hDown, hHeld, touch);
	}

	if (hDown & KEY_X) {
		GameLoader::updateCheck2();
	}
}

void TitleSelection::DrawGameSelector(void) const
{
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, Lang::get("GAME_GROUP_SELECT"), 398);
	Gui::DrawStringCentered(0, 214, 0.9f, WHITE, Lang::get("Y_SETTINGS"), 398);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(0, 0, 0, fadealpha)); // Fade in/out effect
	Gui::DrawBottom();
	for (int i = 0; i < 4; i++) {
		Gui::Draw_Rect(gameButtons[i].x, gameButtons[i].y, gameButtons[i].w, gameButtons[i].h, UNSELECTED_COLOR);
		if (selectedGame == i) {
			Gui::drawAnimatedSelector(gameButtons[i].x, gameButtons[i].y, gameButtons[i].w, gameButtons[i].h, .030f, SELECTED_COLOR);
		}
	}

	Gui::sprite(0, sprites_Icon_idx, 16, 90);
	Gui::sprite(0, sprites_IconWA_idx, 96, 90);
	Gui::sprite(0, sprites_IconWL_idx, 176, 90);
	Gui::sprite(0, sprites_IconWW_idx, 256, 90);

	Gui::DrawStringCentered(-120, 140, 0.75f, WHITE, Lang::get("NEW_LEAF_1"), 50);
	Gui::DrawStringCentered(-120, 155, 0.75f, WHITE, Lang::get("NEW_LEAF_2"), 50);
	Gui::DrawStringCentered(-40, 140, 0.75f, WHITE, Lang::get("WELCOME_AMIIBO_1"), 50);
	Gui::DrawStringCentered(-40, 155, 0.75f, WHITE, Lang::get("WELCOME_AMIIBO_2"), 50);
	Gui::DrawStringCentered(40, 140, 0.75f, WHITE, Lang::get("WELCOME_LUXURY_1"), 50);
	Gui::DrawStringCentered(40, 155, 0.75f, WHITE, Lang::get("WELCOME_LUXURY_2"), 50);
	Gui::DrawStringCentered(120, 140, 0.75f, WHITE, Lang::get("WILD_WORLD_1"), 50);
	Gui::DrawStringCentered(120, 155, 0.75f, WHITE, Lang::get("WILD_WORLD_2"), 50);
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(0, 0, 0, fadealpha)); // Fade in/out effect
}

void TitleSelection::gameLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_RIGHT || hDown & KEY_R) {
		if (selectedGame < 3)	selectedGame++;
	} else if (hDown & KEY_LEFT || hDown & KEY_L) {
		if (selectedGame > 0)	selectedGame--;
	}

	if (hDown & KEY_START) {
		exiting = true;
	}

	if (hDown & KEY_Y) {
		Gui::setScreen(std::make_unique<Settings>());
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, gameButtons[0])) {
			selectedGame = 0;
			selectMode = 1;
		} else if (touching(touch, gameButtons[1])) {
			selectedGame = 1;
			selectMode = 1;
		} else if (touching(touch, gameButtons[2])) {
			if (GameLoader::checkTitle(wlID[0])) {
				isROMHack = true;
				isCard = false;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (touching(touch, gameButtons[3])) {
			isACWW = true;
			Gui::setScreen(std::make_unique<MainMenu>());
		}
	}


	if (hDown & KEY_A) {
		if (selectedGame == 2) {
			if (GameLoader::checkTitle(wlID[0])) {
				isROMHack = true;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (selectedGame == 3) {
				isACWW = true;
				isCard = true;
				Gui::setScreen(std::make_unique<MainMenu>());
		} else {
			selectMode = 1;
		}
	}
}

void TitleSelection::DrawVersionSelector(void) const
{
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, Lang::get("VERSION_SELECT"), 400);

	if (selectedGame == 0) {
		Gui::sprite(0, sprites_Icon_idx, 37.5, 80);
	} else if (selectedGame == 1) {
		Gui::sprite(0, sprites_IconWA_idx, 37.5, 80);
	}

	Gui::DrawBottom();
	
	for (int i = 0; i < 2; i++) {
		Gui::Draw_Rect(versionButtons[i].x, versionButtons[i].y, versionButtons[i].w, versionButtons[i].h, UNSELECTED_COLOR);
		if (selectedVersion == i) {
			Gui::drawAnimatedSelector(versionButtons[i].x, versionButtons[i].y, versionButtons[i].w, versionButtons[i].h, .030f, SELECTED_COLOR);
		}
	}

	Gui::sprite(0, sprites_gameCard_idx, 78, 100);
	Gui::sprite(0, sprites_sdCard_idx, 198, 100);

	Gui::DrawStringCentered(-10-50, 80, 0.8, WHITE, Lang::get("CARD"), 80);
	Gui::DrawStringCentered(110-50, 80, 0.8, WHITE, Lang::get("INSTALLED"), 80);
}

void TitleSelection::versionLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		selectMode = 0;
	}
	if (hDown & KEY_RIGHT || hDown & KEY_R) {
		if (selectedVersion < 1)	selectedVersion++;
	} else if (hDown & KEY_LEFT || hDown & KEY_L) {
		if (selectedVersion > 0)	selectedVersion--;
	}

	if (hDown & KEY_TOUCH) {
		if (touching(touch, versionButtons[0])) {
			selectedVersion = 0;
			selectMode = 2;
		} else if (touching(touch, versionButtons[1])) {
			selectedVersion = 1;
			selectMode = 2;
		}
	}

	if (hDown & KEY_A) {
		selectMode = 2;
	}
}

void TitleSelection::DrawRegionSelector(void) const {
	Gui::DrawTop();
	Gui::DrawStringCentered(0, 2, 0.9f, WHITE, Lang::get("REGION_SELECT"), 400);

	if (selectedGame == 0) {
		Gui::sprite(0, sprites_Icon_idx, 37.5, 80);
	} else if (selectedGame == 1) {
		Gui::sprite(0, sprites_IconWA_idx, 37.5, 80);
	}

	if (selectedVersion == 0) {
		Gui::sprite(0, sprites_gameCard_idx, 140, 80);
	} else if (selectedVersion == 1) {
		Gui::sprite(0, sprites_sdCard_idx, 140, 80);
	}


	Gui::DrawBottom();
	
	for (int i = 0; i < 4; i++) {
		Gui::Draw_Rect(regionButtons[i].x, regionButtons[i].y, regionButtons[i].w, regionButtons[i].h, UNSELECTED_COLOR);
		if (selectedRegion == i) {
			Gui::drawAnimatedSelector(regionButtons[i].x, regionButtons[i].y, regionButtons[i].w, regionButtons[i].h, .030f, SELECTED_COLOR);
		}
	}

	Gui::DrawStringCentered(-90-30, 95, 0.8, WHITE, Lang::get("JAPAN"), 50);
	Gui::DrawStringCentered(-10-30, 95, 0.8, WHITE, Lang::get("USA"), 50);
	Gui::DrawStringCentered(70-30, 95, 0.8, WHITE, Lang::get("EUROPE"), 50);
	Gui::DrawStringCentered(150-30, 95, 0.8, WHITE, Lang::get("KOREA"), 50);

	Gui::sprite(0, sprites_japan_idx, 18, 120);
	Gui::sprite(0, sprites_usa_idx, 98, 120);
	Gui::sprite(0, sprites_europe_idx, 178, 120);
	Gui::sprite(0, sprites_korea_idx, 258, 120);
}

void TitleSelection::regionLogic(u32 hDown, u32 hHeld, touchPosition touch) {
	if (hDown & KEY_B) {
		selectMode = 1;
	}

	if (hDown & KEY_RIGHT || hDown & KEY_R) {
		if (selectedRegion < 3)	selectedRegion++;
	} else if (hDown & KEY_LEFT || hDown & KEY_L) {
		if (selectedRegion > 0)	selectedRegion--;
	}

	if (hDown & KEY_TOUCH) {
		// JPN.
		if (touching(touch, regionButtons[0])) {
			if (selectedGame == 0 && selectedVersion == 1) {
				if (GameLoader::checkTitle(oldIDs[0])) {
					isCard = false;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			} else if (selectedGame == 1 && selectedVersion == 1) {
				if (GameLoader::checkTitle(newIDs[0])) {
					isCard = false;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			} else if (selectedGame == 0 && selectedVersion == 0) {
				if (GameLoader::scanCard(oldIDs[0])) {
					isCard = true;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			} else if (selectedGame == 1 && selectedVersion == 0) {
				if (GameLoader::scanCard(newIDs[0])) {
					isCard = true;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			}
			// USA.
		} else if (touching(touch, regionButtons[1])) {
			if (selectedGame == 0 && selectedVersion == 1) {
				if (GameLoader::checkTitle(oldIDs[1])) {
					isCard = false;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			} else if (selectedGame == 1 && selectedVersion == 1) {
				if (GameLoader::checkTitle(newIDs[1])) {
					isCard = false;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			} else if (selectedGame == 0 && selectedVersion == 0) {
				if (GameLoader::scanCard(oldIDs[1])) {
					isCard = true;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			} else if (selectedGame == 1 && selectedVersion == 0) {
				if (GameLoader::scanCard(newIDs[1])) {
					isCard = true;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			}
			// EUR.
		} else if (touching(touch, regionButtons[2])) {
			if (selectedGame == 0 && selectedVersion == 1) {
				if (GameLoader::checkTitle(oldIDs[2])) {
					isCard = false;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			} else if (selectedGame == 1 && selectedVersion == 1) {
				if (GameLoader::checkTitle(newIDs[2])) {
					isCard = false;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			} else if (selectedGame == 0 && selectedVersion == 0) {
				if (GameLoader::scanCard(oldIDs[2])) {
					isCard = true;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			} else if (selectedGame == 1 && selectedVersion == 0) {
				if (GameLoader::scanCard(newIDs[2])) {
					isCard = true;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			}
			// KOR.
		} else if (touching(touch, regionButtons[3])) {
			if (selectedGame == 0 && selectedVersion == 1) {
				if (GameLoader::checkTitle(oldIDs[3])) {
					isCard = false;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			} else if (selectedGame == 1 && selectedVersion == 1) {
				if (GameLoader::checkTitle(newIDs[3])) {
					isCard = false;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			} else if (selectedGame == 0 && selectedVersion == 0) {
				if (GameLoader::scanCard(oldIDs[3])) {
					isCard = true;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			} else if (selectedGame == 1 && selectedVersion == 0) {
				if (GameLoader::scanCard(newIDs[3])) {
					isCard = true;
					Gui::setScreen(std::make_unique<MainMenu>());
				}
			}
		}
	}

	if (hDown & KEY_A) {
		// JPN.
		if (selectedGame == 0 && selectedVersion == 1 && selectedRegion == 0) {
			if (GameLoader::checkTitle(oldIDs[0])) {
				isCard = false;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (selectedGame == 1 && selectedVersion == 1 && selectedRegion == 0) {
			if (GameLoader::checkTitle(newIDs[0])) {
				isCard = false;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (selectedGame == 0 && selectedVersion == 0 && selectedRegion == 0) {
			if (GameLoader::scanCard(oldIDs[0])) {
				isCard = true;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (selectedGame == 1 && selectedVersion == 0 && selectedRegion == 0) {
			if (GameLoader::scanCard(newIDs[0])) {
				isCard = true;
				Gui::setScreen(std::make_unique<MainMenu>());
			}


		// USA.
		} else if (selectedGame == 0 && selectedVersion == 1 && selectedRegion == 1) {
			if (GameLoader::checkTitle(oldIDs[1])) {
				isCard = false;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (selectedGame == 1 && selectedVersion == 1 && selectedRegion == 1) {
			if (GameLoader::checkTitle(newIDs[1])) {
				isCard = false;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (selectedGame == 0 && selectedVersion == 0 && selectedRegion == 1) {
			if (GameLoader::scanCard(oldIDs[1])) {
				isCard = true;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (selectedGame == 1 && selectedVersion == 0 && selectedRegion == 1) {
			if (GameLoader::scanCard(newIDs[1])) {
				isCard = true;
				Gui::setScreen(std::make_unique<MainMenu>());
			}

		// EUR.
		} else if (selectedGame == 0 && selectedVersion == 1 && selectedRegion == 2) {
			if (GameLoader::checkTitle(oldIDs[2])) {
				isCard = false;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (selectedGame == 1 && selectedVersion == 1 && selectedRegion == 2) {
			if (GameLoader::checkTitle(newIDs[2])) {
				isCard = false;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (selectedGame == 0 && selectedVersion == 0 && selectedRegion == 2) {
			if (GameLoader::scanCard(oldIDs[2])) {
				isCard = true;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (selectedGame == 1 && selectedVersion == 0 && selectedRegion == 2) {
			if (GameLoader::scanCard(newIDs[2])) {
				isCard = true;
				Gui::setScreen(std::make_unique<MainMenu>());
			}

		// KOR.
		} else if (selectedGame == 0 && selectedVersion == 1 && selectedRegion == 3) {
			if (GameLoader::checkTitle(oldIDs[3])) {
				isCard = false;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (selectedGame == 1 && selectedVersion == 1 && selectedRegion == 3) {
			if (GameLoader::checkTitle(newIDs[3])) {
				isCard = false;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (selectedGame == 0 && selectedVersion == 0 && selectedRegion == 3) {
			if (GameLoader::scanCard(oldIDs[3])) {
				isCard = true;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		} else if (selectedGame == 1 && selectedVersion == 0 && selectedRegion == 3) {
			if (GameLoader::scanCard(newIDs[3])) {
				isCard = true;
				Gui::setScreen(std::make_unique<MainMenu>());
			}
		}
	}
}