/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/lcwpipe.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/lcwpipe.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/lcwpipe.cpp where both functions declare the same local types (names only).

// lcwpipe.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/lcwpipe.json carries the addresses and the rows that
// stay in their own files.


#include "shared/lcw_pipe.hpp"
#include "shared/crt_string.hpp"


/***********************************************************************************************
 * LCWPipe::LCWPipe -- Constructor for the LCW processor pipe.                                 *
 *                                                                                             *
 *    This will initialize the LCWPipe object so that it is prepared for compression or        *
 *    decompression as indicated.                                                              *
 *                                                                                             *
 * INPUT:   decrypt  -- Should decompression be performed?                                     *
 *                                                                                             *
 *          blocksize-- The size of the data blocks to process.                                *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00534EE0)
// Allocates the staging and output buffers with a safety margin of at least
// 128 bytes for blocks that grow when compressed.

LCWPipe::LCWPipe(CompControl control, int blocksize) :
    Control(control),
    Counter(0),
    Buffer(0),
    Buffer2(0),
    BlockSize(blocksize)
{
    int margin = BlockSize / 128 + 1;
    if (margin < 128) {
        margin = 128;
    }
    SafetyMargin = margin;
    Buffer = new char[BlockSize + SafetyMargin];
    Buffer2 = new char[BlockSize + SafetyMargin];
    BlockHeader.CompCount = 0xFFFF;
}


/***********************************************************************************************
 * LCWPipe::~LCWPipe -- Deconstructor for the LCW pipe object.                                 *
 *                                                                                             *
 *    This will free any buffers it may have allocated.                                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00534F50)
// Releases both block buffers.

LCWPipe::~LCWPipe()
{
    delete [] Buffer;
    Buffer = 0;

    delete [] Buffer2;
    Buffer2 = 0;
}


/***********************************************************************************************
 * LCWPipe::Put -- Send some data through the LCW processor pipe.                              *
 *                                                                                             *
 *    This routine will take the data requested and process it (decompression or compression). *
 *    It does this by accumulating the necessary bytes to make a whole block. Then the block   *
 *    is processed and the entire contents are flushed to the next pipe segment in the chain.  *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the data to be fed to this LCW processor.                   *
 *                                                                                             *
 *          length   -- The number of bytes received.                                          *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of bytes output at the far distant final link in    *
 *          the pipe chain.                                                                    *
 *                                                                                             *
 * WARNINGS:   The compression process may be slow as well as consuming two buffers.           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00534F90, virtual slot 4)
// Decompressing: gathers each 4-byte block header and its compressed block,
// then passes the uncompressed block on. Compressing: gathers BlockSize
// bytes, then passes the block header and compressed block on. Returns the
// bytes passed down the chain.

int LCWPipe::Put(void const *source, int slen)
{
    if (source == 0 || slen < 1) {
        return Pipe::Put(source, slen);
    }

    int total = 0;

    if (Control == DECOMPRESS) {
        while (slen > 0) {
            // Accumulate the block header first.
            if (BlockHeader.CompCount == 0xFFFF) {
                int count = (slen < sizeof(BlockHeader) - Counter) ? slen : sizeof(BlockHeader) - Counter;
                memmove(Buffer + Counter, source, count);
                source = (char const *)source + count;
                slen -= count;
                Counter += count;

                if (Counter == sizeof(BlockHeader)) {
                    memmove(&BlockHeader, Buffer, sizeof(BlockHeader));
                    Counter = 0;
                }
            }

            // Then the compressed block; uncompress it once complete.
            if (slen > 0) {
                int needed = BlockHeader.CompCount - Counter;
                int count = (slen < needed) ? slen : needed;
                memmove(Buffer + Counter, source, count);
                slen -= count;
                source = (char const *)source + count;
                Counter += count;

                if (Counter == BlockHeader.CompCount) {
                    LCW_Uncomp(Buffer, Buffer2);
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
            int count = (slen < needed) ? slen : needed;
            memmove(Buffer + Counter, source, count);
            Counter += count;
            source = (char const *)source + count;
            slen -= count;

            if (Counter == BlockSize) {
                int compressed = LCW_Comp(Buffer, Buffer2, BlockSize);
                BlockHeader.CompCount = (unsigned short)compressed;
                BlockHeader.UncompCount = (unsigned short)BlockSize;
                total += Pipe::Put(&BlockHeader, sizeof(BlockHeader));
                total += Pipe::Put(Buffer2, compressed);
                Counter = 0;
            }
        }

        // Compress whole blocks straight from the source.
        while (slen >= BlockSize) {
            int compressed = LCW_Comp(source, Buffer2, BlockSize);
            source = (char const *)source + BlockSize;
            slen -= BlockSize;
            BlockHeader.CompCount = (unsigned short)compressed;
            BlockHeader.UncompCount = (unsigned short)BlockSize;
            total += Pipe::Put(&BlockHeader, sizeof(BlockHeader));
            total += Pipe::Put(Buffer2, compressed);
        }

        // Keep the remainder for the next block.
        if (slen > 0) {
            memmove(Buffer, source, slen);
            Counter = slen;
        }
    }

    return total;
}


/***********************************************************************************************
 * LCWPipe::Flush -- Flushes any partially accumulated block.                                  *
 *                                                                                             *
 *    This routine is called when any buffered data must be flushed out the pipe. For the      *
 *    compression process, this will generate the sub-sized compressed block. For              *
 *    decompression, this routine should not have any data in the buffer. In such a case, it   *
 *    means that the data source was prematurely truncated. In such a case, just dump the      *
 *    accumulated data through the pipe.                                                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of data bytes output to the distant final link in   *
 *          the pipe chain.                                                                    *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x005351C0, virtual slot 1)
// Passes on whatever partial block or header is held, then flushes the
// chain. Returns the bytes passed down the chain.

int LCWPipe::Flush()
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
            int len = LCW_Comp(Buffer, Buffer2, Counter);
            BlockHeader.CompCount = (unsigned short)len;
            BlockHeader.UncompCount = (unsigned short)Counter;
            total += Pipe::Put(&BlockHeader, sizeof(BlockHeader));
            total += Pipe::Put(Buffer2, len);
            Counter = 0;
        }
    }
    total += Pipe::Flush();
    return total;
}


// LCWPipe scalar deleting destructor (0x00535280, virtual slot 0)
// Compiler generated with the LCWPipe vtable.
