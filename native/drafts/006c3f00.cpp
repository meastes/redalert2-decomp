// NON-EXACT DRAFT 0x6C3F00 TechnoClass::unknown_slot248 (health bar)
// entry 0x6C3F00; exact-match score 58.26 (misses_index best), this text 58.26; plan "draft as-is"
// origin lane: Scout1059 (build/class-work/Scout1059/keep/hb2.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1098 (native/drafts.json). Not original bytes.
// void TechnoClass::unknown_slot248(Point2D const &point, Rect const &window, bool arg3) (0x006C3F00)
// Draws the health bar: buildings get a diagonal pip bar sized from the
// type's lepton dimensions, others the select bracket and a pip row; then the
// pips (slot 249) for allied or spied-on objects.
#include "shared/techno.hpp"
#include "shared/object_type.hpp"
#include "shared/palette.hpp"
#include "shared/gadget.hpp"
#include "shared/gscreen.hpp"
#include "shared/tactical.hpp"
#include "shared/rules.hpp"
#include "shared/selection_coord.hpp"

void TechnoClass::unknown_slot248(Point2D const &point, Rect const &window, bool arg3)
{
    if (Kind_Of() == 6) {
        Get_Height_AGL();
        CoordStruct dim = Techno_Type_Class()->Lepton_Dimensions();
        CoordStruct coord = dim - dim / 2;
        coord.X = -coord.X;
        coord.Z = dim.Z;
        Point2D p0 = unknown_tactical_83996c->Coord_To_Pixel_Absolute(coord);
        coord.X = -coord.X;
        coord.Y = -coord.Y;
        coord.X = -coord.X;
        Point2D p1 = unknown_tactical_83996c->Coord_To_Pixel_Absolute(coord);
        coord.Z = 0;
        coord.Y = -coord.Y;
        Point2D p2 = unknown_tactical_83996c->Coord_To_Pixel_Absolute(coord);
        coord.X = -coord.X;
        coord.Y = -coord.Y;
        coord.X = -coord.X;

        int length = (p0.Y - p1.Y) / 2;
        int count = (int)(Health_Ratio() * length);
        if (count <= 1) {
            count = 1;
        }
        if (count >= length) {
            count = length;
        }
        int frame = 1;
        if ((char)unknown_func_5d4600()) {
            frame = 2;
        } else if ((char)unknown_func_5d45b0()) {
            frame = 4;
        }
        int ybase = 2 - 2 * length;
        Point2D position;
        int index;
        int yoff = 0;
        int xoff = 0;
        for (index = 0; index < count; index++) {
            position.X = point.X + p0.X + 4 * length + 3;
            position.Y = point.Y + p0.Y + ybase + 2;
            position.X -= xoff;
            position.Y -= yoff;
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)unknown_pips_shapes_a72fe4,
                          frame, position, window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            xoff += 4;
            yoff -= 2;
        }
        yoff = -2 * count;
        xoff = 4 * count;
        for (index = count; index < length; index++) {
            position.X = point.X + p0.X + 4 * length + 3;
            position.Y = point.Y + p0.Y + ybase + 2;
            position.X -= xoff;
            position.Y -= yoff;
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)unknown_pips_shapes_a72fe4,
                          0, position, window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            xoff += 4;
            yoff -= 2;
        }
        if (House->Is_Ally(unknown_player_a35db4) || (unknown_1a8 & (1 << unknown_player_a35db4->Type->House))) {
            unknown_slot249(point + p2, point, window);
        }
    } else {
        Point2D offset;
        int length;
        if (Kind_Of() == 15) {
            if (unknown_77) {
                CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)unknown_pipbrd_shapes_a72fe0,
                              1, point + Point2D(11, -25), window, 0xE00, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
            offset = Point2D(-5, -24);
            length = 8;
        } else {
            if (unknown_77) {
                CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)unknown_pipbrd_shapes_a72fe0,
                              0, point + Point2D(1, -26), window, 0xE00, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
            }
            offset = Point2D(-15, -25);
            length = 17;
        }
        int count = (int)(Health_Ratio() * length);
        if (count <= 1) {
            count = 1;
        }
        if (count >= length) {
            count = length;
        }
        int frame = 15;
        if (Health_Ratio() <= Rules->unknown_1368) {
            frame = 16;
        }
        if (Health_Ratio() <= Rules->unknown_1370) {
            frame = 17;
        }
        Point2D position;
        for (int index = 0; index < count; index++) {
            position.X = point.X + offset.X;
            position.Y = offset.Y + point.Y;
            position.X += 2 * index;
            CC_Draw_Shape(unknown_surface_83995c, unknown_drawer_8323c4, (ShapeFileStruct const *)unknown_pips_shapes_a72fe4,
                          frame, position, window, 0x600, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        }
        if (House->Is_Ally(unknown_player_a35db4) || (unknown_1a8 & (1 << unknown_player_a35db4->Type->House))) {
            unknown_slot249(point + Point2D(-10, 10), point, window);
        }
    }
}
