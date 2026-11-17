// walk.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/walk.json carries the addresses and the rows that
// stay in their own files.


#define RA2_COORD_AS_CELL  // the unit expands CoordStruct::As_Cell and the Cell comparisons (shared/coord.hpp)
// Not exact (RA2_NON_MATCHING) prelude: header switches the drafts below need (guard-on compile only)
#ifdef RA2_NON_MATCHING
#define RA2_COORD_TPOINT3D
#define RA2_COORD_LENGTH
#define RA2_COORD_FROM_CELL_INLINE
#endif

#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/walk_locomotion.hpp"
#include "shared/cell.hpp"
#include "shared/com_ptr.hpp"
#include "shared/foot.hpp"


// Header-static dynamic initializers of the unit at 0x0071DC10: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x0071DC10-0x0071DDF1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0071DC30, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_0071dc10.cpp; no source of its own.


// Dynamic initializer (0x0071DC60, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_0071dc10.cpp; no source of its own.


// Dynamic initializer (0x0071DC80, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_0071dc10.cpp; no source of its own.


// Dynamic initializer (0x0071DCA0, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_0071dc10.cpp; no source of its own.


// Dynamic initializer (0x0071DCC0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_0071dc10.cpp; no source of its own.


// Dynamic initializer (0x0071DCE0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_0071dc10.cpp; no source of its own.


// Dynamic initializer (0x0071DD00, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_0071dc10.cpp; no source of its own.


// Dynamic initializer (0x0071DD40, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_0071dc10.cpp; no source of its own.


// Dynamic initializer (0x0071DD70, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_0071dc10.cpp; no source of its own.


// Dynamic initializer (0x0071DDA0, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_0071dc10.cpp; no source of its own.


// Dynamic initializer (0x0071DDD0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_0071dc10.cpp; no source of its own.


// Dynamic initializer (0x0071DDE0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_0071dc10.cpp; no source of its own.


// WalkLocomotionClass::WalkLocomotionClass() (0x0071DE00)

WalkLocomotionClass::WalkLocomotionClass() :
    unknown_1c(unknown_geometry_l),
    unknown_28(unknown_geometry_l),
    IsMoving(false),
    IsProcessingMovement(false),
    IsReallyMoving(false)
{
}


// WalkLocomotionClass::~WalkLocomotionClass() (0x0071DE70)
// Releases the carried locomotor.

WalkLocomotionClass::~WalkLocomotionClass()
{
}


// bool __stdcall WalkLocomotionClass::Is_Moving() (0x0071DEA0, ILocomotion 4)

bool __stdcall WalkLocomotionClass::Is_Moving()
{
    return IsMoving;
}


// bool __stdcall WalkLocomotionClass::Is_Moving_Now() (0x0071DEB0, ILocomotion 32)
// Moving, with speed, toward a head-to coordinate.

bool __stdcall WalkLocomotionClass::Is_Moving_Now()
{
    if (Is_Moving() && LinkedTo->Speed > 0.0 && unknown_28 != unknown_geometry_l) {
        return true;
    }
    return false;
}


// CoordStruct __stdcall WalkLocomotionClass::Destination() (0x0071DF10, ILocomotion 5)

CoordStruct __stdcall WalkLocomotionClass::Destination()
{
    if (Is_Moving()) {
        return CoordStruct(unknown_1c.X, unknown_1c.Y, unknown_1c.Z);
    }
    return CoordStruct(unknown_geometry_l.X, unknown_geometry_l.Y, unknown_geometry_l.Z);
}


// CoordStruct __stdcall WalkLocomotionClass::Head_To_Coord() (0x0071DF70, ILocomotion 6)

CoordStruct __stdcall WalkLocomotionClass::Head_To_Coord()
{
    if (unknown_28 != unknown_geometry_l) {
        return CoordStruct(unknown_28.X, unknown_28.Y, unknown_28.Z);
    }
    CoordStruct coord = LinkedTo->Coord;
    return coord;
}


// bool __stdcall WalkLocomotionClass::Process() (0x0071DFF0, ILocomotion 16)
// One step (0x0071E220) under the processing flag, then whether moving.

bool __stdcall WalkLocomotionClass::Process()
{
    IsProcessingMovement = true;
    unknown_71e220(true);
    IsProcessingMovement = false;
    return Is_Moving();
}


// void __stdcall WalkLocomotionClass::Move_To(CoordStruct to) (0x0071E020, ILocomotion 17)
// Unless the linked object is busy (slots 201, 111, 112), heads for the
// coordinate (raised on flagged cells); a null coordinate without a
// head-to coordinate ends the walk (slot 308).

void __stdcall WalkLocomotionClass::Move_To(CoordStruct to)
{
    if (!LinkedTo->unknown_slot201() && !LinkedTo->unknown_slot111() && !LinkedTo->unknown_slot112()) {
        unknown_1c = CoordStruct(to.X, to.Y, to.Z);
        if (to != unknown_geometry_l) {
            to = CoordStruct(to.X, to.Y, to.Z);
            if (Loco_Cell(to)->Flags & 0x100) {
                unknown_1c.Z += Tactical_Pixel_To_Z_Lepton(60);
            }
            IsMoving = true;
        } else if (unknown_28 == unknown_geometry_l) {
            bool moving = IsMoving;
            IsMoving = false;
            if (moving) {
                LinkedTo->unknown_slot308();
            }
        }
    }
}


// void __stdcall WalkLocomotionClass::Stop_Moving() (0x0071E110, ILocomotion 18)
// Drops the destination; without a head-to coordinate the walk ends and
// the linked object is told (slot 308).

void __stdcall WalkLocomotionClass::Stop_Moving()
{
    unknown_1c = unknown_geometry_l;
    if (unknown_28 == unknown_geometry_l) {
        IsMoving = false;
        IsReallyMoving = false;
        LinkedTo->unknown_slot308();
    }
}


// void __stdcall WalkLocomotionClass::Do_Turn(UnknownFacingDir dir) (0x0071E170, ILocomotion 19)
// Faces the direction at once.

void __stdcall WalkLocomotionClass::Do_Turn(UnknownFacingDir dir)
{
    LinkedTo->PrimaryFacing.Set(UnknownFacingDir(dir));
}


// void __stdcall WalkLocomotionClass::Force_Immediate_Destination(CoordStruct coord) (0x0071E1A0, ILocomotion 30)
// Sets the head-to coordinate; with neither it nor a destination the walk
// ends (slot 308 when it was moving).

void __stdcall WalkLocomotionClass::Force_Immediate_Destination(CoordStruct coord)
{
    bool moving = IsMoving;
    unknown_28 = CoordStruct(coord.X, coord.Y, coord.Z);
    if (unknown_28 == unknown_geometry_l && unknown_1c == unknown_geometry_l) {
        IsMoving = false;
        if (moving) {
            LinkedTo->unknown_slot308();
        }
    }
}


// Not exact (RA2_NON_MATCHING): WalkLocomotionClass::unknown_71e220 at 0x0071E220, best score 89.83, 2026-11-13; differs: 1505 instructions in the original, 1483 in the draft (shape 97.46)
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
extern CoordStruct unknown_coord_af9060;   // 0x00AF9060

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
    return (Map.Get_Height_GL(exit) - current_height) / count;
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
                if (LinkedTo->PathDelay != 0) {
                    LinkedTo->Stop_Movement_Animation();
                    return;
                }
                LinkedTo->PathDelay = Rules->PathDelay * 900.0;
                if (!LinkedTo->unknown_4c3000(unknown_1c.As_Cell(), 0, 0)) {
                    IsReallyMoving = false;
                    if (LinkedTo->unknown_slot171(unknown_1c)) {
                        LinkedTo->On_Movement_Blocked();
                        if (Coord_Length(LinkedTo->Center_Coord(), unknown_1c) < Rules->CloseEnoughDistance && !LinkedTo->IsTethered) {
                            LinkedTo->Assign_Destination(0, true);
                        } else if (LinkedTo->unknown_53c != 0) {
                            LinkedTo->unknown_53c--;
                        } else {
                            if (LinkedTo->IsNewNavCom) {
                                Voc_Play(Rules->ScoldSound, 1.0f, 0x2000, 0);
                            }
                            LinkedTo->IsNewNavCom = false;
                            Cell dest = unknown_1c.As_Cell();
                            if (LinkedTo->IsLocked) {
                                Cell nav_dest = LinkedTo->Destination_Coord(0).As_Cell();
                                if (!Map.Unknown_0054F7C0(nav_dest, dest, LinkedTo->Techno_Type_Class()->MovementZone, LinkedTo->Is_Moving_Onto_Bridge(0), Map[dest].IsUnderBridge, LinkedTo->unknown_slot189())) {
                                    LinkedTo->Assign_Destination(0, true);
                                }
                            }
                            if (LinkedTo->TarCom != 0) {
                                Cell tar_dest = LinkedTo->TarCom->Destination_Coord(LinkedTo).As_Cell();
                                if (LinkedTo->IsLocked && LinkedTo->TarCom != 0) {
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
                    memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);
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
                unknown_28 = unknown_coord_af9060;
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
                    IsReallyMoving = false;
                    LinkedTo->Stop_Movement_Animation();
                    if (move == 6) {
                        CellClass &cellptr = Map[cell];
                        if (first_pass) {
                            LinkedTo->Path[0] = -1;
                            unknown_28 = unknown_coord_af9060;
                            LinkedTo->PathDelay = 0;
                            unknown_71e220(false);
                        } else {
                            int dist = Distance3(LinkedTo->Center_Coord(), unknown_1c);
                            if (dist < Rules->CloseEnoughDistance && !LinkedTo->HasAnyLink()
                                && abs(unknown_1c.Z - Object_Coord(LinkedTo).Z) < unknown_geometry_g * 2
                                && Map[Object_Coord(LinkedTo)].Land != 10) {
                                unknown_28 = unknown_coord_af9060;
                                Stop_Moving();
                                LinkedTo->unknown_slot306(0.0);
                                LinkedTo->Assign_Destination(0, true);
                                LinkedTo->Path[0] = -1;
                            } else {
                                bool isbridge = cellptr.IsUnderBridge && abs(Object_Coord(LinkedTo).Z / unknown_geometry_g - cellptr.Height) > 2;
                                cellptr.Incoming(&unknown_coord_af9060, true, true, isbridge);
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
                            unknown_28 = unknown_coord_af9060;
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
                        unknown_28 = unknown_coord_af9060;
                        LinkedTo->PathDelay = 0;
                        unknown_71e220(false);
                    }
                    return;
                }
                if (Mark_Head_To(coord)) {
                    IsReallyMoving = true;
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
    IsReallyMoving = true;
    if (Distance2(Flat(LinkedTo->Center_Coord()), Flat(unknown_28)) < 17) {
        LinkedTo->Mark(0);
        if (LinkedTo->Path[0] == -1) {
            LinkedTo->Path[1] = -1;
        }
        memcpy(&LinkedTo->Path[0], &LinkedTo->Path[1], sizeof(LinkedTo->Path[0]) * 23);
        LinkedTo->Path[23] = -1;
        LinkedTo->Set_Coord(unknown_28);
        LinkedTo->LastPathingCell = unknown_28.As_Cell();
        LinkedTo->Set_Height_AGL(0);
        LinkedTo->IsToPathAroundBlockage = false;
        Mark_Head_To(unknown_coord_af9060);
        if (LinkedTo->Path[0] == -1) {
            LinkedTo->unknown_slot306(0.0);
        }
        LinkedTo->unknown_slot93(2);
        if (LinkedTo->IsActive && !LinkedTo->IsInLimbo && !LinkedTo->IsFalling) {
            if (unknown_1c == unknown_coord_af9060
                || (LinkedTo->Destination_Coord(0).As_Cell() == unknown_1c.As_Cell()
                    && abs(LinkedTo->Destination_Coord(0).Z - unknown_1c.Z) < unknown_geometry_g * 2)) {
                LinkedTo->Assign_Destination(0, true);
                LinkedTo->unknown_slot306(0.0);
                unknown_28 = unknown_coord_af9060;
                Stop_Moving();
                IsReallyMoving = false;
            }
            LinkedTo->Mark(1);
            LinkedTo->IsNewNavCom = false;
        }
        return;
    }
    if (LinkedTo->unknown_slot201()) {
        IsReallyMoving = false;
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

#undef RA2_COORD_AS_CELL
#endif

// bool WalkLocomotionClass::Mark_Head_To(CoordStruct const &coord) (0x0071F590; TibSun walk.cpp Mark_Head_To)
// Releases the old head-to mark, claims the free spot nearest coord (any spot when the
// navigation target is the unit, aircraft or building at coord; the bridge spot when
// the object is well above the ground of an under-bridge cell) and marks it, or the
// object's own spot. True when a head-to coordinate was taken.

bool WalkLocomotionClass::Mark_Head_To(CoordStruct const &coord)
{
    if (unknown_28 != unknown_geometry_l) {
        LinkedTo->Clear_Occupy_Bit(unknown_28);
    } else {
        LinkedTo->Clear_Occupy_Bit(Object_Coord(LinkedTo));
    }

    CoordStruct crd = coord;
    bool any_spot = false;

    if (coord != unknown_geometry_l) {
        if (LinkedTo->Get_Mission() == 8 || LinkedTo->Get_Mission() == 9 || LinkedTo->Get_Mission() == 7
            || LinkedTo->Get_Mission() == 11 || LinkedTo->Get_Mission() == 25) {
            ObjectClass *unit = (ObjectClass *)LinkedTo->NavCom;
            if (unit != 0 && unit->Kind_Of() == 1 && unit->Get_Cell() == coord.As_Cell()) {
                any_spot = true;
            }
            ObjectClass *aircraft = (ObjectClass *)LinkedTo->NavCom;
            if (aircraft != 0 && aircraft->Kind_Of() == 2 && aircraft->Get_Cell() == coord.As_Cell()) {
                any_spot = true;
            }
            ObjectClass *building = (ObjectClass *)LinkedTo->NavCom;
            if (building != 0 && building->Kind_Of() == 6 && Map[coord].Cell_Building() == building) {
                any_spot = true;
            }
        }

        bool isbridge = Map[crd].IsUnderBridge && Object_Coord(LinkedTo).Z > unknown_geometry_g * 3 + Map.Get_Height_GL(crd);
        unknown_28 = Map[crd].Closest_Free_Spot(crd, any_spot, isbridge);

        if (!Map[unknown_28].Goodie_Check(LinkedTo) && !LinkedTo->IsInLimbo) {
            unknown_28 = unknown_geometry_l;
            if (!LinkedTo->IsActive) {
                return false;
            }
        }
    } else {
        unknown_28 = coord;
    }

    if (unknown_28 != unknown_geometry_l) {
        LinkedTo->Set_Occupy_Bit(unknown_28);
        return true;
    }

    LinkedTo->Set_Occupy_Bit(Object_Coord(LinkedTo));
    return false;
}


// HRESULT __stdcall WalkLocomotionClass::GetClassID(GUID *clsid) (0x0071F900)

HRESULT __stdcall WalkLocomotionClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_WalkLocomotionClass;
    return S_OK;
}


// HRESULT __stdcall WalkLocomotionClass::Load(IStream *stream) (0x0071F940)
// LocomotionClass::Load, rebuilt in place; then a flag and, when set, the
// carried locomotor (OleLoadFromStream).

HRESULT __stdcall WalkLocomotionClass::Load(IStream *stream)
{
    HRESULT result = LocomotionClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) WalkLocomotionClass(NoInitClass());
        bool piggybacking;
        stream->Read(&piggybacking, sizeof(piggybacking), 0);
        if (piggybacking) {
            result = OleLoadFromStream(stream, IID_ILocomotion, (void **)&Piggybacker);
        }
    }
    return result;
}


// HRESULT __stdcall WalkLocomotionClass::Save(IStream *stream, BOOL clear_dirty) (0x0071F9C0)
// LocomotionClass::Save, then whether a locomotor is carried and, if so,
// that locomotor (OleSaveToStream through its IPersistStream).

HRESULT __stdcall WalkLocomotionClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = LocomotionClass::Save(stream, clear_dirty);
    if (FAILED(result)) {
        return result;
    }
    bool piggybacking;
    if (Piggybacker != 0) {
        piggybacking = true;
        stream->Write(&piggybacking, sizeof(piggybacking), 0);
        UnknownComPtr<IPersistStream, &unknown_uuid_IPersistStream_7d5f38> persist(static_cast<ILocomotion *>(Piggybacker));
        return OleSaveToStream(persist, stream);
    }
    piggybacking = false;
    stream->Write(&piggybacking, sizeof(piggybacking), 0);
    return result;
}


// int __stdcall WalkLocomotionClass::Layer() (0x0071FAA0, ILocomotion 29)

int __stdcall WalkLocomotionClass::Layer()
{
    return 2;
}


// HRESULT __stdcall WalkLocomotionClass::QueryInterface(GUID const &iid, void **object) (0x0071FAB0)
// The base interfaces, then IPiggyback.

HRESULT __stdcall WalkLocomotionClass::QueryInterface(GUID const &iid, void **object)
{
    HRESULT result = LocomotionClass::QueryInterface(iid, object);
    if (result == E_NOINTERFACE) {
        if (iid == IID_IPiggyback) {
            *object = static_cast<IPiggyback *>(this);
        }
        if (*object == 0) {
            return E_NOINTERFACE;
        }
        AddRef();
        result = S_OK;
    }
    return result;
}


// HRESULT __stdcall WalkLocomotionClass::Begin_Piggyback(ILocomotion *pointer) (0x0071FB10, IPiggyback 3)
// Carries the locomotor unless one is already carried.

HRESULT __stdcall WalkLocomotionClass::Begin_Piggyback(ILocomotion *pointer)
{
    if (pointer == 0) {
        return E_POINTER;
    }
    if (Piggybacker == 0) {
        Piggybacker = pointer;
        return S_OK;
    }
    return E_FAIL;
}


// HRESULT __stdcall WalkLocomotionClass::End_Piggyback(ILocomotion **pointer) (0x0071FB60, IPiggyback 4)
// Hands the carried locomotor (and its reference) back.

HRESULT __stdcall WalkLocomotionClass::End_Piggyback(ILocomotion **pointer)
{
    if (pointer == 0) {
        return E_POINTER;
    }
    if (Piggybacker != 0) {
        *pointer = Piggybacker.Pointer;
        Piggybacker.Pointer = 0;
        return S_OK;
    }
    return S_FALSE;
}


// bool __stdcall WalkLocomotionClass::Is_Ok_To_End() (0x0071FBA0, IPiggyback 5)
// Once stopped (and not processing) while carrying a locomotor.

bool __stdcall WalkLocomotionClass::Is_Ok_To_End()
{
    if (!Is_Moving() && Piggybacker != 0 && !IsProcessingMovement) {
        return true;
    }
    return false;
}


// HRESULT __stdcall WalkLocomotionClass::Piggyback_CLSID(GUID *classid) (0x0071FBD0, IPiggyback 6)
// The class ID of the carried locomotor, or this locomotor's own when none is
// carried; each is queried for IPersist (TibSun Drive.cpp Piggyback_CLSID).

HRESULT __stdcall WalkLocomotionClass::Piggyback_CLSID(GUID *classid)
{
    if (classid == 0) {
        return E_POINTER;
    }
    if (Piggybacker) {
        UnknownComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist(Piggybacker);
        if (persist == 0) {
            return E_FAIL;
        }
        return persist->GetClassID(classid);
    }
    UnknownHeldComPtr<IPersist, &unknown_uuid_IPersist_7cf208> persist(static_cast<IPersistStream *>(this));
    if (persist == 0) {
        return E_FAIL;
    }
    return persist->GetClassID(classid);
}


// void __stdcall WalkLocomotionClass::Mark_All_Occupation_Bits(int mark) (ILocomotion 39)
// Lifting the mark (0) clears the occupation of the head-to coordinate (object slot 59).

void __stdcall WalkLocomotionClass::Mark_All_Occupation_Bits(int mark)
{
    if (mark == 0) {
        UnknownCoord434820 coord = Head_To_Coord();
        LinkedTo->Clear_Occupy_Bit(coord);
    }
}


// bool __stdcall WalkLocomotionClass::Is_Moving_Here(CoordStruct to) (0x0071FD30, ILocomotion 40)
// The head-to coordinate is in the cell of the coordinate and within the
// height tolerance of it.

bool __stdcall WalkLocomotionClass::Is_Moving_Here(CoordStruct to)
{
    CoordStruct head = Head_To_Coord();
    if (Coord_Cell(head) == Coord_Cell(to) && abs(head.Z - to.Z) <= unknown_geometry_g) {
        return true;
    }
    return false;
}


// bool __stdcall WalkLocomotionClass::Is_Really_Moving_Now() (0x0071FDD0, ILocomotion 42)

bool __stdcall WalkLocomotionClass::Is_Really_Moving_Now()
{
    return IsReallyMoving;
}


// unsigned long __stdcall WalkLocomotionClass::AddRef() (0x0071FDE0)

unsigned long __stdcall WalkLocomotionClass::AddRef()
{
    return LocomotionClass::AddRef();
}


// unsigned long __stdcall WalkLocomotionClass::Release() (0x0071FDF0)

unsigned long __stdcall WalkLocomotionClass::Release()
{
    return LocomotionClass::Release();
}


// bool __stdcall WalkLocomotionClass::Is_Piggybacking() (0x0071FE00, IPiggyback 7)

bool __stdcall WalkLocomotionClass::Is_Piggybacking()
{
    return Piggybacker != 0;
}


// void __stdcall WalkLocomotionClass::Stop_Movement_Animation() (0x0071FE20, ILocomotion 43)

void __stdcall WalkLocomotionClass::Stop_Movement_Animation()
{
    IsReallyMoving = false;
}


// int WalkLocomotionClass::Size() (0x0071FE30): inline in shared/walk_locomotion.hpp, emitted with the vtable; no source of its own.


// void *WalkLocomotionClass::`scalar deleting destructor'(unsigned int flags) (0x0071FE40)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.


// WalkLocomotionClass::QueryInterface adjustor thunk for ILocomotion (0x0071FE90)
// Compiler generated (this adjusted, then QueryInterface). No source of its own.


// WalkLocomotionClass::AddRef adjustor thunk for ILocomotion (0x0071FEA0)
// Compiler generated (this adjusted, then AddRef). No source of its own.


// WalkLocomotionClass::Release adjustor thunk for ILocomotion (0x0071FEB0)
// Compiler generated (this adjusted, then Release). No source of its own.


// WalkLocomotionClass::QueryInterface adjustor thunk for IPiggyback (0x0071FEC0)
// Compiler generated (this adjusted, then QueryInterface). No source of its own.


// WalkLocomotionClass::AddRef adjustor thunk for IPiggyback (0x0071FED0)
// Compiler generated (this adjusted, then AddRef). No source of its own.


// WalkLocomotionClass::Release adjustor thunk for IPiggyback (0x0071FEE0)
// Compiler generated (this adjusted, then Release). No source of its own.
