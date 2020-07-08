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

#include "cardSaves.hpp"
#include "coreUtils.hpp"
#include "lang.hpp"
#include "msg.hpp"
#include "Sav.hpp"

std::shared_ptr<Sav> save;
extern bool loadedFromCart;
extern bool changes;
SaveType savesType = SaveType::UNUSED;
static std::string saveName = "";

bool CoreUtils::loadSave(const std::string saveFile) {
	save = nullptr;
	saveName = saveFile;
	FILE* in = fopen(saveName.c_str(), "rb");
	if (in) {
		fseek(in, 0, SEEK_END);
		u32 size = ftell(in);
		fseek(in, 0, SEEK_SET);
		std::shared_ptr<u8[]> saveData = std::shared_ptr<u8[]>(new u8[size]);
		fread(saveData.get(), 1, size, in);
		fclose(in);
		save = Sav::getSave(saveData, size);
		// Only allow Wild World saves.
		if (save) {
			if (save->getType() != SaveType::WW) {
				printf("SaveFile is not a Wild World save!\n");
				save = nullptr;
				return false;
			}
		}

		// Check if town exist.
		if (save) {
			if (save->town()->exist() != true) {
				printf("Town does not exist!\n");
				save = nullptr;
				return false; // Town does not exist!
			}
		}

	} else {
		printf("Could not open SaveFile.\n");
		return false;
	}

	if (!save) {
		printf("SaveFile returned nullptr.\n");
		return false;
	}
	
	savesType = save->getType();
	
	return true;
}

void CoreUtils::saveChanges() {
	if (changes) {
		if (save != nullptr) {
			save->Finish();
			FILE* out = fopen(saveName.c_str(), "rb+");
			fwrite(save->rawData().get(), 1, save->getLength(), out);
			fclose(out);
			if (loadedFromCart) {
				restoreSave();
			}
		}
	} else {
		Msg::DisplayWaitMsg(Lang::get("SAVING_USELESS"));
	}
}