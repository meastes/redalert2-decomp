// ra2_port core: logging, last-error, the handle table, DOS path mapping and
// the host hooks shared by the Win32 seam files.
#include "ra2port/win32_port.hpp"

#include <dirent.h>
#include <execinfo.h>
#include <signal.h>
#include <mutex>
#include <set>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <strings.h>
#include <sys/stat.h>
#include <vector>

namespace ra2port {

namespace {
std::mutex &table_lock() { static std::mutex m; return m; }
std::vector<HandleObject *> &table() { static std::vector<HandleObject *> t; return t; }
thread_local DWORD last_error = 0;
std::string &data_root() { static std::string d; return d; }
const Host *current_host = nullptr;
const uintptr_t HANDLE_BASE = 0x100;
}

void unimplemented(const char *name)
{
    static std::mutex m;
    static std::set<std::string> seen;
    std::lock_guard<std::mutex> g(m);
    if (seen.insert(name).second && getenv("RA2_PORT_QUIET") == nullptr)
        fprintf(stderr, "ra2_port: %s not implemented (returns failure)\n", name);
}

void trace(const char *fmt, ...)
{
    if (getenv("RA2_PORT_TRACE") == nullptr) return;
    va_list ap;
    va_start(ap, fmt);
    fprintf(stderr, "ra2_port: ");
    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);
    va_end(ap);
}

void set_last_error(DWORD code) { last_error = code; }

HANDLE handle_new(HandleObject *object)
{
    std::lock_guard<std::mutex> g(table_lock());
    auto &t = table();
    for (size_t i = 0; i < t.size(); ++i) {
        if (t[i] == nullptr) {
            t[i] = object;
            return (HANDLE)(HANDLE_BASE + i * 4);
        }
    }
    t.push_back(object);
    return (HANDLE)(HANDLE_BASE + (t.size() - 1) * 4);
}

HandleObject *handle_get(HANDLE handle, int kind)
{
    uintptr_t v = (uintptr_t)handle;
    if (v < HANDLE_BASE || (v - HANDLE_BASE) % 4) return nullptr;
    std::lock_guard<std::mutex> g(table_lock());
    size_t i = (v - HANDLE_BASE) / 4;
    auto &t = table();
    if (i >= t.size() || t[i] == nullptr) return nullptr;
    if (kind && t[i]->kind != kind) return nullptr;
    return t[i];
}

bool handle_close(HANDLE handle)
{
    uintptr_t v = (uintptr_t)handle;
    if (v < HANDLE_BASE || (v - HANDLE_BASE) % 4) return false;
    HandleObject *object = nullptr;
    {
        std::lock_guard<std::mutex> g(table_lock());
        size_t i = (v - HANDLE_BASE) / 4;
        auto &t = table();
        if (i >= t.size() || t[i] == nullptr) return false;
        object = t[i];
        t[i] = nullptr;
    }
    delete object;
    return true;
}

void set_data_dir(const char *dir) { data_root() = dir ? dir : ""; }
const char *data_dir() { return data_root().empty() ? "." : data_root().c_str(); }

// Resolve one path component case-insensitively inside dir; returns the real
// name or the component unchanged when nothing matches (a file to create).
static std::string match_component(const std::string &dir, const std::string &name)
{
    struct stat st;
    std::string direct = dir + "/" + name;
    if (stat(direct.c_str(), &st) == 0) return name;
    DIR *d = opendir(dir.empty() ? "/" : dir.c_str());
    if (!d) return name;
    std::string found = name;
    while (struct dirent *e = readdir(d)) {
        if (strcasecmp(e->d_name, name.c_str()) == 0) { found = e->d_name; break; }
    }
    closedir(d);
    return found;
}

char *host_path(const char *game_path, char *out, size_t n)
{
    std::string p = game_path ? game_path : "";
    for (auto &c : p) if (c == '\\') c = '/';
    // A drive-qualified path ("C:/...") is taken relative to the data root:
    // the game only names its own installation and save folders.
    if (p.size() >= 2 && p[1] == ':') p = p.substr(2);
    std::string base = data_dir();
    if (!p.empty() && p[0] == '/') {
        // An absolute host path that exists stays as it is.
        struct stat st;
        if (stat(p.c_str(), &st) == 0) { snprintf(out, n, "%s", p.c_str()); return out; }
        p = p.substr(1);
    }
    std::string result = base;
    size_t start = 0;
    while (start <= p.size()) {
        size_t slash = p.find('/', start);
        std::string part = p.substr(start, slash == std::string::npos ? std::string::npos : slash - start);
        if (!part.empty() && part != ".") result += "/" + match_component(result, part);
        if (slash == std::string::npos) break;
        start = slash + 1;
    }
    snprintf(out, n, "%s", result.c_str());
    return out;
}

size_t wlen(const WCHAR *s)
{
    size_t n = 0;
    while (s && s[n]) ++n;
    return n;
}

void set_host(const Host *h) { current_host = h; }
const Host *host() { return current_host; }

}  // namespace ra2port

extern "C" {
DWORD GetLastError(void) { return ra2port::last_error; }
void SetLastError(DWORD code) { ra2port::last_error = code; }
}

// Start-up progress (diagnostic). native_units.py puts a call at the head of
// every unit that owns original _initterm entries, with the index of its first
// one, so the last index recorded names the unit whose initialisers were
// running when a trap or crash stopped start-up. RA2_NATIVE_INIT_TRACE=1
// prints each mark as it is reached.
namespace {
int initterm_last = -1;
int initterm_units = 0;

// RA2_NATIVE_CRASH_TRACE=1: a fault prints the start-up position and a
// backtrace (diagnostics while the tree is still being closed).
void crash_trace(int sig)
{
    fprintf(stderr, "ra2-native: signal %d after initterm #%d (%d units)\n", sig, initterm_last, initterm_units);
    void *frames[48];
    int n = backtrace(frames, 48);
    backtrace_symbols_fd(frames, n, 2);
    signal(sig, SIG_DFL);
    raise(sig);
}
}

extern "C" int ra2port_initterm_mark(int index)
{
    if (initterm_units == 0 && getenv("RA2_NATIVE_CRASH_TRACE") != nullptr) {
        signal(SIGSEGV, crash_trace);
        signal(SIGBUS, crash_trace);
        signal(SIGABRT, crash_trace);
        signal(SIGILL, crash_trace);
    }
    initterm_last = index;
    ++initterm_units;
    if (getenv("RA2_NATIVE_INIT_TRACE") != nullptr)
        fprintf(stderr, "ra2-native: initterm #%d\n", index);
    return index;
}

extern "C" int ra2port_initterm_last(int *units)
{
    if (units) *units = initterm_units;
    return initterm_last;
}
