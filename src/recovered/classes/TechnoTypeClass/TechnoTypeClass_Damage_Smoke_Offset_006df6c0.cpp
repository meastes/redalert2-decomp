// CoordStruct TechnoTypeClass::Damage_Smoke_Offset() const (0x006DF6C0)
// The damage smoke offset; when it is given relative to the screen
// (DamSmkOffScrnRel) its x/y are converted through the tactical view and
// its height (in tenths) is applied as a diagonal shift and a lift.
#include "shared/techno_type.hpp"
#include "shared/fast_math.hpp"
#include "shared/tactical.hpp"
// @ref ?unknown_tactical_83996c@@3PAVTactical@@A=0083996c
// @ref ?Unknown_006A2900@Tactical@@QAE?AUPoint2D@@ABU2@@Z=006a2900
// @ref ?Fast_Sqrt@@YAMN@Z=004ba410
// @ref __real@8@3ffe947ae147ae147800=007ad168
// @ref __ftol=0077f110

// The projected screen point as a coordinate on the ground plane (the same
// inline helper the BuildingClass fire-coordinate units use, 0x0043BBD0).
static inline CoordStruct Projected_Coord_006df6c0(Point2D const &point) { return CoordStruct(point.X, point.Y, 0); }

CoordStruct TechnoTypeClass::Damage_Smoke_Offset() const
{
    if (DamSmkOffScrnRel) {
        Point2D screen = Point2D(DamageSmokeOffset.X, DamageSmokeOffset.Y);
        CoordStruct offset = Projected_Coord_006df6c0(unknown_tactical_83996c->Unknown_006A2900(screen));
        int height = DamageSmokeOffset.Z * 10;
        offset.X -= height;
        offset.Y -= height;
        double side = height;
        int lift = (int)(Fast_Sqrt(side * side + side * side) * 0.58);
        if (DamageSmokeOffset.Z < 0) {
            offset.Z = lift;
        } else {
            offset.Z -= lift;
        }
        return offset;
    }
    return DamageSmokeOffset;
}
