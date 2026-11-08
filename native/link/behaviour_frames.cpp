// Behaviour-written bodies of unmatched rows the skirmish harness reaches after
// its first Main_Loop iteration (Scout1089; --run-to=frames). NOT EXACT; never
// original code or bytes.
#define UNKNOWN_BEACONS_DECLARE_MEMBERS
#include "shared/unknown_globals.hpp"
#include "shared/win32_window.hpp"
#include "shared/lobby.hpp"

// 0x0042EBE0 (seed src/recovered/0042ebe0.cpp; HouseClass::MPlayer_Defeated
// 0x004E77F0 calls it): while beacons are enabled (+0x60), removes each of the
// house's three beacon slots that holds one (0x0042E990).
void Unknown84E8D0::Unknown_0042EBE0(int house)
{
    if (unknown_60 != 0) {
        for (int slot = 0; slot < 3; slot++) {
            if (unknown_00[house][slot] != 0) {
                Unknown_0042E990(house, slot);
            }
        }
    }
}

// 0x00735B20 (leaf src/recovered/00735b20.cpp): the current dialog's resource
// id (0x00B25DD8).
int Current_Dialog_Id_00735b20()
{
    return CurrentDialogResource;
}

// 0x005173A0 (leaf src/recovered/005173a0.cpp): an empty function.
void Unknown_005173A0()
{
}

#include "shared/techno.hpp"

// 0x0041B380 TechnoClass slot 193 (+0x304): `xor al, al; ret`, false.
bool TechnoClass::unknown_slot193()
{
    return false;
}
