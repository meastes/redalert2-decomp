// int __cdecl VQA_Unknown_0071D640(VQAHandle *handle, int unknown2, int unknown3, int unknown4) (0x0071D640)
// Library callback: forwards to the owning player's 0x0071D4B0.
#include "shared/movie.hpp"
// @ref ?MixFileHandler@VQAClass@@QAEHHHH@Z=0071d4b0
// @ref ?CacheHandler@VQAClass@@QAEHHHH@Z=0071ca00

int __cdecl VQA_Unknown_0071D640(VQAHandle *handle, int unknown2, int unknown3, int unknown4)
{
    VQAClass *player = handle->Player;
    return player->MixFileHandler(unknown2, unknown3, unknown4);
}
