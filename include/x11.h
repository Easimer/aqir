/*
 * x11.h - XOrg header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#pragma once

#include <string>
#include <X11/Xlib.h>

#define KEY_0 0
#define KEY_1 1
#define KEY_2 2
#define KEY_3 3
#define KEY_4 4
#define KEY_5 5
#define KEY_6 6
#define KEY_7 7
#define KEY_8 8
#define KEY_9 9

#define KEY_A 10
#define KEY_B 11
#define KEY_C 12
#define KEY_D 13
#define KEY_E 14
#define KEY_F 15
#define KEY_G 16
#define KEY_H 17
#define KEY_I 18
#define KEY_J 19
#define KEY_K 20
#define KEY_L 21
#define KEY_M 22
#define KEY_N 23
#define KEY_O 24
#define KEY_P 25
#define KEY_Q 26
#define KEY_R 27
#define KEY_S 28
#define KEY_T 29
#define KEY_U 30
#define KEY_V 31
#define KEY_W 32
#define KEY_X 33
#define KEY_Y 34
#define KEY_Z 35

#define KEY_LSHIFT 36

void x11_open(void);
void x11_close(void);

void x11_mclick(unsigned x, unsigned y);
void x11_mmove(unsigned x, unsigned y);
void x11_kbkey(int key);

void x11_kbhold(int key);
void x11_kbrel(int key);

#define MB_LEFT 1
#define MB_MIDDLE 2
#define MB_RIGHT 3

