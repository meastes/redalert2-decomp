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
// Function comment blocks from mischa85/RedAlert2 392e904 src/xpipe.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/xpipe.cpp where both define the function (names only).

// xpipe.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/xpipe.json carries the addresses and the rows that
// stay in their own files.


#include "shared/file_pipe.hpp"


/// <summary>
/// Closes the attached file if this pipe was the one that opened it.
/// A file that was already open when it was submitted to the pipe is left untouched, since
/// whoever opened it stays responsible for closing it. A pipe chain should call End()
/// rather than rely on this routine, because destructor order between the chained pipes is
/// not easily controlled.
/// </summary>
// (0x00773790)
// Closes the file if this pipe opened it.
// The scalar deleting destructor the linker kept was compiled in another
// object file, where this destructor is called rather than inlined;
// auto_inline(off) reproduces that copy here.
#pragma auto_inline(off)
FilePipe::~FilePipe()
{
    if (Valid_File() && HasOpened) {
        HasOpened = false;
        File->Close();
        File = 0;
    }
}
#pragma auto_inline(on)


/***********************************************************************************************
 * FilePipe::End -- End the file pipe handler.                                                 *
 *                                                                                             *
 *    This routine is called when there will be no more data sent through the pipe. It is      *
 *    responsible for cleaning up anything it needs to. This is not handled by the             *
 *    destructor, although it serves a similar purpose, because pipe are linked together and   *
 *    the destructor order is not easily controlled. If the destructors for a pipe chain were  *
 *    called out of order, the result might be less than pleasant.                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes flushed out the final end of the pipe as a        *
 *          consequence of this routine.                                                       *
 *                                                                                             *
 * WARNINGS:   Don't send any more data through the pipe after this routine is called.         *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/05/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x007737C0, virtual slot 2)
// Flushes the chain, then closes the file if this pipe opened it.

int FilePipe::End()
{
    int total = Pipe::End();
    if (Valid_File() && HasOpened) {
        HasOpened = false;
        File->Close();
    }
    return total;
}


/***********************************************************************************************
 * BufferPipe::Put -- Submit data to the buffered pipe segment.                                *
 *                                                                                             *
 *    The buffered pipe is a pipe terminator. That is, the data never flows onto subsequent    *
 *    pipe chains. The data is stored into the buffer previously submitted to the pipe.        *
 *    If the buffer is full, no more data is output to the buffer.                             *
 *                                                                                             *
 * INPUT:   source   -- Pointer to the data to submit.                                         *
 *                                                                                             *
 *          length   -- The number of bytes to be submitted.                                   *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes output to the destination buffer.                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   07/03/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x007737F0, virtual slot 4)
// Writes to the file, opening it for writing first if it is closed (and
// remembering to close it later). Returns the bytes written, 0 on failure.

int FilePipe::Put(void const *source, int slen)
{
    if (Valid_File() && source && slen > 0) {
        if (!File->Is_Open()) {
            HasOpened = true;
            File->Open(FILE_RIGHTS_WRITE);
        }
        return File->Write(source, slen);
    }
    return 0;
}
