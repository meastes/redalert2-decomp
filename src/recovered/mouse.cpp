// mouse.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mouse.json carries the addresses and the rows that
// stay in their own files.


#include "shared/mouse_class.hpp"
#include "shared/mouse.hpp"
#include "shared/map_cell.hpp"
#include "shared/anim_type.hpp"
#include "shared/building_type.hpp"
#include "shared/overlay_type.hpp"
#include "shared/smudge_type.hpp"
#include "shared/terrain_type.hpp"
#include "shared/map_view.hpp"
#include "shared/tab.hpp"


// Dynamic initializer of MouseClass::Timer (0x0059EFD0, _$E) and its guarded
// atexit destructor (0x0059F000; guard byte 0x00A70F18), emitted by this
// definition (the timer's destructor is empty).

CDTimerClass<SystemTimerClass> MouseClass::Timer;


// Static-member initializer or guarded atexit destructor (0x0059F000, _$E) emitted by the definition in MouseClass_Timer_init_0059efd0.cpp; no source of its own.


// MouseClass::MouseClass() (0x0059F020)
// Constructs the scroll layer and clears the mouse state.

MouseClass::MouseClass() : IsSmall(false), CurrentMouseShape(0), NormalMouseShape(0), Frame(0)
{
}


// void MouseClass::Set_Default_Mouse(int mouse, bool wsmall) (0x0059F060, slot 18)
// Remembers mouse as the normal shape and shows it.

void MouseClass::Set_Default_Mouse(int mouse, bool wsmall)
{
    NormalMouseShape = mouse;
    Override_Mouse_Shape(mouse, wsmall);
}


// void MouseClass::Revert_Mouse_Shape() (0x0059F080, slot 20)
// Shows the normal shape again (large form).

void MouseClass::Revert_Mouse_Shape()
{
    Override_Mouse_Shape(NormalMouseShape, false);
}


// void MouseClass::Mouse_Small(bool wsmall) (0x0059F090, slot 21)
// Switches between the small and large forms of the current shape and sets
// the cursor again (TS Mouse_Small).

void MouseClass::Mouse_Small(bool wsmall)
{
    if (IsSmall == wsmall) {
        return;
    }
    IsSmall = wsmall;
    int start = Get_Mouse_Start_Frame(CurrentMouseShape, wsmall);
    WWMouse->Set_Cursor(Get_Mouse_Hotspot(CurrentMouseShape), (ShapeFileStruct const *)MouseShapes, start + Frame);
}


// int MouseClass::Get_Mouse_Start_Frame(int mouse, bool wsmall) const (0x0059F170)
// The small form's first frame when asked and present, else the normal one.

int MouseClass::Get_Mouse_Start_Frame(int mouse, bool wsmall) const
{
    MouseStruct const &control = unknown_mouse_control_7e2af8[mouse];
    if (wsmall && control.SmallFrame != -1) {
        return control.SmallFrame;
    }
    return control.StartFrame;
}


// int MouseClass::Get_Mouse_Frame(int mouse, bool wsmall) const (0x0059F1A0)
// The current animation frame of mouse.

int MouseClass::Get_Mouse_Frame(int mouse, bool wsmall) const
{
    return Frame + Get_Mouse_Start_Frame(mouse, wsmall);
}


// Point2D MouseClass::Get_Mouse_Hotspot(int mouse) const (0x0059F1D0)
// The centre or far edge of the shape file where the table asks for it,
// else 0.

Point2D MouseClass::Get_Mouse_Hotspot(int mouse) const
{
    MouseShapeHeader const *shapes = MouseShapes;
    MouseStruct const &control = unknown_mouse_control_7e2af8[mouse];
    int x = 0;
    int y = 0;
    if (control.X == 12345) {
        x = shapes->Width / 2;
    }
    if (control.X == 54321) {
        x = shapes->Width;
    }
    if (control.Y == 12345) {
        y = shapes->Height / 2;
    }
    if (control.Y == 54321) {
        y = shapes->Height;
    }
    return Point2D(x, y);
}


// bool MouseClass::Override_Mouse_Shape(int mouse, bool wsmall) (0x0059F250, slot 19)
// Shows another mouse shape (TS Override_Mouse_Shape): the small form only
// when the shape has one; nothing happens after the first call unless the
// shapes are loaded and the shape or form changes. Restarts the animation.

bool MouseClass::Override_Mouse_Shape(int mouse, bool wsmall)
{
    MouseStruct const *control = &unknown_mouse_control_7e2af8[mouse];
    static bool startup = false;
    if (control->SmallFrame == -1) {
        wsmall = false;
    }
    if (startup && (MouseShapes == 0 || (mouse == CurrentMouseShape && wsmall == IsSmall))) {
        return false;
    }
    startup = true;
    Timer = control->FrameRate;
    Frame = 0;
    int start = Get_Mouse_Start_Frame(mouse, wsmall);
    WWMouse->Set_Cursor(Get_Mouse_Hotspot(mouse), (ShapeFileStruct const *)MouseShapes, start);
    CurrentMouseShape = mouse;
    IsSmall = wsmall;
    return true;
}


// void MouseClass::AI(int &input, Point2D const &xy) (0x0059F390, slot 10)
// Animates the current mouse shape (TS MouseClass::AI): when the shape has
// a rate and the timer ran out, steps the frame, restarts the timer and sets
// the cursor again; then the scroll layer's AI.

void MouseClass::AI(int &input, Point2D const &xy)
{
    MouseStruct const *control = &unknown_mouse_control_7e2af8[CurrentMouseShape];
    if (control->FrameRate != 0 && Timer.Value() == 0) {
        Frame++;
        Frame %= control->FrameCount;
        Timer = control->FrameRate;
        int start = Get_Mouse_Start_Frame(CurrentMouseShape, IsSmall);
        WWMouse->Set_Cursor(Get_Mouse_Hotspot(CurrentMouseShape), (ShapeFileStruct const *)MouseShapes, start + Frame);
    }
    ScrollClass::AI(input, xy);
}


// void MouseClass::One_Time() (0x0059F4F0, slot 5)
// After the scroll layer's, loads the mouse shapes "MOUSE.SHP".

void MouseClass::One_Time()
{
    ScrollClass::One_Time();
    MouseShapes = (MouseShapeHeader const *)Unknown_Load_Shape_00495270("MOUSE.SHP", MouseShapesAllocated);
}


// void MouseClass::Init_Clear() (0x0059F510, slot 7)
// After the sidebar's, back to the large default shape.

void MouseClass::Init_Clear()
{
    ScrollClass::Init_Clear();
    IsSmall = false;
    NormalMouseShape = 0;
}


// HRESULT MouseClass::Load(IStream *stream) (0x0059F530, slot 30)
// Loads the whole screen object: the layers' own loads, then the raw object
// image (0x4F3C bytes), rebuilt in place with the load constructor, its
// pointers swizzled and the map buffers and hash tables rebuilt, followed
// by the saved vectors; the theater types are reinitialized last.

// The zone cell table: 256 buckets, growth step 20 (as MapClass::One_Time builds it).
static HashTableClass<unsigned int, unsigned int> *New_Cell_Table()
{
    return new HashTableClass<unsigned int, unsigned int>(20, 256, Unknown_Hash_0054F240);
}

HRESULT MouseClass::Load(IStream *stream)
{
    unknown_1158.Clear();
    unknown_d0.Clear();
    HRESULT result = SidebarClass::Load(stream);
    if (result < 0) {
        return result;
    }
    int unused;
    result = stream->Read(&unused, 4, 0);
    if (result < 0) {
        return result;
    }
    unknown_loaded_theater_7d88dc = -1;
    unknown_slot23();
    delete CellSubzones;
    CellSubzones = 0;
    delete CellZones;
    CellZones = 0;
    delete unknown_14;
    unknown_14 = 0;
    for (int clear = 0; clear < 3; clear++) {
        SubzoneTracking[clear].Clear();
    }
    for (int free_buffer = 0; free_buffer < 12; free_buffer++) {
        delete unknown_18[free_buffer];
        unknown_18[free_buffer] = 0;
    }
    for (int free_table = 0; free_table < 3; free_table++) {
        if (unknown_7c[free_table] != 0) {
            delete unknown_7c[free_table];
            unknown_7c[free_table] = 0;
        }
    }
    Cells.Clear();
    result = stream->Read(this, 0x4F3C, 0);
    if (result < 0) {
        return result;
    }
    new (this) MouseClass(NoInitClass());
    for (int strip = 0; strip < 4; strip++) {
        UnknownBuildable30 *buildable = Column[strip].Buildables;
        for (int item = 0; item < 75; item++) {
            Swizzle_Pointer(buildable[item].Factory);
        }
    }
    Swizzle_Pointer(unknown_11a0);
    Swizzle_Pointer(unknown_11a4);
    Swizzle_Pointer(unknown_119c);
    unknown_slot22();
    unknown_slot24();
    CellSubzones = 0;
    CellZones = 0;
    Set_Map_Dimensions(&MapSize, true, false, false);
    if (CellSubzones != 0) {
        delete CellSubzones;
        CellSubzones = 0;
    }
    if (CellZones != 0) {
        delete CellZones;
        CellZones = 0;
    }
    CellSubzones = new char[CellZoneCount * 10];
    CellZones = new UnknownMapElem4[CellZoneCount];
    unknown_14 = New_Cell_Table();
    for (int tier = 0; tier < 3; tier++) {
        int factor = 1 << (tier + 1);
        SubzoneTracking[tier].Clear();
        int cells = MapSize.Height * MapSize.Width * 4;
        SubzoneTracking[tier].Set_Growth_Step(cells / (factor * factor));
        unknown_7c[tier] = new HashTableClass<unsigned int, SubzoneConnectionStruct>(20, 256, Unknown_Hash_0054F240);
    }
    result = stream->Read(CellZones, CellZoneCount * 4, 0);
    if (result < 0) {
        return result;
    }
    for (int buffer = 0; buffer < 12; buffer++) {
        unknown_18[buffer] = new char[unknown_48 * 2];
        result = stream->Read(unknown_18[buffer], unknown_48 * 2, 0);
        if (result < 0) {
            return result;
        }
    }
    int zones = 0;
    result = stream->Read(&zones, 4, 0);
    if (result < 0) {
        return result;
    }
    ZoneConnections.Clear();
    for (int zone = 0; zone < zones; zone++) {
        ZoneConnectionClass connection;
        result = stream->Read(&connection, sizeof(connection), 0);
        if (result < 0) {
            return result;
        }
        ZoneConnections.Add(connection);
    }
    for (int cell = 0; cell < Cells.Length(); cell++) {
        if (Cells[cell] != 0) {
            delete Cells[cell];
            Cells[cell] = 0;
        }
    }
    int count;
    result = stream->Read(&count, 4, 0);
    if (result < 0) {
        return result;
    }
    for (int object = 0; object < count; object++) {
        void *loaded;
        OleLoadFromStream(stream, IID_IUnknown_7afce0, &loaded);
    }
    result = stream->Read(&count, 4, 0);
    if (result < 0) {
        return result;
    }
    for (int entry = 0; entry < count; entry++) {
        Cell cell;
        result = stream->Read(&cell, 4, 0);
        if (result < 0) {
            return result;
        }
        unknown_1158.Add(cell);
    }
    result = stream->Read(&count, 4, 0);
    if (result < 0) {
        return result;
    }
    for (int entry2 = 0; entry2 < count; entry2++) {
        Cell cell;
        result = stream->Read(&cell, 4, 0);
        if (result < 0) {
            return result;
        }
        unknown_d0.Add(cell);
    }
    TerrainTypeClass::Init(unknown_id_source_a3d290->Theater);
    if (unknown_id_source_a3d290->Theater != unknown_loaded_theater_7d88dc) {
        Unknown_00529870(unknown_id_source_a3d290->Theater, true);
    } else {
        Unknown_0052B650();
    }
    Unknown_0052B2E0(false, false);
    OverlayTypeClass::Init(unknown_id_source_a3d290->Theater);
    BuildingTypeClass::Init(unknown_id_source_a3d290->Theater);
    AnimTypeClass::Init(unknown_id_source_a3d290->Theater);
    SmudgeTypeClass::Init(unknown_id_source_a3d290->Theater);
    unknown_11b8 = 0;
    unknown_loaded_theater_7d88dc = unknown_id_source_a3d290->Theater;
    return 0;
}


// HRESULT MouseClass::Save(IStream *stream) (0x0059FC80, slot 31)
// Saves the whole screen object: the layers' own saves, the theater, the
// raw object image (0x4F3C bytes), the map buffers, the saved vectors and
// the count cells.

HRESULT MouseClass::Save(IStream *stream)
{
    HRESULT result = SidebarClass::Save(stream);
    if (result < 0) {
        return result;
    }
    int value = unknown_id_source_a3d290->Theater;
    result = stream->Write(&value, 4, 0);
    if (result < 0) {
        return result;
    }
    result = stream->Write(this, 0x4F3C, 0);
    if (result < 0) {
        return result;
    }
    result = stream->Write(CellZones, CellZoneCount * 4, 0);
    if (result < 0) {
        return result;
    }
    for (int buffer = 0; buffer < 12; buffer++) {
        result = stream->Write(unknown_18[buffer], unknown_48 * 2, 0);
        if (result < 0) {
            return result;
        }
    }
    int zones = ZoneConnections.Count();
    result = stream->Write(&zones, 4, 0);
    if (result < 0) {
        return result;
    }
    for (int zone = 0; zone < zones; zone++) {
        ZoneConnectionClass connection = ZoneConnections[zone];
        result = stream->Write(&connection, sizeof(connection), 0);
        if (result < 0) {
            return result;
        }
    }
    int count = 0;
    Reset_Iterator();
    for (CellClass *cell = Iterate(); cell != 0; cell = Iterate()) {
        Cell where = cell->MapCoords;
        if (Has_Cell(where)) {
            count++;
        }
    }
    result = stream->Write(&count, 4, 0);
    if (result < 0) {
        return result;
    }
    Reset_Iterator();
    for (CellClass *cell2 = Iterate(); cell2 != 0; cell2 = Iterate()) {
        Cell where2 = cell2->MapCoords;
        if (Has_Cell(where2)) {
            OleSaveToStream((IPersistStream *)cell2, stream);
            count--;
        }
    }
    if (count != 0) {
        return result;
    }
    count = unknown_1158.Count();
    result = stream->Write(&count, 4, 0);
    if (result < 0) {
        return result;
    }
    for (int entry = 0; entry < count; entry++) {
        result = stream->Write(&unknown_1158[entry], 4, 0);
        if (result < 0) {
            return result;
        }
    }
    count = unknown_d0.Count();
    result = stream->Write(&count, 4, 0);
    if (result < 0) {
        return result;
    }
    for (int entry2 = 0; entry2 < count; entry2++) {
        result = stream->Write(&unknown_d0[entry2], 4, 0);
        if (result < 0) {
            return result;
        }
    }
    return 0;
}


// void MouseClass::Unknown_0059FEF0() (0x0059FEF0, static)
// Frees the mouse shapes when they were loaded rather than retrieved.

void MouseClass::Unknown_0059FEF0()
{
    if (MouseShapesAllocated) {
        operator delete((void *)MouseShapes);
        MouseShapesAllocated = false;
    }
    MouseShapes = 0;
}


// int MouseClass::Get_Mouse_Start(int mouse) const (0x0059FF20)
// First frame of mouse in the table. No direct callers (inlined elsewhere).

int MouseClass::Get_Mouse_Start(int mouse) const
{
    return unknown_mouse_control_7e2af8[mouse].StartFrame;
}


// int MouseClass::Get_Mouse_Count(int mouse) const (0x0059FF40)
// Frame count of mouse in the table. No direct callers (inlined elsewhere).

int MouseClass::Get_Mouse_Count(int mouse) const
{
    return unknown_mouse_control_7e2af8[mouse].FrameCount;
}


// TabClass::TabClass(NoInitClass const &) (0x0059FF60)
// The restore-in-place constructor (SidebarClass's, then the vtables), kept
// out of line in Mouse.cpp where MouseClass::Load calls it.


// TabClass scalar deleting destructor (0x0059FF90, slot 4)
// Compiler generated with the TabClass vtable emitted by the constructor
// above: the sidebar strips' buildable arrays (four columns of 75), then
// RadarClass's destructor, then operator delete when bit 0 of the flags is set.
