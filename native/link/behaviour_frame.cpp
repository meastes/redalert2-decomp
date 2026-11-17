// Behaviour-written bodies of unmatched rows the skirmish harness reaches in
// its first Main_Loop iteration (Scout1089). Each follows the evidence named
// with it. NOT EXACT: every row stays unrecovered in the exact tree; never
// original code or bytes.
#include "shared/audio.hpp"
#include "shared/sound_play.hpp"
#include "shared/voc_class.hpp"
#include "shared/object.hpp"

// --- Audio event library (VocClass::Play 0x00713DB0 and its callers) ---------

// 0x00405110 (seed src/recovered/00405110.cpp): an event type's range (+0x50),
// 0 while the event system is off.
int __fastcall Audio_Event_Type_Range(AudioEventType *type)
{
    if (Audio_Events_Ready == 0) {
        return 0;
    }
    return type->unknown_50;
}

// 0x00405140 (seed src/recovered/00405140.cpp): its +0x14 word, 0 while off.
unsigned int __fastcall Audio_Event_Type_Flags(AudioEventType *type)
{
    if (Audio_Events_Ready == 0) {
        return 0;
    }
    return (unsigned int)type->unknown_14;
}

// 0x00406320 (leaf src/recovered/00406320.cpp): the event's type (+0x24).
AudioEventType *__fastcall Audio_Event_Type_Of(AudioEvent *event)
{
    return event->Type;
}

// 0x00406140 (seed src/recovered/00406140.cpp): the event a handle stands for,
// or 0 when the handle no longer matches (audio.hpp Audio_Event_Resolve, the
// inline it is the out-of-line copy of).
AudioEvent *__fastcall Audio_Event_Handle_Event(AudioEventHandle *handle)
{
    return Audio_Event_Resolve(handle);
}

// 0x00406100 (seed src/recovered/00406100.cpp): points a handle of the event
// bag at an event (its serial +0x138; its own type when none is given) and
// records the handle in the event (+0x278).
void __fastcall Audio_Event_Handle_Set(AudioEventHandle *handle, AudioEvent *event, AudioEventType *type)
{
    if (handle->Token != &Audio_Event_Bag) {
        return;
    }
    handle->Event = event;
    handle->Type = type;
    if (event != 0) {
        handle->Serial = event->unknown_138;
        if (type == 0) {
            handle->Type = event->Type;
        }
        event->unknown_278 = handle;
    }
}

// --- Objects -------------------------------------------------------------------

// 0x00423DC0 ObjectClass slot 65 (+0x104): `ret 8`, nothing (the row's leaf
// src/recovered/classes/ObjectClass/00423dc0.cpp).
void ObjectClass::unknown_slot65(int arg1, int arg2)
{
}

extern bool unknown_flag_a767a4;   // 0x00A767A4 (shared/planning.hpp)

// 0x00614840 Planning_Active (seed src/recovered/planmgr/Planning_Active.cpp): the byte at
// 0x00A767A4 (the planning mode flag).
bool Planning_Active()
{
    return unknown_flag_a767a4;
}

// --- DelayTimerClass (TechnoClass +0x2A8) -------------------------------------------
#include "shared/techno.hpp"

// 0x00496CE0 (seed src/recovered/00496ce0.cpp; TechnoClass::AI 0x006C74C0 calls it
// once the timer completed): an active reverse change ends as reversed (+0x18 0,
// +0x19 1); an active forward change ends cleared (both 0). Native name: the seed
// passed the object as an int (fixup in TechnoClass_AI_006c74c0.cpp).
void Native_Timed_Finish_00496ce0(DelayTimerClass *timed)
{
    if (timed->IsActive) {
        if (timed->IsToCountUp) {
            timed->IsActive = false;
            timed->IsToCountUp = true;
        }
        if (timed->IsActive && !timed->IsToCountUp) {
            timed->IsActive = false;
            timed->IsToCountUp = false;
        }
    }
}

// 0x0041B440 TechnoClass slot 258 (+0x408): `xor al, al; ret`, false.
bool TechnoClass::unknown_slot258()
{
    return false;
}

// 0x00406180 Audio_Event_Handle_Type (seed src/recovered/00406180.cpp): the
// handle's type (+0x08) when the event system is up and the handle is the bag's.
AudioEventType *__fastcall Audio_Event_Handle_Type(AudioEventHandle *handle)
{
    if (Audio_Events_Ready != 0 && handle->Token == &Audio_Event_Bag) {
        return handle->Type;
    }
    return 0;
}
