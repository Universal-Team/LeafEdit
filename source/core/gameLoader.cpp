/*
*   This file is part of LeafEdit
*   Copyright (C) 2019 VoltZ, Epicpkmn11, Flame, RocketRobz, TotallyNotGuy
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

#include <3ds.h>
#include "core/gameLoader.hpp"
#include "gui.hpp"
#include "settings.hpp"
#include <array>

// Check, if update for the old AC:NL version is found.
bool updateFound;

static constexpr std::array<unsigned long long, 6> titleIds = {

    // old version.
    0x0004000000086300, // USA.
    0x0004000000086400, // EUR.
    0x0004000000086200, // JPN.

    // Welcome Amiibo.
    0x0004000000198E00, // USA.
    0x0004000000198F00, // EUR.
    0x0004000000198D00 //  JPN.
};


void GameLoader::checkUpdate(void)
{
    if (Config::check == 0) {
    Result res = 0;
	u32 updateTitleCount;

    res = AM_GetTitleCount(MEDIATYPE_SD, &updateTitleCount);
    if (R_FAILED(res))
    {
        return;
    }

    // get title list and check if a title matches the ids we want
    std::vector<u64> updateIds;
    updateIds.resize(updateTitleCount);
    res    = AM_GetTitleList(nullptr, MEDIATYPE_SD, updateTitleCount, &updateIds[0]);
    if (R_FAILED(res))
    {
        return;
    }

    Gui::DisplayWarnMsg(Lang::messages2[6]);

    if (std::find(updateIds.begin(), updateIds.end(), 0x0004000E00086300) != updateIds.end() // USA.
        || std::find(updateIds.begin(), updateIds.end(), 0x0004000E00086400) != updateIds.end() // EUR.
        || std::find(updateIds.begin(), updateIds.end(), 0x0004000E00086200) != updateIds.end()) // JPN.
        {
            Gui::DisplayWarnMsg(Lang::update[0]);
            Config::update = 1;
        } else {
            Gui::DisplayWarnMsg(Lang::update[1]);
            Config::update = 0;
        }

        Config::check = 1;
        Config::saveConfig();

    } else if (Config::check == 1) {
    }
}


// Scan the installed Titles, to check if Animal Crossing : New Leaf is found.
void GameLoader::scanTitleID(void)
{
    Result res = 0;
    u32 count  = 0;

    // clear title list if filled previously
    installedTitles.clear();

    scanCard();

    res = AM_GetTitleCount(MEDIATYPE_SD, &count);
    if (R_FAILED(res))
    {
        return;
    }

    // get title list and check if a title matches the ids we want
    std::vector<u64> ids(count);
    u64* p = ids.data();
    res    = AM_GetTitleList(NULL, MEDIATYPE_SD, count, p);
    if (R_FAILED(res))
    {
        return;
    }

    for (size_t i = 0; i < titleIds.size(); i++)
    {
        u64 id = titleIds.at(i);
        if (std::find(ids.begin(), ids.end(), id) != ids.end())
        {
            auto title = std::make_shared<Title>();
            if (title->load(id, MEDIATYPE_SD, CARD_CTR))
            {
                installedTitles.push_back(title);
            }
        }
    }

    // sort the list alphabetically
    std::sort(installedTitles.begin(), installedTitles.end(), [](std::shared_ptr<Title>& l, std::shared_ptr<Title>& r) { return l->ID() < r->ID(); });
}

// Scan the Gamecard, if the Title ID matches with the Cartridge.

bool GameLoader::scanCard()
{
    static bool isScanning = false;
    if (isScanning)
    {
        return false;
    }
    else
    {
        isScanning = true;
    }
    bool ret   = false;
    cardTitle  = nullptr;
    Result res = 0;
    u32 count  = 0;
    // check for the cartridge.
    FS_CardType cardType;
    res = FSUSER_GetCardType(&cardType);
    if (R_SUCCEEDED(res))
    {
        if (cardType == CARD_CTR)
        {
            res = AM_GetTitleCount(MEDIATYPE_GAME_CARD, &count);
            if (R_SUCCEEDED(res) && count > 0)
            {
                ret = true;
                u64 id;
                res = AM_GetTitleList(NULL, MEDIATYPE_GAME_CARD, count, &id);

                // check if this id is in our list
                if (R_SUCCEEDED(res) && std::find(titleIds.begin(), titleIds.end(), id) != titleIds.end())
                {
                    auto title = std::make_shared<Title>();
                    if (title->load(id, MEDIATYPE_GAME_CARD, cardType))
                    {
                        cardTitle = title;
                    }
                    }
                }
            }
        }
    isScanning = false;
    return ret;
}

// Update the GameCard.

bool GameLoader::cardUpdate()
{
    static bool first     = true;
    static bool oldCardIn = false;
    if (first)
    {
        FSUSER_CardSlotIsInserted(&oldCardIn);
        first = false;
        return false;
    }
    bool cardIn = false;

    FSUSER_CardSlotIsInserted(&cardIn);
    if (cardIn != oldCardIn)
    {
        bool power;
        FSUSER_CardSlotGetCardIFPowerStatus(&power);
        if (cardIn)
        {
            if (!power)
            {
                FSUSER_CardSlotPowerOn(&power);
            }
            while (!power)
            {
                FSUSER_CardSlotGetCardIFPowerStatus(&power);
            }
            return oldCardIn = scanCard();
        }
        else
        {
            cardTitle = nullptr;
            oldCardIn = false;
            return true;
        }
    }
    return false;
}