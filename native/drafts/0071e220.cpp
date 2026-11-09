// NON-EXACT DRAFT 0x71E220 WalkLocomotionClass::unknown_71e220(bool) (infantry movement step) (4967 B)
// entry 0x71E220; exact-match score 89.83 (lane best), this text 89.83; plan "draft, verify"
// origin lane: Scout1104 (Scout1101 89.52 and Scout1099 74.48 before it) (build/class-work/Scout1104/keep/walk_best_8983.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1103 (native/drafts.json, native M10). Not original bytes.
// Its own unit: the population macros the lane scored it under come first.
#define RA2_COORD_TPOINT3D
#define RA2_TPOINT3D_DIFFERENCE
#define RA2_COORD_FROM_CELL_INLINE
#define RA2_CELL_AS_COORD
#define RA2_COORD_AS_CELL
#include "shared/walk_locomotion.hpp"
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
// @ref ?unknown_coord_af9060@@3UCoordStruct@@A=00af9060
// @ref ?Mark_Head_To@WalkLocomotionClass@@QAE_NABUCoordStruct@@@Z=0071f590
// @ref ??0?$TPoint3D@H@@QAE@HHH@Z=0041b630

static inline int Coord_Length(CoordStruct const &a, CoordStruct const &b)
{
    UnknownVector3 delta = UnknownVector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
    return delta.Length();
}

static inline int Distance3(CoordStruct const &a, CoordStruct const &b)
{
    return (a - b).Length();
}

static inline CoordStruct Adjacent_Coord(CoordStruct const &coord, int dir)
{
    TPoint2D<int> const &offset = unknown_facing_offsets_851ac0[dir & 7];
    return CoordStruct::BASECLASS(coord.X + offset.X, coord.Y + offset.Y, coord.Z);
}

static inline CoordStruct Move_Coord(CoordStruct const &coord, UnknownFacingDir facing, int distance)
{
    float angle = ((short)facing.Raw - 0x3fff) * unknown_facing_radians_79b620;
    return CoordStruct((int)(coord.X + unknown_cos_4ba4d0(angle) * distance), (int)(coord.Y - unknown_sin_4ba480(angle) * distance), coord.Z);
}

static inline UnknownFacingDir Direction(CoordStruct const &coord1, CoordStruct const &coord2)
{
    int x1 = coord1.X;
    int x2 = coord2.X;
    return UnknownFacingDir((unsigned short)(int)((Fast_Atan2((double)coord1.Y - (double)coord2.Y, (double)x2 - (double)x1) - 1.5707963267948966) * -10430.060040584269));
}

static inline Point2D Flat(CoordStruct const &coord)
{
    return Point2D(coord.X, coord.Y);
}

static inline int Distance2(Point2D const &a, Point2D const &b)
{
    Point2D delta(a.X - b.X, a.Y - b.Y);
    return (int)Fast_Sqrt((double)delta.Y * (double)delta.Y + (double)delta.X * (double)delta.X);
}

static inline int Tube_Height_Step(CoordStruct const &exit, TubeClass const *tube, int current_height)
{
    int count = tube->unknown_1c0;
    return (Map.Unknown_0055A280(exit) - current_height) / count;
}

static inline Cell Adjacent_Tube(Cell const &cell, int facing)
{
    return cell + AdjacentCell[facing & 7];
}

void WalkLocomotionClass::unknown_71e220(bool first_pass)
{
    if (unknown_28 == unknown_coord_af9060) {
        if (unknown_1c != unknown_coord_af9060) {
            if (LinkedTo->Path[0] == -1) {
                if (LinkedTo->unknown_530 != 0) {
                    LinkedTo->unknown_slot307();
                    return;
                }
                LinkedTo->unknown_530 = Rules->unknown_13c8 * 900.0;
                if (!LinkedTo->unknown_4c3000(unknown_1c.As_Cell(), 0, 0)) {
                    unknown_36 = false;
                    if (LinkedTo->unknown_slot171(unknown_1c)) {
                        LinkedTo->unknown_slot288();
                        if (Coord_Length(LinkedTo->Center_Coord(), unknown_1c) < Rules->unknown_1380 && !LinkedTo->unknown_370) {
                            LinkedTo->Assign_Destination(0, true);
                        } else if (LinkedTo->unknown_53c != 0) {
                            LinkedTo->unknown_53c--;
                        } else {
                            if (LinkedTo->unknown_57a) {
                                Voc_Play(Rules->unknown_620, 1.0f, 0x2000, 0);
                            }
                            LinkedTo->unknown_57a = false;
                            Cell dest = unknown_1c.As_Cell();
                            if (LinkedTo->unknown_32d) {
                                Cell nav_dest = LinkedTo->unknown_slot19(0).As_Cell();
                                if (!Map.Unknown_0054F7C0(nav_dest, dest, LinkedTo->unknown_slot32()->MovementZone, LinkedTo->unknown_slot46(0), Map[dest].IsUnderBridge, LinkedTo->unknown_slot189())) {
                                    LinkedTo->Assign_Destination(0, true);
                                }
                            }
                            if (LinkedTo->unknown_228 != 0) {
                                Cell tar_dest = LinkedTo->unknown_228->unknown_slot19(LinkedTo).As_Cell();
                                if (LinkedTo->unknown_32d && LinkedTo->unknown_228 != 0) {
                                    Cell nav_dest = LinkedTo->unknown_slot19(LinkedTo).As_Cell();
                                    if (!Map.Unknown_0054F7C0(nav_dest, tar_dest, LinkedTo->unknown_slot32()->MovementZone, LinkedTo->unknown_slot46(LinkedTo), Map[tar_dest].IsUnderBridge, LinkedTo->unknown_slot189())) {
                                        LinkedTo->unknown_slot220(0);
                                    }
                                }
                            }
                        }
                        LinkedTo->unknown_slot306(0.0);
                        Stop_Moving();
                        return;
                    }
                    LinkedTo->Assign_Destination(0, true);
                    return;
                }
                LinkedTo->unknown_53c = 10;
                if (LinkedTo->unknown_slot289()) {
                    return;
                }
            }

            int dir = LinkedTo->Path[0];
            if (dir == 8) {
                int tube = Map[Object_Coord(LinkedTo)].unknown_116;
                if (tube >= 0 && tube < unknown_TubeClass_list_008661c8.Count()) {
                    LinkedTo->Mark(0);
                    LinkedTo->unknown_slot59(Object_Coord(LinkedTo));
                    TubeClass *tube_ptr = unknown_TubeClass_list_008661c8[tube];
                    Cell cell = tube_ptr->ExitCell;
                    unknown_28 = Cell_Coord(cell);
                    memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);
                    LinkedTo->Path[23] = -1;
                    LinkedTo->unknown_574 = tube;
                    LinkedTo->unknown_575 = 0;
                    TPoint2D<short> const &enter = tube_ptr->EnterCell;
                    CoordStruct enter_coord = Cell_Coord(Cell(enter));
                    Cell visual_cell = Adjacent_Tube(Cell(enter), tube_ptr->Directions[0]);
                    LinkedTo->unknown_45c = Object_Coord(LinkedTo) + (Map[visual_cell].unknown_475d60() - enter_coord);
                    int start_h = Map.Unknown_0055A280(Object_Coord(LinkedTo));
                    Cell exit_cell = tube_ptr->ExitCell;
                    LinkedTo->unknown_45c.Z = start_h + Tube_Height_Step(CoordStruct(exit_cell, 0), tube_ptr, start_h);
                    return;
                }
                LinkedTo->Path[0] = -1;
                unknown_28 = unknown_coord_af9060;
                Stop_Moving();
                LinkedTo->Assign_Destination(0, true);
                return;
            } else {
                CoordStruct coord = Adjacent_Coord(Object_Coord(LinkedTo), dir);
                Cell cell = coord.As_Cell();
                if (LinkedTo->unknown_79 ^ Map[Adjacent_Coord(Object_Coord(LinkedTo), dir)].IsUnderBridge) {
                    LinkedTo->unknown_57b = true;
                }
                int move = LinkedTo->unknown_slot101(&Map[cell], dir, LinkedTo->Unknown_005D47D0(), 0, true);
                if (move != 0) {
                    unknown_36 = false;
                    LinkedTo->unknown_slot307();
                    if (move == 6) {
                        CellClass &cellptr = Map[cell];
                        if (first_pass) {
                            LinkedTo->Path[0] = -1;
                            unknown_28 = unknown_coord_af9060;
                            LinkedTo->unknown_530 = 0;
                            unknown_71e220(false);
                        } else {
                            int dist = Distance3(LinkedTo->Center_Coord(), unknown_1c);
                            if (dist < Rules->unknown_1380 && !LinkedTo->In_Radio_Contact()
                                && abs(unknown_1c.Z - Object_Coord(LinkedTo).Z) < unknown_geometry_g * 2
                                && Map[Object_Coord(LinkedTo)].unknown_ec != 10) {
                                unknown_28 = unknown_coord_af9060;
                                Stop_Moving();
                                LinkedTo->unknown_slot306(0.0);
                                LinkedTo->Assign_Destination(0, true);
                                LinkedTo->Path[0] = -1;
                            } else {
                                bool isbridge = cellptr.IsUnderBridge && abs(Object_Coord(LinkedTo).Z / unknown_geometry_g - cellptr.unknown_11b) > 2;
                                cellptr.unknown_476920(&unknown_coord_af9060, true, true, isbridge);
                            }
                        }
                        return;
                    }
                    if (move == 2) {
                        if (!LinkedTo->unknown_5a1) {
                            LinkedTo->unknown_5a1 = true;
                            LinkedTo->unknown_558 = Rules->unknown_13d0;
                        }
                        if (LinkedTo->unknown_530 != 0) {
                            return;
                        }
                        bool blocked = LinkedTo->unknown_5a1 && LinkedTo->unknown_558 == 0;
                        bool found = LinkedTo->unknown_4c3000(unknown_1c.As_Cell(), 0, blocked ? 2 : 1);
                        LinkedTo->unknown_530 = Rules->unknown_13c8 * 900.0;
                        if (!found) {
                            if (!LinkedTo->unknown_slot171(unknown_1c)) {
                                LinkedTo->Assign_Destination(0, true);
                            } else {
                                LinkedTo->unknown_slot288();
                            }
                        } else {
                            LinkedTo->unknown_slot289();
                        }
                        return;
                    }
                    if (move == 3) {
                        Map.Unknown_0055ACD0(LinkedTo, cell);
                        LinkedTo->unknown_53c = 10;
                        return;
                    }
                    if (move == 5 || move == 4) {
                        ObjectClass *object = Map[cell].unknown_471830(Point2D(0, 0), false);
                        if (first_pass) {
                            LinkedTo->Path[0] = -1;
                            unknown_28 = unknown_coord_af9060;
                            LinkedTo->unknown_530 = 0;
                            unknown_71e220(false);
                            return;
                        }
                        if (object != 0) {
                            if (!LinkedTo->House->Is_Ally((TechnoClass *)object)) {
                                LinkedTo->unknown_slot119(1, object, 0);
                            }
                        } else if (Map[cell].OverlayIndex != -1 && OverlayTypes[Map[cell].OverlayIndex]->Wall) {
                            LinkedTo->unknown_slot119(1, &Map[cell], 0);
                        }
                    }
                    if (move != 1) {
                        if (move != 7) {
                            LinkedTo->Path[0] = -1;
                            LinkedTo->unknown_slot306(0.0);
                            Stop_Moving();
                            LinkedTo->unknown_57a = false;
                            LinkedTo->unknown_57a = false;
                            return;
                        }
                    } else {
                        Map[cell].unknown_478740();
                    }
                    if (first_pass) {
                        LinkedTo->Path[0] = -1;
                        unknown_28 = unknown_coord_af9060;
                        LinkedTo->unknown_530 = 0;
                        unknown_71e220(false);
                    }
                    return;
                }
                if (Mark_Head_To(coord)) {
                    unknown_36 = true;
                    if (!LinkedTo->unknown_7d) {
                        return;
                    }
                    UnknownFacingDir face = Direction(LinkedTo->Center_Coord(), unknown_28);
                    Do_Turn(face);
                    LinkedTo->unknown_slot306(1.0);
                } else {
                    LinkedTo->unknown_slot306(0.0);
                }
                LinkedTo->unknown_57a = false;
            }
        } else {
            if (LinkedTo->unknown_468 > 0.0) {
                LinkedTo->unknown_slot306(0.0);
            }
            LinkedTo->unknown_57a = false;
        }
    } else {
    unknown_36 = true;
    if (Distance2(Flat(LinkedTo->Center_Coord()), Flat(unknown_28)) < 17) {
        LinkedTo->Mark(0);
        if (LinkedTo->Path[0] == -1) {
            LinkedTo->Path[1] = -1;
        }
        memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);
        LinkedTo->Path[23] = -1;
        LinkedTo->unknown_slot103(unknown_28);
        LinkedTo->unknown_450 = unknown_28.As_Cell();
        LinkedTo->unknown_slot109(0);
        LinkedTo->unknown_5a1 = false;
        Mark_Head_To(unknown_coord_af9060);
        if (LinkedTo->Path[0] == -1) {
            LinkedTo->unknown_slot306(0.0);
        }
        LinkedTo->unknown_slot93(2);
        if (LinkedTo->unknown_7d && !LinkedTo->unknown_76 && !LinkedTo->unknown_7a) {
            if (unknown_1c == unknown_coord_af9060
                || (LinkedTo->unknown_slot19(0).As_Cell() == unknown_1c.As_Cell()
                    && abs(LinkedTo->unknown_slot19(0).Z - unknown_1c.Z) < unknown_geometry_g * 2)) {
                LinkedTo->Assign_Destination(0, true);
                LinkedTo->unknown_slot306(0.0);
                unknown_28 = unknown_coord_af9060;
                Stop_Moving();
                unknown_36 = false;
            }
            LinkedTo->Mark(1);
            LinkedTo->unknown_57a = false;
        }
        return;
    }
    if (LinkedTo->unknown_slot201()) {
        unknown_36 = false;
        LinkedTo->unknown_57a = false;
        return;
    }
    LinkedTo->unknown_slot306(1.0);
    int speed = LinkedTo->unknown_slot303();
    LinkedTo->unknown_5a1 = false;
    UnknownFacingDir face = Direction(LinkedTo->Center_Coord(), unknown_28);
    Do_Turn(face);
    Cell old_cell = LinkedTo->unknown_slot104();
    CoordStruct new_coord = Move_Coord(Object_Coord(LinkedTo), face, speed);
    Cell new_cell = new_coord.As_Cell();
    if (old_cell != new_cell) {
        LinkedTo->Mark(0);
        LinkedTo->unknown_slot103(new_coord);
        CellClass *old_cellptr = &Map[old_cell];
        CellClass *new_cellptr = &Map[new_cell];
        if (new_cellptr->unknown_11b == old_cellptr->unknown_11b - 4) {
            if (new_cellptr->IsUnderBridge) {
                LinkedTo->unknown_79 = true;
            }
        }
        if (!new_cellptr->IsUnderBridge && old_cellptr->IsUnderBridge) {
            LinkedTo->unknown_79 = false;
        }
        LinkedTo->unknown_slot109(0);
        LinkedTo->Mark(1);
        Map[Object_Coord(LinkedTo)].unknown_47bb30();
        LinkedTo->unknown_57a = false;
    } else {
        bool was_down = LinkedTo->unknown_74;
        LinkedTo->unknown_74 = false;
        LinkedTo->unknown_slot103(new_coord);
        LinkedTo->unknown_slot109(0);
        LinkedTo->unknown_74 = was_down;
        LinkedTo->unknown_57a = false;
    }
    }
}
