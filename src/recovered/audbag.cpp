// audbag.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/audbag.json carries the addresses and the rows that
// stay in their own files.


#include "shared/crt_memory.hpp"
#include "shared/crt_string.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/win32_find.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/audio.hpp"
#include "shared/raw_file_class.hpp"


// 0x004011b0: function not yet identified
//
// Body: 9 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: int __fastcall FUN_004011b0(int *param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004011b0 {

int __fastcall FUN_004011b0(int *bag_words, int entry_index)
{
    return *bag_words + entry_index * 0x24;
}

}  // namespace seed_004011b0


// AudioBag *__fastcall Audio_Bag_Create(char const *path, char const *directory) -- 0x004011C0
// Purpose: opens the sound bag named path (its extension replaced: ".idx"
// for the index, ".bag" for the sounds): reads the index header (an id, a
// version and the entry count) and the entry table, sorts the table by name,
// and when directory names an existing directory remembers it (with a
// trailing backslash) for loose sound files. Returns 0 (deleting what was
// made) when a file is missing or short.

extern "C" char *__cdecl strrchr(char const *string, int character);
extern "C" void __cdecl qsort(void *base, unsigned int count, unsigned int width,
                              int (__cdecl *compare)(void const *, void const *));
extern "C" int __cdecl _stricmp(char const *left, char const *right);

void *__cdecl operator new(unsigned int size);
void __cdecl operator delete(void *block);

AudioBag *__fastcall Audio_Bag_Create(char const *path, char const *directory)
{
    AudioBag *bag = (AudioBag *)operator new(sizeof(AudioBag));
    memset(bag, 0, sizeof(AudioBag));
    int header[3];
    char name[_MAX_PATH];
    WIN32_FIND_DATA found;
    strcpy(name, path);
    char *extension = strrchr(name, '.');
    if (extension != 0) {
        *extension = 0;
    }
    strcat(name, ".idx");
    CCFileClass idx(name);
    if (!idx.Is_Available(0)) {
        goto fail;
    }
    extension = strrchr(name, '.');
    if (extension != 0) {
        *extension = 0;
    }
    strcat(name, ".bag");
    bag->BagFile = new CCFileClass(name);
    if (!bag->BagFile->Is_Available(0)) {
        goto fail;
    }
    idx.Open(1);
    bag->BagFile->Open(1);
    if (idx.Read(header, 12) != 12) {
        goto fail;
    }
    bag->Count = header[2];
    if (bag->Count > 0) {
        bag->Entries = (AudioBagEntry *)operator new(bag->Count * sizeof(AudioBagEntry));
        memset(bag->Entries, 0, bag->Count * sizeof(AudioBagEntry));
        if (header[1] == 1) {
            for (int i = 0; i < bag->Count; i++) {
                if (idx.Read(&bag->Entries[i], 0x20) != 0x20) {
                    goto fail;
                }
                bag->Entries[i].BlockSize = 0;
            }
        } else if (idx.Read(bag->Entries, bag->Count * sizeof(AudioBagEntry)) != (long)(bag->Count * sizeof(AudioBagEntry))) {
            goto fail;
        }
    }
    qsort(bag->Entries, bag->Count, sizeof(AudioBagEntry), (int (__cdecl *)(void const *, void const *))_stricmp);
    bag->Loose = 0;
    if (directory != 0) {
        strcpy(bag->Path, directory);
        if (bag->Path[strlen(bag->Path)] == '\\') {
            bag->Path[strlen(bag->Path)] = 0;
        }
        HANDLE handle = FindFirstFileA(bag->Path, &found);
        if (handle != INVALID_HANDLE_VALUE) {
            while (!(found.dwFileAttributes & 0x10)) {
                if (!FindNextFileA(handle, &found)) {
                    goto close;
                }
            }
            bag->Loose = 1;
close:
            FindClose(handle);
        }
        if (bag->Path[strlen(bag->Path)] != '\\') {
            strcat(bag->Path, "\\");
        }
    }
    return bag;
fail:
    if (bag != 0) {
        if (bag->BagFile != 0) {
            delete bag->BagFile;
        }
        if (bag->File != 0) {
            delete bag->File;
        }
        if (bag->Entries != 0) {
            operator delete(bag->Entries);
        }
        operator delete(bag);
    }
    return 0;
}


// void __fastcall Audio_Bag_Destroy(AudioBag *bag) -- 0x00401580
// Purpose: deletes the bag's files and entry table, then the bag.

void __cdecl operator delete(void *block);

void __fastcall Audio_Bag_Destroy(AudioBag *bag)
{
    if (bag->BagFile != 0) {
        delete bag->BagFile;
    }
    if (bag->File != 0) {
        delete bag->File;
    }
    if (bag->Entries != 0) {
        operator delete(bag->Entries);
    }
    operator delete(bag);
}


// 0x00401600: function not yet identified
//
// Body: 24 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: char * __fastcall FUN_00401600(int *param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00401600 {

extern "C" char s_Invalid_007cc958[];

char * __fastcall FUN_00401600(int *bag_words, int entry_index)
{
    if ((entry_index >= 0) && (entry_index < bag_words[1])) {
        return (char *)(*bag_words + entry_index * 0x24);
    }
    return s_Invalid_007cc958;
}

}  // namespace seed_00401600


// 0x00401620: function not yet identified
//
// Body: 22 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __fastcall FUN_00401620(int *param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00401620 {

unsigned int __fastcall FUN_00401620(int *bag_words, int entry_index)
{
    if ((entry_index >= 0) && (entry_index < bag_words[1])) {
        return *(unsigned int *)(*bag_words + 0x14 + entry_index * 0x24);
    }
    return 0;
}

}  // namespace seed_00401620


// void __fastcall Audio_Bag_Get_Format(AudioBag *bag, int index, AudioFormat *format) -- 0x00401640
// Purpose: describes entry index's sound: IMA ADPCM when flagged (2 bytes
// a sample), else PCM of one or, flagged, two bytes a sample; mono or, when
// flagged, stereo.

void __fastcall Audio_Bag_Get_Format(AudioBag *bag, int index, AudioFormat *format)
{
    AudioBagEntry *entry = &bag->Entries[index];
    format->unknown_00 = 4;
    format->Rate = entry->Rate;
    format->Channels = (entry->Flags & AUDIO_BAG_STEREO) ? 2 : 1;
    format->BlockSize = entry->BlockSize;
    if (entry->Flags & AUDIO_BAG_ADPCM) {
        format->Type = AUDIO_FORMAT_IMA_ADPCM;
        format->BytesPerSample = 2;
    } else {
        format->Type = AUDIO_FORMAT_PCM;
        format->BytesPerSample = (entry->Flags & AUDIO_BAG_16_BIT) ? 2 : 1;
    }
}


// 0x004016b0: function not yet identified
//
// Body: 60 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall FUN_004016b0(int *param_1,int param_2,int param_3)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004016b0 {

void __fastcall FUN_004016b0(int *bag_words, int entry_index, int format_address)
{
    int entry_address;
    unsigned int sample_rate;

    entry_address = (int)(*bag_words + entry_index * 0x24);
    sample_rate = (unsigned int)(*(unsigned int *)(format_address + 8));
    *(unsigned int *)(entry_address + 0x1c) = 0;
    *(unsigned int *)(entry_address + 0x18) = sample_rate;
    if (*(int *)(format_address + 0xc) == 2) {
        *(unsigned int *)(entry_address + 0x1c) = 1;
    }
    if (*(int *)(format_address + 0x10) == 2) {
        *(unsigned int *)(entry_address + 0x1c) = *(unsigned int *)(entry_address + 0x1c) | 4;
    }
    return;
}

}  // namespace seed_004016b0


// int Audio_Bag_Open(AudioBag *bag, int index) (0x004016F0)
// Makes entry index the one Audio_Bag_Read reads: closes the loose file of
// the previous entry, seeks the bag file to the entry (the result is 0 when
// the seek fails or the entry is empty). With loose files (bag->Loose) the
// entry is read from <directory><name>.wav instead when that file opens and
// has a wave header: the entry takes the file's format and data size and
// the result is 1; otherwise the loose file is deleted again.

void *__cdecl operator new(unsigned int size);

int __fastcall Audio_Bag_Open(AudioBag *bag, int index)
{
    int result = 1;
    AudioFormat format;
    int size;
    char name[0x104];
    AudioBagEntry *entry = &bag->Entries[index];
    Audio_Bag_Close(bag);
    bag->Reader = bag->BagFile;
    bag->Remaining = entry->Size;
    if (bag->BagFile->Seek(entry->Offset, 0) != entry->Offset) {
        result = 0;
    }
    if (entry->Size == 0) {
        result = 0;
    }
    if (bag->Loose) {
        strcpy(name, bag->Path);
        strcat(name, entry->Name);
        strcat(name, ".wav");
        FileClass *file = new RawFileClass(name);
        bag->File = file;
        if (file->Is_Available(0) && file->Open(1)) {
            Audio_Format_Init(&format);
            if (Audio_Format_Read_Wave(file, &format, &size)) {
                entry->Flags = 2;
                if (format.BytesPerSample == 2) {
                    entry->Flags = 6;
                }
                if (format.Channels == 2) {
                    entry->Flags |= AUDIO_BAG_STEREO;
                }
                entry->Rate = format.Rate;
                bag->Remaining = size;
                bag->Reader = bag->File;
                return 1;
            }
        }
        if (bag->File != 0) {
            delete bag->File;
            bag->File = 0;
        }
    }
    return result;
}


// int __fastcall Audio_Bag_Read(AudioBag *bag, void *buffer, int length) -- 0x004018C0
// Purpose: read up to length bytes of the open entry (clamped to what is left) into buffer; returns the bytes read.

int __fastcall Audio_Bag_Read(AudioBag *bag, void *buffer, int length)
{
    if (bag->Reader == 0) {
        return 0;
    }
    if (length > bag->Remaining) {
        length = bag->Remaining;
    }
    int read = length > 0 ? bag->Reader->Read(buffer, length) : 0;
    if (read < 0) {
        read = 0;
    }
    bag->Remaining -= read;
    return read;
}


// void __fastcall Audio_Bag_Close(AudioBag *bag) -- 0x00401910
// Purpose: forgets the entry being read and deletes the loose file that
// held it.

void __fastcall Audio_Bag_Close(AudioBag *bag)
{
    bag->Reader = 0;
    if (bag->File != 0) {
        delete bag->File;
        bag->File = 0;
    }
}
