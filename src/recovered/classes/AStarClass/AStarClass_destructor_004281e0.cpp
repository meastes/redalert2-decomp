// AStarClass::~AStarClass() (0x004281E0)
// Frees the estimator's blocks and tables; the three vectors at +0x74 are
// destroyed as members.
#ifndef VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#endif
#ifndef VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#endif
#include "shared/path_estimator.hpp"
// @ref ??_7?$VectorClass@I@@6B@=0079c4fc

AStarClass::~AStarClass()
{
    UnknownPathEstimatorBlock *block = RegularQueue;
    if (block) {
        delete block->unknown_08;
        delete block;
    }
    RegularQueue = 0;
    block = HierQueue;
    if (block) {
        delete block->unknown_08;
        delete block;
    }
    HierQueue = 0;
    if (RegularOpenNodes) {
        delete RegularOpenNodes;
    }
    RegularOpenNodes = 0;
    if (RegularNodes) {
        delete RegularNodes;
    }
    RegularNodes = 0;
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
    for (int index = 0; index < 3; index++) {
        if (HierOnPath[index]) {
            delete HierOnPath[index];
            HierOnPath[index] = 0;
        }
        if (HierOpened[index]) {
            delete HierOpened[index];
            HierOpened[index] = 0;
        }
        if (HierCosts[index]) {
            delete HierCosts[index];
            HierCosts[index] = 0;
        }
    }
    delete HierNodePool;
}
