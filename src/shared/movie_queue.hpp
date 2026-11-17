// The radar's queued movies (0x005A0840): MovieHandles opened on the sidebar surface and played in the radar's
// movie mode (shared/movie.hpp has the handle types).
#ifndef RA2_SHARED_MOVIE_QUEUE_HPP
#define RA2_SHARED_MOVIE_QUEUE_HPP

#include "shared/vector_class.hpp"

class MovieHandle;
// The movies queued for the radar (0x00AF8FD8; mode 3 plays the first).
extern DynamicVectorClass<MovieHandle *> IngameVQ;
// Opens a movie on the sidebar surface at the radar's corner, sets the end message (0x005A08F0) and queues it
// (0x005A0840; the flag lands in the handle's +0x04; nothing happens in a game).
void __fastcall Play_Ingame_Movie(char const *name, bool flag);
// The message a finished queued movie posts (0x005A08F0; the argument is the movie handle).
void __fastcall Ingame_Movie_Subtitle(int data);
// The radar view's position (0x008336CC, 0x008336D0; hypothesis).
extern int SidebarMovieX;
extern int SidebarMovieY;

#endif
