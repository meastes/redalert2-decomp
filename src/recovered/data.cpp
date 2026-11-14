// data.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/data.json carries the addresses and the rows that
// stay in their own files.


#include "shared/raw_file_class.hpp"
#include "shared/crt_memory.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/wstring.hpp"
#include "shared/compressed_file.hpp"
#include "shared/win32_api.hpp"


void *__fastcall Load_Alloc_Data(FileClass &file)
{
    void *buffer = 0;
    if (file.Is_Available()) {
        long size = file.Size();
        buffer = new char[size];
        if (buffer) file.Read(buffer, size);
    }
    return buffer;
}


void *__fastcall Load_Alloc_Data(FileClass &file);
void const *__fastcall Unknown_Load_Shape_00495270(char const *filename, bool &allocated)
{
    allocated = false;
    void const *cached = MixFileClass::Retrieve(filename);
    if (cached) return cached;
    CCFileClass file(filename);
    void *buffer = Load_Alloc_Data(file);
    allocated = buffer != 0;
    return buffer;
}


void const *__fastcall Unknown_Load_Shape_00495270(char const *filename, bool &allocated);
void const *__fastcall Unknown_Load_Shape_00495310(Wstring const &filename, bool &allocated)
{
    return Unknown_Load_Shape_00495270(filename.Peek_Buffer(), allocated);
}


int __fastcall Load_Compressed_Data_004953b0(FileClass &file, Buffer const &scratch,
    Buffer const &output, void *extra)
{
    void *destination = output.Get_Buffer();
    char *data = (char *)scratch.Get_Buffer();
    int close_file = 0;
    if (!file.Is_Open()) {
        if (!file.Open(FILE_RIGHTS_READ)) return 0;
        close_file = 1;
    }
    unsigned short bytes;
    file.Read(&bytes, 2);
    CompressedFileHeader header;
    file.Read(&header, sizeof(header));
    bytes -= sizeof(header);
    if (header.ExtraBytes) {
        bytes -= header.ExtraBytes;
        if (extra) file.Read(extra, header.ExtraBytes);
        else file.Seek(header.ExtraBytes, FILE_SEEK_CURRENT);
        header.ExtraBytes = 0;
    }
    if (scratch.Get_Buffer() == output.Get_Buffer())
        data += scratch.Get_Size() - bytes - sizeof(header);
    memmove(data, &header, sizeof(header));
    file.Read(data + sizeof(header), bytes);
    bytes = (unsigned short)Unknown_Uncompress_Data_0053aeb0(data, destination);
    if (close_file) file.Close();
    return bytes;
}


typedef HANDLE HINSTANCE;
typedef HANDLE HRSRC;
typedef HANDLE HGLOBAL;
WIN32_IMPORT HRSRC WINAPI FindResourceA(HINSTANCE instance, LPCSTR name, LPCSTR type);
WIN32_IMPORT HGLOBAL WINAPI LoadResource(HINSTANCE instance, HRSRC resource);
WIN32_IMPORT LPVOID WINAPI LockResource(HGLOBAL resource);
extern HINSTANCE unknown_instance_00851ca8;
void *__fastcall Load_Resource_004954e0(unsigned int id, char const *type)
{
    HRSRC resource = FindResourceA(unknown_instance_00851ca8, (LPCSTR)(id & 0xffff), type);
    if (!resource) return 0;
    HGLOBAL data = LoadResource(unknown_instance_00851ca8, resource);
    if (!data) return 0;
    return LockResource(data);
}


int __fastcall Load_Compressed_Data_Scaled_00495520(FileClass &file, Buffer const &scratch, Buffer const &output, void *extra, int unknown_stack_0c)
{
    return Load_Compressed_Data_004953b0(file, scratch, output, extra) / 8000;
}


// 0x004954d0: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 Fetch_String_004954d0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004954d0 {

unsigned int  Fetch_String_004954d0(void)
{
    return 0;
}

}  // namespace seed_004954d0


class Leaf_4955a0 { public: void get(); };
void Leaf_4955a0::get() { *(unsigned char*)((char*)this + 0) = 0; }
