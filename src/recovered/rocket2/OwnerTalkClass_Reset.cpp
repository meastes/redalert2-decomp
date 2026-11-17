// void OwnerTalkClass::Reset_0063f070() -- 0x0063F070
// Purpose: frees and removes the first connection until the list is empty,
// then clears +0x18 and the router process handle (+0x1C).
// Assumptions: name hypothesis (ra2-decomp YR twin OwnerTalkClass::Reset).
#define VECTOR_IMPLEMENT_ALL
#include "shared/vector_class.hpp"
#include "shared/owner_talk.hpp"
// @ref ??3@YAXPAX@Z=00781d3b

void OwnerTalkClass::Reset_0063f070()
{
    while (Connections.Count() != 0) {
        delete Connections[0];
        Connections.Delete(0);
    }
    unknown_18 = false;
    RouterProcess = 0;
}
