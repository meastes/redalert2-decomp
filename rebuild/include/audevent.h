// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#pragma once

#include "audlist.h"
#include "audattr.h"
#include "audlock.h"
#include "audtimer.h"

struct AudioCacheItem;
struct AudioChannel;
struct AudioSample;
struct AudioController;

#define AUDIO_INVALID_STAMP 0 // RA2: the stamp of an event that is gone (0x00404DB0 clears it)
#define AUDIO_MAX_EVENTS 100 // RA2: the event pool holds 100 events (MemoryPoolCreate(100, 0x280) at 0x00403F1D)
#define EVENT_MIN_DELAY 33 // RA2: a delay below this is no delay (cmp 33 at 0x004048A3)
#define NUM_BUCKETS_PRI 7 // RA2: the priority buckets, 7 lists of 10 (0x348 bytes at 0x00830B58)
#define NUM_BUCKETS_VOL 10
#define VOLUME_QUANTIZE (AUDIO_LEVEL_MAX / NUM_BUCKETS_VOL)
#define MAX_AUDIO_EVENT_SAMPLES 32 // RA2: the sample slots of an event (item[] at +0x28 to +0xA8)

enum AudioEventState
{
	AUDIO_EVENT_NEW,
	AUDIO_EVENT_START_PLAYING,
	AUDIO_EVENT_WAITING,
	AUDIO_EVENT_PLAYING,
	AUDIO_EVENT_DONE
};

#define mAUDIO_EVENT_DEAD 0x01
#define mAUDIO_EVENT_PLAYING 0x02
#define mAUDIO_EVENT_NO_ATTACK 0x08
#define mAUDIO_EVENT_NO_DECAY 0x10
#define mAUDIO_EVENT_END 0x20
#define mAUDIO_EVENT_DO_END 0x40
#define mAUDIO_EVENT_ALLOCATED 0x80

#define AUDIO_EVENT_CTRL_LOOP 0x01
#define AUDIO_EVENT_CTRL_RANDOM 0x02
#define AUDIO_EVENT_CTRL_ALL 0x04
#define AUDIO_EVENT_CTRL_PREDELAY 0x08
#define AUDIO_EVENT_CTRL_INTERRUPT 0x10
#define AUDIO_EVENT_CTRL_ATTACK 0x20
#define AUDIO_EVENT_CTRL_DECAY 0x40
#define AUDIO_EVENT_CTRL_AMBIENT 0x80

/// <summary>
/// The settings and samples of one sound effect: what the INI keys of a sound effect fill in, and what
/// every event started for it reads.
/// </summary>
class AudioEventType
{
	public:
		ListNode Header; // RA2: +0x00; the node in the list of event types
		int SamplesOK; // RA2: +0x0C; cleared when a sample is missing
		unsigned Control;
		unsigned Type;
		AudioLevel Volume;
		unsigned : 32;
		unsigned : 32;
		int Priority;
		int Count;
		int Limit;
		int Loop;
		int Range;
		float MinVolume;
		unsigned MinDelay;
		unsigned MaxDelay;
		int MinFDelta;
		int MaxFDelta;
		int VShift;
		char Name[32];
		ListHead local;
		int lastFrame;
		unsigned lastBucketStamp;
		int maxPri;
		ListNode nodeVol; // RA2: +0xA4; the node in the priority bucket
		unsigned maxVol;
		int SampleIndex[32];
		int NumSamples;
		int Attack;
		int Decay;
		unsigned : 32;
		unsigned : 32;
};

/// <summary>
/// One sound that is playing or waiting to play; it is made from an event type and owns a channel while it plays.
/// </summary>
class AudioEvent
{
	public:
		ListNode nd;
		ListNode local;
		volatile int flags; // RA2: the audio callbacks change these members (flags, state, delay, timeOut, startSample): the executable reloads them after each call and store, which only compiles that way when they are declared volatile, as in the library's source
		volatile int state;
		int nextState;
		AudioEventType * eclass;
		AudioCacheItem * item[MAX_AUDIO_EVENT_SAMPLES];
		int numItems;
		int currentItem;
		AudioChannel * channel;
		unsigned : 32;
		AudioAttribs attribs;
		unsigned : 32;
		unsigned : 32;
		int stamp;
		volatile unsigned delay;
		volatile TimeStamp timeOut;
		AudioSample * volatile startSample;
		int fshift;
		int vshift;
		int adjustPriority;
		Lock paused;
		unsigned : 32;
		int sequence[MAX_AUDIO_EVENT_SAMPLES];
		int numSequence;
		int loopCount;
		int loadSequence[MAX_AUDIO_EVENT_SAMPLES];
		int numLoadSequence;
		int loadLoopCount;
		int loadNdx;
		int loadSaveNdx;
		AudioController * handle;
		unsigned : 32;
};

struct AudioDevice;
class AudioIDXData;

int __fastcall AudioEventSetUp(AudioDevice * device, AudioIDXData * idx); // RA2: 0x00403EB0
void AudioEventCloseDown(void); // RA2: 0x00403F80
void __fastcall eventReleaseChannel(AudioEvent * event); // RA2: 0x00404120
void AudioServiceAllEvents(void); // RA2: 0x004041B0
AudioSample * __fastcall eventFirstSample(AudioEvent * event); // RA2: 0x004046E0
AudioSample * __fastcall eventNextSample(AudioEvent * event); // RA2: 0x00404790
void __fastcall eventLoadAndLockSamples(AudioEvent * event); // RA2: 0x00404890
int __fastcall eventLoadSchedule(AudioEvent * event); // RA2: 0x00404B90
void __fastcall eventUnlockSamples(AudioEvent * event); // RA2: 0x00404D60
void __fastcall audioEventDestroy(AudioEvent * event); // RA2: 0x00404DB0
AudioEventType * __fastcall bucketGetWeaker(int priority); // RA2: 0x00404E00
void AudioKillAllEvents(void); // RA2: 0x00404E50
void AudioPauseAllEvents(void); // RA2: 0x00404FB0
void AudioResumeAllEvents(void); // RA2: 0x00405020
void AudioFlushAllDeadEvents(void); // RA2: 0x004050A0
int __fastcall AudioEventIsPaused(AudioEvent * event); // RA2: 0x004054A0
int __fastcall audioEventStart(AudioEvent * event); // RA2: 0x004054B0
void __fastcall AudioEventEnd(AudioEvent * event); // RA2: 0x00405590
void __fastcall AudioEventService(AudioEvent * event); // RA2: 0x004055D0
int __fastcall audioEventStop(AudioChannel * chan); // RA2: 0x00405A00
int __fastcall audioEventSampleDone(AudioChannel * chan); // RA2: 0x00405A10
int __fastcall audioEventNextSample(AudioChannel * chan); // RA2: 0x00405AD0
AudioChannel * __fastcall audioPrepareChannel(int pri, int flags); // RA2: 0x00405B60
void __fastcall AudioControllerStopLooping(AudioController * controller); // RA2: 0x00405E90; as AudioController::Stop, only for an event that loops without end
void __fastcall AudioEventJumpVolume(AudioEvent * event, int volume); // RA2: 0x004061A0; sets the volume at once
void __fastcall AudioEventJumpPan(AudioEvent * event, int pan); // RA2: 0x00406260; sets the pan at once
void __fastcall AudioEventSetAdjustDuration(AudioEvent * event, TimeStamp time); // RA2: 0x004062D0
int __fastcall AudioEventNeverEnds(AudioEvent * event); // RA2: 0x00406330
char const * __fastcall AudioEventName(AudioEvent * event); // RA2: 0x00406350
AudioEventType * __fastcall AudioEventClassFind(char const * name); // RA2: 0x00406370
void __fastcall Play_Named_Sound(char const * name, int volume); // RA2: 0x00406680; plays the event type of this name once
void AudioEventTurnOff(void); // RA2: 0x00406740
void AudioEventTurnOn(void); // RA2: 0x00406750
int AudioEventOn(void); // RA2: 0x00406760
void __fastcall AudioEventsDump(void (__fastcall * print)(char const * line)); // RA2: 0x00406770
