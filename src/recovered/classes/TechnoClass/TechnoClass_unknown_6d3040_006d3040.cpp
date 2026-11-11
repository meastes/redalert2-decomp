// void TechnoClass::unknown_6d3040(TechnoClass *killer) (0x006D3040)
// Destroys every passenger: each leaves its team, is detached from the
// cargo, gets slot 55 (killer) and is deleted.
#include "shared/techno.hpp"
// @ref ?Attached_Object@CargoClass@@QBEPAVObjectClass@@XZ=00468a10
// @ref ?Detach_Object@CargoClass@@QAEPAVObjectClass@@XZ=004689f0
// @ref ?unknown_6b8fb0@UnknownPassengerTeam@@QAEXPAVObjectClass@@H_N@Z=006b8fb0

void TechnoClass::unknown_6d3040(TechnoClass *killer)
{
    while (Cargo.Object != 0) {
        ObjectClass *passenger = Cargo.Attached_Object();
        if (((UnknownPassengerView *)passenger)->Team != 0) {
            ((UnknownPassengerView *)passenger)->Team->unknown_6b8fb0(passenger, -1, false);
        }
        passenger = Cargo.Detach_Object();
        if (passenger != 0) {
            passenger->Record_The_Kill(killer);
            delete passenger;
        }
    }
}
