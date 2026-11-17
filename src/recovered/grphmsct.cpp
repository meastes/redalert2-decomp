/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/grphmsct.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Local variable names from mischa85/RedAlert2 392e904 src/grphmsct.cpp where both functions declare the same local types (names only).

// grphmsct.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/grphmsct.json carries the addresses and the rows that
// stay in their own files.


#include "shared/graphic_menu.hpp"
#include "shared/ini_class.hpp"
#include "shared/wstring.hpp"
#include "shared/vector_cursor.hpp"


// Header-static dynamic initializer of the GraphicMenuShortcutItem unit
// (0x004E0230, _$E): shape_file.hpp's empty rect (0x00A40E68), emitted
// through shared/graphic_menu.hpp by the drafts of this plan. The unit
// defines the container members it uses.
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif


// GraphicMenuItem *__fastcall Create_Shortcut_Item(Wstring const &section, INIClass &ini)
// (0x004E0250): a GraphicMenuShortcutItem from a menu INI section's ID (none for -1)
// and Keys. Name is a hypothesis; the argument shapes follow the original: the ID key
// is an explicit Wstring temporary (pushed as the constructor's result), the Keys
// entry and its default convert implicitly (their temporaries' addresses are pushed).

extern char unknown_string_83c55c[];

GraphicMenuItem *__fastcall Create_Shortcut_Item(Wstring const &section, INIClass &ini)
{
    int id = ini.Get_Int(section, Wstring("ID"), -1);
    if (id == -1) {
        return 0;
    }
    Wstring keys;
    ini.Get_String(section, "Keys", unknown_string_83c55c, keys);
    return new GraphicMenuShortcutItem(id, keys);
}


/// <summary>
/// Creates a shortcut item for a graphic menu.
/// The key list is split apart and each description is converted into a key code
/// that the item will answer to. Descriptions that cannot be recognized are quietly
/// discarded.
/// </summary>
/// <param name="keys">The shortcut key descriptions, separated by commas.</param>
// (0x004E0310)
// Splits the comma (or comma/semicolon) separated key names and keeps the key code of
// every name Parse_Shortcut_Key knows.
int __fastcall Parse_Shortcut_Key(Wstring const &text);

GraphicMenuShortcutItem::GraphicMenuShortcutItem(int id, Wstring const &keys) : GraphicMenuItem(id)
{
    Wstring tok;
    int pos = keys.Get_Token(0, ",", tok);
    while (tok.Length() > 0) {
        int key = Parse_Shortcut_Key(tok);
        if (key != 0) {
            Keys.Add(key);
        }
        pos = keys.Get_Token(pos, ",;", tok);
        if (pos == -1) {
            tok.Release();
        }
    }
}


// int __fastcall Parse_Shortcut_Key(Wstring const &text) (0x004E0410): the key code of a
// shortcut name: optional "ctrl-" (0x200), "alt-" (0x400) and "shift-" (0x100) prefixes,
// then one character (its code minus 0x20) or a name from the table at 0x007D9EF8
// ("esc", "backspace"); 0 when the name is empty or unknown. Name is a hypothesis.

struct KeyName {
    char const *Name;
    int Code;
};
extern KeyName const KeyNames_7d9ef8[2];       // 0x007D9EF8 {"esc", 0x1B}, {"backspace", 8}

int __fastcall Parse_Shortcut_Key(Wstring const &text)
{
    int key = 0;
    Wstring name(text);
    name.To_Lower();
    Wstring prefix(name);
    prefix.Truncate(5);
    if (prefix == "ctrl-") {
        key = 0x200;
        name.Remove(0, 5);
    }
    prefix = name;
    prefix.Truncate(4);
    if (prefix == "alt-") {
        key |= 0x400;
        name.Remove(0, 4);
    }
    prefix = name;
    prefix.Truncate(6);
    if (prefix == "shift-") {
        key |= 0x100;
        name.Remove(0, 6);
    }
    int value = 0;
    switch (name.Length()) {
        case 0:
            break;
        case 1:
            value = name.Get_Char(0) - ' ';
            break;
        default: {
            int index;
            for (index = 1; index >= 0; index--) {
                if (!(name != KeyNames_7d9ef8[index].Name)) {
                    break;
                }
            }
            if (index >= 0) {
                value = KeyNames_7d9ef8[index].Code;
            }
            break;
        }
    }
    if (value != 0) {
        key |= value;
    } else {
        key = 0;
    }
    return key;
}


/// <summary>
/// Destroys the graphic menu shortcut item.
/// The shortcut list cleans up after itself, so there is nothing left for this
/// routine to do.
/// </summary>
// (0x004E0560)
// Destroys the shortcut list (VectorClass<unsigned int>, expanded inline),
// then the base part.

GraphicMenuShortcutItem::~GraphicMenuShortcutItem()
{
}


/// <summary>
/// Determines if a key press triggers this menu item.
/// This routine is called by the graphic menu when it is looking for the item that
/// should respond to a key press. A disabled item never claims a key.
/// </summary>
/// <returns>bool; Is this key one of the item's shortcuts?</returns>
// (0x004E05B0)
// True when the item is visible and one of its shortcut keys is `key`.
// The cursor is used through a pointer, so its virtual slots stay calls.
typedef VectorCursor<unsigned int, DynamicVectorClass<unsigned int> > KeyCursor;

bool GraphicMenuShortcutItem::Is_Input_Key(int key)
{
    if (!Visible) {
        return false;
    }
    KeyCursor cursor(Keys);
    KeyCursor *at = &cursor;
    while (at->Valid() && at->Current() != key) {
        at->Next();
    }
    return at->Valid();
}


// GraphicMenuShortcutItem scalar deleting destructor (0x004E0630, ??_G):
// emitted with the vtable by the destructor's definition; the destructor is
// expanded inline.


// VectorCursor_UnsignedInt_Boolean_Conversion_004e0690 (0x004E0690)

bool __fastcall VectorCursor_UnsignedInt_Boolean_Conversion_004e0690(int cursor_address)
{
    return *(unsigned int *)(cursor_address + 8) < *(unsigned int *)(*(int *)(cursor_address + 4) + 0x10);
}



// 0x004e06a0: function not yet identified
//
// Owner: VectorCursor<unsigned int, DynamicVectorClass<unsigned int>>, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 32 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall VectorCursor_UnsignedInt_Scalar_Deleting_Destructor_004e06a0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/libcmt/operator_delete.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004e06a0 {

extern void  operator_delete_00781d3b(unsigned int allocation_address);
extern "C" void* VectorCursor_UnsignedInt_VTable_007a2c08;

struct Owner_004e06a0 { unsigned int * VectorCursor_UnsignedInt_Scalar_Deleting_Destructor_004e06a0(unsigned char deletion_flags); };

unsigned int * Owner_004e06a0::VectorCursor_UnsignedInt_Scalar_Deleting_Destructor_004e06a0(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&VectorCursor_UnsignedInt_VTable_007a2c08);
    if ((deletion_flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_004e06a0


class Leaf_4e06c0 { public: unsigned int get(); };
unsigned int Leaf_4e06c0::get() { unsigned int cursor_address = (unsigned int)this; unsigned int result_address; unsigned int unused_value; result_address = cursor_address; *(unsigned int*)(result_address + 8) = (*(unsigned int*)(result_address + 8)) + 1; return result_address; }


class Leaf_4e06d0 { public: unsigned int get(); };
unsigned int Leaf_4e06d0::get() { unsigned int cursor_address = (unsigned int)this; unsigned int result_address; unsigned int unused_value; result_address = cursor_address; *(unsigned int*)(result_address + 8) = (*(unsigned int*)(result_address + 8)) - 1; return result_address; }
