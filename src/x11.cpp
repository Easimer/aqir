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
	//XSync(x11_d, 0);
	XTestFakeButtonEvent(x11_d, 3, 1, CurrentTime);
	XTestFakeButtonEvent(x11_d, 3, 0, CurrentTime);
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

CXOrg::CXOrg(std::string window_name)
{
	m_pDisplay = XOpenDisplay(NULL);

	if(!m_pDisplay)
	{
		throw std::runtime_error("Cannot open X11 display!!");
	}

	Atom a = XInternAtom(m_pDisplay, "_NET_CLIENT_LIST", true);
	Atom actualType;
	int format;
	unsigned long numItems, bytesAfter;
	unsigned char *data = NULL;
	int status = XGetWindowProperty(m_pDisplay, DefaultRootWindow(m_pDisplay), a, 0L, (~0L), false,
                                AnyPropertyType, &actualType, &format, &numItems,
                                &bytesAfter, &data);
	if(status >= Success && numItems)
	{
		std::cout << "Searching" << std::endl;
		long *array = reinterpret_cast<long*>(data);
		for (unsigned long k = 0; k < numItems; k++)
		{
			Window w = (Window) array[k];
			char* name = NULL;
			status = XFetchName(m_pDisplay, w, &name);
			if(status >= Success)
			{
				if(strcmp(name, window_name.c_str()) == 0)
				{
					m_iWindow = w;
					XFree(name);
					XFree(data);
					std::cout << "Window found " << std::endl;
					return;
				}
			}
			XFree(name);
		}
		XFree(data);
	}
	else
	{
		throw std::runtime_error("XGetWindowProperty failed");
	}
}

CXOrg::~CXOrg(void)
{
	XCloseDisplay(m_pDisplay);
}

void CXOrg::MouseClick(unsigned t, unsigned x, unsigned y)
{
	XButtonEvent ev;
	ev.button = t;
	ev.display = m_pDisplay;
	ev.window = m_iWindow;
	ev.x = x;
	ev.y = y;

	//XSendEvent(m_pDisplay, m_iWindow, 1, ButtonPressMask, reinterpret_cast<XEvent*>(&ev));

	// first we release the button
	ev.type = ButtonRelease;
	XEV_SEND_MB(ev);

	// then press

	ev.type = ButtonPress;
	XEV_SEND_MB(ev);

	// finally, release
	ev.type = ButtonRelease;
	XEV_SEND_MB(ev);
}
