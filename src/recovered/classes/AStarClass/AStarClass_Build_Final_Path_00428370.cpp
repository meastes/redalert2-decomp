// AStarClass::Build_Final_Path() (0x00428370): the path from the
// search's start to a node.
#include "shared/path_estimator.hpp"
#include "shared/cell.hpp"
#include "shared/crt_memory.hpp"
// @ref ?unknown_path_levels_84c858@@3PAHA=0084c858
// @ref _?path@?1??Build_Final_Path@AStarClass@@QAEPAUUnknownPathType@@PAUUnknownPathNode@@PAH@Z@4U3@A=0084c810
// @ref _?$S43@?1??Build_Final_Path@AStarClass@@QAEPAUUnknownPathType@@PAUUnknownPathNode@@PAH@Z@4EA=0084e7d4
// @ref _$E44=004284d0

PathStruct *AStarClass::Build_Final_Path(RegularOpenNode *node, int *buffer)
{
    static PathStruct path;
    path.Cost = node->unknown_08;
    path.Length = node->unknown_0c;
    path.Overlap = 0;
    path.LastOverlap.Set(0, 0);
    path.Command = buffer;
    path.Height = unknown_path_levels_84c858;
    RegularNode *entry = node->unknown_00;
    RegularNode *parent = entry->unknown_08;
    for (int index = node->unknown_0c - 2; index >= 0; index--) {
        if (parent) {
            unknown_path_levels_84c858[index] = parent->unknown_04;
            buffer[index] = Facing_Between((*entry->unknown_00)->MapCoords, (*parent->unknown_00)->MapCoords);
        }
        entry = entry->unknown_08;
        parent = parent->unknown_08;
    }
    buffer[node->unknown_0c - 1] = -1;
    path.Start = (*entry->unknown_00)->MapCoords;
    if (path.Cost == 0) {
        path.Cost = 1;
    }
    return &path;
}
