// audsrc.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/audsrc.json carries the addresses and the rows that
// stay in their own files.


#include "shared/crt_memory.hpp"
#include "shared/audio.hpp"


// AudioSource *__fastcall Audio_Source_Create(int size) -- 0x00408380
// Purpose: makes a zero-filled source owning a data buffer of size bytes.
// Returns 0 (freeing the source) when the buffer cannot be allocated.

void *__cdecl operator new(unsigned int size);
void __cdecl operator delete(void *block);

AudioSource *__fastcall Audio_Source_Create(int size)
{
    AudioSource *source = (AudioSource *)operator new(sizeof(AudioSource));
    memset(source, 0, sizeof(AudioSource));
    source->unknown_00 = 0;
    source->unknown_04 = 0;
    source->Format = 0;
    source->unknown_18 = 0;
    Audio_List_Init(&source->Pieces);
    source->unknown_04 = size;
    source->unknown_00 = (char *)operator new(size);
    if (source->unknown_00 == 0) {
        if (source != 0) {
            operator delete(source);
        }
        return 0;
    }
    return source;
}


// void __fastcall Audio_Source_Destroy(AudioSource *source) -- 0x004083E0
// Purpose: frees the source's data buffer and the source.

void __cdecl operator delete(void *block);

void __fastcall Audio_Source_Destroy(AudioSource *source)
{
    if (source->unknown_00 != 0) {
        operator delete(source->unknown_00);
    }
    operator delete(source);
}


// void __fastcall Audio_Source_Reset(AudioSource *source) -- 0x00408400
// Purpose: empties the source (no data, size, format or pieces).

void __fastcall Audio_Source_Reset(AudioSource *source)
{
    source->unknown_00 = 0;
    source->unknown_04 = 0;
    source->Format = 0;
    source->unknown_18 = 0;
    Audio_List_Init(&source->Pieces);
}


// void __fastcall Audio_Source_Add_Piece(AudioSource *source, AudioSourceInfo *piece) -- 0x00408420
// Purpose: appends a piece to the source, adding its size to the total.

void __fastcall Audio_Source_Add_Piece(AudioSource *source, AudioSourceInfo *piece)
{
    Audio_List_Add_Tail(&source->Pieces, piece);
    source->unknown_04 += piece->unknown_10;
    piece->Source = source;
}


// AudioSourceInfo *AudioSource::Info() -- 0x00408440
// Purpose: the source's first piece, or 0 when it has none.

AudioSourceInfo *AudioSource::Info()
{
    return (AudioSourceInfo *)Audio_List_Next_Unchecked(&Pieces);
}


class Leaf_408450 { public: void get(); };
void Leaf_408450::get() {  }


// int AudioFormat::Bytes_For_Time(__int64 milliseconds) -- 0x00408460
// Purpose: the number of bytes playing for milliseconds lasts.

int AudioFormat::Bytes_For_Time(__int64 milliseconds)
{
    return BytesPerSecond * (int)milliseconds / 1000;
}


// unsigned __int64 __fastcall AudioFormat::Time_For_Bytes(int bytes) -- 0x00408480
// Purpose: the milliseconds bytes play for, or 0 without a rate.

unsigned __int64 __fastcall AudioFormat::Time_For_Bytes(int bytes)
{
    if (BytesPerSecond != 0) {
        return (unsigned __int64)bytes * 1000 / BytesPerSecond;
    }
    return 0;
}


// void __fastcall Audio_Format_Init(AudioFormat *format) -- 0x004084C0
// Purpose: zero-fills the format.

void __fastcall Audio_Format_Init(AudioFormat *format)
{
    memset(format, 0, sizeof(AudioFormat));
}


// void AudioFormat::Update_Rate() -- 0x004084D0
// Purpose: computes the bytes per second (a quarter of the PCM rate for the
// 4:1 ADPCM types 1 and 2).

void AudioFormat::Update_Rate()
{
    BytesPerSecond = BytesPerSample * Channels * Rate;
    if (Type == AUDIO_FORMAT_IMA_ADPCM || Type == 2) {
        BytesPerSecond >>= 2;
    }
}


// void __fastcall AudioSourceInfo::Init(void *data, int size) -- 0x00408500
// Purpose: makes the piece an unlinked piece of size bytes at data.

void __fastcall AudioSourceInfo::Init(void *data, int size)
{
    unknown_10 = size;
    unknown_0c = data;
    Audio_List_Init_Node(this);
    Source = 0;
}


// void AudioSourceInfo::Detach() -- 0x00408520
// Purpose: forgets the piece's source.

void AudioSourceInfo::Detach()
{
    Source = 0;
}
