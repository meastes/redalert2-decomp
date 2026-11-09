// void __fastcall Unknown_0042e830(Unknown84E8D0 *beacons) (0x0042E830): when the beacons
// are enabled (+0x60), clears the blink bit (bit 1) of every beacon in the eight houses'
// three slots (Clear_Selected_Objects 0x00482BC0 calls it).
#include "shared/beacon.hpp"

void __fastcall Unknown_0042e830(Unknown84E8D0 *beacons)
{
    if (beacons->unknown_60 != 0) {
        for (int house = 0; house < 8; house++) {
            for (int slot = 0; slot < 3; slot++) {
                if (beacons->unknown_00[house][slot] != 0) {
                    beacons->unknown_00[house][slot]->unknown_flags &= ~2;
                }
            }
        }
    }
}
