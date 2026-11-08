// Behaviour-written bodies of unmatched rows on the skirmish launch path
// (Scout1069; ra2-native --skirmish harness, native/behaviour_rewrites.json).
// Each follows the evidence named with it (the row's seed/decompile, TibSun,
// the accepted callers). NOT EXACT: every row stays unrecovered in the exact
// tree; never original code or bytes.
#include "shared/audio.hpp"
#include "shared/gadget.hpp"

// --- Audio library levels (Audio_Init's level sets; audio.hpp) ---------------

// 0x00403820 AudioTimeSpan::Reset (the row's leaf src/recovered/00403820.cpp):
// every statistic zero, +0x40 = 5000.
void AudioTimeSpan::Reset()
{
    unknown_00 = 0;
    LateLimit = 0;
    Elapsed = 0;
    Peak = 0;
    PeakWindow = 0;
    LastTime = 0;
    Maximum = 0;
    unknown_38 = 0;
    unknown_40 = 5000;
    TickCount = 0;
    LateCount = 0;
    unknown_50 = 0;
    unknown_54 = 0;
}

// 0x00406F40 Audio_Get_Device (audio.hpp; the row's leaf returns the dword at 0x00831438).
AudioDevice *Audio_Get_Device()
{
    return Audio_Main_Device;
}

// 0x00407080 AudioLevel::Set (the row's seed): jump to value on the next update.
void __fastcall AudioLevel::Set(int value)
{
    Flags |= AUDIO_LEVEL_PENDING;
    Target = (unsigned int)value << 16;
}

// 0x004070D0 AudioLevel::Set_Rate (the row's seed; __fastcall puts step in edx,
// the 64-bit period on the stack): the step per millisecond in 16.16 is
// (step << 16) / period (unsigned, low dword of the period), MaxElapsed = period.
void __fastcall AudioLevel::Set_Rate(__int64 period, int step)
{
    MaxElapsed = period;
    Step = ((unsigned int)step << 16) / (unsigned int)period;
}

// --- Gadgets -----------------------------------------------------------------

// 0x004CF290 GadgetClass::Extract_At (gadget.hpp; GadgetClass::Input 0x004CF330;
// the row's decompile): of the enabled gadgets in the list from this one that
// contain (x, y), the one with the smallest area (the last of equal ones).
// NATIVE: the original starts from a width/height pair in data (0x007ADD70,
// 0x007ADD7C, unsourced); here the first containing gadget always qualifies.
GadgetClass *GadgetClass::Extract_At(int x, int y)
{
    GadgetClass *found = 0;
    bool any = false;
    int best_w = 0;
    int best_h = 0;
    for (GadgetClass *gadget = this; gadget != 0; gadget = (GadgetClass *)gadget->Get_Next()) {
        if (gadget->IsDisabled) {
            continue;
        }
        int w = gadget->Width;
        int h = gadget->Height;
        if (gadget->X <= x && x < gadget->X + w && gadget->Y <= y && y < gadget->Y + h
            && (!any || h * w - best_h * best_w <= 0)) {
            found = gadget;
            any = true;
            best_w = w;
            best_h = h;
        }
    }
    return found;
}

// --- BitFontClass (text_print.hpp) -------------------------------------------

#include "shared/text_print.hpp"
#include "shared/surface.hpp"

// 0x00431460 BitFontClass::Set_Clip_Box (the row's seed): copies the box, or
// clears it without one.
void BitFontClass::Set_Clip_Box(tagRECT const *box)
{
    if (box != 0) {
        ClipBox = *box;
        return;
    }
    ClipBox.bottom = 0;
    ClipBox.right = 0;
    ClipBox.top = 0;
    ClipBox.left = 0;
}

// 0x004318D0 BitFontClass::Set_unknown_20 (the row's leaf: stores the value at +0x20).
void BitFontClass::Set_unknown_20(int value)
{
    unknown_20 = value;
}

// 0x00432150 BitFontClass::Release_Surface (the row's leaf: the surface's
// Unlock, slot 24, then Buffer and Pitch cleared).
void BitFontClass::Release_Surface(Surface *surface)
{
    surface->Unlock();
    Buffer = 0;
    Pitch = 0;
}

// --- CD file search, sound availability, strings, theme, sidebar ------------

#include "shared/cd_file_class.hpp"
#include "shared/win32_find.hpp"
#include "shared/sound_available.hpp"
#include "shared/theme.hpp"
#include "shared/map_view.hpp"
#include "shared/voc_class.hpp"

// 0x00470470 CDFileClass::Find_Close (the row's seed): closes the search
// handle Find_File opened and marks it idle (INVALID_HANDLE_VALUE).
void CDFileClass::Find_Close()
{
    if (FindHandle != (void *)-1) {
        FindClose((HANDLE)FindHandle);
        FindHandle = (void *)-1;
    }
}

// 0x00406F30 (sound_available.hpp; the row's seed): whether the audio device exists.
int unknown_check_406f30()
{
    return Audio_Main_Device != 0;
}

// 0x006FA0C0 (the row's seed): widens a narrow string into buffer, at most
// length characters (-1: no limit), always terminated.
void __fastcall Unknown_006FA0C0(wchar_t *buffer, char const *text, int length)
{
    unsigned char c = (unsigned char)*text;
    while (c != 0 && length != 0) {
        *buffer++ = c;
        ++text;
        if (length != -1) {
            --length;
        }
        c = (unsigned char)*text;
    }
    *buffer = 0;
}

// 0x006E8590 ThemeClass::Set_Volume (theme.hpp; the row's leaf): at most 255.
void ThemeClass::Set_Volume(int volume)
{
    if (volume >= 255) {
        volume = 255;
    }
    Volume = volume;
}

// 0x0067B880 SidebarClass::Unknown_0067B880 (map_view.hpp; the row is a bare return).
void SidebarClass::Unknown_0067B880(bool cameo_text)
{
}

// --- Audio event types (voc_class.hpp setters; each row's leaf stores its
// argument at the field named in the header) ---------------------------------

void __fastcall Audio_Event_Type_Set_Intro(AudioEventType *type, int count) { type->Intro = count; }        // 0x00406530
void __fastcall Audio_Event_Type_Set_Outro(AudioEventType *type, int count) { type->Outro = count; }        // 0x00406540
void __fastcall Audio_Event_Type_Set_Priority(AudioEventType *type, int priority) { type->unknown_40 = priority; }   // 0x00406550
void __fastcall Audio_Event_Type_Set_Type(AudioEventType *type, unsigned int flags) { type->unknown_14 = flags; }   // 0x004065D0
void __fastcall Audio_Event_Type_Set_Limit(AudioEventType *type, int limit) { type->unknown_48 = limit; }   // 0x004065E0
void __fastcall Audio_Event_Type_Set_Range(AudioEventType *type, int range) { type->unknown_50 = range; }   // 0x004065F0
void __fastcall Audio_Event_Type_Set_Min_Volume(AudioEventType *type, float volume) { type->unknown_54 = volume; }  // 0x00406600
void __fastcall Audio_Event_Type_Set_Delay(AudioEventType *type, int low, int high)                         // 0x00406610
{
    type->unknown_58 = low;
    type->unknown_5c = high;
}
void __fastcall Audio_Event_Type_Set_FShift(AudioEventType *type, int low, int high)                        // 0x00406620
{
    type->unknown_60 = low;
    type->unknown_64 = high;
}
void __fastcall Audio_Event_Type_Set_Loop(AudioEventType *type, int count) { type->LoopCount = count; }     // 0x00406650

// 0x004067E0 Audio_Priority_From_Name (the row's seed): looks the name up
// (case-insensitive) in the {name, priority} table at 0x007CCAE8, ended by a
// null name; stores the priority (the terminator's when not found) and returns
// whether it was found. NATIVE: the table's names and values are unsourced
// data, so here no name is found and the priority is set to 0.
bool __fastcall Audio_Priority_From_Name(char const *name, int *priority)
{
    if (priority != 0) {
        *priority = 0;
    }
    return false;
}

// 0x00406830 Audio_Control_Apply_Keyword (the row's seed): looks the keyword up
// (case-insensitive) in the {name, flag} table at 0x007CCB90, ended by a null
// name; ORs its flag (the terminator's when not found) into *flags and returns
// whether it was found. NATIVE: the table is unsourced data, so here no
// keyword is found and *flags is unchanged (the terminator's flag word is
// taken as 0).
bool __fastcall Audio_Control_Apply_Keyword(unsigned int *flags, char const *name)
{
    return false;
}
