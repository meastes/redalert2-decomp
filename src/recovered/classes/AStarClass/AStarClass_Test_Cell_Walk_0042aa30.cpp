// AStarClass::Test_Cell_Walk() (0x0042AA30): the estimated
// length of a walk between two cells (Tiberian Sun's Test_Cell_Walk;
// hypothesis).
// @ref _unknown_geometry_k=0084e7a8
// @ref ?Find_Path_Hierarchical@AStarClass@@QAE_NABVCell@@0HPAVFootClass@@@Z=00429b60
// @ref ?Get_Bridge_Zone_Connection_Cell@MapClass@@QAE?AVCell@@PAVCellClass@@_N@Z=00564f70
// @ref ?Find_Bridge_End_Cell_For_Subzone@MapClass@@QAE?AVCell@@ABV2@HI@Z=00565610
#include "shared/path_estimator.hpp"
#include "shared/foot.hpp"
#include "shared/techno_type.hpp"
#include "shared/map_view.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/crt_memory.hpp"

template <class T> T max(T a, T b)
{
    return a > b ? a : b;
}

int AStarClass::Test_Cell_Walk(Cell const &from, Cell const &to, FootClass *foot, bool from_bridge, bool to_bridge, int arg6)
{
    IsHSEnabled = true;
    Clear();
    for (int index = 0; index < 3; index++) {
        HierBannedEdges[index].Clear();
    }
    CellClass *from_ptr = &Map[from];
    CellClass *to_ptr = &Map[to];
    Cell hs_from = Map.Get_Bridge_Zone_Connection_Cell(from_ptr, from_bridge);
    Cell hs_to = Map.Get_Bridge_Zone_Connection_Cell(to_ptr, to_bridge);
    if (arg6 == -1) {
        if (foot) {
            arg6 = foot->unknown_slot32()->MovementZone;
        } else {
            arg6 = 0;
        }
    }
    if (Find_Path_Hierarchical(hs_from, hs_to, arg6, foot)) {
        int radius = max(abs(from.X - to.X), abs(from.Y - to.Y));
        int node_count = HierSubzonePathCount[0];
        int distance = 2 * node_count - 2;
        bool add_bridge_dist = true;
        if (to_bridge && from_bridge) {
            int to_conn = Map.Zone_Connection_Index(to, 3, 0);
            int from_conn = Map.Zone_Connection_Index(from, 3, 0);
            if (to_conn == from_conn && to_conn != -1) {
                add_bridge_dist = false;
            }
        }
        if (add_bridge_dist) {
            if (to_bridge) {
                Cell bridge_end = unknown_geometry_k;
                if (node_count >= 4) {
                    bridge_end = Map.Find_Bridge_End_Cell_For_Subzone(to, 0, HierSubzonePath[0][node_count - 2]);
                }
                if (node_count < 4 || bridge_end == unknown_geometry_k) {
                    bridge_end = Map.Find_Bridge_End_Cell_For_Subzone(to, 0, HierSubzonePath[0][node_count - 1]);
                }
                if (bridge_end == unknown_geometry_k) {
                    bridge_end = hs_to;
                }
                if (bridge_end != unknown_geometry_k) {
                    distance += max(abs(to.X - bridge_end.X), abs(to.Y - bridge_end.Y));
                }
            }
            if (from_bridge) {
                Cell bridge_end = unknown_geometry_k;
                if (node_count >= 4) {
                    bridge_end = Map.Find_Bridge_End_Cell_For_Subzone(from, 0, HierSubzonePath[0][1]);
                }
                if (node_count < 4 || bridge_end == unknown_geometry_k) {
                    bridge_end = Map.Find_Bridge_End_Cell_For_Subzone(from, 0, HierSubzonePath[0][0]);
                }
                if (bridge_end == unknown_geometry_k) {
                    bridge_end = hs_from;
                }
                if (bridge_end != unknown_geometry_k) {
                    distance += max(abs(from.X - bridge_end.X), abs(from.Y - bridge_end.Y));
                }
            }
            if (distance > radius) {
                return distance;
            }
        }
        return radius;
    }
    return 0x7FFFFFFF;
}
