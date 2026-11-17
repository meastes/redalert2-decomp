#include "shared/score.hpp"
#include "shared/sound_available.hpp"
#include "shared/crt_memory.hpp"
// 0x006682B0: 14-byte body and two original padding bytes.
ScoreSoundClass::~ScoreSoundClass()
{
    if (Name) {
        free(Name);
    }
}
