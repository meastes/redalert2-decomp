// Behaviour-written bodies of unmatched rows on the skirmish launch path
// (Scout1069; ra2-native --skirmish harness, native/behaviour_rewrites.json).
// Each follows the evidence named with it (the row's seed/decompile, TibSun,
// the accepted callers). NOT EXACT: every row stays unrecovered in the exact
// tree; never original code or bytes.
#include "shared/audio.hpp"
#include "shared/gadget.hpp"
#include "shared/display.hpp"
#include "shared/rgb_class.hpp"
#include "shared/object_type.hpp"
#include "shared/vector_class.hpp"
#include "shared/multi_mission.hpp"

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

// --- DisplayClass (Scout1072) --------------------------------------------------

// 0x0049E7D0 DisplayClass::unknown_slot34 (display.hpp slot 34; the accepted
// body is the seed src/recovered/classes/DisplayClass/0049e7d0.cpp, a free
// function the vtable name never binds to): if the window handle kept at
// 0x00B263DC is set, CloseWindow it and clear it. NOT EXACT (a native body of
// the same behaviour; port handles are small tokens, so the 32-bit slot keeps them).
extern "C" unsigned int DAT_00b263dc;
extern "C" int ra2_CloseWindow_49e7d0(void *window) __asm__("_CloseWindow");
void DisplayClass::unknown_slot34()
{
    if (DAT_00b263dc != 0) {
        ra2_CloseWindow_49e7d0((void *)(unsigned long)DAT_00b263dc);
        DAT_00b263dc = 0;
    }
}

// --- Init_Bulk_Data's One_Time calls (Scout1072) ------------------------------

// 0x00458370 Unknown_00458370 (Init_Bulk_Data 0x00516900 calls it after
// ObjectTypeClass::One_Time; the decompile is an empty body, `return;`; the
// row's leaf view is Leaf_458370::get). NOT EXACT: the same empty behaviour.
void Unknown_00458370()
{
}

// 0x0063CFB0 RGBClass::Difference (rgb_class.hpp; the accepted body is the seed
// src/recovered/0063cfb0.cpp, an owner view the member name never binds to):
// the weighted distance 2|dRed| + 4|dGreen| + 3|dBlue| over the three bytes in
// member order. NOT EXACT: the seed's arithmetic on the class's members.
int RGBClass::Difference(RGBClass const &other) const
{
    int red = (int)Red - (int)other.Red;
    int green = (int)Green - (int)other.Green;
    int blue = (int)Blue - (int)other.Blue;
    if (red < 0) red = -red;
    if (green < 0) green = -green;
    if (blue < 0) blue = -blue;
    return blue * 3 + (red + green * 2) * 2;
}

// 0x004E2E30 Unknown_004E2E30 (Init_Bulk_Data's One_Time sequence; TibSun's
// HouseClass::One_Time position): the decompile is an empty body. NOT EXACT.
void Unknown_004E2E30()
{
}

// --- CPU identification (Get_CPU_Info 0x004D2A20; the probes are handwritten
// assembly in the library: 0x0077F14D and 0x0077F20F, read from their listing) ---
//
// 0x0077F14D: toggles EFLAGS.AC (386 vs 486) and EFLAGS.ID (CPUID present); with
// CPUID stores the vendor text at 0x008015A5 (12 bytes and a space), the family
// (0x008015A3) and model (0x008015A4), and returns 1 with 0x008015A2 = 1 when
// family >= 5 and CPUID.1:EDX bit 23 (MMX) is set, else 0 (0x008015A2 = 0).
// 0x0077F20F: with family >= 5 and CPUID.1:EDX bit 15 (CMOV) sets 0x008015A1 = 1
// and 0x008015A0 = (family > 5); otherwise both 0; returns the 0x008015A1 value.
// NATIVE: arm64 has neither instruction: the port reports a CPUID-capable
// family-6 processor without MMX and CMOV (every MMX/CMOV branch then takes the
// plain path), model 0, and leaves the stored "not available" text unchanged.
extern char unknown_cpu_family_8015a3;
extern char unknown_cpu_model_8015a4;
namespace {
unsigned char cpu_cmov_p6_8015a0;   // 0x008015A0 (no name in the tree yet)
unsigned char cpu_cmov_8015a1;      // 0x008015A1
unsigned char cpu_mmx_8015a2;       // 0x008015A2
}

unsigned char unknown_cpu_probe_77f14d()
{
    unknown_cpu_model_8015a4 = 0;
    unknown_cpu_family_8015a3 = 6;
    cpu_mmx_8015a2 = 0;
    return 0;
}

void unknown_cpu_family_probe_77f20f()
{
    cpu_cmov_p6_8015a0 = 0;
    cpu_cmov_8015a1 = 0;
}

// 0x0079777A (handwritten assembly; called by LightConvertClass::Set_Tint
// 0x00538AA0; read from its listing/decompile): builds a 256-entry 15-bit
// (5:5:5) colour table from the palette's RGB bytes. Entry 0 is 0; entry i
// scales the palette's red, green and blue bytes by red/green/blue when
// remap[i] is nonzero, else all three by level (16.16 fixed point: the channel
// is (byte * scale) >> 16, saturated to 255), and packs
// (r & 0xF8) << 7 | (g & 0xF8) << 2 | b >> 3. The original has three paths
// (MMX pmulhw, CMOV and plain) giving the same table; this is the plain one.
class PaletteClass;
void __cdecl FUN_0079777a(PaletteClass const *palette, unsigned short *table, int red, int green, int blue,
                          int level, unsigned char const *remap)
{
    unsigned char const *rgb = (unsigned char const *)palette;
    table[0] = 0;
    for (int i = 1; i < 256; ++i) {
        unsigned int r_scale = remap[i] ? red : level;
        unsigned int g_scale = remap[i] ? green : level;
        unsigned int b_scale = remap[i] ? blue : level;
        unsigned int r = ((unsigned int)rgb[3 * i] * r_scale) >> 16;
        unsigned int g = ((unsigned int)rgb[3 * i + 1] * g_scale) >> 16;
        unsigned int b = ((unsigned int)rgb[3 * i + 2] * b_scale) >> 16;
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        table[i] = (unsigned short)((r & 0xF8) << 7 | (g & 0xF8) << 2 | b >> 3);
    }
}

// 0x00797440 (handwritten assembly, the 16-bit 5:6:5 sibling of 0x0079777A
// above; read from its decompile): the same table packed as
// (r & 0xF8) << 8 | (g & 0xFC) << 3 | b >> 3.
void __cdecl FUN_00797440(PaletteClass const *palette, unsigned short *table, int red, int green, int blue,
                          int level, unsigned char const *remap)
{
    unsigned char const *rgb = (unsigned char const *)palette;
    table[0] = 0;
    for (int i = 1; i < 256; ++i) {
        unsigned int r_scale = remap[i] ? red : level;
        unsigned int g_scale = remap[i] ? green : level;
        unsigned int b_scale = remap[i] ? blue : level;
        unsigned int r = ((unsigned int)rgb[3 * i] * r_scale) >> 16;
        unsigned int g = ((unsigned int)rgb[3 * i + 1] * g_scale) >> 16;
        unsigned int b = ((unsigned int)rgb[3 * i + 2] * b_scale) >> 16;
        if (r > 255) r = 255;
        if (g > 255) g = 255;
        if (b > 255) b = 255;
        table[i] = (unsigned short)((r & 0xF8) << 8 | (g & 0xFC) << 3 | b >> 3);
    }
}

// 0x004070A0 AudioLevel::Set_Target (audio.hpp; the row's seed): clear the
// pending flag; when the level is at rest (target == current) restart its
// clock (Audio_Now); the new target is value << 16. NOT EXACT.
__int64 Audio_Now();
void __fastcall AudioLevel::Set_Target(int value)
{
    Flags &= ~1u;
    if (Target == Current) {
        LastTime = Audio_Now();
    }
    Target = (unsigned int)value << 16;
}

// 0x0051FC50 Unknown_0051FC50 (the seed returns the byte at 0x00A51934,
// StormActive). NOT EXACT.
extern bool StormActive;
bool Unknown_0051FC50()
{
    return StormActive;
}

// 0x0051FBB0 Unknown_0051FBB0 (Clear_Scenario, a trigger action and a team
// mission call it; the row's code: while a storm is active, set the byte at
// 0x00A51944, StormTimeToEnd). Scout1076. NOT EXACT.
extern bool StormTimeToEnd;
void Unknown_0051FBB0()
{
    if (StormActive) {
        StormTimeToEnd = true;
    }
}

// 0x005D7C20 ObjectTypeClass::Generic_Naming_Convention (object_type.hpp;
// the decompile stores 'G' at buffer[1]; TibSun objtype.cpp: the theater
// letter of a file name becomes the generic one). NOT EXACT.
void __fastcall ObjectTypeClass::Generic_Naming_Convention(char *buffer)
{
    buffer[1] = 'G';
}

// 0x00674AA0 VectorClass<DiskID>::VectorClass(unsigned, DiskID const *) and
// 0x00674AF0 VectorClass<Wstring>::VectorClass (multi_mission.hpp declares them
// as specialisations defined in their own rows; the 0x00674AA0 decompile is the
// shared template body of vector_class.hpp): empty storage of size elements,
// the caller's array in place (not owned) or a new owned block. NOT EXACT.
template <>
VectorClass<DiskID>::VectorClass(unsigned size, DiskID const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = (DiskID *)array;
        } else {
            Vector = new DiskID[size];
            IsAllocated = true;
        }
    }
}

template <>
VectorClass<Wstring>::VectorClass(unsigned size, Wstring const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) Wstring[size];
        } else {
            Vector = new Wstring[size];
            IsAllocated = true;
        }
    }
}

// --- Tactical (vtable 0x007AC570): the slots that make the native vtable ------
// Tactical's key function (GetClassID) had no definition, so natively the class
// had no vtable and `delete` of the view (Delete_All_Objects, Clear_Scenario)
// called a zero slot. Written from each row's code (Scout1076). NOT EXACT.
// Save 0x006AC400 and Detach 0x006AAB60 stay unrecovered (link-closure traps).
#include "shared/tactical.hpp"

struct ra2_guid_bytes { unsigned int a; unsigned short b, c; unsigned char d[8]; };
extern "C" const ra2_guid_bytes Startup_CLSID_Tactical;   // 0x007A1E90 (data/com_guids.cpp)

// 0x006AC2E0: E_POINTER for null, else the class id at 0x007A1E90; S_OK.
HRESULT __stdcall Tactical::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return (HRESULT)0x80004003;
    }
    *clsid = *(GUID const *)&Startup_CLSID_Tactical;
    return 0;
}

// 0x006A2710: clears the view pointer 0x0083996C (then ~AbstractClass).
Tactical::~Tactical()
{
    unknown_tactical_83996c = 0;
}

// 0x006AC7E0 / 0x006AC7F0: the kind (0x38) and the original object size (0xE18).
int Tactical::Kind_Of() const
{
    return 0x38;
}

int Tactical::Size_Of() const
{
    return 0xE18;
}

// 0x00529710 Unknown_00529710 (the row's code is `xor al, al; ret`: false, nothing freed;
// LightSourceClass 0x00537830 and MapClass slot 23 call it). Scout1076. NOT EXACT.
bool __fastcall Unknown_00529710(int time, bool flag)
{
    return false;
}

// --- Scenario post-load / building placement (Clear_Scenario path; Scout1081) --

class BuildingClass;
extern int unknown_group_frame_7f8bcc;     // 0x007F8BCC (data/original_data.cpp)

// 0x006F89E0 Unknown_006F89E0 (the row's leaf src/recovered/006f89e0.cpp: 0x007F8BCC
// and 0x007F8BD4 = -100, 0x007F8BD0 = -1). Only 0x007F8BCC has a reader in the
// recovered tree (Unknown_Group_Press 0x006F5F00); the other two words have no
// name natively. NOT EXACT.
void Unknown_006F89E0()
{
    unknown_group_frame_7f8bcc = -100;
}

// The two pending placements 0x00AC1334 / 0x00AC1338 (the seeds 006f8a00.cpp and
// 006f8a20.cpp: a building whose owner's +0xBA0 is 5 goes in the second slot).
// Pointer-sized here: the BSS storage of both names is 16 bytes (data/bss_globals.cpp).
extern "C" BuildingClass *ra2_pending_placement_ac1334 __asm__("_DAT_00ac1334");
extern "C" BuildingClass *ra2_pending_placement_ac1338 __asm__("_DAT_00ac1338");

// 0x006F8A20 Unknown_006F8A20 (seed 006f8a20.cpp): null forgets both, else the
// slot that holds the building. NOT EXACT.
void __fastcall Unknown_006F8A20(BuildingClass *building)
{
    if (building == 0) {
        ra2_pending_placement_ac1334 = 0;
        ra2_pending_placement_ac1338 = 0;
        return;
    }
    if (building == ra2_pending_placement_ac1334) {
        ra2_pending_placement_ac1334 = 0;
        return;
    }
    if (building == ra2_pending_placement_ac1338) {
        ra2_pending_placement_ac1338 = 0;
    }
}

// --- INI type names (CCINIClass::Get_*Type defaults; Scout1081) ---------------
// The three seeds (src/recovered/00482f00.cpp, 00482f60.cpp, 00482fc0.cpp) index
// a name table (LandNames 12, SpeedNames 8, LayerNames 5; read from game.exe at
// run time, data/original_data.cpp) and return "<none>" (0x007CDECC) past it.
// The declarations in ccini_extra.hpp use enums the units define; plain ints
// carry them here (VC6 passes an enum as a 32-bit slot). NOT EXACT.
enum LandType : int;
enum SpeedType : int;
enum LayerType : int;
extern char const *LandNames[12];
extern char const *SpeedNames[8];
extern char const *LayerNames[5];
extern "C" char s__none__007cdecc[];

char const *__fastcall Name_From_Land(LandType land)
{
    if ((unsigned)land < 12) {
        return LandNames[land];
    }
    return s__none__007cdecc;
}

char const *__fastcall Name_From_Speed(SpeedType speed)
{
    if ((unsigned)speed < 8) {
        return SpeedNames[speed];
    }
    return s__none__007cdecc;
}

char const *__fastcall Name_From_Layer(LayerType layer)
{
    if ((unsigned)layer < 5) {
        return LayerNames[layer];
    }
    return s__none__007cdecc;
}

// 0x007168C0 Unknown_007168C0 (the row's leaf src/recovered/007168c0.cpp stores
// its ECX argument at 0x00AD0958, unknown_flag_ad0958 in vox_speech.hpp: the
// side whose speech files EVA plays). Scout1081. NOT EXACT.
extern int unknown_flag_ad0958;
void __fastcall Unknown_007168C0(int side)
{
    unknown_flag_ad0958 = side;
}

// --- Owner-draw dialogs (main menu path; Scout1081) ---------------------------

// 0x005DE970 unknown_hash_key_5de970 (the row's seed: `return *key;`): the
// owner-draw control registry's hash of a window key is the key itself. NOT EXACT.
unsigned int __fastcall unknown_hash_key_5de970(unsigned int const &key)
{
    return key;
}

// 0x00522B20 Unknown_Add_Modeless_Dialog_00522b20 (the row's seed): while the
// registry is active (0x00A522B0), files the dialog in the first free slot of
// the block 0x00A522B4 (0x00A521DC slots, slot 0 unused) unless it is there,
// counting it in 0x00A522B8. Slots hold 32-bit handles as in VC6 (port window
// handles are small tokens). NOT EXACT.
extern "C" int DAT_00a521dc;
extern "C" int DAT_00a522b0;
extern "C" void *DAT_00a522b4;
extern "C" int DAT_00a522b8;
void __fastcall Unknown_Add_Modeless_Dialog_00522b20(HWND__ *dialog)
{
    unsigned int handle = (unsigned int)(uintptr_t)dialog;
    if (DAT_00a522b0 == 0 || handle == 0) {
        return;
    }
    unsigned int *slot = (unsigned int *)DAT_00a522b4;
    unsigned int *free_slot = 0;
    for (int index = 1; index < DAT_00a521dc; index++, slot++) {
        if (*slot == 0) {
            if (free_slot == 0) {
                free_slot = slot;
            }
        } else if (*slot == handle) {
            return;
        }
    }
    if (free_slot != 0) {
        *free_slot = handle;
        DAT_00a522b8++;
    }
}
