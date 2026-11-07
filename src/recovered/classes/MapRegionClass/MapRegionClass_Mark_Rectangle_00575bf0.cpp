#include "shared/map_region.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &);
void MapRegionClass::Mark_Rectangle_00575bf0(Rect const &area)
{
    for(int y=area.Y;y<area.Y+area.Height;++y) {
        for(int x=area.X;x<area.X+area.Width;++x) {
            Cell const position(x,y);
            if(Generation_Cell_In_Bounds_0058da10(position)) {
                Set_Cell_Region_Index_0056e970(position,Index);
                CellClass &target=Map[position];
                target.unknown_11b=(char)unknown_10;
            }
        }
    }
}

