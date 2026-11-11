// bool Voices::Anim::Has_Finished() (0x00732C10)
// True once nothing is being said.
#include "shared/wdt_voices.hpp"

namespace WorldDominationTour {

bool Voices::Anim::Has_Finished()
{
    return !Speech_Is_Playing();
}

}
