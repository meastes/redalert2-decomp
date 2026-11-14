// SPDX-License-Identifier: GPL-3.0-or-later
// <fnmatch.h> for the port on the Windows build (FindFirstFileA's patterns,
// kernel32.cpp): '*', '?' and '[...]' sets, FNM_CASEFOLD (platform/win/posix_win.cpp).
#ifndef RA2_POSIX_FNMATCH_H
#define RA2_POSIX_FNMATCH_H
#ifdef __cplusplus
extern "C" {
#endif
#define FNM_NOMATCH 1
#define FNM_NOESCAPE 0x01
#define FNM_PATHNAME 0x02
#define FNM_PERIOD 0x04
#define FNM_CASEFOLD 0x10
int ra2_win_fnmatch(const char *pattern, const char *name, int flags);
#define fnmatch ra2_win_fnmatch
#ifdef __cplusplus
}
#endif
#endif
