/*
 * offsets.h - Offsets
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#pragma once

#include <config.h>

#if GAME_VERSION == 18414

// camera
#define CAMSTOFF 0x14A13B0
#define CAMOFF 0x85E8
#define CAMORIGOFF 0x10
#define CAMMATOFF 0x1C
#define CAMNPOFF 0x38
#define CAMFPOFF 0x3C
#define CAMFOVOFF 0x40
#define CAMAROFF 0x44

// entity
#define ENT_OFF_TYPE 0x18

// object - gameobject
#define GOBJ_OFF_BOBBING 376
#define GOBJ_OFF_POSX 448
#define GOBJ_OFF_POSY 452
#define GOBJ_OFF_POSZ 456
#define GOBJ_OFF_DESC 8
#define GOBJ_OFF_DESC_GUID 0

// object - unit
#define UNIT_OFF_DESC 0x8
#define UNIT_OFF_DESC_GUID 0
#define UNIT_OFF_DESC_FLAGSD 24
#define UNIT_OFF_DESC_CREATOR 72
#define UNIT_OFF_DESC_HEALTH 132
#define UNIT_OFF_DESC_POWER 136
#define UNIT_OFF_DESC_HEALTHM 156
#define UNIT_OFF_DESC_POWERM 160
#define UNIT_OFF_DESC_LEVEL 220
#define UNIT_OFF_POSX 4160
#define UNIT_OFF_POSY 4164
#define UNIT_OFF_POSZ 4168
#define UNIT_OFF_ROT 4176
#define UNIT_OFF_CASTING 5640
#define UNIT_OFF_CHANNELING 5664

// object manager
#define BASEADDR (0x140000000)
#define ENTLIST (0x1387048)
#define OBJMGR_0 (0x18)
#define OBJMGR_N (0x58)
#define MOUSE_GUID (0x14A20C0)

// player
#define PLAYER_CACHE (4592)
#define PLAYER_CACHE_NAME (152)

#define PLAYER_ISMOUNTED (0x1EBC)
#define PLAYER_ISFISHING1 (0x1621) // possible candidate
#define PLAYER_ISFISHING2 (0x1627) // possible candidate
#define PLAYER_ISBOBBING1 (0x22a23c)
#define PLAYER_ISBOBBING2 (0x22a258)
#define PLAYER_ISLOOTING (0x15121C4)

#define PLAYER_X UNIT_OFF_POSX
#define PLAYER_Y UNIT_OFF_POSY
#define PLAYER_Z UNIT_OFF_POSZ

// target
#define TARGETOFF 0x14A20D8

// game
#define GAMEBUILD_OFF (0x1274FE4)
#define PLAYERPTR_OFF (0x141C2D0)

#define BASEADDR (0x140000000)
#define CLIENTCONN (0x009BE678)

#endif /* GAME_VERSION == 18414 */
