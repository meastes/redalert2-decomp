/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/lzostraw.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// lzostraw.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/lzostraw.json carries the addresses and the rows that
// stay in their own files.


#include "shared/lzo_straw.hpp"
#include "shared/crt_string.hpp"


/***********************************************************************************************
 * LZOStraw::LZOStraw -- Constructor for LZO straw object.                                     *
 *                                                                                             *
 *    This will initialize the LZO straw object. Whether the object is to compress or          *
 *    decompress and the block size to use is specified. The data is compressed in blocks      *
 *    that are sized to be quick to compress and yet still yield good compression ratios.      *
 *                                                                                             *
 * INPUT:   decrypt  -- Should the data be decompressed?                                       *
 *                                                                                             *
 *          blocksize-- The size of the blocks to process.                                     *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   It takes two buffers of the blocksize specified if compression is to be         *
 *             performed.                                                                      *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053EFB0)
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


/***********************************************************************************************
 * LZOStraw::~LZOStraw -- Destructor for the LZO straw.                                        *
 *                                                                                             *
 *    The destructor will free up the allocated buffers that it allocated in the constructor.  *
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
// (0x0053F010)
// Releases both block buffers.

LZOStraw::~LZOStraw()
{
    delete [] Buffer;
    Buffer = 0;

    delete [] Buffer2;
    Buffer2 = 0;
}


/***********************************************************************************************
 * LZOStraw::Get -- Fetch data through the LZO processor.                                      *
 *                                                                                             *
 *    This routine will fetch the data bytes specified. It does this by first accumulating     *
 *    a full block of data and then compressing or decompressing it as indicated. Subsequent   *
 *    requests for data will draw from this buffer of processed data until it is exhausted     *
 *    and another block must be fetched.                                                       *
 *                                                                                             *
 * INPUT:   destbuf  -- Pointer to the buffer to hold the data requested.                      *
 *                                                                                             *
 *          length   -- The number of data bytes requested.                                    *
 *                                                                                             *
 * OUTPUT:  Returns with the actual number of bytes stored into the buffer. If this number     *
 *          is less than that requested, then this indicates that the data source has been     *
 *          exhausted.                                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/04/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x0053F050, virtual slot 2)
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
