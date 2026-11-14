// intro.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/intro.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/special.hpp"
#include "shared/movie_list.hpp"
#include "shared/play_movie.hpp"


// Header-static dynamic initializers of the unit at 0x0051ECF0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x0051ECF0-0x0051EED6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0051ED20, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0051ecf0.cpp; no source of its own.


// Dynamic initializer (0x0051ED40, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0051ecf0.cpp; no source of its own.


// Dynamic initializer (0x0051ED60, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0051ecf0.cpp; no source of its own.


// Dynamic initializer (0x0051ED80, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0051ecf0.cpp; no source of its own.


// Dynamic initializer (0x0051EDA0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0051ecf0.cpp; no source of its own.


// Dynamic initializer (0x0051EDC0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0051ecf0.cpp; no source of its own.


// Dynamic initializer (0x0051EE00, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0051ecf0.cpp; no source of its own.


// Dynamic initializer (0x0051EE30, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0051ecf0.cpp; no source of its own.


// Dynamic initializer (0x0051EE60, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0051ecf0.cpp; no source of its own.


// Dynamic initializer (0x0051EE90, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0051ecf0.cpp; no source of its own.


// Dynamic initializer (0x0051EEA0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0051ecf0.cpp; no source of its own.


// Dynamic initializer (0x0051EEC0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_0051ecf0.cpp; no source of its own.


// void Choose_Side() (0x0051EEE0)
// Plays the intro movie when the options ask for it (TibSun intro.cpp; in RA2
// all this does is play a movie).

void Choose_Side()
{
    if (unknown_special_a40920.IsFromInstall) {
        Play_Movie(unknown_name_index_482e60("INTRO"), -1, false, true, true);
    }
}
