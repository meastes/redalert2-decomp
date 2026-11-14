// cell.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/cell.json carries the addresses and the rows that
// stay in their own files.


#define RA2_CELL_CLEAR_TO_BUILD   // declares CellClass::Land_Type for Is_Clear_To_Build 0x004718B0 (shared/cell_class.hpp)
#define RA2_TACTICAL_PLACEMENT   // declares CellClass::Draw_Placement_Cursor 0x00473F20 (shared/cell_class.hpp)
#define RA2_CELL_SET_CELLID     // declares CellClass::Set_CellID 0x0047A450 (shared/cell_class.hpp)
#define RA2_CELL_RECALC_ATTRIBUTES
#define RA2_COORD_FROM_CELL_INLINE
#define RA2_CELL_LAT_BOOL
#define RA2_CELL_GOODIE_CHECK
#define RA2_CELL_AS_COORD
#define RA2_SHIP_MOVE_TO_VALUE
#define RA2_IS_MOVING_HERE_VALUE
#define RA2_RECT_CORNER_INLINE
#define CELL_DECLARE_PREVIEW_CELL_COLOR
#define CELL_DECLARE_CLEAR_ICON
#define RGB_DECLARE_ACCESSORS
#define CELL_DECLARE_CELL_COLOR
#define CELL_DECLARE_DRAW_IT
#define RGB_DECLARE_BRIGHTEN
#define RA2_SHAPE_FRAME_COLOR
#define RA2_DRAW_SHAPE_OPERATIONS
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/cell.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/light_convert.hpp"
#include "shared/building_class.hpp"
#include "shared/building_type_class.hpp"
#include "shared/house.hpp"
#include "shared/color_scheme.hpp"
#include "shared/convert.hpp"
#include "shared/overlay_type.hpp"
#include "shared/isometric_tile_type.hpp"
#include "shared/tileset_index.hpp"
#include "shared/hicolor_pixel.hpp"
#include "shared/display.hpp"
#include "shared/overlay.hpp"
#include "shared/anim_type.hpp"
#include "shared/shape_file.hpp"
#include "shared/theater_type.hpp"
#include "shared/id_source.hpp"
#include "shared/object.hpp"
#include "shared/map_view.hpp"
#include "shared/ground_type.hpp"
#include "shared/rules.hpp"
#include "shared/smudge.hpp"
#include "shared/house_type.hpp"
#include "shared/anim.hpp"
#include "shared/tactical.hpp"
#include "shared/tube.hpp"
#include "shared/palette.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/blit.hpp"
#include "shared/tiberium.hpp"
#include "shared/cd_timer.hpp"
#include "shared/debug_printf.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/smudge_type.hpp"
#include "shared/abstract.hpp"
#include "shared/infantry.hpp"
#include "shared/random2.hpp"
#include "shared/crt_memory.hpp"
#include "shared/techno.hpp"
#include "shared/foot.hpp"
#include "shared/techno_type.hpp"
#include "shared/tag.hpp"
#include "shared/warhead_type.hpp"
#include "shared/bullet.hpp"
#include "shared/sound_play.hpp"
#include "shared/vox_class.hpp"
#include "shared/fast_math.hpp"
#include "shared/coord_distance.hpp"
#include "shared/layer.hpp"
#include "shared/unknown_list_832470.hpp"
#include "shared/super.hpp"
#include "shared/super_weapon_type.hpp"
#include "shared/ini_enums.hpp"
#include "shared/terrain.hpp"
#include "shared/terrain_type.hpp"
#include "shared/swizzle.hpp"
#include "shared/lightning_storm.hpp"
#include "shared/light_source.hpp"
#include "shared/cell_class.hpp"
#include "shared/fogged_object.hpp"
#include "shared/unknown_vector3.hpp"
#define Min(a, b) ((a) < (b) ? (a) : (b))
#include "shared/rect.hpp"


// The Cell.cpp unit expands Rect::operator- 0x0047D000 in TibSun form (Rect(Top_Left() - point, ...)) with
// Top_Left and the corner constructor inline (rect.hpp RA2_RECT_CORNER_INLINE); 0x0047D040 stays out of line.
// CellClass::Preview_Cell_Color 0x004710D0 and the tile variation helper Clear_Icon 0x004767A0 it calls,
// with the RGBClass channel accessors.
// Header-static dynamic initializers of the unit at 0x00470490:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00470490-0x00470676); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004704C0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00470490.cpp; no source of its own.


// Dynamic initializer (0x004704E0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00470490.cpp; no source of its own.


// Dynamic initializer (0x00470500, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00470490.cpp; no source of its own.


// Dynamic initializer (0x00470520, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00470490.cpp; no source of its own.


// Dynamic initializer (0x00470540, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00470490.cpp; no source of its own.


// Dynamic initializer (0x00470560, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00470490.cpp; no source of its own.


// Dynamic initializer (0x004705A0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00470490.cpp; no source of its own.


// Dynamic initializer (0x004705D0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00470490.cpp; no source of its own.


// Dynamic initializer (0x00470600, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00470490.cpp; no source of its own.


// Dynamic initializer (0x00470630, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00470490.cpp; no source of its own.


// Dynamic initializer (0x00470640, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00470490.cpp; no source of its own.


// Dynamic initializer (0x00470660, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00470490.cpp; no source of its own.


// CellClass::CellClass(NoInitClass const &noinit) (0x00470680): installs the
// vtables and clears +0x34 and +0x4C (Load builds the cell in place).

CellClass::CellClass(NoInitClass const &noinit) :
    AbstractClass(noinit),
    unknown_34(0),
    unknown_4c(0)
{
}


// int CellClass::unknown_4706c0(Point2D const &point) const (0x004706C0): TibSun Get_Height.

int CellClass::unknown_4706c0(Point2D const &point) const
{
    static double _level_height = unknown_geometry_g;
    static double _level_slope = _level_height / 256.0;

    static struct {
        double XChange;
        double YChange;
        double Base;
        double Max;
        double Extra;
    } _ramp_control[20] = {
        {  1.0,  0.0,  0.0,                           _level_height,                  0.0 },
        {  0.0,  1.0,  0.0,                           _level_height,                  0.0 },
        { -1.0,  0.0,  _level_height,                 _level_height,                  0.0 },
        {  0.0, -1.0,  _level_height,                 _level_height,                  0.0 },
        {  1.0,  1.0, -_level_height,                 _level_height,                  0.0 },
        { -1.0,  1.0,  0.0,                           _level_height,                  0.0 },
        { -1.0, -1.0,  _level_height,                 _level_height,                  0.0 },
        {  1.0, -1.0,  0.0,                           _level_height,                  0.0 },
        {  1.0,  1.0,  0.0,                           _level_height,                  0.0 },
        { -1.0,  1.0,  _level_height,                 _level_height,                  0.0 },
        { -1.0, -1.0,  _level_height + _level_height, _level_height,                  0.0 },
        {  1.0, -1.0,  _level_height,                 _level_height,                  0.0 },
        {  1.0,  1.0,  0.0,                           _level_height + _level_height,  0.0 },
        { -1.0,  1.0,  _level_height,                 _level_height + _level_height,  0.0 },
        { -1.0, -1.0,  _level_height + _level_height, _level_height + _level_height,  0.0 },
        {  1.0, -1.0,  _level_height,                 _level_height + _level_height,  0.0 },
        {  0.0,  0.0,  0.0,                           _level_height * 0.5,            _level_height * 0.5 },
        {  0.0,  0.0,  _level_height,                 _level_height * 0.5,            _level_height * -0.5 },
        {  0.0,  0.0,  0.0,                           _level_height * 0.5,            _level_height * 0.5 },
        {  0.0,  0.0,  _level_height,                 _level_height * 0.5,            _level_height * -0.5 }
    };

    int height = int(unknown_geometry_g * unknown_11b + 0.5);

    if (unknown_11c > 0) {
        int ramp = unknown_11c - 1;
        double rampheight = ((point.X & 255) * _ramp_control[ramp].XChange * _level_slope) +
                            ((point.Y & 255) * _ramp_control[ramp].YChange * _level_slope) +
                            _ramp_control[ramp].Base +
                            _ramp_control[ramp].Extra;

        if (rampheight < 0.0) {
            rampheight = 0.0;
        }
        if (rampheight > _ramp_control[ramp].Max) {
            rampheight = _ramp_control[ramp].Max;
        }

        height += rampheight;
    }

    return height;
}


// CellClass::~CellClass() (0x00470E80): deletes the fogged-object list and
// the owned +0xFC object, clears the occupier and, for a cell other than the
// zero cell, releases +0x34 (its count drops while unknown_flag_a40958 is set).

CellClass::~CellClass()
{
    if (FoggedObjects != 0) {
        delete FoggedObjects;
        FoggedObjects = 0;
    }
    if (unknown_fc != 0) {
        delete unknown_fc;
        unknown_fc = 0;
    }
    OccupierPtr = 0;
    if (MapCoords != unknown_geometry_k && unknown_34 != 0) {
        if (unknown_flag_a40958) {
            unknown_34->unknown_194--;
        }
        unknown_34 = 0;
    }
}


// CellClass::CellClass() (0x00470F10): an empty cell at the zero cell: no
// tile (0xFFFF), overlay or smudge (-1), the per-house counters and +0xAC
// cleared (two element loops: VC6 turns each into lea edi / mov ecx / rep stosd,
// a memset call schedules mov ecx first), levels and flags reset (the +0x12C
// and +0x140 bitfields clear in the member-init order); then registers its ID.

CellClass::CellClass() :
    MapCoords(unknown_geometry_k),
    FoggedObjects(0),
    unknown_2c(0),
    unknown_30(0),
    unknown_34(0),
    IsoTileIndex(0xFFFF),
    unknown_3c(0),
    unknown_40(0),
    OverlayIndex(-1),
    SmudgeIndex(-1),
    unknown_4c(0),
    unknown_50(-1),
    unknown_54(-1),
    unknown_58(-1),
    unknown_5c(-1),
    unknown_60(-1),
    unknown_64(-1),
    unknown_68(Rect(0, 0, 0, 0)),
    unknown_78(0),
    unknown_dc(0),
    unknown_e0(0),
    OccupierPtr(0),
    AltOccupierPtr(0),
    unknown_ec(0),
    unknown_f0(0),
    unknown_f8(0),
    unknown_fc(0),
    unknown_100(0),
    unknown_104(0x10000),
    unknown_108(0),
    unknown_10a(1000),
    unknown_10c(1000),
    unknown_10e(1000),
    unknown_110(1000),
    unknown_112(1000),
    unknown_114(1000),
    unknown_116(-1),
    unknown_118(0xFF),
    unknown_119(0),
    unknown_11a(0),
    unknown_11b(0),
    unknown_11c(0),
    unknown_11d(0),
    OverlayData(0),
    SmudgeData(0),
    unknown_120(-2),
    unknown_121(-2),
    unknown_122(0),
    Flags_12c(0),
    unknown_130(1),
    unknown_134(0),
    unknown_138(false),
    unknown_13c(0),
    Flags_140(0)
{
    int index;
    for (index = 0; index < 24; index++) {
        unknown_7c[index] = 0;
    }
    for (index = 0; index < 12; index++) {
        unknown_ac[index] = 0;
    }
    Create_ID();
    unknown_124 = 0;
    unknown_128 = 0;
}


// unsigned int CellClass::Preview_Cell_Color(unsigned char &unknown, bool flag) (0x004710D0)
// TibSun Preview_Cell_Color: the cell's two preview pixels, packed first << 16 | second. A visible
// building (unless flag) gives its house colour scheme's colour; under a bridge, the radar colour of
// overlay type 24 (TibSun OVERLAY_BRIDGE1) as a surface pixel; otherwise the preview pixels of the
// cell's tile variation (TibSun Clear_Icon picks it). Cell_Building is unknown_4717b0 and IsUnderBridge
// the accessor Is_Bridge_Here, both inline in this unit (the original calls Find_Object out of line
// here because of them). The unit's header statics 0x00470490 define CELL_DECLARE_PREVIEW_CELL_COLOR,
// CELL_DECLARE_CLEAR_ICON and RGB_DECLARE_ACCESSORS.

// The building's visual state (ra2-decomp cell.cpp Building_Visual, file-local inline).
inline int Building_Visual(BuildingClass *building)
{
    return building->unknown_slot25(0, 0);
}

unsigned int CellClass::Preview_Cell_Color(unsigned char &unknown, bool flag)
{
    unknown = 0;
    BuildingClass *obj = (BuildingClass *)Cell_Building();

    if (!flag && obj != 0 && !obj->Type->Invisible && !obj->Type->InvisibleInGame && Building_Visual(obj) != 5) {
        ColorScheme *scheme = ColorSchemes[obj->House->ColorSchemeIndex];
        unsigned int color = scheme->Converter->Convert(scheme->unknown_0x314);
        return (color << 16) | color;
    }

    if (IsUnderBridge) {
        RGBClass bridge_color = OverlayTypes[24]->Get_Radar_Color(0);
        unsigned int color = RGB_To_Pixel(bridge_color.Get_Red(), bridge_color.Get_Green(), bridge_color.Get_Blue());
        return (color << 16) | color;
    }

    IsometricTileTypeClass *ittype;
    int icon;
    if (IsoTileIndex != 0xFFFF) {
        ittype = IsoTileTypes[IsoTileIndex];
    } else {
        ittype = IsoTileTypes[unknown_tileset_a52f00];
    }

    icon = 0;
    if (IsoTileIndex != 0xFFFF) {
        if (ittype->NumTileTypesInSet > 1) {
            if (ittype->Is_Randomized((unsigned char)unknown_11a)) {
                icon = (unknown_140 >> 13) & 1;
            } else {
                int numtiles = ittype->NumTileTypesInSet;
                icon = Clear_Icon(IsoTileIndex, numtiles);
            }
        }
    } else {
        icon = Clear_Icon(unknown_tileset_a52f00, ittype->NumTileTypesInSet);
    }

    IsometricTileTypeClass *next = ittype->Next_Tile_From_Set(icon);
    unsigned short *tile = next->Preview_Tile((unsigned char)unknown_11a, unknown_11b);
    return (tile[0] << 16) | tile[1];
}


// void CellClass::Cell_Color(RGBClass &lowcolor, RGBClass &highcolor) const (0x004712C0): TibSun Cell_Color.


void CellClass::Cell_Color(RGBClass &lowcolor, RGBClass &highcolor) const
{
    if (Cell_Terrain() != 0) {
        lowcolor = RGBClass(200, 200, 160);
        highcolor = RGBClass(200, 200, 160);
        return;
    }

    if (IsUnderBridge) {
        highcolor = lowcolor = OverlayTypes[24]->Get_Radar_Color(0);
        return;
    }

    IsometricTileTypeClass *itype = (IsoTileIndex != 0xFFFF) ? IsoTileTypes[IsoTileIndex] : IsoTileTypes[unknown_tileset_a52f00];

    int overlay = OverlayIndex;
    if ((OverlayIndex != -1 && OverlayIndex != 100 && OverlayIndex != 101) || (OverlayIndex != -1 && OverlayIndex != 231 && OverlayIndex != 232)) {
        OverlayTypeClass *otype = OverlayTypes[overlay];

        if (!otype->Tiberium) {
            if ((overlay >= 74 && overlay <= 99) || (overlay >= 205 && overlay <= 230)) {
                lowcolor = OverlayTypes[overlay]->Get_Radar_Color(1);
            } else {
                lowcolor = OverlayTypes[overlay]->Get_Radar_Color(OverlayData);
            }
            highcolor = lowcolor;
            return;
        }

        if (otype->CellAnim != 0) {
            ShapeFileStruct const *shape = (ShapeFileStruct const *)otype->CellAnim->Get_Image_Data();
            highcolor = lowcolor = shape->Get_Color(OverlayData);
            return;
        }

        if (OverlayClass::To_Tiberium(overlay) != -1) {
            lowcolor = RGBClass(170, 170, 130);
            highcolor = RGBClass(170, 170, 130);
            return;
        }
    }

    int tile = 0;
    if (IsoTileIndex != 0xFFFF) {
        if (itype->NumTileTypesInSet > 1) {
            if (itype->Is_Randomized((unsigned char)unknown_11a)) {
                tile = (unknown_140 >> 13) & 1;
            } else {
                int numtiles = itype->NumTileTypesInSet;
                tile = Clear_Icon(IsoTileIndex, numtiles);
            }
        }
    } else {
        int numtiles = itype->NumTileTypesInSet;
        tile = Clear_Icon(unknown_tileset_a52f00, numtiles);
    }

    IsometricTileTypeClass *next = itype->Next_Tile_From_Set(tile);
    if (next->unknown_a4 == 0 && next->unknown_2c4) {
        next->Load_Tile_Image();
    }

    IsoTileImageStruct const *image = (IsoTileImageStruct const *)next->unknown_a4;
    if (image->Tiles[(unsigned char)unknown_11a] != 0) {
        IsoTileCellStruct const *record = (IsoTileCellStruct const *)image->Tiles[(unsigned char)unknown_11a];
        RGBClass lowest(record->LowRed, record->LowGreen, record->LowBlue);
        lowest = RGBClass(NoInitClass()).Set(lowest, Theaters[unknown_id_source_a3d290->Theater].unknown_50);
        int red = lowest.Get_Red() >> 1;
        int green = lowest.Get_Green() >> 1;
        int blue = lowest.Get_Blue() >> 1;
        lowcolor = RGBClass(red, green, blue);
        highcolor = RGBClass(red, green, blue);
    } else {
        lowcolor = RGBClass(60, 60, 60);
        highcolor = RGBClass(60, 60, 60);
    }
}


// ObjectClass *CellClass::Cell_Techno(Point2D const &spot, bool alt, ObjectClass *except) const
// (0x00471660): Westwood's Cell_Techno. The techno occupier (alt: on the bridge) other than except
// closest to the pixel spot inside the cell.

ObjectClass *CellClass::Cell_Techno(Point2D const &spot, bool alt, ObjectClass *except) const
{
    ObjectClass *object;
    CoordStruct click;
    ObjectClass *close = 0;
    int distance = 0;

    click = CoordStruct(spot.X * 7, spot.Y * 7, 0);
    ObjectClass *occupier = alt ? AltOccupierPtr : OccupierPtr;
    if (occupier != 0) {
        object = occupier;
        while (object != 0) {
            bool techno;
            if (object != 0) {
                techno = object->AbstractFlags.IsTechno;
            }
            if (object != 0 && techno && object != except) {
                CoordStruct coord = object->Center_Coord();
                // Westwood's Coord_Fraction(coord) - click.
                CoordStruct fraction = CoordStruct(coord.X & 0xFF, coord.Y & 0xFF, coord.Z) - click;
                int dist = Point2D(fraction.X, fraction.Y).Length();
                if (close == 0 || dist < distance) {
                    close = object;
                    distance = dist;
                }
            }
            object = object->unknown_30;
        }
    }
    return close;
}


// ObjectClass *CellClass::Find_Object(int kind, bool alt) const (0x00471760):
// the first occupant (alt: of the alternate list) whose Kind_Of is kind;
// none while unknown_flag_a40958 is clear (TibSun cell.cpp Cell_Find_Object: the list head once, then a while
// loop; this form lets Draw_Placement_Cursor 0x00473F20 expand it through Cell_Terrain as the original does).

ObjectClass *CellClass::Find_Object(int kind, bool alt) const
{
    if (unknown_flag_a40958) {
        ObjectClass *object = alt ? AltOccupierPtr : OccupierPtr;

        while (object != 0) {
            if (object->Kind_Of() == kind) {
                return object;
            }
            object = object->unknown_30;
        }
    }
    return 0;
}


// ObjectClass *CellClass::Cell_Building() const (0x004717B0): the building
// in the cell (Find_Object kind 6).

ObjectClass *CellClass::Cell_Building() const
{
    return Find_Object(6);
}


// ObjectClass *CellClass::Cell_Terrain(bool alt) const (0x004717E0): the terrain object in the
// cell (Find_Object kind 0x24).

ObjectClass *CellClass::Cell_Terrain(bool alt) const
{
    return Find_Object(0x24, alt);
}


// ObjectClass *CellClass::Cell_Object(Point2D const &spot, bool alt) (0x00471830): TibSun cell.cpp Cell_Object:
// an aircraft (kind 2) in the occupant list, else the occupant nearest the spot (Cell_Techno), else the terrain
// object (Cell_Terrain). TeamClass 0x006BB260 and Draw_Placement_Cursor 0x00473F20 (which expands it) ask it.

ObjectClass *CellClass::Cell_Object(Point2D const &spot, bool alt)
{
    ObjectClass *object;

    object = Find_Object(2);
    if (object != 0) {
        return object;
    }

    object = Cell_Techno(spot, alt, 0);
    if (object != 0) {
        return object;
    }
    object = Cell_Terrain();
    return object;
}


// bool CellClass::Is_Clear_To_Build(int speed, BuildingTypeClass const *what, HouseClass *who) (0x004718B0):
// TibSun cell.cpp Is_Clear_To_Build with RA2's naval tile check (Is_Tile_Water) and float movement costs. The
// gate/post arm is written before the foundation arm, the laser-fence arm as one condition, and the function ends in
// the if/else: VC6 shares one return-false epilogue from these forms (Scout1228 NOTES, return-false layout).

bool CellClass::Is_Clear_To_Build(int speed, BuildingTypeClass const *what, HouseClass *who)
{
    if (unknown_a40774) {
        return true;
    }

    if (what != 0) {
        if (what->LaserFence) {
            if (Cell_Building() != 0 || Cell_Terrain() != 0) {
                return false;
            }
        } else if (what->LaserFencePost || what->Gate) {
            ObjectClass *obj = Cell_Object(Point2D(0, 0), false);
            if (obj != 0) {
                if (obj->Kind_Of() != 6) {
                    return false;
                }
                if (!((BuildingClass *)obj)->Type->LaserFence || obj->Owning_House() != who) {
                    return false;
                }
            }
            if ((unknown_124 & 0x3F) != 0) {
                return false;
            }
        } else {
            if (what->unknown_bf0 == 0) {
                if (Cell_Object(Point2D(0, 0), false) != 0) {
                    return false;
                }
                if ((unknown_124 & 0x3F) != 0) {
                    return false;
                }
            } else {
                if (IsoTileIndex >= 0 && IsoTileIndex < IsoTileTypes.Count() && !IsoTileTypes[IsoTileIndex]->unknown_2b0) {
                    return false;
                }
                if (Cell_Building() != 0) {
                    return false;
                }
            }
        }
    }

    if (Debug_Map) {
        if (!Map.In_Radar(MapCoords)) {
            return false;
        }
    } else {
        if (!Map.Unknown_0055A740((CellClass *)this, true)) {
            return false;
        }
    }

    if (OverlayIndex != -1) {
        HouseClass *owner = 0;
        if (unknown_50 >= 0) {
            owner = Houses[unknown_50];
        }

        if (OverlayIndex == 2 || OverlayIndex == 0) {
            if (what != 0) {
                if (what->ToOverlay != 0 && what->ToOverlay->ArrayIndex == OverlayIndex && OverlayData >= 0x10 || what == Rules->unknown_78c || what == Rules->unknown_77c || what == Rules->unknown_780) {
                    if (owner == who) {
                        return true;
                    }
                }
            }
        }

        if (OverlayIndex == 0x1A) {
            if (what != 0) {
                if ((what->ToOverlay != 0 && what->ToOverlay->ArrayIndex == 0x1A && OverlayData >= 0x10) || what == Rules->unknown_784 || what == Rules->unknown_788) {
                    if (owner == who) {
                        return true;
                    }
                }
            }
        }

        if (what != 0 && what->LaserFence && (OverlayIndex == 0x7E || OverlayClass::To_Tiberium(OverlayIndex) != -1)) {
            if (Is_Bridge_Here() || WasUnderBridge || unknown_11c != 0) {
                return false;
            }
            return true;
        }

        if (!Debug_Map || OverlayTypes[OverlayIndex]->Wall) {
            return false;
        }
    }

    if (speed == -1) {
        if (Is_Bridge_Here() || WasUnderBridge || unknown_11c != 0) {
            return false;
        }
        if (what->Naval) {
            if (!Is_Tile_Water()) {
                return false;
            }
            return true;
        }
        return Ground[Land_Type()].Build;
    } else {
        if (Ground[Land_Type()].Cost[speed] == 0) {
            return false;
        }
        return true;
    }
}


// LAT boundary repair, full original00471D10..00472499.
extern int unknown_tileset_a52f84;
extern int unknown_tileset_a52c70;
extern int unknown_tileset_a52590;
extern int unknown_tileset_a52ef8;
extern int unknown_tileset_a52ee0;
extern int unknown_tileset_a6cb74;
extern int unknown_tileset_a52ef4;
extern int unknown_tileset_a6dd0c;
extern int unknown_tileset_a52c68;
extern int unknown_tileset_a52ea8;
extern int unknown_tileset_a6e018;
extern int unknown_tileset_a6e0e8;
extern int unknown_tileset_a6cf4c;
extern int unknown_tileset_a52c60;
extern int unknown_tileset_a6e0e0;

static Cell Translate_Cell_00471d10(Cell const &origin, Cell delta)
{
    Cell result;
    result.X = delta.X + origin.X;
    result.Y = origin.Y + delta.Y;
    return result;
}

static bool Is_LAT_Family_Tile_00471d10(int tile, int ground, int first, int last)
{
    return tile == ground || (tile >= first && tile <= last);
}

bool CellClass::Fixup_LAT()
{
    int road_first = unknown_tileset_a52ee0;
    int road_last = road_first + 1;
    if (road_first == -1) road_last = -1;
    int misc_first = unknown_tileset_a6cb74;
    int misc_last = misc_first + 41;
    if (misc_first == -1) misc_last = -1;
    int rough_first = unknown_tileset_a52f84;
    int rough_last = rough_first + 15;
    if (rough_first == -1) rough_last = -1;
    int sand_first = unknown_tileset_a52c70;
    int sand_last = sand_first + 15;
    if (sand_first == -1) sand_last = -1;
    int green_first = unknown_tileset_a52590;
    int green_last = green_first + 15;
    if (green_first == -1) green_last = -1;
    int pave_first = unknown_tileset_a52ef8;
    int pave_last = pave_first + 15;
    if (pave_first == -1) pave_last = -1;
    int median_last = unknown_tileset_a52ef4;
    int median_first = median_last;
    median_last += 13;
    if (median_first == -1) median_last = -1;
    int pave_extra_last = unknown_tileset_a6dd0c;
    int pave_extra_first = pave_extra_last;
    pave_extra_last += 20;
    if (pave_extra_first == -1) pave_extra_last = -1;
    int pave_alt_last = unknown_tileset_a52c68;
    int pave_alt_first = pave_alt_last;
    pave_alt_last += 13;
    if (pave_alt_first == -1) pave_alt_last = -1;
    int smooth_first = unknown_tileset_a52ea8;
    int smooth_last = smooth_first + 11;
    if (smooth_first == -1) smooth_last = -1;
    int ramp_first = unknown_tileset_a6e018;
    int ramp_last = ramp_first + 19;
    if (ramp_first == -1) ramp_last = -1;
    int offset;
    unsigned facing;
    int index;
    int old_tile = IsoTileIndex;
    if (Is_LAT_Family_Tile_00471d10(IsoTileIndex, unknown_tileset_a6e0e8, rough_first, rough_last)) {
        offset = 0;
        facing = 0;
        for (index = 0; index < 4; index++) {
            CellClass const &neighbor = Adjacent_Cell(facing);
            int tile = neighbor.IsoTileIndex;
            if (!Is_LAT_Family_Tile_00471d10(tile, unknown_tileset_a6e0e8, rough_first, rough_last)) offset |= 1 << index;
            facing = (facing + 2) & 7;
        }
        if (offset != 0) IsoTileIndex = rough_first + offset;
        else IsoTileIndex = unknown_tileset_a6e0e8;
    }
    if (sand_first != -1 && (Is_LAT_Family_Tile_00471d10(IsoTileIndex, unknown_tileset_a6cf4c, sand_first, sand_last))) {
        offset = 0;
        facing = 0;
        for (index = 0; index < 4; index++) {
            CellClass const &neighbor = Adjacent_Cell(facing);
            int tile = neighbor.IsoTileIndex;
            if (!Is_LAT_Family_Tile_00471d10(tile, unknown_tileset_a6cf4c, sand_first, sand_last)) offset |= 1 << index;
            facing = (facing + 2) & 7;
        }
        if (offset != 0) IsoTileIndex = sand_first + offset;
        else IsoTileIndex = unknown_tileset_a6cf4c;
    }
    if (green_first != -1 && (Is_LAT_Family_Tile_00471d10(IsoTileIndex, unknown_tileset_a52c60, green_first, green_last))) {
        offset = 0;
        facing = 0;
        for (index = 0; index < 4; index++) {
            CellClass const &neighbor = Adjacent_Cell(facing);
            int tile = neighbor.IsoTileIndex;
            if (!Is_LAT_Family_Tile_00471d10(tile, unknown_tileset_a52c60, green_first, green_last) &&
                (tile < misc_first || tile > misc_last) &&
                (tile < road_first || tile > road_last)) offset |= 1 << index;
            facing = (facing + 2) & 7;
        }
        if (offset != 0) IsoTileIndex = green_first + offset;
        else IsoTileIndex = unknown_tileset_a52c60;
    }
    if (pave_first != -1 && (Is_LAT_Family_Tile_00471d10(IsoTileIndex, unknown_tileset_a6e0e0, pave_first, pave_last))) {
        offset = 0;
        facing = 0;
        for (index = 0; index < 4; index++) {
            CellClass const &neighbor = Adjacent_Cell(facing);
            int tile = neighbor.IsoTileIndex;
            if (!Is_LAT_Family_Tile_00471d10(tile, unknown_tileset_a6e0e0, pave_first, pave_last) &&
                (tile < median_first || tile > median_last) &&
                (tile < pave_alt_first || tile > pave_alt_last) &&
                (tile < pave_extra_first || tile > pave_extra_last)) offset |= 1 << index;
            facing = (facing + 2) & 7;
        }
        if (offset != 0) IsoTileIndex = pave_first + offset;
        else IsoTileIndex = unknown_tileset_a6e0e0;
    }
    int tile = IsoTileIndex;
    if ((tile >= ramp_first && tile <= ramp_last) ||
        (tile >= smooth_first && tile <= smooth_last)) {
        offset = 0;
        if (unknown_11c == 1) {
            if (Map[::Adjacent_Cell(Fetch_CellID(), 6)].unknown_11c == 0) offset |= 1;
            if (Map[Translate_Cell_00471d10(Fetch_CellID(), AdjacentCell[2])].unknown_11c == 0) offset |= 2;
            if (offset != 0) IsoTileIndex = offset + smooth_first + -1;
        }
        if (unknown_11c == 2) {
            if (Map[Translate_Cell_00471d10(Fetch_CellID(), AdjacentCell[0])].unknown_11c == 0) offset |= 1;
            if (Map[::Adjacent_Cell(Fetch_CellID(), 4)].unknown_11c == 0) offset |= 2;
            if (offset != 0) IsoTileIndex = offset + smooth_first + 2;
        }
        if (unknown_11c == 3) {
            if (Map[Translate_Cell_00471d10(Fetch_CellID(), AdjacentCell[2])].unknown_11c == 0) offset |= 1;
            if (Map[::Adjacent_Cell(Fetch_CellID(), 6)].unknown_11c == 0) offset |= 2;
            if (offset != 0) IsoTileIndex = offset + smooth_first + 5;
        }
        if (unknown_11c == 4) {
            if (Map[::Adjacent_Cell(Fetch_CellID(), 4)].unknown_11c == 0) offset |= 1;
            if (Map[Translate_Cell_00471d10(Fetch_CellID(), AdjacentCell[0])].unknown_11c == 0) offset |= 2;
            if (offset != 0) IsoTileIndex = offset + smooth_first + 8;
        }
        if (offset == 0) IsoTileIndex = unknown_11c + ramp_first - 1;
    }
    if (old_tile != IsoTileIndex) {
        IsometricTileTypeClass *tile_type = IsoTileTypes[IsoTileIndex];
        if (tile_type->Get_Image_Data() == 0) tile_type->Load_Tile_Image();
    }
    return old_tile != IsoTileIndex;
}


// void CellClass::Set_Wall_Owner() (0x004724A0): for a wall overlay, stores
// in +0x50 the owner (slot 14) of the nearest building (ObjectClass
// unknown_5d4c00 distance) that is on the map (+0x74, +0x7D) and belongs to a
// house whose type is a WallOwner; -1 when there is none (MapClass 0x0054B535).

void CellClass::Set_Wall_Owner()
{
    if (OverlayIndex != -1 && OverlayTypes[OverlayIndex]->Wall) {
        int owner = -1;
        int best = 0x7FFFFFFF;
        for (int index = 0; index < unknown_BuildingClass_list_00a40af8.Count(); index++) {
            BuildingClass *building = unknown_BuildingClass_list_00a40af8[index];
            if (building->unknown_7d && building->unknown_74 && building->House->Type->WallOwner) {
                int distance = building->unknown_5d4c00(this);
                if (distance < best) {
                    best = distance;
                    owner = building->Owner();
                }
            }
        }
        unknown_50 = owner;
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void CellClass::Recalc_Attributes(int cell_height) (0x00472540): Recalc_Attributes.
// Adopted from ra2-decomp CellClass::Recalc_Attributes (YR 0x0047D2B0).
// Recomputes the cell's land type (+0xEC), ramp (+0x11C) and elevation (+0x11D)
// from its overlay and tile, makes a tunnel's tube, attaches the tile's
// animation, marks the cells a cliff shadows, applies the rules' cliff back
// impassability and stores the passability and height in the map's zone tables.
// Needs RA2_CELL_RECALC_ATTRIBUTES and RA2_COORD_FROM_CELL_INLINE (first row 0x00470490).
// The tube facing of each tunnel tile (0x007D2B08, a function-local static of the original).
extern int unknown_tube_facings_7d2b08[];

void CellClass::Recalc_Attributes(int cell_height)
{
    if (this == &unknown_dummy_cell_a6f9f8) {
        return;
    }

    UnknownMapElem4 *zone = &Map.CellZones[Map.Get_Cell_Zone_Index(MapCoords)];
    char *subzone = Map.CellSubzones + Map.Get_Cell_Zone_Index(MapCoords) * 10;

    int overlay = OverlayIndex;

    if (overlay != -1) {
        OverlayTypeClass *otype = OverlayTypes[overlay];
        unknown_ec = otype->Land;
        if (unknown_ec == 4 || unknown_ec == 9 || otype->NoUseTileLandType) {
            if (IsoTileIndex != 0xFFFF && IsoTileIndex < IsoTileTypes.Count()) {
                IsometricTileTypeClass *itype = IsoTileTypes[IsoTileIndex];
                unknown_11c = itype->Ramp_Type((unsigned char)unknown_11a);
            }
            if (unknown_11c != 0 && otype->Tiberium) {
                OverlayIndex = -1;
                OverlayData = 0;
            }
            if (Rules->CliffBackImpassability != 0) {
                if (unknown_11b + 4 <= Map[MapCoords + Cell(+0, -1)].unknown_11b
                || (unknown_11b + 4 <= Map[MapCoords + Cell(-1, +0)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(+2, +2)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(+1, +1)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(-1, +1)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(+1, -1)].unknown_11b)) {
                    if (Rules->CliffBackImpassability == 2) {
                        unknown_ec = 3;
                    }
                }
            }
            Fixup_LAT();
            Recalc_Passability();
            zone->unknown_01 = unknown_11b;
            subzone[8] = unknown_11b;
            zone->unknown_00 = unknown_4c;
            return;
        }
    }

    if (IsoTileIndex >= IsoTileTypes.Count()) {
        IsoTileIndex = 0xFFFF;
    }

    if (IsoTileIndex != 0xFFFF) {
        IsometricTileTypeClass *itype = IsoTileTypes[IsoTileIndex];
        bool valid = itype->Is_Tile_Index_Valid((unsigned char)unknown_11a, false);

        if (unknown_11c != 0 && !valid) {
            valid = itype->Is_Tile_Index_Valid((unsigned char)unknown_11a, true);
        }

        if (!valid) {
            IsoTileIndex = 0xFFFF;
            unknown_11a = 0;
            unknown_ec = 0;
            unknown_11c = 0;
            if (Rules->CliffBackImpassability != 0) {
                if (unknown_11b + 4 <= Map[MapCoords + Cell(+0, -1)].unknown_11b
                || (unknown_11b + 4 <= Map[MapCoords + Cell(-1, +0)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(+2, +2)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(+1, +1)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(-1, +1)].unknown_11b)
                || (unknown_11b + 4 <= Map[MapCoords + Cell(+1, -1)].unknown_11b)) {
                    if (Rules->CliffBackImpassability == 2 && unknown_ec == 0) {
                        unknown_ec = 3;
                    }
                }
            }
            Recalc_Passability();
            zone->unknown_01 = unknown_11b;
            subzone[8] = unknown_11b;
            zone->unknown_00 = unknown_4c;
            return;
        }

        unknown_11c = itype->Ramp_Type((unsigned char)unknown_11a);
        Fixup_LAT();
        if (OverlayIndex != -1) {
            if (OverlayClass::To_Tiberium(OverlayIndex) != -1) {
                if (unknown_11c > 4) {
                    unknown_ec = itype->Land_Type((unsigned char)unknown_11a);
                    OverlayIndex = -1;
                    OverlayData = 0;
                } else if (!OverlayTypes[OverlayIndex]->Land) {
                    unknown_ec = 5;
                }
            } else if (!OverlayTypes[OverlayIndex]->NoUseTileLandType) {
                unknown_ec = itype->Land_Type((unsigned char)unknown_11a);
            }
        } else {
            unknown_ec = itype->Land_Type((unsigned char)unknown_11a);
        }

        if (unknown_ec == 10) {
            if (unknown_116 < 0 || unknown_116 >= unknown_TubeClass_list_008661c8.Count()) {
                int tunnel_dir = -1;

                if (IsoTileIndex >= unknown_tileset_a52ea4 && IsoTileIndex <= unknown_tileset_a52ea4 + 3) {
                    tunnel_dir = IsoTileIndex - unknown_tileset_a52ea4;
                } else if (IsoTileIndex >= unknown_tileset_a6cf50 && IsoTileIndex <= unknown_tileset_a6cf50 + 3) {
                    tunnel_dir = IsoTileIndex - unknown_tileset_a6cf50;
                } else if (IsoTileIndex >= unknown_tileset_a52f04 && IsoTileIndex <= unknown_tileset_a52f04 + 3) {
                    tunnel_dir = IsoTileIndex - unknown_tileset_a52f04;
                } else if (IsoTileIndex >= unknown_tileset_a6cb78 && IsoTileIndex <= unknown_tileset_a6cb78 + 3) {
                    tunnel_dir = IsoTileIndex - unknown_tileset_a6cb78;
                }

                if (tunnel_dir != -1) {
                    new TubeClass(MapCoords, unknown_tube_facings_7d2b08[tunnel_dir]);
                }
            }
        }

        if (cell_height != -1) {
            unknown_11b = cell_height;
        }

        int width;
        int height;
        itype->Get_Tile_Pixel_Dimensions((unsigned char)unknown_11a, width, height);
        unknown_11d = (height - 30) / 15;
        if (!IsAnimAttached && itype->unknown_298 != -1 && itype->AttachesTo == (unsigned char)unknown_11a) {
            CoordStruct anim_coord = CoordStruct(MapCoords, unknown_11b * unknown_geometry_g) + unknown_tactical_83996c->Unknown_006A2BF0(itype->Offset);
            AnimClass *anim = new AnimClass(AnimTypes[itype->unknown_298], anim_coord, 0, -1, 0x1600, 0);
            anim->ZAdjust = itype->ZAdjust;
            anim->unknown_176 = true;
            anim->unknown_177 = true;
            IsAnimAttached = true;
        }

        if (itype->IsShadowCaster) {
            Cell const *list = itype->Shadow_Caster_List();
            if (list) {
                Cell pos;
                while (*list != Cell(0x7FFF, 0x7FFF) && *list != unknown_geometry_k) {
                    pos = *list + MapCoords;
                    Map[pos].IsOvershadowed = true;
                    list++;
                }
            }
        }
    } else {
        if (overlay == -1 || OverlayTypes[overlay]->NoUseTileLandType) {
            unknown_ec = 0;
        } else {
            unknown_ec = OverlayTypes[overlay]->Land;
        }
        unknown_11c = 0;
    }

    if (Rules->CliffBackImpassability != 0) {
        if (unknown_11b + 4 <= Map[MapCoords + Cell(+0, -1)].unknown_11b
        || (unknown_11b + 4 <= Map[MapCoords + Cell(-1, +0)].unknown_11b)
        || (unknown_11b + 4 <= Map[MapCoords + Cell(+2, +2)].unknown_11b)
        || (unknown_11b + 4 <= Map[MapCoords + Cell(+1, +1)].unknown_11b)
        || (unknown_11b + 4 <= Map[MapCoords + Cell(-1, +1)].unknown_11b)
        || (unknown_11b + 4 <= Map[MapCoords + Cell(+1, -1)].unknown_11b)) {
            if (Rules->CliffBackImpassability == 2) {
                if (unknown_ec == 0 || unknown_ec == 2 || unknown_ec == 6 || unknown_ec == 8) {
                    unknown_ec = 3;
                }
            }
        }
    }

    Recalc_Passability();
    zone->unknown_01 = unknown_11b;
    zone->unknown_00 = unknown_4c;
    subzone[8] = unknown_11b;
}


// void CellClass::Destroy_Bridge() (0x00473000): unless the map is being
// edited, destroys the cell's occupants, drops the bridge occupants, queues the
// cell for the bridge pass (Map +0xD0) and sets off bridge explosions and
// debris (TibSun Destroy_Bridge).

void CellClass::Destroy_Bridge()
{
    if (!Debug_Map) {
        ObjectClass *occupier = OccupierPtr;
        while (occupier != 0) {
            ObjectClass *next = occupier->unknown_30;
            occupier->Take_Damage(&occupier->Strength, 0, Rules->unknown_c74, 0, true, true);
            occupier = next;
        }

        occupier = AltOccupierPtr;
        while (occupier != 0) {
            ObjectClass *next = occupier->unknown_30;
            occupier->Fall_From_Height();
            occupier = next;
        }

        Map.unknown_d0.Add(MapCoords);

        if (Rules->BridgeExplosions.Count() > 0 && Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10 < 0.95) {
            CoordStruct coord;
            coord.X = MapCoords.X * 256 + 128;
            coord.Y = MapCoords.Y * 256 + 128;
            coord.Z = unknown_11b * unknown_geometry_g + unknown_geometry_j;
            coord.X += (Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10 - 0.5) * 50.0;
            coord.Y += (Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10 - 0.5) * 50.0;

            if (Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10 < 0.5) {
                new AnimClass(Rules->MetallicDebris[Random_Pick(0, Rules->MetallicDebris.Count() - 1)], coord, 0, 1, 0x600, 0);
            }

            new AnimClass(Rules->BridgeExplosions[Random_Pick(0, Rules->BridgeExplosions.Count() - 1)], coord, Random_Pick(1, 5), 1, 0x600, 0);
        }
    }
}


// void CellClass::Set_Under_Bridge(unsigned facing, bool state) (0x004732D0):
// marks the cells one high-bridge deck piece covers (TibSun Set_Under_Bridge).

void CellClass::Set_Under_Bridge(unsigned facing, bool state)
{
    if (facing == 0) {
        OverlayData = 0;
    } else {
        OverlayData = 9;
    }

    IsBridgeDeck = state;
    IsUnderBridge = state;
    IsBridgeTraversable = state;
    IsBridgeSurface = state;
    IsOvershadowed = state;
    WasUnderBridge = !state;
    IsBridgeEastWest = (facing == 0) ? true : false;
    if (state) {
        OverlayData = (facing != 0) ? 9 : 0;
    } else {
        OverlayData = 0;
        Destroy_Bridge();
    }
    Map.Radar_Background(MapCoords);

    CellClass *across1 = &Adjacent_Cell(facing);
    across1->unknown_2c = state ? this : 0;
    across1->IsUnderBridge = state;
    across1->IsBridgeTraversable = state;
    across1->IsBridgeSurface = state;
    across1->IsOvershadowed = state;
    across1->WasUnderBridge = !state;
    across1->IsBridgeEastWest = (facing == 0) ? true : false;
    if (state) {
        across1->OverlayData = (facing != 0) ? 9 : 0;
    } else {
        across1->OverlayData = 0;
        across1->Destroy_Bridge();
    }
    Map.Radar_Background(across1->MapCoords);

    CellClass *across2 = &across1->Adjacent_Cell(facing);
    across2->unknown_2c = state ? this : 0;
    across2->IsUnderBridge = state;
    across2->IsBridgeTraversable = false;
    across2->IsBridgeSurface = state;
    across2->IsOvershadowed = state;
    across2->WasUnderBridge = !state;
    across2->IsBridgeEastWest = (facing == 0) ? true : false;
    if (state) {
        across2->OverlayData = (facing != 0) ? 9 : 0;
    } else {
        across2->OverlayData = 0;
        across2->Destroy_Bridge();
    }
    Map.Radar_Background(across2->MapCoords);

    CellClass *across3 = &across2->Adjacent_Cell(facing);
    across3->IsBridgeSurface = state;

    CellClass *back = (CellClass *)&((CellClass const *)this)->Adjacent_Cell((facing - 4) & 7);
    back->unknown_2c = state ? this : 0;
    back->IsUnderBridge = state;
    back->IsBridgeTraversable = state;
    back->WasUnderBridge = !state;
    back->IsBridgeSurface = false;
    back->IsOvershadowed = state;
    back->IsBridgeEastWest = (facing == 0) ? true : false;
    if (state) {
        back->OverlayData = (facing != 0) ? 9 : 0;
    } else {
        back->OverlayData = 0;
        back->Destroy_Bridge();
    }
    Map.Radar_Background(back->MapCoords);

    if (facing == 6) {
        CellClass *back2 = &back->Adjacent_Cell(2);
        back2->unknown_2c = state ? this : 0;
        back2->IsOvershadowed = state;
    }
}


// void CellClass::Set_Under_Rail_Bridge(unsigned facing, bool state) (0x00473700):
// marks the cells one train-bridge deck piece covers (TibSun Set_Under_Rail_Bridge;
// the same body as Set_Under_Bridge 0x004732D0).

void CellClass::Set_Under_Rail_Bridge(unsigned facing, bool state)
{
    if (facing == 0) {
        OverlayData = 0;
    } else {
        OverlayData = 9;
    }

    IsBridgeDeck = state;
    IsUnderBridge = state;
    IsBridgeTraversable = state;
    IsBridgeSurface = state;
    IsOvershadowed = state;
    WasUnderBridge = !state;
    IsBridgeEastWest = (facing == 0) ? true : false;
    if (state) {
        OverlayData = (facing != 0) ? 9 : 0;
    } else {
        OverlayData = 0;
        Destroy_Bridge();
    }
    Map.Radar_Background(MapCoords);

    CellClass *across1 = &Adjacent_Cell(facing);
    across1->unknown_2c = state ? this : 0;
    across1->IsUnderBridge = state;
    across1->IsBridgeTraversable = state;
    across1->IsBridgeSurface = state;
    across1->IsOvershadowed = state;
    across1->WasUnderBridge = !state;
    across1->IsBridgeEastWest = (facing == 0) ? true : false;
    if (state) {
        across1->OverlayData = (facing != 0) ? 9 : 0;
    } else {
        across1->OverlayData = 0;
        across1->Destroy_Bridge();
    }
    Map.Radar_Background(across1->MapCoords);

    CellClass *across2 = &across1->Adjacent_Cell(facing);
    across2->unknown_2c = state ? this : 0;
    across2->IsUnderBridge = state;
    across2->IsBridgeTraversable = false;
    across2->IsBridgeSurface = state;
    across2->IsOvershadowed = state;
    across2->WasUnderBridge = !state;
    across2->IsBridgeEastWest = (facing == 0) ? true : false;
    if (state) {
        across2->OverlayData = (facing != 0) ? 9 : 0;
    } else {
        across2->OverlayData = 0;
        across2->Destroy_Bridge();
    }
    Map.Radar_Background(across2->MapCoords);

    CellClass *across3 = &across2->Adjacent_Cell(facing);
    across3->IsBridgeSurface = state;

    CellClass *back = (CellClass *)&((CellClass const *)this)->Adjacent_Cell((facing - 4) & 7);
    back->unknown_2c = state ? this : 0;
    back->IsUnderBridge = state;
    back->IsBridgeTraversable = state;
    back->WasUnderBridge = !state;
    back->IsBridgeSurface = false;
    back->IsOvershadowed = state;
    back->IsBridgeEastWest = (facing == 0) ? true : false;
    if (state) {
        back->OverlayData = (facing != 0) ? 9 : 0;
    } else {
        back->OverlayData = 0;
        back->Destroy_Bridge();
    }
    Map.Radar_Background(back->MapCoords);

    if (facing == 6) {
        CellClass *back2 = &back->Adjacent_Cell(2);
        back2->unknown_2c = state ? this : 0;
        back2->IsOvershadowed = state;
    }
}


// void CellClass::Occupy_Down(ObjectClass *object, bool alt) (0x00473B30): TibSun's Occupy_Down: links
// object into the occupier list (alt: the alternate list), a building at the end of the chain, reveals it
// and marks the cell's occupy bit.

void CellClass::Occupy_Down(ObjectClass *object, bool alt)
{
    ObjectClass *optr = alt ? AltOccupierPtr : OccupierPtr;

    if (object == 0) {
        return;
    }

    if (object->Kind_Of() == 6 && optr != 0) {
        while (optr->unknown_30 != 0) {
            optr = optr->unknown_30;
        }
        optr->unknown_30 = object;
        object->unknown_30 = 0;
    } else if (alt) {
        if (optr == 0 || optr->unknown_30 != object) {
            object->unknown_30 = optr;
            AltOccupierPtr = object;
        }
    } else {
        if (optr == 0 || optr->unknown_30 != object) {
            object->unknown_30 = optr;
            OccupierPtr = object;
        }
    }

    if (!Map.Is_Shrouded(unknown_475d60()) && !Map.Is_Fogged(unknown_475d60()) || unknown_game_mode_a3d298 != 0) {
        object->Revealed(unknown_player_a35db4);
    }

    if (object->Kind_Of() != 0x0F && object->unknown_slot47()) {
        if (object->Kind_Of() == 6) {
            CoordStruct center(MapCoords.X * 256 + 128, MapCoords.Y * 256 + 128, 0);
            object->Set_Occupy_Bit(center);
        } else {
            CoordStruct coord = object->Coord;
            object->Set_Occupy_Bit(coord);
        }
    }
}


// void CellClass::Occupy_Up(ObjectClass *object, bool alt) (0x00473D20):
// unlinks object from the occupier list (alt: the alternate list) and then,
// unless it is infantry (kind 0x0F), calls its slot 59 with the cell's centre
// (buildings) or its own coordinate when its slot 47 agrees.

void CellClass::Occupy_Up(ObjectClass *object, bool alt)
{
    if (object != 0) {
        ObjectClass *occupier = alt ? AltOccupierPtr : OccupierPtr;
        if (occupier == object) {
            if (alt) {
                AltOccupierPtr = object->unknown_30;
            } else {
                OccupierPtr = object->unknown_30;
            }
            object->unknown_30 = 0;
        } else {
            for (ObjectClass *previous = occupier; previous != 0; previous = previous->unknown_30) {
                if (previous->unknown_30 == object) {
                    previous->unknown_30 = object->unknown_30;
                    object->unknown_30 = 0;
                    break;
                }
            }
        }
        if (object->Kind_Of() != 0x0F && object->unknown_slot47()) {
            CoordStruct coord;
            if (object->Kind_Of() == 6) {
                CoordStruct center(MapCoords.X * 256 + 128, MapCoords.Y * 256 + 128, 0);
                object->Clear_Occupy_Bit(center);
            } else {
                CoordStruct coord = object->Coord;
                object->Clear_Occupy_Bit(coord);
            }
        }
    }
}


// ObjectClass *CellClass::Cell_Unit(bool alt) const (0x00473E30): the unit in the
// cell (Find_Object kind 0x01).

ObjectClass *CellClass::Cell_Unit(bool alt) const
{
    return Find_Object(0x01, alt);
}


// ObjectClass *CellClass::Cell_Aircraft(bool alt) const (0x00473E80): the aircraft in the
// cell (Find_Object kind 0x02).

ObjectClass *CellClass::Cell_Aircraft(bool alt) const
{
    return Find_Object(0x02, alt);
}


// ObjectClass *CellClass::Cell_Infantry(bool alt) const (0x00473ED0): the infantry in the
// cell (Find_Object kind 0x0F).

ObjectClass *CellClass::Cell_Infantry(bool alt) const
{
    return Find_Object(0x0F, alt);
}


// bool CellClass::Draw_Placement_Cursor(Point2D const &pixel, Rect const &rect, bool drawtrans) (0x00473F20):
// draws the placement cursor over a cell the pending building or the second cursor covers, valid or blocked to
// suit, and answers whether the building may stand here (TibSun CellClass::Draw_Placement_Cursor).

bool CellClass::Draw_Placement_Cursor(Point2D const &pixel, Rect const &rect, bool drawtrans)
{
    Point2D point = Point2D(30, -1);

    bool ok = false;
    if ((unknown_12c & 2) != 0 || (unknown_12c & 4) != 0) {
        ObjectTypeClass const *pending = (unknown_12c & 2) ? (ObjectTypeClass const *)Map.unknown_11a4 : (ObjectTypeClass const *)Map.unknown_1190;
        int loco = -1;
        if (pending != 0 && pending->Kind_Of() == 7) {
            loco = ((BuildingTypeClass const *)pending)->SpeedType;
        }

        int lift = 15 * unknown_11b;
        point -= Point2D(0, lift);
        int height_offset = -2 - lift;
        if (unknown_11c != 0) {
            height_offset -= 10;
        }

        if ((unknown_12c & 2) != 0 && ((ObjectTypeClass const *)Map.unknown_11a4)->Kind_Of() == 7) {
            CoordStruct coord = CoordStruct(MapCoords, 0);
            coord.Z = Map.Get_Height_GL(coord);

            if (Map.Is_Shrouded(coord)) {
                if (Map.unknown_117c && Map.unknown_11a4 != 0 && ((ObjectTypeClass const *)Map.unknown_11a4)->Kind_Of() == 7 && ((BuildingTypeClass *)Map.unknown_11a4)->unknown_bf0 != 0) {
                    ok = true;
                } else {
                    ok = false;
                }
            } else if (((BuildingTypeClass *)Map.unknown_11a4)->PowersUpBuilding[0] != '\0') {
                BuildingClass *building = (BuildingClass *)Cell_Object(Point2D(0, 0), false);
                if (building != 0 && building->Kind_Of() == 6) {
                    BuildingTypeClass *ptype = (BuildingTypeClass *)Map.unknown_11a4;
                    if (building->unknown_44d900(ptype, unknown_player_a35db4)) {
                        ok = true;
                    }
                }
            } else {
                if (Map.unknown_117c && Map.unknown_11a4 != 0 && ((ObjectTypeClass const *)Map.unknown_11a4)->Kind_Of() == 7
                        && Is_Clear_To_Build(loco, static_cast<BuildingTypeClass const *>(Map.unknown_11a4), unknown_player_a35db4)
                        && !(((BuildingTypeClass *)Map.unknown_11a4)->unknown_bf0 != 0 && unknown_ec == 1)) {
                    ok = true;
                }
            }
        }

        int flags = drawtrans ? 0x10600 : 0x20600;
        if ((unknown_12c & 4) != 0) {
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)DisplayClass::TransIconset, 1, point + pixel, rect,
                flags, 0, height_offset, 0, 1000, 0, 0, Point2D(0, 0));
        } else if (ok) {
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)DisplayClass::TransIconset, 0, point + pixel, rect,
                flags, 0, height_offset, 0, 1000, 0, 0, Point2D(0, 0));
        } else {
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)DisplayClass::TransIconset, unknown_11c + 2, point + pixel, rect,
                flags, 0, height_offset, 0, 1000, 0, 0, Point2D(0, 0));
        }
    }
    return ok;
}


// void CellClass::Draw_Fog_Shape(Point2D const &point, Rect const &clip, int frame) (0x00474510):
// TibSun's Draw_Fog_Shape: blends FOG.SHP frame `frame` (values below 0x80) into the
// depth buffer at point, clipped to clip; 0x7F entries take the shape value, others
// add value - 0x7F, floored at 0. The ring-wrapping loop runs only when the block can
// reach the buffer end.
void CellClass::Draw_Fog_Shape(Point2D const &point, Rect const &clip, int frame)
{
    static bool shapes_loaded = false;
    static ShapeFileStruct const *shapes = 0;

    if (!shapes_loaded) {
        shapes_loaded = true;
        shapes = (ShapeFileStruct const *)MixFileClass::Retrieve("FOG.SHP");
    }

    Rect shaperect = shapes->Get_Frame_Rect(frame);

    if (point.Y + shaperect.Y + shaperect.Height < clip.Y + 1) return;
    if (point.Y >= clip.Height + clip.Y) return;
    if (point.X + shaperect.X + shaperect.Width < clip.X + 1) return;
    if (point.X >= clip.X + clip.Width) return;

    int top = clip.Y > point.Y + shaperect.Y ? clip.Y : point.Y + shaperect.Y;
    int src_y = top - point.Y - shaperect.Y;
    int bottom = clip.Y + clip.Height < point.Y + shaperect.Y + shaperect.Height ? clip.Y + clip.Height : point.Y + shaperect.Y + shaperect.Height;

    int left = clip.X > point.X + shaperect.X ? clip.X : point.X + shaperect.X;
    int src_x = left - point.X - shaperect.X;
    int right = clip.X + clip.Width < point.X + shaperect.X + shaperect.Width ? clip.X + clip.Width : point.X + shaperect.X + shaperect.Width;

    int shape_skip = point.X - right + shaperect.X + shaperect.Width + src_x;
    int depth_skip = left - right + AlphaBuffer->Width;

    unsigned char const *shapeptr = (unsigned char const *)shapes->Get_Frame_Data(frame);
    unsigned short *depth = AlphaBuffer->Get_Buffer_Offset(Point2D(left, top - unknown_rect_839630.Y));

    shapeptr += src_x + src_y * shaperect.Width;
    if (&depth[right - left + (bottom - top) * AlphaBuffer->Width + 2] >= (unsigned short *)AlphaBuffer->BufferTail) {
        for (int i = top; i < bottom; i++) {
            for (int j = left; j < right; j++) {
                unsigned char pixel = *shapeptr++;
                if (pixel <= 0x7F) {
                    if (*depth == 127) {
                        *depth = pixel;
                    } else {
                        *depth = ((unsigned short)*depth + pixel - 127 <= 0) ? 0 : (unsigned short)*depth + pixel - 127;
                    }
                }
                depth++;
                depth = AlphaBuffer->Wrap_Pointer(depth);
            }
            shapeptr += shape_skip;
            depth += depth_skip;
            depth = AlphaBuffer->Wrap_Pointer(depth);
        }
    } else {
        for (int i = top; i < bottom; i++) {
            for (int j = left; j < right; j++) {
                unsigned char pixel = *shapeptr++;
                if (pixel <= 0x7F) {
                    if (*depth == 127) {
                        *depth = pixel;
                    } else {
                        *depth = ((unsigned short)*depth + pixel - 127 <= 0) ? 0 : (unsigned short)*depth + pixel - 127;
                    }
                }
                ++depth;
            }
            shapeptr += shape_skip;
            depth += depth_skip;
        }
    }
}


// void CellClass::Draw_Overlay_Shadow(Point2D const &point, Rect const &clip) (0x00474830):
// Westwood's Draw_Overlay_Shadow: the overlay's shadow frame (the second half of
// its frames; frame 0 for a crate), or a rubble building's shadow, through the
// cell's drawer.
void CellClass::Draw_Overlay_Shadow(Point2D const &xpoint, Rect const &clip)
{
    int yoffset = unknown_11b * 15;
    OverlayTypeClass *overlay = OverlayTypes[OverlayIndex];
    ShapeFileStruct *shape = (ShapeFileStruct *)overlay->Get_Image_Data();
    Point2D point = xpoint + unknown_475440();
    point -= Point2D(clip.X, clip.Y);
    if (unknown_140 & 0x80) {
        if (OverlayData >= 9 && OverlayData <= 17) {
            point.X -= 15;
            point.Y += 7;
        }
    }
    if (unknown_34 == 0) {
        Init_Drawer(0, 0x10000, 0, 1000, 1000, 1000);
    }
    unsigned char frame = OverlayData;
    if (overlay->Crate) {
        frame = 0;
    } else if (overlay->IsRubble) {
        if (unknown_40 != 0) {
            ShapeFileStruct *image;
            int rubble;
            if (((BuildingTypeClass *)unknown_40)->unknown_func_458e50((void **)&image, &rubble)) {
                CC_Draw_Shape(unknown_surface_83995c, unknown_34, image, rubble, point, clip, 0x4601, 0, -2 - yoffset, 0, 1000, 0, 0, Point2D(0, 0));
            }
        }
        return;
    }
    CC_Draw_Shape(unknown_surface_83995c, unknown_34, shape, frame + shape->FrameCount / 2, point, clip, 0x4601, 0, -2 - yoffset, 0, 1000, 0, 0, Point2D(0, 0));
}


// void CellClass::Draw_Overlay(Point2D const &xpoint, Rect const &clip) (0x004749C0):
// Westwood's Draw_Overlay: draws the cell's overlay (bridge decks once per
// frame and view, tiberium through its growth image, walls in the player's
// colours, veins, rubble buildings and ordinary clutter).

extern unsigned char unknown_redraws_833634;
extern int unknown_bridge_variation_7d2b18[16];
extern void *unknown_slope_shapes_a52eb0[];
extern Rect unknown_rect_839630;

void CellClass::Draw_Overlay(Point2D const &xpoint, Rect const &clip)
{
    if (OverlayIndex == 0xA7 || OverlayIndex == 0xB2) {
        return;
    }
    OverlayTypeClass *otype = OverlayTypes[OverlayIndex];
    Point2D point = unknown_475440() + xpoint;
    int yoffset = 15 * (unknown_11b + (IsBridgeDeck ? 4 : 0));
    point -= clip.Top_Left();
    if (unknown_34 == 0) {
        Init_Drawer(0, 0x10000, 0, 1000, 1000, 1000);
    }
    ShapeFileStruct *shape = (ShapeFileStruct *)otype->Get_Image_Data();
    if (IsBridgeDeck) {
        if (unknown_64 != Frame || unknown_118 != unknown_redraws_833634
            || unknown_68 != clip) {
            int shapenum = OverlayData;
            if (shapenum == 0 || shapenum == 9) {
                shapenum += unknown_bridge_variation_7d2b18[(MapCoords.X & 3) | (4 * (MapCoords.Y & 3))];
            }
            CC_Draw_Shape(unknown_surface_83995c, unknown_34, shape, shapenum, point, clip, 0x4E00, 0, -2 - yoffset, 0, unknown_10e, 0, 0, Point2D(0, 0));
            unknown_64 = Frame;
            unknown_68 = unknown_rect_839630;
            unknown_118 = unknown_redraws_833634;
        }
        return;
    }
    if (otype->Tiberium) {
        int tib = Tiberium_Type_Here();
        if (tib == -1) {
            return;
        }
        TiberiumClass *tiberium = unknown_TiberiumClass_list_00ac0b30[tib];
        OverlayTypeClass *tib_overlay;
        if (unknown_11c != 0) {
            tib_overlay = OverlayTypes[tiberium->Image->ArrayIndex + tiberium->NumFrames + tiberium->NumImages / 4 * (unknown_11c - 1) + MapCoords.X * MapCoords.Y % (tiberium->NumImages / 4)];
        } else {
            tib_overlay = OverlayTypes[tiberium->Image->ArrayIndex + MapCoords.X * MapCoords.Y % tiberium->NumFrames];
        }
        ShapeFileStruct *tibshape = (ShapeFileStruct *)tib_overlay->Get_Image_Data();
        if (unknown_11c != 0) {
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323b8, tibshape, OverlayData, point, clip, 0x4E00, 0, -2 - yoffset, 0, 1000, (ShapeFileStruct const *)unknown_slope_shapes_a52eb0[unknown_11c - 1], 0, Point2D(0, 0));
        } else {
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323b8, tibshape, OverlayData, point, clip, 0x4E00, 0, -2 - yoffset, 0, 1000, 0, 0, Point2D(0, 0));
        }
    } else if (otype->Wall) {
        CC_Draw_Shape(unknown_surface_83995c, ColorSchemes[unknown_player_a35db4->ColorSchemeIndex]->Converter, shape, OverlayData, point, clip, 0x4E00, 0, -2 - yoffset, 2, unknown_10a, 0, 0, Point2D(0, 0));
    } else if (otype->ArrayIndex == 0x7E) {
        if (unknown_11c != 0) {
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323b8, shape, OverlayData, point, clip, 0x4E00, 0, -2 - yoffset, 0, unknown_10c, (ShapeFileStruct const *)unknown_slope_shapes_a52eb0[unknown_11c - 1], 0, Point2D(30, -2));
        } else {
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323b8, shape, OverlayData, point, clip, 0x4E00, 0, -2 - yoffset, 0, unknown_10c, 0, 0, Point2D(0, 0));
        }
    } else {
        bool flat = otype->DrawFlat;
        int offsety = flat ? 0 : -15;
        if (otype->IsARock) {
            offsety = 0;
        }
        if (otype->Crate) {
            CC_Draw_Shape(unknown_surface_83995c, unknown_34, shape, 0, point, clip, 0x4E00, 0, offsety - yoffset - 2, flat ? 0 : 2, unknown_10c, 0, 0, Point2D(0, 0));
        } else if (otype->IsRubble) {
            if (unknown_40 != 0) {
                ShapeFileStruct *image;
                int frame;
                if (((BuildingTypeClass *)unknown_40)->unknown_func_458e00((void **)&image, &frame)) {
                    CC_Draw_Shape(unknown_surface_83995c, unknown_34, image, frame, point, clip, 0x4E00, 0, offsety - yoffset - 2, flat ? 0 : 2, unknown_10c, 0, 0, Point2D(0, 0));
                }
            }
        } else {
            CC_Draw_Shape(unknown_surface_83995c, unknown_34, shape, OverlayData, point, clip, 0x4E00, 0, offsety - yoffset - 2, flat ? 0 : 2, unknown_10c, 0, 0, Point2D(0, 0));
        }
    }
}


// Rect CellClass::Overlay_Render_Rect() const (0x00474EB0): Westwood's
// Overlay_Render_Rect: the screen rectangle of the overlay image (the ore
// image of the cell's tiberium and ramp, or a rubble building's).

Rect CellClass::Overlay_Render_Rect() const
{
    if (OverlayIndex == -1) {
        return Rect(0, 0, 0, 0);
    }
    int frame = OverlayData;
    OverlayTypeClass *overlay = OverlayTypes[OverlayIndex];
    ShapeFileStruct *image = 0;
    if (overlay->Tiberium) {
        int tib = Tiberium_Type_Here();
        if (tib != -1) {
            TiberiumClass *tiberium = unknown_TiberiumClass_list_00ac0b30[tib];
            if (unknown_11c != 0) {
                image = (ShapeFileStruct *)OverlayTypes[tiberium->Image->ArrayIndex + tiberium->NumFrames + tiberium->NumImages / 4 * (unknown_11c - 1) + MapCoords.X * MapCoords.Y % (tiberium->NumImages / 4)]->Get_Image_Data();
            } else {
                image = (ShapeFileStruct *)OverlayTypes[tiberium->Image->ArrayIndex + MapCoords.X * MapCoords.Y % tiberium->NumFrames]->Get_Image_Data();
            }
        }
    } else if (overlay->IsRubble) {
        image = 0;
        if (unknown_40 != 0) {
            ((BuildingTypeClass *)unknown_40)->unknown_func_458e00((void **)&image, &frame);
        }
    } else {
        image = (ShapeFileStruct *)overlay->Get_Image_Data();
    }
    Point2D point;
    unknown_tactical_83996c->Coord_To_Pixel(Coord_Snap(Cell_Coord(MapCoords)), point);
    point += unknown_475440();
    point.X -= 30;
    if (image == 0) {
        return Rect(0, 0, 0, 0);
    }
    if (overlay->Crate) {
        frame = 0;
    }
    return image->Get_Frame_Rect(frame) + (point - Point2D(image->Get_Width() / 2, image->Get_Height() / 2));
}


// Rect CellClass::Overlay_Shadow_Render_Rect() const (0x00475100): Westwood's
// Overlay_Shadow_Render_Rect: the screen rectangle of the overlay's shadow frame.

Rect CellClass::Overlay_Shadow_Render_Rect() const
{
    if (OverlayIndex == -1) {
        return Rect(0, 0, 0, 0);
    }
    OverlayTypeClass *overlay = OverlayTypes[OverlayIndex];
    ShapeFileStruct *image = (ShapeFileStruct *)overlay->Get_Image_Data();
    if (image == 0) {
        return Rect(0, 0, 0, 0);
    }
    unsigned char frame = OverlayData;
    if (overlay->Crate) {
        frame = 0;
    }
    int shadow = frame + image->FrameCount / 2;
    Point2D point;
    unknown_tactical_83996c->Coord_To_Pixel(Coord_Snap(Cell_Coord(MapCoords)), point);
    point += unknown_475440();
    point.X -= 30;
    if (unknown_140 & 0x80) {
        if (OverlayData >= 9 && OverlayData <= 17) {
            point += Point2D(-15, 7);
        }
    }
    return image->Get_Frame_Rect(shadow) + (point - Point2D(image->Get_Width() / 2, image->Get_Height() / 2));
}


// Rect CellClass::Cell_Render_Rect() const (0x004752B0): Westwood's Cell_Render_Rect: the
// screen rectangle of the cell's tile, taller by its extra image.

Rect CellClass::Cell_Render_Rect() const
{
    Point2D point;
    unknown_tactical_83996c->Coord_To_Pixel(Coord_Snap(Cell_Coord(MapCoords)), point);
    point.X -= 30;
    point.Y += unknown_int_839634 - 15 * unknown_11b;

    Rect rect;
    rect.X = point.X;
    rect.Y = point.Y;
    rect.Width = 60;
    rect.Height = 30;

    if (IsoTileIndex == 0xFFFF) {
        return rect;
    }

    if (IsoTileIndex >= 0 && IsoTileIndex < IsoTileTypes.Count()) {
        IsometricTileTypeClass *itype = IsoTileTypes[IsoTileIndex];
        int subtile = (unsigned char)unknown_11a;
        IsoTileImageStruct const *image = (IsoTileImageStruct const *)itype->Get_Image_Data();
        if (image == 0) {
            return Rect();
        }
        subtile = subtile < image->unknown_00[0] * image->unknown_00[1] - 1 ? subtile : image->unknown_00[0] * image->unknown_00[1] - 1;
        IsoTileCellStruct const *tile = (IsoTileCellStruct const *)image->Tiles[subtile];
        if (tile->IsHasExtraData) {
            int dy = tile->Y - tile->ExtraY;
            dy = dy < 0 ? 0 : dy;
            rect.Y -= dy;
            rect.Height += dy;
        }
        return rect;
    } else {
        Debug_Printf("Invalid tile at (%d, %d)\n", MapCoords.X, MapCoords.Y);
        return rect;
    }
}


// Point2D CellClass::unknown_475440() const (0x00475440)
// Westwood's Overlay_Draw_Offset: the pixel offset of the cell's overlay
// image: the overlay's draw offset, raised on a bridge cell (+0x140 bit 7;
// further for the high bridge overlay frames 9-17) or for overlay 0xEF,
// moved by the tactical Y (0x00839634) less the cell level and by the half
// tile (30, 15). Never inlined by its four callers (474830, 4749C0, 474EB0,
// 475100): this declare-then-assign, `offset = offset + ...` form is over
// VC6's auto-inline size cap.

Point2D CellClass::unknown_475440() const
{
    Point2D offset;
    offset = OverlayClass::Draw_Offset(OverlayIndex);
    if (unknown_140 & 0x80) {
        offset.Y -= 16;
        if (OverlayData >= 9 && OverlayData <= 17) {
            offset.Y -= 15;
        }
    } else if (OverlayIndex == 0xEF) {
        offset.Y -= 15;
    }
    offset.Y += unknown_int_839634 - unknown_11b * 15;
    offset = offset + Point2D(30, 15);
    return offset;
}


// void CellClass::unknown_4754b0(Point2D const &point, Rect const &rect) (0x004754B0)
// Draws the first isometric tile type's image at point, clipped to rect, with the cell's
// +0x11B and +0x10C values (Tactical 0x006A3641).

void CellClass::unknown_4754b0(Point2D const &point, Rect const &rect)
{
    IsoTileTypes[0]->unknown_func_52c230(unknown_LightConvertClass_list_00832398[0], 0, unknown_surface_83995c,
        point.X, point.Y, rect, unknown_11b, unknown_10c, 1, 0, 0, 1, 0, 0);
}


// void CellClass::Draw_Shroud_And_Fog(Point2D const &point, Rect const &clip) (0x00475520; TibSun name)
// Refreshes the cell's shroud frame (+0x120, layer 0) and draws it, then its
// fog frame (+0x121, layer 1), drawn only when the scenario has fog of war
// (+0x000 bit 0x1000) and the player's house does not see everything
// (+0x13D). Frame -2 draws as 15, -1 as 0.

void CellClass::Draw_Shroud_And_Fog(Point2D const &point, Rect const &clip)
{
    unknown_120 = unknown_tactical_83996c->Cell_Shadow(MapCoords, 0);
    int frame = unknown_120;
    if (frame == -2) {
        frame = 15;
    } else if (frame == -1) {
        frame = 0;
    }
    Draw_Shroud_Or_Fog_Shape(point, clip, frame);
    unknown_121 = unknown_tactical_83996c->Cell_Shadow(MapCoords, 1);
    if ((unknown_id_source_a3d290->unknown_000 & 0x1000) && !unknown_player_a35db4->unknown_13d) {
        frame = unknown_121;
        if (frame == -2) {
            frame = 15;
        } else if (frame == -1) {
            frame = 0;
        }
        Draw_Fog_Shape(point, clip, frame);
    }
}


// void CellClass::Draw_Shadow_Cast(Point2D const &point, Rect const &clip) (0x004755D0): TibSun
// Draw_Shadow_Cast: the shadow a shadow casting tile throws, offset by the cell's height.
inline void CellClass::Fetch_Icon(IsometricTileTypeClass *&ittype, int &subtile, int *icon, bool doicon) const
{
    if (IsoTileIndex != 0xFFFF) {
        ittype = IsoTileTypes[IsoTileIndex];
        subtile = (unsigned char)unknown_11a;
        if (ittype->NumTileTypesInSet > 1) {
            if (ittype->Is_Randomized(subtile)) {
                if (doicon) *icon = (unknown_140 >> 13) & 1;
            } else {
                if (doicon) {
                    int count = ittype->NumTileTypesInSet;
                    *icon = Clear_Icon(IsoTileIndex, count);
                }
            }
        }
    } else {
        ittype = IsoTileTypes[unknown_tileset_a52f00];
        subtile = 0;
        if (doicon) {
            int count = ittype->NumTileTypesInSet;
            *icon = Clear_Icon(unknown_tileset_a52f00, count);
        }
    }
}

void CellClass::Draw_Shadow_Cast(Point2D const &point, Rect const &clip)
{
    int subtile;
    IsometricTileTypeClass *isotype;

    Fetch_Icon(isotype, subtile, 0, false);
    if (isotype->IsShadowCaster) {
        Point2D drawpoint;
        drawpoint.X = point.X;
        drawpoint.Y = point.Y - 15 * unknown_11b;
        isotype->Draw_Shadow_Caster(subtile, unknown_surface_83995c, drawpoint, clip, -2 - 15 * (unknown_11b - 4));
    }
}


// void CellClass::Draw_It(Point2D const &point, Rect const &clip, bool objects) (0x00475680):
// TibSun Draw_It: the cell's tile, then any smudge, unless this is the objects pass.
extern int unknown_CellCount_a35f28;     // 0x00A35F28 (TibSun CellCount)
extern int unknown_level_850c40;         // 0x00850C40 (TibSun LEVEL_LEPTON_H)

void CellClass::Draw_It(Point2D const &xpoint, Rect const &clip, bool objects)
{
    if (!objects) {
        if (unknown_34 == 0) {
            Init_Drawer(0, 0x10000, 0, 1000, 1000, 1000);
        }

        IsometricTileTypeClass *ittype = 0;
        int icon = 0;
        int subtile = 0;

        Fetch_Icon(ittype, subtile, &icon, true);

        unknown_CellCount_a35f28++;

        Point2D point;
        point.X = xpoint.X;
        point.Y = xpoint.Y - 15 * unknown_11b;

        if (ittype->Get_Image_Data()) {
            ittype->unknown_func_52c230(unknown_34, subtile, unknown_surface_83995c, point.X, point.Y + unknown_int_839634, clip, unknown_11b, unknown_10c, 1, icon, 0, 0, 0, 0);
        }

        if (SmudgeIndex != -1) {
            Point2D p = point + Point2D(30, unknown_int_839634) - Point2D(clip.X, clip.Y);
            SmudgeTypes[SmudgeIndex]->Draw_It(&p, &clip, SmudgeData, unknown_level_850c40 * unknown_11b, &MapCoords);
        }
    }
}


// bool CellClass::Has_Wall_Or_Gate(int overlay, int facing) const (0x00475840):
// whether a wall of the overlay type joins this cell toward facing: the cell
// holds the same overlay; for no overlay (-1), it holds overlay 2 or 0x1A; for
// overlays 2 and 0 a living GDI gate across facing (GDIGateOne for facings 2
// and 6, GDIGateTwo for 0 and 4) or a wall tower stands here; for 0x1A a Nod
// gate does (hypothesis from the Rules types).

bool CellClass::Has_Wall_Or_Gate(int overlay, int facing) const
{
    if (OverlayIndex == overlay && OverlayIndex != -1) {
        return true;
    }
    if (overlay == -1) {
        if (OverlayIndex == 2 || OverlayIndex == 0x1A) {
            return true;
        }
    } else {
        if (overlay == 2 || overlay == 0) {
            for (ObjectClass *object = OccupierPtr; object != 0; object = object->unknown_30) {
                if (object->Kind_Of() == 6) {
                    BuildingTypeClass *type = ((BuildingClass *)object)->Type;
                    if (object->Strength > 0
                        && ((type == Rules->unknown_77c && (facing == 2 || facing == 6))
                            || (type == Rules->unknown_780 && (facing == 0 || facing == 4))
                            || type == Rules->unknown_78c)) {
                        return true;
                    }
                }
            }
        }
        if (overlay == 0x1A) {
            for (ObjectClass *object = OccupierPtr; object != 0; object = object->unknown_30) {
                if (object->Kind_Of() == 6) {
                    BuildingTypeClass *type = ((BuildingClass *)object)->Type;
                    if (object->Strength > 0
                        && ((type == Rules->unknown_784 && (facing == 2 || facing == 6))
                            || (type == Rules->unknown_788 && (facing == 0 || facing == 4)))) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


// void CellClass::Wall_Update(bool flag) (0x00475960): TibSun Wall_Update. For each of the four
// neighbours and the cell itself (0x007D2B58 offsets, -1 last), computes the overlay redraw area
// (Union of 0x00474EB0 and 0x00475100 moved by the tactical origin; the result is unused; skipped for
// the cell itself when flag is set), redraws the cell (0x0047C080) and, for a wall overlay, rebuilds its
// connection bits from the four neighbours and removes incomplete damaged wall stages, updating the
// zones when the passability changes.

extern int unknown_wall_offsets_7d2b58[5];

// An inline operand is evaluated before a plain call: passing the first rectangle through this identity
// helper gives the original's call order (0x00474EB0 before 0x00475100) at the dead redraw-area site.
inline Rect const &Wall_Rect(Rect const &rect) { return rect; }

void CellClass::Wall_Update(bool flag)
{
    for (int *offset = unknown_wall_offsets_7d2b58; offset < &unknown_wall_offsets_7d2b58[5]; offset++) {
        CellClass &newcell = (CellClass &)((CellClass const *)this)->Adjacent_Cell(*offset);
        if (!flag || *offset != -1) {
            Union(Wall_Rect(newcell.Overlay_Render_Rect()), newcell.Overlay_Shadow_Render_Rect()) - unknown_rect_839630.Top_Left();
        }
        newcell.Register_For_Redraw();

        if (newcell.OverlayIndex != -1 && OverlayTypes[newcell.OverlayIndex]->Wall) {
            int icon = 0;
            unsigned i = 0;
            for (int *dir = unknown_wall_offsets_7d2b58; dir < &unknown_wall_offsets_7d2b58[4]; dir++) {
                if (newcell.Adjacent_Cell(*dir).Has_Wall_Or_Gate(newcell.OverlayIndex, *dir)) {
                    icon |= 1 << i;
                }
                i++;
            }
            newcell.OverlayData = (newcell.OverlayData & 0xF0) | icon;

            bool recalc_around = false;
            int old_passability = newcell.unknown_4c;
            if ((newcell.OverlayIndex == 2 || newcell.OverlayIndex == 0x1A) && (newcell.OverlayData == 0x30 || newcell.OverlayData == 0x20)) {
                newcell.OverlayIndex = -1;
                newcell.OverlayData = 0;
                Unknown_006ECB90(&newcell, true);
                recalc_around = true;
            }
            if (newcell.OverlayIndex == 0 && (newcell.OverlayData == 0x10 || newcell.OverlayData == 0x20)) {
                newcell.OverlayIndex = -1;
                newcell.OverlayData = 0;
                newcell.unknown_50 = -1;
                Unknown_006ECB90(&newcell, true);
                recalc_around = true;
            }
            if (newcell.OverlayIndex == 1 && newcell.OverlayData == 0x20) {
                newcell.OverlayIndex = -1;
                newcell.OverlayData = 0;
                newcell.unknown_50 = -1;
                Unknown_006ECB90(&newcell, true);
                recalc_around = true;
            }
            if (newcell.OverlayIndex == 0x16 && (newcell.OverlayData == 0x10 || newcell.OverlayData == 0x20)) {
                newcell.OverlayIndex = -1;
                newcell.OverlayData = 0;
                newcell.unknown_50 = -1;
                Unknown_006ECB90(&newcell, true);
                recalc_around = true;
            }
            if (newcell.OverlayIndex == 3 && newcell.OverlayData == 0x10) {
                newcell.OverlayIndex = -1;
                newcell.OverlayData = 0;
                newcell.unknown_50 = -1;
                Unknown_006ECB90(&newcell, true);
                recalc_around = true;
            }

            newcell.Recalc_Attributes(-1);

            if (newcell.unknown_4c != old_passability) {
                if (recalc_around) {
                    Map.Update_Cell_Zone(newcell.MapCoords);
                    Map.Unknown_00566340(newcell.MapCoords);
                    for (unsigned dir = 0; dir < 8; dir++) {
                        CellClass &adjacent = Map[Adjacent_Cell(newcell.MapCoords, dir)];
                        adjacent.unknown_122--;
                    }
                } else {
                    Map.Update_Cell_Zone_Constructively(newcell.MapCoords);
                    Map.Unknown_00566340(newcell.MapCoords);
                }
            }
        }
    }
}


// CoordStruct CellClass::unknown_475d60() const (0x00475D60): the cell's
// centre at the height of the cell's middle point (128, 128).

CoordStruct CellClass::unknown_475d60() const
{
    CoordStruct center = Cell_Center(MapCoords);
    return CoordStruct(center.X, center.Y, unknown_4706c0(Point2D(128, 128)));
}


// int CellClass::Reduce_Wall(int damage) (0x00475FE0): TibSun Reduce_Wall.
// Damages the cell's wall overlay (-1: destroy it outright); a wall that
// reaches its last damage stage takes its same-type neighbours down a stage,
// and a destroyed wall loses its overlay and owner, updates the zones, the
// radar and the neighbouring walls, detaches the cell and lowers the
// neighbours' adjacent counts. Answers whether the wall was destroyed.

int CellClass::Reduce_Wall(int damage)
{
    if (OverlayIndex != -1) {
        bool destroyed;
        OverlayTypeClass const *wall = OverlayTypes[OverlayIndex];
        if (wall->Wall) {
            if (damage == -1 || damage >= wall->Strength || unknown_a40774) {
                destroyed = true;
            } else {
                destroyed = Random_Pick(0, wall->Strength) < damage;
            }

            if (destroyed) {
                Rect rect = Union(Overlay_Render_Rect(), Overlay_Shadow_Render_Rect());
                rect.Y -= unknown_rect_839630.Y;
                unknown_tactical_83996c->Register_Dirty_Area(rect, false);
                OverlayData += 0x10;
                if ((OverlayData >> 4) == wall->DamageLevels - 1 && wall->DamageLevels > 2) {
                    for (int dir = 0; dir < 8; dir += 2) {
                        CellClass *adjacent = (CellClass *)&((CellClass const *)this)->Adjacent_Cell(dir);
                        if (adjacent->OverlayIndex != -1 && OverlayTypes[adjacent->OverlayIndex]->Wall && adjacent->OverlayIndex == OverlayIndex && adjacent->OverlayData < 0x10) {
                            adjacent->Reduce_Wall(200);
                        }
                    }
                }
                if (damage == -1
                    || (OverlayData >> 4) >= wall->DamageLevels
                    || ((OverlayData >> 4) == wall->DamageLevels - 1 && (OverlayData & 0x0F) == 0)) {
                    unknown_50 = -1;
                    OverlayIndex = -1;
                    OverlayData = 0;
                    Recalc_Attributes(-1);
                    Map.Update_Cell_Zone(MapCoords);
                    Map.Unknown_00566340(MapCoords);
                    Map.Radar_Background(MapCoords);
                    ((CellClass &)((CellClass const *)this)->Adjacent_Cell(0)).Wall_Update(false);
                    ((CellClass &)((CellClass const *)this)->Adjacent_Cell(6)).Wall_Update(false);
                    ((CellClass &)((CellClass const *)this)->Adjacent_Cell(4)).Wall_Update(false);
                    Adjacent_Cell(2).Wall_Update(false);
                    Unknown_006ECB90(this, true);
                    for (int facing = 0; facing < 8; facing++) {
                        Adjacent_Cell(facing).unknown_122--;
                    }
                    return true;
                }
            }
        }
    }
    return false;
}


// Not exact (RA2_NON_MATCHING): CellClass::unknown_475db0 at 0x00475DB0, best score 76.24, 2026-11-13; differs: 180 instructions in the original, 182 in the draft (shape 98.34)
#ifdef RA2_NON_MATCHING
// int CellClass::unknown_475db0(int levels) (0x00475DB0): Westwood's
// Reduce_Tiberium: takes up to levels stages of the cell's ore and answers how
// many it took. An emptied cell loses its overlay (Recalc_Attributes(-1)), is
// redrawn on the radar, leaves the spread lists and queues its ore-free
// neighbours for spreading; the overlay area is registered for redraw.
#include "shared/veinhole.hpp"

int CellClass::unknown_475db0(int levels)
{
    Rect dirty = Union(Overlay_Render_Rect(), Overlay_Shadow_Render_Rect());
    dirty.Y -= unknown_rect_839630.Y;

    int tibtype = OverlayClass::To_Tiberium(OverlayIndex);
    int reducer = levels;

    if (levels > 0 && tibtype != -1) {
        TiberiumClass *tiberium = unknown_TiberiumClass_list_00ac0b30[tibtype];
        if (OverlayData == 11) {
            tiberium->Add_Growth_Cell(MapCoords);
        }
        if (OverlayData + 1 > levels) {
            OverlayData -= levels;
            reducer = levels;
        } else {
            reducer = OverlayData;
            OverlayIndex = -1;
            OverlayData = 0;
            Recalc_Attributes(-1);
            Map.Radar_Background(MapCoords);
            TiberiumClass::Clear_Spread_Cell(MapCoords);
            for (int facing = 0; facing < 8; facing++) {
                Cell adjacent = Adjacent_Cell(MapCoords, facing);
                if (Map.In_Radar(adjacent) && !tiberium->SpreadMap[unknown_cell_index_428a90(adjacent)]) {
                    tiberium->Add_Spread_Cell(adjacent);
                }
            }
        }
        unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
        return reducer;
    }
    return 0;
}
#endif

// int __fastcall Unknown_004763D0(CoordStruct const &coord) (0x004763D0)
// Westwood's CellClass::Spot_Index: the sub-cell spot of a coordinate. 0 near
// the centre of the cell (closer than 60 leptons to (0x80, 0x80)), otherwise
// 1 plus a bit for each of X and Y past the centre (InfantryClass, DisplayClass
// 0x0049CBF0, JumpjetLocomotionClass). Declared in shared/infantry.hpp.


// The distance between two points (Westwood's Distance(Point2D, Point2D); Closest_Free_Spot
// 0x004764B0 expands Spot_Index with Length 0x0047D060 out of line).
inline int Distance(Point2D const &point1, Point2D const &point2)
{
    return (point1 - point2).Length();
}

int __fastcall Unknown_004763D0(CoordStruct const &coord)
{
    int x = coord.X & 0xff;
    int y = coord.Y & 0xff;
    Point2D rel(x, y);
    if (Distance(rel, Point2D(0x80, 0x80)) < 60) {
        return 0;
    }

    int index = 0;
    if (x > 0x80) {
        index = 1;
    }
    if (y > 0x80) {
        index |= 2;
    }
    if (index == 0) {
        return 0;
    }
    return index + 1;
}


// bool CellClass::Is_Spot_Free(int facing, bool alt) const (0x00476460)
// False for facings 0 and 1; else whether the facing's bit of +0x124 (+0x128
// when alt) is clear. The explicit else keeps the two returns apart (no tail
// merge), as in the original.

bool CellClass::Is_Spot_Free(int facing, bool alt) const
{
    if (facing == 0 || facing == 1) {
        return false;
    }
    if (alt) {
        return (unknown_128 & (1 << facing)) == 0;
    } else {
        return (unknown_124 & (1 << facing)) == 0;
    }
}


// CoordStruct CellClass::Closest_Free_Spot(CoordStruct const &coord, bool any, bool alt) const
// (0x004764B0): Westwood's Closest_Free_Spot. The free sub-cell spot of the cell (alt: on the bridge)
// closest to coord's sub-cell position, or the null coordinate when the cell holds a vehicle, a
// closed gate or a building or no spot is free; any: the nearest spot even if taken.
// The offset of each sub-cell spot from the cell corner (Westwood's StoppingCoordAbs, 0x00850E50).
extern CoordStruct unknown_cell_offsets_00850e50[5];
// The spots nearest each spot, closest first (Westwood's _sequence, a function-local static).
extern unsigned char unknown_spot_sequence_7d2b6c[5][4];
// The orders to try the outer spots in when the centre is taken (Westwood's _alternate).
extern unsigned char unknown_spot_alternate_7d2b80[4][4];


// The cell corner of a coordinate: its sub-cell part dropped (Westwood's Coord_Whole).
inline CoordStruct Coord_Whole(CoordStruct const &coord)
{
    int x = coord.X - (coord.X & 0xFF);
    int y = coord.Y - (coord.Y & 0xFF);
    return CoordStruct(x, y, coord.Z);
}

// The gate building in the cell, if any (Westwood's Get_Gate; inline here).
inline BuildingClass *CellClass::Get_Gate() const
{
    BuildingClass *building = (BuildingClass *)Find_Object(6);
    if (building != 0 && !building->Type->Gate) {
        building = 0;
    }
    return building;
}

CoordStruct CellClass::Closest_Free_Spot(CoordStruct const &xcoord, bool any, bool alt) const
{
    int spot_index = Unknown_004763D0(xcoord);

    CoordStruct coord = Coord_Whole(xcoord);

    bool is_vehicle_occupied;
    if (alt) {
        is_vehicle_occupied = Occupy_128.Vehicle;
    } else {
        is_vehicle_occupied = Occupy_124.Vehicle;
    }

    if (!any) {
        if (is_vehicle_occupied || (Occupy_124.Monolith && (!Get_Gate() || !Get_Gate()->unknown_44d880()))) {
            return unknown_geometry_l;
        }
    }

    if (any || Is_Spot_Free(spot_index, alt)) {
        coord = coord + unknown_cell_offsets_00850e50[spot_index];
        coord.Z = Map.Get_Height_GL(xcoord);
        if (alt) {
            coord.Z += unknown_geometry_j;
        }
        return coord;
    }

    unsigned char *sequence;
    if (spot_index == 0) {
        sequence = &unknown_spot_alternate_7d2b80[Random_Pick(0, 3)][0];
    } else {
        sequence = &unknown_spot_sequence_7d2b6c[spot_index][0];
    }
    for (int index = 0; index < 4; index++) {
        int pos = *sequence++;

        if (Is_Spot_Free(pos, alt)) {
            coord = coord + unknown_cell_offsets_00850e50[pos];
            coord.Z = Map.Get_Height_GL(xcoord);
            if (alt) {
                coord.Z += unknown_geometry_j;
            }
            return coord;
        }
    }
    return unknown_geometry_l;
}


// ObjectClass *CellClass::unknown_476750() const (0x00476750): the building
// in the cell when its type is a gate (BuildingTypeClass::Gate), else null.

ObjectClass *CellClass::unknown_476750() const
{
    BuildingClass *building = (BuildingClass *)Cell_Building();
    if (building != 0 && !building->Type->Gate) {
        building = 0;
    }
    return building;
}


struct ClearIconOffset { int X,Y; };
extern "C" bool Clear_Icon_Init;
extern "C" int Clear_Icon_Large[64];
extern "C" int const Clear_Icon_Small[16];
extern "C" ClearIconOffset const Clear_Icon_Adjacent[8];
// Original tables above are independently inspected original data, not candidate-owned arrays.
static Point2D Clear_Icon_Dimensions(IsometricTileTypeClass const *type)
{
    Point2D size;
    size.X=((IsoTileImageStruct const*)type->Get_Image_Data())->unknown_00[0];
    size.Y=((IsoTileImageStruct const*)type->Get_Image_Data())->unknown_00[1];
    return size;
}
// Choose a tile variation from the cell's wrapped position and the seeded pattern.
int CellClass::Clear_Icon(int type,int count) const
{
    if(!Clear_Icon_Init) {
        Clear_Icon_Init=true;
        memset(Clear_Icon_Large,255,sizeof(Clear_Icon_Large));
        for(int i=0;i<64;++i) {
            int random=-1;
            for(int attempt=0;attempt<64;++attempt) {
                random=unknown_random_839218()&7;
                bool duplicate=false;
                for(int adjacent=0;adjacent<8;++adjacent) {
                    ClearIconOffset const &offset=Clear_Icon_Adjacent[adjacent];
                    int neighbor=i+offset.Y*8+offset.X;
                    if(neighbor<0) neighbor+=64;
                    if(neighbor>=64) neighbor-=64;
                    if(Clear_Icon_Large[neighbor]==random) { duplicate=true;break; }
                }
                if(!duplicate) break;
            }
            Clear_Icon_Large[i]=random;
        }
    }
    Cell cell=Fetch_CellID();
    if(unknown_11a) {
        IsometricTileTypeClass const *tile=IsoTileTypes[type];
        if(!tile->Get_Image_Data()) return 0;
        Point2D dimensions=Clear_Icon_Dimensions(tile);
        cell=cell-Cell((unsigned char)unknown_11a%dimensions.X,(unsigned char)unknown_11a/dimensions.X);
        cell.X/=dimensions.X;
        cell.Y/=dimensions.Y;
    }
    if(count<=4) return Clear_Icon_Small[(cell.X&3)|((cell.Y&3)<<2)];
    return Clear_Icon_Large[(cell.X&7)+(cell.Y&7)*8];
}


// void CellClass::Incoming(void const *threat, bool forced, bool nokidding, bool alt)
// (0x00476920): Westwood's Incoming. Every occupier (alt: the bridge ones) scatters from the threat
// when one of them is elite, when the call insists, when the rules let everything scatter
// (PlayerScatter) or when it is a techno that may scatter (ability 3) or whose house is smart enough
// (IQScatter). threat is a CoordStruct const * (retyping it renames the callers' binding).

void CellClass::Incoming(void const *threat, bool forced, bool nokidding, bool alt)
{
    ObjectClass *object = alt ? AltOccupierPtr : OccupierPtr;
    bool elite = false;
    if (!nokidding) {
        while (object != 0) {
            TechnoClass *techno = object->As_Techno();
            if (techno != 0 && techno->unknown_118.Is_Elite()) {
                elite = true;
                break;
            }
            object = object->unknown_30;
        }
    }

    object = alt ? AltOccupierPtr : OccupierPtr;

    DynamicVectorClass<ObjectClass *> objs;
    while (object != 0) {
        objs.Add(object);
        object = object->unknown_30;
    }

    for (int i = 0; i < objs.Count(); i++) {
        TechnoClass *techno = objs[i]->As_Techno();
        if (elite || nokidding || Rules->unknown_1431
            || (techno != 0 && (techno->unknown_6d7910(3) || techno->House->unknown_194 >= Rules->IQScatter))) {
            objs[i]->Scatter(*(CoordStruct const *)threat, forced, nokidding);
        }
    }
}


// CellClass &CellClass::Adjacent_Cell(unsigned facing) (0x00476AC0): the
// neighbouring map cell toward facing; this cell for facings past 7.

CellClass &CellClass::Adjacent_Cell(unsigned facing)
{
    if (facing >= 8) {
        return *this;
    }
    Cell newcell = Adjacent_Cell(Fetch_CellID(), facing);
    return Map[newcell];
}


// void CellClass::Adjust_Threat(int house, int value) (0x00476B20): passes
// the cell's map block (Map.Unknown_0054E370) and value to
// HouseClass::unknown_func_4e5dc0 of every other house that fails
// Unknown_004F52F0 or is not house's ally.

void CellClass::Adjust_Threat(int house, int value)
{
    Cell cell = MapCoords;
    int block = Map.Unknown_0054E370(cell);
    for (int index = 0; index < Houses.Count(); index++) {
        HouseClass *other = Houses[index];
        if (other != 0 && other->ArrayIndex != house
            && (!other->Unknown_004F52F0() || !other->Is_Ally(house))) {
            other->unknown_func_4e5dc0(block, value);
        }
    }
}


// int CellClass::Tiberium_Adjust(bool pregame) (0x00476BA0)
// Westwood's CellClass::Tiberium_Adjust: an ore overlay takes the stage its
// neighbours of the same tiberium give it (from the table at 0x007D2C10; with
// pregame, a random image of the tiberium first); answers the cell's value.

int CellClass::Tiberium_Adjust(bool pregame)
{
    if (OverlayIndex != -1) {
        int tiberium = OverlayClass::To_Tiberium(OverlayIndex);
        if (tiberium != -1) {
            int count = 0;
            int value = unknown_TiberiumClass_list_00ac0b30[tiberium]->Value;
            if (pregame) {
                TiberiumClass *type = unknown_TiberiumClass_list_00ac0b30[tiberium];
                OverlayIndex = (int)Random_Pick(type->Image, type->Image + type->NumFrames - 1);
            }
            for (int facing = 0; facing < 8; facing++) {
                CellClass &cell = Adjacent_Cell(facing);
                if (OverlayClass::To_Tiberium(cell.OverlayIndex) == tiberium) {
                    count++;
                }
            }
            OverlayData = unknown_tiberium_stage_7d2c10[count % unknown_TiberiumClass_list_00ac0b30[tiberium]->unknown_e4];
            return (OverlayData + 1) * value;
        }
    }
    return 0;
}


// bool CellClass::Goodie_Check(ObjectClass *object) (0x00476CC0): Westwood's Goodie_Check. Picks up
// the crate overlay in this cell for the foot object entering it: the powerup comes from the overlay
// data or the crate shares, is replaced by money when it would have no effect, and then takes effect.


extern Cell unknown_cell_850bd8;

inline int Length3_YZX(CoordStruct const &c)
{
    double y = c.Y;
    double z = c.Z;
    double x = c.X;
    return (int)Fast_Sqrt(y * y + (z * z + x * x));
}

inline int Length3(CoordStruct const &c)
{
    double x = c.X;
    double y = c.Y;
    double z = c.Z;
    return (int)Fast_Sqrt(x * x + y * y + z * z);
}


bool CellClass::Goodie_Check(ObjectClass *crate_object)
{
    FootClass *object = (FootClass *)crate_object;
    if (object != 0 && OverlayIndex != -1 && OverlayTypes[OverlayIndex]->Crate) {
        bool force_mcv = false;
        int force_money = 0;
        int damage;
        CoordStruct coord;


        if (unknown_game_mode_a3d298 != 0 && object->House->Type->MultiplayPassive) {
            return true;
        }

        if (OverlayTypes[OverlayIndex]->CrateTrigger) {
            if (object->unknown_34 != 0) {
                Debug_Printf("Springing trigger on crate at %d,%d\n", MapCoords.X, MapCoords.Y);
                object->unknown_34->Spring(0x31, object, unknown_cell_850bd8, false, 0);
                if (!object->unknown_7d) {
                    return false;
                }
            }
            unknown_id_source_a3d290->unknown_223c = true;
        }

        CrateType powerup;
        int index;
        if (OverlayData >= CRATE_COUNT) {
            int total_shares = 0;
            for (index = CRATE_FIRST; index < CRATE_COUNT; index++) {
                total_shares += CrateShares[index];
            }
            int pick = Random_Pick(1, total_shares);
            int share_count = 0;
            for (index = CRATE_FIRST; index < CRATE_COUNT; index++) {
                share_count += CrateShares[index];
                if (pick <= share_count) break;
            }
            powerup = (CrateType)index;
        } else {
            powerup = (CrateType)OverlayData;
        }

        if (unknown_game_mode_a3d298 == 0) {
            if (OverlayData == 0) {
                force_money = Rules->SoloCrateMoney;
                if (OverlayTypes[OverlayIndex] == Rules->unknown_f4) {
                    powerup = Rules->SilverCrate;
                }
                if (OverlayTypes[OverlayIndex] == Rules->unknown_f0) {
                    powerup = Rules->WoodCrate;
                }
                if (OverlayTypes[OverlayIndex] == Rules->unknown_f8) {
                    powerup = Rules->WaterCrate;
                }
            }
        } else {
            HouseClass *house = object->House;
            UnitTypeClass *mcv = house->Unknown_004EF6F0(Rules->BaseUnit);
            if (house->CurBuildings == 0 && house->Available_Money() > 1500 &&
                    house->unknown_5430.unknown_func_491d10(mcv->ArrayIndex) == 0 && unknown_flag_a3d2b8) {
                powerup = CRATE_UNIT;
                force_mcv = true;
            }

            switch (powerup) {
                case CRATE_UNIT:
                    if (object->House->unknown_228 > 50) powerup = CRATE_MONEY;
                    if (unknown_ec == 2 || unknown_ec == 6) powerup = CRATE_MONEY;
                    break;

                case CRATE_SQUAD:
                    if (object->House->unknown_234 > 100) powerup = CRATE_MONEY;
                    if (unknown_ec == 2 || unknown_ec == 6) powerup = CRATE_MONEY;
                    break;

                case CRATE_ARMOR:
                    if (object->unknown_120 != 1) powerup = CRATE_MONEY;
                    break;

                case CRATE_VETERAN:
                    if (object->AbstractFlags.IsTechno) {
                        if (!object->Techno_Type_Class()->Trainable) {
                            powerup = CRATE_MONEY;
                        } else if (object->AbstractFlags.IsTechno && object->unknown_118.Is_Elite()) {
                            powerup = CRATE_MONEY;
                        }
                    }
                    break;

                case CRATE_SPEED:
                    if (object->unknown_470 != 1 || object->Kind_Of() == 2) powerup = CRATE_MONEY;
                    break;

                case CRATE_FIREPOWER:
                    if (object->unknown_128 != 1 || !object->unknown_slot163()) powerup = CRATE_MONEY;
                    break;

                case CRATE_CLOAK:
                    if (object->unknown_32a) powerup = CRATE_MONEY;
                    break;

            }

            if (unknown_ec == 2 && !CrateWater[powerup]) {
                powerup = CRATE_MONEY;
            }

            if (unknown_game_mode_a3d298 == 4) {
                object->House->unknown_4ab0.Unknown_0070C490(powerup);
            }
        }

        Map.Remove_Crate(Fetch_CellID());

        if (unknown_game_mode_a3d298 != 0 && unknown_flag_a3d2c1) {
            Map.Unknown_0054E410();
        }

        if (powerup == CRATE_SQUAD) {
            powerup = CRATE_MONEY;
        }

        double data = CrateData[powerup];
        bool tospeak = false;
        switch (powerup) {

            case CRATE_MONEY:
crate_money:
                Debug_Printf("Crate at %d,%d contains money\n", MapCoords.X, MapCoords.Y);
                if (!force_money) {
                    force_money = Random_Pick((int)data, (int)data + 900);
                }
                if (!object->House->Is_Player_Control() || unknown_game_mode_a3d298 != 0) {
                    object->House->Refund_Money(force_money);
                } else {
                    unknown_player_a35db4->Refund_Money(force_money);
                }
                if (object->House->Is_Player_Control()) {
                    Voc_Play_At(Rules->CrateMoneySound, Ground_Coord(), 0);
                }
                break;

            case CRATE_GAS: {
                Debug_Printf("Crate at %d,%d contains poison gas\n", MapCoords.X, MapCoords.Y);
                WarheadTypeClass *gas = WarheadTypeClass::From_Name("GAS");
                damage = data;
                Unknown_0047E2C0(Center_Coord(), damage, (TechnoClass *)0, gas, false);
                for (FacingType facing = FACING_N; facing < FACING_COUNT; facing++) {
                    Unknown_0047E2C0(Adjacent_Cell(facing).Center_Coord(), damage, (TechnoClass *)0, gas, false);
                }
                break;
            }

            case CRATE_TIBERIUM: {
                Debug_Printf("Crate at %d,%d contains tiberium\n", MapCoords.X, MapCoords.Y);
                int tibtype = Random_Pick(0, unknown_TiberiumClass_list_00ac0b30.Count() - 1);
                if (tibtype == 1) {
                    tibtype = 0;
                }
                Place_Tiberium(tibtype, true);
                int count = Random_Pick(10, 20);
                while (count--) {
                    CoordStruct scatter = Coord_Scatter(Center_Coord(), Random_Pick(0, 3 * 256), true);
                    Map[scatter].Place_Tiberium(tibtype, true);
                }
                break;
            }

            case CRATE_DARKNESS:
                Debug_Printf("Crate at %d,%d contains 'shroud'\n", MapCoords.X, MapCoords.Y);
                if (object->House->Is_Player_Control()) {
                    Map.Unknown_00559F10();
                }
                break;

            case CRATE_REVEAL:
                Debug_Printf("Crate at %d,%d contains 'reveal'\n", MapCoords.X, MapCoords.Y);
                if (object->House->Is_Player_Control()) {
                    Map.Unknown_0055A120();
                    Voc_Play_At(Rules->CrateRevealSound, Ground_Coord(), 0);
                }
                break;

            case CRATE_UNIT: {
                Debug_Printf("Crate at %d,%d contains a unit\n", MapCoords.X, MapCoords.Y);
                UnitTypeClass *utp = 0;
                if (force_mcv) {
                    utp = object->House->Unknown_004EF6F0(Rules->BaseUnit);
                }
                if (utp == 0 &&
                        (object->House->unknown_5420.unknown_func_491d10(Rules->BuildRefinery[0]->ArrayIndex) > 0 ||
                         object->House->unknown_5420.unknown_func_491d10(Rules->BuildRefinery[1]->ArrayIndex) > 0) &&
                        object->House->unknown_5430.unknown_func_491d10(Rules->unknown_9ec[0]->ArrayIndex) == 0 &&
                        object->House->unknown_5430.unknown_func_491d10(Rules->unknown_9ec[1]->ArrayIndex) == 0) {
                    utp = object->House->Unknown_004EF6F0(Rules->unknown_9ec);
                }
                if (Rules->UnitCrateType != 0) {
                    utp = Rules->UnitCrateType;
                }
                while (utp == 0) {
                    utp = UnitTypes[Random_Pick(0, UnitTypes.Count() - 1)];
                    bool isbase = Rules->BaseUnit.Is_Present(utp);
                    bool human = object->House->Unknown_004F52F0();
                    if (utp->CrateGoodie) {
                        if (!unknown_flag_a3d2b8) {
                            if (!isbase) break;
                        } else if (!isbase || human || force_mcv) {
                            break;
                        }
                    }
                    utp = 0;
                }

                if (utp != 0) {
                    UnitClass *goodie_unit = (UnitClass *)utp->Create_One_Of(object->House);
                    if (goodie_unit != 0) {
                        if (goodie_unit->Unlimbo(&Ground_Coord(), 0)) {
                            if (object->House->Is_Player_Control()) {
                                Voc_Play_At(Rules->CrateUnitSound, Ground_Coord(), 0);
                            }
                            return false;
                        }
                        Cell cell = Map.Nearby_Location(Fetch_CellID(), goodie_unit->Type->SpeedType, -1, 0, false, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
                        if (cell != unknown_cell_850bd8 && goodie_unit->Unlimbo(&UnknownCellCoord(cell), 0)) {
                            if (object->House->Is_Player_Control()) {
                                Voc_Play_At(Rules->CrateUnitSound, Ground_Coord(), 0);
                            }
                            return false;
                        }
                        delete goodie_unit;
                        powerup = CRATE_MONEY;
                        goto crate_money;
                    }
                }
                break;
            }

            case CRATE_EXPLOSION:
                Debug_Printf("Crate at %d,%d contains explosives\n", MapCoords.X, MapCoords.Y);
                damage = data;
                {
                    int d = damage;
                    object->Take_Damage(&d, 0, Rules->unknown_c74, 0, true, 0);
                }
                {
                for (int i = 0; i < 5; i++) {
                    CoordStruct frag_coord = Coord_Scatter(Ground_Coord(), Random_Pick(0, 2 * 256), false);
                    Unknown_0047E2C0(frag_coord, damage, (TechnoClass *)0, Rules->unknown_c74, true);
                    AnimTypeClass *anim = Select_Damage_Anim(damage, Rules->unknown_c74, 0, &frag_coord);
                    new AnimClass(anim, frag_coord, 0, 1, 0x2600, Get_Explosion_Z_0047fcc0(frag_coord));
                    Unknown_0047F600(frag_coord, damage, Rules->unknown_c74, 0, 0);
                }
                }
                break;

            case CRATE_NAPALM: {
                Debug_Printf("Crate at %d,%d contains napalm\n", MapCoords.X, MapCoords.Y);
                coord = (object->Center_Coord() + Ground_Coord()) / 2;
                new AnimClass(AnimTypes[0], coord, 0, 1, 0x600, 0);
                damage = data;
                {
                    int d = damage;
                    object->Take_Damage(&d, 0, Rules->FlameDamage, 0, true, 0);
                }
                Unknown_0047E2C0(coord, damage, (TechnoClass *)0, Rules->FlameDamage, true);
                break;
            }

            case CRATE_CLOAK:
                Debug_Printf("Crate at %d,%d contains cloaking device\n", MapCoords.X, MapCoords.Y);
                {
                for (int i = 0; i < DisplayClass::Layer[2].Count(); i++) {
                    ObjectClass *obj = DisplayClass::Layer[2][i];
                    if (obj != 0 && obj->unknown_74 && obj->Is_Techno() && Length3(Ground_Coord() - obj->Center_Coord()) < Rules->CrateRadius) {
                        ((TechnoClass *)obj)->unknown_32a = true;
                    }
                }
                }
                break;

            case CRATE_VETERAN:
                Debug_Printf("Crate at %d,%d contains veterancy(TM)\n", MapCoords.X, MapCoords.Y);
                {
                for (int i = 0; i < DisplayClass::Layer[2].Count(); i++) {
                    ObjectClass *obj = DisplayClass::Layer[2][i];
                    if (obj != 0 && obj->unknown_74 && obj->Is_Techno() && Length3(Ground_Coord() - obj->Center_Coord()) < Rules->CrateRadius &&
                            ((TechnoClass *)obj)->Techno_Type_Class()->Trainable) {
                        for (int count = 0; count < data; count++) {
                            VeterancyClass *vet = &((TechnoClass *)obj)->unknown_118;
                            if (vet->Is_Veteran()) vet->Set_Elite(true);
                            if (vet->unknown_7133d0()) vet->Set_Veteran(true);
                            if (vet->Is_Dumbass()) vet->unknown_713510(true);
                        }
                    }
                }
                }
                if (object->House->Is_Player_Control()) {
                    Voc_Play_At(Rules->CratePromoteSound, Ground_Coord(), 0);
                }
                break;

            case CRATE_HEAL_BASE:
                Debug_Printf("Crate at %d,%d contains base healing\n", MapCoords.X, MapCoords.Y);
                if (object->House->Is_Player_Control()) {
                    Voc_Play_At(Rules->HealCrateSound, Ground_Coord(), 0);
                }
                {
                for (int i = 0; i < unknown_list_832470.Count(); i++) {
                    ObjectClass *obj = unknown_list_832470[i];
                    if (obj != 0 && object->Is_Techno() && object->House == obj->Owning_House()) {
                        int damage = obj->Strength - obj->Class_Of()->MaxStrength;
                        obj->Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, 1);
                    }
                }
                }
                break;

            case CRATE_ICBM: {
                Debug_Printf("Crate at %d,%d contains ICBM\n", MapCoords.X, MapCoords.Y);
                SuperWeaponTypeClass *super = SuperWeaponTypeClass::From_Action(0x14);
                int sindex = -1;
                {
                for (int i = 0; i < object->House->Supers.Count(); i++) {
                    if (object->House->Supers[i]->Type->Type == 0) {
                        sindex = i;
                        break;
                    }
                }
                }
                if (sindex != -1 && object->House->Supers[super->ArrayIndex]->Grant(true, false, false) && object->unknown_372) {
                    Map.Unknown_0067C530(0x1F, super->ArrayIndex);
                }
                break;
            }

            case CRATE_ARMOR:
                Debug_Printf("Crate at %d,%d contains armor\n", MapCoords.X, MapCoords.Y);
                {
                for (int i = 0; i < DisplayClass::Layer[2].Count(); i++) {
                    ObjectClass *obj = DisplayClass::Layer[2][i];
                    if (obj != 0 && obj->Is_Techno() && Length3(Ground_Coord() - obj->Center_Coord()) < Rules->CrateRadius &&
                            ((TechnoClass *)obj)->unknown_120 == 1) {
                        ((TechnoClass *)obj)->unknown_120 = data * ((TechnoClass *)obj)->unknown_120;
                        if (obj->Owning_House()->Is_Player_Control()) tospeak = true;
                    }
                }
                }
                if (tospeak) Speech_Play_Name("EVA_UnitArmorUpgraded", -1, -1);
                if (object->House->Is_Player_Control()) {
                    Voc_Play_At(Rules->CrateArmourSound, Ground_Coord(), 0);
                }
                break;

            case CRATE_SPEED:
                Debug_Printf("Crate at %d,%d contains speed\n", MapCoords.X, MapCoords.Y);
                {
                for (int i = 0; i < DisplayClass::Layer[2].Count(); i++) {
                    ObjectClass *obj = DisplayClass::Layer[2][i];
                    if (obj != 0 && obj->Is_Foot() && Length3(Ground_Coord() - obj->Center_Coord()) < Rules->CrateRadius &&
                            ((FootClass *)obj)->unknown_470 == 1 && obj->Kind_Of() != 2) {
                        ((FootClass *)obj)->unknown_470 = data * ((FootClass *)obj)->unknown_470;
                        if (((FootClass *)obj)->House->unknown_135) tospeak = true;
                    }
                }
                }
                if (tospeak) Speech_Play_Name("EVA_UnitSpeedUpgraded", -1, -1);
                if (object->House->Is_Player_Control()) {
                    Voc_Play_At(Rules->CrateSpeedSound, Ground_Coord(), 0);
                }
                break;

            case CRATE_FIREPOWER:
                Debug_Printf("Crate at %d,%d contains firepower\n", MapCoords.X, MapCoords.Y);
                {
                for (int i = 0; i < DisplayClass::Layer[2].Count(); i++) {
                    ObjectClass *obj = DisplayClass::Layer[2][i];
                    if (obj != 0 && obj->Is_Techno() && Length3_YZX(Ground_Coord() - obj->Center_Coord()) < Rules->CrateRadius &&
                            ((TechnoClass *)obj)->unknown_128 == 1) {
                        ((TechnoClass *)obj)->unknown_128 = data * ((TechnoClass *)obj)->unknown_128;
                        if (obj->Owning_House()->Is_Player_Control()) tospeak = true;
                    }
                }
                }
                if (tospeak) Speech_Play_Name("EVA_UnitFirePowerUpgraded", -1, -1);
                if (object->House->Is_Player_Control()) {
                    Voc_Play_At(Rules->CrateFireSound, Ground_Coord(), 0);
                }
                break;

            case CRATE_INVULN:
                break;

            case CRATE_ION_STORM:
                break;

            default:
                break;
        }

        if (CrateAnims[powerup] != -1) {
            CoordStruct coord = Ground_Coord() + CoordStruct(0, 0, 200);
            new AnimClass(AnimTypes[CrateAnims[powerup]], coord, 0, 1, 0x600, 0);
        }
    }

    return true;
}


// bool CellClass::unknown_4786d0(int value) (0x004786D0): when flag 0x10 of
// +0x140 is clear and Is_Clear_To_Move(1, false, false, -1, 0, -1, 1) agrees,
// sets the flag, stores value at +0x50 and answers true.

bool CellClass::unknown_4786d0(int value)
{
    if ((unknown_140 & 0x10) == 0 && Is_Clear_To_Move(1, 0, 0, -1, 0, -1, 1)) {
        unknown_140 |= 0x10;
        unknown_50 = value;
        return true;
    }
    return false;
}


// bool CellClass::unknown_478720() (0x00478720): undoes unknown_4786d0: when
// flag 0x10 of +0x140 is set, clears it, sets +0x50 to -1 and answers true.

bool CellClass::unknown_478720()
{
    if (unknown_140 & 0x10) {
        unknown_50 = -1;
        unknown_140 &= ~0x10;
        return true;
    }
    return false;
}


// void CellClass::unknown_478740() (0x00478740): calls ObjectClass slot 61
// on every occupant.

void CellClass::unknown_478740()
{
    for (ObjectClass *object = OccupierPtr; object != 0; object = object->unknown_30) {
        object->Do_Shimmer();
    }
}


// bool CellClass::Is_Clear_To_Move(int speed, bool arg2, bool arg3, int zone,
// int zone_type, int level, bool alt) (0x00478760): whether an object of the
// speed type may enter the cell (hypothesis): the map zone and the level (the
// cell's own, or four above on a bridge) must match when given; the blocked
// direction bits (+0x124, or +0x128 on a bridge; arg2 keeps 0xE0, arg3 0x5F)
// must be clear; a wall overlay refuses all but zone types 2, 3 and 8 (and 1
// and 4 for a wall with +0x1FD) and counts as land type 0; the land type
// must have a speed (a bridge always does). Speed type 4 always may.

bool CellClass::Is_Clear_To_Move(int speed, bool arg2, bool arg3, int zone, int zone_type, int level, bool alt)
{
    if (speed == 4) {
        return true;
    }
    if (zone != -1 && zone != Map.Zone(MapCoords, zone_type, alt)) {
        return false;
    }
    if (level != -1) {
        if (level != unknown_11b) {
            if (!(unknown_140 & 0x100) || level != unknown_11b + 4) {
                return false;
            }
        } else if ((unknown_140 & 0x100) && !alt) {
            return false;
        }
    }
    bool bridge = false;
    int blocked;
    if ((level == -1 || level == unknown_11b + 4) && (unknown_140 & 0x100)) {
        bridge = true;
        blocked = unknown_128;
    } else {
        blocked = unknown_124;
    }
    if (arg2) {
        blocked &= 0xE0;
    }
    if (arg3) {
        blocked &= 0x5F;
    }
    if (blocked != 0) {
        return false;
    }
    int land = unknown_ec;
    if (OverlayIndex != -1) {
        OverlayTypeClass *overlay = OverlayTypes[OverlayIndex];
        if (overlay != 0 && overlay->Wall) {
            if (zone_type != 2 && zone_type != 3 && zone_type != 8
                && ((zone_type != 1 && zone_type != 4) || !overlay->unknown_1fd)) {
                return false;
            }
            land = 0;
        }
    }
    if (Ground[land].Cost[speed] == 0.0f && !bridge) {
        return false;
    }
    return true;
}


// bool CellClass::Is_Bridge_Here() const (0x004788D0)
// TibSun's IsUnderBridge read as a member function (bit 8 of +0x140); Preview_Cell_Color
// 0x004710D0 expands it, TeleportLocomotionClass 0x006DFF00 takes its address.

bool CellClass::Is_Bridge_Here() const
{
    return IsUnderBridge;
}


// bool CellClass::Can_Tiberium_Grow() const (0x004788E0): whether the cell's ore
// may grow (hypothesis): growth is on in the scenario (+0x222A), the overlay is
// ore, +0x11C is clear, the ore is below its last stage and its type's growth
// rate (+0xB0) is not below 0.00001.

bool CellClass::Can_Tiberium_Grow() const
{
    if (!unknown_id_source_a3d290->unknown_222a) {
        return false;
    }
    int tiberium = OverlayClass::To_Tiberium(OverlayIndex);
    if (tiberium == -1) {
        return false;
    }
    TiberiumClass *type = unknown_TiberiumClass_list_00ac0b30[tiberium];
    if (unknown_11c) {
        return false;
    }
    if (OverlayData >= type->unknown_e4 - 1) {
        return false;
    }
    if (type->unknown_b0 < 0.00001) {
        return false;
    }
    return true;
}


// bool CellClass::Can_Tiberium_Spread() const (0x00478950): whether ore may spread
// from the cell (hypothesis): spreading is on in the scenario (bit 0x80 of its
// +0x000), the overlay is ore past half of ... (the overlay data is compared
// with the ore type's index / 2, as the original does), +0x11C is clear, the
// type's spread rate (+0xA0) is not below 0.00001 and nothing occupies the cell.

bool CellClass::Can_Tiberium_Spread() const
{
    if (!(unknown_id_source_a3d290->unknown_000 & 0x80)) {
        return false;
    }
    int tiberium = OverlayClass::To_Tiberium(OverlayIndex);
    if (tiberium == -1) {
        return false;
    }
    if (OverlayData <= tiberium / 2) {
        return false;
    }
    if (unknown_11c) {
        return false;
    }
    if (unknown_TiberiumClass_list_00ac0b30[tiberium]->unknown_a0 < 0.00001) {
        return false;
    }
    if (OccupierPtr != 0) {
        return false;
    }
    return true;
}


// bool CellClass::Grow_Tiberium() (0x004789D0): grows the cell's ore one
// stage through Place_Tiberium when Can_Tiberium_Grow's conditions hold (they
// are tested inline again); false otherwise.

bool CellClass::Grow_Tiberium()
{
    if (unknown_id_source_a3d290->unknown_222a) {
        int tiberium = OverlayClass::To_Tiberium(OverlayIndex);
        if (tiberium != -1) {
            TiberiumClass *type = unknown_TiberiumClass_list_00ac0b30[tiberium];
            if (!unknown_11c && OverlayData < type->unknown_e4 - 1 && !(type->unknown_b0 < 0.00001)) {
                int type_index = OverlayClass::To_Tiberium(OverlayIndex);
                return Place_Tiberium(type_index, 1);
            }
        }
    }
    return false;
}


// bool CellClass::Spread_Tiberium(bool forced) (0x00478A40)
// Westwood's CellClass::Spread_Tiberium: unless forced, only when the ore may
// spread from here (Can_Tiberium_Spread); the cell's tiberium (the first one when
// forced on a cell without) is planted in the first neighbour, from a random
// facing on, where it may germinate (TerrainClass 0x006E40B6, 0x006E984F).

bool CellClass::Spread_Tiberium(bool forced)
{
    if (!forced) {
        if (!Can_Tiberium_Spread()) {
            return false;
        }
    }
    int overlay = OverlayIndex;
    int tiberium = OverlayClass::To_Tiberium(overlay);
    if (!forced && tiberium == -1) {
        return false;
    }
    if (tiberium == -1) {
        tiberium = 0;
    }
    TiberiumClass *type = unknown_TiberiumClass_list_00ac0b30[tiberium];
    int offset = Random_Pick(0, 7);
    int count = 8;
    for (int index = 0; index < count; index++) {
        int facing = (index + offset) & 7;
        CellClass *cell = &Adjacent_Cell(facing);
        if (cell != 0 && cell->Can_Tiberium_Germinate(type)) {
            int amount = 3;
            return cell->Place_Tiberium(tiberium, amount);
        }
    }
    return false;
}


// bool CellClass::Can_Tiberium_Germinate(TiberiumClass *tiberium) (0x00478BA0)
// TibSun name (tiberium unused; Spread_Tiberium 0x00478A40 passes its type,
// other callers 0): the map accepts the cell
// (Map.Unknown_0055A660), it is not flagged 0x500, holds no living visible
// building and no tiberium-spawning terrain object, its land allows building,
// it has no overlay, +0x11C is clear and its tile type allows it (+0x2D6; any
// tile outside the type list does).

bool CellClass::Can_Tiberium_Germinate(TiberiumClass *tiberium)
{
    if (Map.Unknown_0055A660(MapCoords, true) && (unknown_140 & 0x500) == 0) {
        BuildingClass *building = (BuildingClass *)Cell_Building();
        if (building != 0 && building->Strength > 0 && !building->Type->Invisible && !building->Type->InvisibleInGame) {
            return false;
        }
        TerrainClass *terrain = (TerrainClass *)Cell_Terrain();
        if (terrain != 0 && terrain->Class->SpawnsTiberium) {
            return false;
        }
        if (Ground[unknown_ec].Build && OverlayIndex == -1 && !unknown_11c
            && (IsoTileIndex < 0 || IsoTileIndex >= IsoTileTypes.Count() || IsoTileTypes[IsoTileIndex]->unknown_2d6)) {
            return true;
        }
    }
    return false;
}


// CellClass persistence at 0x00478CB0; saved pointers are fixed up after restoring the layout.

// Actual MapClass::Cells subobject, +0x134 from the global Map object.
extern VectorClass<CellClass *> unknown_map_cells_832614;

HRESULT __stdcall CellClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) CellClass(NoInitClass());
        if (FoggedObjects != 0) {
            FoggedObjects = new DynamicVectorClass<FoggedObjectClass *>;
            int count;
            result = stream->Read(&count, sizeof(count), 0);
            if (FAILED(result)) return result;
            for (int index = 0; index < count; index++) {
                FoggedObjectClass *fogged = 0;
                if (FAILED(stream->Read(&fogged, sizeof(fogged), 0))) return S_OK;
                FoggedObjects->Add(fogged);
            }
            for (int index2 = 0; index2 < FoggedObjects->Count(); index2++) {
                Swizzle_Pointer((*FoggedObjects)[index2]);
            }
        }
        unknown_fc = 0;
        int id = MapCoords.X + MapCoords.Y * 512;
        if (unknown_map_cells_832614[id] != 0) {
            delete unknown_map_cells_832614[id];
            unknown_map_cells_832614[id] = 0;
        }
        unknown_map_cells_832614[id] = this;
        Swizzle_Pointer(unknown_2c);
        Swizzle_Pointer(unknown_30);
        Swizzle_Pointer(unknown_3c);
        Swizzle_Pointer(unknown_e0);
        Swizzle_Pointer(OccupierPtr);
        Swizzle_Pointer(AltOccupierPtr);
        Swizzle_Pointer(unknown_f8);
        Swizzle_Pointer(unknown_40);
        if (OverlayIndex == 0xb3 || OverlayIndex == 0xf2) {
            OverlayIndex = -1;
            OverlayData = 0;
        }
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall CellClass::Save(IStream *stream, BOOL clear_dirty)
// (0x00478ED0, slot 6): saves the abstract part, then the fogged-object list
// (when there is one) as a count and its pointers; a failed pointer write
// still reports success.

HRESULT __stdcall CellClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (FAILED(result)) {
        return result;
    }
    if (FoggedObjects != 0) {
        int count = FoggedObjects->Count();
        result = stream->Write(&count, sizeof(count), 0);
        if (FAILED(result)) {
            return result;
        }
        for (int index = 0; index < count; index++) {
            if (FAILED(stream->Write(&(*FoggedObjects)[index], sizeof(FoggedObjectClass *), 0))) {
                break;
            }
        }
    }
    return S_OK;
}


// void CellClass::Recalc_Passability() (0x00478F40): Westwood's Recalc_Passability. Sets unknown_4c
// (Passability: 0 land, 1 crush, 2 blocked, 3 beach, 4 water, 5 partially blocked, 6 impassable,
// 7 outside the local radar) from the overlay, the land and the occupiers.

void CellClass::Recalc_Passability()
{
    if (!Map.Unknown_0055A660(MapCoords, true)) {
        unknown_4c = 7;
        return;
    }

    if (OverlayIndex != -1) {
        OverlayTypeClass const *overlay = OverlayTypes[OverlayIndex];
        if (overlay->unknown_1fd) {
            unknown_4c = 1;
            return;
        }
        if (overlay->Wall) {
            unknown_4c = 2;
            return;
        }
        if (Ground[overlay->Land].Cost[2] == 0) {
            unknown_4c = 6;
            return;
        }
        if (overlay->IsARock) {
            unknown_4c = 6;
            return;
        }
        if (overlay->IsRubble) {
            unknown_4c = 0;
            return;
        }
    }

    if (unknown_ec == 2) {
        unknown_4c = 4;
        return;
    }
    if (unknown_ec == 6) {
        unknown_4c = 3;
        return;
    }

    if (Ground[unknown_ec].Cost[2] <= 0.01) {
        unknown_4c = 6;
        return;
    }

    for (ObjectClass *occupier = OccupierPtr; occupier != 0; occupier = occupier->unknown_30) {
        switch (occupier->Kind_Of()) {
            case 6:
                if (((BuildingClass *)occupier)->Type->FirestormWall) {
                    if (((BuildingClass *)occupier)->House->unknown_142) {
                        unknown_4c = 6;
                        return;
                    }
                } else if (((BuildingClass *)occupier)->Type->LaserFence) {
                    if (((BuildingClass *)occupier)->unknown_4ec != 12 && ((BuildingClass *)occupier)->unknown_4ec != 8) {
                        unknown_4c = 6;
                    }
                }
                break;

            case 0x24:
                if ((unknown_id_source_a3d290->Theater != 1 && ((TerrainClass *)occupier)->Class->TemperateOccupationBits != 7)
                    || (unknown_id_source_a3d290->Theater == 1 && ((TerrainClass *)occupier)->Class->SnowOccupationBits != 7)) {
                    unknown_4c = 5;
                    return;
                }
                unknown_4c = 2;
                return;
        }
    }

    unknown_4c = 0;
}


// void CellClass::Init_Drawer(LightConvertClass *drawer, int intensity, int ambient,
//     int brightness, int tile_brightness, int alt_brightness) (0x004790F0)


inline void CellClass::Inc_Drawer_Ref_Count(LightConvertClass *drawer) const
{
    drawer->unknown_194++;
}

inline void CellClass::Dec_Drawer_Ref_Count(LightConvertClass *drawer) const
{
    if (unknown_flag_a40958) {
        drawer->unknown_194--;
    }
}

void CellClass::Init_Drawer(LightConvertClass *drawer, int intensity, int ambient, int brightness, int tile_brightness, int alt_brightness)
{
    bool got_drawer = false;
    int red;
    int green;
    int blue;

    if (drawer != 0) {
        if (unknown_34 != 0) {
            Dec_Drawer_Ref_Count(unknown_34);
        }
        unknown_34 = drawer;
        Inc_Drawer_Ref_Count(unknown_34);
        red = unknown_34->Red;
        green = unknown_34->Green;
        blue = unknown_34->Blue;
        got_drawer = true;
    } else if (MapCoords != Cell(0, 0) && MapCoords != Cell(-1, -1)) {
        Init_Light(intensity, ambient, brightness, tile_brightness, alt_brightness, red, green, blue);
        if (unknown_34 != 0) {
            int r = red;
            int g = green;
            int b = blue;
            Clamp_Tile_RGB(r, g, b);
            if (unknown_34->Red != r || unknown_34->Green != g || unknown_34->Blue != b) {
                Dec_Drawer_Ref_Count(unknown_34);
                unknown_34 = 0;
            }
        }
        if (unknown_34 == 0) {
            unknown_34 = IsometricTileTypeClass::Find_Or_Make_Drawer(red, green, blue);
            if (unknown_34 != 0) {
                Inc_Drawer_Ref_Count(unknown_34);
            }
        }
        got_drawer = true;
    }

    if (got_drawer) {
        unknown_104 = intensity;
        unknown_108 = ambient;
        unknown_10a = brightness;
        unknown_10c = tile_brightness;
        unknown_10e = alt_brightness;
        unknown_110 = red;
        unknown_112 = green;
        unknown_114 = blue;
        return;
    }

    unknown_34 = IsometricTileTypeClass::Find_Or_Make_Drawer(1000, 1000, 1000);
    if (unknown_34 != 0) {
        Inc_Drawer_Ref_Count(unknown_34);
    }
    unknown_10a = 1000;
    unknown_10c = 1000;
    unknown_10e = 1000;
    unknown_110 = 1000;
    unknown_112 = 1000;
    unknown_114 = 1000;
    unknown_104 = 0x10000;
    unknown_108 = 0;
}


// void CellClass::Pick_Drawer(LightConvertClass *&drawer, int &intensity, int &ambient,
//     int &brightness, int &tile_brightness, int &alt_brightness) const (0x00479310)


void CellClass::Pick_Drawer(LightConvertClass *&drawer, int &intensity, int &ambient, int &brightness, int &tile_brightness, int &alt_brightness) const
{
    intensity = 0x10000;
    ambient = 0;
    brightness = 1000;
    tile_brightness = 1000;
    alt_brightness = 1000;

    int red;
    int green;
    int blue;

    if (MapCoords != Cell(0, 0) && MapCoords != Cell(-1, -1)) {
        Init_Light(intensity, ambient, brightness, tile_brightness, alt_brightness, red, green, blue);

        bool new_drawer = false;
        if (unknown_34 != 0) {
            int r = red;
            int g = green;
            int b = blue;
            Clamp_Tile_RGB(r, g, b);
            if (unknown_34->Red == r && unknown_34->Green == g && unknown_34->Blue == b) {
                drawer = unknown_34;
            } else {
                new_drawer = true;
            }
        } else {
            new_drawer = true;
        }
        if (new_drawer) {
            drawer = IsometricTileTypeClass::Find_Or_Make_Drawer(red, green, blue);
        }
    } else {
        drawer = IsometricTileTypeClass::Find_Or_Make_Drawer(1000, 1000, 1000);
    }
}


// void CellClass::Init_Light(int &intensity, int &ambient, int &brightness, int &tile_brightness,
//     int &alt_brightness, int &red, int &green, int &blue) const (0x00479440): Westwood's
// Init_Light: the scenario ambient and tints plus every active light within reach of the
// cell's centre, the level light for the cell height (ion storm, dominator or normal), the
// tile tints through Adjust_Tile_RGB, limited to 0..2000; full light for the corner cells.

// Scales a cell's tile brightness and tints by the light intensity (0x005383C0; Westwood's
// Adjust_Tile_RGB, LIGHTCALC).
void __fastcall Adjust_Tile_RGB(int &intensity, int &tile_brightness, int &red, int &green, int &blue);

void CellClass::Init_Light(int &intensity, int &ambient, int &brightness, int &tile_brightness, int &alt_brightness, int &red, int &green, int &blue) const
{
    if (MapCoords != Cell(0, 0) && MapCoords != Cell(-1, -1)) {
        brightness = 1000 * unknown_id_source_a3d290->unknown_22ac / 100;
        red = 1000 * unknown_id_source_a3d290->AmbientRed / 100;
        green = 1000 * unknown_id_source_a3d290->AmbientGreen / 100;
        blue = 1000 * unknown_id_source_a3d290->AmbientBlue / 100;
        ambient = 0;

        for (int i = 0; i < unknown_LightSourceClass_list_00a6e818.Count(); i++) {
            LightSourceClass *light = unknown_LightSourceClass_list_00a6e818[i];
            if (light->Activated && unknown_a40b2c >= light->DetailLevel) {
                CoordStruct cc = unknown_475d60();
                unsigned int dx = cc.X - ((CoordStruct)light->Location).X;
                unsigned int visibility = light->Visibility;
                unsigned int dy = cc.Y - ((CoordStruct)light->Location).Y;
                if (dx * dx + dy * dy <= visibility * visibility) {
                    double fdx = (int)(cc.X - ((CoordStruct)light->Location).X);
                    double fdy = (int)(cc.Y - ((CoordStruct)light->Location).Y);
                    unsigned int dist = (int)Fast_Sqrt(fdx * fdx + fdy * fdy);
                    if (dist <= visibility) {
                        int num = (1000 * visibility - 1000 * dist) / visibility;
                        ambient += (num * light->Intensity) / 1000;
                        red += (num * light->Tint.Red) / 1000;
                        green += (num * light->Tint.Green) / 1000;
                        blue += (num * light->Tint.Blue) / 1000;
                    }
                }
            }
        }

        brightness += ambient;
        alt_brightness = brightness;

        if (Unknown_0051FC50()) {
            brightness += unknown_11b * unknown_id_source_a3d290->IonLevel - unknown_id_source_a3d290->IonGround;
            alt_brightness += (unknown_11b + 4) * unknown_id_source_a3d290->IonLevel - unknown_id_source_a3d290->IonGround;
        } else if (Unknown_0051FC60()) {
            brightness += unknown_11b * unknown_id_source_a3d290->unknown_22f4 - unknown_id_source_a3d290->unknown_22f0;
            alt_brightness += (unknown_11b + 4) * unknown_id_source_a3d290->unknown_22f4 - unknown_id_source_a3d290->unknown_22f0;
        } else {
            brightness += unknown_11b * unknown_id_source_a3d290->AmbientLevel - unknown_id_source_a3d290->AmbientGround;
            alt_brightness += (unknown_11b + 4) * unknown_id_source_a3d290->AmbientLevel - unknown_id_source_a3d290->AmbientGround;
        }

        brightness = (brightness < 2000) ? brightness : 2000;
        tile_brightness = brightness;

        Adjust_Tile_RGB(intensity, tile_brightness, red, green, blue);

        alt_brightness = (alt_brightness * intensity) >> 16;
        alt_brightness = (alt_brightness < 2000) ? alt_brightness : 2000;

        brightness = (brightness > 0) ? brightness : 0;
        tile_brightness = (tile_brightness > 0) ? tile_brightness : 0;
        alt_brightness = (alt_brightness > 0) ? alt_brightness : 0;
    } else {
        intensity = 0x10000;
        ambient = 0;
        brightness = 1000;
        tile_brightness = 1000;
        alt_brightness = 1000;
        red = 1000;
        green = 1000;
        blue = 1000;
    }
}


// void CellClass::Recalc_Light() (0x004798F0): Westwood's Recalc_Light. The cell's brightness
// (+0x10A), tile brightness (+0x10C) and bridge brightness (+0x10E) from its ambient (+0x108), the
// scenario light (ion storm, dominator or normal ground/level values) and its intensity (+0x104),
// limited to 0..2000.
void CellClass::Recalc_Light()
{
    unknown_10a = unknown_108 + 1000 * unknown_id_source_a3d290->unknown_22ac / 100;
    unknown_10e = unknown_10a;

    if (Unknown_0051FC50()) {
        unknown_10a += unknown_11b * unknown_id_source_a3d290->IonLevel - unknown_id_source_a3d290->IonGround;
        unknown_10e += (unknown_11b + 4) * unknown_id_source_a3d290->IonLevel - unknown_id_source_a3d290->IonGround;
    } else if (Unknown_0051FC60()) {
        unknown_10a += unknown_11b * unknown_id_source_a3d290->unknown_22f4 - unknown_id_source_a3d290->unknown_22f0;
        unknown_10e += (unknown_11b + 4) * unknown_id_source_a3d290->unknown_22f4 - unknown_id_source_a3d290->unknown_22f0;
    } else {
        unknown_10a += unknown_11b * unknown_id_source_a3d290->AmbientLevel - unknown_id_source_a3d290->AmbientGround;
        unknown_10e += (unknown_11b + 4) * unknown_id_source_a3d290->AmbientLevel - unknown_id_source_a3d290->AmbientGround;
    }

    unknown_10c = (unknown_10a * unknown_104) >> 16;
    unknown_10e = (unknown_10e * unknown_104) >> 16;
    unknown_10a = (unknown_10a < 2000) ? unknown_10a : 2000;
    unknown_10c = (unknown_10c < 2000) ? unknown_10c : 2000;
    unknown_10e = (unknown_10e < 2000) ? unknown_10e : 2000;

    unknown_10a = (unknown_10a > 0) ? unknown_10a : 0;
    unknown_10c = (unknown_10c > 0) ? unknown_10c : 0;
    unknown_10e = (unknown_10e > 0) ? unknown_10e : 0;
}


// int CellClass::Bounce_Direction(CoordStruct const &origin) const (0x00479AC0): Westwood's
// Bounce_Direction. The facing from origin's cell to this one (a table by the offset), turned one
// step toward the side whose neighbour stands two levels above origin's cell when only one does.
// The facing by dx + 3 * dy (0x007D2C44, a function-local static of the original; not range checked).
extern int unknown_facing_lookup_7d2c44[];
int CellClass::Bounce_Direction(CoordStruct const &xorigin) const
{
    Cell origin = Map[xorigin].MapCoords;

    int dx = MapCoords.X - origin.X;
    int dy = MapCoords.Y - origin.Y;

    int base = unknown_facing_lookup_7d2c44[(MapCoords.X - origin.X) + 3 * (MapCoords.Y - origin.Y)];
    if (dx != 0 && dy != 0) {
        bool left;
        bool right;
        if (dx == dy) {
            left = Map[origin + Cell(dx, 0)].unknown_11b >= Map[xorigin].unknown_11b + 2;
            right = Map[origin + Cell(0, dy)].unknown_11b >= Map[xorigin].unknown_11b + 2;
        } else {
            left = Map[origin + Cell(dy, 0)].unknown_11b >= Map[xorigin].unknown_11b + 2;
            right = Map[origin + Cell(0, dx)].unknown_11b >= Map[xorigin].unknown_11b + 2;
        }
        if (left) {
            if (!right) {
                base = (base - 1) & 7;
            }
        } else if (right) {
            base++;
        }
    }
    return base;
}


// 0x00479c90: function not yet identified
//
// Body: 13 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void Inc_Drawer_Ref_Count_00479c90(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00479c90 {

void __stdcall Inc_Drawer_Ref_Count_00479c90(int drawer)
{
    *(int *)(drawer + 0x194) = *(int *)(drawer + 0x194) + 1;
    return;
}

}  // namespace seed_00479c90


// 0x00479ca0: function not yet identified
//
// Body: 22 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void Dec_Drawer_Ref_Count_00479ca0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00479ca0 {

extern "C" char GameActive_00a40958;

void __stdcall Dec_Drawer_Ref_Count_00479ca0(int drawer)
{
    if (GameActive_00a40958 != '\0') {
        *(int *)(drawer + 0x194) = *(int *)(drawer + 0x194) + -1;
    }
    return;
}

}  // namespace seed_00479ca0


// bool CellClass::Has_Tunnel() const (0x00479CC0): Westwood's Has_Tunnel. A tube runs through
// the cell (a valid index into the tube list) and its land is LAND_TUNNEL (10). The named land
// local is the original's /Ob2 size: it keeps the last two Is_Near_Tunnel_NW sites out of line.

bool CellClass::Has_Tunnel() const
{
    int land = unknown_ec;
    return unknown_116 >= 0 && unknown_116 < unknown_TubeClass_list_008661c8.Count() && land == 10;
}


// bool CellClass::Is_Near_Tunnel_NW() (0x00479CF0): Westwood's Is_Near_Tunnel_NW. This cell holds
// a tunnel, or one of the next two cells to the north or to the west holds a tunnel mouth (a tunnel
// cell whose next cell on that line has none).

bool CellClass::Is_Near_Tunnel_NW()
{
    if (Has_Tunnel()) {
        return true;
    }

    CellClass *north1 = &Adjacent_Cell(0);
    if (north1 != 0) {
        CellClass *north2 = &north1->Adjacent_Cell(0);
        if (north2 != 0) {
            if (north1->Has_Tunnel() && !north2->Has_Tunnel()) {
                return true;
            }
            CellClass *north3 = &north2->Adjacent_Cell(0);
            if (north3 != 0) {
                if (north2->Has_Tunnel() && !north3->Has_Tunnel()) {
                    return true;
                }
            }
        }
    }

    CellClass *west1 = &Adjacent_Cell(6);
    if (west1 != 0) {
        CellClass *west2 = &west1->Adjacent_Cell(6);
        if (west2 != 0) {
            if (west1->Has_Tunnel() && !west2->Has_Tunnel()) {
                return true;
            }
            CellClass *west3 = &west2->Adjacent_Cell(6);
            if (west3 != 0) {
                if (west2->Has_Tunnel() && !west3->Has_Tunnel()) {
                    return true;
                }
            }
        }
    }

    return false;
}


// bool CellClass::Is_Near_Tunnel_ES() (0x00479F70): TibSun name and shape. This cell, the next
// two to the south-east, or the cells south and east of the first of them hold a tunnel
// (Has_Tunnel 0x00479CC0, expanded at every site).

bool CellClass::Is_Near_Tunnel_ES()
{
    if (Has_Tunnel()) {
        return true;
    }

    CellClass *south_east = &Adjacent_Cell(3);
    if (south_east != 0) {
        CellClass *south_east2 = &south_east->Adjacent_Cell(3);
        if (south_east2 != 0) {
            if (south_east->Has_Tunnel() || south_east2->Has_Tunnel()) {
                return true;
            }

            CellClass *south = &south_east->Adjacent_Cell(4);
            CellClass *east = &south_east->Adjacent_Cell(2);
            if (south != 0 && east != 0) {
                if (south->Has_Tunnel() || east->Has_Tunnel()) {
                    return true;
                }
            }
        }
    }

    return false;
}


// 0x0047a120: function not yet identified
//
// Body: 5 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: undefined1 Can_Enter_Tunnel_0047a120(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0047a120 {

unsigned char __stdcall Can_Enter_Tunnel_0047a120(int unused_0)
{
    return 1;
}

}  // namespace seed_0047a120


// void CellClass::Force_New_Slope_For_Occupiers() (0x0047A160): takes up to ten occupants,
// calls their slot 109 with 0 and passes +0x11C to the locomotor of each unit
// or aircraft (ILocomotion::Force_New_Slope). No direct callers.

void CellClass::Force_New_Slope_For_Occupiers()
{
    ObjectClass *objects[10];
    int count = 0;
    for (ObjectClass *object = OccupierPtr; object != 0 && count < 10; object = object->unknown_30) {
        objects[count++] = object;
    }
    for (int index = 0; index < count; index++) {
        ObjectClass *occupant = objects[index];
        occupant->Set_Height_AGL(0);
        if (occupant->Kind_Of() == 1 || occupant->Kind_Of() == 2) {
            ((FootClass *)occupant)->Locomotor.Checked()->Force_New_Slope(unknown_11c);
        }
    }
}


// wchar_t const *CellClass::unknown_47a200() const (0x0047A200): the UI name
// of the cell's tiberium; null when the overlay is not ore.

wchar_t const *CellClass::unknown_47a200() const
{
    int tiberium = OverlayClass::To_Tiberium(OverlayIndex);
    if (tiberium != -1) {
        return unknown_TiberiumClass_list_00ac0b30[tiberium]->UIName;
    }
    return 0;
}


// int CellClass::Tiberium_Type_Here() const (0x0047A220): the tiberium type of
// the cell's overlay (OverlayClass::To_Tiberium; -1 when it is not ore).

int CellClass::Tiberium_Type_Here() const
{
    return OverlayClass::To_Tiberium(OverlayIndex);
}


// int CellClass::Get_Tiberium_Value() const (0x0047A230): the credit value
// of the cell's ore: its tiberium type's value times (OverlayData + 1); 0
// when the overlay is not ore.

int CellClass::Get_Tiberium_Value() const
{
    int tiberium = OverlayClass::To_Tiberium(OverlayIndex);
    if (tiberium == -1) {
        return 0;
    }
    return unknown_TiberiumClass_list_00ac0b30[tiberium]->Value * (OverlayData + 1);
}


// bool CellClass::Is_Tile_Water() const (0x0047A270): the cell's tile belongs to the water set (ra2-decomp cell.cpp
// Is_Tile_Water text; TibSun IS_TILE_IN_SET(WaterSet, WATER_COUNT)). Is_Clear_To_Build 0x004718B0 expands it.

bool CellClass::Is_Tile_Water() const
{
    return IsoTileIndex >= unknown_tileset_a52580 && IsoTileIndex < unknown_tileset_a52580 + 14;
}


// int CellClass::Occupier_Height() const (0x0047A290): the height of the top
// of the cell: the ground height at its middle plus, with a building, the
// height (Z) of its type's slot 31 dimensions, or else 0x55 when an occupant
// stands at the spot (0, 0) (name hypothesis).

int CellClass::Occupier_Height() const
{
    int height;
    {
        Point2D center = Point2D(128, 128);
        height = unknown_4706c0(center);
    }
    BuildingClass *building = (BuildingClass *)Cell_Building();
    if (building != 0) {
        CoordStruct dimensions = building->Type->Lepton_Dimensions();
        height += dimensions.Z;
    } else if (Cell_Techno(Point2D(0, 0), false, 0) != 0) {
        height += 0x55;
    }
    return height;
}


// void CellClass::unknown_47a340(AbstractClass *target) (0x0047A340): forgets
// target: the cell tag (and the tagged-cell entry), +0x2C, +0x30 and, for a
// fogged object (kind 0x3D), its entry in the fogged-object list.

void CellClass::unknown_47a340(AbstractClass *target)
{
    if (unknown_3c == target) {
        if (unknown_3c != 0) {
            unknown_3c->AttachCount--;
        }
        unknown_3c = 0;
        unknown_map_tagged_cells_833638.Delete(MapCoords);
    }
    if (unknown_2c == target) {
        unknown_2c = 0;
    }
    if (unknown_30 == target) {
        unknown_30 = 0;
    }
    if (FoggedObjects != 0 && target->Kind_Of() == 0x3D) {
        FoggedObjects->Delete((FoggedObjectClass *)target);
    }
}


// void CellClass::Set_CellID(Cell const &cell) (0x0047A450)

void CellClass::Set_CellID(Cell const &cell)
{
    MapCoords = cell;
}


// void CellClass::Attach_Tag(TagClass *tag) (0x0047A460): attaches the
// cell tag: the old tag's carrier count drops, a new one is counted and the
// cell joins the map's tagged-cell list.

void CellClass::Attach_Tag(TagClass *tag)
{
    if (unknown_3c != 0) {
        unknown_3c->AttachCount--;
    }
    unknown_3c = tag;
    if (tag != 0) {
        unknown_map_tagged_cells_833638.Add(MapCoords);
        unknown_3c->AttachCount++;
    }
}


// void CellClass::Register_As_Dirty() (0x0047A4E0): redraws the cell's screen
// area: the pixel of the cell's corner coordinate, moved left by 30 and down
// by the screen offset at 0x00839634 less the cell level (+0x11B, 15 pixels
// each) and 15, as a 60x30 rectangle (Tactical::Register_Dirty_Area).

void CellClass::Register_As_Dirty()
{
    CoordStruct corner = Coord_Snap(CoordStruct(MapCoords.X * 256 + 128, MapCoords.Y * 256 + 128, 0));
    Point2D pixel;
    unknown_tactical_83996c->Coord_To_Pixel(corner, pixel);
    pixel.Y += unknown_int_839634 - unknown_11b * 15;
    pixel.X -= 30;
    pixel.Y -= 15;
    unknown_tactical_83996c->Register_Dirty_Area(Rect(pixel.X, pixel.Y, 60, 30), 0);
}


// int CellClass::Get_Vein_Frame() (0x0047A5A0): one bit per cardinal
// neighbour (facings 0, 2, 4, 6) that holds veins: an IsVeins overlay (when
// the neighbour's +0x11C is set, only those) or overlay 0x7E with overlay
// data of at least 48 (name hypothesis).

int CellClass::Get_Vein_Frame()
{
    int mask = 0;
    for (int index = 0; index < 4; index++) {
        CellClass const &cell = ((CellClass const *)this)->Adjacent_Cell(index << 1);
        bool connected=false;
        if(cell.OverlayIndex!=-1 && (cell.unknown_11c<=0 || OverlayTypes[cell.OverlayIndex]->IsVeins)) {
            connected=(cell.OverlayIndex!=126 && OverlayTypes[cell.OverlayIndex]->IsVeins)
                || (cell.OverlayIndex==126 && cell.OverlayData>=48);
        }
        if(connected) mask|=1<<index;
    }
    return mask;
}


// bool CellClass::Can_Place_Veins() const (0x0047A670): whether veins may
// spread into the cell (hypothesis; OverlayClass 0x005DC180 and
// VeinholeMonsterClass 0x00710BB0 ask it): the cell's +0x11C is below 5, its
// land type is not 2, 3, 8 or 6, it has no overlay or a vein overlay, and
// each cardinal neighbour likewise (a neighbour with +0x11C above 4 refuses
// while this cell's +0x11C is 0, unless it holds veins).

bool CellClass::Can_Place_Veins() const
{
    if (unknown_11c <= 4 && unknown_ec != 2 && unknown_ec != 3 && unknown_ec != 8 && unknown_ec != 6
        && (OverlayIndex == -1 || OverlayTypes[OverlayIndex]->IsVeins)) {
        for (int facing = 0; facing < 8; facing += 2) {
            CellClass const &cell = ((CellClass *)this)->Adjacent_Cell(facing);
            if ((cell.unknown_11c > 4 && unknown_11c == 0 && (cell.OverlayIndex == -1 || !OverlayTypes[cell.OverlayIndex]->IsVeins))
                || cell.unknown_ec == 2 || cell.unknown_ec == 3 || cell.unknown_ec == 8 || cell.unknown_ec == 6
                || (cell.OverlayIndex != -1 && !OverlayTypes[cell.OverlayIndex]->IsVeins)) {
                return false;
            }
        }
        return true;
    }
    return false;
}


extern Rect unknown_rect_839630;
extern "C" int __cdecl abs(int value);
// Recompute this vein connection and its cardinal neighbors, redrawing their old and new areas.
void CellClass::Redraw_Veins()
{
    Rect dirty(0,0,0,0);
    dirty=Union(dirty,Union(Overlay_Render_Rect(),Overlay_Shadow_Render_Rect()));
    OverlayData=0;
    int frame=Get_Vein_Frame();
    if(frame==0 || unknown_11c) OverlayIndex=-1;
    else OverlayData=3*frame+abs(unknown_id_source_a3d290->Random()()%3);
    Map.Radar_Background(MapCoords);
    for(int direction=0;direction<8;direction+=2) {
        CellClass &adjacent=(CellClass&)((CellClass const*)this)->Adjacent_Cell(direction);
        if(adjacent.unknown_11c==0 && adjacent.OverlayIndex==126 && adjacent.OverlayData<48) {
            int neighbor_frame=adjacent.Get_Vein_Frame();
            Rect area(0,0,0,0);
            area=Union(area,Union(adjacent.Overlay_Render_Rect(),adjacent.Overlay_Shadow_Render_Rect()));
            if(neighbor_frame==0) adjacent.OverlayIndex=-1;
            else adjacent.OverlayData=3*neighbor_frame+abs(unknown_id_source_a3d290->Random()()%3);
            adjacent.Recalc_Attributes(-1);
            area=Union(area,Union(adjacent.Overlay_Render_Rect(),adjacent.Overlay_Shadow_Render_Rect()))-unknown_rect_839630.Top_Left();
            unknown_tactical_83996c->Register_Dirty_Area(area,false);
            Map.Radar_Background(adjacent.MapCoords);
        }
    }
    dirty=Union(dirty,Union(Overlay_Render_Rect(),Overlay_Shadow_Render_Rect()))-unknown_rect_839630.Top_Left();
    unknown_tactical_83996c->Register_Dirty_Area(dirty,false);
    Recalc_Attributes(-1);
}


// bool CellClass::unknown_47b590() const (0x0047B590): whether the cell has
// no tile (IsoTileIndex 0xFFFF or 0).

bool CellClass::unknown_47b590() const
{
    if (IsoTileIndex == 0xFFFF || IsoTileIndex == 0) {
        return true;
    }
    return false;
}


// bool CellClass::unknown_47b5b0() const (0x0047B5B0): whether the cell's
// tile is in the 20-tile set at 0x00A6E018 or the 12-tile set at 0x00A52EA8.

bool CellClass::unknown_47b5b0() const
{
    if ((IsoTileIndex < unknown_tileset_a6e018 || IsoTileIndex >= unknown_tileset_a6e018 + 20) && (IsoTileIndex < unknown_tileset_a52ea8 || IsoTileIndex >= unknown_tileset_a52ea8 + 12)) {
        return false;
    }
    return true;
}


// bool CellClass::unknown_47b5e0() const (0x0047B5E0): Westwood's Is_Tile_Cliff. The tile is in
// one of the cliff sets: cliffs, the four waterfall sets (except the two end tiles' open
// subtiles), cliff ramps, water caves, bridges, train bridges, destroyable and water cliffs.

bool CellClass::unknown_47b5e0() const
{
    if (unknown_tileset_a52e68 != -1 && IsoTileIndex >= unknown_tileset_a52e68 && IsoTileIndex < unknown_tileset_a52e68 + 40) {
        return true;
    }
    if (unknown_tileset_a52584 != -1 && IsoTileIndex >= unknown_tileset_a52584 && IsoTileIndex < unknown_tileset_a52584 + 4) {
        if (IsoTileIndex == unknown_tileset_a52584 || IsoTileIndex == unknown_tileset_a52584 + 3) {
            if (unknown_11a == 0 || unknown_11a == 4) {
                return false;
            }
        }
        return true;
    }
    if (unknown_tileset_a6cf58 != -1 && IsoTileIndex >= unknown_tileset_a6cf58 && IsoTileIndex < unknown_tileset_a6cf58 + 4) {
        if (IsoTileIndex == unknown_tileset_a6cf58 || IsoTileIndex == unknown_tileset_a6cf58 + 3) {
            if (unknown_11a == 1 || unknown_11a == 3) {
                return false;
            }
        }
        return true;
    }
    if (unknown_tileset_a52ea0 != -1 && IsoTileIndex >= unknown_tileset_a52ea0 && IsoTileIndex < unknown_tileset_a52ea0 + 4) {
        if (IsoTileIndex == unknown_tileset_a52ea0 || IsoTileIndex == unknown_tileset_a52ea0 + 3) {
            if (unknown_11a == 0 || unknown_11a == 1) {
                return false;
            }
        }
        return true;
    }
    if (unknown_tileset_a52ef0 != -1 && IsoTileIndex >= unknown_tileset_a52ef0 && IsoTileIndex < unknown_tileset_a52ef0 + 4) {
        if (IsoTileIndex == unknown_tileset_a52ef0 || IsoTileIndex == unknown_tileset_a52ef0 + 3) {
            if (unknown_11a == 2 || unknown_11a == 3) {
                return false;
            }
        }
        return true;
    }
    if (unknown_tileset_a6dd00 != -1 && IsoTileIndex >= unknown_tileset_a6dd00 && IsoTileIndex < unknown_tileset_a6dd00 + 20) {
        return true;
    }
    if (unknown_tileset_a6cb70 != -1 && IsoTileIndex >= unknown_tileset_a6cb70 && IsoTileIndex < unknown_tileset_a6cb70 + 4) {
        return true;
    }
    if (unknown_tileset_a52c74 != -1 && IsoTileIndex >= unknown_tileset_a52c74 && IsoTileIndex < unknown_tileset_a52c74 + 16) {
        return true;
    }
    if (unknown_tileset_a6cb68 != -1 && IsoTileIndex >= unknown_tileset_a6cb68 && IsoTileIndex < unknown_tileset_a6cb68 + 16) {
        return true;
    }
    if (unknown_tileset_a6e0f8 != -1 && IsoTileIndex >= unknown_tileset_a6e0f8 && IsoTileIndex < unknown_tileset_a6e0f8 + 2) {
        return true;
    }
    if (unknown_tileset_a52e64 != -1 && IsoTileIndex >= unknown_tileset_a52e64 && IsoTileIndex < unknown_tileset_a52e64 + 28) {
        return true;
    }
    return false;
}


// bool CellClass::unknown_47b7c0() const (0x0047B7C0): whether the cell's tile
// is one of the 42 tiles of the set starting at 0x00A6CB74.

bool CellClass::unknown_47b7c0() const
{
    if (IsoTileIndex >= unknown_tileset_a6cb74 && IsoTileIndex < unknown_tileset_a6cb74 + 42) {
        return true;
    }
    return false;
}


// bool CellClass::unknown_47b7e0() const (0x0047B7E0): whether the cell's
// tile is in the 42-tile set at 0x00A6CB74 (unknown_47b7c0) or in one of the
// sets at 0x00A52580 (14), 0x00A52584, 0x00A6CF58, 0x00A52EA0 and 0x00A52EF0 (4).

bool CellClass::unknown_47b7e0() const
{
    if (IsoTileIndex >= unknown_tileset_a6cb74 && IsoTileIndex < unknown_tileset_a6cb74 + 42) {
        return true;
    }
    if ((IsoTileIndex < unknown_tileset_a52580 || IsoTileIndex >= unknown_tileset_a52580 + 14)
        && (IsoTileIndex < unknown_tileset_a52584 || IsoTileIndex >= unknown_tileset_a52584 + 4)
        && (IsoTileIndex < unknown_tileset_a6cf58 || IsoTileIndex >= unknown_tileset_a6cf58 + 4)
        && (IsoTileIndex < unknown_tileset_a52ea0 || IsoTileIndex >= unknown_tileset_a52ea0 + 4)
        && (IsoTileIndex < unknown_tileset_a52ef0 || IsoTileIndex >= unknown_tileset_a52ef0 + 4)) {
        return false;
    }
    return true;
}


// bool CellClass::unknown_47b860() const (0x0047B860): whether the cell's tile
// is one of the 14 tiles of the set starting at 0x00A52EF4.

bool CellClass::unknown_47b860() const
{
    if (IsoTileIndex >= unknown_tileset_a52ef4 && IsoTileIndex < unknown_tileset_a52ef4 + 14) {
        return true;
    }
    return false;
}


// bool CellClass::unknown_47b880() const (0x0047B880): whether the cell's tile
// is one of the 16 tiles of the set starting at 0x00A6E0E0.

bool CellClass::unknown_47b880() const
{
    if (IsoTileIndex >= unknown_tileset_a6e0e0 && IsoTileIndex < unknown_tileset_a6e0e0 + 16) {
        return true;
    }
    return false;
}


// bool CellClass::unknown_47b8a0() const (0x0047B8A0): whether the cell's
// tile is in the sets at 0x00A6CB6C (11), 0x00A6E014 (24) or 0x00A52EFC (66).

bool CellClass::unknown_47b8a0() const
{
    if ((IsoTileIndex < unknown_tileset_a6cb6c || IsoTileIndex >= unknown_tileset_a6cb6c + 11)
        && (IsoTileIndex < unknown_tileset_a6e014 || IsoTileIndex >= unknown_tileset_a6e014 + 24)
        && (IsoTileIndex < unknown_tileset_a52efc || IsoTileIndex >= unknown_tileset_a52efc + 66)) {
        return false;
    }
    return true;
}


// bool CellClass::unknown_47b8e0() const (0x0047B8E0): whether the cell's tile
// is one of the 15 tiles of the set starting at 0x00A6DD0C.

bool CellClass::unknown_47b8e0() const
{
    if (IsoTileIndex >= unknown_tileset_a6dd0c && IsoTileIndex < unknown_tileset_a6dd0c + 15) {
        return true;
    }
    return false;
}


// bool CellClass::unknown_47b900() const (0x0047B900): whether the cell's tile
// is one of the 4 tiles of the set starting at 0x00A6DD08.

bool CellClass::unknown_47b900() const
{
    if (IsoTileIndex >= unknown_tileset_a6dd08 && IsoTileIndex < unknown_tileset_a6dd08 + 4) {
        return true;
    }
    return false;
}


// bool CellClass::unknown_47b920() const (0x0047B920): whether the cell's tile
// is one of the 4 tiles of the set starting at 0x00A52EE4.

bool CellClass::unknown_47b920() const
{
    if (IsoTileIndex >= unknown_tileset_a52ee4 && IsoTileIndex < unknown_tileset_a52ee4 + 4) {
        return true;
    }
    return false;
}


// bool CellClass::unknown_47b940() const (0x0047B940): whether the cell's tile
// is one of the 14 tiles of the set starting at 0x00A52C68.

bool CellClass::unknown_47b940() const
{
    if (IsoTileIndex >= unknown_tileset_a52c68 && IsoTileIndex < unknown_tileset_a52c68 + 14) {
        return true;
    }
    return false;
}


// bool CellClass::unknown_47b960() const (0x0047B960): whether the cell's tile
// is one of the 16 tiles of the set starting at 0x00A52C74.

bool CellClass::unknown_47b960() const
{
    if (IsoTileIndex >= unknown_tileset_a52c74 && IsoTileIndex < unknown_tileset_a52c74 + 16) {
        return true;
    }
    return false;
}


// bool CellClass::unknown_47b980() const (0x0047B980): whether the cell's tile
// is one of the 16 tiles of the set starting at 0x00A6CB68.

bool CellClass::unknown_47b980() const
{
    if (IsoTileIndex >= unknown_tileset_a6cb68 && IsoTileIndex < unknown_tileset_a6cb68 + 16) {
        return true;
    }
    return false;
}


// bool CellClass::unknown_47b9a0() const (0x0047B9A0): whether the cell's tile
// is one of the 16 tiles of the set starting at 0x00A52C70.

bool CellClass::unknown_47b9a0() const
{
    if (IsoTileIndex >= unknown_tileset_a52c70 && IsoTileIndex < unknown_tileset_a52c70 + 16) {
        return true;
    }
    return false;
}


// bool CellClass::unknown_47b9c0() const (0x0047B9C0): whether the cell's
// tile is the one at 0x00A52C60 or in the 16-tile set at 0x00A52590.

bool CellClass::unknown_47b9c0() const
{
    if (IsoTileIndex != unknown_tileset_a52c60 && (IsoTileIndex < unknown_tileset_a52590 || IsoTileIndex >= unknown_tileset_a52590 + 16)) {
        return false;
    }
    return true;
}


// Not exact (RA2_NON_MATCHING): CellClass::Place_Veins at 0x0047AD00, best score 78.39, 2026-11-13; differs: 669 instructions in the original, 673 in the draft (shape 98.66)
#ifdef RA2_NON_MATCHING
// void CellClass::Place_Veins() (0x0047AD00): TibSun Place_Veins.


inline bool Is_Solid_Vein(CellClass const &cell)
{
    return cell.OverlayIndex == 0x7E && cell.OverlayData >= 0x30;
}

void CellClass::Place_Veins()
{
    Rect rect(0, 0, 0, 0);

    if (Can_Place_Veins()) {
        rect = Union(rect, Union(Overlay_Render_Rect(), Overlay_Shadow_Render_Rect()));

        Map.Radar_Background(MapCoords);
        OverlayIndex = 0x7E;

        if (unknown_11c == 0) {
            OverlayData = abs(unknown_id_source_a3d290->Random()()) % 3 + 0x30;

            int dir;
            for (dir = 0; dir < 8; dir += 2) {
                CellClass &adjacent = (CellClass &)Adjacent_Cell((FacingType)dir);

                if (Is_Solid_Vein(adjacent)) {
                    continue;
                }
                if (adjacent.OverlayIndex != 0x7E && adjacent.OverlayIndex != -1 && OverlayTypes[adjacent.OverlayIndex]->IsVeins) {
                    continue;
                }

                if (adjacent.unknown_11c == 0) {
                    int frame = adjacent.Get_Vein_Frame();
                    if (adjacent.OverlayIndex == -1 || (frame <= 15 && adjacent.OverlayData / 3 != frame)) {
                        Rect adjrect(0, 0, 0, 0);
                        adjrect = Union(adjrect, Union(adjacent.Overlay_Render_Rect(), adjacent.Overlay_Shadow_Render_Rect()));
                        adjacent.OverlayIndex = 0x7E;
                        adjacent.OverlayData = 3 * frame + abs(unknown_id_source_a3d290->Random()() % 3);
                        adjacent.Recalc_Attributes(-1);
                        RectPoint2D origin = unknown_rect_839630.Top_Left();
                        adjrect = Union(adjrect, Union(adjacent.Overlay_Render_Rect(), adjacent.Overlay_Shadow_Render_Rect())) - origin;
                        unknown_tactical_83996c->Register_Dirty_Area(adjrect, 0);
                    }
                } else {
                    Rect adjrect(0, 0, 0, 0);
                    adjrect = Union(adjrect, Union(adjacent.Overlay_Render_Rect(), adjacent.Overlay_Shadow_Render_Rect()));
                    adjacent.OverlayIndex = 0x7E;
                    adjacent.OverlayData = 2 * adjacent.unknown_11c + (abs(unknown_id_source_a3d290->Random()()) & 1) + 0x33;
                    adjrect = Union(adjrect, Union(adjacent.Overlay_Render_Rect(), adjacent.Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
                    unknown_tactical_83996c->Register_Dirty_Area(adjrect, 0);
                }

                Map.Radar_Background(adjacent.MapCoords);
            }

            Trigger_Veins();
        } else {
            OverlayData = 2 * unknown_11c + (abs(unknown_id_source_a3d290->Random()()) & 1) + 0x33;
        }

        rect = Union(rect, Union(Overlay_Render_Rect(), Overlay_Shadow_Render_Rect())) - unknown_rect_839630.Top_Left();
        unknown_tactical_83996c->Register_Dirty_Area(rect, 0);
        Recalc_Attributes(-1);
    }
}
#endif

// bool CellClass::On_Ground() const (0x0047B9F0, slot 20): false when
// the cell's tile is in the 14-tile set at 0x00A52580, else true.

bool CellClass::On_Ground() const
{
    if (IsoTileIndex >= unknown_tileset_a52580 && IsoTileIndex < unknown_tileset_a52580 + 14) {
        return false;
    }
    return true;
}


// bool CellClass::unknown_47ba10(int house) const (0x0047BA10): with a player
// house and the house's bit set, true for the player's own house or when the
// player's per-house counter at +0x7C is not positive.

bool CellClass::unknown_47ba10(int house) const
{
    if (unknown_player_a35db4 != 0 && Is_House_Bit(house)) {
        if (house == unknown_player_a35db4->ArrayIndex) {
            return true;
        }
        if (unknown_7c[unknown_player_a35db4->ArrayIndex] <= 0) {
            return true;
        }
    }
    return false;
}


// CoordStruct CellClass::Center_Coord() const (0x0047BA50, slot 18): the
// cell's centre at the ground height there.

CoordStruct CellClass::Center_Coord() const
{
    int x = MapCoords.X * 256 + 128;
    int y = MapCoords.Y * 256 + 128;
    return CoordStruct(x, y, unknown_4706c0(Point2D(x, y)));
}


// CoordStruct CellClass::unknown_slot22() const (0x0047BAA0, slot 22): the
// cell's centre (slot 18), raised by unknown_geometry_j (four levels: the
// bridge height) when flag 0x100 of +0x140 is set.

CoordStruct CellClass::unknown_slot22() const
{
    if (unknown_140 & 0x100) {
        return Center_Coord() + CoordStruct(0, 0, unknown_geometry_j);
    }
    return Center_Coord();
}


// bool CellClass::Is_Tile_Destroyable_Cliff() const (0x0047BB10): whether the cell's
// tile is the one at 0x00A6E0F8 or the next.

bool CellClass::Is_Tile_Destroyable_Cliff() const
{
    if (IsoTileIndex == unknown_tileset_a6e0f8) {
        return true;
    }
    return IsoTileIndex == unknown_tileset_a6e0f8 + 1 ? true : false;
}


// void CellClass::Trigger_Veins() (0x0047BB30)
// Veins attack: on a grown vein cell (overlay 0x7E, data 48 or more, +0x11C
// clear) not yet marked (+0x140 bit 0x20000), each low occupant (slot 108 at
// most 5) that is a techno not immune to veins and without ability 8 gets the
// Rules VeinAttack animation at the cell's corner, and the cell is marked.

void CellClass::Trigger_Veins()
{
    if (OverlayIndex == 0x7E && OverlayData >= 48 && !unknown_11c && !(unknown_140 & 0x20000)) {
        for (ObjectClass *object = OccupierPtr; object != 0; object = object->unknown_30) {
            if (object->Get_Height_AGL() <= 5) {
                bool is_techno;
                if (object != 0) {
                    is_techno = object->AbstractFlags.IsTechno;
                }
                TechnoClass *techno = (object != 0 && is_techno) ? (TechnoClass *)object : 0;
                if (techno != 0 && !techno->Techno_Type_Class()->ImmuneToVeins && !techno->unknown_6d7910(8)) {
                    new AnimClass(Rules->VeinAttack, unknown_475d60() - CoordStruct(128, 128, 0), 0, 1, 0x600, 0);
                    unknown_140 |= 0x20000;
                }
            }
        }
    }
}


// void CellClass::Fog_Cell() (0x0047BC80): Westwood's Fog_Cell. With
// fog of war on, walks the seven cells down the diagonal from this one; each
// whose level (+0x11B) is within two below 1, 3, .., 15 in turn and is not yet
// fogged (+0x140 flag 0x400000) is fogged: its buildings leave fogged copies in
// a new list, other units are unselected.
void CellClass::Fog_Cell()
{
    if (unknown_id_source_a3d290->unknown_000 & 0x1000) {
        Cell cell = MapCoords;
        for (int level = 1; level <= 15; level += 2) {
            CellClass &other = Map[cell];
            if (other.unknown_11b >= level - 2 && other.unknown_11b <= level) {
                if (!(other.unknown_140 & 0x400000)) {
                    DynamicVectorClass<FoggedObjectClass *> *fogged = new DynamicVectorClass<FoggedObjectClass *>;
                    fogged->Resize(1);
                    fogged->Set_Growth_Step(1);
                    other.unknown_140 |= 0x400000;
                    for (ObjectClass *object = other.OccupierPtr; object != 0; object = object->unknown_30) {
                        int kind = object->What_Am_I();
                        if (kind == 6) {
                            BuildingClass *building = (BuildingClass *)object;
                            building->unknown_slot104();
                            if (building->unknown_452be0()) {
                                bool fade;
                                if (building->unknown_slot31() || building->unknown_5bd == 15) {
                                    fade = false;
                                } else {
                                    fade = true;
                                }
                                building->Create_Fogged_Copy_00452c70(fogged, &other, fade);
                            }
                        } else if (kind == 1 || kind == 2 || kind == 0x0F) {
                            object->Unselect();
                        }
                    }
                    if (fogged->Count() > 0) {
                        other.FoggedObjects = fogged;
                    } else {
                        delete fogged;
                    }
                }
            }
            cell.X++;
            cell.Y++;
        }
    }
}


// void CellClass::unknown_47be00() (0x0047BE00): walks the seven cells down
// the diagonal from this one; each whose level (+0x11B) is within two below
// 1, 3, .., 13 in turn loses flag 0x400000 of +0x140 and runs Remove_Fogged_Objects.

void CellClass::unknown_47be00()
{
    Cell cell = MapCoords;
    for (int level = 1; level < 15; level += 2) {
        CellClass &other = Map[cell];
        if (other.unknown_11b >= level - 2 && other.unknown_11b <= level) {
            other.unknown_140 &= ~0x400000;
            other.Remove_Fogged_Objects();
        }
        cell.X++;
        cell.Y++;
    }
}


// void CellClass::Remove_Fogged_Objects() (0x0047BE60): discards the cell's fogged
// objects: each is removed from the fogged lists of the other cells it covers
// and deleted, then the list itself is deleted.

void CellClass::Remove_Fogged_Objects()
{
    if (FoggedObjects != 0) {
        int index = FoggedObjects->Count();
        while (--index >= 0) {
            FoggedObjectClass *object = (*FoggedObjects)[index];
            if (object->unknown_4c1f00() != 0) {
                for (Cell const *offset = object->unknown_4c1f00(); offset->X != 0x7FFF || offset->Y != 0x7FFF; offset++) {
                    CoordStruct coord = object->Coord;
                    CellClass &cell = Map[Coord_Cell(coord) + *offset];
                    if (&cell != this && cell.FoggedObjects != 0) {
                        cell.FoggedObjects->Delete(object);
                    }
                }
            }
            delete object;
        }
        FoggedObjects->Clear();
        delete FoggedObjects;
        FoggedObjects = 0;
    }
}


// int CellClass::Occupation_Mask(int house) (0x0047BFA0): -1 unless the cell's unknown_dc mask has the
// house's bit, else a mask of the facings whose adjacent cell has it.

int CellClass::Occupation_Mask(int house)
{
    int bit = 1 << house;
    if (!(unknown_dc & bit)) {
        return -1;
    }
    int bits = 0;
    for (int facing = 0; facing < 8; facing++) {
        if (Adjacent_Cell(facing).unknown_dc & bit) {
            bits |= 1 << facing;
        }
    }
    return bits;
}


// int CellClass::unknown_47c040() (0x0047C040): tells the object found for
// the cell (Unknown_007101B0; tube or bridge hypothesis) about the cell, or
// else runs Redraw_Veins on the map's cell; answers 0.

int CellClass::unknown_47c040()
{
    UnknownCellLookup *lookup = Unknown_007101B0(MapCoords);
    if (lookup != 0) {
        lookup->Unknown_00712300(this);
        return 0;
    }
    Map[MapCoords].Redraw_Veins();
    return 0;
}


// cell's three screen rectangles (0x004752B0, 0x00474EB0, 0x00475100) moved
// back by the tactical area origin (0x00839630, through Rect::operator- 0x0047D000), and marks the cell for the
// radar (RadarClass 0x00631570).

void CellClass::Register_For_Redraw()
{
    Rect rect = Union(Union(Cell_Render_Rect(), Overlay_Render_Rect()), Overlay_Shadow_Render_Rect());
    unknown_tactical_83996c->Register_Dirty_Area(rect - unknown_rect_839630.Top_Left(), 0);
    Map.Radar_Background(MapCoords);
}


// bool CellClass::Can_Burrow_Here() (0x0047C200): true when the map refuses
// the cell (Map.Unknown_0055A740(this, true) fails); otherwise false when its
// tile type's +0x2D5 flag is clear, when +0x11C or flags 0x500 of +0x140 are
// set, or when a building or terrain object occupies it.

bool CellClass::Can_Burrow_Here()
{
    if (!Map.Unknown_0055A740(this, true)) {
        return true;
    }
    if ((IsoTileIndex >= 0 && IsoTileIndex < IsoTileTypes.Count() && !IsoTileTypes[IsoTileIndex]->unknown_2d5)
        || unknown_11c || (unknown_140 & 0x500) != 0) {
        return false;
    }
    if (Cell_Building() != 0) {
        return false;
    }
    if (Cell_Terrain() != 0) {
        return false;
    }
    return true;
}


// bool CellClass::Is_House_Bit(int house) const (0x0047C2C0).

bool CellClass::Is_House_Bit(int house) const
{
    return (unknown_78 & (1 << house)) != 0;
}


// bool CellClass::Is_Sensed(int house) const (0x0047C2E0): a sensor of the house covers the cell.

bool CellClass::Is_Sensed(int house) const
{
    return unknown_7c[house] > 0;
}


// 0x0047c300: function not yet identified
//
// Body: 20 bytes (frozen inventory bound).
// Referenced from 4 sites (unconditional call).
// Ghidra prototype: bool __thiscall Is_Disguise_Sensed_0047c300(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0047c300 {

struct Owner_0047c300 { bool Is_Disguise_Sensed_0047c300(int house); };

bool Owner_0047c300::Is_Disguise_Sensed_0047c300(int house)
{
    int self = (int)this;
    return 0 < *(short *)(self + 0xac + house * 2);
}

}  // namespace seed_0047c300


// void CellClass::Set_House_Bit(int house) (0x0047C320).

void CellClass::Set_House_Bit(int house)
{
    unknown_78 |= 1 << house;
}


// void CellClass::Clear_House_Bit(int house) (0x0047C340).

void CellClass::Clear_House_Bit(int house)
{
    unknown_78 &= ~(1 << house);
}


class Leaf_47c360 { public: void get(unsigned int a1); };
void Leaf_47c360::get(unsigned int a1) { unsigned int self = (unsigned int)this; unsigned int index; unsigned int spare; index = a1; *(unsigned short*)(self + index*2 + 0x7c) = (*(unsigned short*)(self + index*2 + 0x7c)) + 1; }


class Leaf_47c370 { public: void get(unsigned int a1); };
void Leaf_47c370::get(unsigned int a1) { unsigned int self = (unsigned int)this; unsigned int index; unsigned int spare; index = a1; *(unsigned short*)(self + index*2 + 0x7c) = (*(unsigned short*)(self + index*2 + 0x7c)) - 1; }


class Leaf_47c380 { public: void get(unsigned int a1); };
void Leaf_47c380::get(unsigned int a1) { unsigned int self = (unsigned int)this; unsigned int index; unsigned int spare; index = a1; *(unsigned short*)(self + index*2 + 0xac) = (*(unsigned short*)(self + index*2 + 0xac)) + 1; }


class Leaf_47c390 { public: void get(unsigned int a1); };
void Leaf_47c390::get(unsigned int a1) { unsigned int self = (unsigned int)this; unsigned int index; unsigned int spare; index = a1; *(unsigned short*)(self + index*2 + 0xac) = (*(unsigned short*)(self + index*2 + 0xac)) - 1; }


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool CellClass::Place_Tiberium(int tiberium, int data) (0x0047C3A0): Place_Tiberium. Adapted from ra2-decomp src/cell.cpp
// CellClass::Place_Tiberium (gamemd 0x00487190, MATCHED there; TibSun shape). VectorClass::operator[](int) (the
// twin's and TibSun's signature, now the shared default) is required; with unsigned the grow branch's second
// Union swaps eax/ebx.

bool CellClass::Place_Tiberium(int tib, int data)
{
    TiberiumClass *tiberium = unknown_TiberiumClass_list_00ac0b30[tib];
    if (data < tiberium->unknown_e4) {
        if (Can_Tiberium_Germinate(tiberium)) {
            if (unknown_11c != 0) {
                new OverlayClass(OverlayTypes[tiberium->Image->ArrayIndex + tiberium->NumFrames + 2 * unknown_11c + (Random_Pick(0, 1) - 2)], Fetch_CellID());
            } else {
                new OverlayClass(OverlayTypes[tiberium->Image->ArrayIndex + Random_Pick(0, 11)], Fetch_CellID());
            }
            tiberium->Add_Growth_Cell(MapCoords);
            OverlayData = data;
            Register_For_Redraw();
            return true;
        }

        if (Can_Tiberium_Grow()) {
            if (Tiberium_Type_Here() == tib) {
                OverlayData += data;
                OverlayData = OverlayData < tiberium->unknown_e4 - 1 ? OverlayData : tiberium->unknown_e4 - 1;
                Rect rect = Union(Union(Cell_Render_Rect(), Overlay_Render_Rect()), Overlay_Shadow_Render_Rect());
                unknown_tactical_83996c->Register_Dirty_Area(rect - unknown_rect_839630.Top_Left(), 0);
                tiberium->Add_Spread_Cell(MapCoords);
                return true;
            }
        }
    }
    return false;
}


// void CellClass::unknown_47c840() (0x0047C840): decrements +0x130 (from 0
// when it is 1); at or below zero sets flags 0x18 of +0x12C unless both are
// set already, in which case it clears flag 0x20 of +0x140.

void CellClass::unknown_47c840()
{
    if (unknown_130 == 1) {
        unknown_130 = 0;
    }
    unknown_130--;
    bool both = (unknown_12c & 8) && (unknown_12c & 0x10);
    if (!both) {
        if (unknown_130 <= 0) {
            unknown_12c |= 0x18;
        }
    } else if (unknown_130 <= 0 && (unknown_140 & 0x20) == 0x20) {
        unknown_140 &= ~0x20;
    }
}


// void CellClass::unknown_47c8a0() (0x0047C8A0): increments +0x130 (from 0
// when it is -1) up to +0x134, setting flag 0x20 of +0x140 when it becomes
// positive.

void CellClass::unknown_47c8a0()
{
    int old = unknown_130;
    if (old == -1) {
        unknown_130 = 0;
    }
    unknown_130++;
    if (unknown_130 > unknown_134) {
        unknown_130 = unknown_134;
    }
    if (old <= 0 && unknown_130 > 0) {
        unknown_140 |= 0x20;
    }
}


// 0x0047c900: function not yet identified
//
// Body: 39 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __fastcall Unshroud_0047c900(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0047c900 {

void __fastcall Unshroud_0047c900(int self)
{
    *(unsigned int *)(self + 300) = *(unsigned int *)(self + 300) | 0x18;
    if (0 < *(int *)(self + 0x130)) {
        *(unsigned int *)(self + 0x140) = *(unsigned int *)(self + 0x140) | 0x20;
    }
    return;
}

}  // namespace seed_0047c900


// void CellClass::On_Bridge_Collapse() (0x0047C930): TibSun's On_Bridge_Collapse:
// when the cell carries bridge occupants, every moving foot object in the
// surrounding 5x5 cells whose locomotor is heading for the bridge deck
// (Is_Moving_Here) is destroyed with the C4 warhead. Height from Map, plus the bridge deck height.

void CellClass::On_Bridge_Collapse()
{
    if (unknown_128) {
        CoordStruct coord = MapCoords.As_Coord();
        coord.Z = Map.Get_Height_GL(coord) + unknown_geometry_j;
        for (int x = -2; x < 3; x++) {
            for (int y = -2; y < 3; y++) {
                Cell cell = MapCoords + Cell(x, y);
                CellClass *cellptr = &Map[cell];
                if (cellptr != this) {
                    ObjectClass *optr = 0;
                    ObjectClass *next = 0;
                    optr = cellptr->AltOccupierPtr;
                    while (optr != 0) {
                        next = optr->unknown_30;
                        bool is_foot;
                        if (optr != 0) {
                            is_foot = optr->AbstractFlags.IsFoot;
                        }
                        if (optr != 0 && is_foot) {
                            FootClass *fptr = (FootClass *)optr;
                            if (fptr->Locomotor.Checked()->Is_Moving_Here(coord)) {
                                int damage = fptr->Strength;
                                fptr->Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, true);
                            }
                        }
                        optr = next;
                    }
                    optr = cellptr->OccupierPtr;
                    while (optr != 0) {
                        next = optr->unknown_30;
                        bool is_foot;
                        if (optr != 0) {
                            is_foot = optr->AbstractFlags.IsFoot;
                        }
                        if (optr != 0 && is_foot) {
                            FootClass *fptr = (FootClass *)optr;
                            if (fptr->Locomotor.Checked()->Is_Moving_Here(coord)) {
                                int damage = fptr->Strength;
                                fptr->Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, true);
                            }
                        }
                        optr = next;
                    }
                }
            }
        }
    }
}


// bool CellClass::unknown_47cb50() (0x0047CB50): Map.Is_Shrouded at the cell's centre
// (at the height of its middle point).

bool CellClass::unknown_47cb50()
{
    return Map.Is_Shrouded(unknown_475d60());
}


// void CellClass::unknown_47cbb0() (0x0047CBB0): Map.Is_Fogged at the cell's centre
// (at the height of its middle point).

void CellClass::unknown_47cbb0()
{
    Map.Is_Fogged(unknown_475d60());
}


static CoordStruct Cell_Ground_Center_0047cc10(CellClass const &cell)
{
    Cell center_cell=cell.MapCoords;
    UnknownVector3 center(center_cell.X*256+128,center_cell.Y*256+128,0);
    center.Z=cell.unknown_4706c0(Point2D(128,128));
    CoordStruct result(center.X,center.Y,center.Z);
    return result;
}

// Remove occupants that cannot remain here, including approaching foot objects.
void CellClass::Kill_Illegal_Occupiers()
{
    ObjectClass *object=OccupierPtr;
    ObjectClass *next=0;
    while(object) {
        next=object->unknown_30;
        if(object->Can_Enter_Cell(this,-1,-1,0,true)==7) {
            int damage=object->Strength;
            object->Take_Damage(&damage,0,Rules->unknown_c74,0,true,true);
        }
        object=next;
    }
    CoordStruct center=Cell_Ground_Center_0047cc10(*this);
    for(int x=-2;x<3;++x) {
        for(int y=-2;y<3;++y) {
            CellClass *cellptr=&Map[MapCoords+Cell(x,y)];
            if(cellptr!=this) {
                object=cellptr->OccupierPtr;
                while(object) {
                    bool is_foot;
                    if(object) is_foot=object->AbstractFlags.IsFoot;
                    if(object && is_foot) {
                        FootClass *foot=(FootClass*)object;
                        if(foot->Locomotor.Checked()->Is_Moving_Here(center)
                            && foot->Can_Enter_Cell(this,-1,-1,0,true)==7) {
                            int damage=foot->Strength;
                            foot->Take_Damage(&damage,0,Rules->unknown_c74,0,true,true);
                        }
                    }
                    object=object->unknown_30;
                }
            }
        }
    }
}


// bool CellClass::Can_Build_Here() const (0x0047CDD0): false when +0x11C is
// set or a building occupies the cell, else the first flag of the cell's
// land type's Build flag (Ground).

bool CellClass::Can_Build_Here() const
{
    if (unknown_11c) {
        return false;
    }
    if (Cell_Building() != 0) {
        return false;
    }
    return Ground[unknown_ec].Build;
}


class Leaf_47ce30 { public: void get(unsigned int a1); };
void Leaf_47ce30::get(unsigned int a1) { unsigned int self = (unsigned int)this; unsigned int value; unsigned int spare; value = a1; *(unsigned int*)(self + 0xf8) = value; }


class Leaf_47ce40 { public: unsigned int get(); };
unsigned int Leaf_47ce40::get() { return *(unsigned int*)((char*)this + 0xf8); }


// int CellClass::unknown_47ce50() const (0x0047CE50): whether +0xF0 is above
// 1.0.

int CellClass::unknown_47ce50() const
{
    if (unknown_f0 > 1.0) {
        return 1;
    }
    return 0;
}


// int CellClass::unknown_47ce70() const (0x0047CE70): the radiation level
// (+0xF0) limited to Rules RadLevelMax, as an int.


int CellClass::unknown_47ce70() const
{
    return Min(unknown_f0, Rules->RadLevelMax);
}


// 0x0047cea0: function not yet identified
//
// Body: 19 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __thiscall RadLevel_Increase_0047cea0(int param_1,double param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0047cea0 {

struct Owner_0047cea0 { void RadLevel_Increase_0047cea0(double amount); };

void Owner_0047cea0::RadLevel_Increase_0047cea0(double amount)
{
    int self = (int)this;
    *(double *)(self + 0xf0) = amount + *(double *)(self + 0xf0);
    return;
}

}  // namespace seed_0047cea0


// void CellClass::Reduce_f0(double amount) (0x0047CEC0): subtracts amount
// from +0xF0, clamping at zero.

void CellClass::Reduce_f0(double amount)
{
    double value = (unknown_f0 -= amount);
    unknown_f0 = 0.0 > value ? 0.0 : value;
}


// bool CellClass::Set_unknown_e0(void *value) (0x0047CF30): clears +0xE0 for
// null; otherwise claims it unless another value holds it.

bool CellClass::Set_unknown_e0(void *value)
{
    if (value == 0) {
        unknown_e0 = 0;
        return true;
    }
    if (unknown_e0 != 0 && unknown_e0 != value) {
        return false;
    }
    unknown_e0 = value;
    return true;
}


class Leaf_47cf70 { public: unsigned int get(unsigned int a1); };
unsigned int Leaf_47cf70::get(unsigned int a1) { unsigned int self = (unsigned int)this; unsigned int result; unsigned int spare; result = 0xb; return result; }


class Leaf_47cf80 { public: unsigned int get(); };
unsigned int Leaf_47cf80::get() { return 0xb; }


class Leaf_47cf90 { public: unsigned int get(); };
unsigned int Leaf_47cf90::get() { return 0x148; }


extern "C" void __cdecl release_47cfa0(void*);
class Leaf_47cfa0 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_47cfa0::get(unsigned int flags) { cleanup(); if (flags & 1) release_47cfa0(this); return this; }


// Cell __fastcall CellClass::Adjacent_Cell(Cell const &cell, int facing)
// (0x0047CFC0): the cell next to cell toward an 8-way facing. The sum is the Cell member whose
// out-of-line copy is 0x0042ADD0: its inline expansions in the unit match 0x0047BFA0, and the
// Is_Near_Tunnel functions call 0x0042ADD0 where the budget runs out.

Cell __fastcall CellClass::Adjacent_Cell(Cell const &cell, int facing)
{
    return cell + AdjacentCell[facing & 7];
}


// Rect Rect::operator-(RectPoint2D const &point) const (0x0047D000): the
// rectangle moved back by point (TibSun rect.h's form: the corner moved by the point; out of line in the
// Cell.cpp unit, which sees Top_Left and the corner constructor inline under RA2_RECT_CORNER_INLINE).

Rect Rect::operator-(RectPoint2D const &point) const
{
    return Rect(Top_Left() - point, Width, Height);
}


// RectPoint2D RectPoint2D::operator-(RectPoint2D const &point) const (0x0047D040):
// member-wise difference (out of line in the Cell.cpp unit).

RectPoint2D RectPoint2D::operator-(RectPoint2D const &point) const
{
    return RectPoint2D(X - point.X, Y - point.Y);
}


extern "C" unsigned char vtable_47d090[]; extern "C" void __cdecl release_buffer_47d090(void*); extern "C" void __cdecl release_object_47d090(void*);
class Leaf_47d090 { public: void* get(unsigned int flags); };
void* Leaf_47d090::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_47d090; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_47d090((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_47d090(this); return this; }


extern "C" unsigned char table_47d0e0[]; extern "C" void* __cdecl storage_47d0e0(unsigned int);
class Storage_47d0e0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_47d0e0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_47d0e0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_47d0e0(requested << 2); owns=1; } return this; }
