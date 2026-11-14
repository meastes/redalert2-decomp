// lzostraw.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/lzostraw.json carries the addresses and the rows that
// stay in their own files.


#include "shared/lzo_straw.hpp"
#include "shared/crt_string.hpp"


// LZOStraw::LZOStraw(CompControl control, int blocksize) (0x0053EFB0)
// Allocates the block buffer; the compressed-block buffer is needed only
// when compressing.

LZOStraw::LZOStraw(CompControl control, int blocksize) :
    Control(control),
    Counter(0),
    Buffer(0),
    Buffer2(0),
    BlockSize(blocksize)
{
    SafetyMargin = BlockSize;
    Buffer = new char[BlockSize + SafetyMargin];
    if (control == COMPRESS) {
        Buffer2 = new char[BlockSize + SafetyMargin];
    }
}


// LZOStraw::~LZOStraw() (0x0053F010)
// Releases both block buffers.

LZOStraw::~LZOStraw()
{
    delete [] Buffer;
    Buffer = 0;

    delete [] Buffer2;
    Buffer2 = 0;
}


// LZOStraw::Get(void *buffer, int length) (0x0053F050, virtual slot 2)
// Hands out processed bytes, refilling a block at a time from the chained
// straw: decompressing reads a block header and its compressed block (into a
// temporary buffer) and uncompresses it into Buffer; compressing reads
// BlockSize bytes and compresses them behind a header in Buffer2. Returns
// the bytes delivered.

int LZOStraw::Get(void *buffer, int length)
{
    int total = 0;

    if (buffer == 0 || length < 1) {
        return 0;
    }

    while (length > 0) {
        if (Counter > 0) {
            int count = (length < Counter) ? length : Counter;
            if (Control == DECOMPRESS) {
                memmove(buffer, &Buffer[BlockHeader.UncompCount - Counter], count);
            } else {
                memmove(buffer, &Buffer2[(BlockHeader.CompCount + sizeof(BlockHeader)) - Counter], count);
            }
            buffer = (char *)buffer + count;
            length -= count;
            Counter -= count;
            total += count;
        }
        if (length == 0) {
            break;
        }

        if (Control == DECOMPRESS) {
            int incount = Straw::Get(&BlockHeader, sizeof(BlockHeader));
            if (incount != sizeof(BlockHeader)) {
                break;
            }

            char *staging = new char[BlockHeader.CompCount];
            incount = Straw::Get(staging, BlockHeader.CompCount);
            if (incount != BlockHeader.CompCount) {
                if (staging) {
                    delete [] staging;
                }
                break;
            }

            unsigned int uncompressed = sizeof(BlockHeader);
            lzo1x_decompress((unsigned char *)staging, BlockHeader.CompCount, (unsigned char *)Buffer, &uncompressed, 0);
            delete [] staging;
            Counter = BlockHeader.UncompCount;
        } else {
            BlockHeader.UncompCount = (unsigned short)Straw::Get(Buffer, BlockSize);
            if (BlockHeader.UncompCount == 0) {
                break;
            }
            void *dictionary = new char[64 * 1024];
            unsigned int compressed = 0;
            lzo1x_1_compress((unsigned char *)Buffer, BlockHeader.UncompCount, (unsigned char *)&Buffer2[sizeof(BlockHeader)], &compressed, dictionary);
            BlockHeader.CompCount = (unsigned short)compressed;
            delete [] dictionary;
            memmove(Buffer2, &BlockHeader, sizeof(BlockHeader));
            Counter = BlockHeader.CompCount + sizeof(BlockHeader);
        }
    }

    return total;
}


// LZOStraw scalar deleting destructor (0x0053F220, virtual slot 0)
// Compiler generated with the LZOStraw vtable.
