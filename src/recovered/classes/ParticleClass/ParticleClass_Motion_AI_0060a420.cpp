// void ParticleClass::Motion_AI() (0x0060A420)
// Moves the particle by its type's behaviour (0, 1 or 2; others do not
// move here).
#include "shared/particle.hpp"
// @ref ?Smoke_Motion_AI@ParticleClass@@QAEXXZ=0060a0e0
// @ref ?Gas_Motion_AI@ParticleClass@@QAEXXZ=0060a230

void ParticleClass::Motion_AI()
{
    switch (Class->BehavesLike) {
    case 0:
        Smoke_Motion_AI();
        break;
    case 1:
        Gas_Motion_AI();
        break;
    case 2:
        Fire_Motion_AI();
        break;
    }
}
