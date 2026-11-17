#include "shared/map_region.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/random2.hpp"
extern Random2Class RMGRandom;
// ECX hidden result, EAX same result pointer, RET0; retry limit precedes the map-clear test.
Cell __fastcall MapRegionClass::Pick_Random_Clear_Map_Cell()
{
    int tries=0;
    for(;;) {
        Cell position=Pick_Random_Map_Cell();
        if(++tries>200)return Cell(0,0);
        if(Map[position].Is_Tile_Clear())return position;
    }
}

#include "shared/random.hpp"
extern Random2Class RMGRandom;
#include "shared/random.hpp"
extern Random2Class RMGRandom;
// Random map callback: map the generator's complete unsigned word to [0,1].
double __cdecl Map_Random_Fraction_00579770()
{
    return (unsigned int)RMGRandom() * (1.0 / 4294967295.0);
}

unsigned int __fastcall Map_Random_Range_005797a0(unsigned int minimum,unsigned int maximum)
{
    unsigned int result;
    do {
        result=(unsigned int)(Map_Random_Fraction_00579770()*(unsigned int)(maximum-minimum+1)+(double)minimum);
    } while(result>maximum);
    return result;
}
