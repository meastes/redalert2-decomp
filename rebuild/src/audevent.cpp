// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
#include "always.h"
#include "geometry.h"
#include "dbgprint.h"
#include "coord.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include "random.h"
#include "_rand.h"
#include "voc.h"
#include "audevent.h"
#include "eventkeyword.h"
#include "audioctl.h"
#include "audcache.h"
#include "auddev.h"
#include "auddsnd.h"
#include "audmem.h"



static void eventWait(AudioEvent * event, TimeStamp delay, int nextState);


typedef struct
{
		ListHead bucketVol[NUM_BUCKETS_VOL];
} EClassBucket;











static EventKeyword PriorityKeywords[] = {
	{"LOWEST", 0},
	{"LOW", 1},
	{"NORMAL", 2},
	{"HIGH", 3},
	{"CRITICAL", 4},
	{NULL, 2}
};

static EventKeyword TypeKeywords[] = {
	{"AMBIENT", 0x1000},
	{"VIOLENT", 0x1},
	{"MOVEMENT", 0x2},
	{"QUIET", 0x4},
	{"LOUD", 0x8},
	{"GLOBAL", 0x10},
	{"SCREEN", 0x20},
	{"LOCAL", 0x40},
	{"PLAYER", 0x80},
	{"NORMAL", 0x0},
	{"GUN_SHY", 0x200},
	{"NOISE_SHY", 0x100},
	{"UNSHROUD", 0x400},
	{"SHROUD", 0x800},
	{NULL, 0}
};

static EventKeyword ControlKeywords[] = {
	{ "ALL", AUDIO_EVENT_CTRL_ALL },
	{ "LOOP", AUDIO_EVENT_CTRL_LOOP },
	{ "RANDOM", AUDIO_EVENT_CTRL_RANDOM },
	{ "PREDELAY", AUDIO_EVENT_CTRL_PREDELAY },
	{ "INTERRUPT", AUDIO_EVENT_CTRL_INTERRUPT },
	{ "ATTACK", AUDIO_EVENT_CTRL_ATTACK },
	{ "DECAY", AUDIO_EVENT_CTRL_DECAY },
	{ "AMBIENT", AUDIO_EVENT_CTRL_AMBIENT },
	{NULL, 0}
};


static ListHead audioEvents;
static AudioAttribs audioCompressionAttribs;
static ListHead audioEventClasses;
int AudioEventsCount;
int AudioEventsPeak;
static AudioCache * audioCache;
static AudioDevice * audioDevice;
static int audioEventsReady;
static int eventsOK;
static AudioMemoryPool * audioEventPool;
static int frameStamp;
static unsigned bucketStamp;
static EClassBucket eClassBucket[NUM_BUCKETS_PRI];
static int service_in;
static int eventsOn = 1;
static int audioStamp = AUDIO_INVALID_STAMP + 1;

static inline int AudioRandomPick(int lo, int hi)
{
	return NonCriticalRandomNumber(lo, hi);
}

static void eventLoadItem(AudioEvent * event, int sampleIndex)
{
	if ((event->item[event->numItems] = AudioCacheLoadItem(audioCache, event->eclass->SampleIndex[sampleIndex]))) {
		AudioCacheItemLock(event->item[event->numItems]);
		event->numItems++;
	}
}

/// <summary>
/// Starts the event layer on a device and the sample bag: empty lists, the sample cache and the event pool.
/// </summary>
int __fastcall AudioEventSetUp(AudioDevice * device, AudioIDXData * idx)
{
	ListInit(&audioEvents);
	ListInit(&audioEventClasses);
	AudioIDXData::Instance = idx;
	audioDevice = device;
	AudioAttribsInit(&audioCompressionAttribs);
	AudioEventsPeak = 0;
	AudioEventsCount = 0;
	audioEventsReady = 1;
	eventsOn = 1;
	if (AudioIDXData::Instance) {
		audioCache = AudioCacheCreate(AudioIDXData::Instance, 0x100000, 200, 0x2000);
		audioEventPool = MemoryPoolCreate(AUDIO_MAX_EVENTS, sizeof(AudioEvent));
		DebugString("event mempool size = %d\n", AUDIO_MAX_EVENTS * sizeof(AudioEvent));
	}
	if (audioCache && AudioIDXData::Instance && audioDevice && audioEventPool) {
		eventsOK = 1;
		return 1;
	}
	return 0;
}

static void eventDestroyAllClasses(void)
{
	AudioEventType * eclass;
	while ((eclass = (AudioEventType *)ListFirstItem(&audioEventClasses))) {
		ListNodeRemove(&eclass->Header);
		delete eclass;
	}
}

/// <summary>
/// Stops every event, drops the event types and frees the sample cache and the event pool.
/// </summary>
void AudioEventCloseDown(void)
{
	AudioKillAllEvents();
	AudioFlushAllDeadEvents();

	eventDestroyAllClasses();
	if (audioCache) {
		AudioCacheDestroy(audioCache);
		audioCache = NULL;
	}
	if (audioEventPool) {
		MemoryPoolDestroy(audioEventPool);
		audioEventPool = NULL;
	}
	eventsOK = 0;
	audioEventsReady = 0;
}

/// <summary>
/// Lets go of the channel an event plays on: stops it if it plays and clears the channel's event callbacks.
/// </summary>
void __fastcall eventReleaseChannel(AudioEvent * event)
{
	if (event->channel && event == (AudioEvent *)event->channel->Data) {
		if (event->flags & mAUDIO_EVENT_PLAYING) {
			AudioChannelStop(event->channel);
		}
		AudioChannelNoUse(event->channel);
		event->channel->Data = NULL;
		event->channel->CB_Stop = NULL;
		event->channel->CB_NextSample = NULL;
		event->channel->CB_SampleDone = NULL;
		event->channel->SfxAttribs = NULL;
		event->channel->CompAttribs = NULL;
		event->channel->FadeAttribs = NULL;
		event->channel = NULL;
	}
}

static void eventAddLocalSort(AudioEvent * newEvent)
{
	AudioEventType * eclass;
	ListNode *node, *head;
	AudioEvent * event;
	unsigned volume, newVolume;

	eclass = newEvent->eclass;
	newVolume = AudioAttribsGetVolume(&newEvent->attribs);
	head = &eclass->local;
	node = head->next;

	while (node != head) {
		int dif, slack;
		unsigned baseLev;
		event = (AudioEvent *)((char *)node - offsetof(AudioEvent, local));
		volume = AudioAttribsGetVolume(&event->attribs);
		baseLev = (unsigned)eclass->Volume.level >> AUDIO_LEVEL_SCALE;
		slack = baseLev / NUM_BUCKETS_VOL;
		dif = volume - newVolume;
		if ((dif >= 0 ? dif : -dif) < slack) {
			if (eclass->Control & AUDIO_EVENT_CTRL_INTERRUPT) {
				if (event->state != AUDIO_EVENT_NEW) {
					break;
				}
			} else {
				if (event->state == AUDIO_EVENT_NEW) {
					break;
				}
			}
		} else if (volume < newVolume) {
			break;
		}
		node = node->next;
	}

	ListNodeInit(&newEvent->local);
	ListNodeInsert(node, &newEvent->local);
	eclass->Count++;
	if (eclass->Limit && (eclass->Count > eclass->Limit)) {
		event = (AudioEvent *)((char *)head->prev - offsetof(AudioEvent, local));
		ListNodeRemove(&event->local);
		AudioEventStop(event);
		eclass->Count--;
	}
}

static void bucketReset(void)
{
	int i, k;
	EClassBucket * ecb;
	for (i = 0, ecb = eClassBucket; i < NUM_BUCKETS_PRI; i++, ecb++) {
		for (k = 0; k < NUM_BUCKETS_VOL; k++) {
			ListInit(&ecb->bucketVol[k]);
		}
	}
	bucketStamp++;
}


static void bucketAdd(AudioEvent * event)
{
	EClassBucket * ecb;
	AudioEventType * eclass;
	int pri;
	unsigned vol;

	eclass = event->eclass;
	pri = eclass->Priority + event->adjustPriority;
	vol = AudioAttribsGetVolume(&event->attribs) / VOLUME_QUANTIZE;
	ecb = &eClassBucket[pri];

	if (eclass->lastBucketStamp == bucketStamp) {
		if (pri > eclass->maxPri) {
			eclass->maxPri = pri;
			eclass->maxVol = vol;
			ListNodeRemove(&eclass->nodeVol);
			ListNodeInsert(&ecb->bucketVol[vol], &eclass->nodeVol);
		} else if (pri == eclass->maxPri) {
			if (vol > eclass->maxVol) {
				eclass->maxVol = vol;
				ListNodeRemove(&eclass->nodeVol);
				ListNodeInsert(&ecb->bucketVol[vol], &eclass->nodeVol);
			}
		}
	} else {
		eclass->lastBucketStamp = bucketStamp;
		eclass->maxPri = pri;
		eclass->maxVol = vol;
		ListNodeInit(&eclass->nodeVol);
		ListNodeInsert(&ecb->bucketVol[vol], &eclass->nodeVol);
	}
}


static int eventClassSacrifice(AudioEvent * newEvent)
{
	AudioEvent *event, *next, *head;
	AudioEventType * eclassSacrifice = NULL;
	int killed = 0;
	eclassSacrifice = bucketGetWeaker(newEvent->eclass->Priority + newEvent->adjustPriority);
	if (eclassSacrifice == NULL) {
		return 0;
	}
	ListNodeRemove(&eclassSacrifice->nodeVol);
	head = (AudioEvent *)&audioEvents;
	event = (AudioEvent *)head->nd.next;
	while (event != head) {
		next = (AudioEvent *)event->nd.next;
		if (event->eclass == eclassSacrifice) {
			AudioEventStop(event);
			audioEventDestroy(event);
			killed = 1;
		}
		event = next;
	}
	return killed;
}

/// <summary>
/// The once-a-frame service of the event layer: sorts each type's events by volume and enforces its limit, updates
/// every event, drops the dead ones, sets the volume compression for the loudness and starts the events waiting for a channel.
/// </summary>
void AudioServiceAllEvents(void)
{
	AudioEvent * event, * next, * head;
	enum { AVERAGE_LOUDNESS = 20, FULL_LOUDNESS = 100 };
	int loudness;
	int processedEvents = 0;

	if (!audioEventsReady || service_in) {
		return;
	}
	service_in = 1;
	frameStamp++;
	head = (AudioEvent *)&audioEvents;

	event = (AudioEvent *)head->nd.next;
	while (event != head) {
		if (event->eclass->lastFrame != frameStamp) {
			ListInit(&event->eclass->local);
			event->eclass->Count = 0;
			event->eclass->lastFrame = frameStamp;
		}
		AudioAttribsUpdate(&event->attribs);
		if (event->state != AUDIO_EVENT_DONE) {
			eventAddLocalSort(event);
		}
		event = (AudioEvent *)event->nd.next;
	}

	event = (AudioEvent *)head->nd.next;
	while (event != head) {
		if (event->state != AUDIO_EVENT_DONE) {
			processedEvents++;
		}
		AudioEventService(event);
		event = (AudioEvent *)event->nd.next;
	}

	event = (AudioEvent *)head->nd.next;
	loudness = 0;
	bucketReset();
	while (event != head) {
		next = (AudioEvent *)event->nd.next;
		if (!event->channel || event != (AudioEvent *)event->channel->Data) {
			AudioEventStop(event);
		}
		if (event->flags & mAUDIO_EVENT_DEAD) {
			audioEventDestroy(event);
		} else {
			if (!AudioEventIsPaused(event)) {
				loudness += AUDIO_LEVEL_APPLY(&event->eclass->Volume, AVERAGE_LOUDNESS);
			}
			bucketAdd(event);
		}
		event = next;
	}

	if (loudness > FULL_LOUDNESS) {
		AudioLevelAdjust(&audioCompressionAttribs.VolumeLevel, (unsigned)(AUDIO_LEVEL_MAX * FULL_LOUDNESS) / loudness);
	} else {
		AudioLevelAdjust(&audioCompressionAttribs.VolumeLevel, AUDIO_LEVEL_MAX);
	}
	AudioAttribsUpdate(&audioCompressionAttribs);

	event = (AudioEvent *)head->nd.next;
	while (event != head) {
		next = (AudioEvent *)event->nd.next;
		if (event->state == AUDIO_EVENT_START_PLAYING && !AudioEventIsPaused(event)) {
			eventLoadAndLockSamples(event);
			while (!(event->startSample = eventFirstSample(event))) {
				if (eventClassSacrifice(event)) {
					next = (AudioEvent *)event->nd.next;
					eventLoadAndLockSamples(event);
				} else {
					break;
				}
			}
			if (!(event->startSample = eventFirstSample(event)) || !audioEventStart(event)) {
				AudioEventStop(event);
				audioEventDestroy(event);
			}
		}
		event = next;
	}

	service_in = 0;
}


/// <summary>
/// The first sample an event plays: sets up the order of the samples between attack and decay and returns the attack
/// sample, or the next one when the event has no attack.
/// </summary>
AudioSample * __fastcall eventFirstSample(AudioEvent * event)
{
	int j = 0;
	int i;

	if ((event->numSequence = event->numItems) == 0 || event->flags & mAUDIO_EVENT_END) {
		return NULL;
	}
	if (event->eclass->MinDelay < EVENT_MIN_DELAY) {
		if (event->eclass->Control & AUDIO_EVENT_CTRL_ATTACK) {
			j++;
			event->numSequence--;
			event->currentItem = 0;
		} else {
			event->currentItem = -1;
		}
		if (event->eclass->Control & AUDIO_EVENT_CTRL_DECAY) {
			event->numSequence--;
		}
		if (event->numSequence <= 0) {
			event->eclass->SamplesOK = 0;
			return NULL;
		}
		for (i = 0; i < event->numSequence; i++, j++) {
			event->sequence[i] = j;
		}
		if (event->flags & mAUDIO_EVENT_NO_ATTACK || !(event->eclass->Control & AUDIO_EVENT_CTRL_ATTACK)) {
			return eventNextSample(event);
		}
	}
	return AudioCacheItemSample(event->item[0]);
}

/// <summary>
/// The next sample an event plays: the next one in order or a random one of the rest, the loop's first sample again
/// when the sequence is used up, and last the decay sample.
/// </summary>
AudioSample * __fastcall eventNextSample(AudioEvent * event)
{
	int i;
	int control;

	control = event->eclass->Control;
	if (event->eclass->MinDelay >= EVENT_MIN_DELAY && !(event->flags & mAUDIO_EVENT_END)) {
		return NULL;
	}
	if (event->numSequence == 0 || event->flags & mAUDIO_EVENT_END) {
		if (control & AUDIO_EVENT_CTRL_LOOP && !(event->flags & mAUDIO_EVENT_END)) {
			if (!event->eclass->Loop || (event->loopCount < event->eclass->Loop - 1)) {
				event->loopCount++;
				return eventFirstSample(event);
			}
		}
		if ((!(event->eclass->Control & AUDIO_EVENT_CTRL_DECAY) || event->flags & mAUDIO_EVENT_NO_DECAY) || event->numItems < 1) {
			return NULL;
		}
		event->flags |= mAUDIO_EVENT_NO_DECAY;
		return AudioCacheItemSample(event->item[event->numItems - 1]);
	}
	if (control & AUDIO_EVENT_CTRL_RANDOM) {
		int ndx;

		ndx = AudioRandomPick(0, event->numSequence - 1);
		event->currentItem = event->sequence[ndx];
		for (i = ndx; i < event->numSequence - 1; i++) {
			event->sequence[i] = event->sequence[i + 1];
		}
	} else {
		event->currentItem++;
	}
	event->numSequence--;
	return AudioCacheItemSample(event->item[event->currentItem]);
}

/// <summary>
/// Loads and locks the samples an event needs from the cache: all of them at once, or one at a time
/// when the type has a delay between its samples.
/// </summary>
void __fastcall eventLoadAndLockSamples(AudioEvent * event)
{
	int i, ndx;
	int control;
	AudioEventType * eclass;

	control = event->eclass->Control;
	eclass = event->eclass;
	if (event->numItems) {
		if (eclass->MinDelay >= EVENT_MIN_DELAY) {
			eventUnlockSamples(event);
		} else {
			return;
		}
	}
	if (eclass->MinDelay < EVENT_MIN_DELAY) {
		if (eclass->Attack) {
			eventLoadItem(event, AudioRandomPick(0, eclass->Attack - 1));
		}
		if (eclass->Control & AUDIO_EVENT_CTRL_ALL) {
			for (i = eclass->Attack; i < eclass->NumSamples - eclass->Decay; i++) {
				eventLoadItem(event, i);
			}
		} else if (eclass->Control & AUDIO_EVENT_CTRL_RANDOM) {
			i = AudioRandomPick(eclass->Attack, eclass->NumSamples - eclass->Decay - 1);
			eventLoadItem(event, i);
		} else {
			eventLoadItem(event, eclass->Attack);
		}
		if (eclass->Decay) {
			eventLoadItem(event, AudioRandomPick(eclass->NumSamples - eclass->Decay, eclass->NumSamples - 1));
		}
	} else {
		int decay_ndx = -1;

		if (eclass->Decay && eclass->Control & AUDIO_EVENT_CTRL_DECAY) {
			decay_ndx = AudioRandomPick(eclass->NumSamples - eclass->Decay, eclass->NumSamples - 1);
		}
		if ((ndx = eventLoadSchedule(event)) >= 0) {
			eventLoadItem(event, ndx);
			if (decay_ndx != -1) {
				eventLoadItem(event, decay_ndx);
			}
		} else {
			if (decay_ndx != -1 && !(event->flags & mAUDIO_EVENT_NO_DECAY)) {
				eventLoadItem(event, decay_ndx);
				event->flags |= mAUDIO_EVENT_NO_DECAY;
			}
		}
	}
}

/// <summary>
/// The index of the next sample to load for an event with delays between its samples, or -1 when the
/// loops are used up; makes the order of the samples for the next round when the last one is used.
/// </summary>
int __fastcall eventLoadSchedule(AudioEvent * event)
{
	AudioEventType * eclass;
	int control;
	int i;
	int item_ndx;

	eclass = event->eclass;
	control = eclass->Control;
	if (event->numLoadSequence == event->loadNdx) {
		int ndx = 0;

		if ((!(eclass->Control & AUDIO_EVENT_CTRL_LOOP) && event->loadLoopCount == 1)
			|| ((eclass->Control & AUDIO_EVENT_CTRL_LOOP) && eclass->Loop && (event->loadLoopCount) >= eclass->Loop)) {
			return -1;
		}
		if (eclass->Attack && !(event->flags & mAUDIO_EVENT_NO_ATTACK)) {
			event->loadSequence[ndx++] = AudioRandomPick(0, eclass->Attack - 1);
		}
		if (eclass->Control & AUDIO_EVENT_CTRL_ALL) {
			for (i = eclass->Attack; i < eclass->NumSamples - eclass->Decay; i++) {
				event->loadSequence[ndx++] = i;
			}
		} else {
			if (eclass->Control & AUDIO_EVENT_CTRL_RANDOM) {
				if (event->loadLoopCount == 0) {
					i = AudioRandomPick(eclass->Attack, eclass->NumSamples - (eclass->Decay + 1));
					event->loadSaveNdx = i;
				} else {
					i = event->loadSaveNdx;
				}
			} else {
				i = eclass->Attack;
			}
			event->loadSequence[ndx++] = i;
		}
		event->numLoadSequence = ndx;
		event->loadNdx = 0;
		event->loadLoopCount++;
	}
	if (control & AUDIO_EVENT_CTRL_RANDOM
		&& ((!eclass->Attack || (event->flags & mAUDIO_EVENT_NO_ATTACK)) || event->loadNdx > 0)) {
		int ndx, currentSampleIndex;

		ndx = AudioRandomPick((!eclass->Attack || (event->flags & mAUDIO_EVENT_NO_ATTACK)) ? 0 : 1, event->numLoadSequence - 1);
		item_ndx = currentSampleIndex = event->loadSequence[ndx];
		for (i = ndx; i < event->numLoadSequence - 1; i++) {
			event->loadSequence[i] = event->loadSequence[i + 1];
		}
		event->numLoadSequence--;
	} else {
		item_ndx = event->loadSequence[event->loadNdx];
		event->loadNdx++;
	}
	return item_ndx;
}

/// <summary>
/// Unlocks the cached samples an event holds.
/// </summary>
void __fastcall eventUnlockSamples(AudioEvent * event)
{
	int i;

	for (i = 0; i < event->numItems; i++) {
		AudioCacheItemUnlock(event->item[i]);
		event->item[i] = NULL;
	}
	event->numItems = 0;
}

/// <summary>
/// Takes a dead event out of the list and gives its memory back: to the pool, or to the heap when it was allocated there.
/// </summary>
void __fastcall audioEventDestroy(AudioEvent * event)
{
	ListNodeRemove(&event->nd);
	event->stamp = AUDIO_INVALID_STAMP;
	if (event->flags & mAUDIO_EVENT_ALLOCATED) {
		delete event;
	} else {
		MemoryPoolReturnItem(audioEventPool, event);
	}
	AudioEventsCount--;
}

/// <summary>
/// The first event type in the buckets of a lower priority than this one: the type to sacrifice for a new event.
/// </summary>
AudioEventType * __fastcall bucketGetWeaker(int pri)
{
	EClassBucket * ecb;
	ListNode * node;
	int i, v;

	for (i = 0, ecb = eClassBucket; i < pri; i++, ecb++) {
		for (v = 0; v < NUM_BUCKETS_VOL; v++) {
			node = ListFirstItem(&ecb->bucketVol[v]);
			if (node) {
				AudioEventType * event = (AudioEventType *)node;
				event = (AudioEventType *)((unsigned)event - ((unsigned)&event->nodeVol - (unsigned)event));
				return event;
			}
		}
	}
	return NULL;
}


/// <summary>
/// Stops and frees every event.
/// </summary>
void AudioKillAllEvents(void)
{
	AudioEvent *event, *next, *head;

	if (!audioEventsReady) {
		return;
	}
	head = (AudioEvent *)&audioEvents;
	event = (AudioEvent *)head->nd.next;
	while (event != head) {
		next = (AudioEvent *)event->nd.next;
		AudioEventStop(event);
		audioEventDestroy(event);
		event = next;
	}
}

/// <summary>
/// Pauses every event that is not paused yet; a waiting event keeps the time it has left.
/// </summary>
void AudioPauseAllEvents(void)
{
	AudioEvent *event, *head;

	if (!audioEventsReady) {
		return;
	}
	head = (AudioEvent *)&audioEvents;
	event = (AudioEvent *)head->nd.next;
	while (event != head) {
		if (!AudioEventIsPaused(event)) {
			if (event->state == AUDIO_EVENT_WAITING) {
				event->timeOut -= AudioGetTime();
			}
		}
		LockAcquire(&event->paused);
		event = (AudioEvent *)event->nd.next;
	}
}

/// <summary>
/// Resumes every paused event; a waiting event gets its time back.
/// </summary>
void AudioResumeAllEvents(void)
{
	AudioEvent *event, *head;

	if (!audioEventsReady) {
		return;
	}
	head = (AudioEvent *)&audioEvents;
	event = (AudioEvent *)head->nd.next;
	while (event != head) {
		if (AudioEventIsPaused(event)) {
			LockRelease(&event->paused);
			if (!AudioEventIsPaused(event)) {
				if (event->state == AUDIO_EVENT_WAITING) {
					event->timeOut += AudioGetTime();
				}
			}
		}
		event = (AudioEvent *)event->nd.next;
	}
}

/// <summary>
/// Frees the events that are dead.
/// </summary>
void AudioFlushAllDeadEvents(void)
{
	AudioEvent *event, *next, *head;

	if (!audioEventsReady) {
		return;
	}
	head = (AudioEvent *)&audioEvents;
	event = (AudioEvent *)head->nd.next;
	while (event != head) {
		next = (AudioEvent *)event->nd.next;
		if (event->flags & mAUDIO_EVENT_DEAD) {
			audioEventDestroy(event);
		}
		event = next;
	}
}

/// <summary>
/// The range of an event type, in cells.
/// </summary>
int __fastcall AudioEventTypeRange(AudioEventType * type)
{
	if (!audioEventsReady) {
		return 0;
	}
	return type->Range;
}

/// <summary>
/// The lowest volume an event type is played at.
/// </summary>
float __fastcall AudioEventTypeMinVolume(AudioEventType * type)
{
	if (!audioEventsReady) {
		return 0.0f;
	}
	return type->MinVolume;
}

/// <summary>
/// The "Type" flags of an event type.
/// </summary>
unsigned int __fastcall AudioEventTypeGetType(AudioEventType * type)
{
	if (!audioEventsReady) {
		return 0;
	}
	return type->Type;
}

/// <summary>
/// The name of an event type.
/// </summary>
char const * __fastcall AudioEventTypeName(AudioEventType * type)
{
	if (!audioEventsReady) {
		return "no events";
	}
	return type->Name;
}

/// <summary>
/// Makes an event of a type, from the pool or from the heap when the pool is empty; NULL when the layer is off or the type has a missing sample.
/// </summary>
AudioEvent * __fastcall AudioEventCreate(AudioEventType * eclass)
{
	AudioEvent * event;
	int allocated = 0;

	if (!eventsOn || !eventsOK) {
		return NULL;
	}
	if (!eclass->SamplesOK) {
		return NULL;
	}
	if (!(event = (AudioEvent *)MemoryPoolGetItem(audioEventPool))) {
		AudioFlushAllDeadEvents();
		if (!(event = (AudioEvent *)MemoryPoolGetItem(audioEventPool))) {
			event = (AudioEvent *)operator new(sizeof(AudioEvent));
			allocated = 1;
			if (!event) {
				return NULL;
			}
		}
	}
	AudioEventsCount++;
	if (AudioEventsCount > AudioEventsPeak) {
		AudioEventsPeak = AudioEventsCount;
	}
	memset(event, 0, sizeof(AudioEvent));
	event->eclass = eclass;
	ListNodeInit(&event->nd);
	event->state = AUDIO_EVENT_NEW;
	event->adjustPriority = 0;
	event->numItems = event->numSequence = event->numLoadSequence = 0;
	AudioAttribsInit(&event->attribs);
	LockInit(&event->paused);
	ListNodeInsert((ListNode *)&audioEvents, &event->nd);
	event->stamp = audioStamp++;
	if (allocated) {
		event->flags |= mAUDIO_EVENT_ALLOCATED;
	}
	return event;
}

/// <summary>
/// Marks an event as past its attack.
/// </summary>
void __fastcall AudioEventSetFlag8(AudioEvent * event)
{
	event->flags |= mAUDIO_EVENT_NO_ATTACK;
}

/// <summary>
/// Stops an event for good: lets go of its channel and samples and marks it dead.
/// </summary>
void __fastcall AudioEventStop(AudioEvent * event)
{
	if (event->flags & mAUDIO_EVENT_DEAD) {
		return;
	}
	eventReleaseChannel(event);
	eventUnlockSamples(event);
	event->state = AUDIO_EVENT_DONE;
	event->stamp = AUDIO_INVALID_STAMP;
	event->handle = NULL;
	event->flags |= mAUDIO_EVENT_DEAD;
}

/// <summary>
/// Pauses one event; a waiting event keeps the time it has left.
/// </summary>
void __fastcall AudioEventPause(AudioEvent * event)
{
	if (!AudioEventIsPaused(event)) {
		if (event->state == AUDIO_EVENT_WAITING) {
			event->timeOut -= AudioGetTime();
		}
	}
	LockAcquire(&event->paused);
}

/// <summary>
/// Resumes one paused event; a waiting event gets its time back.
/// </summary>
void __fastcall AudioEventResume(AudioEvent * event)
{
	if (AudioEventIsPaused(event)) {
		LockRelease(&event->paused);
		if (!AudioEventIsPaused(event)) {
			if (event->state == AUDIO_EVENT_WAITING) {
				event->timeOut += AudioGetTime();
			}
		}
	}
}

/// <summary>
/// The pause count of an event.
/// </summary>
int __fastcall AudioEventIsPaused(AudioEvent * event)
{
	return event->paused.count;
}

/// <summary>
/// Starts the sample an event has ready on its channel at the event's pitch and volume shift; undoes it and returns 0 when the channel does not start.
/// </summary>
int __fastcall audioEventStart(AudioEvent * event)
{
	AudioSample * sample;
	unsigned volume;

	if (event != (AudioEvent *)event->channel->Data) {
		return 0;
	}
	AudioChannelLock(event->channel);
	sample = (AudioSample *)event->startSample;
	event->flags |= mAUDIO_EVENT_NO_ATTACK;
	AudioChannelSetSample(event->channel, sample);
	AudioLevelSet(&event->channel->ChannelAttribs.PitchLevel, event->fshift);
	volume = AUDIO_LEVEL_MAX;
	if (event->vshift > 0) {
		volume -= (unsigned)(AUDIO_LEVEL_MAX * event->vshift) / 100;
	}
	AudioLevelSet(&event->channel->ChannelAttribs.VolumeLevel, volume);
	event->state = AUDIO_EVENT_PLAYING;
	event->flags |= mAUDIO_EVENT_PLAYING;
	event->startSample = NULL;
	if (AudioChannelStart(event->channel) != vNO_ERROR) {
		event->state = AUDIO_EVENT_START_PLAYING;
		event->startSample = sample;
		event->flags &= ~mAUDIO_EVENT_PLAYING;
		AudioChannelUnlock(event->channel);
		return 0;
	}
	AudioChannelUnlock(event->channel);
	return 1;
}

/// <summary>
/// Asks an event to end: it plays its decay and stops instead of looping on.
/// </summary>
void __fastcall AudioEventEnd(AudioEvent * event)
{
	if (event->flags & mAUDIO_EVENT_END) {
		return;
	}
	if (event->channel && event == (AudioEvent *)event->channel->Data) {
		AudioChannelLock(event->channel);
		event->flags |= mAUDIO_EVENT_END | mAUDIO_EVENT_DO_END;
		AudioChannelUnlock(event->channel);
	} else {
		event->flags |= mAUDIO_EVENT_END | mAUDIO_EVENT_DO_END;
	}
}

static int audioEventPrep(AudioEvent * event)
{
	AudioEventType * eclass;
	int delay;

	eclass = event->eclass;
	if (eclass->NumSamples == 0 || !eclass->SamplesOK) {
		return 0;
	}
	event->loopCount = 0;
	if (!(event->channel = audioPrepareChannel(eclass->Priority + event->adjustPriority, 0))) {
		return 0;
	}
	event->fshift = 100 + AudioRandomPick(eclass->MinFDelta, eclass->MaxFDelta);
	event->vshift = AudioRandomPick(0, eclass->VShift);
	AudioChannelUse(event->channel);
	event->channel->Data = event;
	event->channel->CB_NextSample = audioEventNextSample;
	event->channel->CB_SampleDone = audioEventSampleDone;
	event->channel->CB_Stop = audioEventStop;
	event->state = AUDIO_EVENT_START_PLAYING;
	event->channel->SfxAttribs = &event->attribs;
	event->channel->CompAttribs = &audioCompressionAttribs;
	event->channel->FadeAttribs = AudioGroupAttribs1;
	if (eclass->Control & (AUDIO_EVENT_CTRL_PREDELAY | AUDIO_EVENT_CTRL_AMBIENT)) {
		delay = AudioRandomPick((eclass->Control & AUDIO_EVENT_CTRL_AMBIENT) ? EVENT_MIN_DELAY : eclass->MinDelay, eclass->MaxDelay);
		if (delay >= EVENT_MIN_DELAY) {
			eventWait(event, delay, AUDIO_EVENT_START_PLAYING);
		}
	}
	return 1;
}


static void eventCheckStart(AudioEvent * event, int succeeded)
{
	if (!succeeded) {
		AudioEventStop(event);
	}
}

/// <summary>
/// Moves one event along its states: gets it a channel, starts it when its wait is over, ends it when asked to, frees it when done.
/// </summary>
void __fastcall AudioEventService(AudioEvent * event)
{
	if (!eventsOn) {
		event->state = AUDIO_EVENT_DONE;
	}

	for (;;) {
		switch (event->state) {
		case AUDIO_EVENT_WAITING:
			if (!(event->flags & mAUDIO_EVENT_DO_END)) {
				if (!AudioEventIsPaused(event) && AudioGetTime() > event->timeOut) {
					event->state = event->nextState;
					continue;
				}
				return;
			}
			break;

		case AUDIO_EVENT_NEW:
			eventCheckStart(event, audioEventPrep(event));
			return;

		case AUDIO_EVENT_PLAYING:
			if ((event->eclass->Control & AUDIO_EVENT_CTRL_LOOP) && !event->eclass->Loop
				&& !(event->flags & mAUDIO_EVENT_END)) {
				AudioController * controller = event->handle;
				if (!controller || AudioControllerGetEvent(controller) != event) {
					AudioEventStop(event);
				}
			}
			if (!(event->flags & mAUDIO_EVENT_DO_END)) {
				return;
			}
			break;

		case AUDIO_EVENT_DONE:
			AudioEventStop(event);
			return;

		default:
			return;
		}

		{
			AudioChannel * channel = event->channel;
			if (channel) {
				AudioChannelLock(channel);
				event->flags &= ~mAUDIO_EVENT_DO_END;
				AudioChannelStop(event->channel);
				if ((event->eclass->Control & AUDIO_EVENT_CTRL_DECAY) && !(event->flags & mAUDIO_EVENT_NO_DECAY)) {
					event->state = AUDIO_EVENT_START_PLAYING;
					event->startSample = eventNextSample(event);
					if (!AudioEventIsPaused(event)) {
						eventCheckStart(event, audioEventStart(event));
					}
				}
				AudioChannelUnlock(channel);
			} else {
				event->flags &= ~mAUDIO_EVENT_DO_END;
			}
		}
		return;
	}
}


/// <summary>
/// The channel callback for a stopped channel: the event is done.
/// </summary>
int __fastcall audioEventStop(AudioChannel * chan)
{
	AudioEvent * event = (AudioEvent *)chan->Data;

	event->state = AUDIO_EVENT_DONE;
	return vNO_ERROR;
}

static void eventWait(AudioEvent * event, TimeStamp delay, int nextState)
{
	event->nextState = nextState;
	if (AudioEventIsPaused(event)) {
		event->timeOut = delay;
	} else {
		event->timeOut = AudioGetTime() + delay;
	}
	event->state = AUDIO_EVENT_WAITING;
}

/// <summary>
/// The channel callback for a sample that is played: the event waits out its delay, plays on, or is done.
/// </summary>
int __fastcall audioEventSampleDone(AudioChannel * chan)
{
	AudioEvent * event = (AudioEvent *)chan->Data;
	int new_state;

	event->flags &= ~mAUDIO_EVENT_PLAYING;
	if (event->state == AUDIO_EVENT_DONE) {
		return vNO_ERROR;
	}
	if (event->startSample || event->numLoadSequence || event->delay) {
		new_state = AUDIO_EVENT_START_PLAYING;
	} else {
		new_state = AUDIO_EVENT_DONE;
		event->delay = 0;
	}
	if (event->delay) {
		eventWait(event, event->delay, new_state);
		event->delay = 0;
	} else {
		event->state = new_state;
	}
	return vNO_ERROR;
}

/// <summary>
/// The channel callback that asks for the next sample: hands it on, or keeps it for after the delay between samples.
/// </summary>
int __fastcall audioEventNextSample(AudioChannel * chan)
{
	AudioEvent * event = (AudioEvent *)chan->Data;
	AudioSample * sample;
	unsigned delay;

	sample = eventNextSample(event);
	if (event->eclass->MaxDelay < EVENT_MIN_DELAY) {
		AudioChannelSetSample(chan, sample);
		return vNO_ERROR;
	}
	AudioChannelSetSample(chan, NULL);
	if ((event->eclass->Control & (AUDIO_EVENT_CTRL_PREDELAY | AUDIO_EVENT_CTRL_AMBIENT)) && !sample && !event->numLoadSequence) {
		return vNO_ERROR;
	}
	delay = AudioRandomPick(event->eclass->MinDelay, event->eclass->MaxDelay);
	if (delay < EVENT_MIN_DELAY) {
		AudioChannelSetSample(chan, sample);
		return vNO_ERROR;
	}
	event->delay = delay;
	event->startSample = sample;
	return vNO_ERROR;
}

/// <summary>
/// A channel for an event of a priority: a free one, or one that plays something of the same or a lower priority.
/// </summary>
AudioChannel * __fastcall audioPrepareChannel(int pri, int flags)
{
	AudioChannel * chan = NULL;

	if (audioDevice && (chan = AudioDeviceGetChannel(audioDevice, AUDIO_CHANNEL_TYPE_STD))) {
		if (!(chan->Control.Status & (mAUDIO_CTRL_PLAYING | mAUDIO_CTRL_PAUSED | mAUDIO_CTRL_INUSE)) || chan->Control.Priority <= pri) {
			AudioChannelStop(chan);
			chan->GroupAttribs = AudioSoundAttribs;
			chan->FadeAttribs = AudioGroupAttribs1;
			chan->Control.Priority = pri;
			if (flags & 1) {
				chan->Control.LoopCount = AUDIO_CTRL_LOOP_FOREVER;
			} else {
				chan->Control.LoopCount = 0;
			}
		} else {
			return NULL;
		}
	}
	return chan;
}


/// <summary>
/// Forgets the event of a handle and its type; the sound it started keeps playing.
/// </summary>
void AudioController::Init(void)
{
	AudioIndex = (void **)&AudioIDXData::Instance;
	Event = NULL;
	Stamp = 0;
	EventType = NULL;
}

/// <summary>
/// Stops the sound of a handle and detaches it from the audio library.
/// </summary>
void AudioController::Release(void)
{
	Stop();
	AudioIndex = NULL;
}

/// <summary>
/// Stops the event of a handle if it is still the same one, and forgets the event and its type.
/// </summary>
void AudioController::Stop(void)
{
	AudioEvent * event = AudioControllerGetEvent(this);

	if (event) {
		AudioEventStop(event);
		Event = NULL;
	}
	EventType = NULL;
}

/// <summary>
/// As Stop, but only for an event that loops without end.
/// </summary>
void __fastcall AudioControllerStopLooping(AudioController * controller)
{
	AudioEvent * event = AudioControllerGetEvent(controller);

	if (event && AudioEventTypeLoopsForever(event->eclass)) {
		AudioEventStop(event);
		controller->Event = NULL;
	}
	controller->EventType = NULL;
}

/// <summary>
/// Ends the event of a handle if it is still the same one (it plays its decay); forgets the event and its type.
/// </summary>
void AudioController::End(void)
{
	AudioEvent * event = AudioControllerGetEvent(this);

	if (event) {
		AudioEventEnd(event);
		Event = NULL;
	}
	EventType = NULL;
}

/// <summary>
/// As End, but only for an event that loops without end.
/// </summary>
void AudioController::EndLooping(void)
{
	AudioEvent * event = AudioControllerGetEvent(this);

	if (event && AudioEventTypeLoopsForever(event->eclass)) {
		AudioEventEnd(event);
		Event = NULL;
	}
	EventType = NULL;
}

/// <summary>
/// Points a handle at an event and its type, and the event back at the handle.
/// </summary>
void __fastcall AudioController::SetEvent(AudioEvent * event, VocClass * type)
{
	if (AudioIndex != (void **)&AudioIDXData::Instance) {
		return;
	}
	EventType = type;
	if ((Event = event)) {
		Stamp = event->stamp;
		if (!type) {
			EventType = (VocClass *)event->eclass;
		}
		event->handle = this;
	}
}

/// <summary>
/// The event of a handle while it is still good: the event is gone, or made again for another type, when its stamp or type differ.
/// </summary>
AudioEvent * __fastcall AudioControllerGetEvent(AudioController * controller)
{
	AudioEvent * event;

	if (!audioEventsReady || controller->AudioIndex != (void **)&AudioIDXData::Instance) {
		return NULL;
	}
	if ((event = controller->Event)) {
		if ((AudioEventType *)controller->EventType != event->eclass || controller->Stamp != (unsigned)event->stamp) {
			controller->Event = event = NULL;
		}
	}
	return event;
}

/// <summary>
/// The event type of a handle.
/// </summary>
AudioEventType * __fastcall AudioControllerGetType(AudioController * controller)
{
	if (!audioEventsReady || controller->AudioIndex != (void **)&AudioIDXData::Instance) {
		return NULL;
	}
	return (AudioEventType *)controller->EventType;
}

static unsigned eventCalcVolume(AudioEvent * event, int volume)
{
	if (volume > AUDIO_PAN_RIGHT) {
		volume = AUDIO_LEVEL_MAX;
	} else if (volume < 0) {
		volume = 0;
	}
	return AUDIO_LEVEL_APPLY(&event->eclass->Volume, volume);
}

/// <summary>
/// Sets the volume of an event at once, scaled by its type's volume.
/// </summary>
void __fastcall AudioEventJumpVolume(AudioEvent * event, int volume)
{
	AudioLevelSet(&event->attribs.VolumeLevel, eventCalcVolume(event, volume));
	AudioLevelUpdate(&event->attribs.VolumeLevel);
}

/// <summary>
/// Sets the volume of an event: fading to it while it plays, at once before.
/// </summary>
void __fastcall AudioEventSetVolume(AudioEvent * event, int volume)
{
	if (event->state == AUDIO_EVENT_PLAYING) {
		AudioLevelAdjust(&event->attribs.VolumeLevel, eventCalcVolume(event, volume));
	} else {
		AudioEventJumpVolume(event, volume);
	}
}

/// <summary>
/// Sets the pan of an event at once.
/// </summary>
void __fastcall AudioEventJumpPan(AudioEvent * event, int pan)
{
	if (pan > AUDIO_PAN_RIGHT) {
		pan = AUDIO_PAN_RIGHT;
	} else if (pan < AUDIO_PAN_LEFT) {
		pan = AUDIO_PAN_LEFT;
	}
	AudioLevelSet(&event->attribs.PanPosition, pan);
}

/// <summary>
/// Sets the pan of an event: moving to it while it plays, at once before.
/// </summary>
void __fastcall AudioEventSetPan(AudioEvent * event, int pan)
{
	if (event->state == AUDIO_EVENT_PLAYING) {
		if (pan > AUDIO_PAN_RIGHT) {
			pan = AUDIO_PAN_RIGHT;
		} else if (pan < AUDIO_PAN_LEFT) {
			pan = AUDIO_PAN_LEFT;
		}
		AudioLevelAdjust(&event->attribs.PanPosition, pan);
	} else {
		AudioEventJumpPan(event, pan);
	}
}

/// <summary>
/// Sets how long the volume, pitch and pan of an event take to reach a new level.
/// </summary>
void __fastcall AudioEventSetAdjustDuration(AudioEvent * event, TimeStamp time)
{
	AudioLevelSetDuration(&event->attribs.VolumeLevel, time, AUDIO_LEVEL_MAX);
	AudioLevelSetDuration(&event->attribs.PitchLevel, time, AUDIO_LEVEL_MAX);
	AudioLevelSetDuration(&event->attribs.PanPosition, time, AUDIO_LEVEL_MAX);
}

/// <summary>
/// The event type of an event.
/// </summary>
AudioEventType * __fastcall AudioEventGetType(AudioEvent * event)
{
	return event->eclass;
}

/// <summary>
/// Whether an event loops without end.
/// </summary>
int __fastcall AudioEventNeverEnds(AudioEvent * event)
{
	return AudioEventTypeLoopsForever(event->eclass);
}

/// <summary>
/// The name of an event's type.
/// </summary>
char const * __fastcall AudioEventName(AudioEvent * event)
{
	if (!audioEventsReady) {
		return "no events";
	}
	if (!event->eclass) {
		return "unknown";
	}
	return event->eclass->Name;
}

/// <summary>
/// The event type of this name, or NULL.
/// </summary>
AudioEventType * __fastcall AudioEventClassFind(char const * name)
{
	AudioEventType * eclass;

	if (audioEventsReady) {
		for (eclass = (AudioEventType *)ListFirstItem(&audioEventClasses); eclass; eclass = (AudioEventType *)ListNextItem(&eclass->Header)) {
			if (!stricmp(name, eclass->Name)) {
				return eclass;
			}
		}
	}
	return NULL;
}

/// <summary>
/// The event type of this name; a new one with the defaults is made when there is none.
/// </summary>
AudioEventType * __fastcall AudioEventTypeFind(char const * name)
{
	AudioEventType * eclass;

	if (!audioEventsReady) {
		return NULL;
	}
	eclass = (AudioEventType *)ListFirstItem(&audioEventClasses);
	while (eclass) {
		if (!stricmp(name, eclass->Name)) {
			break;
		}
		eclass = (AudioEventType *)ListNextItem(&eclass->Header);
	}
	if (eclass) {
		return eclass;
	}
	eclass = new AudioEventType;
	memset(eclass, 0, sizeof(AudioEventType));
	memset(eclass, 0, sizeof(AudioEventType));
	ListNodeInit(&eclass->Header);
	eclass->SamplesOK = 1;
	eclass->Control = 0;
	eclass->Type = 0x20;
	eclass->Limit = 3;
	eclass->Priority = 2;
	eclass->Range = 10;
	eclass->MinVolume = 0.2f;
	AudioLevelInit(&eclass->Volume, AUDIO_LEVEL_MAX);
	strncpy(eclass->Name, name, 31);
	eclass->Name[31] = 0;
	ListNodeInsert((ListNode *)&audioEventClasses, &eclass->Header);
	return eclass;
}

/// <summary>
/// Adds a sample of the bag to an event type; a missing sample is logged and clears SamplesOK.
/// </summary>
int __fastcall AudioEventTypeAddSample(AudioEventType * type, char const * sample)
{
	int index;

	if (type->NumSamples == MAX_AUDIO_EVENT_SAMPLES) {
		return 0;
	}
	if (audioEventsReady && AudioIDXData::Instance) {
		while (*sample == '$' || *sample == '#') {
			sample++;
		}
		index = AudioIDXData::Instance->FindSampleIndex(sample);
		if (index != -1) {
			type->SampleIndex[type->NumSamples] = index;
			type->NumSamples++;
			return 1;
		}
		DebugString("Missing sample %s\n", sample);
		type->SamplesOK = 0;
	}
	return 1;
}

/// <summary>
/// Sets how many samples of an event type are attack samples.
/// </summary>
void __fastcall AudioEventTypeSetAttack(AudioEventType * type, int attack)
{
	type->Attack = attack;
}

/// <summary>
/// Sets how many samples of an event type are decay samples.
/// </summary>
void __fastcall AudioEventTypeSetDecay(AudioEventType * type, int decay)
{
	type->Decay = decay;
}

/// <summary>
/// Sets the priority of an event type.
/// </summary>
void __fastcall AudioEventTypeSetPriority(AudioEventType * type, int priority)
{
	type->Priority = priority;
}

/// <summary>
/// Sets the volume of an event type.
/// </summary>
void __fastcall AudioEventTypeSetVolume(AudioEventType * type, int volume)
{
	AudioLevelSet(&type->Volume, volume);
	AudioLevelUpdate(&type->Volume);
}

/// <summary>
/// Sets the control flags of an event type; an attack or decay sample is needed when its flag is set and none when it is not.
/// </summary>
void __fastcall AudioEventTypeSetControl(AudioEventType * type, unsigned int control)
{
	type->Control = control;
	if (control & AUDIO_EVENT_CTRL_ATTACK) {
		if (type->Attack == 0) {
			type->Attack = 1;
		}
	} else {
		type->Attack = 0;
	}
	if (control & AUDIO_EVENT_CTRL_DECAY) {
		if (type->Decay == 0) {
			type->Decay = 1;
		}
	} else {
		type->Decay = 0;
	}
}

/// <summary>
/// Sets the "Type" flags of an event type.
/// </summary>
void __fastcall AudioEventTypeSetType(AudioEventType * type, unsigned int flags)
{
	type->Type = flags;
}

/// <summary>
/// Sets how many events of an event type may play at once.
/// </summary>
void __fastcall AudioEventTypeSetLimit(AudioEventType * type, int limit)
{
	type->Limit = limit;
}

/// <summary>
/// Sets the range of an event type, in cells.
/// </summary>
void __fastcall AudioEventTypeSetRange(AudioEventType * type, int range)
{
	type->Range = range;
}

/// <summary>
/// Sets the lowest volume an event type is played at.
/// </summary>
void __fastcall AudioEventTypeSetMinVolume(AudioEventType * type, float minVolume)
{
	type->MinVolume = minVolume;
}

/// <summary>
/// Sets the shortest and longest delay between the samples of an event type.
/// </summary>
void __fastcall AudioEventTypeSetDelay(AudioEventType * type, int minDelay, int maxDelay)
{
	type->MinDelay = minDelay;
	type->MaxDelay = maxDelay;
}

/// <summary>
/// Sets the lowest and highest pitch shift of an event type.
/// </summary>
void __fastcall AudioEventTypeSetFShift(AudioEventType * type, int minShift, int maxShift)
{
	type->MinFDelta = minShift;
	type->MaxFDelta = maxShift;
}

/// <summary>
/// Sets the volume shift of an event type, in percent.
/// </summary>
void __fastcall AudioEventTypeSetVShift(AudioEventType * type, int shift)
{
	if (shift > 100) {
		shift = 100;
	} else if (shift < 0) {
		shift = 0;
	}
	type->VShift = shift;
}

/// <summary>
/// Sets how often an event type loops; 0 is for ever.
/// </summary>
void __fastcall AudioEventTypeSetLoop(AudioEventType * type, int loop)
{
	type->Loop = loop;
}

/// <summary>
/// Whether an event type loops without end.
/// </summary>
int __fastcall AudioEventTypeLoopsForever(AudioEventType * type)
{
	if ((type->Control & AUDIO_EVENT_CTRL_LOOP) && !type->Loop) {
		return 1;
	}
	return 0;
}

/// <summary>
/// Plays the event type of this name once at a volume (0 to 256).
/// </summary>
void __fastcall Play_Named_Sound(char const * name, int volume)
{
	AudioEventType * eclass;
	AudioEvent * event;

	if (audioEventsReady) {
		for (eclass = (AudioEventType *)ListFirstItem(&audioEventClasses); eclass; eclass = (AudioEventType *)ListNextItem(&eclass->Header)) {
			if (!stricmp(name, eclass->Name)) {
				break;
			}
		}
		if (eclass && (event = AudioEventCreate(eclass))) {
			AudioEventJumpVolume(event, (int)((__int64)volume * AUDIO_LEVEL_MAX / 256));
		}
	}
}

/// <summary>
/// Turns the event layer off: every event is done at its next service.
/// </summary>
void AudioEventTurnOff(void)
{
	eventsOn = 0;
}

/// <summary>
/// Turns the event layer on.
/// </summary>
void AudioEventTurnOn(void)
{
	eventsOn = 1;
}

/// <summary>
/// Whether the event layer is on.
/// </summary>
int AudioEventOn(void)
{
	return eventsOn;
}

/// <summary>
/// Prints the number of events and of static sounds, now and at most, through a line printer.
/// </summary>
void __fastcall AudioEventsDump(void (__fastcall * print)(char const * line))
{
	char buffer[200];

	if (audioCache) {
		sprintf(buffer, "AudioEvents : %05d - Max: %05d", AudioEventsCount, AudioEventsPeak);
		print(buffer);
		sprintf(buffer, "StaticSounds: %05d - Max: %05d", StaticSoundCount, StaticSoundMaximum);
		print(buffer);
	}
}


/// <summary>
/// The priority of a name (LOWEST to CRITICAL); returns whether the name is one.
/// </summary>
bool __fastcall AudioPriorityFromName(char const * name, int * priority)
{
	EventKeyword * entry;

	for (entry = PriorityKeywords; entry->Name; entry++) {
		if (!stricmp(name, entry->Name)) {
			break;
		}
	}
	if (priority) {
		*priority = entry->Value;
	}
	return entry->Name != NULL;
}


/// <summary>
/// Adds the control flag of a name to the flags; returns whether the name is one.
/// </summary>
bool __fastcall AudioControlApplyKeyword(unsigned int * control, char const * name)
{
	EventKeyword * entry;

	for (entry = ControlKeywords; entry->Name; entry++) {
		if (!stricmp(name, entry->Name)) {
			break;
		}
	}
	if (control) {
		*control |= entry->Value;
	}
	return entry->Name != NULL;
}


/// <summary>
/// Adds the type flag of a name to the flags (a SCREEN, LOCAL or GLOBAL replaces the others of its kind, as UNSHROUD and SHROUD do);
/// returns whether the name is one.
/// </summary>
int __fastcall AudioTypeApplyKeyword(unsigned int * type, char const * name)
{
	EventKeyword * entry;

	for (entry = TypeKeywords; entry->Name; entry++) {
		if (!stricmp(name, entry->Name)) {
			break;
		}
	}
	if (type) {
		if (entry->Value & 0x60) {
			*type &= ~0x60;
		} else if (entry->Value & 0xC00) {
			*type &= ~0xC00;
		}
		*type |= entry->Value;
	}
	return entry->Name != NULL;
}


/// <summary>
/// The release executable's debug print: nothing is printed, but the calls and their format strings stay.
/// </summary>
