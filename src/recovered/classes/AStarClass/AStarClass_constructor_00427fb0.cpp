// AStarClass::AStarClass() (0x00427FB0)
#ifndef VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#endif
#ifndef VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#endif
#include "shared/path_estimator.hpp"
// @ref ??0?$VectorClass@I@@QAE@IPBI@Z=0042b510
// @ref ??_7?$DynamicVectorClass@I@@6B@=0079c4dc

AStarClass::AStarClass()
    : unknown_00(false), IsAvoidBridges(false), unknown_02(false), IsAvoidPathCollision(true), MovementCostMultiplier(1.0f), UseLocomotorEnterCheck(true),
      RegularVisited(0), RegularBridgeVisited(0), RegularBridgeMovementCosts(0), RegularMovementCosts(0), UniqueID(-1), ObjectSpeed(-1), IsHSEnabled(true),
      Avoidance(0), HierNodeIndex(-1), HierLastNodeCell(0, 0)
{
    RegularQueue = new UnknownPathEstimatorBlock(0x10000);
    HierQueue = new UnknownPathEstimatorBlock(0x2710);
    RegularOpenNodes = new RegularOpenNodePool;
    RegularNodes = new RegularNodePool;
    RegularNodes->count = 0;
    RegularOpenNodes->count = 0;
    RegularQueue->Clear();
    HierQueue->Clear();
    for (int index = 0; index < 3; index++) {
        HierOnPath[index] = 0;
        HierOpened[index] = 0;
        HierCosts[index] = 0;
        HierBannedEdges[index].Clear();
        for (int slot = 0; slot < 500; slot++) {
            HierSubzonePath[index][slot] = 0;
        }
        HierSubzonePathCount[index] = 0;
    }
    HierNodePool = new char[0x27100];
}
