#include "lang/langStrings.h"

namespace Lang {
	std::vector<std::string>
	// App strings

	// [editor]
	editor = {"Do you want to save your changes?", "Player", "Bank Amount: ", "Wallet Amount: ", "Town Name: ", "Player Name: ",
			  "Editor", "Select a save for the Editor.", "Do you want to load this save?", "Do you want to return to the Main Menu?"},
	// [mainMenu]
	mainMenu = {"Town Manager", "Editor", "Settings"},

	// [messages]
	messages = {"Creating Directory Failed!", "Copying Files Failed!", "Backup success!", "Failed to commit Save Data!",
				"Failed to fix secure value!", "Restore success!", "Failed to do App Jump!", "Do you want to Cancel?"},

	// [messages2]
	messages2 = {"Press A to continue.", "Press A to confirm or B to Cancel.", "Do you want to Backup this save?",
				 "Do you want to restore this Save?", "Do you want to launch this Town?", 
				 "Do you want to start the game normally?", "Checking for Old AC:NL Updates...", 
				 "Backing up files, please wait...", "Restoring files, please wait...", "Press Select to Refresh the FileBrowse.",
				 "Would you like to backup the current town\nbefore restoring the selected one?", "A save with this name already exists.\nWould you like to overwrite the save?"},
	
	// [playerEditor]
	playerEditor = {"Please type in the new player name."},

	// [settings]
	settings = {"Language", "Selector", "?"},
	
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