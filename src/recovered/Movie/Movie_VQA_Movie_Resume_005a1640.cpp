// void __fastcall VQA_Movie_Resume(VQAMovie *movie) (0x005A1640)
// Resumes the movie's player, if any.
// @ref ?Resume_VQA@VQAClass@@QAEXXZ=0071d1e0
#include "shared/movie.hpp"

void __fastcall VQA_Movie_Resume(VQAMovie *movie)
{
    if (movie && movie->Player) {
        movie->Player->Resume_VQA();
    }
}
