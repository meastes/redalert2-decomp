// The movie subtitle manager unit (0x0069B170-0x0069C3AF; RA2 only): VQAMovieCompanion shows a movie's
// subtitles (shared/subtitle.hpp) read from subtitle.txt by SubTitleParser.
// Its first line: the dynamic initializer of shared/unknown_static_rect.hpp's file-local constant (0x0069B170).
#define VQA_MOVIE_COMPANION_UNIT
#define VECTOR_IMPLEMENT_ALL
#include "shared/unknown_static_rect.hpp"
#include "shared/movie.hpp"
#include "shared/subtitle.hpp"
#include "shared/vector_class.hpp"
#include "shared/file_straw.hpp"
#include "shared/text_print.hpp"
#include "shared/hicolor.hpp"
#include "shared/crt_string.hpp"
#include "shared/debug_printf.hpp"

