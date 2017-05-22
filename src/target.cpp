#include <target.h>

namespace wow {
	uintptr_t target::getptr(void)
	{
		return *reinterpret_cast<uintptr_t*>(TARGETOFF);
	}

	float target::getx(void)
	{

	}
}
