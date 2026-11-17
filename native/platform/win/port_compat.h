// SPDX-License-Identifier: GPL-3.0-or-later
// Forced include for the port (native/port/src) and the SDL host on the x86_64
// Windows build (llvm-mingw; README "Windows build"). The port is written to
// standard C++ and POSIX; mingw-w64 supplies most of POSIX (unistd.h, dirent.h,
// pthread.h through winpthreads, sys/stat.h, utime.h), and this header and the
// headers under platform/win/posix supply the rest on Win32 (implemented in
// platform/win/posix_win.cpp, the only file of this layer that includes
// <windows.h>). Nothing here changes what the port does on macOS or iOS.
//
// The port defines some VC6 C runtime names itself (crt.cpp: _open, _lseek,
// _filelength, ...) with the VC6 signatures the recovered code calls; the UCRT
// declares the same names. Their UCRT declarations are hidden from the port's
// translation units (renamed while the CRT headers are read), so the port's own
// definitions are the only ones it sees; at link time the port's objects come
// before the UCRT import libraries (README "Windows build": duplicate names).
#ifndef RA2_PORT_COMPAT_WIN_H
#define RA2_PORT_COMPAT_WIN_H
#if !defined(_WIN64) || !defined(__x86_64__)
#error "port_compat.h is for the x86_64 Windows build"
#endif

#define _USE_MATH_DEFINES 1         // M_PI (bink.cpp)
#define _POSIX_THREAD_SAFE_FUNCTIONS 1   // gmtime_r, localtime_r (kernel32.cpp)

// UCRT declarations the port's VC6-signature definitions replace (crt.cpp).
#define _lseek ra2_ucrt_lseek_hidden
#define _filelength ra2_ucrt_filelength_hidden
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#undef _lseek
#undef _filelength

#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <direct.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef long suseconds_t;           // struct timeval's tv_usec type (long on Win64)

// Environment, temporary folders, paths (the host's command line handling,
// platform/sdl/main.cpp and app_mode.hpp).
int ra2_win_setenv(const char *name, const char *value, int overwrite);
char *ra2_win_mkdtemp(char *path_template);
char *ra2_win_realpath(const char *path, char *resolved);
// rename() over an existing file replaces it, as POSIX does (MoveFileEx).
int ra2_win_rename(const char *from, const char *to);
// timegm (kernel32.cpp SystemTimeToFileTime) and gethostname (GetComputerNameA;
// Winsock's, through the BSD sockets layer: posix/ra2_bsd_socket.h).
#define timegm _mkgmtime
int ra2_bsd_gethostname(char *name, size_t length);
#ifndef RA2_WIN32_HEADERS             // not in the files that include <windows.h> (Winsock's own)
#define gethostname ra2_bsd_gethostname
#endif
#define lstat stat                     // no symbolic links to tell apart (app_mode.hpp)
#define setenv ra2_win_setenv
#define mkdtemp ra2_win_mkdtemp
#define realpath ra2_win_realpath
#define rename ra2_win_rename

// futimes(fd, times[2]): access and modification times of an open file.
int ra2_win_futimes(int fd, const struct timeval times[2]);
#define futimes ra2_win_futimes

// fcntl for the one use the port makes of it: O_NONBLOCK on a socket
// (wsock32.cpp WSAAsyncSelect). Files are never switched.
#define F_GETFL 3
#define F_SETFL 4
#ifndef O_NONBLOCK
#define O_NONBLOCK 0x4000
#endif
int ra2_win_fcntl(int fd, int command, ...);
#define fcntl ra2_win_fcntl

// Signals the port installs handlers for (diagnostics: --run-to fault summary,
// RA2_NATIVE_CRASH_TRACE). Windows raises SIGSEGV, SIGILL, SIGFPE and SIGABRT;
// SIGBUS and SIGUSR2 never arrive. sigaction is signal() plus a vectored
// exception handler that hands an access violation's address and CONTEXT to an
// SA_SIGINFO handler (os::fault_registers reads the CONTEXT).
#ifndef SIGBUS
#define SIGBUS 10
#endif
#ifndef SIGUSR2
#define SIGUSR2 12
#endif
#ifndef SIGTRAP
#define SIGTRAP 5
#endif
typedef struct { int si_signo; void *si_addr; } siginfo_t;
typedef unsigned int sigset_t;
#define SA_SIGINFO 0x40
struct sigaction {
    void (*sa_handler)(int);
    void (*sa_sigaction)(int, siginfo_t *, void *);
    sigset_t sa_mask;
    int sa_flags;
};
int ra2_win_sigaction(int sig, const struct sigaction *action, struct sigaction *old);
static inline int sigemptyset(sigset_t *set) { *set = 0; return 0; }
// The crash handler's alternate stack (app_mode.hpp): the exception filter runs on
// the faulting thread's stack, so there is nothing to set up.
#define SA_ONSTACK 0x08000000
typedef struct { void *ss_sp; size_t ss_size; int ss_flags; } stack_t;
static inline int sigaltstack(const stack_t *stack, stack_t *old) { (void)stack; (void)old; return 0; }
#define sigaction(s, a, o) ra2_win_sigaction((s), (a), (o))

#ifdef __cplusplus
}

// mkdir(path, mode): the POSIX two-argument form (the mode has no meaning here).
static inline int mkdir(const char *path, int mode) { (void)mode; return _mkdir(path); }

// std::basic_string<unsigned short> (storage.cpp's UTF-16 names): the generic
// character traits that libc++ removed (macOS's libc++ still supplies them).
#include <string>
namespace std {
template <> struct char_traits<unsigned short> {
    typedef unsigned short char_type;
    typedef unsigned int int_type;
    typedef streamoff off_type;
    typedef u16streampos pos_type;
    typedef mbstate_t state_type;
    static inline void assign(char_type &a, const char_type &b) noexcept { a = b; }
    static inline bool eq(char_type a, char_type b) noexcept { return a == b; }
    static inline bool lt(char_type a, char_type b) noexcept { return a < b; }
    static int compare(const char_type *a, const char_type *b, size_t n)
    {
        for (; n; --n, ++a, ++b) {
            if (*a < *b) return -1;
            if (*b < *a) return 1;
        }
        return 0;
    }
    static size_t length(const char_type *s) { size_t n = 0; while (s[n]) ++n; return n; }
    static const char_type *find(const char_type *s, size_t n, const char_type &c)
    {
        for (; n; --n, ++s) if (*s == c) return s;
        return nullptr;
    }
    static char_type *move(char_type *d, const char_type *s, size_t n) { return n ? (char_type *)memmove(d, s, n * 2) : d; }
    static char_type *copy(char_type *d, const char_type *s, size_t n) { return n ? (char_type *)memcpy(d, s, n * 2) : d; }
    static char_type *assign(char_type *d, size_t n, char_type c) { for (size_t i = 0; i < n; ++i) d[i] = c; return d; }
    static inline int_type not_eof(int_type c) noexcept { return c == eof() ? 0 : c; }
    static inline char_type to_char_type(int_type c) noexcept { return (char_type)c; }
    static inline int_type to_int_type(char_type c) noexcept { return c; }
    static inline bool eq_int_type(int_type a, int_type b) noexcept { return a == b; }
    static inline int_type eof() noexcept { return 0xFFFFFFFFu; }
};
}  // namespace std
#endif

#endif
