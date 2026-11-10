// Collect the nonzero cells in this region that have an in-bounds neighbor in another region.
#include "shared/unknown_map_generator.hpp"
#include "shared/map_region.hpp"
#include "shared/cell.hpp"

static inline bool In_Zone_Map_Bounds(Cell const &position);
static inline int Read_Generation_Region(Cell const &position);

DynamicVectorClass<Cell> *MapGeneratorClass::Build_Region_Border_Cell_List(int region)
{
    DynamicVectorClass<Cell> *border = new DynamicVectorClass<Cell>;
    border->Set_Growth_Step((int)((unsigned)LocalHeight * (unsigned)LocalWidth));
    int size = (int)((unsigned)unknown_map_cell_side_84e804 * (unsigned)unknown_map_cell_side_84e804);
    for (int index = 0; index < size; ++index) {
        Cell position = unknown_map_cells_a70a24[index].Position;
        if ((position.X != 0 || position.Y != 0) && Read_Generation_Region(position) == region) {
            bool boundary = false;
            for (int facing = 0; facing < 8; ++facing) {
                Cell adjacent = CellClass::Adjacent_Cell(position, facing);
                if (In_Zone_Map_Bounds(adjacent) && Read_Generation_Region(adjacent) != region) {
                    boundary = true;
                    break;
                }
            }
            if (boundary) {
                border->Add(position);
            }
        }
    }
    return border;
}
