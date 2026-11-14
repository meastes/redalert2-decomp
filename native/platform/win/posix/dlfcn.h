// SPDX-License-Identifier: GPL-3.0-or-later
// <dlfcn.h> for the port on the Windows build: dladdr names the module and its
// base for an address (no symbol names: a stripped PE has none), dlsym looks up
// an export of the executable (platform/win/posix_win.cpp). Diagnostics only.
#ifndef RA2_POSIX_DLFCN_H
#define RA2_POSIX_DLFCN_H
#ifdef __cplusplus
extern "C" {
#endif
typedef struct {
    const char *dli_fname;
    void *dli_fbase;
    const char *dli_sname;
    void *dli_saddr;
} Dl_info;
#define RTLD_DEFAULT ((void *)0)
int ra2_win_dladdr(const void *address, Dl_info *info);
void *ra2_win_dlsym(void *handle, const char *name);
#define dladdr ra2_win_dladdr
#define dlsym ra2_win_dlsym
#ifdef __cplusplus
}
#endif
#endif
