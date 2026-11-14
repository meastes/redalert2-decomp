// SPDX-License-Identifier: GPL-3.0-or-later
// The POSIX pieces mingw-w64 does not supply, on Win32, for the port on the
// x86_64 Windows build (port_compat.h, posix/*.h; README "Windows build"):
// futimes, fcntl(O_NONBLOCK) on sockets, sigaction, backtrace, fnmatch, dladdr,
// dlsym and the BSD sockets subset of wsock32.cpp on Winsock 2. With os_win.cpp,
// the port layer's only file that includes <windows.h> (built with
// RA2_WIN32_HEADERS); it does not include the posix/ socket headers (their BSD
// structs share Winsock's names), so the BSD layouts are restated here under
// bsd_* names with the same fields.
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>

#include <ctype.h>
#include <errno.h>
#include <io.h>
#include <mutex>
#include <signal.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utime.h>
#include <vector>

namespace {

// ---- BSD layouts (ra2_bsd_socket.h) --------------------------------------------
struct bsd_sockaddr { uint8_t sa_len; uint8_t sa_family; char sa_data[14]; };
struct bsd_in_addr { uint32_t addr; };   // BSD s_addr (a Winsock macro in this file)
struct bsd_sockaddr_in { uint8_t sin_len; uint8_t sin_family; uint16_t sin_port; bsd_in_addr sin_addr; char sin_zero[8]; };
struct bsd_hostent { char *h_name; char **h_aliases; int h_addrtype; int h_length; char **h_addr_list; };
struct bsd_pollfd { int fd; short events; short revents; };
struct bsd_ifaddrs {
    bsd_ifaddrs *ifa_next;
    char *ifa_name;
    unsigned int ifa_flags;
    bsd_sockaddr *ifa_addr;
    bsd_sockaddr *ifa_netmask;
    bsd_sockaddr *ifa_dstaddr;
    void *ifa_data;
};
static_assert(sizeof(bsd_sockaddr_in) == 16, "BSD sockaddr_in is 16 bytes");

void winsock_start()
{
    static std::once_flag once;
    std::call_once(once, [] {
        WSADATA data;
        WSAStartup(MAKEWORD(2, 2), &data);
    });
}

// The last Winsock error as errno (wsock32.cpp's set_error reads errno).
int fail()
{
    int e = WSAGetLastError();
    switch (e) {
    case WSAEWOULDBLOCK: errno = EWOULDBLOCK; break;
    case WSAEADDRINUSE: errno = EADDRINUSE; break;
    case WSAECONNRESET: errno = ECONNRESET; break;
    case WSAEMSGSIZE: errno = EMSGSIZE; break;
    case WSAEINVAL: errno = EINVAL; break;
    case WSAENOTSOCK: errno = ENOTSOCK; break;
    case WSAEACCES: errno = EACCES; break;
    case WSAEAFNOSUPPORT: errno = EAFNOSUPPORT; break;
    case WSAEPROTONOSUPPORT: errno = EPROTONOSUPPORT; break;
    default: errno = EIO; break;
    }
    return -1;
}

void to_winsock(const void *bsd, sockaddr_in *w)
{
    const bsd_sockaddr_in *b = (const bsd_sockaddr_in *)bsd;
    memset(w, 0, sizeof *w);
    w->sin_family = b->sin_family;
    w->sin_port = b->sin_port;
    w->sin_addr.s_addr = b->sin_addr.addr;
}
void to_bsd(const sockaddr_in *w, void *bsd)
{
    bsd_sockaddr_in *b = (bsd_sockaddr_in *)bsd;
    memset(b, 0, sizeof *b);
    b->sin_len = sizeof *b;
    b->sin_family = (uint8_t)w->sin_family;
    b->sin_port = w->sin_port;
    b->sin_addr.addr = w->sin_addr.s_addr;
}

// ---- sigaction ------------------------------------------------------------------
typedef void (*info_handler)(int, siginfo_t *, void *);
info_handler info_handlers[32];

LONG WINAPI unhandled(EXCEPTION_POINTERS *e)
{
    int sig = 0;
    switch (e->ExceptionRecord->ExceptionCode) {
    case EXCEPTION_ACCESS_VIOLATION:
    case EXCEPTION_IN_PAGE_ERROR:
    case EXCEPTION_STACK_OVERFLOW:
    case EXCEPTION_DATATYPE_MISALIGNMENT: sig = SIGSEGV; break;
    case EXCEPTION_ILLEGAL_INSTRUCTION:
    case EXCEPTION_PRIV_INSTRUCTION:
    case EXCEPTION_BREAKPOINT: sig = SIGILL; break;
    case EXCEPTION_INT_DIVIDE_BY_ZERO:
    case EXCEPTION_INT_OVERFLOW:
    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
    case EXCEPTION_FLT_INVALID_OPERATION: sig = SIGFPE; break;
    default: return EXCEPTION_CONTINUE_SEARCH;
    }
    if (!info_handlers[sig]) return EXCEPTION_CONTINUE_SEARCH;
    siginfo_t info = {sig, nullptr};
    if (e->ExceptionRecord->NumberParameters >= 2 && sig == SIGSEGV)
        info.si_addr = (void *)e->ExceptionRecord->ExceptionInformation[1];
    else
        info.si_addr = e->ExceptionRecord->ExceptionAddress;
    info_handlers[sig](sig, &info, e->ContextRecord);
    return EXCEPTION_CONTINUE_SEARCH;
}

void abort_trampoline(int sig)
{
    if (sig > 0 && sig < 32 && info_handlers[sig]) info_handlers[sig](sig, nullptr, nullptr);
}

// ---- fnmatch --------------------------------------------------------------------
bool match(const char *p, const char *n, bool fold)
{
    auto low = [fold](unsigned char c) { return fold ? tolower(c) : c; };
    for (; *p; ++p, ++n) {
        if (*p == '*') {
            while (p[1] == '*') ++p;
            if (!p[1]) return true;
            for (const char *s = n; *s; ++s)
                if (match(p + 1, s, fold)) return true;
            return match(p + 1, n + strlen(n), fold);
        }
        if (!*n) return false;
        if (*p == '?') continue;
        if (*p == '[') {
            const char *q = p + 1;
            bool negate = *q == '!' || *q == '^';
            if (negate) ++q;
            bool hit = false;
            for (bool first = true; *q && (first || *q != ']'); ++q, first = false) {
                if (q[1] == '-' && q[2] && q[2] != ']') {
                    if (low(*n) >= low(*q) && low(*n) <= low(q[2])) hit = true;
                    q += 2;
                } else if (low(*q) == low(*n)) hit = true;
            }
            if (*q != ']') { if (*n != '[') return false; continue; }   // no closing bracket: a literal '['
            if (hit == negate) return false;
            p = q;
            continue;
        }
        if (low(*p) != low(*n)) return false;
    }
    return !*n;
}

}  // namespace

extern "C" {

// ---- POSIX names of the UCRT ----------------------------------------------------------
// The UCRT's POSIX names (open, close, strcasecmp, ...) are weak aliases of its
// underscore names (_open, _close, _stricmp, ...), which the port defines itself for
// the recovered code with VC6 behaviour (port/src/crt.cpp) and implements with the
// POSIX names: through the alias, a port call such as close() would come back into
// the port's _close. These strong definitions go to the UCRT's own entry points
// (called through its import table: this file defines none of the underscore names).
int open(const char *path, int flags, ...)
{
    va_list ap;
    va_start(ap, flags);
    int mode = (flags & _O_CREAT) ? va_arg(ap, int) : 0;
    va_end(ap);
    // Binary unless text is asked for: the port opens files as POSIX does (CreateFileA,
    // the VC6 _open's translated flags); the UCRT's default is text mode.
    if (!(flags & _O_TEXT)) flags |= _O_BINARY;
    return _open(path, flags, mode);
}
int close(int fd) { return _close(fd); }
int read(int fd, void *buffer, unsigned int count) { return _read(fd, buffer, count); }
int unlink(const char *path) { return _unlink(path); }
int strcasecmp(const char *a, const char *b) { return _stricmp(a, b); }
int strncasecmp(const char *a, const char *b, size_t n) { return _strnicmp(a, b, n); }
char *strdup(const char *s) { return _strdup(s); }

// ---- names the native link sources declare themselves -----------------------------
// dprintf (POSIX; link/behaviour_startup.cpp, behaviour_m14_ownerdraw.cpp).
int dprintf(int fd, const char *format, ...)
{
    char buffer[4096];
    va_list ap;
    va_start(ap, format);
    int n = vsnprintf(buffer, sizeof buffer, format, ap);
    va_end(ap);
    if (n < 0) return n;
    return _write(fd, buffer, (unsigned)(n < (int)sizeof buffer ? n : (int)sizeof buffer - 1));
}

// macOS libmalloc's "which zone holds this block" (link/behaviour_m23.cpp: a seed's
// free of a cut pointer is refused): the process heap (the UCRT's malloc heap) when
// it holds the block, else null.
void *malloc_zone_from_ptr(const void *block)
{
    HANDLE heap = GetProcessHeap();
    return HeapValidate(heap, 0, block) ? (void *)heap : nullptr;
}

// ---- files ----------------------------------------------------------------------
int ra2_win_futimes(int fd, const struct timeval times[2])
{
    struct __utimbuf64 t;
    t.actime = times[0].tv_sec;
    t.modtime = times[1].tv_sec;
    return _futime64(fd, &t);
}

int ra2_win_setenv(const char *name, const char *value, int overwrite)
{
    if (!overwrite && getenv(name)) return 0;
    return _putenv_s(name, value) == 0 ? 0 : -1;
}

char *ra2_win_mkdtemp(char *path_template)
{
    size_t n = strlen(path_template);
    if (n < 6 || strcmp(path_template + n - 6, "XXXXXX")) { errno = EINVAL; return nullptr; }
    for (int attempt = 0; attempt < 100; ++attempt) {
        static const char digits[] = "abcdefghijklmnopqrstuvwxyz0123456789";
        uint64_t r = (uint64_t)GetTickCount64() * 6364136223846793005ull + GetCurrentProcessId() * 1442695040888963407ull
                   + (uint64_t)attempt * 2862933555777941757ull;
        for (int i = 0; i < 6; ++i) { path_template[n - 6 + i] = digits[r % 36]; r /= 36; }
        if (CreateDirectoryA(path_template, nullptr)) return path_template;
        if (GetLastError() != ERROR_ALREADY_EXISTS) break;
    }
    errno = EEXIST;
    return nullptr;
}

char *ra2_win_realpath(const char *path, char *resolved)
{
    char *r = _fullpath(resolved, path, MAX_PATH);
    if (!r) return nullptr;
    if (GetFileAttributesA(r) == INVALID_FILE_ATTRIBUTES) {
        if (!resolved) free(r);
        errno = ENOENT;
        return nullptr;
    }
    return r;
}

int ra2_win_rename(const char *from, const char *to)
{
    if (MoveFileExA(from, to, MOVEFILE_REPLACE_EXISTING | MOVEFILE_COPY_ALLOWED)) return 0;
    errno = GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND ? ENOENT : EACCES;
    return -1;
}

int ra2_win_fcntl(int fd, int command, ...)
{
    va_list ap;
    va_start(ap, command);
    int value = command == 4 ? va_arg(ap, int) : 0;
    va_end(ap);
    if (command == 3) return 0;                       // F_GETFL: no flags are tracked
    if (command == 4) {                               // F_SETFL: O_NONBLOCK on a socket
        u_long on = (value & 0x4000) ? 1 : 0;
        return ioctlsocket((SOCKET)fd, FIONBIO, &on) == 0 ? 0 : fail();
    }
    errno = EINVAL;
    return -1;
}

// ---- signals --------------------------------------------------------------------
// struct sigaction and siginfo_t: port_compat.h (the forced include of this layer).
int ra2_win_sigaction(int sig, const struct sigaction *action, struct sigaction *old)
{
    if (old) memset(old, 0, sizeof *old);
    if (!action) return 0;
    if (sig <= 0 || sig >= 32) { errno = EINVAL; return -1; }
    if (action->sa_flags & 0x40) {                    // SA_SIGINFO
        static std::once_flag once;
        std::call_once(once, [] { SetUnhandledExceptionFilter(unhandled); });
        info_handlers[sig] = action->sa_sigaction;
        // abort() and raise() reach the handler through the CRT's signal().
        if (sig == SIGABRT || sig == SIGFPE || sig == SIGILL || sig == SIGSEGV) signal(sig, abort_trampoline);
    } else {
        info_handlers[sig] = nullptr;
        if (sig == SIGABRT || sig == SIGFPE || sig == SIGILL || sig == SIGSEGV || sig == SIGINT || sig == SIGTERM)
            signal(sig, action->sa_handler);
    }
    return 0;
}

// ---- diagnostics ----------------------------------------------------------------
int ra2_win_backtrace(void **frames, int count)
{
    return (int)RtlCaptureStackBackTrace(1, (DWORD)count, frames, nullptr);
}

void ra2_win_backtrace_symbols_fd(void *const *frames, int count, int fd)
{
    for (int i = 0; i < count; ++i) {
        HMODULE module = nullptr;
        char name[MAX_PATH] = "?";
        if (GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                               (LPCSTR)frames[i], &module))
            GetModuleFileNameA(module, name, sizeof name);
        const char *base = name;
        for (const char *p = name; *p; ++p) if (*p == '\\' || *p == '/') base = p + 1;
        char line[MAX_PATH + 64];
        int n = snprintf(line, sizeof line, "%-3d %s+0x%llx\n", i, base,
                         (unsigned long long)((const char *)frames[i] - (const char *)module));
        if (n > 0) _write(fd, line, (unsigned)(n < (int)sizeof line ? n : (int)sizeof line - 1));
    }
}

typedef struct { const char *dli_fname; void *dli_fbase; const char *dli_sname; void *dli_saddr; } ra2_win_dl_info;
int ra2_win_dladdr(const void *address, ra2_win_dl_info *info)
{
    static thread_local char name[MAX_PATH];
    memset(info, 0, sizeof *info);
    HMODULE module = nullptr;
    if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                            (LPCSTR)address, &module))
        return 0;
    GetModuleFileNameA(module, name, sizeof name);
    info->dli_fname = name;
    info->dli_fbase = (void *)module;
    return 1;
}

void *ra2_win_dlsym(void *handle, const char *name)
{
    return (void *)GetProcAddress(handle ? (HMODULE)handle : GetModuleHandleA(nullptr), name);
}

int ra2_win_fnmatch(const char *pattern, const char *name, int flags)
{
    return match(pattern, name, (flags & 0x10) != 0) ? 0 : 1;
}

// ---- BSD sockets on Winsock 2 ------------------------------------------------------
int ra2_bsd_socket(int family, int type, int protocol)
{
    winsock_start();
    SOCKET s = ::socket(family, type, protocol);
    if (s == INVALID_SOCKET) return fail();
    return (int)s;
}

int ra2_bsd_bind(int fd, const void *address, int length)
{
    (void)length;
    sockaddr_in a;
    to_winsock(address, &a);
    return ::bind((SOCKET)fd, (sockaddr *)&a, sizeof a) == 0 ? 0 : fail();
}

ptrdiff_t ra2_bsd_sendto(int fd, const void *buffer, size_t length, int flags, const void *to, int tolength)
{
    (void)tolength;
    sockaddr_in a;
    to_winsock(to, &a);
    int n = ::sendto((SOCKET)fd, (const char *)buffer, (int)length, flags, (sockaddr *)&a, sizeof a);
    return n == SOCKET_ERROR ? fail() : n;
}

ptrdiff_t ra2_bsd_recvfrom(int fd, void *buffer, size_t length, int flags, void *from, int *fromlength)
{
    sockaddr_in a;
    int al = sizeof a;
    memset(&a, 0, sizeof a);
    int n = ::recvfrom((SOCKET)fd, (char *)buffer, (int)length, flags, (sockaddr *)&a, &al);
    if (n == SOCKET_ERROR) {
        // A datagram larger than the buffer: BSD truncates and returns the part read.
        if (WSAGetLastError() != WSAEMSGSIZE) return fail();
        n = (int)length;
    }
    if (from) to_bsd(&a, from);
    if (fromlength) *fromlength = (int)sizeof(bsd_sockaddr_in);
    return n;
}

int ra2_bsd_setsockopt(int fd, int level, int name, const void *value, int length)
{
    return ::setsockopt((SOCKET)fd, level, name, (const char *)value, length) == 0 ? 0 : fail();
}

int ra2_bsd_getsockopt(int fd, int level, int name, void *value, int *length)
{
    return ::getsockopt((SOCKET)fd, level, name, (char *)value, length) == 0 ? 0 : fail();
}

int ra2_bsd_gethostname(char *name, size_t length)
{
    winsock_start();
    return ::gethostname(name, (int)length) == 0 ? 0 : fail();
}

bsd_hostent *ra2_bsd_gethostbyname(const char *name)
{
    winsock_start();
    static thread_local bsd_hostent result;
    hostent *h = ::gethostbyname(name);
    if (!h) { fail(); return nullptr; }
    result.h_name = h->h_name;
    result.h_aliases = h->h_aliases;
    result.h_addrtype = h->h_addrtype;
    result.h_length = h->h_length;
    result.h_addr_list = h->h_addr_list;
    return &result;
}

char *ra2_bsd_inet_ntoa(bsd_in_addr address)
{
    in_addr a;
    a.s_addr = address.addr;
    return ::inet_ntoa(a);
}

int ra2_bsd_poll(bsd_pollfd *fds, unsigned int count, int timeout_ms)
{
    std::vector<WSAPOLLFD> w(count);
    for (unsigned int i = 0; i < count; ++i) {
        w[i].fd = (SOCKET)fds[i].fd;
        w[i].events = fds[i].events;
        w[i].revents = 0;
    }
    int n = WSAPoll(w.data(), count, timeout_ms);
    if (n == SOCKET_ERROR) return fail();
    for (unsigned int i = 0; i < count; ++i) fds[i].revents = w[i].revents;
    return n;
}

int ra2_bsd_getifaddrs(bsd_ifaddrs **list)
{
    *list = nullptr;
    ULONG size = 16 * 1024;
    std::vector<unsigned char> buffer;
    ULONG r;
    do {
        buffer.resize(size);
        r = GetAdaptersAddresses(AF_INET, GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER,
                                 nullptr, (IP_ADAPTER_ADDRESSES *)buffer.data(), &size);
    } while (r == ERROR_BUFFER_OVERFLOW);
    if (r != NO_ERROR) { errno = EIO; return -1; }
    bsd_ifaddrs **tail = list;
    for (IP_ADAPTER_ADDRESSES *a = (IP_ADAPTER_ADDRESSES *)buffer.data(); a; a = a->Next) {
        for (IP_ADAPTER_UNICAST_ADDRESS *u = a->FirstUnicastAddress; u; u = u->Next) {
            if (!u->Address.lpSockaddr || u->Address.lpSockaddr->sa_family != AF_INET) continue;
            // One allocation: the node, its address and its name.
            size_t name_length = strlen(a->AdapterName) + 1;
            unsigned char *block = (unsigned char *)calloc(1, sizeof(bsd_ifaddrs) + sizeof(bsd_sockaddr_in) + name_length);
            if (!block) break;
            bsd_ifaddrs *node = (bsd_ifaddrs *)block;
            bsd_sockaddr_in *address = (bsd_sockaddr_in *)(block + sizeof(bsd_ifaddrs));
            char *name = (char *)(block + sizeof(bsd_ifaddrs) + sizeof(bsd_sockaddr_in));
            memcpy(name, a->AdapterName, name_length);
            to_bsd((const sockaddr_in *)u->Address.lpSockaddr, address);
            node->ifa_name = name;
            node->ifa_addr = (bsd_sockaddr *)address;
            node->ifa_flags = (a->OperStatus == IfOperStatusUp ? 0x1u : 0u)
                            | (a->IfType == IF_TYPE_SOFTWARE_LOOPBACK ? 0x8u : 0x2u);
            *tail = node;
            tail = &node->ifa_next;
        }
    }
    return 0;
}

void ra2_bsd_freeifaddrs(bsd_ifaddrs *list)
{
    while (list) {
        bsd_ifaddrs *next = list->ifa_next;
        free(list);
        list = next;
    }
}

int ra2_bsd_close(int fd)
{
    if (::closesocket((SOCKET)fd) == 0) return 0;
    if (WSAGetLastError() == WSAENOTSOCK || WSAGetLastError() == WSANOTINITIALISED) return _close(fd);
    return fail();
}

}  // extern "C"
