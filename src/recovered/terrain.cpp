// terrain.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/terrain.json carries the addresses and the rows that
// stay in their own files.

// TerrainClass Render (0x006E4430) expands TibSun's Rect + Top_Left() inline (rect.hpp RA2_RECT_POINT_INLINE).
#define RA2_RECT_POINT_INLINE
// Header-static dynamic initializers of the unit at 0x006E2E10:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006E2E10-0x006E2FF6); the other
// drafts of this plan are emitted by these includes.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"


// Dynamic initializer (0x006E2E40, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006e2e10.cpp; no source of its own.


// Dynamic initializer (0x006E2E60, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006e2e10.cpp; no source of its own.


// Dynamic initializer (0x006E2E80, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006e2e10.cpp; no source of its own.


// Dynamic initializer (0x006E2EA0, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006e2e10.cpp; no source of its own.


// Dynamic initializer (0x006E2EC0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006e2e10.cpp; no source of its own.


// Dynamic initializer (0x006E2EE0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006e2e10.cpp; no source of its own.


// Dynamic initializer (0x006E2F20, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006e2e10.cpp; no source of its own.


// Dynamic initializer (0x006E2F50, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006e2e10.cpp; no source of its own.


// Dynamic initializer (0x006E2F80, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006e2e10.cpp; no source of its own.


// Dynamic initializer (0x006E2FB0, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006e2e10.cpp; no source of its own.


// Dynamic initializer (0x006E2FC0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006e2e10.cpp; no source of its own.


// Dynamic initializer (0x006E2FE0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006e2e10.cpp; no source of its own.




#include "shared/terrain.hpp"
#include "shared/terrain_object_index.hpp"
#include "shared/tactical.hpp"
#include "shared/anim.hpp"
#include "shared/rules.hpp"
#include "shared/warhead_type.hpp"
#include "shared/unknown_static_rect.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/palette.hpp"
#include "shared/map_view.hpp"
#include "shared/random.hpp"
#include "shared/crt_memory.hpp"
#include "shared/shape_file.hpp"
#include "shared/unknown_globals.hpp"


// TerrainClass::~TerrainClass() (0x006E3000)
// Leaves the notice lists, the terrain list and the object index; when the
// 0x00A40958 flag is set a typed object first leaves the map (slot 52).

TerrainClass::~TerrainClass()
{
    Unknown_006ECB90(this, true);
    Terrains.Delete(this);
    if (unknown_flag_a40958 && Class != 0) {
        unknown_7d = true;
        TerrainClass::Limbo();
    }
    unknown_object_index_abff58.Remove_Index(AbstractClass::Fetch_ID());
}


// int TerrainClass::Take_Damage(int *damage, int distance, WarheadTypeClass *warhead, TechnoClass *source, bool forced, int arg6) (0x006E3170, slot 86)
// Takes damage (hypothesis: Take_Damage). Only wood-affecting warheads hurt
// terrain whose type is not immune (+0x203). When the object is destroyed
// (result 4) a tiberium spawner explodes (the C4 warhead's animation and
// damage around it), a burning one (+0xB4) stops its attached animations and
// anything else starts crumbling; then its area is redrawn and it leaves the
// map (slots 54 and 60).

// The object's coordinate as a value (local helper; each use is its own copy).
static inline CoordStruct Get_Coord(ObjectClass const *object)
{
    return object->Coord;
}

// The land type of the cell at a coordinate (local helper).
static inline int Land_Type_At(CoordStruct coord)
{
    return Map[coord].unknown_ec;
}

int TerrainClass::Take_Damage(int *damage, int distance, WarheadTypeClass *warhead, TechnoClass *source, bool forced, int arg6)
{
    int result = 0;
    if (warhead == 0) {
        return 0;
    }
    if (warhead->Wood && !Class->unknown_203) {
        result = ObjectClass::Take_Damage(damage, distance, warhead, source, forced, arg6);
        if (result == 5) {
            return result;
        }
        if (result == 4) {
            if (Class->SpawnsTiberium) {
                new AnimClass(Select_Damage_Anim(100, Rules->unknown_c74, Land_Type_At(Get_Coord(this)), &Get_Coord(this)), Get_Coord(this), 0, 1, 0x2600, Get_Explosion_Z_0047fcc0(Get_Coord(this)));
                Unknown_0047E2C0(Get_Coord(this), 100, 0, Rules->unknown_c74, 1);
                unknown_slot104().unknown_47e2b0();
            } else if (unknown_b4) {
                Unknown_00420D30(this);
            } else {
                Start_Crumbling();
            }
            unknown_tactical_83996c->Register_Dirty_Area(Get_Render_Rect(), 0);
            unknown_slot54(true);
            Delete_Me();
        }
    }
    return result;
}


// TerrainClass::TerrainClass(TerrainTypeClass *type, Cell const &cell) (0x006E33D0)
// Takes the type's strength, places the object at the centre of the cell
// (unless it is the zero cell; removed again when placement fails), resets
// the animation timer and registers the object in the terrain list and the
// object index.

TerrainClass::TerrainClass(TerrainTypeClass *type, Cell const &cell) :
    Class(type),
    unknown_b4(false),
    unknown_b5(false),
    unknown_b8(0, 0, 0, 0)
{
    AbstractClass::Create_ID();
    Strength = Class->MaxStrength;
    if (cell.X != unknown_geometry_k.X || cell.Y != unknown_geometry_k.Y) {
        CoordStruct coord(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
        if (!TerrainClass::Unlimbo(&coord, 0)) {
            ObjectClass::Delete_Me();
        }
    }
    Animation.Set_Rate(0);
    Terrains.Add(this);
    unknown_object_index_abff58.Add_Index(AbstractClass::Fetch_ID(), this);
}


// TerrainClass::TerrainClass() (0x006E3620)
// An untyped terrain object, registered in the terrain list and the object
// index.

TerrainClass::TerrainClass() :
    Class(0),
    unknown_b4(false),
    unknown_b5(false),
    unknown_b8(0, 0, 0, 0)
{
    Terrains.Add(this);
    unknown_object_index_abff58.Add_Index(AbstractClass::Fetch_ID(), this);
}


// bool TerrainClass::Mark(int arg1) (0x006E37E0)
// After the base handling succeeds, takes the object off its cell (mode 0)
// or puts it on the cell (modes 1 and 3).

bool TerrainClass::Mark(int arg1)
{
    if (ObjectClass::Mark(arg1)) {
        Cell cell = unknown_slot104();
        switch (arg1) {
        case 0:
            Map.Pick_Up(cell, this);
            break;
        case 1:
        case 3:
            Map.Place_Down(cell, this);
            return true;
        }
        return true;
    }
    return false;
}


// void TerrainClass::Clear_Occupy_Bit(CoordStruct const &coord) (0x006E3850)
// Clears the cell's terrain occupation bits named by the type's
// occupation bits for the theater (snow: SnowOccupationBits).

void TerrainClass::Clear_Occupy_Bit(CoordStruct const &coord)
{
    int bits;
    if (unknown_id_source_a3d290->Theater != 1) {
        bits = Class->TemperateOccupationBits;
    } else {
        bits = Class->SnowOccupationBits;
    }
    CellClass &cell = Map_Cell(Coord_Cell(coord));
    if (bits & 1) {
        cell.unknown_124 &= ~0x04;
    }
    if (bits & 2) {
        cell.unknown_124 &= ~0x08;
    }
    if (bits & 4) {
        cell.unknown_124 &= ~0x10;
    }
}


// void TerrainClass::Set_Occupy_Bit(CoordStruct const &coord) (0x006E38F0)
// Sets the cell's terrain occupation bits named by the type's
// occupation bits for the theater (snow: SnowOccupationBits).

void TerrainClass::Set_Occupy_Bit(CoordStruct const &coord)
{
    int bits;
    if (unknown_id_source_a3d290->Theater != 1) {
        bits = Class->TemperateOccupationBits;
    } else {
        bits = Class->SnowOccupationBits;
    }
    CellClass &cell = Map_Cell(Coord_Cell(coord));
    if (bits & 1) {
        cell.unknown_124 |= 0x04;
    }
    if (bits & 2) {
        cell.unknown_124 |= 0x08;
    }
    if (bits & 4) {
        cell.unknown_124 |= 0x10;
    }
}


// void TerrainClass::unknown_slot67(Point2D const &point, Rect const &window) (0x006E3990, slot 67)
// Draws the terrain object (hypothesis: Draw_It) at the screen point point
// within the window window: the frame follows the animation stage (crumbling
// objects offset by their +0xB5 flag; a damaged static object shows frame
// 1), lifted by its height. Tiberium spawners draw with the 0x008323BC
// drawer 16 pixels higher; the shadow (second half of the frames) follows
// when shadows are on (0x007D88D5).


// The object is animated or crumbling (local helper; slot 67's shadow flags).
static inline bool Is_Animating(TerrainClass const *terrain)
{
    return terrain->Class->IsAnimated || terrain->unknown_b5;
}

void TerrainClass::unknown_slot67(Point2D const &point, Rect const &window)
{
    Cell where = unknown_slot104();
    CellClass &cell = Map_Cell(where);
    ShapeFileStruct const *shape = (ShapeFileStruct const *)Get_Image_Data();
    if (shape != 0) {
        int frame = 0;
        if (Class->IsAnimated) {
            frame = Animation.Value;
        } else if (unknown_b5) {
            frame = Animation.Value + unknown_b5;
        } else if (Strength < 2) {
            frame = 1;
        }
        Point2D position = point;
        int height = -Unknown_006A2970(Get_Height());
        if (cell.unknown_34 == 0) {
            cell.Init_Drawer(0, 0x10000, 0, 1000, 1000, 1000);
        }
        ConvertClass *drawer;
        int brightness;
        if (Class->SpawnsTiberium) {
            drawer = unknown_drawer_8323bc;
            brightness = cell.unknown_10a;
            position -= Point2D(0, 16);
        } else {
            drawer = (ConvertClass *)cell.unknown_34;
            brightness = cell.unknown_10c;
        }
        int flags;
        if (Is_Animating(this)) {
            flags = 0x2E00;
        } else {
            flags = 0x4E00;
        }
        CC_Draw_Shape(unknown_surface_83995c, drawer, shape, frame, position, window, flags, 0, height - 12, 2, brightness, 0, 0, Point2D(0, 0));
        if (unknown_flag_7d88d5) {
            CC_Draw_Shape(unknown_surface_83995c, (ConvertClass *)cell.unknown_34, shape, shape->FrameCount / 2 + frame, position, window, flags | 1, 0, height - 3, 0, 1000, 0, 0, Point2D(0, 0));
        }
    }
}


// void TerrainClass::Editor_Draw_It(Point2D const &point, Rect const &window) (0x006E3B40, slot 68)
// The second drawing pass (as slot 67): frame from the stage when crumbling,
// flags 0xE00 (shadow 0x601), lifted 4 (shadow 2) pixels less.

void TerrainClass::Editor_Draw_It(Point2D const &point, Rect const &window)
{
    Cell where = unknown_slot104();
    CellClass &cell = Map_Cell(where);
    ShapeFileStruct const *shape = (ShapeFileStruct const *)Get_Image_Data();
    if (shape != 0) {
        int frame = 0;
        if (unknown_b5) {
            frame = Animation.Value + unknown_b5;
        }
        Point2D position = point;
        int height = -Unknown_006A2970(Get_Height());
        if (cell.unknown_34 == 0) {
            cell.Init_Drawer(0, 0x10000, 0, 1000, 1000, 1000);
        }
        ConvertClass *drawer;
        int brightness;
        if (Class->SpawnsTiberium) {
            drawer = unknown_drawer_8323bc;
            brightness = cell.unknown_10a;
            position -= Point2D(0, 16);
        } else {
            drawer = (ConvertClass *)cell.unknown_34;
            brightness = cell.unknown_10c;
        }
        CC_Draw_Shape(unknown_surface_83995c, drawer, shape, frame, position, window, 0xE00, 0, height - 4, 2, brightness, 0, 0, Point2D(0, 0));
        if (unknown_flag_7d88d5) {
            CC_Draw_Shape(unknown_surface_83995c, (ConvertClass *)cell.unknown_34, shape, shape->FrameCount / 2 + frame, position, window, 0x601, 0, height - 2, 0, 1000, 0, 0, Point2D(0, 0));
        }
    }
}


// int TerrainClass::Can_Enter_Cell(...) (0x006E3CB0; ret 0x14)
// Whether the terrain object may stand in a cell: 0 when every cell of its
// occupation list (offsets from the cell, ended by 0x7FFF, 0x7FFF) accepts
// it, 7 when one does not (hypothesis: Can_Enter_Cell's MOVE_OK / MOVE_NO).
// Water-bound types ask with speed type 5, the others with 1. arg1 is the
// cell (its coordinates at +0x24); the other arguments are unused.
struct UnknownTerrainCellArg {
    char unknown_00[0x24];
    Cell MapCoords;                   // +0x24
};

static inline bool Is_List_End(Cell const *offset)
{
    return offset->X == 0x7FFF && offset->Y == 0x7FFF;
}

int TerrainClass::Can_Enter_Cell(CellClass *arg1, int arg2, int arg3, CellClass *arg4, bool arg5)
{
    Cell const *offset = Occupy_List(0);
    Cell cell = ((UnknownTerrainCellArg *)arg1)->MapCoords;
    while (!Is_List_End(offset)) {
        if (Class->WaterBound) {
            if (!((UnknownPlacementCellView &)Map[*offset++ + cell]).Unknown_004718B0(5, 0, 0)) {
                return 7;
            }
        } else {
            if (!((UnknownPlacementCellView &)Map[*offset++ + cell]).Unknown_004718B0(1, 0, 0)) {
                return 7;
            }
        }
    }
    return 0;
}


// bool TerrainClass::Catch_Fire() (0x006E3D90)
// A burning ordinary terrain object (type kind 6, not a tiberium spawner)
// that is not yet burning starts one of two flame animations 80 leptons
// above its center (one in front of it), and ends in the burning state.

bool TerrainClass::Catch_Fire()
{
    if (!unknown_b5 && !unknown_b4 && Class->unknown_9c == 6 && !Class->SpawnsTiberium) {
        int pick = unknown_id_source_a3d290->Random()();
        AnimClass *anim = new AnimClass(Rules->TreeFire[pick & 1], Center_Coord() + CoordStruct(0, 0, 80), 0, 255, 0x600, 0);
        if (anim) {
            anim->Attach_To(this);
        }
        anim->ZAdjust -= 20;
        unknown_b4 = true;
        return true;
    }
    return false;
}


// void TerrainClass::Fire_Out() (0x006E3E90)
// Ends burning (unknown_b4); a destroyed object then starts crumbling.

void TerrainClass::Fire_Out()
{
    if (unknown_b4) {
        unknown_b4 = false;
        if (!unknown_b5 && Strength == 0) {
            unknown_slot54(1);
            Mark(2);
            Start_Crumbling();
        }
    }
}


// void TerrainClass::AI() (0x006E3F10)
// Animated terrain starts its animation at random (AnimationProbability per
// frame, at AnimationRate); the stage advances when the timer expires. A
// crumbling object is removed on its last frame; a tiberium spawner spreads
// tiberium (0x00478A40) at its middle frame and stops. The stage steps
// through UnknownStage::Graphic_Logic, whose timer test keeps
// CDTimerClass::Value (0x00453EB0) out of line.

void TerrainClass::AI()
{
    ObjectClass::AI();
    if (Class->IsAnimated && Animation.Fetch_Rate() == 0) {
        double chance = abs(unknown_id_source_a3d290->Random()()) % 1000000 * 0.000001;
        if (chance < Class->AnimationProbability) {
            Animation.Set_Stage(0);
            Animation.Set_Rate(Class->AnimationRate);
        }
    }
    if (Animation.Graphic_Logic()) {
        if (unknown_b5) {
            if (Animation.Fetch_Stage() == ((ShapeFileStruct const *)Class->Get_Image_Data())->FrameCount - 1) {
                Delete_Me();
                return;
            }
        }
        if (Class->SpawnsTiberium && Class->IsAnimated) {
            if (Animation.Fetch_Stage() == ((ShapeFileStruct const *)Class->Get_Image_Data())->FrameCount / 2) {
                Animation.Set_Stage(0);
                Animation.Set_Rate(0);
                CoordStruct coord = Coord;
                Map[coord].Spread_Tiberium(true);
            }
        }
    }
}


// void TerrainClass::Start_Crumbling() (0x006E40D0)
// Starts the crumbling animation once (rate 2, from stage 0).
// Assumptions: the name follows Westwood's TerrainClass; unknown_b5 marks it.

void TerrainClass::Start_Crumbling()
{
    if (!unknown_b5) {
        unknown_b5 = true;
        Animation.Set_Rate(2);
        Animation.Value = 0;
    }
}


// bool TerrainClass::Limbo() (0x006E4120)
// Takes the terrain object off the map (ObjectClass slot 52). Unless it was
// never placed (+0x76) every neighbouring cell counts one adjacent terrain
// object less and the cell under it loses the Monolith occupation bit; the
// cell's overlay is then cleared and, outside the map editor mode,
// the cell is refreshed (map, radar). Returns the base result.

bool TerrainClass::Limbo()
{
    if (!unknown_76) {
        Cell cell = unknown_slot104();
        for (int facing = 0; facing < 8; facing++) {
            CellClass &next = Map[Adjacent_Cell(cell, facing)];
            next.unknown_122--;
        }
        CoordStruct coord = Coord;
        ((TerrainCellView &)Map[coord]).unknown_124.Occupy.Monolith = false;
    }
    Cell cell = unknown_slot104();
    bool result = ObjectClass::Limbo();
    Map[cell].Recalc_Attributes(-1);
    if (unknown_a40774 == 0) {
        Map.Update_Cell_Zone(cell);
        Map.Unknown_00566340(cell);
        ((RadarClass &)Map).Radar_Background(cell);
    }
    return result;
}


// static void __fastcall TerrainClass::Read_INI(CCINIClass &ini) (0x006E4260)
// Creates the terrain objects listed in the [Terrain] section ("cell=type";
// the cell number is x + y * 128 before format 4, x + y * 1000 after).
// An INI entry naming a type: the type found or made, else defvalue
// (TibSun findmake.h TGet_Class).
template<class T>
inline T *TGet_Class(CCINIClass &ini, char const *section, char const *entry, T *defvalue)
{
    char buffer[128];

    if (ini.Get_String(section, entry, unknown_string_83c55c, buffer, sizeof(buffer)) != 0) {
        return T::Find_Or_Make(buffer);
    }

    return defvalue;
}

void __fastcall TerrainClass::Read_INI(CCINIClass &ini)
{
    int len = ini.Entry_Count(unknown_terrain_section_7ad390);

    for (int index = 0; index < len; index++) {
        char const *entry = ini.Get_Entry(unknown_terrain_section_7ad390, index);
        TerrainTypeClass *terrain = TGet_Class<TerrainTypeClass>(ini, unknown_terrain_section_7ad390, entry, 0);

        Cell cell;
        if (unknown_ini_format_a40d24 >= 4) {
            int val = atoi(entry);
            cell = Cell(val % 1000, val / 1000);
        } else {
            int val = atoi(entry);
            cell = Cell(val % 128, val / 128);
        }

        if (terrain != 0) {
            new TerrainClass(terrain, cell);
        }
    }
}


// static void __fastcall TerrainClass::Write_INI(CCINIClass &ini) (0x006E4380)
// Rewrites the [Terrain] section ("x + y * 1000=type") from the terrain
// objects that are on the map and marked for saving (unknown_7d).

void __fastcall TerrainClass::Write_INI(CCINIClass &ini)
{
    ini.Clear(unknown_terrain_section_7ad390);
    for (int index = 0; index < Terrains.Count(); index++) {
        TerrainClass *terrain = Terrains[index];
        if (terrain != 0 && !terrain->unknown_76 && terrain->unknown_7d) {
            Cell cell = terrain->unknown_slot104();
            char entry[12];
            sprintf(entry, "%d", cell.X + cell.Y * 1000);
            ini.Put_String(unknown_terrain_section_7ad390, entry, terrain->Class->ID);
        }
    }
}


// bool TerrainClass::Render(Rect *bounds, bool forced, bool extras_only) (0x006E4430; TibSun
// terrain.cpp Render): when the object is to be drawn (debug map, no main window, or forced/needing
// a redraw while down and not in limbo), clips bounds to the tactical rectangle and, when the
// drawn area (slot 73, moved to the tactical origin) overlaps it, draws the object at its pixel
// position adjusted to the clipped origin (slot 67).

bool TerrainClass::Render(Rect *bounds, bool forced, bool extras_only)
{
    if (Debug_Map || !unknown_b263d8 || ((forced || unknown_75) && unknown_74 && !unknown_76)) {
        unknown_75 = false;

        Point2D point;

        *bounds = Intersect(*bounds, unknown_rect_839630);

        Rect rect = Get_Render_Rect() + unknown_rect_839630.Top_Left();

        if (bounds->Is_Overlapping(rect)) {
            unknown_tactical_83996c->Coord_To_Pixel(unknown_slot42(), point);

            if (bounds->X > unknown_rect_839630.X) {
                point.X += unknown_rect_839630.X - bounds->X;
            }
            if (bounds->Y > unknown_rect_839630.Y) {
                point.Y += unknown_rect_839630.Y - bounds->Y;
            }

            unknown_slot67(point, *bounds);
            return true;
        }
    }
    return false;
}


// HRESULT __stdcall TerrainClass::Load(IStream *stream) (0x006E4570)
// Leaves the object index under the old ID, restores the object in place,
// rejoins the index under the loaded ID and swizzles the type.

HRESULT __stdcall TerrainClass::Load(IStream *stream)
{
    unknown_object_index_abff58.Remove_Index(Fetch_ID());
    HRESULT result = ObjectClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) TerrainClass(NoInitClass());
    Add_Object_Index(Fetch_ID(), this);
    Swizzle_Pointer(Class);
    return result;
}


// HRESULT __stdcall TerrainClass::Save(IStream *stream, BOOL clear_dirty) (0x006E4700)

HRESULT __stdcall TerrainClass::Save(IStream *stream, BOOL clear_dirty)
{
    return ObjectClass::Save(stream, clear_dirty);
}


// void TerrainClass::Compute_CRC(WWCRCEngine &crc) const (0x006E4720)

void TerrainClass::Compute_CRC(WWCRCEngine &crc) const
{
    ObjectClass::Compute_CRC(crc);
    crc((long)Class->Kind_Of());
    crc(Class->Fetch_ID());
    crc(unknown_b4);
    crc(unknown_b5);
    crc((long)unknown_b8.X);
    crc((long)unknown_b8.Y);
}


// void TerrainClass::Detach(AbstractClass *target, bool all) (0x006E47A0)

void TerrainClass::Detach(AbstractClass *target, bool all)
{
    ObjectClass::Detach(target, all);
    if (Class == target) {
        Class = 0;
    }
}


// bool TerrainClass::Unlimbo(CoordStruct const *coord, int dir) (0x006E47D0)
// Places the terrain object (ObjectClass slot 53). On success every
// neighbouring cell counts one more adjacent terrain object (+0x122), the
// screen position of the object is stored (offset by the view's position)
// and a tiberium-spawning type clears the overlay of its cell.

bool TerrainClass::Unlimbo(CoordStruct const *coord, int dir)
{
    if (ObjectClass::Unlimbo(coord, dir)) {
        Cell cell(coord->X / 256, coord->Y / 256);
        for (int facing = 0; facing < 8; facing++) {
            CellClass &next = Map[cell + AdjacentCell[facing & 7]];
            next.unknown_122++;
        }
        Point2D &pixel = (Point2D &)unknown_b8.Width;
        unknown_tactical_83996c->Coord_To_Pixel(unknown_slot42(), pixel);
        Point2D offset(unknown_tactical_83996c->TacPixelX, unknown_tactical_83996c->TacPixelY);
        pixel.X += offset.X;
        pixel.Y += offset.Y;
        if (Class->SpawnsTiberium) {
            Map[*coord].OverlayIndex = -1;
            Map[*coord].OverlayData = 0;
        }
        return true;
    }
    return false;
}


// ObjectTypeClass *TerrainClass::Class_Of() const (0x006E4900)

ObjectTypeClass *TerrainClass::Class_Of() const
{
    return Class;
}


// Rect TerrainClass::Get_Render_Rect() (0x006E4910, slot 73)
// The area the object draws over, in tactical-area pixels (hypothesis):
// the union of its image's first frame and its shadow frame (the second
// half), centred on the image and placed at its screen point (+0xC0)
// relative to the view origin; the empty rectangle without an image.

Rect TerrainClass::Get_Render_Rect()
{
    Point2D origin(unknown_tactical_83996c->TacPixelX, unknown_tactical_83996c->TacPixelY);
    Point2D offset(unknown_b8.Width - origin.X, unknown_b8.Height - origin.Y);
    ShapeFileStruct const *shape = (ShapeFileStruct const *)ObjectClass::Get_Image_Data();
    if (shape == 0) {
        return SHAPE_FILE_EMPTY_RECT;
    }
    Rect image = shape->Get_Frame_Rect(0);
    Rect shadow = shape->Get_Frame_Rect(shape->FrameCount / 2);
    int width = shape->Width;
    int height = shape->Height;
    Rect rect = Union(image, shadow);
    return Rect(rect.X - width / 2 + offset.X, rect.Y - height / 2 + offset.Y, rect.Width, rect.Height);
}


// int TerrainClass::Size_Of() const (0x006E4AF0)
// Compiler-emitted from shared/terrain.hpp in this unit; no source of its own.


// int TerrainClass::Kind_Of() const (0x006E4B00)
// Compiler-emitted from shared/terrain.hpp in this unit; no source of its own.


// HRESULT __stdcall TerrainClass::GetClassID(GUID *clsid) (0x006E4B10)
// Compiler-emitted from shared/terrain.hpp in this unit; no source of its own.


// void *TerrainClass::`scalar deleting destructor'(unsigned int) (0x006E4B50)
// Compiler-emitted from shared/terrain.hpp in this unit; no source of its own.
