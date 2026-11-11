// bool ParticleSystemTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const (0x00620EA0, slot 32)
// Particle system types are never placed as objects.
#include "shared/particle_system.hpp"

bool ParticleSystemTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    return false;
}
