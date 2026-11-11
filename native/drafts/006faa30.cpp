#define RA2_COORD_PLUS_POINT2D
#define RA2_COORD_LENGTH
// void UnitClass::Unknown_006FAA30() (0x006FAA30)
// TibSun UnitClass::Tunnel_AI: walks the unit along the tube it entered, then hands it back to the
// map at the far mouth, or waits underground and shoos idle occupiers off the exit cell.
#include "shared/unit.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/tube.hpp"
#include "shared/fast_math.hpp"
#include "shared/unknown_vector3.hpp"
TubeClass *__fastcall Cell_Tube_0047a130(CellClass const *cell);
// @ref ?Cell_Tube_0047a130@@=0047a130
static double const unknown_tube_speed_79d568 = 1.5;   // native: the value at 0x0079D568
static double const unknown_diagonal_lepton_7a3a20 = 362.03867196751236;   // native: the value at 0x007A3A20
static double const unknown_lepton_79a700 = 256.0;   // native: the value at 0x0079A700
extern double const unknown_facing_radians_79b620;
#include "shared/rect.hpp"
extern Point2D const unknown_adjacent_offsets_00851ac0[8];   // 0x00851AC0
inline CoordStruct Adjacent_Coord_006faa30(CoordStruct const &coord, int dir)
{
    return coord + unknown_adjacent_offsets_00851ac0[dir & 7];
}
// @ref ?unknown_tube_speed_79d568@@=0079d568
// @ref ?unknown_diagonal_lepton_7a3a20@@=007a3a20
// @ref ?unknown_lepton_79a700@@=0079a700
// @ref ?unknown_facing_radians_79b620@@=0079b620
// @ref ?unknown_adjacent_coord_851ac0@@=00851ac0
// Hypotheses: the out-of-line length (0x0041B780) reads the coordinate difference as UnknownVector3.
// @ref ??GCoordStruct@@QBE?AU0@ABU0@@Z=004348f0
// @ref ??0CoordStruct@@QAE@HHH@Z=0041b630
// @ref ??0?$TPoint3D@H@@QAE@HHH@Z=0041b630
// @ref ?Length@CoordStruct@@QBEHXZ=0041b780
inline int Distance_006faa30(CoordStruct const &a, CoordStruct const &b)
{
    return (a - b).Length();
}
inline CoordStruct Cell_Center_006faa30(Cell cell, int z)
{
    return CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, z);
}
inline double Radian_006faa30(UnknownFacingDir const &dir)
{
    return ((short)dir.Raw - 0x3FFF) * unknown_facing_radians_79b620;
}
inline CoordStruct Move_Coord_006faa30(CoordStruct const &start, UnknownFacingDir const &dir, int distance)
{
    double radians = Radian_006faa30(dir);
    return CoordStruct((int)(start.X + Fast_Cos(radians) * distance), (int)(start.Y - Fast_Sin(radians) * distance), start.Z);
}

void UnitClass::Unknown_006FAA30()
{
    CoordStruct coord;
    bool finished = unknown_TubeClass_list_008661c8[unknown_574]->Directions[unknown_575] == -1;
    if (!finished) {
        int distance = Distance_006faa30(Center_Coord(), unknown_45c);
        TubeClass *tube = unknown_TubeClass_list_008661c8[unknown_574];
        UnknownFacingDir direction_temp;
        UnknownFacingDir direction = direction_temp.Direction(Center_Coord(), unknown_45c);
        int speed = (int)(Type->Speed * unknown_tube_speed_79d568);
        CoordStruct exit = Cell_Center_006faa30(tube->ExitCell, 0);
        CoordStruct enter = Cell_Center_006faa30(tube->EnterCell, 0);
        int height_step = tube->unknown_1c0;
        int enter_height = Map.Get_Height_GL(enter);
        height_step = (Map.Get_Height_GL(exit) - enter_height) / height_step;
        if (distance <= speed) {
            unknown_575++;
            int tube_dir = tube->Directions[unknown_575];
            finished = tube_dir == -1;
            Set_Coord(unknown_45c);
            if (!finished) {
                coord = Adjacent_Coord_006faa30(unknown_45c, tube_dir);
                CoordStruct delta = Map[coord].unknown_475d60() - Map[unknown_45c].unknown_475d60();
                unknown_45c += CoordStruct(delta.X, delta.Y, 0);
                unknown_45c.Z += height_step;
                UnknownFacingDir dir = UnknownFacingDir().Direction(Center_Coord(), unknown_45c);
                CoordStruct new_coord = Move_Coord_006faa30(Coord, dir, speed - distance);
                double divisor;
                if (tube_dir % 2) {
                    divisor = unknown_diagonal_lepton_7a3a20;
                } else {
                    divisor = unknown_lepton_79a700;
                }
                new_coord.Z += (speed - distance) / divisor * height_step;
                Set_Coord(new_coord);
                return;
            }
        } else {
            int tube_dir = tube->Directions[unknown_575];
            CoordStruct new_coord = Move_Coord_006faa30(Coord, direction, speed);
            double divisor;
            if (tube_dir % 2) {
                divisor = unknown_diagonal_lepton_7a3a20;
            } else {
                divisor = unknown_lepton_79a700;
            }
            new_coord.Z += speed / divisor * height_step;
            Set_Coord(new_coord);
            return;
        }
    }
    CellClass *cell = &Map[CoordStruct(Coord)];
    ObjectClass *occupier = cell->OccupierPtr;
    if (occupier != 0) {
        DynamicVectorClass<ObjectClass *> list;
        do {
            list.Add(occupier);
            occupier = occupier->unknown_30;
        } while (occupier != 0);
        for (int index = 0; index < list.Count(); index++) {
            ObjectClass *object = list[index];
            if (object->Kind_Of() == 1 || object->Kind_Of() == 15) {
                if (!static_cast<FootClass *>(object)->Locomotor.Checked()->Is_Moving()) {
                    object->Scatter(CoordStruct(0, 0, 0), true, true);
                }
            }
        }
        unknown_slot306(0.0);
    } else {
        TubeClass *tube = unknown_TubeClass_list_008661c8[unknown_574];
        CoordStruct exit_coord = Cell_Center_006faa30(tube->ExitCell, unknown_45c.Z);
        Set_Coord(exit_coord);
        unknown_574 = -1;
        Mark(1);
        if (unknown_494 == cell) {
            Scatter(CoordStruct(0, 0, 0), true, true);
        }
        unknown_57b = true;
        unknown_slot93(2);
        unknown_slot306(1.0);
        TubeClass *here = Cell_Tube_0047a130(&Map[CoordStruct(Coord)]);
        if (here != 0) {
            unknown_2e0.Set(UnknownFacingDir((unsigned short)((here->Facing << 13) - 0x8000)));
        }
    }
}
