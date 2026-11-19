// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#pragma once

// The DirectSound driver of the audio library. The library's types come from their own headers
// (one definition each); this header adds only what the driver alone provides.
#include "auddev.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>

#define mAUDIO_CTRL_ACTIVE 5

AudioDevice * __fastcall AudioGetDevice(void);
void AudioLoseFocus(void); // RA2: 0x0040A860
void AudioRegainFocus(void); // RA2: 0x0040A870

extern AudioAttribs * AudioMasterAttribs;
extern AudioAttribs * AudioVoiceAttribs;
extern AudioAttribs * AudioScoreAttribs;
extern AudioAttribs * AudioSoundAttribs;
extern AudioAttribs * AudioCompAttribs;
extern AudioAttribs * AudioGroupAttribs2;
extern AudioAttribs * AudioFadeAttribs;
extern AudioAttribs * AudioGroupAttribs1;

extern AudioSystemMaster * AudioSystemDSD;
extern LPDIRECTSOUND AUD_sound_object;
extern LPDIRECTSOUNDBUFFER AUD_primary_buffer;

#include "imadata.h"
#include "audtransfer.h"
#include "auddrvchan.h"
#include "audthread.h"
