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
// Source: TibSun blitblit.h:2352 and TibSun blitblit.h:2362.
#include "shared/basic_xlat_blitters.hpp"
// The archive disables optimization around the assembler specializations (blitblit.h:2319).
#pragma optimize("", off)
template <>
void BlitTransXlat<unsigned short>::BlitForward(void *dest,void const *source,int len,int,int,int,int,int) const
{
    unsigned short const * xlator = Palette;

	__asm {
		mov	ebx,[xlator]
		mov	ecx,[len]
		inc	ecx
		mov	edi,[dest]
		sub	edi,2
		mov	esi,[source]
		xor	eax,eax
	}
again:
	__asm {
		dec	ecx
		jz		over
		add	edi,2
		mov	al,[esi]
		inc	esi
		or		al,al
		jz		again
		mov	dx,[ebx+eax*2]
		mov	[edi],dx
		jmp	again
	}
over:;
}

#pragma optimize("", on)
