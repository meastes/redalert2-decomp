// grphmenu.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/grphmenu.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_CURSOR_VIRTUAL_OPERATORS
#include "shared/graphic_menu_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/ini_class.hpp"
#include "shared/ms_anim.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/crt_string.hpp"
#include "shared/vector_cursor.hpp"
#include "shared/dsurface.hpp"
#include "shared/keyboard.hpp"
#include "shared/mouse.hpp"
#include "shared/theme.hpp"
#include "shared/surface.hpp"


// Header-static dynamic initializer of the GraphicMenu unit (0x004DE240, _$E): shape_file.hpp's empty rect
// (0x00A40E28), emitted through shared/graphic_menu.hpp by the drafts of this plan; no source of its own.


// GraphicMenu *GraphicMenu::Load(Wstring const &file, Wstring const &section) (0x004DE260): the menu of the
// INI file's section, or 0 when the file is missing or does not load.

GraphicMenu *__fastcall GraphicMenu::Load(Wstring const &file, Wstring const &section)
{
    CCFileClass inifile(file);
    if (!inifile.Is_Available()) {
        return 0;
    }
    INIClass ini;
    GraphicMenu *menu;
    if (ini.Load(inifile, false) > 0) {
        menu = Load(ini, section);
    } else {
        menu = 0;
    }
    return menu;
}


// GraphicMenu *GraphicMenu::Load(INIClass &ini, Wstring const &section) (0x004DE380): a new menu from the
// section: Background= names the picture (<name>.VQA, else <name>.BIK, else <name>.PCX plays as the background
// animation, whose origin offsets the items), Theme= the music, items in the keys 0..ItemMax (Create_Menu_Item).
// 0 when the section is missing.
GraphicMenuItem *__fastcall Create_Menu_Item(Wstring const &section, INIClass &ini, void *unknown_a, Point2D const *offset);

GraphicMenu *__fastcall GraphicMenu::Load(INIClass &ini, Wstring const &section)
{
    if (ini.Find_Section(section) == 0) {
        return 0;
    }
    GraphicMenu *menu = new GraphicMenu;
    if (menu == 0) {
        return 0;
    }

    Wstring value;
    bool found = ini.Get_String(section, "Background", "", value) > 0;
    Wstring background = value + ".PCX";
    menu->Set_Background(background);
    Point2D offset(0, 0);
    if (found) {
        Wstring vqa = value + ".VQA";
        MSAnim *anim = 0;
        if (CCFileClass(vqa).Is_Available()) {
            anim = new MSVQAnim(vqa, unknown_surface_839958, &menu->Engine.Anims, true);
        }
        if (anim == 0) {
            Wstring bik = value + ".BIK";
            if (CCFileClass(bik).Is_Available()) {
                Rect rect(0, 0, unknown_8523e0, unknown_8523e4);
                anim = new MSBinkAnim(bik, unknown_surface_839958, &menu->Engine.Anims, rect, true);
            }
        }
        if (anim == 0) {
            anim = new MSPCXAnim(background.Get(), &menu->Engine.Anims, true);
        }
        menu->Set_Anim(anim);
        // The items are placed relative to the animation's corner (read straight from the returned rectangle).
        offset = Offset_Point(Point2D(0, 0), anim->Get_Rect());
    }

    if (ini.Get_String(section, "Theme", "", value) > 0) {
        menu->Set_Theme(value);
    }

    char key[8];
    int max = ini.Get_Int(section, "ItemMax", 100);
    for (int index = 0; index <= max; index++) {
        sprintf(key, "%d", index);
        if (ini.Get_String(section, key, "", value) > 0) {
            GraphicMenuItem *item = Create_Menu_Item(value, ini, &menu->Engine, &offset);
            if (item) {
                menu->Add_Item(item);
            }
        }
    }
    return menu;
}


// GraphicMenu::GraphicMenu() (0x004DE820): the default background "Title.PCX" and theme "Intro", no animation, no items.

GraphicMenu::GraphicMenu() :
    Background("Title.PCX"),
    Theme("Intro"),
    Anim(0)
{
}


// GraphicMenu::~GraphicMenu() (0x004DE880): deletes the items, clears the background and back buffers and
// copies the cleared buffer to the screen.
typedef VectorCursor<GraphicMenuItem *, DynamicVectorClass<GraphicMenuItem *> > GraphicMenuCursor;

GraphicMenu::~GraphicMenu()
{
    GraphicMenuCursor cursor(Items);
    while (cursor) {
        delete *cursor;
        cursor++;
    }
    unknown_surface_839958->Fill(0);
    unknown_surface_839954->Fill(0);
    unknown_surface_839950->Copy_From(*unknown_surface_839954, false, true);
}


// void GraphicMenu::Set_Item_Visible(int id, bool visible) (0x004DE960): shows or hides every item with the id.
typedef VectorCursor<GraphicMenuItem *, DynamicVectorClass<GraphicMenuItem *> > GraphicMenuCursor;

void GraphicMenu::Set_Item_Visible(int id, bool visible)
{
    for (GraphicMenuCursor cursor(Items); cursor; cursor++) {
        if ((*cursor)->ID == id) {
            GraphicMenuItem *item = *cursor;
            item->Set_Visible(visible);
        }
    }
}


// int GraphicMenu::Run() (0x004DE9E0): plays the menu's theme over the cleared screen, waits for the background
// animation, then runs the engine until a key picks an item (1 and 13 choose the item under the mouse, other
// keys the item whose shortcut matches); the mouse highlights the item under it. Returns the chosen item's ID.
int unknown_5ff5d0();

int GraphicMenu::Run()
{
    ::Theme.Play_Song(::Theme.From_Name(Theme.Get()));
    unknown_surface_839958->Fill(0);
    unknown_surface_839954->Fill(0);
    unknown_surface_839950->Copy_From(*unknown_surface_839954, false, true);
    bool done = false;
    GraphicMenuItem *selected = 0;
    WWKeyboard->Clear();
    if (Anim) {
        Engine.Wait_For(Anim, 300000);
    }
    Engine.Broadcast((int)&unknown_surface_839958->Get_Rect());
    Engine.Redraw();
    do {
        WWMouse->Hide_Mouse();
        Engine.Check_Focus();
        WWMouse->Show_Mouse();
        Point2D point(WWMouse->Get_Mouse_X(), WWMouse->Get_Mouse_Y());
        if (WWKeyboard->Check()) {
            int key = WWKeyboard->Get();
            GraphicMenuItem *item;
            if (key == 1 || key == 13) {
                item = Item_At(point);
            } else {
                item = Item_For(key);
            }
            if (item) {
                if (selected != item) {
                    if (selected) {
                        selected->Set_Enabled(false);
                    }
                    selected = item;
                    item->Set_Enabled(true);
                }
                done = true;
            }
        } else {
            GraphicMenuItem *item = Item_At(point);
            if (item != selected) {
                if (selected) {
                    selected->Set_Enabled(false);
                }
                selected = item;
                if (item) {
                    item->Set_Enabled(true);
                }
            }
        }
        Engine.Run(1);
    } while (!done);
    if (selected) {
        selected->Play_Sound((int)&Engine);
    }
    unknown_5ff5d0();
    ::Theme.Queue_Song(-3);
    return selected ? selected->ID : -1;
}


// void GraphicMenu::Set_Anim(MSAnim *anim) (0x004DED20): replaces the background animation in the engine.

void GraphicMenu::Set_Anim(MSAnim *anim)
{
    if (Anim) {
        Engine.Replace_Anim(Anim, anim);   // argument order as the original pushes it
    } else {
        Engine.Add_Anim(anim);
    }
    Anim = anim;
}


// void GraphicMenu::Set_Theme(Wstring const &theme) (0x004DED60).

void GraphicMenu::Set_Theme(Wstring const &theme)
{
    Theme = theme;
}


// void GraphicMenu::Add_Item(GraphicMenuItem *item) (0x004DED70).

void GraphicMenu::Add_Item(GraphicMenuItem *item)
{
    Items.Add(item);
}


// GraphicMenuItem *GraphicMenu::Item_At(Point2D const &point) (0x004DEDC0): the first item containing the point, or 0.
typedef VectorCursor<GraphicMenuItem *, DynamicVectorClass<GraphicMenuItem *> > GraphicMenuCursor;

GraphicMenuItem *GraphicMenu::Item_At(Point2D const &point)
{
    GraphicMenuCursor cursor(Items);
    while (cursor && !(*cursor)->Contains(point)) {
        cursor++;
    }
    return cursor ? *cursor : 0;
}


// GraphicMenuItem *GraphicMenu::Item_For(int key) (0x004DEE50): the first item whose slot 1 accepts key, or 0.
typedef VectorCursor<GraphicMenuItem *, DynamicVectorClass<GraphicMenuItem *> > GraphicMenuCursor;

GraphicMenuItem *GraphicMenu::Item_For(int key)
{
    GraphicMenuCursor cursor(Items);
    while (cursor && !(*cursor)->Is_Input_Key(key)) {
        cursor++;
    }
    return cursor ? *cursor : 0;
}


// void GraphicMenu::Set_Background(Wstring const &background) (0x004DEEE0).

void GraphicMenu::Set_Background(Wstring const &background)
{
    Background = background;
}


extern "C" void __cdecl release_4deef0(void*);
class Leaf_4deef0 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_4deef0::get(unsigned int flags) { cleanup(); if (flags & 1) release_4deef0(this); return this; }


extern "C" void __cdecl release_4def10(void*);
class Leaf_4def10 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_4def10::get(unsigned int flags) { cleanup(); if (flags & 1) release_4def10(this); return this; }


// 0x004def90: function not yet identified
//
// Owner: VectorCursor<GraphicMenuItem *, DynamicVectorClass<GraphicMenuItem *>>, virtual slot 1 (vtable +0x4) (from RTTI/vtables).
// Body: 14 bytes (frozen inventory bound).
// Referenced from 8 sites (computed call, data).
// Ghidra prototype: bool __fastcall FUN_004def90(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004def90 {

bool __fastcall FUN_004def90(int cursor_address)
{
    return *(unsigned int *)(cursor_address + 8) < *(unsigned int *)(*(int *)(cursor_address + 4) + 0x10);
}

}  // namespace seed_004def90


// 0x004defa0: function not yet identified
//
// Owner: VectorCursor<GraphicMenuItem *, DynamicVectorClass<GraphicMenuItem *>>, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 32 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_004defa0(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004defa0 {

extern void  FUN_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_007a2b84;

struct Owner_004defa0 { unsigned int * FUN_004defa0(unsigned char deletion_flags); };

unsigned int * Owner_004defa0::FUN_004defa0(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_007a2b84);
    if ((deletion_flags & 1) != 0) {
        FUN_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_004defa0


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<GraphicMenuItem *>::Clear (0x004DEF30; DynamicVectorClass<GraphicMenuItem *> vtable 0x007a2b44 slot 3).
// DynamicVectorClass<GraphicMenuItem *>::ID_pointer (0x004DEF60; DynamicVectorClass<GraphicMenuItem *> vtable 0x007a2b44 slot 5).
// VectorClass<GraphicMenuItem *>::Fetch (0x004DEF80; DynamicVectorClass<GraphicMenuItem *> vtable 0x007a2b44 slot 6).
// VectorClass<GraphicMenuItem *>::operator equal (0x004DEFC0; VectorClass<GraphicMenuItem *> vtable 0x007a2b64 slot 1).
// VectorClass<GraphicMenuItem *>::Resize (0x004DF000; VectorClass<GraphicMenuItem *> vtable 0x007a2b64 slot 2).
// VectorClass<GraphicMenuItem *>::Clear (0x004DF0B0; VectorClass<GraphicMenuItem *> vtable 0x007a2b64 slot 3).
// VectorClass<GraphicMenuItem *>::ID_object (0x004DF0E0; VectorClass<GraphicMenuItem *> vtable 0x007a2b64 slot 4).
// VectorClass<GraphicMenuItem *>::ID(GraphicMenuItem * const *) (vtable 0x007A2B64 slot 5) (0x004DF120).
// DynamicVectorClass<GraphicMenuItem *>::Resize (0x004DF140; DynamicVectorClass<GraphicMenuItem *> vtable 0x007a2b44 slot 2).
// DynamicVectorClass<GraphicMenuItem *>::ID_object (0x004DF1F0; DynamicVectorClass<GraphicMenuItem *> vtable 0x007a2b44 slot 4).


class Leaf_4df220 { public: unsigned int get(); };
unsigned int Leaf_4df220::get() { unsigned int cursor_address = (unsigned int)this; unsigned int result_address; unsigned int unused_value; result_address = cursor_address; *(unsigned int*)(result_address + 8) = (*(unsigned int*)(result_address + 8)) + 1; return result_address; }


class Leaf_4df230 { public: unsigned int get(); };
unsigned int Leaf_4df230::get() { unsigned int cursor_address = (unsigned int)this; unsigned int result_address; unsigned int unused_value; result_address = cursor_address; *(unsigned int*)(result_address + 8) = (*(unsigned int*)(result_address + 8)) - 1; return result_address; }


extern "C" unsigned char vtable_4df240[]; extern "C" void __cdecl release_buffer_4df240(void*); extern "C" void __cdecl release_object_4df240(void*);
class Leaf_4df240 { public: void* get(unsigned int flags); };
void* Leaf_4df240::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4df240; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4df240((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4df240(this); return this; }


extern "C" unsigned char vtable_4df290[]; extern "C" void __cdecl release_buffer_4df290(void*); extern "C" void __cdecl release_object_4df290(void*);
class Leaf_4df290 { public: void* get(unsigned int flags); };
void* Leaf_4df290::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4df290; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4df290((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4df290(this); return this; }


extern "C" unsigned char table_4df2e0[]; extern "C" void* __cdecl storage_4df2e0(unsigned int);
class Storage_4df2e0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_4df2e0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_4df2e0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_4df2e0(requested << 2); owns=1; } return this; }
