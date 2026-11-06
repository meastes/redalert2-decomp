// Operation/name/return intent are hypotheses; physical RET28 and AL predicate. A member of the generator: both
// callers (0x57C9E0, 0x57D3A0) pass this in ECX, which is not read (ra2-decomp YR twin MapGeneratorClass::FUN_0059BAB0).
#include "shared/coord.hpp"
#include "shared/map_view.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/unknown_map_generator.hpp"
extern double const unknown_half_79a728,unknown_one_79a708;
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?unknown_half_79a728@@=0079a728
// @ref ?unknown_one_79a708@@=0079a708
// @ref _unknown_static_rect=00a70080
bool UnknownMapGenerator::Cell_In_Generation_Shape_0057d290(Cell const &position,Rect const &area,bool ellipse,double x_weight,double y_weight)
{
    if(area==unknown_static_rect)return true;
    if(ellipse) {
        int x=(int)((unsigned)(int)position.X-2u*(unsigned)area.X-(unsigned)(int)position.Y-1u+(unsigned)Map.MapSize.Width);
        int y=(int)((unsigned)(int)position.X-2u*(unsigned)area.Y-(unsigned)Map.MapSize.Width-1u+(unsigned)(int)position.Y);
        double dx=(double)x*unknown_half_79a728-(double)area.Width*unknown_half_79a728;
        double dy=(double)y*unknown_half_79a728-(double)area.Height*unknown_half_79a728;
        double weight=dy*dy*y_weight;
        weight+=dx*dx*x_weight;
        return weight<unknown_one_79a708;
    }
    int px=position.X;
    int py=position.Y;
    return px>=area.X && px<(int)((unsigned)area.X+(unsigned)area.Width) &&
           py>=area.Y && py<(int)((unsigned)area.Y+(unsigned)area.Height);
}
