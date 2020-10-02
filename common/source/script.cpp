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

#include "encryptedInt32.hpp"
#include "lang.hpp"
#include "msg.hpp"
#include "Sav.hpp"
#include "saveUtils.hpp"
#include "script.hpp"
#include <unistd.h>

/*
	Prototype Functions.
*/

/*
	Write an u8 to the savefile.

	"type": "WriteU8",
	"offset": "0x220",
	"data": "1"
*/

/*
	Write an u8 value multiple times.

	"type": "ForWrite",
	"offset": "0x220",
	"length": "0x10",
	"data": "1"
*/

/*
	Write an u16 to the savefile.

	"type": "WriteU16",
	"offset": "0x220",
	"data": "1"
*/


/*
	Write an u32 to the savefile.

	"type": "WriteU32",
	"offset": "0x220",
	"data": "1"
*/

/*
	Write an EncryptedInt32 to the savefile. (AC:NL | AC:WA only).

	"type": "WriteEncryptedInt32",
	"offset": "0x220",
	"data": "0x00000000"
*/

/*
	Write a bit.

	"type": "WriteBit",
	"offset": "0x220",
	"bitIndex": 1,
	"data": 1
*/

/*
	Dump a specific location and size from the save to a file.

	"type": "DumpFile",
	"offset": "0x220",
	"length": "0x10",
	"file": "sdmc:/3ds/LeafEdit/Test.dat"
*/

/*
	Inject a file to save.

	"type": "InjectFile",
	"offset": "0x220",
	"file": "sdmc:/3ds/LeafEdit/Test.dat"
*/

Script::Script(std::string fileName) {
	if (access(fileName.c_str(), F_OK) != 0) {
		this->valid = false;
		return;
	}

	FILE* file = fopen(fileName.c_str(), "r");
	this->scriptJson = nlohmann::json::parse(file, nullptr, false);
	fclose(file);

	/* Check version. */
	if (this->scriptJson["scriptInfo"].contains("version")) {
		if (this->scriptVersion == this->scriptJson["scriptInfo"]["version"]) {
			this->valid = true;
			this->ReadMain();
		}
	}
}

/* Read Main stuff. */
void Script::ReadMain() {
	if (this->valid) {
		if (this->scriptJson["scriptInfo"].contains("author")) {
			this->author = this->scriptJson["scriptInfo"]["author"];
		} else {
			this->author = "?";
		}

		if (this->scriptJson["scriptInfo"].contains("title")) {
			this->scriptName = this->scriptJson["scriptInfo"]["title"];
		} else {
			this->scriptName = "?";
		}

		if (this->scriptJson["scriptInfo"].contains("description")) {
			this->description = this->scriptJson["scriptInfo"]["description"];
		} else {
			this->description = "?";
		}
	}
}

void Script::DumpToFile(u32 offset, u32 length, const std::string &fileName) {
	if (offset + length < save->getLength() && save) {
		FILE *fl = fopen(fileName.c_str(), "w");
		u8 *fileData = new u8[length];

		for (u32 i = 0; i < length; i++) {
			fileData[i] = save->savePointer()[offset + i];
		}

		fwrite(fileData, 1, length, fl);
		fclose(fl);

		delete[] fileData;
	}
}

void Script::InjectToFile(u32 offset, const std::string &fileName) {
	FILE* fl = fopen(fileName.c_str(), "rb");

	if (fl && save) {
		fseek(fl, 0, SEEK_END);
		u32 size = ftell(fl);

		if (offset + size < save->getLength()) {
			fseek(fl, 0, SEEK_SET);

			/* Create Buffer with the size and read the file. */
			u8 *fileData = new u8[size];
			fread(fileData, 1, size, fl);

			/* Set Buffer data to save. */
			for(u32 i = 0; i < size; i++) {
				this->Write<u8>(offset + i, fileData[i]);
			}

			/* Free Buffer. */
			delete[] fileData;
		}

		/* Close File, cause we don't need it. */
		fclose(fl);
	}
}

/* Write an for loop. */
void Script::WriteFor(u32 offset, u32 length, u8 data) {
	if (save) {
		for (u32 i = offset; i < offset + length; i++) {
			save->savePointer()[i] = data;
		}

		save->changesMade(true);
	}
}

/* Write an EncryptedInt32. */
void Script::DoEI32(u32 offset, u32 value) {
	if (save) {
		if (save->getType() == SaveType::NL || save->getType() == SaveType::WA || save->getType() == SaveType::HHD) {
			u32 encryptedInt = 0;
			u32 encryptionData = 0;

			EncryptedInt32 EI32 = EncryptedInt32(SaveUtils::Read<u64>(save->savePointer(), offset));

			EI32.value = value;
			/* Writing. */
			EI32.encrypt(encryptedInt, encryptionData);
			SaveUtils::Write<u32>(save->savePointer(), offset, encryptedInt);
			SaveUtils::Write<u32>(save->savePointer(), offset + 0x04, encryptionData);
		}
	}
}

/* Check, if game is supported. */
bool Script::gameSupported(int entry) const {
	if (entry > this->getEntrySize()) return false;

	if (save) {
		const std::vector<std::string> games = this->getSupportedSavesEntry(entry);
		bool canContinue = false;

		switch(save->getType()) {
			case SaveType::WW:
				for (int i = 0; i < (int)games.size(); i++) {
					if (games[i] == "WW") {
						canContinue = true;
						break;
					}
				}

				if (canContinue) {
					const std::vector<std::string> regions = this->getSupportedRegionsEntry(entry);

					switch(save->getRegion()) {
						case WWRegion::USA_REV0:
						case WWRegion::USA_REV1:
						case WWRegion::EUR_REV1:
							for (int i = 0; i < (int)regions.size(); i++) {
								if (regions[i] == "EUR" || regions[i] == "USA") return true;
							}
							break;

						case WWRegion::JPN_REV0:
						case WWRegion::JPN_REV1:
							for (int i = 0; i < (int)regions.size(); i++) {
								if (regions[i] == "JPN") return true;
							}
							break;

						case WWRegion::KOR_REV1:
							for (int i = 0; i < (int)regions.size(); i++) {
								if (regions[i] == "KOR") return true;
							}
							break;

						case WWRegion::UNKNOWN:
							return false;
							break;
					}
				}
				break;

			case SaveType::NL:
				for (int i = 0; i < (int)games.size(); i++) {
					if (games[i] == "NL") return true;
				}
				break;

			case SaveType::WA:
				for (int i = 0; i < (int)games.size(); i++) {
					if (games[i] == "WA") return true;
				}
				break;

			case SaveType::HHD:
				for (int i = 0; i < (int)games.size(); i++) {
					if (games[i] == "HHD") return true;
				}
				break;

			case SaveType::UNUSED:
				return false;
				break;
		}
	}

	return false;
}

/* Get Entry Title. */
std::string Script::getEntryTitle(int entry) {
	if (!this->valid) return "?";

	if (entry > this->getEntrySize()) return "?"; // Out of bounds access.

	if (this->scriptJson["scriptContent"][entry]["info"].contains("title")) {
		return this->scriptJson["scriptContent"][entry]["info"]["title"];
	}

	return "?";
}

/* Get Entry Description. */
std::string Script::getEntryDesc(int entry) {
	if (!this->valid) return "?";

	if (entry > this->getEntrySize()) return "?"; // Out of bounds access.

	if (this->scriptJson["scriptContent"][entry]["info"].contains("description")) {
		return this->scriptJson["scriptContent"][entry]["info"]["description"];
	}

	return "?";
}

/* Execute script entry. */
ScriptError Script::execute(int entry) {
	if (!this->valid) return ScriptError::Invalid;
	if (entry > this->getEntrySize()) return ScriptError::OutOfBounds; // Out of bounds access.

	ScriptError ret = ScriptError::None; // No Error has been occured now.


	for(int i = 0; i < (int)this->scriptJson.at("scriptContent").at(entry).at("script").size(); i++) {
		if (ret == ScriptError::None) {
			std::string type = this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("type");

			/* Write an uint8_t. */
			if (type == "WriteU8") {
				u32 offset = 0; u8 data = 0; std::string offsetString = "0x0", dataString = "0x0";

				/* Get offset from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("offset") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("offset").is_string()) {
					offsetString = this->scriptJson["scriptContent"][entry]["script"][i]["offset"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				offset = (u32)std::stoi(offsetString, 0, 16);

				/* Get data from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("data") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("data").is_string()) {
					dataString = this->scriptJson["scriptContent"][entry]["script"][i]["data"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				data = (u8)std::stoi(dataString, 0, 16);

				if (save) {
					save->savePointer()[offset] = data; // Do not cast for an u8, because we don't need that.
					save->changesMade(true);
				}


				/* Write an uint16_t. */
			} else if (type == "WriteU16") {
				u32 offset = 0; u16 data = 0; std::string offsetString = "0x0", dataString = "0x0";

				/* Get offset from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("offset") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("offset").is_string()) {
					offsetString = this->scriptJson["scriptContent"][entry]["script"][i]["offset"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				offset = (u32)std::stoi(offsetString, 0, 16);

				/* Get data from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("data") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("data").is_string()) {
					dataString = this->scriptJson["scriptContent"][entry]["script"][i]["data"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				data = (u16)std::stoi(dataString, 0, 16);

				Script::Write<u16>(offset, data);


				/* Write an uint32_t. */
			} else if (type == "WriteU32") {
				u32 offset = 0, data = 0; std::string offsetString = "0x0", dataString = "0x0";

				/* Get offset from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("offset") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("offset").is_string()) {
					offsetString = this->scriptJson["scriptContent"][entry]["script"][i]["offset"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				offset = (u32)std::stoi(offsetString, 0, 16);

				/* Get data from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("data") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("data").is_string()) {
					dataString = this->scriptJson["scriptContent"][entry]["script"][i]["data"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				data = (u32)std::stoi(dataString, 0, 16);

				Script::Write<u32>(offset, data);


				/* Write an uint8_t in a for loop for a specific amount. */
			} else if (type == "ForWrite") {
				u32 offset = 0, length; u8 data = 0; std::string offsetString = "0x0", lengthString = "0x0", dataString = "0x0";

				/* Get offset from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("offset") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("offset").is_string()) {
					offsetString = this->scriptJson["scriptContent"][entry]["script"][i]["offset"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				offset = (u32)std::stoi(offsetString, 0, 16);

				/* Get Length from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("length") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("length").is_string()) {
					lengthString = this->scriptJson["scriptContent"][entry]["script"][i]["length"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				length = (u32)std::stoi(lengthString, 0, 16);

				/* Get data from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("data") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("data").is_string()) {
					dataString = this->scriptJson["scriptContent"][entry]["script"][i]["data"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				data = (u8)std::stoi(dataString, 0, 16);

				Script::WriteFor(offset, length, data);


				/* Write an EncryptedInt32. */
			} else if (type == "WriteEncryptedInt32") {
				u32 offset = 0, data = 0; std::string offsetString = "", dataString = "";

				/* Get offset from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("offset") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("offset").is_string()) {
					offsetString = this->scriptJson["scriptContent"][entry]["script"][i]["offset"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				offset = (u32)std::stoi(offsetString, 0, 16);

				/* Get data from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("data") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("data").is_string()) {
					dataString = this->scriptJson["scriptContent"][entry]["script"][i]["data"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				data = (u32)std::stoi(dataString, 0, 16);

				Script::DoEI32(offset, data);


				/* Write a bit index. */
			} else if (type == "WriteBit") {
				u32 offset = 0; u8 data = 0, bitIndex = 0; std::string offsetString = "0x0";

				/* Get offset from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("offset") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("offset").is_string()) {
					offsetString = this->scriptJson["scriptContent"][entry]["script"][i]["offset"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				offset = (u32)std::stoi(offsetString, 0, 16);

				/* Get data from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("data") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("data").is_number()) {
					data = this->scriptJson["scriptContent"][entry]["script"][i]["data"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				/* Get bit index from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("bitIndex") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("bitIndex").is_number()) {
					bitIndex = this->scriptJson["scriptContent"][entry]["script"][i]["bitIndex"];
				} else {
					ret = ScriptError::Syntax;
					break;
				}

				if (save) SaveUtils::SetBit(save->savePointer(), offset, bitIndex, data);


				/* Dump a specific location of the savefile to a file. */
			} else if (type == "DumpFile") {
				u32 offset = 0, length = 0x0; std::string offsetString = "0x0", lengthString = "0x0", fileName = "";

				/* Get offset from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("offset") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("offset").is_string()) {
					offsetString = this->scriptJson["scriptContent"][entry]["script"][i]["offset"];

				} else {
					ret = ScriptError::Syntax;
					break;
				}

				offset = (u32)std::stoi(offsetString, 0, 16);

				/* Get length from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("length") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("length").is_string()) {
					lengthString = this->scriptJson["scriptContent"][entry]["script"][i]["length"];

				} else {
					ret = ScriptError::Syntax;
					break;
				}

				length = (u32)std::stoi(lengthString, 0, 16);

				/* Get filename from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("file") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("file").is_string()) {
					fileName = this->scriptJson["scriptContent"][entry]["script"][i]["file"];

				} else {
					ret = ScriptError::Syntax;
					break;
				}

				if (save) this->DumpToFile(offset, length, fileName);


				/* Inject a file's data to the savefile. */
			} else if (type == "InjectFile") {
				u32 offset = 0; std::string offsetString = "0x0", fileName = "";

				/* Get offset from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("offset") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("offset").is_string()) {
					offsetString = this->scriptJson["scriptContent"][entry]["script"][i]["offset"];

				} else {
					ret = ScriptError::Syntax;
					break;
				}

				offset = (u32)std::stoi(offsetString, 0, 16);

				/* Get filename from json. */
				if (this->scriptJson.at("scriptContent").at(entry).at("script").at(i).contains("file") && this->scriptJson.at("scriptContent").at(entry).at("script").at(i).at("file").is_string()) {
					fileName = this->scriptJson["scriptContent"][entry]["script"][i]["file"];

				} else {
					ret = ScriptError::Syntax;
					break;
				}

				if (save) this->InjectToFile(offset, fileName);
			}
		}
	}

	/* Return the results here. */
	if (ret == ScriptError::Syntax) Msg::DisplayWaitMsg(Lang::get("SYNTAX_ERROR"));
	else if (ret == ScriptError::OutOfBounds) Msg::DisplayWaitMsg(Lang::get("OUT_OF_BOUNDS_ERROR"));
	else if (ret == ScriptError::Invalid) Msg::DisplayWaitMsg(Lang::get("INVALID_SCRIPT_ERROR"));
	else if (ret == ScriptError::None) Msg::DisplayWaitMsg(Lang::get("NO_ERROR_SCRIPT"));

	return ret;
}