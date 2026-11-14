// cargo.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/cargo.json carries the addresses and the rows that
// stay in their own files.


#include "shared/foot.hpp"
#include "shared/techno.hpp"


// void CargoClass::Attach(FootClass *passenger) (0x00468960)
// Adds a passenger to the hold (TibSun CargoClass::Attach): the passenger
// leaves the map, any foot objects already chained to it are kept in front of
// the old hold, and the count is recomputed along the foot chain.

// TibSun's Is_Foot() test of a link that may be null (VC6_MATCHING_NOTES
// Cell351: the flag byte is spilled once).
inline bool Cargo_Is_Foot(ObjectClass const *object)
{
    bool is_foot;
    if (object != 0) is_foot = object->AbstractFlags.IsFoot;
    return object != 0 && is_foot;
}

void CargoClass::Attach(FootClass *passenger)
{
    if (passenger == 0) return;

    passenger->Limbo();

    ObjectClass *o = 0;
    if (passenger->unknown_30 && Cargo_Is_Foot(passenger->unknown_30)) {
        o = passenger->unknown_30;
        while (o != 0) {
            if (o->unknown_30 == 0) break;
            if (!Cargo_Is_Foot(o->unknown_30)) {
                o = 0;
                break;
            }
            o = o->unknown_30;
            if (o == 0) break;
        }
    }
    if (o != 0) {
        o->unknown_30 = Object;
    } else {
        passenger->unknown_30 = Object;
    }

    Object = passenger;
    Count = 0;
    passenger = (FootClass *)Object;
    while (passenger != 0) {
        Count++;
        if (passenger->unknown_30 == 0) break;
        if (!Cargo_Is_Foot(passenger->unknown_30)) break;
        passenger = (FootClass *)passenger->unknown_30;
    }
}


ObjectClass *CargoClass::Detach_Object() {
    ObjectClass *passenger = Object;
    if (!passenger) return 0;
    Object = passenger->unknown_30;
    passenger->unknown_30 = 0;
    --Count;
    return passenger;
}


// Existing canonical getter00468A10, rewritten with its real current class; zero new credit.
ObjectClass *CargoClass::Attached_Object() const { return Object; }


// Cargo total size00468A20, complete77+3. Private current diagnostic; not accepted.
// OwnerR489scope approved2026-10-26T0210Z. CapturedCount, per-passenger int truncation.

int CargoClass::GetTotalSize() const
{
    int limit = Count;
    int total_size = 0;
    ObjectClass *first = Object;
    if (first) {
        FootClass *passenger = static_cast<FootClass *>(Attached_Object());
        if (passenger) {
            for (int index = 0; index < limit && passenger; ++index) {
                total_size = (int)(total_size + passenger->Techno_Type_Class()->Size);
                passenger = static_cast<FootClass *>(passenger->unknown_30);
            }
        }
    }
    return total_size;
}


// Unlinks only: original65-bytebody leaves Count and removed-next unchanged.
void CargoClass::Detach(FootClass *passenger)
{
    if (passenger && Object) {
        if (Object == passenger) Object = Object->unknown_30;
        else {
            ObjectClass *previous = Object;
            while (previous->unknown_30) {
                if (passenger == previous->unknown_30) {
                    previous->unknown_30 = previous->unknown_30->unknown_30;
                    break;
                }
                previous = previous->unknown_30;
            }
        }
    }
}
