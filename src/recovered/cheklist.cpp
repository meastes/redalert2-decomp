// cheklist.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/cheklist.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/check_list.hpp"


// Header-static dynamic initializer of the unit at 0x0047D2C0: the empty
// rectangle of shared/shape_file.hpp (0x00850C58), the unit's first function.


// CheckListClass::CheckListClass(int id, int x, int y, int w, int h, unsigned flags, void const *up, void const *down) (0x0047D2E0)
// A list that accepts input.

CheckListClass::CheckListClass(int id, int x, int y, int w, int h, unsigned flags, void const *up, void const *down) :
    ListClass(id, x, y, w, h, flags, up, down),
    IsReadOnly(false)
{
}


// CheckListClass::~CheckListClass() (0x0047D330)
// Deletes the check objects while removing the entries.

CheckListClass::~CheckListClass()
{
    while (List.Count()) {
        CheckObject *object = (CheckObject *)ListClass::Get_Item(0);
        ListClass::Remove_Item(0);
        delete object;
    }
}


// int CheckListClass::Add_Item(wchar_t *text) (0x0047D370, slot 34)
// Adds an unchecked entry.

int CheckListClass::Add_Item(wchar_t *text)
{
    return ListClass::Add_Item((wchar_t *)new CheckObject(text, false));
}


// wchar_t *CheckListClass::Current_Item() const (0x0047D3B0, slot 39)

wchar_t *CheckListClass::Current_Item() const
{
    CheckObject *object = (CheckObject *)ListClass::Current_Item();
    if (object != 0) {
        return object->Text;
    }
    return 0;
}


// wchar_t *CheckListClass::Get_Item(int index) const (0x0047D3C0, slot 40)

wchar_t *CheckListClass::Get_Item(int index) const
{
    CheckObject *object = (CheckObject *)ListClass::Get_Item(index);
    if (object != 0) {
        return object->Text;
    }
    return 0;
}


// void CheckListClass::Remove_Item(wchar_t *text) (0x0047D3E0, slot 43)
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


// void CheckListClass::Set_Selected_Index(wchar_t *text) (0x0047D450, slot 45)
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


// void CheckListClass::Check_Item(int index, bool checked) (0x0047D4B0)
// Sets an entry's check mark, redrawing on a change.

void CheckListClass::Check_Item(int index, bool checked)
{
    CheckObject *object = (CheckObject *)ListClass::Get_Item(index);
    if (object != 0 && object->IsChecked != checked) {
        object->IsChecked = checked;
        Flag_To_Redraw();
    }
}


// bool CheckListClass::Is_Checked(int index) const (0x0047D4E0)

bool CheckListClass::Is_Checked(int index) const
{
    CheckObject *object = (CheckObject *)ListClass::Get_Item(index);
    if (object != 0) {
        return object->IsChecked;
    }
    return false;
}


// int CheckListClass::Action(unsigned flags, unsigned &key) (0x0047D500, slot 31)
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


// void CheckListClass::Draw_Entry(int index, int x, int y, int width, int selected) (0x0047D580, slot 50)
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
