// lzopipe.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/lzopipe.json carries the addresses and the rows that
// stay in their own files.


#include "shared/lzo_pipe.hpp"
#include "shared/crt_string.hpp"


// LZOPipe::LZOPipe(CompControl control, int blocksize) (0x0053EB40)
// Allocates the staging and output buffers, each with a safety margin of a
// whole block for data that grows when compressed.

LZOPipe::LZOPipe(CompControl control, int blocksize) :
    Control(control),
    Counter(0),
    Buffer(0),
    Buffer2(0),
    BlockSize(blocksize)
{
    SafetyMargin = BlockSize;
    Buffer = new char[BlockSize + SafetyMargin];
    Buffer2 = new char[BlockSize + SafetyMargin];
    BlockHeader.CompCount = 0xFFFF;
}


// LZOPipe::~LZOPipe() (0x0053EBA0)
// Releases both block buffers.

LZOPipe::~LZOPipe()
{
    delete [] Buffer;
    Buffer = 0;

    delete [] Buffer2;
    Buffer2 = 0;
}


// LZOPipe::Put(void const *source, int length) (0x0053EBE0, virtual slot 4)
// Decompressing: gathers each 4-byte block header and its compressed block,
// then passes the uncompressed block on. Compressing: gathers BlockSize
// bytes, then passes the block header and compressed block on. Returns the
// bytes passed down the chain.

int LZOPipe::Put(void const *source, int length)
{
    if (source == 0 || length < 1) {
        return Pipe::Put(source, length);
    }

    int total = 0;

    if (Control == DECOMPRESS) {
        while (length > 0) {
            // Accumulate the block header first.
            if (BlockHeader.CompCount == 0xFFFF) {
                int count = (length < sizeof(BlockHeader) - Counter) ? length : sizeof(BlockHeader) - Counter;
                memmove(Buffer + Counter, source, count);
                source = (char const *)source + count;
                length -= count;
                Counter += count;

                if (Counter == sizeof(BlockHeader)) {
                    memmove(&BlockHeader, Buffer, sizeof(BlockHeader));
                    Counter = 0;
                }
            }

            // Then the compressed block; uncompress it once complete.
            if (length > 0) {
                int needed = BlockHeader.CompCount - Counter;
                int count = (length < needed) ? length : needed;
                memmove(Buffer + Counter, source, count);
                length -= count;
                source = (char const *)source + count;
                Counter += count;

                if (Counter == BlockHeader.CompCount) {
                    unsigned int uncompressed = sizeof(BlockHeader);
                    lzo1x_decompress((unsigned char *)Buffer, BlockHeader.CompCount, (unsigned char *)Buffer2, &uncompressed, 0);
                    total += Pipe::Put(Buffer2, BlockHeader.UncompCount);
                    Counter = 0;
                    BlockHeader.CompCount = 0xFFFF;
                }
            }
        }
    } else {
        // Complete a partially filled block first.
        if (Counter > 0) {
            int needed = BlockSize - Counter;
            int count = (length < needed) ? length : needed;
            memmove(Buffer + Counter, source, count);
            source = (char const *)source + count;
            length -= count;
            Counter += count;

            if (Counter == BlockSize) {
                unsigned int compressed = sizeof(BlockHeader);
                void *dictionary = new char[64 * 1024];
                lzo1x_1_compress((unsigned char *)Buffer, BlockSize, (unsigned char *)Buffer2, &compressed, dictionary);
                delete [] dictionary;
                BlockHeader.CompCount = (unsigned short)compressed;
                BlockHeader.UncompCount = (unsigned short)BlockSize;
                total += Pipe::Put(&BlockHeader, sizeof(BlockHeader));
                total += Pipe::Put(Buffer2, compressed);
                Counter = 0;
            }
        }

        // Compress whole blocks straight from the source.
        while (length >= BlockSize) {
            unsigned int compressed = 0;
            void *dictionary = new char[64 * 1024];
            lzo1x_1_compress((unsigned char const *)source, BlockSize, (unsigned char *)Buffer2, &compressed, dictionary);
            delete [] dictionary;
            source = (char const *)source + BlockSize;
            length -= BlockSize;
            BlockHeader.CompCount = (unsigned short)compressed;
            BlockHeader.UncompCount = (unsigned short)BlockSize;
            total += Pipe::Put(&BlockHeader, sizeof(BlockHeader));
            total += Pipe::Put(Buffer2, compressed);
        }

        // Keep the remainder for the next block.
        if (length > 0) {
            memmove(Buffer, source, length);
            Counter = length;
        }
    }

    return total;
}


// LZOPipe::Flush() (0x0053EE70, virtual slot 1)
// Passes on whatever partial block or header is held, then flushes the
// chain. Returns the bytes passed down the chain.

int LZOPipe::Flush()
{
    int total = 0;

    if (Counter > 0) {
        if (Control == DECOMPRESS) {
            // An incomplete header is passed on as raw data.
            if (BlockHeader.CompCount == 0xFFFF) {
                total += Pipe::Put(Buffer, Counter);
                Counter = 0;
            }
            if (Counter > 0) {
                total += Pipe::Put(&BlockHeader, sizeof(BlockHeader));
                total += Pipe::Put(Buffer, Counter);
                Counter = 0;
                BlockHeader.CompCount = 0xFFFF;
            }
        } else {
            unsigned int compressed = 0;
            void *dictionary = new char[64 * 1024];
            lzo1x_1_compress((unsigned char *)Buffer, Counter, (unsigned char *)Buffer2, &compressed, dictionary);
            delete [] dictionary;
            BlockHeader.CompCount = (unsigned short)compressed;
            BlockHeader.UncompCount = (unsigned short)Counter;
            total += Pipe::Put(&BlockHeader, sizeof(BlockHeader));
            total += Pipe::Put(Buffer2, compressed);
            Counter = 0;
        }
    }
    total += Pipe::Flush();
    return total;
}


// LZOPipe scalar deleting destructor (0x0053EF60, virtual slot 0)
// Compiler generated with the LZOPipe vtable.
