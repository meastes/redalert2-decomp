// macOS/iOS implementations of the port's operating-system services (os.hpp).
// The only port file with Apple-only calls (Scout1209).
#if defined(__APPLE__)
#include "os.hpp"
#include <mach/mach_time.h>
#include <sys/mount.h>
#include <sys/sysctl.h>
#include <sys/ucontext.h>
#include <string.h>

namespace ra2port {
namespace os {

uint64_t monotonic_ns()
{
    static mach_timebase_info_data_t base;
    if (base.denom == 0) mach_timebase_info(&base);
    return mach_absolute_time() * base.numer / base.denom;
}

uint64_t thread_id()
{
    uint64_t id = 0;
    pthread_threadid_np(nullptr, &id);
    return id;
}

uint64_t thread_id(pthread_t thread)
{
    uint64_t id = 0;
    pthread_threadid_np(thread, &id);
    return id;
}

FileTimes file_times(const struct stat &st)
{
    return FileTimes{st.st_atimespec, st.st_mtimespec, st.st_birthtimespec};
}

bool volume(const char *path, Volume *out)
{
    struct statfs st;
    if (statfs(path, &st) != 0) return false;
    out->mount_point = st.f_mntonname;
    out->serial = (uint32_t)st.f_fsid.val[0] ^ (uint32_t)st.f_fsid.val[1];
    out->type = st.f_fstypename;
    out->free_bytes = (uint64_t)st.f_bavail * st.f_bsize;
    out->total_bytes = (uint64_t)st.f_blocks * st.f_bsize;
    return true;
}

uint64_t physical_memory()
{
    uint64_t phys = 0;
    size_t len = sizeof phys;
    if (sysctlbyname("hw.memsize", &phys, &len, nullptr, 0) != 0) return 0;
    return phys;
}

bool fault_registers(void *context, uintptr_t *pc, uintptr_t *lr, uintptr_t *fp)
{
#if defined(__arm64__)
    if (!context) return false;
    const ucontext_t *uc = (const ucontext_t *)context;
    *pc = (uintptr_t)uc->uc_mcontext->__ss.__pc;
    *lr = (uintptr_t)uc->uc_mcontext->__ss.__lr;
    *fp = (uintptr_t)uc->uc_mcontext->__ss.__fp;
    return true;
#else
    (void)context; (void)pc; (void)lr; (void)fp;
    return false;
#endif
}

}  // namespace os
}  // namespace ra2port
#endif
