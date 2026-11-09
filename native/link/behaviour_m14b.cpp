// Behaviour rows (Scout1121, native M14b; NOT EXACT, native/behaviour_rewrites.json):
// leaves the menu-started skirmish reaches that have no recovered body.
#include "shared/object.hpp"

// 0x00423D30 (ObjectClass slot 31, +0x7C): the shared leaf `return false`
// (pass-6a decompile 00423d30: Leaf_423d30::get). Hunting units ask it.
bool ObjectClass::unknown_slot31()
{
    return false;
}

// 0x0041B300 (ObjectClass slot 83, +0x14C): the shared leaf `return false`
// (pass-6a decompile 0041b300).
bool ObjectClass::unknown_slot83()
{
    return false;
}

// ParticleSystemClass 0x0060AA70 under shared/particle_system.hpp's TPoint3D
// profile (RA2_PARTICLE_SYSTEM_POINT3D, the Techno unit) is the same
// constructor as its CoordStruct spelling (TPoint3D<int> and CoordStruct are
// three ints): one address, two declarations; the TPoint3D name forwards to
// the defined one.
extern "C" void ra2_particle_system_ctor_coord(void *self, void *type, void const *coord, void *target, void *owner,
    void const *target_coord) __asm__("__ZN19ParticleSystemClassC1EP23ParticleSystemTypeClassRK11CoordStructP13AbstractClassP11ObjectClassS4_");
extern "C" void ra2_particle_system_ctor_point3d(void *self, void *type, void const *coord, void *target, void *owner,
    void const *target_coord) __asm__("__ZN19ParticleSystemClassC1EP23ParticleSystemTypeClassRK8TPoint3DIiEP13AbstractClassP11ObjectClassRK11CoordStruct");
void ra2_particle_system_ctor_point3d(void *self, void *type, void const *coord, void *target, void *owner,
    void const *target_coord)
{
    ra2_particle_system_ctor_coord(self, type, coord, target, owner, target_coord);
}
