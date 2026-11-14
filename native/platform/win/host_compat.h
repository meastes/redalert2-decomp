// SPDX-License-Identifier: GPL-3.0-or-later
// Forced include for the SDL3 host's sources on the x86_64 Windows build
// (platform/sdl/*.cpp; README "Windows build"): the port's layer (port_compat.h)
// and one name SDL and the port both declare. On Windows <SDL3/SDL_system.h>
// declares the system's MSG (for SDL_SetWindowsMessageHook, which the host does
// not use) and the port's win32_port.hpp defines the game's own MSG; SDL's is read
// here first under another name, so the host's translation units see the port's.
#ifndef RA2_HOST_COMPAT_WIN_H
#define RA2_HOST_COMPAT_WIN_H
#include "port_compat.h"
#ifdef __cplusplus
#define MSG RA2_SDL_SYSTEM_MSG
#include <SDL3/SDL_system.h>
#undef MSG
#endif
#endif
