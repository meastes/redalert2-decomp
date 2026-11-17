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
// Function comment blocks from mischa85/RedAlert2 392e904 src/list.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/list.cpp where both define the function (names only).

// list.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/list.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/gadget.hpp"
#include "shared/mouse.hpp"


// Header-static dynamic initializer of the unit at 0x00539BD0: the empty
// rectangle of shared/shape_file.hpp (0x00A6E9A0), the unit's first function.


/***************************************************************************
 * ListClass::ListClass -- class constructor                               *
 *                                                                         *
 * INPUT:            id               button ID                            *
 *                                                                         *
 *                     x,y            upper-left corner, in pixels         *
 *                                                                         *
 *                     w,h            width, height, in pixels             *
 *                                                                         *
 *                     list            ptr to array of char strings to list*
 *                                                                         *
 *                   flags, style   flags for mouse, style of listbox      *
 *                                                                         *
 * OUTPUT:           none.                                                 *
 *                                                                         *
 * WARNINGS:         none.                                                 *
 *                                                                         *
 * HISTORY:          01/05/1995 MML : Created.                             *
 *=========================================================================*/
// (0x00539BF0)
// The arrows sit inside the right edge (up at the top, down at the bottom)
// with the scroll bar between them; the bar is only linked in when needed.

ListClass::ListClass(int id, int x, int y, int w, int h, unsigned flags, void const *up, void const *down) :
    ControlClass(id, x, y, w, h, LEFTPRESS | LEFTRELEASE | KEYBOARD, false),
    UpGadget(0, up, x + w, y),
    DownGadget(0, down, x + w, y + h),
    ScrollGadget(0, x + w, y, 0, h, true)
{
    UpGadget.X -= UpGadget.Width;
    DownGadget.X -= DownGadget.Width;
    DownGadget.Y -= DownGadget.Height;
    ScrollGadget.X -= (UpGadget.Width > DownGadget.Width) ? UpGadget.Width : DownGadget.Width;
    ScrollGadget.Y = Y + UpGadget.Height;
    ScrollGadget.Height -= UpGadget.Height + DownGadget.Height;
    ScrollGadget.Width = (UpGadget.Width > DownGadget.Width) ? UpGadget.Width : DownGadget.Width;

    TextFlags = flags;
    IsScrollActive = false;
    Tabs = 0;
    SelectedIndex = 0;
    CurrentTopIndex = 0;
    LineHeight = Font_For_Style(flags)->Height - 1;
    LineCount = (h - 1) / LineHeight;
}


/// <summary>
/// Copy constructor for list box objects.
/// The copy takes over the source list's entries and scroll bar settings, but the scroll bar
/// gadgets are made peers of the new list box so that they report to it rather than to the
/// list they were copied from.
/// </summary>
/// <param name="list">The list box to copy.</param>
// (0x00539D40)
// A copy whose arrows and scroll bar report to the new list.

ListClass::ListClass(ListClass const &list) :
    ControlClass(list),
    TextFlags(list.TextFlags),
    Tabs(list.Tabs),
    List(list.List),
    LineHeight(list.LineHeight),
    LineCount(list.LineCount),
    IsScrollActive(list.IsScrollActive),
    UpGadget(list.UpGadget),
    DownGadget(list.DownGadget),
    ScrollGadget(list.ScrollGadget),
    SelectedIndex(list.SelectedIndex),
    CurrentTopIndex(list.CurrentTopIndex)
{
    UpGadget.Make_Peer(*this);
    DownGadget.Make_Peer(*this);
    ScrollGadget.Make_Peer(*this);
}


// void ListClass::Set_Position(int x, int y) (0x00539F30, slot 25)
// Moves the list and pins its arrow buttons and slider to its right hand edge
// (TibSun list.cpp, text unchanged).

void ListClass::Set_Position(int x, int y)
{
    UpGadget.X = x + Width - UpGadget.Width;
    UpGadget.Y = y;
    DownGadget.X = x + Width - DownGadget.Width;
    DownGadget.Y = y + Height - DownGadget.Height;
    ScrollGadget.X = x + Width - (UpGadget.Width > DownGadget.Width ? UpGadget.Width : DownGadget.Width);
    ScrollGadget.Y = y + UpGadget.Height;
    ScrollGadget.Height = Height - (UpGadget.Height + DownGadget.Height);
    ScrollGadget.Width = UpGadget.Width > DownGadget.Width ? UpGadget.Width : DownGadget.Width;
}


/***********************************************************************************************
 * ListClass::~ListClass -- Destructor for list class objects.                                 *
 *                                                                                             *
 *    This is the destructor for list objects. It handles removing anything it might have      *
 *    allocated. This is typically the scroll bar.                                             *
 *                                                                                             *
 * INPUT:      none                                                                            *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/16/1995 JLB : Created.                                                       *
 *=============================================================================================*/
// (0x00539FC0)

ListClass::~ListClass()
{
    Remove_Scroll_Bar();
}


/***********************************************************************************************
 * ListClass::Add_Item -- Adds an item to the list box.                                        *
 *                                                                                             *
 *    This will add the specified string to the list box. The string is added to the end       *
 *    of the list.                                                                             *
 *                                                                                             *
 * INPUT:      text  -- Pointer to the string to add to the list box.                          *
 * OUTPUT:     none                                                                            *
 * WARNINGS:   none                                                                            *
 * HISTORY:    01/15/1995 JLB : Created.                                                       *
 *=============================================================================================*/
// (0x0053A060, slot 34)
// Appends text (null is ignored); returns the index of the last item.

int ListClass::Add_Item(wchar_t *text)
{
    if (text != 0) {
        List.Add(text);
        Flag_To_Redraw();
        if (List.Count() > LineCount) {
            Add_Scroll_Bar();
        }
        if (IsScrollActive) {
            ScrollGadget.Set_Maximum(List.Count());
        }
    }
    return List.Count() - 1;
}


/// <summary>
/// Removes the entry at the index specified.
/// This routine will drop the entry from the list box and then bring the scroll bar and the
/// selection back into agreement with the shortened list. An index that is not in the list
/// is quietly ignored.
/// </summary>
/// <param name="index">The index of the entry to remove.</param>
// (0x0053A0F0, slot 42)
// Removes an item and keeps the selection and view inside the list.

void ListClass::Remove_Item(int index)
{
    if ((unsigned)index < (unsigned)List.Count()) {
        List.Delete(index);
        if (List.Count() <= LineCount) {
            Remove_Scroll_Bar();
        }
        if (IsScrollActive) {
            ScrollGadget.Set_Maximum(List.Count());
        }
        if (SelectedIndex >= List.Count()) {
            SelectedIndex--;
            if (SelectedIndex < 0) {
                SelectedIndex = 0;
            }
        }
        if (CurrentTopIndex >= List.Count()) {
            CurrentTopIndex--;
            if (CurrentTopIndex < 0) {
                CurrentTopIndex = 0;
            }
            if (IsScrollActive) {
                ScrollGadget.Step(1);
            }
        }
    }
}


/***********************************************************************************************
 * ListClass::Remove_Item -- Remove specified text from list box.                              *
 *                                                                                             *
 *    This routine will remove the specified text string from the list box.                    *
 *                                                                                             *
 * INPUT:      text  -- Pointer to the string to remove.                                       *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * WARNINGS:   The text pointer passed into this routine MUST be the same text pointer that    *
 *             was used to add the string to the list.                                         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A1C0, slot 43)

void ListClass::Remove_Item(wchar_t *text)
{
    if (text != 0) {
        Remove_Item(List.ID(text));
    }
}


/***************************************************************************
 * ListClass::Action -- If clicked on, do this!                            *
 *                                                                         *
 * INPUT:            int flags -- combination of mouse flags indicating    *
 *                                  what action to take.                   *
 *                                                                         *
 * OUTPUT:           bool result.                                          *
 *                                                                         *
 * WARNINGS:         none.                                                 *
 *                                                                         *
 * HISTORY:          01/05/1995 MML : Created.                             *
 *=========================================================================*/
// (0x0053A1F0, slot 31)
// A release ends the click; the up/down keys step the selection; a click
// selects the line under the mouse.

int ListClass::Action(unsigned flags, unsigned &key)
{
    if (flags & LEFTRELEASE) {
        key = 0;
        flags &= ~LEFTRELEASE;
        ControlClass::Action(flags, key);
        return true;
    } else {
        if (flags & KEYBOARD) {
            if (key == 0x26) {
                Step_Selected_Index(-1);
                key = 0;
            } else if (key == 0x28) {
                Step_Selected_Index(1);
                key = 0;
            } else {
                flags &= ~KEYBOARD;
            }
        } else {
            SelectedIndex = CurrentTopIndex + (WWMouse->Get_Mouse_Y() - Y - 1) / LineHeight;
            SelectedIndex = (SelectedIndex < List.Count() - 1) ? SelectedIndex : List.Count() - 1;
            if (SelectedIndex == -1) {
                SelectedIndex = 0;
            }
        }
    }
    return ControlClass::Action(flags, key);
}


/***********************************************************************************************
 * ListClass::Draw_Me -- Draws the listbox.                                                    *
 *                                                                                             *
 *    This routine will render the listbox.                                                    *
 *                                                                                             *
 * INPUT:   forced   -- Should the listbox be redrawn even if it already thinks it doesn't     *
 *                      need to be? This is true when something outside of the gadget system   *
 *                      has trashed the screen.                                                *
 *                                                                                             *
 * OUTPUT:  Was the listbox redrawn?                                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A2E0, slot 27)
// Draws the frame and the visible lines with the mouse hidden over them.

int ListClass::Draw_Me(int forced)
{
    if (GadgetClass::Draw_Me(forced)) {
        WWMouse->Conditional_Hide_Mouse(Rect(X, Y, Width, Height));
        Draw_Box(Rect(X, Y, Width, Height), 4, true);
        if (List.Count()) {
            for (int index = 0; index < LineCount; index++) {
                int line = CurrentTopIndex + index;
                if (List.Count() > line) {
                    Draw_Entry(line, X + 1, Y + LineHeight * index + 1, Width - 2, line == SelectedIndex);
                }
            }
        }
        WWMouse->Conditional_Show_Mouse();
        return true;
    }
    return false;
}


/***********************************************************************************************
 * ListClass::Bump -- Bumps the list box up/down one "page".                                   *
 *                                                                                             *
 *    Use this routine to adjust the "page" that is being viewed in the list box. The view     *
 *    will move up or down (as specified) one page (screen full) of text strings.              *
 *                                                                                             *
 * INPUT:   up -- Should the adjustment be up?                                                 *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A3E0, slot 36)

void ListClass::Bump(int up)
{
    if (IsScrollActive) {
        if (ScrollGadget.Step(up)) {
            CurrentTopIndex = ScrollGadget.Get_Value();
            Flag_To_Redraw();
        }
    }
}


/***********************************************************************************************
 * ListClass::Step -- Moves the list view one line in direction specified.                     *
 *                                                                                             *
 *    This routine will move the current view "page" one line in the direction specified.      *
 *                                                                                             *
 * INPUT:   up -- Should the view be moved upward?                                             *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A430, slot 49)

void ListClass::Step(int up)
{
    if (IsScrollActive) {
        if (ScrollGadget.Step(up)) {
            CurrentTopIndex = ScrollGadget.Get_Value();
            Flag_To_Redraw();
        }
    }
}


/***********************************************************************************************
 * ListClass::Get_Item -- Fetches an arbitrary item string.                                    *
 *                                                                                             *
 *    This routine will fetch an item string from the list box. The item fetched can be any    *
 *    one of the ones in the list.                                                             *
 *                                                                                             *
 * INPUT:   index -- The index to examine and return the text pointer from.                    *
 *                                                                                             *
 * OUTPUT:  Returns with the text pointer to the string at the index position specified.       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A480, slot 40)

wchar_t *ListClass::Get_Item(int index) const
{
    if (List.Count() == 0) {
        return 0;
    }
    int last = List.Count() - 1;
    if (index >= last) {
        index = last;
    }
    return List[index];
}


/***********************************************************************************************
 * ListClass::Current_Item -- Fetches pointer to current item string.                          *
 *                                                                                             *
 *    This routine will fetch a pointer to the currently selected item's text.                 *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Return with pointer to currently selected text.                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A4A0, slot 39)

wchar_t *ListClass::Current_Item() const
{
    if (List.Count() <= SelectedIndex) {
        return 0;
    }
    return List[SelectedIndex];
}


/***********************************************************************************************
 * ListClass::Current_Index -- Fetches the current selected index.                             *
 *                                                                                             *
 *    This routine will fetch the index number for the currently selected line.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the index of the currently selected line. This ranges from zero to    *
 *          the number of items in the list minus one.                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A4C0, slot 38)

int ListClass::Current_Index() const
{
    return SelectedIndex;
}


/***********************************************************************************************
 * ListClass::Peer_To_Peer -- A peer gadget was touched -- make adjustments.                   *
 *                                                                                             *
 *    This routine is called when one of the peer gadgets (the scroll arrows or the slider)    *
 *    was touched in some fashion. This routine will sort out whom and why and then make       *
 *    any necessary adjustments to the list box.                                               *
 *                                                                                             *
 * INPUT:   flags    -- The event flags that affected the peer gadget.                         *
 *                                                                                             *
 *          key      -- The key value at the time of the event.                                *
 *                                                                                             *
 *          whom     -- Which gadget is being touched.                                         *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A4D0, slot 19)
// Arrow releases step the view; a scroll bar change moves it.

void ListClass::Peer_To_Peer(unsigned flags, unsigned &key, ControlClass &whom)
{
    if (flags & LEFTRELEASE) {
        if (&whom == &UpGadget) {
            Step(true);
        }
        if (&whom == &DownGadget) {
            Step(false);
        }
    }
    if (&whom == &ScrollGadget) {
        Set_View_Index(ScrollGadget.Get_Value());
    }
}


/***********************************************************************************************
 * ListClass::Set_View_Index -- Sets the top line for the current list view.                   *
 *                                                                                             *
 *    This routine is used to set the line that will be at the top of the list view. This is   *
 *    how the view can be scrolled up and down. This does not affect the currently selected    *
 *    item.                                                                                    *
 *                                                                                             *
 * INPUT:   index -- The line (index) to move to the top of the list view.                     *
 *                                                                                             *
 * OUTPUT:  bool; Was the view actually changed?                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A530, slot 48)
// Shows the list from item index (clamped so the last page stays full).

int ListClass::Set_View_Index(int index)
{
    int last = List.Count() - LineCount;
    last = (last < 0) ? 0 : last;
    if (index < 0) {
        index = 0;
    } else if (index > last) {
        index = last;
    }
    if (index != CurrentTopIndex) {
        CurrentTopIndex = index;
        Flag_To_Redraw();
        if (IsScrollActive) {
            ScrollGadget.Set_Value(CurrentTopIndex);
        }
        return true;
    }
    return false;
}


/***********************************************************************************************
 * ListClass::Add_Scroll_Bar -- Adds a scroll bar to the list box.                             *
 *                                                                                             *
 *    This routine will add a scroll bar (with matching arrows) to the list box. They are      *
 *    added to the right edge and cause the interior of the list box to become narrower.       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the scroll bar added?                                                    *
 *                                                                                             *
 * WARNINGS:   The list box becomes narrower when the scroll bar is added.                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A5A0, slot 35)
// Narrows the list and links the arrows and scroll bar in after it.

int ListClass::Add_Scroll_Bar()
{
    if (!IsScrollActive) {
        IsScrollActive = true;
        Flag_To_Redraw();
        Width -= ScrollGadget.Width;
        UpGadget.Make_Peer(*this);
        DownGadget.Make_Peer(*this);
        ScrollGadget.Make_Peer(*this);
        UpGadget.Add(*this);
        DownGadget.Add(*this);
        ScrollGadget.Add(*this);
        UpGadget.Flag_To_Redraw();
        DownGadget.Flag_To_Redraw();
        ScrollGadget.Flag_To_Redraw();
        ScrollGadget.Set_Maximum(List.Count());
        ScrollGadget.Set_Thumb_Size(LineCount);
        ScrollGadget.Set_Value(CurrentTopIndex);
        return true;
    }
    return false;
}


/***********************************************************************************************
 * ListClass::Remove_Scroll_Bar -- Removes the scroll bar if present                           *
 *                                                                                             *
 *    Use this routine to remove any attached scroll bar to this list box. If the scroll bar   *
 *    is not present, then no action occurs.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the scroll bar removed?                                                  *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A670, slot 44)

int ListClass::Remove_Scroll_Bar()
{
    if (IsScrollActive) {
        Width += ScrollGadget.Width;
        IsScrollActive = false;
        ScrollGadget.Remove();
        UpGadget.Remove();
        DownGadget.Remove();
        Flag_To_Redraw();
        return true;
    }
    return false;
}


/***********************************************************************************************
 * ListClass::Set_Tabs -- Sets the tab stop list to be used for text printing.                 *
 *                                                                                             *
 *    This sets the tab stop list to be used for text printing. It specifies a series of       *
 *    pixel offsets for each tab stop. The offsets are from the starting pixel position that   *
 *    the text begins at.                                                                      *
 *                                                                                             *
 * INPUT:   tabs  -- Pointer to a list of tab pixel offsets.                                   *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   Only a pointer to the tabs is recorded by the ListClass object. Make sure that  *
 *             the list remains intact for the duration of the existence of this object.       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A6D0, slot 47)

void ListClass::Set_Tabs(int const *tabs)
{
    Tabs = tabs;
}


/***********************************************************************************************
 * ListClass::Draw_Entry -- Draws a list box text line as indicated.                           *
 *                                                                                             *
 *    This routine is called by the Draw_Me function when it desired to redraw a particular    *
 *    text line in the list box.                                                               *
 *                                                                                             *
 * INPUT:   index    -- The index of the list entry to draw. This index is based on the        *
 *                      total list and NOT the current visible view page.                      *
 *                                                                                             *
 *          x,y      -- Pixel coordinates for the upper left corner of the text entry.         *
 *                                                                                             *
 *          width    -- The maximum width that the text may draw over. It is expected that     *
 *                      this drawing routine entirely fills this length.                       *
 *                                                                                             *
 *          selected -- bool; Is this a selected (highlighted) listbox entry?                  *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 * WARNINGS:   none                                                                            *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A6E0, slot 50)
// Prints one entry clipped to `width` with the list's tab stops in the box
// colour scheme: a selected entry is bright (0x2000) on a bar of the
// scheme's palette index +0x31C; others get 0x1000 unless their flags have
// 0x4000 (Westwood's ListClass::Draw_Entry shape).

void ListClass::Draw_Entry(int index, int x, int y, int width, int selected)
{
    unsigned flags = TextFlags;
    int scheme = Gadget_Color_Scheme();

    if (selected) {
        flags |= 0x2000;
        LogicalSurface->Fill_Rect(Rect(x, y, width, LineHeight),
                                          NormalDrawer->Convert(ColorSchemes[scheme]->unknown_color_0x31c));
    } else if (!(flags & 0x4000)) {
        flags |= 0x1000;
    }
    Point2D point(x, y);
    Fancy_Clip_Text_Print(List[index], LogicalSurface, &LogicalSurface->Get_Rect(), &point,
                          ColorSchemes[scheme], 0, flags, width, Tabs);
}


/***********************************************************************************************
 * ListClass::Add -- Adds myself to list immediately after given object                        *
 *                                                                                             *
 * Adds the list box to the chain, immediately after the given object.  The order will be:     *
 * - Listbox                                                                                   *
 * - Up arrow (if active)                                                                      *
 * - Down arrow (if active)                                                                    *
 * - Scroll gadget (if active)                                                                 *
 *                                                                                             *
 * INPUT:   object   -- Pointer to the object to be added right after this one.                *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the head of the list.                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A7D0, slot 3)
// The scroll parts follow the list.

LinkClass &ListClass::Add(LinkClass &list)
{
    if (IsScrollActive) {
        ScrollGadget.Add(list);
        DownGadget.Add(list);
        UpGadget.Add(list);
    }
    return ControlClass::Add(list);
}


/***********************************************************************************************
 * ListClass::Add_Head -- Adds myself to head of the given list                                *
 *                                                                                             *
 * INPUT:   list -- list to add myself to                                                      *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the object at the head of the list. This should be     *
 *          the same object that is passed in.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A820, slot 5)

LinkClass &ListClass::Add_Head(LinkClass &list)
{
    if (IsScrollActive) {
        ScrollGadget.Add_Head(list);
        DownGadget.Add_Head(list);
        UpGadget.Add_Head(list);
    }
    return ControlClass::Add_Head(list);
}


/***********************************************************************************************
 * ListClass::Add_Tail -- Adds myself to tail of given list                                    *
 *                                                                                             *
 * Adds the list box to the tail of the give chain.  The order will be:                        *
 * - Listbox                                                                                   *
 * - Up arrow (if active)                                                                      *
 * - Down arrow (if active)                                                                    *
 * - Scroll gadget (if active)                                                                 *
 *                                                                                             *
 * INPUT:   list -- list to add myself to                                                      *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The previous and next pointers for the added object MUST have been properly     *
 *             initialized for this routine to work correctly.                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A870, slot 4)

LinkClass &ListClass::Add_Tail(LinkClass &list)
{
    ControlClass::Add_Tail(list);
    if (IsScrollActive) {
        UpGadget.Add_Tail(list);
        DownGadget.Add_Tail(list);
        ScrollGadget.Add_Tail(list);
    }
    return Head_Of_List();
}


/***********************************************************************************************
 * ListClass::Remove -- Removes the specified object from the list.                            *
 *                                                                                             *
 *    This routine will remove the specified object from the list of objects. Because of the   *
 *    previous and next pointers, it is possible to remove an object from the list without     *
 *    knowing the head of the list. To do this, just call Remove() with the parameter of       *
 *    "this".                                                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the new head of list.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A8C0, slot 9)

LinkClass *ListClass::Remove()
{
    if (IsScrollActive) {
        ScrollGadget.Remove();
        DownGadget.Remove();
        UpGadget.Remove();
    }
    return ControlClass::Remove();
}


/***********************************************************************************************
 * ListClass::Set_Selected_Index -- Set the top of the listbox to index specified.             *
 *                                                                                             *
 *    This routine will set the top line of the listbox to the index value specified.          *
 *                                                                                             *
 * INPUT:   index -- The index to set the top of the listbox to.                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   The requested index may be adjusted to fit within legal parameters.             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *   01/23/1996 JLB : Forces selected index to always be zero for a null list.                 *
 *=============================================================================================*/
// (0x0053A900, slot 46)
// Selects an item and scrolls it into view (invalid indices select 0).

void ListClass::Set_Selected_Index(int index)
{
    if ((unsigned)index < (unsigned)List.Count()) {
        SelectedIndex = index;
        Flag_To_Redraw();
        if (SelectedIndex < CurrentTopIndex) {
            Set_View_Index(SelectedIndex);
        }
        if (SelectedIndex >= CurrentTopIndex + LineCount) {
            Set_View_Index(SelectedIndex - LineCount + 1);
        }
    } else {
        SelectedIndex = 0;
    }
}


/***********************************************************************************************
 * ListClass::Step_Selected_Index -- Change the listbox top line in direction specified.       *
 *                                                                                             *
 *    This routine will scroll the top line of the listbox in the direction specified.         *
 *                                                                                             *
 * INPUT:   step  -- The direction (and amount) to adjust the listbox. If negative value, then *
 *                   the top line is scrolled upward.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with the original top line index number.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   06/25/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053A970, slot 41)
// Moves the selection by forward; returns the old index.

int ListClass::Step_Selected_Index(int step)
{
    int old = SelectedIndex;
    Set_Selected_Index(old + step);
    return old;
}


/// <summary>
/// Flags the list box to be redrawn.
/// This routine marks the list box, and any scroll bar gadgets it owns, as needing to be
/// redrawn at the next opportunity. The scroll bar pieces are peers rather than children,
/// so they must be flagged along with the list itself.
/// </summary>
// (0x0053A990, slot 18)

void ListClass::Flag_To_Redraw()
{
    if (IsScrollActive) {
        UpGadget.Flag_To_Redraw();
        DownGadget.Flag_To_Redraw();
        ScrollGadget.Flag_To_Redraw();
    }
    ControlClass::Flag_To_Redraw();
}


/// <summary>
/// Sets the selected entry to the one matching the text specified.
/// This routine is used when the caller knows what the entry says but not where it sits in
/// the list. The comparison ignores case. If no entry matches, the selection is left alone.
/// </summary>
/// <param name="text">The text of the entry to select.</param>
// (0x0053A9D0, slot 45)
// Selects the first item equal to text (case-insensitive).

void ListClass::Set_Selected_Index(wchar_t *text)
{
    if (text != 0 && List.Count() > 0) {
        for (int index = 0; index < List.Count(); index++) {
            if (_wcsicmp(List[index], text) == 0) {
                Set_Selected_Index(index);
                break;
            }
        }
    }
}


// void *ListClass::`scalar deleting destructor'(unsigned int) (0x0053AA20)
// Compiler-emitted from shared/gadget.hpp in this unit (destructor inlined); no source of its own.


// SliderClass::Thumb_Pixels (0x0053AAE0)
// Compiler-emitted from shared/gadget.hpp in this unit (kept copy); no source of its own.


// DynamicVectorClass<unsigned short *>::`scalar deleting destructor' (0x0053AAF0)
// Compiler-emitted from shared/vector_class.hpp in this unit; no source of its own.


// SliderClass::scalar_deleting_destructor (0x0053AB40)
// Compiler-emitted from shared/gadget.hpp in this unit (kept copy); no source of its own.


// GaugeClass::copy_constructor (0x0053AB60)
// Compiler-emitted from shared/gadget.hpp in this unit (kept copy); no source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<unsigned short *>::Clear (0x0053ABA0; DynamicVectorClass<unsigned short *> vtable 0x007a5714 slot 3).
// DynamicVectorClass<unsigned short *>::ID_pointer (0x0053ABD0; DynamicVectorClass<unsigned short *> vtable 0x007a5714 slot 5).
// VectorClass<unsigned short *>::Fetch (0x0053ABF0; VectorClass<unsigned short *> vtable 0x007a5734 slot 6).
// VectorClass<unsigned short *>::operator equal (0x0053AC00; VectorClass<unsigned short *> vtable 0x007a5734 slot 1).
// VectorClass<unsigned short *>::Resize (0x0053AC40; VectorClass<unsigned short *> vtable 0x007a5734 slot 2).
// VectorClass<unsigned short *>::Clear (0x0053ACF0; VectorClass<unsigned short *> vtable 0x007a5734 slot 3).
// VectorClass<unsigned short *>::ID_object (0x0053AD20; VectorClass<unsigned short *> vtable 0x007a5734 slot 4).
// VectorClass<unsigned short *>::ID_pointer (0x0053AD60; VectorClass<unsigned short *> vtable 0x007a5734 slot 5).
// DynamicVectorClass<unsigned short *>::Resize (0x0053AD80; DynamicVectorClass<unsigned short *> vtable 0x007a5714 slot 2).
// DynamicVectorClass<unsigned short *>::ID_object (0x0053AE30; DynamicVectorClass<unsigned short *> vtable 0x007a5714 slot 4).


// VectorClass<unsigned short *>::`scalar deleting destructor' (0x0053AE60)
// Compiler-emitted from shared/vector_class.hpp in this unit; no source of its own.
