// The DirectSound driver descriptor (0x007CCE20) and its channel function
// table (0x007CCDF0) for the native build (Scout1091, native M8). The original
// keeps them in .data as code addresses; the run-time data loader cannot map
// those (Audio_Create_Driver called through a pair of 32-bit addresses), so
// the native descriptor names the recovered functions at those addresses and
// reads its other fields (name, flags, tag, the table's first word) from the
// user's game.exe at run time. Never original code or bytes.
#include "shared/audio.hpp"

namespace ra2port {
bool original_bytes(unsigned int va, void *out, unsigned long n);
void *original_data_pointer(unsigned int va);
}

int __fastcall Audio_DS_Channel_Is_Playing(AudioChannel *channel);   // 0x0040A5C0
int __fastcall Audio_DS_Channel_Close(AudioChannel *channel);        // 0x0040A1F0
HRESULT __fastcall Audio_DS_Channel_Start(AudioChannel *channel);    // 0x0040A260
int __fastcall Audio_DS_Channel_Stop(AudioChannel *channel);         // 0x0040A530
int __fastcall Audio_DS_Channel_Pause(AudioChannel *channel);        // 0x0040A570
HRESULT __fastcall Audio_DS_Channel_Resume(AudioChannel *channel);   // 0x0040A590
int __fastcall Audio_DS_Channel_Apply_Settings(AudioChannel *channel);   // 0x0040A5F0

namespace {
// 0x00409F10 / 0x0040A130 take the driver / device in ECX and do not read it.
void __fastcall DS_Release(AudioDriver *) { Audio_DS_Release(); }
void __fastcall DS_Close(AudioDevice *) { Audio_DS_Close(); }
// 0x0040A690 (leaf src/recovered/0040a690.cpp): returns 0x80040000.
HRESULT __fastcall DS_Channel_24(AudioChannel *) { return (HRESULT)0x80040000; }
// 0x0040A6A0 / 0x0040A6B0 (seeds): enter / leave the library lock (0x00831590).
int __fastcall DS_Channel_Lock(AudioChannel *) { EnterCriticalSection(&Audio_Lock); return 0; }
int __fastcall DS_Channel_Unlock(AudioChannel *) { LeaveCriticalSection(&Audio_Lock); return 0; }

unsigned int Stored_Word(unsigned int va)
{
    unsigned int value = 0;
    ra2port::original_bytes(va, &value, 4);
    return value;
}
}  // namespace

AudioDriverInfo *Native_Audio_Default_Driver()
{
    static AudioChannelDriver channels;
    static AudioDriverInfo driver;
    static bool built = false;
    if (!built) {
        built = true;
        channels.unknown_00 = (int)Stored_Word(0x007CCDF0);
        channels.Open = Audio_DS_Channel_Open;
        channels.Close = Audio_DS_Channel_Close;
        channels.Start = Audio_DS_Channel_Start;
        channels.Stop = Audio_DS_Channel_Stop;
        channels.Pause = Audio_DS_Channel_Pause;
        channels.Resume = Audio_DS_Channel_Resume;
        channels.Is_Playing = Audio_DS_Channel_Is_Playing;
        channels.Apply_Settings = Audio_DS_Channel_Apply_Settings;
        channels.unknown_24 = DS_Channel_24;
        channels.Lock = DS_Channel_Lock;
        channels.Unlock = DS_Channel_Unlock;
        driver.Name = (char const *)ra2port::original_data_pointer(Stored_Word(0x007CCE20));
        driver.unknown_04 = (int)Stored_Word(0x007CCE24);
        driver.unknown_flags_08 = Stored_Word(0x007CCE28);
        driver.Flags = Stored_Word(0x007CCE2C);
        driver.Tag = Stored_Word(0x007CCE30);
        driver.Init = Audio_DS_Init;
        driver.Release = DS_Release;
        driver.Open = Audio_DS_Open;
        driver.Close = DS_Close;
        driver.ChannelDriver = &channels;
    }
    return &driver;
}
