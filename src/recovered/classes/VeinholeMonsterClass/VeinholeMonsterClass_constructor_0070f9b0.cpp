// VeinholeMonsterClass::VeinholeMonsterClass(Cell const *cell) (0x0070F9B0)
// A veinhole monster at the cell. Only where one may spawn (Can_Monster_Go_Here)
// does it join VeinholeMonsters, sink and retile its 3x3 cells (unless 0x00A40774 is
// set), start its stage timer, build the node block, heap and tables, move
// onto the map at the cell's center and register in the techno index.
#include "shared/veinhole.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/techno_index.hpp"
#include "shared/smudge.hpp"
#include "shared/tileset_index.hpp"
#include "shared/terrain_type.hpp"
// @ref ??0ObjectClass@@QAE@XZ=005d22e0
// @ref ?Create_ID@AbstractClass@@UAGXXZ=0040ffd0
// @ref ?Fetch_ID@AbstractClass@@UAGJXZ=0040ffc0
// @ref ?Rules@@3PAVRulesClass@@A=00839848
// @ref ?Frame@@3HA=00a40d2c
// @ref ??_7VeinholeMonsterClass@@6BIPersistStream@@@=007ae7a0
// @ref ??_7VeinholeMonsterClass@@6BIRTTITypeInfo@@@=007ae784
// @ref ??_7VeinholeMonsterClass@@6BINoticeSink@@@=007ae77c
// @ref ??_7VeinholeMonsterClass@@6BINoticeSource@@@=007ae774
// @ref ?unknown_cells_428ac0@@YIHXZ=00428ac0
// @ref ?unknown_veinhole_buffer_ad07a0@@3PAEA=00ad07a0
// @ref ?unknown_veinhole_tiles_7f9508@@3QAY02$$CBEA=007f9508
// @ref ?unknown_a40774@@3HA=00a40774
// @ref ?unknown_tileset_a6e018@@3HA=00a6e018
// @ref ?unknown_object_index_abff58@@3V?$IndexClass@JPAVAbstractClass@@@@A=00abff58
// @ref ?Increase_Table_Size@?$IndexClass@JPAVAbstractClass@@@@QAE_NH@Z=0041b900

// The tile offsets (within the veinhole tile set; 0: none) of the 3x3 cells
// under a new monster (0x007F9508).
extern unsigned char const unknown_veinhole_tiles_7f9508[3][3];

VeinholeMonsterClass::VeinholeMonsterClass(::Cell const *cell) :
    GrowthCount(0),
    GrowthQueue(0),
    GrowthNodes(0),
    GrowthTimer(Rules->VeinholeGrowthRate),
    GrowthState(0),
    CurrentState(0),
    DesiredState(0),
    CellID(*cell),
    ShapeFrame(0),
    IsDead(false),
    IsToPuffGas(false),
    VeinCount(0)
{
    AbstractClass::Create_ID();
    if (!Can_Monster_Go_Here(*cell)) {
        return;
    }
    VeinholeMonsters.Add(this);
    if (unknown_a40774 == 0) {
        for (int y = -1; y <= 1; y++) {
            for (int x = -1; x <= 1; x++) {
                CellClass *spot = &Map[*cell + ::Cell(x, y)];
                spot->unknown_11c = unknown_veinhole_tiles_7f9508[y + 1][x + 1];
                if (spot->unknown_11c > 0) {
                    spot->IsoTileIndex = spot->unknown_11c + unknown_tileset_a6e018 - 1;
                } else {
                    spot->IsoTileIndex = 0;
                }
                spot->unknown_11a = 0;
                spot->unknown_11b--;
            }
        }
    }
    Control.Set_Rate(4);
    Control.unknown_14 = 0;
    Control.Value = -1;
    GrowthNodes = new UnknownVeinNode[Rules_Vein_Node_Count()];
    GrowthQueue = new UnknownVeinHeap(Rules_Vein_Node_Count());
    GrowthQueue->Clear();
    if (unknown_veinhole_buffer_ad07a0 == 0) {
        unknown_veinhole_buffer_ad07a0 = new unsigned char[unknown_cells_428ac0()];
    }
    GrowthState = new unsigned char[unknown_cells_428ac0()];
    CoordStruct coord = CoordStruct(cell->X * 256 + 128, cell->Y * 256 + 128, 0);
    coord.Z = Map.Get_Height_GL(coord);
    ObjectClass::unknown_slot53(&coord, 0);
    Strength = Rules->unknown_cb4->MaxStrength;
    unknown_object_index_abff58.Add_Index(Fetch_ID(), this);
}
