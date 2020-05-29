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

#include "editor.hpp"
#include "fileBrowse.hpp"
#include "init.hpp"
#include "itemUtils.hpp"
#include "lang.hpp"

#include "playerSelector.hpp"

#include "pluginScreen.hpp"
#include "saveUtils.hpp"
#include "Sav.hpp"
#include "screenCommon.hpp"

#include "townMapEditorNL.hpp"
#include "townMapEditorWW.hpp"

#include "villagerViewerNL.hpp"
#include "villagerViewerWW.hpp"

extern bool touching(touchPosition touch, ButtonType button);
extern bool iconTouch(touchPosition touch, Structs::ButtonPos button);

std::shared_ptr<Sav> save;
std::unique_ptr<Town> town;
static std::string SaveFile;
// Bring that to other screens too.
SaveType savesType = SaveType::UNUSED;
bool changes = false;

// Japanese | PAL.
const std::vector<std::string> titleNames = {
	"おいでよ どうぶつの森",
	"Wild World",
	"とびだせ どうぶつの森",
	"New Leaf",
	"とびだせ どうぶつの森 amiibo+",
	"Welcome amiibo",
};

bool Editor::loadSave() {
	save = nullptr;
	FILE* in = fopen(saveName.c_str(), "rb");
	if(in) {
		fseek(in, 0, SEEK_END);
		u32 size = ftell(in);
		fseek(in, 0, SEEK_SET);
		std::shared_ptr<u8[]> saveData = std::shared_ptr<u8[]>(new u8[size]);
		fread(saveData.get(), 1, size, in);
		fclose(in);
		save = Sav::getSave(saveData, size);

		// Check if town exist on AC:WW.
		if (save) {
			if (save->getType() == SaveType::WW) {
				if (save->town()->exist() != true) {
					return false; // Town does not exist!
				}
			}
		}

	} else {
		printf("Could not open SaveFile.\n");
		return false;
	}

	if(!save) {
		printf("SaveFile returned nullptr.\n");
		return false;
	}
	
	if (save->getType() == SaveType::WW)		saveT = 0;
	else if (save->getType() == SaveType::NL)	saveT = 2;
	else if (save->getType() == SaveType::WA)	saveT = 4;

	savesType = save->getType();

	// Now handle the region struct thing.
	if (savesType == SaveType::WA) {
		this->RegionLock.RawByte = save->savePointer()[0x621CE];
		this->RegionLock.DerivedID = this->RegionLock.RawByte & 0xF;
		this->RegionLock.RegionID = static_cast<CFG_Region>(this->RegionLock.RawByte >> 4);
	}
	
	return true;
}

void Editor::SaveInitialize() {
	saveName = SaveBrowse::searchForSave({"sav", "dat"}, "sdmc:/3ds/LeafEdit/Towns/", "Select your SaveFile.");
	// If User canceled, go screen back.
	if (saveName == "") {
		Gui::screenBack(true);
	}

	if (!loadSave()) {
		Msg::DisplayWarnMsg("Invalid SaveFile!");
	} else {
		Msg::DisplayWarnMsg("Loading Editor... Please wait.");
		if (Init::loadSheets() == 0) {
			ItemUtils::LoadDatabase(savesType);
			Lang::loadGameStrings(1, savesType);
			loadState = SaveState::Loaded;
		} else {
			Msg::DisplayWarnMsg("Failed to load SpriteSheets...");
			Gui::screenBack(true);
		}
	}
}


void Editor::Draw(void) const {
	if (loadState == SaveState::Loaded) {
		GFX::DrawTop();
		Gui::DrawStringCentered(0, -2 + barOffset, 0.9f, WHITE, "LeafEdit - Editor", 395, 0, font);
		if (saveT != -1) {
			Gui::DrawStringCentered(0, 60, 0.9f, WHITE, "SaveType: " + titleNames[saveT+1], 400, 0, font); // +1 for PAL names.
			std::string length = "SaveSize: " + std::to_string(save->getLength()) + " Byte | " + std::to_string(save->getLength() / 1024) + " KB.";
			Gui::DrawStringCentered(0, 100, 0.9f, WHITE, length, 400, 0, font);
		}
		if (fadealpha > 0) Gui::Draw_Rect(0, 0, 400, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
		GFX::DrawBottom();
		for (int i = 0; i < 3; i++) {
			GFX::DrawButton(mainButtons[i]);
			if (i == Selection)	GFX::DrawGUI(gui_pointer_idx, mainButtons[i].x+100, mainButtons[i].y+30);
		}
		GFX::DrawGUI(gui_back_idx, icons[0].x, icons[0].y);
		GFX::DrawGUI(gui_save_idx, icons[1].x, icons[1].y);
	}
	if (fadealpha > 0) Gui::Draw_Rect(0, 0, 320, 240, C2D_Color32(fadecolor, fadecolor, fadecolor, fadealpha));
}

void Editor::Saving() {
	if (!changes) {
		Msg::DisplayWaitMsg("Saving is useless. No changes have been made.");
		return;
	}

	if (Msg::promptMsg("Do you like to save?")) {
		// Handle AC:WA stuff here.
		if (savesType == SaveType::WA) {
			CoreUtils::FixInvalidBuildings();
		}
		save->Finish();
		FILE* out = fopen(saveName.c_str(), "rb+");
		fwrite(save->rawData().get(), 1, save->getLength(), out);
		fclose(out);
		hasSaved = true;
	}
}

void Editor::Logic(u32 hDown, u32 hHeld, touchPosition touch) {

	if (loadState == SaveState::Loaded) {
		if (hDown & KEY_TOUCH) {
			if (iconTouch(touch, icons[0])) {
				if (changes == true && hasSaved == false) {
					if (Msg::promptMsg("You have unsaved changes. Do you like to exit without saving?")) {
						savesType = SaveType::UNUSED;
						Gui::screenBack(true);
					}
				} else if ((changes == true && hasSaved == true) || (!changes)) {
					savesType = SaveType::UNUSED;
					Gui::screenBack(true);
				}
			} else if (iconTouch(touch, icons[1])) {
				Saving();
			}
		}
		// Navigation.
		if(hDown & KEY_UP) {
			if(Selection > 0)	Selection --;
		} else if(hDown & KEY_DOWN) {
			if(Selection < 2)	Selection++;
		}

		if (hDown & KEY_A) {
			if (savesType != SaveType::UNUSED) {
				// Player Editor.
				if (Selection == 0) {
					if (save->player(0) != nullptr) {
						Gui::setScreen(std::make_unique<PlayerSelector>(), true, true);
					} else {
						Msg::NotImplementedYet();
					}
					// Villager Viewer.
				} else if (Selection == 1) {
					if (save->villager(0) != nullptr) {
						if (savesType == SaveType::WW) {
							Gui::setScreen(std::make_unique<VillagerViewerWW>(), true, true);
						} else if (savesType == SaveType::NL || savesType == SaveType::WA) {
							Gui::setScreen(std::make_unique<VillagerViewerNL>(), true, true);
						}
					} else {
						Msg::NotImplementedYet();
					}
					// Town Map Editor.
				} else if (Selection == 2) {
					if (save->town()->acre(0) != nullptr && save->town()->item(0) != nullptr) {
						if (savesType == SaveType::WW) {
							Gui::setScreen(std::make_unique<TownMapEditorWW>(), true, true);
						} else if (savesType == SaveType::NL || savesType == SaveType::WA) {
							Gui::setScreen(std::make_unique<TownMapEditorNL>(), true, true);
						}
					} else {
						Msg::NotImplementedYet();
					}
				}
			}
		}

		if (hDown & KEY_X) {
			Gui::setScreen(std::make_unique<PluginScreen>(), true, true);
		}
		
		if (hDown & KEY_B) {
			savesType = SaveType::UNUSED;
			Gui::screenBack(true);
		}
	} else {
		SaveInitialize(); // Display Browse.
	}
}