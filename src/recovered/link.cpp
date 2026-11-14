// link.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/link.json carries the addresses and the rows that
// stay in their own files.


#include "shared/gadget.hpp"


// LinkClass::LinkClass(LinkClass &link) (0x00538F60)
// A new node added after link in link's list.

LinkClass::LinkClass(LinkClass &link) :
    Next(0),
    Prev(0)
{
    Add(link);
}


// LinkClass::~LinkClass() (0x00538FA0)
// Unlinks the node.

LinkClass::~LinkClass()
{
    Remove();
}


// void LinkClass::Zap() (0x00538FF0)
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


// LinkClass *LinkClass::Get_Next() const (0x00539020)

LinkClass *LinkClass::Get_Next() const
{
    return Next;
}


// LinkClass *LinkClass::Get_Prev() const (0x00539030)

LinkClass *LinkClass::Get_Prev() const
{
    return Prev;
}


// LinkClass &LinkClass::Head_Of_List() (0x00539040)
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


// LinkClass &LinkClass::Tail_Of_List() (0x00539070)
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


// LinkClass &LinkClass::Add(LinkClass &list) (0x005390A0)
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


// LinkClass &LinkClass::Add_Head(LinkClass &list) (0x005390C0)
// Puts the node in front of list's head.

LinkClass &LinkClass::Add_Head(LinkClass &list)
{
    LinkClass *head = &list.Head_Of_List();
    head->Prev = this;
    Next = head;
    Prev = 0;
    return *this;
}


// LinkClass &LinkClass::Add_Tail(LinkClass &list) (0x005390E0)
// Appends the node after list's tail; returns the head of the list.

LinkClass &LinkClass::Add_Tail(LinkClass &list)
{
    LinkClass *tail = &list.Tail_Of_List();
    tail->Next = this;
    Prev = tail;
    Next = 0;
    return Head_Of_List();
}


// LinkClass *LinkClass::Remove() (0x00539110)
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
