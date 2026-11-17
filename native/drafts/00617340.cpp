// NON-EXACT DRAFT 0x617340 Planning_Left_Down_00617340
// entry 0x617340; exact-match score 38.31 (misses_index best, Scout923; text Scout779).
// plan "draft, verify". Origin: build/class-work/Scout779/keep/ld1_3831.cpp.
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by Scout1115
// (native/drafts.json). Not original bytes. Reached by the first left click on the map
// (ScrollClass 0x0066A400's WM_LBUTTONDOWN; native M13 interactive input).
// Native adaptation (Scout1115): HEAD's planning_mouse.hpp signature (int x, int y) instead of
// the draft's Point2D; joined to Planning_Left_Up 0x00617680's unit, whose light MouseClass
// and Map declarations it shares (the draft's own copies dropped). Marked NATIVE.
// @ref ?Hit_Marker@PlanningNodeClass@@QBE?AVRect@@XZ=00611360
// @ref ??_C@_0BN@GEHP@MSG?3PlanningModeInvalidNodeX?$AA@=007ebeec
// @ref ??_C@_0BL@OMHN@MSG?3PlanningModeNoAircraft?$AA@=007ebed0
// @ref ??_C@_0BM@JDCL@MSG?3PlanningModeNoBuildings?$AA@=007ebeb4
// @ref ??_C@_0BL@CGPI@MSG?3PlanningModeNoUnknowns?$AA@=007ebe98
bool __stdcall Planning_Left_Down_00617340(int x, int y)   // NATIVE: HEAD signature
{
    Point2D point(x, y);   // NATIVE: the draft's parameter
{
    if (!Planning_Is_Active()) {
        return false;
    }
    PlanningNodeClass *remembered = Planning_Get_a766f8();
    PlanningNodeClass *node = Planning_Node_At(point.X, point.Y);
    if (node != 0 && node != remembered) {
        Map.Set_Default_Mouse(0, true);
        Planning_Set_a766f8(node);
    } else {
        PlanningNodeClass *picked = Planning_Pick_Node(point.X, point.Y);
        if (picked != 0) {
            if (Planning_Selection_Node_State(picked) != -1) {
                Planning_Refuse("MSG:PlanningModeInvalidNodeX");   // behaviour fix (codex4 r546): the original key ends in X (0x007EBEEC)
                return true;
            }
        } else {
            int kind = Planning_Selection_Kind();
            if (kind != -1) {
                Planning_Refuse_Selection(kind);
                return true;
            }
        }
    }
    return remembered != 0 || node != 0;
}
}   // NATIVE: closes the adapted signature's block
