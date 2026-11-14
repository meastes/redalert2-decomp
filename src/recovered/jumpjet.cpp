// jumpjet.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/jumpjet.json carries the addresses and the rows that
// stay in their own files.


#include "shared/jumpjet_locomotion.hpp"
#include "shared/house.hpp"
#include "shared/infantry.hpp"
#include "shared/id_source.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/cell.hpp"
#include "shared/techno_type.hpp"
#include "shared/rules.hpp"
#include "shared/building_type.hpp"
#include "shared/fast_math.hpp"
#include "shared/building.hpp"


// bool __stdcall JumpjetLocomotionClass::Is_Moving() (0x0052F350, ILocomotion 4)

bool __stdcall JumpjetLocomotionClass::Is_Moving()
{
    return unknown_48;
}


// CoordStruct __stdcall JumpjetLocomotionClass::Destination() (0x0052F360, ILocomotion 5)
// The destination while moving, otherwise the null coordinate.

CoordStruct __stdcall JumpjetLocomotionClass::Destination()
{
    if (Is_Moving()) {
        return CoordStruct(unknown_3c.X, unknown_3c.Y, unknown_3c.Z);
    }
    return CoordStruct(unknown_geometry_l.X, unknown_geometry_l.Y, unknown_geometry_l.Z);
}


// bool __stdcall JumpjetLocomotionClass::Process() (0x0052F3C0, ILocomotion 16)
// While moving: the movement step (0x00530B00); a dead object stops here.
// Held locomotors (house 0x0051FC80) outside states 0 and 5 call slot 224;
// a crashing techno (+0x37D) with height enters state 5 (infantry play
// sequence 0x22). Then the state routine runs and the object is revealed
// or shrouded for the player. A changed layer resubmits the object.

bool __stdcall JumpjetLocomotionClass::Process()
{
    int layer = Layer();
    if (Is_Moving() || Is_Moving_Now()) {
        unknown_530b00();
        if (!LinkedTo->unknown_7d) {
            return false;
        }
        if (LinkedTo->House->Unknown_0051FC80() && unknown_4c != 0 && unknown_4c != 5) {
            LinkedTo->unknown_slot224(0);
        }
        if (LinkedTo->unknown_37d && unknown_4c != 5 && unknown_4c != 6 && LinkedTo->Get_Height_AGL() > 0) {
            unknown_78 = -5;
            unknown_4c = 5;
            if (LinkedTo->Kind_Of() == 0xF) {
                ((InfantryClass *)LinkedTo)->unknown_slot311(0x22, false, false);
            }
        }
        switch (unknown_4c) {
        case 0:
            unknown_52fca0();
            break;
        case 1:
            unknown_52fd50();
            break;
        case 2:
            unknown_52fff0();
            break;
        case 3:
            unknown_530180();
            break;
        case 4:
            unknown_530550();
            break;
        case 5:
            unknown_530940();
            break;
        case 6:
            unknown_530ad0();
            break;
        }
        if (LinkedTo->unknown_77 && LinkedTo->House != unknown_player_a35db4) {
            if (Map.Is_Shrouded(Object_Coord(LinkedTo)) ||
                ((unknown_id_source_a3d290->unknown_000 & 0x1000) && Map.Is_Fogged(Object_Coord(LinkedTo)))) {
                LinkedTo->Unselect();
            }
        }
        if (!LinkedTo->unknown_373 && LinkedTo->House != unknown_player_a35db4) {
            if (!Map.Is_Shrouded(Object_Coord(LinkedTo))) {
                LinkedTo->Revealed(unknown_player_a35db4);
            }
        }
    }
    if (LinkedTo->unknown_7d && Layer() != layer) {
        Map.Submit(LinkedTo);
    }
    return false;
}


// void JumpjetLocomotionClass::Move_To(CoordStruct to) (0x0052F600)

void __stdcall JumpjetLocomotionClass::Move_To(CoordStruct to)
{
    if (unknown_3c != unknown_geometry_l && unknown_4c != 0 && unknown_88) {
        LinkedTo->Clear_Occupy_Bit(unknown_3c);
        unknown_88 = false;
    }
    unknown_3c = CoordStruct(to.X, to.Y, to.Z);
    if (to != unknown_geometry_l) {
        Cell cell;
        if (LinkedTo->Kind_Of() == 1 && !((UnitClass *)LinkedTo)->Type->BalloonHover) {
            cell = Map.Nearby_Location(Coord_Cell(to), LinkedTo->Techno_Type_Class()->SpeedType, -1, 9, false, Point2D(1, 1), 0, 0, 0, 0, Cell(0, 0), 0);
        } else {
            cell = Map.Nearby_Location(Coord_Cell(to), LinkedTo->Techno_Type_Class()->SpeedType, -1, 9, (Map[CoordStruct(to)].unknown_140 >> 8) & 1, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
        }
        CoordStruct coord = unknown_5310a0(CoordStruct(cell));
        if (coord != unknown_geometry_l) {
            if (LinkedTo->Kind_Of() != 1 || (!LinkedTo->Techno_Type_Class()->BalloonHover && LinkedTo->MissionQueue != 7 && LinkedTo->Get_Mission() != 7)) {
                unknown_3c = coord;
            }
            if (LinkedTo->MissionQueue != 7 && LinkedTo->Get_Mission() != 7) {
                LinkedTo->unknown_494 = &Map[unknown_3c];
            }
            unknown_48 = true;
            if (unknown_4c == 4) {
                unknown_4c = 1;
                unknown_78 = Height;
            }
        }
    } else {
        unknown_48 = false;
    }
}


// void __stdcall JumpjetLocomotionClass::Stop_Moving() (0x0052F960, ILocomotion 18)
// While moving (and 0x00A40774 clear): the target claim is released; the
// jumpjet then moves to the nearest free cell (0x005502E0 for its speed
// type and movement zone) at ground (bridge) height. With no free cell a
// living object is destroyed with the rules' C4 warhead and the target
// cleared.

void __stdcall JumpjetLocomotionClass::Stop_Moving()
{
    if (unknown_a40774 == 0 && unknown_48) {
        if (unknown_3c != unknown_geometry_l && unknown_4c != 0 && unknown_88) {
            LinkedTo->Clear_Occupy_Bit(unknown_3c);
            unknown_88 = false;
        }
        Cell here = Coord_Cell(Object_Coord(LinkedTo));
        Cell cell = Map.Nearby_Location(here, LinkedTo->Techno_Type_Class()->SpeedType, -1, LinkedTo->Techno_Type_Class()->MovementZone, false, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
        if (cell != unknown_geometry_k) {
            CoordStruct coord = Cell_Coord(cell);
            coord.Z = Map.Get_Height_GL(coord);
            if (Map[cell].unknown_140 & 0x100) {
                coord.Z += unknown_geometry_j;
            }
            Move_To(CoordStruct(coord.X, coord.Y, coord.Z));
            return;
        }
        if (LinkedTo->Strength > 0) {
            LinkedTo->Take_Damage(&LinkedTo->Strength, 0, Rules->unknown_c74, 0, 1, 1);
            unknown_3c = unknown_geometry_l;
        }
    }
}


// void __stdcall JumpjetLocomotionClass::Do_Turn(UnknownFacingDir dir) (0x0052FB70, ILocomotion 19)
// Turns the body facing at once.

void __stdcall JumpjetLocomotionClass::Do_Turn(UnknownFacingDir dir)
{
    LinkedTo->unknown_2e0.Set(UnknownFacingDir(dir));
}


// HRESULT __stdcall JumpjetLocomotionClass::GetClassID(GUID *clsid) (0x0052FBA0)

HRESULT __stdcall JumpjetLocomotionClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_JumpjetLocomotionClass;
    return S_OK;
}


// HRESULT __stdcall JumpjetLocomotionClass::Load(IStream *stream) (0x0052FBE0)
// LocomotionClass::Load, then the vtables are restored in place.

HRESULT __stdcall JumpjetLocomotionClass::Load(IStream *stream)
{
    HRESULT result = LocomotionClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) JumpjetLocomotionClass(NoInitClass());
    }
    return result;
}


// int __stdcall JumpjetLocomotionClass::Layer() (0x0052FC10, ILocomotion 29)
// The layer from the height above ground (slot 108; less the bridge height
// over a bridge cell when the object is not on the bridge): 2 on the
// ground, 4 at or above the jumpjet height (+0x28), 3 between.

int __stdcall JumpjetLocomotionClass::Layer()
{
    int height = LinkedTo->Get_Height_AGL();
    if (!LinkedTo->unknown_79) {
        if ((Loco_Cell(Object_Coord(LinkedTo))->Flags & 0x100) && height >= unknown_geometry_j && !LinkedTo->unknown_7a) {
            height -= unknown_geometry_j;
        }
    }
    if (height == 0) {
        return 2;
    }
    return height >= Height ? 4 : 3;
}


// void JumpjetLocomotionClass::unknown_52fca0() (0x0052FCA0)
// Takes off when moving: the object's slot 306 ratio goes to 1.0, the
// facing (+0x50) is set to the object's facing (+0x2E0), both speeds (+0x68,
// +0x70) are cleared and the target height (+0x78) becomes the jumpjet
// height; unless the owner's house holds the locomotors (0x0051FC80), an
// object at no cell is reported to the arrival tracker and the state
// (+0x4C) becomes 1.

void JumpjetLocomotionClass::unknown_52fca0()
{
    if (Is_Moving()) {
        LinkedTo->unknown_slot306(1.0);
        unknown_50.Set(LinkedTo->unknown_2e0.Current());
        unknown_68 = 0;
        unknown_70 = 0;
        unknown_78 = Height;
        if (!LinkedTo->House->Unknown_0051FC80()) {
            if (LinkedTo->unknown_slot178() == unknown_geometry_k) {
                unknown_tracker_839ea0.Unknown_004131E0(LinkedTo);
            }
            unknown_4c = 1;
        }
    }
}


// void JumpjetLocomotionClass::unknown_52fd50() (0x0052FD50)
// Process state 1 (ascending): the height above ground (less a bridge)
// against the target height (+0x78); a changed cell is reported to the
// tracker. At the height, a crowded cell (0x004132E0) sends the object to a
// random adjacent cell (state 3), else it claims its cell (state 2). Past a
// quarter (BalloonHover: half) of the height it gets the jumpjet speed and
// turns toward the target; leaving a factory or barracks notifies the
// object (0x117B); above the target cell a non-balloon starts cruising.
extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;

void JumpjetLocomotionClass::unknown_52fd50()
{
    int height = LinkedTo->Get_Height_AGL();
    if (!LinkedTo->unknown_79 && (Map[Object_Coord(LinkedTo)].unknown_140 & 0x100) && height >= unknown_geometry_j) {
        height -= unknown_geometry_j;
    }
    Cell cell = LinkedTo->unknown_slot104();
    if (cell != LinkedTo->unknown_458) {
        unknown_tracker_839ea0.Update_00413600(LinkedTo, LinkedTo->unknown_458, cell);
    }
    if (height >= unknown_78) {
        if (unknown_tracker_839ea0.Unknown_004132E0(LinkedTo)) {
            int facing = Random_Pick(0, 7);
            CellClass &here = Map[cell];
            LinkedTo->Assign_Destination(&here.Adjacent_Cell(facing), true);
            unknown_4c = 3;
        } else {
            ((CellClass *)LinkedTo->Get_Cell_Ptr())->Set_unknown_e0(LinkedTo);
            unknown_4c = 2;
        }
        return;
    }
    int limit;
    if (LinkedTo->Kind_Of() == 1 && LinkedTo->Techno_Type_Class()->BalloonHover) {
        limit = unknown_78 / 2;
    } else {
        limit = unknown_78 / 4;
    }
    if (height > limit) {
        unknown_70 = Speed;
        CoordStruct from = LinkedTo->Coord;
        CoordStruct const &to = unknown_3c;
        UnknownFacingDir dir;
        if (from.X == to.X && from.Y == to.Y) {
            dir.Raw = 0;
        }
        dir = UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y) - to.Y, double(to.X) - from.X)
            - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
        unknown_50.Set_Desired(dir);
        TechnoClass *contact = LinkedTo->Contact(0);
        if (contact != 0 && contact->Kind_Of() == 6) {
            BuildingTypeClass *type = ((BuildingClass *)contact)->Type;
            if (type->WeaponsFactory || type->GDIBarracks || type->NODBarracks) {
                LinkedTo->INoticeSink_Unknown(0x117B, 0);
            }
        }
        if (cell == Coord_Cell(unknown_3c) && !LinkedTo->Techno_Type_Class()->BalloonHover) {
            unknown_78 = LinkedTo->Get_Height_AGL();
            unknown_4c = 3;
        }
    }
}


// void JumpjetLocomotionClass::unknown_52fff0() (0x0052FFF0)
// Process state 2 (hovering) while moving: over the target X/Y with no
// +0x228 target, the state becomes 2 for a BalloonHover type, else 4, and
// the techno check 0x006D1470 runs; elsewhere the facing (+0x50) turns toward
// the target, the cell claims (+0xE0) of the old and current cells are
// released and the state becomes 3.
extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;

void JumpjetLocomotionClass::unknown_52fff0()
{
    if (Is_Moving()) {
        CoordStruct head = unknown_3c;
        CoordStruct coord = LinkedTo->Coord;
        if (head.X == coord.X && head.Y == coord.Y) {
            if (LinkedTo->unknown_228 == 0) {
                if (LinkedTo->Techno_Type_Class()->BalloonHover) {
                    unknown_4c = 2;
                    LinkedTo->unknown_6d1470();
                } else {
                    unknown_4c = 4;
                    LinkedTo->unknown_6d1470();
                }
            }
        } else {
            CoordStruct from = LinkedTo->Coord;
            CoordStruct const &to = unknown_3c;
            UnknownFacingDir dir;
            if (from.X == to.X && from.Y == to.Y) {
                dir.Raw = 0;
            }
            dir = UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y) - to.Y, double(to.X) - from.X)
                - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
            unknown_50.Set_Desired(dir);
            if (((CellClass *)LinkedTo->Get_Cell_Ptr())->unknown_e0 == 0 && Map[LinkedTo->unknown_458].unknown_e0 == LinkedTo) {
                Map[LinkedTo->unknown_458].Set_unknown_e0(0);
            }
            ((CellClass *)LinkedTo->Get_Cell_Ptr())->Set_unknown_e0(0);
            unknown_4c = 3;
        }
    }
}


extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;
void JumpjetLocomotionClass::unknown_530180()
{
    CoordStruct position = LinkedTo->Coord;
    Cell cell = LinkedTo->unknown_slot104();
    if (cell != LinkedTo->unknown_458) {
        unknown_tracker_839ea0.Update_00413600(LinkedTo, LinkedTo->unknown_458, cell);
    }
    CoordStruct const &to = unknown_3c;
    UnknownFacingDir dir;
    if (position.X == to.X && position.Y == to.Y) dir.Raw = 0;
    dir = UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(position.Y) - to.Y, double(to.X) - position.X)
        - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
    unknown_50.Set_Desired(dir);
    UnknownFacingDir difference = unknown_50.Difference();
    bool balloon = LinkedTo->Techno_Type_Class()->BalloonHover;
    TPoint2D<int> here(position.X, position.Y);
    TPoint2D<int> goal(to.X, to.Y);
    int distance = here.Distance_To(goal);
    if (distance < 20) {
        unknown_68 = 0;
        unknown_70 = 0;
        position.X = unknown_3c.X;
        position.Y = unknown_3c.Y;
        bool down = LinkedTo->unknown_74;
        LinkedTo->unknown_74 = false;
        LinkedTo->Set_Coord(position);
        LinkedTo->unknown_74 = down;
        if (LinkedTo->unknown_228 == 0 && !balloon) {
            unknown_78 = 0;
            unknown_4c = 4;
            LinkedTo->unknown_6d1470();
        } else {
            if (unknown_tracker_839ea0.Unknown_004132E0(LinkedTo)) {
                int facing = Random_Pick(0, 7);
                CellClass &here_cell = Map[cell];
                LinkedTo->Assign_Destination(&here_cell.Adjacent_Cell(facing), true);
            } else {
                ((CellClass *)LinkedTo->Get_Cell_Ptr())->Set_unknown_e0(LinkedTo);
                unknown_4c = 2;
            }
        }
    } else if (distance < Speed) {
        unknown_70 = Speed / 8;
        if (LinkedTo->unknown_228 == 0) unknown_78 = Height / 2;
    } else if (distance < Speed * 2) {
        unknown_70 = Speed / 4;
        if (LinkedTo->unknown_228 == 0) unknown_78 = Height / 2;
        if (Dir_To_256(difference) > TurnRate) {
            unknown_70 = Speed / 10;
            if (unknown_70 < 1.0) unknown_70 = 1.0;
        }
    } else {
        if (distance < (Speed * 50) / TurnRate) {
            unknown_70 = Speed / 2;
            if (LinkedTo->unknown_228 == 0) unknown_78 = (int)(Height * 0.75);
            if (Dir_To_256(difference) > TurnRate * 5) {
                unknown_70 = Speed / 5;
                if (unknown_70 < 1.0) unknown_70 = 1.0;
            }
        } else {
            unknown_78 = Height;
            unknown_70 = Speed;
        }
    }
    CellClass &destination = Map[unknown_3c];
    if (balloon || destination.unknown_ec == 2 || destination.unknown_ec == 6) unknown_78 = Height;
}


// void JumpjetLocomotionClass::unknown_530550() (0x00530550)
// Process state 4 (descending). Over a cell of land type 2 or 6 (outside
// mission 7) the jumpjet stays up: a crowded cell (0x004132E0) sends it to a
// random adjacent cell (state 3), else it claims its cell (state 2, full
// height). Otherwise an unlandable target (slot 101, spot test 0x00476460;
// a unit on spot 0 always lands) stops the move; the target is claimed
// (+0x88, slot 58). The target height drops to 0; on the ground the object
// is placed at the target (on a bridge above ground: +0x79), the move ends,
// the object is redrawn and its cell claim released (state 0).

void JumpjetLocomotionClass::unknown_530550()
{
    CellClass &cell = Map[unknown_3c];
    if ((cell.unknown_ec == 2 || cell.unknown_ec == 6) && LinkedTo->MissionQueue != 7 && LinkedTo->Get_Mission() != 7) {
        if (unknown_tracker_839ea0.Unknown_004132E0(LinkedTo)) {
            int facing = Random_Pick(0, 7);
            CellClass *here = (CellClass *)LinkedTo->Get_Cell_Ptr();
            LinkedTo->Assign_Destination(&here->Adjacent_Cell(facing), true);
            unknown_4c = 3;
        } else {
            ((CellClass *)LinkedTo->Get_Cell_Ptr())->Set_unknown_e0(LinkedTo);
            unknown_4c = 2;
            unknown_78 = Height;
        }
        return;
    }
    int result = LinkedTo->Can_Enter_Cell(&cell, -1, -1, 0, 1);
    int spot = Unknown_004763D0(unknown_3c);
    bool landable;
    if (cell.Is_Spot_Free(spot, (cell.unknown_140 >> 8) & 1) || unknown_88) {
        landable = true;
    } else {
        landable = false;
    }
    if (LinkedTo->Kind_Of() == 1 && spot == 0) {
        landable = true;
    }
    if (LinkedTo->MissionQueue != 7 && LinkedTo->Get_Mission() != 7) {
        if (!landable || (!(Map[unknown_3c].unknown_140 & 0x100) && (result > 2 || (result == 2 && !unknown_88)))) {
            Stop_Moving();
            return;
        }
        if (!unknown_88) {
            unknown_88 = true;
            LinkedTo->Set_Occupy_Bit(unknown_3c);
        }
    }
    unknown_78 = 0;
    int height = LinkedTo->Get_Height_AGL();
    if (!LinkedTo->unknown_79 && (Map[Object_Coord(LinkedTo)].unknown_140 & 0x100) && height >= unknown_geometry_j) {
        height -= unknown_geometry_j;
    }
    if (height == 0) {
        LinkedTo->unknown_slot306(0);
        LinkedTo->Mark(0);
        LinkedTo->Set_Coord(unknown_3c);
        if (Object_Coord(LinkedTo).Z > Map.Get_Height_GL(Object_Coord(LinkedTo)) && (Map[Object_Coord(LinkedTo)].unknown_140 & 0x100)) {
            LinkedTo->unknown_79 = true;
        }
        LinkedTo->Mark(1);
        unknown_3c = unknown_geometry_l;
        unknown_48 = false;
        LinkedTo->unknown_slot93(2);
        LinkedTo->Assign_Destination(0, true);
        if (LinkedTo->unknown_7d && !LinkedTo->unknown_76 && !LinkedTo->unknown_7a) {
            LinkedTo->unknown_slot263(0, 0, 0, 0);
            if (LinkedTo->unknown_7d) {
                LinkedTo->unknown_slot262(0, 0, 0, 0);
            }
            if (LinkedTo->unknown_7d) {
                Map.Unknown_0054A550(Object_Coord(LinkedTo), LinkedTo->unknown_1f4 - 3, LinkedTo->unknown_1f4 + 3, 0);
            }
            if (((CellClass *)LinkedTo->Get_Cell_Ptr())->unknown_e0 == LinkedTo) {
                ((CellClass *)LinkedTo->Get_Cell_Ptr())->Set_unknown_e0(0);
            }
            unknown_tracker_839ea0.Unknown_00413310(LinkedTo);
            unknown_4c = 0;
            unknown_88 = false;
        }
    }
}


// void JumpjetLocomotionClass::unknown_530940() (0x00530940)
// Process state 5 (crashing): the object sinks by JumpjetCrash and turns
// (+0x2D00 a step); on the map it is moved to the new coordinate. Once on
// the ground (slot 108 at most 0) its cell claim is released, the arrival
// tracker is told, the speed (+0x70) cleared, the state set to 6 and the
// object notified (0x117C).

void JumpjetLocomotionClass::unknown_530940()
{
    CoordStruct coord = LinkedTo->Coord;
    coord.Z -= Crash;
    {
        UnknownFacingDir dir = UnknownFacingDir((unsigned short)(unknown_50.Current().Raw + 0x2D00));
        unknown_50.Set_Desired(dir);
    }
    Map.Get_Height_GL(coord);
    Map[coord];
    if (Map.In_Radar(Coord_Cell(coord))) {
        Map.Remove(LinkedTo);
        LinkedTo->Mark(0);
        LinkedTo->Set_Coord(coord);
        LinkedTo->Mark(1);
        Map.Submit(LinkedTo);
    }
    if (LinkedTo->Get_Height_AGL() <= 0) {
        if (((CellClass *)LinkedTo->Get_Cell_Ptr())->unknown_e0 == LinkedTo) {
            ((CellClass *)LinkedTo->Get_Cell_Ptr())->Set_unknown_e0(0);
        }
        unknown_tracker_839ea0.Unknown_00413310(LinkedTo);
        unknown_4c = 6;
        unknown_70 = 0;
        LinkedTo->INoticeSink_Unknown(0x117C, 0);
    }
}


// void JumpjetLocomotionClass::unknown_530ad0() (0x00530AD0)
// Process state 6: the routine has no code. Process expands it (its jump
// table keeps the case 6 entry at the switch exit); this copy is unreferenced.

void JumpjetLocomotionClass::unknown_530ad0()
{
}


// bool __stdcall JumpjetLocomotionClass::Is_Moving_Now() (0x00530AE0, ILocomotion 32)
// Moving in every state but 0 and 2 (+0x4C).

bool __stdcall JumpjetLocomotionClass::Is_Moving_Now()
{
    if (unknown_4c != 0 && unknown_4c != 2) {
        return true;
    }
    return false;
}


extern double const unknown_jumpjet_full_turn_79c9b8;
extern double const unknown_jumpjet_cycle_7a5398;
extern double const unknown_move_angle_scale_79b620;
inline CoordStruct Jumpjet_Move_Coord_00530b00(CoordStruct const &start, UnknownFacingDir facing, int distance)
{
    double angle=((short)facing.Raw-0x3fff)*unknown_move_angle_scale_79b620;
    return CoordStruct(
        (int)(double(start.X)+Fast_Cos(angle)*double(distance)),
        (int)(double(start.Y)-Fast_Sin(angle)*double(distance)),start.Z);
}
void JumpjetLocomotionClass::unknown_530b00()
{
    bool need_mark=unknown_4c!=2 && unknown_4c!=3;
    bool was_down=LinkedTo->unknown_74;
    if (need_mark) LinkedTo->Mark(0);
    else LinkedTo->unknown_74=false;
    if (unknown_70>unknown_68) {
        unknown_68+=Accel;
        unknown_68=unknown_68<Speed?unknown_68:Speed;
    }
    if (unknown_70<unknown_68) {
        unknown_68-=Accel*1.5;
        unknown_68=unknown_68>0?unknown_68:0;
    }
    LinkedTo->unknown_slot306(unknown_68/Speed);
    bool at_destination=LinkedTo->unknown_slot104()==Coord_Cell(unknown_3c);
    if ((unknown_4c==2 || unknown_4c==3) && !NoWobbles)
        unknown_80+=unknown_jumpjet_full_turn_79c9b8/(unknown_jumpjet_cycle_7a5398/Wobbles);
    else unknown_80=0;
    int desired_height=(int)(Fast_Sin(unknown_80)*Deviation+unknown_78);
    int height=LinkedTo->Get_Height();
    int ground=Map.Get_Height_GL(Object_Coord(LinkedTo));
    if ((Map[Object_Coord(LinkedTo)].unknown_140&0x100) && Object_Coord(LinkedTo).Z>=ground+unknown_geometry_g*4)
        ground+=unknown_geometry_j;
    int difference;
    if (unknown_4c!=4 && unknown_4c!=0 && (!at_destination || LinkedTo->Techno_Type_Class()->BalloonHover))
        difference=height-unknown_531140();
    else difference=height-ground;
    bool moved=false;
    if (difference<desired_height) {
        int height_agl=LinkedTo->Get_Height_AGL();
        if ((Map[Object_Coord(LinkedTo)].unknown_140&0x100) && !LinkedTo->unknown_79) {
            if (Object_Coord(LinkedTo).Z>=Map.Get_Height_GL(Object_Coord(LinkedTo))+unknown_geometry_j)
                height_agl-=unknown_geometry_j;
        }
        if (height_agl==0) {
            LinkedTo->Clear_Occupy_Bit(Object_Coord(LinkedTo));
            LinkedTo->unknown_79=false;
        }
        if (difference+Climb>desired_height) height+=desired_height-difference;
        else height=(int)(height+Climb);
        moved=true;
    } else if (difference>desired_height) {
        if (difference-Climb<desired_height) height+=desired_height-difference;
        else height=(int)(height-Climb);
        if (height<=ground) height=ground;
        difference=difference>0?difference:0;
        moved=true;
    }
    if (LinkedTo->unknown_slot104()!=Coord_Cell(unknown_3c)) {
        if (difference<desired_height/2) unknown_68=0;
        if (difference<desired_height/4) unknown_68=0;
    }
    if (moved) {
        LinkedTo->unknown_func_5d4920(height);
        if (need_mark) LinkedTo->Mark(0);
    }
    CoordStruct start=Object_Coord(LinkedTo);
    int distance=(int)unknown_68;
    CoordStruct next=Jumpjet_Move_Coord_00530b00(start,unknown_50.Current(),distance);
    LinkedTo->Set_Coord(next);
    BuildingClass *building=static_cast<BuildingClass *>(Map[next].Cell_Building());
    if (building && building->Type->FirestormWall && building->House->unknown_142)
        building->unknown_450cb0(LinkedTo,true);
    LinkedTo->unknown_2e0.Set(unknown_50.Current());
    if (need_mark) LinkedTo->Mark(1);
    else LinkedTo->unknown_74=was_down;
}


// CoordStruct JumpjetLocomotionClass::unknown_5310a0(CoordStruct const &coord) (0x005310A0)
// The free spot near a coordinate (0x0049CB70); unless none, at ground
// height plus the bridge height on a bridge cell. Move_To expands it; this
// out-of-line copy has no callers (entry bounded by the preceding padding).

CoordStruct JumpjetLocomotionClass::unknown_5310a0(CoordStruct const &coord)
{
    CoordStruct result;
    result = Map.Unknown_0049CB70(coord, false);
    if (result != unknown_geometry_l) {
        result.Z = Map.Get_Height_GL(result);
        if (Map[result].unknown_140 & 0x100) {
            result.Z += unknown_geometry_j;
        }
    }
    return result;
}


// int JumpjetLocomotionClass::unknown_531140() (0x00531140)
// The ground height to hold: the cell top under the object (plus the bridge
// height on a bridge cell); with speed (+0x68 above zero) also the cell one
// step ahead in the current facing (+0x50): the higher of the two when the
// ahead cell is higher, else their mean.

int JumpjetLocomotionClass::unknown_531140()
{
    CoordStruct coord = LinkedTo->Coord;
    int height = Map[coord].Occupier_Height();
    if (Map[coord].unknown_140 & 0x100) {
        height += unknown_geometry_j;
    }
    if (unknown_68 > 0.0) {
        TPoint2D<int> const &offset = unknown_facing_offsets_851ac0[Dir_To_8(unknown_50.Current())];
        coord = CoordStruct(coord.X + offset.X, coord.Y + offset.Y, coord.Z);
        int ahead = Map[coord].Occupier_Height();
        if (Map[coord].unknown_140 & 0x100) {
            height += unknown_geometry_j;
        }
        if (ahead > height) {
            return ahead;
        }
        return (height + ahead) / 2;
    }
    return height;
}


// void __stdcall JumpjetLocomotionClass::Mark_All_Occupation_Bits(int mark) (0x00531250, ILocomotion 39)
// When clearing (mark 0) with a head-to coordinate, while idle (+0x4C 0) or
// still marked (+0x88), unmarks the head-to cell (slot 59) and clears +0x88.
// The head-to copy is an UnknownCoord434820 (its forwarding constructor makes
// VC6 copy all three words before the comparison, as the original does).

void __stdcall JumpjetLocomotionClass::Mark_All_Occupation_Bits(int mark)
{
    if (mark == 0) {
        UnknownCoord434820 head = Head_To_Coord();
        if (head != unknown_geometry_l && (unknown_4c == 0 || unknown_88)) {
            LinkedTo->Clear_Occupy_Bit(head);
            unknown_88 = false;
        }
    }
}


// CoordStruct __stdcall JumpjetLocomotionClass::Head_To_Coord() (0x005312D0, ILocomotion 6)
// The object's own coordinate while idle (+0x4C 0), otherwise the target
// coordinate (+0x3C).

CoordStruct __stdcall JumpjetLocomotionClass::Head_To_Coord()
{
    if (unknown_4c == 0) {
        CoordStruct coord = LinkedTo->Coord;
        return coord;
    }
    return CoordStruct(unknown_3c.X, unknown_3c.Y, unknown_3c.Z);
}
