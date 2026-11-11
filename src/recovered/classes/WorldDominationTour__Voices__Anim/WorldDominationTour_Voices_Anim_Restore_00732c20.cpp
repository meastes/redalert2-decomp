// void Voices::Anim::Restore(int unknown1) (0x00732C20)
#include "shared/wdt_voices.hpp"

namespace WorldDominationTour {

void Voices::Anim::Restore(int unknown1)
{
    Owner->Update();
}

}
