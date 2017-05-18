/*
 * utils.cpp - Utility functions
 * Author: Daniel Meszaros <easimer@gmail.com>
 * EasimerNet-Confidental
 */
#include <utils.h>

#include <stdexcept>
#include <sys/mman.h>
#include <unistd.h>

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
