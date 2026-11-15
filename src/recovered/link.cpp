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
// Function comment blocks from mischa85/RedAlert2 392e904 src/link.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// link.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/link.json carries the addresses and the rows that
// stay in their own files.


#include "shared/gadget.hpp"


/***********************************************************************************************
 * LinkClass::LinkClass -- Copy constructor for linked list object.                            *
 *                                                                                             *
 *    This copy constructor, unlike the assignment operator, doesn't have to deal with an      *
 *    already initialized and legal link object to the left of the "=". It merely puts the     *
 *    destination object into the same list as the source object.                              *
 *                                                                                             *
 * INPUT:   link  -- Reference to the object on the right of the "=".                          *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/16/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00538F60)
// A new node added after link in link's list.

LinkClass::LinkClass(LinkClass &link) :
    Next(0),
    Prev(0)
{
    Add(link);
}


/***********************************************************************************************
 * LinkClass::~LinkClass -- Default destructor for linked list object.                         *
 *                                                                                             *
 *    This default destructor will remove the object from any linked list it may be part of.   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00538FA0)
// Unlinks the node.

LinkClass::~LinkClass()
{
    Remove();
}


/***********************************************************************************************
 * LinkClass::Zap -- Forces the link pointers to NULL.                                         *
 *                                                                                             *
 *    This routine will "zap" out the link pointers. This is usually necessary when the link   *
 *    pointers start in an undefined state, but we KNOW that they aren't pointing to anything  *
 *    valid. In such a case it becomes necessary to zap them so that when the object is added  *
 *    to a list, it will be added correctly.                                                   *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00538FF0)
// Forgets both neighbours without unlinking them.

void LinkClass::Zap()
{
    Next = 0;
    Prev = 0;
}


// LinkClass &LinkClass::operator=(LinkClass &link) (0x00539000)
// Leaves the current list and joins link's.

LinkClass &LinkClass::operator=(LinkClass &link)
{
    if (&link != this) {
        Remove();
        Add(link);
    }
    return *this;
}


/***********************************************************************************************
 * LinkClass::Get_Next -- Fetches the next object in list.                                     *
 *                                                                                             *
 *    This routine will return with a pointer to the next object in the list. If there are     *
 *    no more objects, then NULL is returned.                                                  *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with pointer to next object in list or NULL if at end of list.             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00539020)

LinkClass *LinkClass::Get_Next() const
{
    return Next;
}


/***********************************************************************************************
 * LinkClass::Get_Prev -- Fetches previous object in linked list.                              *
 *                                                                                             *
 *    Use this routine to get a pointer to the previous object in the linked list. If there    *
 *    are no previous objects (such as at the head of the list), then NULL is returned.        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the previous object in the list or NULL if none.         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00539030)

LinkClass *LinkClass::Get_Prev() const
{
    return Prev;
}


/***********************************************************************************************
 * LinkClass::Head_Of_List -- Finds the head of the list.                                      *
 *                                                                                             *
 *    Use this routine to scan for and return a reference to the object at the head of the     *
 *    list.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the object at the head of the list.                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00539040)
// The first node (following Prev; stops if the list loops back).

LinkClass &LinkClass::Head_Of_List()
{
    LinkClass *link = this;
    while (link->Prev != 0) {
        link = link->Prev;
        if (link == this) {
            break;
        }
    }
    return *link;
}


/***********************************************************************************************
 * LinkClass::Tail_Of_List -- Scans for the object at the end of the list.                     *
 *                                                                                             *
 *    Use this routine to scan for and return a reference to the object at the end of the      *
 *    list.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the object at the end of the list.                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00539070)
// The last node (following Next; stops if the list loops back).

LinkClass &LinkClass::Tail_Of_List()
{
    LinkClass *link = this;
    while (link->Next != 0) {
        link = link->Next;
        if (link == this) {
            break;
        }
    }
    return *link;
}


/***********************************************************************************************
 * LinkClass::Add -- This object adds itself to the given list                                 *
 *                                                                                             *
 *    Use this routine to add a link object to the list, but to be added right after the       *
 *    given link. This allows inserting a link in the middle of the chain. A quite necessary   *
 *    ability if the chain is order dependant (e.g., the gadget system).                       *
 *                                                                                             *
 * INPUT:   list -- gadget object to add this one to                                           *
 *                                                                                             *
 * OUTPUT:  Returns with a pointer to the head of the list.                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x005390A0)
// Inserts the node after list; returns the head of the list.

LinkClass &LinkClass::Add(LinkClass &list)
{
    LinkClass *next = list.Next;
    list.Next = this;
    Prev = &list;
    Next = next;
    if (next != 0) {
        next->Prev = this;
    }
    return Head_Of_List();
}


/***********************************************************************************************
 * LinkClass::Add_Head -- This gadget makes itself the head of the given list.                 *
 *                                                                                             *
 * INPUT:   list -- the list to make myself the head of                                        *
 *                                                                                             *
 * OUTPUT:  Returns with a reference to the object at the head of the list. This should be     *
 *          the same object that is passed in.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/19/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x005390C0)
// Puts the node in front of list's head.

LinkClass &LinkClass::Add_Head(LinkClass &list)
{
    LinkClass *head = &list.Head_Of_List();
    head->Prev = this;
    Next = head;
    Prev = 0;
    return *this;
}


/***********************************************************************************************
 * LinkClass::Add_Tail -- Add myself to the end of the given list.                             *
 *                                                                                             *
 * INPUT:   list -- list to add myself to                                                      *
 *                                                                                             *
 * OUTPUT:  the head of the list                                                               *
 *                                                                                             *
 * WARNINGS:   The previous and next pointers for the added object MUST have been properly     *
 *             initialized for this routine to work correctly.                                 *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   01/15/1995 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x005390E0)
// Appends the node after list's tail; returns the head of the list.

LinkClass &LinkClass::Add_Tail(LinkClass &list)
{
    LinkClass *tail = &list.Tail_Of_List();
    tail->Next = this;
    Prev = tail;
    Next = 0;
    return Head_Of_List();
}


/***********************************************************************************************
 * LinkClass::Remove -- Removes the specified object from the list.                            *
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
// (0x00539110)
// Unlinks the node; returns the head of what remains (null when it was alone).

LinkClass *LinkClass::Remove()
{
    LinkClass *head = &Head_Of_List();
    LinkClass *tail = &Tail_Of_List();

    if (Prev != 0) {
        Prev->Next = Next;
    }
    if (Next != 0) {
        Next->Prev = Prev;
    }
    Prev = 0;
    Next = 0;

    if (head == this) {
        if (tail == this) {
            return 0;
        }
        return &tail->Head_Of_List();
    }
    return head;
}
