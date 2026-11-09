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

// --- CD file search, sound availability, strings, theme, sidebar ------------

#include "shared/cd_file_class.hpp"
#include "shared/win32_find.hpp"
#include "shared/sound_available.hpp"
#include "shared/theme.hpp"
#include "shared/map_view.hpp"
#include "shared/voc_class.hpp"

// --- Audio event types (voc_class.hpp setters; each row's leaf stores its
// argument at the field named in the header) ---------------------------------

// --- Init_Bulk_Data's One_Time calls (Scout1072) ------------------------------

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

#include "shared/tactical.hpp"

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

// --- Menu path (Scout1083) ----------------------------------------------------

// 0x0068CA20 UnknownBitmapWriter::Load_Image_0068ca20 (pcx_cache.hpp; the row's
// leaf src/recovered/0068ca20.cpp forwards (name, 1, 1) to 0x0068C600).
#define PCX_CACHE_DECLARE_LOADS 1
#include "shared/pcx_cache.hpp"

// 0x005DE970 under its window-keyed name (the window table 0x00A73400 and the
// window procedure table 0x00A73688 declare it over HWND keys; same seed
// `return *key;`): the hash of a window is its handle value. Port handles are
// small tokens, so the low 32 bits are the whole handle.
unsigned int __fastcall unknown_hash_window_5de970(HWND const &key)
{
    return (unsigned int)(uintptr_t)key;
}

// --- Owner-draw dialog preparation (unknown_prepare_dialog's callees; Scout1083) --

#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/session_state.hpp"

BOOL __stdcall unknown_prepare_control_735bc0(HWND window, LPARAM lparam);

bool __fastcall OwnerDraw_Control_Test_005e0760(unsigned int key, HWND window);
int __fastcall OwnerDraw_Control_Height_005defb0(unsigned int key, HWND window);
int __fastcall OwnerDraw_Control_Kind_005df8e0(unsigned int key, HWND window);
int __fastcall OwnerDraw_Control_Flags_005e0010(unsigned int key, HWND window);

namespace OwnerDraw {
bool __fastcall unknown_5e7230(unsigned int key, HWND window);
bool __fastcall unknown_5e7620(unsigned int key, HWND window);
bool __fastcall unknown_5e79a0(unsigned int key, HWND window);
bool __fastcall unknown_5e0e40(unsigned int key, HWND window);
bool __fastcall unknown_5e0ff0(unsigned int key, HWND window);
int __fastcall unknown_5e1a20(unsigned int key, HWND window);
bool __fastcall unknown_5e1050(unsigned int key, HWND window);
int __fastcall unknown_5e1490(unsigned int key, HWND window);
void *__fastcall unknown_5e1820(unsigned int key, HWND window);
int __fastcall unknown_5e06c0(unsigned int key, HWND window);
void const *__fastcall unknown_5e1a80(unsigned int key, HWND window, bool &allocated);

// 0x005E8720 EnumChildWindows callback (OwnrDraw233 draft c/5e8720e3.cpp, 51.12;
// Scout720 keep/e8720_n1.cpp 53.29): a registered control's +0xDC and +0xC8 from
// its dialog.
BOOL __stdcall unknown_5e8720(HWND window, LPARAM)
{
    HWND parent = GetParent(window);
    UnknownControlRecord *record = unknown_registry_a73640.Find((unsigned int)(uintptr_t)window);
    if (record) {
        record->unknown_dc = unknown_5e12a0((unsigned int)(uintptr_t)parent, window);
        record->unknown_c8 = unknown_5e1ca0(parent, window);
    }
    return TRUE;
}

// 0x005E7EB0 EnumChildWindows callback (Scout720 draft keep/e7eb0_8955.cpp,
// 89.55): the record's image state (+0xB0) from the control's tests.
BOOL __stdcall unknown_5e7eb0(HWND window, LPARAM)
{
    UnknownControlRecord *record = unknown_registry_a73640.Find((unsigned int)(uintptr_t)window);
    if (record != 0) {
        unsigned int parent = (unsigned int)(uintptr_t)GetParent(window);
        if (!unknown_session_a3d298.Unknown_00672F10()) {
            if (unknown_5e7230(parent, window) || unknown_5e7620(parent, window)) {
                record->unknown_b0 = 1;
                return TRUE;
            }
        }
        if (unknown_session_a3d298.Unknown_00672F10()) {
            if (unknown_5e7230(parent, window) || unknown_5e7620(parent, window)) {
                record->unknown_b0 = 2;
                return TRUE;
            }
        }
        if (unknown_5e79a0(parent, window)) {
            record->unknown_b0 = 3;
        }
    }
    return TRUE;
}

// 0x005E8130 EnumChildWindows callback (Scout1004 draft keep/e8130_ke.cpp,
// 49.20): a registered control's animation state: list-style controls get
// height/kind/flags; image (2), static (3) and animated (4) controls get their
// frame data and a timer of the control's period.
BOOL __stdcall unknown_5e8130(HWND window, LPARAM)
{
    unsigned int parent = (unsigned int)(uintptr_t)GetParent(window);
    UnknownControlRecord *record = unknown_registry_a73640.Find((unsigned int)(uintptr_t)window);
    if (record == 0) {
        return TRUE;
    }
    if (OwnerDraw_Control_Test_005e0760(parent, window)) {
        record->unknown_70 = 1;
        record->unknown_a8 = false;
        record->unknown_80 = 1;
        record->unknown_84 = OwnerDraw_Control_Height_005defb0(parent, window);
        record->unknown_88 = OwnerDraw_Control_Kind_005df8e0(parent, window);
        record->unknown_8c = OwnerDraw_Control_Flags_005e0010(parent, window);
        record->unknown_90 = unknown_5e06c0(parent, window);
        return TRUE;
    }
    bool animated = false;
    if (unknown_5e0e40(parent, window)) {
        record->unknown_70 = 2;
        if (record->image_14 == 0) {
            // NATIVE: 0x005E1A20 returns the image as an int (int/pointer list).
            record->image_14 = (Surface *)(intptr_t)unknown_5e1a20(parent, window);
        }
    } else {
        if (unknown_5e0ff0(parent, window)) {
            record->unknown_70 = 3;
        } else if (unknown_5e1050(parent, window)) {
            record->unknown_70 = 4;
            animated = true;
            if (record->unknown_78 == 0) {
                bool allocated = false;
                void *art = unknown_5e1820(parent, window);
                record->unknown_78 = unknown_5e1a80(parent, window, allocated);
                // NATIVE: +0x74 is an int in src/shared (int/pointer list): the
                // art pointer is truncated as in 0x005EA960.
                record->unknown_74 = (int)(intptr_t)art;
                record->unknown_7c = allocated;
            }
        } else {
            goto done;
        }
    }
    record->unknown_98 = 0;
    record->unknown_9c = GetTickCount();
    record->unknown_a0 = unknown_5e1490(parent, window);
done:
    if (record->unknown_a0 < 1) {
        if (animated && record->unknown_78) {
            record->unknown_94 = ((short const *)record->unknown_78)[3];
            record->unknown_a8 = false;
        }
    } else {
        SetTimer(window, 0, record->unknown_a0, 0);
        if (animated) {
            record->unknown_a8 = true;
        }
    }
    return TRUE;
}
}

// 0x00735B40 (the row's seed src/recovered/00735b40.cpp: eax = 0): the dialog
// procedure 0x00735BC0 hands Dialog_Template_Size handles no message.
#include "shared/win_util.hpp"

// --- HouseClass subobjects (Scout1083) ------------------------------------------
// The constructors/destructors of house.hpp's subobject views. Natively each
// view holds its real class at offset 0 (port/fixups.json), so these run it.
#include "shared/house.hpp"
#include "shared/unit_tracker.hpp"

// 0x00491C00 / 0x00491C20: CounterClass's constructor and destructor
// (CounterClass_Construct_00491c00.cpp); the member runs them.
UnknownHouseCounter::UnknownHouseCounter() {}
UnknownHouseCounter::~UnknownHouseCounter() {}
// 0x004A6910 (Unknown4A6AE0Owner_Constructor_004a6910.cpp) / 0x004A6930 (the
// row's seed: an empty body).
UnknownHouseBase::UnknownHouseBase() {}
UnknownHouseBase::~UnknownHouseBase() {}
// 0x0070C440 / 0x0070C480: UnitTrackerClass (same 0x808 bytes on both targets).
UnknownHouseTracker::UnknownHouseTracker() { new (unknown_000) UnitTrackerClass; }
UnknownHouseTracker::~UnknownHouseTracker() { ((UnitTrackerClass *)unknown_000)->~UnitTrackerClass(); }

// --- Scenario load (Scout1083) ---------------------------------------------------

// 0x0068B410 SpecialClass::Unknown_0068B410 (the row's seed
// src/recovered/0068b410.cpp has an empty body: the original returns at once).
#include "shared/special.hpp"

