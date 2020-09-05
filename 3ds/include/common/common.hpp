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

#ifndef _LEAFEDIT_COMMON_HPP
#define _LEAFEDIT_COMMON_HPP

#include <3ds.h>
#include <citro2d.h>

#ifdef __cplusplus
extern "C" {
#endif
#include "files.h"

#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#ifdef __cplusplus
}

#include "config.hpp"
#include "gfx.hpp"
#include "gui.hpp"
#include "json.hpp"
#include "lang.hpp"
#include "msg.hpp"
#include "overlay.hpp"
#include "screenCommon.hpp"
#include "utils.hpp"

#include <memory>
#endif

/* Title IDs. */
#define OldJPN				0x0004000000086200
#define OldUSA				0x0004000000086300
#define OldEUR				0x0004000000086400
#define OldKOR				0x0004000000086500

#define WelcomeAmiiboJPN	0x0004000000198D00
#define WelcomeAmiiboUSA	0x0004000000198E00
#define WelcomeAmiiboEUR	0x0004000000198F00
#define WelcomeAmiiboKOR	0x0004000000199000

#define WelcomeLuxury		0x00040000004C5700

/* GodMode and whatnot. */
extern bool doFade;
extern C2D_Font font;

#endif