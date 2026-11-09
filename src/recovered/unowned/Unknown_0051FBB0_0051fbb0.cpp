// void Unknown_0051FBB0() (0x0051FBB0)
// Ends an active lightning storm at its next update.
#include "shared/lightning_storm.hpp"

void Unknown_0051FBB0()
{
    if (StormActive) {
        StormTimeToEnd = true;
    }
}
