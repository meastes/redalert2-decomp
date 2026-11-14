// NON-EXACT DRAFT 0x5C7EF0 FUN_005c7ef0
// entry 0x5C7EF0; exact-match score 88.77 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout992 (build/class-work/Scout992/keep/upload_k7_8877.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// void Upload_Map_Preview_005c7ef0(MapPreviewClass *preview) (0x005C7EF0)
// The host sends the map preview to the other players. In a World Online game a player whose NAT
// flags or addresses rule out a direct transfer makes the host send the public options text "Z3"
// instead. Otherwise the players are told the preview mode (global command 0x19, 1200 ticks to
// send), each must answer 0x1A within 1800 ticks, and the preview's low-resolution image is LZO
// compressed into Preview.bin (its length first) and sent with 0x0066BB20 (Send_Remote_File).
#include "shared/lobby.hpp"
#include "shared/wol_chat.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/ipx_address.hpp"
#include "shared/system_timer.hpp"
#include "shared/cd_timer.hpp"
#include "shared/debug_printf.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/buffer_pipe.hpp"
#include "shared/lzo_pipe.hpp"
#include "shared/map_preview.hpp"
#include "shared/crt_string.hpp"
#include "shared/firewall_settings.hpp"
// @ref ??_7BufferPipe@@6B@=0079ea08
// @ref ??_C@_0BK@LFJL@Preview?5size?5is?5?$CFd?5bytes?6?$AA@=007e2754
// @ref ??_C@_0BN@KJJF@Starting?5map?5preview?5upload?6?$AA@=007e2794
// @ref ??_C@_0CA@DIKN@Creating?5low?9res?5preview?5image?6?$AA@=007e719c
// @ref ??_C@_0CC@OLL@Sending?5public?5options?5string?5?$CFs@=007e71f0
// @ref ??_C@_0CG@PAND@Compressed?5preview?5image?5is?5?$CFd?5b@=007e272c
// @ref ??_C@_0CO@INCK@Calling?5Send_Remote_File?5to?5send@=007e26f0
// @ref ??_C@_0DD@FMAM@Waiting?5for?5?$CFd?5players?5to?5signal@=007e71bc
// @ref ??_C@_0DE@JGDI@Player?5?$CFs?5has?5a?5NAT?5?9?5no?5preview@=007e7264
// @ref ??_C@_0EP@CBDM@Player?5?$CFs?5has?5different?5local?5an@=007e7214
// @ref ??_C@_0M@CNHP@Preview?4bin?$AA@=007e2720
// @ref ?Create_Paletted_Preview@MapPreviewClass@@QAEPAGHPAH@Z=0061e490
// @ref ?Send_Remote_File_0066bb20@@YI_NPBDHHH@Z=0066bb20
// @ref ?Unknown_00672F90@SessionClass@@QAEHH@Z=00672f90

void __fastcall Upload_Map_Preview_005c7ef0(MapPreviewClass *preview)
{
    CDTimerClass<SystemTimerClass> timer;
    char options[256];
    if (preview == 0 || unknown_players_a3fa7c.Count() <= 1) {
        return;
    }
    if (unknown_game_mode_a3d298 == 4) {
        bool direct = true;
        if (unknown_firewall_a71b18.unknown_00 != 0 && !(unknown_firewall_a71b18.unknown_00 & 1)) direct = false;
        for (int i = 1; direct && i < unknown_players_a3fa7c.Count(); i++) {
            int house = unknown_session_a3d298.Unknown_00672F90(i);
            if (house >= 0 && house < 8) {
                unsigned int flags = unknown_session_a3d298.unknown_299c.unknown[house];
                if (flags != 0 && !(flags & 1)) {
                    Debug_Printf("Player %s has a NAT - no preview download possible\n", Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name));
                    direct = false;
                    break;
                } else if (!unknown_session_a3d298.unknown_293c.Addresses[house].Is_Same_Node(unknown_players_a3fa7c[i]->Address)) {
                    Debug_Printf("Player %s has different local and external IPs - no preview download possible\n", Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name));
                    direct = false;
                    break;
                }
            }
        }
        if (!direct) {
            options[0] = 'Z';
            options[1] = '3';
            options[2] = 0;
            Debug_Printf("Sending public options string %s\n", options);
            unknown_chat_a72cfc->unknown_slot15(options);
            return;
        }
    }
    Debug_Printf("Starting map preview upload\n");
    UnknownGlobalPacket packet;
    memset(&packet, 0, sizeof(packet));
    packet.Command = 0x19;
    strcpy(packet.Name, unknown_name_a3d3a0);
    unknown_ipx_manager_a40978.Set_Timing(50, -1, 5000, true);
    timer = 1200;
    for (int i = 1; i < unknown_players_a3fa7c.Count(); i++) {
        unknown_ipx_manager_a40978.unknown_5259a0(&packet, sizeof(packet), 1, unknown_players_a3fa7c[i]->Name);
        Service_Game_IO_00481fb0();
    }
    while (unknown_ipx_manager_a40978.Global_Num_Send() && timer) {
        Service_Game_IO_00481fb0();
    }
    if (!timer) {
        return;
    }
    IPXAddressClass address;
    bool ready[8];
    memset(ready, 0, sizeof(ready));
    int count = 0;
    timer = 1800;
    Debug_Printf("Waiting for %d players to signal ready to receive\n", unknown_players_a3fa7c.Count() - 1);
    while (count < unknown_players_a3fa7c.Count() - 1 && timer) {
        Service_Game_IO_00481fb0();
        UnknownGlobalPacket reply;
        int length = sizeof(reply);
        unsigned short product;
        if (unknown_ipx_manager_a40978.Get_Global_Message(&reply, &length, &address, &product) && reply.Command == 0x1A) {
            for (int j = 1; j < unknown_players_a3fa7c.Count(); j++) {
                if (address.Is_Same_Node(unknown_players_a3fa7c[j]->Address)) {
                    if (!ready[j]) {
                        ready[j] = true;
                        count++;
                    }
                    break;
                }
            }
        }
    }
    if (count != unknown_players_a3fa7c.Count() - 1) {
        return;
    }
    Debug_Printf("Creating low-res preview image\n");
    int size = 0;
    unsigned short *image = preview->Create_Paletted_Preview(0x40, &size);
    Debug_Printf("Preview size is %d bytes\n", size);
    void *compressed = operator new(size * 4);
    BufferPipe target(compressed, size * 4);
    LZOPipe pipe(LZOPipe::COMPRESS, 0x2000);
    pipe.Put_To(&target);
    int length = pipe.Put(image, size);
    length += pipe.End();
    operator delete(image);
    Debug_Printf("Compressed preview image is %d bytes\n", length);
    RawFileClass file("Preview.bin");
    if (file.Is_Available()) {
        file.Delete();
    }
    file.Open(FILE_RIGHTS_WRITE);
    file.Write(&size, 4);
    file.Write(compressed, length);
    file.Close();
    operator delete(compressed);
    Debug_Printf("Calling Send_Remote_File to send the preview\n");
    Send_Remote_File_0066bb20("Preview.bin", 3, 1, 0);
    unknown_ipx_manager_a40978.Set_Timing(30, -1, 600, true);
}
