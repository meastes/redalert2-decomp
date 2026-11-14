// mixfile.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/mixfile.json carries the addresses and the rows that
// stay in their own files.


#include "shared/mix_file_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/file_straw.hpp"
#include "shared/pk_straw.hpp"
#include "shared/cd.hpp"
#include "shared/exit.hpp"
#include "shared/sha_straw.hpp"
#include "shared/crt_memory.hpp"
#include "shared/unknown_globals.hpp"


class Leaf_595170 { public: void get(); };
void Leaf_595170::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int zero_value; unsigned int unused_value; zero_value = 0; *(unsigned int*)(0xa70ce0) = zero_value; *(unsigned int*)(0xa70ce4) = zero_value; *(unsigned int*)(0xa70ce8) = zero_value; *(unsigned int*)(0xa70cec) = zero_value; }


// MixFileClass::MixList (0x00A70CC0), the list every constructed archive joins. Its definition
// (last in the unit) emits the dynamic initializer 0x00595190, the atexit destructor 0x005951E0
// (guarded by bit 0 of 0x00A70CF0, as VC6 guards static data members) and, through the emitted
// List vtable, List's scalar deleting destructor 0x00595D60.

List<MixFileClass *> MixFileClass::MixList;


// Atexit destructor of MixFileClass::MixList (0x005951E0, _$E): emitted by the definition in
// MixFileClass_MixList_init_00595190.cpp; no source of its own.


// int MixFileClass::Compare_Blocks(void const *left, void const *right) (0x005952D0)
// qsort/bsearch order of SubBlock records: signed compare of the CRCs.

class CCFileClass;

int MixFileClass::Compare_Blocks(void const *left, void const *right)
{
    if (*(long const *)left < *(long const *)right) {
        return -1;
    }
    return *(long const *)left > *(long const *)right;
}


// MixFileClass::MixFileClass(char const *filename, PKey const *key) (0x005952F0)
// TibSun mixfile.cpp: makes sure a disk is present, opens the archive, reads the (optionally
// Blowfish/PK encrypted) header and index, notes where the member data starts and links the
// archive at the tail of MixList.

extern "C" char *__cdecl _strdup(char const *string);
extern "C" void *__cdecl memmove(void *dest, void const *src, unsigned int count);

#pragma pack(1)
struct MixFileHeader {
    short count;
    long size;
};
struct MixFileAlternate {
    short First;
    unsigned IsDigest : 1;
    unsigned IsEncrypted : 1;
};
#pragma pack()

MixFileClass::MixFileClass(char const *filename, PKey const *key) :
    Filename(0),
    IsDigest(false),
    IsEncrypted(false),
    IsAllocated(false),
    Count(0),
    DataSize(0),
    DataStart(0),
    HeaderBuffer(0),
    Data(0)
{
    if (!CD().Force_Available(CD::RequiredCD)) {
        Unknown_00691320();
        Emergency_Exit(1);
    }

    CCFileClass file(filename);
    Filename = _strdup(file.File_Name());
    FileStraw fstraw(file);
    PKStraw pstraw(PKStraw::DECRYPT, CryptRandom);
    Straw *straw = &fstraw;

    if (!file.Is_Available()) return;

    MixFileHeader fileheader;
    MixFileAlternate alternate;

    straw->Get(&alternate, 4);

    if (alternate.First == 0) {
        IsDigest = alternate.IsDigest;
        IsEncrypted = alternate.IsEncrypted;
        if (IsEncrypted) {
            pstraw.Key(key);
            pstraw.Get_From(&fstraw);
            straw = &pstraw;
        }
        straw->Get(&fileheader, sizeof(fileheader));
    } else {
        memmove(&fileheader, &alternate, 4);
        straw->Get(((char *)&fileheader) + 4, sizeof(fileheader) - 4);
    }

    Count = fileheader.count;
    DataSize = fileheader.size;

    HeaderBuffer = new SubBlock[Count];
    if (!HeaderBuffer) return;

    straw->Get(HeaderBuffer, Count * sizeof(SubBlock));

    DataStart = file.Seek(0, FILE_SEEK_CURRENT) + file.BiasStart;

    MixList.Add_Tail(this);
}


// bool __fastcall MixFileClass::Free(char const *filename) (0x00595560)
// Releases the cache of the named registered archive; false when it is not registered.

class CCFileClass;

bool __fastcall MixFileClass::Free(char const *filename)
{
    MixFileClass *ptr = Finder(filename);
    if (ptr) {
        ptr->Free();
        return true;
    }
    return false;
}


// MixFileClass::~MixFileClass() (0x00595620)
// Frees the name, the cache and the index, then leaves the archive list.

class CCFileClass;

MixFileClass::~MixFileClass()
{
    if (Filename) {
        free((char *)Filename);
    }
    if (Data && IsAllocated) {
        delete[] (char *)Data;
        IsAllocated = false;
    }
    Data = 0;
    if (HeaderBuffer) {
        delete[] HeaderBuffer;
        HeaderBuffer = 0;
    }
    Unlink();
}


// MixFileClass *__fastcall MixFileClass::Finder(char const *filename) (0x005956E0)
// First registered archive whose name and extension equal filename (case-insensitive).

class CCFileClass;

MixFileClass *__fastcall MixFileClass::Finder(char const *filename)
{
    MixFileClass *ptr = MixList.First();
    while (ptr->Is_Valid()) {
        char path[260];
        char name[256];
        char extension[256];
        _splitpath(ptr->Filename, 0, 0, name, extension);
        _makepath(path, 0, 0, name, extension);
        if (_stricmp(path, filename) == 0) {
            return ptr;
        }
        ptr = ptr->Next();
    }
    return 0;
}


// bool __fastcall MixFileClass::Cache(char const *filename, Buffer const *buffer) (0x00595770)
// Caches the named registered archive; false when it is not registered.

class CCFileClass;

bool __fastcall MixFileClass::Cache(char const *filename, Buffer const *buffer)
{
    MixFileClass *ptr = Finder(filename);
    if (ptr) {
        return ptr->Cache(buffer);
    }
    return false;
}


// bool MixFileClass::Cache(Buffer const *buffer) (0x00595810)
// TibSun mixfile.cpp: loads the archive's member data into memory (the caller's buffer when it is
// large enough, else an owned allocation touched every 4 KB), reads it through the file straw (and
// a SHA straw for a digested archive), and checks the digest that follows the data.

bool MixFileClass::Cache(Buffer const *buffer)
{
    if (Data != 0) return true;

    if (buffer != 0 && (buffer->Get_Size() == 0 || buffer->Get_Size() >= DataSize)) {
        Data = buffer->Get_Buffer();
    } else {
        Data = new char[DataSize];
        IsAllocated = true;
        for (char *i = (char *)Data; i < &((char *)Data)[DataSize]; i += 4096) {
            volatile char tmp = *i;
        }
        Service_Game_IO_00481fb0();
    }

    if (Data != 0) {
        CCFileClass file(Filename);
        FileStraw fstraw(file);
        Straw *straw = &fstraw;

        SHAStraw sha;
        if (IsDigest) {
            sha.Get_From(fstraw);
            straw = &sha;
        }

        file.Open(FILE_RIGHTS_READ);
        file.Bias(0);
        file.Bias(DataStart);

        int actual = straw->Get(Data, DataSize);
        if (actual != DataSize) {
            delete[] (char *)Data;
            Data = 0;
            file.Error(5);
            return false;
        }

        if (IsDigest) {
            char digest1[20];
            char digest2[20];
            sha.Result(digest2);
            fstraw.Get(digest1, sizeof(digest1));
            if (memcmp(digest1, digest2, sizeof(digest1)) != 0) {
                delete[] (char *)Data;
                Data = 0;
                return false;
            }
        }
        return true;
    }
    IsAllocated = false;
    return false;
}


// void MixFileClass::Free() (0x00595A70)
// Releases the cached data (when owned) and forgets it.

class CCFileClass;

void MixFileClass::Free()
{
    if (Data && IsAllocated) {
        delete[] (char *)Data;
    }
    Data = 0;
    IsAllocated = false;
}


// bool __fastcall MixFileClass::Offset(char const *filename, void **realptr, MixFileClass **mixfile, (0x00595AA0)
// Locates a member by the CRC of its upper-cased name in every registered archive.

class CCFileClass;

bool __fastcall MixFileClass::Offset(char const *filename, void **realptr, MixFileClass **mixfile,
                                        long *offset, long *size)
{
    if (filename == 0) {
        return false;
    }

    // Note: upper-cases the caller's string in place.
    SubBlock key;
    key.ID = CRCEngine()(_strupr((char *)filename), strlen(filename));

    MixFileClass *ptr = MixList.First();
    while (ptr->Is_Valid()) {
        SubBlock *block = Binary_Search(ptr->HeaderBuffer, ptr->Count, key);
        if (block != 0) {
            if (mixfile != 0) {
                *mixfile = ptr;
            }
            if (size != 0) {
                *size = block->Size;
            }
            if (realptr != 0) {
                *realptr = 0;
            }
            if (offset != 0) {
                *offset = block->Offset;
            }
            if (realptr != 0 && ptr->Data != 0) {
                *realptr = (char *)ptr->Data + block->Offset;
            }
            if (ptr->Data == 0 && offset != 0) {
                *offset += ptr->DataStart;
            }
            return true;
        }
        ptr = ptr->Next();
    }
    return false;
}


// 0x00595bc0: function not yet identified
//
// Body: 45 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: void __fastcall FUN_00595bc0(int param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00595bc0 {

void __fastcall FUN_00595bc0(int node_address)
{
    if (((node_address != 0) && (*(int *)(node_address + 4) != 0)) && (*(int *)(node_address + 8) != 0)) {
        *(int *)(*(int *)(node_address + 8) + 4) = *(int *)(node_address + 4);
        *(unsigned int *)(*(int *)(node_address + 4) + 8) = *(unsigned int *)(node_address + 8);
        *(unsigned int *)(node_address + 8) = 0;
        *(unsigned int *)(node_address + 4) = 0;
    }
    return;
}

}  // namespace seed_00595bc0


// BlowStraw scalar deleting destructor (0x00595BF0)
// Compiler generated from BlowStraw's inline virtual destructor (vtable 0x007A6460 slot 0), emitted
// by the PKStraw local of MixFileClass's constructor (0x005952F0) above; no source of its own.


extern "C" void __cdecl release_595c40(void*);
class Leaf_595c40 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_595c40::get(unsigned int flags) { cleanup(); if (flags & 1) release_595c40(this); return this; }


// MixFileClass scalar deleting destructor (0x00595C60)
// Compiler generated from the virtual destructor (vtable 0x007A6470 slot 0).

class CCFileClass;

// Emitted with ~MixFileClass (0x00595620) above; no source of its own.


// SHAEngine::SHAEngine() (0x00595D10)
// The inline constructor of shared/sha.hpp (cleared cache and counts, the five SHA-1 start words),
// emitted out of line by the SHAStraw local of MixFileClass::Cache(Buffer const *) above; no source
// of its own.


// SHAStraw scalar deleting destructor (0x00595D40)
// Compiler generated from SHAStraw's implicit virtual destructor (vtable 0x007A6478 slot 0), emitted
// by the SHAStraw local of MixFileClass::Cache(Buffer const *) above; no source of its own.


// List<MixFileClass *>::`scalar deleting destructor' (0x00595D60): emitted through the List
// vtable by the MixList definition in MixFileClass_MixList_init_00595190.cpp; no source of its own.
