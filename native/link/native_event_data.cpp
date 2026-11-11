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

// Combo item data that holds a lobby slot entry (NodeNameType *): the lobby stores the entry's
// address as the 32-bit item data (CB_SETITEMDATA, Refill_Slot_Combos 0x005CA6C0) and casts the
// data back (Host_Dialog_Proc 0x005BEFE0, Join_Lobby_Dialog_Proc 0x005C2330) or compares it with
// an entry's address (unknown_combo_select_by_data 0x005C9950). A 64-bit address does not fit, so
// these sites carry a 32-bit handle instead (port/fixups.json, the drafts); the other combos keep
// their small integers (countries, colours, -2, -3), below the handles' range.
namespace {
const int kEntryBase = 0x40000000;
void *g_entries[1024];
int g_entry_count = 0;
}
int Native_Entry_Handle(void const *entry)
{
    if (!entry) return 0;
    for (int i = 0; i < g_entry_count; ++i)
        if (g_entries[i] == entry) return kEntryBase + i;
    if (g_entry_count == 1024) return 0;
    g_entries[g_entry_count] = (void *)entry;
    return kEntryBase + g_entry_count++;
}
void *Native_Entry_From_Handle(int handle)
{
    int i = handle - kEntryBase;
    return i >= 0 && i < g_entry_count ? g_entries[i] : 0;
}

// Extract_Compressed_Events (draft 0x006282D0) stops at a malformed record and reports the packet
// here (Scout1223: a stack overrun there ended a two-Mac LAN game). The first eight are logged.
#include <stdio.h>
static int g_malformed = 0;
int Native_Malformed_Count() { return g_malformed; }
void Native_Malformed(void const *buf, int bufsize, int pos, int type, char const *why)
{
    static int reported = 0;
    ++g_malformed;
    if (reported++ >= 8) return;
    fprintf(stderr, "Extract_Compressed_Events: malformed packet (%s) at %d of %d, type 0x%02X:", why, pos, bufsize, type);
    for (int i = 0; i < bufsize && i < 96; ++i) fprintf(stderr, " %02X", ((unsigned char const *)buf)[i]);
    fprintf(stderr, "\n");
}

// Sync_Delay's in-wait pass in network games (input, tactical AI, render; fixup): at most one per
// 33 ms, the time a render took on the original's machines (Scout1223: edge scrolling ran many
// times a frame natively).
extern "C" __int64 ra2port_now_ms(void);   // port/src/user32.cpp (steady clock)
// Scout1231: measured against a skirmish (one input/render pass per game frame, ScrollClass's edge
// scroll a fixed step per pass), a LAN game still edge-scrolled about three times as fast with the
// 33 ms limit alone, because a LAN frame here lasts longer than 33 ms (the main loop's pass plus
// one or two in-wait passes). The in-wait pass is for the wait on a slow peer: it runs only once
// the frame has stalled (100 ms since the main loop's last frame), then at most every 33 ms, so in
// normal play a LAN game scrolls at the single-player rate.
static __int64 g_frame_ms = 0;
void Native_Note_Frame()
{
    g_frame_ms = ra2port_now_ms();
}
bool Native_Wait_Pass_Due()
{
    static __int64 last = 0;
    __int64 now = ra2port_now_ms();
    if (now - g_frame_ms < 100) return false;
    if (now - last < 33) return false;
    last = now;
    return true;
}
