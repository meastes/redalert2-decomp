#include "shared/veinhole.hpp"
// @ref ?unknown_cells_428ac0@@YIHXZ=00428ac0
// @ref ?unknown_veinhole_buffer_ad07a0@@3PAEA=00ad07a0

bool __fastcall VeinholeMonsterClass::Save_All(IStream *stream)
{
    int monster_count = VeinholeMonsters.Count();
    if (FAILED(stream->Write(&monster_count, sizeof(monster_count), 0))) {
        return false;
    }

    int cell_count = unknown_cells_428ac0();
    if (FAILED(stream->Write(unknown_veinhole_buffer_ad07a0, cell_count, 0))) {
        return false;
    }

    for (int i = 0; i < monster_count; i++) {
        LONG_PTR id = (LONG_PTR)VeinholeMonsters[i];
        if (FAILED(stream->Write(&id, sizeof(id), 0))) {
            return false;
        }

        if (FAILED(stream->Write(VeinholeMonsters[i], sizeof(VeinholeMonsterClass), 0))) {
            return false;
        }

        if (FAILED(stream->Write(VeinholeMonsters[i]->GrowthState, cell_count, 0))) {
            return false;
        }

        if (FAILED(stream->Write(VeinholeMonsters[i]->GrowthNodes, sizeof(UnknownVeinNode) * Rules->unknown_d4, 0))) {
            return false;
        }

        VeinholeMonsters[i]->GrowthQueue->Save(stream, VeinholeMonsters[i]->GrowthNodes);
    }

    return true;
}
