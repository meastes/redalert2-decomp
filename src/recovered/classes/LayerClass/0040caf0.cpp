// LayerClass_Init (0x0040CAF0, LayerClass slot 8): TibSun layer.h LayerClass::Init() { Clear(); }, a tail call through slot 3
#include "shared/ghidra_types.hpp"

void __fastcall LayerClass_Init(int *layer)
{
                                        /* WARNING: Could not recover jumptable at 0x0040caf2. Too many branches */
                                        /* WARNING: Treating indirect jump as call */
    (**(code **)(*layer + 0xc))();
    return;
}

