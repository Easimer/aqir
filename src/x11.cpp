/*
 * x11.cpp - XOrg functions
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */

#include <x11.h>
#include <stdexcept>
#include <X11/Xatom.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>
#include <X11/Intrinsic.h>
#include <unistd.h>
#include <iostream>

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

	int kc;
	switch(key)
	{
		case KEY_4:
			kc = XK_4;
			break;
		case KEY_LSHIFT:
			kc = XK_Shift_L;
			break;
		default:
			return;
	}

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

	int kc;
	switch(key)
	{
		case KEY_4:
			kc = XK_4;
			break;
		case KEY_LSHIFT:
			kc = XK_Shift_L;
			break;
		default:
			return;
	}

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

	int kc;
	switch(key)
	{
		case KEY_4:
			kc = XK_4;
			break;
		case KEY_LSHIFT:
			kc = XK_Shift_L;
			break;
		default:
			return;
	}

	KeyCode keycode = 0;
	keycode = XKeysymToKeycode(x11_d, kc);
	XTestFakeKeyEvent(x11_d, keycode, 0, 0);

	XSync(x11_d, 0);
	XTestGrabControl(x11_d, 0);
}
