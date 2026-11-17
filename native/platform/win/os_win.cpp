// SPDX-License-Identifier: GPL-3.0-or-later
// Windows implementations of the port's operating-system services
// (port/src/os.hpp; os_apple.cpp is the macOS/iOS file). x86_64 Windows build
// (README "Windows build").
#include <windows.h>

#include "../../port/src/os.hpp"

#include <stdio.h>
#include <string.h>

namespace ra2port {
namespace os {

uint64_t monotonic_ns()
{
    static LARGE_INTEGER frequency;
    if (frequency.QuadPart == 0) QueryPerformanceFrequency(&frequency);
    LARGE_INTEGER now;
    QueryPerformanceCounter(&now);
    uint64_t f = (uint64_t)frequency.QuadPart;
    uint64_t t = (uint64_t)now.QuadPart;
    return t / f * 1000000000ull + t % f * 1000000000ull / f;
}

uint64_t thread_id()
{
    return GetCurrentThreadId();
}

uint64_t thread_id(pthread_t thread)
{
    return GetThreadId(pthread_gethandle(thread));
}

FileTimes file_times(const struct stat &st)
{
    FileTimes t = {};
    t.accessed.tv_sec = st.st_atime;
    t.modified.tv_sec = st.st_mtime;
    t.created.tv_sec = st.st_ctime;   // the creation time on Windows
    return t;
}

bool volume(const char *path, Volume *out)
{
    // The free space is the folder's own (GetDiskFreeSpaceEx takes any folder); the
    // volume's name, serial and type where the system reports them (a mapped or
    // network drive may not).
    char full[MAX_PATH], root[MAX_PATH];
    if (!GetFullPathNameA(path, sizeof full, full, nullptr)) return false;
    ULARGE_INTEGER available, total, free_total;
    if (!GetDiskFreeSpaceExA(full, &available, &total, &free_total)) return false;
    if (!GetVolumePathNameA(full, root, sizeof root)) snprintf(root, sizeof root, "%.3s", full);
    DWORD serial = 0, max_component = 0, flags = 0;
    char type[MAX_PATH] = "";
    GetVolumeInformationA(root, nullptr, 0, &serial, &max_component, &flags, type, sizeof type);
    out->mount_point = root;
    out->serial = serial;
    out->type = type;
    out->free_bytes = available.QuadPart;
    out->total_bytes = total.QuadPart;
    return true;
}

uint64_t physical_memory()
{
    MEMORYSTATUSEX status;
    status.dwLength = sizeof status;
    return GlobalMemoryStatusEx(&status) ? status.ullTotalPhys : 0;
}

// The CONTEXT the exception filter passes (platform/win/posix_win.cpp sigaction).
// x86_64 has no link register: lr is the faulting instruction itself, the
// frame walk then follows rbp's saved-frame chain.
bool fault_registers(void *context, uintptr_t *pc, uintptr_t *lr, uintptr_t *fp)
{
    if (!context) return false;
    const CONTEXT *c = (const CONTEXT *)context;
    *pc = (uintptr_t)c->Rip;
    *lr = (uintptr_t)c->Rip;
    *fp = (uintptr_t)c->Rbp;
    return true;
}

}  // namespace os
}  // namespace ra2port
