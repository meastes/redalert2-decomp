// Slot68 keeps its established address-word ABI until the shared virtual interface is retyped.
#include "shared/building.hpp"
#include "shared/building_map.hpp"
#include "shared/cell.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_static_rect
#include "shared/shape_file.hpp"
// @ref ?Get_Object@UnknownBuildingFactory@@QAEPAVTechnoClass@@XZ=004b9930
void BuildingClass::unknown_slot68(Point2D const &point_ref, Rect const &bounds)
{
    Cell cell = unknown_slot104();
    ShapeFileStruct const *shape = (ShapeFileStruct const *)unknown_slot26();
    if (shape == 0) return;
    if (Type->InvisibleInGame) return;
    if (Get_Mission() != 24 || unknown_2a8.Is_Count_Down_Complete()) {
        if (Get_Mission() == 16 && Type->unknown_11a0 != 0) {
            TechnoClass *product = ((UnknownBuildingFactory *)unknown_41c)->Get_Object();
            if (product != 0 && product->unknown_slot32()->JumpJet) shape = (ShapeFileStruct const *)Type->unknown_11b0;
            else shape = (ShapeFileStruct const *)Type->unknown_11a0;
        }
        Point2D point = point_ref;
        short shape_height = shape->Get_Height();
        int height = point.Y - shape_height / 2;
        Rect clip = bounds;
        if (clip.Height > height) clip.Height = height;
        if (clip.Height > 0) {
            unknown_6d1510(shape, unknown_43be20(), &point, &clip, 0, 256,
                -2 - Unknown_006A2970(unknown_slot110()), 2, 0, Map[cell].unknown_10a, 0, 0, Point2D(0, 0), 0);
        }
        clip = bounds;
        clip.Y += height;
        clip.Height = shape_height;
        Point2D lower_point = point;
        lower_point.Y -= height;
        if (clip.Y + clip.Height > bounds.Y + bounds.Height) clip.Height = bounds.Y + bounds.Height - clip.Y;
        if (clip.Y < bounds.Y) {
            clip.Height += clip.Y - bounds.Y;
            lower_point.Y += clip.Y - bounds.Y;
            clip.Y = bounds.Y;
        }
        if (clip.Height > 0) {
            unknown_6d1510(shape, unknown_43be20(), &lower_point, &clip, 0, 256,
                -Unknown_006A2970(unknown_slot110()), 0, 0, Map[cell].unknown_10a, 0, 0, Point2D(0, 0), 0);
        }
    }
}
