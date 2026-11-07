// void FootClass::unknown_slot288() (0x004CA880)
// In mission 15 (hypothesis: unloading) clears slot 220 and calls slot 260.
#include "shared/foot.hpp"

void FootClass::unknown_slot288()
{
    if (unknown_slot91() == 15) {
        unknown_slot220(0);
        Assign_Destination(0, true);
    }
}
