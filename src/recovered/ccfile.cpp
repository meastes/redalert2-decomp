// Westwood ccfile.cpp in one source (twins: mischa85/RedAlert2 src/ccfile.cpp, TibSun ccfile.cpp): the accepted
// bodies; the file and size types stay long until the FileClass hierarchy is converted.

#include "shared/cc_file_class.hpp"
#include "shared/cd.hpp"
#include "shared/wstring.hpp"
#include "shared/crt_string.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/crt_new.hpp"


// void CCFileClass::Error(int error, int can_retry, char const *filename))
// A file could not be opened: asks a temporary CD object to make the
// required disk available. The answer is ignored.
// Assumes: CD slot 0 is Force_Available (see shared/cd.hpp); the parameters
// are unused, as in Westwood's earlier ccfile.cpp.

void CCFileClass::Error(int, int, char const *)
{
    CD().Force_Available(CD::RequiredCD);
}


// CCFileClass::Open(char const *, int))
// Names the file, then opens it with the given rights.

int CCFileClass::Open(char const *filename, int rights)
{
    Set_Name(filename);
    return Open(rights);
}


// CCFileClass scalar deleting destructor)
// Compiler generated from the inline ~CCFileClass.


// CCFileClass::CCFileClass(char const *)
// A named file; the name is resolved against the search directories.

CCFileClass::CCFileClass(char const *filename) :
    Position(0)
{
    CDFileClass::Set_Name(filename);
}


// CCFileClass::CCFileClass(Wstring const &)
// A named file from a string's text (MSPCXAnim, the game modes' INI). Draft: Scout1076.

CCFileClass::CCFileClass(Wstring const &filename) :
    Position(0)
{
    CDFileClass::Set_Name(filename.Peek_Buffer());
}


// CCFileClass::CCFileClass()
// An unnamed file.

CCFileClass::CCFileClass() :
    Position(0)
{
}


// CCFileClass::Write)
// Files served from memory cannot be written (reported as EACCES).

long CCFileClass::Write(void const *buffer, long size)
{
    if (Data.Get_Buffer() != 0) {
        Error(FILE_ERROR_ACCESS, false, File_Name());
    }
    return CDFileClass::Write(buffer, size);
}


// CCFileClass::Read)
// Reads from memory when the file is resident, otherwise from disk. A
// closed file is opened (and closed again) around the read.

long CCFileClass::Read(void *buffer, long size)
{
    bool opened = false;
    if (!Is_Open()) {
        if (Open()) {
            opened = true;
        }
    }
    if (Data.Get_Buffer() != 0) {
        long maximum = Data.Get_Size() - Position;
        if (maximum < size) {
            size = maximum;
        }
        if (size) {
            memmove(buffer, (char *)Data.Get_Buffer() + Position, size);
            Position += size;
        }
        if (opened) {
            Close();
        }
        return size;
    }
    size = CDFileClass::Read(buffer, size);
    if (opened) {
        Close();
    }
    return size;
}


// CCFileClass::Seek)
// Moves the read position of a resident file, clamped to its size;
// otherwise seeks on disk.

long CCFileClass::Seek(long position, int origin)
{
    if (Data.Get_Buffer() != 0) {
        switch (origin) {
        case FILE_SEEK_START:
            Position = 0;
            break;
        case FILE_SEEK_END:
            Position = Data.Get_Size();
            break;
        }
        Position += position;
        Position = (Position < 0) ? 0 : Position;
        Position = (Position > Data.Get_Size()) ? Data.Get_Size() : Position;
        return Position;
    }
    return CDFileClass::Seek(position, origin);
}


// CCFileClass::Size)
// A resident file's size; for a file found only inside a mixfile, the member
// size; otherwise the disk size.

long CCFileClass::Size()
{
    if (Data.Get_Buffer() != 0) {
        return Data.Get_Size();
    }
    if (!CDFileClass::Is_Available()) {
        long length = 0;
        MixFileClass::Offset(File_Name(), 0, 0, 0, &length);
        return length;
    }
    return CDFileClass::Size();
}


// CCFileClass::Is_Available)
// Available when open, when some mixfile holds it, or when it exists on disk.

bool CCFileClass::Is_Available(int)
{
    if (Is_Open()) {
        return true;
    }
    if (MixFileClass::Offset(File_Name())) {
        return true;
    }
    return CDFileClass::Is_Available();
}


// CCFileClass::Is_Open)
// A resident file is always open.

bool CCFileClass::Is_Open() const
{
    if (Data.Get_Buffer() != 0) {
        return true;
    }
    return CDFileClass::Is_Open();
}


// CCFileClass::Close)
// Forgets resident data and closes the disk file.

void CCFileClass::Close()
{
    new (&Data) ::Buffer;
    Position = 0;
    CDFileClass::Close();
}


// CCFileClass::Open(int))
// Writes, and names found on disk, open normally. A name found only inside
// a mixfile opens the archive with a bias window over the member, or, when
// the archive is cached in memory, serves the member from that memory.

int CCFileClass::Open(int rights)
{
    Close();
    if ((rights & FILE_RIGHTS_WRITE) || CDFileClass::Is_Available()) {
        return CDFileClass::Open(rights);
    }
    MixFileClass *mixfile = 0;
    void *pointer = 0;
    long length = 0;
    long start = 0;
    if (MixFileClass::Offset(File_Name(), &pointer, &mixfile, &start, &length)) {
        if (pointer == 0 && mixfile != 0) {
            char *name = _strdup(File_Name());
            Open(mixfile->Filename, FILE_RIGHTS_READ);
            IsDisabled = true;
            Set_Name(name);
            IsDisabled = false;
            free(name);
            Bias(0);
            Bias(start, length);
            Seek(0, FILE_SEEK_START);
        } else {
            new (&Data) ::Buffer(pointer, length);
            Position = 0;
        }
        return true;
    }
    return CDFileClass::Open(rights);
}


// CCFileClass::Get_Date_Time)
// A file inside a mixfile has the archive's modification time.

unsigned long CCFileClass::Get_Date_Time()
{
    MixFileClass *mixfile = 0;
    unsigned long datetime = CDFileClass::Get_Date_Time();
    if (datetime == 0) {
        if (MixFileClass::Offset(File_Name(), 0, &mixfile, 0, 0)) {
            return CCFileClass(mixfile->Filename).Get_Date_Time();
        }
    }
    return datetime;
}


// CCFileClass::Set_Date_Time)
// A file inside a mixfile sets the archive's modification time.

bool CCFileClass::Set_Date_Time(unsigned long datetime)
{
    MixFileClass *mixfile = 0;
    bool result = CDFileClass::Set_Date_Time(datetime);
    if (!result) {
        if (MixFileClass::Offset(File_Name(), 0, &mixfile, 0, 0)) {
            return CCFileClass(mixfile->Filename).Set_Date_Time(datetime);
        }
    }
    return result;
}
