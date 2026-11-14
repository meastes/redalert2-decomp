// fog.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/fog.json carries the addresses and the rows that
// stay in their own files.


#define RA2_RECT_POINT_INLINE
#define RA2_COORD_TPOINT3D
#define RA2_COORD_FROM_CELL_INLINE
#define RA2_TPOINT3D_DIFFERENCE
// Header-static dynamic initializers of the unit at 0x004BFCD0 and its
// list globals (FoggedObjectClass); the other drafts of this plan are emitted by this file.
// A list whose atexit thunk tests and sets a guard bit is a static data
// member (VC6 guards their destruction); its owner class is a hypothesis.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

#include "shared/fogged_object.hpp"

DynamicVectorClass<FoggedObjectClass *> FoggedObjectClass::Array;
// Array cleanup selector must be verified from the fresh CRT/object.
// Array guard at00865E54; its compiler symbol requires fresh verification.


// Dynamic initializer (0x004BFCF0, _$E5) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


// Dynamic initializer (0x004BFD20, _$E8) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


// Dynamic initializer (0x004BFD40, _$E11) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


// Dynamic initializer (0x004BFD60, _$E14) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


// Dynamic initializer (0x004BFD80, _$E17) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


// Dynamic initializer (0x004BFDA0, _$E20) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


// Dynamic initializer (0x004BFDC0, _$E23) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


// Dynamic initializer (0x004BFE00, _$E26) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


// Dynamic initializer (0x004BFE30, _$E29) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


// Dynamic initializer (0x004BFE60, _$E32) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


// Dynamic initializer (0x004BFE90, _$E35) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


// Dynamic initializer (0x004BFEA0, _$E38) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


// Dynamic initializer (0x004BFEC0; selector requires fresh verification) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


// atexit destructor (0x004BFF00; selector requires fresh verification) emitted by HeaderStatics_empty_rect_init_004bfcd0.cpp; no source of its own.


#include "shared/ini_class.hpp"
// Observed865E40 table; compiler emits initializer and guarded atexit wrapper.
IndexClass<int, FoggedObjectClass *> FoggedObjectClass::Index;


// Original004BFF90 guarded cleanup emitted naturally by FoggedObjectClass::Index.


FoggedObjectClass::FoggedObjectClass(NoInitClass const &noinit)
    : AbstractClass(noinit)
{
}


// FoggedObjectClass::FoggedObjectClass() (0x004C0020)
// An empty fogged object, listed in Array.
FoggedObjectClass::FoggedObjectClass()
    : unknown_24(-1), unknown_28(0), unknown_2c(0), unknown_30(0), Coord(0, 0, 0),
      unknown_40(0, 0, 0, 0), unknown_50(0), unknown_54(-1), unknown_58(0), unknown_74(true)
{
    Array.Add(this);
}


// FoggedObjectClass::FoggedObjectClass(CoordStruct const &coord, int arg24, int arg2c) (0x004C00F0)
// A fogged object of kind 0x14 at a coordinate: its redraw rectangle is the union of the
// cell's two bounds moved into view coordinates, and it is listed in Array and Index.
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/tactical.hpp"
FoggedObjectClass::FoggedObjectClass(CoordStruct const &coord, int arg24, int arg2c)
    : unknown_24(arg24), unknown_28(0), unknown_2c(arg2c), unknown_30(0x14), Coord(coord),
      unknown_50(Map[coord].unknown_11b), unknown_54(-1), unknown_58(0), unknown_74(true)
{
    CellClass &cell = Map[coord];
    unknown_40 = Union(cell.Overlay_Render_Rect(), cell.Overlay_Shadow_Render_Rect());
    unknown_40.X += unknown_tactical_83996c->TacPixelX;
    unknown_40.Y += unknown_tactical_83996c->TacPixelY;
    unknown_40 -= Point2D(unknown_int_839630, unknown_int_839634);
    Array.Add(this);
    Index.Add_Index(unknown_index_key(unknown_coord_cell(Coord), unknown_30), this);
}


// FoggedObjectClass::FoggedObjectClass(CoordStruct const &coord, int arg54, unsigned int arg58) (0x004C03B0)
// A fogged object of kind 0x1D standing at the cell's level height: a 60x30 redraw
// rectangle around its screen position, listed in Array and Index.
FoggedObjectClass::FoggedObjectClass(CoordStruct const &coord, int arg54, unsigned int arg58)
    : unknown_24(-1), unknown_28(0), unknown_2c(0), unknown_30(0x1D),
      unknown_50(Map[coord].unknown_11b), unknown_54(arg54), unknown_58(arg58), unknown_74(true)
{
    CellClass &cell = Map[coord];
    CoordStruct position = coord;
    position.Z = cell.unknown_11b * unknown_geometry_g;
    CoordStruct &at = Coord;
    at = position;
    Point2D pixel;
    unknown_tactical_83996c->Coord_To_Pixel(position, pixel);
    pixel -= Point2D(30, 15);
    unknown_40 = Rect(pixel.X, pixel.Y, 60, 30);
    unknown_40.X += unknown_tactical_83996c->TacPixelX;
    unknown_40.Y += unknown_tactical_83996c->TacPixelY;
    Array.Add(this);
    Index.Add_Index(unknown_index_key(XY_Cell(at.X / 256, at.Y / 256), unknown_30), this);
}


#include "shared/building_class.hpp"
#include "shared/house.hpp"
#include "shared/anim.hpp"
FoggedObjectClass::FoggedObjectClass(BuildingClass *building,bool fade)
    : unknown_24(-1),unknown_28(building->House),unknown_2c(0),unknown_30(6),Coord(building->Coord),
      unknown_50(Map[CoordStruct(building->Coord)].unknown_11b),unknown_54(-1),unknown_58(0),unknown_74(fade)
{
    unknown_40=building->Get_Render_Rect();
    Array.Add(this);
    Index.Add_Index(unknown_index_key(unknown_coord_cell(Coord),unknown_30),this);
    building->unknown_5b7=true;
    int shape=building->unknown_43be20();
    DrawRecord record;
    record.Type=building->Type;
    record.unknown_04=shape;
    if(building->Type->LaserFence && (building->unknown_4ec==12 || building->unknown_4ec==8))record.unknown_08=1;
    else {
        record.unknown_08=0;
        if(building->Type->FirestormWall)record.unknown_08=1;
    }
    DrawRecords.Add(record);
    for(int index=0;index<18;++index) {
        if(building->unknown_454[index]) {
            ((AnimClass*)building->unknown_454[index])->unknown_179=true;
            AnimTypeClass *type=((AnimClass*)building->unknown_454[index])->Class;
            unknown_40=Union(unknown_40,((AnimClass*)building->unknown_454[index])->Get_Visual_Rect());
            record.Type=type;
            record.unknown_04=type->Start+((AnimClass*)building->unknown_454[index])->Stage;
            record.unknown_08=0;
            record.unknown_0c=((AnimClass*)building->unknown_454[index])->ZAdjust;
            DrawRecords.Add(record);
        }
    }
    unknown_tactical_83996c->Register_Dirty_Area(unknown_40,false);
    unknown_40.X+=unknown_tactical_83996c->TacPixelX;
    unknown_40.Y+=unknown_tactical_83996c->TacPixelY;
}


// FoggedObjectClass::FoggedObjectClass(AnimClass *anim) (0x004C0AF0)
// A fogged copy of an animation: one draw record for its type, listed in Array and Index.
FoggedObjectClass::FoggedObjectClass(AnimClass *anim)
    : unknown_24(-1), unknown_28(0), unknown_2c(0), unknown_30(0x24), Coord(anim->Coord),
      unknown_50(Map[CoordStruct(anim->Coord)].unknown_11b), unknown_54(-1), unknown_58(0), unknown_74(true)
{
    unknown_40 = anim->Get_Render_Rect();
    unknown_40.X += unknown_tactical_83996c->TacPixelX;
    unknown_40.Y += unknown_tactical_83996c->TacPixelY;
    Array.Add(this);
    DrawRecord record;
    record.Type = anim->Class;
    DrawRecords.Add(record);
    Index.Add_Index(unknown_index_key(XY_Cell(Coord.X / 256, Coord.Y / 256), unknown_30), this);
}


// FoggedObjectClass::~FoggedObjectClass() (0x004C0DD0)
// Leaves Array and Index (the inlined Remove_Index), clears the building-fog flags of the
// building in its cell and redraws the rectangle it covered.

FoggedObjectClass::~FoggedObjectClass()
{
    Array.Delete(this);
    Cell cell = XY_Cell(Coord.X / 256, Coord.Y / 256);
    int key = unknown_index_key(cell, unknown_30);
    Index.Remove_Index(key);
    if (unknown_30 == 6) {
        BuildingClass *building = (BuildingClass *)Map[cell].Cell_Building();
        if (building != 0) {
            building->unknown_5b7 = false;
            for (int index = 0; index < 18; index++) {
                if (building->unknown_454[index] != 0) {
                    ((AnimClass *)building->unknown_454[index])->unknown_179 = false;
                }
            }
        }
    }
    if (unknown_tactical_83996c != 0) {
        Rect rect = unknown_40;
        rect -= Point2D(unknown_tactical_83996c->TacPixelX, unknown_tactical_83996c->TacPixelY);
        unknown_tactical_83996c->Register_Dirty_Area(rect, 0);
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __fastcall Draw_Fogged_Objects(Rect const &rect) (0x004C1010)
// from ra2-decomp 0x004D1890 (Draw_Fogged_Objects, src/fog.cpp; YR's tint argument dropped)
// Draws the remembered objects under the fog of war, each as the player last saw it:
// overlays (with the cell's overlay swapped in), terrain, buildings with their
// animations, and smudges.
// Unit macros (first row of the unit): RA2_RECT_POINT_INLINE, RA2_COORD_TPOINT3D,
// RA2_COORD_FROM_CELL_INLINE, RA2_TPOINT3D_DIFFERENCE.
// The animation and smudge cases read the position through a TPoint3D pointer
// (the twin's Point3D *): the CoordStruct(BASECLASS const &) conversion sets the
// copy order of their Draw_Shape arguments.
#include "shared/object_type.hpp"
#include "shared/building_type_class.hpp"
#include "shared/anim_type.hpp"
#include "shared/smudge_type.hpp"
#include "shared/palette.hpp"
#include "shared/win32_window.hpp"
#include "shared/color_scheme.hpp"
#include "shared/tool_tip.hpp"


// TibSun inline.h: the coordinate of the cell's upper left corner.
inline CoordStruct Coord_Whole(CoordStruct const &coord)
{
    int x = coord.X / 256;
    int y = coord.Y / 256;

    return(CoordStruct(x * 256, y * 256, coord.Z));
}

void __fastcall Draw_Fogged_Objects(Rect const &rect)
{
    if (MainWindow == 0) {
        return;
    }

    for (int index = 0; index < FoggedObjectClass::Index.Count(); index++) {

        FoggedObjectClass *data = FoggedObjectClass::Index.Fetch_By_Position(index);

        if (!data->unknown_74) {
            continue;
        }

        Rect rectangle = data->unknown_40;
        rectangle.X -= unknown_tactical_83996c->TacPixelX;
        rectangle.Y -= unknown_tactical_83996c->TacPixelY;
        rectangle += unknown_rect_839630.Top_Left();

        Rect clipped = Intersect(rect, rectangle);
        if (!clipped.Is_Valid()) {
            continue;
        }

        switch (data->unknown_30) {

            case 0x14: {
                CellClass *cellptr = &Map[CoordStruct(data->Coord)];

                int saveoverlay = cellptr->OverlayIndex;
                int saveoverlaydata = cellptr->OverlayData;
                cellptr->OverlayIndex = data->unknown_24;
                cellptr->OverlayData = data->unknown_2c;

                Point2D point;
                unknown_tactical_83996c->Coord_To_Pixel(Coord_Whole(cellptr->MapCoords), point);
                point.X += 60 / -2;

                cellptr->Draw_Overlay(point, rect);
                cellptr->Draw_Overlay_Shadow(point, rect);

                cellptr->OverlayIndex = saveoverlay;
                cellptr->OverlayData = saveoverlaydata;
                break;
            }

            case 0x24: {
                CoordStruct *position = &data->Coord;
                ObjectTypeClass *type = data->DrawRecords[0].Type;
                CellClass *cellptr = &Map[CoordStruct(*position)];
                int shapenum = data->DrawRecords[0].unknown_04;
                ShapeFileStruct const *shape = (ShapeFileStruct const *)type->Get_Image_Data();

                Point2D xy;
                unknown_tactical_83996c->Coord_To_Pixel(CoordStruct(*position), xy);
                xy += unknown_rect_839630.Top_Left() - rect.Top_Left();

                int zadjust = -Unknown_006A2970(CoordStruct(*position).Z);

                if (cellptr->unknown_34 == 0) {
                    cellptr->Init_Drawer(0, 0x10000, 0, 1000, 1000, 1000);
                }

                CC_Draw_Shape(unknown_surface_83995c, (ConvertClass *)cellptr->unknown_34, shape, shapenum, xy, rect, 0x4e00, 0, zadjust - 4, 2, cellptr->unknown_10c, 0, 0, Point2D(0, 0));

                if (unknown_flag_7d88d5) {
                    CC_Draw_Shape(unknown_surface_83995c, (ConvertClass *)cellptr->unknown_34, shape, shapenum + shape->Get_Count() / 2, xy, rect, 0x4601, 0, zadjust - 2, 0, 1000, 0, 0, Point2D(0, 0));
                }
                break;
            }

            case 6: {
                for (int record = 0; record < data->DrawRecords.Count(); record++) {

                    BuildingTypeClass *type = (BuildingTypeClass *)data->DrawRecords[record].Type;
                    int shapenum = data->DrawRecords[record].unknown_04;
                    ShapeFileStruct const *shape = (ShapeFileStruct const *)type->Get_Image_Data();
                    unsigned char heightadjust = data->DrawRecords[record].unknown_08;
                    ConvertClass *converter = ColorSchemes[((HouseClass *)data->unknown_28)->ColorSchemeIndex]->Converter;

                    if (record == 0) {

                        CoordStruct *position = &data->Coord;
                        Point2D drawpoint;
                        unknown_tactical_83996c->Coord_To_Pixel(CoordStruct(*position) - CoordStruct(256 / 2, 256 / 2, 0), drawpoint);
                        CellClass *cellptr = &Map[CoordStruct(*position)];
                        drawpoint += unknown_rect_839630.Top_Left() - rect.Top_Left();

                        if (type->InvisibleInGame) {
                            break;
                        }

                        int zadjust = type->NormalZAdjust;

                        Point2D shapepoint = drawpoint;
                        int height = drawpoint.Y + shape->Height / 2;
                        Rect cliprect = rect;
                        if (cliprect.Height > height) {
                            cliprect.Height = height;
                        }

                        Point2D zdrawpoint(144, 172);
                        zdrawpoint += type->ZShapePointMove;
                        Point2D zsizeoffset(type->Width() * 256 - 256, type->Foundation_Height(false) * 256 - 256);
                        zdrawpoint -= unknown_tactical_83996c->Coord_To_Pixel_Absolute(zsizeoffset);

                        ShapeFileStruct *zshapefile = unknown_building_depth_shape_85029c;

                        int zlepton = -Unknown_006A2970(CoordStruct(*position).Z);
                        int brightness = Map[CoordStruct(*position)].unknown_10a + type->ExtraLight;

                        if (cellptr->unknown_34 == 0) {
                            cellptr->Init_Drawer(0, 0x10000, 0, 1000, 1000, 1000);
                        }
                        ConvertClass *drawconvert = type->TerrainPalette ? (ConvertClass *)cellptr->unknown_34 : converter;

                        if (cliprect.Height > 0) {
                            if (heightadjust) {
                                CC_Draw_Shape(unknown_surface_83995c, drawconvert, shape, shapenum, shapepoint, cliprect, 0x4e00, 0, zlepton - 2, 0, brightness, 0, 0, Point2D(0, 0));
                                CC_Draw_Shape(unknown_surface_83995c, drawconvert, shape, shapenum + shape->Get_Count() / 2, shapepoint, cliprect, 0x4e01, 0, zlepton - 2, 0, 1000, 0, 0, Point2D(0, 0));
                            } else {
                                CC_Draw_Shape(unknown_surface_83995c, drawconvert, shape, shapenum, shapepoint, cliprect, 0x4e00, 0, zlepton + zadjust - 2, 2, brightness, zshapefile, 0, zdrawpoint);
                                CC_Draw_Shape(unknown_surface_83995c, drawconvert, shape, shapenum + shape->Get_Count() / 2, shapepoint, cliprect, 0x4e01, 0, zlepton - 2, 0, 1000, 0, 0, Point2D(0, 0));
                            }
                        }

                        if (type->unknown_11cc != 0) {
                            CC_Draw_Shape(unknown_surface_83995c, drawconvert, (ShapeFileStruct const *)type->unknown_11cc, shapenum, drawpoint, rect, 0x4e00, 0, zlepton - 3, 0, brightness, 0, 0, Point2D(0, 0));
                        }

                    } else {

                        AnimTypeClass *anim = (AnimTypeClass *)type;

                        if (anim != 0 && anim->Kind_Of() == 5 && anim->DetailLevel <= unknown_a40b2c && shape != 0) {

                            TPoint3D<int> *position = &data->Coord;
                            Point2D drawpoint;
                            unknown_tactical_83996c->Coord_To_Pixel(CoordStruct(*position) - CoordStruct(256 / 2, 256 / 2, 0), drawpoint);
                            drawpoint += unknown_rect_839630.Top_Left() - rect.Top_Left();

                            int brightness = 1000;
                            if (!anim->UseNormalLight) {
                                brightness = Map[CoordStruct(*position)].unknown_10a;
                            }

                            ConvertClass *animconvert;
                            if (converter != 0 && anim->ShouldUseCellDrawer) {
                                animconvert = converter;
                            } else {
                                animconvert = Unit_Draw_Convert_008323C0;
                            }

                            int zadjust = data->DrawRecords[record].unknown_0c;

                            if (anim->Flat) {
                                CC_Draw_Shape(unknown_surface_83995c, animconvert, shape, shapenum, drawpoint, rect, 0x2e00, 0, zadjust - Unknown_006A2970(CoordStruct(*position).Z) - 2, 0, brightness, 0, 0, Point2D(0, 0));
                            } else {
                                CC_Draw_Shape(unknown_surface_83995c, animconvert, shape, shapenum, drawpoint, rect, 0x2e00, 0, zadjust - Unknown_006A2970(CoordStruct(*position).Z) - 2, 2, brightness, 0, 0, Point2D(0, 0));
                            }
                        }
                    }
                }
                break;
            }

            case 0x1d: {
                Point2D point = data->unknown_40.Top_Left() - RectPoint2D(unknown_tactical_83996c->TacPixelX, unknown_tactical_83996c->TacPixelY) + (unknown_rect_839630.Top_Left() + RectPoint2D(60 / 2, 0)) - rect.Top_Left();

                ShapeFileStruct const *shape = (ShapeFileStruct const *)SmudgeTypes[data->unknown_54]->Get_Image_Data();
                if (shape != 0) {
                    TPoint3D<int> *position = &data->Coord;
                    CellClass *cellptr = &Map[CoordStruct(*position)];
                    int brightness = cellptr->unknown_10c;
                    CC_Draw_Shape(unknown_surface_83995c, (ConvertClass *)cellptr->unknown_34, shape, data->unknown_58, point, rect, 0xe00, 0, -Unknown_006A2970(CoordStruct(*position).Z), 2, brightness, 0, 0, Point2D(0, 0));
                }
                break;
            }

            default:
                break;
        }
    }
}


// void FoggedObjectClass::unknown_4c1ae0() (0x004C1AE0)
// Marks (+0x140 bit 22) the cell of every listed fogged object: each cell a fogged
// building covers (its cell plus the type's cell offsets), or the cell under the coordinate.

void FoggedObjectClass::unknown_4c1ae0()
{
    for (int index = 0; index < Array.Count(); index++) {
        FoggedObjectClass *fogged = Array[index];
        if (fogged->unknown_4c1f00() != 0) {
            Cell const *offset = fogged->unknown_4c1f00();
            Cell base = fogged->Get_Cell();
            for (; offset->X != 0x7FFF || offset->Y != 0x7FFF; offset++) {
                Cell cell = Cell(offset->X + base.X, offset->Y + base.Y);
                Map[cell].unknown_140 |= 0x400000;
            }
        } else {
            CoordStruct coord = fogged->Coord;
            Map[coord].unknown_140 |= 0x400000;
        }
    }
}


// HRESULT __stdcall FoggedObjectClass::Save(IStream *stream, BOOL clear_dirty) (0x004C1C10)
// Saves the base object, then the draw record count and each 16-byte draw record.

HRESULT __stdcall FoggedObjectClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        int count = DrawRecords.Count();
        result = stream->Write(&count, sizeof(count), 0);
        if (SUCCEEDED(result)) {
            for (int index = 0; index < count; index++) {
                result = stream->Write(&DrawRecords[index], sizeof(DrawRecord), 0);
                if (FAILED(result)) {
                    return S_OK;
                }
            }
            return S_OK;
        }
    }
    return result;
}


// HRESULT __stdcall FoggedObjectClass::Load(IStream *stream) (0x004C1C80)
// Loads the base object, rebuilds the object in place, then reads the draw record
// count and records and lists the object in Index under its cell.

HRESULT __stdcall FoggedObjectClass::Load(IStream *stream)
{
    HRESULT result = AbstractClass::Load(stream);
    if (result >= 0) {
        new (this) FoggedObjectClass(NoInitClass());
        Swizzle_Pointer(unknown_28);
        int count = 0;
        result = stream->Read(&count, sizeof(count), 0);
        if (result < 0) {
            return result;
        }
        for (int index = 0; index < count; index++) {
            DrawRecord record;
            result = stream->Read(&record, sizeof(record), 0);
            if (result < 0) {
                return result;
            }
            DrawRecords.Add(record);
            Swizzle_Pointer(DrawRecords[DrawRecords.Count() - 1].Type);
        }
        Cell cell = Get_Cell();
        Index.Add_Index(unknown_index_key(cell, unknown_30), this);
    }
    return result;
}


// Cell const *FoggedObjectClass::unknown_4c1f00() const (0x004C1F00): the
// cells a fogged building covers (its type's slot 36 list); null for objects
// of other kinds.

Cell const *FoggedObjectClass::unknown_4c1f00() const
{
    if (unknown_30 == 6) {
        return DrawRecords[0].Type->Occupy_List(0);
    }
    return 0;
}


// ?Kind_Of@FoggedObjectClass@@UBEHXZ (0x004C1F20)
// RTTI kind 0x3D.

int FoggedObjectClass::Kind_Of() const
{
    return 0x3D;
}


// ?Size_Of@FoggedObjectClass@@UBEHXZ (0x004C1F30)
// Object size.

int FoggedObjectClass::Size_Of() const
{
    return sizeof(FoggedObjectClass);
}


// ?GetClassID@FoggedObjectClass@@UAGJPAUGUID@@@Z (0x004C1F40)
// The class identifier.

HRESULT __stdcall FoggedObjectClass::GetClassID(GUID *classid)
{
    if (classid == 0) {
        return E_POINTER;
    }
    *classid = CLSID_FoggedObjectClass;
    return S_OK;
}


// Naturally emitted by the shared VectorClass/DrawRecord template context
// of this cumulative Fogged unit; no explicit helper call or replacement.


// Naturally emitted by the shared VectorClass/DrawRecord template context
// of this cumulative Fogged unit; no explicit helper call or replacement.


// Naturally emitted by the shared VectorClass/DrawRecord template context
// of this cumulative Fogged unit; no explicit helper call or replacement.


// Naturally emitted by the shared VectorClass/DrawRecord template context
// of this cumulative Fogged unit; no explicit helper call or replacement.


class Call_4c1f80_0 {public: unsigned int Compute_CRC_004101b0(unsigned int crc_engine_argument);};
class Call_4c1f80_1 {public: unsigned int target(unsigned int field_value);};
class Call_4c1f80_2 {public: unsigned int target(unsigned int field_value);};
class Call_4c1f80_3 {public: unsigned int target(unsigned int field_value);};
class Call_4c1f80_4 {public: unsigned int target(unsigned int field_value);};
class Call_4c1f80_5 {public: unsigned int target(unsigned int field_value);};
class Call_4c1f80_6 {public: unsigned int target(unsigned int field_value);};
class Call_4c1f80_7 {public: unsigned int target(unsigned int field_value);};
class Call_4c1f80_8 {public: unsigned int target(unsigned int field_value);};
class Call_4c1f80_9 {public: unsigned int target(unsigned int field_value);};
class Call_4c1f80_10 {public: unsigned int target(unsigned int field_value);};
class Call_4c1f80_11 {public: unsigned int target(unsigned int field_value);};
class Call_4c1f80_12 {public: unsigned int target(unsigned int field_value);};
class Call_4c1f80_13 {public: unsigned int target(unsigned int field_value);};
class Call_4c1f80_14 {public: unsigned int target(unsigned int field_value);};
class Leaf_4c1f80 { public: unsigned int get(unsigned int crc_engine_address); };
unsigned int Leaf_4c1f80::get(unsigned int crc_engine_address) { unsigned int unused_base_return = ((Call_4c1f80_0*)((unsigned int)this))->Compute_CRC_004101b0((unsigned int)(crc_engine_address)); unsigned int unused_field_24_return = ((Call_4c1f80_1*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x24))); unsigned int unused_field_2c_return = ((Call_4c1f80_2*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x2c))); unsigned int unused_field_30_return = ((Call_4c1f80_3*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x30))); unsigned int unused_field_34_return = ((Call_4c1f80_4*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x34))); unsigned int unused_field_38_return = ((Call_4c1f80_5*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x38))); unsigned int unused_field_3c_return = ((Call_4c1f80_6*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x3c))); unsigned int unused_field_40_return = ((Call_4c1f80_7*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x40))); unsigned int unused_field_44_return = ((Call_4c1f80_8*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x44))); unsigned int unused_field_48_return = ((Call_4c1f80_9*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x48))); unsigned int unused_field_4c_return = ((Call_4c1f80_10*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x4c))); unsigned int unused_field_50_return = ((Call_4c1f80_11*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x50))); unsigned int unused_field_54_return = ((Call_4c1f80_12*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x54))); unsigned int unused_field_58_return = ((Call_4c1f80_13*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x58))); unsigned int field_6c_return = ((Call_4c1f80_14*)(crc_engine_address))->target((unsigned int)(*(unsigned int*)(((unsigned int)this) + 0x6c))); return field_6c_return; }


// 0x004c2030: function not yet identified
//
// Body: 16 bytes (frozen inventory bound).
// Referenced from 2 sites (unconditional call).
// Ghidra prototype: undefined4 __fastcall FUN_004c2030(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004c2030 {

unsigned int __fastcall FUN_004c2030(int object_address)
{
    if (0 < *(int *)(object_address + 0x6c)) {
        return **(unsigned int **)(object_address + 0x60);
    }
    return 0;
}

}  // namespace seed_004c2030


extern "C" void __cdecl release_4c2080(void*);
class Leaf_4c2080 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_4c2080::get(unsigned int flags) { cleanup(); if (flags & 1) release_4c2080(this); return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<FoggedObjectClass::DrawRecord>::Clear (0x004C20A0; DynamicVectorClass<FoggedObjectClass::DrawRecord> vtable 0x007a11b0 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// DynamicVectorClass<FoggedObjectClass::DrawRecord>::ID_pointer (0x004C20D0; DynamicVectorClass<FoggedObjectClass::DrawRecord> vtable 0x007a11b0 slot 5).
// FoggedObjectClass::DrawRecord is a size-only stand-in (16 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// VectorClass<FoggedObjectClass::DrawRecord>::Clear (0x004C2290; VectorClass<FoggedObjectClass::DrawRecord> vtable 0x007a11d0 slot 3).
// The element type is left incomplete: Clear does not depend on it.
// VectorClass<FoggedObjectClass::DrawRecord>::ID_object (0x004C22C0; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp. DrawRecord as shared/fogged_object.hpp declares it (that header's
// statics would add an initializer to this instance unit); the comparison is TibSun fog.h's (type, then frame).
// VectorClass<FoggedObjectClass::DrawRecord>::ID_pointer (0x004C2310; VectorClass<FoggedObjectClass::DrawRecord> vtable 0x007a11d0 slot 5).
// FoggedObjectClass::DrawRecord is a size-only stand-in (16 bytes, from the ID(T const *) divisor);
// its real fields are not recovered.
// DynamicVectorClass<FoggedObjectClass::DrawRecord>::ID_object (0x004C2440; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp. DrawRecord as shared/fogged_object.hpp declares it (that header's
// statics would add an initializer to this instance unit); the comparison is TibSun fog.h's (type, then frame).


extern "C" unsigned char vtable_4c2480[]; extern "C" void __cdecl release_buffer_4c2480(void*); extern "C" void __cdecl release_object_4c2480(void*);
class Leaf_4c2480 { public: void* get(unsigned int flags); };
void* Leaf_4c2480::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4c2480; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4c2480((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4c2480(this); return this; }


extern "C" unsigned char vtable_4c24d0[]; extern "C" void __cdecl release_buffer_4c24d0(void*); extern "C" void __cdecl release_object_4c24d0(void*);
class Leaf_4c24d0 { public: void* get(unsigned int flags); };
void* Leaf_4c24d0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_4c24d0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_4c24d0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_4c24d0(this); return this; }


// IndexClass<int, FoggedObjectClass *>::Increase_Table_Size (0x004C25B0)
// Template member defined in shared/ini_class.hpp; the Add_Index expansion of the 0x004C00F0
// constructor calls it out of line (the other expansions inline it), so this unit
// instantiates it. Replaces the UnknownGrowArray<UnknownGrowElement8> view of the same bytes.


// IndexClass<int, FoggedObjectClass *>::search_compfunc (0x004C2640)
// Template member defined in shared/ini_class.hpp; the Remove_Index and Load expansions in
// this unit instantiate it. No source of its own.


// Binary_Search<IndexClass<int, FoggedObjectClass *>::NodeElement> (0x004C2670)
// Fastcall binary search over sorted index records. Template instantiation; the
// definition is in shared/ini_class.hpp and this unit's Remove_Index expansion
// instantiates it.
