// NON-EXACT DRAFT 0x4386F0 FUN_004386f0
// entry 0x4386F0; exact-match score 82.58 (misses_index best); plan "draft, verify"
// origin lane: Scout773 (build/class-work/Scout773/keep/dt_p5_probe_fetch_8927.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// bool __fastcall Building_Player_Detector_Covers_Target_004386f0(FootClass *object) (0x004386F0)
// Tests the player's active psychic detectors against an unallied object's
// target and the last entry of its navigation queue.
#include "shared/building.hpp"
#include "shared/house.hpp"
#include "shared/cell.hpp"
#include "shared/foot.hpp"
#include "shared/unknown_vector3.hpp"
// @ref ??0UnknownVector3@@QAE@HHH@Z=0041b630
// @ref ?Length@UnknownVector3@@QBEHXZ=0041b780
// @ref ?unknown_475d60@CellClass@@QBE?AUCoordStruct@@XZ=00475d60
static inline UnknownVector3 Detector_Offset(CoordStruct const &a, CoordStruct const &b)
{
    return UnknownVector3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
}

bool __fastcall Building_Player_Detector_Covers_Target_004386f0(FootClass *object)
{
    if (!unknown_player_a35db4->Is_Ally(object->House)) {
    int count = unknown_player_a35db4->unknown_b0.Count();
    for (int index = 0; index < count; ++index) {
        BuildingClass *detector = unknown_player_a35db4->unknown_b0.Fetch(index);
        if (detector->unknown_slot195()) {
            AbstractClass *target = object->unknown_228;
            if (target != 0) {
                if (target->AbstractFlags.IsTechno) {
                    CoordStruct position = detector->Center_Coord();
                    CoordStruct other = target->Center_Coord();
                    UnknownVector3 offset = Detector_Offset(position, other);
                    bool hit = offset.Length() <= detector->Type->PsychicDetectionRadius * 256;
                    if (hit) {
                        return hit;
                    }
                } else {
                    bool hit = target->Kind_Of() == 11;
                    if (hit) {
                        CoordStruct position = detector->Center_Coord();
                        CoordStruct other = ((CellClass *)target)->unknown_475d60();
                        UnknownVector3 offset = Detector_Offset(position, other);
                        hit = offset.Length() <= detector->Type->PsychicDetectionRadius * 256;
                    }
                    if (hit) {
                        return hit;
                    }
                }
            }
            if (object != 0) {
            bool const &is_foot = object->AbstractFlags.IsFoot;
            if (is_foot) {
                AbstractClass *nav = object->unknown_478.Count() == 0 ? object->unknown_494 : object->unknown_478[object->unknown_478.Count() - 1];
                if (nav != 0) {
                    bool hit = nav->Kind_Of() == 11;
                    if (hit) {
                        CoordStruct position = detector->Center_Coord();
                        CoordStruct other = ((CellClass *)nav)->unknown_475d60();
                        UnknownVector3 offset = Detector_Offset(position, other);
                        double y = offset.Y;
                        double z = offset.Z;
                        double x = offset.X;
                        hit = (int)unknown_sqrt_4ba410(x * x + z * z + y * y) <= detector->Type->PsychicDetectionRadius * 256;
                        if (hit) {
                            return hit;
                        }
                    } else if (nav->AbstractFlags.IsTechno) {
                        CoordStruct position = detector->Center_Coord();
                        CoordStruct other = nav->Center_Coord();
                        UnknownVector3 offset = Detector_Offset(position, other);
                        hit = offset.Length() <= detector->Type->PsychicDetectionRadius * 256;
                        if (hit) {
                            return hit;
                        }
                    }
                }
            }
            }
        }
    }
    }
    return false;
}
