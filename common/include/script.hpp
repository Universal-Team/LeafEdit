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

#ifndef _LEAFEDIT_SCRIPT_HPP
#define _LEAFEDIT_SCRIPT_HPP

enum class ScriptError {
	None,
	Syntax,
	Invalid,
	OutOfBounds
};

#include "json.hpp"
#include "saveUtils.hpp"
#include <string>

#define MAIN_SCRIPT_PATH "sdmc:/3ds/LeafEdit/scripts/"

class Script {
public:
	Script(std::string fileName = MAIN_SCRIPT_PATH "Test.json");
	ScriptError execute(int entry);
	void ReadMain();
	std::string getEntryTitle(int entry);
	std::string getEntryDesc(int entry);
	bool getValid() { return this->valid; }

	std::string getAuthor() const { return this->author; }
	std::string getScriptName() const { return this->scriptName; }
	std::string getDescription() const { return this->description; }

	bool entryExist(int entry) const {
		if (!this->valid) return false;

		if (entry <= this->getEntrySize()) return true;
		return false;
	}

	int getEntrySize() const {
		if (!this->valid) return 0;

		if (this->scriptJson.contains("scriptContent")) {
			return (int)this->scriptJson["scriptContent"].size();

		} else {
			return 0;
		}
	}

	bool gameSupported(int entry) const;

	std::vector<std::string> getSupportedSavesEntry(int entry) const {
		if (!this->valid) return {""};
		if (entry > this->getEntrySize()) return {""};

		if (this->scriptJson["scriptContent"][entry]["info"].contains("savetypes")) {
			return this->scriptJson["scriptContent"][entry]["info"]["savetypes"];

		} else {
			return {""};
		}
	}

	std::vector<std::string> getSupportedRegionsEntry(int entry) const {
		if (!this->valid) return {""};
		if (entry > this->getEntrySize()) return {""};

		if (this->scriptJson["scriptContent"][entry]["info"].contains("regions")) {
			return this->scriptJson["scriptContent"][entry]["info"]["regions"];

		} else {
			return {""};
		}
	}

private:
	/* Writing. */
	template <typename T>
	void Write(u32 offset, T data) {
		if (save) {
			*reinterpret_cast<T*>(save->savePointer() + offset) = data;
			save->changesMade(true);
		}
	}

	/* Write a for loop. */
	void WriteFor(u32 offset, u32 length, u8 data);
	void DoEI32(u32 offset, u32 value);

	void DumpToFile(u32 offset, u32 length, const std::string &fileName);
	void InjectToFile(u32 offset, const std::string &fileName);


	/* Some main script informations. */
	bool valid = false;
	nlohmann::json scriptJson;
	std::string author = "?";
	std::string scriptName = "?";
	std::string description = "?";
	int scriptVersion = 1;
};

#endif