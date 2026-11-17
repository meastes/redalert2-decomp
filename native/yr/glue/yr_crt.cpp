// The VC6 C runtime names the YR tree calls that the port spells with an underscore, and the
// secure forms the mingw COM headers call (lane Y1; native/yr/sdk/yr_crt.h declares them).
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
extern "C" {
char *_strupr(char *s);
char *_strlwr(char *s);
int _strnicmp(const char *a, const char *b, size_t n);
char *_strrev(char *s);
char *_itoa(int value, char *out, int radix);
int ra2__vsnprintf(char *dst, size_t cap, const char *fmt, va_list ap);
int ra2__vsnwprintf(unsigned short *dst, size_t cap, const unsigned short *fmt, va_list ap);

char *strupr(char *s) { return _strupr(s); }
char *strlwr(char *s) { return _strlwr(s); }
int strnicmp(const char *a, const char *b, size_t n) { return _strnicmp(a, b, n); }
char *strrev(char *s) { return _strrev(s); }
char *itoa(int value, char *out, int radix) { return _itoa(value, out, radix); }
int sprintf_s(char *dst, size_t cap, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int n = ra2__vsnprintf(dst, cap, fmt, ap);
    va_end(ap);
    if (cap) dst[cap - 1] = 0;
    return n;
}
int swprintf_s(unsigned short *dst, size_t cap, const unsigned short *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    int n = ra2__vsnwprintf(dst, cap, fmt, ap);
    va_end(ap);
    if (cap) dst[cap - 1] = 0;
    return n;
}
}

extern "C" {
int32_t _filelength(int fd);
int32_t filelength(int fd) { return _filelength(fd); }
size_t _mbsnccnt(const unsigned char *s, size_t n)
{
    size_t count = 0;
    while (count < n && s[count]) ++count;
    return count;
}
}
