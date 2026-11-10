// KERNEL32 on POSIX: files, directories, time, memory, threads, synchronisation
// and the small string/locale helpers the recovered code imports. Behaviour
// follows the Win32 contract the game relies on; anything else is a logged
// failure (ra2port::unimplemented) with the documented Win32 failure value.
#include "ra2port/win32_port.hpp"
#include "os.hpp"
#include <ctype.h>

#include <condition_variable>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fnmatch.h>
#include <mutex>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
#include <string>
#include <strings.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <thread>
#include <time.h>
#include <unistd.h>
#include <utime.h>
#include <vector>

using namespace ra2port;

namespace {

const DWORD ERROR_FILE_NOT_FOUND = 2, ERROR_PATH_NOT_FOUND = 3, ERROR_ACCESS_DENIED = 5,
            ERROR_INVALID_HANDLE = 6, ERROR_NO_MORE_FILES = 18, ERROR_CALL_NOT_IMPLEMENTED = 120,
            ERROR_ALREADY_EXISTS = 183, ERROR_FILE_EXISTS = 80, WAIT_OBJECT_0 = 0, WAIT_TIMEOUT = 258,
            WAIT_FAILED = 0xFFFFFFFFu, INFINITE_WAIT = 0xFFFFFFFFu;

DWORD errno_to_win32(int e)
{
    switch (e) {
    case ENOENT: return ERROR_FILE_NOT_FOUND;
    case ENOTDIR: return ERROR_PATH_NOT_FOUND;
    case EACCES: case EPERM: return ERROR_ACCESS_DENIED;
    case EEXIST: return ERROR_FILE_EXISTS;
    default: return 31;  // ERROR_GEN_FAILURE
    }
}

struct File : HandleObject { int fd = -1; std::string path; ~File() { if (fd >= 0) close(fd); } };

// FILETIME: 100 ns ticks since 1601-01-01 UTC.
const uint64_t EPOCH_DIFF = 116444736000000000ull;
void to_filetime(const struct timespec &t, FILETIME *ft)
{
    uint64_t v = (uint64_t)t.tv_sec * 10000000ull + (uint64_t)t.tv_nsec / 100 + EPOCH_DIFF;
    ft->dwLowDateTime = (DWORD)v;
    ft->dwHighDateTime = (DWORD)(v >> 32);
}
uint64_t ft64(const FILETIME *ft) { return ((uint64_t)ft->dwHighDateTime << 32) | ft->dwLowDateTime; }

struct WIN32_FIND_DATAA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime, ftLastAccessTime, ftLastWriteTime;
    DWORD nFileSizeHigh, nFileSizeLow, dwReserved0, dwReserved1;
    char cFileName[260];
    char cAlternateFileName[14];
};

}  // namespace
namespace ra2port { std::string overlay_path(const std::string &host); }
namespace {
struct Find : HandleObject {
    std::string dir, pattern, dir_game;
    DIR *d = nullptr;
    // The write directory's twin of dir is listed first (files the game
    // wrote there, e.g. its saved games; Scout1125), then dir; a name
    // listed once is not listed again.
    std::string next_dir;
    std::set<std::string> seen;
    ~Find() { if (d) closedir(d); }
};

DWORD attributes(const struct stat &st)
{
    DWORD a = S_ISDIR(st.st_mode) ? 0x10 : 0x80;   // DIRECTORY / NORMAL
    if (!(st.st_mode & S_IWUSR)) a |= 0x01;         // READONLY
    return a;
}

bool find_next(Find *f, WIN32_FIND_DATAA *data)
{
    for (;;) {
    struct dirent *e = f->d ? readdir(f->d) : nullptr;
    if (!e) {
        if (f->next_dir.empty()) return false;
        if (f->d) closedir(f->d);
        f->dir = f->next_dir;
        f->next_dir.clear();
        f->d = opendir(f->dir.c_str());
        continue;
    }
    {
        if (fnmatch(f->pattern.c_str(), e->d_name, FNM_CASEFOLD) != 0) {
            // "*.*" also matches names without a dot, as on DOS.
            if (!(f->pattern == "*.*")) continue;
        }
        std::string lower = e->d_name;
        for (auto &c : lower) c = (char)tolower((unsigned char)c);
        if (!f->seen.insert(lower).second) continue;
        std::string full = f->dir + "/" + e->d_name;
        struct stat st;
        if (stat(full.c_str(), &st) != 0) continue;
        memset(data, 0, sizeof *data);
        data->dwFileAttributes = attributes(st);
        ra2port::os::FileTimes times = ra2port::os::file_times(st);
        to_filetime(times.modified, &data->ftLastWriteTime);
        to_filetime(times.created, &data->ftCreationTime);
        to_filetime(times.accessed, &data->ftLastAccessTime);
        data->nFileSizeLow = (DWORD)st.st_size;
        data->nFileSizeHigh = (DWORD)((uint64_t)st.st_size >> 32);
        snprintf(data->cFileName, sizeof data->cFileName, "%s", e->d_name);
        return true;
    }
    }
}

struct Event : HandleObject {
    std::mutex m;
    std::condition_variable cv;
    bool manual = false, signaled = false;
    std::string name;
};
struct Mutex : HandleObject {
    std::recursive_mutex m;
    std::string name;
};
struct Thread : HandleObject {
    pthread_t t{};
    DWORD id = 0;
    bool done = false;
    DWORD exit_code = 259;  // STILL_ACTIVE
    std::mutex m;
    std::condition_variable cv;
};

std::mutex &names_lock() { static std::mutex m; return m; }
std::vector<std::pair<std::string, HANDLE>> &named() { static std::vector<std::pair<std::string, HANDLE>> v; return v; }

HANDLE find_named(const char *name)
{
    if (!name) return nullptr;
    std::lock_guard<std::mutex> g(names_lock());
    for (auto &p : named()) if (p.first == name && handle_get(p.second)) return p.second;
    return nullptr;
}

DWORD thread_id()
{
    uint64_t id = 0;
    id = ra2port::os::thread_id();
    return (DWORD)id;
}

// Waits on one object; deadline in ms (INFINITE_WAIT = no timeout).
DWORD wait_one(HANDLE handle, DWORD ms)
{
    HandleObject *o = handle_get(handle);
    if (!o) return WAIT_FAILED;
    auto deadline = std::chrono::steady_clock::now() + std::chrono::milliseconds(ms == INFINITE_WAIT ? 0 : ms);
    if (o->kind == H_EVENT) {
        Event *e = (Event *)o;
        std::unique_lock<std::mutex> l(e->m);
        if (ms == INFINITE_WAIT) e->cv.wait(l, [&] { return e->signaled; });
        else if (!e->cv.wait_until(l, deadline, [&] { return e->signaled; })) return WAIT_TIMEOUT;
        if (!e->manual) e->signaled = false;
        return WAIT_OBJECT_0;
    }
    if (o->kind == H_THREAD) {
        Thread *t = (Thread *)o;
        std::unique_lock<std::mutex> l(t->m);
        if (ms == INFINITE_WAIT) t->cv.wait(l, [&] { return t->done; });
        else if (!t->cv.wait_until(l, deadline, [&] { return t->done; })) return WAIT_TIMEOUT;
        return WAIT_OBJECT_0;
    }
    if (o->kind == H_MUTEX) {
        Mutex *m = (Mutex *)o;
        if (ms == INFINITE_WAIT) { m->m.lock(); return WAIT_OBJECT_0; }
        while (!m->m.try_lock()) {
            if (std::chrono::steady_clock::now() >= deadline) return WAIT_TIMEOUT;
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        return WAIT_OBJECT_0;
    }
    if (o->kind == H_PROCESS) return WAIT_OBJECT_0;
    return WAIT_FAILED;
}

}  // namespace

extern "C" {

// ---- files ---------------------------------------------------------------

HANDLE CreateFileA(const char *name, DWORD access, DWORD share, void *security, DWORD disposition,
                   DWORD flags, HANDLE templ)
{
    (void)share; (void)security; (void)flags; (void)templ;
    // Windows: an empty name is not a path, and a directory opens only with
    // FILE_FLAG_BACKUP_SEMANTICS (0x02000000); otherwise access is denied.
    if (!name || !*name) { set_last_error(ERROR_PATH_NOT_FOUND); return RA2_INVALID_HANDLE; }
    char path[1024];
    bool rd = access & 0x80000000u, wr = access & 0x40000000u;
    bool writes = wr || disposition == 1 || disposition == 2 || disposition == 4 || disposition == 5;
    if (writes) host_path_write(name, path, sizeof path);
    else host_path(name, path, sizeof path);
    // A file to read that the listings know is missing fails as open() would,
    // without the open (one file-system round trip per probe; Scout1209).
    if (!writes) {
        if (host_path_missing()) {
            set_last_error(ERROR_FILE_NOT_FOUND);   // what the failed open() reported (ENOENT)
            trace("CreateFileA %s -> %s missing", name, path);
            return RA2_INVALID_HANDLE;
        }
    }
    int mode = 0;
    mode = (rd && wr) ? O_RDWR : wr ? O_WRONLY : O_RDONLY;
    switch (disposition) {
    case 1: mode |= O_CREAT | O_EXCL; break;   // CREATE_NEW
    case 2: mode |= O_CREAT | O_TRUNC; break;  // CREATE_ALWAYS
    case 3: break;                             // OPEN_EXISTING
    case 4: mode |= O_CREAT; break;            // OPEN_ALWAYS
    case 5: mode |= O_TRUNC; break;            // TRUNCATE_EXISTING
    }
    fs_op(FS_OPEN);
    int fd = open(path, mode, 0644);
    if (fd < 0) {
        set_last_error(errno_to_win32(errno));
        trace("CreateFileA %s -> %s failed", name, path);
        return RA2_INVALID_HANDLE;
    }
    struct stat st;
    if (!(flags & 0x02000000u) && fstat(fd, &st) == 0 && S_ISDIR(st.st_mode)) {
        close(fd);
        set_last_error(5);  // ERROR_ACCESS_DENIED
        return RA2_INVALID_HANDLE;
    }
    File *f = new File;
    f->kind = H_FILE;
    f->fd = fd;
    f->path = path;
    set_last_error(0);
    progress_count(writes ? "files written" : "files opened", name);
    return handle_new(f);
}

BOOL ReadFile(HANDLE handle, void *buffer, DWORD size, DWORD *read_bytes, void *overlapped)
{
    (void)overlapped;
    File *f = (File *)handle_get(handle, H_FILE);
    if (!f) { set_last_error(ERROR_INVALID_HANDLE); return 0; }
    fs_op(FS_READ);
    ssize_t n = read(f->fd, buffer, size);
    if (n < 0) { set_last_error(errno_to_win32(errno)); if (read_bytes) *read_bytes = 0; return 0; }
    if (read_bytes) *read_bytes = (DWORD)n;
    return 1;
}

BOOL WriteFile(HANDLE handle, const void *buffer, DWORD size, DWORD *written, void *overlapped)
{
    (void)overlapped;
    File *f = (File *)handle_get(handle, H_FILE);
    if (!f) { set_last_error(ERROR_INVALID_HANDLE); return 0; }
    ssize_t n = write(f->fd, buffer, size);
    if (n < 0) { set_last_error(errno_to_win32(errno)); if (written) *written = 0; return 0; }
    if (written) *written = (DWORD)n;
    return 1;
}

DWORD SetFilePointer(HANDLE handle, LONG distance, LONG *high, DWORD method)
{
    File *f = (File *)handle_get(handle, H_FILE);
    if (!f) { set_last_error(ERROR_INVALID_HANDLE); return 0xFFFFFFFFu; }
    int64_t offset = high ? (((int64_t)*high << 32) | (uint32_t)distance) : (int64_t)distance;
    fs_op(FS_SEEK);
    off_t r = lseek(f->fd, offset, method == 0 ? SEEK_SET : method == 1 ? SEEK_CUR : SEEK_END);
    if (r < 0) { set_last_error(131); return 0xFFFFFFFFu; }   // ERROR_NEGATIVE_SEEK
    if (high) *high = (LONG)((uint64_t)r >> 32);
    return (DWORD)r;
}

DWORD GetFileSize(HANDLE handle, DWORD *high)
{
    File *f = (File *)handle_get(handle, H_FILE);
    struct stat st;
    if (!f || fstat(f->fd, &st) != 0) { set_last_error(ERROR_INVALID_HANDLE); return 0xFFFFFFFFu; }
    if (high) *high = (DWORD)((uint64_t)st.st_size >> 32);
    return (DWORD)st.st_size;
}

BOOL GetFileTime(HANDLE handle, FILETIME *created, FILETIME *accessed, FILETIME *written)
{
    File *f = (File *)handle_get(handle, H_FILE);
    struct stat st;
    if (!f || fstat(f->fd, &st) != 0) { set_last_error(ERROR_INVALID_HANDLE); return 0; }
    ra2port::os::FileTimes times = ra2port::os::file_times(st);
    if (created) to_filetime(times.created, created);
    if (accessed) to_filetime(times.accessed, accessed);
    if (written) to_filetime(times.modified, written);
    return 1;
}

BOOL SetFileTime(HANDLE handle, const FILETIME *created, const FILETIME *accessed, const FILETIME *written)
{
    (void)created;
    File *f = (File *)handle_get(handle, H_FILE);
    if (!f) { set_last_error(ERROR_INVALID_HANDLE); return 0; }
    struct timeval tv[2];
    struct stat st;
    fstat(f->fd, &st);
    auto conv = [](const FILETIME *ft, struct timeval &out) {
        uint64_t v = ft64(ft) - EPOCH_DIFF;
        out.tv_sec = (time_t)(v / 10000000ull);
        out.tv_usec = (suseconds_t)((v % 10000000ull) / 10);
    };
    ra2port::os::FileTimes times = ra2port::os::file_times(st);
    tv[0].tv_sec = times.accessed.tv_sec; tv[0].tv_usec = 0;
    tv[1].tv_sec = times.modified.tv_sec; tv[1].tv_usec = 0;
    if (accessed) conv(accessed, tv[0]);
    if (written) conv(written, tv[1]);
    return futimes(f->fd, tv) == 0;
}

struct BY_HANDLE_FILE_INFORMATION {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime, ftLastAccessTime, ftLastWriteTime;
    DWORD dwVolumeSerialNumber, nFileSizeHigh, nFileSizeLow, nNumberOfLinks, nFileIndexHigh, nFileIndexLow;
};

BOOL GetFileInformationByHandle(HANDLE handle, BY_HANDLE_FILE_INFORMATION *info)
{
    File *f = (File *)handle_get(handle, H_FILE);
    struct stat st;
    if (!f || fstat(f->fd, &st) != 0) { set_last_error(ERROR_INVALID_HANDLE); return 0; }
    memset(info, 0, sizeof *info);
    info->dwFileAttributes = attributes(st);
    ra2port::os::FileTimes times = ra2port::os::file_times(st);
    to_filetime(times.created, &info->ftCreationTime);
    to_filetime(times.accessed, &info->ftLastAccessTime);
    to_filetime(times.modified, &info->ftLastWriteTime);
    info->dwVolumeSerialNumber = (DWORD)st.st_dev;
    info->nFileSizeHigh = (DWORD)((uint64_t)st.st_size >> 32);
    info->nFileSizeLow = (DWORD)st.st_size;
    info->nNumberOfLinks = (DWORD)st.st_nlink;
    info->nFileIndexHigh = (DWORD)((uint64_t)st.st_ino >> 32);
    info->nFileIndexLow = (DWORD)st.st_ino;
    return 1;
}

BOOL CloseHandle(HANDLE handle)
{
    if (!handle_close(handle)) { set_last_error(ERROR_INVALID_HANDLE); return 0; }
    return 1;
}

BOOL DeleteFileA(const char *name)
{
    char path[1024];
    host_path_write(name, path, sizeof path);
    if (unlink(path) != 0) { set_last_error(errno_to_win32(errno)); return 0; }
    return 1;
}

BOOL CopyFileA(const char *from, const char *to, BOOL fail_if_exists)
{
    char a[1024], b[1024];
    host_path(from, a, sizeof a);
    host_path_write(to, b, sizeof b);
    int in = open(a, O_RDONLY);
    if (in < 0) { set_last_error(errno_to_win32(errno)); return 0; }
    int out = open(b, O_WRONLY | O_CREAT | (fail_if_exists ? O_EXCL : O_TRUNC), 0644);
    if (out < 0) { set_last_error(errno_to_win32(errno)); close(in); return 0; }
    char buf[65536];
    ssize_t n;
    while ((n = read(in, buf, sizeof buf)) > 0) write(out, buf, (size_t)n);
    close(in);
    close(out);
    return 1;
}

BOOL CreateDirectoryA(const char *name, void *security)
{
    (void)security;
    char path[1024];
    host_path_write(name, path, sizeof path);
    if (mkdir(path, 0755) != 0) { set_last_error(errno == EEXIST ? ERROR_ALREADY_EXISTS : errno_to_win32(errno)); return 0; }
    return 1;
}

HANDLE FindFirstFileA(const char *spec, WIN32_FIND_DATAA *data)
{
    char path[1024];
    host_path(spec, path, sizeof path);
    std::string p = path;
    size_t slash = p.rfind('/');
    Find *f = new Find;
    f->kind = H_FIND;
    f->dir = slash == std::string::npos ? "." : p.substr(0, slash);
    // The pattern is the game's own last component (host_path may have
    // case-matched a literal name; wildcards are kept).
    std::string g = spec ? spec : "";
    size_t gs = g.find_last_of("\\/:");
    f->pattern = gs == std::string::npos ? g : g.substr(gs + 1);
    std::string twin = overlay_path(f->dir);
    struct stat twin_st;
    if (!twin.empty() && twin != f->dir && stat(twin.c_str(), &twin_st) == 0 && S_ISDIR(twin_st.st_mode)) {
        f->next_dir = f->dir;
        f->dir = twin;
    }
    f->d = opendir(f->dir.c_str());
    if (!f->d && !f->next_dir.empty()) {
        f->dir = f->next_dir;
        f->next_dir.clear();
        f->d = opendir(f->dir.c_str());
    }
    if (!f->d || !find_next(f, data)) {
        delete f;
        set_last_error(ERROR_FILE_NOT_FOUND);
        return RA2_INVALID_HANDLE;
    }
    return handle_new(f);
}

BOOL FindNextFileA(HANDLE handle, WIN32_FIND_DATAA *data)
{
    Find *f = (Find *)handle_get(handle, H_FIND);
    if (!f) { set_last_error(ERROR_INVALID_HANDLE); return 0; }
    if (!find_next(f, data)) { set_last_error(ERROR_NO_MORE_FILES); return 0; }
    return 1;
}

BOOL FindClose(HANDLE handle) { return handle_close(handle) ? 1 : 0; }

DWORD GetCurrentDirectoryA(DWORD size, char *buffer)
{
    // The game's working directory is its installation: report it as C:\ form
    // would mislead path checks, so the host path of the data root is given.
    const char *d = data_dir();
    DWORD n = (DWORD)strlen(d);
    if (buffer && size > n) memcpy(buffer, d, n + 1);
    return size > n ? n : n + 1;
}

BOOL SetCurrentDirectoryA(const char *name)
{
    char path[1024];
    host_path(name, path, sizeof path);
    struct stat st;
    if (stat(path, &st) != 0 || !S_ISDIR(st.st_mode)) { set_last_error(ERROR_PATH_NOT_FOUND); return 0; }
    set_data_dir(path);
    return 1;
}

BOOL GetDiskFreeSpaceA(const char *root, DWORD *sectors_per_cluster, DWORD *bytes_per_sector,
                       DWORD *free_clusters, DWORD *total_clusters)
{
    (void)root;
    ra2port::os::Volume v;
    if (!ra2port::os::volume(data_dir(), &v)) return 0;
    // Win9x-era callers multiply these in 32 bits: clamp to 2 GB like Win98 did.
    uint64_t free_bytes = v.free_bytes, total = v.total_bytes;
    const uint64_t cap = 0x7FFF8000ull;
    if (free_bytes > cap) free_bytes = cap;
    if (total > cap) total = cap;
    if (sectors_per_cluster) *sectors_per_cluster = 64;
    if (bytes_per_sector) *bytes_per_sector = 512;
    if (free_clusters) *free_clusters = (DWORD)(free_bytes / 32768);
    if (total_clusters) *total_clusters = (DWORD)(total / 32768);
    return 1;
}

UINT GetDriveTypeA(const char *root) { (void)root; return 3; }   // DRIVE_FIXED

DWORD GetLogicalDriveStringsA(DWORD size, char *buffer)
{
    static const char drives[] = "C:\\\0";
    if (buffer && size >= sizeof drives) memcpy(buffer, drives, sizeof drives);
    return sizeof drives - 1;
}

BOOL GetVolumeInformationA(const char *root, char *name, DWORD name_size, DWORD *serial, DWORD *max_component,
                           DWORD *flags, char *fs_name, DWORD fs_size)
{
    // The volume that holds the drive's folder (every drive letter maps onto
    // the installation): its mount name as the label, its file-system id as the
    // serial, its type as the file-system name. A disc mounted by the host
    // reports its own label (RA1/RA2 for the game discs).
    char path[1024];
    host_path(root && *root ? root : "C:\\", path, sizeof path);
    ra2port::os::Volume st;
    if (!ra2port::os::volume(path, &st) && !ra2port::os::volume(data_dir(), &st)) { set_last_error(21); return 0; }  // ERROR_NOT_READY
    const char *label = strrchr(st.mount_point.c_str(), '/');
    label = label && label[1] ? label + 1 : "";
    if (name && name_size) { strncpy(name, label, name_size - 1); name[name_size - 1] = 0; }
    if (serial) *serial = st.serial;
    if (max_component) *max_component = 255;
    if (flags) *flags = 0x3;   // FS_CASE_SENSITIVE | FS_CASE_IS_PRESERVED
    if (fs_name && fs_size) { strncpy(fs_name, st.type.c_str(), fs_size - 1); fs_name[fs_size - 1] = 0; }
    set_last_error(0);
    return 1;
}

DWORD GetModuleFileNameA(HINSTANCE module, char *buffer, DWORD size)
{
    (void)module;
    // The game derives its folder from this: report the data root's game.exe.
    char path[1024];
    snprintf(path, sizeof path, "%s/game.exe", data_dir());
    for (char *c = path; *c; ++c) if (*c == '/') *c = '\\';
    DWORD n = (DWORD)strlen(path);
    if (size == 0) return 0;
    if (n >= size) n = size - 1;
    memcpy(buffer, path, n);
    buffer[n] = 0;
    return n;
}

// ---- time ----------------------------------------------------------------

static void fill_systemtime(const struct tm &tm, int ms, SYSTEMTIME *st)
{
    st->wYear = (WORD)(tm.tm_year + 1900);
    st->wMonth = (WORD)(tm.tm_mon + 1);
    st->wDayOfWeek = (WORD)tm.tm_wday;
    st->wDay = (WORD)tm.tm_mday;
    st->wHour = (WORD)tm.tm_hour;
    st->wMinute = (WORD)tm.tm_min;
    st->wSecond = (WORD)tm.tm_sec;
    st->wMilliseconds = (WORD)ms;
}

void GetSystemTime(SYSTEMTIME *st)
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    struct tm tm;
    gmtime_r(&t.tv_sec, &tm);
    fill_systemtime(tm, (int)(t.tv_nsec / 1000000), st);
}

BOOL FileTimeToSystemTime(const FILETIME *ft, SYSTEMTIME *st)
{
    uint64_t v = ft64(ft);
    if (v < EPOCH_DIFF) return 0;
    v -= EPOCH_DIFF;
    time_t s = (time_t)(v / 10000000ull);
    struct tm tm;
    gmtime_r(&s, &tm);
    fill_systemtime(tm, (int)((v % 10000000ull) / 10000), st);
    return 1;
}

BOOL SystemTimeToFileTime(const SYSTEMTIME *st, FILETIME *ft)
{
    struct tm tm = {};
    tm.tm_year = st->wYear - 1900;
    tm.tm_mon = st->wMonth - 1;
    tm.tm_mday = st->wDay;
    tm.tm_hour = st->wHour;
    tm.tm_min = st->wMinute;
    tm.tm_sec = st->wSecond;
    time_t s = timegm(&tm);
    struct timespec t = {s, (long)st->wMilliseconds * 1000000L};
    to_filetime(t, ft);
    return 1;
}

BOOL FileTimeToLocalFileTime(const FILETIME *in, FILETIME *out)
{
    time_t now = time(nullptr);
    struct tm tm;
    localtime_r(&now, &tm);
    int64_t v = (int64_t)ft64(in) + (int64_t)tm.tm_gmtoff * 10000000ll;
    out->dwLowDateTime = (DWORD)v;
    out->dwHighDateTime = (DWORD)((uint64_t)v >> 32);
    return 1;
}

LONG CompareFileTime(const FILETIME *a, const FILETIME *b)
{
    uint64_t x = ft64(a), y = ft64(b);
    return x < y ? -1 : x > y ? 1 : 0;
}

BOOL FileTimeToDosDateTime(const FILETIME *ft, WORD *date, WORD *time_)
{
    SYSTEMTIME st;
    if (!FileTimeToSystemTime(ft, &st) || st.wYear < 1980) return 0;
    *date = (WORD)(((st.wYear - 1980) << 9) | (st.wMonth << 5) | st.wDay);
    *time_ = (WORD)((st.wHour << 11) | (st.wMinute << 5) | (st.wSecond / 2));
    return 1;
}

BOOL DosDateTimeToFileTime(WORD date, WORD time_, FILETIME *ft)
{
    SYSTEMTIME st = {};
    st.wYear = (WORD)((date >> 9) + 1980);
    st.wMonth = (WORD)((date >> 5) & 15);
    st.wDay = (WORD)(date & 31);
    st.wHour = (WORD)(time_ >> 11);
    st.wMinute = (WORD)((time_ >> 5) & 63);
    st.wSecond = (WORD)((time_ & 31) * 2);
    return SystemTimeToFileTime(&st, ft);
}

int GetDateFormatA(DWORD locale, DWORD flags, const SYSTEMTIME *st, const char *format, char *out, int size)
{
    (void)locale; (void)flags; (void)format;
    SYSTEMTIME now;
    if (!st) { GetSystemTime(&now); st = &now; }
    char buf[64];
    // Windows' en-US short date (DATE_SHORTDATE, "M/d/yyyy"; M25: the load dialog's
    // 56-pixel date column fits it).
    int n = snprintf(buf, sizeof buf, "%u/%u/%04u", st->wMonth, st->wDay, st->wYear) + 1;
    if (size == 0) return n;
    if (n > size) return 0;
    memcpy(out, buf, (size_t)n);
    return n;
}

int GetTimeFormatA(DWORD locale, DWORD flags, const SYSTEMTIME *st, const char *format, char *out, int size)
{
    (void)locale; (void)flags; (void)format;
    SYSTEMTIME now;
    if (!st) { GetSystemTime(&now); st = &now; }
    char buf[64];
    // Windows' en-US time ("h:mm:ss tt"; M25).
    unsigned hour = st->wHour % 12 ? st->wHour % 12 : 12;
    int n = snprintf(buf, sizeof buf, "%u:%02u:%02u %s", hour, st->wMinute, st->wSecond, st->wHour < 12 ? "AM" : "PM") + 1;
    if (size == 0) return n;
    if (n > size) return 0;
    memcpy(out, buf, (size_t)n);
    return n;
}

// ---- memory --------------------------------------------------------------

struct MEMORYSTATUS { DWORD dwLength, dwMemoryLoad, dwTotalPhys, dwAvailPhys, dwTotalPageFile,
                      dwAvailPageFile, dwTotalVirtual, dwAvailVirtual; };

void GlobalMemoryStatus(MEMORYSTATUS *status)
{
    // The game sizes caches from these (32-bit fields): report what a
    // well-equipped Win98 machine reported, capped at 2 GB.
    uint64_t phys = ra2port::os::physical_memory();
    const DWORD cap = 0x7FFFFFFFu;
    DWORD p = phys > cap ? cap : (DWORD)phys;
    status->dwLength = sizeof *status;
    status->dwMemoryLoad = 10;
    status->dwTotalPhys = p;
    status->dwAvailPhys = p / 2;
    status->dwTotalPageFile = cap;
    status->dwAvailPageFile = cap;
    status->dwTotalVirtual = cap;
    status->dwAvailVirtual = cap;
}

BOOL HeapDestroy(HANDLE heap) { (void)heap; return 1; }
BOOL VirtualProtect(void *address, uintptr_t size, DWORD protect, DWORD *old)
{
    // Used to make the original's self-modifying blitters writable; the native
    // blitters are ordinary code, so this only reports success.
    (void)address; (void)size; (void)protect;
    if (old) *old = 0x40;  // PAGE_EXECUTE_READWRITE
    return 1;
}
uintptr_t VirtualQuery(const void *address, void *info, uintptr_t size)
{
    (void)address; (void)info; (void)size;
    unimplemented("VirtualQuery");
    return 0;
}
BOOL IsBadReadPtr(const void *p, uintptr_t size) { (void)size; return p == nullptr; }
BOOL IsBadCodePtr(void *p) { return p == nullptr; }
BOOL FlushInstructionCache(HANDLE process, const void *address, uintptr_t size)
{
    (void)process; (void)address; (void)size;
    return 1;
}
// File mappings: only the launcher's message (set_launcher below) exists natively.
struct Mapping : HandleObject {
    std::string text;
};
void *MapViewOfFileEx(HANDLE mapping, DWORD access, DWORD high, DWORD low, uintptr_t size, void *base)
{
    (void)access; (void)high; (void)size; (void)base;
    if (Mapping *m = (Mapping *)handle_get(mapping, H_MAPPING)) {
        return low < m->text.size() + 1 ? (void *)(m->text.c_str() + low) : nullptr;
    }
    unimplemented("MapViewOfFileEx");
    return nullptr;
}
BOOL UnmapViewOfFile(const void *base) { return base != nullptr; }

// ---- threads and processes -------------------------------------------------

typedef DWORD (*THREAD_PROC)(void *);
struct ThreadStart { THREAD_PROC proc; void *param; Thread *thread; };

static void *thread_main(void *arg)
{
    ThreadStart start = *(ThreadStart *)arg;
    delete (ThreadStart *)arg;
    start.thread->id = thread_id();
    DWORD code = start.proc(start.param);
    std::lock_guard<std::mutex> g(start.thread->m);
    start.thread->exit_code = code;
    start.thread->done = true;
    start.thread->cv.notify_all();
    return nullptr;
}

HANDLE CreateThread(void *security, uintptr_t stack, THREAD_PROC proc, void *param, DWORD flags, DWORD *id)
{
    (void)security; (void)flags;
    Thread *t = new Thread;
    t->kind = H_THREAD;
    HANDLE h = handle_new(t);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, stack > 1u << 20 ? stack : 1u << 20);
    if (pthread_create(&t->t, &attr, thread_main, new ThreadStart{proc, param, t}) != 0) {
        pthread_attr_destroy(&attr);
        handle_close(h);
        return nullptr;
    }
    pthread_attr_destroy(&attr);
    pthread_detach(t->t);
    if (id) {
        *id = (DWORD)ra2port::os::thread_id(t->t);
    }
    return h;
}

DWORD GetCurrentThreadId(void) { return thread_id(); }
BOOL SetThreadPriority(HANDLE thread, int priority) { (void)thread; (void)priority; return 1; }
HANDLE GetCurrentProcess(void) { return (HANDLE)(intptr_t)-1; }
BOOL TerminateProcess(HANDLE process, UINT code)
{
    if (process == (HANDLE)(intptr_t)-1) _exit((int)code);
    return 0;
}
void ExitProcess(UINT code) { exit((int)code); }
BOOL GetExitCodeProcess(HANDLE process, DWORD *code) { (void)process; if (code) *code = 0; return 1; }
BOOL CreateProcessA(const char *app, char *cmd, void *pa, void *ta, BOOL inherit, DWORD flags, void *env,
                    const char *dir, void *startup, void *info)
{
    // Launching other programs (the autorun, the browser, patches) is not part
    // of the native game.
    (void)app; (void)cmd; (void)pa; (void)ta; (void)inherit; (void)flags; (void)env; (void)dir; (void)startup; (void)info;
    unimplemented("CreateProcessA");
    set_last_error(ERROR_CALL_NOT_IMPLEMENTED);
    return 0;
}
void GetStartupInfoA(void *info)
{
    // STARTUPINFOA: cb first; zero the rest of what the caller sized.
    DWORD cb = *(DWORD *)info;
    memset(info, 0, cb ? cb : 68);
    *(DWORD *)info = cb;
}

// ---- synchronisation -----------------------------------------------------

// CRITICAL_SECTION (shared/win32_api.hpp): the first field (DebugInfo, a
// pointer) holds the native recursive mutex.
struct CRITICAL_SECTION_PORT { void *impl; };

void InitializeCriticalSection(CRITICAL_SECTION_PORT *cs) { cs->impl = new std::recursive_mutex; }
void DeleteCriticalSection(CRITICAL_SECTION_PORT *cs) { delete (std::recursive_mutex *)cs->impl; cs->impl = nullptr; }
void EnterCriticalSection(CRITICAL_SECTION_PORT *cs)
{
    if (!cs->impl) cs->impl = new std::recursive_mutex;  // zero-initialised static use
    ((std::recursive_mutex *)cs->impl)->lock();
}
void LeaveCriticalSection(CRITICAL_SECTION_PORT *cs) { ((std::recursive_mutex *)cs->impl)->unlock(); }

LONG InterlockedIncrement(LONG volatile *v) { return __atomic_add_fetch(v, 1, __ATOMIC_SEQ_CST); }
LONG InterlockedDecrement(LONG volatile *v) { return __atomic_sub_fetch(v, 1, __ATOMIC_SEQ_CST); }

HANDLE CreateEventA(void *security, BOOL manual, BOOL initial, const char *name)
{
    (void)security;
    if (HANDLE h = find_named(name)) { set_last_error(ERROR_ALREADY_EXISTS); return h; }
    Event *e = new Event;
    e->kind = H_EVENT;
    e->manual = manual;
    e->signaled = initial;
    HANDLE h = handle_new(e);
    if (name) { std::lock_guard<std::mutex> g(names_lock()); named().push_back({name, h}); }
    set_last_error(0);
    return h;
}

HANDLE OpenMutexA(DWORD access, BOOL inherit, const char *name)
{
    // Named objects of this process only (the autoplay/launcher mutexes of
    // other programs do not exist natively).
    (void)access; (void)inherit;
    HANDLE h = find_named(name);
    if (!h) set_last_error(ERROR_FILE_NOT_FOUND);
    return h;
}

HANDLE OpenEventA(DWORD access, BOOL inherit, const char *name)
{
    (void)access; (void)inherit;
    HANDLE h = find_named(name);
    if (!h) set_last_error(ERROR_FILE_NOT_FOUND);
    return h;
}

namespace {
std::string launcher_event_name, launcher_text;
bool launcher_answered = false;
}

extern "C++" {   // this part of the file is extern "C"; the port's own API is C++
namespace ra2port {
void set_launcher(const char *event_name, const char *message)
{
    launcher_event_name = event_name ? event_name : "";
    launcher_text = message ? message : "";
}
}  // namespace ra2port
}

BOOL SetEvent(HANDLE handle)
{
    Event *e = (Event *)handle_get(handle, H_EVENT);
    if (!e) return 0;
    {
        std::lock_guard<std::mutex> g(e->m);
        e->signaled = true;
        if (e->manual) e->cv.notify_all(); else e->cv.notify_one();
    }
    // The launcher answers the game's signal once (see set_launcher).
    if (!launcher_answered && !launcher_event_name.empty() && handle == find_named(launcher_event_name.c_str())) {
        launcher_answered = true;
        Mapping *m = new Mapping;
        m->kind = H_MAPPING;
        m->text = launcher_text;
        post_message(nullptr, 0xBEEF, 0, (LPARAM)handle_new(m));
    }
    return 1;
}

BOOL ResetEvent(HANDLE handle)
{
    Event *e = (Event *)handle_get(handle, H_EVENT);
    if (!e) return 0;
    std::lock_guard<std::mutex> g(e->m);
    e->signaled = false;
    return 1;
}

HANDLE CreateMutexA(void *security, BOOL owned, const char *name)
{
    (void)security;
    // The single-instance check: one native process, so a name is always new.
    if (HANDLE h = find_named(name)) { set_last_error(ERROR_ALREADY_EXISTS); return h; }
    Mutex *m = new Mutex;
    m->kind = H_MUTEX;
    if (owned) m->m.lock();
    HANDLE h = handle_new(m);
    if (name) { std::lock_guard<std::mutex> g(names_lock()); named().push_back({name, h}); }
    set_last_error(0);
    return h;
}

BOOL ReleaseMutex(HANDLE handle)
{
    Mutex *m = (Mutex *)handle_get(handle, H_MUTEX);
    if (!m) return 0;
    m->m.unlock();
    return 1;
}

DWORD WaitForSingleObject(HANDLE handle, DWORD ms) { return wait_one(handle, ms); }

DWORD WaitForMultipleObjects(DWORD count, const HANDLE *handles, BOOL all, DWORD ms)
{
    auto start = std::chrono::steady_clock::now();
    for (;;) {
        bool every = true;
        for (DWORD i = 0; i < count; ++i) {
            DWORD r = wait_one(handles[i], 0);
            if (r == WAIT_OBJECT_0 && !all) return WAIT_OBJECT_0 + i;
            if (r != WAIT_OBJECT_0) every = false;
        }
        if (all && every) return WAIT_OBJECT_0;
        if (ms != INFINITE_WAIT &&
            std::chrono::steady_clock::now() - start >= std::chrono::milliseconds(ms)) return WAIT_TIMEOUT;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

// ---- modules and resources -------------------------------------------------

HINSTANCE GetModuleHandleA(const char *name) { (void)name; return (HINSTANCE)(uintptr_t)0x400000; }
// System DLLs whose functionality the port itself provides: the common controls
// (user32.cpp/gdi). Their DllGetVersion reports the version the port stands in
// for (comctl32 5.81, the Windows 2000 / IE5 common controls; RA2 asks >= 4.70).
// Any other DLL (Blowfish is a built-in COM server; Bink/WOL helpers are linked
// or absent) is not found.
namespace {
struct DllVersionInfoPort { DWORD cbSize, dwMajorVersion, dwMinorVersion, dwBuildNumber, dwPlatformID; };
int32_t comctl32_DllGetVersion(DllVersionInfoPort *info)
{
    if (!info || info->cbSize < sizeof(DllVersionInfoPort)) return (int32_t)0x80070057u;  // E_INVALIDARG
    info->dwMajorVersion = 5;
    info->dwMinorVersion = 81;
    info->dwBuildNumber = 0;
    info->dwPlatformID = 2;  // DLLVER_PLATFORM_NT
    return 0;
}
const uintptr_t COMCTL32_MODULE = 0x7E000000;
}

HINSTANCE LoadLibraryA(const char *name)
{
    trace("LoadLibraryA %s", name ? name : "(null)");
    if (name && !strcasecmp(name, "comctl32.dll")) return (HINSTANCE)COMCTL32_MODULE;
    set_last_error(126);  // ERROR_MOD_NOT_FOUND
    return nullptr;
}
void *GetProcAddress(HINSTANCE module, const char *name)
{
    if ((uintptr_t)module == COMCTL32_MODULE && name && (uintptr_t)name >= 0x10000 && !strcmp(name, "DllGetVersion"))
        return (void *)&comctl32_DllGetVersion;
    return nullptr;
}
BOOL FreeLibrary(HINSTANCE module) { (void)module; return 1; }
// Resources (dialog templates, icons, version info) are read from the supported
// game.exe at run time (ra2port::original_resource; AGENTS.md "Native phase").
// The resource handle is the data's address in the mapped image (load and lock
// return it unchanged, as Win32 does for a module's resources).
} // extern "C"
namespace ra2port { const void *original_resource(uintptr_t type, uintptr_t name, size_t *size); }
extern "C" {
HANDLE FindResourceA(HINSTANCE module, const char *name, const char *type)
{
    (void)module;
    const void *data = ra2port::original_resource((uintptr_t)type, (uintptr_t)name, nullptr);
    if (!data) {
        char what[96];
        if ((uintptr_t)name < 0x10000) snprintf(what, sizeof what, "FindResourceA (#%u type %u: not in the supported game.exe)", (unsigned)(uintptr_t)name, (unsigned)(uintptr_t)type);
        else snprintf(what, sizeof what, "FindResourceA (\"%.40s\": not in the supported game.exe)", name);
        unimplemented(what);
    }
    return (HANDLE)data;
}
HANDLE LoadResource(HINSTANCE module, HANDLE resource) { (void)module; return resource; }
void *LockResource(HANDLE resource) { return (void *)resource; }

// ---- strings, locale, misc -----------------------------------------------

int MulDiv(int a, int b, int c)
{
    // Win32 computes a*b in 64 bits and rounds the quotient half away from zero.
    if (c == 0) return -1;
    long double q = (long double)((int64_t)a * b) / c;
    int64_t r = (int64_t)(q < 0 ? q - 0.5L : q + 0.5L);
    return (r > INT32_MAX || r < INT32_MIN) ? -1 : (int)r;
}

char *lstrcatA(char *a, const char *b) { return strcat(a, b); }
char *lstrcpyA(char *a, const char *b) { return strcpy(a, b); }
int lstrlenA(const char *a) { return a ? (int)strlen(a) : 0; }

// Code pages: the game converts its own Latin-1/1252 strings; bytes 0x80-0x9F
// of cp1252 are mapped, the rest is Latin-1.
static const WCHAR CP1252_HIGH[32] = {
    0x20AC, 0x0081, 0x201A, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021, 0x02C6, 0x2030, 0x0160, 0x2039, 0x0152, 0x008D, 0x017D, 0x008F,
    0x0090, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014, 0x02DC, 0x2122, 0x0161, 0x203A, 0x0153, 0x009D, 0x017E, 0x0178};

int MultiByteToWideChar(UINT code_page, DWORD flags, const char *text, int length, WCHAR *out, int size)
{
    (void)code_page; (void)flags;
    if (length < 0) length = (int)strlen(text) + 1;
    if (size == 0) return length;
    int n = length < size ? length : size;
    for (int i = 0; i < n; ++i) {
        unsigned char c = (unsigned char)text[i];
        out[i] = (c >= 0x80 && c < 0xA0) ? CP1252_HIGH[c - 0x80] : c;
    }
    return n;
}

int WideCharToMultiByte(UINT code_page, DWORD flags, const WCHAR *text, int length, char *out, int size,
                        const char *default_char, BOOL *used_default)
{
    (void)code_page; (void)flags;
    if (length < 0) length = (int)wlen(text) + 1;
    if (used_default) *used_default = 0;
    if (size == 0) return length;
    int n = length < size ? length : size;
    for (int i = 0; i < n; ++i) {
        WCHAR c = text[i];
        char m = '?';
        if (c < 0x80 || (c >= 0xA0 && c < 0x100)) m = (char)c;
        else {
            for (int k = 0; k < 32; ++k) if (CP1252_HIGH[k] == c) { m = (char)(0x80 + k); break; }
            if (m == '?') { if (default_char) m = *default_char; if (used_default) *used_default = 1; }
        }
        out[i] = m;
    }
    return n;
}

DWORD FormatMessageA(DWORD flags, const void *source, DWORD id, DWORD language, char *buffer, DWORD size, va_list *args)
{
    (void)flags; (void)source; (void)language; (void)args;
    if (!buffer || size == 0) return 0;
    int n = snprintf(buffer, size, "Error %u", id);
    return n < 0 ? 0 : (DWORD)n;
}

struct OSVERSIONINFOA { DWORD dwOSVersionInfoSize, dwMajorVersion, dwMinorVersion, dwBuildNumber, dwPlatformId; char szCSDVersion[128]; };
BOOL GetVersionExA(OSVERSIONINFOA *info)
{
    // Windows 2000 (NT 5.0): the newest system the game knows.
    info->dwMajorVersion = 5;
    info->dwMinorVersion = 0;
    info->dwBuildNumber = 2195;
    info->dwPlatformId = 2;  // VER_PLATFORM_WIN32_NT
    info->szCSDVersion[0] = 0;
    return 1;
}

BOOL GetComputerNameA(char *buffer, DWORD *size)
{
    char host[256] = "RA2";
    gethostname(host, sizeof host);
    char *dot = strchr(host, '.');
    if (dot) *dot = 0;
    DWORD n = (DWORD)strlen(host);
    if (!size || *size <= n) { if (size) *size = n + 1; set_last_error(111); return 0; }
    memcpy(buffer, host, n + 1);
    *size = n;
    return 1;
}

void *SetUnhandledExceptionFilter(void *filter) { (void)filter; return nullptr; }
BOOL AllocConsole(void) { return 1; }
BOOL FreeConsole(void) { return 1; }

// Serial ports and modems (the modem/serial game types) are not ported.
BOOL DeviceIoControl(HANDLE h, DWORD code, void *in, DWORD in_size, void *out, DWORD out_size, DWORD *ret, void *ov)
{ (void)h; (void)code; (void)in; (void)in_size; (void)out; (void)out_size; (void)ret; (void)ov; unimplemented("DeviceIoControl"); return 0; }
BOOL GetOverlappedResult(HANDLE h, void *ov, DWORD *n, BOOL wait) { (void)h; (void)ov; (void)n; (void)wait; unimplemented("GetOverlappedResult"); return 0; }
BOOL ClearCommBreak(HANDLE h) { (void)h; unimplemented("ClearCommBreak"); return 0; }
BOOL ClearCommError(HANDLE h, DWORD *e, void *s) { (void)h; (void)e; (void)s; unimplemented("ClearCommError"); return 0; }
BOOL EscapeCommFunction(HANDLE h, DWORD f) { (void)h; (void)f; unimplemented("EscapeCommFunction"); return 0; }
BOOL GetCommMask(HANDLE h, DWORD *m) { (void)h; (void)m; unimplemented("GetCommMask"); return 0; }
BOOL GetCommModemStatus(HANDLE h, DWORD *s) { (void)h; (void)s; unimplemented("GetCommModemStatus"); return 0; }
BOOL GetCommState(HANDLE h, void *d) { (void)h; (void)d; unimplemented("GetCommState"); return 0; }
BOOL PurgeComm(HANDLE h, DWORD f) { (void)h; (void)f; unimplemented("PurgeComm"); return 0; }
BOOL SetCommBreak(HANDLE h) { (void)h; unimplemented("SetCommBreak"); return 0; }
BOOL SetCommState(HANDLE h, void *d) { (void)h; (void)d; unimplemented("SetCommState"); return 0; }
BOOL SetCommTimeouts(HANDLE h, void *t) { (void)h; (void)t; unimplemented("SetCommTimeouts"); return 0; }
BOOL SetupComm(HANDLE h, DWORD i, DWORD o) { (void)h; (void)i; (void)o; unimplemented("SetupComm"); return 0; }

}  // extern "C"
