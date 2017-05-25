/*
 * x11.cpp - XOrg functions
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */

#include <x11.h>
#include <stdexcept>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <X11/Intrinsic.h>
#include <unistd.h>
#include <iostream>

const unsigned x11_xk_map[] = {
	XK_0, XK_1, XK_2, XK_3, XK_4, XK_5, XK_6, XK_7, XK_8, XK_9, 
	XK_A, XK_B, XK_C, XK_D, XK_E, XK_F, XK_G, XK_H, XK_I, XK_J, XK_K, XK_L,
	XK_M, XK_N, XK_O, XK_P, XK_Q, XK_R, XK_S, XK_T, XK_U, XK_V, XK_W, XK_X,
	XK_Y, XK_Z
};

#define X11_XK_MAP_LEN (36)

Display *x11_d = NULL;

void x11_open(void)
{
	if(x11_d)
		return;
	x11_d = XOpenDisplay(NULL);
}

void x11_close(void)
{
	if(!x11_d)
		return;
	XCloseDisplay(x11_d);
	x11_d = NULL;
}

void x11_mclick(unsigned x, unsigned y)
{
	if(!x11_d)
		x11_open();
	if(!x11_d)
		return;

	XTestFakeMotionEvent(x11_d, 0, x, y, CurrentTime);
	XSync(x11_d, 0);
	XTestFakeButtonEvent(x11_d, 3, 1, CurrentTime);
	XTestFakeButtonEvent(x11_d, 3, 0, CurrentTime);
	XSync(x11_d, 0);
}

void x11_mmove(unsigned x, unsigned y)
{
	XTestFakeMotionEvent(x11_d, 0, x, y, CurrentTime);
	XSync(x11_d, 0);
}

void x11_kbkey(int key)
{
	if(!x11_d)
		x11_open();
	if(!x11_d)
		return;

	if(key >= X11_XK_MAP_LEN)
		return;

	int kc = x11_xk_map[key];

	KeyCode keycode = 0;
	keycode = XKeysymToKeycode(x11_d, kc);
	XTestGrabControl(x11_d, 1);
	XTestFakeKeyEvent(x11_d, keycode, 1, 0);
	XTestFakeKeyEvent(x11_d, keycode, 0, 0);

	XSync(x11_d, 0);
	XTestGrabControl(x11_d, 0);
}

void x11_kbhold(int key)
{
	if(!x11_d)
		x11_open();
	if(!x11_d)
		return;

	if(key >= X11_XK_MAP_LEN && key != KEY_LSHIFT)
		return;

	int kc;

	if(key == KEY_LSHIFT)
		kc = XK_Shift_L;
	else
		kc = x11_xk_map[key];

	KeyCode keycode = 0;
	keycode = XKeysymToKeycode(x11_d, kc);
	XTestGrabControl(x11_d, 1);
	XTestFakeKeyEvent(x11_d, keycode, 1, 0);
}

void x11_kbrel(int key)
{
	if(!x11_d)
		x11_open();
	if(!x11_d)
		return;

	if(key >= X11_XK_MAP_LEN && key != KEY_LSHIFT)
		return;

	int kc;

	if(key == KEY_LSHIFT)
		kc = XK_Shift_L;
	else
		kc = x11_xk_map[key];

	KeyCode keycode = 0;
	keycode = XKeysymToKeycode(x11_d, kc);
	XTestFakeKeyEvent(x11_d, keycode, 0, 0);

	XSync(x11_d, 0);
	XTestGrabControl(x11_d, 0);
}
