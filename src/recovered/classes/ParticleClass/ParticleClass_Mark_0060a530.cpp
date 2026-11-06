// bool ParticleClass::Mark(int arg1) (0x0060A530)
// The ObjectClass result as a truth value.
#include "shared/particle.hpp"
// @ref ?Mark@ObjectClass@@UAE_NH@Z=005d4160

bool ParticleClass::Mark(int arg1)
{
    return ObjectClass::Mark(arg1) ? true : false;
}
