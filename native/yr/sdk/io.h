// <io.h> for the native YR build (lane Y1): the VC6 low-level file calls as the port supplies them
// (native/port/src/crt.cpp: DOS paths, 32-bit offsets). The POSIX names (open, read, close, lseek)
// are the host's, declared here as <unistd.h>/<fcntl.h> declare them (those headers clash with
// Winsock's gethostname); mingw's io.h would redeclare them with Windows types.
#ifndef YR_NATIVE_IO_H
#define YR_NATIVE_IO_H
#include <stdint.h>
#include <sys/types.h>
#ifdef __cplusplus
extern "C" {
#endif
int _open(const char *name, int flags, ...);
int _read(int fd, void *buffer, unsigned int n);
int _close(int fd);
int32_t _lseek(int fd, int32_t offset, int origin);
int32_t _filelength(int fd);
int32_t filelength(int fd);
int open(const char *name, int flags, ...);
ssize_t read(int fd, void *buffer, size_t n);
int close(int fd);
off_t lseek(int fd, off_t offset, int whence);
#ifndef O_RDONLY
#define O_RDONLY 0x0000
#define O_WRONLY 0x0001
#define O_RDWR 0x0002
#endif
#ifdef __cplusplus
}
#endif
#endif
