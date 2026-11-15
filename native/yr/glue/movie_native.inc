
// ---------------------------------------------------------------------------------------------
// NATIVE NAME BINDINGS (lane Y3), appended to the movie unit. The tree defines the movie entry
// points as Play_Movie (matched) and calls two of them under other names and argument types:
// Game::PlayMovie (0x005BED40, Play_Movie with the full-screen flag; init.cpp passes ints) and
// FUN_005BF260 (0x005BF260, Play_Movie by VQType; scenario.cpp declares it with ints, taction.cpp
// with the enums). Each forwards to the one definition at that address.
// Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
struct Game { static void __fastcall PlayMovie(const char *name, int theme, int clrscrn_after, int stretch, int clrscrn_before, int fullscreen); };

void __fastcall Game::PlayMovie(const char *name, int theme, int clrscrn_after, int stretch, int clrscrn_before, int fullscreen)
{
	Play_Movie(name, (ThemeType)theme, clrscrn_after != 0, stretch != 0, clrscrn_before != 0, fullscreen != 0);
}

void __fastcall FUN_005BF260(VQType movie, ThemeType theme, bool clrscrn_after, bool stretch, bool clrscrn_before)
{
	Play_Movie(movie, theme, clrscrn_after, stretch, clrscrn_before);
}

void __fastcall FUN_005BF260(int movie, int theme, bool clrscrn_after, bool stretch, bool clrscrn_before)
{
	Play_Movie((VQType)movie, (ThemeType)theme, clrscrn_after, stretch, clrscrn_before);
}
