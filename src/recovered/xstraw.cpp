// xstraw.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/xstraw.json carries the addresses and the rows that
// stay in their own files.


#include "shared/file_straw.hpp"


// FileStraw::Get (0x007738A0, virtual slot 2)
// Reads from the file, opening it for reading first if it is closed (and
// remembering to close it later). Returns the bytes read, 0 on failure.

int FileStraw::Get(void *buffer, int length)
{
    if (Valid_File() && buffer && length > 0) {
        if (!File->Is_Open()) {
            HasOpened = true;
            if (!File->Is_Available()) {
                return 0;
            }
            if (!File->Open(FILE_RIGHTS_READ)) {
                return 0;
            }
        }
        return File->Read(buffer, length);
    }
    return 0;
}


// FileStraw::~FileStraw (0x00773900)
// Closes the file if this straw opened it.
// The scalar deleting destructor the linker kept was compiled in another
// object file, where this destructor is called rather than inlined;
// auto_inline(off) reproduces that copy here.
#pragma auto_inline(off)
FileStraw::~FileStraw()
{
    if (Valid_File() && HasOpened) {
        File->Close();
        HasOpened = false;
        File = 0;
    }
}
#pragma auto_inline(on)
