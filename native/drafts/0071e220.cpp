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
// @ref ?Mark_Head_To@WalkLocomotionClass@@QAE_NABUCoordStruct@@@Z=0071f590
// @ref ??0?$TPoint3D@H@@QAE@HHH@Z=0041b630
// @ref _unknown_geometry_l=00af9060

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
    // NATIVE (C16 behaviour check, 2026-11-15): the original keeps the angle as a double
    // (0x0071F3CF fstp qword) for the sin/cos table lookups; a float angle picked another table
    // entry for 15 of the 65536 facings (e.g. 0xBFB4), moving the infantry elsewhere.
    double angle = ((short)facing.Raw - 0x3fff) * unknown_facing_radians_79b620;
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
    return (Map.Get_Height_GL(exit) - current_height) / count;
}

static inline Cell Adjacent_Tube(Cell const &cell, int facing)
{
    return cell + AdjacentCell[facing & 7];
}

void WalkLocomotionClass::unknown_71e220(bool first_pass)
{
    if (unknown_28 == unknown_geometry_l) {
        if (unknown_1c != unknown_geometry_l) {
            if (LinkedTo->Path[0] == -1) {
                if (LinkedTo->PathDelay != 0) {
                    LinkedTo->Stop_Movement_Animation();
                    return;
                }
                LinkedTo->PathDelay = Rules->PathDelay * 900.0;
                if (!LinkedTo->unknown_4c3000(unknown_1c.As_Cell(), 0, 0)) {
                    unknown_36 = false;
                    if (LinkedTo->unknown_slot171(unknown_1c)) {
                        LinkedTo->On_Movement_Blocked();
                        if (Coord_Length(LinkedTo->Center_Coord(), unknown_1c) < Rules->CloseEnoughDistance && !LinkedTo->unknown_370) {
                            LinkedTo->Assign_Destination(0, true);
                        } else if (LinkedTo->unknown_53c != 0) {
                            LinkedTo->unknown_53c--;
                        } else {
                            if (LinkedTo->IsNewNavCom) {
                                Voc_Play(Rules->ScoldSound, 1.0f, 0x2000, 0);
                            }
                            LinkedTo->IsNewNavCom = false;
                            Cell dest = unknown_1c.As_Cell();
                            if (LinkedTo->unknown_32d) {
                                Cell nav_dest = LinkedTo->Destination_Coord(0).As_Cell();
                                if (!Map.Unknown_0054F7C0(nav_dest, dest, LinkedTo->Techno_Type_Class()->MovementZone, LinkedTo->Is_Moving_Onto_Bridge(0), Map[dest].IsUnderBridge, LinkedTo->unknown_slot189())) {
                                    LinkedTo->Assign_Destination(0, true);
                                }
                            }
                            if (LinkedTo->TarCom != 0) {
                                Cell tar_dest = LinkedTo->TarCom->Destination_Coord(LinkedTo).As_Cell();
                                if (LinkedTo->unknown_32d && LinkedTo->TarCom != 0) {
                                    Cell nav_dest = LinkedTo->Destination_Coord(LinkedTo).As_Cell();
                                    if (!Map.Unknown_0054F7C0(nav_dest, tar_dest, LinkedTo->Techno_Type_Class()->MovementZone, LinkedTo->Is_Moving_Onto_Bridge(LinkedTo), Map[tar_dest].IsUnderBridge, LinkedTo->unknown_slot189())) {
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
                if (LinkedTo->JumpJet_To_Walk()) {
                    return;
                }
            }

            int dir = LinkedTo->Path[0];
            if (dir == 8) {
                int tube = Map[Object_Coord(LinkedTo)].Tube;
                if (tube >= 0 && tube < Tubes.Count()) {
                    LinkedTo->Mark(0);
                    LinkedTo->Clear_Occupy_Bit(Object_Coord(LinkedTo));
                    TubeClass *tube_ptr = Tubes[tube];
                    Cell cell = tube_ptr->ExitCell;
                    unknown_28 = Cell_Coord(cell);
                    memmove(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);   // NATIVE (C16): overlapping (the original's forward rep movsd)
                    LinkedTo->Path[23] = -1;
                    LinkedTo->CurrentTube = tube;
                    LinkedTo->CurrentTubeDir = 0;
                    TPoint2D<short> const &enter = tube_ptr->EnterCell;
                    CoordStruct enter_coord = Cell_Coord(Cell(enter));
                    Cell visual_cell = Adjacent_Tube(Cell(enter), tube_ptr->Directions[0]);
                    LinkedTo->unknown_45c = Object_Coord(LinkedTo) + (Map[visual_cell].unknown_475d60() - enter_coord);
                    int start_h = Map.Get_Height_GL(Object_Coord(LinkedTo));
                    Cell exit_cell = tube_ptr->ExitCell;
                    LinkedTo->unknown_45c.Z = start_h + Tube_Height_Step(CoordStruct(exit_cell, 0), tube_ptr, start_h);
                    return;
                }
                LinkedTo->Path[0] = -1;
                unknown_28 = unknown_geometry_l;
                Stop_Moving();
                LinkedTo->Assign_Destination(0, true);
                return;
            } else {
                CoordStruct coord = Adjacent_Coord(Object_Coord(LinkedTo), dir);
                Cell cell = coord.As_Cell();
                if (LinkedTo->IsOnBridge ^ Map[Adjacent_Coord(Object_Coord(LinkedTo), dir)].IsUnderBridge) {
                    LinkedTo->IsPlanningToLook = true;
                }
                int move = LinkedTo->Can_Enter_Cell(&Map[cell], dir, LinkedTo->Unknown_005D47D0(), 0, true);
                if (move != 0) {
                    unknown_36 = false;
                    LinkedTo->Stop_Movement_Animation();
                    if (move == 6) {
                        CellClass &cellptr = Map[cell];
                        if (first_pass) {
                            LinkedTo->Path[0] = -1;
                            unknown_28 = unknown_geometry_l;
                            LinkedTo->PathDelay = 0;
                            unknown_71e220(false);
                        } else {
                            int dist = Distance3(LinkedTo->Center_Coord(), unknown_1c);
                            if (dist < Rules->CloseEnoughDistance && !LinkedTo->HasAnyLink()
                                && abs(unknown_1c.Z - Object_Coord(LinkedTo).Z) < unknown_geometry_g * 2
                                && Map[Object_Coord(LinkedTo)].Land != 10) {
                                unknown_28 = unknown_geometry_l;
                                Stop_Moving();
                                LinkedTo->unknown_slot306(0.0);
                                LinkedTo->Assign_Destination(0, true);
                                LinkedTo->Path[0] = -1;
                            } else {
                                bool isbridge = cellptr.IsUnderBridge && abs(Object_Coord(LinkedTo).Z / unknown_geometry_g - cellptr.Height) > 2;
                                cellptr.Incoming(&unknown_geometry_l, true, true, isbridge);
                            }
                        }
                        return;
                    }
                    if (move == 2) {
                        if (!LinkedTo->IsToPathAroundBlockage) {
                            LinkedTo->IsToPathAroundBlockage = true;
                            LinkedTo->BlockagePathDelay = Rules->BlockagePathDelay;
                        }
                        if (LinkedTo->PathDelay != 0) {
                            return;
                        }
                        bool blocked = LinkedTo->IsToPathAroundBlockage && LinkedTo->BlockagePathDelay == 0;
                        bool found = LinkedTo->unknown_4c3000(unknown_1c.As_Cell(), 0, blocked ? 2 : 1);
                        LinkedTo->PathDelay = Rules->PathDelay * 900.0;
                        if (!found) {
                            if (!LinkedTo->unknown_slot171(unknown_1c)) {
                                LinkedTo->Assign_Destination(0, true);
                            } else {
                                LinkedTo->On_Movement_Blocked();
                            }
                        } else {
                            LinkedTo->JumpJet_To_Walk();
                        }
                        return;
                    }
                    if (move == 3) {
                        Map.Try_Open_Gate(LinkedTo, cell);
                        LinkedTo->unknown_53c = 10;
                        return;
                    }
                    if (move == 5 || move == 4) {
                        ObjectClass *object = Map[cell].Cell_Object(Point2D(0, 0), false);
                        if (first_pass) {
                            LinkedTo->Path[0] = -1;
                            unknown_28 = unknown_geometry_l;
                            LinkedTo->PathDelay = 0;
                            unknown_71e220(false);
                            return;
                        }
                        if (object != 0) {
                            if (!LinkedTo->House->Is_Ally((TechnoClass *)object)) {
                                LinkedTo->Override_Mission(1, object, 0);
                            }
                        } else if (Map[cell].OverlayIndex != -1 && OverlayTypes[Map[cell].OverlayIndex]->Wall) {
                            LinkedTo->Override_Mission(1, &Map[cell], 0);
                        }
                    }
                    if (move != 1) {
                        if (move != 7) {
                            LinkedTo->Path[0] = -1;
                            LinkedTo->unknown_slot306(0.0);
                            Stop_Moving();
                            LinkedTo->IsNewNavCom = false;
                            LinkedTo->IsNewNavCom = false;
                            return;
                        }
                    } else {
                        Map[cell].Shimmer();
                    }
                    if (first_pass) {
                        LinkedTo->Path[0] = -1;
                        unknown_28 = unknown_geometry_l;
                        LinkedTo->PathDelay = 0;
                        unknown_71e220(false);
                    }
                    return;
                }
                if (Mark_Head_To(coord)) {
                    unknown_36 = true;
                    if (!LinkedTo->IsActive) {
                        return;
                    }
                    UnknownFacingDir face = Direction(LinkedTo->Center_Coord(), unknown_28);
                    Do_Turn(face);
                    LinkedTo->unknown_slot306(1.0);
                } else {
                    LinkedTo->unknown_slot306(0.0);
                }
                LinkedTo->IsNewNavCom = false;
            }
        } else {
            if (LinkedTo->Speed > 0.0) {
                LinkedTo->unknown_slot306(0.0);
            }
            LinkedTo->IsNewNavCom = false;
        }
    } else {
    unknown_36 = true;
    if (Distance2(Flat(LinkedTo->Center_Coord()), Flat(unknown_28)) < 17) {
        LinkedTo->Mark(0);
        if (LinkedTo->Path[0] == -1) {
            LinkedTo->Path[1] = -1;
        }
        memmove(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);   // NATIVE (C16): overlapping (the original's forward rep movsd)
        LinkedTo->Path[23] = -1;
        LinkedTo->Set_Coord(unknown_28);
        LinkedTo->LastPathingCell = unknown_28.As_Cell();
        LinkedTo->Set_Height_AGL(0);
        LinkedTo->IsToPathAroundBlockage = false;
        Mark_Head_To(unknown_geometry_l);
        if (LinkedTo->Path[0] == -1) {
            LinkedTo->unknown_slot306(0.0);
        }
        LinkedTo->unknown_slot93(2);
        if (LinkedTo->IsActive && !LinkedTo->IsInLimbo && !LinkedTo->IsFalling) {
            if (unknown_1c == unknown_geometry_l
                || (LinkedTo->Destination_Coord(0).As_Cell() == unknown_1c.As_Cell()
                    && abs(LinkedTo->Destination_Coord(0).Z - unknown_1c.Z) < unknown_geometry_g * 2)) {
                LinkedTo->Assign_Destination(0, true);
                LinkedTo->unknown_slot306(0.0);
                unknown_28 = unknown_geometry_l;
                Stop_Moving();
                unknown_36 = false;
            }
            LinkedTo->Mark(1);
            LinkedTo->IsNewNavCom = false;
        }
        return;
    }
    if (LinkedTo->unknown_slot201()) {
        unknown_36 = false;
        LinkedTo->IsNewNavCom = false;
        return;
    }
    LinkedTo->unknown_slot306(1.0);
    int speed = LinkedTo->Current_Speed();
    LinkedTo->IsToPathAroundBlockage = false;
    UnknownFacingDir face = Direction(LinkedTo->Center_Coord(), unknown_28);
    Do_Turn(face);
    Cell old_cell = LinkedTo->Get_Cell();
    CoordStruct new_coord = Move_Coord(Object_Coord(LinkedTo), face, speed);
    Cell new_cell = new_coord.As_Cell();
    if (old_cell != new_cell) {
        LinkedTo->Mark(0);
        LinkedTo->Set_Coord(new_coord);
        CellClass *old_cellptr = &Map[old_cell];
        CellClass *new_cellptr = &Map[new_cell];
        if (new_cellptr->Height == old_cellptr->Height - 4) {
            if (new_cellptr->IsUnderBridge) {
                LinkedTo->IsOnBridge = true;
            }
        }
        if (!new_cellptr->IsUnderBridge && old_cellptr->IsUnderBridge) {
            LinkedTo->IsOnBridge = false;
        }
        LinkedTo->Set_Height_AGL(0);
        LinkedTo->Mark(1);
        Map[Object_Coord(LinkedTo)].Trigger_Veins();
        LinkedTo->IsNewNavCom = false;
    } else {
        bool was_down = LinkedTo->IsDown;
        LinkedTo->IsDown = false;
        LinkedTo->Set_Coord(new_coord);
        LinkedTo->Set_Height_AGL(0);
        LinkedTo->IsDown = was_down;
        LinkedTo->IsNewNavCom = false;
    }
    }
}
