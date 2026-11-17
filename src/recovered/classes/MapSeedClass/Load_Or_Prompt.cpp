#include "shared/map_seed.hpp"
bool MapSeedClass::Load_Or_Prompt_005791a0(char const *filename)
{
    if(filename) return Load(filename);
    return Load_Dialog_0053b190()!=0 ? true : false;
}
