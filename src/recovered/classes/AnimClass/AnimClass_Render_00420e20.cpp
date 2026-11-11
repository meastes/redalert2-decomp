// bool AnimClass::Render(Rect *bounds, bool arg2, bool arg3) (0x00420E20)
// Refused while the animation waits out its loop delay; otherwise the
// ObjectClass slot. Needs ObjectClass::Render declared bool
// (0x005D34E0 returns al).
#include "shared/anim.hpp"
// @ref ?Render@ObjectClass@@UAE_NHHH@Z=005d34e0

bool AnimClass::Render(Rect *bounds, bool arg2, bool arg3)
{
    if (LoopDelay != 0) {
        return false;
    }
    return ObjectClass::Render(bounds, arg2, arg3);
}
