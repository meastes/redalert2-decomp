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
// Function comment blocks from mischa85/RedAlert2 392e904 src/tooltip.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// tooltip.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/tooltip.json carries the addresses and the rows that
// stay in their own files.


#define TOOL_TIP_UNIT
#include "shared/tool_tip.hpp"
#include "shared/debug_printf.hpp"
#include "shared/display.hpp"


/// <summary>
/// Constructs an empty tooltip manager for the window specified.
/// The manager starts out deactivated and carrying no tooltips, with the default hover
/// delay and lifetime.
/// </summary>
/// <param name="window">The window whose tooltips this manager will look after.</param>
/// <remarks>No tooltip will appear until Activate is called.</remarks>
// (0x006EB2E0)

ToolTipManager::ToolTipManager(HWND window) :
    Current(0),
    Window(window),
    Enabled(false),
    Delay(1000),
    Duration(10000)
{
    Tips.Clear();
    Index.Clear();
}


/// <summary>
/// Destroys the tooltip manager.
/// The hover timer is handed back to Windows and every tooltip registered with the manager
/// is destroyed along with it.
/// </summary>
// (0x006EB390)
// Stops the timer, hides the tip being shown and frees the registered tips.

ToolTipManager::~ToolTipManager()
{
    KillTimer(Window, 0x54544950);
    if (Current != 0) {
        // The original dispatches virtually even from the destructor (an inlined helper).
        ((ToolTipManager *)this)->Hide(&Record);
    }
    Index.Clear();
    for (int index = 0; index < Tips.Count(); index++) {
        if (Tips[index] != 0) {
            delete Tips[index];
        }
    }
    Tips.Clear();
}


// void ToolTipManager::Enable(bool enable) (0x006EB480)
// Turns the tips on or off; turning them off stops the timer and hides the tip.

void ToolTipManager::Enable(bool enable)
{
    if (Enabled != enable) {
        Enabled = enable;
        if (!enable) {
            KillTimer(Window, 0x54544950);
            if (Current != 0) {
                Hide(&Record);
            }
        }
        Debug_Printf("Tooltips are %s.\n", (Enabled == true) ? "on" : "off");
    }
}


// void ToolTipManager::Process_Message(MSG *message) (0x006EB4E0; Tiberian Sun's Message_Handler)
// Feeds the window messages to the tips: a button press hides the tip, mouse movement restarts the
// delay timer (or, with no delay or while 0x00A416BC is set, looks for a tip at once), and the timer
// either hides the shown tip or finds the tip under the cursor and shows it for Duration.

void ToolTipManager::Process_Message(MSG *message)
{
    if (Enabled == true) {
        switch (message->message) {
        case 0x201:     // WM_LBUTTONDOWN
        case 0x202:     // WM_LBUTTONUP
        case 0x204:     // WM_RBUTTONDOWN
        case 0x205:     // WM_RBUTTONUP
        case 0x207:     // WM_MBUTTONDOWN
        case 0x208:     // WM_MBUTTONUP
            KillTimer(Window, 0x54544950);
            Hide_Current();
            break;

        case 0x200:     // WM_MOUSEMOVE
            if (Delay != 0 && !CursorCheat) {
                KillTimer(Window, 0x54544950);
                SetTimer(Window, 0x54544950, Delay, 0);
                Hide_Current();
            } else {
                GetCursorPos(&Cursor);
                ScreenToClient(Window, &Cursor);
                Current = Find(&Cursor);
                if (Update()) {
                    SetTimer(Window, 0x54544950, Duration, 0);
                }
            }
            break;

        case 0x113:     // WM_TIMER
            if (message->wParam == 0x54544950) {
                KillTimer(Window, 0x54544950);
                if (Current != 0) {
                    Hide_Current();
                } else {
                    GetCursorPos(&Cursor);
                    ScreenToClient(Window, &Cursor);
                    Current = Find(&Cursor);
                    if (Update()) {
                        SetTimer(Window, 0x54544950, Duration, 0);
                    }
                }
            }
            break;
        }
    }
}


// int ToolTipManager::Get_Delay() const (0x006EB7F0)

int ToolTipManager::Get_Delay() const
{
    return Delay;
}


// void ToolTipManager::Set_Delay(int delay) (0x006EB800)

void ToolTipManager::Set_Delay(int delay)
{
    Delay = delay;
}


// void ToolTipManager::Save_Delay() (0x006EB810)

void ToolTipManager::Save_Delay()
{
    SavedDelay = Delay;
}


// void ToolTipManager::Restore_Delay() (0x006EB820)

void ToolTipManager::Restore_Delay()
{
    Delay = SavedDelay;
}


// int ToolTipManager::Get_Duration() const (0x006EB830)

int ToolTipManager::Get_Duration() const
{
    return Duration;
}


// void ToolTipManager::Set_Duration(int duration) (0x006EB840)

void ToolTipManager::Set_Duration(int duration)
{
    Duration = duration;
}


// int ToolTipManager::Count() const (0x006EB850)

int ToolTipManager::Count() const
{
    return Tips.Count();
}


/// <summary>
/// Adds a tooltip to the manager.
/// The manager takes a copy, so the caller's record does not have to outlive the call.
/// Identifiers must be unique -- a duplicate is refused rather than displacing the tooltip
/// already registered under that identifier.
/// </summary>
/// <param name="tooltip">The tooltip to copy into the manager.</param>
/// <returns>bool; Was the tooltip added?</returns>
// (0x006EB860)
// Registers a copy of the tip unless its ID is already known: the copy goes into Tips and the ID index.

bool ToolTipManager::Add(ToolTip const &tip)
{
    if (!Find(tip.ID, 0)) {
        ToolTip *tooltip = new ToolTip(tip);
        if (tooltip != 0) {
            if (Tips.Add(tooltip)) {
                Index.Add_Index(tooltip->ID, tooltip);
                return true;
            }
            delete tooltip;
        }
    }
    return false;
}


/// <summary>
/// Removes the tooltip with the identifier specified.
/// The tooltip is destroyed outright. If it happens to be the one on display, the display
/// is cleared first so that nothing is left pointing at it.
/// </summary>
/// <param name="id">Identifier of the tooltip to remove.</param>
// (0x006EBA10)
// Unregisters and deletes the tip with this ID (hiding it first when it is the one shown).

void ToolTipManager::Remove(unsigned int id)
{
    if (Index.Is_Present(id)) {
        ToolTip *tooltip = Index[id];
        if (Current == tooltip) {
            Hide_Current();
        }
        Index.Remove_Index(tooltip->ID);
        Tips.Delete(tooltip);
        if (tooltip != 0) {
            delete tooltip;
        }
    }
}


/// <summary>
/// Finds the tooltip with the identifier specified.
/// </summary>
/// <param name="id">Identifier of the tooltip to look for.</param>
/// <param name="tooltip">Optional destination to copy the tooltip found into.</param>
/// <returns>bool; Was a tooltip with that identifier registered?</returns>
// (0x006EBBA0; Tiberian Sun's Find)
// The tip registered under id exists; when tooltip is not 0 the tip is copied there. No caller keeps
// this copy: Add expands it.

bool ToolTipManager::Find(unsigned int id, ToolTip *tooltip)
{
    if (!Index.Is_Present(id)) {
        return false;
    }
    if (tooltip != 0) {
        *tooltip = *Index[id];
    }
    return true;
}


// ToolTip *ToolTipManager::Find(POINT const *point) (0x006EBD10; Tiberian Sun's Find_From_Pos)
// The registered tip whose area contains the point, or 0. No caller keeps this copy: Process_Message
// expands it.

ToolTip *ToolTipManager::Find(POINT const *point)
{
    for (int index = 0; index < Tips.Count(); index++) {
        Rect const *region = &Tips[index]->Region;
        if (point->x >= region->X && point->x <= region->X + region->Width
            && point->y >= region->Y && point->y <= region->Y + region->Height) {
            return Tips[index];
        }
    }
    return 0;
}


// bool ToolTipManager::Show(ToolTipRecord *record) (0x006EBD80, slot 1)

bool ToolTipManager::Show(ToolTipRecord *record)
{
    return true;
}


// void ToolTipManager::Hide(ToolTipRecord *record) (0x006EBD90, slot 2)
// Clears the shown tip (the argument is the member record).

void ToolTipManager::Hide(ToolTipRecord *record)
{
    Record.unknown_00 = 0;
    Record.unknown_04 = 0;
    Record.unknown_08 = 0;
    Record.unknown_0c = 0;
    Current = 0;
}


/// <summary>
/// Works out the display metrics for the tooltip text specified.
/// This is the hook a derived manager overrides in order to measure the text and decide
/// where the tooltip box will sit. The base manager takes the text as it stands.
/// </summary>
/// <param name="text">The tooltip display record to prepare.</param>
/// <returns>bool; Can the tooltip be displayed?</returns>
// (0x006EBDB0; Tiberian Sun's Process)
// Prepares the tip under the mouse: its text (the string table label, or Get_Text(ID) without one) is
// copied into Record at the cursor and offered to Show; a tip without text, or one Show refuses, is
// dropped. True while a tip remains.

bool ToolTipManager::Update()
{
    ToolTip const *tip = Current;
    if (tip != 0) {
        wchar_t const *text = tip->Label == 0 ? Get_Text(tip->ID)
                                              : StringTable::Load_String(tip->Label, 0, "D:\\Ra2\\ToolTip.cpp", 0x233);
        if (text != 0 && wcslen(text) > 0) {
            wcsncpy(Record.Text, text, 256);
            Record.unknown_00 = Cursor.x;
            Record.unknown_04 = Cursor.y;
            ToolTipRecord &record = Record;
            if (!Show(&Record)) {
                record.unknown_00 = 0;
                record.unknown_04 = 0;
                record.unknown_08 = 0;
                record.unknown_0c = 0;
                Current = 0;
            }
        } else {
            Record.unknown_00 = 0;
            Record.unknown_04 = 0;
            Record.unknown_08 = 0;
            Record.unknown_0c = 0;
            Current = 0;
        }
    }
    return Current != 0;
}


// void ToolTipManager::Refresh(bool update) (0x006EBE60, slot 3)

void ToolTipManager::Refresh(bool update)
{
    if (update == true) {
        Update();
    }
    if (Current != 0) {
        Debug_Print(&Record);
    }
}


// void ToolTipManager::Debug_Print(ToolTipRecord *record) (0x006EBE90, slot 4)

void ToolTipManager::Debug_Print(ToolTipRecord *record)
{
    Debug_Printf("ToolTip: %s\n", record->Text);
}


// wchar_t const *ToolTipManager::Get_Text(int id) (0x006EBEB0, slot 5)

wchar_t const *ToolTipManager::Get_Text(int id)
{
    return 0;
}


// void ToolTipManager::Hide_Current() (0x006EBEC0)

void ToolTipManager::Hide_Current()
{
    if (Current != 0) {
        Hide(&Record);
    }
}


// bool ToolTipManager::Is_Active() const (0x006EBEE0)

bool ToolTipManager::Is_Active() const
{
    return Current != 0;
}


// ToolTipManager scalar deleting destructor (0x006EBEF0): compiler generated
// (the destructor body is 0x006EB390; flag 1 also frees the object). It is
// the virtual destructor slot of the vtable at 0x007AD934.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<ToolTip *>::Clear (0x006EBF10; DynamicVectorClass<ToolTip *> vtable 0x007ad950 slot 3).
// DynamicVectorClass<ToolTip *>::ID_pointer (0x006EBF40; DynamicVectorClass<ToolTip *> vtable 0x007ad950 slot 5).
// VectorClass<ToolTip *>::Fetch (0x006EBF60; VectorClass<ToolTip *> vtable 0x007ad970 slot 6).
// VectorClass<ToolTip *>::operator equal (0x006EBF70; VectorClass<ToolTip *> vtable 0x007ad970 slot 1).
// VectorClass<ToolTip *>::Resize (0x006EBFB0; VectorClass<ToolTip *> vtable 0x007ad970 slot 2).
// VectorClass<ToolTip *>::Clear (0x006EC060; VectorClass<ToolTip *> vtable 0x007ad970 slot 3).
// VectorClass<ToolTip *>::ID_object (0x006EC090; VectorClass<ToolTip *> vtable 0x007ad970 slot 4).
// VectorClass<ToolTip *>::ID_pointer (0x006EC0D0; VectorClass<ToolTip *> vtable 0x007ad970 slot 5).
// DynamicVectorClass<ToolTip *>::Resize (0x006EC0F0; DynamicVectorClass<ToolTip *> vtable 0x007ad950 slot 2).
// DynamicVectorClass<ToolTip *>::ID_object (0x006EC1A0; DynamicVectorClass<ToolTip *> vtable 0x007ad950 slot 4).


extern "C" unsigned char vtable_6ec1d0[]; extern "C" void __cdecl release_buffer_6ec1d0(void*); extern "C" void __cdecl release_object_6ec1d0(void*);
class Leaf_6ec1d0 { public: void* get(unsigned int flags); };
void* Leaf_6ec1d0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6ec1d0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6ec1d0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6ec1d0(this); return this; }


// unsigned int *Owner_006ec220::unknown_scalar_deleting_destructor_006ec220(unsigned char deletion_flags)
// (0x006EC220): stores the VectorClass<ToolTip const *> vtable (0x007AD970), frees the owned buffer
// (+0x04 when +0x0D is set), clears the size, and frees the object when bit 0 of the flags is set.

namespace seed_006ec220 {

extern void  operator_delete_00781d3b(unsigned int allocation_address);
extern "C" void* VectorClass_ToolTipPtr_VTable_007ad970;

struct Owner_006ec220 { unsigned int * unknown_scalar_deleting_destructor_006ec220(unsigned char deletion_flags); };

unsigned int * Owner_006ec220::unknown_scalar_deleting_destructor_006ec220(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&VectorClass_ToolTipPtr_VTable_007ad970);
    if ((object_words[1] != 0) && (*(char *)((int)object_words + 0xd) != '\0')) {
        operator_delete_00781d3b((unsigned int)(object_words[1]));
        object_words[1] = 0;
    }
    *(unsigned char *)((int)object_words + 0xd) = 0;
    object_words[2] = 0;
    if ((deletion_flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_006ec220


// ToolTipIndexNode::ToolTipIndexNode() (0x006EC270)
// The index record's empty constructor (it returns this).

ToolTipIndexNode::ToolTipIndexNode()
{
}


// bool ToolTipIndex::Is_Archive_Same(unsigned int const &id) const (0x006EC280)

bool ToolTipIndex::Is_Archive_Same(unsigned int const &id) const
{
    if (Archive != 0 && Archive->ID == id) {
        return true;
    }
    return false;
}


// ToolTipIndexNode const *ToolTipIndex::Search_For_Node(unsigned int const &id) const (0x006EC2A0)
// Sorts the table when needed, then binary searches it.

ToolTipIndexNode const *ToolTipIndex::Search_For_Node(unsigned int const &id) const
{
    if (IndexCount == 0) {
        return 0;
    }
    if (!IsSorted) {
        qsort(&IndexTable[0], IndexCount, sizeof(IndexTable[0]), search_compfunc);
        ((ToolTipIndex *)this)->Archive = 0;
        ((ToolTipIndex *)this)->IsSorted = true;
    }
    ToolTipIndexNode node;
    node.ID = id;
    return Binary_Search(IndexTable, IndexCount, node);
}


// int __cdecl ToolTipIndex::search_compfunc(void const *left, void const *right) (0x006EC320)
// qsort comparator: orders records by ID (signed).

int __cdecl ToolTipIndex::search_compfunc(void const *left, void const *right)
{
    if (*(int const *)left == *(int const *)right) {
        return 0;
    }
    if (*(int const *)left < *(int const *)right) {
        return -1;
    }
    return 1;
}


// ToolTipIndexNode *__fastcall Binary_Search(table, count, target) (0x006EC350)
// Tiberian Sun's bsearch.h Binary_Search on the index nodes (expanded twice in Find 0x006EBBA0).

ToolTipIndexNode *__fastcall Binary_Search(ToolTipIndexNode *table, int count, ToolTipIndexNode const &target)
{
    ToolTipIndexNode const *pointer = table;
    int stride = count;

    while (0 < stride) {
        int const pivot = stride / 2;
        ToolTipIndexNode const *const tryptr = pointer + pivot;
        if (target < *tryptr) {
            stride = pivot;
        } else {
            if (*tryptr == target) {
                return (ToolTipIndexNode *)tryptr;
            }
            pointer = tryptr + 1;
            stride -= pivot + 1;
        }
    }
    return 0;
}
