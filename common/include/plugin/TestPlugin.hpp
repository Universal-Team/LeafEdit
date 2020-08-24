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

#ifndef LEAFEDIT_TEST_PLUGIN_HPP
#define LEAFEDIT_TEST_PLUGIN_HPP

#include "Plugin.hpp"
#include "types.hpp"

#include <string>
#include <vector>

/* This is used for Tests. */
class TestPlugin : public Plugin {
public:
	/* Constructor and Destructor. DO NOT CHANGE THIS! */
	TestPlugin() : Plugin() { }
	virtual ~TestPlugin() { }
	/* Main Plugin function which get's called. */
	int scriptMain() override;
private:
	/* All Script Entries are listed at this vector. */
	const std::vector<std::string> scriptEntries = {""};
	
	/* All Functions are here. All Functions *must* have an integer as a return value. */
};

#endif