// Behaviour rows (Scout1133, native M16; NOT EXACT, native/behaviour_rewrites.json).
// The radar's transmission (mode 4) helpers RadarClass::Update_Transmission calls
// (pass-6a decompiles; Allied 1's opening transmission).
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/vox_class.hpp"
#include "shared/vox_speech.hpp"
extern DynamicVectorClass<VoxClass *> unknown_VoxClass_list_00ad0930;   // 0x00AD0930
void Unknown_00716940();                       // shared/radar.hpp
bool __fastcall Unknown_00716690(int voice);   // shared/radar.hpp
// 0x00716940: blocks the speech queue once more (Speech_Hold's twin for the
// queue counter 0x00AD0878, counted down by 0x00716950).
void Unknown_00716940()
{
    unknown_vox_ad0878 = unknown_vox_ad0878 + 1;
}
// 0x00716690: whether the VoxClass at an index is neither playing nor queued
// (State 2); true for an index out of range.
bool __fastcall Unknown_00716690(int voice)
{
    if (voice > -1 && voice < unknown_VoxClass_list_00ad0930.Count()) {
        return unknown_VoxClass_list_00ad0930[voice]->State == 2;
    }
    return true;
}
