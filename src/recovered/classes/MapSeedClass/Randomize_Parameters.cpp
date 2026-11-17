// Map-type index range0..4 follows Clamp_Parameters_00578D80.
// Range helper spells a real RNG operation; original helper name is unknown.
#include "shared/map_seed.hpp"
#include "shared/random.hpp"
extern int _water_min[5],_water_max[5];
extern int _rugged_min[5],_rugged_max[5];
extern int _urban_min[5],_urban_max[5];
extern int _access_min[5],_access_max[5];
extern int _region_size_min[5],_region_size_max[5];
extern int *_vegetation_min,*_vegetation_max;
inline int MapSeed_Random_Range(int minimum,int maximum)
{ return NonCriticalRandomNumber(minimum,maximum); }
void MapSeedClass::Randomize_Parameters_005789f0(int map_type)
{
    WaterAmount=MapSeed_Random_Range(_water_min[map_type],_water_max[map_type]);
    Ruggedness=MapSeed_Random_Range(_rugged_min[map_type],_rugged_max[map_type]);
    UrbanPresence=MapSeed_Random_Range(_urban_min[map_type],_urban_max[map_type]);
    Accessibility=MapSeed_Random_Range(_access_min[map_type],_access_max[map_type]);
    RegionSize=MapSeed_Random_Range(_region_size_min[map_type],_region_size_max[map_type]);
    Tiberium=Resources*20;
    TiberiumLayout=MapSeed_Random_Range(0,100);
    int minimum=_vegetation_min[map_type];
    int maximum=_vegetation_max[map_type];
    if(minimum<0) minimum=0;
    else if(minimum>100) minimum=100;
    if(maximum<0) maximum=0;
    else if(maximum>100) maximum=100;
    if(minimum>maximum) minimum=maximum;
    Vegetation=MapSeed_Random_Range(minimum,maximum);
    Seed=MapSeed_Random_Range(0,65535);
}
