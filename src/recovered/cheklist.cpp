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
// Function comment blocks from mischa85/RedAlert2 392e904 src/cheklist.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// cheklist.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/cheklist.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/check_list.hpp"


// Header-static dynamic initializer of the unit at 0x0047D2C0: the empty
// rectangle of shared/shape_file.hpp (0x00850C58), the unit's first function.


/***************************************************************************
 * CheckListClass::CheckListClass -- constructor                           *
 *                                                                         *
 * INPUT:                                                                  *
 *      id         control ID for this list box                            *
 *      x         x-coord                                                  *
 *      y         y-coord                                                  *
 *      w         width                                                    *
 *      h         height                                                   *
 *      flags      mouse event flags                                       *
 *      up         ptr to Up-arrow shape                                   *
 *      down      ptr to Down-arrow shape                                  *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   02/16/1995 BR : Created.                                              *
 *=========================================================================*/
// (0x0047D2E0)
// A list that accepts input.

CheckListClass::CheckListClass(int id, int x, int y, int w, int h, unsigned flags, void const *up, void const *down) :
    ListClass(id, x, y, w, h, flags, up, down),
    IsReadOnly(false)
{
}


/***********************************************************************************************
 * CheckListClass::~CheckListClass -- Destructor for check list object.                        *
 *                                                                                             *
 *    This destructor will delete all entries attached to it.                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0047D330)
// Deletes the check objects while removing the entries.

CheckListClass::~CheckListClass()
{
    while (List.Count()) {
        CheckObject *object = (CheckObject *)ListClass::Get_Item(0);
        ListClass::Remove_Item(0);
        delete object;
    }
}


/***********************************************************************************************
 * CheckListClass::Add_Item -- Adds specifies text to check list box.                          *
 *                                                                                             *
 *    This routine will add the specified text string to the check list.                       *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text string to add to the list box.                        *
 *                                                                                             *
 * OUTPUT:  Returns the index number where the text object was added.                          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   02/14/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0047D370, slot 34)
// Adds an unchecked entry.

int CheckListClass::Add_Item(wchar_t *text)
{
    return ListClass::Add_Item((wchar_t *)new CheckObject(text, false));
}


/// <summary>
/// Fetches a pointer to the text of the current selection.
/// This routine will find the text associated with the currently selected entry and return
/// a pointer to it. Use this routine rather than the base list class version, which hands
/// back the check object instead of the text.
/// </summary>
/// <returns>Returns with the text pointer of the selected entry. If there is no selection,
/// then NULL is returned.</returns>
// (0x0047D3B0, slot 39)

wchar_t *CheckListClass::Current_Item() const
{
    CheckObject *object = (CheckObject *)ListClass::Current_Item();
    if (object != 0) {
        return object->Text;
    }
    return 0;
}


/***********************************************************************************************
 * CheckListClass::Get_Item -- Fetches a pointer to the text associated with the index.        *
 *                                                                                             *
 *    This routine will find the text associated with the entry specified and return a pointer *
 *    to that text.                                                                            *
 *                                                                                             *
 * INPUT:   index -- The entry (index) to fetch a pointer to.                                  *
 *                                                                                             *
 * OUTPUT:  Returns with the text pointer associated with the index specified.                 *
 *                                                                                             *
 * WARNINGS:   If the index is out of range, then NULL is returned.                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0047D3C0, slot 40)

wchar_t *CheckListClass::Get_Item(int index) const
{
    CheckObject *object = (CheckObject *)ListClass::Get_Item(index);
    if (object != 0) {
        return object->Text;
    }
    return 0;
}


/***********************************************************************************************
 * CheckListClass::Remove_Item -- Remove the item that matches the text pointer specified.     *
 *                                                                                             *
 *    This routine will find the entry that matches the text pointer specified and then        *
 *    delete that entry.                                                                       *
 *                                                                                             *
 * INPUT:   text  -- The text pointer to use to find the exact match in the list.              *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0047D3E0, slot 43)
// Removes (and deletes) the first entry with this text.

void CheckListClass::Remove_Item(wchar_t *text)
{
    for (int index = 0; index < Count(); index++) {
        CheckObject *object = (CheckObject *)ListClass::Get_Item(index);
        if (object != 0 && _wcsicmp(object->Text, text) == 0) {
            ListClass::Remove_Item(index);
            delete object;
            return;
        }
    }
}


/***********************************************************************************************
 * CheckListClass::Set_Selected_Index -- Set the selected index to match the text pointer spec *
 *                                                                                             *
 *    This routine will find the entry that exactly matches the text pointer specified. If     *
 *    found, then that entry will be set as the currently selected index.                      *
 *                                                                                             *
 * INPUT:   text  -- Pointer to the text string to find the match for.                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   If an exact match to the specified text string could not be found, then the     *
 *             currently selected index is not changed.                                        *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/06/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0047D450, slot 45)
// Selects the first entry with this text.

void CheckListClass::Set_Selected_Index(wchar_t *text)
{
    for (int index = 0; index < Count(); index++) {
        CheckObject *object = (CheckObject *)ListClass::Get_Item(index);
        if (object != 0 && _wcsicmp(object->Text, text) == 0) {
            Set_Selected_Index(index);
            return;
        }
    }
}


/***************************************************************************
 * CheckListClass::Check_Item -- [un]checks an items                       *
 *                                                                         *
 * INPUT:                                                                  *
 *      index         index of item to check or uncheck                    *
 *      checked      0 = uncheck, non-zero = check                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   02/16/1995 BR : Created.                                              *
 *   02/14/1996 JLB : Revamped.                                            *
 *=========================================================================*/
// (0x0047D4B0)
// Sets an entry's check mark, redrawing on a change.

void CheckListClass::Check_Item(int index, bool checked)
{
    CheckObject *object = (CheckObject *)ListClass::Get_Item(index);
    if (object != 0 && object->IsChecked != checked) {
        object->IsChecked = checked;
        Flag_To_Redraw();
    }
}


/***************************************************************************
 * CheckListClass::Is_Checked -- returns checked state of an item          *
 *                                                                         *
 * INPUT:                                                                  *
 *      index         index of item to query                               *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      0 = item is unchecked, 1 = item is checked                         *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   02/16/1995 BR : Created.                                              *
 *   02/14/1996 JLB : Revamped.                                            *
 *=========================================================================*/
// (0x0047D4E0)

bool CheckListClass::Is_Checked(int index) const
{
    CheckObject *object = (CheckObject *)ListClass::Get_Item(index);
    if (object != 0) {
        return object->IsChecked;
    }
    return false;
}


/***************************************************************************
 * CheckListClass::Action -- action function for this class                *
 *                                                                         *
 * INPUT:                                                                  *
 *      flags      the reason we're being called                           *
 *      key      the KN_number that was pressed                            *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      true = event was processed, false = event not processed            *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   02/16/1995 BR : Created.                                              *
 *=========================================================================*/
// (0x0047D500, slot 31)
// Ignored while read-only; a left press toggles the selected entry's mark.

int CheckListClass::Action(unsigned flags, unsigned &key)
{
    if (IsReadOnly) {
        return 0;
    }
    int result = ListClass::Action(flags, key);
    if (flags & 1) {
        Check_Item(SelectedIndex, !Is_Checked(SelectedIndex));
    }
    return result;
}


/***************************************************************************
 * CheckListClass::Draw_Entry -- draws a list box entry                    *
 *                                                                         *
 * INPUT:                                                                  *
 *      index         index into List of item to draw                      *
 *      x,y         x,y coords to draw at                                  *
 *      width         maximum width allowed for text                       *
 *      selected      true = this item is selected                         *
 *                                                                         *
 * OUTPUT:                                                                 *
 *      none.                                                              *
 *                                                                         *
 * WARNINGS:                                                               *
 *      none.                                                              *
 *                                                                         *
 * HISTORY:                                                                *
 *   12/14/1995 BRR : Created.                                             *
 *=========================================================================*/
// (0x0047D580, slot 50)
// Draws an entry as its check mark (character 3, or a blank) and a blank
// before the text, in ListClass::Draw_Entry's style.

extern "C" int __cdecl swprintf(wchar_t *buffer, wchar_t const *format, ...);

void CheckListClass::Draw_Entry(int index, int x, int y, int width, int selected)
{
    if (index < Count()) {
        CheckObject *object = (CheckObject *)ListClass::Get_Item(index);
        if (object != 0) {
            wchar_t buffer[100] = {0};
            buffer[0] = object->IsChecked ? 3 : L' ';
            buffer[1] = L' ';
            swprintf(&buffer[2], object->Text);
            unsigned flags = TextFlags;
            int scheme = Gadget_Color_Scheme();
            if (selected) {
                flags |= 0x2000;
                unknown_surface_83995c->Fill_Rect(Rect(x, y, width, LineHeight),
                                                  unknown_drawer_8323c4->Convert(ColorSchemes[scheme]->unknown_color_0x31c));
            } else if (!(flags & 0x4000)) {
                flags |= 0x1000;
            }
            Point2D point(x, y);
            Fancy_Clip_Text_Print(buffer, unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &point,
                                  ColorSchemes[scheme], 0, flags, width, Tabs);
        }
    }
}


// void GadgetClass::Flag_List_To_Redraw() (0x0047D6F0; kept copy)
// Compiler-emitted from the gadget headers in this unit; no source of its own.


// bool GadgetClass::Is_To_Redraw() (0x0047D700; kept copy)
// Compiler-emitted from the gadget headers in this unit; no source of its own.


// int ListClass::Count() const (0x0047D710; kept copy)
// Compiler-emitted from the gadget headers in this unit; no source of its own.


// void CheckListClass::Remove_Item(int index) (0x0047D720)
// Compiler-emitted from the gadget headers in this unit; no source of its own.


// void CheckListClass::Set_Selected_Index(int index) (0x0047D730)
// Compiler-emitted from the gadget headers in this unit; no source of its own.


// void *CheckListClass::`scalar deleting destructor'(unsigned int) (0x0047D740)
// Compiler-emitted from the gadget headers in this unit; no source of its own.
