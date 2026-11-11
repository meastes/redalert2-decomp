// bool ParticleTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const (0x00621E60, slot 32)
// Particle types are never placed as objects.
#include "shared/particle_system.hpp"

bool ParticleTypeClass::Create_And_Place(Cell const &cell, HouseClass *house) const
{
    return false;
}
