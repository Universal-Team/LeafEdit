#include <fat.h>

#include "colors.hpp"
#include "config.hpp"
#include "fileBrowse.hpp"
#include "flashcard.hpp"
#include "graphics.hpp"
#include "lang.hpp"
#include "lang.hpp"
#include "nitrofs.h"
#include "sound.hpp"

int main(int argc, char **argv) {
	initGraphics();
	keysSetRepeat(25,5);
	sysSetCardOwner(BUS_OWNER_ARM9); // Give ARM9 access to Slot-1 (for dumping/injecting saves)
	defaultExceptionHandler();
	scanKeys(); // So it doesn't open the SD if A is held

	drawRectangle(0, 0, 256, 192, DARKERER_GRAY, DARKER_GRAY, true, false);
	drawRectangle(0, 0, 256, 192, DARKERER_GRAY, DARKER_GRAY, false, false);

	// Init filesystem
	if(!fatInitDefault()) {
		// Prints error if fatInitDefault() fails
		consoleDemoInit();
		printf("fatInitDefault() failed...");
		while(1) swiWaitForVBlank();
	}

	// Make directories
	mkdir(sdFound() ? "sd:/_nds" : "fat:/_nds", 0777);
	mkdir(sdFound() ? "sd:/_nds/LeafEdit" : "fat:/_nds/LeafEdit", 0777);
	mkdir(sdFound() ? "sd:/_nds/LeafEdit/backups" : "fat:/_nds/LeafEdit/backups", 0777);

	// Try to init NitroFS from argv provided to the game when it was launched
	if(!nitroFSInit(argv[0])) {
		// If that fails, try to init NitroFS on 'LeafEdit.nds'
		if(!nitroFSInit("LeafEdit.nds")) {
			if(!nitroFSInit("/_nds/LeafEdit/LeafEdit.nds")) {
				// Prints error if nitroFSInit() fails
				consoleDemoInit();
				printf("nitroFSInit() failed...\n\n");
				printf("Please copy LeafEdit.nds to:\n\n");
				printf("%s:/_nds/LeafEdit/\n", sdFound() ? (access("/Nintendo 3DS", F_OK) == 0 ? "sdmc" : "sd") : "fat");
				printf("                  LeafEdit.nds\n\n");
				printf("or launch LeafEdit using\n\n");
				printf("      TWiLight Menu++ or HBMenu\n\n\n\n\n");
				printf("(Note: TWiLight's Acekard\n");
				printf("        theme needs a copy in ^)\n\n");
				while(1) swiWaitForVBlank();
			}
		}
	}

	Config::load();
	Colors::load();
	loadFont();
	Lang::load(Config::getLang("lang"));
	printTextCentered(Lang::get("loading"), 0, 32, false, true);

	Sound::init();
	// initSprites();
	// loadGraphics();

	while(1) {
		browseForSave();
		// check if invalid {
		// 	drawRectangle(20, 20, 216, 152, DARK_RED, true, true);
		// 	printTextCentered(Lang::get("invalidSave"), 0, 24, true, true);
		// 	for(int i=0;i<120;i++)	swiWaitForVBlank();
		// 	drawRectangle(20, 20, 216, 152, CLEAR, true, true);
		// 	continue;
		// }

		// Enter main management or so
	}

	return 0;
}