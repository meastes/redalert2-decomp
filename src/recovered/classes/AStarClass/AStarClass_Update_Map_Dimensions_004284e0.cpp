// AStarClass::Update_Map_Dimensions() (0x004284E0): sizes the cell marks
// and the facing offsets for the map rectangle.
#include "shared/path_estimator.hpp"
// Shared map-grid dimension; this pathfinding routine needs no MapRegionClass layout.
extern int unknown_map_cell_side_84e804;

void AStarClass::Update_Map_Dimensions(Rect const &size)
{
    if (RegularVisited) {
        delete RegularVisited;
        RegularVisited = 0;
    }
    if (RegularBridgeVisited) {
        delete RegularBridgeVisited;
        RegularBridgeVisited = 0;
    }
    if (RegularMovementCosts) {
        delete RegularMovementCosts;
        RegularMovementCosts = 0;
    }
    if (RegularBridgeMovementCosts) {
        delete RegularBridgeMovementCosts;
        RegularBridgeMovementCosts = 0;
    }
    unknown_map_cell_side_84e804 = size.Width + size.Height + 1;
    int count = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804;
    RegularBridgeVisited = new int[count];
    RegularVisited = new int[count];
    RegularMovementCosts = new float[count];
    RegularBridgeMovementCosts = new float[count];
    unknown_path_facing_offsets_84c838[0] = -unknown_map_cell_side_84e804;
    unknown_path_facing_offsets_84c838[1] = 1 - unknown_map_cell_side_84e804;
    unknown_path_facing_offsets_84c838[2] = 1;
    unknown_path_facing_offsets_84c838[3] = unknown_map_cell_side_84e804 + 1;
    unknown_path_facing_offsets_84c838[4] = unknown_map_cell_side_84e804;
    unknown_path_facing_offsets_84c838[5] = unknown_map_cell_side_84e804 - 1;
    unknown_path_facing_offsets_84c838[6] = -1;
    unknown_path_facing_offsets_84c838[7] = -1 - unknown_map_cell_side_84e804;
}
