#include "shared/map_region.hpp"
#include "shared/unknown_map_generator.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_view.hpp"
void Init_Dirt_Roads();
#include "shared/map_gaussian.hpp"
double __stdcall Sample_Truncated_Normal(double mean,double deviation,double minimum,double maximum)
{
    if(maximum<mean-deviation || mean+deviation<minimum) {
        deviation=(maximum-minimum)*0.5;
        mean=deviation+minimum;
    }
    double value;
    do {
        value=unknown_map_gaussian_a6fd50.Next()*deviation+mean;
    } while(value<minimum || value>maximum);
    return value;
}

bool __fastcall Neighborhood_Is_Clear_00588380(Cell const &center);
namespace seed_0056dac0 { bool __fastcall unknown_generation_cell_clear_0056dac0(short *position); }
void MapGeneratorClass::Generate_Urban_Areas()
{
    int count=(int)((SeedData.UrbanPresence+0.32)*3);
    int tries=0,placed=0;
    Init_Dirt_Roads();
    while(tries<10) {
        if(placed>=count)break;
        Cell position;
        position.X=0;position.Y=0;
        int localTries=0;
        for(;;) {
            position=MapRegionClass::Pick_Random_Clear_Map_Cell();
            if(++localTries>=20)break;
            if(seed_0056dac0::unknown_generation_cell_clear_0056dac0((short *)&position) || Neighborhood_Is_Clear_00588380(position))break;
        }
        if(Map_Random_Fraction_00579770()<1.0-SeedData.UrbanPresence) {
            if(Generate_Rural_Roads(position)) {
                Generate_Rural_Buildings(position);
                Generate_Rural_Units(position);
                ++placed;
            }
        } else {
            if(localTries!=20) {
                int mean=(int)((unsigned)Map.MapSize.Width*(unsigned)Map.MapSize.Height*2u)/30;
                int size=(int)Sample_Truncated_Normal((double)mean,300.0,100.0,2000.0);
                DynamicVectorClass<Cell> *cells=Create_Urban_Area(position,size);
                if(cells) {
                    Generate_Urban_Roads(*cells);
                    Generate_Urban_Buildings(*cells);
                    Generate_Urban_Units(*cells);
                    Generate_Urban_Pavement(*cells);
                    ++placed;
                    delete cells;
                }
            }
            ++tries;
        }

    }
}

// Packed coordinate ECX and AL-only result; freefastcall/constreference/bool/name are source hypotheses.
#include "shared/coord.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_view.hpp"
bool __fastcall Unknown_Cell_Predicate_0047b5e0(CellClass *cell);
bool __fastcall Neighborhood_Is_Clear_00588380(Cell const &center)
{
    int centerY=center.Y;
    int top=centerY-7;
    int centerX=center.X;
    int right=centerX+7;
    int bottom=centerY+7;
    for(int x=centerX-7;x<right;++x) {
        for(int y=top;y<bottom;++y) {
            Cell position((short)x,(short)y);
            if(Unknown_Cell_Predicate_0047b5e0(&Map[position]))return false;
        }
    }
    return true;
}
