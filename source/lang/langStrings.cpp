#include "lang/langStrings.h"

namespace Lang {
	std::vector<std::string>
	// App strings

	// [mainMenu]
	mainMenu = {"Town Manager", "Editor", "Settings"},

	// [messages]
	messages = {"Creating Directory Failed!", "Copying Files Failed!", "Backup success!", "Failed to commit Save Data!",
				"Failed to fix secure value!", "Restore success!", "Failed to do App Jump!", "Do you want to Cancel?"},

	// [messages2]
	messages2 = {"Press A to continue.", "Press A to confirm or B to Cancel.", "Do you want to Backup this save?",
				 "Do you want to restore this Save?", "Do you want to launch this Town?", 
				 "Do you want to start the game normally?", "Checking for Old AC:NL Updates...", 
				 "Backing up files, please wait...", "Restoring files, please wait...", "Press Select to Refresh the FileBrowse."},
	
	// [titleSelector]
	titleSelector = {"Select a Title to start.", "Cartridge", "installed Titles"},

	// [townmanager]
	townmanager = {"Launch Town", "Backup Town", "Restore Town", "Select a Town to restore.", "Select a Town to Launch."},

	// [update]
	update = {"Update Found!", "Update Not Found!"};


	std::string
	// Single Strings.

	// [keyboard]
	typeName = "Please Type in your Save Name.",

	// [languages]
	language = "Choose the current Language.",

	// [misc]
	notImplemented = "This is not Implemented yet!",

	// [title]
	title = "LeafEdit";
}