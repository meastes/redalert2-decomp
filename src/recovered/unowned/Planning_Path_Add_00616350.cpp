#define RA2_COORD_EQUAL_MEMBER
// void __fastcall Planning_Path_Add(TechnoClass *techno, PlanningNodeClass *node) (0x00616350)
// Gives the techno a planning token if it has none, then closes its path
// into a loop at the node when the path holds it already, else appends it.
// Planning_Close_Loop is defined here, `inline`: this function expands it, and so
// does Planning_Close_Loop_006164e0 (0x006164E0, ra2-decomp's FUN_00639740 form),
// the function Planning_Execute 0x00614BA0 calls.
#include "shared/planning.hpp"
// @ref ??0?$VectorClass@PAVPlanningNodeClass@@@@QAE@IPBQAVPlanningNodeClass@@@Z=0061bd70
// @ref ?Planning_Path_Append@@YIXPAVTechnoClass@@PAVPlanningNodeClass@@@Z=006165a0
// @ref ?unknown_flag_a76788@@3_NA=00a76788
// @ref ?Planning_Unknown_616a50@@YIXPAVTechnoClass@@@Z=00616a50

// void __fastcall Planning_Close_Loop(TechnoClass *techno, PlanningNodeClass *node) (inline; expanded at
// 0x00616350 and 0x006164E0)
// Makes the techno's path loop back to a node: the loop runs from the node's
// index to the path's end. The first time, the members of the path's last
// node owned by a human house get slot 81; then 0x00616A50 runs.
inline void __fastcall Planning_Close_Loop(TechnoClass *techno, PlanningNodeClass *node)
{
    int start = Token_Node_Index(Planning_Token(techno), node);
    PlanningTokenClass *token = Planning_Token(techno);
    token->LoopLength = token->Nodes.Count() - start;
    token->LoopStart = start;
    token->unknown_98 = true;
    if (!unknown_flag_a76788) {
        if (techno->House->Unknown_004F52B0()) {
            unknown_flag_a76788 = true;
            PlanningNodeClass *last = token->Last_Node();
            int count = last->Members.Count();
            for (int i = 0; i < count; i++) {
                TechnoClass *member = last->Members[i]->Owner;
                if (member->unknown_77) {
                    member->unknown_slot81();
                }
            }
        }
        Planning_Unknown_616a50(techno);
    }
    node->unknown_611770();
}


void __fastcall Planning_Path_Add(TechnoClass *techno, PlanningNodeClass *node)
{
    PlanningTokenClass *token = Planning_Token(techno);
    if (!token) {
        token = Planning_New_Token(techno);
    }
    if (token->Node_Index(node) != -1) {
        Planning_Close_Loop(techno, node);
    } else {
        Planning_Path_Append(techno, node);
    }
}
