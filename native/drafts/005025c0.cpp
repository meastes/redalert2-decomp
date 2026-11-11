// Advances underground Infantry through a tube and releases it at the exit (0x005025C0).
#include "shared/infantry.hpp"
#include "shared/locomotion.hpp"
extern CoordStruct unknown_coord_a410f8;   // 0x00A410F8
#include "shared/cell.hpp"
#include "shared/tube.hpp"
#include "shared/techno_map.hpp"
#include "shared/unknown_vector3.hpp"
double __cdecl unknown_sin_4ba480(double angle);
double __cdecl unknown_cos_4ba4d0(double angle);
// Already accepted kept copies; full73-byte bodies and7NOP padding independently rechecked in r363. Zero new credit.
// @ref ??_G?$VectorClass@PAVObjectClass@@@@UAEPAXI@Z=0040ce20
// @ref ??_G?$DynamicVectorClass@PAVObjectClass@@@@UAEPAXI@Z=0047d090

// @ref ??0UnknownVector3@@QAE@HHH@Z=0041b630
// @ref ?Length@UnknownVector3@@QBEHXZ=0041b780
// @ref ??0UnknownFacingDir@@QAE@ABUCoordStruct@@0@Z=00423ef0
// @ref ?unknown_475d60@CellClass@@QBE?AUCoordStruct@@XZ=00475d60
// @ref ?Unknown_004764B0@UnknownCellClass@@QAE?AUCoordStruct@@ABU2@H_N@Z=004764b0
// @ref ?Unknown_0055A280@MapClass@@QAEHABUCoordStruct@@@Z=0055a280
// @ref ?unknown_sin_4ba480@@YANN@Z=004ba480
// @ref ?unknown_cos_4ba4d0@@YANN@Z=004ba4d0
// @ref ?unknown_TubeClass_list_008661c8@@3V?$DynamicVectorClass@PAVTubeClass@@@@A=008661c8
// @ref ?unknown_facing_offsets_851ac0@@3QBUUnknownFacingOffset@@B=00851ac0
// @ref ?unknown_coord_a410f8@@3UCoordStruct@@A=00a410f8
// @ref ?unknown_tube_angle_scale_79b620@@3NB=0079b620
// @ref ?unknown_tube_diagonal_7a3a20@@3NB=007a3a20
// @ref ?unknown_tube_width_79a700@@3NB=0079a700
// @ref ?_com_issue_error@@YGXJ@Z=00795940
// @ref ??0?$VectorClass@PAVObjectClass@@@@QAE@IPBQAVObjectClass@@@Z=004bfbd0
// @ref ??GCoordStruct@@QBE?AU0@ABU0@@Z=004348f0
extern double const unknown_tube_angle_scale_79b620;
static double const unknown_tube_diagonal_7a3a20 = 362.03867196751236;   // native: the value at 0x007A3A20 (256 * sqrt 2)
extern double const unknown_facing_radians_79b620;   // 0x0079B620
static double const unknown_tube_width_79a700 = 256.0;   // native: the value at 0x0079A700

// UnknownVector3 is CoordStruct under a placeholder name (shared/coord.hpp, 0x0041B780).
static inline int Tube_Length(CoordStruct const &coord)
{
    return reinterpret_cast<UnknownVector3 const &>(coord).Length();
}
static inline int Tube_Coord_Distance(CoordStruct const &from, CoordStruct const &to)
{
    CoordStruct const &delta = from - to;
    return Tube_Length(delta);
}
static inline int Tube_Distance(InfantryClass *self, CoordStruct const &coord)
{
    return Tube_Coord_Distance(self->Center_Coord(), coord);
}
static inline CoordStruct Tube_Cell_Coord(Cell const &cell)
{
    return CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0);
}
static inline CoordStruct Tube_Plus(CoordStruct const &coord, TPoint2D<int> const &point)
{
    return CoordStruct(coord.X + point.X, coord.Y + point.Y, coord.Z);
}
static inline CoordStruct Tube_Adjacent(CoordStruct const &coord, int dir)
{
    return Tube_Plus(coord, reinterpret_cast<TPoint2D<int> const &>(unknown_facing_offsets_851ac0[dir & 7]));
}
inline CoordStruct Move_Coord(CoordStruct const &start, UnknownFacingDir const &direction, int distance)
{
    double angle = ((short)direction.Raw - 0x3fff) * unknown_facing_radians_79b620;   // native: FacingDir as radians
    return CoordStruct(int(start.X + unknown_cos_4ba4d0(angle) * distance), int(start.Y - unknown_sin_4ba480(angle) * distance), start.Z);
}

// TibSun InfantryClass::Tunnel_AI: walks the soldier along its tube and sets it down at the exit.
void InfantryClass::unknown_5025c0()
{
    bool finished = unknown_TubeClass_list_008661c8[unknown_574]->Directions[unknown_575] == -1;
    if (!finished) {
        int distance = Tube_Distance(this, unknown_45c);
        TubeClass *tube = unknown_TubeClass_list_008661c8[unknown_574];
        UnknownFacingDir facing;
        UnknownFacingDir direction = facing.Direction(Center_Coord(), unknown_45c);
        int speed = Type->Speed;
        int height_step;
        {
            Cell tube_cell = tube->ExitCell;
            CoordStruct exit_coord = Tube_Cell_Coord(tube_cell);
            tube_cell = tube->EnterCell;
            CoordStruct enter_coord = Tube_Cell_Coord(tube_cell);
            height_step = tube->unknown_1c0;
            height_step = (Map.Get_Height_GL(exit_coord) - Map.Get_Height_GL(enter_coord)) / height_step;
        }
        if (distance <= speed) {
            unknown_575++;
            int tube_dir = tube->Directions[unknown_575];
            finished = tube_dir == -1;
            Set_Coord(unknown_45c);
            if (!finished) {
                CoordStruct coord = Tube_Adjacent(unknown_45c, tube->Directions[unknown_575]);
                unknown_45c = Tube_Plus(unknown_45c, reinterpret_cast<TPoint2D<int> const &>(Map[coord].unknown_475d60()) - reinterpret_cast<TPoint2D<int> const &>(Map[unknown_45c].unknown_475d60()));
                unknown_45c.Z += height_step;
                UnknownFacingDir dir = UnknownFacingDir().Direction(Center_Coord(), unknown_45c);
                CoordStruct new_coord = Move_Coord(Object_Coord(this), dir, speed - distance);
                double divisor;
                if (tube_dir % 2) {
                    divisor = unknown_tube_diagonal_7a3a20;
                } else {
                    divisor = unknown_tube_width_79a700;
                }
                new_coord.Z += (speed - distance) / divisor * height_step;
                Set_Coord(new_coord);
                return;
            }
        } else {
            int tube_dir = tube->Directions[unknown_575];
            CoordStruct new_coord = Move_Coord(Object_Coord(this), direction, speed);
            double divisor;
            if (tube_dir % 2) {
                divisor = unknown_tube_diagonal_7a3a20;
            } else {
                divisor = unknown_tube_width_79a700;
            }
            new_coord.Z += speed / divisor * height_step;
            Set_Coord(new_coord);
            return;
        }
    }
    if (finished) {
        CellClass *cell = &Map[Object_Coord(this)];
        ObjectClass *occupier = cell->OccupierPtr;
        // Existing slot101 uses a 32-bit int placeholder for its cell pointer.
        if (unknown_slot101(cell, -1, 0, 0, 1)) {
            DynamicVectorClass<ObjectClass *> objects;
            for (; occupier != 0; occupier = occupier->unknown_30) {
                objects.Add(occupier);
            }
            for (int i = 0; i < objects.Count(); ++i) {
                ObjectClass *object = objects[i];
                if (object->Kind_Of() == 1 || object->Kind_Of() == 15) {
                    FootClass *unit = static_cast<FootClass *>(object);
                    if (!unit->Locomotor.Checked()->Is_Moving()) {
                        unit->Scatter(CoordStruct(0, 0, 0), true, true);
                    }
                }
            }
            unknown_slot306(0.0);
        } else {
            CoordStruct spot = reinterpret_cast<UnknownCellClass *>(cell)->Unknown_004764B0(Object_Coord(this), 0, false);
            spot.Z = Map.Get_Height_GL(spot);
            Set_Coord(spot);
            Locomotor.Checked()->Force_Immediate_Destination(unknown_coord_a410f8);
            Locomotor.Checked()->Stop_Movement_Animation();
            Mark(1);
            Set_Occupy_Bit(Object_Coord(this));
            if (unknown_494 == cell) {
                Scatter(CoordStruct(0, 0, 0), true, true);
            } else {
                unknown_slot306(1.0);
            }
            unknown_574 = -1;
            unknown_57b = true;
            unknown_slot93(2);
        }
    }
}
