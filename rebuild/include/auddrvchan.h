// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#pragma once

#include "audchan.h"
#include "audtransfer.h"
#include <windows.h>
#include <dsound.h>

typedef int(__fastcall CB_TRANSFER)(AudioDriverChannel *, unsigned char * dst, int * dst_bytes, unsigned char * src, int * src_bytes);

struct AudioDriverChannel
{
	AudioChannel * chan;
	unsigned flags;
	volatile int pcm_pos;
	volatile unsigned frame;
	volatile TimeStamp last_poll;
	volatile TimeStamp poll_interval;
	volatile unsigned char * src_start;
	volatile int src_size;
	volatile unsigned char * src_pos;
	volatile int src_left;
	volatile unsigned long dst_pos;
	volatile unsigned long write_pos;
	volatile int dst_left;
	volatile int silence_left;
	int min_fill;
	AudioFormat format;
	LPDIRECTSOUNDBUFFER dsbuf;
	int buf_size;
	int frame_bytes;
	CB_TRANSFER * transfer;
	TRANSFER transfer_data;
	volatile int next_frame_called;
	volatile int service_count;
	volatile int frames_played;
	volatile int original_frame;
	volatile int original_dst_pos;
};
