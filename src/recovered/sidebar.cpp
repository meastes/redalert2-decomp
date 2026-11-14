// sidebar.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/sidebar.json carries the addresses and the rows that
// stay in their own files.


#define RA2_SELECT_ACTION_UNIT
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/sidebar.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/sidebar_screen.hpp"
#include "shared/object.hpp"
#include "shared/tool_tip.hpp"
#include "shared/mouse.hpp"
#include "shared/win32_window.hpp"
#include "shared/rules.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/techno_type.hpp"
#include "shared/house_type.hpp"
#include "shared/sidebar_building.hpp"
#include "shared/event.hpp"
#include "shared/house.hpp"
#include "shared/super.hpp"
#include "shared/techno.hpp"
#include "shared/vox_class.hpp"
#include "shared/object_type.hpp"
#include "shared/factory_class_decl.hpp"
#include "shared/text_print.hpp"
#include "shared/hicolor.hpp"
#include "shared/radar.hpp"
#include "shared/string_table.hpp"
#include "shared/building_type_class.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/mouse_class.hpp"
#include "shared/map_view.hpp"
#include "shared/color_scheme.hpp"
#include "shared/progress_screen.hpp"


// Header-static dynamic initializers of the unit at 0x0067B2C0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0067B2C0-0x0067B4A1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0067B2E0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0067b2c0.cpp; no source of its own.


// Dynamic initializer (0x0067B310, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0067b2c0.cpp; no source of its own.


// Dynamic initializer (0x0067B330, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0067b2c0.cpp; no source of its own.


// Dynamic initializer (0x0067B350, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0067b2c0.cpp; no source of its own.


// Dynamic initializer (0x0067B370, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0067b2c0.cpp; no source of its own.


// Dynamic initializer (0x0067B390, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0067b2c0.cpp; no source of its own.


// Dynamic initializer (0x0067B3B0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0067b2c0.cpp; no source of its own.


// Dynamic initializer (0x0067B3F0, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0067b2c0.cpp; no source of its own.


// Dynamic initializer (0x0067B420, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0067b2c0.cpp; no source of its own.


// Dynamic initializer (0x0067B450, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0067b2c0.cpp; no source of its own.


// Dynamic initializer (0x0067B480, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0067b2c0.cpp; no source of its own.


// Dynamic initializer (0x0067B490, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0067b2c0.cpp; no source of its own.


// The sidebar's static gadgets (dynamic initializers 0x0067B4B0-0x0067B6B0
// and their guarded atexit destructors), emitted by these definitions.

SidebarClass::SBGadgetClass SidebarClass::Background;
ShapeButtonClass SidebarClass::unknown_button_ab96b8;
ShapeButtonClass SidebarClass::unknown_button_abcc58;
ShapeButtonClass SidebarClass::unknown_buttons_ab9508[4];
ShapeButtonClass SidebarClass::unknown_button_abcbe8;
ShapeButtonClass SidebarClass::unknown_button_abccc0;
SidebarClass::StripClass::SelectClass SidebarClass::SelectButton[240];


// Background atexit destructor (0x0067B4E0), emitted by the static definitions in SidebarClass_statics_init_0067b4b0.cpp.


// unknown_button_ab96b8 initializer (0x0067B500), emitted by the static definitions in SidebarClass_statics_init_0067b4b0.cpp.


// unknown_button_ab96b8 atexit destructor (0x0067B520), emitted by the static definitions in SidebarClass_statics_init_0067b4b0.cpp.


// unknown_button_abcc58 initializer (0x0067B540), emitted by the static definitions in SidebarClass_statics_init_0067b4b0.cpp.


// unknown_button_abcc58 atexit destructor (0x0067B560), emitted by the static definitions in SidebarClass_statics_init_0067b4b0.cpp.


// unknown_buttons_ab9508 initializer (0x0067B580), emitted by the static definitions in SidebarClass_statics_init_0067b4b0.cpp.


// unknown_buttons_ab9508 atexit destructor (0x0067B5B0), emitted by the static definitions in SidebarClass_statics_init_0067b4b0.cpp.


// unknown_button_abcbe8 initializer (0x0067B5E0), emitted by the static definitions in SidebarClass_statics_init_0067b4b0.cpp.


// unknown_button_abcbe8 atexit destructor (0x0067B600), emitted by the static definitions in SidebarClass_statics_init_0067b4b0.cpp.


// unknown_button_abccc0 initializer (0x0067B620), emitted by the static definitions in SidebarClass_statics_init_0067b4b0.cpp.


// unknown_button_abccc0 atexit destructor (0x0067B640), emitted by the static definitions in SidebarClass_statics_init_0067b4b0.cpp.


// SelectButton initializer (0x0067B660), emitted by the static definitions in SidebarClass_statics_init_0067b4b0.cpp.


// SelectButton atexit destructor (0x0067B6B0), emitted by the static definitions in SidebarClass_statics_init_0067b4b0.cpp.


// SidebarClass::SBGadgetClass::SBGadgetClass() (0x0067B6E0)
// The sidebar background gadget: a 1x1 gadget with flag 8.

SidebarClass::SBGadgetClass::SBGadgetClass() : GadgetClass(0, 0, 1, 1, 8, false)
{
}


SidebarClass::SidebarClass() :
    unknown_4ed8(0),
    unknown_4edc(0),
    unknown_4ee0(false),
    unknown_4ee1(false),
    unknown_4ee2(true),
    unknown_4ee4(false),
    unknown_4ee5(false),
    unknown_4ee6(false)
{
    for (int index = 0; index < 4; index++) {
        new (&Column[index]) StripClass(InitClass());
    }
}


SidebarClass::SidebarClass(NoInitClass const &noinit) : PowerClass(noinit)
{
}


// void SidebarClass::Unknown_0067B880(bool cameo_text) (0x0067B880)
// Empty in this build (OptionsClass's SidebarCameoText setting).

void SidebarClass::Unknown_0067B880(bool cameo_text)
{
}


// void SidebarClass::One_Time() (0x0067B890, slot 5)
// After the power bar's, retrieves "DARKEN.SHP" once per strip.

void SidebarClass::One_Time()
{
    PowerClass::One_Time();
    for (int index = 0; index < 4; index++) {
        unknown_shape_ab9480 = MixFileClass::Retrieve("DARKEN.SHP");
    }
}


void SidebarClass::Init_Clear()
{
    PowerClass::Init_Clear();
    unknown_4ee2 = true;
    unknown_4ee6 = false;
    unknown_4ee5 = false;
    for (int index = 0; index < 4; index++) {
        Column[index].Unknown_0067E770();
    }
    unknown_4ed8 = 0;
    unknown_4edc = 0;
    unknown_slot54(0);
}


// void SidebarClass::Unknown_0067B910() (0x0067B910)
// Lays out the sidebar from the screen size: its width and height, the
// strip and button positions (two variants by the 0x00A3D290 option), the
// icon count that fits (a multiple of 50 pixels of height) and the
// background rectangle.

void SidebarClass::Unknown_0067B910()
{
    unknown_int_839628 = unknown_int_7add80;
    unknown_int_839624 = 0x9E;
    unknown_int_83962c = unknown_int_839634 + unknown_int_83963c + unknown_int_7add84 - 0x9E;
    if (unknown_id_source_a3d290->unknown_2239) {
        unknown_abcd4c = 0xA6;
        unknown_abcd50 = 0x40;
        unknown_abcd58 = 0xC5;
        unknown_abcd5c = 0x1D;
        unknown_abcd64 = 0xE3;
        unknown_abcd68 = 0x3F;
    } else {
        unknown_abcd4c = 0xA5;
        unknown_abcd50 = 0x34;
        unknown_abcd58 = 0xC5;
        unknown_abcd5c = 0x20;
        unknown_abcd64 = 0xE3;
        unknown_abcd68 = 0x40;
    }
    unknown_abcd6c = 0x32;
    int extra;
    if (unknown_id_source_a3d290->unknown_2239) {
        extra = 26;
    } else {
        extra = 18;
    }
    unknown_int_abcd70 = (unknown_int_83962c - extra - 0x4C) / 50 * 50;
    if (unknown_id_source_a3d290->unknown_2239) {
        unknown_abcd78 = unknown_int_abcd70 + 0xEA;
        unknown_abcd7c = 0x2E;
        unknown_abcd80 = 0x32;
    } else {
        unknown_abcd78 = unknown_int_abcd70 + 0xEA;
        unknown_abcd7c = 0x2D;
        unknown_abcd80 = 0x32;
    }
}


// void SidebarClass::Unknown_0067BA60() (0x0067BA60)
// Places the sidebar right of the tactical area (when shown) and its
// control columns.

void SidebarClass::Unknown_0067BA60()
{
    int x = unknown_flag_a40b30 ? unknown_int_839630 + unknown_int_839638 : 0;
    unknown_int_839620 = x;
    unknown_abcd60 = x + 22;
    if (unknown_id_source_a3d290->unknown_2239) {
        unknown_abcd48 = x + 20;
        unknown_abcd54 = x + 26;
        unknown_abcd74 = x + 39;
    } else {
        unknown_abcd48 = x + 33;
        unknown_abcd54 = x + 20;
        unknown_abcd74 = x + 39;
    }
}


// void SidebarClass::Init_IO() (0x0067BAD0, slot 8)
// Places the sidebar, configures its buttons (the two top buttons, the four
// tab buttons and the two scroll buttons) and the four strips.

void SidebarClass::Init_IO()
{
    RadarClass::Init_IO();
    Unknown_0067B910();
    Unknown_0067BA60();
    if (Debug_Map) {
        return;
    }

    unknown_button_abcc58.IsSticky = true;
    unknown_button_abcc58.ID = 101;
    unknown_button_abcc58.X = unknown_abcd48;
    unknown_button_abcc58.Y = unknown_abcd4c;
    unknown_button_abcc58.DrawOffsetX = -480;
    unknown_button_abcc58.DrawOffsetY = 0;
    unknown_button_abcc58.DrawOnSidebar = true;
    unknown_button_abcc58.ShapeDrawer = unknown_drawer_8323cc;
    unknown_button_abcc58.IsOn = false;
    unknown_button_abcc58.ToggleType = 1;
    unknown_button_abcc58.ReflectButtonState = true;
    unknown_button_abcc58.Stop_Flashing();

    unknown_button_ab96b8.IsSticky = true;
    unknown_button_ab96b8.ID = 102;
    unknown_button_ab96b8.X = unknown_button_abcc58.X + unknown_abcd50;
    unknown_button_ab96b8.Y = unknown_button_abcc58.Y;
    unknown_button_ab96b8.DrawOffsetX = -480;
    unknown_button_ab96b8.DrawOffsetY = 0;
    unknown_button_ab96b8.DrawOnSidebar = true;
    unknown_button_ab96b8.ShapeDrawer = unknown_drawer_8323cc;
    unknown_button_ab96b8.IsOn = false;
    unknown_button_ab96b8.ToggleType = 1;
    unknown_button_ab96b8.ReflectButtonState = true;
    unknown_button_ab96b8.Stop_Flashing();

    for (int index = 0; index < 4; index++) {
        unknown_buttons_ab9508[index].IsSticky = true;
        unknown_buttons_ab9508[index].ID = 203 + index;
        unknown_buttons_ab9508[index].X = unknown_abcd5c * index + unknown_abcd54;
        unknown_buttons_ab9508[index].Y = unknown_abcd58;
        unknown_buttons_ab9508[index].DrawOffsetX = -480;
        unknown_buttons_ab9508[index].DrawOffsetY = 0;
        unknown_buttons_ab9508[index].DrawOnSidebar = true;
        unknown_buttons_ab9508[index].ShapeDrawer = unknown_drawer_8323cc;
        unknown_buttons_ab9508[index].IsOn = false;
        unknown_buttons_ab9508[index].ToggleType = 2;
        unknown_buttons_ab9508[index].ReflectButtonState = true;
        unknown_buttons_ab9508[index].Disable();
        unknown_buttons_ab9508[index].Stop_Flashing();
    }

    unknown_button_abcbe8.IsSticky = true;
    unknown_button_abcbe8.ID = 201;
    unknown_button_abcbe8.DrawOnSidebar = true;
    unknown_button_abcbe8.ShapeDrawer = unknown_drawer_8323cc;
    unknown_button_abcbe8.Flags = 0x55;
    unknown_button_abcbe8.Stop_Flashing();

    unknown_button_abccc0.IsSticky = true;
    unknown_button_abccc0.ID = 200;
    unknown_button_abccc0.DrawOnSidebar = true;
    unknown_button_abccc0.ShapeDrawer = unknown_drawer_8323cc;
    unknown_button_abccc0.Flags = 0x55;
    unknown_button_abccc0.Stop_Flashing();

    for (int column = 0; column < 4; column++) {
        Column[column].unknown_1c = unknown_abcd60;
        Column[column].unknown_20 = unknown_abcd64;
        Column[column].unknown_24 = Rect(0, 0, 60, unknown_int_abcd70);
        Column[column].Unknown_0067E7E0(column);
    }
    Reposition_Sidebar();
    if (unknown_4ee1) {
        unknown_slot54(0);
        unknown_slot54(1);
    }
}


// void SidebarClass::Init_For_House() (0x0067BD90, slot 53)
// Loads the sidebar shapes: a new drawer for the current palette, the
// clock, the sell/repair/tab/scroll button shapes and the sidebar pieces.

void SidebarClass::Init_For_House()
{
    PowerClass::Init_For_House();
    PaletteClass palette = *Unknown_006F45E0();
    if (unknown_drawer_8323cc) {
        delete unknown_drawer_8323cc;
        unknown_drawer_8323cc = 0;
    }
    unknown_drawer_8323cc = new ConvertClass(palette, palette, unknown_surface_839950, 1, false);
    unknown_shapes_abcd20[7] = Unknown_Load_Shape_00495270("GCLOCK2.SHP", unknown_shapes_allocated_abcd40[7]);

    bool allocated = false;
    unknown_button_ab96b8.Set_Shape(Unknown_Load_Shape_00495270("SELL.SHP", allocated));
    unknown_button_ab96b8.Set_Shape_Loaded(allocated);
    unknown_button_ab96b8.ShapeDrawer = unknown_drawer_8323cc;

    unknown_button_abcc58.Set_Shape(Unknown_Load_Shape_00495270("REPAIR.SHP", allocated));
    unknown_button_abcc58.Set_Shape_Loaded(allocated);
    unknown_button_abcc58.ShapeDrawer = unknown_drawer_8323cc;

    for (int index = 0; index < 4; index++) {
        char name[256];
        sprintf(name, "TAB%02d.SHP", index);
        unknown_buttons_ab9508[index].Set_Shape(Unknown_Load_Shape_00495270(name, allocated));
        unknown_buttons_ab9508[index].Set_Shape_Loaded(allocated);
        unknown_buttons_ab9508[index].ShapeDrawer = unknown_drawer_8323cc;
    }

    unknown_button_abcbe8.Set_Shape(Unknown_Load_Shape_00495270("R-DN.SHP", allocated));
    unknown_button_abcbe8.Set_Shape_Loaded(allocated);
    unknown_button_abcbe8.ShapeDrawer = unknown_drawer_8323cc;

    unknown_button_abccc0.Set_Shape(Unknown_Load_Shape_00495270("R-UP.SHP", allocated));
    unknown_button_abccc0.Set_Shape_Loaded(allocated);
    unknown_button_abccc0.ShapeDrawer = unknown_drawer_8323cc;

    unknown_shapes_abcd20[0] = Unknown_Load_Shape_00495270("SIDE1.SHP", unknown_shapes_allocated_abcd40[0]);
    unknown_shapes_abcd20[1] = Unknown_Load_Shape_00495270("SIDE2.SHP", unknown_shapes_allocated_abcd40[1]);
    unknown_shapes_abcd20[2] = Unknown_Load_Shape_00495270("SIDE3.SHP", unknown_shapes_allocated_abcd40[2]);
    unknown_shapes_abcd20[3] = Unknown_Load_Shape_00495270("ADDON.SHP", unknown_shapes_allocated_abcd40[3]);
}


// void SidebarClass::DisposeOfArt() (0x0067BFC0, slot 50)
// Frees the allocated sidebar shapes and the buttons' shapes.

void SidebarClass::DisposeOfArt()
{
    PowerClass::DisposeOfArt();
    if (unknown_shapes_allocated_abcd40[0]) {
        operator delete((void *)unknown_shapes_abcd20[0]);
        unknown_shapes_allocated_abcd40[0] = false;
    }
    if (unknown_shapes_allocated_abcd40[1]) {
        operator delete((void *)unknown_shapes_abcd20[1]);
        unknown_shapes_allocated_abcd40[1] = false;
    }
    if (unknown_shapes_allocated_abcd40[2]) {
        operator delete((void *)unknown_shapes_abcd20[2]);
        unknown_shapes_allocated_abcd40[2] = false;
    }
    if (unknown_shapes_allocated_abcd40[3]) {
        operator delete((void *)unknown_shapes_abcd20[3]);
        unknown_shapes_allocated_abcd40[3] = false;
    }
    if (unknown_shapes_allocated_abcd40[4]) {
        operator delete((void *)unknown_shapes_abcd20[4]);
        unknown_shapes_allocated_abcd40[4] = false;
    }
    if (unknown_shapes_allocated_abcd40[5]) {
        operator delete((void *)unknown_shapes_abcd20[5]);
        unknown_shapes_allocated_abcd40[5] = false;
    }
    if (unknown_shapes_allocated_abcd40[6]) {
        operator delete((void *)unknown_shapes_abcd20[6]);
        unknown_shapes_allocated_abcd40[6] = false;
    }
    if (unknown_shapes_allocated_abcd40[7]) {
        operator delete((void *)unknown_shapes_abcd20[7]);
        unknown_shapes_allocated_abcd40[7] = false;
    }
    unknown_button_ab96b8.Clear_Shape();
    unknown_button_abcc58.Clear_Shape();
    for (int index = 0; index < 4; index++) {
        unknown_buttons_ab9508[index].Clear_Shape();
    }
    unknown_button_abcbe8.Clear_Shape();
    unknown_button_abccc0.Clear_Shape();
}


// void SidebarClass::Unknown_0067C100() (0x0067C100)
// Redraws the active strip, and again within two frames.

void SidebarClass::Unknown_0067C100()
{
    unknown_4ee2 = true;
    Column[unknown_4ed8].unknown_38 = true;
    Map.Flag_To_Redraw(false);
    if (unknown_4edc <= Frame) {
        unknown_4edc = Frame + 2;
    }
}


// void SidebarClass::Unknown_0067C150(ObjectClass *object) (0x0067C150)
// Redraws the active strip when an object of its kind appears that the
// player can build, or the fourth strip for buildings flagged at +0x1365.

void SidebarClass::Unknown_0067C150(ObjectClass *object)
{
    if (!unknown_4ee1 || !object) {
        return;
    }
    TechnoTypeClass *type = object->Techno_Type_Class();
    if (!unknown_player_a35db4) {
        return;
    }
    int kind = object->Kind_Of();
    int id = object->Fetch_Heap_ID();
    if (Which_Column(kind, id) == unknown_4ed8) {
        if (((UnknownSidebarHouse *)unknown_player_a35db4)->Unknown_004F4F30(type)) {
            Unknown_0067C100();
        }
    } else if (kind == 6 && ((UnknownSidebarBuilding *)object)->Type->unknown_1365) {
        Unknown_0067C310(3);
    }
}


// void SidebarClass::Unknown_0067C2D0(int column) (0x0067C2D0)
// Redraws a strip when it is the active one.

void SidebarClass::Unknown_0067C2D0(int column)
{
    if (column == unknown_4ed8) {
        unknown_4ee2 = true;
        Column[column].unknown_38 = true;
        Map.Flag_To_Redraw(false);
    }
}


// void SidebarClass::Unknown_0067C310(int column) (0x0067C310)
// Redraws a strip when it is the active one, and again within two frames.

void SidebarClass::Unknown_0067C310(int column)
{
    if (column == unknown_4ed8) {
        unknown_4ee2 = true;
        Column[unknown_4ed8].unknown_38 = true;
        Map.Flag_To_Redraw(false);
        if (unknown_4edc <= Frame) {
            unknown_4edc = Frame + 2;
        }
    }
}


// bool SidebarClass::Unknown_0067C370(FactoryClass *factory, int type, int id) (0x0067C370)
// Finds the strip a buildable type belongs to and updates it there.

bool SidebarClass::Unknown_0067C370(FactoryClass *factory, int type, int id)
{
    return Column[Which_Column(type, id)].Unknown_006816D0(factory, type, id);
}


bool SidebarClass::Unknown_0067C450(int control)
{
    bool old = unknown_4ee6;
    if (control == -1) {
        control = !old;
    }
    switch (control) {
    case 1:
        unknown_4ee6 = true;
        break;
    default:
        unknown_4ee6 = false;
        break;
    }
    if (old != unknown_4ee6) {
        Flag_To_Redraw(false);
        unknown_4ee2 = true;
        if (!unknown_4ee6) {
            Set_Default_Mouse(0, false);
        }
    }
    return old;
}


bool SidebarClass::Unknown_0067C4C0(int control)
{
    bool old = unknown_4ee5;
    if (control == -1) {
        control = !old;
    }
    switch (control) {
    case 1:
        unknown_4ee5 = true;
        break;
    default:
        unknown_4ee5 = false;
        break;
    }
    if (old != unknown_4ee5) {
        Flag_To_Redraw(false);
        unknown_4ee2 = true;
        if (!unknown_4ee5) {
            Set_Default_Mouse(0, false);
        }
    }
    return old;
}


// bool SidebarClass::Unknown_0067C530(int type, int id) (0x0067C530)
// Adds a buildable to its strip: enables the strip's tab, shows the sidebar
// and switches to the first filled strip when every strip was empty.

bool SidebarClass::Unknown_0067C530(int type, int id)
{
    if (!Debug_Map) {
        int first = Unknown_00681980();
        int column = Which_Column(type, id);
        if (Column[column].Unknown_0067EDF0(type, id)) {
            Unknown_0067C910(column);
            unknown_slot54(1);
            if (first == -1) {
                Select_Tab(Unknown_00681980());
            }
            unknown_4ee2 = true;
            Flag_To_Redraw(false);
            if (column == unknown_4ed8) {
                unknown_flag_abcd84 = true;
            }
            return true;
        }
    }
    return false;
}


// void SidebarClass::Unknown_0067C850() (0x0067C850)
// Enables the scroll buttons while the active strip has more icons than fit.

void SidebarClass::Unknown_0067C850()
{
    if (Column[unknown_4ed8].BuildableCount > Unknown_006820F0()) {
        if (!unknown_button_abcbe8.Is_Enabled()) {
            unknown_button_abcbe8.Enable();
        }
        if (!unknown_button_abccc0.Is_Enabled()) {
            unknown_button_abccc0.Enable();
        }
    } else {
        if (unknown_button_abcbe8.Is_Enabled()) {
            unknown_button_abcbe8.Disable();
        }
        if (unknown_button_abccc0.Is_Enabled()) {
            unknown_button_abccc0.Disable();
        }
    }
}


// void SidebarClass::Unknown_0067C910(int index) (0x0067C910)
// Enables a tab button (choosing the first tab when no strip has icons and
// the sidebar piece at 0xABCD30 is loaded) and refreshes the scroll buttons
// for the active tab.

void SidebarClass::Unknown_0067C910(int index)
{
    if (Unknown_00681980() == -1 && unknown_shapes_abcd20[4]) {
        unknown_4ed4 = 1;
        unknown_4ed0 = 0;
    }
    if (!unknown_buttons_ab9508[index].Is_Enabled()) {
        unknown_buttons_ab9508[index].Enable();
    }
    if (index == unknown_4ed8) {
        Unknown_0067C850();
    }
}


// void SidebarClass::Unknown_0067CA50(int index) (0x0067CA50)
// A strip changed: refreshes the scroll buttons while it still has buildables; otherwise
// disables its tab and switches to the first filled strip; with none left it refreshes the
// scroll buttons and resets +0x4ED4/+0x4ED0 from shape 4's frame count.

void SidebarClass::Unknown_0067CA50(int index)
{
    if (Column[index].BuildableCount > 0) {
        Unknown_0067C850();
    } else {
        unknown_buttons_ab9508[index].Disable();
        int first = -1;
        for (int i = 0; i < 4; i++) {
            if (Column[i].BuildableCount > 0) {
                first = i;
                break;
            }
        }
        if (first == -1) {
            Unknown_0067C850();
            if (unknown_shapes_abcd20[4]) {
                unknown_4ed4 = -1;
                unknown_4ed0 = ((ShapeFileStruct const *)unknown_shapes_abcd20[4])->FrameCount;
            }
        } else {
            Select_Tab(first);
        }
    }
}


// bool SidebarClass::Scroll(bool up, int column) (0x0067CDD0)
// Scrolls a strip (the active one for -1) by a row and redraws.

bool SidebarClass::Scroll(bool up, int column)
{
    if (unknown_int_a40d34) {
        return false;
    }
    if (column == -1) {
        column = unknown_4ed8;
    }
    if (Column[column].Scroll(up, 1)) {
        Flag_To_Redraw(false);
        return true;
    }
    return false;
}


// bool SidebarClass::Scroll_Page(bool up, int column) (0x0067CE90)
// Scrolls a strip (the active one for -1) by a page and redraws.

bool SidebarClass::Scroll_Page(bool up, int column)
{
    if (column == -1) {
        column = unknown_4ed8;
    }
    StripClass &strip = Column[column];
    int amount = Unknown_006820A0();
    if (strip.Scroll(up, amount)) {
        Flag_To_Redraw(false);
        return true;
    }
    return false;
}


// void SidebarClass::Draw_It(bool complete) (0x0067CF80, slot 16)
// Draws the sidebar onto its surface: the frame pieces (SIDE1, a SIDE2 per
// icon row, SIDE3, ADDON), the buttons, the overlay at 0xABCD30-0xABCD38,
// the active strip and the power bar; then notes repaints.

void SidebarClass::Draw_It(bool complete)
{
    unknown_rect_8336e8 = StripClass::unknown_rect_ab9688;
    Surface *old = unknown_surface_83995c;
    unknown_surface_83995c = unknown_surface_839948;
    bool drawn = false;
    Rect window(0, 0, unknown_surface_839948->Get_Width(), unknown_surface_839948->Get_Height());
    complete = complete || unknown_4ee2 || unknown_4ee3;

    if (unknown_4ee1 && complete && !Debug_Map) {
        if (unknown_4ee3) {
            drawn = true;
            CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shapes_abcd20[0], 0,
                Point2D(0, unknown_int_839624), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            int y = unknown_abcd64;
            int row = 0;
            while (true) {
                if (row >= StripClass::Max_Visible_Rows()) {
                    break;
                }
                CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shapes_abcd20[1], 0,
                    Point2D(0, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                y += ((ShapeFileStruct const *)unknown_shapes_abcd20[1])->Height;
                row++;
            }
            CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shapes_abcd20[2], 0,
                Point2D(0, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shapes_abcd20[3], 0,
                Point2D(0, y + ((ShapeFileStruct const *)unknown_shapes_abcd20[2])->Height), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        }
        unknown_button_ab96b8.Draw_Me(drawn);
        unknown_button_abcc58.Draw_Me(drawn);
        for (int index = 0; index < 4; index++) {
            unknown_buttons_ab9508[index].Draw_Me(true);
        }
        unknown_button_abcbe8.Draw_Me(drawn);
        unknown_button_abccc0.Draw_Me(drawn);
        if (unknown_4ed4) {
            CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shapes_abcd20[4], unknown_4ed0,
                Point2D(0, unknown_abcd64), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shapes_abcd20[5], unknown_4ed0,
                Point2D(0, unknown_abcd64), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shapes_abcd20[6], unknown_4ed0,
                Point2D(0, unknown_abcd64), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        }
        unknown_flag_abcd84 = true;
    }
    Column[unknown_4ed8].Unknown_0067F800(unknown_4ee3);
    PowerClass::Draw_It(complete || drawn);
    if (unknown_button_ab96b8.IsDrawn) {
        unknown_button_ab96b8.IsDrawn = false;
        unknown_flag_abcd84 = true;
    }
    if (unknown_button_abcc58.IsDrawn) {
        unknown_button_abcc58.IsDrawn = false;
        unknown_flag_abcd84 = true;
    }
    for (int tab = 0; tab < 4; tab++) {
        if (unknown_buttons_ab9508[tab].IsDrawn) {
            unknown_flag_abcd84 = true;
            unknown_buttons_ab9508[tab].IsDrawn = false;
        }
    }
    if (unknown_button_abcbe8.IsDrawn) {
        unknown_button_abcbe8.IsDrawn = false;
        unknown_flag_abcd84 = true;
    }
    if (unknown_button_abccc0.IsDrawn) {
        unknown_button_abccc0.IsDrawn = false;
        unknown_flag_abcd84 = true;
    }
    if (ToolTips) {
        ToolTips->Refresh(true);
    }
    Unknown_0067D420(unknown_flag_abcd84 || unknown_4ee3);
    unknown_4ee2 = false;
    unknown_4ee3 = false;
    unknown_surface_83995c = old;
}


// void SidebarClass::Unknown_0067D420(bool complete) (0x0067D420)
// Blits the sidebar surface to the screen: everything when complete or a
// full redraw was requested, else only the changed rectangle (or the credits
// strip when that alone changed).
extern bool unknown_session_a40d04;     // 0x00A40D04

void SidebarClass::Unknown_0067D420(bool complete)
{
    if (unknown_4ee1 && unknown_flag_a40958 && unknown_session_a40d04) {
        RECT crect;
        GetClientRect(MainWindow, &crect);
        POINT scrnpt;
        scrnpt.x = crect.left;
        scrnpt.y = crect.top;
        ClientToScreen(MainWindow, &scrnpt);

        if (!unknown_flag_abcd84 && !complete) {
            unknown_flag_abcd84 = false;
            if (unknown_rect_8336e8 == StripClass::unknown_rect_ab9688) {
                if (unknown_4ee4) {
                    WWMouse->Draw_Mouse(unknown_surface_839948, true);
                    unknown_surface_839950->Copy_From(
                        Rect(scrnpt.x + (unknown_flag_a40b30 ? unknown_int_839638 : 0), scrnpt.y, unknown_int_839628, 16),
                        *unknown_surface_839948, Rect(0, 0, unknown_int_839628, 16), false, true);
                    unknown_4ee4 = false;
                    unknown_flag_abcd85 = true;
                }
                unknown_flag_abcd84 = false;
                return;
            }
        } else {
            unknown_flag_abcd84 = true;
        }

        WWMouse->Draw_Mouse(unknown_surface_839948, true);
        if (unknown_rect_8336e8 == StripClass::unknown_rect_ab9688 && !complete) {
            if (unknown_4ee4) {
                unknown_surface_839950->Copy_From(
                    Rect(scrnpt.x + (unknown_flag_a40b30 ? unknown_int_839638 : 0), scrnpt.y, unknown_int_839628, 16),
                    *unknown_surface_839948, Rect(0, 0, unknown_int_839628, 16), false, true);
                unknown_4ee4 = false;
            }
            unknown_surface_839950->Copy_From(
                Rect(scrnpt.x + (unknown_flag_a40b30 ? unknown_int_839638 : 0), scrnpt.y + unknown_int_839624,
                     unknown_int_839628, unknown_surface_839948->Get_Height() - unknown_int_839624),
                *unknown_surface_839948, Rect(0, unknown_int_839624, unknown_int_839628, unknown_int_83962c), false, true);
        } else if (unknown_flag_abcd84) {
            Rect sb_rect = unknown_surface_839948->Get_Rect();
            unknown_surface_839950->Copy_From(
                Rect(scrnpt.x + (unknown_flag_a40b30 ? unknown_int_839638 : 0), scrnpt.y, sb_rect.Width, sb_rect.Height),
                *unknown_surface_839948, Rect(0, 0, sb_rect.Width, sb_rect.Height), false, true);
        } else {
            unknown_surface_839950->Copy_From(
                Rect(scrnpt.x + unknown_rect_8336e8.X + (unknown_flag_a40b30 ? unknown_int_839638 : 0),
                     scrnpt.y + unknown_rect_8336e8.Y, unknown_rect_8336e8.Width, unknown_rect_8336e8.Height),
                *unknown_surface_839948, unknown_rect_8336e8, false, true);
        }
        unknown_flag_abcd85 = true;
    }
    unknown_flag_abcd84 = false;
}


// void SidebarClass::Activate_Tab(int index) (0x0067D7E0)
// Makes a tab current: presses its button, shows its strip and refreshes
// the scroll buttons.

void SidebarClass::Activate_Tab(int index)
{
    unknown_4ed8 = index;
    unknown_buttons_ab9508[index].Turn_On();
    Column[unknown_4ed8].Activate();
    Unknown_0067C850();
}


// void SidebarClass::Select_Tab(int index) (0x0067D990)
// Switches to another tab: releases the old tab button and hides its strip,
// then presses the new tab button, shows its strip and refreshes the scroll buttons.

void SidebarClass::Select_Tab(int index)
{
    if (index == unknown_4ed8) {
        return;
    }
    ShapeButtonClass &old_button = unknown_buttons_ab9508[unknown_4ed8];
    old_button.Turn_Off();
    StripClass &old = Column[unknown_4ed8];
    old.Unknown_0067F730();
    unknown_4ed8 = index;
    ShapeButtonClass &button = unknown_buttons_ab9508[unknown_4ed8];
    button.Turn_On();
    StripClass &current = Column[unknown_4ed8];
    current.Activate();
    Unknown_0067C850();
    unknown_4ee3 = true;
}


void SidebarClass::Unknown_0067DC00(int index)
{
    int delay = 10 - Frame % 10;
    unknown_buttons_ab9508[index].Start_Flashing(10, delay, (Frame + delay) / 10 % 2 == 0);
    Map.unknown_4ee3 = true;
    Map.Flag_To_Redraw(false);
}


bool SidebarClass::Unknown_0067DC70(int index)
{
    return unknown_buttons_ab9508[index].Stop_Flashing();
}


// void SidebarClass::AI(int &input, Point2D const &xy) (0x0067DC90, slot 10)
// The sidebar's per-frame update: runs the strips with the mouse position
// relative to the sidebar, steps the 0xABCD30 animation, follows the
// player's +0x230 count for the +0x4EE5 state, handles the sidebar's own
// buttons (0x8066 / 0x8065 mode switches, tabs 0x80CB-0x80CE, the scroll
// buttons), redraws flashing tabs, releases the mode buttons when their
// modes ended, and then runs the power bar.

void SidebarClass::AI(int &input, Point2D const &xy)
{
    Point2D point = xy - Point2D(unknown_rect_839620.X, 0);
    if (!Debug_Map) {
        unknown_slot54(1);
        for (int index = 0; index < 4; index++) {
            Column[index].AI(input, point);
        }
    }

    if (unknown_4ee1) {
        if (unknown_4ed4 == 1) {
            unknown_4ed0++;
            if (unknown_4ed0 > ((ShapeFileStruct const *)unknown_shapes_abcd20[4])->FrameCount) {
                unknown_4ed4 = 0;
                unknown_4ed0 = 0;
            }
        } else if (unknown_4ed4 == -1) {
            unknown_4ed0--;
            if (unknown_4ed0 < 0) {
                unknown_4ed4 = 0;
                unknown_4ed0 = 0;
            }
        }

        Unknown_0067C4C0(unknown_player_a35db4->unknown_230 > 0);

        if (input == 0x8066) {
            Voc_Play(Rules->unknown_180, 1.0f, 0x2000, 0);
            Unknown_0049C7C0(-1);
        } else if (input == 0x8065) {
            Voc_Play(Rules->unknown_180, 1.0f, 0x2000, 0);
            Unknown_0049CA20(-1);
        } else if (input >= 0x80CB && input < 0x80CF) {
            Select_Tab(input - 0x80CB);
            Voc_Play(Rules->unknown_188, 1.0f, 0x2000, 0);
        } else if ((input & ~0x4000) == (unknown_button_abcbe8.ID | 0x8000)) {
            unknown_button_abcbe8.IsPressed = false;
            StripClass &strip = Column[unknown_4ed8];
            if (!strip.Scroll(false, (unknown_int_839624 + unknown_int_83962c - unknown_abcd64 - (unknown_id_source_a3d290->unknown_2239 ? 26 : 18) - 7) / 50)) {
                Voc_Play(Rules->unknown_620, 1.0f, 0x2000, 0);
            } else {
                Voc_Play(Rules->unknown_188, 1.0f, 0x2000, 0);
            }
        } else if ((input & ~0x4000) == (unknown_button_abccc0.ID | 0x8000)) {
            unknown_button_abccc0.IsPressed = false;
            if (!Column[unknown_4ed8].Page(true)) {
                Voc_Play(Rules->unknown_620, 1.0f, 0x2000, 0);
            } else {
                Voc_Play(Rules->unknown_188, 1.0f, 0x2000, 0);
            }
        }

        bool flashed = false;
        for (int tab = 0; tab < 4; tab++) {
            if (unknown_buttons_ab9508[tab].AI()) {
                flashed = true;
            }
        }
        if (flashed) {
            unknown_4ee2 = true;
            unknown_flag_abcd84 = true;
            unknown_flag_8373c3 = true;
            Flag_To_Redraw(false);
        }
    }

    if (!unknown_11ad && unknown_button_ab96b8.IsOn) {
        unknown_button_ab96b8.Turn_Off();
    }
    if (!unknown_11ac && unknown_button_abcc58.IsOn) {
        unknown_button_abcc58.Turn_Off();
    }
    PowerClass::AI(input, xy);
}


// void SidebarClass::Unknown_0067E1C0() (0x0067E1C0)
// Runs every strip; redraws when the active one changed.

void SidebarClass::Unknown_0067E1C0()
{
    bool redraw = false;
    for (int index = 0; index < 4; index++) {
        if (Column[index].Unknown_006802D0() && index == unknown_4ed8) {
            Column[index].unknown_38 = true;
            redraw = true;
        }
    }
    if (redraw) {
        unknown_4ee2 = true;
        Flag_To_Redraw(false);
    }
}


// Not exact (RA2_NON_MATCHING): SidebarClass::unknown_slot54 at 0x0067E210, best score 99.04, 2026-11-13; differs: same instruction count (314), shape 99.04: instruction choice and registers differ
#ifdef RA2_NON_MATCHING
// bool SidebarClass::unknown_slot54(int control) (0x0067E210, slot 54)
// Shows (1), hides (0) or toggles (-1) the sidebar, adding or removing its
// buttons and the active strip; true when the state changed.

bool SidebarClass::unknown_slot54(int control)
{
    bool old = unknown_4ee1;
    if ((unknown_flags_a3f600 & 2) && unknown_game_mode_a3d298 && unknown_game_mode_a3d298 != 5) {
        return old;
    }
    if (control != -1) {
        unknown_4ee1 = (control == 1);
    } else {
        unknown_4ee1 = !old;
    }
    bool changed = (unknown_4ee1 != old);
    if (changed) {
        Set_Buttons(unknown_4ee1);
        if (unknown_4ee1) {
            Reposition_Sidebar();
            unknown_4ee2 = true;
            unknown_button_ab96b8.Zap();
            Add_A_Button(unknown_button_ab96b8);
            unknown_button_abcc58.Zap();
            Add_A_Button(unknown_button_abcc58);
            int index;
            for (index = 0; index < 4; index++) {
                unknown_buttons_ab9508[index].Zap();
                unknown_buttons_ab9508[index].IsPressed = false;
                Add_A_Button(unknown_buttons_ab9508[index]);
            }
            unknown_button_abcbe8.Zap();
            Map.Add_A_Button(unknown_button_abcbe8);
            unknown_button_abccc0.Zap();
            Map.Add_A_Button(unknown_button_abccc0);
            Background.Zap();
            Add_A_Button(Background);
            RadarButton.Zap();
            Add_A_Button(RadarButton);
            for (index = 0; index < 4; index++) {
                Column[index].unknown_40 = 0;
                if (Column[index].BuildableCount > 0) {
                    unknown_buttons_ab9508[index].Enable();
                }
            }
            unknown_buttons_ab9508[unknown_4ed8].Turn_On();
            Column[unknown_4ed8].Activate();
            Unknown_0067C850();
        } else {
            Unknown_005A09E0();
            Remove_A_Button(unknown_button_ab96b8);
            Remove_A_Button(unknown_button_abcc58);
            Remove_A_Button(Background);
            for (int index = 0; index < 4; index++) {
                Remove_A_Button(unknown_buttons_ab9508[index]);
            }
            Remove_A_Button(unknown_button_abcbe8);
            Remove_A_Button(unknown_button_abccc0);
            Column[unknown_4ed8].Deactivate();
            Remove_A_Button(RadarButton);
        }
        Flag_To_Redraw(2);
    }
    return changed;
}
#endif

// int __fastcall SidebarClass::Unknown_0067E640(int value) (0x0067E640)

int __fastcall SidebarClass::Unknown_0067E640(int value)
{
    return unknown_abcd68 * value;
}


// int __fastcall SidebarClass::Unknown_0067E650(int value) (0x0067E650)

int __fastcall SidebarClass::Unknown_0067E650(int value)
{
    return unknown_abcd6c * value + 1;
}


SidebarClass::StripClass::StripClass(InitClass const &) :
    unknown_1c(0),
    unknown_20(0),
    unknown_24(unknown_rect_ab9688),
    unknown_34(0),
    unknown_38(true),
    unknown_39(false),
    unknown_3a(false),
    unknown_3b(false),
    unknown_3c(-1),
    unknown_40(0),
    unknown_44(0),
    unknown_48(0),
    unknown_4c(0),
    BuildableCount(0)
{
    for (int index = 0; index < 75; index++) {
        Buildables[index].unknown_00 = 0;
        Buildables[index].unknown_04 = 0;
        Buildables[index].unknown_08 = 0;
        Buildables[index].Factory = 0;
    }
}


// void SidebarClass::StripClass::One_Time(int id) (0x0067E720)
// Loads the darkening overlay shape.

void SidebarClass::StripClass::One_Time(int id)
{
    SidebarClass::unknown_shape_ab9480 = MixFileClass::Retrieve("DARKEN.SHP");
}


// void const *SidebarClass::StripClass::Unknown_0067E740(int index) (0x0067E740)
// A super weapon type's sidebar icon, or null for a bad index.

void const *SidebarClass::StripClass::Unknown_0067E740(int index)
{
    if ((unsigned)index < (unsigned)SuperWeaponTypes.Count()) {
        return SuperWeaponTypes[index]->SidebarIcon;
    }
    return 0;
}


// void SidebarClass::StripClass::Unknown_0067E770() (0x0067E770)
// Clears the strip's state (Init_Clear): no scroll or redraw flags, no
// first icon, no buildables, and every buildable entry reset.

void SidebarClass::StripClass::Unknown_0067E770()
{
    unknown_3a = false;
    unknown_3b = false;
    unknown_39 = false;
    unknown_3c = -1;
    unknown_40 = 0;
    unknown_48 = 0;
    BuildableCount = 0;
    for (int index = 0; index < 75; index++) {
        Buildables[index].unknown_00 = 0;
        Buildables[index].unknown_04 = 0;
        Buildables[index].unknown_08 = 0;
        Buildables[index].Factory = 0;
        Buildables[index].unknown_10 = 0;
        Buildables[index].Stage.Set_Rate(0);
        Buildables[index].Stage.Value = 0;
        Buildables[index].unknown_2c = 0;
    }
}


// void SidebarClass::StripClass::Unknown_0067E7E0(int id) (0x0067E7E0)
// Numbers the strip and lays out its icon buttons (two per row) at the
// strip's origin.

void SidebarClass::StripClass::Unknown_0067E7E0(int id)
{
    unknown_34 = id;
    int rows = Map.Unknown_006820A0();
    for (int row = 0; row < rows; row++) {
        for (int column = 0; column < 2; column++) {
            int index = row * 2 + column;
            SelectClass &button = SelectButton[unknown_34 * 60 + index];
            button.ID = 202;
            button.X = Map.Column[unknown_34].unknown_1c + unknown_abcd68 * column;
            button.Y = Map.Column[unknown_34].unknown_20 + unknown_abcd6c * row + 1;
            button.Width = 60;
            button.Height = 48;
            button.Strip = this;
            button.Index = index;
        }
    }
}


// void SidebarClass::StripClass::Unknown_0067E8F0() (0x0067E8F0)
// Adds the strip's visible icon buttons to the screen's list.

void SidebarClass::StripClass::Unknown_0067E8F0()
{

    int max = Map.Unknown_006820F0();
    for (int index = 0; index < max; index++) {
        SelectButton[unknown_34 * 60 + index].Zap();
        Map.Add_A_Button(SelectButton[unknown_34 * 60 + index]);
    }
    unknown_flag_8373c3 = true;
}


// void SidebarClass::StripClass::Deactivate() (0x0067E9A0)
// Removes the strip's visible icon buttons from the screen's list.

void SidebarClass::StripClass::Deactivate()
{
    int max = Map.Unknown_006820F0();
    for (int index = 0; index < max; index++) {
        Map.Remove_A_Button(SelectButton[unknown_34 * 60 + index]);
    }
}


// bool SidebarClass::StripClass::Unknown_0067EA20(int type, int id, int other_type, int other_id) (0x0067EA20)
// The strip's sort order: whether the buildable (type, id) goes before
// (other_type, other_id). Superweapons (kinds 0x1F, 0x20, 0x39) come
// first, by recharge time then name; then the player's side's own types;
// then ground units, aircraft and naval units (kinds 3 and 0x28 by
// ConsideredAircraft and Naval); then by tech level, cost and name.

extern "C" int __cdecl wcscmp(wchar_t const *string1, wchar_t const *string2);   // 0x007837D3

bool SidebarClass::StripClass::Unknown_0067EA20(int type, int id, int other_type, int other_id)
{
    TechnoTypeClass *techno = (TechnoTypeClass *)Unknown_00482C00(type, id);
    TechnoTypeClass *other = (TechnoTypeClass *)Unknown_00482C00(other_type, other_id);
    wchar_t const *name;
    wchar_t const *other_name;
    if (other_type == 0) {
        return true;
    }

    bool super = type == 0x1F || type == 0x39 || type == 0x20;
    bool other_super = other_type == 0x1F || other_type == 0x39 || other_type == 0x20;
    if (super) {
        if (other_super) {
            SuperWeaponTypeClass *weapon = SuperWeaponTypes[id];
            SuperWeaponTypeClass *other_weapon = SuperWeaponTypes[other_id];
            if (weapon->RechargeTime < other_weapon->RechargeTime) {
                return true;
            }
            if (weapon->RechargeTime > other_weapon->RechargeTime) {
                return false;
            }
            name = weapon->UIName;
            other_name = other_weapon->UIName;
            return wcscmp(name, other_name) <= 0 ? true : false;
        }
    } else if (!other_super) {
        int side = unknown_player_a35db4->Type->Side;
        bool own = side == techno->AIBasePlanningSide;
        bool other_own = side == other->AIBasePlanningSide;
        if (own && !other_own) {
            return true;
        }
        if (!own && other_own) {
            return false;
        }
    }

    bool unit = type == 0x28 || type == 3;
    bool aircraft = unit && techno->ConsideredAircraft;
    bool naval = unit && techno->Naval;
    bool ground = !aircraft && !naval;
    bool other_unit = other_type == 0x28 || other_type == 3;
    bool other_aircraft = other_unit && other->ConsideredAircraft;
    bool other_naval = other_unit && other->Naval;
    bool other_ground = !other_aircraft && !other_naval;

    if (super) {
        if (other_aircraft || other_naval || other_ground) {
            return true;
        }
    } else if (ground) {
        if (other_super) {
            return false;
        }
        if (other_aircraft || other_naval) {
            return true;
        }
    } else if (aircraft) {
        if (other_super) {
            return false;
        }
        if (other_ground) {
            return false;
        }
        if (other_naval) {
            return true;
        }
    } else if (naval) {
        if (other_super) {
            return false;
        }
        if (other_ground || other_aircraft) {
            return false;
        }
    }

    if (techno->TechLevel < other->TechLevel) {
        return true;
    }
    if (techno->TechLevel > other->TechLevel) {
        return false;
    }
    int cost = techno->Cost_Of(unknown_player_a35db4);
    int other_cost = other->Cost_Of(unknown_player_a35db4);
    if (cost < other_cost) {
        return true;
    }
    if (cost > other_cost) {
        return false;
    }
    name = techno->UIName;
    other_name = other->UIName;
    return wcscmp(name, other_name) <= 0 ? true : false;
}


// void SidebarClass::StripClass::Unknown_0067ED10(int type, int id) (0x0067ED10)
// Inserts a buildable into the strip in sort order (0x0067EA20): the later
// entries move down one place, the new entry gets its type and id (and the
// kind of a type 7 buildable) and has its factory, build progress and
// stage cleared.

void SidebarClass::StripClass::Unknown_0067ED10(int type, int id)
{
    BuildableCount++;
    for (int index = 0; index < BuildableCount; index++) {
        int other_type = Buildables[index].unknown_04;
        int other_id = Buildables[index].unknown_00;
        if (Unknown_0067EA20(type, id, other_type, other_id)) {
            for (int move = BuildableCount - 1; move > index; move--) {
                Buildables[move] = Buildables[move - 1];
            }
            Buildables[index].unknown_04 = type;
            Buildables[index].unknown_00 = id;
            if (type == 7) {
                Buildables[index].unknown_08 = Unknown_004EB550(type, id);
            }
            Buildables[index].Factory = 0;
            Buildables[index].unknown_10 = 0;
            Buildables[index].Stage.Value = 0;
            Buildables[index].Stage.Set_Rate(0);
            Buildables[index].unknown_2c = 0;
            return;
        }
    }
}


// bool SidebarClass::StripClass::Unknown_0067EDF0(int type, int id) (0x0067EDF0)
// Adds a buildable unless full or present, announcing new construction
// options (except type 31).

bool SidebarClass::StripClass::Unknown_0067EDF0(int type, int id)
{
    if (BuildableCount <= 75) {
        for (int index = 0; index < BuildableCount; index++) {
            if (Buildables[index].unknown_04 == type && Buildables[index].unknown_00 == id) {
                return false;
            }
        }
        if (!unknown_a40774 && type != 31) {
            Speech_Play_Name("EVA_NewConstructionOptions", -1, -1);
        }
        Unknown_0067ED10(type, id);
        unknown_38 = true;
        unknown_flag_8373c3 = true;
        return true;
    }
    return false;
}


// bool SidebarClass::StripClass::Scroll(bool up, int amount) (0x0067EE60)
// Requests a scroll of the strip unless it is already at that end.

bool SidebarClass::StripClass::Scroll(bool up, int amount)
{
    if (up) {
        if (!unknown_40) {
            return false;
        }
        unknown_44 -= amount;
        return true;
    }
    if ((unknown_40 + Map.Unknown_006820A0()) * 2 >= BuildableCount) {
        return false;
    }
    unknown_44 += amount;
    return true;
}


// bool SidebarClass::StripClass::Step(bool up) (0x0067EEF0)
// Requests a one-row scroll.

bool SidebarClass::StripClass::Step(bool up)
{
    return Scroll(up, 1);
}


// bool SidebarClass::StripClass::Page(bool up) (0x0067EF70)
// Requests a one-page scroll.

bool SidebarClass::StripClass::Page(bool up)
{
    int amount = Map.Unknown_006820A0();
    return Scroll(up, amount);
}


// void SidebarClass::StripClass::Flag_To_Redraw() (0x0067F030)

void SidebarClass::StripClass::Flag_To_Redraw()
{
    unknown_38 = true;
    Map.Flag_To_Redraw(false);
}


// bool SidebarClass::StripClass::AI(int &input, Point2D const &xy) (0x0067F050)
// The strip's per-frame update: scrolling, the flash, the tab flash and the build clocks.

bool SidebarClass::StripClass::AI(int &input, Point2D const &xy)
{
    bool redraw = false;

    if (!unknown_3b && unknown_44) {
        int rows = Map.Unknown_006820A0();
        if (BuildableCount <= rows * 2) {
            unknown_44 = 0;
        } else if (unknown_44 < 0) {
            if (!unknown_40) {
                unknown_44 = 0;
            } else {
                unknown_44++;
                unknown_3a = false;
                unknown_3b = true;
                unknown_40--;
                unknown_48 = 0;
            }
        } else if ((unknown_40 + rows) * 2 >= BuildableCount) {
            unknown_44 = 0;
        } else {
            unknown_44--;
            unknown_48 = unknown_abcd6c;
            unknown_3a = true;
            unknown_3b = true;
        }
    }

    if (unknown_3b) {
        if (unknown_3a) {
            unknown_48 -= unknown_abcd80;
            if (unknown_48 <= 0) {
                unknown_3b = false;
                unknown_48 = 0;
                unknown_40++;
            }
        } else {
            unknown_48 += unknown_abcd80;
            if (unknown_48 >= unknown_abcd6c) {
                unknown_3b = false;
                unknown_48 = 0;
            }
        }
        redraw = true;
    }

    if (unknown_3c != -1) {
        if (unknown_00.Graphic_Logic()) {
            redraw = true;
            if (unknown_00.Fetch_Stage() >= 7) {
                unknown_00.Set_Rate(0);
                unknown_00.Set_Stage(0);
                unknown_3c = -1;
            }
        }
    }

    if (Map.unknown_4edc > Frame) {
        redraw = true;
    }

    if (unknown_34 == 0 || unknown_34 == 1) {
        for (int index = 0; index < BuildableCount; index++) {
            if (Buildables[index].unknown_04 == 31) {
                SuperClass *super = unknown_player_a35db4->Supers[Buildables[index].unknown_00];
                if (super && super->Unknown_0069D820()) {
                    goto flash;
                }
            } else {
                UnknownFactory *factory = Buildables[index].Factory;
                if (factory && factory->Unknown_004B9900()) {
                    ObjectClass *object = factory->Get_Object();
                    if (object && object->Kind_Of() == 6) {
                        goto flash;
                    }
                }
            }
        }
        unknown_buttons_ab9508[unknown_34].Stop_Flashing();
    }
    goto building;
flash:
    {
        int frame = Frame;
        int length = 10 - frame % 10;
        unknown_buttons_ab9508[unknown_34].Start_Flashing(10, length, ((frame + length) / 10) % 2 == 0);
        Map.unknown_4ee3 = true;
        Map.Flag_To_Redraw(false);
    }
building:

    if (unknown_39) {
        for (int index = 0; index < BuildableCount; index++) {
            UnknownFactory *factory = Buildables[index].Factory;
            if (factory && factory->Unknown_004B9430()) {
                redraw = true;
                if (factory->Unknown_004B9900()) {
                    TechnoClass *pending = (TechnoClass *)factory->Get_Object();
                    if (pending) {
                        switch (pending->Kind_Of()) {
                        case 6:
                            Speech_Play_Name("EVA_ConstructionComplete", -1, -1);
                            Unknown_006F8A00(As_Building(pending));
                            break;
                        case 1:
                        case 2:
                        case 15:
                            OutList.Add(EventClass(pending->Owner(), 11, pending->Kind_Of(), -1,
                                pending->Techno_Type_Class()->Naval, unknown_geometry_k));
                            Buildables[index].unknown_10 = 0;
                            break;
                        }
                    }
                }
            }
        }
    }

    for (int index = 0; index < BuildableCount; index++) {
        if (Buildables[index].unknown_10 == 1) {
            UnknownFactory *factory = Buildables[index].Factory;
            if (factory && (!factory->unknown_34 || factory->unknown_6c || factory->Unknown_004B9420())) {
                Buildables[index].Stage.Set_Rate(0);
                Buildables[index].Stage.Set_Stage(0);
            }
            if (Buildables[index].Stage.Graphic_Logic()) {
                redraw = true;
                if (Buildables[index].Stage.Fetch_Stage() >= 53) {
                    Buildables[index].Stage.Set_Rate(0);
                }
            }
        }
    }

    if (redraw) {
        Map.Unknown_0067C2D0(Map.unknown_4ed8);
        Flag_To_Redraw();
        Map.unknown_4ee3 = true;
        unknown_flag_abcd84 = true;
    }
    return redraw;
}


// wchar_t const *SidebarClass::StripClass::Help_Text(int gadget_index) (0x0067F580)
// The help text of a visible icon: a super weapon's name, or a type's name
// and cost for the player (one word per line).

wchar_t const *SidebarClass::StripClass::Help_Text(int gadget_index)
{
    int index = gadget_index + unknown_40 * 2;
    if (!unknown_flag_a40958 || index >= BuildableCount || BuildableCount >= 75) {
        return 0;
    }
    if (Buildables[index].unknown_04 == 31) {
        return SuperWeaponTypes[Buildables[index].unknown_00]->UIName;
    }
    ObjectTypeClass *type = Unknown_00482C00(Buildables[index].unknown_04, Buildables[index].unknown_00);
    if (unknown_flag_8373c0) {
        swprintf(unknown_text_ab9484,
            StringTable::Load_String("TXT_MONEY_FORMAT_1", 0, "D:\\Ra2\\Sidebar.CPP", 2627),
            type->Cost_Of(unknown_player_a35db4));
    } else {
        swprintf(unknown_text_ab9484,
            StringTable::Load_String("TXT_MONEY_FORMAT_2", 0, "D:\\Ra2\\Sidebar.CPP", 2629),
            type->UIName, type->Cost_Of(unknown_player_a35db4));
    }
    int length = wcslen(unknown_text_ab9484);
    for (int i = 0; i < length; i++) {
        if (unknown_text_ab9484[i] == ' ') {
            unknown_text_ab9484[i] = '\n';
        }
    }
    return unknown_text_ab9484;
}


// void SidebarClass::StripClass::Activate() (0x0067F680)
// Flags the strip and adds its visible icon buttons.

void SidebarClass::StripClass::Activate()
{
    unknown_18 = true;
    Unknown_0067E8F0();
}


// void SidebarClass::StripClass::Unknown_0067F730() (0x0067F730)
// Unflags the strip and removes its visible icon buttons.

void SidebarClass::StripClass::Unknown_0067F730()
{
    unknown_18 = false;
    Deactivate();
}


// int SidebarClass::StripClass::Unknown_0067F7C0(int index) (0x0067F7C0)
// An icon's progress: its factory's completion, or (for type 1) the mean
// with the stage value when that is ahead.

int SidebarClass::StripClass::Unknown_0067F7C0(int index)
{
    int progress = 0;
    if (Buildables[index].Factory) {
        progress = Buildables[index].Factory->Unknown_004B98F0();
    }
    if (Buildables[index].unknown_10 == 1 && Buildables[index].Stage.Value > progress) {
        progress = (progress + Buildables[index].Stage.Value) / 2;
    }
    return progress;
}


// void SidebarClass::StripClass::Unknown_0067F800(bool complete) (0x0067F800)
// Draws the strip's visible cameos: shape, hover frame, darkening, flash,
// caption, queue count, ready/hold text and the build clock.

static inline int Bottom_Extra()
{
    if (unknown_id_source_a3d290->unknown_2239) {
        return 26;
    }
    return 18;
}

void SidebarClass::StripClass::Unknown_0067F800(bool complete)
{
    if (unknown_18 && (unknown_38 || complete)) {
        unknown_38 = false;
        unknown_flag_abcd84 = true;
        Rect cliprect;
        cliprect.X = 0;
        cliprect.Y = 0;
        cliprect.Width = unknown_surface_839948->Get_Width();
        cliprect.Height = unknown_surface_839948->Get_Height();

        bool busy = false;
        for (int i = 0; i < BuildableCount; i++) {
            if (Buildables[i].unknown_10 == 1 || Buildables[i].unknown_10 == 2) {
                busy = true;
                break;
            }
        }

        int rows = (unknown_int_839624 + unknown_int_83962c - unknown_abcd64 - Bottom_Extra() - 7) / 50 + (unknown_3b ? 1 : 0);
        int columns = 2;
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                bool draw = complete;
                int stage = 0;
                void const *shape = 0;
                UnknownFactory *factory = 0;
                wchar_t const *state = 0;
                int index = col + (unknown_40 + row) * columns;
                bool production = false;
                bool completed = false;
                bool darken = false;
                bool hover = false;
                int x = unknown_abcd68 * col - unknown_rect_839620.X + unknown_1c;
                int y = unknown_abcd6c * row + 1 + unknown_20;
                SelectClass *button = &SelectButton[row * columns + unknown_34 * 60 + col];
                if (unknown_3b) {
                    y += unknown_48 - unknown_abcd6c;
                }
                wchar_t const *name = 0;
                TechnoTypeClass *obj = 0;

                if (index < BuildableCount) {
                    hover = button->Unknown_00681690();
                    if (Buildables[index].unknown_04 != 31) {
                        obj = (TechnoTypeClass *)Unknown_00482C00(Buildables[index].unknown_04, Buildables[index].unknown_00);
                        if (obj) {
                            name = obj->UIName;
                            bool isbusy = false;
                            if (obj->Kind_Of() == 7 && busy) {
                                isbusy = true;
                            } else if (obj->Who_Can_Build_Me(true, true, true, unknown_player_a35db4) == 0) {
                                isbusy = true;
                            } else {
                                TechnoTypeClass *type = (TechnoTypeClass *)Unknown_00482C00(Buildables[index].unknown_04, Buildables[index].unknown_00);
                                bool none = unknown_player_a35db4->Can_Build(type, false, false) == -1;
                                bool limited = ((UnknownSidebarHouse *)unknown_player_a35db4)->Unknown_004F4F30(type);
                                if (none || limited) {
                                    isbusy = true;
                                }
                            }
                            shape = obj->unknown_slot38();
                            factory = Buildables[index].Factory;
                            if (factory) {
                                production = true;
                                completed = factory->Unknown_004B9900();
                                if (completed) {
                                    state = StringTable::Load_String("TXT_READY", 0, "D:\\Ra2\\Sidebar.CPP", 2845);
                                } else {
                                    draw = true;
                                }
                                stage = Unknown_0067F7C0(index);
                                darken = false;
                            } else {
                                production = false;
                                darken = isbusy;
                                switch (Buildables[index].unknown_10) {
                                case 1:
                                    draw = true;
                                    production = true;
                                    stage = Unknown_0067F7C0(index);
                                    darken = false;
                                    break;
                                case 3:
                                    production = true;
                                    completed = true;
                                    darken = false;
                                    break;
                                case 2:
                                    production = true;
                                    completed = false;
                                    darken = true;
                                    stage = Unknown_0067F7C0(index);
                                    break;
                                case 0:
                                    if (!complete) {
                                        continue;
                                    }
                                    break;
                                }
                            }
                        } else {
                            darken = false;
                        }
                    } else {
                        int id = Buildables[index].unknown_00;
                        name = SuperWeaponTypes[id]->UIName;
                        shape = Unknown_0067E740(id);
                        production = true;
                        completed = !unknown_player_a35db4->Supers[id]->unknown_69e990();
                        state = unknown_player_a35db4->Supers[id]->State_String();
                        stage = unknown_player_a35db4->Supers[id]->Anim_Stage();
                        darken = false;
                        draw = true;
                    }
                } else {
                    production = false;
                }

                Rect window(0, 0, x + 60, y + 48);
                if (draw) {
                    if (shape) {
                        CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323b0, (ShapeFileStruct const *)shape, 0,
                            Point2D(x, y), window, 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                    }
                    if (hover) {
                        RGBClass color = unknown_color_ac1028;
                        unknown_surface_839948->Draw_Rect(window, Rect(x, y, 60, 48),
                            (int)(((color.Green >> unknown_shift_852fcc) << unknown_shift_852fc8) | ((color.Red >> unknown_shift_852fbc) << unknown_shift_852fb8) | ((color.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0)));
                    }
                    if (darken) {
                        CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shape_ab9480, 0,
                            Point2D(x, y), window, 0x401, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                    }
                    if (Buildables[index].unknown_2c > Frame && Frame % 16 > 8) {
                        CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shape_ab9480, 0,
                            Point2D(x, y), window, 0x404, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                    }
                    if (name) {
                        Unknown_00682140(name, Point2D(x, y + 36), cliprect, 60);
                    }
                    bool hasqueue = false;
                    unsigned pixel = ((unknown_color_ac1028.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
                        | ((unknown_color_ac1028.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0)
                        | ((unknown_color_ac1028.Green >> unknown_shift_852fcc) << unknown_shift_852fc8);
                    if (obj) {
                        FactoryClass *queue = ((UnknownSidebarHouse *)unknown_player_a35db4)->Unknown_004EB580(obj->Kind_Of(), obj->Naval, 0);
                        if (queue) {
                            int total = queue->Count_Type_004B9E40(obj);
                            if (total > 1 || total > 0 && (!queue->Object || queue->Object->Techno_Type_Class() && queue->Object->Techno_Type_Class() != obj)) {
                                wchar_t buffer[256];
                                swprintf(buffer, L"%d", total);
                                Point2D point(x + 60, y + 1);
                                Rect rect = Text_Rect(buffer, point.X, point.Y, 0x242, 2, 1);
                                Unknown_005feb60(&rect, unknown_surface_839948, 0, 0xaf);
                                Plain_Text_Print(L"%d", unknown_surface_839948, &cliprect, &point, pixel, 0, 0x242, total);
                                hasqueue = true;
                            }
                        }
                    }
                    if (production) {
                        if (state) {
                            Point2D point(x + 30, y + 1);
                            Rect rect = Text_Rect(state, point.X, point.Y, 0x142, 2, 1);
                            Unknown_005feb60(&rect, unknown_surface_839948, 0, 0xaf);
                            Plain_Text_Print(state, unknown_surface_839948, &cliprect, &point, pixel, 0, 0x142);
                        }
                        if (!completed) {
                            CC_Draw_Shape(unknown_surface_839948, unknown_drawer_8323cc, (ShapeFileStruct const *)unknown_shapes_abcd20[7], stage + 1,
                                Point2D(x, y), window, 0x404, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
                            if (factory && (!factory->unknown_34 || factory->unknown_6c) || Buildables[index].unknown_10 == 2) {
                                wchar_t const *hold = StringTable::Load_String("TXT_HOLD", 0, "D:\\Ra2\\Sidebar.CPP", 3070);
                                if (hasqueue) {
                                    Point2D point(x + 2, y + 1);
                                    Rect rect = Text_Rect(hold, point.X, point.Y, 0x42, 2, 1);
                                    Unknown_005feb60(&rect, unknown_surface_839948, 0, 0xaf);
                                    Plain_Text_Print(hold, unknown_surface_839948, &cliprect, &point, pixel, 0, 0x42);
                                } else {
                                    Point2D point(x + 30, y + 1);
                                    Rect rect = Text_Rect(hold, point.X, point.Y, 0x142, 2, 1);
                                    Unknown_005feb60(&rect, unknown_surface_839948, 0, 0xaf);
                                    Plain_Text_Print(hold, unknown_surface_839948, &cliprect, &point, pixel, 0, 0x142);
                                }
                            }
                        }
                    }
                }
            }
        }
        unknown_4c = unknown_48;
    }
}


SidebarClass::StripClass::SelectClass::SelectClass() :
    ControlClass(0, 0, 0, 60, 48, 0x19),
    Strip(0),
    Index(0)
{
}


// Not exact (RA2_NON_MATCHING): SidebarClass::StripClass::Unknown_006802D0 at 0x006802D0, best score 39.60, 2026-11-13; differs: 531 instructions in the original, 469 in the draft (shape 76.20)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x6802D0 SidebarClass::StripClass::Unknown_006802D0 (Recalc)
// entry 0x6802D0; exact-match score 35.99 (misses_index best); plan "draft, verify"
// origin lane: Sidebar361 (build/class-work/Sidebar361/w/rc_0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1089): the overlay w/hdr names replaced by HEAD's; each changed line is marked NATIVE.
// Best recorded 35.99 (Sidebar361, with its private header overlay w/hdr; later lanes 30.30/26.72 rebaselines); TS Recalc shape. Low score: behaviour follows TibSun sidebar.cpp StripClass::Recalc.
// bool SidebarClass::StripClass::Unknown_006802D0() (0x006802D0)
// Recalc: drops the icons the player can no longer build (abandoning their
// production), refreshes the tab, and keeps the first visible icon in view.

extern "C" void *__cdecl memmove(void *dest, void const *source, unsigned int count);

// NATIVE: the overlay's UnknownBuildable30 operator== (TibSun sidebar.h BuildType::operator==:
// the same id and type).
static inline bool Same_Buildable(UnknownBuildable30 const &a, UnknownBuildable30 const &b)
{
    return a.unknown_00 == b.unknown_00 && a.unknown_04 == b.unknown_04;
}

bool SidebarClass::StripClass::Unknown_006802D0()
{
    if (Debug_Map || !BuildableCount) {
        return false;
    }
    bool scroll = false;
    int max_visible = Max_Visible_Rows() * 2;
    UnknownBuildable30 *unshifted = new UnknownBuildable30[max_visible];
    for (int i = 0; i < max_visible; i++) {
        if (i + unknown_40 * 2 < BuildableCount) {
            unshifted[i] = Buildables[i + unknown_40 * 2];
        }
    }

    bool redraw = false;
    for (int index = 0; index < BuildableCount; index++) {
        int ok;
        TechnoTypeClass *tech = (TechnoTypeClass *)Unknown_00482C00(Buildables[index].unknown_04, Buildables[index].unknown_00);
        if (tech) {
            TechnoClass *who = (TechnoClass *)tech->Who_Can_Build_Me(true, false, false, unknown_player_a35db4);
            ok = who && who->House->Can_Build(tech, false, true);   // NATIVE: the overlay's UnknownSidebarHouse::Unknown_004E3660 is HouseClass::Can_Build 0x004E3660
        } else {
            if ((unsigned)Buildables[index].unknown_00 < (unsigned)unknown_player_a35db4->Supers.Count()) {
                ok = unknown_player_a35db4->Supers[Buildables[index].unknown_00]->Is_Present();
            } else {
                ok = false;
            }
        }
        if (!ok) {
            if (Buildables[index].Factory) {
                OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 16, Buildables[index].unknown_04,
                    Buildables[index].unknown_00, tech->Naval));
                if (Buildables[index].unknown_04 == 7 || Buildables[index].unknown_04 == 6) {
                    Map.unknown_11a0 = 0;
                    Map.unknown_11a4 = 0;
                    Map.unknown_11a8 = -1;
                    Map.Unknown_00498F10(0);
                }
            }
            if (tech) {
                FactoryClass *factory = unknown_player_a35db4->unknown_func_4eb580(tech->Kind_Of(), tech->Naval, 0);   // NATIVE: 0x004EB580 under house_screen.hpp's name
                if (factory) {
                    for (int q = factory->Queue.Count() - 1; q >= 0; q--) {
                        if (factory->Queue[q] == tech) {
                            OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 16, Buildables[index].unknown_04,
                                Buildables[index].unknown_00, tech->Naval));
                        }
                    }
                }
            }
            for (int i = 0; i < max_visible; i++) {
                if (Same_Buildable(unshifted[i], Buildables[index])) {   // NATIVE: the overlay's operator==
                    // Clear the identity used to retain the first surviving visible icon.
                    unshifted[i].unknown_00 = 0;
                    unshifted[i].unknown_04 = 0;
                }
            }
            if (BuildableCount > 1 && index < BuildableCount - 1) {
                memmove(&Buildables[index], &Buildables[index + 1], sizeof(Buildables[0]) * ((BuildableCount - index) - 1));
            }
            redraw = true;
            scroll = true;
            Buildables[BuildableCount - 1].Factory = 0;
            Buildables[BuildableCount - 1].unknown_00 = 0;
            Buildables[BuildableCount - 1].unknown_04 = 0;
            Buildables[BuildableCount - 1].unknown_08 = 0;
            Buildables[BuildableCount - 1].unknown_10 = 0;
            Buildables[BuildableCount - 1].Stage.Set_Rate(0);
            Buildables[BuildableCount - 1].Stage.Value = 0;
            Buildables[BuildableCount - 1].unknown_2c = 0;
            unknown_38 = true;
            BuildableCount--;
            index--;
        }
    }

    Map.Unknown_0067CA50(unknown_34);

    if (scroll) {
        bool got_old = false;
        bool got_new = false;
        int oldpos;
        for (oldpos = 0; oldpos < max_visible; oldpos++) {
            if (unshifted[oldpos].unknown_00 != 0 || unshifted[oldpos].unknown_04 != 0) {
                got_old = true;
                break;
            }
        }
        int newpos;
        if (got_old && BuildableCount != 0) {
            for (newpos = 0; newpos < BuildableCount; newpos++) {
                if (Same_Buildable(Buildables[newpos], unshifted[oldpos])) {   // NATIVE
                    got_new = true;
                    break;
                }
            }
        }
        if (got_old && got_new) {
            unknown_40 -= (oldpos - newpos) / 2;
            int last = (BuildableCount - max_visible) / 2;
            unknown_40 = 0 > (unknown_40 < last ? unknown_40 : last) ? 0 : (unknown_40 < last ? unknown_40 : last);
        } else {
            unknown_40 = 0;
        }
        Map.unknown_4ee3 = true;
    }
    delete [] unshifted;
    return redraw;
}
#endif

// void SidebarClass::StripClass::SelectClass::Set_Owner(StripClass &strip, int index) (0x006809D0)

void SidebarClass::StripClass::SelectClass::Set_Owner(StripClass &strip, int index)
{
    Strip = &strip;
    Index = index;
}


// int SidebarClass::StripClass::SelectClass::Action(unsigned flags, unsigned &key) (0x006809F0, slot 31)
// A click on a strip icon: a super weapon fires or enters targeting; a right
// click puts production on hold or cancels it; a left click places or
// releases a completed object, resumes held production or starts it.
// Unknown_0067F7C0 is the row's inlined progress read; its body is visible as
// the strip pointer held in ebp across the factory call.


int SidebarClass::StripClass::SelectClass::Action(unsigned flags, unsigned &key)
{
    if (Strip == 0) {
        return 1;
    }

    TechnoTypeClass *choice = 0;
    int spc = -1;

    int index = Strip->unknown_40 * 2 + Index;
    int kind = Strip->Buildables[index].unknown_04;
    int id = Strip->Buildables[index].unknown_00;
    int arg = Strip->Buildables[index].unknown_08;
    UnknownFactory *factory = Strip->Buildables[index].Factory;

    Map.Override_Mouse_Shape(0, false);

    if (index < Strip->BuildableCount) {
        if (kind != 31) {
            choice = (TechnoTypeClass *)Unknown_00482C00(kind, id);
        } else {
            spc = id;
        }
    }

    if (spc != -1) {
        if (flags & 8) {
            flags &= ~8;
        }
        if (flags & 0x10) {
            Map.unknown_11b4 = -1;
        }
        if (flags & 1) {
            if ((unsigned)spc < (unsigned)unknown_player_a35db4->Supers.Count()) {
                Unknown_Play_Sound(Rules->unknown_184);
                SuperClass *super = unknown_player_a35db4->Supers[spc];
                if (super->Unknown_0069D820()) {
                    if (unknown_player_a35db4->Supers[spc]->Type->Action == 0) {
                        Cell cell;
                        cell.X = 0;
                        cell.Y = 0;
                        OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, (unsigned char)0x12,
                            unknown_player_a35db4->Supers[spc]->Type->ArrayIndex, cell));
                    } else {
                        Map.unknown_11a0 = 0;
                        Map.unknown_11a4 = 0;
                        Map.unknown_11a8 = -1;
                        Map.Unknown_00498F10(0);
                        Map.Unknown_0049CA20(0);
                        Map.Unknown_0049C7C0(0);
                        Map.unknown_11ae = false;
                        Map.unknown_11af = false;
                        Map.unknown_11b0 = false;
                        Map.unknown_11b4 = spc;
                        Unselect_All();
                        Speech_Play_Name("EVA_SelectTarget", -1, -1);
                    }
                } else {
                    super->Unknown_0069D680();
                }
            }
        }
    } else if (choice != 0) {
        if (flags & 8) {
            flags &= ~8;
        }

        if (flags & 0x10) {
            if (factory != 0) {
                if (Map.unknown_11a0 && ((AbstractClass *)Map.unknown_11a0)->As_Techno()) {
                    Map.unknown_11a0 = 0;
                    Map.unknown_11a4 = 0;
                    Map.unknown_11a8 = -1;
                    Map.Unknown_00498F10(0);
                }
                if (!factory->Is_Building()) {
                    Unknown_Play_Sound(Rules->unknown_184);
                    Speech_Play_Name("EVA_Canceled", -1, -1);
                    OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0x10, kind, id, choice->Naval));
                } else {
                    Unknown_Play_Sound(Rules->unknown_184);
                    Speech_Play_Name("EVA_OnHold", -1, -1);
                    OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0xF, kind, id, choice->Naval));
                    Map.Column[Strip->unknown_34].unknown_38 = true;
                }
            } else {
                if (Strip->Buildables[index].unknown_10 == 1) {
                    Strip->Buildables[index].unknown_10 = 2;
                    Strip->Flag_To_Redraw();
                    Unknown_Play_Sound(Rules->unknown_184);
                    Speech_Play_Name("EVA_OnHold", -1, -1);
                    OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0xF, kind, id, choice->Naval));
                }
                FactoryClass *fptr = ((UnknownSidebarHouse *)unknown_player_a35db4)->Unknown_004EB580(kind, choice->Naval, arg);
                if (fptr != 0 && fptr->Has_Queued_004B9E80(choice)) {
                    Unknown_Play_Sound(Rules->unknown_184);
                    OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0x10, kind, id, choice->Naval));
                }
            }
        }

        if (flags & 1) {
            if (factory != 0 && (!factory->Is_Building())) {
                if (factory->Unknown_004B9900()) {
                    TechnoClass *pending = (TechnoClass *)factory->Get_Object();
                    if (!pending) {
                        if (factory->Unknown_004B9940() != -1) {
                            Map.unknown_11b4 = 1;
                        }
                    } else {
                        Unknown_Play_Sound(Rules->unknown_184);
                        BuildingClass *builder = pending->Who_Can_Build_Me(false, false);
                        if (!builder) {
                            OutList.Add(EventClass(pending->Owner(), 0x10, kind, id, choice->Naval));
                            Speech_Play_Name("EVA_UnableToComply", -1, -1);
                        } else if (pending->Kind_Of() == 6) {
                            ((UnknownSidebarHouse *)unknown_player_a35db4)->Unknown_004E7070((TechnoClass *)builder, (BuildingClass *)pending);
                        } else {
                            OutList.Add(EventClass(pending->Owner(), 0xB, kind, -1, choice->Naval, unknown_geometry_k));
                        }
                    }
                } else {
                    Unknown_Play_Sound(Rules->unknown_184);
                    if (kind == 16) {
                        Speech_Play_Name("EVA_Training", -1, -1);
                    } else {
                        Speech_Play_Name("EVA_Building", -1, -1);
                    }
                    TechnoTypeClass *type = (TechnoTypeClass *)Unknown_00482C00(Strip->Buildables[index].unknown_04, Strip->Buildables[index].unknown_00);
                    bool naval = type && type->Naval;
                    ((UnknownSidebarHouse *)unknown_player_a35db4)->Unknown_004EB620(kind, choice->Naval, arg);
                    OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0xE, Strip->Buildables[index].unknown_04,
                        Strip->Buildables[index].unknown_00, naval));
                    Strip->Buildables[index].unknown_10 = 1;
                    Strip->Buildables[index].Stage.Value = Strip->Unknown_0067F7C0(index);
                    int rate = factory->Unknown_004B9780();
                    Strip->Buildables[index].Stage.Set_Rate(rate + 1);
                    Strip->Flag_To_Redraw();
                    unknown_flag_8373c3 = true;
                }
            } else {
                factory = (UnknownFactory *)((UnknownSidebarHouse *)unknown_player_a35db4)->Unknown_004EB580(kind, choice->Naval, arg);
                TechnoTypeClass *type = (TechnoTypeClass *)Unknown_00482C00(Strip->Buildables[index].unknown_04, Strip->Buildables[index].unknown_00);
                bool naval = type && type->Naval;
                bool limited = ((UnknownSidebarHouse *)unknown_player_a35db4)->Unknown_004F4F30(type);
                bool busy;
                bool produce = false;
                if (factory != 0 && (factory->Is_Building() || ((FactoryClass *)factory)->Object != 0 || ((FactoryClass *)factory)->Queue.Count() > 0)) {
                    busy = true;
                    if (kind == 7) {
                        Speech_Play_Name("EVA_UnableToComply", -1, -1);
                    } else {
                        produce = true;
                    }
                } else {
                    busy = false;
                    if (kind == 16) {
                        if (!limited) {
                            Speech_Play_Name("EVA_Training", -1, -1);
                        }
                    } else {
                        if (!limited) {
                            Speech_Play_Name("EVA_Building", -1, -1);
                        }
                    }
                    produce = true;
                }
                if (produce) {
                    bool queued = ((UnknownSidebarHouse *)unknown_player_a35db4)->Unknown_004EB770(kind, choice->Naval, arg);
                    bool started = false;
                    if (!limited) {
                        Unknown_Play_Sound(Rules->unknown_184);
                        if (factory == 0 && !queued) {
                            ((UnknownSidebarHouse *)unknown_player_a35db4)->Unknown_004EB620(kind, choice->Naval, arg);
                            started = true;
                        }
                        OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 0xE, Strip->Buildables[index].unknown_04,
                            Strip->Buildables[index].unknown_00, naval));
                    }
                    if (started || !queued) {
                        if (!busy && !limited) {
                            Strip->Buildables[index].unknown_10 = 1;
                            Strip->Flag_To_Redraw();
                            TechnoTypeClass *tech = (TechnoTypeClass *)Unknown_00482C00(kind, Strip->Buildables[index].unknown_00);
                            if (tech) {
                                int ahead = unknown_max_ahead_a3d560;
                                if (((UnknownSidebarIHouse *)((char *)unknown_player_a35db4 + 0x24))->Available_Money() >= tech->Cost_Of(unknown_player_a35db4) * ahead / 54
                                    && tech->Who_Can_Build_Me(true, false, false, (HouseClass *)unknown_player_a35db4)) {
                                    int time = tech->unknown_slot34();
                                    if (tech->Kind_Of() == 7 && ((BuildingTypeClass *)tech)->Wall) {
                                        time = time * Rules->WallBuildSpeedCoefficient;
                                    }
                                    int least = ahead + 15;
                                    if (time <= least) {
                                        time = least;
                                    }
                                    int steps = time / 54;
                                    if (steps * 54 < least) {
                                        steps++;
                                    }
                                    if (steps < 1) {
                                        steps = 1;
                                    } else if (steps > 255) {
                                        steps = 255;
                                    }
                                    Strip->Buildables[index].Stage.Value = 0;
                                    Strip->Buildables[index].Stage.Set_Rate(steps);
                                }
                            }
                        }
                    }
                }
            }
        }
    } else {
        flags = 0;
    }

    ControlClass::Action(flags, key);
    return 1;
}


extern bool unknown_flag_abcd84;                        // 0x00ABCD84

void SidebarClass::StripClass::SelectClass::Mouse_Enter()
{
    ToolTips->Save_Delay();
    ToolTips->Set_Delay(0);
    unknown_34 = true;
    Strip->unknown_38 = true;
    Map.Flag_To_Redraw(false);
    Map.unknown_4ee3 = true;
    unknown_flag_abcd84 = true;
}


void SidebarClass::StripClass::SelectClass::Mouse_Leave()
{
    ToolTips->Restore_Delay();
    unknown_34 = false;
    Strip->unknown_38 = true;
    Map.Flag_To_Redraw(false);
    Map.unknown_4ee3 = true;
    unknown_flag_abcd84 = true;
}


// bool SidebarClass::StripClass::SelectClass::Unknown_00681690() (0x00681690)

bool SidebarClass::StripClass::SelectClass::Unknown_00681690()
{
    return unknown_34 && !unknown_flag_a40d44;
}


bool SidebarClass::StripClass::Unknown_006816D0(FactoryClass *factory, int type, int id)
{
    for (int index = 0; index < BuildableCount; index++) {
        if (Buildables[index].unknown_04 == type && Buildables[index].unknown_00 == id) {
            Buildables[index].Factory = (UnknownFactory *)factory;
            unknown_39 = true;
            unknown_38 = true;
            Map.Flag_To_Redraw(false);
            return true;
        }
    }
    return false;
}


// 0x006816b0: function not yet identified
//
// Owner: SidebarClass::SBGadgetClass, virtual slot 31 (vtable +0x7c) (from RTTI/vtables).
// Body: 22 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 FUN_006816b0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006816b0 {

struct Callee_0059f250 { unsigned int Override_Mouse_Shape_0059f250(int mouse_shape, char wsmall); };
extern "C" unsigned int Map_008324e0;

unsigned int __stdcall FUN_006816b0(int unused_0, int unused_1)
{
    ((Callee_0059f250*)(&Map_008324e0))->Override_Mouse_Shape_0059f250((int)(0), (char)(0));
    return 1;
}

}  // namespace seed_006816b0


// bool SidebarClass::StripClass::Unknown_00681730() (0x00681730)

bool SidebarClass::StripClass::Unknown_00681730()
{
    return BuildableCount > 0;
}


// bool SidebarClass::Unknown_00681740(int type, int id, UnknownFactory *factory) (0x00681740)
// Passes a finished factory to the strip of its buildable type.

bool SidebarClass::Unknown_00681740(int type, int id, UnknownFactory *factory)
{
    return Column[Which_Column(type, id)].Unknown_006817D0(factory);
}


// bool SidebarClass::StripClass::Unknown_006817D0(UnknownFactory *factory) (0x006817D0)
// Drops a factory from the strip's icons (a building's placement is
// forgotten first); true when an icon used it. The strip stops counting as
// busy (+0x39) when no icon has a factory left.


bool SidebarClass::StripClass::Unknown_006817D0(UnknownFactory *factory)
{
    if (factory && factory->Get_Object() && Is_Building(factory->Get_Object())) {
        Unknown_006F8A20(As_Building(factory->Get_Object()));
    }
    bool found = false;
    bool idle = true;
    for (int index = 0; index < BuildableCount; index++) {
        if (Buildables[index].Factory == factory) {
            factory->Unknown_004B97C0();
            Buildables[index].Factory = 0;
            found = true;
            Buildables[index].unknown_10 = 0;
        } else if (Buildables[index].Factory) {
            idle = false;
        }
    }
    if (found) {
        unknown_38 = true;
        Map.Flag_To_Redraw(false);
        unknown_flag_8373c3 = true;
    }
    if (idle) {
        unknown_39 = false;
    }
    return found;
}


class Leaf_6818b0 { public: unsigned int get(); };
unsigned int Leaf_6818b0::get() { unsigned int unused_this_address=(unsigned int)this, global_value=0, unused_value=0; unsigned char unused_byte=0; global_value = *(unsigned int*)(0xa3d298); if ((global_value & global_value) == 0) goto L6818c3; *(unsigned int*)(0xa40d48) = 8; L6818c3:; return global_value; }


// int __fastcall Unknown_006818D0(int type, int id, int arg3) (0x006818D0)
// A buildable type's sidebar column (2 for 0xF/0x10, 3 for 1/2/3/0x28, 1 for 0x1F/0x20/0x39; types 6 and 7 answer
// whether their kind is 5), or -1.
int __fastcall Unknown_006818D0(int type, int id, int arg3)
{
    if (type == 0xF || type == 0x10) {
        return 2;
    }
    if (type == 1 || type == 0x28 || type == 2 || type == 3) {
        return 3;
    }
    if (type == 6 || type == 7) {
        return Unknown_004EB550(type, id) == 5;
    }
    if (type == 0x39 || type == 0x20 || type == 0x1F) {
        return 1;
    }
    return -1;
}


int SidebarClass::Unknown_00681980()
{
    for (int index = 0; index < 4; index++) {
        if (Column[index].BuildableCount > 0) {
            return index;
        }
    }
    return -1;
}


// 0x00681940: function not yet identified
//
// Body: 53 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: uint __fastcall FUN_00681940(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00681940 {

unsigned int __fastcall FUN_00681940(int kind, int subtype, int unused_0)
{
    if (kind == 0x10) {
        return 2;
    }
    if ((kind != 0x28) && (kind != 3)) {
        if (kind == 7) {
            return (unsigned int)(subtype == 5);
        }
        return 0xffffffff;
    }
    return 3;
}

}  // namespace seed_00681940


// void SidebarClass::Reposition_Sidebar() (0x006819A0, slot 34)
// Lays the sidebar out again: after the power bar's layout it places the
// sidebar at the right of the tactical area (or at 0 when it is on the left),
// moves the button columns with it (the 18/26 command bar offsets), loads the
// three background shapes once, repositions every sidebar control and the
// strips' select buttons (each control's OffsetX keeps the sidebar's -X), and
// registers their help regions: the four tabs (0xCB..), the select buttons
// (1000..), sell (0x66), repair (0x65) and the two scroll buttons (0xC8, 0xC9).

// The rows of icons that fit, as the YR twin's file-static Max_Visible_Cameos: the
// strip top is read into a local before the 26/18 bottom piece, so VC6 subtracts the
// piece before it (out-of-line SidebarClass::Unknown_006820A0 keeps the other order).
static inline int Max_Visible_Cameos()
{
    int top = unknown_abcd64;
    int reserved;
    if (unknown_id_source_a3d290->unknown_2239) {
        reserved = 26;
    } else {
        reserved = 18;
    }
    return (unknown_int_839624 + unknown_int_83962c - top - reserved - 7) / 50;
}

void SidebarClass::Reposition_Sidebar()
{
    PowerClass::Reposition_Sidebar();

    if (unknown_flag_a40b30) {
        unknown_rect_839620.X = unknown_int_839638 + unknown_int_839630;
    } else {
        unknown_rect_839620.X = 0;
    }
    unknown_abcd60 = unknown_rect_839620.X + 0x16;
    if (unknown_id_source_a3d290->unknown_2239) {
        unknown_abcd48 = unknown_rect_839620.X + 0x14;
        unknown_abcd54 = unknown_rect_839620.X + 0x1A;
        unknown_abcd74 = unknown_rect_839620.X + 0x27;
    } else {
        unknown_abcd48 = unknown_rect_839620.X + 0x21;
        unknown_abcd54 = unknown_rect_839620.X + 0x14;
        unknown_abcd74 = unknown_rect_839620.X + 0x27;
    }

    if (unknown_shapes_abcd20[0] == 0) {
        unknown_shapes_abcd20[0] = MixFileClass::Retrieve("SIDEGDI1.SHP");
        unknown_shapes_abcd20[1] = MixFileClass::Retrieve("SIDEGDI2.SHP");
        unknown_shapes_abcd20[2] = MixFileClass::Retrieve("SIDEGDI3.SHP");
    }

    Background.Set_Position(unknown_rect_839620.X + 0x10, unknown_int_839634);
    Background.Flag_To_Redraw();
    int top = RadarButton.Y + RadarButton.Height;
    if (unknown_flag_a40b30) {
        Background.Set_Position(unknown_rect_839630.X + unknown_rect_839630.Width, top);
    } else {
        Background.Set_Position(0, top);
    }
    Background.Set_Size(unknown_surface_839948->Get_Width(), unknown_surface_839948->Get_Height() - top);

    unknown_button_abcc58.Set_Position(unknown_abcd48, unknown_abcd4c);
    unknown_button_abcc58.Flag_To_Redraw();
    unknown_button_abcc58.DrawOffsetX = -unknown_rect_839620.X;

    unknown_button_ab96b8.Set_Position(unknown_button_abcc58.X + unknown_abcd50, unknown_button_abcc58.Y);
    unknown_button_ab96b8.Flag_To_Redraw();
    unknown_button_ab96b8.DrawOffsetX = -unknown_rect_839620.X;

    for (int tab = 0; tab < 4; tab++) {
        unknown_buttons_ab9508[tab].Set_Position(unknown_abcd5c * tab + unknown_abcd54, unknown_abcd58);
        unknown_buttons_ab9508[tab].Flag_To_Redraw();
        unknown_buttons_ab9508[tab].DrawOffsetX = -unknown_rect_839620.X;
    }

    unknown_button_abcbe8.Set_Position(unknown_abcd74, unknown_abcd78);
    unknown_button_abcbe8.Flag_To_Redraw();
    unknown_button_abcbe8.DrawOffsetX = -unknown_rect_839620.X;

    unknown_button_abccc0.Set_Position(unknown_abcd74 + unknown_abcd7c, unknown_abcd78);
    unknown_button_abccc0.Flag_To_Redraw();
    unknown_button_abccc0.DrawOffsetX = -unknown_rect_839620.X;

    int rows = Max_Visible_Cameos();
    int count = rows * 2;
    for (int column = 0; column < 4; column++) {
        StripClass::SelectClass *button = &SelectButton[column * 60];
        for (int row = 0; row < rows; row++) {
            for (int side = 0; side < 2; side++, button++) {
                button->Set_Position(unknown_abcd68 * side + unknown_abcd60, unknown_abcd6c * row + unknown_abcd64 + 1);
                button->Flag_To_Redraw();
            }
        }
    }

    if (ToolTips != 0) {
        ToolTip tip;
        for (int tab = 0; tab < 4; tab++) {
            tip.ID = tab + 0xCB;
            tip.Label = 0;
            tip.Region.X = unknown_buttons_ab9508[tab].X;
            tip.Region.Y = unknown_buttons_ab9508[tab].Y;
            tip.Region.Width = unknown_buttons_ab9508[tab].Width;
            tip.Region.Height = unknown_buttons_ab9508[tab].Height;
            ToolTips->Remove(tip.ID);
            ToolTips->Add(tip);
        }
        for (int index = 0; index < count; index++) {
            tip.ID = index + 1000;
            tip.Label = 0;
            tip.Region.X = SelectButton[index].X;
            tip.Region.Y = SelectButton[index].Y;
            tip.Region.Width = SelectButton[index].Width;
            tip.Region.Height = SelectButton[index].Height;
            tip.unknown_18 = true;
            ToolTips->Remove(tip.ID);
            ToolTips->Add(tip);
        }
        tip.unknown_18 = false;
        tip.ID = 0x66;
        tip.Label = "TXT_SELL_MODE";
        tip.Region.X = unknown_button_ab96b8.X;
        tip.Region.Y = unknown_button_ab96b8.Y;
        tip.Region.Width = unknown_button_ab96b8.Width;
        tip.Region.Height = unknown_button_ab96b8.Height;
        ToolTips->Remove(0x66);
        ToolTips->Add(tip);
        tip.ID = 0x65;
        tip.Label = "TXT_REPAIR_MODE";
        tip.Region.X = unknown_button_abcc58.X;
        tip.Region.Y = unknown_button_abcc58.Y;
        tip.Region.Width = unknown_button_abcc58.Width;
        tip.Region.Height = unknown_button_abcc58.Height;
        ToolTips->Remove(0x65);
        ToolTips->Add(tip);
        tip.ID = 0xC8;
        tip.Label = 0;
        tip.Region.X = unknown_button_abccc0.X;
        tip.Region.Y = unknown_button_abccc0.Y;
        tip.Region.Width = unknown_button_abccc0.Width;
        tip.Region.Height = unknown_button_abccc0.Height;
        ToolTips->Remove(0xC8);
        ToolTips->Add(tip);
        tip.ID = 0xC9;
        tip.Label = 0;
        tip.Region.X = unknown_button_abcbe8.X;
        tip.Region.Y = unknown_button_abcbe8.Y;
        tip.Region.Width = unknown_button_abcbe8.Width;
        tip.Region.Height = unknown_button_abcbe8.Height;
        ToolTips->Remove(0xC9);
        ToolTips->Add(tip);
    }
}


// wchar_t const *SidebarClass::Help_Text(int id) (0x00681ED0, slot 33)
// Help text of a button: the power bar's and radar's first; the two scroll
// buttons (ids 200, 201) and the four tab buttons (ids 203-206) show their
// tip, marked "Disabled" when the button is off; ids 1000 and up are the
// build icons of the active strip.
extern wchar_t unknown_text_ab9274[];   // 0x00AB9274

wchar_t const *SidebarClass::Help_Text(int id)
{
    wchar_t const *text = PowerClass::Help_Text(id);
    if (text == 0) {
        bool disabled = false;
        if (id == 200) {
            text = StringTable::Load_String("Tip:ScrollUp", 0, "D:\\Ra2\\Sidebar.CPP", 4297);
            disabled = !unknown_button_abccc0.Is_Enabled();
        } else if (id == 201) {
            text = StringTable::Load_String("Tip:ScrollDown", 0, "D:\\Ra2\\Sidebar.CPP", 4303);
            disabled = !unknown_button_abcbe8.Is_Enabled();
        } else if (id >= 203 && id < 207) {
            int tab = id - 203;
            if (tab == 0) {
                text = StringTable::Load_String("Tip:Tab1", 0, "D:\\Ra2\\Sidebar.CPP", 4311);
            } else if (tab == 1) {
                text = StringTable::Load_String("Tip:Tab2", 0, "D:\\Ra2\\Sidebar.CPP", 4313);
            } else if (tab == 2) {
                text = StringTable::Load_String("Tip:Tab3", 0, "D:\\Ra2\\Sidebar.CPP", 4315);
            } else if (tab == 3) {
                text = StringTable::Load_String("Tip:Tab4", 0, "D:\\Ra2\\Sidebar.CPP", 4317);
            }
            disabled = !unknown_buttons_ab9508[tab].Is_Enabled();
        } else {
            int count = Unknown_006820F0();
            if (id >= 1000 && id < 1000 + count) {
                return Column[unknown_4ed8].Help_Text(id - 1000);
            }
        }
        if (disabled) {
            swprintf(unknown_text_ab9274, L"%s\n(%s)", text,
                     StringTable::Load_String("Tip:Disabled", 0, "D:\\Ra2\\Sidebar.CPP", 4336));
            return unknown_text_ab9274;
        }
    }
    return text;
}


// int SidebarClass::Unknown_006820A0() (0x006820A0)
// The rows of icons that fit in a strip: the sidebar height below the top
// pieces, less the bottom piece (26 lines when the scenario flag at +0x2239
// is set, else 18) and 7, in rows of 50.

int SidebarClass::Unknown_006820A0()
{
    int extra;
    if (unknown_id_source_a3d290->unknown_2239) {
        extra = 26;
    } else {
        extra = 18;
    }
    return (unknown_int_839624 + unknown_int_83962c - unknown_abcd64 - extra - 7) / 50;
}


// int SidebarClass::Unknown_006820F0() (0x006820F0)
// The icons that fit in a strip: two per row.

int SidebarClass::Unknown_006820F0()
{
    int const rows = Unknown_006820A0();
    int const count = rows + rows;
    return count;
}


// void __fastcall Unknown_00682140(wchar_t const *text, Point2D const &point, Rect const &window, int width) (0x00682140)
// With captions on (0x008373C0), prints text on the sidebar surface in the
// "DarkRed" scheme's base colour, clipped to window and wrapped to width,
// with its last line on the font's line at point.


void __fastcall Unknown_00682140(wchar_t const *text, Point2D const &point, Rect const &window, int width)
{
    if (unknown_flag_8373c0 && text != 0) {
        Point2D xy = point;
        BitFontClass *font = BitFont;
        ColorScheme *scheme = Fetch_Scheme_By_Name("DarkRed", 1);
        RGBClass color = scheme->BaseColor.To_RGB();
        unsigned pixel = ((color.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
            | ((color.Green >> unknown_shift_852fcc) << unknown_shift_852fc8)
            | ((color.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0);
        tagRECT box;
        box.left = window.X;
        box.top = window.Y;
        box.right = window.X + window.Width;
        box.bottom = window.Y + window.Height;
        font->Set_Clipping(1);
        font->Set_Clip_Box(&box);
        font->Set_Color(pixel);
        int text_width;
        int text_height;
        font->Get_Text_Area(text, &text_width, &text_height, width);
        xy.Y += font->Height - text_height;
        BitText->Print_Box(font, unknown_surface_839948, text, xy.X, xy.Y, width, 0, 0, 0, 0);
    }
}


class Target_682290 { public: unsigned int Load_00632bf0(unsigned int target_stream); };
class Leaf_682290 { public: unsigned int get(unsigned int stream_address); };
unsigned int Leaf_682290::get(unsigned int stream_address) { return ((Target_682290*)(this))->Load_00632bf0((unsigned int)(stream_address)); }


class Target_6822a0 { public: unsigned int Save_00632e10(unsigned int target_stream); };
class Leaf_6822a0 { public: unsigned int get(unsigned int stream_address); };
unsigned int Leaf_6822a0::get(unsigned int stream_address) { return ((Target_6822a0*)(this))->Save_00632e10((unsigned int)(stream_address)); }


// void SidebarClass::Unknown_006822B0(TechnoTypeClass *type, int delay) (0x006822B0)
// Finds a techno type's index in its type list and delays the matching
// icon of its strip (and the sidebar's next update) by `delay` frames.
class InfantryTypeClass;
class UnitTypeClass;
class AircraftTypeClass;
class BuildingTypeClass;
extern DynamicVectorClass<InfantryTypeClass *> InfantryTypes;   // 0x00A40350
extern DynamicVectorClass<UnitTypeClass *> UnitTypes;           // 0x00A35D48
extern DynamicVectorClass<AircraftTypeClass *> AircraftTypes;   // 0x00A3D278
extern DynamicVectorClass<BuildingTypeClass *> BuildingTypes;   // 0x00A35CD8

void SidebarClass::Unknown_006822B0(TechnoTypeClass *type, int delay)
{
    int index;
    for (index = 0; index < InfantryTypes.Count(); index++) {
        if ((TechnoTypeClass *)InfantryTypes[index] == type) {
            goto found;
        }
    }
    for (index = 0; index < UnitTypes.Count(); index++) {
        if (type == (TechnoTypeClass *)UnitTypes[index]) {
            goto found;
        }
    }
    for (index = 0; index < AircraftTypes.Count(); index++) {
        if (type == (TechnoTypeClass *)AircraftTypes[index]) {
            goto found;
        }
    }
    for (index = 0; index < BuildingTypes.Count(); index++) {
        if (type == (TechnoTypeClass *)BuildingTypes[index]) {
            goto found;
        }
    }
    return;

found:
    int kind = type->Kind_Of();
    int column;
    if (kind == 15 || kind == 16) {
        column = 2;
    } else if (kind == 1 || kind == 40 || kind == 2 || kind == 3) {
        column = 3;
    } else if (kind == 6 || kind == 7) {
        column = (Unknown_004EB550(kind, index) == 5);
    } else if (kind == 57 || kind == 32 || kind == 31) {
        column = 1;
    } else {
        return;
    }
    Column[column].Unknown_00682400(index, delay);
    unknown_4edc = Frame + 1 + delay;
}


void SidebarClass::StripClass::Unknown_00682400(int id, int delay)
{
    for (int index = 0; index < 75; index++) {
        if (Buildables[index].unknown_00 == id) {
            Buildables[index].unknown_2c = Frame + delay;
            return;
        }
    }
}


// SidebarClass::StripClass::SelectClass scalar deleting destructor (0x00682440)
// Compiler generated with SelectButton's vtable: GadgetClass's destructor, then operator delete.


// SidebarClass::SBGadgetClass scalar deleting destructor (0x00682460)
// Compiler generated with Background's vtable: GadgetClass's destructor, then operator delete.


// UnknownBuildable30 default constructor (0x00682480), emitted for the strips' array construction.


// SidebarClass scalar deleting destructor (0x006824A0), emitted by the implicit virtual destructor.


class Leaf_682520 { public: unsigned int get(); };
unsigned int Leaf_682520::get() { unsigned int initialization_value = (unsigned int)this; unsigned int object_address; unsigned int current_frame; object_address = initialization_value; initialization_value = 0; *(unsigned int*)(object_address) = initialization_value; current_frame = *(unsigned int*)(0xa40d2c); *(unsigned int*)(object_address + 4) = current_frame; *(unsigned int*)(object_address + 0xc) = initialization_value; *(unsigned int*)(object_address + 0x10) = initialization_value; *(unsigned int*)(object_address + 0x14) = 1; return object_address; }
