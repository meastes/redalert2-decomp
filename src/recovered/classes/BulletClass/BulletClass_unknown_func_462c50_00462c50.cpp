// BulletClass 0x00462C50 (shrapnel)
#include "shared/bullet.hpp"
#include "shared/bullet_type.hpp"
#include "shared/weapon_type.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/infantry.hpp"
#include "shared/techno.hpp"
#include "shared/house.hpp"
#include "shared/random.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/map_cell_view.hpp"

// The first index of each ring in unknown_cells_a6f248 (0x007A5908).
extern int const unknown_radius_starts_7a5908[];
// Cell offsets in rings of growing radius (0x00A6F248; MapClass unit).
extern Cell unknown_cells_a6f248[];

// Hypothesis (Inline156): an inline accessor returning the object coordinate by value; its sites give
// the original inline pattern (each Coord copy is a pending site under the depth-first rule).
inline CoordStruct Coord_Of_462c50(ObjectClass const *o) { return o->Coord; }

// Westwood's Direction(Coord const &, Coord const &) (TibSun inline.h): only the X members get locals,
// and the angle becomes a DirType.
inline DirType Coord_Direction_462c50(CoordStruct const &coord1, CoordStruct const &coord2)
{
    int x1 = coord1.X;
    int x2 = coord2.X;
    return(DirType(Fast_Atan2((double)coord1.Y - (double)coord2.Y, (double)x2 - (double)x1)));
}

void BulletClass::unknown_func_462c50()
{
    int shrapnel = Type->ShrapnelCount;
    int count;
    if (shrapnel < 0) {
        if (Owner == 0) {
            count = 3;
        } else {
            CoordStruct coord = Coord_Of_462c50(this);
            UnknownVector3 offset = Coord_Offset(Owner->Center_Coord(), coord);
            count = -shrapnel - offset.Length() / 256;
            if (count <= 0) return;
        }
    } else {
        count = shrapnel;
    }
    if (static_cast<CellClass *>(unknown_slot105())->OccupierPtr == 0) return;
    CellClass *center = static_cast<CellClass *>(unknown_slot105());
    if (center->OccupierPtr->Kind_Of() == 6) return;
    WeaponTypeClass *weapon = Type->ShrapnelWeapon;
    BulletTypeClass *projectile = weapon->Projectile;
    Cell origin = unknown_slot104();
    int range = weapon->unknown_b4 / 256;
    int fired = 0;
    // One velocity for both loops, zeroed with Set() before each fragment (TibSun building.cpp idiom).
    TVelocity3D<double> velocity;
    for (int radius = 1; radius <= range; radius++) {
        int end = unknown_radius_starts_7a5908[radius];
        for (int index = unknown_radius_starts_7a5908[radius - 1]; index < end; index++) {
            CellClass *cellptr = &Map[unknown_cells_a6f248[index] + origin];
            ObjectClass *target = cellptr->OccupierPtr;
            if (target != 0 && target != Owner && Owner != 0 && !Owner->House->Is_Ally((TechnoClass *)target)) {
                BulletClass *fragment = Create_Bullet(projectile, target, Owner, weapon->unknown_a4, weapon->Warhead, weapon->unknown_a8, weapon->unknown_10b);
                if (fragment != 0) {
                    velocity.Set(0, 0, 0);
                    velocity.Set_Yaw(Coord_Direction_462c50(Coord_Of_462c50(this), target->Center_Coord()));
                    velocity.Set_Pitch(DirType(0x2000));
                    velocity.Set_Speed(weapon->unknown_a8);
                    fragment->unknown_slot118(Coord_Of_462c50(this), velocity);
                    CoordStruct source = Coord_Of_462c50(this);
                    if (weapon->unknown_121 && Owner != 0) Owner->unknown_6c9df0(target, 0, weapon, source);
                    if (weapon->unknown_127 && Owner != 0) Owner->unknown_6ca020(target, weapon, source);
                }
                if (++fired == count) return;
            }
        }
    }
    if (fired < count) {
        int remaining = count - fired;
        do {
            Cell cell = unknown_slot104();
            cell.X += Random_Pick(-2, 2);
            cell.Y += Random_Pick(-2, 2);
            BulletClass *fragment = Create_Bullet(projectile, &Map[cell], Owner, weapon->unknown_a4, weapon->Warhead, weapon->unknown_a8, weapon->unknown_10b);
            if (fragment != 0) {
                velocity.Set(0, 0, 0);
                velocity.Set_Yaw(Coord_Direction_462c50(Coord_Of_462c50(this), Map_Cell(cell).Center_Coord()));
                velocity.Set_Speed(weapon->unknown_a8);
                velocity.Set_Pitch(DirType(0x2000));
                fragment->unknown_slot118(Coord_Of_462c50(this), velocity);
                CoordStruct source = Coord_Of_462c50(this);
                if (weapon->unknown_121 && Owner != 0) Owner->unknown_6c9df0(&Map_Cell(cell), 0, weapon, source);
                if (weapon->unknown_127 && Owner != 0) Owner->unknown_6ca020(&Map_Cell(cell), weapon, source);
            }
        } while (--remaining != 0);
    }
}
// @ref ?Speed@?$TVelocity2D@N@@QBENXZ=0041b750
// @ref ?Speed@?$TVelocity3D@N@@QBENXZ=0041b7c0
// @ref ?Horizontal_Speed@?$TVelocity3D@N@@QBENXZ=0041b830
// @ref ?Nonzero_Check@?$TVelocity3D@N@@QAEXXZ=0041b7f0
// @ref ?Nonzero_Check@?$TVelocity2D@N@@QAEXXZ=0041b860
// @ref ?Get_Pitch@?$TVelocity3D@N@@QBE?AUUnknownFacingDir@@XZ=0041b6e0
// @ref ?unknown_double_79b610@@3NB=0079b610
// @ref ?unknown_6c9df0@TechnoClass@@QAEPAVLaserDrawClass@@PAVAbstractClass@@HPAVWeaponTypeClass@@ABUCoordStruct@@@Z=006c9df0
// @ref ?unknown_radius_starts_7a5908@@3QBHB=007a5908
