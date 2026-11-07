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
// TibSun mpu.cpp:134, TibSun mpu.cpp:134, TibSun mpu.cpp:154
#define ASM_RDTSC _asm _emit 0x0f _asm _emit 0x31

extern unsigned long TSC_Low, TSC_High;
// @ref ?TSC_Low@@3KA=00a71780
// @ref ?TSC_High@@3KA=00a7177c
void RDTSC(void)
{
	_asm
	{
		ASM_RDTSC;
		mov	TSC_Low, eax
		mov	TSC_High, edx
	}
}
