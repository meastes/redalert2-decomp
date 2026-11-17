// The voxel draw cache's static buffer (0x00839AA0, 4,000,000 bytes): its definition emits the
// dynamic initializer 0x0040F070 and the atexit destructor 0x0040F090.
#define STATIC_BUFFER_DECLARE_LIFETIME
#include "shared/static_buffer.hpp"

StaticBufferClass VoxelStaticBuffer(4000000);
