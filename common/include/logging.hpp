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

#ifndef _LEAFEDIT_LOGGING_HPP
#define _LEAFEDIT_LOGGING_HPP

#include <fstream>
#include <memory>
#include <string>

class Logging {
public:
	Logging();
	void Write(const std::string debugText = "", const bool coreLog = true);
	void Create();
	void Open();
	void Close();
private:
	const std::string logDate(void);
	const std::string format(const std::string& fmt_str, ...);
	std::ofstream logFile;
};

extern std::unique_ptr<Logging> Log;

#endif