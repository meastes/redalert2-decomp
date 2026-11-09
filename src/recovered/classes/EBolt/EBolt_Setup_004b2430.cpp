#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/ebolt.hpp"
#include "shared/random.hpp"
#include "shared/particle_system.hpp"
// @ref _unknown_geometry_l=00853028
void EBolt::Setup_004B2430(CoordStruct start,CoordStruct end,int value)
{
 Start=start;
 End=end;
 unknown_18=value;
 unknown_1c=unknown_random_839218(0,256);
 unknown_EBolt_list_00853050.Add(this);
 new ParticleSystemClass(Rules->unknown_ce0,end,0,0,unknown_geometry_l);
}
