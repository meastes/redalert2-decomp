#define RA2_COORD_FROM_CELL_INLINE
#define RA2_COORD_LENGTH
#define RA2_COORD_AS_CELL
#include "shared/mech_locomotion.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/cell.hpp"
#include "shared/rules.hpp"
#include "shared/tube.hpp"
#include "shared/overlay_type.hpp"
#include "shared/house.hpp"
#include "shared/techno_type.hpp"
#include "shared/fast_math.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/adjacent_cell.hpp"
#include "shared/infantry.hpp"
#include "shared/radio.hpp"
#include "shared/sound_play.hpp"
#include "shared/building_light.hpp"
// @ref ?unknown_geometry_l@@3UCoordStruct@@A=00a70b18
// @ref ?unknown_4c3000@FootClass@@QAE_NVCell@@HH@Z=004c3000

static inline int Coord_Length(CoordStruct const &a, CoordStruct const &b)
{
    UnknownVector3 delta = UnknownVector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
    return delta.Length();
}

static inline int Distance3(CoordStruct const &a, CoordStruct const &b)
{
    return (a - b).Length();
}

static inline Point2D Flat(CoordStruct const &coord)
{
    return Point2D(coord.X, coord.Y);
}

static inline int Distance2(Point2D const &a, Point2D const &b)
{
    Point2D delta(a.X - b.X, a.Y - b.Y);
    double x = delta.X;
    double y = delta.Y;
    return (int)Fast_Sqrt(y * y + x * x);
}

static inline CoordStruct Move_Coord(CoordStruct const &coord, UnknownFacingDir facing, int distance)
{
    double angle = ((short)facing.Raw - 0x3fff) * unknown_facing_radians_79b620;
    return CoordStruct((int)(coord.X + unknown_cos_4ba4d0(angle) * distance), (int)(coord.Y - unknown_sin_4ba480(angle) * distance), coord.Z);
}

static inline UnknownFacingDir Direction(CoordStruct const &coord1, CoordStruct const &coord2)
{
    int x1 = coord1.X;
    int x2 = coord2.X;
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2((double)coord1.Y - (double)coord2.Y, (double)x2 - (double)x1) - 1.5707963267948966) * -10430.060040584269));
}

// The height step per tube direction; the count is read after the caller has built the coordinate.
static inline int Tube_Height_Step(CoordStruct const &exit, TubeClass const *tube, int current_height)
{
    int count = tube->unknown_1c0;
    return (Map.Get_Height_GL(exit) - current_height) / count;
}

static inline Cell Adjacent_Tube(Cell const &cell, int facing)
{
    return cell + AdjacentCell[facing & 7];
}

static inline Cell Adjacent_Cell_V(Cell cell, int facing)
{
    Cell const &offset = AdjacentCell[facing & 7];
    return Cell(cell.X + offset.X, cell.Y + offset.Y);
}

void MechLocomotionClass::Movement_AI(bool continue_moving)
{
    if (LinkedTo->unknown_2e0.Is_Rotating()) {
        return;
    }

    if (unknown_24 == unknown_geometry_l) {
        if (unknown_18 != unknown_geometry_l) {
            if (LinkedTo->Path[0] == -1) {
                if (LinkedTo->unknown_530 != 0) {
                    LinkedTo->unknown_slot307();
                    return;
                }
                LinkedTo->unknown_530 = Rules->unknown_13c8 * 900.0;
                if (!LinkedTo->unknown_4c3000(unknown_18.As_Cell(), 0, 0)) {
                    if (!LinkedTo->unknown_slot171(unknown_18)) {
                        LinkedTo->Assign_Destination(0, 1);
                        return;
                    }
                    {
                        if (Coord_Length(LinkedTo->Center_Coord(), unknown_18) < Rules->unknown_1380 && !LinkedTo->unknown_370) {
                            LinkedTo->Assign_Destination(0, 1);
                        } else if (LinkedTo->unknown_53c != 0) {
                            LinkedTo->unknown_53c--;
                        } else {
                            if (LinkedTo->unknown_57a) {
                                Voc_Play(Rules->unknown_620, 1.0f, 0x2000, 0);
                            }
                            LinkedTo->unknown_57a = false;
                            Cell dest = unknown_18.As_Cell();
                            if (LinkedTo->unknown_32d) {
                                Cell nav_dest = LinkedTo->unknown_slot19(0).As_Cell();
                                if (!Map.Unknown_0054F7C0(nav_dest, dest, LinkedTo->Techno_Type_Class()->MovementZone, LinkedTo->Is_Moving_Onto_Bridge(0), Map[dest].IsUnderBridge, LinkedTo->unknown_slot189())) {
                                    LinkedTo->Assign_Destination(0, 1);
                                }
                            }
                            if (LinkedTo->unknown_228 != 0) {
                                Cell tar_dest = LinkedTo->unknown_228->unknown_slot19(LinkedTo).As_Cell();
                                if (LinkedTo->unknown_32d) {
                                    Cell nav_dest = LinkedTo->unknown_slot19(LinkedTo).As_Cell();
                                    if (!Map.Unknown_0054F7C0(nav_dest, tar_dest, LinkedTo->Techno_Type_Class()->MovementZone, LinkedTo->Is_Moving_Onto_Bridge(LinkedTo), Map[tar_dest].IsUnderBridge, LinkedTo->unknown_slot189())) {
                                        LinkedTo->unknown_slot220(0);
                                    }
                                }
                            }
                        }
                        LinkedTo->unknown_slot306(0.0);
                        Stop_Moving();
                        unknown_30 = false;
                        return;
                    }
                }
                LinkedTo->unknown_53c = 10;
            }

            int dir = LinkedTo->Path[0];
            if (dir == 8) {
                int tube = Map[Object_Coord(LinkedTo)].unknown_116;
                if (tube >= 0 && tube < unknown_TubeClass_list_008661c8.Count()) {
                    LinkedTo->Mark(0);
                    LinkedTo->Clear_Occupy_Bit(Object_Coord(LinkedTo));
                    TubeClass *tube_ptr = unknown_TubeClass_list_008661c8[tube];
                    Cell cell = tube_ptr->ExitCell;
                    unknown_24 = Cell_Coord(cell);
                    memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);
                    LinkedTo->Path[23] = -1;
                    LinkedTo->unknown_574 = tube;
                    LinkedTo->unknown_575 = 0;
                    TPoint2D<short> const &enter = tube_ptr->EnterCell;
                    CoordStruct enter_coord = Cell_Coord(Cell(enter));
                    Cell visual_cell = Adjacent_Tube(Cell(enter), tube_ptr->Directions[0]);
                    LinkedTo->unknown_45c = Object_Coord(LinkedTo) + (Map[visual_cell].unknown_475d60() - enter_coord);
                    int start_h = Map.Get_Height_GL(Object_Coord(LinkedTo));
                    cell = tube_ptr->ExitCell;
                    LinkedTo->unknown_45c.Z = start_h + Tube_Height_Step(CoordStruct(cell, 0), tube_ptr, start_h);
                    return;
                }
                LinkedTo->Path[0] = -1;
                unknown_24 = unknown_geometry_l;
                Stop_Moving();
                LinkedTo->Assign_Destination(0, 1);
                return;
            } else {

            Cell what = Adjacent_Cell_V(Object_Coord(LinkedTo).As_Cell(), dir);
            CoordStruct head_coord = Cell_Coord(what);
            head_coord.Z = Map.Get_Height_GL(head_coord);
            CellClass *what_cell = &Map[what];
            if (LinkedTo->unknown_79 ^ what_cell->IsUnderBridge) {
                LinkedTo->unknown_57b = true;
            }
            int can_enter = LinkedTo->unknown_slot101(&Map[what], dir, LinkedTo->Unknown_005D47D0(), 0, true);
            if (((can_enter != 5 && can_enter != 4) || !LinkedTo->Techno_Type_Class()->Crusher || what_cell->OverlayIndex != 0) && can_enter != 0) {
                LinkedTo->unknown_slot307();
                if (can_enter == 6) {
                    CellClass *cellptr = &Map[what];
                    if (continue_moving) {
                        LinkedTo->Path[0] = -1;
                        unknown_24 = unknown_geometry_l;
                        LinkedTo->unknown_530 = 0;
                        Movement_AI(false);
                        return;
                    }
                    int dist = Distance3(LinkedTo->Center_Coord(), unknown_18);
                    if (dist < Rules->unknown_1380 && !LinkedTo->In_Radio_Contact()
                        && abs(unknown_18.Z - Object_Coord(LinkedTo).Z) < unknown_geometry_g * 2
                        && Map[Object_Coord(LinkedTo)].unknown_ec != 10) {
                        LinkedTo->unknown_slot306(0.0);
                        LinkedTo->Assign_Destination(0, 1);
                        LinkedTo->Path[0] = -1;
                        Stop_Moving();
                        unknown_30 = false;
                    } else {
                        bool bridge;
                        if (cellptr->IsUnderBridge) {
                            int z = Object_Coord(LinkedTo).Z;
                            bridge = true;
                            if (abs(z / unknown_geometry_g - cellptr->unknown_11b) <= 2) {
                                bridge = false;
                            }
                        } else {
                            bridge = false;
                        }
                        cellptr->Incoming(&unknown_geometry_l, true, true, bridge);
                    }
                    return;
                }

                if (can_enter == 2) {
                    if (!LinkedTo->unknown_5a1) {
                        LinkedTo->unknown_5a1 = true;
                        LinkedTo->unknown_558 = Rules->unknown_13d0;
                    }
                    if (LinkedTo->unknown_530 == 0) {
                        bool blocked = LinkedTo->unknown_5a1 && LinkedTo->unknown_558 == 0;
                        bool found = LinkedTo->unknown_4c3000(unknown_18.As_Cell(), 0, blocked ? 2 : 1);
                        LinkedTo->unknown_530 = Rules->unknown_13c8 * 900.0;
                        if (!found) {
                            if (!LinkedTo->unknown_slot171(unknown_18)) {
                                LinkedTo->Assign_Destination(0, 1);
                            } else {
                                LinkedTo->unknown_slot288();
                            }
                        } else {
                            LinkedTo->unknown_slot289();
                        }
                        return;
                    }
                    UnknownFacingDir face = Direction(LinkedTo->Center_Coord(), head_coord);
                    Do_Turn(face);
                    return;
                }

                if (can_enter == 3) {
                    Map.Unknown_0055ACD0(LinkedTo, what);
                    LinkedTo->unknown_53c = 10;
                    return;
                }

                if (can_enter != 1) {
                    if (can_enter != 7) {
                        if (can_enter == 5 || can_enter == 4) {
                            ObjectClass *object = Map[what].Cell_Object(Point2D(0, 0), false);
                            if (continue_moving) {
                                LinkedTo->Path[0] = -1;
                                unknown_24 = unknown_geometry_l;
                                LinkedTo->unknown_530 = 0;
                                Movement_AI(false);
                                return;
                            }
                            if (object != 0) {
                                if (!LinkedTo->House->Is_Ally((TechnoClass *)object)) {
                                    LinkedTo->Override_Mission(1, (AbstractClass *)object, 0);
                                }
                            } else if (Map[what].OverlayIndex != -1 && OverlayTypes[Map[what].OverlayIndex]->Wall) {
                                LinkedTo->Override_Mission(1, (AbstractClass *)&Map[what], 0);
                            }
                        }
                        LinkedTo->Path[0] = -1;
                        LinkedTo->unknown_slot306(0.0);
                        Stop_Moving();
                        LinkedTo->unknown_57a = false;
                        goto done;
                    }
                } else {
                    Map[what].unknown_478740();
                }
                if (!continue_moving) return;
                LinkedTo->Path[0] = -1;
                unknown_24 = unknown_geometry_l;
                LinkedTo->unknown_530 = 0;
                Movement_AI(false);
                return;
            }

            if (Mark_Head_To(head_coord)) {
                {
                    if (LinkedTo->Path[0] != -1 && LinkedTo->Path[0] != 8) {
                        Cell cell = Adjacent_Tube(LinkedTo->unknown_slot104(), LinkedTo->Path[0]);
                        int overlay = Map[cell].OverlayIndex;
                        if (overlay != -1) {
                            OverlayTypeClass *otype = OverlayTypes[overlay];
                            if ((LinkedTo->Techno_Type_Class()->Crusher || LinkedTo->unknown_6d7910(0x11)) && otype->ArrayIndex == 0) {
                                LinkedTo->unknown_59f = true;
                                if (LinkedTo->Techno_Type_Class()->TiltsWhenCrushes) {
                                    LinkedTo->unknown_28c = -0.01f;
                                }
                            }
                        }
                    }
                }
                if (!LinkedTo->unknown_7d) {
                    return;
                }
                UnknownFacingDir face = Direction(LinkedTo->Center_Coord(), unknown_24);
                Do_Turn(face);
                if (LinkedTo->unknown_2e0.Is_Rotating()) {
                    return;
                }
                LinkedTo->unknown_slot306(1.0);
            } else {
                LinkedTo->unknown_slot306(0.0);
            }
            }
        } else {
            if (LinkedTo->unknown_468 > 0.0) {
                LinkedTo->unknown_slot306(0.0);
            }
        }
    } else {

    if (Distance2(Flat(LinkedTo->Center_Coord()), Flat(unknown_24)) < 16) {
        LinkedTo->Mark(0);
        if (LinkedTo->Path[0] == -1) {
            LinkedTo->Path[1] = -1;
        }
        memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);
        LinkedTo->Path[23] = -1;
        LinkedTo->Set_Coord(unknown_24);
        LinkedTo->unknown_450 = unknown_24.As_Cell();
        LinkedTo->Set_Height_AGL(0);
        Mark_Head_To(unknown_geometry_l);
        if (LinkedTo->Path[0] == -1) {
            LinkedTo->unknown_slot306(0.0);
        }
        LinkedTo->unknown_5a0 = true;
        LinkedTo->unknown_5a1 = false;
        if (unknown_18 == unknown_geometry_l
            || (LinkedTo->unknown_slot104() == unknown_18.As_Cell()
                && abs(LinkedTo->unknown_slot19(0).Z - unknown_18.Z) < unknown_geometry_g * 2)) {
            LinkedTo->Assign_Destination(0, 1);
            LinkedTo->unknown_slot306(0.0);
            Stop_Moving();
            unknown_30 = false;
        }
        LinkedTo->unknown_slot93(2);
        if (!LinkedTo->unknown_7d) {
            return;
        }
        if (LinkedTo->unknown_76 || LinkedTo->unknown_7a) {
            return;
        }
        {
            {
                {
                    if (LinkedTo->Path[0] != -1 && LinkedTo->Path[0] != 8) {
                        Cell cell = Adjacent_Tube(LinkedTo->unknown_slot104(), LinkedTo->Path[0]);
                        int overlay = Map[cell].OverlayIndex;
                        if (overlay != -1) {
                            OverlayTypeClass *otype = OverlayTypes[overlay];
                            if ((LinkedTo->Techno_Type_Class()->Crusher || LinkedTo->unknown_6d7910(0x11)) && otype->ArrayIndex == 0) {
                                LinkedTo->unknown_59f = true;
                                if (LinkedTo->Techno_Type_Class()->TiltsWhenCrushes) {
                                    LinkedTo->unknown_28c = -0.01f;
                                }
                            }
                        }
                    }
                }
                if (!LinkedTo->unknown_7d || LinkedTo->unknown_76) {
                    return;
                }
                LinkedTo->Mark(1);
            }
        }
    } else {

    LinkedTo->unknown_slot306(1.0);
    int speed = LinkedTo->unknown_slot303();
    UnknownFacingDir desired = Direction(LinkedTo->Center_Coord(), unknown_24);
    Do_Turn(desired);
    if (LinkedTo->unknown_2e0.Is_Rotating()) {
        return;
    }
    if (LinkedTo->unknown_slot47()) {
        LinkedTo->Clear_Occupy_Bit(Object_Coord(LinkedTo));
        LinkedTo->unknown_5a0 = false;
        LinkedTo->unknown_5a1 = false;
    }
    {
    Cell old_cell = LinkedTo->unknown_slot104();
    CoordStruct new_coord = Move_Coord(Object_Coord(LinkedTo), desired, speed);
    Cell new_cell = new_coord.As_Cell();
    if (old_cell != new_cell) {
        LinkedTo->Mark(0);
        LinkedTo->Set_Coord(new_coord);
        CellClass *old_cellptr = &Map[old_cell];
        CellClass *new_cellptr = &Map[new_cell];
        if (new_cellptr->unknown_11b == old_cellptr->unknown_11b - 4 && new_cellptr->IsUnderBridge) {
            LinkedTo->unknown_79 = true;
        }
        if (!new_cellptr->IsUnderBridge && old_cellptr->IsUnderBridge) {
            LinkedTo->unknown_79 = false;
        }
        LinkedTo->Set_Height_AGL(0);
        LinkedTo->Mark(1);
        Map[Object_Coord(LinkedTo)].Trigger_Veins();
    } else {
        bool was_down = LinkedTo->unknown_74;
        LinkedTo->unknown_74 = false;
        LinkedTo->Set_Coord(new_coord);
        LinkedTo->Set_Height_AGL(0);
        LinkedTo->unknown_74 = was_down;
    }
    }
    }
    }
done:
    LinkedTo->unknown_57a = false;
}
