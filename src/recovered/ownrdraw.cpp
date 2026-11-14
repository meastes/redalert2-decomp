// ownrdraw.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ownrdraw.json carries the addresses and the rows that
// stay in their own files.


// Container members stay undefined here: this text does not yet reproduce the original
// unit's list uses (shared/vector_class.hpp).
#define VECTOR_DECLARATIONS_ONLY
#define RA2_OWNERDRAW_COPY_LIST_SOURCE
#define PCX_CACHE_DECLARE_DRAWS
#define RA2_OWNERDRAW_BUTTON_FIELDS
#define RA2_OWNERDRAW_CHECK_FIELDS
#define RA2_OWNERDRAW_EDIT_FIELDS
#define RA2_WOL_DICTIONARY_KEY_SPECIALIZATIONS
#define RA2_WOL_DICTIONARY_REMOVE_SPECIALIZATION
#define RA2_WOL_ARRAY_LIST_ASSIGN
#define RA2_OWNERDRAW_RECORD_DTOR
#define RA2_WOL_DICTIONARY_SHRINK_ACCESSORS
#define RA2_WOL_DICTIONARY_WLIB_STATEMENTS
#define RA2_OWNERDRAW_WNDPROC_DICTIONARY
#define RA2_OWNERDRAW_CONTROL_DICTIONARY
// Header-static dynamic initializers of the unit at 0x005DE660: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x005DE660-0x005DE841); the other
// drafts of this plan are emitted by these includes.
#define RA2_COMBO_DROP_UNIT
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x005DE680, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_005de660.cpp; no source of its own.


// Dynamic initializer (0x005DE6B0, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_005de660.cpp; no source of its own.


// Dynamic initializer (0x005DE6D0, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_005de660.cpp; no source of its own.


// Dynamic initializer (0x005DE6F0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_005de660.cpp; no source of its own.


// Dynamic initializer (0x005DE710, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_005de660.cpp; no source of its own.


// Dynamic initializer (0x005DE730, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_005de660.cpp; no source of its own.


// Dynamic initializer (0x005DE750, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_005de660.cpp; no source of its own.


// Dynamic initializer (0x005DE790, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_005de660.cpp; no source of its own.


// Dynamic initializer (0x005DE7C0, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_005de660.cpp; no source of its own.


// Dynamic initializer (0x005DE7F0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_005de660.cpp; no source of its own.


// Dynamic initializer (0x005DE820, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_005de660.cpp; no source of its own.


// Dynamic initializer (0x005DE830, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_005de660.cpp; no source of its own.


// unknown_bitmap_writer_a76378 (0x00A76378), defined by the unit at 0x005DE660: its dynamic initializer
// calls the out-of-line constructor (0x68BD50) and registers the atexit
// destructor (the next draft), which jumps to the destructor (0x68BE30).
#include "shared/pcx_cache.hpp"

SurfaceCacheClass unknown_bitmap_writer_a76378;


// atexit destructor (0x005DE870, _$E) of unknown_bitmap_writer_a76378: emitted by the definition in
// unknown_bitmap_writer_a76378_init_005de850.cpp; no source of its own.


#include "shared/win32_api.hpp"
#include "shared/owner_draw_dialog_control.hpp"

namespace OwnerDraw {

void unknown_5de880()
{
    unknown_color_a73928 = 1;
    unknown_color_a733d0 = 0x7f;
    unknown_color_a733e4 = 0xffff;
    unknown_color_a737e8 = 0xeeeeee;
    unknown_color_a76150 = 0x9f9f;
    unknown_color_a7338c = 0xffffff;
    unknown_color_a7615c = 0x808080;
    unknown_color_a7613c = 0xff;
    unknown_color_a763b0 = 0x626262;
    unknown_color_a76158 = 0xff;
    unknown_color_a73910 = 0x929292;
    unknown_color_a737ec = 0x9f;
    unknown_color_a7363c = 0x8f8f8f;
    unknown_color_a737e0 = 0x9f;
    unknown_color_a76154 = 0x646464;
    unknown_color_a763b8 = 0x60;
    unknown_color_a76140 = 0xaaaa;
    unknown_color_a763d4 = 0x221b0b;
    unknown_color_a736d0 = 0x443716;
}

}


namespace OwnerDraw {

bool UnknownPairKey::operator==(UnknownPairKey const &other) const
{
    if (other.unknown_04 == unknown_04 && other.unknown_00 == unknown_00) {
        return true;
    }
    return false;
}

}


// 0x005de970: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall unknown_hash_key_005de970(undefined4 *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005de970 {

unsigned int __fastcall unknown_hash_key_005de970(unsigned int *key)
{
    return *key;
}

}  // namespace seed_005de970


// 0x005de980: function not yet identified
//
// Body: 7 bytes (frozen inventory bound).
// Referenced from 1 site (computed call).
// Ghidra prototype: int __fastcall unknown_product_005de980(int *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005de980 {

int __fastcall unknown_product_005de980(int *pair)
{
    return pair[1] * *pair;
}

}  // namespace seed_005de980


// The ownrdraw window table at 0x00A73688 as wlib's chained dictionary (shared/wol_dictionary.hpp:
// thresholds 0.2/0.8, 128 buckets), keyed by window: its dynamic initializer (0x005DE9C0) inlines the
// constructor and registers the atexit destructor (0x005DEA60, the next draft), which inlines the
// destructor with clear() and shrink(). The unit's window procedures reach it through this object
// (RA2_OWNERDRAW_WNDPROC_DICTIONARY); its out-of-line members are getPointer 0x006018E0, remove 0x00601950,
// expand 0x00601B90, keyHash 0x00601C30 and shrink 0x00602730.
#include "shared/wol_dictionary.hpp"

// The window's handle value as its hash (0x005DE970).
unsigned int __fastcall unknown_hash_window_5de970(HWND const &key);

WolDictionary<HWND, WNDPROC> unknown_window_proc_table_a73688(unknown_hash_window_5de970);


// Compiler-generated atexit destructor of unknown_window_proc_table_a73688 (defined in the previous draft,
// 0x005DE9C0): WolDictionary's destructor inline; no handwritten body.


// A second window-keyed table of the unit at 0x00A73400: the same wlib dictionary instantiation as the
// window procedure table 0x00A73688 (one out-of-line Shrink_Table, 0x00602730, serves both: 0x005EDCE1 calls
// it with 0x00A73400, 0x005F9913 with 0x00A73688): its dynamic initializer (0x005DEB90) and atexit destructor
// (0x005DEC30, the next draft).

unsigned int __fastcall unknown_hash_window_5de970(HWND const &key);

WolDictionary<HWND, WNDPROC> unknown_window_table_a73400(unknown_hash_window_5de970);


// Compiler-generated atexit destructor of unknown_window_table_a73400 (defined in the previous draft,
// 0x005DEB90): WolDictionary's destructor inline; no handwritten body.


// The ownrdraw control registry at 0x00A73640 as wlib's chained dictionary (shared/wol_dictionary.hpp),
// keyed by window handle value: its dynamic initializer (0x005DED60) inlines the constructor and
// registers the atexit destructor (0x005DEE00, the next draft), which inlines the destructor with each
// record's destructor, clear() and shrink(). TibSun's ODWinData (Dictionary<HWND, WinData>).

inline void UnknownNodeList_006003d0::Remove_Node_00600410(UnknownNode_006003d0 *node)
{
    UnknownNode_006003d0 *previous = 0;
    UnknownNode_006003d0 *current = Head;
    while (current != 0) {
        if (node == current) {
            break;
        }
        previous = current;
        current = current->Next;
    }
    if (current != 0) {
        if (previous != 0) {
            UnknownNode_006003d0 *next = current->Next;
            previous->Next = next;
        } else {
            UnknownNode_006003d0 *next = current->Next;
            Head = next;
        }
        operator delete(current);
    }
}

namespace OwnerDraw {

inline WinData::~WinData()
{
    Set_Wide_Text_600510(0);
    while (list_34) {
        ((UnknownNodeList_006003d0 *)this)->Remove_Node_00600410(list_34);
    }
    delete unknown_3c;
}

}

// The key's value as its hash (0x005DE970, shared with the window tables).
unsigned int __fastcall unknown_hash_key_5de970(unsigned int const &key);

WolDictionary<unsigned int, OwnerDraw::WinData> unknown_control_table_a73640(unknown_hash_key_5de970);


// Compiler-generated atexit destructor of unknown_control_table_a73640 (defined in the previous draft,
// 0x005DED60): WolDictionary's destructor with ~WinData inline; no handwritten body.


#include "shared/rules.hpp"

namespace OwnerDraw {

int __fastcall unknown_5e06c0(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (((kind == 0x103) || (kind == 0xbc7)) && (id == 0x6f4)) {
        return Rules->MessageCharTyped;
    }
    return -1;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5e0e40(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (((((((kind != 0xbc) && (kind != 0xbd)) && (kind != 0xc2)) && (kind != 0xc9)) ||
             ((((id != 0x6d2 && (id != 0x6d3)) &&
                 ((id != 0x6d4 && ((id != 0x6d5 && (id != 0x6d6)))))) &&
                ((id != 0x6d7 && ((id != 0x6d8 && (id != 0x6d9)))))))) &&
            ((((kind != 0x102 && (((kind != 0xbc && (kind != 0xbd)) && (kind != 0xc2)))) &&
                (kind != 0xc9)) ||
             (((((id != 0x6da && (id != 0x6db)) &&
                    ((id != 0x6dc && ((id != 0x6dd && (id != 0x6de)))))) && (id != 0x6df)) &&
                ((id != 0x6e0 && (id != 0x6e1)))))))) &&
         (((kind != 0xc2 && (kind != 0xc9)) ||
            (((((id != 0x754 && (id != 0x755)) && (id != 0x756)) &&
                (((id != 0x757 && (id != 0x758)) && (id != 0x759)))) &&
             ((id != 0x75a && (id != 0x75b)))))))) {
        return false;
    }
    return true;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5e0ff0(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    return false;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5e1050(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (Is_Listed_Kind_5e8590(kind) && id == 0x71c) {
        return true;
    }
    if (kind == 0x94) {
        if (((id == 0x6ea) || (id == 0x6ec)) || (id == 0x6eb)) {
            return true;
        }
    }
    else if (kind == 0x103) {
        if (id == 0x72b) {
            return true;
        }
    }
    else if ((kind == 0xbc7) && (id == 0x72b)) {
        return true;
    }
    return false;
}

}


namespace OwnerDraw {

int __fastcall unknown_5e1240(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    return 0xc;
}

}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int OwnerDraw::unknown_5e12a0(HWND dialog, HWND window) (0x005E12A0): the sound slot of a dialog control
// (0x25 for the sidebar button 0x71C of the listed dialog kinds, 0xE for controls 0x6EC/0x5A8 of the option
// dialogs). Shape adapted from ra2-decomp src/ownrdraw.cpp Control_Sound_Slot (gamemd twin 0x00603040,
// matched there); see THIRD_PARTY.md. Expanded into the 0x005E8720 callback, where the Get_Record form sets
// its lookup's inline budget.

namespace OwnerDraw {

int __fastcall unknown_5e12a0(HWND dialog, HWND window)
{
    WinData *record = Get_Record(dialog);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (Is_Listed_Kind_5e8590(kind) && id == 0x71c) {
        return 0x25;
    }
    if (((((kind == 0xbc) || (kind == 0xbd)) || (kind == 0xc2)) ||
            ((kind == 0xc9 || (kind == 0x102)))) && ((id == 0x6ec || (id == 0x5a8)))) {
        return 0xe;
    }
    return 0;
}

}


namespace OwnerDraw {

int __fastcall unknown_5e1490(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (Is_Listed_Kind_5e8590(kind) && id == 0x71c) {
        return 1;
    }
    return 0;
}

}


namespace OwnerDraw {

int __fastcall unknown_5e1640(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (Is_Listed_Kind_5e8590(kind) && id == 0x71c) {
        return 100;
    }
    if (kind == 0x94) {
        if (id == 0x6ea || id == 0x6ec || id == 0x6eb) {
            return 100;
        }
    }
    else if (kind == 0x103) {
        if (id == 0x72b) {
            return 100;
        }
    }
    else if (kind == 0xbc7 && id == 0x72b) {
        return 100;
    }
    return 0;
}

}


void *__fastcall Unknown_Get_Art_006f3480();
void *__fastcall Unknown_Get_Art_006f34a0();
void *Unknown_Get_Art_6f2de0();
void *Unknown_Get_Art_6f2830();

namespace OwnerDraw {

void *__fastcall unknown_5e1820(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    void *art = Unknown_Get_Art_006f3480();
    if (Is_Listed_Kind_5e8590(kind) && id == 0x71c) {
        art = Unknown_Get_Art_006f34a0();
    }
    if (kind == 0x94) {
        if (id == 0x6ea || id == 0x6eb || id == 0x6ec) {
            return Unknown_Get_Art_6f2de0();
        }
    } else if (kind == 0x103) {
        if (id == 0x72b) {
            return Unknown_Get_Art_6f2830();
        }
    } else if (kind == 0xbc7 && id == 0x72b) {
        return Unknown_Get_Art_6f2830();
    }
    return art;
}

}


namespace OwnerDraw {

int __fastcall unknown_5e1a20(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    return 0;
}

}


void const *__fastcall Unknown_Load_Shape_00495270(char const *filename, bool &allocated);
extern char const *unknown_name_7e8d3c;
extern void *unknown_art_ac1120;
extern void *unknown_art_ac1040;
extern void *unknown_art_ac10fc;
extern void *unknown_art_ac1108;

namespace OwnerDraw {

void const *__fastcall unknown_5e1a80(unsigned int key, HWND window, bool &allocated)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (Is_Listed_Kind_5e8590(kind) && id == 0x71c) {
        return Unknown_Load_Shape_00495270(unknown_name_7e8d3c, allocated);
    }
    if (kind == 0x94) {
        if (id == 0x6ea) {
            return unknown_art_ac1120;
        }
        if (id == 0x6eb) {
            return unknown_art_ac1040;
        }
        if (id == 0x6ec) {
            return unknown_art_ac10fc;
        }
    } else if ((kind == 0x103 || kind == 0xbc7) && id == 0x72b) {
        return unknown_art_ac1108;
    }
    return 0;
}

}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int OwnerDraw::unknown_5e1ca0(HWND dialog, HWND window) (0x005E1CA0): 0xC0 when the dialog is one of the
// listed kinds (marking it band 2) and the control is an edit, a type-1 list box or a non-dropdown combo box.
// Shape adapted from ra2-decomp src/ownrdraw.cpp FUN_00603AA0 / TrySetDialogLayoutBand2 (gamemd 0x0060C7D0) with
// the class-name test as its own helper (FUN_006242B0, gamemd 0x006242B0), matched there; see THIRD_PARTY.md.
// The helper is expanded here and in the 0x005E8720 callback, which auto-inlines this body; game.exe has no body
// of the helper (GetClassNameA is called only here and inside 0x005E8720 in this unit).
#include "shared/crt_string.hpp"

namespace OwnerDraw {

// Whether the control is an edit, a registered list box of type 1 or a non-dropdown combo box
// (ra2-decomp FUN_006242B0).
inline bool __fastcall Is_Text_Control(HWND control)
{
    unsigned int key = (unsigned int)control;
    char name[128];
    GetClassNameA(control, name, 128);
    if (strcmp(name, "Edit") == 0) {
        return true;
    }
    if (strcmp(name, "NewEdit") == 0) {
        return true;
    }
    if (strcmp(name, "ListBox") == 0) {
        WinData *record = 0;
        if (unknown_control_table_a73640.Get_Pointer(key, &record)) {
            return record->unknown_68 == 1;
        }
    }
    if (strcmp(name, "ComboBox") == 0) {
        return (GetWindowLongA((HWND)key, GWL_STYLE) & 3) != 3;
    }
    return false;
}

int __fastcall unknown_5e1ca0(HWND dialog, HWND window)
{
    WinData *record = Get_Record(dialog);
    if (record != 0) {
        int kind = record->unknown_6c;
        if (kind == 0x10d || kind == 0xd9 || kind == 0xf0 || kind == 0xce || kind == 0x120 ||
            kind == 0x121 || kind == 0x115 || kind == 0xd3 || kind == 0xcf || kind == 0x11f ||
            kind == 0xc3 || kind == 0x11b || kind == 0xe1 || kind == 0x11e || kind == 0xc4 ||
            kind == 300 || kind == 0xd0 || kind == 0xfc || kind == 0x126) {
            record->unknown_b0 = 2;
            if (Is_Text_Control(window)) {
                return 0xc0;
            }
        }
    }
    return 0;
}

}


namespace OwnerDraw {

void __fastcall unknown_5e1f30(HWND control, Surface *image)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        record->unknown_70 = 2;
        record->image_14 = image;
        InvalidateRect(control, 0, TRUE);
    }
}

}


namespace OwnerDraw {

// Whether the control answers message 0x4E7 (0x005E1FF0).
bool __fastcall unknown_5e1ff0(HWND window)
{
    return SendMessageA(window, 0x4e7, 0, 0) ? true : false;
}

}


namespace OwnerDraw {

void __fastcall unknown_5e2040(HWND control, int value)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        record->unknown_c0 = value;
    }
}

}


namespace OwnerDraw {

int __fastcall unknown_5e20a0(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        return record->unknown_c0;
    }
    return -1;
}

}


namespace OwnerDraw {

WideString __fastcall unknown_5e2100(HWND window, int a, int b)
{
    UnknownTextRequest request;
    request.unknown_00 = a;
    request.unknown_04 = b;
    SendMessageA(window, 0x4e9, 0, (LPARAM)&request);
    return request.text;
}

}


namespace OwnerDraw {

WideString __fastcall unknown_5e2160(HWND window, int a)
{
    UnknownTextRequest request;
    request.unknown_00 = a;
    request.unknown_04 = -1;
    SendMessageA(window, 0x4e9, 0, (LPARAM)&request);
    return request.text;
}

}


namespace OwnerDraw {

WideString __fastcall unknown_5e21c0(HWND window, HWND item, int x, int y)
{
    int index = SendMessageA(item, 0x4e8, 0, (unsigned short)x | ((unsigned long)(unsigned short)y << 16));
    WideString text = unknown_5e2100(window, (int)item, index);
    return text;
}

}


#include "shared/string_table.hpp"
#include "shared/ipx_global_conn.hpp"

namespace OwnerDraw {

WideString __fastcall unknown_5e2260(unsigned int key, HWND window)
{
    char const *label = unknown_5e22b0(key, window);
    if (label) {
        return WideString(StringTable::Load_String(label, 0, "D:\\Ra2\\ownrdraw.cpp", 1902));
    }
    return WideString(unknown_empty_wide_839d4c);
}

}


#include "shared/unknown_globals.hpp"
#include "shared/wol_sink.hpp"

namespace OwnerDraw {

// The kind of the control registered under key (record +0x6C), 0 when it has no record (inline here:
// its record pointer gets no frame slot in 0x005E22B0).
inline int Record_Kind(unsigned int key)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    return record != 0 ? record->unknown_6c : 0;
}
char const *__fastcall unknown_5e22b0(unsigned int key, HWND window)
{
    int kind = Record_Kind(key);
    int id = GetDlgCtrlID(window);
    if (kind == 0xe2) {
        if (id == 0x683) {
            return "STT:MainButtonSinglePlayer";
        }
        if (id == 0x684) {
            return "STT:MainButtonWWOnline";
        }
        if (id == 0x578) {
            return "STT:MainButtonNetwork";
        }
        if (id == 0x686) {
            return "STT:MainButtonMovies";
        }
        if (id == 0x55c) {
            return "STT:MainButtonOptions";
        }
        if (id == 0x3ee) {
            return "STT:MainButtonExitGame";
        }
    } else if (kind == 0x100) {
        if (id == 0x688) {
            return "STT:SingleButtonNewCampaign";
        }
        if (id == 0x689) {
            return "STT:SingleButtonLoadSavedGame";
        }
        if (id == 0x579) {
            return "STT:SingleButtonSkirmish";
        }
        if (id == 0x686) {
            return "STT:SingleButtonBack";
        }
    } else if (kind == 0x94) {
        if (id == 0x6ea) {
            return "STT:CampaignAnimAllied";
        }
        if (id == 0x6eb) {
            return "STT:CampaignAnimTutorial";
        }
        if (id == 0x6ec) {
            return "STT:CampaignAnimSoviet";
        }
        if (id == 0x50f) {
            return "STT:CampaignSliderDifficulty";
        }
        if (id == 0x40e) {
            return "STT:CampaignButtonLoad";
        }
        if (id == 0x455) {
            return "STT:CampaignList";
        }
        if (id == 0x686) {
            return "STT:CampaignButtonBack";
        }
    } else if (kind == 0xb7) {
        if (id == 0x525) {
            return "STT:LoadList";
        }
        if (id == 0x40f) {
            return "STT:LoadButtonLoad";
        }
        if (id == 0x686) {
            return "STT:LoadButtonBack";
        }
    } else if (kind == 0x102) {
        if (id == 0x6a0) {
            return "STT:SkirmishEditPlayer";
        }
        if ((((id == 0x50b) || (id == 0x50e)) || (id == 0x516)) || (((id == 0x51a || (id == 0x51b)) || ((id == 0x51c || (id == 0x51d)))))) {
            return "STT:SkirmishComboAIPlayer";
        }
        if (((id == 0x6da) || (id == 0x6db)) || (((id == 0x6dc || (((id == 0x6dd || (id == 0x6de)) || (id == 0x6df)))) || ((id == 0x6e0 || (id == 0x6e1)))))) {
            return "STT:SkirmishPictureFlag";
        }
        if (((id == 0x6a1) || (id == 0x510)) || ((id == 0x513 || ((((id == 0x51e || (id == 0x514)) || (id == 0x51f)) || ((id == 0x520 || (id == 0x521)))))))) {
            return "STT:SkirmishComboCountry";
        }
        if (((id == 0x6a2) || (id == 0x522)) || (((id == 0x523 || (((id == 0x524 || (id == 0x525)) || (id == 0x526)))) || ((id == 0x527 || (id == 0x528)))))) {
            return "STT:SkirmishComboColor";
        }
        if (id == 0x529) {
            return "STT:SkirmishSliderSpeed";
        }
        if (id == 0x511) {
            return "STT:SkirmishSliderCredits";
        }
        if (id == 0x50c) {
            return "STT:SkirmishSliderUnit";
        }
        if (id == 0x5aa) {
            return "STT:SkirmishButtonChooseMap";
        }
        if (id == 0x693) {
            return "STT:SkirmishCBoxRedeploys";
        }
        if (id == 0x54e) {
            return "STT:SkirmishCBoxShortGame";
        }
        if (id == 0x69a) {
            return "STT:SkirmishCBoxSWAllowed";
        }
        if (id == 0x69d) {
            return "STT:SkirmishCBoxBuildOffAlly";
        }
        if (id == 0x696) {
            return "STT:SkirmishCBoxCrates";
        }
        if (id == 0x468) {
            return "STT:SkirmishMapThumbnail";
        }
        if (id == 0x6ec) {
            return "STT:SkirmishLabelGameType";
        }
        if (id == 0x5a8) {
            return "STT:SkirmishLabelScenario";
        }
        if (id == 0x617) {
            return "STT:SkirmishButtonStartGame";
        }
        if (id == 0x5c0) {
            return "STT:SkirmishButtonBack";
        }
    } else if (kind == 0xbb) {
        if (id == 0x541) {
            return "STT:LobbyEditYourName";
        }
        if (id == 0x53e) {
            return "STT:LobbyListUsers";
        }
        if (id == 0x549) {
            return "STT:LobbyListGames";
        }
        if (id == 0x53f) {
            return "STT:LobbyEditOutput";
        }
        if (id == 0x540) {
            return "STT:LobbyEditInput";
        }
        if (id == 0x6cc) {
            return "STT:LobbyButtonNew";
        }
        if (id == 0x6cd) {
            if (unknown_a72c68 == 0) {
                return "STT:LobbyButtonJoin";
            }
            return "STT:LobbyButtonObserve";
        }
        if (id == 0x760) {
            return "STT:ObserverToggle";
        }
        if (id == 0x6ce) {
            return "STT:LobbyButtonObserve";
        }
        if (id == 0x5c0) {
            return "STT:LobbyButtonBack";
        }
    } else if (kind == 0xbc || kind == 0xc2) {
        if ((((id == 0x6d2) || (((id == 0x6d3 || (id == 0x6d4)) || (id == 0x6d5)))) || (((id == 0x6d6 || (id == 0x6d7)) || (id == 0x6d8)))) || (id == 0x6d9)) {
            return "STT:HostPictureAcceptance";
        }
        if (((id == 0x411) || (id == 0x412)) || ((id == 0x413 || ((((id == 0x414 || (id == 0x415)) || (id == 0x416)) || ((id == 0x417 || (id == 0x418)))))))) {
            return "STT:HostComboPlayer";
        }
        if (((id == 0x6da) || (id == 0x6db)) || (((id == 0x6dc || (((id == 0x6dd || (id == 0x6de)) || (id == 0x6df)))) || ((id == 0x6e0 || (id == 0x6e1)))))) {
            return "STT:HostPictureFlag";
        }
        if ((((id == 0x6a1) || (id == 0x510)) || ((id == 0x513 || (((id == 0x51e || (id == 0x514)) || (id == 0x51f)))))) || ((id == 0x520 || (id == 0x521)))) {
            return "STT:HostComboCountry";
        }
        if (((id == 0x6a2) || (id == 0x522)) || ((id == 0x523 || ((((id == 0x524 || (id == 0x525)) || (id == 0x526)) || ((id == 0x527 || (id == 0x528)))))))) {
            return "STT:HostComboColor";
        }
        if (id == 0x529) {
            return "STT:HostSliderSpeed";
        }
        if (id == 0x402) {
            return "STT:HostSliderCredits";
        }
        if (id == 0x54f) {
            return "STT:HostSliderUnit";
        }
        if (id == 1000) {
            return "STT:HostCBoxRedeploys";
        }
        if (id == 0x54e) {
            return "STT:HostCBoxShortGame";
        }
        if (id == 0x69a) {
            return "STT:HostCBoxSWAllowed";
        }
        if (id == 0x69d) {
            return "STT:HostCBoxBuildOffAlly";
        }
        if (id == 0x54d) {
            return "STT:HostCBoxCrates";
        }
        if (id == 0x53f) {
            return "STT:HostEditOutput";
        }
        if (id == 0x540) {
            return "STT:HostEditInput";
        }
        if (id == 0x468) {
            return "STT:HostMapThumbnail";
        }
        if (id == 0x5a8) {
            return "STT:HostLabelScenario";
        }
        if (id == 0x588) {
            return "STT:HostButtonGo";
        }
        if (id == 0x5aa) {
            return "STT:HostButtonChooseMap";
        }
        if (id == 2) {
            if (kind == 0xc2) {
                return "STT:WOLNewGameCancel";
            }
            return "STT:HostButtonBack";
        }
    } else if (kind == 0xbd || kind == 0xc9) {
        if (((id == 0x6d2) || (((id == 0x6d3 || (id == 0x6d4)) || (id == 0x6d5)))) || ((((id == 0x6d6 || (id == 0x6d7)) || (id == 0x6d8)) || (id == 0x6d9)))) {
            return "STT:GuestPictureAcceptance";
        }
        if ((((id == 0x411) || (id == 0x412)) || ((id == 0x413 || (((id == 0x414 || (id == 0x415)) || (id == 0x416)))))) || ((id == 0x417 || (id == 0x418)))) {
            return "STT:GuestComboPlayer";
        }
        if (((id == 0x6da) || (id == 0x6db)) || ((id == 0x6dc || ((((id == 0x6dd || (id == 0x6de)) || (id == 0x6df)) || ((id == 0x6e0 || (id == 0x6e1)))))))) {
            return "STT:GuestPictureFlag";
        }
        if (((id == 0x6a1) || (id == 0x510)) || (((id == 0x513 || (((id == 0x51e || (id == 0x514)) || (id == 0x51f)))) || ((id == 0x520 || (id == 0x521)))))) {
            return "STT:GuestComboCountry";
        }
        if ((((id == 0x6a2) || (id == 0x522)) || ((id == 0x523 || (((id == 0x524 || (id == 0x525)) || (id == 0x526)))))) || ((id == 0x527 || (id == 0x528)))) {
            return "STT:GuestComboColor";
        }
        if (id == 0x529) {
            return "STT:GuestSliderSpeed";
        }
        if (id == 0x402) {
            return "STT:GuestSliderCredits";
        }
        if (id == 0x54f) {
            return "STT:GuestSliderUnit";
        }
        if (id == 1000) {
            return "STT:GuestCBoxRedeploys";
        }
        if (id == 0x54e) {
            return "STT:GuestCBoxShortGame";
        }
        if (id == 0x69a) {
            return "STT:GuestCBoxSWAllowed";
        }
        if (id == 0x69d) {
            return "STT:GuestCBoxBuildOffAlly";
        }
        if (id == 0x54d) {
            return "STT:GuestCBoxCrates";
        }
        if (id == 0x53f) {
            return "STT:GuestEditOutput";
        }
        if (id == 0x540) {
            return "STT:GuestEditInput";
        }
        if (id == 0x468) {
            return "STT:GuestMapThumbnail";
        }
        if (id == 0x5a8) {
            return "STT:GuestLabelScenario";
        }
        if (id == 0x59f) {
            return "STT:GuestButtonAccept";
        }
        if (id == 2) {
            if (kind == 0xc9) {
                return "STT:WOLNewGameCancel";
            }
            return "STT:GuestButtonBack";
        }
    } else if (kind == 0x101) {
        if (id == 0x68d) {
            return "STT:OptionsButtonSneak";
        }
        if (id == 0x68e) {
            return "STT:OptionsButtonMovies";
        }
        if (id == 0x68f) {
            return "STT:OptionsButtonCredits";
        }
        if (id == 0x686) {
            return "STT:OptionsButtonBack";
        }
    } else if (kind == 0x129) {
        if (id == 0x745) {
            return "GUI:PlayMovie";
        }
        if (id == 0x686) {
            return "STT:MsnDltButtonBack";
        }
        if (id == 0x744) {
            return "GUI:SelectMovie";
        }
    } else if (kind == 0xd5) {
        if (id == 0x5ce) {
            return "STT:MainOptButtonKeyboard";
        }
        if (id == 0x5cd) {
            return "STT:MainOptButtonNetwork";
        }
        if (id == 0x686) {
            return "STT:MainOptButtonBack";
        }
        if (id == 0x674) {
            return "STT:MainOptCBoxStretch";
        }
        if (id == 0x52b) {
            return "STT:MainOptSliderVisual";
        }
        if (id == 0x6ed) {
            return "STT:MainOptComboModes";
        }
        if (id == 0x529) {
            return "STT:MainOptSliderSpeed";
        }
        if (id == 0x50f) {
            return "STT:MainOptSliderDifficulty";
        }
        if (id == 0x601) {
            return "STT:MainOptCBoxTargetLines";
        }
        if (id == 0x604) {
            return "STT:MainOptCBoxHidden";
        }
        if (id == 0x602) {
            return "STT:MainOptCBoxTooltips";
        }
        if (id == 0x52a) {
            return "STT:MainOptSliderScroll";
        }
        if (id == 0x51a) {
            return "STT:MainOptCBoxScrollCoasting";
        }
        if (id == 0x52f) {
            return "STT:MainOptSliderMusic";
        }
        if (id == 0x532) {
            return "STT:MainOptSliderSound";
        }
        if (id == 0x536) {
            return "STT:MainOptSliderVoice";
        }
    } else if (kind == 0xd7) {
        if (id == 0x5d4) {
            return "STT:MultiOptComboCard";
        }
        if (id == 0x5d1) {
            return "STT:MultiOptEditSocketNum";
        }
        if (id == 0x5d3) {
            return "STT:MultiOptEditDestAddr";
        }
        if (id == 0x762) {
            return "STT:MultiOptSendDelay";
        }
        if (id == 0x5d7) {
            return "STT:MultiOptPortNumber";
        }
        if (id == 1) {
            return "STT:MultiOptButtonOk";
        }
        if (id == 2) {
            return "STT:MultiOptButtonCancel";
        }
    } else if (kind == 0xa3) {
        if (id == 0x4c7) {
            return "STT:KeyboardComboCategory";
        }
        if (id == 0x6e9) {
            return "STT:KeyboardGroupDescription";
        }
        if (id == 0x4c8) {
            return "STT:KeyboardListCommands";
        }
        if (id == 0x63c) {
            return "STT:KeyboardLabelShortcut";
        }
        if (id == 0x63d) {
            return "STT:KeyboardLabelError";
        }
        if (id == 0x4cd) {
            return "STT:KeyboardEditEntry";
        }
        if (id == 0x4cf) {
            return "STT:KeyboardButtonAssign";
        }
        if (id == 0x63b) {
            return "STT:KeyboardLabelAssigned";
        }
        if (id == 0x4d0) {
            return "STT:KeyboardButtonResetAll";
        }
        if (id == 0x686) {
            return "STT:KeyboardButtonBack";
        }
    } else if (kind == 0x10c) {
        if (id == 0x6e7) {
            return "STT:BriefingLabelContent";
        }
        if (id == 0x423) {
            return "STT:BriefingButtonReturn";
        }
    } else if (kind == 0xb6) {
        if (id == 0x6c9) {
            int state = unknown_5e20a0(window);
            if (state == 3) {
                return "STT:ConfirmExitButtonLeaveEx";
            }
            if (state == 2) {
                return "STT:ConfirmExitButtonLeave";
            }
            return "STT:Empty";
        }
        if (id == 0x712) {
            int state = unknown_5e20a0(window);
            if (state == 4) {
                return "STT:ConfirmExitButtonObserve";
            }
            if (state == 5) {
                return "STT:ConfirmExitButtonRestart";
            }
            return "STT:Empty";
        }
        if (id == 0x686) {
            return "STT:ConfirmExitButtonResume";
        }
    } else if (kind == 0xd9) {
        if (id == 0x5c1) {
            return "STT:ConfirmModeButtonOk";
        }
        if (id == 0x6ca) {
            return "STT:ConfirmModeButtonCancel";
        }
    } else if (kind == 0x73) {
        if ((((((id == 0x411) || (id == 0x412)) || (id == 0x413)) || ((id == 0x414 || (id == 0x415)))) || (id == 0x416)) || ((id == 0x417 || (id == 0x418)))) {
            return "STT:DiplomacyLabelPlayer";
        }
        if ((((id == 0x6cf) || (id == 0x6d0)) || (id == 0x6d1)) || (((id == 0x6d2 || (id == 0x6d3)) || ((id == 0x6d4 || (id == 0x6d5)))))) {
            return "STT:DiplomacyCBoxAllies";
        }
        if ((((id == 0x6d6) || (id == 0x6d7)) || ((id == 0x6d8 || (((id == 0x6d9 || (id == 0x6e2)) || (id == 0x6e3)))))) || (id == 0x6e4)) {
            return "STT:DiplomacyCBoxListeners";
        }
        if ((((id == 0x419) || (id == 0x41a)) || ((id == 0x41b || ((id == 0x41c || (id == 0x41d)))))) || ((id == 0x41e || ((id == 0x41f || (id == 0x420)))))) {
            return "STT:DiplomacyCBoxKills";
        }
        if (id == 0x423) {
            return "STT:DiplomacyButtonReturn";
        }
        if (id == 0x730) {
            return "STT:TauntsOn";
        }
    } else if (kind == 0xbbb) {
        if (id == 0x529) {
            return "STT:IGGameOptSliderSpeed";
        }
        if (id == 0x52a) {
            return "STT:IGGameOptSliderScroll";
        }
        if (id == 0x52b) {
            return "STT:IGGameOptSliderVisual";
        }
        if (id == 0x602) {
            return "STT:IGGameOptCBoxTooltips";
        }
        if (id == 0x601) {
            return "STT:IGGameOptCBoxTargetLines";
        }
        if (id == 0x604) {
            return "STT:IGGameOptCBoxHidden";
        }
        if (id == 0x51a) {
            return "STT:IGGameOptCBoxScrollCoast";
        }
        if (id == 0x52d) {
            return "STT:IGGameOptButtonSound";
        }
        if (id == 0x52c) {
            return "STT:IGGameOptButtonKeyboard";
        }
        if (id == 0x686) {
            return "STT:IGGameOptButtonBack";
        }
    } else if (kind == 0x105) {
        if (id == 0x405) {
            return "STT:GenerateCBoxEnvironment";
        }
        if (id == 0x3ea) {
            return "STT:GenerateCBoxTime";
        }
        if (id == 0x407) {
            return "STT:GenerateCBoxTheater";
        }
        if (id == 0x406) {
            return "STT:GenerateCBoxMapSize";
        }
        if (id == 0x408) {
            return "STT:GenerateCBoxResources";
        }
        if (id == 0x3eb) {
            return "STT:GenerateSliderNumPlayers";
        }
        if (id == 0x621) {
            return "STT:GenerateButtonSurprise";
        }
        if (id == 0x620) {
            return "STT:GenerateButtonPreview";
        }
        if (id == 0x6c2) {
            return "STT:GenerateButtonLoadMap";
        }
        if (id == 0x6c3) {
            return "STT:GenerateButtonSaveMap";
        }
        if (id == 0x6c4) {
            return "STT:GenerateButtonDeleteMap";
        }
        if (id == 0x6c5) {
            return "STT:GenerateButtonUseMap";
        }
        if (id == 0x5c0) {
            return "STT:GenerateButtonCancel";
        }
    } else if (kind == 0x2b5) {
        if (id == 0x528) {
            return "STT:MsnDltListMission";
        }
        if (id == 0x6c8) {
            return "STT:MsnDltButtonDelete";
        }
        if (id == 0x686) {
            return "STT:MsnDltButtonBack";
        }
    } else if (kind == 0x2b4) {
        if (id == 0x527) {
            return "STT:MsnSaveListMission";
        }
        if (id == 0x526) {
            return "STT:MsnSaveEditName";
        }
        if (id == 0x6c7) {
            return "STT:MsnSaveButtonSave";
        }
        if (id == 0x686) {
            return "STT:MsnSaveButtonBack";
        }
    } else if (kind == 0x108) {
        if (id == 0x6d2) {
            return "STT:MPScoreLabelGame";
        }
        if (id == 0x3ea) {
            return "STT:MPScoreLabelTime";
        }
        if ((id == 0x411) || (id == 0x412) || (id == 0x413) || (id == 0x414) || (id == 0x415) || (id == 0x416) || (id == 0x417) || (id == 0x418)) {
            return "STT:MPScoreLabelPlayer";
        }
        if ((id == 0x419) || (id == 0x41a) || (id == 0x41b) || (id == 0x41c) || (id == 0x41d) || (id == 0x41e) || (id == 0x41f) || (id == 0x420)) {
            return "STT:MPScoreLabelKills";
        }
        if ((id == 0x6a3) || (id == 0x6a4) || (id == 0x6a9) || (id == 0x6ac) || (id == 0x6af) || (id == 0x6b2) || (id == 0x6b5) || (id == 0x6b8)) {
            return "STT:MPScoreLabelLosses";
        }
        if ((id == 0x6a5) || (id == 0x6a7) || (id == 0x6aa) || (id == 0x6ad) || (id == 0x6b0) || (id == 0x6b3) || (id == 0x6b6) || (id == 0x6b9)) {
            return "STT:MPScoreLabelBuilt";
        }
        if ((id == 0x6a6) || (id == 0x6a8) || (id == 0x6ab) || (id == 0x6ae) || (id == 0x6b1) || (id == 0x6b4) || (id == 0x6b7) || (id == 0x6ba)) {
            return "STT:MPScoreLabelScore";
        }
        if (id == 0x6d1) {
            return "STT:MPScoreButtonContinue";
        }
    } else if (kind == 0xb5) {
        if (id == 0x521) {
            return "STT:IGSingleOptButtonControls";
        }
        if (id == 0x51e) {
            return "STT:IGSingleOptButtonLoad";
        }
        if (id == 0x51f) {
            return "STT:IGSingleOptButtonSave";
        }
        if (id == 0x520) {
            return "STT:IGSingleOptButtonDelete";
        }
        if (id == 0x522) {
            return "STT:IGSingleOptButtonAbort";
        }
        if (id == 0x686) {
            return "STT:IGSingleOptButtonBack";
        }
    } else if (kind == 0xbba) {
        if (id == 0x521) {
            return "STT:IGMultiOptButtonControls";
        }
        if (id == 0x522) {
            return "STT:IGMultiOptButtonAbort";
        }
        if (id == 0x686) {
            return "STT:IGMultiOptButtonResume";
        }
    } else if (kind == 0xea) {
        if ((((id == 0x640) || (id == 0x642)) || (id == 0x641)) || (((id == 0x643 || (id == 0x644)) || ((id == 0x645 || ((id == 0x646 || (id == 0x647)))))))) {
            return "STT:ReconnectButtonPlayer";
        }
        if ((((id == 0x648) || (id == 0x649)) || (id == 0x64a)) || (((id == 0x64b || (id == 0x64c)) || ((id == 0x64d || ((id == 0x64e || (id == 0x64f)))))))) {
            return "STT:ReconnectProgressPlayer";
        }
        if (id == 0x650) {
            return "STT:ReconnectLabelTime";
        }
        if (id == 0x651) {
            return "STT:ReconnectEditInfo";
        }
        if (id == 2) {
            return "STT:ReconnectButtonLeaveGame";
        }
    } else if (kind == 0x6b) {
        if (id == 0x6eb) {
            return "STT:ScenarioListGameType";
        }
        if (id == 0x553) {
            return "STT:ScenarioListMaps";
        }
        if (id == 0x468) {
            return "STT:ScenarioMapThumbnail";
        }
        if (id == 0x6c5) {
            return "STT:ScenarioButtonUseMap";
        }
        if (id == 0x583) {
            return "STT:ScenarioButtonRandom";
        }
        if (id == 0x5c0) {
            return "STT:ScenarioButtonCancel";
        }
    } else if (kind == 0xb8) {
        if (id == 0x52f) {
            return "STT:SoundOptSliderMusic";
        }
        if (id == 0x532) {
            return "STT:SoundOptSliderSound";
        }
        if (id == 0x536) {
            return "STT:SoundOptSliderVoice";
        }
        if (id == 0x530) {
            return "STT:SoundOptListScore";
        }
        if (id == 0x533) {
            return "STT:SoundOptCBoxShuffle";
        }
        if (id == 0x534) {
            return "STT:SoundOptCBoxRepeat";
        }
        if (id == 0x531) {
            return "STT:SoundOptButtonPlay";
        }
        if (id == 0x535) {
            return "STT:SoundOptButtonStop";
        }
        if (id == 0x686) {
            return "STT:SoundOptButtonBack";
        }
    } else if (kind == 0x103) {
        if (id == 0x6f1) {
            return "STT:StandScoreParTime";
        }
        if (id == 0x6f2) {
            return "STT:StandScoreYourTime";
        }
        if (id == 0x6f3) {
            return "STT:StandScoreTitle";
        }
        if (id == 0x6f4) {
            return "STT:StandScoreMessage";
        }
        if (id == 0x6f6) {
            return "STT:StandScoreKills";
        }
        if (id == 0x423) {
            return "STT:StandScoreButtonReturn";
        }
    } else if (kind == 0xff) {
        if (id == 0x3ef) {
            return "STT:OptionInternetLatency";
        }
        if (id == 0x529) {
            return "STT:OptionInternetSpeedSlider";
        }
        if (id == 0x521) {
            return "STT:OptionInternetGameControls";
        }
        if (id == 0x522) {
            return "STT:OptionInternetAbortMission";
        }
        if (id == 0x675) {
            return "STT:IGMultiOptButtonResume";
        }
    } else if (kind == 0x122) {
        if (id == 0x6f9) {
            return "STT:WOLAddBuddyQueryDoNotAsk";
        }
        if (id == 1) {
            return "STT:WOLAddBuddyQueryYes";
        }
        if (id == 2) {
            return "STT:WOLAddBuddyQueryNo";
        }
    } else if (kind == 0x112) {
        if (id == 0x6f9) {
            return "STT:WOLAutoLoginDoNotAsk";
        }
        if (id == 1) {
            return "STT:WOLAutoLoginYes";
        }
        if (id == 2) {
            return "STT:WOLAutoLoginNo";
        }
    } else if (kind == 0x116) {
        if (id == 0x689) {
            return "STT:WOLBuddyBuddyList";
        }
        if (id == 0x6f0) {
            return "STT:WOLBuddyWins";
        }
        if (id == 0x6f1) {
            return "STT:WOLBuddyLosses";
        }
        if (id == 0x701) {
            return "STT:WOLBuddyDisconnects";
        }
        if (id == 0x6f4) {
            return "STT:WOLBuddyRank";
        }
        if (id == 0x6f5) {
            return "STT:WOLBuddyPoints";
        }
        if (id == 0x62b) {
            return "STT:WOLBuddyCreateGame";
        }
        if (id == 0x62d) {
            return "STT:WOLBuddyJoin";
        }
        if (id == 0x760) {
            return "STT:ObserverToggle";
        }
        if (id == 0x5c2) {
            return "STT:WOLBuddyPage";
        }
        if (id == 0x702) {
            return "STT:WOLBuddyAdd";
        }
        if (id == 0x703) {
            return "STT:WOLBuddyRemove";
        }
        if (id == 0x704) {
            return "STT:WOLBuddyRefresh";
        }
        if (id == 0x686) {
            return "STT:WOLBuddyBack";
        }
    } else if (kind == 0x113) {
        if (id == 0x55d) {
            return "STT:WOLLobbyRefreshChannels";
        }
        if (id == 0x53f) {
            return "STT:LobbyEditOutput";
        }
        if (id == 0x540) {
            return "STT:LobbyEditInput";
        }
        if (id == 0x3ed) {
            return "STT:EmoteButton";
        }
        if (id == 0x53d) {
            return "STT:WOLLobbyChannelList";
        }
        if (id == 0x62b) {
            return "STT:LobbyButtonNew";
        }
        if (id == 0x62d) {
            if (unknown_a72c68 == 0) {
                return "STT:LobbyButtonJoin";
            }
            return "STT:LobbyButtonObserve";
        }
        if (id == 0x760) {
            return "STT:ObserverToggle";
        }
        if (id == 0x689) {
            return "STT:WOLBuddy";
        }
        if (id == 0x558) {
            return "STT:WOLClanLadder";
        }
        if (id == 0x61f) {
            return "STT:WOLLobbyOptions";
        }
        if (id == 0x686) {
            return "STT:WOLLobbyBack";
        }
    } else if (kind == 0xfe) {
        if (id == 0x3f0) {
            return "STT:WOLMyInformationLocale";
        }
        if (id == 1) {
            return "STT:WOLLocaleOK";
        }
        if (id == 2) {
            return "STT:WOLLocaleCancel";
        }
    } else if (kind == 0x10f) {
        if (id == 0x628) {
            return "STT:WOLLoginNickname";
        }
        if (id == 0x72c) {
            return "STT:WOLLoginPassword";
        }
        if (id == 0x72d) {
            return "STT:WOLLoginRemember";
        }
        if (id == 0x539) {
            return "STT:WOLLoginLogin";
        }
        if (id == 0x6ea) {
            return "STT:WOLNewAccount";
        }
        if (id == 0x6eb) {
            return "STT:WOLLoginForget";
        }
        if (id == 0x686) {
            return "STT:WOLLoginBack";
        }
        if (id == 0x6ec) {
            return "STT:WOLManageAccount";
        }
    } else if (kind == 0x11d) {
        if (id == 0x628) {
            return "STT:IGRLogin";
        }
        if (id == 0x72c) {
            return "STT:WOLLoginPassword";
        }
        if (id == 0x539) {
            return "STT:WOLLoginLogin";
        }
        if (id == 0x6ea) {
            return "STT:WOLNewAccount";
        }
        if (id == 0x686) {
            return "STT:WOLLoginBack";
        }
        if (id == 0x6ec) {
            return "STT:WOLManageAccount";
        }
        if (id == 0x629) {
            return "STT:WOLMyInformationServer";
        }
        if (id == 0x62a) {
            return "STT:WOLMyInformationLocale";
        }
        if (id == 0x6a1) {
            return "STT:WOLMyInformationCountry";
        }
        if (id == 0x6a2) {
            return "STT:WOLMyInformationColor";
        }
    } else if (kind == 0x117) {
        if (id == 0x53f) {
            return "STT:WOLMatchOutput";
        }
        if (id == 0x5c0) {
            return (unknown_game_mode_a3d298 == 4) && (unknown_a3d2a4 == 2) ? "WOL:WDTMatchBack" : "STT:WOLMatchCancel";
        }
    } else if (kind == 0x114) {
        if (id == 0x628) {
            return "STT:WOLMyInformationName";
        }
        if (id == 0x629) {
            return "STT:WOLMyInformationServer";
        }
        if (id == 0x62a) {
            return "STT:WOLMyInformationLocale";
        }
        if (id == 0x6a1) {
            return "STT:WOLMyInformationCountry";
        }
        if (id == 0x6a2) {
            return "STT:WOLMyInformationColor";
        }
        if (id == 0x6ff) {
            return "STT:WOLMyInformationAutoLogin";
        }
        if (id == 0x6f0) {
            return "STT:WOLMyInformationWins";
        }
        if (id == 0x6f1) {
            return "STT:WOLMyInformationLosses";
        }
        if (id == 0x701) {
            return "STT:WOLMyInformationDisconnects";
        }
        if (id == 0x6f4) {
            return "STT:WOLMyInformationRank";
        }
        if (id == 0x6f5) {
            return "STT:WOLMyInformationPoints";
        }
        if (id == 0x687) {
            return "STT:WOLBuddy";
        }
        if (id == 0x688) {
            return "STT:WOLClanLadder";
        }
        if (id == 0x686) {
            return "STT:WOLMyInformationBack";
        }
        if (id == 0x6ea) {
            return "STT:WOLNewAccount";
        }
        if (id == 0x6eb) {
            return "STT:WOLLoginForget";
        }
        if (id == 0x6ec) {
            return "STT:WOLManageAccount";
        }
    } else if (kind == 0xc3) {
        if (id == 0x548) {
            return "STT:WOLNewGameMaxPlayers";
        }
        if (id == 0x72c) {
            return "STT:WOLNewGamePassword";
        }
        if (id == 0x629) {
            return "STT:WOLNewGameTournament";
        }
        if (id == 0x66c) {
            return "STT:WOLNewGameClanTournament";
        }
        if (id == 1) {
            return "STT:WOLNewGameOK";
        }
        if (id == 2) {
            return "STT:WOLNewGameCancel";
        }
        if (id == 0x71e) {
            return "STT:WOLLimitScreenResolution";
        }
    } else if (kind == 0x2bc) {
        if (id == 0x5f5) {
            return "STT:WOLOptionsPage";
        }
        if (id == 0x5f6) {
            return "STT:WOLOptionsLanguageFilter";
        }
        if (id == 0x5f7) {
            return "STT:WOLOptionsFind";
        }
        if (id == 0x5f8) {
            return "STT:WOLOptionsMusic";
        }
        if (id == 0x5f9) {
            return "STT:WOLOptionsShowAllInLobby";
        }
        if (id == 1) {
            return "STT:WOLOptionsOk";
        }
        if (id == 0x63a) {
            return "STT:WOLOptionsSoundOptions";
        }
        if (id == 2) {
            return "STT:WOLOptionsCancel";
        }
        if (id == 0x6ed) {
            return "STT:MainOptComboModes";
        }
        if (id == 0x5fa) {
            return "STT:WOLOptionFilterAsian";
        }
        if (id == 0x5fb) {
            return "STT:WOLOptionFilterNonAsian";
        }
    } else if (kind == 0x10e) {
        if (id == 0x6f0) {
            return "STT:WOLMyInformationWins";
        }
        if (id == 0x6f1) {
            return "STT:WOLMyInformationLosses";
        }
        if (id == 0x701) {
            return "STT:WOLMyInformationDisconnects";
        }
        if (id == 0x6f4) {
            return "STT:WOLMyInformationRank";
        }
        if (id == 0x6f5) {
            return "STT:WOLMyInformationPoints";
        }
        if (id == 0x6e0) {
            return "STT:WOLWelcomeQuickMatch";
        }
        if (id == 0x6e1) {
            return "STT:WOLWelcomeCustomMatch";
        }
        if (id == 0x6e2) {
            return "STT:WOLBuddy";
        }
        if (id == 0x6e3) {
            return "STT:WOLWelcomeWDT";
        }
        if (id == 0x6e4) {
            return "STT:WOLWelcomeMyInformation";
        }
        if (id == 0x686) {
            return "STT:WOLWelcomeBack";
        }
    } else if (kind == 0x11c) {
        if (id == 0x6e0) {
            return "STT:WOLWelcomeQuickMatch";
        }
        if (id == 0x6e1) {
            return "STT:WOLWelcomeCustomMatch";
        }
        if (id == 0x6e2) {
            return "STT:WOLBuddy";
        }
        if (id == 0x6e3) {
            return "STT:WOLWelcomeWDT";
        }
        if (id == 0x686) {
            return "STT:WOLWelcomeBack";
        }
    } else if (kind == 0xe7) {
        if (id == 0x62e) {
            return "STT:WOLBeginNickMonth";
        }
        if (id == 0x62f) {
            return "STT:WOLBeginNickDay";
        }
        if (id == 0x630) {
            return "STT:WOLBeginNickYear";
        }
        if (id == 0x626) {
            return "STT:WOLBeginNickEmail";
        }
        if ((id == 0x631) || (id == 0x70a)) {
            return "STT:WOLBeginNickNewsletter";
        }
        if (id == 1) {
            return "STT:WOLBeginNickContinue";
        }
        if (id == 0x576) {
            return "STT:WOLBeginNickViewPrivacy";
        }
        if (id == 2) {
            return "STT:WOLBeginNickCancel";
        }
    } else if (kind == 0xe6 || kind == 0xf3 || kind == 0xf4) {
        if (id == 1) {
            return "STT:WOLNewNickOk";
        }
        if (id == 2) {
            return "STT:WOLNewNickCancel";
        }
        if (id == 0x628) {
            return "STT:WOLNewNickNickname";
        }
        if (id == 0x72c) {
            return "STT:WOLNewNickPassword";
        }
        if (id == 0x627) {
            return "STT:WOLNewNickVerify";
        }
        if (id == 0x5e5) {
            return "STT:WOLNewNickParentEMail";
        }
        if ((id == 0x66b) || (id == 0x70b)) {
            return "STT:WOLNewNickSellInfo";
        }
    } else if (kind == 0x125) {
        if (id == 0x686) {
            return "WOL:WDTFactionBack";
        }
        if (id != 0) {
            return "WDT:FactionSelectToolTip";
        }
    } else if (kind == 0xbc6) {
        if (((id == 0x411) || (id == 0x412)) || ((id == 0x413 || ((((id == 0x414 || (id == 0x415)) || (id == 0x416)) || ((id == 0x417 || (id == 0x418)))))))) {
            return "STT:MPScoreLabelPlayer";
        }
        if (((id == 0x419) || ((id == 0x41a || (id == 0x41b)))) || (((id == 0x41c || (((id == 0x41d || (id == 0x41e)) || (id == 0x41f))) ) || (id == 0x420)))) {
            return "STT:MPScoreLabelKills";
        }
        if ((((id == 0x6a3) || (id == 0x6a4)) || ((id == 0x6a9 || ((id == 0x6ac || (id == 0x6af)))))) || ((id == 0x6b2 || ((id == 0x6b5 || (id == 0x6b8)))))) {
            return "STT:MPScoreLabelLosses";
        }
        if ((((id == 0x6a5) || (((id == 0x6a7 || (id == 0x6aa)) || (id == 0x6ad)))) || (((id == 0x6b0 || (id == 0x6b3)) || (id == 0x6b6)))) || (id == 0x6b9)) {
            return "STT:MPScoreLabelBuilt";
        }
        if (((id == 0x6a6) || (id == 0x6a8)) || ((id == 0x6ab || ((((id == 0x6ae || (id == 0x6b1)) || (id == 0x6b4)) || ((id == 0x6b7 || (id == 0x6ba)))))))) {
            return "STT:MPScoreLabelScore";
        }
        if (((id == 0x70a) || ((id == 0x70b || (id == 0x70c)))) || (((id == 0x70d || (((id == 0x70e || (id == 0x70f)) || (id == 0x710)))) || (id == 0x711)))) {
            return "STT:MPScoreLabelStatus";
        }
        if (id == 0x733) {
            return "STT:MPScoreLabelCampaignName";
        }
        if (id == 0x71b) {
            return "STT:MPScoreLabelMapName";
        }
        if (id == 0x732) {
            return "STT:MPScoreLabelDifficulty";
        }
        if (id == 0x6d1) {
            return "STT:MPCampScoreContinue";
        }
        if (id == 0x5c0) {
            return "STT:MPCampScoreQuit";
        }
        if (id == 0x3ea) {
            return "STT:MPScoreLabelTime";
        }
    } else if (kind == 0xbc7) {
        if (id == 0x733) {
            return "STT:MPScoreLabelCampaignName";
        }
        if (id == 0x735) {
            return "STT:MPCampEndCampaignNumber";
        }
        if (id == 0x732) {
            return "STT:MPScoreLabelDifficulty";
        }
        if (id == 0x3ea) {
            return "STT:MPCampEndTime";
        }
        if (((id == 0x411) || (id == 0x412)) || ((id == 0x73a || (id == 0x739)))) {
            return "STT:MPScoreLabelPlayer";
        }
        if ((id == 0x419) || (id == 0x41a)) {
            return "STT:MPScoreLabelKills";
        }
        if ((id == 0x6a3) || (id == 0x6a4)) {
            return "STT:MPScoreLabelLosses";
        }
        if ((id == 0x6a5) || (id == 0x6a7)) {
            return "STT:MPScoreLabelBuilt";
        }
        if ((((id == 0x6a6) || (id == 0x6a8)) || (id == 0x73b)) || (id == 0x738)) {
            return "STT:MPScoreLabelScore";
        }
        if (id == 0x6d1) {
            return "STT:MPScoreButtonContinue";
        }
    }
    return 0;
}

}


namespace OwnerDraw {

void __fastcall unknown_5e46c0(HWND control, bool value)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        record->unknown_bc = value;
    }
}

}


namespace OwnerDraw {

bool __fastcall unknown_5e4720(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        return record->unknown_bc;
    }
    return false;
}

}


namespace seed_005df660 {
unsigned char __fastcall unknown_kind_test_005df660(int);
}
namespace OwnerDraw {

bool __fastcall Is_Listed_Kind_5e0da0(int kind);
BOOL __fastcall Is_Listed_Kind_5e8590(int kind);
void __fastcall unknown_5e46c0(HWND control, bool value);

inline bool Is_Option_Kind(int kind)
{
    return kind == 0xbc || kind == 0xbd || kind == 0x102 || kind == 0xc2 || kind == 0xc9;
}
inline bool Is_Kind_103(int kind)
{
    return kind == 0x103 || kind == 0xbc7;
}
inline bool Is_Kind_108(int kind)
{
    return kind == 0x108 || kind == 0xbc6;
}
inline bool Is_Rail_ID(int id)
{
    return id == 0x6ec || id == 0x5a8;
}
// BS_OWNERDRAW style.
inline bool Is_Owner_Button(HWND window)
{
    return (GetWindowLongA(window, GWL_STYLE) & 0xb) == 0xb;
}
inline bool Is_Free_Record(HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)window, &record);
    return record != 0 && record->unknown_68 == 0;
}
inline int Kind_Of(unsigned int key)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    return record != 0 ? record->unknown_6c : 0;
}
// EnumChildWindows callback (0x005E4780): stores lparam in the control's record +0xBC for the listed
// kind and control ID pairs and for free owner-draw buttons accepted by 0x005E6860 or 0x005E72C0.
BOOL __stdcall unknown_5e4780(HWND window, LPARAM lparam)
{
    HWND parent = GetParent(window);
    int kind = Kind_Of((unsigned int)parent);
    int id = GetDlgCtrlID(window);
    bool value = lparam != 0;
    if (!(((kind == 0x10c || kind == 0x94 || kind == 0xa3 || kind == 0x73 || kind == 0xd8 || kind == 0xbbb || kind == 0xf5 || kind == 0x105 || kind == 0xe2 || kind == 0xd5 || kind == 0x2b5 || kind == 0xb7 || kind == 0x2b4 || kind == 0xbc7 || kind == 0x101 || kind == 0x129 || kind == 0xbc6 || kind == 0xd7 || kind == 0x108 || kind == 0xbc || kind == 0xbd || kind == 0xbb || kind == 0xb5 || kind == 0xbba || kind == 0xff || kind == 0x6b || kind == 0x100 || kind == 0x102 || kind == 0xb8 || kind == 0xd6 || kind == 0x103 || Is_Listed_Kind_5e0da0(kind)) && id == 0x694)
        || (seed_005df660::unknown_kind_test_005df660(kind) && id == 0x695)
        || (Is_Listed_Kind_5e8590(kind) && id == 0x71c)
        || ((Is_Option_Kind(kind) || kind == 0x105 || kind == 0x6b || kind == 0x113) && id == 0x468)
        || ((Is_Option_Kind(kind)) && Is_Rail_ID(id))
        || (Is_Kind_103(kind) && (id == 0x6f3 || id == 0x6f4 || id == 0x72b || id == 0x6ef || id == 0x6f1 || id == 0x6f0 || id == 0x6f2 || id == 0x69f || id == 0x6f6 || id == 0x733 || id == 0x735 || id == 0x732 || id == 0x3ea || id == 0x736 || id == 0x737 || id == 0x73c || id == 0x73d || id == 0x73e || id == 0x73f || id == 0x740 || id == 0x411 || id == 0x412 || id == 0x419 || id == 0x41a || id == 0x6a3 || id == 0x6a4 || id == 0x6a5 || id == 0x6a7 || id == 0x6a6 || id == 0x6a8 || id == 0x73a || id == 0x739 || id == 0x73b || id == 0x738))
        || (Is_Kind_108(kind) && (id == 0x6d2 || id == 0x3ea || id == 0x69d || id == 0x69f || id == 0x69e || id == 0x6a0 || id == 0x6a1 || id == 0x5c8 || id == 0x411 || id == 0x412 || id == 0x413 || id == 0x414 || id == 0x415 || id == 0x416 || id == 0x417 || id == 0x418 || id == 0x419 || id == 0x41a || id == 0x41b || id == 0x41c || id == 0x41d || id == 0x41e || id == 0x41f || id == 0x420 || id == 0x6a3 || id == 0x6a4 || id == 0x6a9 || id == 0x6ac || id == 0x6af || id == 0x6b2 || id == 0x6b5 || id == 0x6b8 || id == 0x6a5 || id == 0x6a7 || id == 0x6aa || id == 0x6ad || id == 0x6b0 || id == 0x6b3 || id == 0x6b6 || id == 0x6b9 || id == 0x6a6 || id == 0x6a8 || id == 0x6ab || id == 0x6ae || id == 0x6b1 || id == 0x6b4 || id == 0x6b7 || id == 0x6ba || id == 0x70a || id == 0x70b || id == 0x70c || id == 0x70d || id == 0x70e || id == 0x70f || id == 0x710 || id == 0x711 || id == 0x733 || id == 0x732 || id == 0x71b || id == 0x735))
        || (Is_Owner_Button(window) && Is_Free_Record(window) && ::OwnerDraw_Control_Test_005e6860((unsigned int)parent, window))
        || (Is_Owner_Button(window) && Is_Free_Record(window) && unknown_5e72c0((unsigned int)parent, window)))) {
        return TRUE;
    }
    unknown_5e46c0(window, value);
    return TRUE;
}

}


#include "shared/win32_window.hpp"
#include "shared/rect.hpp"
#include "shared/surface.hpp"
#include "shared/palette.hpp"
#include "shared/sound_play.hpp"
#include "shared/mouse.hpp"

void *__fastcall Unknown_Get_Art_006f3480();
void *__fastcall Unknown_Get_Art_006f34c0();
void *Unknown_Get_Art_6f2830();
Point2D __fastcall Unknown_Point_Copy_006F1AE0(Point2D const *source);
void __cdecl Audio_Update();
extern ShapeFileStruct *unknown_shape_ac10e8;
extern ShapeFileStruct *unknown_shape_ac10b8;
extern ShapeFileStruct *unknown_shape_ac0fec;
extern ShapeFileStruct *unknown_shape_ac10b4;
extern ShapeFileStruct *unknown_shape_ac10f0;
extern ShapeFileStruct *unknown_shape_ac110c;
extern int unknown_count_ac102c;
extern Rect *unknown_rect_ac11b4;
extern Rect *unknown_rect_ac11b8;
extern Rect *unknown_rect_ac11bc;
extern Rect *unknown_rect_ac11c0;
extern Rect *unknown_rect_ac11c4;
extern Rect *unknown_rect_ac11cc;

namespace OwnerDraw {

BOOL __stdcall unknown_5e7d00(HWND window, LPARAM);
BOOL __stdcall unknown_5e7dd0(HWND window, LPARAM);

// Visible owner-drawn buttons of the dialog (inline; ra2-decomp's FUN_0060A230 in YR).
inline int Visible_Count_A737E4(HWND dialog)
{
    unknown_visible_count_a737e4 = 0;
    EnumChildWindows(dialog, unknown_5e7d00, 0);
    return unknown_visible_count_a737e4;
}

// Whether any 0x005E7DD0 control is visible (inline; ra2-decomp's FUN_0060A300 in YR).
inline bool Any_Visible_A763B4(HWND dialog)
{
    unknown_visible_count_a763b4 = 0;
    EnumChildWindows(dialog, unknown_5e7dd0, 0);
    return unknown_visible_count_a763b4 >= 1;
}

// A piece's slide phase at a step: -1 before it starts, -2 once its six frames are done.
inline int Slide_Phase(int delta)
{
    if (delta < 0) return -1;
    if (delta >= 6) return -2;
    return delta;
}

// The latest start step in a piece order table.
inline int Last_Step(int const *order, int pieces)
{
    int steps = 0;
    for (int j = 0; j < pieces; j++) {
        if (steps <= order[j]) steps = order[j];
    }
    return steps;
}

// Slides the dialog's frame pieces in (reverse) or out, one piece after another, then notifies the
// dialog with 0x4EC (in) or 0x4ED (out) (0x005E5150; YR DrawCampaignMenuTransition).
void __fastcall unknown_5e5150(HWND dialog, bool reverse)
{
    WinData *record = Get_Record(dialog);
    bool has_d7 = record ? record->unknown_d7 : false;
    record = Get_Record(dialog);
    bool has_d6 = record ? record->unknown_d6 : false;
    record = Get_Record(dialog);
    bool has_d5 = record ? record->unknown_d5 : false;

    ConvertClass *drawer = (ConvertClass *)Unknown_Get_Art_006f3480();
    ConvertClass *drawer_b = (ConvertClass *)Unknown_Get_Art_006f34c0();
    ShapeFileStruct *shape_e8 = unknown_shape_ac10e8;
    ShapeFileStruct *shape_b8 = unknown_shape_ac10b8;
    ShapeFileStruct *shape_ec = unknown_shape_ac0fec;
    ShapeFileStruct *shape_b4 = unknown_shape_ac10b4;
    ConvertClass *drawer_c = (ConvertClass *)Unknown_Get_Art_6f2830();
    ShapeFileStruct *shape_f0 = unknown_shape_ac10f0;
    int count = unknown_count_ac102c;
    Rect window = unknown_surface_839958->Get_Rect();
    Rect rect_c4 = *unknown_rect_ac11c4;
    Rect rect_b8 = *unknown_rect_ac11b8;
    Rect rect_bc = *unknown_rect_ac11bc;
    Rect rect_b4 = *unknown_rect_ac11b4;
    Rect rect_c0 = *unknown_rect_ac11c0;

    RECT client;
    Window_Rect_In_Main_Client(dialog, &client);
    RECT screen;
    GetWindowRect(dialog, &screen);
    int dx = screen.left - client.left;
    int dy = screen.top - client.top;

    int x = rect_b4.X;
    int y = rect_b4.Y;
    int width = rect_b4.Width;
    int height = rect_b4.Height * count;
    if (has_d5) {
        x = rect_bc.X;
        width = rect_bc.Width;
        y = rect_bc.Y;
        height += rect_b4.Y;
    }
    Rect from(x, y, width, height);
    Rect to = from;
    to.X += dx;
    to.Y += dy;
    Rect from_c0 = rect_c0;
    Rect to_c0 = from_c0;
    to_c0.X += dx;
    to_c0.Y += dy;

    int dir = reverse ? -1 : 1;
    int base_64 = 5;
    if (reverse) base_64 = 10;
    int base_8c = 11;
    if (reverse) base_8c = 16;
    int base_94 = 1;
    if (reverse) base_94 = 6;
    int base_88 = 0;
    if (reverse) base_88 = 5;
    int base_90 = 0;
    if (reverse) base_90 = 5;

    int buttons = Visible_Count_A737E4(dialog);
    int last = count - 1;
    int top = rect_b4.Y;
    if (has_d6) top += rect_b4.Height;
    bool any = Any_Visible_A763B4(dialog);
    if (!any) last++;

    // One start step per piece: the button rows, then the d6, d5 and d7 frame pieces.
    int pieces = count + 3;
    int limit = pieces - 2;
    int *order = new int[pieces];
    int total = count + 3;  // recounted after the allocation (the original recomputes count + 3 here)
    int value = 1;
    for (int i = 0; i < limit; i++) {
        order[i] = value++;
    }
    order[total - 1] = 0;
    order[limit] = value + 1;
    order[pieces - 3] = 0;
    int steps = Last_Step(order, pieces) + 6;

    for (int step = 0; step < steps; step++) {
        if (has_d6) {
            Point2D point = Unknown_Point_Copy_006F1AE0((Point2D const *)&rect_b8);
            int phase = Slide_Phase(step - order[pieces - 3]);
            if (phase == -1) {
                CC_Draw_Shape(unknown_surface_839958, drawer, shape_ec, reverse ? 6 : 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else if (phase == -2) {
                if (!reverse) {
                    CC_Draw_Shape(unknown_surface_839958, drawer, shape_ec, 6, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                }
            }
        }
        if (has_d5) {
            Point2D point = Unknown_Point_Copy_006F1AE0((Point2D const *)&rect_bc);
            int phase = Slide_Phase(step - order[pieces - 2]);
            if (phase == -1) {
                CC_Draw_Shape(unknown_surface_839958, drawer, shape_e8, !reverse, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else if (phase == -2) {
                CC_Draw_Shape(unknown_surface_839958, drawer, shape_e8, reverse != 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else {
                point = Unknown_Point_Copy_006F1AE0((Point2D const *)&rect_c4);
                CC_Draw_Shape(unknown_surface_839958, drawer, shape_e8, 1, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                point = Unknown_Point_Copy_006F1AE0((Point2D const *)&rect_bc);
                CC_Draw_Shape(unknown_surface_839958, drawer, shape_b4, phase * dir + base_88, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        if (has_d6) {
            Point2D point = Unknown_Point_Copy_006F1AE0((Point2D const *)&rect_b8);
            int phase = Slide_Phase(step - order[pieces - 3]);
            if (phase == -2) {
                if (reverse) {
                    CC_Draw_Shape(unknown_surface_839958, drawer, shape_ec, 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                }
            } else {
                CC_Draw_Shape(unknown_surface_839958, drawer, shape_ec, phase * dir + base_94, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        int k;
        for (k = 0; k < buttons; k++) {
            Point2D point(rect_b4.X, k * rect_b4.Height + top);
            int phase = Slide_Phase(step - order[k]);
            if (phase == -1) {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, reverse ? 10 : 1, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else if (phase == -2) {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, reverse ? 1 : 10, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, phase * dir + base_64, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        for (k = buttons; k < last; k++) {
            Point2D point(rect_b4.X, k * rect_b4.Height + top);
            int phase = Slide_Phase(step - order[k]);
            if (phase == -1) {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, reverse ? 10 : 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else if (phase == -2) {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, reverse ? 0 : 10, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, phase * dir + base_8c, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        if (any) {
            Point2D point(rect_b4.X, unknown_rect_ac11cc->Y - rect_b4.Height);
            int phase = Slide_Phase(step - order[count - 1]);
            if (phase == -1) {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, reverse ? 10 : 1, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else if (phase == -2) {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, reverse ? 1 : 10, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else {
                CC_Draw_Shape(unknown_surface_839958, drawer_b, shape_b8, phase * dir + base_64, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        if (has_d7) {
            Point2D point = Unknown_Point_Copy_006F1AE0((Point2D const *)&rect_c0);
            int phase = Slide_Phase(step - order[total - 1]);
            if (phase == -1) {
                if (!reverse) {
                    if (unknown_8523e0 >= unknown_7add6c) point.X += 0x50;
                }
                CC_Draw_Shape(unknown_surface_839958, drawer_c, (reverse ? unknown_shape_ac110c : shape_f0), 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else if (phase == -2) {
                if (reverse) {
                    if (unknown_8523e0 >= unknown_7add6c) point.X += 0x50;
                }
                CC_Draw_Shape(unknown_surface_839958, drawer_c, (reverse ? shape_f0 : unknown_shape_ac110c), 0, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else {
                if (unknown_8523e0 >= unknown_7add6c) point.X += 0x50;
                CC_Draw_Shape(unknown_surface_839958, drawer_c, shape_f0, phase * dir + base_90, point, window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        unknown_surface_839950->Lock();
        unknown_surface_839958->Lock();
        unknown_surface_839950->Copy_From(to, *unknown_surface_839958, from, false, true);
        if (has_d7) {
            unknown_surface_839950->Copy_From(to_c0, *unknown_surface_839958, from_c0, false, true);
        }
        unknown_surface_839958->Unlock();
        unknown_surface_839950->Unlock();
        Audio_Update();
        Sleep(30);
    }
    delete [] order;
    if (reverse) {
        Voc_Play(Rules->ShellButtonSlideSound, 1.0f, 0x2000, 0);
        while (WWMouse->Get_Mouse_State()) {
            WWMouse->Show_Mouse();
        }
        SendMessageA(dialog, 0x4ec, 0, 0);
    } else {
        SendMessageA(dialog, 0x4ed, 0, 0);
    }
}

}


#include "shared/campaign_score.hpp"

// When the dialog's record has its +0xBE flag set: notifies control 0x71A (0x4E2), calls 0x005E5150
// and clears the flag (0x005E5F20).
void __fastcall unknown_dialog_5e5f20(HWND window)
{
    OwnerDraw::WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)window, &record);
    if (record && record->unknown_be) {
        HWND item = GetDlgItem(window, 0x71a);
        if (item) {
            SendMessageA(item, 0x4e2, 0, 0);
        }
        OwnerDraw::unknown_5e5150(window, false);
        record->unknown_be = false;
    }
}


#include "shared/ms_message_box.hpp"
#include "shared/session_state.hpp"

namespace OwnerDraw {

bool __fastcall unknown_5e5fc0(HWND dialog)
{
    if (unknown_session_a3d298.Unknown_00672F10()) {
        return false;
    }
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)dialog, &record);
    if (!record) {
        return false;
    }
    bool active = record->unknown_bd && record->unknown_b0 == 1;
    bool visible = IsWindowVisible(dialog) != 0;
    if (!active || !visible) {
        return false;
    }
    Voc_Play(Rules->GUIMoveOutSound, 1.0f, 0x2000, 0);
    bool enabled = IsWindowEnabled(dialog) != 0;
    EnableWindow(dialog, FALSE);
    EnumChildWindows(dialog, unknown_5e4780, 1);
    record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)dialog, &record);
    if (!record) {
        return false;
    }
    record->unknown_be = true;
    InvalidateRect(dialog, 0, FALSE);
    DWORD start = GetTickCount();
    while (!unknown_6000d0()) {
        int done = !record->unknown_be;
        if ((int)(GetTickCount() - start) > 5000 || done) {
            break;
        }
    }
    EnumChildWindows(dialog, unknown_5e4780, 0);
    EnableWindow(dialog, enabled);
    return true;
}

}


namespace OwnerDraw {

bool __fastcall unknown_show_dialog_5e61b0(HWND dialog)
{
    if (unknown_session_a3d298.Unknown_00672F10()) {
        return false;
    }
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)dialog, &record);
    if (!record) {
        return false;
    }
    bool active = record->unknown_bd && record->unknown_b0 == 1;
    bool visible = IsWindowVisible(dialog) != 0;
    if (!active || !visible) {
        return false;
    }
    Voc_Play(Rules->GUIMoveInSound, 1.0f, 0x2000, 0);
    bool enabled = IsWindowEnabled(dialog) != 0;
    EnableWindow(dialog, FALSE);
    EnumChildWindows(dialog, unknown_5e4780, 1);
    unknown_5e5150(dialog, true);
    EnumChildWindows(dialog, unknown_5e4780, 0);
    EnableWindow(dialog, enabled);
    InvalidateRect(dialog, 0, FALSE);
    return true;
}

}


namespace OwnerDraw {

void __fastcall unknown_5e62d0(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        record->unknown_bd = true;
    }
}

}


namespace OwnerDraw {

void __fastcall unknown_5e6330(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        record->unknown_bd = false;
    }
}

}


namespace OwnerDraw {

void __fastcall unknown_5e6390(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        record->unknown_d4 = true;
    }
}

}


namespace OwnerDraw {

void __fastcall unknown_5e63f0(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        record->unknown_d4 = false;
    }
}

}


// RA2-specific dialog/control rectangle selection, independently bounded005E6450.
#include "shared/id_source.hpp"
namespace OwnerDraw {
bool __fastcall Select_Control_Rect_005e6450(HWND dialog,HWND control,Rect *bounds)
{
    WinData *record = Get_Record(dialog);
    int kind = record ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(control);
    Rect *selected = 0;
    if (kind == 0x94) {
        if (unknown_8523e0 < unknown_screen_widths_7add68[1]) return false;
        if (id == 0x6ea) selected = new Rect(0x1e,0x1a,0x23a,0x87);
        else if (id == 0x6eb) selected = new Rect(0x52,0xbb,0x1d4,0x6c);
        else if (id == 0x6ec) selected = new Rect(0x62,0x12a,0x1bc,0x95);
        else if (id == 0x50f) selected = new Rect(0xb1,0x1fd,0x110,0x15);
        else if (id == 0x71e) selected = new Rect(0xb1,0x1dc,0x8d,0x14);
        else if (id == 0x670) selected = new Rect(0x135,0x1dc,0x8d,0x14);
        else return false;
    }
    else if (kind == 0x103) {
        if (id != 0x72b) return false;
        if (unknown_8523e0 >= unknown_screen_widths_7add68[1]) {
            if (unknown_id_source_a3d290->unknown_2239) selected = new Rect(0x68,0x8e,0x1a8,0xdc);
            else selected = new Rect(0x67,0x82,0x1a8,0xe6);
        } else {
            if (unknown_id_source_a3d290->unknown_2239) selected = new Rect(0x18,0x8e,0x1a8,0xdc);
            else selected = new Rect(0x17,0x82,0x1a8,0xe6);
        }
    }
    else if (kind == 0xbc7) {
        if (id != 0x72b) return false;
        if (unknown_8523e0 >= unknown_screen_widths_7add68[1]) {
            if (unknown_id_source_a3d290->unknown_2239) selected = new Rect(0x68,0x8e,0x1a8,0xdc);
            else selected = new Rect(0x67,0x82,0x1a8,0xe6);
        } else {
            if (unknown_id_source_a3d290->unknown_2239) selected = new Rect(0x18,0x8e,0x1a8,0xdc);
            else selected = new Rect(0x17,0x82,0x1a8,0xe6);
        }
    }
    else if (kind == 0xbc) {
        if (id != 0x72f) return false;
        if (unknown_8523e0 == unknown_screen_widths_7add68[0]) selected = new Rect(0x34,0x4e,0x18c,0x8f);
        else if (unknown_8523e0 == unknown_screen_widths_7add68[1] || unknown_8523e0 == unknown_screen_widths_7add68[2]) selected = new Rect(0x84,0x8a,0x18c,0x8f);
        else return false;
    }
    else if (kind == 0xbd) {
        if (id != 0x72f) return false;
        if (unknown_8523e0 == unknown_screen_widths_7add68[0]) selected = new Rect(0x34,0x4e,0x18c,0x8f);
        else if (unknown_8523e0 == unknown_screen_widths_7add68[1] || unknown_8523e0 == unknown_screen_widths_7add68[2]) selected = new Rect(0x84,0x8a,0x18c,0x8f);
        else return false;
    }
    else if (kind == 0xc2) {
        if (id != 0x72f) return false;
        if (unknown_8523e0 == unknown_screen_widths_7add68[0]) selected = new Rect(0x34,0x4e,0x18c,0x8f);
        else if (unknown_8523e0 == unknown_screen_widths_7add68[1] || unknown_8523e0 == unknown_screen_widths_7add68[2]) selected = new Rect(0x84,0x8a,0x18c,0x8f);
        else return false;
    }
    else if (kind == 0xc9) {
        if (id != 0x72f) return false;
        if (unknown_8523e0 == unknown_screen_widths_7add68[0]) selected = new Rect(0x34,0x4e,0x18c,0x8f);
        else if (unknown_8523e0 == unknown_screen_widths_7add68[1] || unknown_8523e0 == unknown_screen_widths_7add68[2]) selected = new Rect(0x84,0x8a,0x18c,0x8f);
        else return false;
    }
    else return false;
    if (!selected) return false;
    *bounds = *selected;
    delete selected;
    return true;
}
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool OwnerDraw::unknown_5e7230(unsigned int key, HWND window) (0x005E7230): an owner-drawn, free control that
// passes test 0x005E6860. Adapted from ra2-decomp src/ownrdraw.cpp FUN_006096A0 (gamemd twin 0x006096A0, matched
// there): the window copy made before record is cleared (wlib Dictionary<HWND>::getPointer key); see THIRD_PARTY.md.

namespace OwnerDraw {

bool __fastcall unknown_5e7230(unsigned int key, HWND window)
{
    if ((GetWindowLongA(window, GWL_STYLE) & BS_OWNERDRAW) != BS_OWNERDRAW) {
        return false;
    }
    HWND control = window;
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int const &)control, &record);
    if (record && record->unknown_68 == 0) {
        return ::OwnerDraw_Control_Test_005e6860(key, window);
    }
    return false;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5e72c0(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (kind == 0xe2) {
        return id == 0x3ee;
    }
    if (((kind == 0x73) || (kind == 0x10c)) || (kind == 0x103)) {
        return id == 0x423;
    }
    if (kind == 0x108) {
        return id == 0x6d1;
    }
    if ((((kind != 0x94) && (kind != 0xb6)) &&
            ((kind != 0xa3 && ((kind != 0xd8 && (kind != 0xbbb)))))) &&
         ((((kind != 0xf5 &&
                ((((kind != 0xd5 && (kind != 0x2b5)) && (kind != 0xb7)) &&
                 ((kind != 0x2b4 && (kind != 0x101)))))) &&
             ((((kind != 0x129 && ((kind != 0xb5 && (kind != 0xbba)))) && (kind != 0x100)) &&
                (((((kind != 0xb8 && (kind != 0xd6)) && (kind != 0x125)) &&
                    ((kind != 0x116 && (kind != 0x11d)))) && (kind != 0x11c)))))) &&
            (((kind != 0x113 && (kind != 0x109)) &&
             ((kind != 0x10f && ((kind != 0x114 && (kind != 0x10e)))))))))) {
        if ((kind == 0x102) ||
             ((((kind == 0x105 || (kind == 0x6b)) || (kind == 0xbb)) ||
                ((kind == 0x117 || (kind == 0xbc6)))))) {
            return id == 0x5c0;
        }
        if (((((kind == 0xbc) || (kind == 0xbd)) ||
                 ((kind == 0xea || (((kind == 0xd7 || (kind == 0xc2)) || (kind == 0xc9)))))) ||
                ((kind == 700 || (kind == 0xe7)))) ||
             ((kind == 0xe6 ||
                (((kind == 0xf3 || (kind == 0xf4)) ||
                 ((kind == 0x122 || (((kind == 0x112 || (kind == 0xfe)) || (kind == 0xfc)))))))))) {
            return id == 2;
        }
        if (kind == 0xd4) {
            return id == 1;
        }
        if (kind == 0xfb) {
            return id == 7;
        }
        if (kind == 0xff) {
            return id == 0x675;
        }
        return false;
    }
    return id == 0x686;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5e7620(unsigned int key, HWND window)
{
    if ((GetWindowLongA(window, GWL_STYLE) & BS_OWNERDRAW) != BS_OWNERDRAW) {
        return false;
    }
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)window, &record);
    if (record && record->unknown_68 == 0) {
        return unknown_5e72c0(key, window);
    }
    return false;
}

}


namespace seed_005df660 {
unsigned char __fastcall unknown_kind_test_005df660(int);   // 0x005DF660
}

namespace OwnerDraw {

bool __fastcall unknown_5e76b0(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (seed_005df660::unknown_kind_test_005df660(kind) && id == 0x695) {
        return true;
    }
    return false;
}

}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool OwnerDraw::unknown_5e7740(unsigned int key, HWND window) (0x005E7740): layout band 1 test outside a session.
// Adapted from ra2-decomp src/ownrdraw.cpp FUN_00609BC0 and Is_Plain_Control (gamemd twin 0x00609BC0, matched
// there); see THIRD_PARTY.md.

namespace OwnerDraw {

// Whether window is an owner-drawn control with a free registry record (ra2-decomp Is_Plain_Control; inline).
inline bool Is_Plain_Control(HWND window)
{
    if ((GetWindowLongA(window, GWL_STYLE) & BS_OWNERDRAW) == BS_OWNERDRAW) {
        HWND control = window;
        WinData *record = 0;
        unknown_control_table_a73640.Get_Pointer((unsigned int const &)control, &record);
        if (record != 0 && record->unknown_68 == 0) {
            return true;
        }
    }
    return false;
}

bool __fastcall unknown_5e7740(unsigned int key, HWND window)
{
    if (unknown_session_a3d298.Unknown_00672F10()) {
        return false;
    }
    if (unknown_5e7230(key, window)) {
        return true;
    }
    if (Is_Plain_Control(window) && unknown_5e72c0(key, window)) {
        return true;
    }
    return false;
}

}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool OwnerDraw::unknown_5e7870(unsigned int key, HWND window) (0x005E7870): layout band 2 test inside a session.
// Adapted from ra2-decomp src/ownrdraw.cpp FUN_00609CF0 (gamemd twin 0x00609CF0, matched there); see THIRD_PARTY.md.

namespace OwnerDraw {

bool __fastcall unknown_5e7870(unsigned int key, HWND window)
{
    if (!unknown_session_a3d298.Unknown_00672F10()) {
        return false;
    }
    if (unknown_5e7230(key, window)) {
        return true;
    }
    if (Is_Plain_Control(window) && unknown_5e72c0(key, window)) {
        return true;
    }
    return false;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5e79a0(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (kind == 0x10d) {
        return id == 0x5c1 || id == 0x5c0;
    }
    if (kind == 0xd9) {
        return id == 0x5c1 || id == 0x6ca;
    }
    if (kind == 0xa3) {
        return id == 0x4cf || id == 0x4d0;
    }
    if (kind == 0x105) {
        return id == 0x620 || id == 0x621;
    }
    if (kind == 0xf0) {
        return id == 0x5c0;
    }
    if (kind == 0xce) {
        return id == 0x5ae;
    }
    if (kind == 0x120) {
        return id == 0x5ae || id == 2;
    }
    if (kind == 0x121) {
        return id == 0x5ae || id == 0x5af || id == 2;
    }
    if (kind == 0xea) {
        return id == 0x640 || id == 0x642 || id == 0x641 || id == 0x643 || id == 0x644 || id == 0x645 || id == 0x646 || id == 0x647;
    }
    if (kind == 0xb8) {
        return id == 0x531 || id == 0x535;
    }
    if (kind == 0x11f) {
        return id == 0x5c0;
    }
    if (kind == 0x115) {
        return id == 0x702 || id == 0x686;
    }
    if (kind == 0xd3) {
        return id == 2;
    }
    if (kind == 0xcf) {
        return id == 0x5c2 || id == 2;
    }
    if (kind == 0xc3) {
        return id == 1 || id == 2;
    }
    if (kind == 0x11b) {
        return id == 0x5c2 || id == 0x686;
    }
    if (kind == 0xe1) {
        return id == 1 || id == 2;
    }
    if (kind == 0x11e) {
        return id == 0x5c2 || id == 0x686;
    }
    if (kind == 0xc4) {
        return id == 0x58d || id == 0x58e;
    }
    if (kind == 0xd0) {
        return id == 1;
    }
    if (kind == 0xfc) {
        return id == 1 || id == 2;
    }
    if (kind == 0x126) {
        return id == 6 || id == 7;
    }
    return false;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5e7c80(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (kind == 0xe2) {
        return id == 0x71d;
    }
    return false;
}

}


namespace OwnerDraw {

BOOL __stdcall unknown_5e7d00(HWND window, LPARAM)
{
    HWND parent = GetParent(window);
    if (unknown_5e7230((unsigned int)parent, window) && IsWindowVisible(window)) {
        unknown_visible_count_a737e4++;
    }
    return TRUE;
}

}


namespace OwnerDraw {

BOOL __stdcall unknown_5e7dd0(HWND window, LPARAM)
{
    HWND parent = GetParent(window);
    if (unknown_5e7620((unsigned int)parent, window) && IsWindowVisible(window)) {
        unknown_visible_count_a763b4++;
    }
    return TRUE;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5e7e80(HWND dialog)
{
    unknown_visible_count_a763b4 = 0;
    EnumChildWindows(dialog, unknown_5e7dd0, 0);
    return unknown_visible_count_a763b4 >= 1;
}

}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// BOOL OwnerDraw::unknown_5e7eb0(HWND window, LPARAM) (0x005E7EB0): EnumChildWindows callback that sorts each
// registered control into layout band 1, 2 or 3 (record +0xB0). Adapted from ra2-decomp src/ownrdraw.cpp
// OwnerDraw::ClassifyLayoutBand (gamemd twin 0x0060A330, matched there); see THIRD_PARTY.md.

namespace OwnerDraw {

BOOL __stdcall unknown_5e7eb0(HWND window, LPARAM)
{
    WinData *record = Get_Record(window);
    if (record != 0) {
        HWND parent = GetParent(window);
        if (unknown_5e7740((unsigned int)parent, window)) {
            record->unknown_b0 = 1;
            return TRUE;
        }
        if (unknown_5e7870((unsigned int)parent, window)) {
            record->unknown_b0 = 2;
            return TRUE;
        }
        if (unknown_5e79a0((unsigned int)parent, window)) {
            record->unknown_b0 = 3;
        }
    }
    return TRUE;
}

}


namespace OwnerDraw {

// Whether kind is one of the listed dialog kinds (0x005E8590).
BOOL __fastcall Is_Listed_Kind_5e8590(int kind)
{
    if (((((((kind != 0x94) && (kind != 0xd8)) && (kind != 0xf5)) &&
                ((kind != 0xe2 && (kind != 0xd5)))) && (kind != 0x101)) &&
            (((((kind != 0x129 && (kind != 0xd7)) &&
                  ((kind != 0xbb && (((kind != 0x100 && (kind != 0xd6)) && (kind != 0x125))))))
                && ((kind != 0x122 && (kind != 0x112)))) && (kind != 0xe7)))) &&
          ((((kind != 0x116 && (kind != 0x11d)) &&
              ((kind != 0x11c && (((kind != 0xfe && (kind != 0x10f)) && (kind != 0x117)))))) &&
            (((kind != 0x114 && (kind != 0xe6)) &&
              ((kind != 0xf3 &&
                (((kind != 0xf4 && (kind != 700)) &&
                  ((kind != 0x10e && ((kind != 0xbc6 && (kind != 0xbc7)))))))))))))) {
        return 0;
    }
    return 1;
}

}


bool __fastcall OwnerDraw_Control_Test_005e0760(unsigned int key, HWND window);

namespace OwnerDraw {

BOOL __stdcall unknown_5e86d0(HWND window, LPARAM)
{
    HWND parent = GetParent(window);
    if (OwnerDraw_Control_Test_005e0760((unsigned int)parent, window)) {
        SendMessageA(window, 0x4ee, 0, 0);
    }
    return TRUE;
}

}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// BOOL OwnerDraw::unknown_5e8720(HWND window, LPARAM) (0x005E8720): EnumChildWindows callback that stores a
// registered control's sound slot (+0xDC, from 0x005E12A0) and text-control alpha (+0xC8, from 0x005E1CA0).
// Adapted from ra2-decomp src/ownrdraw.cpp OwnerDraw::InitCompactDialogControlsProc (gamemd twin 0x0060AAB0,
// matched there); see THIRD_PARTY.md. Run by 0x005E8B90.

namespace OwnerDraw {

BOOL __stdcall unknown_5e8720(HWND window, LPARAM)
{
    HWND parent = GetParent(window);
    WinData *record = Get_Record(window);
    if (record) {
        record->unknown_dc = unknown_5e12a0(parent, window);
        record->unknown_c8 = unknown_5e1ca0(parent, window);
    }
    return TRUE;
}

}


#include "shared/owner_draw_sidebar.hpp"
namespace OwnerDraw {

void __fastcall unknown_5e8ba0(HWND window, Rect const &rect)
{
    HWND parent = GetParent(window);
    unsigned char flag = unknown_session_a3d298.Unknown_00672F10();
    RECT parent_rect;
    GetWindowRect(parent, &parent_rect);
    Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
    int offset = 0;
    if (!flag) {
        offset = (((area.Width - unknown_7add6c) / 2) < 0 ? 0 : ((area.Width - unknown_7add6c) / 2));
    }
    Rect area2(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
    int shift = 0;
    if (!flag) {
        shift = (((area2.Height - unknown_7add78) / 2) < 0 ? 0 : ((area2.Height - unknown_7add78) / 2));
    }
    MoveWindow(window, rect.X + offset, rect.Y + shift, rect.Width, rect.Height, FALSE);
}

}


extern "C" int __cdecl abs(int);
#pragma intrinsic(abs)
namespace OwnerDraw {

void __fastcall unknown_5e8c50(HWND window, Point2D const &size)
{
    HWND parent = GetParent(window);
    unsigned char flag = unknown_session_a3d298.Unknown_00672F10();
    RECT parent_rect;
    RECT rect;
    GetWindowRect(parent, &parent_rect);
    GetWindowRect(window, &rect);
    Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
    int offset = 0;
    if (!flag) {
        offset = (area.Width - unknown_7add6c) / 2;
        offset = offset < 0 ? 0 : offset;
    }
    Rect area2(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
    int shift = 0;
    if (!flag) {
        shift = (((area2.Height - unknown_7add78) / 2) < 0 ? 0 : ((area2.Height - unknown_7add78) / 2));
        shift -= (((size.Y - unknown_7add78) / 2) < 0 ? 0 : ((size.Y - unknown_7add78) / 2));
    }
    int x;
    int y;
    int width;
    int height;
    if (flag) {
        width = unknown_shape_ac0ff8->Width;
        height = unknown_shape_ac0ff8->Height;
        Rect *strip = unknown_rect_ac11ec;
        x = parent_rect.right - offset - parent_rect.left - 0x93;
        HWND dialog = GetParent(window);
        RECT parent2;
        RECT rect2;
        GetWindowRect(dialog, &parent2);
        GetWindowRect(window, &rect2);
        int top = rect2.top - parent2.top;
        y = ((rect2.bottom - parent2.top - top) / 2 - 0xc6 + top) / 0x2c * height + strip->Y;
    } else {
        width = unknown_shape_ac10b8->Width;
        height = unknown_shape_ac10b8->Height;
        Rect *strip = unknown_rect_ac11c8;
        x = parent_rect.right - offset - parent_rect.left - 0x9c;
        int top = strip->Y;
        int pos = rect.top - top - parent_rect.top + shift;
        int row = pos / strip->Height;
        int prod = strip->Height * row;
        int up = abs(strip->Height - pos + prod);
        int down = abs(pos - prod);
        y = (down < up ? row : row + 1) * strip->Height + top;
    }
    MoveWindow(window, x, y, width, height, FALSE);
}

}


namespace OwnerDraw {

void __fastcall unknown_5e8e20(HWND window, Point2D const &size)
{
    HWND parent = GetParent(window);
    unsigned char flag = unknown_session_a3d298.Unknown_00672F10();
    RECT parent_rect;
    RECT rect;
    GetWindowRect(parent, &parent_rect);
    GetWindowRect(window, &rect);
    Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
    int offset = 0;
    if (!flag) {
        int excess = (area.Width - unknown_7add6c) / 2;
        offset = excess < 0 ? 0 : excess;
    }
    Rect area2(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
    int shift = 0;
    if (!flag) {
        shift = (((area2.Height - unknown_7add78) / 2) < 0 ? 0 : ((area2.Height - unknown_7add78) / 2));
        shift -= (((size.Y - unknown_7add78) / 2) < 0 ? 0 : ((size.Y - unknown_7add78) / 2));
    }
    Rect box(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    MoveWindow(window, parent_rect.right - Sidebar_Margin(window, box.Width) - box.Width - offset - parent_rect.left, rect.top - parent_rect.top + shift, box.Width, box.Height, FALSE);
}

}


namespace OwnerDraw {

void __fastcall unknown_5e8fa0(HWND window)
{
    HWND parent = GetParent(window);
    unsigned char flag = unknown_session_a3d298.Unknown_00672F10();
    RECT parent_rect;
    RECT rect;
    GetWindowRect(parent, &parent_rect);
    GetWindowRect(window, &rect);
    Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
    int offset = Sidebar_Inset(area.Width, flag);
    int x;
    int y;
    int width;
    int height;
    if (flag) {
        x = parent_rect.right - offset - parent_rect.left - 0x93;
        width = unknown_shape_ac0ff8->Width;
        height = unknown_shape_ac0ff8->Height;
        y = unknown_rect_ac11f0->Y - height;
    } else {
        width = unknown_shape_ac10b8->Width;
        x = parent_rect.right - offset - parent_rect.left - 0x9c;
        height = unknown_shape_ac10b8->Height;
        int top = unknown_rect_ac11c8->Y;
        int cell = unknown_rect_ac11c8->Height;
        y = ((unknown_rect_ac11cc->Y - top) / cell - 1) * cell + top;
    }
    MoveWindow(window, x, y, width, height, FALSE);
}

}


namespace OwnerDraw {

void __fastcall unknown_5e9070(HWND window)
{
    HWND parent = GetParent(window);
    unsigned char flag = unknown_session_a3d298.Unknown_00672F10();
    RECT parent_rect;
    RECT rect;
    GetClientRect(parent, &parent_rect);
    GetWindowRect(window, &rect);
    Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
    int offset = 0;
    if (!flag) {
        int excess = (area.Width - unknown_7add6c) / 2;
        offset = excess < 0 ? 0 : excess;
    }
    Rect box(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    box.X = parent_rect.right - Sidebar_Margin(window, box.Width) - box.Width - offset - parent_rect.left;
    int bottom;
    if (flag) bottom = unknown_rect_ac11f4->Y;
    else bottom = unknown_rect_ac11cc->Y;
    box.Y = bottom - box.Height;
    MoveWindow(window, box.X, box.Y, box.Width, box.Height, FALSE);
}

}


namespace OwnerDraw {

void __fastcall unknown_5e91a0(HWND window)
{
    HWND parent = GetParent(window);
    unsigned char flag = unknown_session_a3d298.Unknown_00672F10();
    RECT parent_rect;
    RECT rect;
    GetClientRect(parent, &parent_rect);
    GetWindowRect(window, &rect);
    int left = parent_rect.left;
    Rect area(left, parent_rect.top, parent_rect.right - left, parent_rect.bottom - parent_rect.top);
    int offset = 0;
    if (!flag) {
        offset = (area.Width - unknown_7add6c) / 2;
        offset = offset < 0 ? 0 : offset;
    }
    Rect area2(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
    int shift = 0;
    if (!flag) {
        shift = (area2.Height - unknown_7add78) / 2;
        shift = shift < 0 ? 0 : shift;
    }
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    MoveWindow(window, offset + 10 + left, parent_rect.bottom - shift - height - 1, width, height, FALSE);
}

}


namespace OwnerDraw {

void __fastcall unknown_5e9260(HWND window)
{
    HWND parent = GetParent(window);
    unsigned char flag = unknown_session_a3d298.Unknown_00672F10();
    RECT parent_rect;
    RECT rect;
    GetClientRect(parent, &parent_rect);
    GetWindowRect(window, &rect);
    Rect area(parent_rect.left, parent_rect.top, parent_rect.right - parent_rect.left, parent_rect.bottom - parent_rect.top);
    int offset = 0;
    if (!flag) {
        int excess = (area.Width - unknown_7add6c) / 2;
        offset = excess < 0 ? 0 : excess;
    }
    Rect box(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    box.X = parent_rect.right - Sidebar_Margin(window, box.Width) - box.Width - offset - parent_rect.left;
    Rect *strip = unknown_rect_ac11f4;
    if (!flag) strip = unknown_rect_ac11cc;
    int bottom = strip->Height + strip->Y;
    box.Y = bottom - box.Height;
    MoveWindow(window, box.X, box.Y, box.Width, box.Height, FALSE);
}

}


namespace OwnerDraw {

void __fastcall unknown_5e9390(HWND window, Point2D const &size)
{
    HWND parent = GetParent(window);
    RECT parent_rect;
    RECT rect;
    GetWindowRect(parent, &parent_rect);
    GetWindowRect(window, &rect);
    MoveWindow(window, (unknown_8523e0 - size.X) / 2 - parent_rect.left + rect.left,
               (unknown_8523e4 - size.Y) / 2 - parent_rect.top + rect.top, rect.right - rect.left,
               rect.bottom - rect.top, FALSE);
}

}


namespace OwnerDraw {
bool __fastcall Select_Control_Rect_005e6450(HWND dialog, HWND control, Rect *bounds);
void __fastcall unknown_5e8ba0(HWND window, Rect const &rect);
void __fastcall unknown_5e8c50(HWND window, Point2D const &size);
void __fastcall unknown_5e8e20(HWND window, Point2D const &size);
void __fastcall unknown_5e8fa0(HWND window);
void __fastcall unknown_5e9070(HWND window);
void __fastcall unknown_5e91a0(HWND window);
void __fastcall unknown_5e9260(HWND window);
void __fastcall unknown_5e9410(HWND window);

inline bool Is_Lobby_Control(unsigned int key, HWND window)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    int kind = record != 0 ? record->unknown_6c : 0;
    int id = GetDlgCtrlID(window);
    if (kind == 0xe2) {
        return id == 0x71d;
    }
    return false;
}
BOOL __stdcall unknown_5e9bb0(HWND window, LPARAM lparam)
{
    HWND parent = GetParent(window);
    if (unknown_dialog_a763cc == parent) {
        Rect rect(0, 0, 0, 0);
        Point2D const &size = *(Point2D const *)lparam;
        if (Select_Control_Rect_005e6450(parent, window, &rect)) {
            unknown_5e8ba0(window, rect);
        } else if (unknown_5e7230((unsigned int)parent, window)) {
            unknown_5e8c50(window, size);
        } else if (::OwnerDraw_Control_Test_005e6860((unsigned int)parent, window)) {
            unknown_5e8e20(window, size);
        } else if (unknown_5e7620((unsigned int)parent, window)) {
            unknown_5e8fa0(window);
        } else if (unknown_5e72c0((unsigned int)parent, window)) {
            unknown_5e9070(window);
        } else if (unknown_5e76b0((unsigned int)parent, window)) {
            unknown_5e91a0(window);
        } else if (Is_Lobby_Control((unsigned int)parent, window)) {
            unknown_5e9260(window);
        } else {
            unknown_5e9390(window, size);
        }
        unknown_5e9410(window);
    }
    return TRUE;
}

}


#include "shared/main_opt.hpp"

// Sizes a dialog to the movie frame and moves its controls from old_size by 0x005E9BB0 (0x005E9F00).
void __fastcall Unknown_005E9F00(HWND dialog, Point2D *old_size)
{
    int width = unknown_8523e0;
    int height = unknown_8523e4;
    MoveWindow(dialog, 0, 0, width, height, 0);
    OwnerDraw::unknown_dialog_a763cc = dialog;
    LPARAM param = (LPARAM)old_size;
    EnumChildWindows(dialog, OwnerDraw::unknown_5e9bb0, param);
}


namespace OwnerDraw {

int __fastcall unknown_5e9f40(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    return record ? record->unknown_74 : 0;
}

}


namespace OwnerDraw {
// Enables the owner-draw flags for the original ordered dialog-kind set, 005E9FA0.
bool __fastcall unknown_5e9fa0(HWND dialog)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)dialog, &record);
    if (!record) return false;
    int kind = record->unknown_6c;
    bool special = kind == 0x10c ||
        kind == 0x94 ||
        kind == 0xb6 ||
        kind == 0xa3 ||
        kind == 0x73 ||
        kind == 0xd8 ||
        kind == 0xbbb ||
        kind == 0xf5 ||
        kind == 0x105 ||
        kind == 0xd5 ||
        kind == 0xe2 ||
        kind == 0x2b5 ||
        kind == 0xb7 ||
        kind == 0x2b4 ||
        kind == 0x101 ||
        kind == 0x129 ||
        kind == 0xbc6 ||
        kind == 0xbc7 ||
        kind == 0x10b ||
        kind == 0xd7 ||
        kind == 0x108 ||
        kind == 0xbc ||
        kind == 0xbd ||
        kind == 0xbb ||
        kind == 0xb5 ||
        kind == 0xbba ||
        kind == 0xff ||
        kind == 0xea ||
        kind == 0x6b ||
        kind == 0x100 ||
        kind == 0x102 ||
        kind == 0xb8 ||
        kind == 0xd6 ||
        kind == 0x103 ||
        kind == 0xd4 ||
        kind == 0x125 ||
        kind == 0x122 ||
        kind == 0x112 ||
        kind == 0xe7 ||
        kind == 0x116 ||
        kind == 0xc2 ||
        kind == 0xc9 ||
        kind == 0x113 ||
        kind == 0xfe ||
        kind == 0x10f ||
        kind == 0x11d ||
        kind == 0x11c ||
        kind == 0x117 ||
        kind == 0x114 ||
        kind == 0xe6 ||
        kind == 0xf3 ||
        kind == 0xf4 ||
        kind == 0x2bc ||
        kind == 0x10e ||
        kind == 0xfb;
    if (special) {
        record->unknown_b0 = 1;
        record->unknown_bd = true;
    }
    return special;
}
}


namespace OwnerDraw {

// A control's registry record, or null. The setters 0x005EA390..0x005EAAA0 read their record through it;
// 0x005FF7E0 inlines them with this helper's Get left out of line, which needs the key as its own local.
inline WinData *unknown_control_record(HWND control)
{
    unsigned int key = (unsigned int)control;
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    return record;
}

bool __fastcall unknown_5ea230(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(*(unsigned int *)&control, &record);
    if (!record) {
        return false;
    }
    int kind = record->unknown_6c;
    bool special = kind == 0x10d ||
        kind == 0xd9 ||
        kind == 0xf0 ||
        kind == 0xce ||
        kind == 0x120 ||
        kind == 0x121 ||
        kind == 0x115 ||
        kind == 0xd3 ||
        kind == 0xcf ||
        kind == 0x11f ||
        kind == 0xc3 ||
        kind == 0x11b ||
        kind == 0xe1 ||
        kind == 0x11e ||
        kind == 0xc4 ||
        kind == 300 ||
        kind == 0xd0 ||
        kind == 0xfc ||
        kind == 0x126;
    if (special) {
        record->unknown_b0 = 2;
    }
    return special;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5ea330(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        return record->unknown_d6;
    }
    return false;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5ea390(HWND control)
{
    WinData *record = unknown_control_record(control);
    if (!record) {
        return false;
    }
    return record->unknown_d6 = record->unknown_6c == 0xbc || record->unknown_6c == 0xbd || record->unknown_6c == 0x102 || record->unknown_6c == 0xc2 || record->unknown_6c == 0xc9;
}

}


namespace OwnerDraw {

void __fastcall unknown_5ea430(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        record->unknown_d6 = true;
    }
}

}


namespace OwnerDraw {

void __fastcall unknown_5ea490(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        record->unknown_d6 = false;
    }
}

}


namespace OwnerDraw {

bool __fastcall unknown_5ea4f0(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        return record->unknown_d5;
    }
    return false;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5ea550(HWND control)
{
    WinData *record = unknown_control_record(control);
    if (!record) {
        return false;
    }
    return record->unknown_d5 = record->unknown_6c == 0xbc || record->unknown_6c == 0xbd || record->unknown_6c == 0x102 || record->unknown_6c == 0xc2 || record->unknown_6c == 0xc9 || record->unknown_6c == 0x105 || record->unknown_6c == 0x6b || record->unknown_6c == 0x113;
}

}


namespace OwnerDraw {

void __fastcall unknown_5ea600(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        record->unknown_d5 = true;
    }
}

}


namespace OwnerDraw {

void __fastcall unknown_5ea660(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        record->unknown_d5 = false;
    }
}

}


namespace OwnerDraw {

bool __fastcall unknown_5ea6c0(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    return record ? record->unknown_d7 : false;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5ea720(HWND control)
{
    WinData *record = unknown_control_record(control);
    if (!record) {
        return false;
    }
    return record->unknown_d7 = record->unknown_6c == 0x103 || record->unknown_6c == 0xbc7;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5ea7b0(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        return record->unknown_d8;
    }
    return false;
}

}


namespace OwnerDraw {

bool __fastcall unknown_5ea810(HWND control)
{
    WinData *record = unknown_control_record(control);
    if (!record) {
        return false;
    }
    return record->unknown_d8 = record->unknown_6c == 0x108 || record->unknown_6c == 0xbc6;
}

}


namespace OwnerDraw {

int __fastcall unknown_5ea8a0(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    return record ? record->unknown_e0 : 0;
}

}


namespace OwnerDraw {

int __fastcall unknown_5ea900(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    return record ? record->unknown_e4 : 0;
}

}


struct ShapeFileStruct;
extern void *unknown_art_ac1074;
extern ShapeFileStruct *unknown_shape_ac1104;
extern ShapeFileStruct *unknown_shape_ac110c;
extern ShapeFileStruct *unknown_shape_ac1128;
extern ShapeFileStruct *unknown_shape_ac1010;
void *Unknown_Get_Art_6f2b40();

namespace OwnerDraw {

// Sets the art of the control registered under key by its kind (unknown_6c): the background image
// (unknown_74) and the two shapes unknown_e0 / unknown_e4.
void __fastcall unknown_set_control_art_5ea960(unsigned int key)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    if (record != 0) {
        int kind = record->unknown_6c;
        if (kind == 0x94) {
            record->unknown_74 = (int)Unknown_Get_Art_6f2de0();
            record->unknown_e0 = (int)unknown_art_ac1074;
            record->unknown_e4 = (int)unknown_art_ac1074;
        } else if (kind == 0x103) {
            record->unknown_74 = (int)Unknown_Get_Art_6f2830();
            record->unknown_e0 = (int)unknown_shape_ac110c;
            record->unknown_e4 = (int)unknown_shape_ac110c;
        } else if (kind == 0x108) {
            record->unknown_74 = (int)Unknown_Get_Art_6f2b40();
            record->unknown_e0 = (int)unknown_shape_ac1104;
            record->unknown_e4 = (int)unknown_shape_ac1104;
        } else if (kind == 0xBC6) {
            record->unknown_74 = (int)Unknown_Get_Art_6f2830();
            record->unknown_e0 = (int)unknown_shape_ac110c;
            record->unknown_e4 = (int)unknown_shape_ac110c;
        } else if (kind == 0xBC7) {
            record->unknown_74 = (int)Unknown_Get_Art_6f2830();
            record->unknown_e0 = (int)unknown_shape_ac110c;
            record->unknown_e4 = (int)unknown_shape_ac110c;
        } else {
            record->unknown_74 = (int)Unknown_Get_Art_006f3480();
            record->unknown_e0 = (int)unknown_shape_ac1128;
            record->unknown_e4 = (int)unknown_shape_ac1010;
        }
    }
}

}


namespace OwnerDraw {

void __fastcall unknown_5eaaa0(HWND control, int value)
{
    WinData *record = unknown_control_record(control);
    if (record) {
        record->unknown_6c = value;
    }
}

}


namespace OwnerDraw {

int __fastcall unknown_5eab00(HWND control)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)control, &record);
    if (record) {
        return record->unknown_6c;
    }
    return 0;
}

}


int __fastcall unknown_dialog_box_5eab60(int resource, int (__stdcall *proc)(struct HWND__ *, unsigned int, unsigned int, long), bool show_5115f0)
{
    HWND dialog = unknown_create_dialog_5ff610(resource, proc, 0);
    int result = 0;
    if (dialog) {
        SetWindowLongA(dialog, DWL_USER, (LONG)&result);
        unknown_dialog_5ff7c0(dialog);
        if (show_5115f0) {
            unknown_5115f0(dialog);
        }
        while (result == 0) {
            if (unknown_6000d0()) {
                break;
            }
        }
        unknown_dialog_5ff6e0(dialog);
    }
    return result;
}


namespace OwnerDraw {

// Registers the "ComboDropWin" and "NewEdit" window classes once.
void __fastcall unknown_5eac30()
{
    if (unknown_classes_registered_a763f0 == 1) {
        return;
    }
    unknown_classes_registered_a763f0 = 1;
    WNDCLASSA window_class;
    memset(&window_class, 0, sizeof(window_class));
    window_class.hInstance = ProgramInstance;
    window_class.style = 3;
    window_class.lpfnWndProc = unknown_combo_drop_proc_5ead20;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hIcon = 0;
    window_class.hCursor = 0;
    window_class.hbrBackground = 0;
    window_class.lpszMenuName = "ComboDropWin";
    window_class.lpszClassName = "ComboDropWin";
    RegisterClassA(&window_class);
    memset(&window_class, 0, sizeof(window_class));
    window_class.style = 0x83;
    window_class.lpfnWndProc = unknown_new_edit_proc_5ead00;
    window_class.cbClsExtra = 0;
    window_class.cbWndExtra = 0;
    window_class.hInstance = ProgramInstance;
    window_class.hIcon = 0;
    window_class.hCursor = 0;
    window_class.hbrBackground = 0;
    window_class.lpszMenuName = 0;
    window_class.lpszClassName = "NewEdit";
    RegisterClassA(&window_class);
}

}


#include "shared/hicolor.hpp"
#include "shared/text_print.hpp"
#include "shared/debug_printf.hpp"
#include "shared/crt_memory.hpp"
#include "shared/mono_window.hpp"

extern "C" wchar_t *__cdecl wcscat(wchar_t *destination, wchar_t const *source);
extern "C" int __cdecl wcscmp(wchar_t const *a, wchar_t const *b);
extern "C" wchar_t *__cdecl wcsncpy(wchar_t *destination, wchar_t const *source, unsigned int count);
extern "C" unsigned int __cdecl wcslen(wchar_t const *string);
extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
extern void *ProgramInstance;

extern "C" __declspec(dllimport) void *__stdcall CreateSolidBrush(unsigned long color);
WIN32_IMPORT BOOL WINAPI BringWindowToTop(HWND window);

namespace OwnerDraw {

BOOL __stdcall unknown_restore_child_5edd30(HWND window, LPARAM lparam);
BOOL __stdcall unknown_5ed180(HWND window, LPARAM lparam);
int __fastcall unknown_draw_text_5fe020(Surface *surface, wchar_t const *text, RECT *area, BitFontClass *font,
    unsigned long color, int flags, int unknown7, int unknown8, int arg9, int arg10);

// The combo box whose list is dropped, its dialog and the drop window (TibSun OwnerComboHandle, _dropdown_owner,
// _dropdown_window).
HWND unknown_combo_owner_a763f4;
HWND unknown_dropdown_owner_a763e8;
HWND unknown_dropdown_window_a763e4;

struct UnknownScrollInfo {
    unsigned int cbSize;
    unsigned int fMask;
    int nMin;
    int nMax;
    unsigned int nPage;
    int nPos;
    int nTrackPos;
};

struct UnknownListItem {
    int unknown_00;
    int unknown_04;
    wchar_t *text;
};

// The wide text of a list item through the control's original procedure.
inline int Get_Item_Text(HWND owner, int const &index, wchar_t *text, int size)
{
    WNDPROC proc = 0;
    unknown_window_proc_table_a73688.Get_Value(owner, proc);
    text[0] = 0;
    int item = CallWindowProcA(proc, owner, 0x199, index, 0);
    if (item == -1 || item == 0) {
        item = CallWindowProcA(proc, owner, 0x150, index, 0);
    }
    if (item != -1 && item != 0) {
        wcsncpy(text, ((UnknownListItem *)item)->text, size);
        text[size - 1] = 0;
        return wcslen(text);
    }
    return 0;
}

// Not exact (RA2_NON_MATCHING): OwnerDraw::unknown_5e9410 at 0x005E9410, best score 99.79, 2026-11-13; differs: same instructions; registers, stack slots or operands differ
#ifdef RA2_NON_MATCHING
namespace seed_005e0da0 {
unsigned int __fastcall Is_Listed_Kind_005e0da0(int);
}


void __fastcall unknown_5e9410(HWND window)
{
    HWND parent = GetParent(window);
    RECT parent_rect;
    RECT rect;
    GetWindowRect(parent, &parent_rect);
    GetWindowRect(window, &rect);
    int x = rect.left - parent_rect.left;
    int y = rect.top - parent_rect.top;
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    int kind = Kind_Of((unsigned int)parent);
    int id = GetDlgCtrlID(window);
    int old_x = x;
    int old_y = y;
    int old_width = width;
    int old_height = height;
    if ((kind == 0x10c || kind == 0x94 || kind == 0xa3 || kind == 0x73 || kind == 0xd8 || kind == 0xbbb || kind == 0xf5 || kind == 0x105 || kind == 0xe2 || kind == 0xd5 || kind == 0x2b5 || kind == 0xb7 || kind == 0x2b4 || kind == 0xbc7 || kind == 0x101 || kind == 0x129 || kind == 0xbc6 || kind == 0xd7 || kind == 0x108 || kind == 0xbc || kind == 0xbd || kind == 0xbb || kind == 0xb5 || kind == 0xbba || kind == 0xff || kind == 0x6b || kind == 0x100 || kind == 0x102 || kind == 0xb8 || kind == 0xd6 || kind == 0x103 || Is_Listed_Kind_5e0da0(kind)) && id == 0x694) {
        bool c12 = kind == 0x10c || kind == 0x94 || kind == 0xa3 || kind == 0x73 || kind == 0xd8 || kind == 0xbbb || kind == 0xf5 || kind == 0x105 || kind == 0xe2 || kind == 0xd5 || kind == 0x2b5 || kind == 0xb7 || kind == 0x2b4 || kind == 0xbc7 || kind == 0x101 || kind == 0x129 || kind == 0xbc6 || kind == 0xd7 || kind == 0x108 || kind == 0xbc || kind == 0xbd || kind == 0xbb || kind == 0xb5 || kind == 0xbba || kind == 0xff || kind == 0x6b || kind == 0x100 || kind == 0x102 || kind == 0xb8 || kind == 0xd6 || kind == 0x103;
        if (c12) {
            bool c4 = Is_Option_Kind(kind) || kind == 0x105 || kind == 0x6b || kind == 0x113;
            if (c4) {
                if (!unknown_session_a3d298.Unknown_00672F10()) {
                    y += 1;
                }
            } else {
                if (!unknown_session_a3d298.Unknown_00672F10()) {
                    y += 7;
                    height += 1;
                }
            }
        } else if (kind == 0x125 || kind == 0xe7 || kind == 0x116 || kind == 0xc2 || kind == 0xc9 || kind == 0x11d || kind == 0x11c || kind == 0x114 || kind == 700 || kind == 0x113 || kind == 0x109 || kind == 0x10f || kind == 0x117 || kind == 0xe6 || kind == 0xf3 || kind == 0xf4 || kind == 0x10e) {
            if (kind != 0xc2 && kind != 0xc9 && kind != 0x113) {
                y += 2;
            }
        }
    } else if (kind == 0x102) {
        if (id == 0x50c) {
            y -= 1;
        } else if (id == 0x54e || id == 0x693 || id == 0x696 || id == 0x69a) {
            x -= 1;
        }
    } else if (kind == 0xbb) {
        if (id == 0x540) {
            width -= 1;
        }
    } else if (kind == 0xbc || kind == 0xbd) {
        if (id == 0x53f) {
            width += 1;
        } else if (id == 0x54f) {
            y -= 1;
        } else if (id == 0x54e || id == 1000 || id == 0x54d || id == 0x69a) {
            x -= 1;
        }
    } else if (kind == 0x103) {
        if (id == 0x6f3 || id == 0x6f4 || id == 0x69f || id == 0x6f6 || id == 0x6ef || id == 0x6f1 || id == 0x6f0 || id == 0x6f2) {
            if (unknown_8523e0 >= unknown_7add6c) {
                y -= (unknown_7add78 - unknown_screen_heights_7add74[0]) / 2;
            }
        }
    } else if (kind == 0xbc7) {
        if (id == 0x733 || id == 0x735 || id == 0x732 || id == 0x3ea || id == 0x73c || id == 0x411 || id == 0x412 || id == 0x73d || id == 0x419 || id == 0x41a || id == 0x73e || id == 0x6a3 || id == 0x6a4 || id == 0x73f || id == 0x6a5 || id == 0x6a7 || id == 0x740 || id == 0x6a6 || id == 0x6a8 || id == 0x736 || id == 0x73a || id == 0x73b || id == 0x737 || id == 0x739 || id == 0x738) {
            if (unknown_8523e0 >= unknown_7add6c) {
                y -= (unknown_7add78 - unknown_screen_heights_7add74[0]) / 2;
            }
        }
    }
    if (x != old_x || y != old_y || width != old_width || old_height != height) {
        MoveWindow(window, x, y, width, height, FALSE);
    }
}
#endif

// Not exact (RA2_NON_MATCHING): OwnerDraw::unknown_5e8130 at 0x005E8130, best score 17.45, 2026-11-13; differs: 324 instructions in the original, 272 in the draft (shape 43.96)
#ifdef RA2_NON_MATCHING
}   // namespace OwnerDraw: the draft declares its global helpers first
bool __fastcall OwnerDraw_Control_Test_005e0760(unsigned int key, HWND window);
int __fastcall OwnerDraw_Control_Height_005defb0(unsigned int key, HWND window);
int __fastcall OwnerDraw_Control_Kind_005df8e0(unsigned int key, HWND window);
int __fastcall OwnerDraw_Control_Flags_005e0010(unsigned int key, HWND window);

namespace OwnerDraw {

bool __fastcall unknown_5e0e40(unsigned int key, HWND window);
bool __fastcall unknown_5e0ff0(unsigned int key, HWND window);
int __fastcall unknown_5e1a20(unsigned int key, HWND window);
bool __fastcall unknown_5e1050(unsigned int key, HWND window);
int __fastcall unknown_5e1490(unsigned int key, HWND window);
void *__fastcall unknown_5e1820(unsigned int key, HWND window);
int __fastcall unknown_5e06c0(unsigned int key, HWND window);
void const *__fastcall unknown_5e1a80(unsigned int key, HWND window, bool &allocated);

// The record's animation fields +0x78..+0xA8 (the shared layout still holds them as padding;
// Scout1219's layout: shape from 0x005E1A80, its allocated flag, timer state).
struct UnknownControlRecordAnimation {
    void const *unknown_78;     // +0x78 shape from 0x005E1A80
    bool unknown_7c;            // +0x7C that shape was allocated
    int unknown_80;
    int unknown_84;
    int unknown_88;
    int unknown_8c;
    int unknown_90;             // +0x90 (WinData::unknown_90)
    int unknown_94;
    int unknown_98;
    int unknown_9c;
    int unknown_a0;
    int unknown_a4;
    bool unknown_a8;
};
static inline UnknownControlRecordAnimation *Animation(WinData *record)
{
    return (UnknownControlRecordAnimation *)record->unknown_78;
}

// The record of key, or null (0x005E8130, 0x005FEE70).
static inline void Lookup_8130(unsigned int const &key, WinData *&record)
{
    unknown_control_table_a73640.Get_Pointer(key, &record);
}
BOOL __stdcall unknown_5e8130(HWND window, LPARAM)
{
    HWND parent = GetParent(window);
    unsigned int key = (unsigned int)window;
    WinData *record = 0;
    Lookup_8130(key, record);
    if (record == 0) {
        return TRUE;
    }
    if (OwnerDraw_Control_Test_005e0760((unsigned int)parent, window)) {
        record->unknown_70 = 1;
        Animation(record)->unknown_a8 = false;
        Animation(record)->unknown_80 = 1;
        Animation(record)->unknown_84 = OwnerDraw_Control_Height_005defb0((unsigned int)parent, window);
        Animation(record)->unknown_88 = OwnerDraw_Control_Kind_005df8e0((unsigned int)parent, window);
        Animation(record)->unknown_8c = OwnerDraw_Control_Flags_005e0010((unsigned int)parent, window);
        record->unknown_90 = unknown_5e06c0((unsigned int)parent, window);
        return TRUE;
    }
    bool animated = false;
    if (unknown_5e0e40((unsigned int)parent, window)) {
        record->unknown_70 = 2;
        if (record->image_14 == 0) {
            record->image_14 = (Surface *)unknown_5e1a20((unsigned int)parent, window);
        }
    } else {
        if (unknown_5e0ff0((unsigned int)parent, window)) {
            record->unknown_70 = 3;
        } else if (unknown_5e1050((unsigned int)parent, window)) {
            record->unknown_70 = 4;
            animated = true;
            if (Animation(record)->unknown_78 == 0) {
                bool allocated = false;
                void *art = unknown_5e1820((unsigned int)parent, window);
                Animation(record)->unknown_78 = unknown_5e1a80((unsigned int)parent, window, allocated);
                record->unknown_74 = (int)art;
                Animation(record)->unknown_7c = allocated;
            }
        } else {
            goto done;
        }
    }
    Animation(record)->unknown_98 = 0;
    Animation(record)->unknown_9c = GetTickCount();
    Animation(record)->unknown_a0 = unknown_5e1490((unsigned int)parent, window);
done:
    if (Animation(record)->unknown_a0 < 1) {
        if (animated && Animation(record)->unknown_78) {
            Animation(record)->unknown_94 = ((short const *)Animation(record)->unknown_78)[3];
            Animation(record)->unknown_a8 = false;
        }
    } else {
        SetTimer(window, 0, Animation(record)->unknown_a0, 0);
        if (animated) {
            Animation(record)->unknown_a8 = true;
        }
    }
    return TRUE;
}

}
namespace OwnerDraw {
#endif

// The drop-down list window procedure of a combo box (0x005EAD20; TibSun ComboDropWinCtrlProc).
LRESULT __stdcall unknown_combo_drop_proc_5ead20(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    static void *solid_brush = CreateSolidBrush(0x306030);
    RECT rect;
    Window_Rect_In_Main_Client(window, &rect);
    RECT client;
    GetClientRect(window, &client);
    // Function scope: the frame shows it live across every switch case (nothing shares its slot).
    Rect fill;
    RECT parent_rect;
    memset(&parent_rect, 0, sizeof(parent_rect));
    int scrollbar_width = 2 * unknown_color_a73928 + 18;
    int need_scrollbar = -1;
    int max_top = 0;
    HWND parent = 0;
    if (unknown_combo_owner_a763f4) {
        parent = GetParent(unknown_combo_owner_a763f4);
    }
    WinData *parent_data = 0;
    if (parent) {
        unknown_control_table_a73640.Get_Pointer((unsigned int const &)parent, &parent_data);
        Window_Rect_In_Main_Client(parent, &parent_rect);
    }
    unknown_dropdown_owner_a763e8 = parent;
    unknown_dropdown_window_a763e4 = window;
    WinData *data = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int const &)window, &data);
    if (data == 0) {
        Debug_Printf("ComboBox dropdown windata = NULL\n");
    }
    WinData *master = 0;
    if (unknown_combo_owner_a763f4) {
        unknown_control_table_a73640.Get_Pointer((unsigned int const &)unknown_combo_owner_a763f4, &master);
    }
    if (message != 0x146 && message != 0x154 && message != 0x115) {
        int item_count = SendMessageA(unknown_combo_owner_a763f4, 0x146, 0, 0);
        int item_height = SendMessageA(unknown_combo_owner_a763f4, 0x154, 0, 0);
        if (item_height <= 1) {
            item_height = 1;
        }
        need_scrollbar = item_count * item_height > client.bottom - client.top;
        max_top = item_count - (client.bottom - client.top) / item_height;
        if (data) {
            if ((unsigned int)data->attached_0c > 1) {
                UnknownScrollInfo info;
                info.fMask = 5;
                info.nMin = 0;
                info.nMax = max_top;
                info.nPos = data->unknown_f0;
                info.cbSize = 0x1c;
                SendMessageA(data->attached_0c, 0xe9, 0, (LPARAM)&info);
            }
            if (data->attached_0c) {
                BringWindowToTop(data->attached_0c);
            }
        }
    }
    switch (message) {
    case 0x14:
        return 0;
    case 1: {
        SetCapture(window);
        unknown_combo_owner_a763f4 = *(HWND *)lparam;
        char buffer[100];
        sprintf(buffer, "Dropd=%p  Master=%p  Dlg=%p", window, unknown_combo_owner_a763f4, GetParent(unknown_combo_owner_a763f4));
        Debug_Printf(buffer);
        SetTimer(window, 0, 20, 0);
        return 0;
    }
    case 0x82:
        unknown_dropdown_window_a763e4 = 0;
        unknown_dropdown_owner_a763e8 = 0;
        ReleaseCapture();
        KillTimer(window, 0);
        break;
    case 0x115: {
        int top = SendMessageA(data->attached_0c, 0xe1, 0, 0);
        if (top != SendMessageA(window, 0x15b, 0, 0)) {
            SendMessageA(window, 0x15c, top, 0);
        }
        break;
    }
    case 0x113: {
        HWND capture = GetCapture();
        if (capture == window) {
            ReleaseCapture();
            Sleep(0);
            SetCapture(capture);
        }
        break;
    }
    case 0x15b:
        if (data) {
            return data->unknown_f0;
        }
        break;
    case 0x200: {
        int x = lparam & 0xffff;
        int y = (unsigned int)lparam >> 16;
        int index = SendMessageA(window, 0x4e8, 0, (y << 16) | x);
        if (index != -1) {
            if (data->unknown_e8 != index) {
                InvalidateRect(window, 0, 0);
            }
            data->unknown_e8 = index;
            HWND tip = GetDlgItem(parent, 0x695);
            if (tip) {
                WideString text;
                text = unknown_5e2100(parent, (int)unknown_combo_owner_a763f4, index);
                if (wcscmp((wchar_t const *)text.Get(), L"")) {
                    SendMessageA(tip, 0x4b2, 0, (LPARAM)text.unknown_text_770430());
                } else {
                    text = unknown_5e2160(parent, (int)unknown_combo_owner_a763f4);
                    if (wcscmp((wchar_t const *)text.Get(), L"")) {
                        SendMessageA(tip, 0x4b2, 0, (LPARAM)text.unknown_text_770430());
                    }
                }
            }
        }
        return 0;
    }
    case 0x15c: {
        int top = wparam;
        int item_count = SendMessageA(unknown_combo_owner_a763f4, 0x146, 0, 0);
        int item_height = SendMessageA(unknown_combo_owner_a763f4, 0x154, 0, 0);
        if (!item_count || !item_height) {
            break;
        }
        int visible = (client.bottom - client.top) / item_height;
        if (top < 0) {
            top = 0;
        }
        int limit = item_count - visible;
        if (limit > 0) {
            if (top > limit) {
                top = limit;
            }
        } else {
            top = 0;
        }
        if (top != data->unknown_f0) {
            data->unknown_f0 = top;
            InvalidateRect(window, 0, 0);
        }
        return 0;
    }
    case 0x201:
    case 0x203: {
        int x = lparam & 0xffff;
        int y = (unsigned int)lparam >> 16;
        if (x > client.right && x < client.right + data->scroll_width_04 && y > 0 && y < client.bottom) {
            SendMessageA(data->attached_0c, message, wparam, lparam);
            break;
        }
        Voc_Play(Rules->GUIComboCloseSound, 1.0f, 0x2000, 0);
        if (x >= 0 && y >= 0 && x <= client.right && y <= client.bottom) {
            int item_height = SendMessageA(unknown_combo_owner_a763f4, 0x154, 0, 0);
            int item_count = SendMessageA(unknown_combo_owner_a763f4, 0x146, 0, 0);
            int index = y / item_height;
            if (data) {
                index += data->unknown_f0;
            }
            int clamped = index < 0 ? 0 : index;
            int max_index = item_count - 1;
            if (max_index < clamped) {
                clamped = max_index;
            }
            SendMessageA(unknown_combo_owner_a763f4, 0x14e, clamped, 0);
            int id = GetWindowLongA(unknown_combo_owner_a763f4, -12);
            if (need_scrollbar) {
                DestroyWindow(data->attached_0c);
            }
            ReleaseCapture();
            SendMessageA(unknown_combo_owner_a763f4, 0x14f, 0, 0);
            PostMessageA(parent, 0x111, (id & 0xffff) | 0x10000, (LPARAM)unknown_combo_owner_a763f4);
            return 0;
        }
        if (need_scrollbar) {
            DestroyWindow(data->attached_0c);
        }
        ReleaseCapture();
        SendMessageA(unknown_combo_owner_a763f4, 0x14f, 0, 0);
        return 0;
    }
    case 0x4e8: {
        int x = lparam & 0xffff;
        int y = (unsigned int)lparam >> 16;
        int result = -1;
        if (x >= 0 && y >= 0 && x < client.right && y < client.bottom) {
            int item_height = SendMessageA(unknown_combo_owner_a763f4, 0x154, 0, 0);
            int item_count = SendMessageA(unknown_combo_owner_a763f4, 0x146, 0, 0);
            int index = y / item_height + data->unknown_f0;
            int clamped = index < 0 ? 0 : index;
            int max_index = item_count - 1;
            if (max_index < clamped) {
                clamped = max_index;
            }
            result = clamped;
        }
        return result;
    }
    case 0x4ea:
        return (LRESULT)unknown_combo_owner_a763f4;
    case 0x7e8:
        data->unknown_e8 = SendMessageA(unknown_combo_owner_a763f4, 0x147, 0, 0);
        return 0;
    // Last in the source: case 0x14 jumps into the epilogue of the last `return 0` written.
    case 0xf: {
        BitFontClass *font = parent_data->font_64;
        int item_count = SendMessageA(unknown_combo_owner_a763f4, 0x146, 0, 0);
        int item_height = SendMessageA(unknown_combo_owner_a763f4, 0x154, 0, 0);
        int client_height = client.bottom - client.top;
        Rect dest_rect(rect.left, rect.top, rect.right - rect.left, client_height);
        int selected = data->unknown_e8;
        Rect src;
        src.X = rect.left - parent_rect.left;
        src.Y = rect.top - parent_rect.top;
        src.Width = client.right;
        src.Height = client.bottom;
        if (data->surface_10 == 0) {
            Rect dst(0, 0, client.right, client.bottom);
            BSurface *surface = new BSurface(client.right, client.bottom, 2);
            data->surface_10 = surface;
            unknown_surface_count_a763d8++;
            if (parent_data && parent_data->surface_10) {
                surface->Copy_From(dst, *parent_data->surface_10, src, false, true);
            }
            int total = client.right * client.bottom;
            unsigned short *pixels = (unsigned short *)surface->Lock();
            for (int i = 0; i < total; i++) {
                unsigned short pixel = pixels[i];
                pixels[i] = Blend_Color(pixel, 0, data->unknown_c8);
            }
            surface->Unlock();
        }
        {
            Rect from;
            from.X = 0;
            from.Y = 0;
            from.Width = dest_rect.Width;
            from.Height = dest_rect.Height;
            unknown_surface_839958->Copy_From(dest_rect, *data->surface_10, from, false, true);
        }
        Rect frame(dest_rect);
        int color = unknown_5fda40(unknown_color_a76158);
        if (master && master->alternate_cd) {
            color = unknown_5fda40(unknown_color_a73910);
        }
        unknown_5fdb20(unknown_surface_839958, frame, 1, color);
        if (need_scrollbar && data && data->attached_0c) {
            Rect outer(dest_rect);
            outer.Width += scrollbar_width;
            unknown_5fdb20(unknown_surface_839958, outer, 1, color);
        }
        int first = data->unknown_f0;
        Rect row;
        for (int index = first; index < item_count; index++) {
            row.X = rect.left;
            int width = rect.right - rect.left;
            row.Y = (index - first) * item_height + rect.top;
            if ((index - first) * item_height + item_height > client_height) {
                break;
            }
            wchar_t text[128];
            Get_Item_Text(unknown_combo_owner_a763f4, index, text, 0x80);
            if (index == selected) {
                fill.X = row.X;
                fill.Y = row.Y;
                fill.Width = width;
                fill.Height = item_height;
                int fill_color;
                if (master ? master->alternate_cd : false) {
                    fill_color = unknown_5fda40(unknown_color_a763b0);
                } else {
                    fill_color = unknown_5fda40(unknown_color_a7613c);
                }
                unknown_surface_839958->Fill_Rect(fill, fill_color);
            }
            RECT text_rect;
            text_rect.top = row.Y;
            text_rect.left = row.X + 3;
            text_rect.right = row.X + width;
            text_rect.bottom = row.Y + item_height;
            unsigned long text_color = unknown_color_a733e4;
            if (master && master->alternate_cd) {
                text_color = unknown_color_a737e8;
            }
            if (index < 50 && master && master->item_colors_110[index] >= 0) {
                text_color = master->item_colors_110[index];
                if (master->item_fills_cc) {
                    Rect fill2;
                    fill2.X = row.X;
                    fill2.Y = row.Y;
                    fill2.Width = width;
                    fill2.Height = item_height;
                    Unknown_Rect_Shrink_006F1B00(&fill2, 2);
                    unknown_surface_839958->Fill_Rect(fill2, unknown_5fda40(text_color));
                }
            }
            int max_width = client.right - 20;
            int length = wcslen(text);
            int text_width = font->String_Pixel_Width(text, 0);
            while (length && text_width > max_width) {
                length--;
                text[length] = 0;
                wcscat(text, L"...");
                text_width = font->String_Pixel_Width(text, 0);
            }
            unknown_draw_text_5fe020(unknown_surface_839958, text, &text_rect, font, text_color, 4, 12, 0, 0, 0);
        }
        unknown_surface_839958->Lock();
        unknown_surface_839950->Lock();
        Rect screen(dest_rect);
        RECT window_rect;
        GetWindowRect(window, &window_rect);
        screen.X += window_rect.left - rect.left;
        screen.Y += window_rect.top - rect.top;
        unknown_surface_839950->Copy_From(screen, *unknown_surface_839958, dest_rect, false, true);
        unknown_surface_839950->Unlock();
        unknown_surface_839958->Unlock();
        ValidateRect(window, 0);
        return 0;
    }
    case 2:
        Unknown_Remove_Modeless_Dialog_00522b80(window);
        break;
    }
    if (need_scrollbar == 1) {
        if (data && !data->attached_0c) {
            data->attached_0c = (HWND)1;
            GetWindowLongA(window, -12);
            parent = GetParent(window);
            RECT parent_display;
            Window_Rect_In_Main_Client(parent, &parent_display);
            RECT drop_display;
            Window_Rect_In_Main_Client(window, &drop_display);
            int left = drop_display.left - parent_display.left;
            int top = drop_display.top - parent_display.top;
            bool alternate = false;
            int x = left - scrollbar_width + client.right;
            int y = top + client.top;
            int height = drop_display.bottom - drop_display.top;
            if (master) {
                alternate = master->alternate_cd;
            }
            HWND scroll = CreateWindowExA(0, "Scrollbar", 0, 0x50010001, x, y, scrollbar_width, height, parent, 0,
                ProgramInstance, 0);
            data->attached_0c = scroll;
            data->scroll_width_04 = scrollbar_width;
            unknown_5ed180(scroll, 0);
            WinData *scroll_data = 0;
            unknown_control_table_a73640.Get_Pointer((unsigned int const &)data->attached_0c, &scroll_data);
            scroll_data->owner_08 = window;
            scroll_data->alternate_cd = data->alternate_cd;
            UnknownScrollInfo info;
            info.nMax = max_top;
            info.fMask = 5;
            info.nMin = 0;
            info.nPos = data->unknown_f0;
            info.cbSize = 0x1c;
            SendMessageA(data->attached_0c, 0xe9, 0, (LPARAM)&info);
            SendMessageA(data->attached_0c, 0x4f1, 0, alternate);
            SetWindowPos(window, 0, 0, 0, drop_display.right - drop_display.left - scrollbar_width, drop_display.bottom - drop_display.top, 2);
            ShowWindow(data->attached_0c, 5);
            BringWindowToTop(data->attached_0c);
            InvalidateRect(data->attached_0c, 0, 0);
            UpdateWindow(data->attached_0c);
            SendMessageA(data->attached_0c, 0x4a9, (WPARAM)data->attached_0c, 1);
            SendMessageA(data->attached_0c, 0x4d2, 0, 1);
            RECT validate;
            validate.left = left;
            validate.top = top;
            validate.right = left + client.right + 1;
            validate.bottom = top + client.bottom + 1;
            ValidateRect(parent, &validate);
        }
    } else if (need_scrollbar == 0 && data && data->attached_0c && !data->unknown_20) {
        DestroyWindow(data->attached_0c);
        unknown_restore_child_5edd30(data->attached_0c, 0);
        data->attached_0c = 0;
        data->scroll_width_04 = 0;
        SetWindowPos(window, 0, 0, 0, client.right - client.left + scrollbar_width, client.bottom - client.top, 2);
        parent = GetParent(window);
        RECT validate_parent;
        Window_Rect_In_Main_Client(parent, &validate_parent);
        RECT validate;
        validate.left = rect.left - validate_parent.left;
        validate.top = rect.top - validate_parent.top;
        validate.right = validate.left + client.right + scrollbar_width;
        validate.bottom = validate.top + client.bottom + 1;
        ValidateRect(parent, &validate);
    }
    return DefWindowProcA(window, message, wparam, lparam);
}

}


namespace OwnerDraw {

// EnumChildWindows callback: clears each control's user data and replaces a visible, enabled edit control with
// an owner list box of the same place and size (user data: "NewEdit" and the edit's style), swapping their IDs.
// (GWL_USERDATA -21, GWL_ID -12, WS_VISIBLE 0x10000000, WS_DISABLED 0x08000000; the list box style is
// WS_CHILD | LBS_HASSTRINGS | LBS_NOTIFY, plus WS_TABSTOP from the edit.)
BOOL __stdcall unknown_replace_edit_5ecb00(HWND window, LPARAM)
{
    unsigned int style = GetWindowLongA(window, GWL_STYLE);
    SetWindowLongA(window, -21, 0);
    if ((style & 0x10000000) && !(style & 0x08000000)) {
        char name[128];
        GetClassNameA(window, name, sizeof(name));
        if (strcmp(name, "Edit") == 0) {
            HWND parent = GetParent(window);
            RECT rect;
            RECT parent_rect;
            Window_Rect_In_Main_Client(window, &rect);
            Window_Rect_In_Main_Client(parent, &parent_rect);
            int x = rect.left - parent_rect.left;
            int y = rect.top - parent_rect.top;
            DWORD list_style = 0x40000110;
            if (style & 0x10000) {
                list_style = 0x40010110;
            }
            HWND list = CreateWindowExA(0, "ListBox", 0, list_style, x, y,
                                        rect.right - rect.left, rect.bottom - rect.top, parent, 0, ProgramInstance, 0);
            LONG *data = (LONG *)operator new(8);
            data[0] = (LONG)"NewEdit";
            data[1] = GetWindowLongA(window, GWL_STYLE);
            SetWindowLongA(list, -21, (LONG)data);
            EnableWindow(window, FALSE);
            ShowWindow(window, SW_HIDE);
            EnableWindow(list, TRUE);
            ShowWindow(list, SW_SHOWNORMAL);
            LONG id = GetWindowLongA(window, -12);
            SetWindowLongA(window, -12, GetWindowLongA(list, -12));
            SetWindowLongA(list, -12, id);
        }
    }
    return TRUE;
}

}


#include "shared/wide_string.hpp"

namespace OwnerDraw {

BOOL __stdcall unknown_replace_edit_5ecb00(HWND window, LPARAM lparam);
BOOL __stdcall unknown_5ed180(HWND window, LPARAM lparam);
BOOL __stdcall Set_Paint_Disabled_5ecf40(HWND window, LPARAM lparam);

// Subclasses a dialog and its controls (TibSun OwnerDraw::Subclass_Dialog), with painting off meanwhile.
bool __fastcall Subclass_Dialog_5ecc90(HWND window, LPARAM lparam)
{
    unknown_5eac30();
    unknown_dialog_a763cc = window;
    EnumChildWindows(window, Set_Paint_Disabled_5ecf40, 1);
    Set_Paint_Disabled_5ecf40(window, 1);
    EnumChildWindows(window, unknown_replace_edit_5ecb00, 1);
    EnumChildWindows(window, unknown_5ed180, lparam);
    unknown_5ed180(window, lparam);
    EnumChildWindows(window, Set_Paint_Disabled_5ecf40, 0);
    Set_Paint_Disabled_5ecf40(window, 0);
    return true;
}

}


namespace OwnerDraw {

// Sets one window's paint suppression flag, giving it a record when it has none (TibSun SetUserData2).
BOOL __stdcall Set_Paint_Disabled_5ecf40(HWND window, LPARAM lparam)
{
    WinData *data = 0;
    if (!unknown_control_table_a73640.Get_Pointer((unsigned int const &)window, &data)) {
        WinData temp;
        unknown_control_table_a73640.Add((unsigned int const &)window, temp);
        unknown_control_table_a73640.Get_Pointer((unsigned int const &)window, &data);
    }
    data->unknown_20 = lparam;
    return TRUE;
}

}


#include "shared/owner_draw_images.hpp"

namespace OwnerDraw {

extern int unknown_masks_ready_a763f8;
LRESULT __stdcall unknown_scroll_proc_5f9fb0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall unknown_list_proc_5f6690(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall unknown_combo_proc_5f4ba0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall unknown_track_proc_5fb270(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall unknown_new_edit_proc_5f22a0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall unknown_static_proc_5f2b50(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall unknown_tab_proc_5f0f40(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall unknown_button_proc_5f0320(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall unknown_control_proc_5ee460(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall unknown_default_proc_5f0210(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall unknown_button_proc_5f42d0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall unknown_checkbox_proc_5f3cf0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall unknown_edit_control_proc_5f1900(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
LRESULT __stdcall unknown_progress_proc_5faff0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
void unknown_5de880();
void unknown_5fc820();

// Prepares one control for owner drawing (TibSun InitializeCtrl): its class picks the control kind and paint
// procedure, 0x005EE460 replaces the window procedure, and the record takes lparam and the window text.
BOOL __stdcall unknown_5ed180(HWND window, LPARAM lparam)
{
    char class_name[128];
    GetClassNameA(window, class_name, sizeof(class_name));
    LONG style = GetWindowLongA(window, GWL_STYLE);
    RECT rect1;
    Window_Rect_In_Main_Client(window, &rect1);
    RECT rect2;
    GetClientRect(window, &rect2);
    Unknown_Add_Modeless_Dialog_00522b20(window);
    unknown_5de880();
    if (!unknown_masks_ready_a763f8) {
        unknown_5fc820();
        Load_Dialog_Images_5fc8a0();
        unknown_masks_ready_a763f8 = 1;
    }
    WNDPROC custom_proc = 0;
    int kind = 0xb;
    LONG *user_data = (LONG *)GetWindowLongA(window, -21);
    if (user_data) {
        strcpy(class_name, (char const *)user_data[0]);
    }
    if (strcmp(class_name, "ScrollBar") == 0) {
        custom_proc = unknown_scroll_proc_5f9fb0;
        kind = 8;
    } else if (strcmp(class_name, "ListBox") == 0) {
        custom_proc = unknown_list_proc_5f6690;
        kind = 4;
    } else if (strcmp(class_name, "ComboBox") == 0) {
        custom_proc = unknown_combo_proc_5f4ba0;
        kind = 3;
    } else if (strcmp(class_name, "msctls_trackbar32") == 0) {
        custom_proc = unknown_track_proc_5fb270;
        kind = 7;
    } else if (strcmp(class_name, "msctls_progress32") == 0) {
        custom_proc = unknown_progress_proc_5faff0;
        kind = 6;
    } else if (strcmp(class_name, "NewEdit") == 0) {
        custom_proc = unknown_new_edit_proc_5f22a0;
        kind = 1;
    } else if (strcmp(class_name, "Edit") == 0) {
        custom_proc = unknown_edit_control_proc_5f1900;
        kind = 1;
    } else if (strcmp(class_name, "Static") == 0) {
        custom_proc = unknown_static_proc_5f2b50;
        kind = 2;
    } else if (strcmp(class_name, "SysTabControl32") == 0) {
        custom_proc = unknown_tab_proc_5f0f40;
        kind = 0xa;
    } else if (strcmp(class_name, "Button") == 0) {
        if ((style & (unsigned char)7) == (unsigned char)7) {
            custom_proc = unknown_group_box_proc_5fc020;
        } else if ((style & (unsigned char)0xb) == (unsigned char)0xb) {
            custom_proc = unknown_button_proc_5f0320;
        } else if ((style & (unsigned char)3) == (unsigned char)3) {
            custom_proc = unknown_checkbox_proc_5f3cf0;
        } else if ((style & (unsigned char)9) == (unsigned char)9) {
            custom_proc = unknown_button_proc_5f42d0;
        }
        kind = 0;
    } else if (strcmp(class_name, "msctls_hotkey32") == 0) {
        custom_proc = unknown_hotkey_proc_5fc330;
        kind = 9;
    } else {
        custom_proc = unknown_default_proc_5f0210;
    }
    WNDPROC original_proc = (WNDPROC)SetWindowLongA(window, -4, (LONG)unknown_control_proc_5ee460);
    if (!unknown_window_table_a73400.Contains(window)) {
        unknown_window_table_a73400.Add(window, custom_proc);
    }
    if (!unknown_window_proc_table_a73688.Contains(window)) {
        unknown_window_proc_table_a73688.Add(window, original_proc);
    }
    WinData *data = 0;
    if (!unknown_control_table_a73640.Get_Pointer((unsigned int const &)window, &data)) {
        WinData temp;
        unknown_control_table_a73640.Add((unsigned int const &)window, temp);
        unknown_control_table_a73640.Get_Pointer((unsigned int const &)window, &data);
    }
    data->unknown_00 = lparam;
    data->unknown_68 = kind;
    if (user_data) {
        data->unknown_54 = user_data[1];
    }
    WNDPROC proc = 0;
    unknown_window_proc_table_a73688.Get_Value(window, proc);
    char text[2048];
    text[0] = 0;
    CallWindowProcA(proc, window, WM_GETTEXT, sizeof(text), (LPARAM)text);
    if (text[0]) {
        data->Set_Wide_Text_600510(StringTable::Load_String(text, 0, "D:\\Ra2\\ownrdraw.cpp", 0x1a47));
    } else {
        data->Set_Wide_Text_600510(0);
    }
    data->unknown_2c = 0;
    SendMessageA(window, 0x497, 0, 0);
    return TRUE;
}

}


// BOOL __fastcall unknown_restore_window_5edb20(HWND window) (0x005EDB20)
// Undoes the owner-draw subclassing of a window and its children (EnumChildWindows with 0x005EDD30): drops its
// window procedure, registry record and 0x00A73400 entry, frees its GWL_USERDATA block and removes it from the
// modeless dialogs (0x00522B80). No callers in the image. Name hypothesis.

void __fastcall Unknown_Remove_Modeless_Dialog_00522b80(HWND__ *dialog);
// unknown_window_table_a73400: defined at 0x005DEB90 in this unit (VC6 refuses a second extern declaration).

namespace OwnerDraw {

// ~WinData: inline, defined beside the control registry's definition (0x005DED60's source).

// The GWL_USERDATA block of a subclassed window (layout unknown). Hypothesis: a class with an inline empty
// destructor; that `delete` is a later /Ob2 inline site, so the 0xA73400 remove calls Shrink_Table 0x602730.
struct UnknownWindowData_5edb20 {
    ~UnknownWindowData_5edb20() {}
};

BOOL __stdcall unknown_restore_child_5edd30(HWND window, LPARAM lparam);

BOOL __fastcall unknown_restore_window_5edb20(HWND window)
{
    EnumChildWindows(window, unknown_restore_child_5edd30, 0);
    unknown_restore_child_5edd30(window, 0);
    return TRUE;
}

}


namespace OwnerDraw {

// EnumChildWindows callback of 0x005EDB20 (TibSun ODRemoveFromDict): drops one window from the registries.
BOOL __stdcall unknown_restore_child_5edd30(HWND window, LPARAM lparam)
{
    unknown_window_proc_table_a73688.Remove(window);
    unknown_control_table_a73640.Remove((unsigned int const &)window);
    unknown_window_table_a73400.Remove(window);
    void *data = (void *)GetWindowLongA(window, -21);
    if (data != 0) {
        operator delete(data);
    }
    SetWindowLongA(window, -21, 0);
    Unknown_Remove_Modeless_Dialog_00522b80(window);
    return 1;
}

}


namespace OwnerDraw {

UnknownTooltipState::UnknownTooltipState()
{
    rect_00.X = 0;
    rect_00.Y = 0;
    rect_00.Width = 0;
    rect_00.Height = 0;
    surface_10 = 0;
    text_14[0] = 0;
    shown_114 = 0;
    drawn_118 = 0;
    unknown_11c = 0;
}

}


// Starts an owner-drawn tooltip after closing the previous one.
extern "C" long __cdecl time(long *);
extern "C" int __cdecl swprintf(unsigned short *, unsigned short const *, ...);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *, unsigned short const *);
extern "C" char const unknown_ownrdraw_file_7e8df8[];
namespace OwnerDraw {
extern long unknown_tooltip_time_a763fc;
bool __fastcall unknown_show_tooltip_5ee110(bool save_background);
bool __fastcall unknown_start_tooltip_5edfd0(Rect const &area, unsigned short const *text, HWND window)
{
    unknown_5ee3b0();
    unknown_tooltip_time_a763fc = time(0);
    swprintf(unknown_tooltip_a737f0.text_14, StringTable::Load_String("GUI:ToolTip", 0, unknown_ownrdraw_file_7e8df8, 0x1a88));
    if (text) wcscpy(unknown_tooltip_a737f0.text_14, text);
    unknown_tooltip_a737f0.rect_00 = area;
    unknown_tooltip_a737f0.shown_114 = 1;
    unknown_tooltip_a737f0.unknown_11c = (int)window;
    return unknown_show_tooltip_5ee110(true);
}
}


// bool OwnerDraw::unknown_show_tooltip_5ee110(bool): restores/draws the active tooltip.
// Native-only complete bound500 code bytes +12 padding.
extern "C" unsigned int __cdecl wcslen(unsigned short const *);
namespace OwnerDraw {
void __fastcall unknown_5fe9b0(Rect const &area, Surface &surface, int a, int b);
bool __fastcall unknown_show_tooltip_5ee110(bool save_background)
{
    if (!unknown_tooltip_a737f0.shown_114) return false;
    if (save_background) {
        if (unknown_tooltip_a737f0.surface_10) delete unknown_tooltip_a737f0.surface_10;
        unknown_tooltip_a737f0.surface_10 = 0;
        Surface *background = new BSurface(unknown_tooltip_a737f0.rect_00.Width, unknown_tooltip_a737f0.rect_00.Height, 2);
        unknown_tooltip_a737f0.surface_10 = background;
        Rect destination(0, 0, unknown_tooltip_a737f0.rect_00.Width, unknown_tooltip_a737f0.rect_00.Height);
        background->Copy_From(destination, *unknown_surface_839950, unknown_tooltip_a737f0.rect_00, false, true);
    }
    unknown_tooltip_a737f0.drawn_118 = 0;
    Rect area = unknown_tooltip_a737f0.rect_00;
    int color = unknown_5fda40(unknown_color_a763d4);
    unknown_surface_839950->Lock();
    unknown_surface_839950->Fill_Rect(area, color);
    unknown_surface_839950->Unlock();
    unknown_5fe9b0(area, *unknown_surface_839950, 8, 4);
    unknown_text_draw_005fe1b0(unknown_color_a733e4, 0, area, unknown_tooltip_a737f0.text_14,
        wcslen(unknown_tooltip_a737f0.text_14), 1, 1, unknown_surface_839950, 0);
    ++area.X;
    ++area.Y;
    area.Width -= 2;
    area.Height -= 2;
    unknown_5fdb20(unknown_surface_839950, area, 1, -1);
    return true;
}
}


namespace OwnerDraw {

// Blits the tooltip's saved surface to the screen once.
bool unknown_5ee310()
{
    if (!unknown_tooltip_a737f0.shown_114) {
        return false;
    }
    if (unknown_tooltip_a737f0.drawn_118 == 1) {
        return false;
    }
    if (!unknown_tooltip_a737f0.surface_10) {
        return false;
    }
    Rect area = unknown_tooltip_a737f0.rect_00;
    unknown_surface_839950->Copy_From(area, *unknown_tooltip_a737f0.surface_10, Rect(0, 0, area.Width, area.Height),
                                      false, true);
    unknown_tooltip_a737f0.drawn_118 = 1;
    return true;
}

}


namespace OwnerDraw {

// Restores the screen under the tooltip and releases its surface.
bool unknown_5ee3b0()
{
    if (!unknown_tooltip_a737f0.shown_114) {
        return false;
    }
    if (unknown_tooltip_a737f0.drawn_118 != 1 && unknown_tooltip_a737f0.surface_10) {
            Rect area = unknown_tooltip_a737f0.rect_00;
            unknown_surface_839950->Copy_From(area, *unknown_tooltip_a737f0.surface_10, Rect(0, 0, area.Width, area.Height),
                                              false, true);
            unknown_tooltip_a737f0.drawn_118 = 1;
    }
    if (unknown_tooltip_a737f0.surface_10) {
        delete unknown_tooltip_a737f0.surface_10;
    }
    unknown_tooltip_a737f0.surface_10 = 0;
    unknown_tooltip_a737f0.shown_114 = 0;
    unknown_tooltip_a737f0.drawn_118 = 0;
    return true;
}

}


// The real atexit function registered at005EE685/005EE6A5 for the modal HWND list.
// OriginalCount/Capacity/Items are atA73918/A7391C/A73920.
#include "shared/wol_array_list.hpp"
typedef WolArrayList<HWND> ModalWindowList_5f00c0;
extern "C" ModalWindowList_5f00c0 unknown_modal_windows_a73918;
void unknown_modal_list_atexit_5f00c0()
{
    unknown_modal_windows_a73918.~ModalWindowList_5f00c0();
}


// Real original atexit function for the active-message pair-keyed dictionary.
// Its existing accepted Remove_Entry00602040 proves key8/value1/next12 layout.
typedef WolDictionary<OwnerDraw::UnknownPairKey,bool> ActiveMessageDictionary_5f00e0;
extern "C" ActiveMessageDictionary_5f00e0 unknown_active_messages_a73398;
void unknown_active_messages_atexit_5f00e0()
{
    unknown_active_messages_a73398.~ActiveMessageDictionary_5f00e0();
}


// Default owner-draw control procedure, original 005f0210.
#include "shared/win32_gdi.hpp"
namespace OwnerDraw {
LRESULT __stdcall unknown_default_proc_5f0210(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    WNDPROC proc = 0;
    unknown_window_proc_table_a73688.Get_Value(window, proc);
    RECT area;
    Window_Rect_In_Main_Client(window, &area);
    RECT client;
    GetClientRect(window, &client);
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int const &)window, &record);
    if (message == 0x133) {
        SetTextColor((HDC)wparam, unknown_color_a733e4);
        SetBkMode((HDC)wparam, 1);
        return (LRESULT)GetStockObject(5);
    }
    return CallWindowProcA(proc, window, message, wparam, lparam);
}
}


// Edit control callback005F1900, native2075code+5padding. Ghidra's stack-probe prototype is false.
extern "C" char const unknown_combo_class_7e8dd4[];
extern "C" char const unknown_crlf_7d7cec[];
extern "C" unsigned short unknown_edit_buffer_a7393c[0x1400];
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *,unsigned short const *,unsigned int);
extern "C" unsigned int __cdecl wcslen(unsigned short const *);
extern "C" __declspec(dllimport) HWND __stdcall GetFocus();
extern "C" __declspec(dllimport) HWND __stdcall GetNextDlgTabItem(HWND,HWND,BOOL);
extern "C" __declspec(dllimport) BOOL __stdcall GetUpdateRect(HWND,RECT *,BOOL);
void __fastcall WideToCharString_006fa0f0(char *,unsigned short const *,int);
namespace OwnerDraw {
void __fastcall unknown_draw_dimmed_back_5fe490(Rect const &,HWND,int);
void __fastcall unknown_draw_edit_text_600830(Surface *,Rect &,wchar_t const *,int,
    BitFontClass *,unsigned int,int *,int,int,int,int);
LRESULT __stdcall unknown_edit_control_proc_5f1900(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    WNDPROC previous=0;
    unknown_window_proc_table_a73688.Get_Value(window, previous);
    WinData *data = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)window, &data);
    char class_name[32];
    char narrow[0x800];
    unsigned short wide[0x800];
    if(!data) return CallWindowProcA(previous,window,message,wparam,lparam);
    if(GetFocus()==window && !data->unknown_f4) {
        data->unknown_f0=1;
        SetFocus(MainWindow);
    }
    LONG style=GetWindowLongA(window,-16);
    if((message==0x101 || message==0x100) && wparam==9)return 0;
    if(message==0x497) {
        RECT winrect,client,parentrect;
        GetWindowRect(window,&winrect);
        GetClientRect(window,&client);
        GetWindowRect(GetParent(window),&parentrect);
        MoveWindow(window,winrect.left-parentrect.left+1,winrect.top-parentrect.top+1,
            client.right-2,client.bottom-2,0);
        if(GetFocus()==window) {
            data->unknown_f0=1;
            SetFocus(MainWindow);
        }
        if(style&0x10000) {
            data->unknown_f8=1;
            SetWindowLongA(window,-16,style&~0x10000);
        }
        return 0;
    }
    if(message==7) {
        SendMessageA(window,0xb1,(WPARAM)-1,-1);
        if(!data->unknown_f4)PostMessageA(window,0x4b0,0,0);
        goto invalidate;
    }
    if(message==0x4b5 || message==0x4b3 || message==0xe) {
        wide[0]=0;
        unknown_edit_text_5f2120(window,wide,0x800,0);
        if(message==0xe)return wcslen(wide);
        if(message==0x4b3) {
            wcsncpy((unsigned short *)lparam,wide,wparam);
            ((unsigned short *)lparam)[wparam-1]=0;
            return wcslen((unsigned short *)lparam);
        }
        WideToCharString_006fa0f0((char *)lparam,wide,wparam);
        ((char *)lparam)[wparam-1]=0;
        return strlen((char *)lparam);
    }
    if(message==0x4b4 || message==0x4b2) {
        narrow[0]=0;
        if(data->text_28)WideCharToMultiByte(0,0,data->text_28,-1,narrow,0x800,0,0);
        return CallWindowProcA(previous,window,0xc,0,(LPARAM)narrow);
    }
    if(message==0x102) {
        if(wparam==0xd) {
            if(style&4) {
                unsigned int length=CallWindowProcA(previous,window,0xe,0,0)+3;
                char *text=new char[length];
                CallWindowProcA(previous,window,0xd,length,(LPARAM)text);
                strcat(text,unknown_crlf_7d7cec);
                CallWindowProcA(previous,window,0xc,0,(LPARAM)text);
                delete [] text;
                unsigned int id=GetWindowLongA(window,-12)&0xffff;
                SendMessageA(GetParent(window),0x111,id|0x5010000,(LPARAM)window);
                return 0;
            }
        } else if(wparam==9) {
            HWND next=window;
            HWND parent=GetParent(next);
            HWND tab=GetNextDlgTabItem(parent,next,0);
            if(tab)next=tab;
            SetFocus(next);
            return 0;
        }
        goto call_previous;
    }
    if(message==0x4af) {
        int pending=data->unknown_f0;
        data->unknown_f4=1;
        if(pending) {
            SetFocus(window);
            data->unknown_f0=0;
        }
        if(data->unknown_f8)SetWindowLongA(window,-16,style|0x10000);
        return 0;
    }
    if(message==0xf || message==0x14) {
        RECT display,update;
        Window_Rect_In_Main_Client(window,&display);
        GetClassNameA(GetParent(window),class_name,16);
        bool combo=strcmp(class_name,unknown_combo_class_7e8dd4)==0;
        if(message==0xf && GetUpdateRect(window,&update,0)) {
            update.left+=display.left;
            update.right+=display.left;
            update.top+=display.top;
            update.bottom+=display.top;
        }
        Rect background(display.left,display.top,display.right-display.left+1,display.bottom-display.top+1);
        unknown_draw_dimmed_back_5fe490(background,window,data->unknown_c8);
        if(!combo)unknown_5fdb20(unknown_surface_839958,background,1,-1);
        int caret=0;
        unknown_edit_text_5f2120(window,unknown_edit_buffer_a7393c,0x1400,&caret);
        Rect text(display.left,display.top,display.right-display.left+1,display.bottom-display.top+1);
        LONG text_style=GetWindowLongA(window,-16);
        unknown_draw_edit_text_600830(unknown_surface_839958,text,unknown_edit_buffer_a7393c,caret,
            data->font_64,unknown_color_a733e4,&data->unknown_44,data->unknown_38,((unsigned int)text_style>>5)&1,0,0);
        ValidateRect(window,0);
    }
    if(message==0x7b)return 1;
    if(message==0x200)return 1;
    if(message==0x100 || message==0x101 || message==0x104 || message==0x105 || message==0x106 || message==0x107 || message==8 || message==0x201) {
invalidate:
        GetClassNameA(GetParent(window),class_name,32);
        if(strcmp(class_name,unknown_combo_class_7e8dd4)==0)InvalidateRect(GetParent(window),0,0);
        InvalidateRect(window,0,0);
    }
call_previous:
    return CallWindowProcA(previous,window,message,wparam,lparam);
}
}


#include "shared/crt_tokens.hpp"

namespace OwnerDraw {

// The text of an edit control as wide text without line breaks ("\r\n" appended if it had any) and the
// caret's character index; returns the length (0x005F2120).
int __fastcall unknown_edit_text_5f2120(HWND window, wchar_t *text, int size, int *caret)
{
    WNDPROC proc = 0;
    unknown_window_proc_table_a73688.Get_Value(window, proc);
    char buffer[1024];
    int start = 0;
    buffer[0] = 0;
    CallWindowProcA(proc, window, WM_GETTEXT, sizeof(buffer), (LPARAM)buffer);
    if (caret) {
        DWORD selection_start;
        DWORD selection_end;
        start = (unsigned long)CallWindowProcA(proc, window, EM_GETSEL, (WPARAM)&selection_start, (LPARAM)&selection_end) >> 16;
    }
    *text = 0;
    MultiByteToWideChar(0, 0, buffer, -1, text, size);
    if (caret) {
        *caret = _mbsnbcnt((unsigned char *)buffer, start);
    }
    if (size > 0) {
        text[size - 1] = 0;
    }
    int length = wcslen(text);
    wchar_t *copy = new wchar_t[length + 3];
    memset(copy, 0, (length + 3) * sizeof(wchar_t));
    int count = 0;
    int i = 0;
    for (; i < length; i++) {
        if (text[i] != '\r' && text[i] != '\n') {
            copy[count++] = text[i];
        }
    }
    if (i != count) {
        wcscat(copy, L"\r\n");
    }
    wcscpy(text, copy);
    delete[] copy;
    return wcslen(text);
}

}


// Checkbox callback005F3CF0: complete native1393code+104switch/alignment+7padding.
extern "C" char const unknown_checkbox_unchecked_7eaecc[];
extern "C" char const unknown_checkbox_right_7eaed8[];
extern "C" char const unknown_checkbox_left_7eaee4[];
extern "C" char const unknown_checkbox_checked_7eaef0[];
namespace OwnerDraw {
LRESULT __stdcall unknown_checkbox_proc_5f3cf0(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    unsigned int key=(unsigned int)window;
    WinData *record=0;
    WinData *&data=record;
    unknown_control_table_a73640.Get_Pointer(key, &data);
    if(data) {
    switch(message) {
    case 0xf:
        if(data->unknown_20)break;
        {
            RECT client;
            struct PaintBounds { RECT display; RECT text; } bounds;
            GetClientRect(window,&client);
            Window_Rect_In_Main_Client(window,&bounds.text);
            Window_Rect_In_Main_Client(window,&bounds.display);
            Rect destination;
            destination=Rect(bounds.display.left,bounds.display.top,bounds.display.right-bounds.display.left,bounds.display.bottom-bounds.display.top);
            destination.Width=18;
            destination.Height=18;
            LONG style=GetWindowLongA(window,-16);
            int state=data->unknown_e8;
            bool special=data->unknown_d9;
            bool alternate=data->unknown_da;
            bool checked=state==1;
            char image_name[64];
            if(special) {
                if(checked) {
                    if(alternate)strcpy(image_name,unknown_checkbox_checked_7eaef0);
                    else strcpy(image_name,unknown_checkbox_left_7eaee4);
                } else {
                    if(alternate)strcpy(image_name,unknown_checkbox_right_7eaed8);
                    else strcpy(image_name,unknown_checkbox_unchecked_7eaecc);
                }
            } else {
                char letter='u';
                if(checked)letter='c';
                sprintf(image_name,"c%ce_i.pcx",letter);
            }
            Surface *image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(image_name,0);
            int height=image->Get_Height();
            Rect source(0,0,image->Get_Width(),height);
            unknown_surface_839958->Copy_From(destination,*image,source,false,true);
            if(style & 0x08000000)Unknown_005feb60(&destination,unknown_surface_839958,0,unknown_color_a763b8);
            wchar_t *text=data->text_28;
            bounds.text.left+=26;
            if(text) {
                unsigned int color=unknown_color_a733e4;
                if(style & 0x08000000)color=unknown_color_a737ec;
                unknown_draw_text_5fe020(unknown_surface_839958,text,&bounds.text,data->font_64,color,4,0xc,0,0,0);
            }
            ValidateRect(window,0);
        }
zero_result:
        return 0;
    case 0xf0:return data->unknown_e8;
    case 0xf1:
        data->unknown_e8=wparam;
        InvalidateRect(window,0,0);
        return 0;
    case 7:case 8:InvalidateRect(window,0,0);break;
    case 0x201:case 0x203: {
        Point2D click((unsigned int)lparam & 0xffff,(unsigned int)lparam>>16);
        if(click.X>=18)goto zero_result;
        if(click.Y>=18)goto zero_result;
        int checked=data->unknown_e8!=1;
        data->unknown_e8=checked;
        InvalidateRect(window,0,0);
        Voc_Play(Rules->GUICheckboxSound,1.0f,0x2000,0);
        HWND parent=GetParent(window);
        SendMessageA(parent,0x111,(unsigned int)(unsigned short)GetWindowLongA(window,-12)|((unsigned int)(unsigned short)checked<<16),(LPARAM)window);
        return 0;
    }
    case 0x497: {
        WNDPROC previous=0;
        unknown_window_proc_table_a73688.Get_Value(window, previous);
        data->unknown_e8=CallWindowProcA(previous,window,0xf0,0,0);
        break;
    }
    case 0x4e5: {
        bool previous=data->unknown_da;
        data->unknown_d9=lparam!=0;
        if(previous!=data->unknown_d9)InvalidateRect(window,0,0);
        break;
    }
    case 0x4e6: {
        bool previous=data->unknown_da;
        data->unknown_da=lparam!=0;
        if(previous!=data->unknown_da)InvalidateRect(window,0,0);
        break;
    }
    case 0x4e7:return data->unknown_da;
    }
    WNDPROC previous=0;
    unknown_window_proc_table_a73688.Get_Value(window, previous);
    return CallWindowProcA(previous,window,message,wparam,lparam);
    }
    return 0;
}
}


extern "C" __declspec(dllimport) BOOL __stdcall LockWindowUpdate(HWND window);
namespace OwnerDraw {
LRESULT __stdcall unknown_button_proc_5f42d0(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    WinData *data=0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)window, &data);
    RECT display; memset(&display,0,sizeof(display));
    Window_Rect_In_Main_Client(window,&display);
    Rect origin;
    origin.X=display.left;origin.Y=display.top;
    origin.Width=display.right-display.left;origin.Height=display.bottom-display.top;
    RECT client; memset(&client,0,sizeof(client));
    GetClientRect(window,&client);
    int state=data->unknown_e8;
    LONG style=GetWindowLongA(window,-16);
    switch(message) {
    case 0xf: {
        Rect rect=origin;
        unsigned long color=unknown_color_a733e4;
        if(!data->surface_10) {
            BSurface *captured=new BSurface(client.right+1,client.bottom+1,2);
            data->surface_10=captured;
            ++unknown_surface_count_a763d8;
            Rect destination,source;
            destination.X=0;destination.Y=0;
            destination.Width=client.right+1;destination.Height=client.bottom+1;
            source.X=display.left;source.Y=display.top;
            source.Width=client.right+1;source.Height=client.bottom+1;
            captured->Copy_From(destination,*unknown_surface_839958,source,false,true);
        }
        if(data->image_14) {
            Surface *image=data->image_14;
            if((state & 1) && data->alternate_image_18)image=data->alternate_image_18;
            Rect source=rect;
            source.Y=source.X=0;
            unknown_surface_839958->Copy_From(rect,*image,source,false,true);
        } else {
            char updown='u';
            if(state & 1)updown='d';
            if(style & 0x08000000)updown='u';
            int widths[2]={7,7};
            int margins[2]={10,10};
            int heights[2]={24,30};
            unsigned index=0;
            for(unsigned i=0;i<2;i++) {
                if(heights[i]>rect.Height && i!=0)break;
                index=i;
            }
            int width=widths[index];
            int height=heights[index];
            int margin=margins[index];
            if(data->surface_10) {
                Rect source,destination;
                source.X=0;source.Y=0;
                source.Width=client.right+1;source.Height=client.bottom+1;
                destination.X=display.left;destination.Y=display.top;
                destination.Width=client.right+1;destination.Height=client.bottom+1;
                unknown_surface_839958->Copy_From(destination,*data->surface_10,source,false,true);
                InvalidateRect(window,0,0);
            }
            origin.Y+=(rect.Height-height)/2;
            if(state & 1)origin.Y+=2;
            char name[40];
            sprintf(name,"b%c%c_li%d.pcx",updown,'e',height);
            Surface *image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(name,0);
            origin.Height=image->Get_Height();
            Rect destination,source;
            destination=origin;
            destination.Width=width;
            source.Width=width;
            source.Y=0;
            source.X=0;
            destination.Height=height;
            source.Height=height;
            unknown_surface_839958->Copy_From(destination,*image,source,false,true);
            sprintf(name,"b%c%c_mi%d.pcx",updown,'e',height);
            image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(name,0);
            Rect middle=origin;
            middle.X+=width;
            middle.Width-=margin;
            middle.Height=image->Get_Height();
            unknown_bitmap_writer_a76378.Draw_0068cce0(middle,unknown_surface_839958,image,0,0);
            sprintf(name,"b%c%c_ri%d.pcx",updown,'e',height);
            image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(name,0);
            destination=origin;
            destination.X+=origin.Width-margin;
            destination.Width=margin;
            destination.Height=image->Get_Height();
            source.Width=destination.Width;
            source.Height=destination.Height;
            source.Y=0;
            source.X=0;
            unknown_surface_839958->Copy_From(destination,*image,source,false,true);
        }
        if(!data->image_14 && data->text_28) {
            RECT unused_client;
            GetClientRect(window,&unused_client);
            RECT text_rect;
            text_rect.left=origin.X;
            text_rect.top=origin.Y+1;
            text_rect.right=origin.Width+origin.X-2;
            text_rect.bottom=origin.Height+origin.Y-2;
            if(state & 1) {text_rect.left+=2;text_rect.top+=4;}
            unknown_draw_text_5fe020(unknown_surface_839958,data->text_28,&text_rect,data->font_64,color,5,0xc,0,0,0);
        }
        if(style & 0x08000000)Unknown_005feb60(&rect,unknown_surface_839958,0,128);
        ValidateRect(window,0);
        return 0;
    }
    case 0xf0:return data->unknown_e8;
    case 0xf1:
        data->unknown_e8=wparam;
        InvalidateRect(window,0,1);
        break;
    case 0x497: {
        WNDPROC previous=0;
        unknown_window_proc_table_a73688.Get_Value(window, previous);
        data->unknown_e8=CallWindowProcA(previous,window,0xf0,0,0);
        break;
    }
    case 0x201:case 0x203:
        if(!data->unknown_e8) {
            data->unknown_e8=1;
            InvalidateRect(window,0,1);
            Voc_Play(Rules->unknown_62c,1.0f,0x2000,0);
            break;
        }
        return 0;
    case 0x202: {
        LockWindowUpdate(GetParent(window));
        WNDPROC previous=0;
        unknown_window_proc_table_a73688.Get_Value(window, previous);
        LRESULT result=CallWindowProcA(previous,window,0x202,wparam,lparam);
        LockWindowUpdate(0);
        return result;
    }
    }
    WNDPROC previous=0;
    unknown_window_proc_table_a73688.Get_Value(window, previous);
    return CallWindowProcA(previous,window,message,wparam,lparam);
}
}


// Not exact (RA2_NON_MATCHING): OwnerDraw::unknown_control_proc_5ee460 at 0x005EE460, best score 64.93, 2026-11-13; differs: 2044 instructions in the original, 1976 in the draft (shape 97.11)
#ifdef RA2_NON_MATCHING
/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Adapted from TibSun ownrdraw.cpp CtrlProc; RA2 native branches, types and extents are authoritative.
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


// From TibSun arraylist.h; release assertions omitted; exact-target callback ABI.
void *__cdecl operator new(unsigned int,void *);
extern "C" double __cdecl floor(double);




LRESULT __fastcall IME_Get_Default_Result_007380b0();
// shared/win_stub.hpp's IME entry points (the unit includes that header further down).
void IME_Reset();                                                                    // 0x00737D90
BOOL __fastcall IME_Process_Message(HWND window, UINT message, WPARAM wparam, LPARAM lparam);   // 0x00737FC0
void __fastcall WideToCharString_006fa0f0(char *dst, unsigned short const *src, int limit);
void __fastcall Unknown_Rect_Shrink_006F1B30(Rect *rect, int x, int y);
WIN32_IMPORT BOOL WINAPI GetUpdateRect(HWND, RECT *, BOOL);
WIN32_IMPORT BOOL WINAPI IntersectRect(RECT *, RECT const *, RECT const *);
WIN32_IMPORT HWND WINAPI GetWindow(HWND, UINT);
WIN32_IMPORT HWND WINAPI WindowFromPoint(POINT);
WIN32_IMPORT int WINAPI GetBkMode(HDC);
WIN32_IMPORT COLORREF WINAPI GetBkColor(HDC);
WIN32_IMPORT COLORREF WINAPI GetTextColor(HDC);
namespace seed_005de980 { int __fastcall FUN_005de980(int *); }
namespace OwnerDraw {
// unknown_window_table_a73400 is defined by this population's initializer005DEB90.
extern HWND unknown_dropdown_window_a763e4;
extern HWND unknown_dropdown_owner_a763e8;
LRESULT __stdcall unknown_button_proc_5f0320(HWND, UINT, WPARAM, LPARAM);
LRESULT __stdcall unknown_list_proc_5f6690(HWND, UINT, WPARAM, LPARAM);
BOOL __stdcall unknown_restore_child_5edd30(HWND, LPARAM);
class UnknownWindowList;
BOOL __stdcall unknown_add_window_5ff430(HWND, UnknownWindowList *);
bool __fastcall unknown_show_tooltip_5ee110(bool);
LRESULT __stdcall unknown_control_proc_5ee460(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	static POINT min_update_rect = {0xFFFFFF, 0xFFFFFF};
	static POINT max_update_rect;
	static int num_rect_updates;

	static bool was_in_focus = true;

	static char in_programmatic_reorder;

	if (message == 0x20) {
		return(1);
	}
    if (IME_Process_Message(window, message, wparam, lparam)) return IME_Get_Default_Result_007380b0();

	// Reference generic layouts: Dictionary56/pack8, ArrayList12. Existing globals remain canonical.
    WNDPROC specific_proc = 0;
	::unknown_window_table_a73400.Get_Value(window, specific_proc);

	WNDPROC original_proc = 0;
	::unknown_window_proc_table_a73688.Get_Value(window, original_proc);

	if (message == 0x105 && wparam == 0x9) {
		SendMessageA(MainWindow, 0x105, 0x9, lparam);
	}

	LRESULT result = 0;
	BOOL show_tooltip = 0;
    BOOL allow_proc = 1;
	int anim_state = 0;

	static WolDictionary<UnknownPairKey, bool> ctrlmessages((unsigned int (__fastcall *)(UnknownPairKey const &))seed_005de980::FUN_005de980);

	UnknownPairKey key;
	key.unknown_04 = (int)window;
	key.unknown_00 = message;

	RECT display_rect;
	Window_Rect_In_Main_Client(window, &display_rect);
	RECT window_rect;
	GetWindowRect(window, &window_rect);
	int offset_x = window_rect.left - display_rect.left;
	int offset_y = window_rect.top - display_rect.top;


	static WolArrayList<HWND> hwndarray;

	if (hwndarray.Length() != 0) {
		HWND topwindow = 0;
		hwndarray.Get(topwindow, hwndarray.Length() - 1);

		BOOL allow = 0;
		BOOL forward = 0;
		if (GetParent(window) == 0) {
			allow = 1;
		}
		if (GetWindowLongA(window, -12) <= 0) {
			allow = 1;
		}

		HWND parent = window;
		while (parent != 0) {
			if (parent == topwindow) {
				allow = 1;
				break;
			}
			parent = GetParent(parent);
		}

		if (message < 0x200 || message > 0x209) {
			forward = 1;
		}
		if (message >= 0xa0 && message <= 0xa9) {
			forward = 0;
		}
		if (message >= 0x100 && message <= 0x108) {
			forward = 0;
		}
		if (message == 0x105 || message == 0x104 || message == 0x112 || message == 0x106) {
			forward = 1;
		}
		if (message == 0x49b || message == 0x113 || message == 0x4ad) {
			forward = 0;
		}
		if (!allow && !forward) {
			return(0);
		}
	}

	if (ctrlmessages.Contains(key)) {
		if (message != 0x111 && message != 0x104 && message != 0x105
			&& message != 0x112 && message != 0x106) {
			return(0);
		}
	}

	bool processing = true;
	ctrlmessages.Remove(key);
	ctrlmessages.Add(key, processing);

	RECT client_rect;
	GetClientRect(window, &client_rect);
	RECT disp_rect;
	Window_Rect_In_Main_Client(window, &disp_rect);

	bool in_focus = unknown_flag_a40d28;

	bool is_paint = false;
	if (message == 0xf) {
		is_paint = true;
		if (!in_focus) {
			ValidateRect(window, 0);
			ctrlmessages.Remove(key);
			return(0);
		}
	}

	if (in_focus == true && !was_in_focus) {
		InvalidateRect(MainWindow, 0, 0);
		in_focus = unknown_flag_a40d28;
	}
	was_in_focus = in_focus;

	RECT update_rect;
	if (is_paint) {
		++num_rect_updates;
		GetUpdateRect(window, &update_rect, 0);
		update_rect.left += disp_rect.left;
		update_rect.right += disp_rect.left;
		update_rect.top += disp_rect.top;
		update_rect.bottom += disp_rect.top;
	}

	WinData *data = Get_Record(window);

	if (message == 0x4a0) {
		result = (data && data->attached_0c != 0);
		goto cleanup;
	}

	if (message == 0x49f && data) {
		HDC hdc = (HDC)lparam;
		HGDIOBJ old = SelectObject(hdc, GetStockObject(0xd));
		(*(HFONT *)((char *)data + 0x1E8)) = (HFONT)old;
		SelectObject(hdc, old);
		(*(int *)((char *)data + 0x1EC)) = GetBkMode(hdc);
		(*(COLORREF *)((char *)data + 0x1F0)) = GetBkColor(hdc);
		(*(COLORREF *)((char *)data + 0x1F4)) = GetTextColor(hdc);
		result = 1;
		goto cleanup;
	}

	if (message == 0x49e && data) {
		HDC hdc = (HDC)lparam;
		SelectObject(hdc, (*(HFONT *)((char *)data + 0x1E8)));
		SetBkMode(hdc, (*(int *)((char *)data + 0x1EC)));
		SetBkColor(hdc, (*(COLORREF *)((char *)data + 0x1F0)));
		SetTextColor(hdc, (*(COLORREF *)((char *)data + 0x1F4)));
		result = 1;
		goto cleanup;
	}

	if (message == 0x49d && data) {
		{
			HWND linked = (HWND)data->attached_0c;
			result = data->unknown_20;
			data->unknown_20 = lparam;
			if (linked != 0) {
				WinData *linkeddata = 0;
				::unknown_control_table_a73640.Get_Pointer((unsigned int const &)data->attached_0c, &linkeddata);
				if (linkeddata != 0) {
					linkeddata->unknown_20 = lparam;
				}
			}
		}

		goto call_custom_proc;
	}

	if (message == 0x4a9) {
		HWND topwindow = 0;
		hwndarray.Get(topwindow, hwndarray.Length() - 1);
		result = (LRESULT)topwindow;

		HWND target = window;
		if (wparam) {
			target = (HWND)wparam;
		}

		HWND found = 0;
		int scan = 0;
		for (int index = 0; index < hwndarray.Length(); index++) {
			hwndarray.Get(found, scan);
			if (found == target) {
				hwndarray.Remove(scan);
			} else {
				scan++;
			}
		}

		if (lparam) {
			hwndarray.Add_Tail(target);
			in_programmatic_reorder = 1;
			SetWindowPos(target, 0, 0, 0, 0, 0, 0x1 | 0x2);
			in_programmatic_reorder = 0;
		}
		goto cleanup;
	}

	if (hwndarray.Length() != 0) {

		if (message == 0x46) {
			if (in_programmatic_reorder != 1) {
				HWND found = 0;
				for (int index = 0; index < hwndarray.Length(); index++) {
					hwndarray.Get(found, index);
					if (found == window) {
						int *wp = (int *)lparam;
						if (index == hwndarray.Length() - 1 && wp[1] != 0 && (wp[6] & 0x4) == 0) {
							wp[1] = 0;
							InvalidateRect(window, 0, 0);
							result = 0;
						} else {
							wp[6] |= 0x200 | 0x4;
							InvalidateRect(window, 0, 0);
							result = 0;
						}
						goto cleanup;
					}
				}
			}
			goto call_custom_proc;
		}

		if (message == 0x2) {
			HWND found = 0;
			int scan = 0;
			for (int index = 0; index < hwndarray.Length(); index++) {
				hwndarray.Get(found, scan);
				if (found == window) {
					hwndarray.Remove(scan);
				} else {
					scan++;
				}
			}
		}
	}

	if (window == (HWND)unknown_tooltip_a737f0.unknown_11c
		&& (message == 0x82 || message == 0x18 || message == 0x8)
		&& unknown_tooltip_a737f0.shown_114) {
		unknown_5ee3b0();
	}

	if (message == 0x14) {
		result = 1;
		goto cleanup;
	} else if (message == 0x7) {
		if (specific_proc == unknown_button_proc_5f0320 || specific_proc == unknown_list_proc_5f6690) {
			SetFocus((HWND)wparam);
		}
        if (data && !data->unknown_38) { IME_Reset(); data->unknown_38 = 1; }
    } else if (message == 8) {
        data->unknown_38 = 0;
	} else if (message == 0x18) {
		if (wparam == 0 && data) {
			(*(int *)((char *)data + 0x1FC)) = 0;
		}
	}
	if (data) {
		if (message == 0x49c) {
			result = (LRESULT)data->image_14;
			data->image_14 = (Surface *)lparam;
			goto cleanup;
		} else if (message == 0x4aa) {
			result = (LRESULT)(*(Surface **)((char *)data + 0x18));
			(*(Surface **)((char *)data + 0x18)) = (Surface *)lparam;
			goto cleanup;
		} else if (message == 0x49a) {
			result = (*(int *)((char *)data + 0x24));
			(*(int *)((char *)data + 0x24)) = lparam;
			goto cleanup;
		}
		if ((*(int *)((char *)data + 0x24)) && (message == 0x113 || (message >= 0x200 && message <= 0x209))) {
			RECT corner;
			GetWindowRect(window, &corner);
			WindowFromPoint(*(POINT *)&corner);
		}
	}
    if (message == 0xC) allow_proc = false;
    else if (message == 0xD) { allow_proc = false; result = 0; }
    else if (message == 0x189) { allow_proc = false; result = 0; }
    else if (message == 0x143 || message == 0x14C || message == 0x158 || message == 0x14D || message == 0x14A || message == 0x148 || message == 0x180 || message == 0x18F || message == 0x1A2 || message == 0x18C || message == 0x181) { allow_proc = false; result = -1; }
    if (data) {
        if (message == 0x4CE) { allow_proc = false; result = data->unknown_2c == 0; }
        else if (message == 0x4B5) {
            char *destination = (char *)lparam;
            if (destination) {
                *destination = 0;
                if (data->text_28) {
                    WideToCharString_006fa0f0(destination, data->text_28, wparam);
                    if (wparam > 0) destination[wparam - 1] = 0;
                }
            }
        } else if (message == 0x4B3) {
            wchar_t *destination = (wchar_t *)lparam;
            if (destination) {
                *destination = 0;
                if (data->text_28) {
                    wcsncpy(destination, data->text_28, wparam);
                    if ((int)wparam > 0) destination[wparam - 1] = 0;
                }
            }
        } else if (message == 0x4EB) {
            *(int *)((char *)data + 0x50) = lparam;
        } else if (message == 0x4B4) {
            data->Set_Text_600560((char const *)lparam);
            data->unknown_2c = 1;
        } else if (message == 0x4D1) {
            result = *(int *)((char *)data + 0x30);
            *(int *)((char *)data + 0x30) = wparam;
            allow_proc = false;
        } else if (message == 0x4B2) {
            wchar_t const *text = data->text_28;
            wchar_t const *new_text = (wchar_t const *)lparam;
            bool either = text != 0 || new_text != 0;
            bool added = text == 0 && new_text != 0;
            bool removed = text != 0 && new_text == 0;
            bool changed = added || removed || (either && wcscmp(text, new_text) != 0);
            data->Set_Wide_Text_600510((wchar_t const *)lparam);
            data->unknown_2c = 0;
            if (changed && data->unknown_70 == 1 && data->unknown_94[0xA8 - 0x94]) {
                KillTimer(window, 0);
                data->unknown_94[0xA8 - 0x94] = 0;
                SendMessageA(window, 0x4EE, 0, 0);
            }
        }
    }
    if (message == 0x200) {
        HWND parent = GetParent(window);
        HWND status = GetDlgItem(parent, 0x695);
        if (status) {
            WideString text;
            unsigned int mx = (unsigned short)lparam;
            unsigned int my = (unsigned short)((unsigned int)lparam >> 16);
            text = unknown_5e21c0(parent, window, mx, my);
            if (wcscmp(text.Get(), (wchar_t const *)L"") == 0) {
                text = unknown_5e2160(parent, (int)window);
                if (wcscmp(text.Get(), (wchar_t const *)L"") == 0) {
                    char const *label = unknown_5e22b0((unsigned int)parent, window);
                    WideString name(label ? StringTable::Load_String(label, 0, "D:\\Ra2\\ownrdraw.cpp", 0x76E) : (wchar_t const *)L"");
                    text = name;
                }
            }
            SendMessageA(status, 0x4B2, 0, (LPARAM)text.unknown_text_770430());
        }
    }

	if (is_paint && data) {

		if (data->unknown_20) {
			ValidateRect(window, 0);
			result = CallWindowProcA(specific_proc, window, 0x4a5, wparam, lparam);
		} else {
			Rect sibling_rect;
			RECT intersect;

			{
				RECT tooltip_rect;
				tooltip_rect.left = unknown_tooltip_a737f0.rect_00.X;
				tooltip_rect.right = unknown_tooltip_a737f0.rect_00.Width + unknown_tooltip_a737f0.rect_00.X + 1;
				tooltip_rect.top = unknown_tooltip_a737f0.rect_00.Y;
				tooltip_rect.bottom = unknown_tooltip_a737f0.rect_00.Height + unknown_tooltip_a737f0.rect_00.Y + 1;
				if (num_rect_updates == 1) {
					RECT intersect;
					if (IntersectRect(&intersect, &disp_rect, &tooltip_rect)) {
						if (unknown_tooltip_a737f0.shown_114 && unknown_tooltip_a737f0.drawn_118 != 1 && unknown_tooltip_a737f0.surface_10 != 0) {
							Rect drect = unknown_tooltip_a737f0.rect_00;
							Rect srect(0, 0, unknown_tooltip_a737f0.rect_00.Width, unknown_tooltip_a737f0.rect_00.Height);
							unknown_surface_839950->Copy_From(drect, *unknown_tooltip_a737f0.surface_10, srect, false, true);
							unknown_tooltip_a737f0.drawn_118 = true;
							show_tooltip = true;
						}
					}
				}
			}

			if (min_update_rect.x >= disp_rect.left) {
				min_update_rect.x = disp_rect.left;
			}
			if (min_update_rect.y >= disp_rect.top) {
				min_update_rect.y = disp_rect.top;
			}
			if (max_update_rect.x <= disp_rect.right) {
				max_update_rect.x = disp_rect.right;
			}
			if (max_update_rect.y <= disp_rect.bottom) {
				max_update_rect.y = disp_rect.bottom;
			}

			HWND owner = window;
			while (owner != 0) {
				if (GetWindowLongA(owner, 0x4) != 0) {
					break;
				}
				owner = GetParent(owner);
			}

			WinData *ownerdata = 0;
			if (owner != 0) {
				unknown_control_table_a73640.Get_Pointer((unsigned int const &)owner, &ownerdata);
			}

			int state;
			if (owner == window) {
				if ((*(int *)((char *)ownerdata + 0x1FC)) < 1) {
					state = 1;
					anim_state = 1;
					goto call_proc_paint;
				}
				state = (*(int *)((char *)ownerdata + 0x1FC));
			} else {
				if (ownerdata == 0) {
					goto skip_paint;
				}
				state = (*(int *)((char *)ownerdata + 0x1FC));
			}

			anim_state = state;
			if (state < 1) {
			skip_paint:
				state = anim_state;
				ValidateRect(window, 0);
				result = 1;
			} else {

			call_proc_paint:
				result = CallWindowProcA(specific_proc, window, message, wparam, lparam);
				if (ownerdata != 0) {
					(*(int *)((char *)ownerdata + 0x1FC)) = state;
				}

				{
					WolArrayList<HWND> children;
					EnumChildWindows(window, (WNDENUMPROC)unknown_add_window_5ff430, (LPARAM)&children);

					HWND combo_owner = 0;
					HWND child = 0;
					for (int index = 0; index < children.Length(); index++) {
						children.Get(child, index);

						WNDPROC childproc = 0;
						::unknown_window_proc_table_a73688.Get_Value(child, childproc);

						if (childproc != (WNDPROC)unknown_combo_drop_proc_5ead20) {
							InvalidateRect(child, 0, 0);
							UpdateWindow(child);
						} else {
							combo_owner = child;
						}
					}

					if (combo_owner != 0) {
						InvalidateRect(combo_owner, 0, 0);
						UpdateWindow(combo_owner);
					} else if (unknown_dropdown_window_a763e4 != 0) {
						if (unknown_dropdown_owner_a763e8 == owner) {
							Rect drop_rect;
							Window_Rect_In_Main_Client(unknown_dropdown_window_a763e4, (RECT *)&drop_rect);
							RECT intersect;
							if (IntersectRect(&intersect, &disp_rect, (const RECT *)&drop_rect)) {
								InvalidateRect(unknown_dropdown_window_a763e4, 0, 0);
								UpdateWindow(unknown_dropdown_window_a763e4);
							}
						}
					}
				}

				state = anim_state;
			}

		finalize_paint:
			if (state > 0) {
				HWND sibling = owner;
				while (window == owner || num_rect_updates == 1) {
					if (sibling == 0) {
						break;
					}
					sibling = GetWindow(sibling, 0x3);
					if (sibling == 0) {
						break;
					}
					if (GetWindowLongA(sibling, 0x4)) {
						Window_Rect_In_Main_Client(sibling, (RECT *)&sibling_rect);
						if (IntersectRect(&intersect, &disp_rect, (const RECT *)&sibling_rect)) {
							InvalidateRect(sibling, 0, 0);
							UpdateWindow(sibling);
							break;
						}
					}
				}
			}
		}
		goto after_proc;
	} else {
        if (allow_proc) goto call_custom_proc;
        goto after_proc;
	}

call_custom_proc:
	if (specific_proc != 0) {
		result = CallWindowProcA(specific_proc, window, message, wparam, lparam);
	}

after_proc:
	if (message == 0x82) {
		HWND destroyed = window;
		WinData * destroydata;
		unsigned int destroyed_key=(unsigned int)destroyed;
		if (::unknown_control_table_a73640.Get_Pointer(destroyed_key, &destroydata)) {
			if (destroydata != 0 && destroydata->surface_10 != 0) {
				delete destroydata->surface_10;
				destroydata->surface_10 = 0;
				unknown_surface_count_a763d8--;
			}
		}
		unknown_restore_child_5edd30(window, 0);
	}

cleanup:
    ctrlmessages.Remove(key);
    if (data && is_paint) {
        if (GetWindowLongA(window, 4) && num_rect_updates > 1) *(int *)((char *)data + 0x1FC) = 2;
        if (--num_rect_updates == 0) {
            if (!data->unknown_20 && anim_state >= 1) {
                Rect source(min_update_rect.x, min_update_rect.y, max_update_rect.x - min_update_rect.x, max_update_rect.y - min_update_rect.y);
                Rect destination(min_update_rect.x, min_update_rect.y, max_update_rect.x - min_update_rect.x, max_update_rect.y - min_update_rect.y);
                if (*(int *)((char *)data + 0x1FC) == 1) {
                    *(int *)((char *)data + 0x1FC) = 2;
                    if (GetWindowLongA(window, 4) && unknown_show_dialog_5e61b0(window)) *(int *)((char *)data + 0x1FC) = 3;
                    Rect whole(min_update_rect.x, min_update_rect.y, max_update_rect.x - min_update_rect.x, max_update_rect.y - min_update_rect.y);
                    destination = Offset_Rect(whole, offset_x, offset_y);
                    unknown_surface_839950->Lock();
                    unknown_surface_839958->Lock();
                    unknown_surface_839950->Copy_From(destination, *unknown_surface_839958, whole, false, true);
                    unknown_surface_839958->Unlock();
                    unknown_surface_839950->Unlock();
                    WolArrayList<HWND> children;
                    EnumChildWindows(window, (WNDENUMPROC)unknown_add_window_5ff430, (LPARAM)&children);
                    HWND child = 0;
                    for (int index = 0; index < children.Length(); index++) {
                        children.Get(child, index);
                        SendMessageA(child, 0x4AF, 0, 0);
                    }
                } else {
                    destination = Offset_Rect(source, offset_x, offset_y);
                    char class_name[128];
                    GetClassNameA(window, class_name, sizeof(class_name));
                    if (strcmp(class_name, "ComboBox") == 0) {
                        Unknown_Rect_Shrink_006F1B30(&source, -1, -1);
                        Unknown_Rect_Shrink_006F1B30(&destination, -1, -1);
                    }
                    unknown_surface_839950->Lock();
                    unknown_surface_839958->Lock();
                    unknown_surface_839950->Copy_From(destination, *unknown_surface_839958, source, false, true);
                    unknown_surface_839958->Unlock();
                    unknown_surface_839950->Unlock();
                }
            }
            min_update_rect.x = 0xFFFFFF;
            min_update_rect.y = 0xFFFFFF;
            max_update_rect.x = 0;
            max_update_rect.y = 0;
        }
    }
    if (show_tooltip) unknown_show_tooltip_5ee110(true);
    if (message == 0x110) {
        result = 0;
    }
    return result;
}

}
#endif

// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// LRESULT OwnerDraw::unknown_scroll_bar_proc_5f9fb0(HWND, UINT, WPARAM, LPARAM) (0x005F9FB0)
// Adopted from ra2-decomp OwnerDraw::ScrollBarHandler (YR 0x0061C690).

// winuser.h SBM_SETSCROLLINFO argument.
struct tagSCROLLINFO {
    UINT cbSize;
    UINT fMask;
    int nMin;
    int nMax;
    UINT nPage;
    int nPos;
    int nTrackPos;
};
typedef tagSCROLLINFO SCROLLINFO;

extern "C" double __cdecl log(double value);
#pragma intrinsic(log)

namespace OwnerDraw {

void __fastcall unknown_draw_edge_glows_5fd810(Surface *surface, Rect const &rect, int raised, int count,
                                               int left_alpha, int top_alpha, int right_alpha, int bottom_alpha);
void __fastcall unknown_draw_arrow_5fd960(Surface *surface, Rect const &rect, int upward, int pressed, bool prefixed);

// A scroll bar's view of its registry record (WinData, 0x200 bytes): the shared fields it
// reads and its state from +0xE8.
struct UnknownScrollBarRecord {
    char unknown_00[0x08];
    HWND owner_08;          // +0x08 told of position changes (WM_VSCROLL)
    char unknown_0c[0x10 - 0x0c];
    Surface *surface_10;    // +0x10 the dimmed background
    char unknown_14[0x20 - 0x14];
    int unknown_20;         // +0x20 nonzero: WM_PAINT draws nothing
    char unknown_24[0xbc - 0x24];
    bool unknown_bc;        // +0xBC WM_PAINT only validates
    char unknown_bd[0xc8 - 0xbd];
    int unknown_c8;         // +0xC8 background dimming alpha
    char unknown_cc;
    bool alternate_cd;      // +0xCD draws the "g" (disabled) art (0x4F1)
    char unknown_ce[0xe8 - 0xce];
    int result;             // +0xE8 the left button is held
    int dragging;           // +0xEC the thumb is dragged
    int range;              // +0xF0
    int position;           // +0xF4
    int up_pressed;         // +0xF8
    int down_pressed;       // +0xFC
    int keep_capture;       // +0x100 give the capture back to owner_08 on release (0x4D2)
};

// The window procedure of the owner-drawn ScrollBar controls.
LRESULT __stdcall unknown_scroll_bar_proc_5f9fb0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    RECT client_rect;
    RECT display_rect;
    GetClientRect(window, &client_rect);
    Window_Rect_In_Main_Client(window, &display_rect);
    client_rect.bottom -= 2 * unknown_color_a73928;
    client_rect.right -= 2 * unknown_color_a73928;
    display_rect.left += unknown_color_a73928;
    display_rect.right -= unknown_color_a73928;
    display_rect.top += unknown_color_a73928;
    display_rect.bottom -= unknown_color_a73928;
    UnknownScrollBarRecord *data = (UnknownScrollBarRecord *)Get_Record(window);
    if (data == 0) {
        return 0;
    }
    bool keep_parent_capture = false;
    if (data->keep_capture) {
        keep_parent_capture = true;
    }
    int message_result = data->result;
    int dragging = data->dragging;
    int range = data->range;
    int position = data->position;
    int up_pressed = data->up_pressed;
    int down_pressed = data->down_pressed;
    if (!range) {
        range = 100;
    }
    int grip_top = 0;
    int grip_bottom = 0;
    int scroll_code = 0;
    int width = client_rect.right - client_rect.left;
    int travel = client_rect.bottom - client_rect.top - 44;
    int grip_height = (int)((double)travel - log((double)(range + 1)) * (double)travel * 0.2);
    if (grip_height <= 14) {
        grip_height = 14;
    }
    travel -= grip_height;
    if (travel <= 1) {
        travel = 1;
    }
    if (message < 0x400 || message == 0x4a5) {
        if (!dragging) {
            grip_top = position * travel / range + client_rect.top + 22;
            grip_bottom = grip_top + grip_height;
        } else {
            POINT cursor;
            GetCursorPos(&cursor);
            ScreenToClient(window, &cursor);
            grip_top = cursor.y - grip_height / 2;
            if (grip_top < 22) {
                grip_top = 22;
            }
            if (client_rect.bottom - grip_height - 22 < grip_top) {
                grip_top = client_rect.bottom - grip_height - 22;
            }
            grip_bottom = grip_top + grip_height;
            scroll_code = 5;
            position = range * (grip_top - 22) / travel;
        }
    }
    switch (message) {
    case 0xe1:
        return position;
    case 0xe0:
        if ((int)wparam <= range && (int)wparam > 0) {
            position = (int)wparam;
        }
        break;
    case 0xe2:
        range = (int)lparam;
        if (position > range) {
            position = range;
        }
        break;
    case 0xe9: {
        SCROLLINFO *info = (SCROLLINFO *)lparam;
        range = info->nMax;
        position = info->nPos;
        break;
    }
    case 0x84:
    case 0x87: {
        WNDPROC proc = 0;
        unknown_window_proc_table_a73688.Get_Value(window, proc);
        return CallWindowProcA(proc, window, message, wparam, lparam);
    }
    case 0x14:
        return 0;
    case 0xf: {
        if (data->unknown_bc) {
            ValidateRect(window, 0);
            break;
        }
        if (data->unknown_20) {
            return 0;
        }
        Rect src_rect(0, 0, client_rect.right, client_rect.bottom);
        Rect full_rect(display_rect.left, display_rect.top, client_rect.right, client_rect.bottom);
        HWND parent = GetParent(window);
        WinData *parent_data = 0;
        if (parent) {
            unknown_control_table_a73640.Get_Pointer((unsigned int const &)parent, &parent_data);
        }
        RECT parent_display;
        Window_Rect_In_Main_Client(parent, &parent_display);
        Rect source_rect = src_rect;
        if (parent_data && parent_data->surface_10) {
            source_rect.X = display_rect.left + source_rect.X - parent_display.left;
            source_rect.Y += display_rect.top - parent_display.top;
        }
        if (data->surface_10) {
            if (data->surface_10->Get_Width() != client_rect.right || data->surface_10->Get_Height() != client_rect.bottom) {
                delete data->surface_10;
                data->surface_10 = 0;
            }
        }
        if (!data->surface_10) {
            BSurface *background = new BSurface(client_rect.right, client_rect.bottom, 2);
            data->surface_10 = background;
            unknown_surface_count_a763d8++;
            if (parent_data && parent_data->surface_10) {
                background->Copy_From(src_rect, *parent_data->surface_10, source_rect, false, true);
            }
            int count = client_rect.bottom * client_rect.right;
            unsigned short *pixels = (unsigned short *)background->Lock();
            for (int i = 0; i < count; i++) {
                pixels[i] = Blend_Color(pixels[i], 0xffff, data->unknown_c8);
            }
            if (pixels) {
                background->Unlock();
            }
        }
        if (parent_data && parent_data->surface_10) {
            unknown_surface_839958->Copy_From(full_rect, *parent_data->surface_10, source_rect, false, true);
        }
        int color = unknown_5fda40(unknown_color_a76158);
        if (data->alternate_cd) {
            color = unknown_5fda40(unknown_color_a73910);
        }
        unknown_5fdb20(unknown_surface_839958, full_rect, unknown_color_a73928, color);
        Rect grip_rect(display_rect.left, grip_top + display_rect.top, client_rect.right, grip_bottom - grip_top);
        full_rect = grip_rect;
        src_rect = grip_rect;
        src_rect.X = 0;
        src_rect.Y = grip_top;
        Surface *grip_middle;
        if (data->alternate_cd) {
            grip_middle = unknown_bitmap_writer_a76378.Get_Image_0068ca40("gsbgripm.pcx", 0);
        } else {
            grip_middle = unknown_bitmap_writer_a76378.Get_Image_0068ca40("sbgripm.pcx", 0);
        }
        if (grip_middle) {
            grip_rect.Width = grip_middle->Get_Width();
        }
        unknown_bitmap_writer_a76378.Draw_0068cce0(grip_rect, unknown_surface_839958, grip_middle, 0, 0);
        Rect grip_src(0, 0, grip_middle->Get_Height(), grip_middle->Get_Width());
        Surface *grip_top_image;
        if (data->alternate_cd) {
            grip_top_image = unknown_bitmap_writer_a76378.Get_Image_0068ca40("gsbgript.pcx", 0);
        } else {
            grip_top_image = unknown_bitmap_writer_a76378.Get_Image_0068ca40("sbgript.pcx", 0);
        }
        if (grip_top_image) {
            full_rect.Height = grip_top_image->Get_Height();
        }
        unknown_surface_839958->Copy_From(full_rect, *grip_top_image, grip_src, false, true);
        Surface *grip_bottom_image;
        if (data->alternate_cd) {
            grip_bottom_image = unknown_bitmap_writer_a76378.Get_Image_0068ca40("gsbgripb.pcx", 0);
        } else {
            grip_bottom_image = unknown_bitmap_writer_a76378.Get_Image_0068ca40("sbgripb.pcx", 0);
        }
        if (grip_bottom_image) {
            full_rect.Y = display_rect.top + grip_bottom - grip_bottom_image->Get_Height();
        }
        unknown_surface_839958->Copy_From(full_rect, *grip_bottom_image, grip_src, false, true);
        Rect up_rect(display_rect.left, display_rect.top, client_rect.right, 22);
        full_rect = up_rect;
        src_rect = up_rect;
        src_rect.X = 0;
        src_rect.Y = 0;
        unknown_surface_839958->Copy_From(full_rect, *data->surface_10, src_rect, false, true);
        unknown_draw_edge_glows_5fd810(unknown_surface_839958, up_rect, up_pressed == 0, 2, unknown_color_a733d0,
                                       unknown_color_a733d0, unknown_color_a733d0, unknown_color_a733d0);
        unknown_draw_arrow_5fd960(unknown_surface_839958, up_rect, 1, up_pressed, data->alternate_cd);
        Rect down_rect(display_rect.left, display_rect.bottom - 22, client_rect.right, 22);
        full_rect = down_rect;
        src_rect = down_rect;
        src_rect.X = 0;
        src_rect.Y = client_rect.bottom - 22;
        unknown_surface_839958->Copy_From(full_rect, *data->surface_10, src_rect, false, true);
        unknown_draw_edge_glows_5fd810(unknown_surface_839958, down_rect, down_pressed == 0, 2, unknown_color_a733d0,
                                       unknown_color_a733d0, unknown_color_a733d0, unknown_color_a733d0);
        unknown_draw_arrow_5fd960(unknown_surface_839958, down_rect, 0, down_pressed, data->alternate_cd);
        ValidateRect(window, 0);
        break;
    }
    case 0x113: {
        POINT cursor;
        GetCursorPos(&cursor);
        ScreenToClient(window, &cursor);
        up_pressed = 0;
        down_pressed = 0;
        if (message_result && cursor.x > client_rect.right - width) {
            if (cursor.y < 22) {
                if (!dragging) {
                    up_pressed = 1;
                }
                if (position != 0) {
                    scroll_code = 0;
                    position--;
                }
            } else if (cursor.y > client_rect.bottom - 22) {
                if (!dragging) {
                    down_pressed = 1;
                }
                if (position + 1 <= range) {
                    scroll_code = 1;
                    position++;
                }
            }
        }
        SetTimer(window, 0, 0x19, 0);
        break;
    }
    case 0x200:
        if (dragging) {
            RECT area = client_rect;
            area.left = client_rect.right - width;
            InvalidateRect(window, &area, 0);
        }
        if (wparam & 1) {
            break;
        }
    case 0x202:
        message_result = 0;
        dragging = 0;
        if (up_pressed || down_pressed) {
            InvalidateRect(window, 0, 0);
        }
        up_pressed = 0;
        down_pressed = 0;
        KillTimer(window, 0);
        ReleaseCapture();
        if (keep_parent_capture) {
            SetCapture(data->owner_08);
        }
        scroll_code = 8;
        break;
    case 0x201:
    case 0x203: {
        if (message == 0x201) {
            message_result = 1;
            SetCapture(window);
            SetTimer(window, 0, 500, 0);
        } else {
            message_result = 0;
            dragging = 0;
            KillTimer(window, 0);
            ReleaseCapture();
            if (keep_parent_capture) {
                SetCapture(data->owner_08);
            }
        }
        POINT click;
        click.x = (unsigned short)lparam;
        click.y = (unsigned short)(((unsigned long)lparam >> 16) & 0xffff);
        int repeat = 1;
        if (message == 0x203) {
            repeat = 2;
        }
        down_pressed = 0;
        up_pressed = 0;
        while (repeat > 0) {
            if (click.x > client_rect.right - width) {
                if (click.y < 22 && position) {
                    up_pressed = 1;
                    scroll_code = 0;
                    position--;
                } else {
                    bool skip_thumb = false;
                    if (click.y > client_rect.bottom - 22) {
                        if (position + 1 <= range) {
                            down_pressed = 1;
                            scroll_code = 1;
                            position++;
                            skip_thumb = true;
                        }
                    }
                    if (!skip_thumb) {
                        if (click.y < grip_top || click.y >= grip_bottom) {
                            grip_top = click.y - grip_height / 2;
                            if (grip_top < 22) {
                                grip_top = 22;
                            }
                            if (client_rect.bottom - grip_height - 22 < grip_top) {
                                grip_top = client_rect.bottom - grip_height - 22;
                            }
                            grip_bottom = grip_top + grip_height;
                            scroll_code = 5;
                            position = range * (grip_top - 22) / travel;
                        } else if (message == 0x201) {
                            dragging = 1;
                        }
                    }
                }
            }
            repeat--;
        }
        break;
    }
    case 0x4d2:
        data->keep_capture = lparam != 0;
        break;
    case 0x4f1:
        data->alternate_cd = lparam == 1;
        break;
    }
    int send_notify = 0;
    if (position != data->position || range != data->range) {
        if (data->owner_08) {
            send_notify = 1;
        }
    }
    data->result = message_result;
    data->dragging = dragging;
    data->range = range;
    data->position = position;
    data->up_pressed = up_pressed;
    data->down_pressed = down_pressed;
    if (send_notify) {
        SendMessageA(data->owner_08, 0x115, (unsigned short)position << 16 | (unsigned short)scroll_code, (LPARAM)window);
        InvalidateRect(window, 0, 0);
    }
    return 0;
}

}


// Progress-bar control callback, independently bounded638 code bytes +2 padding.
namespace OwnerDraw {
LRESULT __stdcall unknown_progress_proc_5faff0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    RECT rect;
    Window_Rect_In_Main_Client(window, &rect);
    WinData *data = 0;
    unknown_control_table_a73640.Get_Pointer((unsigned int)window, &data);
    if (data) {
        switch (message) {
        case 0x497:
            data->unknown_ec = 100;
            break;
        case 0x401:
            data->unknown_e8 = (unsigned int)lparam & 0xffff;
            data->unknown_ec = (unsigned int)lparam >> 16;
            break;
        case 0x402: {
            int position = (int)wparam;
            if (position < data->unknown_e8) position = data->unknown_e8;
            if (position > data->unknown_ec) position = data->unknown_ec;
            data->unknown_f0 = position;
            InvalidateRect(window, 0, 0);
            break;
        }
        case 0xf: {
            Rect source(0, 0, rect.right-rect.left+1, rect.bottom-rect.top+1);
            Rect destination(rect.left, rect.top, rect.right-rect.left+1, rect.bottom-rect.top+1);
            if (!data->surface_10) {
                Surface *background = new BSurface(rect.right-rect.left+1, rect.bottom-rect.top+1, 2);
                data->surface_10 = background;
                ++unknown_surface_count_a763d8;
                background->Copy_From(source, *unknown_surface_839958, destination, false, true);
            }
            unknown_surface_839958->Copy_From(destination, *data->surface_10, source, false, true);
            int position = (data->unknown_f0 * 65536) / (data->unknown_ec - data->unknown_e8);
            int color = unknown_5fda40(0xff);
            unknown_5fe7c0(destination, unknown_surface_839958, color, position);
            ValidateRect(window, 0);
            break;
        }
        }
    }
    return 0;
}
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// LRESULT OwnerDraw::unknown_trackbar_proc_5fb270(HWND, UINT, WPARAM, LPARAM) (0x005FB270)
// Adopted from ra2-decomp OwnerDraw::TrackBarHandler (YR 0x0061D950).

namespace OwnerDraw {

// A track bar's state in its registry record from +0xE8 (the progress bar keeps its range there).
struct UnknownTrackBarState {
    int result;         // +0xE8
    int dragging;       // +0xEC
    int range;          // +0xF0
    int value;          // +0xF4
    int minimum;        // +0xF8
    int thumb_pos;      // +0xFC
    int step;           // +0x100
    int show_numbers;   // +0x104
    int click_suppress; // +0x108
};

// The window procedure of the msctls_trackbar32 controls.
LRESULT __stdcall unknown_trackbar_proc_5fb270(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    bool play_click = true;

    RECT client_rect;
    RECT display_rect;
    GetClientRect(window, &client_rect);
    Window_Rect_In_Main_Client(window, &display_rect);
    int number_width = 50;

    WinData *data = Get_Record(window);
    if (data == 0) {
        return 0;
    }
    UnknownTrackBarState *track = (UnknownTrackBarState *)&data->unknown_e8;

    int message_result = track->result;
    int dragging = track->dragging;
    int range = track->range;
    int value = track->value;
    int minimum = track->minimum;
    int maximum;
    int thumb_pos = track->thumb_pos;
    int step = track->step;
    int show_numbers = track->show_numbers;

    if (!show_numbers) {
        number_width = 0;
    }

    int slider_width = client_rect.right - client_rect.left - number_width - 13;
    if (slider_width <= 1) {
        slider_width = 1;
    }

    if (!range) {
        WNDPROC proc = 0;
        unknown_window_proc_table_a73688.Get_Value(window, proc);

        int range_max = CallWindowProcA(proc, window, 0x402, 0, 0);
        minimum = CallWindowProcA(proc, window, 0x401, 0, 0);
        range = range_max - minimum;
        value = CallWindowProcA(proc, window, 0x400, 0, 0) - minimum;
        thumb_pos = value * slider_width / range;
        if (!range) {
            range = 100;
        }

        track->thumb_pos = thumb_pos;
        track->range = range;
        track->value = value;
        track->minimum = minimum;
        track->step = step;
        track->show_numbers = show_numbers;
    }

    if (!step) {
        number_width = 50;
        step = 1;
        show_numbers = 1;
    }

    LONG grip_left;
    LONG grip_right;
    if (!dragging) {
        grip_left = client_rect.left + thumb_pos + 1;
        grip_right = grip_left + 12;
    } else {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(window, &point);

        int xpos = point.x - 6;
        if (xpos < 1) {
            xpos = 1;
        }
        if (client_rect.right - number_width - 12 < xpos) {
            xpos = client_rect.right - number_width - 12;
        }

        int idx = ((range + 1) * (xpos - 1)) / slider_width;
        if (idx >= range) {
            idx = range;
        }

        value = step * ((minimum + idx) / step) - minimum;
        thumb_pos = value * slider_width / range;
        grip_left = thumb_pos + 1;
        grip_right = grip_left + 12;
    }

    switch (message) {
    case 0x84:
    case 0x87: {
        WNDPROC proc = 0;
        unknown_window_proc_table_a73688.Get_Value(window, proc);
        return CallWindowProcA(proc, window, message, wparam, lparam);
    }

    case 0xa:
        InvalidateRect(window, 0, FALSE);
        break;

    case 0xf: {
        RECT paint_rect;
        Window_Rect_In_Main_Client(window, &paint_rect);
        Rect disp_rect(paint_rect.left, paint_rect.top, paint_rect.right - paint_rect.left, paint_rect.bottom - paint_rect.top);
        LONG style = GetWindowLongA(window, -16);

        if (data->surface_10 == 0) {
            RECT disp_copy;
            RECT client_copy;
            Window_Rect_In_Main_Client(window, &disp_copy);
            GetClientRect(window, &client_copy);

            Surface *surf = new BSurface(client_copy.right + 1, client_copy.bottom + 1, 2);
            data->surface_10 = surf;
            unknown_surface_count_a763d8++;

            Rect src_rect(0, 0, client_copy.right + 1, client_copy.bottom + 1);
            Rect dst_rect(disp_copy.left, disp_copy.top, client_copy.right + 1, client_copy.bottom + 1);
            surf->Copy_From(src_rect, *unknown_surface_839958, dst_rect, false, true);
            Unknown_005feb60(&src_rect, surf, 0, data->unknown_c8);
        }

        if (data->surface_10 != 0) {
            RECT disp_copy;
            RECT client_copy;
            Window_Rect_In_Main_Client(window, &disp_copy);
            GetClientRect(window, &client_copy);

            Rect src_rect(0, 0, client_copy.right + 1, client_copy.bottom + 1);
            Rect dst_rect(disp_copy.left, disp_copy.top, client_copy.right + 1, client_copy.bottom + 1);
            unknown_surface_839958->Copy_From(dst_rect, *data->surface_10, src_rect, false, true);
        }

        if (show_numbers) {
            {
                Surface *center = unknown_bitmap_writer_a76378.Get_Image_0068ca40("trofm.pcx", 0);
                Rect center_rect(disp_rect.X + disp_rect.Width - number_width + 1, disp_rect.Y - 1, number_width, center->Get_Height());
                unknown_bitmap_writer_a76378.Draw_0068cce0(center_rect, unknown_surface_839958, center, 0, 0);
            }
            {
                Surface *left = unknown_bitmap_writer_a76378.Get_Image_0068ca40("trofl.pcx", 0);
                Rect left_dst(disp_rect.X + disp_rect.Width - number_width + 1, disp_rect.Y - 1, left->Get_Width(), left->Get_Height());
                Rect left_src = left_dst;
                left_src.Y = 0;
                left_src.X = 0;
                unknown_surface_839958->Copy_From(left_dst, *left, left_src, false, true);
            }
            {
                Surface *right = unknown_bitmap_writer_a76378.Get_Image_0068ca40("trofr.pcx", 0);
                Rect right_dst(disp_rect.X + disp_rect.Width - right->Get_Width() + 1, disp_rect.Y - 1, right->Get_Width(), right->Get_Height());
                Rect right_src = right_dst;
                right_src.Y = 0;
                right_src.X = 0;

                unknown_surface_839958->Copy_From(right_dst, *right, right_src, false, true);
            }
        }

        Rect grip_rect(paint_rect.left + grip_left, paint_rect.top, grip_right - grip_left, paint_rect.bottom - paint_rect.top);
        Surface *grip = unknown_bitmap_writer_a76378.Get_Image_0068ca40("trakgrip.pcx", 0);
        Rect grip_src;
        grip_src.X = 0;
        grip_src.Y = 0;
        grip_src.Width = grip->Get_Width();
        grip_src.Height = grip->Get_Height();
        unknown_surface_839958->Copy_From(grip_rect, *grip, grip_src, false, true);

        int disabled = style & 0x8000000;
        if (disabled) {
            Unknown_005feb60(&grip_rect, unknown_surface_839958, 0, unknown_color_a763b8);
        }

        int frame_color = unknown_5fda40(unknown_color_a76158);
        if (disabled) {
            frame_color = unknown_5fda40(unknown_color_a737e0);
        }

        Rect frame_rect = disp_rect;
        frame_rect.Width = disp_rect.Width - number_width;
        unknown_5fdb20(unknown_surface_839958, frame_rect, 1, frame_color);

        Rect number_rect = disp_rect;
        int inset = (show_numbers != 0) + 1;
        number_rect.X = inset - number_width + disp_rect.Width + disp_rect.X;
        number_rect.Width = number_width - inset;
        unknown_5fdb20(unknown_surface_839958, number_rect, 1, frame_color);

        if (show_numbers) {
            wchar_t buffer[32];


            swprintf(buffer, L"%d", step * ((minimum + value) / step));

            int text_color = unknown_color_a733e4;
            if (disabled) {
                text_color = unknown_color_a737ec;
            }

            RECT text_rect = paint_rect;
            text_rect.left = paint_rect.right - 49;
            unknown_draw_text_5fe020(unknown_surface_839958, buffer, &text_rect, data->font_64, text_color, 5, 0xc, 0, 0, 0);
        }

        ValidateRect(window, 0);
        break;
    }

    case 0x14:
        return 0;

    case 0x200:
        if (dragging) {
            RECT rect = client_rect;
            InvalidateRect(window, &rect, FALSE);
        }
        if (wparam & 1) {
            break;
        }

    case 0x202:
        message_result = 0;
        dragging = 0;
        ReleaseCapture();
        break;

    case 0x201:
    case 0x203: {
        if (message == 0x201) {
            message_result = 1;
            SetCapture(window);
        } else {
            message_result = 0;
            ReleaseCapture();
        }

        int xpos = (unsigned short)lparam;
        int ypos = (unsigned short)((unsigned int)lparam >> 16);
        if (ypos > client_rect.bottom - 18) {
            if (xpos >= grip_left && xpos < grip_right) {
                if (message == 0x201) {
                    dragging = 1;
                }
            } else {
                int x = xpos - 6;
                if (x < 1) {
                    x = 1;
                }
                if (client_rect.right - number_width - 12 < x) {
                    x = client_rect.right - number_width - 12;
                }


                int idx = ((range + 1) * (x - 1)) / slider_width;
                if (idx >= range) {
                    idx = range;
                }

                value = step * ((minimum + idx) / step) - minimum;
                thumb_pos = value * slider_width / range;
            }
        }
        break;
    }

    case 0x400:
        return step * ((minimum + value) / step);

    case 0x405:
        if (lparam - minimum <= range && lparam - minimum >= 0) {
            value = lparam - minimum;
        }
        play_click = false;
        thumb_pos = value * slider_width / range;
        break;

    case 0x406:
        minimum = (unsigned short)lparam;
        maximum = (unsigned short)((unsigned int)lparam >> 16);
        range = maximum - minimum;
        if (value > range) {
            value = range;
        }
        if (value < minimum) {
            value = minimum;
        }
        play_click = false;
        thumb_pos = value * slider_width / range;
        break;

    case 0x4ab:
        step = lparam;
        break;

    case 0x4ac:
        show_numbers = lparam;
        break;

    case 0x4ae:
        track->click_suppress = (wparam == 0);
        break;
    }

    int changed = 0;
    if (value != track->value || range != track->range || minimum != track->minimum) {
        changed = 1;
    }

    track->result = message_result;
    track->dragging = dragging;
    track->thumb_pos = thumb_pos;
    track->range = range;
    track->value = value;
    track->minimum = minimum;
    track->step = step;
    track->show_numbers = show_numbers;

    if (changed) {
        InvalidateRect(window, 0, FALSE);

        HWND parent = GetParent(window);
        SendMessageA(parent, 0x114, (WPARAM)((unsigned short)5 | (unsigned long)(unsigned short)(value + minimum) << 16), (LPARAM)window);

        if (play_click == true && !track->click_suppress) {
            Voc_Play(Rules->unknown_62c, 1.0f, 0x2000, 0);
        }
    }

    return 0;
}

}


#include "shared/vector_class.hpp"
#include "shared/queue.hpp"
#include "shared/keyboard.hpp"
#include "shared/lobby.hpp"
#include "shared/dialog_default_proc.hpp"
#include "shared/win_stub.hpp"
#include "shared/dialog_create.hpp"


namespace OwnerDraw {

// The group box window procedure: WM_PAINT prints the caption and draws the frame around it; everything
// else goes to the original procedure (0x005FC020).
LRESULT __stdcall unknown_group_box_proc_5fc020(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case 0xf: {
        int gap = 0;
        WinData *record = Get_Record(window);
        if (!record) {
            return 0;
        }
        wchar_t const *text = record->text_28;
        if (!text) {
            text = L"";
        }
        RECT area;
        Window_Rect_In_Main_Client(window, &area);
        int width = area.right - area.left;
        int middle = record->font_64->Height / 2 + area.top;
        int text_width;
        int text_height;
        record->font_64->Get_Text_Area(text, &text_width, &text_height, width);
        Rect caption(area.left + 10, area.top, width, area.bottom - area.top);
        unknown_text_draw_005fe1b0(unknown_color_a733e4, 0, caption, text, wcslen(text), 0, 0, unknown_surface_839958, 0);
        int color = unknown_5fda40(unknown_color_a76158);
        unknown_surface_839958->Draw_Line(Point2D(area.left, middle), Point2D(area.left + 8, middle), color);
        if (text_width) {
            gap = 12;
        }
        unknown_surface_839958->Draw_Line(Point2D(area.left + text_width + gap, middle), Point2D(area.right, middle), color);
        unknown_surface_839958->Draw_Line(Point2D(area.left, middle), Point2D(area.left, area.bottom), color);
        unknown_surface_839958->Draw_Line(Point2D(area.left, area.bottom), Point2D(area.right, area.bottom), color);
        unknown_surface_839958->Draw_Line(Point2D(area.right, middle), Point2D(area.right, area.bottom), color);
        ValidateRect(window, 0);
        return 0;
    }
    case 0x14:
        return 1;
    case 0x85:
        return 0;
    }
    WNDPROC proc = 0;
    unknown_window_proc_table_a73688.Get_Value(window, proc);
    return CallWindowProcA(proc, window, message, wparam, lparam);
}

}


namespace OwnerDraw {

// The msctls_hotkey32 window procedure: WM_PAINT draws the frame and the hot key's text over the saved
// background; everything else goes to the original procedure (0x005FC330).
LRESULT __stdcall unknown_hotkey_proc_5fc330(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    switch (message) {
    case 0xf: {
        RECT area;
        Window_Rect_In_Main_Client(window, &area);
        Rect screen(area.left, area.top, area.right - area.left + 1, area.bottom - area.top + 1);
        Rect local(0, 0, screen.Width, screen.Height);
        WinData *record = 0;
        unknown_control_table_a73640.Get_Pointer((unsigned int)window, &record);
        if (!record) {
            return 0;
        }
        if (!record->surface_10) {
            Surface *surface = new BSurface(screen.Width, screen.Height, 2);
            record->surface_10 = surface;
            unknown_surface_count_a763d8++;
            surface->Copy_From(local, *unknown_surface_839958, screen, false, true);
        }
        wchar_t text[64];
        unsigned short hotkey = (unsigned short)SendMessageA(window, 0x402, 0, 0);
        unknown_hotkey_text_5fc600(hotkey, text);
        if (record->surface_10) {
            unknown_surface_839958->Copy_From(screen, *record->surface_10, local, false, true);
        }
        unknown_5fdb20(unknown_surface_839958, screen, 1, -1);
        if (wcslen(text) > 0) {
            area.left += 4;
            area.top += 4;
            area.right -= 4;
            area.bottom -= 4;
            unknown_draw_text_5fdf40(unknown_surface_839958, text, &area, record->font_64, unknown_color_a733e4, 0x24);
        }
        ValidateRect(window, 0);
        return 0;
    }
    case 0x14:
        return 1;
    case 0x85:
        return 0;
    }
    WNDPROC proc = 0;
    unknown_window_proc_table_a73688.Get_Value(window, proc);
    return CallWindowProcA(proc, window, message, wparam, lparam);
}

}


namespace OwnerDraw {

// The text of a hot key control's value ("Alt+Ctrl+Shift+Key"; nothing for an extended key) (0x005FC600).
void __fastcall unknown_hotkey_text_5fc600(unsigned short hotkey, wchar_t *text)
{
    char name[32];
    char buffer[500];
    unsigned char modifiers = (unsigned char)(hotkey >> 8);
    buffer[0] = 0;
    *text = 0;
    if (modifiers & 8) {
        return;
    }
    if (modifiers & 4) {
        LONG key = MapVirtualKeyA(0x12, 0) << 16 | 0x2000001;
        GetKeyNameTextA(key, name, sizeof(name));
        strcat(buffer, name);
        strcat(buffer, "+");
    }
    if (modifiers & 2) {
        LONG key = MapVirtualKeyA(0x11, 0) << 16 | 0x2000001;
        GetKeyNameTextA(key, name, sizeof(name));
        strcat(buffer, name);
        strcat(buffer, "+");
    }
    if (modifiers & 1) {
        LONG key = MapVirtualKeyA(0x10, 0) << 16 | 0x2000001;
        GetKeyNameTextA(key, name, sizeof(name));
        strcat(buffer, name);
        strcat(buffer, "+");
    }
    LONG key = MapVirtualKeyA(hotkey & 0xff, 0) << 16 | 0x2000001;
    GetKeyNameTextA(key, name, sizeof(name));
    strcat(buffer, name);
    swprintf(text, L"%hs", buffer);
}

}


namespace OwnerDraw {

unsigned short unknown_red_green_masks_a763dc[2];
unsigned short unknown_blue_mask_a763e0 = 0;

// Builds the screen pixel format's channel masks from the hicolor shifts (0x005FC820).
void unknown_5fc820()
{
    unknown_red_green_masks_a763dc[0] = 0xff;
    unknown_red_green_masks_a763dc[0] >>= Get_Red_Right();
    unknown_red_green_masks_a763dc[0] <<= Get_Red_Left();
    unknown_red_green_masks_a763dc[1] = 0xff;
    unknown_red_green_masks_a763dc[1] >>= Get_Green_Right();
    unknown_red_green_masks_a763dc[1] <<= Get_Green_Left();
    unknown_blue_mask_a763e0 = 0xff;
    unknown_blue_mask_a763e0 >>= Get_Blue_Right();
    unknown_blue_mask_a763e0 <<= Get_Blue_Left();
}

}


template<class T>
inline void swap(T &left, T &right)
{
    T temp = left;
    left = right;
    right = temp;
}

namespace OwnerDraw {

// Draws a line from start to end over surface, blending color into each pixel by alpha (a plain colour fill
// on an 8-bit surface).
bool __fastcall unknown_draw_edge_glow_5fd290(Surface *surface, Point2D const &start, Point2D const &end, int color,
                                              int alpha)
{
    Point2D startpoint = start;
    Point2D endpoint = end;
    if (startpoint.X > endpoint.X) {
        swap(startpoint, endpoint);
    }
    int bpp = surface->Bytes_Per_Pixel();
    void *buffer = surface->Lock(startpoint);
    if (buffer != 0) {
        if (startpoint.Y == endpoint.Y) {
            if (bpp == 1) {
                memset(buffer, color, endpoint.X - startpoint.X + 1);
            } else {
                for (int i = 0; i <= endpoint.X - startpoint.X; i++) {
                    *((unsigned short *)buffer + i) = Blend_Color(*((unsigned short *)buffer + i), color, alpha);
                }
            }
        } else if (startpoint.X == endpoint.X) {
            int pitch = startpoint.Y > endpoint.Y ? -surface->Stride() : surface->Stride();
            int dy = abs(endpoint.Y - startpoint.Y);
            for (int i = 0; i <= dy; i++) {
                if (bpp == 1) {
                    *(unsigned char *)buffer = color;
                } else {
                    *(unsigned short *)buffer = Blend_Color(*(unsigned short *)buffer, color, alpha);
                }
                buffer = (unsigned char *)buffer + pitch;
            }
        } else {
            int dx = endpoint.X - startpoint.X;
            int dy = endpoint.Y - startpoint.Y;
            int pitch = surface->Stride();
            if (dy < 0) {
                pitch = -pitch;
            }
            dy = abs(dy);
            int dx2 = 2 * dx;
            int dy2 = 2 * dy;
            if (dx > dy) {
                int delta = dy2 - dx;
                for (int i = 0; i < dx; i++) {
                    if (bpp == 1) {
                        *((unsigned char *)buffer + i) = color;
                    } else {
                        *((unsigned short *)buffer + i) = Blend_Color(*((unsigned short *)buffer + i), color, alpha);
                    }
                    if (delta > 0) {
                        buffer = (unsigned char *)buffer + pitch;
                        delta -= dx2;
                    }
                    delta += dy2;
                }
            } else {
                int delta = dx2 - dy;
                int k = 0;
                for (int i = 0; i < dy; i++) {
                    if (bpp == 1) {
                        *((unsigned char *)buffer + k) = color;
                    } else {
                        *((unsigned short *)buffer + k) = Blend_Color(*((unsigned short *)buffer + k), color, alpha);
                    }
                    if (delta > 0) {
                        k++;
                        delta -= dy2;
                    }
                    delta += dx2;
                    buffer = (unsigned char *)buffer + pitch;
                }
            }
        }
        surface->Unlock();
        return true;
    }
    return false;
}

}


namespace OwnerDraw {

// Draws an edge glow line from start to end over surface (0x005FD290, not yet recovered).
bool __fastcall unknown_draw_edge_glow_5fd290(Surface *surface, Point2D const &start, Point2D const &end, int color,
                                              int alpha);

// Draws count nested raised (or sunken) glow frames inward from rect, one blend strength per edge.
void __fastcall unknown_draw_edge_glows_5fd810(Surface *surface, Rect const &rect, int raised, int count,
                                               int left_alpha, int top_alpha, int right_alpha, int bottom_alpha)
{
    int light = 0xffff;
    int dark = 0;
    if (!raised) {
        dark = 0xffff;
        light = 0;
    }
    for (int i = 0; i < count; i++) {
        unknown_draw_edge_glow_5fd290(surface, Point2D(rect.X + i, rect.Y + i), Point2D(rect.X + rect.Width - 2 - i, rect.Y + i), light, top_alpha);
        unknown_draw_edge_glow_5fd290(surface, Point2D(rect.X + i, rect.Y + rect.Height - 1 - i),
                                      Point2D(rect.X + rect.Width - 1 - i, rect.Y + rect.Height - 1 - i), dark, bottom_alpha);
        unknown_draw_edge_glow_5fd290(surface, Point2D(rect.X + i, rect.Y + i + 1), Point2D(rect.X + i, rect.Y + rect.Height - 1 - i), light,
                                      left_alpha);
        unknown_draw_edge_glow_5fd290(surface, Point2D(rect.X + rect.Width - 1 - i, rect.Y + i),
                                      Point2D(rect.X + rect.Width - 1 - i, rect.Y + rect.Height - 2 - i), dark, right_alpha);
    }
}

}


namespace OwnerDraw {

// Draws the up or down scroll arrow image (released or pressed; "g"-prefixed art when prefixed) at its own
// size from the top left corner of rect.
void __fastcall unknown_draw_arrow_5fd960(Surface *surface, Rect const &rect, int upward, int pressed, bool prefixed)
{
    char name[32];
    char state = 'r';
    if (pressed) {
        state = 'p';
    }
    if (upward) {
        sprintf(name, "guparrow%c.pcx", state);
    } else {
        sprintf(name, "gdnarrow%c.pcx", state);
    }
    Surface *arrow = unknown_bitmap_writer_a76378.Get_Image_0068ca40(name + !prefixed, 0);
    Rect destination = rect;
    destination.Width = arrow->Get_Width();
    destination.Height = arrow->Get_Height();
    Rect source;
    source.Y = 0;
    source.X = 0;
    source.Width = arrow->Get_Width();
    source.Height = arrow->Get_Height();
    surface->Copy_From(destination, *arrow, source, false, true);
}

}


namespace OwnerDraw {

// A Win32 colour (0x00BBGGRR) as a screen pixel; -1 stays -1.
int __fastcall unknown_5fda40(unsigned long color)
{
    if (color == 0xffffffff) {
        return -1;
    }
    // Keep the green channel as an unsigned bitfield, matching the constant-color
    // progress-bar caller without materializing the high byte in other callers.
    union ColorChannels {
        struct { unsigned int red : 8; unsigned int green : 8; unsigned int blue : 8; unsigned int alpha : 8; } channels;
        int value;
    } packed;
    packed.value = color;
    return Build_Hicolor_Pixel(color & 0xff, packed.channels.green, (color >> 16) & 0xff);
}

}


namespace OwnerDraw {

// A screen pixel as a palette-relative Win32 colour (PALETTERGB: 0x02BBGGRR).
unsigned long __fastcall unknown_5fdaa0(unsigned short pixel)
{
    RGBClass color = Unpack_Hicolor_Pixel(pixel);
    return 0x02000000 | ((unsigned long)color.Blue << 16) | ((unsigned short)color.Green << 8) | color.Red;
}

}


namespace OwnerDraw {

// Draws a frame thickness pixels wide around rect, in the owner-draw frame colour by default.
void __fastcall unknown_5fdb20(Surface *surface, Rect const &rect, int thickness, int color)
{
    if (color == -1) {
        color = unknown_5fda40(unknown_color_a76158);
    }
    Rect frame = rect;
    frame.X -= thickness;
    frame.Y -= thickness;
    frame.Width += thickness * 2;
    frame.Height += thickness * 2;
    for (int i = 0; i < thickness; i++) {
        surface->Draw_Line(Point2D(frame.X + i, frame.Y + i), Point2D(frame.X + frame.Width - 2 - i, frame.Y + i), color);
        surface->Draw_Line(Point2D(frame.X + i, frame.Y + frame.Height - 1 - i),
                           Point2D(frame.X + frame.Width - 1 - i, frame.Y + frame.Height - 1 - i), color);
        surface->Draw_Line(Point2D(frame.X + i, frame.Y + i + 1), Point2D(frame.X + i, frame.Y + frame.Height - 1 - i), color);
        surface->Draw_Line(Point2D(frame.X + frame.Width - 1 - i, frame.Y + i),
                           Point2D(frame.X + frame.Width - 1 - i, frame.Y + frame.Height - 2 - i), color);
    }
}

}


namespace OwnerDraw {

// Draws a bevelled button frame thickness pixels wide around rect, one green shade per edge.
void __fastcall unknown_draw_button_frame_5fdcb0(Surface *surface, Rect const &rect, int thickness)
{
    Rect frame = rect;
    int top_color = Build_Hicolor_Pixel(0x27, 0xf8, 0x74);
    int bottom_color = Build_Hicolor_Pixel(0x13, 0x7b, 0x39);
    int left_color = Build_Hicolor_Pixel(0x1e, 0xba, 0x57);
    int right_color = Build_Hicolor_Pixel(0x18, 0x99, 0x47);
    frame.X += -1 - thickness;
    frame.Y += -1 - thickness;
    frame.Width += 2 * thickness + 2;
    frame.Height += 2 * thickness + 2;
    for (int i = 0; i < thickness; i++) {
        surface->Draw_Line(Point2D(frame.X + i, frame.Y + i), Point2D(frame.X + frame.Width - 2 - i, frame.Y + i), top_color);
        surface->Draw_Line(Point2D(frame.X + i, frame.Y + frame.Height - 1 - i),
                           Point2D(frame.X + frame.Width - 1 - i, frame.Y + frame.Height - 1 - i), bottom_color);
        surface->Draw_Line(Point2D(frame.X + i, frame.Y + i + 1), Point2D(frame.X + i, frame.Y + frame.Height - 1 - i), left_color);
        surface->Draw_Line(Point2D(frame.X + frame.Width - 1 - i, frame.Y + i),
                           Point2D(frame.X + frame.Width - 1 - i, frame.Y + frame.Height - 2 - i), right_color);
    }
}

}


namespace OwnerDraw {
int __fastcall unknown_draw_text_5fdf40(Surface *surface, wchar_t const *text, RECT *area,
    BitFontClass *font, unsigned long color, int flags)
{
    unsigned int pixel = (((unsigned char)color >> (unsigned char)unknown_shift_852fbc) << unknown_shift_852fb8)
        | (((unsigned char)(color >> 16) >> (unsigned char)unknown_shift_852fc4) << unknown_shift_852fc0)
        | (((unsigned char)((unsigned short)color >> 8) >> (unsigned char)unknown_shift_852fcc) << unknown_shift_852fc8);
    int x = area->left;
    int y = area->top;
    int width = area->right - x;
    int height = area->bottom - y;
    if (font == 0) {
        font = BitFont;
    }
    font->Set_Clipping(1);
    font->Set_Clip_Box((tagRECT const *)area);
    font->Set_Color((unsigned short)pixel);
    BitText->Print_Box(font, surface, text, x, y, width, height, 0, 0, 0);
    return 0;
}
}


namespace OwnerDraw {
int __fastcall unknown_draw_text_5fe020(Surface *surface, wchar_t const *text, RECT *area, BitFontClass *font,
    unsigned long color, int flags, int unknown7, int unknown8, int arg9, int arg10)
{
    unsigned int pixel = (((unsigned char)color >> (unsigned char)unknown_shift_852fbc) << unknown_shift_852fb8)
        | (((unsigned char)(color >> 16) >> (unsigned char)unknown_shift_852fc4) << unknown_shift_852fc0)
        | (((unsigned char)((unsigned short)color >> 8) >> (unsigned char)unknown_shift_852fcc) << unknown_shift_852fc8);
    int x=area->left;
    int y=area->top;
    int width=area->right-x;
    int height=area->bottom-y;
    if(flags & 4) {
        int text_width,text_height;
        font->Get_Text_Area(text,&text_width,&text_height,width);
        y+=(height-text_height)/2;
    }
    font->Set_Clipping(1);
    font->Set_Clip_Box((tagRECT const *)area);
    font->Set_Color((unsigned short)pixel);
    BitText->Print_Box(font,surface,text,x,y,width,height,flags,arg9,arg10);
    return 0;
}
}


namespace OwnerDraw {

// A brightness factor for a palette index: 1.0, or 0.4..1.0 within 16 of 0x2B, 0x80 or 0xD5 (0x005FE130).
float __fastcall unknown_5fe130(int value)
{
    int centers[3] = { 0x2b, 0x80, 0xd5 };
    float factor = 1.0f;
    for (int i = 0; i < 3; i++) {
        int center = centers[i];
        if (value > center - 16 && value <= center) {
            factor = 0.6f * ((center - value) * 0.0625f) + 0.4f;
        } else if (value > center && value <= center + 16) {
            factor = 0.6f * ((value - center) * 0.0625f) + 0.4f;
        }
    }
    return factor;
}

}


int __fastcall unknown_text_draw_005fe1b0(unsigned int color, BitFontClass *font, Rect const &area, wchar_t const *text,
    int length, int x_alignment, int y_alignment, Surface *surface, int unknown9)
{
    if (!unknown_flag_a40d28) return 0;
    if (!surface) surface = unknown_surface_839958;
    unsigned int pixel = (((unsigned char)color >> (unsigned char)unknown_shift_852fbc) << unknown_shift_852fb8)
        | (((unsigned char)((unsigned short)color >> 8) >> (unsigned char)unknown_shift_852fcc) << unknown_shift_852fc8)
        | (((unsigned char)(color >> 16) >> (unsigned char)unknown_shift_852fc4) << unknown_shift_852fc0);
    int x = area.X;
    int y = area.Y;
    int width = area.Width;
    int height = area.Height;
    if (!font) font = BitFont;
    int text_width, text_height;
    font->Get_Text_Area(text, &text_width, &text_height, width);
    if (x_alignment == 1) x += (width - text_width + 1) / 2;
    else if (x_alignment == 2) x -= (text_width + 1) / 2;
    else if (x_alignment == 3) x += -1 - text_width;
    else if (x_alignment == 4) x += width - text_width - 1;
    if (y_alignment == 1) y += (height - text_height + 1) / 2;
    else if (y_alignment == 2) y -= (text_height + 1) / 2;
    else if (y_alignment == 3) y += -1 - text_height;
    tagRECT clip;
    clip.left = area.X;
    clip.top = area.Y;
    clip.right = clip.left + width;
    clip.bottom = clip.top + height;
    font->Set_Clipping(1);
    font->Set_Clip_Box(&clip);
    font->Set_Color((unsigned short)pixel);
    int end = BitText->Print(font, surface, text, x, y, length, unknown9);
    return end - x;
}


// WM_DRAWITEM (item is the DRAWITEMSTRUCT): while the movie surfaces exist, a type 4 control's
// registry record takes the item state and the item rectangle is repainted (0x005FE380).
void __fastcall Unknown_005FE380(long item)
{
    DRAWITEMSTRUCT *draw = (DRAWITEMSTRUCT *)item;
    if (unknown_surface_839950 && unknown_surface_839958) {
        RECT area;
        RECT client;
        Window_Rect_In_Main_Client(draw->hwndItem, &area);
        GetClientRect(draw->hwndItem, &client);
        if (GetWindowLongA(draw->hwndItem, GWL_STYLE) & 0x800000) {
            int border_x = GetSystemMetrics(5);
            int border_y = GetSystemMetrics(6);
            area.left += border_x;
            area.right -= border_x;
            area.top += border_y;
            area.bottom -= border_y;
        }
        if (draw->CtlType == 4) {
            HWND window = draw->hwndItem;
            OwnerDraw::WinData *record = 0;
            unknown_control_table_a73640.Get_Pointer((unsigned int)window, &record);
            if (record) {
                record->unknown_e8 = draw->itemState;
                InvalidateRect(window, &draw->rcItem, FALSE);
                UpdateWindow(window);
            }
        }
    }
}


namespace OwnerDraw {

// Draws area of the background surface from window's cached copy of what lies under it, dimmed by alpha;
// the copy is taken (from the parent's cached copy or the screen) and dimmed on first use.
void __fastcall unknown_draw_dimmed_back_5fe490(Rect const &area, HWND window, int alpha)
{
    WinData *record = 0;
    WinData *parent_record = 0;
    unknown_control_table_a73640.Get_Pointer(*(unsigned int *)&window, &record);
    RECT client;
    GetClientRect(window, &client);
    RECT screen;
    Window_Rect_In_Main_Client(window, &screen);
    Surface *surface = record->surface_10;
    if (surface == 0) {
        surface = new BSurface(client.right + 1, client.bottom + 1, 2);
        record->surface_10 = surface;
        unknown_surface_count_a763d8++;
        Rect full(0, 0, client.right + 1, client.bottom + 1);
        HWND parent = GetParent(window);
        unknown_control_table_a73640.Get_Pointer(*(unsigned int *)&parent, &parent_record);
        Surface *parent_surface = parent_record->surface_10;
        if (parent_surface) {
            RECT parent_screen;
            Window_Rect_In_Main_Client(parent, &parent_screen);
            Rect from;
            from.X = screen.left - parent_screen.left;
            from.Y = screen.top - parent_screen.top;
            from.Width = screen.right - screen.left + 1;
            from.Height = screen.bottom - screen.top + 1;
            surface->Copy_From(full, *parent_surface, from, false, true);
        } else {
            Surface *source = unknown_surface_839950;
            Rect from;
            from.X = screen.left;
            from.Y = screen.top;
            from.Width = screen.right - screen.left + 1;
            from.Height = screen.bottom - screen.top + 1;
            if (source) {
                surface->Copy_From(full, *source, from, false, true);
            }
        }
        unsigned short *pixels = (unsigned short *)surface->Lock();
        if (pixels) {
            for (int i = 0; i < surface->Get_Width() * surface->Get_Height(); i++) {
                pixels[i] = Blend_Color(pixels[i], 0, alpha);
            }
            surface->Unlock();
        }
    }
    Rect from;
    from.X = area.X;
    from.Y = area.Y;
    from.Width = area.Width;
    from.Height = area.Height;
    from.X -= screen.left;
    from.Y -= screen.top;
    unknown_surface_839958->Copy_From(area, *surface, from, false, true);
}

}


namespace OwnerDraw {

// Draws a progress bar's fill of fraction (16.16) over area of surface: solid in the middle half of the rows,
// fading in from the left in the top and bottom quarters.
void __fastcall unknown_5fe7c0(Rect const &area, Surface *surface, int color, int fraction)
{
    int fade = 1;
    int run = (area.Width * fraction) >> 16;
    if (run < 0) return;
    if (run == 0) run = 1;
    unsigned short *pixels = (unsigned short *)surface->Lock();
    if (!pixels) return;
    for (int row = 0; row < area.Height; row++) {
        int index = area.X + (area.Y + row) * (surface->Stride() / 2);
        int quarter = area.Height / 4;
        if (row == quarter * 3) {
            fade = 1;
        }
        if (row == quarter) {
            fade = 0;
        }
        if (run > 0) {
            unsigned short *pixel = &pixels[index];
            for (int i = 0; i < run; i++) {
                if (fade) {
                    int alpha = (255 * (i + 1)) / area.Width;
                    *pixel = Blend_Color(*pixel, color, alpha);
                } else {
                    *pixel = color;
                }
                pixel++;
            }
        }
    }
    surface->Unlock();
}

}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Darkens the left_width band and the top_height band of area on a locked 16-bit surface to half (0x005FE9B0).
// Adapted from ra2-decomp src/ownrdraw.cpp FUN_006219D0 (gamemd twin 0x006219D0, matched there); see THIRD_PARTY.md.
namespace OwnerDraw {
void __fastcall unknown_5fe9b0(Rect const &area, Surface &surface, int left_width, int top_height)
{
    unsigned short *pixels = (unsigned short *)surface.Lock();

    if (pixels != 0) {
        int stride = surface.Stride() / 2;
        int y;

        for (y = area.Y; y < area.Y + area.Height; ++y) {
            int index = y * stride + area.X;

            for (int x = area.X; x < area.X + left_width; ++x) {
                unsigned short pixel = pixels[index];

                pixels[index] = ((pixel >> 1) & ((unknown_red_green_masks_a763dc[0] >> 1) & (unsigned int)unknown_red_green_masks_a763dc[0])) |
                                ((pixel >> 1) & ((unknown_red_green_masks_a763dc[1] >> 1) & unknown_red_green_masks_a763dc[1])) |
                                ((pixel >> 1) & (unknown_blue_mask_a763e0 >> 1));

                index++;
            }
        }

        for (y = area.Y; y < area.Y + top_height; ++y) {
            int index = y * stride + area.X + left_width;

            for (int x = area.X + left_width; x < area.Width + area.X; ++x) {
                unsigned short pixel = pixels[index];

                pixels[index] = ((pixel >> 1) & ((unknown_red_green_masks_a763dc[0] >> 1) & (unsigned int)unknown_red_green_masks_a763dc[0])) |
                                ((pixel >> 1) & ((unknown_red_green_masks_a763dc[1] >> 1) & unknown_red_green_masks_a763dc[1])) |
                                ((pixel >> 1) & (unknown_blue_mask_a763e0 >> 1));

                index++;
            }
        }

        surface.Unlock();
    }
}
}


// Fill a locked surface rectangle through the owner-draw alpha blend, 005FEB60.
void __fastcall Unknown_005feb60(Rect *area, Surface *surface, int color, int alpha)
{
    unsigned short *pixels = (unsigned short *)surface->Lock();
    if (pixels) {
        int stride = surface->Stride() / 2;
        for (int y = area->Y; y < area->Y + area->Height; ++y) {
            int index = y * stride + area->X;
            for (int x = area->X; x < area->X + area->Width; ++x) {
                pixels[index] = OwnerDraw::Blend_Color(pixels[index], color, alpha);
                ++index;
            }
        }
        surface->Unlock();
    }
}


namespace OwnerDraw {

// Draws a row of random bits_i.pcx frames (10x12, through bits_a.pcx, brightened palette) along the top
// and bottom edges of area, with a 10-pixel gap after every sixth (0x005FED10).
void __fastcall unknown_draw_bits_5fed10(Rect const *area, Surface *surface)
{
    unsigned char palette[768];
    Surface *image = unknown_bitmap_writer_a76378.Get_Image_0068ca40("bits_i.pcx", (int)palette);
    Surface *alpha = unknown_bitmap_writer_a76378.Get_Image_0068ca40("bits_a.pcx", 0);
    int count = 0;
    unknown_brighten_palette_68bce0(palette);
    int x = 0;
    Rect rect = *area;
    if (x < area->Width) {
        do {
            int frame = rand() % 8;
            rect.X = area->X + x;
            rect.Y = area->Y + 1;
            rect.Width = 10;
            rect.Height = 12;
            unknown_bitmap_writer_a76378.Draw_0068cf90(rect, surface, image, alpha, palette, 0, frame * 10, 0);
            frame = rand() % 8;
            rect.X = area->X + x;
            rect.Width = 10;
            rect.Height = 12;
            rect.Y = area->Y + area->Height - 16;
            unknown_bitmap_writer_a76378.Draw_0068cf90(rect, surface, image, alpha, palette, 0, frame * 10, 0);
            count++;
            if (count % 6 == 0) {
                x += 10;
            }
            x += 10;
        } while (x < area->Width);
    }
}

}


bool __fastcall Unknown_Get_Art_006f3460();
void __fastcall Unknown_006F3650(Surface *surface, Rect const &window, bool arg3);
void __fastcall Unknown_006F3930(Surface *surface, Rect const &window, ConvertClass *drawer, ShapeFileStruct const *shape640,
                                 ShapeFileStruct const *shape);
void __fastcall Unknown_6F3A60(Surface *surface, Rect const &window);
void __fastcall Unknown_6F3AC0(Surface *surface, Rect const &window);
void __fastcall Unknown_6F3B20(Surface *surface, Rect const &window);
void __fastcall Unknown_6F4620(Surface *surface, Rect const &window, bool arg3);
void *Unknown_Get_Art_6f2230();
extern void *unknown_art_ac1218;
extern void *unknown_art_ac121c;
extern void *unknown_art_ac1220;

namespace OwnerDraw {

// Draws a dialog's background (TibSun OwnerDraw::Draw_Dialog_Back): built once into the record's cached
// surface (interface art, the multiplayer art shape, or the dbak6440.pcx image), then blitted to the
// background surface.
extern char const *unknown_dialog_back_name_7e8d1c;
// One more inline level over Get: at this depth Bucket_Index stays out of line (0x00602020).
static inline void Lookup(unsigned int const &key, WinData *&record)
{
    unknown_control_table_a73640.Get_Pointer(key, &record);
}
void __fastcall unknown_draw_dialog_back_5fee70(HWND window)
{
    ConvertClass *drawer = 0;
    unsigned int key = (unsigned int)window;
    WinData *record = 0;
    Lookup(key, record);
    WinData *const &found = record;
    if (found) {
        RECT client;
        GetClientRect(window, &client);
        RECT screen;
        Window_Rect_In_Main_Client(window, &screen);
        Rect full;
        full.Set(screen.left, screen.top, screen.right - screen.left, screen.bottom - screen.top);
        if (full.Width <= client.right) {
            full.Width = client.right;
        }
        if (full.Height <= client.bottom) {
            full.Height = client.bottom;
        }
        if (record->unknown_20 == 0) {
            Surface *surface = record->surface_10;
            if (!surface) {
                surface = new BSurface(client.right, client.bottom, 2);
                record->surface_10 = surface;
                unknown_surface_count_a763d8++;
            }
            if (record->unknown_b0 == 1) {
                if (unknown_session_a3d298.Unknown_00672F10()) {
                    Unknown_6F4620(surface, full, false);
                } else {
                    if (Unknown_Get_Art_006f3460()) {
                        Unknown_006F3650(surface, full, !record->unknown_d4);
                        drawer = (ConvertClass *)unknown_5e9f40(window);
                        ShapeFileStruct const *shape640 = (ShapeFileStruct const *)unknown_5ea8a0(window);
                        ShapeFileStruct const *shape = (ShapeFileStruct const *)unknown_5ea900(window);
                        Unknown_006F3930(surface, full, drawer, shape640, shape);
                        if (record->unknown_d5) {
                            Unknown_6F3AC0(surface, full);
                        }
                        if (record->unknown_d6) {
                            Unknown_6F3A60(surface, full);
                        }
                        if (unknown_5ea6c0(window)) {
                            Unknown_6F3B20(surface, full);
                        }
                    }
                }
            } else if (record->unknown_b0 == 2) {
                Rect to = full;
                to.X = 0;
                to.Y = 0;
                surface->Copy_From(to, *unknown_surface_839958, full, false, true);
                void *shape;
                if (unknown_session_a3d298.Unknown_00672F10()) {
                    if (unknown_id_source_a3d290->unknown_2239) {
                        shape = unknown_art_ac121c;
                    } else {
                        shape = unknown_art_ac1220;
                    }
                } else {
                    shape = unknown_art_ac1218;
                }
                drawer = (ConvertClass *)Unknown_Get_Art_6f2230();
                CC_Draw_Shape(surface, drawer, (ShapeFileStruct const *)shape, 0,
                              Point2D(0, 0), to, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            } else {
                Surface *back = unknown_bitmap_writer_a76378.Get_Image_0068ca40(unknown_dialog_back_name_7e8d1c, 0);
                Rect to = full;
                Rect from = full;
                to.X -= screen.left;
                to.Y -= screen.top;
                int width = back->Get_Width();
                int height = back->Get_Height();
                if (unknown_8523e0 > width) {
                    from.X -= (unknown_8523e0 - width) / 2;
                }
                if (unknown_8523e4 > height) {
                    from.Y -= (unknown_8523e4 - height) / 2;
                }
                surface->Copy_From(to, *back, from, false, true);
            }
            if (surface) {
                Rect from = full;
                from.X -= screen.left;
                from.Y -= screen.top;
                unknown_surface_839958->Copy_From(full, *surface, from, false, true);
            }
        }
    }
}

}


namespace OwnerDraw {

// Deletes a registered control's surface (+0x10).
void __fastcall unknown_5ff380(unsigned int key)
{
    WinData *record = 0;
    unknown_control_table_a73640.Get_Pointer(key, &record);
    if (record && record->surface_10) {
        delete record->surface_10;
        record->surface_10 = 0;
        --unknown_surface_count_a763d8;
    }
}

}


namespace OwnerDraw {

// A growable array of windows (an ArrayList<HWND> shape: count, capacity, items; grows to twice the count,
// at least 10). Filled by the EnumChildWindows callback 0x005FF430.
class UnknownWindowList {
public:
    int Count;      // +0x00
    int Capacity;   // +0x04
    HWND *Items;    // +0x08
    char Grow()
    {
        if (Count < Capacity) return 0;
        int capacity = Count * 2;
        if (capacity < 10) capacity = 10;
        HWND *items = (HWND *)(new unsigned char[capacity * sizeof(HWND)]);
        memset(items, 0, capacity * sizeof(HWND));
        if (Items != 0) memcpy(items, Items, Count * sizeof(HWND));
        delete [] (unsigned char *)Items;
        Items = items;
        Capacity = capacity;
        return 1;
    }
    char Add(HWND &window, int index)
    {
        if (index > Count) index = Count;
        if (index >= Capacity) Grow();
        if (Count >= Capacity) Grow();
        if (index < Count) memmove(Items + index + 1, Items + index, sizeof(HWND) * (Count - index));
        new ((void *)(Items + index)) HWND((HWND &)window);
        Count++;
        return 1;
    }
    int Length() const { return Count; }
};

// Adds window to the end of list (an EnumChildWindows callback).
BOOL __stdcall unknown_add_window_5ff430(HWND window, UnknownWindowList *list)
{
    if (list != 0) {
        list->Add(window, list->Length());
    }
    return TRUE;
}

}


namespace OwnerDraw {

// Releases a captured mouse and counts one more release (0x005FF5A0).
int unknown_5ff5a0()
{
    if (WWMouse && WWMouse->Is_Captured() == true) {
        WWMouse->Release_Mouse();
    }
    return ++unknown_mouse_release_depth_a76410;
}

}


namespace OwnerDraw {

// Counts one release off and captures the mouse again once none is left (0x005FF5D0).
int unknown_5ff5d0()
{
    if (unknown_mouse_release_depth_a76410 > 0) {
        --unknown_mouse_release_depth_a76410;
    }
    if (unknown_mouse_release_depth_a76410 == 0 && WWMouse && !WWMouse->Is_Captured()) {
        WWMouse->Capture_Mouse();
    }
    return unknown_mouse_release_depth_a76410;
}

}


// Creates a modeless dialog from template resource with the main window as its parent and pushes it
// on the dialog stack as the current dialog; 0 when the template is missing or creation fails (0x005FF610).
HWND __fastcall unknown_create_dialog_5ff610(int resource, DialogProc proc, int unknown3)
{
    unsigned short id = (unsigned short)resource;
    DLGTEMPLATE const *dialog_template = (DLGTEMPLATE const *)Load_Resource_004954e0(id, (char const *)5);
    if (!dialog_template) {
        return 0;
    }
    int index = DialogStackCount;
    UnknownDialogCreateParam param;
    DialogStack[index].Window = 0;
    DialogStack[index].Resource = 0;
    DialogStackCount++;
    param.Resource = id;
    param.Param = unknown3;
    HWND window = CreateDialogIndirectParamA(ProgramInstance, dialog_template, MainWindow, (DLGPROC)proc, (LPARAM)&param);
    if (!window) {
        DialogStackCount--;
        return 0;
    }
    DialogStack[index].Window = window;
    DialogStack[index].Resource = id;
    Unknown_005B6000(window);
    CurrentDialog = window;
    CurrentDialogResource = id;
    return window;
}


// Clears the keyboard buffer, destroys a dialog and removes it from the dialog stack; the new top
// (or the main window) becomes the foreground window with the focus (0x005FF6E0).
void __fastcall unknown_dialog_5ff6e0(HWND window)
{
    WWKeyboard->Clear();
    OwnerDraw::unknown_5e5fc0(window);
    DestroyWindow(window);
    for (int index = 0; index < DialogStackCount; index++) {
        if (DialogStack[index].Window == window) {
            memmove(&DialogStack[index], &DialogStack[index + 1], (DialogStackCount - (index + 1)) * sizeof(DialogStackEntry));
            DialogStack[DialogStackCount - 1].Window = 0;
            DialogStack[DialogStackCount - 1].Resource = 0;
            DialogStackCount--;
            if (DialogStackCount > 0) {
                CurrentDialog = DialogStack[DialogStackCount - 1].Window;
                CurrentDialogResource = DialogStack[DialogStackCount - 1].Resource;
                SetForegroundWindow(CurrentDialog);
                SetFocus(CurrentDialog);
            } else {
                CurrentDialog = 0;
                CurrentDialogResource = 0;
                SetForegroundWindow(MainWindow);
                SetFocus(MainWindow);
            }
            return;
        }
    }
}


// Shows a window, brings it to the foreground and clears the keyboard buffer (0x005FF7C0).
void __fastcall unknown_dialog_5ff7c0(HWND window)
{
    ShowWindow(window, SW_SHOWNORMAL);
    SetForegroundWindow(window);
    WWKeyboard->Clear();
}


void __fastcall unknown_735ba0(HWND window);
void __fastcall unknown_737060(HWND window);

namespace OwnerDraw {

// EnumChildWindows callbacks and the dialog test 0x005FF7E0 runs over a dialog (not yet recovered).
BOOL __stdcall unknown_5ed180(HWND window, LPARAM lparam);
BOOL __stdcall unknown_5e8720(HWND window, LPARAM lparam);
BOOL __stdcall unknown_5e7eb0(HWND window, LPARAM lparam);
BOOL __stdcall unknown_5e8130(HWND window, LPARAM lparam);
bool __fastcall unknown_5e9fa0(HWND dialog);

// Prepares a dialog's controls for owner drawing: art by the control kind, the listed setters, the movie
// frame size, and the per-control callbacks.
inline void unknown_prepare_dialog(HWND dialog, int kind)
{
    unknown_735ba0(dialog);
    unknown_5eaaa0(dialog, kind);
    if (!unknown_session_a3d298.Unknown_00672F10()) {
        unknown_set_control_art_5ea960((unsigned int)dialog);
    }
    unknown_5ea550(dialog);
    unknown_5ea390(dialog);
    unknown_5ea720(dialog);
    unknown_5ea810(dialog);
    EnumChildWindows(dialog, unknown_5e8720, 0);
    if (unknown_5e9fa0(dialog)) {
        Point2D size;
        size.X = unknown_screen_widths_7add68[0];
        size.Y = unknown_screen_heights_7add74[0];
        Unknown_005E9F00(dialog, &size);
    } else {
        unknown_5ea230(dialog);
    }
    EnumChildWindows(dialog, unknown_5e7eb0, 0);
    EnumChildWindows(dialog, unknown_5e8130, 0);
    unknown_737060(dialog);
}

}

// Subclasses a dialog's controls for owner drawing and prepares them (0x005FF7E0).
void __fastcall unknown_show_dialog_5ff7e0(void *handle, int kind)
{
    using namespace OwnerDraw;
    HWND dialog = (HWND)handle;
    unknown_5eac30();
    unknown_dialog_a763cc = dialog;
    EnumChildWindows(dialog, (WNDENUMPROC)unknown_dialog_5ecf40, 1);
    unknown_dialog_5ecf40(dialog, 1);
    EnumChildWindows(dialog, unknown_replace_edit_5ecb00, 1);
    EnumChildWindows(dialog, unknown_5ed180, 0);
    unknown_5ed180(dialog, 0);
    EnumChildWindows(dialog, (WNDENUMPROC)unknown_dialog_5ecf40, 0);
    unknown_dialog_5ecf40(dialog, 0);
    unknown_prepare_dialog(dialog, kind);
}


// Services the game for a modal dialog loop (0x006000D0).
bool unknown_6000d0()
{
    Unknown_005B5EE0();
    if (unknown_game_mode_a3d298 == 0 || unknown_game_mode_a3d298 == 5 || unknown_session_a3f616 ||
        unknown_counter_a3fabc != 0) {
        Service_Game_IO_00481fb0();
        return false;
    }
    if (!unknown_53f460() && unknown_53fbd0()) {
        return true;
    }
    return false;
}


void __fastcall unknown_move_window_600120(HWND window, int x, int y)
{
    RECT screen;
    RECT rect;
    screen.right = unknown_8523e0;
    screen.left = 0;
    screen.top = 0;
    screen.bottom = unknown_8523e4;
    ClientToScreen(MainWindow, (POINT *)&screen.left);
    ClientToScreen(MainWindow, (POINT *)&screen.right);
    GetWindowRect(window, &rect);
    rect.right -= rect.left;
    rect.bottom -= rect.top;
    rect.left = x == -1 ? rect.left - screen.left : x;
    rect.top = y == -1 ? rect.top - screen.top : y;
    MoveWindow(window, rect.left, rect.top, rect.right, rect.bottom, FALSE);
}


HWND __fastcall unknown_cancel_dialog_6001e0(unsigned short const *text, unsigned short const *button, bool *cancelled)
{
    HWND dialog = unknown_create_dialog_5ff610(0xf0, unknown_cancel_dialog_proc_600260, 0);
    if (dialog) {
        SetWindowLongA(dialog, DWL_USER, (LONG)cancelled);
        HWND label = GetDlgItem(dialog, 0x5b0);
        SendMessageA(label, 0x4b2, 0, (LPARAM)text);
        if (button) {
            HWND item = GetDlgItem(dialog, 0x5c0);
            SendMessageA(item, 0x4b2, 0, (LPARAM)button);
            EnableWindow(item, TRUE);
            ShowWindow(item, SW_SHOW);
        }
    }
    return dialog;
}


// int __stdcall unknown_cancel_dialog_proc_600260(HWND, UINT, WPARAM, LPARAM) (0x00600260)
// The cancel dialog's procedure: the default dialog handling first; its cancel button (0x5C0)
// sets the caller's flag (DWL_USER) and queues key 0x1B (Escape).

int __stdcall unknown_cancel_dialog_proc_600260(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result) {
        return result;
    }
    if (message == 0x111 && wparam == 0x5c0) {
        bool *cancelled = (bool *)GetWindowLongA(window, DWL_USER);
        if (cancelled) {
            WWKeyboard->Put((KeyNumType)0x1b);
            *cancelled = true;
        }
    }
    return 0;
}


namespace OwnerDraw {

WinData::WinData()
{
    memset(this, 0, sizeof(WinData));
    unknown_68 = 0xb;
    font_64 = BitFont;
    unknown_3c = new WideString;
    unknown_40 = 0;
    unknown_5c = 0;
    unknown_90 = -1;
}

}


// Copies the owner-draw record list; full original162code+14NOP independently verified.

namespace OwnerDraw {
inline void WinData::Copy_List_600460(UnknownNode_006003d0 const *source)
{
    while (list_34) {
        UnknownNode_006003d0 *node = list_34;
        UnknownNode_006003d0 *previous = 0;
        UnknownNode_006003d0 *current;
        for (current = list_34; current; current = current->Next) {
            if (node == current) {
                break;
            }
            previous = current;
        }
        if (current) {
            if (previous) {
                previous->Next = current->Next;
            } else {
                list_34 = current->Next;
            }
            operator delete(current);
        }
    }
    for (; source; source = source->Next) {
        UnknownNode_006003d0 *node = (UnknownNode_006003d0 *)operator new(wcslen(source->Data) * 2 + 0x12);
        if (node) {
            node->Data = node->Text;
            node->Next = list_34;
            node->unknown_04 = 0;
            list_34 = node;
        }
        node->unknown_0c = source->unknown_0c;
        node->unknown_04 = source->unknown_04;
        wcscpy(node->Data, source->Data);
    }
}
}


namespace OwnerDraw {

void WinData::Set_Text_600560(char const *text)
{
    if (text_28) {
        operator delete(text_28);
        text_28 = 0;
    }
    if (text && *text) {
        text_28 = (wchar_t *)operator new((strlen(text) + 1) * sizeof(wchar_t));
        swprintf(text_28, L"%hs", text);
    }
}

}


namespace OwnerDraw {

inline WinData &WinData::operator=(WinData const &other)
{
    UnknownNode_006003d0 *list = list_34;
    wchar_t *text = text_28;
    WideString *kept_3c = unknown_3c;
    if (this == &other) {
        return *this;
    }
    memcpy(this, &other, sizeof(WinData));
    text_28 = text;
    unknown_3c = kept_3c;
    list_34 = list;
    Set_Wide_Text_600510(other.text_28);
    Copy_List_600460(other.list_34);
    return *this;
}

}


#ifndef RA2_OWNERDRAW_CHECK_FIELDS
#define RA2_OWNERDRAW_CHECK_FIELDS
#endif
// Original257code+15NOP; record copy without a self-assignment test.

namespace OwnerDraw {

WinData &WinData::unknown_copy_6006e0(WinData const &other)
{
    UnknownNode_006003d0 *list = list_34;
    wchar_t *text = text_28;
    WideString *kept_3c = unknown_3c;
    memcpy(this, &other, sizeof(WinData));
    text_28 = text;
    unknown_3c = kept_3c;
    list_34 = list;
    wchar_t *source = other.text_28;
    if (text_28) {
        operator delete(text_28);
        text_28 = 0;
    }
    if (source && *source) {
        text_28 = (wchar_t *)operator new(wcslen(source) * 2 + 2);
        wcscpy(text_28, source);
    }
    {
        UnknownNode_006003d0 const *source_list=other.list_34;
    while (list_34) {
        UnknownNode_006003d0 *node = list_34;
        UnknownNode_006003d0 *previous = 0;
        UnknownNode_006003d0 *current;
        for (current = list_34; current; current = current->Next) {
            if (node == current) {
                break;
            }
            previous = current;
        }
        if (current) {
            if (previous) {
                previous->Next = current->Next;
            } else {
                list_34 = current->Next;
            }
            operator delete(current);
        }
    }
    for (; source_list; source_list = source_list->Next) {
        UnknownNode_006003d0 *node = (UnknownNode_006003d0 *)operator new(wcslen(source_list->Data) * 2 + 0x12);
        if (node) {
            node->Data = node->Text;
            node->Next = list_34;
            node->unknown_04 = 0;
            list_34 = node;
        }
        node->unknown_0c = source_list->unknown_0c;
        node->unknown_04 = source_list->unknown_04;
        wcscpy(node->Data, source_list->Data);
    }
    }
    return *this;
}

}


namespace OwnerDraw {

// Nonzero: do not draw the edit caret this time (cleared after every edit text draw; name hypothesis, 0x00A763EC).
extern int unknown_caret_hidden_a763ec;

// The larger of a and b (b when they are equal).
template<class T> inline T unknown_max(T a, T b) { return a < b ? b : a; }

// Draws an edit box's text in font at rect of surface, scrolled so the caret stays visible (*scroll is the
// first shown character). With ime the open IME composition is spliced in at the caret and drawn in the
// composition color; password shows '*' for every character; frame clears the text's background first.
// Each drawn run moves rect's left edge past it; the caret is drawn after the run that ends at it.
void __fastcall unknown_draw_edit_text_600830(Surface *surface, Rect &rect, wchar_t const *text, int caret,
                                              BitFontClass *font, unsigned int color, int *scroll, int ime,
                                              int password, int frame, int unknown9)
{
    wchar_t buffer[0x800];
    int caret_x = -1;
    int composition_length = 0;
    int composing = 0;
    if (text == 0) {
        text = L"";
    }
    int text_length = wcslen(text);
    if (caret < 0 || caret > text_length) {
        caret = 0;
    }
    if (ime) {
        IME_Update_Composition();
        composition_length = IMECompositionLength;
        composing = IMEComposing;
    }
    buffer[0] = 0;
    wcsncpy(buffer, text, 0x800);
    if (composition_length) {
        wcsncpy(&buffer[caret], IMECompositionString, 0x800 - caret);
        wcsncpy(&buffer[composition_length + caret], &text[caret], 0x800 - composition_length - caret);
    }
    buffer[0x7FF] = 0;
    int length = wcslen(buffer);
    if (password && length > 0) {
        wchar_t *p = buffer;
        for (int i = length; i != 0; i--) {
            *p++ = '*';
        }
    }
    int cursor;
    if (composing || composition_length) {
        cursor = IMECursorPosition + caret;
    } else {
        cursor = caret;
    }
    if (cursor < *scroll + 5) {
        if (cursor >= 5) {
            *scroll = cursor - 5;
        } else {
            *scroll = 0;
        }
    }
    while (*scroll < length) {
        if (font->Chars_In_Width(&buffer[*scroll], rect.Width - 5, 0, 0) >= cursor - *scroll) {
            break;
        }
        (*scroll)++;
    }
    int before = caret - *scroll;
    int lead = unknown_max(before, 0);
    int composed = composition_length;
    if (before < 0) {
        before += composition_length;
        composed = unknown_max(before, 0);
    }
    int after = length - composition_length - caret;
    if (before < 0) {
        after += before;
    }
    if (after < 0) {
        after = 0;
    }
    text = &buffer[*scroll];
    if (frame) {
        Rect area(rect.X - 1, rect.Y - 1, font->String_Pixel_Width(text, 0) + 5, rect.Height + 2);
        surface->Fill_Rect(area, 0);
    }
    if (lead) {
        int width = unknown_text_draw_005fe1b0(color, font, rect, text, lead, 0, 0, surface, unknown9);
        rect.X += width;
        rect.Width -= width;
        if (rect.Width < 0) {
            rect.Width = 0;
        }
        text += lead;
    }
    if (composed) {
        cursor -= *scroll + lead;
        int rest;
        if (cursor < 0 || cursor > composed) {
            rest = -1;
        } else {
            rest = composed - cursor;
            composed -= rest;
        }
        if (composed > 0) {
            int width = unknown_text_draw_005fe1b0(unknown_color_a76150, font, rect, text, composed, 0, 0, surface, unknown9);
            rect.X += width;
            rect.Width -= width;
            if (rect.Width < 0) {
                rect.Width = 0;
            }
            text += composed;
        }
        if (rest >= 0) {
            caret_x = rect.X;
            if (rest) {
                int width = unknown_text_draw_005fe1b0(unknown_color_a76150, font, rect, text, rest, 0, 0, surface, unknown9);
                rect.X += width;
                rect.Width -= width;
                if (rect.Width < 0) {
                    rect.Width = 0;
                }
                text += rest;
            }
        }
    } else {
        caret_x = rect.X;
    }
    if (after) {
        int width = unknown_text_draw_005fe1b0(color, font, rect, text, after, 0, 0, surface, unknown9);
        rect.X += width;
        rect.Width -= width;
        if (rect.Width < 0) {
            rect.Width = 0;
        }
    }
    if (ime && caret_x > 0 && !unknown_caret_hidden_a763ec) {
        unknown_draw_edge_glow_5fd290(surface, Point2D(caret_x, rect.Y), Point2D(caret_x, rect.Y + rect.Height - 2),
                                      unknown_color_a7338c, 0xFF);
        unknown_draw_edge_glow_5fd290(surface, Point2D(caret_x + 1, rect.Y), Point2D(caret_x + 1, rect.Y + rect.Height - 2),
                                      unknown_color_a7338c, 0xFF);
    }
    unknown_caret_hidden_a763ec = 0;
}

}


bool __fastcall unknown_choice_dialog_600c80(unsigned short const *text)
{
    int result = -1;
    HWND dialog = unknown_create_dialog_5ff610(0x10d, unknown_choice_dialog_proc_600d50, 0);
    if (dialog) {
        HWND item = GetDlgItem(dialog, 0x5b0);
        SendMessageA(item, 0x4b2, 0, (LPARAM)text);
        SetWindowLongA(dialog, DWL_USER, (LONG)&result);
        unknown_dialog_5ff7c0(dialog);
        while (result < 0) {
            Unknown_005B5EE0();
            if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5 && !unknown_session_a3f616 &&
                unknown_counter_a3fabc == 0 && !unknown_53f460() && unknown_53fbd0()) {
                break;
            }
        }
        unknown_dialog_5ff6e0(dialog);
    }
    return result == 0x5c1;
}


int __stdcall unknown_choice_dialog_proc_600d50(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result) {
        return result;
    }
    int *choice = (int *)GetWindowLongA(window, DWL_USER);
    if (message == 0x111) {
        int id = wparam & 0xffff;
        if (id >= 0x5c0 && id <= 0x5c1) {
            *choice = id;
        }
    }
    return 0;
}


#ifndef RA2_OWNERDRAW_CHECK_FIELDS
#define RA2_OWNERDRAW_CHECK_FIELDS
#endif
// RTTI-confirmed OwnerDraw::DialogControl constructor, original9code+7NOP.
// An otherwise empty constructor establishes this class's vptr.
OwnerDraw::DialogControl::DialogControl() {}


// Modal dialog driven by the RTTI-confirmed DialogControl virtual interface.
// Original182code+10NOP; closed complete CFG, both returns RET4.
namespace OwnerDraw {
int __fastcall unknown_controlled_dialog_6011a0(int resource,DialogProc procedure,DialogControl *control)
{
    HWND window=unknown_create_dialog_5ff610(resource,procedure,(int)control);
    if(!window) {
        control->Set_Window_006010f0(0,0);
        return 0;
    }
    ShowWindow(window,1);
    SetForegroundWindow(window);
    WWKeyboard->Clear();
    while(!control->unknown_slot2()&&!control->unknown_slot4()) {
        if(unknown_6000d0())break;
        unknown_517370(false);
    }
    unknown_dialog_5ff6e0(window);
    return (int)control->Get_Window_00601180();
}
}


// Tests edit classes, a registry-marked list box, or an editable combo.
// Original427code+5NOP; complete original-only171instruction closed CFG.
extern "C" char const control_edit_class_7e8df0[];
extern "C" char const control_new_edit_class_7e8de8[];
extern "C" char const control_list_class_7e8de0[];
extern "C" char const control_combo_class_7e8dd4[];
namespace OwnerDraw {
bool __fastcall unknown_control_test_601260(HWND window)
{
    char name[128];
    GetClassNameA(window,name,sizeof(name));
    if(!strcmp(name,control_edit_class_7e8df0))return true;
    if(!strcmp(name,control_new_edit_class_7e8de8))return true;
    if(!strcmp(name,control_list_class_7e8de0)) {
        WinData *record=0;
        unsigned int const &key=(unsigned int const &)window;
        if(unknown_control_table_a73640.Get_Pointer(key, &record))return record->unknown_68==1;
    }
    if(!strcmp(name,control_combo_class_7e8dd4))return (GetWindowLongA(window,-16)&3)!=3;
    return false;
}
}


// 0x00600510: function not yet identified
//
// Body: 75 bytes (frozen inventory bound).
// Referenced from 3 sites (unconditional call).
// Ghidra prototype: void __thiscall Set_Wide_Text_00600510(int param_1,short *param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//   Wide_Length_00783605 -> src/recovered/00783605.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00600510 {

extern void  operator_delete_00781d3b(unsigned int);
extern unsigned int  Wide_Length_00783605(short *);
extern void  Wide_Copy_00783689(short *, short *);
extern "C" unsigned int __cdecl operator_new(...);  // @ref _operator_new=00782017

struct Owner_00600510 { void Set_Wide_Text_00600510(short *text); };

void Owner_00600510::Set_Wide_Text_00600510(short *text)
{
    int self = (int)this;
    int length;
    void *copy;

    if (*(int *)(self + 0x28) != 0) {
        operator_delete_00781d3b((unsigned int)(*(int *)(self + 0x28)));
        *(unsigned int *)(self + 0x28) = 0;
    }
    if ((text != (short *)0x0) && (*text != 0)) {
        length = (int)(Wide_Length_00783605((short *)(text)));
        copy = (void *)(operator_new(length * 2 + 2));
        *(void **)(self + 0x28) = copy;
        Wide_Copy_00783689((short *)(copy), (short *)(text));
    }
    return;
}

}  // namespace seed_00600510


// 0x00601040: function not yet identified
//
// Owner: OwnerDraw::DialogControl, stores the OwnerDraw::DialogControl vtable (constructor or destructor) (from RTTI/vtables).
// Body: 7 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall DialogControl_dtor_00601040(undefined4 *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00601040 {

extern "C" void* DialogControl_vtable_007a7b28;

void __fastcall DialogControl_dtor_00601040(unsigned int *self)
{
    *self = (unsigned int)(&DialogControl_vtable_007a7b28);
    return;
}

}  // namespace seed_00601040


class Call_601410_0 {public: unsigned int target();};
class Call_601410_1 {public: unsigned int target();};
class Leaf_601410 { public: unsigned int get(); };
unsigned int Leaf_601410::get() { unsigned int r0 = ((Call_601410_0*)(((unsigned int)this) + 8))->target(); unsigned int r1 = ((Call_601410_1*)(((unsigned int)this) + 4))->target(); return r1; }


// 0x00601430: function not yet identified
//
// Body: 30 bytes (frozen inventory bound).
// Referenced from 4 sites (unconditional call).
// Ghidra prototype: undefined4 __thiscall scalar_deleting_destructor_00601430(undefined4 param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00601430 {

extern void __fastcall WideString_dtor_0076fa50(void);
extern void  operator_delete_00781d3b(unsigned int);

struct Owner_00601430 { unsigned int scalar_deleting_destructor_00601430(unsigned char flags); };

unsigned int Owner_00601430::scalar_deleting_destructor_00601430(unsigned char flags)
{
    unsigned int self = (unsigned int)this;
    WideString_dtor_0076fa50();
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_00601430


// void WolArrayList<UnknownListEntry_6017c0>::Clear_Vector() (0x00601490): wlib clear() out of line for the
// 24-byte element list (destroys each element's two WideStrings, then empties the list).

// A 24-byte list element of ownrdraw.cpp: two WideStrings (0x00601490 destroys them, 0x006025A0 assigns them);
// the default constructor sets 0, -1, 0, -1 (0x006025A0's temporary). Field names are hypotheses.
struct UnknownListEntry_6017c0 {
    int unknown_00;
    WideString unknown_04;
    WideString unknown_08;
    int unknown_0c;
    int unknown_10;
    int unknown_14;
    UnknownListEntry_6017c0() { unknown_00 = 0; unknown_0c = -1; unknown_10 = 0; unknown_14 = -1; }
    // Member-wise copy, out of line (0x00601780).
    UnknownListEntry_6017c0(UnknownListEntry_6017c0 const &other);
};

template<> void WolArrayList<UnknownListEntry_6017c0>::Clear_Vector()
{
    Clear();
}


// wlib Dictionary::add, the out-of-line instance (0x005ECC90 calls it): expands operator= and leaves Copy_List 0x00600460 out of line.
template bool WolDictionary<unsigned int, OwnerDraw::WinData>::Add(unsigned int const &key, OwnerDraw::WinData const &value);


// wlib Dictionary<HWND, WinData>::getPointer out of line for the control registry (keyed by the window's
// value): looks up key; on a hit stores its record's address in value and returns true (0x00601710).
template bool WolDictionary<unsigned int, OwnerDraw::WinData>::Get_Pointer(unsigned int const &key, OwnerDraw::WinData **value) const;


// UnknownListEntry_6017c0::UnknownListEntry_6017c0(UnknownListEntry_6017c0 const &) (0x00601780): the 24-byte
// ownrdraw.cpp list element's member-wise copy (the two WideStrings through their copy constructor); the struct is
// defined beside 0x00601490. Called by the list's add (0x006025A0).
UnknownListEntry_6017c0::UnknownListEntry_6017c0(UnknownListEntry_6017c0 const &other)
    : unknown_00(other.unknown_00), unknown_04(other.unknown_04), unknown_08(other.unknown_08),
      unknown_0c(other.unknown_0c), unknown_10(other.unknown_10), unknown_14(other.unknown_14)
{
}


// bool WolArrayList<UnknownListEntry_6017c0>::Grow_Vector() (0x006017C0): a 24-byte element list (0x006025A0
// copies one).
// ownrdraw.cpp's wlib array lists (shared/wol_array_list.hpp, WolArrayList: Count, Capacity, Items): the
// out-of-line growVector/shrinkVector instances of its element types (explicit specializations of
// Grow_Vector/Shrink_Vector). Element types are hypotheses.

// UnknownListEntry_6017c0: defined beside 0x00601490 in this unit.

template<> bool WolArrayList<UnknownListEntry_6017c0>::Grow_Vector()
{
    if (Count < Capacity) return false;
    int capacity = Count * 2;
    if (capacity < 10) capacity = 10;
    UnknownListEntry_6017c0 *items = (UnknownListEntry_6017c0 *)operator new(capacity * sizeof(UnknownListEntry_6017c0));
    memset(items, 0, capacity * sizeof(UnknownListEntry_6017c0));
    if (Items) memcpy(items, Items, Count * sizeof(UnknownListEntry_6017c0));
    operator delete(Items);
    Items = items;
    Capacity = capacity;
    return true;
}


// bool WolArrayList<UnknownListEntry_6017c0>::Shrink_Vector() (0x00601850)
// ownrdraw.cpp's wlib array lists (shared/wol_array_list.hpp, WolArrayList: Count, Capacity, Items): the
// out-of-line growVector/shrinkVector instances of its element types (explicit specializations of
// Grow_Vector/Shrink_Vector). Element types are hypotheses.

// UnknownListEntry_6017c0: defined beside 0x00601490 in this unit.

template<> bool WolArrayList<UnknownListEntry_6017c0>::Shrink_Vector()
{
    return Shrink();
}


// wlib Dictionary<HWND, WNDPROC>::getPointer out of line: looks up key; on a hit stores its window procedure
// slot in value and returns true (0x006018E0).
template bool WolDictionary<HWND, WNDPROC>::Get_Pointer(HWND const &key, WNDPROC **value) const;


// wlib Dictionary<HWND, WNDPROC>::remove(key, value) out of line: removes key, passing its window procedure
// back; the table shrinks (shrink() inline) once its load falls to the shrink threshold (0x00601950).
template bool WolDictionary<HWND, WNDPROC>::Remove_Entry(HWND const &key, WNDPROC *old_value);


// wlib Dictionary<HWND, WNDPROC>::expand out of line: doubles the bucket count and rehashes every node; a
// fixed-size table is left alone (0x00601B90).
template void WolDictionary<HWND, WNDPROC>::Grow_Table();


// Native dictionary bucket index;29 complete body bytes +3 padding.
template<> unsigned int WolDictionary<HWND, WNDPROC>::Key_Hash(HWND const &key) const
{
    unsigned int hash = Hash(key);
    return ((1u << Bits) - 1) & hash;
}


// wlib Dictionary<HWND, WinData>::remove(key, value) out of line for the control registry: removes key,
// passing its record back; the table shrinks (shrink() out of line, 0x006027E0) once its load falls to the
// shrink threshold (0x00601C50).
template bool WolDictionary<unsigned int, OwnerDraw::WinData>::Remove_Entry(unsigned int const &key, OwnerDraw::WinData *old_value);


// wlib Dictionary<HWND, WinData>::expand out of line for the control registry: doubles the bucket count and
// rehashes every node; a fixed-size table is left alone (0x00601F70).
template void WolDictionary<unsigned int, OwnerDraw::WinData>::Grow_Table();


// wlib Dictionary::keyHash, the control registry's out-of-line instance (0x00602020): the template's own
// instance (shared/wol_dictionary.hpp), emitted for the call sites /Ob2 refuses (see the template).
template unsigned int WolDictionary<unsigned int, OwnerDraw::WinData>::Key_Hash(unsigned int const &key) const;


// 00602040: pair-keyed active-message dictionary removal,607 code bytes +1 padding.
// Explicit specialization stands in for the unrecovered CtrlProc005ee460 caller.
// The original copies a one-byte value from node+8 and links at node+12.
template<> bool WolDictionary<OwnerDraw::UnknownPairKey, bool>::Remove_Entry(OwnerDraw::UnknownPairKey const &key, bool *old_value)
{
        if (Count == 0) return false;
        float load = (float)(Count - 1);
        load /= (float)BucketCount;
        unsigned int bucket = Key_Hash(key);
        Node *head = Buckets[bucket];
        Node *previous = head;
        if (!head) return false;
        if (head->Key == key) {
            *old_value = head->Value;
            Node *removed = Buckets[bucket];
            Node *next = removed->Next;
            delete removed;
            Buckets[bucket] = next;
            --Count;
            if (load <= ShrinkLimit) Shrink_Table();
            return true;
        }
        Node *node = head->Next;
        bool removed = false;
        while (node) {
            if (node->Key == key) {
                *old_value = node->Value;
                previous->Next = node->Next;
                --Count;
                delete node;
                removed = true;
                break;
            }
            previous = node;
            node = node->Next;
        }
        if (load <= ShrinkLimit) Shrink_Table();
        return removed;
    }


// void WolDictionary<OwnerDraw::UnknownPairKey, int>::Grow_Table() (0x006022A0): the pair-keyed table
// 0x00A73398 (16-byte nodes; the value type is a 4-byte hypothesis).
// The out-of-line expand() (wlib Dictionary::expand, shared/wol_dictionary.hpp's Grow_Table) of one of
// ownrdraw.cpp's dictionaries. Written as an explicit specialization with the template's body: its callers
// (unrecovered window procedures) are what emit the implicit instance in the original; when they land, drop
// this specialization (it must precede their uses) and let the template instantiate.

template<> void WolDictionary<OwnerDraw::UnknownPairKey, int>::Grow_Table()
{
    if (ResizeLocked != true) {
        int old_count = BucketCount;
        Node **old_buckets = Buckets;
        BucketCount *= 2;
        ++Bits;
        Buckets = new Node *[BucketCount];
        memset(Buckets, 0, BucketCount * sizeof(Node *));
        for (int index = 0; index < old_count; ++index) {
            Node *node = old_buckets[index];
            while (node) {
                unsigned int hash = Hash(node->Key);
                unsigned int bucket = ((1u << Bits) - 1) & hash;
                Node *head = Buckets[bucket];
                Buckets[bucket] = node;
                Node *next = node->Next;
                node->Next = head;
                node = next;
            }
        }
        delete [] old_buckets;
    }
}


// Native dictionary bucket index;29 complete body bytes +3 padding.
template<> unsigned int WolDictionary<OwnerDraw::UnknownPairKey, bool>::Key_Hash(OwnerDraw::UnknownPairKey const &key) const
{
    unsigned int hash = Hash(key);
    return ((1u << Bits) - 1) & hash;
}


// bool WolArrayList<int>::Grow_Vector() (0x00602360)
// ownrdraw.cpp's wlib array lists (shared/wol_array_list.hpp, WolArrayList: Count, Capacity, Items): the
// out-of-line growVector/shrinkVector instances of its three element types (explicit specializations of
// Grow_Vector/Shrink_Vector). Element types are hypotheses: two 4-byte lists (int, HWND; identical code at
// two addresses) and one 20-byte record list.

template<> bool WolArrayList<int>::Grow_Vector()
{
    return Grow();
}


// bool WolArrayList<int>::Shrink_Vector() (0x006023F0): wlib's shrinkVector with new[]/delete[] (the class's
// inline Shrink, through operator new, schedules the stores differently for 4-byte elements).
// ownrdraw.cpp's wlib array lists (shared/wol_array_list.hpp, WolArrayList: Count, Capacity, Items): the
// out-of-line growVector/shrinkVector instances of its three element types (explicit specializations of
// Grow_Vector/Shrink_Vector). Element types are hypotheses: two 4-byte lists (int, HWND; identical code at
// two addresses) and one 20-byte record list.

template<> bool WolArrayList<int>::Shrink_Vector()
{
    if (Count * 3 > Capacity) return false;
    int capacity = Capacity / 2;
    if (capacity < 10) capacity = 10;
    if (capacity >= Capacity) return false;
    int *items = new int[capacity];
    if (Items) memcpy(items, Items, Count * sizeof(int));
    delete[] Items;
    Items = items;
    Capacity = capacity;
    return true;
}


// bool WolArrayList<UnknownListEntry_602480>::Shrink_Vector() (0x00602480)
// ownrdraw.cpp's wlib array lists (shared/wol_array_list.hpp, WolArrayList: Count, Capacity, Items): the
// out-of-line growVector/shrinkVector instances of its three element types (explicit specializations of
// Grow_Vector/Shrink_Vector). Element types are hypotheses: two 4-byte lists (int, HWND; identical code at
// two addresses) and one 20-byte record list.

// A 20-byte list element of ownrdraw.cpp (layout unknown).
struct UnknownListEntry_602480 { int unknown_00[5]; };

template<> bool WolArrayList<UnknownListEntry_602480>::Shrink_Vector()
{
    return Shrink();
}


// UnknownListState_6025a0::UnknownListState_6025a0(UnknownListState_6025a0 const &) (0x006025A0): copies two
// words and the 24-byte element list through wlib's ArrayList copy constructor (empty list, then operator=:
// clear, then get/add each element through a default-constructed temporary). Struct and field names are
// hypotheses; no other user of this layout is known yet.

struct UnknownListState_6025a0 {
    int unknown_00;
    int unknown_04;
    WolArrayList<UnknownListEntry_6017c0> List;
    UnknownListState_6025a0(UnknownListState_6025a0 const &other);
};

UnknownListState_6025a0::UnknownListState_6025a0(UnknownListState_6025a0 const &other)
    : unknown_00(other.unknown_00), unknown_04(other.unknown_04), List(other.List)
{
}


// Halves and rehashes the HWND-to-window-procedure tables (0x00602730).
// The shared template body is instantiated by the existing Restore_Window caller.
// This explicit instantiation records that original out-of-line copy.
template void WolDictionary<HWND, WNDPROC>::Shrink_Table();


// void WolDictionary<unsigned int, OwnerDraw::WinData>::Shrink_Table() (0x006027E0): the
// control registry 0x00A73640 (nodes of 0x208 bytes; called from Remove_Entry 0x00601C50, 0x005EC360 and
// 0x005F8740). wlib Dictionary::shrink out of line, the template's own instance (the registry's atexit
// destructor 0x005DEE00 expands the same body through Shrink_Buckets; RA2_WOL_DICTIONARY_SHRINK_ACCESSORS).

template void WolDictionary<unsigned int, OwnerDraw::WinData>::Shrink_Table();


// void WolDictionary<OwnerDraw::UnknownPairKey, int>::Shrink_Table() (0x00602890): the pair-keyed table
// 0x00A73398 (16-byte nodes; the value type is a 4-byte hypothesis; called from 0x005EFD6F).
// The out-of-line shrink() (wlib Dictionary::shrink, shared/wol_dictionary.hpp's Shrink_Table) of one of
// ownrdraw.cpp's dictionaries. Written as an explicit specialization with the template's body: its callers
// (unrecovered window procedures) are what emit the implicit instance in the original; when they land, drop
// this specialization (it must precede their uses) and let the template instantiate.

template<> void WolDictionary<OwnerDraw::UnknownPairKey, int>::Shrink_Table()
{
    if (BucketCount > MinimumBuckets && ResizeLocked != true) {
        int old_count = BucketCount;
        Node **old_buckets = Buckets;
        BucketCount >>= 1;
        --Bits;
        Buckets = new Node *[BucketCount];
        memset(Buckets, 0, BucketCount * sizeof(Node *));
        for (int index = 0; index < old_count; ++index) {
            Node *node = old_buckets[index];
            while (node) {
                unsigned int hash = Hash(node->Key);
                unsigned int bucket = ((1u << Bits) - 1) & hash;
                Node *head = Buckets[bucket];
                Buckets[bucket] = node;
                Node *next = node->Next;
                node->Next = head;
                node = next;
            }
        }
        delete [] old_buckets;
    }
}


// bool WolArrayList<HWND>::Grow_Vector() (0x00602940)
// ownrdraw.cpp's wlib array lists (shared/wol_array_list.hpp, WolArrayList: Count, Capacity, Items): the
// out-of-line growVector/shrinkVector instances of its three element types (explicit specializations of
// Grow_Vector/Shrink_Vector). Element types are hypotheses: two 4-byte lists (int, HWND; identical code at
// two addresses) and one 20-byte record list.

template<> bool WolArrayList<HWND>::Grow_Vector()
{
    return Grow();
}


// bool WolArrayList<UnknownListEntry_602480>::Grow_Vector() (0x006029D0)
// ownrdraw.cpp's wlib array lists (shared/wol_array_list.hpp, WolArrayList: Count, Capacity, Items): the
// out-of-line growVector/shrinkVector instances of its three element types (explicit specializations of
// Grow_Vector/Shrink_Vector). Element types are hypotheses: two 4-byte lists (int, HWND; identical code at
// two addresses) and one 20-byte record list.

// UnknownListEntry_602480: defined beside 0x00602480 in this unit.

template<> bool WolArrayList<UnknownListEntry_602480>::Grow_Vector()
{
    return Grow();
}
