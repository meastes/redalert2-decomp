// NON-EXACT DRAFT 0x616A50 FUN_00616a50
// entry 0x616A50; exact-match score 77.83 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout923 (build/class-work/Scout923/w/a2.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
#define RA2_COORD_EQUAL_MEMBER
// void __fastcall Planning_Unknown_616a50(TechnoClass *techno) (0x00616A50)
// When a member of the first node of the techno's path takes slot 275,
// every node of the loop (from LoopStart to the path's end) turns its
// members' kind-2 orders into kind 0x1D (PlanningNodeClass::unknown_611440).
#include "shared/planning.hpp"

void __fastcall Planning_Unknown_616a50(TechnoClass *techno)
{
    PlanningTokenClass *token = Planning_Token(techno);
    int count = token->Nodes.Count();
    int start = token->LoopStart;
    PlanningNodeClass *first = token->Node_At(count - count);
    int members = first->Members.Count();
    for (int i = 0; i < members; i++) {
        if (first->Members[i]->Owner->unknown_slot275()) {
            for (int index = start; index < count; index++) {
                token->Node_At(index)->unknown_611440();
            }
            return;
        }
    }
}
