// mech.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mech.json carries the addresses and the rows that
// stay in their own files.


// Not exact (RA2_NON_MATCHING) prelude: header switches the drafts below need (guard-on compile only)
#ifdef RA2_NON_MATCHING
#define RA2_COORD_AS_CELL
#define RA2_COORD_TPOINT3D
#define RA2_TPOINT3D_DIFFERENCE
#define RA2_COORD_FROM_CELL_INLINE
#define RA2_CELL_AS_COORD
#endif

#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/mech_locomotion.hpp"
#include "shared/cell.hpp"
#include "shared/foot.hpp"


// Header-static dynamic initializers of the unit at 0x005914E0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x005914E0-0x005916C6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00591510, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_005914e0.cpp; no source of its own.


// Dynamic initializer (0x00591530, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_005914e0.cpp; no source of its own.


// Dynamic initializer (0x00591550, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_005914e0.cpp; no source of its own.


// Dynamic initializer (0x00591570, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_005914e0.cpp; no source of its own.


// Dynamic initializer (0x00591590, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_005914e0.cpp; no source of its own.


// Dynamic initializer (0x005915B0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_005914e0.cpp; no source of its own.


// Dynamic initializer (0x005915F0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_005914e0.cpp; no source of its own.


// Dynamic initializer (0x00591620, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_005914e0.cpp; no source of its own.


// Dynamic initializer (0x00591650, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_005914e0.cpp; no source of its own.


// Dynamic initializer (0x00591680, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_005914e0.cpp; no source of its own.


// Dynamic initializer (0x00591690, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_005914e0.cpp; no source of its own.


// Dynamic initializer (0x005916B0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_005914e0.cpp; no source of its own.


// MechLocomotionClass::MechLocomotionClass() (0x005916D0)
// No destination or head-to coordinate, not moving.

MechLocomotionClass::MechLocomotionClass() :
    unknown_18(unknown_geometry_l),
    unknown_24(unknown_geometry_l),
    unknown_30(false)
{
}


// MechLocomotionClass::~MechLocomotionClass() (0x00591730)

MechLocomotionClass::~MechLocomotionClass()
{
}


// bool __stdcall MechLocomotionClass::Is_Moving() (0x00591750, ILocomotion 4)

bool __stdcall MechLocomotionClass::Is_Moving()
{
    return unknown_30;
}


// CoordStruct __stdcall MechLocomotionClass::Destination() (0x00591760, ILocomotion 5)
// The destination while moving, otherwise none.

CoordStruct __stdcall MechLocomotionClass::Destination()
{
    if (Is_Moving()) {
        return CoordStruct(unknown_18.X, unknown_18.Y, unknown_18.Z);
    }
    return CoordStruct(unknown_geometry_l.X, unknown_geometry_l.Y, unknown_geometry_l.Z);
}


// CoordStruct __stdcall MechLocomotionClass::Head_To_Coord() (0x005917C0, ILocomotion 6)
// The head-to coordinate when set, otherwise where the linked object is.

CoordStruct __stdcall MechLocomotionClass::Head_To_Coord()
{
    if (unknown_24 != unknown_geometry_l) {
        return CoordStruct(unknown_24.X, unknown_24.Y, unknown_24.Z);
    }
    CoordStruct coord = LinkedTo->Coord;
    return coord;
}


// bool __stdcall MechLocomotionClass::Process() (0x00591840, ILocomotion 16)
// One step (0x005919A0), then whether still moving.

bool __stdcall MechLocomotionClass::Process()
{
    Movement_AI(true);
    return Is_Moving();
}


// void __stdcall MechLocomotionClass::Move_To(CoordStruct to) (0x00591860, ILocomotion 17)
// Unless the linked object is busy (slots 201, 202, 111, 112), heads for
// the coordinate (raised on flagged cells) and starts moving.

void __stdcall MechLocomotionClass::Move_To(CoordStruct to)
{
    if (!LinkedTo->unknown_slot201() && !LinkedTo->unknown_slot202()
        && !LinkedTo->unknown_slot111() && !LinkedTo->unknown_slot112()) {
        // The original copies the argument member by member first.
        to = CoordStruct(to.X, to.Y, to.Z);
        unknown_18 = to;
        if (Loco_Cell(to)->Flags & 0x100) {
            unknown_18.Z += Unknown_006A29B0(60);
        }
        unknown_30 = true;
    }
}


// void __stdcall MechLocomotionClass::Stop_Moving() (0x00591900, ILocomotion 18)
// Drops the destination; stops unless a head-to coordinate remains.

void __stdcall MechLocomotionClass::Stop_Moving()
{
    unknown_18 = unknown_geometry_l;
    if (unknown_24 == unknown_geometry_l) {
        unknown_30 = false;
    }
}


// void __stdcall MechLocomotionClass::Do_Turn(UnknownFacingDir dir) (0x00591950, ILocomotion 19)
// Turns the linked object's body facing.

void __stdcall MechLocomotionClass::Do_Turn(UnknownFacingDir dir)
{
    LinkedTo->unknown_2e0.Set_Desired(UnknownFacingDir(dir));
}


// void __stdcall MechLocomotionClass::Force_Immediate_Destination(CoordStruct coord) (0x00591980, ILocomotion 30)

void __stdcall MechLocomotionClass::Force_Immediate_Destination(CoordStruct coord)
{
    unknown_24 = CoordStruct(coord.X, coord.Y, coord.Z);
}


// Not exact (RA2_NON_MATCHING): FUN_005919a0 at 0x005919A0, best score 89.00, 2026-11-13; differs: same instruction count (1664), shape 99.52: instruction choice and registers differ
#ifdef RA2_NON_MATCHING
#define RA2_COORD_AS_CELL
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

// One step along the facing: the new coordinate, the bridge state when the step changes cells.
static inline void Step_Along(MechLocomotionClass *loco, UnknownFacingDir desired, int speed)
{
    Cell old_cell = loco->LinkedTo->unknown_slot104();
    CoordStruct new_coord = Move_Coord(Object_Coord(loco->LinkedTo), desired, speed);
    Cell new_cell = new_coord.As_Cell();
    if (old_cell != new_cell) {
        loco->LinkedTo->Mark(0);
        loco->LinkedTo->Set_Coord(new_coord);
        CellClass *old_cellptr = &Map[old_cell];
        CellClass *new_cellptr = &Map[new_cell];
        if (new_cellptr->unknown_11b == old_cellptr->unknown_11b - 4 && new_cellptr->IsUnderBridge) {
            loco->LinkedTo->unknown_79 = true;
        }
        if (!new_cellptr->IsUnderBridge && old_cellptr->IsUnderBridge) {
            loco->LinkedTo->unknown_79 = false;
        }
        loco->LinkedTo->Set_Height_AGL(0);
        loco->LinkedTo->Mark(1);
        Map[Object_Coord(loco->LinkedTo)].Trigger_Veins();
    } else {
        bool was_down = loco->LinkedTo->unknown_74;
        loco->LinkedTo->unknown_74 = false;
        loco->LinkedTo->Set_Coord(new_coord);
        loco->LinkedTo->Set_Height_AGL(0);
        loco->LinkedTo->unknown_74 = was_down;
    }
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
                    LinkedTo->Stop_Movement_Animation();
                    return;
                }
                LinkedTo->unknown_530 = Rules->unknown_13c8 * 900.0;
                if (!LinkedTo->unknown_4c3000(unknown_18.As_Cell(), 0, 0)) {
                    if (!LinkedTo->unknown_slot171(unknown_18)) {
                        LinkedTo->Assign_Destination(0, true);
                        return;
                    }
                    if (Coord_Length(LinkedTo->Center_Coord(), unknown_18) < Rules->unknown_1380 && !LinkedTo->unknown_370) {
                        LinkedTo->Assign_Destination(0, true);
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
                            if (!Map.Unknown_0054F7C0(nav_dest, dest, LinkedTo->Techno_Type_Class()->MovementZone, LinkedTo->Is_Moving_Onto_Bridge((TechnoClass *)0), Map[dest].IsUnderBridge, LinkedTo->unknown_slot189())) {
                                LinkedTo->Assign_Destination(0, true);
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
                    Cell const &enter = tube_ptr->EnterCell;
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
                LinkedTo->Assign_Destination(0, true);
                return;
            } else {
                Cell what = Adjacent_Tube(Object_Coord(LinkedTo).As_Cell(), dir);
                CoordStruct head_coord = Cell_Coord(what);
                head_coord.Z = Map.Get_Height_GL(head_coord);
                CellClass *what_cell = &Map[what];
                if (LinkedTo->unknown_79 ^ what_cell->IsUnderBridge) {
                    LinkedTo->unknown_57b = true;
                }
                int can_enter = LinkedTo->Can_Enter_Cell(&Map[what], dir, LinkedTo->Unknown_005D47D0(), 0, true);
                if (((can_enter != 5 && can_enter != 4) || !LinkedTo->Techno_Type_Class()->Crusher || what_cell->OverlayIndex != 0) && can_enter != 0) {
                    LinkedTo->Stop_Movement_Animation();
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
                        if (dist < Rules->unknown_1380 && !LinkedTo->HasAnyLink()
                            && abs(unknown_18.Z - Object_Coord(LinkedTo).Z) < unknown_geometry_g * 2
                            && Map[Object_Coord(LinkedTo)].unknown_ec != 10) {
                            LinkedTo->unknown_slot306(0.0);
                            LinkedTo->Assign_Destination(0, true);
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
                                    LinkedTo->Assign_Destination(0, true);
                                } else {
                                    LinkedTo->On_Movement_Blocked();
                                }
                            } else {
                                LinkedTo->JumpJet_To_Walk();
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

                    if (can_enter != 1 && can_enter != 7) {
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
                                    LinkedTo->Override_Mission(1, object, 0);
                                }
                            } else if (Map[what].OverlayIndex != -1 && OverlayTypes[Map[what].OverlayIndex]->Wall) {
                                LinkedTo->Override_Mission(1, &Map[what], 0);
                            }
                        }
                        LinkedTo->Path[0] = -1;
                        LinkedTo->unknown_slot306(0.0);
                        Stop_Moving();
                        LinkedTo->unknown_57a = false;
                    } else {
                        if (can_enter == 1) {
                            Map[what].unknown_478740();
                        }
                        if (!continue_moving) return;
                        LinkedTo->Path[0] = -1;
                        unknown_24 = unknown_geometry_l;
                        LinkedTo->unknown_530 = 0;
                        Movement_AI(false);
                        return;
                    }
                } else if (Mark_Head_To(head_coord)) {
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
                LinkedTo->unknown_57a = false;
                return;
            }
            LinkedTo->unknown_57a = false;
            return;
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
                LinkedTo->Assign_Destination(0, true);
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
            if (!LinkedTo->unknown_7d || LinkedTo->unknown_76) {
                return;
            }
            LinkedTo->Mark(1);
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
            Step_Along(this, desired, speed);
        }
    }
    LinkedTo->unknown_57a = false;
}

#undef RA2_COORD_AS_CELL
#endif

// bool MechLocomotionClass::Mark_Head_To(CoordStruct const &coord) (0x00592F70)
// Heads for a coordinate on the ground (raised by the bridge height on a
// bridge cell well below the object): clears the old head-to mark, takes
// the coordinate when its cell can be entered, and marks the head-to cell
// (or, when none and +0x7D is set, the object's own cell). True when a
// head-to coordinate was taken.

bool MechLocomotionClass::Mark_Head_To(CoordStruct const &coord)
{
    CoordStruct to = coord;
    if (to != unknown_geometry_l) {
        to.Z = Map.Get_Height_GL(to);
        if (Loco_Cell(coord)->Flags & 0x100) {
            if (Object_Coord(LinkedTo).Z > to.Z + unknown_geometry_g * 2 + unknown_geometry_g) {
                to.Z += unknown_geometry_j;
            }
        }
    }
    if (unknown_24 != unknown_geometry_l) {
        LinkedTo->Clear_Occupy_Bit(unknown_24);
    }
    if (Map[to].Goodie_Check(LinkedTo) && !LinkedTo->unknown_76) {
        unknown_24 = to;
    } else {
        unknown_24 = unknown_geometry_l;
        if (!LinkedTo->unknown_7d) {
            return false;
        }
    }
    if (unknown_24 != unknown_geometry_l) {
        LinkedTo->Set_Occupy_Bit(unknown_24);
        return true;
    }
    LinkedTo->Set_Occupy_Bit(Object_Coord(LinkedTo));
    return false;
}


// HRESULT __stdcall MechLocomotionClass::GetClassID(GUID *clsid) (0x00593120)

HRESULT __stdcall MechLocomotionClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_MechLocomotionClass;
    return S_OK;
}


// HRESULT __stdcall MechLocomotionClass::Load(IStream *stream) (0x00593160)
// LocomotionClass::Load, then rebuilt in place.

HRESULT __stdcall MechLocomotionClass::Load(IStream *stream)
{
    HRESULT result = LocomotionClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) MechLocomotionClass(NoInitClass());
    }
    return result;
}


// int __stdcall MechLocomotionClass::Layer() (0x00593190, ILocomotion 29)
// Layer 2 (hypothesis: ground).

int __stdcall MechLocomotionClass::Layer()
{
    return 2;
}


// bool __stdcall MechLocomotionClass::Is_Moving_Now() (0x005931A0, ILocomotion 32)
// Turning, or moving toward a head-to coordinate with some speed.

bool __stdcall MechLocomotionClass::Is_Moving_Now()
{
    if (LinkedTo->unknown_2e0.Is_Rotating()) {
        return true;
    }
    if (Is_Moving() && unknown_24 != unknown_geometry_l && LinkedTo->unknown_slot303() > 0) {
        return true;
    }
    return false;
}


// void __stdcall MechLocomotionClass::Mark_All_Occupation_Bits(int mark) (ILocomotion 39)
// Lifting the mark (0) clears the occupation of the head-to coordinate (object slot 59).

void __stdcall MechLocomotionClass::Mark_All_Occupation_Bits(int mark)
{
    if (mark == 0) {
        UnknownCoord434820 coord = Head_To_Coord();
        LinkedTo->Clear_Occupy_Bit(coord);
    }
}


// bool __stdcall MechLocomotionClass::Is_Moving_Here(CoordStruct to) (0x00593260, ILocomotion 40)
// The head-to coordinate is in the cell of the coordinate and within the
// height tolerance of it.

bool __stdcall MechLocomotionClass::Is_Moving_Here(CoordStruct to)
{
    CoordStruct head = Head_To_Coord();
    if (Coord_Cell(head) == Coord_Cell(to) && abs(head.Z - to.Z) <= unknown_geometry_g) {
        return true;
    }
    return false;
}


// int MechLocomotionClass::Size() (0x00593300): inline in shared/mech_locomotion.hpp, emitted with the vtable; no source of its own.


// void *MechLocomotionClass::`scalar deleting destructor'(unsigned int flags) (0x00593310)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.
