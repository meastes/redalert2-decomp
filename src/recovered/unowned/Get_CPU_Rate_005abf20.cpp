// Get_CPU_Rate (mpu.cpp; mischa85/RedAlert2 dbca015 src/mpu.cpp, TibSun mpu.cpp): the performance counter
// frequency, its low word returned and its high word stored, both 0 when there is none. Modified: the
// high word goes through a pointer, as this tree's callers declare it (cpu_info.hpp), until mpu.cpp is
// one module source.
#include "shared/win32_api.hpp"
#include "shared/cpu_info.hpp"

unsigned int __fastcall Get_CPU_Rate(unsigned int *high)
{
	union {
		LARGE_INTEGER LargeInt;
		struct {
			unsigned int LowPart;
			unsigned int HighPart;
		} QuadPart;
	} value;

	if (QueryPerformanceFrequency(&value.LargeInt)) {
		*high = value.QuadPart.HighPart;
		return(value.QuadPart.LowPart);
	}
	*high = 0;
	return(0);
}
