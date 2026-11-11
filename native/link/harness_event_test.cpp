// Event packet round trip (Scout1231; test scaffolding, NOT EXACT, never original code):
// RA2_NATIVE_EVENT_TEST=<packets>[,<max units>] builds random outgoing events (megamission runs
// of the same order for groups of units, repeated groups, the other fixed-length types), packs
// them as Build_Send_Packet/Add_Compressed_Events 0x00627B60 do and decodes the packet with
// Extract_Compressed_Events 0x006282D0, comparing what the receiver would queue with what the
// sender queued, through the sender's eight-slot unit-list cache and the receiver's per-house
// lists. Runs once, at the tenth frame of any game; logs `event-test: ...` to stderr.
#include "shared/queue.hpp"
#include "shared/house.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/session_state.hpp"
#include "shared/crt_memory.hpp"
#include "shared/crt_string.hpp"
#include <stdio.h>
#include <stdlib.h>

extern unsigned char unknown_event_lengths_7d659c[];
extern xTargetClass unknown_unit_lists_aabed0[64][128];
extern int unknown_unit_counts_a76b74[64];
int Native_Malformed_Count();   // link/native_event_data.cpp

namespace {
unsigned g_seed = 12345;
unsigned Rnd() { g_seed = g_seed * 1103515245u + 12345u; return (g_seed >> 8) & 0xFFFFFF; }
int Rnd(int n) { return (int)(Rnd() % (unsigned)n); }

struct Group { int count; int unit[256]; };
Group g_groups[32];
int g_group_count = 0;

void Fill_Target(EventClass &e, int offset, int id, int kind)
{
    e.LongAt(offset) = id;
    e.ByteAt(offset + 4) = (unsigned char)kind;
}

void Hex(char const *buf, int size)
{
    for (int i = 0; i < size; ++i) fprintf(stderr, "%s%02X", i % 32 == 0 ? "\n    " : " ", (unsigned char)buf[i]);
    fprintf(stderr, "\n");
}

// Queues one random run or event in OutList; returns false when OutList is full.
bool Queue_Random(int max_units)
{
    int house = unknown_player_a35db4->ArrayIndex;
    if (Rnd(100) < 70) {
        Group *g;
        if (g_group_count && Rnd(2)) {
            g = &g_groups[Rnd(g_group_count)];
        } else {
            g = &g_groups[g_group_count < 32 ? g_group_count++ : Rnd(32)];
            g->count = 1 + Rnd(Rnd(4) ? 12 : max_units);
            for (int i = 0; i < g->count; ++i) g->unit[i] = 1 + Rnd(200);
        }
        int mission = Rnd(3) ? 2 : 1 + Rnd(20);
        int target_id = Rnd(2) ? 0 : 1 + Rnd(300), target_kind = target_id ? 1 + Rnd(10) : 0;
        int dest_id = 1 + Rnd(5000), dest_kind = 0x0C;
        int follow = Rnd(4) ? 0 : 1 + Rnd(100);
        for (int i = 0; i < g->count; ++i) {
            EventClass e;
            memset(&e, 0, sizeof e);
            e.Type = 4;
            e.House = (char)house;
            Fill_Target(e, 0x07, g->unit[i], 1 + Rnd(2));
            e.ByteAt(0x0C) = (unsigned char)mission;
            Fill_Target(e, 0x0E, target_id, target_kind);
            Fill_Target(e, 0x13, dest_id, dest_kind);
            Fill_Target(e, 0x18, follow, follow ? 1 : 0);
            if (!OutList.Add(e)) return false;
        }
        return true;
    }
    static const unsigned char types[] = {1, 2, 3, 6, 7, 9, 0x0B, 0x0D, 0x0E, 0x11, 0x14, 0x15, 0x16, 0x17, 0x18, 0x1B, 0x20, 0x21, 0x23, 0x2D, 0x05, 0x12, 0x1E, 0x2A, 0x2C};
    EventClass e;
    memset(&e, 0, sizeof e);
    e.Type = types[Rnd((int)sizeof types)];
    e.House = (char)house;
    for (int i = 0; i < unknown_event_lengths_7d659c[e.Type]; ++i) e.Data.Bytes[i] = (unsigned char)Rnd(256);
    return OutList.Add(e);
}

// What the receiver must get for this event (the bytes the wire carries).
bool Same_Event(EventClass const &sent, EventClass const &got, EventClass const &run_first)
{
    if (sent.Type != got.Type || sent.Frame != got.Frame || sent.House != got.House) return false;
    if (sent.Type == 4) {
        // A run's later units carry their Whom only; the rest comes from the run's first event.
        if (memcmp(&sent.Data.Bytes[0], &got.Data.Bytes[0], 6) != 0) return false;
        if (memcmp(&sent.Data.Bytes[7], &got.Data.Bytes[7], 10) != 0) return false;
        return memcmp(&run_first.Data.Bytes[17], &got.Data.Bytes[17], 6) == 0;
    }
    if (sent.Type == 0x1B) return sent.Data.Bytes[6] == got.Data.Bytes[6];
    return memcmp(sent.Data.Bytes, got.Data.Bytes, unknown_event_lengths_7d659c[sent.Type]) == 0;
}
}

void Native_Harness_Event_Test(int frame)
{
    static bool done = false;
    if (done || frame != 10) return;
    done = true;
    char const *env = getenv("RA2_NATIVE_EVENT_TEST");
    if (!env || !*env) return;
    int packets = atoi(env);
    int max_units = 60;
    if (char const *comma = strchr(env, ',')) max_units = atoi(comma + 1);
    if (max_units < 1) max_units = 1;
    if (max_units > 250) max_units = 250;

    // The game's queues and protocol, restored afterwards.
    static QueueClass<EventClass, 128> saved_out;
    saved_out = OutList;
    int saved_count = DoList.Count, saved_head = DoList.Head, saved_tail = DoList.Tail;
    int saved_protocol = unknown_session_a3d2ac;
    unknown_session_a3d2ac = 1;
    memset(unknown_megamission_lists_a76ca8, 0, sizeof unknown_megamission_lists_a76ca8);
    memset(unknown_megamission_counts_aabdb0, 0, sizeof unknown_megamission_counts_aabdb0);
    memset(unknown_megamission_frames_aabe94, 0, sizeof unknown_megamission_frames_aabe94);
    memset(unknown_unit_lists_aabed0, 0, sizeof unknown_unit_lists_aabed0);
    memset(unknown_unit_counts_a76b74, 0, sizeof unknown_unit_counts_a76b74);
    OutList.Init();

    int events = 0, mismatches = 0, runs = 0, biggest = 0;
    char buf[0x22E];
    for (int p = 0; p < packets && mismatches < 5; ++p) {
        DoList.Count = saved_count; DoList.Head = saved_head; DoList.Tail = saved_tail;
        if (OutList.Count == 0) {
            int n = 1 + Rnd(6);
            for (int i = 0; i < n; ++i) if (!Queue_Random(max_units)) break;
        }
        Frame += 1 + Rnd(3);
        // Build_Send_Packet's header (0x1C, 14 bytes) and the compressed events after it; the bytes
        // after the packet are stale, as in the game's receive buffer (random here).
        for (int i = 0; i < (int)sizeof buf; ++i) buf[i] = (char)Rnd(256);
        EventClass *finfo = (EventClass *)buf;
        int delay = 5;
        finfo->Type = 0x1C;
        finfo->Frame = ::Frame + delay;
        finfo->House = (char)unknown_player_a35db4->ArrayIndex;
        finfo->LongAt(0x07) = 0x12345678;
        finfo->ShortAt(0x0B) = (short)events;
        finfo->ByteAt(0x0D) = (unsigned char)delay;
        int cap = OutList.Count;
        int added = 0;
        int sent_tail = DoList.Tail;
        int size = Add_Compressed_Events(buf, sizeof buf, delay, 0x0E, cap, &added);
        if (size > biggest) biggest = size;
        int got_tail = DoList.Tail;
        int malformed_before = Native_Malformed_Count();
        int count = Extract_Compressed_Events(buf, size);
        // The decoder queues the header (0x1C) first (Process_Receive_Packet counts it off).
        bool bad = count != added + 1 || DoList.Array[got_tail & 0x3FFF].Type != 0x1C || Native_Malformed_Count() != malformed_before;
        got_tail++;
        EventClass const *run_first = 0;
        int units = 0;
        for (int i = 0; !bad && i < added; ++i) {
            EventClass const &sent = DoList.Array[(sent_tail + i) & 0x3FFF];
            EventClass const &got = DoList.Array[(got_tail + i) & 0x3FFF];
            if (sent.Type == 4) {
                if (!run_first || run_first->Data.Bytes[5] != sent.Data.Bytes[5] || memcmp(&run_first->Data.Bytes[7], &sent.Data.Bytes[7], 10) != 0) { run_first = &sent; ++runs; units = 0; }
                ++units;
            } else {
                run_first = 0;
            }
            if (!Same_Event(sent, got, run_first ? *run_first : sent)) {
                fprintf(stderr, "event-test: packet %d event %d of %d differs: sent type 0x%02X got 0x%02X (run unit %d)\n", p, i, added, sent.Type, got.Type, units);
                bad = true;
            }
        }
        if (bad) {
            ++mismatches;
            fprintf(stderr, "event-test: packet %d: %d events sent, %d decoded, %d bytes:", p, added, count, size);
            Hex(buf, size);
            fprintf(stderr, "event-test: sent types:");
            for (int i = 0; i < added; ++i) fprintf(stderr, " %02X", DoList.Array[(sent_tail + i) & 0x3FFF].Type);
            fprintf(stderr, "\nevent-test: got types:");
            for (int i = 0; i < count; ++i) fprintf(stderr, " %02X", DoList.Array[(got_tail + i) & 0x3FFF].Type);
            fprintf(stderr, "\n");
        }
        events += added;
    }
    fprintf(stderr, "event-test: %d packets, %d events, %d runs, biggest packet %d bytes, max units %d, %d mismatches, %d malformed\n", packets, events, runs, biggest, max_units, mismatches, Native_Malformed_Count());

    OutList = saved_out;
    DoList.Count = saved_count; DoList.Head = saved_head; DoList.Tail = saved_tail;
    unknown_session_a3d2ac = saved_protocol;
    memset(unknown_megamission_lists_a76ca8, 0, sizeof unknown_megamission_lists_a76ca8);
    memset(unknown_megamission_counts_aabdb0, 0, sizeof unknown_megamission_counts_aabdb0);
    memset(unknown_megamission_frames_aabe94, 0, sizeof unknown_megamission_frames_aabe94);
    memset(unknown_unit_lists_aabed0, 0, sizeof unknown_unit_lists_aabed0);
    memset(unknown_unit_counts_a76b74, 0, sizeof unknown_unit_counts_a76b74);
}

// The number of on-screen messages (chat) in the session's list, for the harness-view log line
// (harness_input.cpp; Scout1231: whether a LAN chat message reached the receiver's list).
int Native_Session_Message_Count()
{
    return unknown_session_a3d298.Messages.Num_Messages();
}
