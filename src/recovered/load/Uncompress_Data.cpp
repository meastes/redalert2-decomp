// unsigned long Uncompress_Data(void const *, void *) (0x0053AEB0)
// Westwood's Uncompress_Data (TibSun load.cpp): decodes a block that starts
// with a CompressedFileHeader; returns the decoded size.
#include "shared/compressed_file.hpp"
#include "shared/lcw.hpp"
#include "shared/crt_memory.hpp"

unsigned long __fastcall Uncompress_Data(void const *source, void *destination)
{
    unsigned int skip;
    int method;
    unsigned long size = 0;

    if (!source || !destination) return 0;

    size = ((CompressedFileHeader const *)source)->DecodedBytes;
    skip = ((CompressedFileHeader const *)source)->ExtraBytes;
    method = ((CompressedFileHeader const *)source)->Type;
    source = ((char const *)source) + (int)sizeof(CompressedFileHeader) + (int)skip;

    switch (method) {
    default:
    case 0:
        memmove(destination, (void *)source, size);
        break;
    case 3:
        break;
    case 4:
        LCW_Uncomp((void *)source, (void *)destination, (unsigned int)size);
        break;
    }
    return size;
}
