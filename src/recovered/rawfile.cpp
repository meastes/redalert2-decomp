// Westwood rawfile.cpp in one source (twins: mischa85/RedAlert2 src/rawfile.cpp, TibSun rawfile.cpp): the accepted
// bodies; the file and size types stay long until the FileClass hierarchy is converted.

#include "shared/raw_file_class.hpp"


// FileClass::Get_Date_Time
// Default: files without a modification time report 0.

unsigned long FileClass::Get_Date_Time()
{
    return 0;
}


// FileClass::Set_Date_Time
// Default: setting the time is unsupported.

bool FileClass::Set_Date_Time(unsigned long)
{
    return false;
}


// FileClass scalar deleting destructor
// Compiler generated from the inline ~FileClass wherever the FileClass vtable
// is emitted (here, by RawFileClass's constructor and destructor).


// RawFileClass::~RawFileClass
// Closes the file and frees an owned name.

RawFileClass::~RawFileClass()
{
    Close();
    if (Allocated && Filename) {
        free((void *)Filename);
        Filename = 0;
        Allocated = false;
    }
}


// RawFileClass::Error
// RawFileClass ignores errors; derived classes (CCFileClass) override this to
// report them.

void RawFileClass::Error(int, int, char const *)
{
}


// RawFileClass::RawFileClass(char const *)
// Creates a closed, unbiased file object. The name is referenced, not copied
// (Allocated stays false), unlike Set_Name().

RawFileClass::RawFileClass(char const *filename) :
    Rights(0),
    BiasStart(0),
    BiasLength(-1),
    Handle(INVALID_HANDLE_VALUE),
    Filename(filename),
    Date(0),
    Time(0),
    Allocated(false)
{
}


// RawFileClass::Set_Name
// Replaces the file name with a private copy, freeing a previously owned one.
// Naming a file also clears any bias window. Returns the new name, or null if
// no name was given or the copy could not be allocated (reported as ENOMEM).

char const *RawFileClass::Set_Name(char const *filename)
{
    if (Filename != 0 && Allocated) {
        free((void *)Filename);
        Filename = 0;
        Allocated = false;
    }
    if (filename == 0) {
        return 0;
    }
    BiasStart = 0;
    BiasLength = -1;
    Filename = _strdup(filename);
    if (Filename == 0) {
        Error(FILE_ERROR_NO_MEMORY, false, filename);
        return 0;
    }
    Allocated = true;
    return Filename;
}


// RawFileClass::Open(char const *, int)
// Names the file, then opens it with the given rights.

int RawFileClass::Open(char const *filename, int rights)
{
    Set_Name(filename);
    return Open(rights);
}


// RawFileClass::Open(int)
// Opens the named file with the requested rights:
//   READ        existing file, shared read/write, sequential-scan hint
//   WRITE       create or truncate, exclusive
//   READ_WRITE  open or create, exclusive
// Any other rights value leaves the handle unchanged. An open bias window
// repositions to its start. Returns whether the file is now open.

int RawFileClass::Open(int rights)
{
    Close();
    if (Filename == 0) {
        Error(FILE_ERROR_NO_FILE_NAME, false, 0);
        return false;
    }
    Rights = rights;
    switch (rights) {
    case FILE_RIGHTS_READ:
        Handle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING,
                             FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, 0);
        break;
    case FILE_RIGHTS_WRITE:
        Handle = CreateFileA(Filename, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        break;
    case FILE_RIGHTS_READ_WRITE:
        Handle = CreateFileA(Filename, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
        break;
    }
    if (BiasStart != 0 || BiasLength != -1) {
        Seek(0, FILE_SEEK_START);
    }
    return Handle != INVALID_HANDLE_VALUE;
}


// RawFileClass::Is_Available
// Reports whether the named file exists. An open file is available. Otherwise
// the file is probed: normally with a short-lived CreateFileA handle, or,
// when forced, by a full Open(READ) and Close().

bool RawFileClass::Is_Available(int forced)
{
    if (Filename == 0) {
        return false;
    }
    if (Is_Open()) {
        return true;
    }
    if (forced) {
        RawFileClass::Open(FILE_RIGHTS_READ);
        RawFileClass::Close();
        return true;
    }
    Handle = CreateFileA(Filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (Handle == INVALID_HANDLE_VALUE) {
        return false;
    }
    if (!CloseHandle(Handle)) {
        Error(GetLastError(), false, Filename);
    }
    Handle = INVALID_HANDLE_VALUE;
    return true;
}


// RawFileClass::Close
// Closes the handle if open. A failed CloseHandle is reported through Error()
// with the Win32 error code; the handle is invalidated either way.

void RawFileClass::Close()
{
    if (Is_Open()) {
        if (!CloseHandle(Handle)) {
            Error(GetLastError(), false, Filename);
        }
        Handle = INVALID_HANDLE_VALUE;
    }
}


// RawFileClass::Read
// Reads up to size bytes, limited to the rest of the bias window. A closed
// file is opened for reading and closed again afterwards. ReadFile failures
// are reported through Error() (retry allowed) unless RawFileReadStopOnError
// asks to stop. Returns the number of bytes read.

long RawFileClass::Read(void *buffer, long size)
{
    long bytes_read = 0;
    int opened = false;

    if (!Is_Open()) {
        if (!Open(FILE_RIGHTS_READ)) {
            return 0;
        }
        opened = true;
    }
    if (BiasLength != -1) {
        long remainder = BiasLength - Seek(0, FILE_SEEK_CURRENT);
        size = size < remainder ? size : remainder;
    }
    long total = 0;
    while (size > 0) {
        bytes_read = 0;
        if (!ReadFile(Handle, buffer, size, (DWORD *)&bytes_read, 0)) {
            buffer = (char *)buffer + bytes_read;
            size -= bytes_read;
            total += bytes_read;
            if (RawFileReadStopOnError) {
                break;
            }
            Error(GetLastError(), true, Filename);
            continue;
        }
        buffer = (char *)buffer + bytes_read;
        size -= bytes_read;
        total += bytes_read;
        if (bytes_read == 0) {
            break;
        }
    }
    bytes_read = total;
    RawFileReadStopOnError = false;
    if (opened) {
        Close();
    }
    return bytes_read;
}


// RawFileClass::Write
// Writes size bytes at the current position. A closed file is opened for
// writing and closed again afterwards. Writing past the end of a bias window
// extends the window. Returns the number of bytes written.

long RawFileClass::Write(void const *buffer, long size)
{
    DWORD bytes_written = 0;
    int opened = false;

    if (!Is_Open()) {
        if (!Open(FILE_RIGHTS_WRITE)) {
            return 0;
        }
        opened = true;
    }
    if (!WriteFile(Handle, buffer, size, &bytes_written, 0)) {
        Error(GetLastError(), false, Filename);
    }
    if (BiasLength != -1) {
        if (Raw_Seek(0) > BiasStart + BiasLength) {
            BiasLength = Raw_Seek(0) - BiasStart;
        }
    }
    if (opened) {
        Close();
    }
    return bytes_written;
}


// RawFileClass::Seek
// Moves the file position. Without a bias window this is a plain Win32 seek.
// With one, positions are relative to the window: a start-relative position
// is clamped to the window length, end-relative seeks become start-relative,
// and a result outside the window is pulled back to its nearest edge.
// Returns the new position relative to the window start.

long RawFileClass::Seek(long position, int origin)
{
    if (BiasLength != -1) {
        switch (origin) {
        case FILE_SEEK_START:
            if (position > BiasLength) {
                position = BiasLength;
            }
            position += BiasStart;
            break;
        case FILE_SEEK_CURRENT:
            break;
        case FILE_SEEK_END:
            origin = FILE_SEEK_START;
            position += BiasStart + BiasLength;
            break;
        }
        long result = Raw_Seek(position, origin) - BiasStart;
        if (result < 0) {
            result = Raw_Seek(BiasStart, FILE_SEEK_START) - BiasStart;
        }
        if (result > BiasLength) {
            result = Raw_Seek(BiasStart + BiasLength, FILE_SEEK_START) - BiasStart;
        }
        return result;
    }
    return Raw_Seek(position, origin);
}


// RawFileClass::Size
// Returns the file length: the bias window length when one is set, otherwise
// the disk size. A closed file is opened temporarily to measure it.

long RawFileClass::Size()
{
    long size = 0;
    if (BiasLength != -1) {
        return BiasLength;
    }
    if (Is_Open()) {
        size = GetFileSize(Handle, 0);
        if (size == -1) {
            Error(GetLastError(), false, Filename);
        }
    } else if (Open()) {
        size = Size();
        Close();
    }
    BiasLength = size - BiasStart;
    return BiasLength;
}


// RawFileClass::Create
// Creates (or truncates) the file by opening it for writing and closing it.
// With a bias window the position is reset to the window start first.

int RawFileClass::Create()
{
    Close();
    if (Open(FILE_RIGHTS_WRITE)) {
        if (BiasLength != -1) {
            Seek(0, FILE_SEEK_START);
        }
        Close();
        return true;
    }
    return false;
}


// RawFileClass::Delete
// Deletes the named file from disk. Fails without a name, when the file does
// not exist, or when DeleteFileA fails (reported through Error()).

int RawFileClass::Delete()
{
    Close();
    if (Filename == 0) {
        Error(FILE_ERROR_NO_FILE_NAME, false, 0);
        return false;
    }
    if (!Is_Available()) {
        return false;
    }
    if (!DeleteFileA(Filename)) {
        Error(GetLastError(), false, Filename);
        return false;
    }
    return true;
}


// RawFileClass::Get_Date_Time
// Returns the last-write time of the open file packed as DOS date (high word)
// and DOS time (low word), or 0 when it cannot be read.

unsigned long RawFileClass::Get_Date_Time()
{
    BY_HANDLE_FILE_INFORMATION info;
    WORD date;
    WORD time;
    if (GetFileInformationByHandle(Handle, &info)) {
        FileTimeToDosDateTime(&info.ftLastWriteTime, &date, &time);
        return ((unsigned long)date << 16) | time;
    }
    return 0;
}


// RawFileClass::Set_Date_Time
// Sets the open file's last-access and last-write times from a packed DOS
// date (high word) and time (low word), keeping its creation time.
// Returns whether the time was set.

bool RawFileClass::Set_Date_Time(unsigned long datetime)
{
    if (Handle != INVALID_HANDLE_VALUE) {
        BY_HANDLE_FILE_INFORMATION info;
        if (GetFileInformationByHandle(Handle, &info)) {
            FILETIME time;
            if (DosDateTimeToFileTime((WORD)(datetime >> 16), (WORD)datetime, &time)) {
                return SetFileTime(Handle, &info.ftCreationTime, &time, &time) != 0;
            }
        }
    }
    return false;
}


// RawFileClass::Bias
// Restricts all positioning and sizes to a window of the disk file. The new
// start is relative to the current window; the length is clamped to the
// file size and to zero. An open file is repositioned to the window start.

void RawFileClass::Bias(int start, int length)
{
    if (start == 0) {
        BiasStart = 0;
        BiasLength = -1;
        return;
    }
    BiasLength = RawFileClass::Size();
    BiasStart += start;
    if (length != -1) {
        BiasLength = (BiasLength < length) ? BiasLength : length;
    }
    BiasLength = (BiasLength <= 0) ? 0 : BiasLength;
    if (Is_Open()) {
        RawFileClass::Seek(0, FILE_SEEK_START);
    }
}


// RawFileClass::Raw_Seek
// Moves the Win32 file pointer, ignoring the bias window. Fails (reporting
// through Error()) when the file is not open or SetFilePointer fails.

long RawFileClass::Raw_Seek(long position, int origin)
{
    if (!Is_Open()) {
        Error(FILE_ERROR_NOT_OPEN, false, Filename);
        return 0;
    }
    switch (origin) {
    case FILE_SEEK_START:
        origin = FILE_BEGIN;
        break;
    case FILE_SEEK_CURRENT:
        origin = FILE_CURRENT;
        break;
    case FILE_SEEK_END:
        origin = FILE_END;
        break;
    }
    position = SetFilePointer(Handle, position, 0, origin);
    if (position == -1) {
        Error(GetLastError(), false, Filename);
        return 0;
    }
    return position;
}


// RawFileClass scalar deleting destructor
// Compiler generated from ~RawFileClass (defined in this class's translation
// unit): runs the destructor and frees the object when asked to.


// RawFileClass::Is_Open
// A file is open while it holds a valid Win32 handle.

bool RawFileClass::Is_Open() const
{
    return Handle != INVALID_HANDLE_VALUE;
}
