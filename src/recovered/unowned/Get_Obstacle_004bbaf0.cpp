// CellClass *Get_Obstacle_004bbaf0(...) (0x004BBAF0): the cell at coord when it blocks the projectile type (a cliff rise
// or a wall), unless coord lies near the cell's centre off the line's main axis and the cell passes the same tests again.
// Adapted from ra2-decomp src/findpath2.cpp TrajectoryHelper::GetObstacle (gamemd twin 0x004CC360, matched there);
// see THIRD_PARTY.md.
// Declaration-count sensitive: exact only with the SDK headers and the cell.h screen chain ahead of CellClass
// (RA2_SDK_HEADERS unit); margin measured with plain-prototype filler before cell_class.hpp: +2,048 still exact,
// +4,096 misses (97.00). On the fragile-probe list.
#include "shared/cell_class.hpp"
#include "shared/cell.hpp"
#include "shared/bullet_type.hpp"
#include "shared/house.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ??AMapClass@@QAEAAVCellClass@@ABVCell@@@Z=00548070
// @ref ?Fast_Sqrt@@YAMN@Z=004ba410
// @ref ?Cell_Height_Transition_004bbe10@@YIHPBVCellClass@@00@Z=004bbe10
// @ref ?Cell_Wall_Transition_004bbe60@@YI_NPBVCellClass@@00PBVHouseClass@@@Z=004bbe60

int __fastcall Cell_Height_Transition_004bbe10(CellClass const *origin, CellClass const *nearby, CellClass const *destination);
bool __fastcall Cell_Wall_Transition_004bbe60(CellClass const *origin, CellClass const *nearby, CellClass const *destination, HouseClass const *house);

CellClass *__fastcall Get_Obstacle_004bbaf0(CellClass *source, CellClass *target, CellClass *last,
    CoordStruct coord, BulletTypeClass const *type, HouseClass const *house)
{
    Cell cell = Cell(coord);
    CellClass *first = &Map[cell];
    if ((type->SubjectToCliffs && Cell_Height_Transition_004bbe10(source, last, first)) ||
        (type->SubjectToWalls && Cell_Wall_Transition_004bbe60(source, first, target, house))) {
        CoordStruct center = first->Center_Coord();
        if (coord.Distance_To(center) > 256 / 3) {
            CoordStruct from = source->Center_Coord();
            CoordStruct to = target->Center_Coord();
            int dx = abs(from.X - to.X);
            int dy = abs(from.Y - to.Y);
            int ox = abs(coord.X - center.X);
            int oy = abs(coord.Y - center.Y);
            if ((dx > dy && oy > ox) || (dy > dx && ox > oy)) {
                CellClass *check = &Map[cell];
                if (type->SubjectToCliffs && Cell_Height_Transition_004bbe10(source, last, check)) {
                    return first;
                }
                if (type->SubjectToWalls && Cell_Wall_Transition_004bbe60(source, check, target, house)) {
                    return first;
                }
                return 0;
            }
            return first;
        }
        return first;
    }
    return 0;
}
