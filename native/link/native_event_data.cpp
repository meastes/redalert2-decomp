// Native-only (Scout1209 r10; NOT EXACT): handles for the variable data of type 0x1F events.
// The original keeps the data's address in the event's 32-bit field at +0x07 (Data.LongsCell.Long1)
// and its size at +0x0B; the event stays 0x6F bytes and travels byte for byte between games
// (Send_Packets 0x006261A0, Add/Extract_Compressed_Events 0x00627B60/0x006282D0). A 64-bit
// address does not fit there, so the field holds a small handle instead and these functions map it
// to the data (port/fixups.json; native/drafts/006282d0.cpp). The address never leaves the game: the
// senders copy the data after the event, not the field, so the wire format is the original's.
namespace {
const int kSlots = 4096;
void *g_data[kSlots];
int g_next = 0;
}

int Native_Event_Data_Handle(void *data)
{
    if (!data) return 0;
    for (int i = 0; i < kSlots; ++i) {
        int slot = (g_next + i) % kSlots;
        if (!g_data[slot]) {
            g_data[slot] = data;
            g_next = (slot + 1) % kSlots;
            return slot + 1;
        }
    }
    return 0;   // full: the event carries no data
}

void *Native_Event_Data(int handle)
{
    return handle > 0 && handle <= kSlots ? g_data[handle - 1] : 0;
}

// The data of handle, forgotten by the table (the caller deletes it).
void *Native_Event_Data_Release(int handle)
{
    void *data = Native_Event_Data(handle);
    if (data) g_data[handle - 1] = 0;
    return data;
}

// RA2_NATIVE_CRC_LOG=1: each multiplayer frame's game CRC (Queue_AI_Multiplayer 0x00623B00 after
// Compute_Game_CRC) is logged as "crc <frame> <value>" so two games' logs can be compared.
extern "C" char *getenv(char const *name);
void Debug_Printf(char const *format, ...);
void Native_Log_Frame_CRC(int frame, unsigned int crc)
{
    static int enabled = -1;
    if (enabled < 0) enabled = getenv("RA2_NATIVE_CRC_LOG") != 0;
    if (enabled) Debug_Printf("crc %d %08x\n", frame, crc);
}
