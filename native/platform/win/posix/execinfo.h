// SPDX-License-Identifier: GPL-3.0-or-later
// <execinfo.h> for the port on the Windows build (diagnostics: fault and stall
// backtraces). backtrace is RtlCaptureStackBackTrace; backtrace_symbols_fd
// writes each frame as module+offset (platform/win/posix_win.cpp).
#ifndef RA2_POSIX_EXECINFO_H
#define RA2_POSIX_EXECINFO_H
#ifdef __cplusplus
extern "C" {
#endif
int ra2_win_backtrace(void **frames, int count);
void ra2_win_backtrace_symbols_fd(void *const *frames, int count, int fd);
#define backtrace ra2_win_backtrace
#define backtrace_symbols_fd ra2_win_backtrace_symbols_fd
#ifdef __cplusplus
}
#endif
#endif
