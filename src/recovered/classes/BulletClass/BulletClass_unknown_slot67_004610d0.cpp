// BulletClass slot 67 0x004610D0 (Draw_It: TibSun bullet.cpp shape; Velocity Get_Yaw/Get_Pitch DirType accessors); ABI legacy address arguments.
#include "shared/bullet.hpp"
#include "shared/bullet_type.hpp"
#include "shared/matrix3d.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
#include "shared/house.hpp"
#include "shared/palette.hpp"
#include "shared/color_scheme.hpp"
extern ConvertClass *bullet_normal_drawer;
extern ConvertClass *bullet_anim_drawer;
extern int bullet_bridge_height;
Matrix3D __fastcall Bullet_Isometric_View();
int __fastcall Bullet_Height_Pixel(int height);
// @ref ?bullet_normal_drawer@@3PAVConvertClass@@A=008323c4
// @ref ?bullet_anim_drawer@@3PAVConvertClass@@A=008323c0
// @ref ?bullet_bridge_height@@3HA=00850338
// @ref ?Bullet_Isometric_View@@YI?AVMatrix3D@@XZ=00717f90
// @ref ?Bullet_Height_Pixel@@YIHH@Z=006a2970
void BulletClass::unknown_slot67(int point_address, int clip_address)
{
    Point2D const &point = *(Point2D const *)point_address;
    Rect const &clip = *(Rect const *)clip_address;
    if ((unknown_id_source_a3d290->unknown_000 & 0x1000) && Map.Unknown_005683C0(CoordStruct(Coord))) return;
    if (Type->Inviso || SpawnNextAnim) return;
    if (Type->unknown_206) {
        Matrix3D matrix;
        matrix.Make_Identity();
        matrix.Rotate_Z(Velocity.Get_Yaw().As_Radian32());
        matrix.Rotate_Y(-Velocity.Get_Pitch().As_Radian32());
        Draw_Voxel_00463a00(Type->unknown_b0, Bullet_Isometric_View() * matrix, *(RectPoint2D const *)point_address, clip, 0, 0x800, 1000);
        return;
    }
    ShapeFileStruct const *image = (ShapeFileStruct const *)unknown_slot26();
    if (!image) return;
    int shape = unknown_slot116();
    int height_agl = unknown_slot108();
    int ground_height = Map.Unknown_0055A280(CoordStruct(Coord));
    if (!unknown_79 && (Map[CoordStruct(Coord)].unknown_140 & 0x100) && height_agl >= bullet_bridge_height) {
        height_agl -= bullet_bridge_height;
        ground_height += bullet_bridge_height;
    }
    if (height_agl > 0 && Type->Shadow) {
        Point2D shadow = point + Point2D(0, Bullet_Height_Pixel(height_agl));
        CC_Draw_Shape(unknown_surface_83995c, bullet_normal_drawer, image, shape, shadow, clip, 0x2601, 0, -10 - Bullet_Height_Pixel(ground_height), 0, 1000, 0, 0, Point2D(0, 0));
    }
    ConvertClass *drawer = bullet_normal_drawer;
    if (Type->AnimPalette) drawer = bullet_anim_drawer;
    else if (Type->FirersPalette) {
        if (InheritedColor != -1) drawer = ColorSchemes[InheritedColor]->Converter;
        else drawer = ColorSchemes[unknown_player_a35db4->ColorSchemeIndex]->Converter;
    }
    CC_Draw_Shape(unknown_surface_83995c, drawer, image, shape, point, clip, 0x2e00, 0, -30 - Bullet_Height_Pixel(unknown_slot110()), 0, 1000, 0, 0, Point2D(0, 0));
}
