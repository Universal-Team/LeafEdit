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

#include "logging.hpp"

#include <stdarg.h>
#include <time.h>
#include <unistd.h>

std::unique_ptr<Logging> Log;

#define LOGGING_FILE "sdmc:/3ds/LeafEdit/LeafEdit.log"

#define DO_LOG false // Do this to true, if Logging should be used.

/* If do Logging, open. */
Logging::Logging() {
	if (DO_LOG) this->Open();
}

/* Create the log file. */
void Logging::Create(void) {
	FILE* logFile = fopen((LOGGING_FILE), "w");
	fclose(logFile);
}

/* Open the log file. */
void Logging::Open() {
	if ((access(LOGGING_FILE, F_OK) != 0)) {
		this->Create();
	}

	this->logFile.open((LOGGING_FILE), std::ofstream::app);
}

/* Close the log file. */
void Logging::Close() {
	if (DO_LOG) this->logFile.close();
}

/* Write a debug Message to the log. */
void Logging::Write(const std::string debugText, const bool coreLog) {
	if (DO_LOG) {
		/* Format: [CORE / APP] [<Date And Time>] DebugMessage. */
		const std::string logWrite = coreLog ? "[CORE] [" + logDate() + "]: " + debugText : "[APP] [" + logDate() + "]: " + debugText;
		this->logFile << logWrite << std::endl;
	}
}

/* For formatting the Date and Time. */
const std::string Logging::format(const std::string& fmt_str, ...) {
	va_list ap;
	char* fp = NULL;
	va_start(ap, fmt_str);
	vasprintf(&fp, fmt_str.c_str(), ap);
	va_end(ap);
	std::unique_ptr<char, decltype(free)*> formatted(fp, free);
	return std::string(formatted.get());
}

/* Log current Time and Date. */
const std::string Logging::logDate(void) {
	time_t unixTime;
	struct tm timeStruct;
	time(&unixTime);
	localtime_r(&unixTime, &timeStruct);
	return format("%04i-%02i-%02i %02i:%02i:%02i", timeStruct.tm_year + 1900, timeStruct.tm_mon + 1, timeStruct.tm_mday, timeStruct.tm_hour, timeStruct.tm_min, timeStruct.tm_sec);
}