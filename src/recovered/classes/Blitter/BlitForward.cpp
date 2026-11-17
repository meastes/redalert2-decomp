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
// void BlitTrans<unsigned char>::BlitForward(...) const (0x00775980): copies all but colour 0.
// Source: TibSun blitblit.h:2324 and TibSun blitblit.h:2332 (the archive's assembler specialization).
#include "shared/blitter.hpp"
// The archive disables optimization around the assembler specializations (blitblit.h:2319).
#pragma optimize("", off)
template <>
void BlitTrans<unsigned char>::BlitForward(void *dest, void const *source, int len, int, void *, void *, int, int) const
{
	__asm {
		mov	esi,[source]
		mov	edi,[dest]
		mov	ecx,[len]
		dec	edi
		inc	ecx
	}
again:
	__asm {
		dec	ecx
		jz		fini
		mov	al,[esi]
		inc	edi
		inc	esi
		test	al,al
		jz		again
		mov	[edi],al
		jmp	again
	}
fini:;
}

#pragma optimize("", on)
