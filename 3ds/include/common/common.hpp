#ifndef COMMON_HPP
#define COMMON_HPP

#pragma once

#include <3ds.h>

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

// This is the new "screenCommon.hpp", lol.
#include "config.hpp"
#include "gfx.hpp"
#include "gui.hpp"
#include "json.hpp"
#include "lang.hpp"
#include "msg.hpp"
#include "screenCommon.hpp"
#include "stringutils.hpp"

#endif

//Title IDs
#define OldJPN     0x0004000000086200
#define OldUSA     0x0004000000086300
#define OldEUR     0x0004000000086400
#define OldKOR     0x0004000000086500

#define WelcomeAmiiboJPN  0x0004000000198D00
#define WelcomeAmiiboUSA  0x0004000000198E00
#define WelcomeAmiiboEUR  0x0004000000198F00
#define WelcomeAmiiboKOR  0x0004000000199000

#define WelcomeLuxury 0x00040000004C5700

// GodMode and whatnot.
extern bool GodMode;
extern bool Debug;

#endif