// audlists.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/audlists.json carries the addresses and the rows that
// stay in their own files.


#include "shared/audio.hpp"


// void __fastcall Audio_List_Init(AudioListNode *list) -- 0x004071F0
// Purpose: makes list an empty list head (links and priority point to
// itself).

void __fastcall Audio_List_Init(AudioListNode *list)
{
    list->Next = list;
    list->Prev = list;
    list->Priority = (int)list;
}


// void __fastcall Audio_List_Init_Node(AudioListNode *node) -- 0x00407200
// Purpose: makes node an unlinked entry with priority 0.

void __fastcall Audio_List_Init_Node(AudioListNode *node)
{
    node->Next = node;
    node->Prev = node;
    node->Priority = 0;
}


// int __fastcall Audio_List_Add_Ascending(AudioListNode *list, AudioListNode *node) -- 0x00407210
// Purpose: links node in before the first entry whose priority is not
// lower than node's (at the tail when there is none) and returns the
// number of entries before it (Audio_List_Next_Unchecked and
// Audio_List_Add_Tail expanded inline).

int __fastcall Audio_List_Add_Ascending(AudioListNode *list, AudioListNode *node)
{
    int priority = node->Priority;
    int position = 0;
    AudioListNode *at = list;
    while ((at = Audio_List_Next_Unchecked(at)) != 0) {
        if (priority <= at->Priority) {
            Audio_List_Add_Tail(at, node);
            return position;
        }
        position++;
    }
    Audio_List_Add_Tail(list, node);
    return position;
}


// void __fastcall Audio_List_Add_Descending(AudioListNode *list, AudioListNode *node) -- 0x00407260
// Purpose: links node in before the first entry whose priority is not
// higher than node's (at the tail when there is none;
// Audio_List_Next_Unchecked and Audio_List_Add_Tail expanded inline).

void __fastcall Audio_List_Add_Descending(AudioListNode *list, AudioListNode *node)
{
    int priority = node->Priority;
    AudioListNode *at = list;
    while ((at = Audio_List_Next_Unchecked(at)) != 0) {
        if (at->Priority <= priority) {
            Audio_List_Add_Tail(at, node);
            return;
        }
    }
    Audio_List_Add_Tail(list, node);
}


// void __fastcall Audio_List_Move_All(AudioListNode *list, AudioListNode *destination) -- 0x004072B0
// Purpose: moves every entry of list to the tail of destination and leaves
// list empty.

void __fastcall Audio_List_Move_All(AudioListNode *list, AudioListNode *destination)
{
    AudioListNode *first = list->Next;
    AudioListNode *last = list->Prev;
    if (first != list) {
        AudioListNode *tail = destination->Prev;
        tail->Next = first;
        first->Prev = tail;
        last->Next = destination;
        destination->Prev = last;
        list->Next = list;
        list->Prev = list;
        list->Priority = (int)list;
    }
}


// int __fastcall Audio_List_Count(AudioListNode *list) -- 0x004072E0
// Purpose: the number of entries in the list.

int __fastcall Audio_List_Count(AudioListNode *list)
{
    int count = 0;
    for (AudioListNode *node = list->Next; node != list; node = node->Next) {
        count++;
    }
    return count;
}


// AudioListNode *__fastcall Audio_List_First(AudioListNode *list) -- 0x004072F0
// Purpose: the first entry of the list, or 0 when it is empty or null.

AudioListNode *__fastcall Audio_List_First(AudioListNode *list)
{
    if (list == 0) {
        return 0;
    }
    AudioListNode *first = list->Next;
    if (first != 0 && (int)first == first->Priority) {
        return 0;
    }
    return first;
}


// AudioListNode *__fastcall Audio_List_Next(AudioListNode *node) -- 0x00407310
// Purpose: the entry after node, or 0 at the end of the list or for a null node.

AudioListNode *__fastcall Audio_List_Next(AudioListNode *node)
{
    if (node == 0) {
        return 0;
    }
    AudioListNode *next = node->Next;
    if (next != 0 && (int)next == next->Priority) {
        return 0;
    }
    return next;
}


// AudioListNode *__fastcall Audio_List_Nth(AudioListNode *list, int index) -- 0x00407330
// Purpose: the entry at position index of the list, or 0 when the list is
// shorter.

AudioListNode *__fastcall Audio_List_Nth(AudioListNode *list, int index)
{
    for (AudioListNode *node = list->Next; node != list; node = node->Next) {
        if (index-- == 0) {
            return node;
        }
    }
    return 0;
}


// void __fastcall Audio_List_Add_Tail(AudioListNode *list, AudioListNode *node) -- 0x00407350
// Purpose: links node in before list (at the tail of the circular list).

void __fastcall Audio_List_Add_Tail(AudioListNode *list, AudioListNode *node)
{
    node->Prev = list->Prev;
    node->Next = list;
    list->Prev = node;
    node->Prev->Next = node;
}


// void __fastcall Audio_List_Add_Head(AudioListNode *list, AudioListNode *node) -- 0x00407370
// Purpose: links node in after list (at the head of the circular list).

void __fastcall Audio_List_Add_Head(AudioListNode *list, AudioListNode *node)
{
    node->Prev = list;
    node->Next = list->Next;
    list->Next = node;
    node->Next->Prev = node;
}


// void __fastcall Audio_List_Remove(AudioListNode *node) -- 0x00407380
// Purpose: unlinks node from its list and leaves it linked to itself.

void __fastcall Audio_List_Remove(AudioListNode *node)
{
    node->Prev->Next = node->Next;
    node->Next->Prev = node->Prev;
    node->Next = node;
    node->Prev = node;
}


// AudioListNode *__fastcall Audio_List_Next_Unchecked(AudioListNode *node) -- 0x004073A0
// Purpose: the entry after node, or 0 at the end of the list (node must
// not be null).

AudioListNode *__fastcall Audio_List_Next_Unchecked(AudioListNode *node)
{
    AudioListNode *next = node->Next;
    if (next != 0 && (int)next == next->Priority) {
        return 0;
    }
    return next;
}


// AudioListNode *__fastcall Audio_List_Last(AudioListNode *list) -- 0x004073B0
// Purpose: the last entry of the list, or 0 when it is empty.

AudioListNode *__fastcall Audio_List_Last(AudioListNode *list)
{
    AudioListNode *last = list->Prev;
    return (int)last == last->Priority ? 0 : last;
}


// 0x004073c0: function not yet identified
//
// Body: 17 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: int * __fastcall FUN_004073c0(int *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004073c0 {

int * __fastcall FUN_004073c0(int *node_words)
{
    node_words = (int *)*node_words;
    if ((node_words == (int *)node_words[2]) && (node_words = (int *)*node_words, node_words == (int *)node_words[2])) {
        node_words = (int *)0x0;
    }
    return node_words;
}

}  // namespace seed_004073c0


// 0x004073e0: function not yet identified
//
// Body: 19 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: int __fastcall FUN_004073e0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004073e0 {

int __fastcall FUN_004073e0(int list_address)
{
    int node_address;

    node_address = (int)(*(int *)(list_address + 4));
    if ((node_address == *(int *)(node_address + 8)) && (node_address = *(int *)(node_address + 4), node_address == *(int *)(node_address + 8)))
    {
        node_address = (int)(0);
    }
    return node_address;
}

}  // namespace seed_004073e0
