// teleport.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/teleport.json carries the addresses and the rows that
// stay in their own files.


#include "shared/teleport_locomotion.hpp"
#include "shared/cell.hpp"
#include "shared/rules.hpp"
#include "shared/techno_type.hpp"
#include "shared/overlay_view.hpp"
#include "shared/bullet.hpp"
#include "shared/anim.hpp"
#include "shared/sound_play.hpp"
#include "shared/coord_distance.hpp"
#include "shared/parasite.hpp"
#include "shared/com_ptr.hpp"
#include "shared/foot.hpp"


// TeleportLocomotionClass::TeleportLocomotionClass() (0x006DFCA0)

TeleportLocomotionClass::TeleportLocomotionClass() :
    unknown_1c(unknown_geometry_l),
    unknown_28(unknown_geometry_l),
    unknown_34(false),
    unknown_35(false),
    unknown_36(false),
    unknown_38(0)
{
}


// bool __stdcall TeleportLocomotionClass::Is_Moving() (0x006DFD20, ILocomotion 4)

bool __stdcall TeleportLocomotionClass::Is_Moving()
{
    return unknown_34 == 1 ? true : false;
}


// bool TeleportLocomotionClass::unknown_slot11() (0x006DFD30, IPersistStream-side slot 11)
// Not moving.

bool TeleportLocomotionClass::unknown_slot11()
{
    return Is_Moving() ? false : true;
}


// CoordStruct __stdcall TeleportLocomotionClass::Destination() (0x006DFD40, ILocomotion 5)
// The destination while moving, otherwise where the linked object is.

CoordStruct __stdcall TeleportLocomotionClass::Destination()
{
    if (Is_Moving()) {
        return CoordStruct(unknown_1c.X, unknown_1c.Y, unknown_1c.Z);
    }
    CoordStruct coord = LinkedTo->Coord;
    return coord;
}


// void __stdcall TeleportLocomotionClass::Move_To(CoordStruct to) (0x006DFDA0, ILocomotion 17)
// Unless the linked object is busy (slots 201, 202, 111, 112: then its
// +0x494 is cleared), an infantry clears the target cell first; the step
// 0x006E06B0 picks the arrival coordinate, which starts the move, or the
// object is told to stop (slot 260).

void __stdcall TeleportLocomotionClass::Move_To(CoordStruct to)
{
    if (LinkedTo->unknown_slot201() || LinkedTo->unknown_slot202() || LinkedTo->unknown_slot111() || LinkedTo->unknown_slot112()) {
        LinkedTo->unknown_494 = 0;
        return;
    }
    CoordStruct coord = CoordStruct(to.X, to.Y, to.Z);
    if (LinkedTo->unknown_190 && LinkedTo->Kind_Of() == 15) {
        CellClass &cell = Map[coord];
        cell.Incoming(&unknown_geometry_l, true, true, false);
    }
    Pick_Arrival(coord);
    if (unknown_28 != unknown_geometry_l) {
        unknown_34 = true;
        unknown_1c = unknown_28;
    } else {
        LinkedTo->Assign_Destination(0, true);
    }
}


// void __stdcall TeleportLocomotionClass::Stop_Moving() (0x006DFED0, ILocomotion 18)

void __stdcall TeleportLocomotionClass::Stop_Moving()
{
    unknown_1c = unknown_geometry_l;
    unknown_34 = false;
    unknown_36 = false;
}


// bool TeleportLocomotionClass::unknown_6dff00(CoordStruct coord, bool arrive) (0x006DFF00)
// One warp step (Process). Before arriving (arrive false) the occupants of
// coord's cell (the bridge layer when the cell is a bridge) are dealt with:
// a matching infantry at the very coordinate, a FootClass occupant and,
// for an occupant slot 83 marks, the linked object itself are destroyed
// (C4Warhead); any other occupant or an unfit bridge blocks the cell. A
// blocked cell, or one whose overlay is not passable (+0x279), picks the
// nearest free cell (0x005502E0) and keeps the offset within it in the
// linked object's +0x21C; that returns false. Arriving lifts the object
// from its old place (slot 59), moves the arrival point to +0x21C on the
// ground or bridge, and places it there (slot 58). Otherwise the object
// is placed at the arrival point (or where it stands) and true returned.
// The overlap test passes CellClass::Is_Bridge_Here without calling it:
// the original reads the function's address as the bool (always true).

// The object as a FootClass, or null (the cast idiom of BuildingTypeClass 0x00458B10).
inline FootClass *Foot_Of_6dff00(ObjectClass *object)
{
    bool foot;
    if (object) foot = object->AbstractFlags.IsFoot;
    if (!object || !foot) return 0;
    return static_cast<FootClass *>(object);
}

bool TeleportLocomotionClass::unknown_6dff00(CoordStruct coord, bool arrive)
{
    if (!arrive) {
        ObjectClass *object;
        if (Map[coord].unknown_140 & 0x100) {
            object = Map[coord].AltOccupierPtr;
        } else {
            object = Map[coord].OccupierPtr;
        }
        int overlay = Map[coord].OverlayIndex;
        bool blocked = false;
        for (; object != 0; object = object->unknown_30) {
            if (!object->unknown_slot83() && object->Kind_Of() == 15 && LinkedTo->Kind_Of() == 15) {
                if (coord == object->Center_Coord()) {
                    int damage = object->Techno_Type_Class()->MaxStrength;
                    object->Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, 0);
                }
            } else if (!object->unknown_slot83() && Foot_Of_6dff00(object) != 0) {
                int damage = object->Techno_Type_Class()->MaxStrength;
                object->Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, 0);
            } else if (object->unknown_slot83()) {
                int damage = LinkedTo->Techno_Type_Class()->MaxStrength;
                LinkedTo->Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, 0);
            } else if (Foot_Of_6dff00(object) == 0) {
                blocked = true;
            }
        }
        if ((Map[coord].unknown_140 & 0x100) && !(Map[coord].unknown_140 & 0x200)) {
            blocked = true;
        }
        CellClass *cell = &Map[Coord_Cell(LinkedTo->Center_Coord())];
        bool alt = cell->Is_Bridge_Here;
        if ((overlay != -1 && !unknown_overlay_types_a35de8[overlay]->unknown_279 && !alt) || blocked) {
            int zone = LinkedTo->Techno_Type_Class()->MovementZone;
            if (zone == 9) {
                zone = 0;
            } else if (zone == 2) {
                zone = 0;
            } else if (zone == 3) {
                zone = 5;
            }
            Cell found = Map.Nearby_Location(Coord_Cell(coord), 1, Map.Zone(cell->MapCoords, zone, alt), zone,
                Map[Coord_Cell(coord)].IsUnderBridge, Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
            CoordStruct offset = coord - Map[coord].Center_Coord();
            CoordStruct target = Map[found].Center_Coord() + offset;
            LinkedTo->unknown_21c = target;
            return false;
        }
    } else {
        if (unknown_28 != unknown_geometry_l) {
            LinkedTo->Clear_Occupy_Bit(unknown_28);
        } else {
            LinkedTo->Clear_Occupy_Bit(Object_Coord(LinkedTo));
        }
        unknown_28 = LinkedTo->unknown_21c;
        unknown_28.Z = Map.Get_Height_GL(unknown_28);
        if ((Map[unknown_28].unknown_140 & 0x100) && !LinkedTo->unknown_79) {
            LinkedTo->unknown_79 = true;
            unknown_28.Z += unknown_geometry_j;
        } else {
            LinkedTo->unknown_79 = false;
        }
        LinkedTo->Set_Occupy_Bit(unknown_28);
    }
    if (unknown_28 != unknown_geometry_l) {
        LinkedTo->Set_Occupy_Bit(unknown_28);
    } else {
        LinkedTo->Set_Occupy_Bit(Object_Coord(LinkedTo));
    }
    return true;
}


// void TeleportLocomotionClass::unknown_6e0430(CoordStruct coord) (0x006E0430)
// The warp arrival at coord (Process, state 5): each occupant of the cell
// that slot 83 marks crushes the linked object (C4Warhead, its full
// strength); a land unit on water that is not on a bridge sinks (+0x325,
// slot 210), and a naval object off water, or one the cell refuses
// (slot 101 answers 7), is destroyed unless the cell (0x0047B7E0) lets an
// infantry-less object sink there.

void TeleportLocomotionClass::unknown_6e0430(CoordStruct coord)
{
    for (ObjectClass *object = Map[coord].OccupierPtr; object != 0; object = object->unknown_30) {
        if (object->unknown_slot83()) {
            int damage = LinkedTo->Techno_Type_Class()->MaxStrength;
            LinkedTo->Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, 0);
        }
    }
    bool kill = false;
    if (LinkedTo->Techno_Type_Class()->Naval && ((Map[coord].unknown_140 & 0x100) || Map[coord].unknown_ec == 1)) {
        kill = true;
    }
    if (Map[Coord_Cell(coord)].unknown_ec == 2 && !LinkedTo->Techno_Type_Class()->Naval && LinkedTo->Kind_Of() != 15
        && !(Map[coord].unknown_140 & 0x100) && Map[coord].unknown_ec != 1) {
        LinkedTo->unknown_325 = true;
        LinkedTo->unknown_slot210();
        return;
    }
    if (LinkedTo->Can_Enter_Cell(&Map[Coord_Cell(coord)], -1, -1, 0, true) == 7 || kill) {
        if (!Map[Coord_Cell(coord)].unknown_47b7e0() || LinkedTo->Kind_Of() == 15) {
            int damage = LinkedTo->Techno_Type_Class()->MaxStrength;
            LinkedTo->Take_Damage(&damage, 0, Rules->unknown_c74, 0, true, 0);
        } else {
            LinkedTo->unknown_325 = true;
            LinkedTo->unknown_slot210();
        }
    }
}


// bool TeleportLocomotionClass::Pick_Arrival(CoordStruct const &to) (0x006E06B0)
// Move_To's step: picks the arrival coordinate (+0x28) for a warp to `to`.
// The object is lifted from its old place (slot 59). Toward a bridge cell
// it counts as on the bridge when it stands well above the ground. An
// infantry takes the free sub-cell spot nearest `to` (0x004764B0),
// preferring the occupied spot when its target (+0x494, a unit, aircraft
// or the building there) is at `to` while on missions 8, 9, 7 or 25; a
// refused cell (slot 101) leaves it searching the cells near its target
// (0x005502E0). Other objects take the centre of `to`'s cell or of the
// nearest free one, unless +0x210 holds. The object is placed (slot 58)
// at the arrival point, or back where it stands when there is none
// (false).

bool TeleportLocomotionClass::Pick_Arrival(CoordStruct const &to)
{
    if (unknown_28 != unknown_geometry_l) {
        LinkedTo->Clear_Occupy_Bit(unknown_28);
    } else {
        LinkedTo->Clear_Occupy_Bit(Object_Coord(LinkedTo));
    }
    CoordStruct coord = to;
    if (to != unknown_geometry_l) {
        bool bridge = (Map[coord].unknown_140 & 0x100) && Object_Coord(LinkedTo).Z > Map.Get_Height_GL(coord) + unknown_geometry_g * 3;
        if (LinkedTo->Kind_Of() == 15) {
            bool target_here = false;
            if (LinkedTo->Get_Mission() == 8 || LinkedTo->Get_Mission() == 9 || LinkedTo->Get_Mission() == 7 || LinkedTo->Get_Mission() == 25) {
                {
                    ObjectClass *target = (ObjectClass *)LinkedTo->unknown_494;
                    if (target != 0 && target->Kind_Of() == 1 && target->unknown_slot104() == Coord_Cell(to)) {
                        target_here = true;
                    }
                }
                {
                    ObjectClass *target = (ObjectClass *)LinkedTo->unknown_494;
                    if (target != 0 && target->Kind_Of() == 2 && target->unknown_slot104() == Coord_Cell(to)) {
                        target_here = true;
                    }
                }
                {
                    ObjectClass *target = (ObjectClass *)LinkedTo->unknown_494;
                    if (target != 0 && target->Kind_Of() == 6 && Map[to].Cell_Building() == target) {
                        target_here = true;
                    }
                }
            }
            CoordStruct old = unknown_28;
            unknown_28 = Map[coord].Closest_Free_Spot(coord, target_here, bridge);
            if (LinkedTo->Can_Enter_Cell(&Map[Coord_Cell(unknown_28)], -1, -1, 0, true) != 0) {
                unknown_28 = unknown_geometry_l;
            }
            Map[old].Center_Coord();    // (the result is unused)
            if (unknown_28 == unknown_geometry_l) {
                AbstractClass *target = LinkedTo->unknown_494;
                if (target != 0 && target->AbstractFlags.IsObject) {
                    int zone = LinkedTo->Techno_Type_Class()->MovementZone;
                    Cell target_cell = Coord_Cell(target->unknown_slot19(0));
                    CellClass *cell = &Map[LinkedTo->unknown_slot19(0)];
                    bool alt = LinkedTo->Is_Moving_Onto_Bridge(0);
                    Cell found = Map.Nearby_Location(target_cell, LinkedTo->Techno_Type_Class()->SpeedType,
                        Map.Zone(cell->MapCoords, zone, alt), zone, Map[target_cell].IsUnderBridge,
                        Point2D(1, 1), 0, 1, 0, 1, Cell(0, 0), 0);
                    if (found != unknown_geometry_k) {
                        unknown_28 = Map[found].Closest_Free_Spot(Map[found].Center_Coord(), target_here, bridge);
                    }
                }
            }
        } else if (LinkedTo->unknown_210) {
            LinkedTo->Set_Occupy_Bit(unknown_28);
        } else {
            int zone = LinkedTo->Techno_Type_Class()->MovementZone;
            Cell cell = Coord_Cell(to);
            if (cell != unknown_geometry_k && LinkedTo->Can_Enter_Cell(&Map[cell], -1, -1, 0, true) != 0) {
                int index = Map.Zone(cell, zone, bridge);
                cell = Map.Nearby_Location(cell, LinkedTo->Techno_Type_Class()->SpeedType, index, zone, bridge,
                    Point2D(1, 1), 0, 0, 0, 1, Cell(0, 0), 0);
            }
            if (cell != unknown_geometry_k) {
                LinkedTo->Clear_Occupy_Bit(unknown_28);
                Cell center = Coord_Cell(Map[cell].Center_Coord());
                unknown_28 = CoordStruct(center.X * 256 + 128, center.Y * 256 + 128, 0);
                unknown_28.Z = Map.Get_Height_GL(unknown_28);
                LinkedTo->Set_Occupy_Bit(unknown_28);
            }
        }
    } else {
        unknown_28 = to;
    }
    if (unknown_28 != unknown_geometry_l) {
        LinkedTo->Set_Occupy_Bit(unknown_28);
        return true;
    }
    LinkedTo->Set_Occupy_Bit(Object_Coord(LinkedTo));
    return false;
}


// void __stdcall TeleportLocomotionClass::Do_Turn(UnknownFacingDir dir) (0x006E0DE0, ILocomotion 19)

void __stdcall TeleportLocomotionClass::Do_Turn(UnknownFacingDir dir)
{
    LinkedTo->unknown_2e0.Set_Desired(UnknownFacingDir(dir));
}


// bool __stdcall TeleportLocomotionClass::Process() (0x006E0E10, ILocomotion 10)
// The chrono warp, staged by +0x38. A pending warp (+0x205) waits on the
// timer (slot 10); a warp the object queued (+0x214) becomes the stage.
// Stage 0 while moving to a fresh destination: targets on the object are
// dropped (0x006D7C80, bullets aimed at it), WarpOut plays, the delay
// timer is set from the distance (ChronoTrigger / ChronoDistanceFactor,
// at least ChronoMinimumDelay; harvesters jump at once), the object is
// taken off the map, moved to +0x28 (on a bridge there or not) and put
// back, with the chrono out and in sounds. An object flagged +0x210 runs
// stages 1-7 instead: warp out (2), clear the arrival cell (3: 0x006DFF00,
// ChronoDelay into +0x218), arrive (4), crush the arrival cell's occupants
// (5: 0x006E0430) and finish (7).

bool __stdcall TeleportLocomotionClass::Process()
{
    int distance = 0;
    if (LinkedTo->unknown_205 && unknown_38 == 0 && LinkedTo->unknown_214 == 0) {
        unknown_slot10();
        return false;
    }
    if (unknown_38 == 0 && LinkedTo->unknown_214 != 0) {
        unknown_38 = LinkedTo->unknown_214;
        return false;
    }
    if (!LinkedTo->unknown_210) {
        if (unknown_38 <= 0) {
            if (Is_Moving()) {
                if (Object_Coord(LinkedTo) != unknown_28 && unknown_28 != unknown_geometry_l) {
                    Unknown_006D7C80(LinkedTo);
                    for (int index = unknown_BulletClass_list_00a40ce8.Count() - 1; index >= 0; index--) {
                        BulletClass *bullet = unknown_BulletClass_list_00a40ce8[index];
                        if (bullet->Target == LinkedTo) {
                            bullet->unknown_func_461470();
                        }
                    }
                    new AnimClass(Rules->WarpOut, Object_Coord(LinkedTo), 0, 1, 0x600, 0);
                    distance = Coord_Distance(LinkedTo->Center_Coord(), unknown_28);
                    unknown_3c = 0;
                    if (Rules->ChronoTrigger) {
                        unknown_3c = distance / Rules->ChronoDistanceFactor;
                    }
                    unknown_3c = unknown_3c > Rules->ChronoMinimumDelay ? unknown_3c : CDTimerClass<FrameTimerClass>(Rules->ChronoMinimumDelay);
                    if (distance < Rules->ChronoRangeMinimum) {
                        unknown_3c = Rules->ChronoMinimumDelay;
                    }
                    LinkedTo->unknown_205 = true;
                    if (LinkedTo->Kind_Of() == 1 && ((UnitClass *)LinkedTo)->Type->Harvester) {
                        unknown_3c = 0;
                        LinkedTo->unknown_205 = false;
                    }
                    if (LinkedTo->unknown_580 != 0) {
                        LinkedTo->unknown_580->unknown_588->Unknown_00607440();
                    }
                    LinkedTo->Mark(0);
                    {
                        FootClass *linked = LinkedTo;
                        int sound = linked->Techno_Type_Class()->ChronoOutSound;
                        if (sound != -1 || (sound = Rules->ChronoOutSound) != -1) {
                            Voc_Play_At(sound, Object_Coord(linked), 0);
                        }
                    }
                    LinkedTo->Set_Coord(unknown_28);
                    Cell where = Coord_Cell(unknown_28);
                    CellClass *cell = &Map[where];
                    LinkedTo->Set_Coord(unknown_28);
                    if (cell->unknown_140 & 0x100) {
                        LinkedTo->unknown_79 = true;
                    } else {
                        LinkedTo->unknown_79 = false;
                    }
                    LinkedTo->Set_Height_AGL(0);
                    LinkedTo->Mark(1);
                    {
                        FootClass *linked = LinkedTo;
                        int sound = linked->Techno_Type_Class()->ChronoInSound;
                        if (sound != -1 || (sound = Rules->ChronoInSound) != -1) {
                            Voc_Play_At(sound, Object_Coord(linked), 0);
                        }
                    }
                    LinkedTo->unknown_slot93(2);
                    Stop_Moving();
                    cell->Goodie_Check(LinkedTo);
                    LinkedTo->Assign_Destination(0, true);
                    new AnimClass(Rules->WarpOut, Object_Coord(LinkedTo), 0, 1, 0x600, 0);
                    LinkedTo->unknown_214 = 0;
                    return false;
                }
                LinkedTo->Assign_Destination(0, true);
                Stop_Moving();
                return false;
            }
            return false;
        }
    } else if (unknown_38 == 0) {
        LinkedTo->unknown_204 = true;
        unknown_3c = 60;
        unknown_38++;
        return false;
    }
    if (unknown_38 == 1) {
        unknown_slot10();
        return false;
    }
    if (unknown_38 == 2) {
        new AnimClass(Rules->WarpOut, Object_Coord(LinkedTo), 0, 1, 0x600, 0);
        LinkedTo->Mark(0);
        {
            FootClass *linked = LinkedTo;
            int sound = linked->Techno_Type_Class()->ChronoOutSound;
            if (sound != -1 || (sound = Rules->ChronoOutSound) != -1) {
                Voc_Play_At(sound, Object_Coord(linked), 0);
            }
        }
        LinkedTo->unknown_205 = true;
        LinkedTo->unknown_210 = false;
        LinkedTo->unknown_204 = false;
        LinkedTo->unknown_79 = false;
        bool ok = unknown_6dff00(LinkedTo->unknown_21c, false);
        unknown_38++;
        if (ok) {
            unknown_38++;
            return false;
        }
        return false;
    }
    if (unknown_38 == 3) {
        if (unknown_6dff00(LinkedTo->unknown_21c, false)) {
            unknown_38++;
        }
        LinkedTo->unknown_218 = Rules->ChronoDelay;
        return false;
    }
    if (unknown_38 == 4) {
        unknown_6dff00(LinkedTo->unknown_21c, true);
        LinkedTo->Set_Coord(unknown_28);
        LinkedTo->Set_Height_AGL(0);
        LinkedTo->Mark(1);
        unknown_38++;
        return false;
    }
    if (unknown_38 == 5) {
        LinkedTo->Set_Coord(unknown_28);
        LinkedTo->Set_Height_AGL(0);
        LinkedTo->Mark(1);
        {
            FootClass *linked = LinkedTo;
            int sound = linked->Techno_Type_Class()->ChronoInSound;
            if (sound != -1 || (sound = Rules->ChronoInSound) != -1) {
                Voc_Play_At(sound, Object_Coord(linked), 0);
            }
        }
        if (!Map.Unknown_0055A660(LinkedTo->unknown_slot104(), true)) {
            LinkedTo->unknown_32d = false;
        }
        if (LinkedTo->unknown_214 == 0) {
            unknown_6e0430(unknown_28);
        }
        if (LinkedTo->unknown_7d) {
            LinkedTo->unknown_slot93(2);
            Stop_Moving();
            LinkedTo->unknown_6d6e50(0);
            LinkedTo->Assign_Destination(0, true);
            unknown_3c = LinkedTo->unknown_218;
            new AnimClass(Rules->WarpOut, Object_Coord(LinkedTo), 0, 1, 0x600, 0);
            unknown_38++;
            return false;
        }
        return false;
    }
    if (unknown_38 == 6) {
        unknown_slot10();
        return false;
    }
    if (unknown_38 == 7) {
        LinkedTo->unknown_205 = false;
        LinkedTo->unknown_6d6e50(0);
        LinkedTo->Assign_Destination(0, true);
        unknown_34 = false;
        LinkedTo->unknown_214 = 0;
        unknown_38 = 0;
    }
    return false;
}


// void __stdcall TeleportLocomotionClass::unknown_slot10() (0x006E16F0, IPersistStream-side slot 10)
// When the warp timer has run out the linked object's +0x205 is cleared
// and a running count at +0x38 advances.

void __stdcall TeleportLocomotionClass::unknown_slot10()
{
    if (unknown_3c.Value() == 0) {
        LinkedTo->unknown_205 = false;
        if (unknown_38 > 0) {
            unknown_38++;
        }
    }
}


// HRESULT __stdcall TeleportLocomotionClass::GetClassID(GUID *clsid) (0x006E1730)

HRESULT __stdcall TeleportLocomotionClass::GetClassID(GUID *clsid)
{
    if (clsid == 0) {
        return E_POINTER;
    }
    *clsid = CLSID_TeleportLocomotionClass;
    return S_OK;
}


// HRESULT __stdcall TeleportLocomotionClass::Load(IStream *stream) (0x006E1770)
// LocomotionClass::Load, rebuilt in place; then a flag and, when set, the
// carried locomotor (OleLoadFromStream).

HRESULT __stdcall TeleportLocomotionClass::Load(IStream *stream)
{
    HRESULT result = LocomotionClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) TeleportLocomotionClass(NoInitClass());
        bool piggybacking;
        stream->Read(&piggybacking, sizeof(piggybacking), 0);
        if (piggybacking) {
            result = OleLoadFromStream(stream, IID_ILocomotion, (void **)&Piggybacker);
        }
    }
    return result;
}


// HRESULT __stdcall TeleportLocomotionClass::Save(IStream *stream, BOOL clear_dirty) (0x006E1810)
// LocomotionClass::Save, then whether a locomotor is carried and, if so,
// that locomotor (OleSaveToStream through its IPersistStream).

HRESULT __stdcall TeleportLocomotionClass::Save(IStream *stream, BOOL clear_dirty)
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


// int __stdcall TeleportLocomotionClass::Layer() (0x006E18F0, ILocomotion 29)

int __stdcall TeleportLocomotionClass::Layer()
{
    return 2;
}


// HRESULT __stdcall TeleportLocomotionClass::QueryInterface(GUID const &iid, void **object) (0x006E1900)
// The base interfaces, then IPiggyback.

HRESULT __stdcall TeleportLocomotionClass::QueryInterface(GUID const &iid, void **object)
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


// HRESULT __stdcall TeleportLocomotionClass::Begin_Piggyback(ILocomotion *pointer) (0x006E1960, IPiggyback 3)
// Carries the locomotor unless one is already carried.

HRESULT __stdcall TeleportLocomotionClass::Begin_Piggyback(ILocomotion *pointer)
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


// HRESULT __stdcall TeleportLocomotionClass::End_Piggyback(ILocomotion **pointer) (0x006E19B0, IPiggyback 4)
// Hands the carried locomotor (and its reference) back.

HRESULT __stdcall TeleportLocomotionClass::End_Piggyback(ILocomotion **pointer)
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


// bool __stdcall TeleportLocomotionClass::Is_Ok_To_End() (0x006E19F0, IPiggyback 5)
// Once idle (no warp under way) while carrying a locomotor.

bool __stdcall TeleportLocomotionClass::Is_Ok_To_End()
{
    if (!Is_Moving() && Piggybacker != 0 && !unknown_35 && !LinkedTo->unknown_210 && !unknown_38) {
        return true;
    }
    return false;
}


// HRESULT __stdcall TeleportLocomotionClass::Piggyback_CLSID(GUID *classid) (0x006E1A40, IPiggyback 6)
// The class ID of the carried locomotor, or this locomotor's own when none is
// carried; each is queried for IPersist (TibSun Drive.cpp Piggyback_CLSID).

HRESULT __stdcall TeleportLocomotionClass::Piggyback_CLSID(GUID *classid)
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


// void __stdcall TeleportLocomotionClass::Mark_All_Occupation_Bits(int mark) (0x006E1B50, ILocomotion 39)
// Lifting the mark (0) clears the occupation of the head-to coordinate (object slot 59).

void __stdcall TeleportLocomotionClass::Mark_All_Occupation_Bits(int mark)
{
    if (mark == 0) {
        UnknownCoord434820 coord = Head_To_Coord();
        LinkedTo->Clear_Occupy_Bit(coord);
    }
}


// unsigned long __stdcall TeleportLocomotionClass::AddRef() (0x006E1BA0)

unsigned long __stdcall TeleportLocomotionClass::AddRef()
{
    return LocomotionClass::AddRef();
}


// unsigned long __stdcall TeleportLocomotionClass::Release() (0x006E1BB0)

unsigned long __stdcall TeleportLocomotionClass::Release()
{
    return LocomotionClass::Release();
}


// bool __stdcall TeleportLocomotionClass::Is_Piggybacking() (0x006E1BC0, IPiggyback 7)

bool __stdcall TeleportLocomotionClass::Is_Piggybacking()
{
    return Piggybacker != 0;
}


// int TeleportLocomotionClass::Size() (0x006E1BE0): inline in shared/teleport_locomotion.hpp, emitted with the vtable; no source of its own.


// void *TeleportLocomotionClass::`scalar deleting destructor'(unsigned int flags) (0x006E1BF0)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.


// TeleportLocomotionClass::QueryInterface adjustor thunk for ILocomotion (0x006E1C20)
// Compiler generated (this adjusted, then QueryInterface). No source of its own.


// TeleportLocomotionClass::AddRef adjustor thunk for ILocomotion (0x006E1C30)
// Compiler generated (this adjusted, then AddRef). No source of its own.


// TeleportLocomotionClass::Release adjustor thunk for ILocomotion (0x006E1C40)
// Compiler generated (this adjusted, then Release). No source of its own.


// TeleportLocomotionClass::QueryInterface adjustor thunk for IPiggyback (0x006E1C50)
// Compiler generated (this adjusted, then QueryInterface). No source of its own.


// TeleportLocomotionClass::AddRef adjustor thunk for IPiggyback (0x006E1C60)
// Compiler generated (this adjusted, then AddRef). No source of its own.


// TeleportLocomotionClass::Release adjustor thunk for IPiggyback (0x006E1C70)
// Compiler generated (this adjusted, then Release). No source of its own.
