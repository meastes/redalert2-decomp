// bool ParticleSystemClass::Is_Inactive() const (0x0060CC70)
// True once the system has ended and has no particles left.
#include "shared/particle_system.hpp"

bool ParticleSystemClass::Is_Inactive() const
{
    return unknown_e4 && Particles.Count() == 0;
}
