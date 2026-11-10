#include "shared/unknown_map_generator.hpp"
// @ref ?Seed_River@MapGeneratorClass@@=0057ecf0
// @ref ?Seed_Lake@MapGeneratorClass@@=0057e100
void MapGeneratorClass::Seed_Water()
{
    ++WorkingRegionID;
    if((SeedData.MapType==3 || SeedData.MapType==4) && SeedData.WaterAmount>20) {
        int attempts=0;
        bool success=false;
        while(!success) {
            if(attempts>=10)break;
            success=Seed_River(Cell(0,0),0.0,false);
            ++attempts;
        }
        if(success)++WorkingRegionID;
    }
    int attempts=0;
    bool success=false;
    while(!success) {
            if(attempts>=10)break;
        success=Seed_Lake(Cell(0,0));
        ++attempts;
    }
    if(success)++WorkingRegionID;
}
