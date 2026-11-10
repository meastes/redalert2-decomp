// Derive a path start after the already queued steps and ask the shared estimator.
#include "shared/foot.hpp"
PathStruct *FootClass::Unknown_004BB350(Cell *dest,int *moves,int maximum,int threshold,int path_offset,int avoidance)
{
    Cell source=Cell(unknown_slot19(0));
    if (!moves) return 0;
    source=AStarClass::Follow_Path(source,path_offset,Path);
    return Search.Find_Path(source,*dest,this,moves,-1,-1,avoidance);
}
