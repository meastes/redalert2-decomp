// readline.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/readline.json carries the addresses and the rows that
// stay in their own files.


#include "shared/reference_counted.hpp"
#include "shared/file_straw.hpp"
#include "shared/crt_string.hpp"
#include "shared/straw.hpp"


// int __fastcall Read_Line(FileClass &file, char *buffer, int len, bool &eof) (0x006395C0; Tiberian Sun readline.cpp): reads through a FileStraw (the Straw version expands inline).

int __fastcall Read_Line(Straw &file, char *buffer, int len, bool &eof);

int __fastcall Read_Line(FileClass &file, char *buffer, int len, bool &eof)
{
    FileStraw fs(file);
    return Read_Line(fs, buffer, len, eof);
}


// int __fastcall Read_Line(Straw &file, char *buffer, int len, bool &eof) (0x00639670; Tiberian Sun readline.cpp): one line through LF, CR dropped, at most len - 1 bytes kept; returns strlen.

int __fastcall Read_Line(Straw &file, char *buffer, int len, bool &eof)
{
    if (len == 0 || buffer == 0) {
        return 0;
    }

    int count = 0;
    for (;;) {
        char c;
        if (file.Get(&c, sizeof(c)) != sizeof(c)) {
            eof = true;
            buffer[count] = '\0';
            break;
        }

        if (c == '\x0A') {
            break;
        }
        if (c != '\x0D' && count + 1 < len) {
            buffer[count++] = c;
        }
    }
    buffer[count] = '\0';
    return strlen(buffer);
}


// ReferenceCounted::~ReferenceCounted() (0x00639710; empty: only the vtable store remains).

ReferenceCounted::~ReferenceCounted()
{
}


// void ReferenceCounted::Release() (0x00639720): deletes the object when the count drops to 0.

void ReferenceCounted::Release()
{
    if (--RefCount == 0 && this != 0) {
        delete this;
    }
}


// bool ReferenceCounted::unknown_slot1(int arg) (0x00639740): false (slot 1).

bool ReferenceCounted::unknown_slot1(int arg)
{
    return false;
}


// ReferenceCounted's scalar deleting destructor (0x00639750), emitted with its vtable.
