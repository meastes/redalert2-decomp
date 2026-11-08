// NON-EXACT DRAFT 0x4BBAF0 FUN_004bbaf0
// entry 0x4BBAF0; exact-match score 97.38 (misses_index best); plan "draft as-is"
// origin lane: Scout535 (build/class-work/Scout535/w/best_4bbaf0_97.38.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): only the entry's function kept (the unit copy's neighbours are accepted rows); each changed line is marked NATIVE.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/cell_class.hpp"
#include "shared/cell.hpp"
#include "shared/weapon_type.hpp"
#include "shared/bullet_type.hpp"
#include "shared/overlay_type.hpp"
#include "shared/house.hpp"
#include "shared/rules.hpp"
#include "shared/map_view.hpp"
#include "shared/fast_math.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ??AMapClass@@QAEAAVCellClass@@ABVCell@@@Z=00548070
// @ref __ftol=0077f110
// @ref ?Fast_Sqrt@@YAMN@Z=004ba410
// @ref ?OverlayTypes@@=00a35de8
// @ref ?Houses@@=00a32298
// @ref ?Rules@@=00839848
// @ref ?Is_Ally@HouseClass@@QBE_NPBV1@@Z=004e5540
// @ref ?Cell_Effective_Height_0047cf10@@=0047cf10
extern int __fastcall Cell_Effective_Height_0047cf10(CellClass const *cell);
CellClass *__fastcall Get_Obstacle_004bbaf0(CellClass *source, CellClass *target, CellClass *last,
    CoordStruct coord, BulletTypeClass const *type, HouseClass const *house);
int __fastcall Cell_Height_Transition_004bbe10(CellClass const *origin, CellClass const *nearby, CellClass const *destination);
bool __fastcall Cell_Wall_Transition_004bbe60(CellClass const *origin, CellClass const *nearby, CellClass const *destination, HouseClass const *house);

CellClass *__fastcall Find_First_Obstacle_004bb890(CoordStruct const &from, CoordStruct const &to,
    BulletTypeClass const *type, HouseClass const *house);


CellClass *__fastcall Get_Obstacle_004bbaf0(CellClass *source, CellClass *target, CellClass *last,
    CoordStruct coord, BulletTypeClass const *type, HouseClass const *house)
{
    Cell cellpos(coord);
    CellClass *cell = &Map[cellpos];
    if ((type->SubjectToCliffs && Cell_Height_Transition_004bbe10(source, last, cell))
        || (type->SubjectToWalls && Cell_Wall_Transition_004bbe60(source, cell, target, house))) {
        CoordStruct center = cell->Center_Coord();
        CoordStruct delta = coord - center;
        int distance = (int)Fast_Sqrt((double)delta.X * (double)delta.X + (double)delta.Z * (double)delta.Z
            + (double)delta.Y * (double)delta.Y);
        if (distance > 0x55) {
            CoordStruct from = source->Center_Coord();
            CoordStruct to = target->Center_Coord();
            int xdist = abs(from.X - to.X);
            int ydist = abs(from.Y - to.Y);
            int xdelta = abs(coord.X - center.X);
            int ydelta = abs(coord.Y - center.Y);
            if ((ydist < xdist && ydelta > xdelta) || (ydist > xdist && xdelta > ydelta)) {
                CellClass *again = &Map[cellpos];
                if (type->SubjectToCliffs) {
                    int lastheight = Cell_Effective_Height_0047cf10(last);
                    if (Cell_Effective_Height_0047cf10(again) - lastheight >= 4) {
                        int sourceheight = Cell_Effective_Height_0047cf10(source);
                        if (Cell_Effective_Height_0047cf10(again) - sourceheight > 0) return cell;
                    }
                }
                if (type->SubjectToWalls && Cell_Wall_Transition_004bbe60(source, again, target, house)) {
                    return cell;
                }
                return 0;
            }
            return cell;
        }
        return cell;
    }
    return 0;
}



