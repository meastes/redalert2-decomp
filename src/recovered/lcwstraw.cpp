// lcwstraw.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/lcwstraw.json carries the addresses and the rows that
// stay in their own files.


#include "shared/lcw_straw.hpp"
#include "shared/crt_string.hpp"


// LCWStraw::LCWStraw(CompControl control, int blocksize) (0x005352D0)
// Allocates the block buffer; the compressed-block buffer is needed only
// when compressing.

LCWStraw::LCWStraw(CompControl control, int blocksize) :
    Control(control),
    Counter(0),
    Buffer(0),
    Buffer2(0),
    BlockSize(blocksize)
{
    SafetyMargin = BlockSize / 128 + 1;
    Buffer = new char[BlockSize + SafetyMargin];
    if (control == COMPRESS) {
        Buffer2 = new char[BlockSize + SafetyMargin];
    }
}


// LCWStraw::~LCWStraw() (0x00535340)
// Releases both block buffers.

LCWStraw::~LCWStraw()
{
    delete [] Buffer;
    Buffer = 0;

    delete [] Buffer2;
    Buffer2 = 0;
}


// LCWStraw::Get(void *buffer, int length) (0x00535380, virtual slot 2)
// Hands out processed bytes, refilling a block at a time from the chained
// straw: decompressing reads a block header and its compressed block (placed
// at the end of Buffer) and uncompresses it into Buffer; compressing reads
// BlockSize bytes and compresses them behind a header in Buffer2. Returns
// the bytes delivered.

int LCWStraw::Get(void *buffer, int length)
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

            char *staging = &Buffer[(BlockSize + SafetyMargin) - BlockHeader.CompCount];
            incount = Straw::Get(staging, BlockHeader.CompCount);
            if (incount != BlockHeader.CompCount) {
                break;
            }

            LCW_Uncomp(staging, Buffer);
            Counter = BlockHeader.UncompCount;
        } else {
            BlockHeader.UncompCount = (unsigned short)Straw::Get(Buffer, BlockSize);
            if (BlockHeader.UncompCount == 0) {
                break;
            }
            BlockHeader.CompCount = (unsigned short)LCW_Comp(Buffer, &Buffer2[sizeof(BlockHeader)], BlockHeader.UncompCount);
            memmove(Buffer2, &BlockHeader, sizeof(BlockHeader));
            Counter = BlockHeader.CompCount + sizeof(BlockHeader);
        }
    }

    return total;
}


// LCWStraw scalar deleting destructor (0x005354E0, virtual slot 0)
// Compiler generated with the LCWStraw vtable.
