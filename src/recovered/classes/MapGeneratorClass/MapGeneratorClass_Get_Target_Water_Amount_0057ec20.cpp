#include "shared/unknown_map_generator.hpp"
extern double const unknown_water_scale_7a5f38,unknown_water_offset_79b7f0;
// @ref ?unknown_water_scale_7a5f38@@=007a5f38
// @ref ?unknown_water_offset_79b7f0@@=0079b7f0
// @ref __ftol=0077f110
int MapGeneratorClass::Get_Target_Water_Amount() const
{
    int water=SeedData.WaterAmount;
    if(water==0)return 0;
    return (int)((double)LocalHeight*LocalWidth*water*unknown_water_scale_7a5f38+unknown_water_offset_79b7f0);
}
