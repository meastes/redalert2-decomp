// void RadBeam::Update_All() (0x006354E0; name hypothesis; called once a frame by 0x006A4E80)
// Runs every listed beam, last first, and deletes each one whose period is over. When the game
// runs below its desired rate (unknown_a6eb1c / unknown_a3d568, cubed), type 1 beams step
// further and active type 2 beams are switched off at random. The list length is read once
// for both passes.
#include "shared/rad_beam_list.hpp"
#include "shared/queue.hpp"
#include "shared/random2.hpp"
// @ref ?unknown_6358d0@RadBeam@@QAE_NXZ=006358d0
// @ref ?unknown_635e00@RadBeam@@QAE_NXZ=00635e00

void __fastcall RadBeam::Update_All()
{
    int total = RadBeamList::Array.Count();
    int count = 0;
    int active = 0;
    for (int index = total - 1; index >= 0; index--) {
        RadBeam *beam = RadBeamList::Array[index];
        if (beam->Type == 2) {
            count++;
            if (beam->IsActive) {
                active++;
            }
        }
    }
    double desired = unknown_a3d568;
    double rate = unknown_a6eb1c / desired;
    rate = rate * rate * rate;
    double excess = (double)active / count - rate;
    double cull = 0.0;
    if (excess > 0.0) {
        cull = count * excess;
    }
    cull /= active;
    double stretch = 1.0 / rate;

    for (index = total - 1; index >= 0; index--) {
        RadBeam *beam = RadBeamList::Array[index];
        bool done;
        if (beam->Type == 1) {
            int length = (int)(stretch * 10.0);
            if (length < 10) {
                length = 10;
            } else if (length > 100) {
                length = 100;
            }
            beam->Set_Step_Length(length);
            done = beam->unknown_6358d0();
        } else if (beam->Type == 2) {
            if (beam->IsActive) {
                if (unknown_random_839218(0, 99) < (int)(cull * 100.0)) {
                    beam->Deactivate();
                }
            }
            done = beam->unknown_635e00();
        } else {
            continue;
        }
        if (done) {
            RadBeamList::Array.Delete(beam);
            delete beam;
        }
    }
}
