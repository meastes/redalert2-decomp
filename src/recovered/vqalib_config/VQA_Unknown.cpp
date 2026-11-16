// void __fastcall VQA_DefaultConfig(VQAConfig *config) -- 0x00777850
// Purpose: VQA_DefaultConfig (TS config.cpp): copies the library's default
// configuration (0x00801470, original data) into the caller's.
#include "shared/vqa_player.hpp"
// @ref ?_defaultconfig@@3UVQAConfig@@A=00801470

extern VQAConfig _defaultconfig;

void __fastcall VQA_DefaultConfig(VQAConfig *config)
{
    *config = _defaultconfig;
}
