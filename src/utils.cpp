/*
 * utils.cpp - Utility functions
 * Author: Daniel Meszaros <easimer@gmail.com>
 * This file is part of Aqir, see LICENSE
 */
#include <utils.h>

#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>

// Checks if a pointer is safe to be dereferenced
// msync is a Linux syscall that return a non-zero value
// if there is a problem with the address
// Consult `man 3 msync` for more details
bool IsPointerValid(uintptr_t p)
{
	size_t page_size = sysconf(_SC_PAGESIZE);
	void *base = (void *)((((size_t)p) / page_size) * page_size);
	return msync(base, page_size, MS_ASYNC) == 0;
}

template<typename T> T safe_read(T* addr)
{
	if(!IsPointerValid(static_cast<uintptr_t>(addr)))
	{
		throw std::runtime_error("safe_read: pointer is invalid");
	}
	return *addr;
}
