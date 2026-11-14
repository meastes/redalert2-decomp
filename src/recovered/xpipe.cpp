// xpipe.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/xpipe.json carries the addresses and the rows that
// stay in their own files.


#include "shared/file_pipe.hpp"


// FilePipe::~FilePipe (0x00773790)
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


// FilePipe::End (0x007737C0, virtual slot 2)
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


// FilePipe::Put (0x007737F0, virtual slot 4)
// Writes to the file, opening it for writing first if it is closed (and
// remembering to close it later). Returns the bytes written, 0 on failure.

int FilePipe::Put(void const *source, int length)
{
    if (Valid_File() && source && length > 0) {
        if (!File->Is_Open()) {
            HasOpened = true;
            File->Open(FILE_RIGHTS_WRITE);
        }
        return File->Write(source, length);
    }
    return 0;
}
