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


// ListClass::ListClass(int id, int x, int y, int w, int h, unsigned flags, void const *up, void const *down) (0x00539BF0)
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


// ListClass::ListClass(ListClass const &list) (0x00539D40)
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


// 0x00539f30: function not yet identified
//
// Owner: ListClass, virtual slot 25 (vtable +0x64) (from RTTI/vtables).
// Body: 136 bytes (frozen inventory bound).
// Referenced from 3 sites (data).
// Ghidra prototype: void __thiscall Set_Position_00539f30(int param_1,int param_2,int param_3)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_00539f30 {

struct Owner_00539f30 { void Set_Position_00539f30(int x, int y); };

void Owner_00539f30::Set_Position_00539f30(int x, int y)
{
    int self = (int)this;
    int width;
    int up_width;
    int down_width;
    int down_offset;
    int scroll_width;

    width = (int)(*(int *)(self + 0x14));
    up_width = (int)(*(int *)(self + 0x6c));
    down_width = (int)(*(int *)(self + 0xcc));
    *(int *)(self + 100) = (width - up_width) + x;
    *(int *)(self + 0x68) = y;
    *(int *)(self + 0xc4) = (width - down_width) + x;
    down_offset = (int)(*(int *)(self + 0x18) - *(int *)(self + 0xd0));
    *(int *)(self + 200) = y + down_offset;
    scroll_width = (int)(up_width);
    if (up_width <= down_width) {
        scroll_width = (int)(down_width);
    }
    *(int *)(self + 0x124) = (width - scroll_width) + x;
    *(int *)(self + 0x128) = y + *(int *)(self + 0x70);
    *(int *)(self + 0x130) = down_offset - *(int *)(self + 0x70);
    if (up_width > down_width) {
        *(int *)(self + 300) = up_width;
        return;
    }
    *(int *)(self + 300) = down_width;
    return;
}

}  // namespace seed_00539f30


// ListClass::~ListClass() (0x00539FC0)

ListClass::~ListClass()
{
    Remove_Scroll_Bar();
}


// int ListClass::Add_Item(wchar_t *text) (0x0053A060, slot 34)
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


// void ListClass::Remove_Item(int index) (0x0053A0F0, slot 42)
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


// void ListClass::Remove_Item(wchar_t *text) (0x0053A1C0, slot 43)

void ListClass::Remove_Item(wchar_t *text)
{
    if (text != 0) {
        Remove_Item(List.ID(text));
    }
}


// int ListClass::Action(unsigned flags, unsigned &key) (0x0053A1F0, slot 31)
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


// int ListClass::Draw_Me(int forced) (0x0053A2E0, slot 27)
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


// void ListClass::Bump(int up) (0x0053A3E0, slot 36)

void ListClass::Bump(int up)
{
    if (IsScrollActive) {
        if (ScrollGadget.Step(up)) {
            CurrentTopIndex = ScrollGadget.Get_Value();
            Flag_To_Redraw();
        }
    }
}


// void ListClass::Step(int up) (0x0053A430, slot 49)

void ListClass::Step(int up)
{
    if (IsScrollActive) {
        if (ScrollGadget.Step(up)) {
            CurrentTopIndex = ScrollGadget.Get_Value();
            Flag_To_Redraw();
        }
    }
}


// wchar_t *ListClass::Get_Item(int index) const (0x0053A480, slot 40)

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


// wchar_t *ListClass::Current_Item() const (0x0053A4A0, slot 39)

wchar_t *ListClass::Current_Item() const
{
    if (List.Count() <= SelectedIndex) {
        return 0;
    }
    return List[SelectedIndex];
}


// int ListClass::Current_Index() const (0x0053A4C0, slot 38)

int ListClass::Current_Index() const
{
    return SelectedIndex;
}


// void ListClass::Peer_To_Peer(unsigned flags, unsigned &key, ControlClass &whom) (0x0053A4D0, slot 19)
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


// int ListClass::Set_View_Index(int index) (0x0053A530, slot 48)
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


// int ListClass::Add_Scroll_Bar() (0x0053A5A0, slot 35)
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


// int ListClass::Remove_Scroll_Bar() (0x0053A670, slot 44)

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


// void ListClass::Set_Tabs(int const *tabs) (0x0053A6D0, slot 47)

void ListClass::Set_Tabs(int const *tabs)
{
    Tabs = tabs;
}


// void ListClass::Draw_Entry(int index, int x, int y, int width, int selected) (0x0053A6E0, slot 50)
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
        unknown_surface_83995c->Fill_Rect(Rect(x, y, width, LineHeight),
                                          unknown_drawer_8323c4->Convert(ColorSchemes[scheme]->unknown_color_0x31c));
    } else if (!(flags & 0x4000)) {
        flags |= 0x1000;
    }
    Point2D point(x, y);
    Fancy_Clip_Text_Print(List[index], unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &point,
                          ColorSchemes[scheme], 0, flags, width, Tabs);
}


// LinkClass &ListClass::Add(LinkClass &object) (0x0053A7D0, slot 3)
// The scroll parts follow the list.

LinkClass &ListClass::Add(LinkClass &object)
{
    if (IsScrollActive) {
        ScrollGadget.Add(object);
        DownGadget.Add(object);
        UpGadget.Add(object);
    }
    return ControlClass::Add(object);
}


// LinkClass &ListClass::Add_Head(LinkClass &object) (0x0053A820, slot 5)

LinkClass &ListClass::Add_Head(LinkClass &object)
{
    if (IsScrollActive) {
        ScrollGadget.Add_Head(object);
        DownGadget.Add_Head(object);
        UpGadget.Add_Head(object);
    }
    return ControlClass::Add_Head(object);
}


// LinkClass &ListClass::Add_Tail(LinkClass &object) (0x0053A870, slot 4)

LinkClass &ListClass::Add_Tail(LinkClass &object)
{
    ControlClass::Add_Tail(object);
    if (IsScrollActive) {
        UpGadget.Add_Tail(object);
        DownGadget.Add_Tail(object);
        ScrollGadget.Add_Tail(object);
    }
    return Head_Of_List();
}


// LinkClass *ListClass::Remove() (0x0053A8C0, slot 9)

LinkClass *ListClass::Remove()
{
    if (IsScrollActive) {
        ScrollGadget.Remove();
        DownGadget.Remove();
        UpGadget.Remove();
    }
    return ControlClass::Remove();
}


// void ListClass::Set_Selected_Index(int index) (0x0053A900, slot 46)
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


// int ListClass::Step_Selected_Index(int forward) (0x0053A970, slot 41)
// Moves the selection by forward; returns the old index.

int ListClass::Step_Selected_Index(int forward)
{
    int old = SelectedIndex;
    Set_Selected_Index(old + forward);
    return old;
}


// void ListClass::Flag_To_Redraw() (0x0053A990, slot 18)

void ListClass::Flag_To_Redraw()
{
    if (IsScrollActive) {
        UpGadget.Flag_To_Redraw();
        DownGadget.Flag_To_Redraw();
        ScrollGadget.Flag_To_Redraw();
    }
    ControlClass::Flag_To_Redraw();
}


// void ListClass::Set_Selected_Index(wchar_t *text) (0x0053A9D0, slot 45)
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


extern "C" unsigned char vtable_53aaf0[]; extern "C" void __cdecl release_buffer_53aaf0(void*); extern "C" void __cdecl release_object_53aaf0(void*);
class Leaf_53aaf0 { public: void* get(unsigned int flags); };
void* Leaf_53aaf0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_53aaf0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_53aaf0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_53aaf0(this); return this; }


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


// 0x0053ae60: function not yet identified
//
// Owner: VectorClass<unsigned short *>, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 73 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall scalar_deleting_destructor_0053ae60(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   operator_delete_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0053ae60 {

extern void  operator_delete_00781d3b(unsigned int);
extern "C" void* VectorClass_unsigned_short_vtable_007a5734;

struct Owner_0053ae60 { unsigned int * scalar_deleting_destructor_0053ae60(unsigned char flags); };

unsigned int * Owner_0053ae60::scalar_deleting_destructor_0053ae60(unsigned char flags)
{
    unsigned int * self = (unsigned int *)this;
    *self = (unsigned int)(&VectorClass_unsigned_short_vtable_007a5734);
    if ((self[1] != 0) && (*(char *)((int)self + 0xd) != '\0')) {
        operator_delete_00781d3b((unsigned int)(self[1]));
        self[1] = 0;
    }
    *(unsigned char *)((int)self + 0xd) = 0;
    self[2] = 0;
    if ((flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(self));
    }
    return self;
}

}  // namespace seed_0053ae60
