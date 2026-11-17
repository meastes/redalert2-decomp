#include "shared/map_seed.hpp"
#include "shared/random.hpp"
// @ref ?Randomize_Parameters_005789f0@MapSeedClass@@=005789f0
// @ref ?Fixup_Settings@MapSeedClass@@=00578d80
void MapSeedClass::Randomize_00578b10()
{
    Theater=NonCriticalRandomNumber(0,100)>=50U;
    MapType=NonCriticalRandomNumber(1,4);
    Time=NonCriticalRandomNumber(0,3);
    Resources=NonCriticalRandomNumber(0,3);
    Height=Width=NonCriticalRandomNumber(0,3);
    Randomize_Parameters_005789f0(MapType);
    Set_Default_Description_00576fe0();
    Seed=NonCriticalRandomNumber(0,65535);
    Fixup_Settings();
}
