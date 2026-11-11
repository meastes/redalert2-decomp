// void ParticleClass::Behavior_AI() (0x00609C90)
// Runs the behaviour routine of the type's BehavesLike value, then counts
// the lifetime down and flags the particle done at zero.
#include "shared/particle.hpp"
// @ref ?Gas_Behavior_AI@ParticleClass@@QAEXXZ=00608bb0
// @ref ?Railgun_Behavior_AI@ParticleClass@@QAEXXZ=00609200
// @ref ?Smoke_Behavior_AI@ParticleClass@@QAEXXZ=006093a0
// @ref ?unknown_func_609540@ParticleClass@@QAEXXZ=00609540
// @ref ?Fire_Behavior_AI@ParticleClass@@QAEXXZ=00609970

void ParticleClass::Behavior_AI()
{
    switch (Class->BehavesLike) {
    case 0:
        Gas_Behavior_AI();
        break;
    case 1:
        Smoke_Behavior_AI();
        break;
    case 2:
        Fire_Behavior_AI();
        break;
    case 3:
        unknown_func_609540();
        break;
    case 4:
        Railgun_Behavior_AI();
        break;
    }
    Lifetime--;
    if (Lifetime == 0) {
        unknown_121 = true;
    }
}
