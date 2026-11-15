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
// Function comment blocks from mischa85/RedAlert2 392e904 src/bfiofile.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.

// bfiofile.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/bfiofile.json carries the addresses and the rows that
// stay in their own files.


#include "shared/buffer_io_file_class.hpp"
#include "shared/crt_string.hpp"


// BufferIOFileClass::BufferIOFileClass(char const *) (0x0042F200)
// An unbuffered file with the given name; Cache() attaches a buffer later.

BufferIOFileClass::BufferIOFileClass(char const *filename) :
    IsAllocated(false),
    IsOpen(false),
    IsDiskOpen(false),
    IsCached(false),
    IsChanged(false),
    UseBuffer(false),
    BufferRights(0),
    Buffer(0),
    BufferSize(0),
    BufferPos(0),
    BufferFilePos(0),
    BufferChangeBeg(-1),
    BufferChangeEnd(-1),
    FileSize(0),
    FilePos(0),
    TrueFileStart(0)
{
    Set_Name(filename);
}


// BufferIOFileClass::BufferIOFileClass() (0x0042F2F0)
// An unbuffered, unnamed file.

BufferIOFileClass::BufferIOFileClass() :
    IsAllocated(false),
    IsOpen(false),
    IsDiskOpen(false),
    IsCached(false),
    IsChanged(false),
    UseBuffer(false),
    BufferRights(0),
    Buffer(0),
    BufferSize(0),
    BufferPos(0),
    BufferFilePos(0),
    BufferChangeBeg(-1),
    BufferChangeEnd(-1),
    FileSize(0),
    FilePos(0),
    TrueFileStart(0)
{
}


/***********************************************************************************************
 * BufferIOFileClass::~BufferIOFileClass -- Destructor for the file object.                    *
 *                                                                                             *
 *    This destructor will free all memory allocated thru using Cache routines.                *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/10/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F350)
// Releases the cache; RawFileClass's destructor then closes the file.

BufferIOFileClass::~BufferIOFileClass()
{
    Free();
}


/***********************************************************************************************
 * BufferIOFileClass::Cache -- Load part or all of a file data into RAM.                       *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Was the file load successful?  It could fail if there wasn't enough room     *
 *                to allocate the raw data block.                                              *
 *                                                                                             *
 * WARNINGS:   This routine goes to disk for a potentially very long time.                     *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/10/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F390)
// Attaches a cache buffer and fills it with the start of the file (or the
// window at the current position of an already open file).
//   size == 0  size the buffer to the whole file
//   size > 0   use that size, at least BUFFER_IO_MINIMUM_SIZE
//   buffer     use caller memory instead of allocating (requires a size)
// Returns false when a buffer is already attached with different arguments,
// when no size can be determined, or when allocation fails (ENOMEM).
// Note: IsAllocated is set even for caller-supplied memory, as in the
// original, so Free() would delete it.

bool BufferIOFileClass::Cache(long size, void *buffer)
{
    if (Buffer) {
        if (size || buffer) {
            return false;
        }
        return true;
    }
    if (Is_Available()) {
        FileSize = Size();
    } else {
        FileSize = 0;
    }
    if (size) {
        if (size < BUFFER_IO_MINIMUM_SIZE) {
            size = BUFFER_IO_MINIMUM_SIZE;
            if (buffer) {
                Error(FILE_ERROR_INVALID, false, 0);
            }
        }
        BufferSize = size;
    } else {
        BufferSize = FileSize;
    }
    if (!size && buffer) {
        return false;
    }
    if (BufferSize) {
        if (buffer) {
            Buffer = (char *)buffer;
        } else {
            Buffer = new char[BufferSize];
        }
        if (Buffer) {
            IsAllocated = true;
            IsDiskOpen = false;
            BufferPos = 0;
            BufferFilePos = 0;
            BufferChangeBeg = -1;
            BufferChangeEnd = -1;
            FilePos = 0;
            TrueFileStart = 0;
            if (FileSize) {
                long position = 0;
                int opened = false;
                long fill = FileSize;
                if (FileSize > BufferSize) {
                    fill = BufferSize;
                }
                if (Is_Open()) {
                    position = Seek(0, FILE_SEEK_CURRENT);
                    if (Handle != INVALID_HANDLE_VALUE) {
                        TrueFileStart = RawFileClass::Seek(0, FILE_SEEK_CURRENT);
                    } else {
                        TrueFileStart = position;
                    }
                    if (FileSize <= BufferSize) {
                        if (position) {
                            Seek(0, FILE_SEEK_START);
                        }
                        BufferPos = position;
                        FilePos = position;
                    } else {
                        BufferFilePos = position;
                        FilePos = position;
                    }
                } else if (Open(FILE_RIGHTS_READ)) {
                    TrueFileStart = RawFileClass::Seek(0, FILE_SEEK_CURRENT);
                    opened = true;
                }
                if (Read(Buffer, fill) != fill) {
                    Error(FILE_ERROR_IO, false, 0);
                }
                if (opened) {
                    Close();
                    IsCached = true;
                    UseBuffer = true;
                    return true;
                }
                Seek(position, FILE_SEEK_START);
                IsCached = true;
            }
            UseBuffer = true;
            return true;
        }
        Error(FILE_ERROR_NO_MEMORY, false, 0);
    }
    return false;
}


/***********************************************************************************************
 * BufferIOFileClass::Free -- Frees the allocated buffer.                                      *
 *                                                                                             *
 *    This routine will free the buffer. By using this in conjunction with the                 *
 *    Cache() function, one can maintain tight control of memory usage.                        *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/10/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F560)
// Detaches the cache, deleting it if Cache() allocated it, and returns the
// file to unbuffered operation.

void BufferIOFileClass::Free()
{
    if (Buffer) {
        if (IsAllocated) {
            delete[] Buffer;
            IsAllocated = false;
        }
        Buffer = 0;
    }
    BufferSize = 0;
    IsOpen = false;
    IsCached = false;
    IsChanged = false;
    UseBuffer = false;
}


/***********************************************************************************************
 * BufferIOFileClass::Commit -- Writes the cache to the file if it has changed.                *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  false, did not need to write the buffer.                                           *
 *          true, wrote the buffer.                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/15/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F5A0)
// Writes the changed part of the buffer back to disk, opening the disk file
// temporarily if needed. Returns whether anything was written.

bool BufferIOFileClass::Commit()
{
    if (UseBuffer && IsChanged) {
        long size = BufferChangeEnd - BufferChangeBeg;
        if (IsDiskOpen) {
            RawFileClass::Seek(TrueFileStart + BufferFilePos + BufferChangeBeg, FILE_SEEK_START);
            RawFileClass::Write(Buffer, size);
            RawFileClass::Seek(TrueFileStart + FilePos, FILE_SEEK_START);
        } else {
            RawFileClass::Open(FILE_RIGHTS_READ);
            RawFileClass::Seek(TrueFileStart + BufferFilePos + BufferChangeBeg, FILE_SEEK_START);
            RawFileClass::Write(Buffer, size);
            RawFileClass::Close();
        }
        IsChanged = false;
        return true;
    }
    return false;
}


/***********************************************************************************************
 * BufferIOFileClass::Set_Name -- Checks for name changed for a cached file.                   *
 *                                                                                             *
 *    Checks for a previous filename and that it is cached.  If so, then check the             *
 *    new filename against the old. If they are the same then return that filename.            *
 *    Otherwise, the file object's name is set with just the raw filename as passed            *
 *    to this routine.                                                                         *
 *                                                                                             *
 * INPUT:   filename -- Pointer to the filename to set as the name of this file object.        *
 *                                                                                             *
 * OUTPUT:  Returns a pointer to the final and complete filename of this file object. This     *
 *          may have a path attached to the file.                                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/15/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F650, virtual slot 2)
// Renaming a buffered file to its current name keeps the cache; any other
// name commits pending changes and invalidates the cached data.

char const *BufferIOFileClass::Set_Name(char const *filename)
{
    if (File_Name() && UseBuffer) {
        if (strcmp(filename, File_Name()) == 0) {
            return File_Name();
        }
        Commit();
        IsCached = false;
    }
    RawFileClass::Set_Name(filename);
    return File_Name();
}


/***********************************************************************************************
 * BufferIOFileClass::Is_Available -- Checks for existence of file cached or on disk.          *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the file available for opening?                                           *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/16/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F6E0, virtual slot 5)
// A file with an attached buffer is always available.

bool BufferIOFileClass::Is_Available(int)
{
    if (UseBuffer) {
        return true;
    }
    return RawFileClass::Is_Available();
}


/***********************************************************************************************
 * BufferIOFileClass::Is_Open -- Determines if the file is open.                               *
 *                                                                                             *
 *    If part or all of the file is cached, then return that it is opened. A closed file       *
 *    doesn't have a valid pointer.                                                            *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the file open?                                                            *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F700, virtual slot 6)
// Open when logically opened through the buffer, or when the disk handle is
// open (RawFileClass::Is_Open, inlined in the original).

bool BufferIOFileClass::Is_Open() const
{
    if (IsOpen && UseBuffer) {
        return true;
    }
    return Handle != INVALID_HANDLE_VALUE;
}


/***********************************************************************************************
 * BufferIOFileClass::Open -- Assigns name and opens file in one operation.                    *
 *                                                                                             *
 *    This routine will assign the specified filename to the file object and open it at the    *
 *    same time. If the file object was already open, then it will be closed first. If the     *
 *    file object was previously assigned a filename, then it will be replaced with the new    *
 *    name. Typically, this routine is used when an anonymous file object has been crated and  *
 *    now it needs to be assigned a name and opened.                                           *
 *                                                                                             *
 * INPUT:   filename -- The filename to assign to this file object.                            *
 *                                                                                             *
 *          rights   -- The open file access rights to use.                                    *
 *                                                                                             *
 * OUTPUT:  bool; Was the file opened? The return value of this is moot, since the open file   *
 *          is designed to never return unless it succeeded.                                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F720, virtual slot 8)
// Names the file, then opens it with the given rights.

int BufferIOFileClass::Open(char const *filename, int rights)
{
    Set_Name(filename);
    return BufferIOFileClass::Open(rights);
}


/***********************************************************************************************
 * BufferIOFileClass::Open -- Opens the file object with the rights specified.                 *
 *                                                                                             *
 *    This routine is used to open the specified file object with the access rights indicated. *
 *    This only works if the file has already been assigned a filename. It is guaranteed, by   *
 *    the error handler, that this routine will always return with success.                    *
 *                                                                                             *
 * INPUT:   rights   -- The file access rights to use when opening this file. This is a        *
 *                      combination of READ and/or WRITE bit flags.                            *
 *                                                                                             *
 * OUTPUT:  bool; Was the file opened successfully? This will always return true by reason of  *
 *          the error handler.                                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F740, virtual slot 7)
// Opens the file logically. Unbuffered files open on disk directly. A
// buffered file small enough to be cached whole is served from memory for
// reading; otherwise the disk file is opened too. Opening for writing first
// creates (truncates) the file and then reopens it read/write.

int BufferIOFileClass::Open(int rights)
{
    BufferIOFileClass::Close();
    if (UseBuffer) {
        BufferRights = rights;
        if (rights != FILE_RIGHTS_READ || FileSize > BufferSize) {
            if (rights == FILE_RIGHTS_WRITE) {
                RawFileClass::Open(FILE_RIGHTS_WRITE);
                RawFileClass::Close();
                rights = FILE_RIGHTS_READ_WRITE;
                TrueFileStart = 0;
            }
            if (TrueFileStart) {
                UseBuffer = false;
                Open(rights);
                UseBuffer = true;
            } else {
                RawFileClass::Open(rights);
            }
            IsDiskOpen = true;
            if (BufferRights == FILE_RIGHTS_WRITE) {
                FileSize = 0;
            }
        } else {
            IsDiskOpen = false;
        }
        BufferPos = 0;
        BufferFilePos = 0;
        BufferChangeBeg = -1;
        BufferChangeEnd = -1;
        FilePos = 0;
        IsOpen = true;
    } else {
        RawFileClass::Open(rights);
    }
    return true;
}


/***********************************************************************************************
 * BufferIOFileClass::Write -- Writes data to the file cache.                                  *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer that holds the data to be written.               *
 *                                                                                             *
 *          size     -- The number of bytes to write.                                          *
 *                                                                                             *
 * OUTPUT:  Returns the number of bytes actually written.                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/15/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042F830, virtual slot 12)
// Writes through the buffer. A partially overwritten window is first filled
// from disk so unchanged bytes survive. Each write extends the changed range
// and, past the end, the logical file size; a full window is committed and
// the next one loaded if more data follows. A closed file is opened for
// writing (and later closed) around the call.

long BufferIOFileClass::Write(void const *buffer, long size)
{
    int opened = false;

    if (!Is_Open()) {
        if (!Open(FILE_RIGHTS_WRITE)) {
            return 0;
        }
        TrueFileStart = RawFileClass::Seek(0, FILE_SEEK_CURRENT);
        opened = true;
    }
    if (UseBuffer) {
        long total = 0;
        if (BufferRights != FILE_RIGHTS_READ) {
            while (size) {
                long chunk = (size >= BufferSize - BufferPos) ? BufferSize - BufferPos : size;
                if (chunk != BufferSize && !IsCached) {
                    long fill;
                    if (FileSize < BufferSize) {
                        fill = FileSize;
                        BufferFilePos = 0;
                    } else {
                        fill = BufferSize;
                        BufferFilePos = FilePos;
                    }
                    if (TrueFileStart) {
                        UseBuffer = false;
                        Seek(FilePos, FILE_SEEK_START);
                        Read(Buffer, BufferSize);
                        Seek(FilePos, FILE_SEEK_START);
                        UseBuffer = true;
                    } else {
                        RawFileClass::Seek(BufferFilePos, FILE_SEEK_START);
                        RawFileClass::Read(Buffer, fill);
                    }
                    BufferPos = 0;
                    BufferChangeBeg = -1;
                    BufferChangeEnd = -1;
                    IsCached = true;
                }
                memmove(Buffer + BufferPos, (char const *)buffer + total, chunk);
                total += chunk;
                size -= chunk;
                IsChanged = true;
                if (BufferChangeBeg == -1) {
                    BufferChangeBeg = BufferPos;
                    BufferChangeEnd = BufferPos;
                } else if (BufferChangeBeg > BufferPos) {
                    BufferChangeBeg = BufferPos;
                }
                BufferPos += chunk;
                if (BufferChangeEnd < BufferPos) {
                    BufferChangeEnd = BufferPos;
                }
                FilePos = BufferFilePos + BufferPos;
                if (FileSize < FilePos) {
                    FileSize = FilePos;
                }
                if (BufferPos == BufferSize) {
                    Commit();
                    BufferPos = 0;
                    BufferFilePos = FilePos;
                    BufferChangeBeg = -1;
                    BufferChangeEnd = -1;
                    if (size && FileSize > FilePos) {
                        if (TrueFileStart) {
                            UseBuffer = false;
                            Seek(FilePos, FILE_SEEK_START);
                            Read(Buffer, BufferSize);
                            Seek(FilePos, FILE_SEEK_START);
                            UseBuffer = true;
                        } else {
                            RawFileClass::Seek(FilePos, FILE_SEEK_START);
                            RawFileClass::Read(Buffer, BufferSize);
                        }
                    } else {
                        IsCached = false;
                    }
                }
            }
        } else {
            Error(FILE_ERROR_ACCESS, false, 0);
        }
        size = total;
    } else {
        size = RawFileClass::Write(buffer, size);
    }
    if (opened) {
        Close();
    }
    return size;
}


/***********************************************************************************************
 * BufferIOFileClass::Read -- Reads data from the file cache.                                  *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   buffer   -- Pointer to the buffer to place the read data.                          *
 *                                                                                             *
 *          size     -- The number of bytes to read.                                           *
 *                                                                                             *
 * OUTPUT:  Returns the actual number of bytes read (this could be less than requested).       *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/15/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042FA80, virtual slot 9)
// Reads through the buffer. An empty buffer is filled first: the whole file
// when it fits, otherwise the buffer-sized window at the current position.
// Data is copied out of the window; when the window is exhausted, changes
// are committed and the next window is read if more data is wanted. A
// closed file is opened (and later closed) around the read.

long BufferIOFileClass::Read(void *buffer, long size)
{
    int opened = false;

    if (!Is_Open()) {
        if (Open(FILE_RIGHTS_READ)) {
            TrueFileStart = RawFileClass::Seek(0, FILE_SEEK_CURRENT);
            opened = true;
        }
    }
    if (UseBuffer) {
        long total = 0;
        if (BufferRights != FILE_RIGHTS_WRITE) {
            while (size) {
                long chunk = (size >= BufferSize - BufferPos) ? BufferSize - BufferPos : size;
                if (!IsCached) {
                    long fill;
                    if (FileSize < BufferSize) {
                        fill = FileSize;
                        BufferFilePos = 0;
                    } else {
                        fill = BufferSize;
                        BufferFilePos = FilePos;
                    }
                    if (TrueFileStart) {
                        UseBuffer = false;
                        Seek(FilePos, FILE_SEEK_START);
                        Read(Buffer, BufferSize);
                        Seek(FilePos, FILE_SEEK_START);
                        UseBuffer = true;
                    } else {
                        RawFileClass::Seek(BufferFilePos, FILE_SEEK_START);
                        RawFileClass::Read(Buffer, fill);
                    }
                    BufferPos = 0;
                    BufferChangeBeg = -1;
                    BufferChangeEnd = -1;
                    IsCached = true;
                }
                memmove((char *)buffer + total, Buffer + BufferPos, chunk);
                total += chunk;
                size -= chunk;
                BufferPos += chunk;
                FilePos = BufferFilePos + BufferPos;
                if (BufferPos == BufferSize) {
                    Commit();
                    BufferPos = 0;
                    BufferFilePos = FilePos;
                    BufferChangeBeg = -1;
                    BufferChangeEnd = -1;
                    if (size && FileSize > FilePos) {
                        if (TrueFileStart) {
                            UseBuffer = false;
                            Seek(FilePos, FILE_SEEK_START);
                            Read(Buffer, BufferSize);
                            Seek(FilePos, FILE_SEEK_START);
                            UseBuffer = true;
                        } else {
                            RawFileClass::Seek(FilePos, FILE_SEEK_START);
                            RawFileClass::Read(Buffer, BufferSize);
                        }
                    } else {
                        IsCached = false;
                    }
                }
            }
        } else {
            Error(FILE_ERROR_ACCESS, false, 0);
        }
        size = total;
    } else {
        size = RawFileClass::Read(buffer, size);
    }
    if (opened) {
        Close();
    }
    return size;
}


/***********************************************************************************************
 * BufferIOFileClass::Seek -- Moves the current file pointer in the file.                      *
 *                                                                                             *
 *    This routine will change the current file pointer to the position specified. It follows  *
 *    the same rules the a normal Seek() does, but if the file is part of the mixfile system,  *
 *    then only the position value needs to be updated.                                        *
 *                                                                                             *
 * INPUT:   pos      -- The position to move the file to relative to the position indicated    *
 *                      by the "dir" parameter.                                                *
 *                                                                                             *
 *          dir      -- The direction to affect the position change against. This can be       *
 *                      either SEEK_CUR, SEEK_END, or SEEK_SET.                                *
 *                                                                                             *
 * OUTPUT:  Returns with the position of the new location.                                     *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/15/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042FC90, virtual slot 10)
// Moves the logical position, clamped to [0, FileSize]. Within the current
// buffer window only BufferPos moves; outside it, changes are committed and
// the disk position follows. Positions at or beyond TrueFileStart are taken
// as absolute disk positions and answered the same way.

long BufferIOFileClass::Seek(long position, int origin)
{
    if (UseBuffer) {
        bool adjusted = false;
        switch (origin) {
        case FILE_SEEK_START:
            FilePos = 0;
            break;
        case FILE_SEEK_END:
            FilePos = FileSize;
            break;
        }
        if (TrueFileStart && position >= TrueFileStart) {
            position -= TrueFileStart;
            adjusted = true;
        }
        FilePos += position;
        if (FilePos < 0) {
            FilePos = 0;
        }
        if (FilePos > FileSize) {
            FilePos = FileSize;
        }
        if (FileSize <= BufferSize) {
            BufferPos = FilePos;
        } else if (FilePos >= BufferFilePos && FilePos < BufferFilePos + BufferSize) {
            BufferPos = FilePos - BufferFilePos;
        } else {
            Commit();
            if (TrueFileStart) {
                UseBuffer = false;
                Seek(FilePos, FILE_SEEK_START);
                UseBuffer = true;
            } else {
                RawFileClass::Seek(FilePos, FILE_SEEK_START);
            }
            IsCached = false;
        }
        if (TrueFileStart && adjusted) {
            return FilePos + TrueFileStart;
        }
        return FilePos;
    }
    return RawFileClass::Seek(position, origin);
}


/***********************************************************************************************
 * BufferIOFileClass::Size -- Determines size of file (in bytes).                              *
 *                                                                                             *
 *    If part or all of the file is cached, then the size of the file is already               *
 *    determined and available. Otherwise, go to the low level system to find the file         *
 *    size.                                                                                    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the number of bytes in the file.                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042FD80, virtual slot 11)
// The logical size while buffered; otherwise the disk size.

long BufferIOFileClass::Size()
{
    if (IsOpen && UseBuffer) {
        return FileSize;
    }
    return RawFileClass::Size();
}


/***********************************************************************************************
 * BufferIOFileClass::Close -- Perform a closure of the file.                                  *
 *                                                                                             *
 *    Call Commit() to write the buffer if the file is cached and the buffer has changed,      *
 *    then call lower level Close().                                                           *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/14/1995 DRD : Created.                                                                 *
 *=============================================================================================*/
// (0x0042FDA0, virtual slot 13)
// Commits buffered changes and closes the disk file if this object opened
// it. When the file lives inside an already open file (TrueFileStart != 0),
// the containing file's own Close() runs with buffering disabled.

void BufferIOFileClass::Close()
{
    if (UseBuffer) {
        Commit();
        if (IsDiskOpen) {
            if (TrueFileStart) {
                UseBuffer = false;
                Close();
                UseBuffer = true;
            } else {
                RawFileClass::Close();
            }
            IsDiskOpen = false;
        }
        IsOpen = false;
    } else {
        RawFileClass::Close();
    }
}
