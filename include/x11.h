/*
 * x11.h - XOrg header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#pragma once

#include <string>
#include <X11/Xlib.h>

#define KEY_4 0
#define KEY_LSHIFT 1

void x11_open(void);
void x11_close(void);

void x11_mclick(unsigned x, unsigned y);
void x11_kbkey(int key);

void x11_kbhold(int key);
void x11_kbrel(int key);

#define MB_LEFT 1
#define MB_MIDDLE 2
#define MB_RIGHT 3

#define XEV_SEND(mask, ev) (XSendEvent(m_pDisplay, m_iWindow, 1, mask, reinterpret_cast<XEvent*>(&ev)))
#define XEV_SEND_MB(ev) (XEV_SEND(ButtonPressMask, ev))
#define XEV_SEND_KB(ev) (XEV_SEND(KeyPressMask, ev))

class CXOrg {
public:
	CXOrg(std::string window_name);
	~CXOrg(void);
	void MouseClick(unsigned t, unsigned x, unsigned y);
	void HoldKey(int key);
	void ReleaseKey(int key);
	void HitKey(int key);
private:
	Display* m_pDisplay;
	Window m_iWindow;
};

