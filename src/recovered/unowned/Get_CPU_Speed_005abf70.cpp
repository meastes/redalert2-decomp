/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// TibSun mpu.cpp:134, TibSun mpu.cpp:134, TibSun mpu.cpp:237, TibSun mpu.cpp:251
#define ASM_RDTSC _asm _emit 0x0f _asm _emit 0x31

union LARGE_INTEGER {
    struct { unsigned long LowPart; long HighPart; };
    __int64 QuadPart;
};
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceCounter(LARGE_INTEGER *);
extern "C" __declspec(dllimport) int __stdcall QueryPerformanceFrequency(LARGE_INTEGER *);
#include "shared/crt_memory.hpp"
#define ROUND_THRESHOLD 6
#define TOLERANCE 1
// @ref __imp__QueryPerformanceCounter@4=0079a1c8
// @ref __imp__QueryPerformanceFrequency@4=0079a1cc
int Get_CPU_Speed(void)
{
	LARGE_INTEGER t0,t1;
	unsigned int	freq=0;						// Most current freq. calc.
	unsigned int	freq2=0;						// 2nd most current freq. calc.
	unsigned int	freq3=0;						// 3rd most current freq. calc.
	unsigned int	total;						// Sum of previous three freq. calc.
	int	tries=0;						// Number of times a calculation has been
												// made on this call
	unsigned int	total_cycles=0, cycles;	// Clock cycles elapsed during test
	unsigned int	stamp0, stamp1;			// Time Stamp for beginning and end of test
	unsigned int	total_ticks=0, ticks;	// Microseconds elapsed during test
// unsigned int	current = 0;				// Elapsed time during loop
	LARGE_INTEGER count_freq;			// Hi-Res Performance Counter frequency


	if ( !QueryPerformanceFrequency(&count_freq) ) return(0);


#if 0 ///Change in Ren
	HANDLE process = GetCurrentProcess();
	DWORD processPri = GetPriorityClass(process);
	SetPriorityClass(process, REALTIME_PRIORITY_CLASS);

	HANDLE thread = GetCurrentThread();
	int threadPri = GetThreadPriority(thread);
	SetThreadPriority(thread, THREAD_PRIORITY_TIME_CRITICAL);
#endif

	/*
	** On processors supporting the TSC opcode, compare elapsed time on the
	** High-Resolution Counter with elapsed cycles on the Time Stamp Counter.
	*/

	do	{
		/*
		** This do loop runs up to 20 times or until the average of the previous
		** three calculated frequencies is within 1 MHz of each of the individual
		** calculated frequencies.   This resampling increases the accuracy of the
		** results since outside factors could affect this calculation.
		*/

		tries++;								// Increment number of times sampled
												//   on this call to cpuspeed

		freq3 = freq2;						// Shift frequencies back to make
		freq2 = freq;						//   room for new frequency measurement

		/*
		** Get high-resolution performance counter time
		*/
		QueryPerformanceCounter(&t0);

		t1.LowPart = t0.LowPart;		// Set Initial time
		t1.HighPart = t0.HighPart;

		/*
		** Loop until 50 ticks have passed since last read of hi-res counter.
		** This accounts for overhead later.
		*/
		while ( (unsigned int)t1.LowPart - (unsigned int)t0.LowPart<50) {
			QueryPerformanceCounter(&t1);
		}

		ASM_RDTSC;
		_asm	mov	stamp0, EAX

		t0.LowPart = t1.LowPart;		// Reset Initial Time
		t0.HighPart = t1.HighPart;

		/*
		** Loop until 1000 ticks have passed since last read of hi-res counter.
		** This allows for elapsed time for sampling.
		*/
		while ( (unsigned int)t1.LowPart - (unsigned int)t0.LowPart < 1000 ) {
			QueryPerformanceCounter(&t1);
		}

		ASM_RDTSC;
		_asm	mov	stamp1, EAX


		cycles = stamp1 - stamp0;					// # of cycles passed between reads

		ticks = ((unsigned int)t1.LowPart - (unsigned int)t0.LowPart);
		ticks = ticks * 100000;						// Convert ticks to hundred
															//   thousandths of a tick
		ticks = (unsigned int)(ticks / (count_freq.LowPart / 10));
															// Hundred Thousandths of a
															//   Ticks / ( 10 ticks/second )
															//   = microseconds (us)
		total_ticks += ticks;
		total_cycles += cycles;
		if ( (ticks % count_freq.LowPart) > (count_freq.LowPart/2) ) ticks++;		// Round up if necessary

		freq = cycles/ticks;							// MHz = cycles / us

		if ( cycles%ticks > ticks/2 ) freq++;	// Round up if necessary

		total = ( freq + freq2 + freq3 );		// Total last three frequency calcs

	} while ( (tries < 3 ) || (tries < 20) && ((abs((int)(3 * freq -total)) > 3*TOLERANCE )|| (abs((int)(3 * freq2-total)) > 3*TOLERANCE )|| (abs((int)(3 * freq3-total)) > 3*TOLERANCE )));

#if 0 ///Change in Ren
	SetThreadPriority(thread, threadPri);
	SetPriorityClass(process, processPri);
#endif

	/*
	** Try one more significant digit.
	*/
	freq3 = ( total_cycles * 10 ) / total_ticks;
	freq2 = ( total_cycles * 100 ) / total_ticks;

	if ( freq2 - (freq3 * 10) >= ROUND_THRESHOLD ) freq3++;

	int norm_freq = total_cycles / total_ticks;

	freq = norm_freq * 10;
	if ( (freq3 - freq) >= ROUND_THRESHOLD ) norm_freq++;

	return (norm_freq);

}
