#include "lang/langStrings.h"

namespace Lang {
	std::vector<std::string>
	// App strings

	// [mainMenu]
	mainMenu = {"Town Manager", "Editor", "Settings"},
	
	// [titleSelector]
	titleSelector = {"Select a Title to start.", "Cartridge", "installed Titles"},

	// [townmanager]
	townmanager = {"Launch Town", "Backup Town", "Restore Town", "Do you want to launch this Town?"},
	
	// [scan]
	scan = {"Title Found!", "Title not Found!", "Card ID matches!", "Card ID doesn't match!", "No Cartridge Inserted!"};


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