/*
 * x11.h - XOrg header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#pragma once

#define KEY_4 0
#define KEY_LSHIFT 1

void x11_open(void);
void x11_close(void);

void x11_mclick(unsigned x, unsigned y);
void x11_kbkey(int key);

void x11_kbhold(int key);
void x11_kbrel(int key);
