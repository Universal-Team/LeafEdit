#include "colors.hpp"
#include <nds.h>

#include "config.hpp"
#include "graphics.hpp"
#include "json.hpp"
#include "tonccpy.h"

const u16 defaultPalette[] = {
	0x0000, 0xFBDE, 0xE739, 0xCA52, 0xBDEF, 0x98C6, 0x94A5, 0x8842, 0x8000, // Grays
	0x801E, 0x800F, 0xF800, 0xBC00, 0x8260, 0x8320, 0x83E0, // Colors
	0x0000, 0xFBDE, 0xBDEF, 0x0000, // WHITE_TEXT
	0x0000, 0x8C63, 0xCA52, 0x0000, // GRAY_TEXT
	0x0000, 0x801E, 0x800F, 0x0000, // RED_TEXT
	0x0000, 0xF800, 0xBC00, 0x0000, // BLUE_TEXT
};


std::array<u16, 0x1B> itemPalette = {
	0x0000, // Empty 0xE0
	0xD294, // Occupied 0xE1
	0xE21D, // Flower 0xE2
	0x9A46, // Weed 0xE3
	0xA1B4, // Tree 0xE4
	0x9A16, // Parched Flower 0xE5
	0xDAC6, // Watered Flower 0xE6
	0xFEB5, // Pattern 0xE7
	0x98C6, // Rock 0xE8
	0xAD76, // Money Rock 0xE9
	0xF3B6, // Paper 0xEA
	0xB195, // Wallpaper / Carpet 0xEB
	0xDA2A, // Clothes 0xEC
	0xB398, // Catchable 0xED
	0xE3B6, // Song 0xEE
	0xCE73, // Tool 0xEF
	0x9BFF, // Money 0xF0
	0x9ADF, // Item 0xF1
	0xD6F8, // Turnip 0xF2
	0xAD8E, // Fossil 0xF3
	0xEB3F, // Shell 0xF4
	0xAF6C, // Furniture 0xF5
	0xAA7B, // Gyroid 0xF6
	0xB9CE, // Building 0xF7
	0xAE2E, // Trash 0xF8
	0xEEDB, // Fruit 0xF9
	0x98DF // Invalid 0xFA
};

const std::string keys[] = {
	"CLEAR", "WHITE", "LIGHT_GRAY", "GRAY", "DARKISH_GRAY", "DARK_GRAY", "DARKER_GRAY", "DARKERER_GRAY", "BLACK",
	"RED", "DARK_RED", "BLUE", "DARK_BLUE",
	"WHITE_TEXT_1", "WHITE_TEXT_2", "WHITE_TEXT_3", "WHITE_TEXT_4"
	"GRAY_TEXT_1",  "GRAY_TEXT_2",  "GRAY_TEXT_3",  "GRAY_TEXT_4"
	"RED_TEXT_1",   "RED_TEXT_2",   "RED_TEXT_3",   "RED_TEXT_4"
	"BLUE_TEXT_1",  "BLUE_TEXT_2",  "BLUE_TEXT_3",  "BLUE_TEXT_4"
};

void Colors::load(void) {
	nlohmann::json themeJson;

	FILE* file = fopen((Config::getString("themeDir")+"/theme.json").c_str(), "r");
	if(file) {
		themeJson = nlohmann::json::parse(file, nullptr, false);
		fclose(file);
	}

	u16 palette[sizeof(defaultPalette)];
	tonccpy(palette, defaultPalette, sizeof(palette));

	for(int i=0;i<16;i++) {
		if(themeJson.contains(keys[i]) && themeJson[keys[i]].is_string()) {
			palette[i] = stoi(themeJson[keys[i]].get_ref<std::string&>(), nullptr, 0);
		}
	}

	tonccpy(BG_PALETTE, palette, sizeof(palette));
	tonccpy(BG_PALETTE_SUB, palette, sizeof(palette));
	tonccpy(BG_PALETTE + 0xE0, itemPalette.data(), itemPalette.size());
	tonccpy(BG_PALETTE_SUB + 0xE0, itemPalette.data(), itemPalette.size());
}
