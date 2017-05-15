/*
 * obj.h - WoW Object header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#pragma once

#include <wow.h>
#include <stdint.h>

class CWowObject {
public:
	CWowObject(uintptr_t baseaddr);
	uint64_t GetGUID(void);
private:
	uintptr_t baseaddr;
};

struct wow_unit_desc {
	uint64_t guid;
	uint8_t unk1[12];
	uint32_t id;
	uint32_t flagsd;
	uint8_t unk2[44];
	uint64_t creator;
	uint8_t unk3[52];
	uint32_t health;
	uint32_t power;
	uint8_t unk4[16];
	uint32_t health_max;
	uint32_t power_max;
	uint8_t unk5[56];
	uint32_t level;
	uint8_t unk6[20];
	uint32_t flags;
} __attribute__((packed));

struct wow_ent {
	uint64_t guid;
	union {
		struct wow_unit_desc* unit;
	} desc;
	uint64_t unk1;
	uint8_t type;
} __attribute__((packed));

enum wow_obj_type {
	O_ITEM = 1,
	O_CONTAINER = 2,
	O_UNIT = 3,
	O_PLAYER = 4,
	O_GAMEOBJ = 5,
	O_DYNOBJ = 6,
	O_CORPSE = 7
};
