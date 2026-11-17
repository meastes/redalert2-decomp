// vqalib_dstream.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/vqalib_dstream.json carries the addresses and the rows that
// stay in their own files.


#include "shared/vqa_player.hpp"
#include "shared/crt_io.hpp"
#include "shared/crt_memory.hpp"


// int __cdecl Disk_VQA_Stream_Handler(VQAHandle *vqa, int action, int buffer, int nbytes) -- 0x00778450
// Purpose: the default stream handler: serves the player's VQACMD_* requests
// from the file handle kept in Config.StreamFileHandle with the CRT's
// low-level file calls. Nonzero means the request failed.

int __cdecl Disk_VQA_Stream_Handler(VQAHandle *vqa, int action, int buffer, int nbytes)
{
    int fh;
    int error = 0;
    int temp;

    fh = ((VQAHandleP *)vqa)->Config.StreamFileHandle;

    switch (action) {
    case VQACMD_OPEN:
        error = _open((char *)buffer, _O_RDONLY | _O_BINARY);
        if (error != -1) {
            ((VQAHandleP *)vqa)->Config.StreamFileHandle = error;
            error = 0;
        }
        break;

    case VQACMD_READ:
        error = (_read(fh, (void *)buffer, nbytes) != nbytes);
        break;

    case VQACMD_WRITE:
        error = 1;
        break;

    case VQACMD_SEEK:
        error = (_lseek(fh, nbytes, buffer) == -1);
        break;

    case VQACMD_SEEKPEEK:
        if (nbytes > 0) {
            error = _lseek(fh, nbytes - 1, buffer) == -1;
            if (error == 0) {
                error = _read(fh, &temp, 1) != 1;
            }
        } else {
            error = _lseek(fh, nbytes, buffer) == -1;
            if (error == 0) {
                error = _read(fh, &temp, 1) != 1;
            }
            if (error == 0) {
                error = _lseek(fh, -1, 1) == -1;
            }
        }
        break;

    case VQACMD_SIZE:
        *((unsigned int *)buffer) = _filelength(fh);
        error = 0;
        break;

    case VQACMD_CLOSE:
        _close(fh);
        error = 0;
        break;

    case VQACMD_INIT:
        error = 0;
        break;

    case VQACMD_CLEANUP:
        error = 0;
        break;
    }

    return error;
}


// int __cdecl Memory_VQA_Stream_Handler(VQAHandle *vqa, int action, int buffer, int nbytes) -- 0x007785B0
// Purpose: the stream handler for a movie held in memory (the loop cache):
// reads copy from LoopCache.Ptr at Offset, seeks move Offset.

int __cdecl Memory_VQA_Stream_Handler(VQAHandle *vqa, int action, int buffer, int nbytes)
{
    int error = 0;
    int p;
    int bytes;
    VQAHandleP *vqap = (VQAHandleP *)vqa;
    VQALoopCache *cache = &vqap->LoopCache;

    switch (action) {
    case VQACMD_OPEN:
        error = 0;
        break;

    case VQACMD_READ:
        bytes = cache->Bytes;
        p = cache->Offset;
        if (p + nbytes <= bytes) {
            memcpy((void *)buffer, &cache->Ptr[p], nbytes);
            cache->Offset += nbytes;
            error = 0;
            break;
        }
        error = 1;
        break;

    case VQACMD_SEEK:
    case VQACMD_SEEKPEEK:
        switch (buffer) {
        case 1:
            cache->Offset += nbytes;
            error = 0;
            break;

        case 0:
            p = (int)cache->Buffer;
            if (nbytes >= p) {
                cache->Offset = nbytes - p;
                break;
            }

        default:
            error = 1;
            break;
        }
        break;

    case VQACMD_SIZE:
        error = 0;
        break;

    case VQACMD_CLOSE:
        error = 0;
        break;

    case VQACMD_INIT:
    case VQACMD_CLEANUP:
        error = 0;
        break;

    case VQACMD_WRITE:
        error = 1;
        break;

    default:
        break;
    }

    return error;
}
