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
// Function comment blocks from mischa85/RedAlert2 392e904 src/drop.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// drop.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/drop.json carries the addresses and the rows that
// stay in their own files.

// Header-static dynamic initializer of the unit at 0x004A4DA0: the empty
// rectangle of shared/shape_file.hpp (0x008529D8), the unit's first function.
#include "shared/shape_file.hpp"


#include "shared/gadget.hpp"


/// <summary>
/// Creates a drop down list gadget.
/// This routine builds an edit box with a list box and a drop button attached to it.
/// The list box remains detached from the display until the button is pressed, at
/// which point it is dropped down beneath the edit box.
/// </summary>
/// <param name="id">The button ID reported to the dialog when a selection is made.</param>
/// <param name="text">Buffer that holds the text of the currently selected item.</param>
/// <param name="max_len">The maximum number of characters the text buffer can hold.</param>
/// <param name="flags">The text print flags to render the edit box and list with.</param>
/// <param name="h">The pixel height of the list box when it is dropped down.</param>
/// <param name="up">The shape for the list's scroll up arrow.</param>
/// <param name="down">The shape for the list's scroll down arrow and for the drop button.</param>
// (0x004A4DC0)
// An 18-pixel edit field (height then refit to the font) with the drop
// button at its right and an h-pixel list below the button's height.

DropListClass::DropListClass(int id, wchar_t *text, int max_len, unsigned flags, int x, int y, int w, int h,
                             void const *up, void const *down) :
    EditClass(id, text, max_len, flags, x, y, w, 18, 7),
    IsDropped(false),
    ListHeight(h),
    DropButton(0, down, x + w, y),
    List(0, x, y + ((short const *)down)[2], w + ((short const *)down)[1], h, flags, up, down)
{
    Height = Font_For_Style(flags)->Height + 1;
    List.Make_Peer(*this);
    DropButton.Make_Peer(*this);
}


/// <summary>
/// Detaches the drop list from the display.
/// The list is collapsed first, then the list box, the drop button, and the edit box
/// itself are all zapped so that none of them refer to a display that is going away.
/// </summary>
// (0x004A4E90, slot 8)

void DropListClass::Zap()
{
    Collapse();
    List.Zap();
    DropButton.Zap();
    EditClass::Zap();
}


/// <summary>
/// Adds this drop list to a gadget list.
/// The drop button is linked in alongside the edit box so that the dialog processes
/// both halves of the gadget.
/// </summary>
/// <param name="object">The gadget list to attach this drop list to.</param>
/// <returns>Returns with a reference to the head of the gadget list.</returns>
// (0x004A4ED0, slot 3)

LinkClass &DropListClass::Add(LinkClass &object)
{
    DropButton.Add(object);
    return EditClass::Add(object);
}


/// <summary>
/// Adds this drop list to the end of a gadget list.
/// The drop button is linked in alongside the edit box so that the dialog processes
/// both halves of the gadget.
/// </summary>
/// <param name="object">The gadget list to attach this drop list to.</param>
/// <returns>Returns with a reference to the head of the gadget list.</returns>
// (0x004A4EF0, slot 4)

LinkClass &DropListClass::Add_Tail(LinkClass &object)
{
    DropButton.Add_Tail(object);
    return EditClass::Add_Tail(object);
}


/// <summary>
/// Adds this drop list to the head of a gadget list.
/// The drop button is linked in alongside the edit box so that the dialog processes
/// both halves of the gadget.
/// </summary>
/// <param name="object">The gadget list to attach this drop list to.</param>
/// <returns>Returns with a reference to the head of the gadget list.</returns>
// (0x004A4F10, slot 5)

LinkClass &DropListClass::Add_Head(LinkClass &object)
{
    DropButton.Add_Head(object);
    return EditClass::Add_Head(object);
}


/// <summary>
/// Removes this drop list from the gadget list.
/// The list box is collapsed first, so that a dropped down list does not linger on
/// the display after the drop list that owns it has gone.
/// </summary>
/// <returns>Returns with a pointer to the head of the gadget list, or NULL if nothing
/// remains in it.</returns>
// (0x004A4F30, slot 9)

LinkClass *DropListClass::Remove()
{
    Collapse();
    DropButton.Remove();
    return EditClass::Remove();
}


/// <summary>
/// Adds an item to the drop down list.
/// The text added also becomes the displayed contents of the edit box, so the item
/// most recently added is the one the player sees.
/// </summary>
/// <param name="text">The text of the item to add.</param>
/// <returns>Returns with the index the item was added at.</returns>
// (0x004A4F60, slot 39)
// Shows text in the field and appends it to the list.

int DropListClass::Add_Item(wchar_t *text)
{
    wcsncpy(String, text, MaxLength);
    Flag_To_Redraw();
    return List.Add_Item(text);
}


/// <summary>
/// Fetches the text of the currently selected item.
/// </summary>
/// <returns>Returns with a pointer to the text of the selected item.</returns>
// (0x004A4FA0, slot 40)

wchar_t *DropListClass::Current_Item()
{
    return List.Current_Item();
}


/// <summary>
/// Fetches the index of the currently selected item.
/// </summary>
/// <returns>Returns with the list index of the selected item.</returns>
// (0x004A4FC0, slot 41)

int DropListClass::Current_Index()
{
    return List.Current_Index();
}


/// <summary>
/// Sets the currently selected item by index.
/// The text of the item picked becomes the contents of the edit box. An index that
/// falls outside the list clears the edit box instead.
/// </summary>
/// <param name="index">The index of the item to select.</param>
// (0x004A4FE0, slot 43)
// Selects a list item and copies it into the field (empty when invalid).

void DropListClass::Set_Selected_Index(int index)
{
    if ((unsigned)index < (unsigned)List.Count()) {
        List.Set_Selected_Index(index);
        wcscpy(String, List.Get_Item(Current_Index()));
    } else {
        String[0] = 0;
    }
}


/// <summary>
/// Clears the input focus from this gadget.
/// The dropped down list is collapsed, since a drop list that has lost the focus has
/// no business covering the display.
/// </summary>
// (0x004A5050, slot 21)

void DropListClass::Clear_Focus()
{
    Collapse();
}


/// <summary>
/// Handles input events from the attached list box and drop button.
/// This routine is how the drop list learns that its button was clicked or that an
/// item was picked. The button toggles the list between expanded and collapsed, and
/// a pick copies the item text into the edit box and reports the drop list's own
/// button ID back to the dialog.
/// </summary>
/// <param name="flags">The input event flags that triggered this notification.</param>
/// <param name="key">The key code returned to the dialog; set when a selection is made.</param>
/// <param name="whom">The peer gadget that generated the event.</param>
// (0x004A5070, slot 19)
// The button toggles the list; picking from the list copies the item.

void DropListClass::Peer_To_Peer(unsigned flags, unsigned &key, ControlClass &whom)
{
    if (&whom == &DropButton && (flags & LEFTRELEASE)) {
        if (IsDropped) {
            Collapse();
            key = ID | KN_BUTTON;
        } else {
            Expand();
        }
    }
    if (&whom == &List) {
        wcsncpy(String, List.Current_Item(), MaxLength);
        Flag_To_Redraw();
        key = ID | KN_BUTTON;
    }
}


/// <summary>
/// Drops the list box down so that it is visible.
/// The list box is positioned under the edit box and linked into the same gadget
/// list, so it will be drawn and will process input until it is collapsed again.
/// </summary>
// (0x004A5140)
// Drops the list under the field and links it in.

void DropListClass::Expand()
{
    if (!IsDropped) {
        List.X = X;
        List.Y = Y + 18;
        List.Width = Width;
        List.Height = ListHeight;
        List.Add(Head_Of_List());
        List.Flag_To_Redraw();
        IsDropped = true;
    }
}


/// <summary>
/// Hides the dropped down list box.
/// The list box is unlinked from the gadget list, but its contents are left intact
/// so that the drop list can be expanded again later.
/// </summary>
// (0x004A51A0)

void DropListClass::Collapse()
{
    if (IsDropped) {
        List.Remove();
        IsDropped = false;
    }
}


DropListClass &DropListClass::operator=(DropListClass &list)
{
 if(this==&list) return *this;
  EditClass::operator=(list);
  List=list.List;
  IsDropped=list.IsDropped;
  ListHeight=list.ListHeight;
  DropButton=list.DropButton;
  List.Make_Peer(*this);
  DropButton.Make_Peer(*this);
 return *this;
}


/// <summary>
/// Creates a copy of an existing drop list.
/// The peer links are rebuilt so that the copied list box and drop button report
/// their events to this drop list rather than to the original.
/// </summary>
/// <param name="list">The drop list to copy.</param>
DropListClass::DropListClass(DropListClass const &list) :
    EditClass(list), IsDropped(list.IsDropped), ListHeight(list.ListHeight),
    DropButton(list.DropButton), List(list.List)
{
    List.Make_Peer(*this);
    DropButton.Make_Peer(*this);
}


/// <summary>
/// Sets the pixel position of the drop list.
/// The list box and the drop button are moved along with the edit box so that the
/// gadget stays assembled.
/// </summary>
/// <param name="x">The new pixel X coordinate of the edit box.</param>
/// <param name="y">The new pixel Y coordinate of the edit box.</param>
// (0x004A5490, slot 25)
// The list goes under the button's height, the button to the field's right.

void DropListClass::Set_Position(int x, int y)
{
    GadgetClass::Set_Position(x, y);
    List.Set_Position(x, ((short const *)DropButton.ShapeData)[2] + y);
    DropButton.Set_Position(Width + x, y);
}


/// <summary>
/// Sets the currently selected item by name.
/// This routine is used when the caller knows the text of the item but not its
/// position in the list. The comparison ignores case, and a name that is not in the
/// list leaves the current selection alone.
/// </summary>
/// <param name="text">The text of the item to select.</param>
// (0x004A54E0, slot 42)
// Selects the first list item equal to text (case-insensitive).

void DropListClass::Set_Selected_Index(wchar_t *text)
{
    if (text != 0) {
        for (int index = 0; index < Count(); index++) {
            if (_wcsicmp(text, List.Get_Item(index)) == 0) {
                Set_Selected_Index(index);
                break;
            }
        }
    }
}


class Leaf_4a5550 { public: unsigned int get(); };
unsigned int Leaf_4a5550::get() { return *(unsigned int*)((char*)this + 0x34); }


// DropListClass::Count (0x004A5560)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.


// DropListClass::Get_Item (0x004A5580)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.


// DropListClass::scalar_deleting_destructor (0x004A55A0)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.


// Implicit ShapeButtonClass copy assignment emitted by DropListClass member assignment.


// Actual owner is SliderClass;implicit copy assignment emitted by DropListClass member assignment.


GadgetClass &GadgetClass::operator=(GadgetClass const &gadget)
{
 LinkClass::operator=(const_cast<GadgetClass &>(gadget));
 X=gadget.X;
 Y=gadget.Y;
 Width=gadget.Width;
 Height=gadget.Height;
 IsToRepaint=gadget.IsToRepaint;
 IsSticky=gadget.IsSticky;
 IsDisabled=gadget.IsDisabled;
 Flags=gadget.Flags;
 return *this;
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<wchar_t *>::operator=; called by ListClass copy construction.


// EditClass::`scalar deleting destructor' (0x004A5780)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.


// ShapeButtonClass::`scalar deleting destructor' (0x004A57A0)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.


// ToggleClass::`scalar deleting destructor' (0x004A57C0)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.
