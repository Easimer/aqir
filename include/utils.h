/*
 * utils.h - Utilities header
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#pragma once

#include <cstdint>

#define READ_UINT64(addr) (*reinterpret_cast<uint64_t*>(addr))
#define READ_UINT32(addr) (*reinterpret_cast<uint32_t*>(addr))
#define READ_UINT16(addr) (*reinterpret_cast<uint16_t*>(addr))
#define READ_UINT8(addr) (*reinterpret_cast<uint8_t*>(addr))

// Dereference pointer safely; returns NULL if the pointer is invalid
#define READ_UINT64S(addr) (safe_read(addr))
#define READ_UINT32S(addr) (safe_read(addr))
#define READ_UINT16S(addr) (safe_read(addr))
#define READ_UINT8S(addr) (safe_read(addr))

// Checks whether a pointer is valid or not
bool IsPointerValid(uintptr_t p);
template<typename T> T safe_read(T* addr);
