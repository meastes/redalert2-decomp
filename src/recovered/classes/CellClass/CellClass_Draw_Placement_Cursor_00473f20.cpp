// bool CellClass::Draw_Placement_Cursor(Point2D const &pixel, Rect const &rect, bool drawtrans) (0x00473F20):
// draws the placement cursor over a cell the pending building or the second cursor covers, valid or blocked to
// suit, and answers whether the building may stand here (TibSun CellClass::Draw_Placement_Cursor).
#include "shared/cell.hpp"
#include "shared/building_class.hpp"
#include "shared/building_type_class.hpp"
#include "shared/display.hpp"
#include "shared/map_view.hpp"
#include "shared/palette.hpp"
#include "shared/unknown_globals.hpp"
// @ref ?TransIconset@DisplayClass@@2PBXB=00852614
// @ref ?unknown_surface_83995c@@3PAVSurface@@A=0083995c
// @ref ?unknown_drawer_8323c4@@3PAVConvertClass@@A=008323c4

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
                BuildingClass *building = (BuildingClass *)unknown_471830(Point2D(0, 0), false);
                if (building != 0 && building->Kind_Of() == 6) {
                    BuildingTypeClass *ptype = (BuildingTypeClass *)Map.unknown_11a4;
                    if (building->unknown_44d900(ptype, unknown_player_a35db4)) {
                        ok = true;
                    }
                }
            } else {
                if (Map.unknown_117c && Map.unknown_11a4 != 0 && ((ObjectTypeClass const *)Map.unknown_11a4)->Kind_Of() == 7
                        && unknown_4718b0(loco, static_cast<BuildingTypeClass const *>(Map.unknown_11a4), unknown_player_a35db4)
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
