// void __fastcall VQA_Movie_Pause(VQAMovie *movie) (0x005A1630)
// Pauses the movie's player, if any.
// @ref ?Pause_VQA@VQAClass@@QAEXXZ=0071d1a0
#include "shared/movie.hpp"

void __fastcall VQA_Movie_Pause(VQAMovie *movie)
{
    if (movie && movie->Player) {
        movie->Player->Pause_VQA();
    }
}
