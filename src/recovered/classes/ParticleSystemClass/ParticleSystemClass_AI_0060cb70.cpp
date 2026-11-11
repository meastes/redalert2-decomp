// void ParticleSystemClass::AI() (0x0060CB70)
// Runs the behaviour routine of the type, ends the system when its lifetime
// runs out, and once ended and empty leaves the map and queues itself in the
// 0x00AC0CD8 list.
#include "shared/particle_system.hpp"
// @ref ?Gas_AI@ParticleSystemClass@@QAEXXZ=0060b4e0
// @ref ?Spark_AI@ParticleSystemClass@@QAEXXZ=0060b670
// @ref ?unknown_func_60bb70@ParticleSystemClass@@QAEXXZ=0060bb70
// @ref ?Railgun_AI@ParticleSystemClass@@QAEXXZ=0060c050
// @ref ?Fire_AI@ParticleSystemClass@@QAEXXZ=0060c7c0

void ParticleSystemClass::AI()
{
    switch (Class->BehavesLike) {
    case 0:
        unknown_func_60bb70();
        break;
    case 1:
        Gas_AI();
        break;
    case 2:
        Fire_AI();
        break;
    case 3:
        Spark_AI();
        break;
    case 4:
        Railgun_AI();
        break;
    }
    if (--Lifetime == 0) {
        Delete_Me();
    }
    if (unknown_7d && unknown_e4 && Particles.Count() == 0) {
        Limbo();
        unknown_7d = false;
        unknown_objects_ac0cd8.Add(this);
    }
}
