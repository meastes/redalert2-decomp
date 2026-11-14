// drop.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/drop.json carries the addresses and the rows that
// stay in their own files.


#include "shared/gadget.hpp"


// DropListClass::DropListClass(int id, wchar_t *text, int max_len, unsigned flags, int x, int y, int w, int h, void const *up, void const *down) (0x004A4DC0)
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


// void DropListClass::Zap() (0x004A4E90, slot 8)

void DropListClass::Zap()
{
    Collapse();
    List.Zap();
    DropButton.Zap();
    EditClass::Zap();
}


// LinkClass &DropListClass::Add(LinkClass &object) (0x004A4ED0, slot 3)

LinkClass &DropListClass::Add(LinkClass &object)
{
    DropButton.Add(object);
    return EditClass::Add(object);
}


// LinkClass &DropListClass::Add_Tail(LinkClass &object) (0x004A4EF0, slot 4)

LinkClass &DropListClass::Add_Tail(LinkClass &object)
{
    DropButton.Add_Tail(object);
    return EditClass::Add_Tail(object);
}


// LinkClass &DropListClass::Add_Head(LinkClass &object) (0x004A4F10, slot 5)

LinkClass &DropListClass::Add_Head(LinkClass &object)
{
    DropButton.Add_Head(object);
    return EditClass::Add_Head(object);
}


// LinkClass *DropListClass::Remove() (0x004A4F30, slot 9)

LinkClass *DropListClass::Remove()
{
    Collapse();
    DropButton.Remove();
    return EditClass::Remove();
}


// int DropListClass::Add_Item(wchar_t *text) (0x004A4F60, slot 39)
// Shows text in the field and appends it to the list.

int DropListClass::Add_Item(wchar_t *text)
{
    wcsncpy(String, text, MaxLength);
    Flag_To_Redraw();
    return List.Add_Item(text);
}


// wchar_t *DropListClass::Current_Item() (0x004A4FA0, slot 40)

wchar_t *DropListClass::Current_Item()
{
    return List.Current_Item();
}


// int DropListClass::Current_Index() (0x004A4FC0, slot 41)

int DropListClass::Current_Index()
{
    return List.Current_Index();
}


// void DropListClass::Set_Selected_Index(int index) (0x004A4FE0, slot 43)
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


// void DropListClass::Clear_Focus() (0x004A5050, slot 21)

void DropListClass::Clear_Focus()
{
    Collapse();
}


// void DropListClass::Peer_To_Peer(unsigned flags, unsigned &key, ControlClass &whom) (0x004A5070, slot 19)
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


// void DropListClass::Expand() (0x004A5140)
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


// void DropListClass::Collapse() (0x004A51A0)

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


DropListClass::DropListClass(DropListClass const &list) :
    EditClass(list), IsDropped(list.IsDropped), ListHeight(list.ListHeight),
    DropButton(list.DropButton), List(list.List)
{
    List.Make_Peer(*this);
    DropButton.Make_Peer(*this);
}


// void DropListClass::Set_Position(int x, int y) (0x004A5490, slot 25)
// The list goes under the button's height, the button to the field's right.

void DropListClass::Set_Position(int x, int y)
{
    GadgetClass::Set_Position(x, y);
    List.Set_Position(x, ((short const *)DropButton.ShapeData)[2] + y);
    DropButton.Set_Position(Width + x, y);
}


// void DropListClass::Set_Selected_Index(wchar_t *text) (0x004A54E0, slot 42)
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


extern "C" void __cdecl release_4a5780(void*);
class Leaf_4a5780 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_4a5780::get(unsigned int flags) { cleanup(); if (flags & 1) release_4a5780(this); return this; }


// 0x004A57A0: ShapeButtonClass's scalar deleting destructor, the copy the linker kept from the
// DropList unit: runs ShapeButtonClass::~ShapeButtonClass, then operator delete when flags bit 0
// is set. Stand-in owner class until the emitting unit produces it naturally.

class Leaf_4a57a0 { public: void* get(unsigned int flags); };
void* Leaf_4a57a0::get(unsigned int flags) { ((ShapeButtonClass *)this)->ShapeButtonClass::~ShapeButtonClass(); if (flags & 1) operator delete(this); return this; }


// 0x004a57c0: function not yet identified
//
// Owner: ToggleClass, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 30 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 __thiscall scalar_deleting_destructor_004a57c0(undefined4 param_1,byte param_2)
// Calls GadgetClass::~GadgetClass, then operator delete when flags bit 0 is set
// (a scalar deleting destructor of a GadgetClass-derived class with a trivial destructor).
//
// Types and names are Ghidra placeholders (see shared/ghidra_types.hpp);
// rename and retype as the purpose becomes known.
//

namespace seed_004a57c0 {

struct Owner_004a57c0 { unsigned int scalar_deleting_destructor_004a57c0(unsigned char flags); };

unsigned int Owner_004a57c0::scalar_deleting_destructor_004a57c0(unsigned char flags)
{
    ((GadgetClass *)this)->GadgetClass::~GadgetClass();
    if ((flags & 1) != 0) {
        operator delete(this);
    }
    return (unsigned int)this;
}

}  // namespace seed_004a57c0
