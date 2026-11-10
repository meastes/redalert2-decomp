// VQMovieHandle::VQMovieHandle(VQAMovie *movie) (0x005A17B0)
// Wraps an opened VQA movie and takes its frame size from the player.
// @ref ??_7VQMovieHandle@@6B@=007a6624
// @ref ?Get_VQA_Width@VQAClass@@QAEHXZ=0071d300
// @ref ?Get_VQA_Height@VQAClass@@QAEHXZ=0071d310
#include "shared/movie.hpp"

VQMovieHandle::VQMovieHandle(VQAMovie *movie) :
    Finished(false)
{
    Movie = movie;
    Width = Movie->Player->Get_VQA_Width();
    Height = Movie->Player->Get_VQA_Height();
}
