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
// @ref ??0?$TPoint3D@H@@QAE@HHH@Z=0041b630
#include "shared/fogged_object.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/object_type.hpp"
#include "shared/building_type_class.hpp"
#include "shared/anim_type.hpp"
#include "shared/smudge_type.hpp"
#include "shared/tactical.hpp"
#include "shared/palette.hpp"
#include "shared/win32_window.hpp"
#include "shared/color_scheme.hpp"
#include "shared/house.hpp"
#include "shared/unknown_globals.hpp"
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

                cellptr->unknown_4749c0(point, rect);
                cellptr->unknown_474830(point, rect);

                cellptr->OverlayIndex = saveoverlay;
                cellptr->OverlayData = saveoverlaydata;
                break;
            }

            case 0x24: {
                CoordStruct *position = &data->Coord;
                ObjectTypeClass *type = data->DrawRecords[0].Type;
                CellClass *cellptr = &Map[CoordStruct(*position)];
                int shapenum = data->DrawRecords[0].unknown_04;
                ShapeFileStruct const *shape = (ShapeFileStruct const *)type->unknown_slot39();

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
                    ShapeFileStruct const *shape = (ShapeFileStruct const *)type->unknown_slot39();
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

                ShapeFileStruct const *shape = (ShapeFileStruct const *)SmudgeTypes[data->unknown_54]->unknown_slot39();
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
