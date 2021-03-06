/*
 * obj.h - WoW Object header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#pragma once

#include <wow.h>
#include <stdint.h>
#include <string>

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
	O_CORPSE = 7,
	O_ANY = 0xFF
};

namespace wow {

class obj {
public:
	obj(void);
	obj(uintptr_t& baseaddr);

	void setbase(uintptr_t addr);


protected:
	uintptr_t baseaddr;
};

class item : public obj {

};

class container : public obj {

};

class unit : public obj {

};

class gameobj : public obj {
public:
	gameobj(void);
	gameobj(uintptr_t& baseaddr);
	bool isbobbing(void);
	float getx(void);
	float gety(void);
	float getz(void);
	void setx(float v);
	void sety(float v);
	void setz(float v);
	uint64_t getguid(void);
	uint64_t getrotation(void);
	void setmouse(void);

};

class player : public gameobj {
public:
	player(void);
	player(uintptr_t& baseaddr);
	bool isbobbing(void);
	float getx(void);
	float gety(void);
	float getz(void);
	void setx(float v);
	void sety(float v);
	void setz(float v);
	uint64_t getguid(void);
	uint64_t getrotation(void);
	std::string name(void);
};

}
