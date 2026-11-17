// FLAT PRELUDE (first RA2_SDK_HEADERS landing, Scout1201 2026-11-09): the system headers a converted unit sees,
// in the order the original's release build reaches them (mischa85 findpath2.cpp, NDEBUG, clang -H; the TibSun
// archive's headers carry the same includes). Shared by every converted unit; included from shared/always.hpp
// (always.h is the first include of every original unit).
//
// Why flat: in the original each of these comes from its own header (rect.h:40 <cstddef>, fastmath.h:16 <cmath>,
// sun.h:19 <cstring>, irtti.h:12 <comdef.h> via abstract.h, win.h via keyboard.h, vector.h:54-57 <cstdlib>
// <new.h>). Our src/shared graph cannot yet express that order: our first COM/Win32 user (gscreen.hpp through
// com_persist.hpp/win32_api.hpp) comes before our counterparts of abstract.h and keyboard.h. When the wrappers
// can be placed at their counterparts, this file goes and each include moves to its own header.
// always.h's crtdbg.h/cstdlib/malloc.h block is _DEBUG/STEVES_NEW_CATCHER only and is not reached in release.
#ifndef RA2_SHARED_SDK_PRELUDE_HPP
#define RA2_SHARED_SDK_PRELUDE_HPP
#ifdef RA2_SDK_HEADERS
#include <cstddef>
#include <cmath>
#include <cstring>
#include "shared/irtti.hpp"
#include "shared/win.hpp"
#include <cstdlib>
#include <new.h>
#endif
#endif
