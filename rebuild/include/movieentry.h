// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 OpenTS contributors
#pragma once

struct MovieEntry {
		char const * Name;
		char const * Label;
		int Side;
};

extern MovieEntry IntroMovieEntry;
extern MovieEntry AlliedMovieList[];
extern MovieEntry SovietMovieList[];
