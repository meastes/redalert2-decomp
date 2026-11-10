// The port's operating-system services that are not standard C++ or POSIX
// (Scout1209, standing portability rule: Apple-only calls live in one file per
// layer). os_apple.cpp implements them on macOS (and iOS); a Windows or other
// port supplies its own os_<platform>.cpp with the same functions.
#ifndef RA2PORT_OS_HPP
#define RA2PORT_OS_HPP

#include <pthread.h>
#include <stdint.h>
#include <string>
#include <sys/stat.h>
#include <time.h>

namespace ra2port {
namespace os {

uint64_t monotonic_ns();                  // a monotonic clock in nanoseconds
uint64_t thread_id();                     // the calling thread's system-wide id
uint64_t thread_id(pthread_t thread);     // another thread's id

struct FileTimes { struct timespec accessed, modified, created; };
FileTimes file_times(const struct stat &st);

// The volume holding path: its mount point, a serial number from its file-system
// id, its file-system type name, and its free and total bytes.
struct Volume { std::string mount_point; uint32_t serial; std::string type; uint64_t free_bytes, total_bytes; };
bool volume(const char *path, Volume *out);

uint64_t physical_memory();               // bytes of RAM (0 if unknown)

// A signal handler's context: the faulting pc, link register and frame
// pointer (false where the platform's layout is not known).
bool fault_registers(void *context, uintptr_t *pc, uintptr_t *lr, uintptr_t *fp);

}  // namespace os
}  // namespace ra2port

#endif
