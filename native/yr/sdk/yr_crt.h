// The VC6 C runtime the YR tree calls, as the port supplies it (native/port/src/crt.cpp; lane Y1).
// The RA2 tree's port rewrites (native_units.py) rename the calls whose VC6 contract differs from
// the host libc's to ra2_ names (printf family %ld, rand, fopen, time_t, overlapping strcpy/memcpy,
// UTF-16 wide strings); these are their declarations, and the Windows-only names beside them.
#ifndef YR_NATIVE_CRT_H
#define YR_NATIVE_CRT_H
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#ifdef __cplusplus
extern "C" {
#endif
int ra2_vsprintf(char *dst, const char *fmt, va_list ap);
int ra2_sprintf(char *dst, const char *fmt, ...);
int ra2__vsnprintf(char *dst, size_t cap, const char *fmt, va_list ap);
int ra2__snprintf(char *dst, size_t cap, const char *fmt, ...);
int ra2_vfprintf(FILE *f, const char *fmt, va_list ap);
int ra2_fprintf(FILE *f, const char *fmt, ...);
int ra2_printf(const char *fmt, ...);
int ra2_sscanf(const char *src, const char *fmt, ...);
int ra2_vswprintf(unsigned short *dst, const unsigned short *fmt, va_list ap);
int ra2_swprintf(unsigned short *dst, const unsigned short *fmt, ...);
int ra2__vsnwprintf(unsigned short *dst, size_t cap, const unsigned short *fmt, va_list ap);
int ra2__snwprintf(unsigned short *dst, size_t cap, const unsigned short *fmt, ...);
int ra2_swscanf(const unsigned short *src, const unsigned short *fmt, ...);
size_t ra2_wcslen(const unsigned short *s);
unsigned short *ra2_wcscpy(unsigned short *d, const unsigned short *s);
unsigned short *ra2_wcsncpy(unsigned short *d, const unsigned short *s, size_t n);
unsigned short *ra2_wcscat(unsigned short *d, const unsigned short *s);
unsigned short *ra2_wcsncat(unsigned short *d, const unsigned short *s, size_t n);
int ra2_wcscmp(const unsigned short *a, const unsigned short *b);
int ra2_wcsncmp(const unsigned short *a, const unsigned short *b, size_t n);
int ra2__wcsicmp(const unsigned short *a, const unsigned short *b);
int ra2__wcsnicmp(const unsigned short *a, const unsigned short *b, size_t n);
unsigned short *ra2__wcsupr(unsigned short *s);
unsigned short *ra2__wcslwr(unsigned short *s);
unsigned short *ra2__wcsdup(const unsigned short *s);
unsigned short *ra2_wcschr(const unsigned short *s, unsigned short c);
unsigned short *ra2_wcsrchr(const unsigned short *s, unsigned short c);
unsigned short *ra2_wcsstr(const unsigned short *h, const unsigned short *n);
size_t ra2_wcscspn(const unsigned short *s, const unsigned short *reject);
unsigned short *ra2_wcspbrk(const unsigned short *s, const unsigned short *accept);
unsigned short *ra2_wcstok(unsigned short *s, const unsigned short *delim);
int32_t ra2_wcstol(const unsigned short *s, unsigned short **end, int base);
uint32_t ra2_wcstoul(const unsigned short *s, unsigned short **end, int base);
int ra2__wtoi(const unsigned short *s);
int ra2_towupper(int c);
int ra2_towlower(int c);
int ra2_iswspace(int c);
int ra2_iswdigit(int c);
int ra2_iswalpha(int c);
size_t ra2_mbstowcs(unsigned short *d, const char *s, size_t n);
size_t ra2_wcstombs(char *d, const unsigned short *s, size_t n);
int ra2__putws(const unsigned short *s);
int _stricmp(const char *a, const char *b);
int stricmp(const char *a, const char *b);
int _strnicmp(const char *a, const char *b, size_t n);
int _strcmpi(const char *a, const char *b);
char *_strupr(char *s);
char *_strlwr(char *s);
char *_strrev(char *s);
char *_strdup(const char *s);
char *_strncpy(char *d, const char *s, size_t n);
char *_itoa(int value, char *out, int radix);
void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext);
void _makepath(char *out, const char *drive, const char *dir, const char *fname, const char *ext);
int _open(const char *name, int flags, ...);
int _read(int fd, void *buffer, unsigned int n);
int _close(int fd);
int32_t _lseek(int fd, int32_t offset, int origin);
int32_t _filelength(int fd);
FILE *ra2_fopen(const char *name, const char *mode);
int32_t ra2_time(int32_t *out);
struct tm *ra2_localtime(const int32_t *t);
struct tm *ra2_gmtime(const int32_t *t);
char *ra2_ctime(const int32_t *t);
int32_t ra2_mktime(struct tm *tm);
void ra2_srand(unsigned int seed);
int ra2_rand(void);
char *ra2_strcpy(char *dst, const char *src);
char *ra2_strncpy(char *dst, const char *src, size_t count);
void *ra2_memcpy(void *dst, const void *src, size_t count);
char *ra2_strcat(char *dst, const char *src);
// Winsock as the port supplies it (native/port/src/wsock32.cpp: SOCKET is the Win32 32-bit
// handle; the RA2 tree's port rewrites send these calls to the ra2_ names).
unsigned int ra2_socket(int family, int type, int protocol);
int ra2_bind(unsigned int s, const void *address, int length);
int ra2_sendto(unsigned int s, const char *buffer, int length, int flags, const void *to, int tolength);
int ra2_recvfrom(unsigned int s, char *buffer, int length, int flags, void *from, int *fromlength);
int ra2_setsockopt(unsigned int s, int level, int name, const char *value, int length);
int ra2_getsockopt(unsigned int s, int level, int name, char *value, int *length);
int ra2_gethostname(char *name, int length);
struct hostent *ra2_gethostbyname(const char *name);
char *ra2_inet_ntoa(uint32_t addr);
uint32_t ra2_htonl(uint32_t v);
uint32_t ra2_ntohl(uint32_t v);
uint16_t ra2_htons(uint16_t v);
uint16_t ra2_ntohs(uint16_t v);
// The VC6 names without the underscore (native/yr/glue/yr_crt.cpp: the port's underscored forms).
char *strupr(char *s);
char *strlwr(char *s);
int strnicmp(const char *a, const char *b, size_t n);
char *strrev(char *s);
char *itoa(int value, char *out, int radix);
// Secure CRT forms the mingw COM headers call (comdef.h _com_error::ErrorMessage).
int sprintf_s(char *dst, size_t cap, const char *fmt, ...);
int swprintf_s(unsigned short *dst, size_t cap, const unsigned short *fmt, ...);
#ifdef __cplusplus
}
#endif
// The wide forms the port spells with the underscore.
#define ra2_wcsicmp ra2__wcsicmp
#define ra2_wcsnicmp ra2__wcsnicmp
#define ra2_wcsupr ra2__wcsupr
#define ra2_wcslwr ra2__wcslwr
#define O_BINARY 0
// VC6 <ctype.h> character-class bits (the YR tree tests them against _ctype tables or constants).
#define _UPPER 0x1
#define _LOWER 0x2
#define _DIGIT 0x4
#define _SPACE 0x8
#define _PUNCT 0x10
#define _CONTROL 0x20
#define _BLANK 0x40
#define _HEX 0x80
#define _MAX_PATH 260
#define _MAX_DRIVE 3
#define _MAX_DIR 256
#define _MAX_FNAME 256
#define _MAX_EXT 256
#endif
