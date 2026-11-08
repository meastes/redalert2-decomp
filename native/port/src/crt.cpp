// The VC6 C runtime surface the recovered code calls, where libc on LP64 macOS
// differs: UTF-16 wide strings (VC6 wchar_t is 16 bits), printf/scanf whose
// %ld is 32 bits, 32-bit time_t, the VC6 rand() LCG, DOS paths in fopen/_open,
// and the VC6-only names (_stricmp, _itoa, _splitpath, ...). native_units.py
// renames the clashing names to ra2_*; VC6-only names keep their own names.
#include "ra2port/win32_port.hpp"

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <strings.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

using namespace ra2port;
typedef uint16_t wch;

namespace {

// ---- one printf engine for narrow and wide output ----------------------------
// VC6 semantics: l = 32 bits, I64/ll = 64, h = 16; in the wide family %s/%c are
// wide and %S/%C narrow, in the narrow family the reverse; %ls/%ws always wide.
struct Out {
    bool wide;
    std::u16string w;
    std::string n;
    void put(uint32_t c) { if (wide) w.push_back((char16_t)c); else n.push_back(c < 256 ? (char)c : '?'); }
    void puts(const char *s, int len = -1) { for (int i = 0; s[i] && (len < 0 || i < len); ++i) put((unsigned char)s[i]); }
};

template <class C> void format(Out &out, const C *fmt, va_list ap)
{
    for (const C *p = fmt; *p; ++p) {
        if (*p != '%') { out.put((uint16_t)*p); continue; }
        const C *start = p++;
        if (*p == '%') { out.put('%'); continue; }
        std::string spec = "%";
        while (*p && strchr("-+ #0", (char)*p)) spec += (char)*p++;
        if (*p == '*') { spec += std::to_string(va_arg(ap, int)); ++p; }
        while (*p >= '0' && *p <= '9') spec += (char)*p++;
        if (*p == '.') {
            spec += (char)*p++;
            if (*p == '*') { spec += std::to_string(va_arg(ap, int)); ++p; }
            while (*p >= '0' && *p <= '9') spec += (char)*p++;
        }
        int size = 32;   // 16, 32, 64; wide-string flag for s/c
        bool force_wide = false, force_narrow = false;
        for (;;) {
            if (*p == 'h') { size = 16; force_narrow = true; ++p; }
            else if (*p == 'l') { if (p[1] == 'l') { size = 64; p += 2; } else { force_wide = true; ++p; } }
            else if (*p == 'w') { force_wide = true; ++p; }
            else if (*p == 'I' && p[1] == '6' && p[2] == '4') { size = 64; p += 3; }
            else if (*p == 'L') { ++p; }
            else break;
        }
        char conv = (char)*p;
        if (!conv) { (void)start; break; }
        char buf[512];
        bool family_wide = sizeof(C) == 2;
        switch (conv) {
        case 'd': case 'i': {
            long long v = size == 64 ? va_arg(ap, long long) : (long long)va_arg(ap, int);
            if (size == 16) v = (short)v;
            snprintf(buf, sizeof buf, (spec + "lld").c_str(), v);
            out.puts(buf);
            break;
        }
        case 'u': case 'x': case 'X': case 'o': {
            unsigned long long v = size == 64 ? va_arg(ap, unsigned long long) : (unsigned long long)va_arg(ap, unsigned int);
            if (size == 16) v = (unsigned short)v;
            snprintf(buf, sizeof buf, (spec + "ll" + conv).c_str(), v);
            out.puts(buf);
            break;
        }
        case 'f': case 'F': case 'e': case 'E': case 'g': case 'G': {
            double v = va_arg(ap, double);
            snprintf(buf, sizeof buf, (spec + conv).c_str(), v);
            out.puts(buf);
            break;
        }
        case 'p': {
            void *v = va_arg(ap, void *);
            snprintf(buf, sizeof buf, "%08llX", (unsigned long long)(uintptr_t)v);
            out.puts(buf);
            break;
        }
        case 'c': case 'C': {
            int v = va_arg(ap, int);
            bool wide_arg = force_wide || (!force_narrow && ((conv == 'c') == family_wide));
            out.put(wide_arg ? (uint16_t)v : (unsigned char)v);
            break;
        }
        case 's': case 'S': {
            bool wide_arg = force_wide || (!force_narrow && ((conv == 's') == family_wide));
            // width/precision for strings
            int width = 0, prec = -1;
            bool left = spec.find('-') != std::string::npos;
            size_t dot = spec.find('.');
            width = atoi(spec.c_str() + 1 + (left ? 1 : 0));
            if (dot != std::string::npos) prec = atoi(spec.c_str() + dot + 1);
            std::u16string s;
            if (wide_arg) {
                const wch *v = va_arg(ap, const wch *);
                if (!v) { static const wch null_text[] = {'(', 'n', 'u', 'l', 'l', ')', 0}; v = null_text; }
                for (int i = 0; v[i] && (prec < 0 || i < prec); ++i) s.push_back((char16_t)v[i]);
            } else {
                const char *v = va_arg(ap, const char *);
                if (!v) v = "(null)";
                for (int i = 0; v[i] && (prec < 0 || i < prec); ++i) s.push_back((unsigned char)v[i]);
            }
            int pad = width - (int)s.size();
            if (!left) while (pad-- > 0) out.put(' ');
            for (char16_t c : s) out.put(c);
            if (left) while (pad-- > 0) out.put(' ');
            break;
        }
        case 'n': { int *v = va_arg(ap, int *); *v = (int)(out.wide ? out.w.size() : out.n.size()); break; }
        default: out.put('%'); out.put((uint16_t)conv); break;
        }
    }
}

int emit_narrow(char *dst, size_t cap, const Out &o)
{
    if (cap) {
        size_t n = o.n.size() < cap - 1 ? o.n.size() : cap - 1;
        memcpy(dst, o.n.data(), n);
        dst[n] = 0;
    }
    return (int)o.n.size();
}

int emit_wide(wch *dst, size_t cap, const Out &o)
{
    if (cap) {
        size_t n = o.w.size() < cap - 1 ? o.w.size() : cap - 1;
        memcpy(dst, o.w.data(), n * 2);
        dst[n] = 0;
    }
    return (int)o.w.size();
}

// scanf: rewrite VC6 length modifiers for LP64 libc (%ld -> %d, %I64d -> %lld).
std::string scan_format(const char *f)
{
    std::string out;
    for (const char *p = f; *p; ++p) {
        out += *p;
        if (*p != '%') continue;
        ++p;
        if (*p == '%') { out += '%'; continue; }
        while (*p == '*' || (*p >= '0' && *p <= '9')) out += *p++;
        if (*p == 'l' && p[1] != 'l' && p[1] != 'f' && p[1] != 'e' && p[1] != 'g') { ++p; }
        else if (*p == 'I' && p[1] == '6' && p[2] == '4') { out += "ll"; p += 3; }
        if (!*p) break;
        out += *p;
    }
    return out;
}

std::string narrow(const wch *s)
{
    std::string r;
    for (; s && *s; ++s) r.push_back(*s < 256 ? (char)*s : '?');
    return r;
}

unsigned int vc6_seed = 1;

}  // namespace

extern "C" {

// ---- printf / scanf family -------------------------------------------------

int ra2_vsprintf(char *dst, const char *fmt, va_list ap) { Out o{false}; format(o, fmt, ap); return emit_narrow(dst, (size_t)-1 >> 1, o); }
int ra2_sprintf(char *dst, const char *fmt, ...) { va_list ap; va_start(ap, fmt); int n = ra2_vsprintf(dst, fmt, ap); va_end(ap); return n; }
int ra2__vsnprintf(char *dst, size_t cap, const char *fmt, va_list ap)
{
    Out o{false};
    format(o, fmt, ap);
    if (o.n.size() >= cap) { if (cap) memcpy(dst, o.n.data(), cap); return -1; }   // VC6: no terminator, -1
    return emit_narrow(dst, cap, o);
}
int ra2__snprintf(char *dst, size_t cap, const char *fmt, ...) { va_list ap; va_start(ap, fmt); int n = ra2__vsnprintf(dst, cap, fmt, ap); va_end(ap); return n; }
int ra2_vfprintf(FILE *f, const char *fmt, va_list ap) { Out o{false}; format(o, fmt, ap); return (int)fwrite(o.n.data(), 1, o.n.size(), f); }
int ra2_fprintf(FILE *f, const char *fmt, ...) { va_list ap; va_start(ap, fmt); int n = ra2_vfprintf(f, fmt, ap); va_end(ap); return n; }
int ra2_printf(const char *fmt, ...) { va_list ap; va_start(ap, fmt); int n = ra2_vfprintf(stdout, fmt, ap); va_end(ap); return n; }
int ra2_sscanf(const char *src, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int n = vsscanf(src, scan_format(fmt).c_str(), ap);
    va_end(ap);
    return n;
}

int ra2_vswprintf(wch *dst, const wch *fmt, va_list ap) { Out o{true}; format(o, fmt, ap); return emit_wide(dst, (size_t)-1 >> 2, o); }
int ra2_swprintf(wch *dst, const wch *fmt, ...) { va_list ap; va_start(ap, fmt); int n = ra2_vswprintf(dst, fmt, ap); va_end(ap); return n; }
int ra2__vsnwprintf(wch *dst, size_t cap, const wch *fmt, va_list ap)
{
    Out o{true};
    format(o, fmt, ap);
    if (o.w.size() >= cap) { if (cap) memcpy(dst, o.w.data(), cap * 2); return -1; }
    return emit_wide(dst, cap, o);
}
int ra2__snwprintf(wch *dst, size_t cap, const wch *fmt, ...) { va_list ap; va_start(ap, fmt); int n = ra2__vsnwprintf(dst, cap, fmt, ap); va_end(ap); return n; }
int ra2_swscanf(const wch *src, const wch *fmt, ...)
{
    // Numbers and narrow-able text only (what the game parses).
    va_list ap;
    va_start(ap, fmt);
    int n = vsscanf(narrow(src).c_str(), scan_format(narrow(fmt).c_str()).c_str(), ap);
    va_end(ap);
    return n;
}

// ---- UTF-16 string functions --------------------------------------------------

size_t ra2_wcslen(const wch *s) { return wlen(s); }
wch *ra2_wcscpy(wch *d, const wch *s) { wch *r = d; while ((*d++ = *s++)) {} return r; }
wch *ra2_wcsncpy(wch *d, const wch *s, size_t n)
{
    size_t i = 0;
    for (; i < n && s[i]; ++i) d[i] = s[i];
    for (; i < n; ++i) d[i] = 0;
    return d;
}
wch *ra2_wcscat(wch *d, const wch *s) { ra2_wcscpy(d + wlen(d), s); return d; }
wch *ra2_wcsncat(wch *d, const wch *s, size_t n)
{
    wch *e = d + wlen(d);
    size_t i = 0;
    for (; i < n && s[i]; ++i) e[i] = s[i];
    e[i] = 0;
    return d;
}
int ra2_wcscmp(const wch *a, const wch *b)
{
    while (*a && *a == *b) { ++a; ++b; }
    return (int)*a - (int)*b;
}
int ra2_wcsncmp(const wch *a, const wch *b, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        if (a[i] != b[i] || !a[i]) return (int)a[i] - (int)b[i];
    }
    return 0;
}
static wch wlower(wch c) { return (c >= 'A' && c <= 'Z') || (c >= 0xC0 && c <= 0xDE && c != 0xD7) ? (wch)(c + 32) : c; }
static wch wupper(wch c) { return (c >= 'a' && c <= 'z') || (c >= 0xE0 && c <= 0xFE && c != 0xF7) ? (wch)(c - 32) : c; }
int ra2__wcsicmp(const wch *a, const wch *b)
{
    while (*a && wlower(*a) == wlower(*b)) { ++a; ++b; }
    return (int)wlower(*a) - (int)wlower(*b);
}
int ra2__wcsnicmp(const wch *a, const wch *b, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        if (wlower(a[i]) != wlower(b[i]) || !a[i]) return (int)wlower(a[i]) - (int)wlower(b[i]);
    }
    return 0;
}
wch *ra2__wcsupr(wch *s) { for (wch *p = s; *p; ++p) *p = wupper(*p); return s; }
wch *ra2__wcslwr(wch *s) { for (wch *p = s; *p; ++p) *p = wlower(*p); return s; }
wch *ra2__wcsdup(const wch *s)
{
    size_t n = wlen(s) + 1;
    wch *r = (wch *)malloc(n * 2);
    if (r) memcpy(r, s, n * 2);
    return r;
}
wch *ra2_wcschr(const wch *s, wch c)
{
    for (;; ++s) { if (*s == c) return (wch *)s; if (!*s) return nullptr; }
}
wch *ra2_wcsrchr(const wch *s, wch c)
{
    const wch *r = nullptr;
    for (;; ++s) { if (*s == c) r = s; if (!*s) return (wch *)r; }
}
wch *ra2_wcsstr(const wch *h, const wch *n)
{
    size_t ln = wlen(n);
    if (!ln) return (wch *)h;
    for (; *h; ++h) if (ra2_wcsncmp(h, n, ln) == 0) return (wch *)h;
    return nullptr;
}
size_t ra2_wcscspn(const wch *s, const wch *reject)
{
    size_t i = 0;
    for (; s[i]; ++i) if (ra2_wcschr(reject, s[i])) break;
    return i;
}
wch *ra2_wcspbrk(const wch *s, const wch *accept)
{
    for (; *s; ++s) if (ra2_wcschr(accept, *s)) return (wch *)s;
    return nullptr;
}
wch *ra2_wcstok(wch *s, const wch *delim)
{
    static wch *next;
    if (!s) s = next;
    if (!s) return nullptr;
    while (*s && ra2_wcschr(delim, *s)) ++s;
    if (!*s) { next = nullptr; return nullptr; }
    wch *t = s;
    while (*s && !ra2_wcschr(delim, *s)) ++s;
    if (*s) { *s = 0; next = s + 1; } else next = nullptr;
    return t;
}
int32_t ra2_wcstol(const wch *s, wch **end, int base)
{
    std::string n = narrow(s);
    char *e;
    long v = strtol(n.c_str(), &e, base);
    if (end) *end = (wch *)s + (e - n.c_str());
    return (int32_t)v;
}
uint32_t ra2_wcstoul(const wch *s, wch **end, int base)
{
    std::string n = narrow(s);
    char *e;
    unsigned long v = strtoul(n.c_str(), &e, base);
    if (end) *end = (wch *)s + (e - n.c_str());
    return (uint32_t)v;
}
int ra2__wtoi(const wch *s) { return atoi(narrow(s).c_str()); }
int ra2_towupper(int c) { return wupper((wch)c); }
int ra2_towlower(int c) { return wlower((wch)c); }
int ra2_iswspace(int c) { return c == ' ' || (c >= 9 && c <= 13) || c == 0xA0 || c == 0x3000; }
int ra2_iswdigit(int c) { return c >= '0' && c <= '9'; }
int ra2_iswalpha(int c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= 0xC0 && c != 0xD7 && c != 0xF7); }
size_t ra2_mbstowcs(wch *d, const char *s, size_t n)
{
    size_t len = strlen(s);
    if (!d) return len;
    size_t i = 0;
    for (; i < n && i < len; ++i) d[i] = (unsigned char)s[i];
    if (i < n) d[i] = 0;
    return i;
}
size_t ra2_wcstombs(char *d, const wch *s, size_t n)
{
    size_t len = wlen(s);
    if (!d) return len;
    size_t i = 0;
    for (; i < n && i < len; ++i) d[i] = s[i] < 256 ? (char)s[i] : '?';
    if (i < n) d[i] = 0;
    return i;
}
int ra2__putws(const wch *s) { std::string n = narrow(s); return puts(n.c_str()) < 0 ? -1 : 0; }

// ---- VC6-only narrow helpers ---------------------------------------------------

int _stricmp(const char *a, const char *b) { return strcasecmp(a, b); }
int stricmp(const char *a, const char *b) { return strcasecmp(a, b); }
int _strnicmp(const char *a, const char *b, size_t n) { return strncasecmp(a, b, n); }
int _strcmpi(const char *a, const char *b) { return strcasecmp(a, b); }
char *_strupr(char *s) { for (char *p = s; *p; ++p) *p = (char)toupper((unsigned char)*p); return s; }
char *_strlwr(char *s) { for (char *p = s; *p; ++p) *p = (char)tolower((unsigned char)*p); return s; }
char *_strrev(char *s)
{
    size_t n = strlen(s);
    for (size_t i = 0; i < n / 2; ++i) { char t = s[i]; s[i] = s[n - 1 - i]; s[n - 1 - i] = t; }
    return s;
}
char *_strdup(const char *s) { return strdup(s); }
char *_strncpy(char *d, const char *s, size_t n) { return strncpy(d, s, n); }
char *_itoa(int value, char *out, int radix)
{
    char buf[40];
    unsigned int v = (radix == 10 && value < 0) ? (unsigned int)-(int64_t)value : (unsigned int)value;
    int i = 0;
    do { int d = (int)(v % (unsigned)radix); buf[i++] = (char)(d < 10 ? '0' + d : 'a' + d - 10); v /= (unsigned)radix; } while (v);
    int o = 0;
    if (radix == 10 && value < 0) out[o++] = '-';
    while (i) out[o++] = buf[--i];
    out[o] = 0;
    return out;
}
size_t _mbsnbcnt(const unsigned char *s, size_t n)
{
    size_t i = 0;
    while (i < n && s[i]) ++i;
    return i;
}

void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
{
    const char *p = path;
    if (drive) drive[0] = 0;
    if (p[0] && p[1] == ':') { if (drive) { drive[0] = p[0]; drive[1] = ':'; drive[2] = 0; } p += 2; }
    const char *last = nullptr;
    for (const char *q = p; *q; ++q) if (*q == '\\' || *q == '/') last = q;
    if (dir) {
        size_t n = last ? (size_t)(last - p + 1) : 0;
        memcpy(dir, p, n);
        dir[n] = 0;
    }
    const char *name = last ? last + 1 : p;
    const char *dot = strrchr(name, '.');
    if (fname) {
        size_t n = dot ? (size_t)(dot - name) : strlen(name);
        memcpy(fname, name, n);
        fname[n] = 0;
    }
    if (ext) strcpy(ext, dot ? dot : "");
}

void _makepath(char *out, const char *drive, const char *dir, const char *fname, const char *ext)
{
    out[0] = 0;
    if (drive && *drive) { strncat(out, drive, 1); strcat(out, ":"); }
    if (dir && *dir) {
        strcat(out, dir);
        char last = dir[strlen(dir) - 1];
        if (last != '\\' && last != '/') strcat(out, "\\");
    }
    if (fname) strcat(out, fname);
    if (ext && *ext) { if (*ext != '.') strcat(out, "."); strcat(out, ext); }
}

// ---- low-level files with DOS paths and VC6 flag values ------------------------

int _open(const char *name, int flags, ...)
{
    // VC6 _O_ values: RDONLY 0, WRONLY 1, RDWR 2, APPEND 8, CREAT 0x100, TRUNC 0x200, EXCL 0x400, BINARY 0x8000.
    int f = (flags & 3) == 1 ? O_WRONLY : (flags & 3) == 2 ? O_RDWR : O_RDONLY;
    if (flags & 0x8) f |= O_APPEND;
    if (flags & 0x100) f |= O_CREAT;
    if (flags & 0x200) f |= O_TRUNC;
    if (flags & 0x400) f |= O_EXCL;
    char path[1024];
    if (f & (O_WRONLY | O_RDWR | O_CREAT | O_TRUNC)) host_path_write(name, path, sizeof path);
    else host_path(name, path, sizeof path);
    return open(path, f, 0644);
}
int _read(int fd, void *buffer, unsigned int n) { return (int)read(fd, buffer, n); }
int _close(int fd) { return close(fd); }
int32_t _lseek(int fd, int32_t offset, int origin) { return (int32_t)lseek(fd, offset, origin); }
int32_t _filelength(int fd)
{
    struct stat st;
    return fstat(fd, &st) == 0 ? (int32_t)st.st_size : -1;
}
int _unlink(const char *name)
{
    char path[1024];
    host_path_write(name, path, sizeof path);
    return unlink(path);
}
FILE *ra2_fopen(const char *name, const char *mode)
{
    char path[1024];
    if (mode && (strchr(mode, 'w') || strchr(mode, 'a') || strchr(mode, '+'))) host_path_write(name, path, sizeof path);
    else host_path(name, path, sizeof path);
    return fopen(path, mode);
}

// The x87 control word (VC6 CRT _controlfp): kept as a value only. arm64 has
// no x87; precision/rounding requests (WinMain asks for 64-bit precision,
// 0x300/0x300) do not change native double arithmetic. Default _CW_DEFAULT.
unsigned int _controlfp(unsigned int value, unsigned int mask)
{
    static unsigned int word = 0x0009001Fu;
    word = (word & ~mask) | (value & mask);
    return word;
}

void __amsg_exit(int code)
{
    fprintf(stderr, "runtime error R60%02d\n", code);
    _exit(255);
}

// ---- 32-bit time_t (VC6) and the VC6 rand() LCG -----------------------------

int32_t ra2_time(int32_t *out)
{
    int32_t t = (int32_t)time(nullptr);
    if (out) *out = t;
    return t;
}
struct tm *ra2_localtime(const int32_t *t) { time_t v = *t; return localtime(&v); }
struct tm *ra2_gmtime(const int32_t *t) { time_t v = *t; return gmtime(&v); }
char *ra2_ctime(const int32_t *t) { time_t v = *t; return ctime(&v); }
int32_t ra2_mktime(struct tm *tm) { return (int32_t)mktime(tm); }

// VC6 rand(): holdrand = holdrand * 214013 + 2531011; return bits 16..30.
// (Per-process here; VC6 keeps it per thread in the multithreaded CRT.)
void ra2_srand(unsigned int seed) { vc6_seed = seed; }
int ra2_rand(void)
{
    vc6_seed = vc6_seed * 214013u + 2531011u;
    return (int)((vc6_seed >> 16) & 0x7FFF);
}

}  // extern "C"
