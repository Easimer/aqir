/*
 * x11.cpp - XOrg functions
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */

#include <x11.h>
#include <X11/extensions/XTest.h>
#include <unistd.h>

void x11_mclick(void)
{
	Display *d = XOpenDisplay(NULL);
	if(!d)
		return;

	for(unsigned x = 800; x < 1000; x+= 8)
	{
		for(unsigned y = 400; y < 600; y++)
		{
			XTestFakeMotionEvent(d, 0, x, y, CurrentTime);
			XSync(d, 0);
			XTestFakeButtonEvent(d, 3, 1, CurrentTime);
			XTestFakeButtonEvent(d, 3, 0, CurrentTime);
		}
	}
	XCloseDisplay(d);
}
