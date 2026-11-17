// bool CaptureManagerClass::Capture(TechnoClass *target) (0x00467FF0)
// Releases the object held so far (back to the house it came from, +0x28),
// then takes over a capturable techno: it changes to the owner's house
// (slot 222; on refusal nothing is held), records the owner as its
// controller (+0x234), leaves its team, drops its targets and goes to
// mission 5; an attached +0x208 object lets go of its target.
#include "shared/capture_manager.hpp"
#include "shared/unit.hpp"
#include "shared/foot.hpp"
#include "shared/techno.hpp"

bool CaptureManagerClass::Capture(TechnoClass *target)
{
    if (Unit != 0) {
        Unit->MindControlledBy = 0;
        Unit->Captured(unknown_28, true);
        Unit = 0;
        unknown_28 = 0;
    }
    bool capture = target != 0 && target->AbstractFlags.IsTechno && Can_Capture(target);
    if (!capture) {
        return false;
    }
    unknown_28 = target->Owning_House();
    if (!target->Captured(Owner->Owning_House(), true)) {
        unknown_28 = 0;
        return false;
    }
    Unit = target;
    target->MindControlledBy = Owner;
    if (Unit != 0 && Unit->AbstractFlags.IsFoot && ((FootClass *)Unit)->Team != 0) {
        ((FootClass *)Unit)->Team->Remove((FootClass *)Unit, -1, 0);
    }
    Unit->Assign_Destination(0, true);
    Unit->unknown_slot220(0);
    Unit->Assign_Archive_Target(0);
    Unit->QueueMission(5, true);
    if (Unit->TemporalImUsing != 0 && Unit->TemporalImUsing->unknown_28 != 0) {
        Unit->TemporalImUsing->LetGo();
    }
    return true;
}
