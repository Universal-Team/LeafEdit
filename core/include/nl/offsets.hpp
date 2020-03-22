/*
	MIT License
	This file is part of NLTK
	Copyright (c) 2018-2019 Slattz, Cuyler

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#pragma once

#ifndef OFFSETS_HPP
#define OFFSETS_HPP

typedef enum{
	TOWN_TREESIZE=			0x04be86,
	TOWN_GRASSTYPE=			0x053481,
	TOWN_PLAYTIME=			0x0621b0,
	TOWN_DAYSPLAYED=		0x06223e,
	TOWN_NATIVEFRUIT=		0x06223a,
	TOWN_ID1=				0x0621b8,
	TOWN_ID2=				0x0621b9,
	TOWN_NAME=				0x0621ba,
	TOWN_AVAILABLEPWPS=		0x050328,
	TOWN_TURNIP_PRICES=		0x06ade0,
	TOWN_MULTVARS=			0x0621d6,

	MUSEUM_ROOMS=			0x06b478,

	MAP_GRASS_TODAY=		0x0584d8,
	MAP_GRASS=				0x059900,
	MAP_ACRES=				0x053484,
	MAP_BUILDINGS=			0x04be88,
	MAP_ITEMS=				0x0534d8,

	ISLAND_GRASSTYPE=		0x06feb6,
	ISLAND_ACRES=			0x06feb8,
	ISLAND_ITEMS=			0x06fed8,
	ISLAND_BUILDINGS=		0x070ed8,

	PLAYERS=				0xa0,
	PLAYER_HAIRSTYLE=		0x04,
	PLAYER_HAIRCOLOR=		0x05,
	PLAYER_FACE=			0x06,
	PLAYER_EYECOLOR=		0x07,
	PLAYER_TAN=				0x08,
	PLAYER_HAT=				0x0a,
	PLAYER_ACCESSORY=		0x0e,
	PLAYER_WETSUIT=			0x12,
	PLAYER_TOPWEAR=			0x16,
	PLAYER_BOTTOMWEAR=		0x1a,
	PLAYER_SOCKS=			0x1e,
	PLAYER_SHOES=			0x22,
	PLAYER_HELDITEM=		0x26,
	PLAYER_PATTERNS=		0x2c,
	PLAYER_ID1=				0x55a6,
	PLAYER_ID2=				0x55a7,
	PLAYER_NAME=			0x55a8,
	PLAYER_GENDER=			0x55ba,
	PLAYER_TPCREGION=		0x55d2,
	PLAYER_BIRTHDAYMONTH=	0x55d4,
	PLAYER_BIRTHDAYDAY=		0x55d5,
	PLAYER_REGYEAR=			0x55d6,
	PLAYER_REGMONTH=		0x55d8,
	PLAYER_REGDAY=			0x55d9,
	PLAYER_BADGE11_VALUE=	0x6ba4,
	PLAYER_BADGES_VALUES=	0x55dc,
	PLAYER_BADGES=			0x569c,
	PLAYER_TPCPIC=			0x5738,
	PLAYER_TPCTEXT=			0x6b38,
	PLAYER_PLAYTIME=		0x6bb0,
	PLAYER_POCKETS=			0x6bd0,
	PLAYER_ENCYCLOPEDIA=	0x6c70,
	PLAYER_CATALOG=			0x6c90,
	PLAYER_BANK=			0x6b8c,
	PLAYER_MEDALS=			0x6b9c,
	PLAYER_WALLET=			0x6f08,
	PLAYER_ISLANDBOX=		0x6f10,
	PLAYER_LETTERS=			0x7008,
	PLAYER_EMOTIONS=		0x89d0,
	PLAYER_MEOW=			0x8d1c,
	PLAYER_DRESSERS=		0x92f0,
	PLAYER_EXTERIORS=		0x5D904,
	PLAYER_ROOMS=			0x05d97a,
	PLAYER_STORAGE=			0x07a778,

	VILLAGERS=				0x0292d0,
	VILLAGER_ID=			0x00,
	VILLAGER_PERSONALITY=	0x02,
	VILLAGER_STOREDLETTER=	0x17c4,
	VILLAGER_SHIRT=			0x246e,
	VILLAGER_SONG=			0x2472,
	VILLAGER_WALL=			0x2476,
	VILLAGER_FLOOR=			0x247a,
	VILLAGER_UMBRELLA=		0x247e,
	VILLAGER_FURNITURE=		0x2482,
	VILLAGER_CATCHPHRASE=	0x24c6,
	VILLAGER_STATUS=		0x24e4, /* is it correct? */

	PAST_VILLAGERS=			0x0408fa,
	CAMPSITE_VILLAGER=		0x040946,
	CARAVAN_VILLAGERS=		0x06ae60,

	SHOP_LOSTFOUND=			0x621e2,
	SHOP_NOOK=				0x62270,
	SHOP_ABLE1=				0x622ec,
	SHOP_ABLEPATTERNS=		0x62338,
	SHOP_ABLE2=				0x666b8,
	SHOP_LEIF=				0x666f6,
	SHOP_REDD=				0x66730,
	SHOP_KICKS=				0x6682e,
	SHOP_RETAIL=			0x668b4,
	SHOP_MUSEUM=			0x6acbe,
	SHOP_HOMES=				0x6acce,
	SHOP_GRACIE=			0x6acf8,
	SHOP_LOLGYROIDS=		0x6ad92,
	SHOP_ISLAND=			0x6adb8,

	HHD_UNLOCK=				0x621dc,

	MIN_WALL=		0x234c,	MAX_WALL=		0x23ea,
	MIN_FLOOR=		0x23eb,	MAX_FLOOR=		0x2492,
	MIN_SONG=		0x212b,	MAX_SONG=		0x2185,
	MIN_SHIRT=		0x2495,	MAX_SHIRT=		0x2680,
	MIN_UMBRELLA=	0x27e6,	MAX_UMBRELLA=	0x280a
} AC_Offsets;

typedef enum{
	SIZE_SAVE					= 0x89B00, //563968
	SIZE_PLAYER 				= 0xA480,
	SIZE_VILLAGER 				= 0x2518,
	SIZE_PLAYER_HOUSE 			= 0x1228,
	SIZE_PLAYER_POCKETS			= 16,
	SIZE_PLAYER_DRESSERS		= 180,
	SIZE_PLAYER_ISLANDBOX		= 40,
	SIZE_PLAYER_STORAGE			= 280,
	//SIZE_PATTERN 				= 2160,
	SIZE_PATTERN 				= 4320,
	SIZE_PAST_VILLAGER 			= 2,
	SIZE_ACRE 					= 256, //16*16
	SIZE_VILLAGE_ITEMS 			= 5120, //SIZE_ACRE*5*4
	SIZE_ISLAND_ITEMS 			= 1024 //SIZE_ACRE*2*2
} AC_Sizes;

typedef struct HouseExterior
{
	int HouseStyle;
	int HouseDoorForm;
	int HouseBrick;
	int HouseRoof;
	int HouseDoor;
	int HouseFence;
	int HousePavement;
	int HouseMailBox;
	int MainRoomSize;
	int UpstairsSize;
	int BasementSize;
	int RightRoomSize;
	int LeftRoomSize;
	int BackRoomSize;
	int HouseSize;

} ACNL_HouseExterior;

typedef struct PlayerData
{
	u8   PID1;
	u8   PID2;
	char DataPlayerName[20];
	u8 	 TID1;
	u8   TID2;
	char DataTownName[20];

} PlayerData;

/*typedef struct Pattern
{
	char Title[42];
	PlayerData CreatorData;
	u16  Unknown1; //values are usually random - changing seems to have no effect
	u8   Palette[16];
	u8   Unknown2; //values are usually random - changing seems to have no effect
	u8   TenConstant; //seems to always be 0x0A
	u8   PatternType; //normal patterns: 0x09; dresses: 0x00; photo boards: 0x08
	u16  ZeroPad_1; //Zero Padding; Always 0x0000
	u8   PatternData1[512]; //madatory
	u8   PatternData2[512]; //optional
	u8   PatternData3[512]; //optional
	u8   PatternData4[512]; //optional
	u32  ZeroPad_2; //Zero Padding; Optional

} ACNL_Pattern;*/

typedef struct Letter
{
	PlayerData Reciever;
	u16 Unknown1;
	u16 ZeroPad_1;
	u8 RecieverID; //
	u8 ZeroPad_2[50];
	u16 Unknown2; //Some form of ID?
	u16 ZeroPad_3;
	char Header[64]; //Max amount is 32 UTF-16 characters
	u16 ZeroPad_4;
	char Body[384]; //Max amount is 192 UTF-16 characters
	u16 ZeroPad_5;
	char Signature[64]; //Max amount is 32 UTF-16 characters


} ACNL_Letter;

/*typedef struct Player_Patterns
{
	ACNL_Pattern Pattern1;
	ACNL_Pattern Pattern2;
	ACNL_Pattern Pattern3;
	ACNL_Pattern Pattern4;
	ACNL_Pattern Pattern5;
	ACNL_Pattern Pattern6;
	ACNL_Pattern Pattern7;
	ACNL_Pattern Pattern8;
	ACNL_Pattern Pattern9;
	ACNL_Pattern Pattern10;

} Player_Patterns;*/

typedef struct Player_Letters
{
	ACNL_Letter Letter1;
	ACNL_Letter Letter2;
	ACNL_Letter Letter3;
	ACNL_Letter Letter4;
	ACNL_Letter Letter5;
	ACNL_Letter Letter6;
	ACNL_Letter Letter7;
	ACNL_Letter Letter8;
	ACNL_Letter Letter9;
	ACNL_Letter Letter10;

} Player_Letters;

typedef struct mPlayer
{
	u8 HairStyle;     					//0xA4
	u8 HairColor;     					//0xA5
	u8 Face;          					//0xA6
	u8 EyeColor;      					//0xA7
	u16 Tan;          					//0xA8
	u32 Hat;          					//0xAA
	u32 Accessory;    					//0xAE
	u32 TopWear;      					//0xB2
	u32 UnderTopWear; 					//0xB6
	u32 BottomWear;   					//0xBA
	u32 Socks;        					//0xBE
	u32 Shoes;        					//0xC2
	u32 HeldItem;     					//0xC6
//	Player_Patterns Patterns; 			//0xCC
	u8 Unknown1[10]; 					//0x552C -> 0x5535
	u16 ZeroPad_1; 						//0x5536: Zero Padding; Always 0x0000
	u8 MiiFace[92]; 					//0x5538 -> 0x5595; Based on https://3dbrew.org/wiki/Mii#Mii_format
	u16 ZeroPad_2; 						//0x5594: Zero Padding; Always 0x0000
	u8 Unknown2[42]; 					//0x5596 -> 0x55BF
	u8 ZeroPad_3[32]; 					//0x55C0 -> 0x55DF
	u32 Unknown3; 						//0x55E0 -> 0x55E3
	u32 Unknown4[24]; 					//0x55E4 -> 0x5643; Array contains u32 item IDS
	u16 ZeroPad_4; 						//0x5644
	u8 PID1;		  					//0x5646
	u8 PID2;          					//0x5647
	char PlayerName[18]; 				//0x5648
	u16 Gender; 						//0x565A
	u8 TID1_1; 							//0x565C
	u8 TID2_1; 							//0x565D
	char TownName_1[20]; 				//0x565E
	u16 TPC_Region;						//0x5672
	u8 BirthMonth; 						//0x5674
	u8 BirthDay; 						//0x5675
	u16 YearRegistered; 				//0x5676
	u8 MonthRegistered; 				//0x5678
	u8 DayRegistered; 					//0x5679
	u16 ZeroPad_5; 						//0x567A: Zero Padding; Always 0x0000
	u64 Badge00_Value; 					//0x567C: Encrypted Value
	u64 Badge01_Value; 					//0x5684: Encrypted Value
	u64 Badge02_Value; 					//0x568C: Encrypted Value
	u64 Badge03_Value; 					//0x5694: Encrypted Value
	u64 Badge04_Value; 					//0x569C: Encrypted Value
	u64 Badge05_Value; 					//0x56A4: Encrypted Value
	u64 Badge06_Value; 					//0x56AC: Encrypted Value
	u64 Badge07_Value; 					//0x56B4: Encrypted Value
	u64 Badge08_Value; 					//0x56BC: Encrypted Value
	u64 Badge09_Value; 					//0x56C4: Encrypted Value
	u64 Badge10_Value; 					//0x56CC: Encrypted Value
	u64 Badge11_Value; 					//0x56D4: Encrypted Value
	u64 Badge12_Value; 					//0x56DC: Encrypted Value
	u64 Badge13_Value; 					//0x56E4: Encrypted Value
	u64 Badge14_Value; 					//0x56EC: Encrypted Value
	u64 Badge15_Value; 					//0x56F4: Encrypted Value
	u64 Badge16_Value; 					//0x56FC: Encrypted Value
	u64 Badge17_Value; 					//0x5704: Encrypted Value
	u64 Badge18_Value; 					//0x570C: Encrypted Value
	u64 Badge19_Value; 					//0x5714: Encrypted Value
	u64 Badge20_Value; 					//0x571C: Encrypted Value
	u64 Badge21_Value; 					//0x5724: Encrypted Value
	u64 Badge22_Value; 					//0x572C: Encrypted Value
	u64 Badge23_Value; 					//0x5734: Encrypted Value
	u8 Badges[23]; 						//0x573C: 24 badges
	u8 Unknown5[130]; 					//0x5754 -> 0x57D5
	u16 ZeroPad_10;						//0x57D6 -> 0x57D7
	u8 TPCPic[0x1400]; 					//0x57D8 -> 0x6BD7
	char TPCText[38];					//0x6BD8 -> 0x6C0F
	u8 ZeroPad_6[24];					//0x6C10 -> 0x6C27
	u32 Unknown6;						//0x6C28 -> 0x6C2B
	u64 BankAmount;						//0x6C2C -> 0x6C33: Encrypted Value
	u64 Debt;							//0x6C34 -> 0x6C3B: Encrypted Value
	u64 MedalAmount;					//0x6C3C -> 0x6C43: Encrypted Value
	u64 Badge11_Value_2;				//0x6C44 -> 0x6C4B
	u32 ZeroPad_7;						//0x6C4C -> 0x6C4F
	u32 Playtime;						//0x6C50 -> 0x6C53
	u32 ZeroPad_8;						//0x6C54 -> 0x6C57
	u8 TID1_2; 							//0x6C68
	u8 TID2_2; 							//0x6C59
	char TownName_2[20]; 				//0x6C5A -> 0x6C6D
	u16 ZeroPad_9;						//0x6C6E -> 0x6C6F
	u32 Inventory[16];					//0x6C70 -> 0x6CAF
	u8 InventoryItemLocks[16];			//0x6CB0 -> 0x6CBF
	u8 Unknown8[80];					//0x6CC0 -> 0x6D0F
	u8 Encyclopedia[28];				//0x6D10 -> 0x6D2B
	u32 Unknown9;						//0x6D2C -> 0x6D2F
	u8 Catalog[424];					//0x6D30 -> 0x6ED7
	u8 Unknown10[208];					//0x6ED8 -> 0x6FA7
	u64 PocketMoney;					//0x6FA8 -> 0x6FAF: Encrypted Value
	u32 IslandBox[40];					//0x6FB0 -> 0x704F
	u32 IslandInventory[16];			//0x7050 -> 0x708F
	u8 IslandInventoryItemLocks[16];	//0x7090 -> 0x709F: may be Padding also, needs testing
	Player_Letters Letters;				//0x70A0 ->




	//PLAYER_BADGE11_VALUE=	0x6ba4,
	//PLAYER_PLAYTIME=		0x6bb0,
	//PLAYER_POCKETS=		0x6bd0,
	//PLAYER_ENCYCLOPEDIA=    0x6c70,
	//PLAYER_CATALOG=		0x6c90,
	//PLAYER_BANK=			0x6b8c,
	//PLAYER_MEDALS=		0x6b9c,
	//PLAYER_WALLET=		0x6f08,
	//PLAYER_ISLANDBOX=		0x6f10,

	//PLAYER_LETTERS=		0x7008,
	//PLAYER_EMOTIONS=		0x89d0,
	//PLAYER_MEOW=			0x8d1c,
	//PLAYER_DRESSERS=		0x92f0,
	//PLAYER_EXTERIORS=		0x5D904,
	//PLAYER_ROOMS=			0x05d97a,
	//PLAYER_STORAGE=		0x07a778,
} ACNL_Player;

#endif