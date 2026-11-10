#include "shared/unknown_map_generator.hpp"
// @ref ?unknown_0057ecf0@MapGeneratorClass@@=0057ecf0
// @ref ?unknown_0057e100@MapGeneratorClass@@=0057e100
void MapGeneratorClass::Run_Region_Attempts_0057dd60()
{
    ++WorkingRegionID;
    if((SeedData.MapType==3 || SeedData.MapType==4) && SeedData.WaterAmount>20) {
        int attempts=0;
        bool success=false;
        while(!success) {
            if(attempts>=10)break;
            success=unknown_0057ecf0(Cell(0,0),0.0,false);
            ++attempts;
        }
        if(success)++WorkingRegionID;
    }
    int attempts=0;
    bool success=false;
    while(!success) {
            if(attempts>=10)break;
        success=unknown_0057e100(Cell(0,0));
        ++attempts;
    }
    if(success)++WorkingRegionID;
}
