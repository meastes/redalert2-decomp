// wonline.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wonline.json carries the addresses and the rows that
// stay in their own files.


#define RA2_WOL_CHAT_STARTUP_SIGNATURES
#define RA2_ATL_COM_OBJECT_CREATE
#define RA2_WOL_INT_ARRAY_SET_EMPTY
#define RA2_WOL_ARRAY_LIST_REMOVE_HEAD
#define RA2_WOL_ARRAY_LIST_COPY
#define RA2_WOL_NAME_SET_CLEAR_INLINE
#define RA2_WOL_CHAT_SETTINGS_SIGNATURES
#define THEME_WHAT_IS_PLAYING
#define RA2_WOL_INT_ARRAY_MAKE_ROOM
#define RA2_WOL_ARRAY_LIST_HEAD_INSERT
#include "shared/ini_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/lobby.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/wol_sink.hpp"
#include "shared/wstring.hpp"
#include "shared/wonline_dialog_control.hpp"
#include "shared/ipx_address.hpp"
#include "shared/theme.hpp"
#include "shared/dialog_create.hpp"
#include "shared/keyboard.hpp"
#include "shared/id_source.hpp"
#include "shared/map_view.hpp"
#include "shared/session_state.hpp"
#include "shared/unknown_player_entry.hpp"
#include "shared/crt_string.hpp"
#include "shared/win32_window.hpp"
#include "shared/crt_stdio.hpp"
#include "shared/wol_settings.hpp"
#include "shared/wol_main.hpp"
#include "shared/crt_memory.hpp"
#include "shared/wide_string.hpp"


// The INI and file classes come first: Pick_Lobby 0x00744090 (later in this unit) depends on how
// INIClass and CCFileClass are declared relative to each other, and declaring both before lobby.hpp
// keeps it independent of declarations added to the wonline headers (VC6_MATCHING_NOTES "Fragile rows").
// Wonline requires full lobby container implementations before their first inclusion.
// Header-static dynamic initializers of the unit at 0x0073A6F0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x0073A6F0-0x0073A8D6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x0073A720, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_0073a6f0.cpp; no source of its own.


// Dynamic initializer (0x0073A740, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_0073a6f0.cpp; no source of its own.


// Dynamic initializer (0x0073A760, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_0073a6f0.cpp; no source of its own.


// Dynamic initializer (0x0073A780, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_0073a6f0.cpp; no source of its own.


// Dynamic initializer (0x0073A7A0, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_0073a6f0.cpp; no source of its own.


// Dynamic initializer (0x0073A7C0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_0073a6f0.cpp; no source of its own.


// Dynamic initializer (0x0073A800, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_0073a6f0.cpp; no source of its own.


// Dynamic initializer (0x0073A830, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_0073a6f0.cpp; no source of its own.


// Dynamic initializer (0x0073A860, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_0073a6f0.cpp; no source of its own.


// Dynamic initializer (0x0073A890, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_0073a6f0.cpp; no source of its own.


// Dynamic initializer (0x0073A8A0, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_0073a6f0.cpp; no source of its own.


// Dynamic initializer (0x0073A8C0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_0073a6f0.cpp; no source of its own.


// WOnline.cpp file statics: COM library lifetime (OleInitialize at startup, OleUninitialize at exit).
WIN32_IMPORT HRESULT WINAPI OleInitialize(void *reserved);
WIN32_IMPORT void WINAPI OleUninitialize();
struct OleLibrary {
    OleLibrary() { OleInitialize(0); }
    ~OleLibrary() { OleUninitialize(); }
};
static OleLibrary unknown_ole_library;


// atexit destructor of the static defined in unknown_ole_library_init_0073a8e0.cpp (0x0073A8E0); emitted by that definition, no source of its own.


// WOnline.cpp file static: the message of the day (unknown_motd_b26954, 0x00B26954).
Wstring unknown_motd_b26954;


// atexit destructor of the static defined in unknown_motd_b26954_init_0073a910.cpp (0x0073A910); emitted by that definition, no source of its own.


WolServerArray unknown_servers_b2a700;


// atexit destructor of the static defined in unknown_servers_b2a700_init_0073a940.cpp (0x0073A940); emitted by that definition, no source of its own.


WolChannelArray unknown_lobbies_b26c88;


// atexit destructor of the static defined in unknown_lobbies_b26c88_init_0073a980.cpp (0x0073A980); emitted by that definition, no source of its own.


WolChannelArray unknown_games_b29990;


// atexit destructor of the static defined in unknown_games_b29990_init_0073a9c0.cpp (0x0073A9C0); emitted by that definition, no source of its own.


WolUserArray unknown_users_b2a728;


// atexit destructor of the static defined in unknown_users_b2a728_init_0073aa00.cpp (0x0073AA00); emitted by that definition, no source of its own.


WolIntArray unknown_channel_requests_b29390;


// atexit destructor of the static defined in unknown_channel_requests_b29390_init_0073aa40.cpp (0x0073AA40); emitted by that definition, no source of its own.


WolLadderCache unknown_ladder_b296f8;


// Compiler-generated atexit destructor of the genuine unknown_ladder_b296f8 definition
// in Wonline_unknown_ladder_b296f8_Initializer_0073aa80.cpp; no handwritten helper body.


WolLadderCache unknown_ladder_b26970;


// Compiler-generated atexit destructor of the genuine unknown_ladder_b26970 definition
// in Wonline_unknown_ladder_b26970_Initializer_0073ac50.cpp; no handwritten helper body.


WolLadderCache unknown_ladder_b2ab98;


// Compiler-generated atexit destructor of the genuine unknown_ladder_b2ab98 definition
// in Wonline_unknown_ladder_b2ab98_Initializer_0073ae20.cpp; no handwritten helper body.


WolChannelNameSet unknown_known_names_b294b8;


// Compiler-generated atexit destructor of the genuine unknown_known_names_b294b8 definition
// in Wonline_unknown_known_names_b294b8_Initializer_0073aff0.cpp; no handwritten helper body.


WolStringArray unknown_game_names_b2a648;


// atexit destructor of the static defined in unknown_game_names_b2a648_init_0073b1c0.cpp (0x0073B1C0); emitted by that definition, no source of its own.


WolChannelNameSet unknown_channel_names_b293a0;


// Compiler-generated atexit destructor of the genuine unknown_channel_names_b293a0 definition
// in Wonline_unknown_channel_names_b293a0_Initializer_0073b220.cpp; no handwritten helper body.


WolStringArray unknown_pending_names_b29468;


// atexit destructor of the static defined in unknown_strings_b29468_init_0073b3f0.cpp (0x0073B3F0); emitted by that definition, no source of its own.


WolStringArray unknown_strings_b26a50;


// atexit destructor of the static defined in unknown_strings_b26a50_init_0073b450.cpp (0x0073B450); emitted by that definition, no source of its own.


WolStringArray unknown_strings_b26570;


// atexit destructor of the static defined in unknown_strings_b26570_init_0073b4b0.cpp (0x0073B4B0); emitted by that definition, no source of its own.


WolStringArray unknown_strings_b299b0;


// atexit destructor of the static defined in unknown_strings_b299b0_init_0073b510.cpp (0x0073B510); emitted by that definition, no source of its own.


WolStringArray unknown_strings_b29380;


// atexit destructor of the static defined in unknown_strings_b29380_init_0073b570.cpp (0x0073B570); emitted by that definition, no source of its own.


WolStringArray unknown_strings_b26960;


// atexit destructor of the static defined in unknown_strings_b26960_init_0073b5d0.cpp (0x0073B5D0); emitted by that definition, no source of its own.


WolStringArray unknown_strings_b2a6c8;


// atexit destructor of the static defined in unknown_strings_b2a6c8_init_0073b630.cpp (0x0073B630); emitted by that definition, no source of its own.


WolStringArray unknown_strings_b26580;


// atexit destructor of the static defined in unknown_strings_b26580_init_0073b690.cpp (0x0073B690); emitted by that definition, no source of its own.


WolStringArray unknown_strings_b29000;


// atexit destructor of the static defined in unknown_strings_b29000_init_0073b6f0.cpp (0x0073B6F0); emitted by that definition, no source of its own.


WolStringArray unknown_strings_b26920;


// atexit destructor of the static defined in unknown_strings_b26920_init_0073b750.cpp (0x0073B750); emitted by that definition, no source of its own.


WolStringArray unknown_strings_b29838;


// atexit destructor of the static defined in unknown_strings_b29838_init_0073b7b0.cpp (0x0073B7B0); emitted by that definition, no source of its own.


WolStringArray unknown_strings_b2a7d8;


// atexit destructor of the static defined in unknown_strings_b2a7d8_init_0073b810.cpp (0x0073B810); emitted by that definition, no source of its own.


// Dynamic initializer of a global Wstring (0x0073B870, _$E): constructs
// unknown_wstring_b26944 (0x00B26944) and registers its atexit destructor
// (0x0073B890, the following _$E draft, emitted by this definition).
// The owning unit (a small WOL-area unit) is not identified yet.

Wstring unknown_wstring_b26944;


// atexit destructor of unknown_wstring_b26944 (0x0073B890, _$E): emitted by
// the definition in unknown_wstring_b26944_init_0073b870.cpp; no source of its own.


// Dynamic initializer of a global Wstring (0x0073b8a0, _$E): constructs
// unknown_wstring_b29cd4 (0x00b29cd4) and registers its atexit destructor
// (0x0073b8c0, the following _$E draft, emitted by this definition).
// The owning unit (a small WOL-area unit) is not identified yet.

Wstring unknown_wstring_b29cd4;


// atexit destructor of unknown_wstring_b29cd4 (0x0073b8c0, _$E): emitted by
// the definition in unknown_wstring_b29cd4_init_0073b8a0.cpp; no source of its own.


WolNameTable unknown_names_b29848;


// Compiler-generated atexit destructor of the genuine unknown_names_b29848 definition
// in Wonline_unknown_names_b29848_Initializer_0073b8d0.cpp; no handwritten helper body.


WolChannelDictionary unknown_channels_b29478;


// Compiler-generated atexit destructor of the genuine unknown_channels_b29478 definition
// in Wonline_unknown_channels_b29478_Initializer_0073baa0.cpp; no handwritten helper body.


extern WolStringArray unknown_strings_b29828;
void Destroy_Wonline_Strings_0073bd40()
{
    unknown_strings_b29828.~WolStringArray();
}


class Leaf_73bc80 { public: void get(); };
void Leaf_73bc80::get() { unsigned int scratch = (unsigned int)this; unsigned int value; unsigned int spare; value = *(unsigned int*)(0x7fd30c); scratch = *(unsigned int*)(0x7fd308); value = (value) - (scratch); value = (value) - (4); *(unsigned int*)(0xb29460) = value; }


class Leaf_73bca0 { public: void get(); };
void Leaf_73bca0::get() { unsigned int scratch = (unsigned int)this; unsigned int value; unsigned int spare; value = *(unsigned int*)(0x7fd31c); scratch = *(unsigned int*)(0x7fd318); value = (value) - (scratch); value = (value) - (4); *(unsigned int*)(0xb26cb4) = value; }


// 0x0073bdb0: function not yet identified
//
// Body: 24 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: undefined4 unknown_mode_1_or_2_0073bdb0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0073bdb0 {

extern "C" unsigned int unknown_mode_00a3d2a4;

unsigned int  unknown_mode_1_or_2_0073bdb0(void)
{
    if ((unknown_mode_00a3d2a4 != 2) && (unknown_mode_00a3d2a4 != 1)) {
        return 0;
    }
    return 1;
}

}  // namespace seed_0073bdb0


// 0x0073bfc0: function not yet identified
//
// Body: 37 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void unknown_timer_init_0073bfc0(void)
// Callees with accepted source:
//   unknown_timer_atexit_0073bff0 -> src/recovered/0073bff0.cpp
//   atexit_0078298a -> src/recovered/0078298a.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0073bfc0 {

struct Callee_0069a430 { unsigned int SystemTimer_Value_0069a430(); };
extern void __cdecl unknown_timer_atexit_0073bff0(void);
extern unsigned int  atexit_0078298a(unsigned int);
extern "C" unsigned int unknown_timer_start_00b292e8;
extern "C" unsigned int unknown_system_timer_00b292ec;
extern "C" unsigned int unknown_timer_accum_00b292f0;

void  unknown_timer_init_0073bfc0(void)
{
    unknown_timer_start_00b292e8 = ((Callee_0069a430*)(&unknown_system_timer_00b292ec))->SystemTimer_Value_0069a430();
    unknown_timer_accum_00b292f0 = 0;
    atexit_0078298a((unsigned int)(unknown_timer_atexit_0073bff0));
    return;
}

}  // namespace seed_0073bfc0


// 0x0073bff0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void unknown_timer_atexit_0073bff0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0073bff0 {

void  unknown_timer_atexit_0073bff0(void)
{
    return;
}

}  // namespace seed_0073bff0


// SimpleWonlineDialogControl::SimpleWonlineDialogControl() (0x0073C000)

SimpleWonlineDialogControl::SimpleWonlineDialogControl()
{
}


// SimpleWonlineDialogControl::~SimpleWonlineDialogControl() (0x0073C020)

SimpleWonlineDialogControl::~SimpleWonlineDialogControl()
{
}


// bool SimpleWonlineDialogControl::unknown_slot4() (0x0073C030, slot 4)
// Runs the World Online lobby services (0x0073E100).

bool SimpleWonlineDialogControl::unknown_slot4()
{
    return Unknown_0073e100();
}


// 0x0073c040: function not yet identified
//
// Body: 34 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_spaces_to_underscores_0073c040(undefined4 param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0073c040 {

extern unsigned int  strchr_00784130(unsigned int *, char);

void __fastcall unknown_spaces_to_underscores_0073c040(unsigned int text)
{
    unsigned char *space;

    for (space = (unsigned char *)strchr_00784130((unsigned int *)(text), (char)(0x20)); space != (unsigned char *)0x0;
            space = (unsigned char *)strchr_00784130((unsigned int *)(space), (char)(0x20))) {
        *space = 0x5f;
    }
    return;
}

}  // namespace seed_0073c040


// bool __fastcall Unknown_0073c070(WOL::User const *user) (0x0073C070)
// Whether a message from user shows: from the channel owner (flag bit 0), with dialog 0xC2 or 0xC9
// open, or else when unknown_b2a7e8 is set; never without a user. The narrow counterpart of
// unknown_can_send_chat 0x0073C0B0 (OnPrivateMessage inlines this test).

bool __fastcall Unknown_0073c070(WOL::User const *user)
{
    if (!user) return false;
    if (user->flags & 1) return true;
    if (WS_Find_Dialog_00735940(0xC2)) return true;
    if (WS_Find_Dialog_00735940(0xC9)) return true;
    return unknown_b2a7e8 != 0;
}


// bool Unknown_0073c120() (0x0073C120)
// The popup callback of the disconnect messages: runs the World Online lobby services (0x0073E100).

bool Unknown_0073c120()
{
    return Unknown_0073e100();
}


// IPXAddressClass __fastcall Unknown_0073c130(WOL::User const *user) (0x0073C130)
// The address of a user from its IP (+0x18, network order): net 0, the IP in the first four node bytes.

IPXAddressClass __fastcall Unknown_0073c130(WOL::User const *user)
{
    NetNumType net;
    NetNodeType node;
    memset(net, 0, sizeof(net));
    memset(node, 0, sizeof(node));
    unsigned long ip = ntohl(user->unknown_18);
    node[3] = (unsigned char)ip;
    node[2] = (unsigned char)(ip >> 8);
    node[1] = (unsigned char)(ip >> 16);
    node[0] = (unsigned char)(ip >> 24);
    IPXAddressClass address(net, node, 0);
    return address;
}


// void __fastcall Unknown_0073c1b0(bool online) (0x0073C1B0)
// The WOnline menu music: with online set, restarts the score (theme -2) unless a theme other than
// "INTRO" is playing or queued; otherwise queues theme -3.

void __fastcall Unknown_0073c1b0(bool online)
{
    if (online) {
        if (!Theme.Still_Playing()) {
            Theme.Stop();
            Theme.Queue_Song(-2);
            return;
        }
        int theme = Theme.Score;
        if (theme == -1) theme = Theme.Pending;
        if (theme == Theme.From_Name("INTRO")) {
            Theme.Stop();
            Theme.Queue_Song(-2);
        }
    } else {
        Theme.Queue_Song(-3);
    }
}


// void __fastcall Unknown_0073c230() (0x0073C230; fastcall: register setups before the pushes)
// Runs dialog 0xCF (procedure 0x007485D0) modally with the keyboard cleared around it, then
// restores the input flag from the scenario's pause state and redraws the map.

void __fastcall Unknown_0073c230()
{
    HWND dialog = unknown_create_dialog_5ff610(0xCF, Unknown_Dialog_Proc_007485d0, 0);
    unknown_flag_a40d44 = true;
    WWKeyboard->Clear();
    ShowWindow(dialog, 1);
    Unknown_007359e0(dialog, unknown_6000d0, 0, 1, 1);
    WWKeyboard->Clear();
    unknown_flag_a40d44 = unknown_id_source_a3d290->unknown_2306;
    Map.Flag_To_Redraw(2);
}


// void Unknown_0073c2b0() (0x0073C2B0)
// Opens the Westwood Online registration page for language code unknown_b2ac5c.

void Unknown_0073c2b0()
{
    char url[256];
    sprintf(url, "http://games2.westwood.com/cgi-bin/cgiclient?register&request=reg_menu&LANGCODE=%d", unknown_b2ac5c);
    unknown_open_url_73c580(url, 0);
}


// int __fastcall Unknown_0073c2e0(Wstring const &name) (0x0073C2E0)
// The index of the user list entry with this name (case-insensitive), -1 when none.

int __fastcall Unknown_0073c2e0(Wstring const &name)
{
    return unknown_users_b2a728.Find_Name(name);
}


// Channel test of a game mode 4 session (false in any other mode).

// 0x0073C340: the channel's +0x14 is set or its flag bit 8.
bool __fastcall Unknown_0073c340(WOL::Channel const *channel)
{
    if (unknown_session_a3d298.Type != 4) return false;
    return channel->unknown_14 || Unknown_0073c3c0(channel);
}


// Channel test of a game mode 4 session (false in any other mode).

// 0x0073C370: the same test of the current game channel.
bool Unknown_0073c370()
{
    return Unknown_0073c340(&unknown_game_channel_b299c0);
}


// Channel test of a game mode 4 session (false in any other mode).

// 0x0073C3A0: flag bit 8 of the current game channel.
bool Unknown_0073c3a0()
{
    return Unknown_0073c3c0(&unknown_game_channel_b299c0);
}


// Channel test of a game mode 4 session (false in any other mode).

// 0x0073C3C0: the channel's flag bit 8.
bool __fastcall Unknown_0073c3c0(WOL::Channel const *channel)
{
    if (unknown_session_a3d298.Type != 4) return false;
    return (channel->unknown_20 >> 8) & 1;
}


// Channel test of a game mode 4 session (false in any other mode).

// 0x0073C3E0: the channel's flag bit 11.
bool __fastcall Unknown_0073c3e0(WOL::Channel const *channel)
{
    if (unknown_session_a3d298.Type != 4) return false;
    return (channel->unknown_20 >> 11) & 1;
}


// Channel test of a game mode 4 session (false in any other mode).

// 0x0073C400: the channel's flag bit 12.
bool __fastcall Unknown_0073c400(WOL::Channel const *channel)
{
    if (unknown_session_a3d298.Type != 4) return false;
    return (channel->unknown_20 >> 12) & 1;
}


bool __fastcall Unknown_005c9320(NodeNameType *entry);
Wstring __fastcall Unknown_Lobby_Entry_Name_005cd0e0(NodeNameType *entry);
extern "C" char *__cdecl strncat(char *destination,char const *source,unsigned count);
void __fastcall Unknown_0073c480()
{
    char text[256];
    strcpy(text,"L");
    for(int i=0;i<8;++i) {
        NodeNameType *entry=unknown_player_slots_a3fa98[i];
        Wstring name;
        if(Unknown_005c9320(entry)) name=Narrow_Copy_00771280(entry->Name);
        else name=Unknown_Lobby_Entry_Name_005cd0e0(entry);
        strncat(text,name.Get(),256);
        strncat(text,",",256);
    }
    unknown_chat_a72cfc->unknown_slot15(text);
}


struct _SECURITY_ATTRIBUTES;
struct _STARTUPINFOA {
    DWORD cb;
    char *lpReserved,*lpDesktop,*lpTitle;
    DWORD dwX,dwY,dwXSize,dwYSize,dwXCountChars,dwYCountChars,dwFillAttribute,dwFlags;
    unsigned short wShowWindow,cbReserved2;
    unsigned char *lpReserved2;
    HANDLE hStdInput,hStdOutput,hStdError;
};
struct _PROCESS_INFORMATION { HANDLE hProcess,hThread; DWORD dwProcessId,dwThreadId; };
WIN32_IMPORT HINSTANCE WINAPI FindExecutableA(char const *,char const *,char *);
WIN32_IMPORT BOOL WINAPI CreateProcessA(char const *,char *,_SECURITY_ATTRIBUTES *,_SECURITY_ATTRIBUTES *,BOOL,DWORD,void *,char const *,_STARTUPINFOA *,_PROCESS_INFORMATION *);
WIN32_IMPORT DWORD WINAPI WaitForInputIdle(HANDLE,DWORD);
WIN32_IMPORT BOOL WINAPI GetExitCodeProcess(HANDLE,DWORD *);
WIN32_IMPORT HWND WINAPI GetTopWindow(HWND);
WIN32_IMPORT HWND WINAPI GetForegroundWindow();
extern "C" char *__cdecl tmpnam(char *);
extern "C" int __cdecl _unlink(char const *);
extern char html_extension_007fd65c[],html_write_mode_007e09c0[],html_title_007fd640[];
extern char html_command_007fd60c[],html_launch_label_007fd618[],html_missing_label_007fd5fc[],wol_source_file_007fd62c[];
void __fastcall View_HTML_0073c580(char const *url,int no_ask)
{
    char filename[160];
    tmpnam(filename);
    strcat(filename,html_extension_007fd65c);
    _iobuf *file=fopen(filename,html_write_mode_007e09c0);
    fprintf(file,html_title_007fd640);
    fclose(file);
    char executable[261];
    int result=(int)FindExecutableA(filename,0,executable);
    _unlink(filename);
    if(result>32) {
        if(!no_ask && Popup_005c3230(StringTable::Load_String(html_launch_label_007fd618,0,wol_source_file_007fd62c,0x494),4,Unknown_0073c120,0)!=6) return;
        char command_line[270];
        sprintf(command_line,html_command_007fd60c,url);
        _STARTUPINFOA startup;
        memset(&startup,0,sizeof(startup));
        startup.cb=sizeof(startup);
        _PROCESS_INFORMATION process_info;
        if(CreateProcessA(executable,command_line,0,0,false,0,0,0,&startup,&process_info)) {
            HANDLE process=process_info.hProcess;
            if(process) {
            int done=0;
            Unknown_0073e100();
            WaitForInputIdle(process,5000);
            do {
                Unknown_0073e100();
                Sleep(500);
                DWORD exit_code;
                GetExitCodeProcess(process,&exit_code);
                if(exit_code!=0x103) done=1;
                if(unknown_flag_a40d28 || GetTopWindow(0)==MainWindow || GetForegroundWindow()==MainWindow) done=1;
                Unknown_0073e100();
            } while(!done);
            Unknown_0073e100();
            if(GetTopWindow(0)!=MainWindow) {
                Unknown_0073e100();
                SetForegroundWindow(MainWindow);
                ShowWindow(MainWindow,9);
                Unknown_0073e100();
            }
            for(int waits=10;waits;--waits) {
                Sleep(100);
                Unknown_0073e100();
            }
            }
        }
    } else Popup_005c3230(StringTable::Load_String(html_missing_label_007fd5fc,0,wol_source_file_007fd62c,0x4d8),0,Unknown_0073c120,0);
}


void __fastcall Apply_WOL_Settings_0073c990();
inline void Restart_Lobby_Music() { Theme.Stop(false); Theme.Queue_Song(-2); }
void __fastcall Apply_WOL_Settings_0073c990()
{
    if(unknown_chat_a72cfc) {
        unknown_chat_a72cfc->SetLangFilter(unknown_wol_lang_filter_b2a7e4);
        unknown_chat_a72cfc->SetFindPage(unknown_wol_allow_find_b2a818,unknown_wol_allow_page_b290a0);
    }
    Unknown_005cd540(0);
    bool music=unknown_wol_lob_music_b297c8!=0;
    if(music && !Theme.Still_Playing()) Restart_Lobby_Music();
    else if(music) { if(Theme.What_Is_Playing()==Theme.From_Name("INTRO")) Restart_Lobby_Music(); }
    else if(!music) Theme.Queue_Song(-3);
}


HWND __fastcall Unknown_00735B10();
extern int unknown_selected_actions_b294f4[128];
int __fastcall Unknown_0073ca50(WideString const &message)
{
    HWND users=GetDlgItem(Unknown_00735B10(),0x53e);
    int count=SendMessageA(users,0x191,32,(LPARAM)unknown_selected_actions_b294f4);
    if(count==0) {
        unknown_chat_a72cfc->unknown_slot58(message.unknown_text_770430());
        return 0;
    } else {
        WOL::User *last=0,*head=0,*item=0;
        WOL::User source;
        memset(&source,0,sizeof(source));
        for(int i=0;i<count;++i) {
            unknown_users_b2a728.Get_Copy(source,unknown_selected_actions_b294f4[i]);
            item=new WOL::User;
            *item=source;
            item->next=0;
            if(i==0) head=item;
            if(last) last->next=item;
            last=item;
        }
        if(head) unknown_chat_a72cfc->unknown_slot59(head,message.unknown_text_770430());
        while(head) { item=head; head=head->next; delete item; }
        return 1;
    }
}


HWND __fastcall Unknown_00735B10();
extern int unknown_selected_users_b26694[128];
int __fastcall Unknown_0073cb80(WideString const &message)
{
    HWND users=GetDlgItem(Unknown_00735B10(),0x53e);
    int count=SendMessageA(users,0x191,32,(LPARAM)unknown_selected_users_b26694);
    if(count==0) {
        unknown_chat_a72cfc->unknown_slot60(message.unknown_text_770430());
        return 0;
    } else {
        WOL::User *last=0,*head=0,*item=0;
        WOL::User source;
        memset(&source,0,sizeof(source));
        for(int i=0;i<count;++i) {
            unknown_users_b2a728.Get_Copy(source,unknown_selected_users_b26694[i]);
            item=new WOL::User;
            *item=source;
            item->next=0;
            if(i==0) head=item;
            if(last) last->next=item;
            last=item;
        }
        if(head) unknown_chat_a72cfc->unknown_slot61(head,message.unknown_text_770430());
        while(head) { item=head; head=head->next; delete item; }
        return 1;
    }
}


// void __fastcall Encode_Game_Options_0073ccb0(char *out) (0x0073CCB0)
// Writes the Westwood Online game options text: the scenario settings, one record per player and
// the computer player tables. The Tiberian Sun twin is Encode_Game_Options. The names of the
// address, port and ping values in the player records are hypotheses.
extern IPXAddressClass wol_addresses_a3fbd4[8];
extern int wol_ports_a3fc34[],wol_pings_a40238[];
extern char wol_useroptions_b29ad4[512];
Wstring __fastcall Wide_To_7_Bit(WideString const &wide);
inline bool Same_Player_Name(unsigned short const *left,unsigned short const *right)
{
    char *name=Wide_To_Narrow_006fa120(left);
    return _stricmp(name,Wide_To_Narrow_006fa120(right))==0;
}
void __fastcall Encode_Game_Options_0073ccb0(char *out)
{
    Wstring name = unknown_map_name_a3d342 ? Wide_To_7_Bit(unknown_map_name_a3d342) : "";
    int value=unknown_session_a3d298.unknown_884;
    sprintf(out,"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%s,%d,%d,%d,%s,%s:",unknown_a40d3c,unknown_a40b18,unknown_session_a3d298.GameSpeed_30,unknown_session_a3d298.unknown_24,unknown_session_a3d298.UnitCount,
        unknown_session_a3d298.unknown_2a,unknown_session_a3d298.unknown_2b,unknown_session_a3d298.unknown_2c,unknown_session_a3d298.unknown_a8,unknown_session_a3d298.unknown_crates_29,unknown_session_a3d298.GameModeIndex,
        name.Get(),value,unknown_session_a3d298.ScenarioIsOfficial_880,unknown_session_a3d298.ScenarioFileLength_87c,unknown_session_a3d298.ScenarioFileName_658,unknown_session_a3d298.ScenarioDigest_85a);
    memset(wol_useroptions_b29ad4,0,sizeof(wol_useroptions_b29ad4));
    for(int i=0,count=unknown_players_a3fa7c.Count();i<count;i++) {
        NodeNameType *entry=unknown_players_a3fa7c[i];
        int country=entry->unknown_66e290();
        int color=entry->unknown_66e2a0();
        int address=0,port=0,ping=0;
        for(int j=0;j<8;j++) {
            if(Same_Player_Name(unknown_players_a3fa7c[i]->Name,unknown_session_a3d298.PlayerSlots[j]->Name)) {
                NetNumType net; NetNodeType node; unsigned short socket;
                wol_addresses_a3fbd4[j].Get_Address(net,node,&socket);
                address=*(int *)node;
                port=wol_ports_a3fc34[j];
                ping=wol_pings_a40238[j];
                break;
            }
        }
        Wstring text(Narrow_Copy_00771280(unknown_players_a3fa7c[i]->Name));
        sprintf(wol_useroptions_b29ad4+strlen(wol_useroptions_b29ad4),"%s,%d,%d,%x,%x,%x,",text.Get(),country,color,address,port,ping);
    }
    wol_useroptions_b29ad4[strlen(wol_useroptions_b29ad4)-1]=':';
    strcat(wol_useroptions_b29ad4,"@:");
    for(int k=0;k<8;k++) sprintf(wol_useroptions_b29ad4+strlen(wol_useroptions_b29ad4),"%d,%d,%d,",unknown_session_a3d298.AIDifficulty[k],unknown_session_a3d298.AICountry[k],unknown_session_a3d298.AIColor[k]);
    strcat(out,wol_useroptions_b29ad4);
}


#include "shared/gdlg_supp.hpp"
#include "shared/debug_printf.hpp"
#include "shared/modem_host.hpp"
#include "shared/player_profile.hpp"
#include "shared/wol_ladder_list_cell.hpp"
#include "shared/string_table.hpp"
#include "shared/multiplayer_game_mode.hpp"
#include "shared/multi_mission.hpp"
#include "shared/lobby_name_set.hpp"
#include "shared/house_type.hpp"
#include "shared/pcx_cache.hpp"
#include "shared/atl_module.hpp"
#include "shared/connection_points.hpp"
#define APP_REG_KEY "SOFTWARE\\Westwood\\Red Alert 2"
#include "shared/map_preview.hpp"
#include "shared/wol_registry_options.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/crt_time.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/options.hpp"
#include "shared/create_game_dialog_control.hpp"
#include "shared/rules.hpp"
#include "shared/sound_controls.hpp"
#include "shared/win_util.hpp"
#include "shared/options_dialog.hpp"
#include "shared/progress_screen.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/win32_api.hpp"
#include "shared/firewall_settings.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/cd_timer.hpp"
#include "shared/system_timer.hpp"
#include "shared/udp_interface.hpp"
#include "shared/owner_talk.hpp"
#include "shared/network_statistics.hpp"
#include "shared/dialog_default_proc.hpp"
#include "shared/wol_chat.hpp"
#include "shared/wol_list_text.hpp"
#include "shared/mouse.hpp"
#include "shared/wdt_campaign_layout.hpp"
#include "shared/unknown_map_generator.hpp"
#include "shared/com_api.hpp"
#include "shared/com_persist.hpp"
#include "shared/gadget.hpp"
#include "shared/sound_play.hpp"


// unsigned int Unknown_0073cfd0() (0x0073CFD0)
// The exclusive or of three words (0x00B2AC18, 0x00B2AC20, 0x00B2AC1C).

unsigned int Unknown_0073cfd0()
{
    return unknown_b2ac18 ^ unknown_b2ac20 ^ unknown_b2ac1c;
}


// Complete original114B+14NOP; full original body mapped before source.
extern char unknown_scenario_a3d8f0[];
extern char user_options_format_7fd6c4[];
int __fastcall Channel_Version_00737af0();
namespace seed_005c9400 { int Count_AI_Slots_005c9400(); }
void __fastcall Format_User_Options_0073cff0(char *text)
{
    int country_zero_count=0;
    int country_one_count=0;
    for(int index=0;index<(int)unknown_game_channel_b299c0.currentUsers;++index) {
        int country=unknown_player_b29cf0[index].unknown_18;
        if(country==0) ++country_zero_count;
        else if(country==1) ++country_one_count;
    }
    int free_sides=0;
    if(country_one_count<(int)unknown_game_channel_b299c0.maxUsers/2) free_sides=2;
    if(country_zero_count<(int)unknown_game_channel_b299c0.maxUsers/2) ++free_sides;
    unsigned int check=Unknown_0073cfd0();
    sprintf(text,user_options_format_7fd6c4,Channel_Version_00737af0(),check,seed_005c9400::Count_AI_Slots_005c9400(),Unknown_005c94b0(),!No_Observer_Slot_005c9490(),unknown_scenario_a3d8f0);
}


// void __fastcall Parse_Channel_Info_0073d070(char *text, int *first, int *second, int *third, int *fourth, int *fifth, Wstring *rest) (0x0073D070)
// Splits a comma list: five numbers (0 for a missing token) and the next token as the text slot.

extern "C" long __cdecl atol(char const *text);

void __fastcall Parse_Channel_Info_0073d070(char *text, int *first, int *second,
                                                             int *third, int *fourth, int *fifth,
                                                             Wstring *rest)
{
    char *token = strtok(text, ",");
    *first = token != 0 ? atol(token) : 0;
    token = strtok(0, ",");
    *second = token != 0 ? atol(token) : 0;
    token = strtok(0, ",");
    *third = token != 0 ? atol(token) : 0;
    token = strtok(0, ",");
    *fourth = token != 0 ? atol(token) : 0;
    token = strtok(0, ",");
    *fifth = token != 0 ? atol(token) : 0;
    *rest = strtok(0, ",");
}


// Complete original 0073D150,661 body bytes and11 alignment bytes.
HWND __fastcall WS_Find_Dialog_00735940(int id);
void __fastcall Unknown_0073FEE0(int unknown, bool update);
extern int unknown_b2ac2c, unknown_b2ab88;
extern long unknown_b2a8a4;

void __fastcall Unknown_0073d150(Wstring const &name, int value)
{
    Wstring nick(name.Length() > 0 ? name : unknown_wstring_b26cb8);
    if (nick == unknown_wstring_b26cb8 && value == 0) {
        HWND dialog = WS_Find_Dialog_00735940(0xc9);
        Unknown_005cd030(dialog, true);
    }
    WOL::User *user = 0;
    for (int index = 0; index < unknown_users_b2a728.Count; ++index) {
        unknown_users_b2a728.Get(user, index);
        if (_stricmp(nick.Peek_Buffer(), user->name) == 0) {
            if (index != -1) {
                unknown_player_b29cf0[index].unknown_20 = value;
                Unknown_0073FEE0(0, false);
            }
            break;
        }
    }
    bool local = unknown_b2ac58 == 1;
    Wstring lower = nick.As_Lower();
    int player_index;
    for (player_index = 0; player_index < unknown_players_a3fa7c.Count(); ++player_index) {
        Wstring player = Narrow_Copy_00771280(unknown_players_a3fa7c[player_index]->Name);
        if (player.As_Lower() == lower) break;
    }
    if (player_index >= unknown_players_a3fa7c.Count()) return;
    unknown_players_a3fa7c[player_index]->unknown_67 = value;
    if (local && Is_Online_Session() && !unknown_b2ac04 && player_index == 1 && unknown_users_b2a728.Count == 2) {
        WOL::User *node;
        WOL::User *head = 0;
        WOL::User copy;
        memset(&copy, 0, sizeof(copy));
        for (int user_index = 0; user_index < unknown_users_b2a728.Count; ++user_index) {
            unknown_users_b2a728.Get_Copy(copy, user_index);
            node = new WOL::User;
            *node = copy;
            node->next = head;
            head = node;
        }
        unknown_b2ac2c = 0;
        unknown_b26898 = 9;
        unknown_b2ab88 = 90;
        unknown_b2a8a4 = time(0);
        unknown_chat_a72cfc->RequestGameStart(head);
        unknown_b2ac04 = true;
        node = head;
        while (node) {
            head = node->next;
            delete node;
            node = head;
        }
    }
    Unknown_0073FEE0(0, false);

}


// int __fastcall Unknown_0073d3f0(Wstring const &name) (0x0073D3F0)
// The session player number of a WOL nickname (empty: the local nickname): 1 for the host, else +0x67 of the
// session player entry with that name (case-insensitive), -1 when there is none.

int __fastcall Unknown_0073d3f0(Wstring const &name)
{
    Wstring nick(name.Length() > 0 ? name : unknown_wstring_b26cb8);
    Wstring lower = nick.As_Lower();
    if (Wstring_To_Wide(lower) == WideString(unknown_host_name_a3f618).As_Lower()) {
        return 1;
    }
    int found = -1;
    for (int index = 0; index < unknown_players_a3fa7c.Count(); index++) {
        Wstring player = Narrow_Copy_00771280(unknown_players_a3fa7c[index]->Name);
        if (player.As_Lower() == lower) found = index;
    }
    if (found != -1) {
        return unknown_players_a3fa7c[found]->unknown_67;
    }
    return -1;
}


int __fastcall Set_Channel_Player_Options_0073d550(Wstring const &name,int country,int color)
{
    int found=-1;
    Wstring lower=name.As_Lower();
    for(int index=0;index<8;++index) {
        Wstring player_name=Narrow_Copy_00771280(unknown_player_slots_a3fa98[index]->Name);
        if(lower==player_name.As_Lower()) {
            found=index;
            break;
        }
    }
    int changed=0;
    if(found==-1) return changed;
    HWND window=(HWND)WS_Find_Dialog_00735940(0xc2);
    if(!window) window=(HWND)WS_Find_Dialog_00735940(0xc9);
    NodeNameType *player=unknown_player_slots_a3fa98[found];
    if(player->unknown_66e290()!=country) {
        changed=1;
        player->unknown_66e2d0(country);
        int side=Player_Side_Combo_ID_4d1350(found);
        int previous=Side_Combo_Data_4d1c70(window,side,-1);
        if(country==-3) {
            if(previous!=-3) {
                Fill_Observer_Combo_4d1640(window,side);
                int observer_control=Slot_Control_Id_005c97a0(found);
                SendMessageA(GetDlgItem(window,observer_control),0x4f1,0,1);
                int color_control=Player_Color_Combo_ID_4d1cb0(found);
                Fill_Random_Color_Combo_4d2250(window,color_control);
                Select_Color_Combo_4d2480(window,color_control,8);
            }
        } else if(previous==-3) {
            Fill_Side_Combo_4d1560(window,side);
            int observer_control=Slot_Control_Id_005c97a0(found);
            SendMessageA(GetDlgItem(window,observer_control),0x4f1,0,0);
            int color_control=Player_Color_Combo_ID_4d1cb0(found);
            Fill_Color_Combo_4d2080(window,color_control);
            Select_Color_Combo_4d2480(window,color_control,-2);
        }
        if(player->unknown_66e290()==-3) player->Team=-1;
    }
    if(unknown_player_slots_a3fa98[found]->unknown_66e2a0()!=color) player->unknown_66e300(color);
    if(unknown_player_slots_a3fa98[found]==unknown_players_a3fa7c[0]) unknown_a3d3b4=color;
    Unknown_0073FEE0(0,false);
    return changed;
}


extern char wol_channel_request_debug_7fd6d8[];
bool __fastcall Request_Channel_List_0073d760(int channel_type,unsigned char autoping)
{
    Debug_Printf(wol_channel_request_debug_7fd6d8,channel_type,autoping);
    if(unknown_chat_a72cfc->unknown_slot06(channel_type,autoping)!=0) return false;
    unknown_channel_requests_b29390.Add_Tail(channel_type);
    return true;
}


extern "C" __declspec(dllimport) DWORD __stdcall WaitForMultipleObjects(DWORD,HANDLE*,BOOL,DWORD);
extern "C" int __cdecl swprintf(wchar_t *,wchar_t const *,...);
extern HANDLE wol_wait_events_b264f8[18];
extern char wol_channel_list_label_007fd754[],wol_connect_failed_label_007fd740[],wol_wait_failed_007fd708[],wol_source_file_007fd62c[];
void __fastcall Open_Wait_Window_0074d4d0(unsigned int,WideString const &,bool);
inline void __fastcall Close_Wonline_Wait(unsigned int flags)
{
    void *wait=WS_Find_Dialog_00735940(0xc4);
    if(wait && !(unknown_b2abe0&=~flags)) Unknown_007357E0(wait,0);
    HWND current=Unknown_00735B10();
    HWND edit=GetDlgItem(current,0x540);
    if(edit) SetFocus(edit);
}
bool __fastcall Get_Channel_List_0073d8f0()
{
    {
        WideString title(StringTable::Load_String(wol_channel_list_label_007fd754,0,wol_source_file_007fd62c,0x7fa));
        Open_Wait_Window_0074d4d0(1,title,false);
    }
    int elapsed=0;
    for(;;) {
        DWORD event=WaitForMultipleObjects(18,wol_wait_events_b264f8,0,100);
        while(event==0x102) {
            elapsed+=100;
            if(elapsed>30000) {
                swprintf(unknown_server_error_b2a8ec,StringTable::Load_String(wol_connect_failed_label_007fd740,0,wol_source_file_007fd62c,0x803));
                SetEvent(wol_wait_events_b264f8[0]);
                return false;
            }
            Unknown_0073e100();
            event=WaitForMultipleObjects(18,wol_wait_events_b264f8,0,100);
        }
        if(event==(DWORD)-1) {
            Close_Wonline_Wait(1);
            Debug_Printf(wol_wait_failed_007fd708);
            return false;
        }
        ResetEvent(wol_wait_events_b264f8[event]);
        if(event==5) {
            Close_Wonline_Wait(1);
            return false;
        }
        if(event==0) {
            Close_Wonline_Wait(1);
            break;
        }
        if(event==3) {
            Close_Wonline_Wait(1);
            return true;
        }
    }
    return false;
}


extern HANDLE wol_wait_events_b264f8[18];
bool __fastcall Get_Channel_List_0073d8f0();
bool __fastcall Request_Channel_List_0073d760(int channel_type,unsigned char autoping);
inline void Reset_Wait_Events()
{
    for(int i=0;i<18;++i) ResetEvent(wol_wait_events_b264f8[i]);
}

bool __fastcall Queue_Channel_List_Request_0073db20(int channel_type,unsigned char autoping,bool wait)
{
    if(unknown_channel_requests_b29390.Count) {
        bool result=Get_Channel_List_0073d8f0();
        if(!result) return result;
    }
    Reset_Wait_Events();
    bool requested=Request_Channel_List_0073d760(channel_type,autoping);
    if(!requested) {
        SetEvent(wol_wait_events_b264f8[0]);
        return false;
    }
    if(wait) {
        bool result=Get_Channel_List_0073d8f0();
        return result;
    }
    return true;
}


extern HANDLE wol_wait_events_b264f8[18];
bool __fastcall Get_Channel_List_0073d8f0();
bool __fastcall Request_Channel_List_0073d760(int channel_type,unsigned char autoping);
bool __fastcall Request_Chat_Channel_List_0073dd20(bool wait)
{
    if(unknown_channel_requests_b29390.Count) {
        bool result=Get_Channel_List_0073d8f0();
        if(!result) return result;
    }
    for(int i=0;i<18;++i) ResetEvent(wol_wait_events_b264f8[i]);
    if(!Request_Channel_List_0073d760(0,0)) {
        SetEvent(wol_wait_events_b264f8[0]);
        return false;
    }
    if(wait) return Get_Channel_List_0073d8f0();
    return true;
}


extern HANDLE wol_wait_events_b264f8[18];
bool __fastcall Get_Channel_List_0073d8f0();
bool __fastcall Request_Channel_List_0073d760(int channel_type,unsigned char autoping);
bool __fastcall Request_Game_Channel_List_0073df10(unsigned char autoping,bool wait)
{
    if(unknown_channel_requests_b29390.Count) {
        bool result=Get_Channel_List_0073d8f0();
        if(!result) return result;
    }
    for(int i=0;i<18;++i) ResetEvent(wol_wait_events_b264f8[i]);
    if(!Request_Channel_List_0073d760(0x21,autoping)) {
        SetEvent(wol_wait_events_b264f8[0]);
        return false;
    }
    if(wait) return Get_Channel_List_0073d8f0();
    return true;
}


namespace ATL {
template<> unsigned long __stdcall CComObject<CChatEventSink>::Release()
{
    unsigned long count = InterlockedDecrement(&m_dwRef);
    if (count == 0) delete this;
    return count;
}
}


namespace ATL {
extern InterfaceMapEntry const CChatEventSinkInterfaceMap[]; // @ref ?CChatEventSinkInterfaceMap@ATL@@=007af760
template<> HRESULT __stdcall CComObject<CChatEventSink>::QueryInterface(GUID const &iid, void **object)
{
    return InternalQueryInterface(this, CChatEventSinkInterfaceMap, iid, object);
}
}


namespace ATL {
template<> unsigned long __stdcall CComObject<CNetUtilEventSink>::Release()
{
    unsigned long count = InterlockedDecrement(&m_dwRef);
    if (count == 0) delete this;
    return count;
}
}


namespace ATL {
extern InterfaceMapEntry const CNetUtilEventSinkInterfaceMap[]; // @ref ?CNetUtilEventSinkInterfaceMap@ATL@@=007af720
template<> HRESULT __stdcall CComObject<CNetUtilEventSink>::QueryInterface(GUID const &iid, void **object)
{
    return InternalQueryInterface(this, CNetUtilEventSinkInterfaceMap, iid, object);
}
}


extern "C" void __cdecl release_765e00(void*);
class Cleanup_765e00 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_765e00::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_765e00(pointer); items=0; } owns=0; count=0; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<tConnInfoStruct *>::Fetch (0x00765E50; VectorClass<tConnInfoStruct *> vtable 0x007af964 slot 6).
// VectorClass<tConnInfoStruct *>::operator equal (0x00766520; VectorClass<tConnInfoStruct *> vtable 0x007af964 slot 1).
// VectorClass<tConnInfoStruct *>::Resize (0x00766560; VectorClass<tConnInfoStruct *> vtable 0x007af964 slot 2).
// VectorClass<tConnInfoStruct *>::Clear (0x00766610; VectorClass<tConnInfoStruct *> vtable 0x007af964 slot 3).
// VectorClass<tConnInfoStruct *>::ID_object (0x00766640; VectorClass<tConnInfoStruct *> vtable 0x007af964 slot 4).
// VectorClass<tConnInfoStruct *>::ID_pointer (0x00766680; VectorClass<tConnInfoStruct *> vtable 0x007af964 slot 5).


extern "C" void __cdecl release_7667a0(void*);
class Leaf_7667a0 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_7667a0::get(unsigned int flags) { cleanup(); if (flags & 1) release_7667a0(this); return this; }


extern "C" void __cdecl release_7667c0(void*);
class Leaf_7667c0 { public: void cleanup();void* get(unsigned int flags); };
void* Leaf_7667c0::get(unsigned int flags) { cleanup(); if (flags & 1) release_7667c0(this); return this; }


// void Reset_WOL_Globals() (0x0073E8D0): puts every Westwood Online global back to its start state (wonline.cpp,
// the lobby names are lines 2485-2492). Called by Initialize_Wonline 0x00742390.
// Clear_Buddy_Lists groups the two string lists: both Clear calls stay out of line (0x007661C0) one level down.
// The Ladder and Channel node deletes are refused here and emit their ??_G bodies 0x00766780 and 0x007667E0.
extern rc_ptr<PlayerProfile> unknown_profile_ptr_b28cc0;
extern char wol_source_file_007fd62c[];
extern char wol_lobby_label_1_007fd7d0[],wol_lobby_label_2_007fd7c4[],wol_lobby_label_3_007fd7b8[],wol_lobby_label_4_007fd7ac[];
extern char wol_lobby_label_5_007fd7a0[],wol_lobby_label_6_007fd794[],wol_lobby_label_7_007fd788[],wol_lobby_label_8_007fd77c[];
extern HANDLE wol_wait_events_b264f8[18];
extern long unknown_b28ff4;
extern long unknown_b28ff8;
extern unsigned int unknown_b2a7f0;
extern unsigned int unknown_b26564;
extern int unknown_b26540[8];
extern int ladder_top_rung_b29ce8;
extern int wol_lan_leave_b2abf0;
extern int unknown_7fd300;
extern int unknown_time_b2abd0, unknown_time_b2abd4;
extern int unknown_b2ac2c,unknown_b2ab88;
extern long unknown_b2a8a4;
extern unsigned long wol_chat_cookie_b2ac60,wol_net_cookie_b2ac64,wol_download_cookie_b2ac68;
extern "C" long __cdecl time(long *);
inline void Clear_Buddy_Lists()
{
    unknown_strings_b26920.Clear();
    unknown_strings_b29838.Clear();
}
template<class T> inline void Zero_Record(T &record) { memset(&record, 0, sizeof(record)); }
void __fastcall Reset_WOL_Globals_0073e8d0()
{
    unknown_profile_ptr_b28cc0 = PlayerProfile::Load_Default();
    unknown_lobbies_b26c88.setEmpty();
    unknown_games_b29990.setEmpty();
    unknown_user_list_count_b2a728 = 0;
    unknown_game_names_b2a648.Clear();
    unknown_known_names_b294b8.Clear();
    unknown_pending_names_b29468.Clear();
    unknown_channel_names_b293a0.Clear();
    unknown_b292dc = time(0) + 86400;
    unknown_b28ff4 = 0;
    unknown_b28ff8 = 0;
    unknown_ladder_b296f8.Clear();
    unknown_ladder_b26970.Clear();
    unknown_channels_b29478.Clear_00766400();
    Clear_Buddy_Lists();
    unknown_names_b29848.Clear_007662f0();
    if (unknown_game_mode_a3d298 == 4 && unknown_a3d2a4 == 2) {
        unknown_b2a7f0 = 0x2300;
        unknown_b26564 = 0x2300;
        unknown_ladder_cache_b2ac24 = &unknown_ladder_b26970;
    } else {
        unknown_b2a7f0 = unknown_protocol_7fd370;
        unknown_b26564 = unknown_protocol_7fd370;
        unknown_ladder_cache_b2ac24 = &unknown_ladder_b2ab98;
    }
    Zero_Record(unknown_channel_b299c0);
    Zero_Record(unknown_pending_channel_b26a60);
    Zero_Record(unknown_game_server_b28cd0);
    Zero_Record(unknown_wdt_server_b29880);
    unknown_connection_error_b2a8ec[0] = 0;
    unknown_b2ac2c = 0;
    unknown_b26898 = 1;
    unknown_b2ab88 = -1;
    unknown_b2a8a4 = time(0);
    unknown_irc_index_b292d8 = 0;
    unknown_irc_index_b2a7f4 = 0;
    unknown_b2ac58 = 0;
    unknown_b2abe0 = 0;
    unknown_b2ac04 = false;
    unknown_b2abd8 = false;
    unknown_7fd2fc = true;
    unknown_motd_b26954.Release();
    memset(wol_wait_events_b264f8, 0, sizeof(wol_wait_events_b264f8));
    unknown_b2abd9 = false;
    unknown_b2abda = false;
    unknown_7fd300 = -1;
    unknown_login_state_00b2abe4 = 0;
    unknown_time_b2abd0 = -1;
    unknown_time_b2abd4 = -1;
    unknown_lobby_names_b26c94[0] = (char *)StringTable::Load_String(wol_lobby_label_1_007fd7d0, 0, wol_source_file_007fd62c, 0x9b5);
    unknown_lobby_names_b26c94[1] = (char *)StringTable::Load_String(wol_lobby_label_2_007fd7c4, 0, wol_source_file_007fd62c, 0x9b6);
    unknown_lobby_names_b26c94[2] = (char *)StringTable::Load_String(wol_lobby_label_3_007fd7b8, 0, wol_source_file_007fd62c, 0x9b7);
    unknown_lobby_names_b26c94[3] = (char *)StringTable::Load_String(wol_lobby_label_4_007fd7ac, 0, wol_source_file_007fd62c, 0x9b8);
    unknown_lobby_names_b26c94[4] = (char *)StringTable::Load_String(wol_lobby_label_5_007fd7a0, 0, wol_source_file_007fd62c, 0x9b9);
    unknown_lobby_names_b26c94[5] = (char *)StringTable::Load_String(wol_lobby_label_6_007fd794, 0, wol_source_file_007fd62c, 0x9ba);
    unknown_lobby_names_b26c94[6] = (char *)StringTable::Load_String(wol_lobby_label_7_007fd788, 0, wol_source_file_007fd62c, 0x9bb);
    unknown_lobby_names_b26c94[7] = (char *)StringTable::Load_String(wol_lobby_label_8_007fd77c, 0, wol_source_file_007fd62c, 0x9bc);
    memset(unknown_b26540, -1, sizeof(unknown_b26540));
    unknown_chat_a72cfc = 0;
    unknown_ladder_client_a72d00 = 0;
    unknown_download_a72d04 = 0;
    wol_chat_cookie_b2ac60 = 0;
    wol_net_cookie_b2ac64 = 0;
    wol_download_cookie_b2ac68 = 0;
    unknown_query_count_b2abf4 = 0;
    unknown_squad_query_b2abf8 = 0;
    memset(unknown_squad_name_b2a8b0, 0, 41);
    unknown_squad_id_b2abfc = 0;
    unknown_b2abe8 = 0;
    ladder_top_rung_b29ce8 = 1;
    wol_lan_leave_b2abf0 = 0;
    unknown_channel_requests_b29390.Set_Empty();
    unknown_b2ac40 = 0;
    unknown_b2ac6c = 0;
    unknown_b2ac70 = 0;
    unknown_b2ac18 = Unknown_006525F0();
    Read_Art_INI();
    unknown_b2ac1c = unknown_art_ini_unique_id();
    unknown_b2ac20 = unknown_ini_8397a0_unique_id();
    unknown_mangler_index_b2ac38 = 0;
}


extern "C" int __cdecl swprintf(wchar_t *,wchar_t const *,...);
extern "C" long __cdecl atol(char const *);
long __fastcall unknown_get_window_item_text(HWND,unsigned int,Wstring &);
int __fastcall unknown_clear_listbox(HWND);
extern char lobby_prefix_007fd358[];
extern wchar_t lobby_first_page_format_007fd7f4[],lobby_later_page_format_007fd7dc[];
extern unsigned int lobby_title_column_b29acc;
void __fastcall Refresh_Lobby_List_0073ee80()
{
    HWND dialog=(HWND)WS_Find_Dialog_00735940(0x113);
    if(!dialog) return;
    HWND list=GetDlgItem(dialog,0x53d);
    if(!list) return;
    Wstring selected;
    int selected_row=SendMessageA(list,0x18e,0,0);
    if(selected_row!=-1) unknown_get_window_item_text(list,selected_row,selected);
    SendMessageA(list,0x49d,0,1);
    unknown_clear_listbox(list);
    int count=unknown_lobbies_b26c88.Count;
    for(int i=0;i<count;++i) {
        WOL::Channel channel;
        unknown_lobbies_b26c88.Get(channel,i);
        if(strncmp(channel.name,lobby_prefix_007fd358,strlen(lobby_prefix_007fd358))!=0) continue;
        int number=atol(channel.name+strlen(lobby_prefix_007fd358));
        bool current=strcmp(channel.name,unknown_game_channel_b299c0.name)==0;
        wchar_t title[80];
        int page=number/8;
        int name_index=number%8;
        if(page==0) swprintf(title,lobby_first_page_format_007fd7f4,(wchar_t const *)unknown_lobby_names_b26c94[name_index],channel.currentUsers);
        else swprintf(title,lobby_later_page_format_007fd7dc,(wchar_t const *)unknown_lobby_names_b26c94[name_index],page+1,channel.currentUsers);
        int rows=SendMessageA(list,0x18b,0,0);
        int at=0;
        WOL::Channel other;
        for(;at<rows;++at) {
            int index=SendMessageA(list,0x199,at,0);
            unknown_lobbies_b26c88.Get(other,index);
            if(atol(other.name+strlen(lobby_prefix_007fd358))>number) break;
        }
        int row=SendMessageA(list,0x4c7,at,(LPARAM)channel.name);
        SendMessageA(list,0x19a,row,i);
        WolLadderListCell cell;
        cell.Flags=1;
        cell.Text.Set(title);
        cell.Secondary.Set(L"");
        SendMessageA(list,0x4a8,(unsigned short)lobby_title_column_b29acc|((unsigned long)(unsigned short)row<<16),(LPARAM)&cell);
        if(current) SendMessageA(list,0x186,row,0);
    }
    int restored=SendMessageA(list,0x4c5,0,(LPARAM)selected.Peek_Buffer());
    if(restored!=-1) SendMessageA(list,0x197,restored,0);
    SendMessageA(list,0x49d,0,0);
    InvalidateRect(list,0,false);
}


extern char wol_locale_label_prefix_7fd808[];
extern char wol_source_file_007fd62c[];
WideString __fastcall Locale_Name_0073f1c0(int locale)
{
    Wstring label=wol_locale_label_prefix_7fd808+Wstring_From_Integer(locale,2);
    WideString text(StringTable::Load_String(label.Get(),0,wol_source_file_007fd62c,0xa54));
    // The original tests this bound before copying the same localized value on either path.
    if(locale<37) return text;
    else return text;
}


WideString __fastcall operator+(unsigned short const *left,WideString const &right);
WideString __fastcall Locale_Name_0073f1c0(int locale);
extern unsigned short wol_user_separator_7fd838[];
extern unsigned short wol_profile_separator_7fd820[];
extern unsigned short wol_rank_space_7d64f4[];
extern char wol_rank_label_7fd814[];
extern char wol_unranked_label_7fd828[];
extern char wol_source_file_007fd62c[];
WideString __fastcall User_Tooltip_0073f250(WOL::User const &user)
{
    Wstring name(user.name);
    WideString text=Wstring_To_Wide(name)+wol_user_separator_7fd838;
    name.To_Lower();
    WOL::Ladder ladder;
    memset(&ladder,0,sizeof(ladder));
    if(unknown_ladder_cache_b2ac24 && unknown_ladder_cache_b2ac24->Get_Copy(ladder,name)) {
        text+=WideString(StringTable::Load_String(wol_rank_label_7fd814,0,wol_source_file_007fd62c,0xa70))+WideString(wol_rank_space_7d64f4)+unknown_number_text_7710c0(ladder.unknown_1c);
    } else {
        text+=StringTable::Load_String(wol_unranked_label_7fd828,0,wol_source_file_007fd62c,0xa72);
    }
    Wstring profile(user.unknown_2e);
    if(profile.Length()==0) profile=user.unknown_57;
    if(profile.Length()>0) text+=wol_profile_separator_7fd820+Wstring_To_Wide(profile);
    if(unknown_channel_names_b293a0.Contains(name)) {
        int *locale=0;
        if(unknown_chat_a72cfc && unknown_channel_names_b293a0.Get_Pointer(name,&locale) && locale && *locale) {
            text+=wol_user_separator_7fd838+Locale_Name_0073f1c0(*locale);
        }
    }
    return text;
}


// WideString __fastcall Unknown_0073f550(Wstring const &filename) (0x0073F550)
// The description of the multiplayer map with this file name: the random-map text for RandMap.Sed,
// empty when the map is not in the session's list.

WideString __fastcall Unknown_0073f550(Wstring const &filename)
{
    if (_stricmp(filename.Peek_Buffer(), "RandMap.Sed") == 0) {
        return WideString(StringTable::Load_String("TXT_RANDOM_MAP_DESCRIPTION", 0, "D:\\Ra2\\wonline.cpp", 2705));
    }
    for (int index = 0; index < unknown_multi_missions_a3d8d8.Count(); index++) {
        MultiMission *mission = unknown_multi_missions_a3d8d8[index];
        if (_stricmp(filename.Peek_Buffer(), mission->unknown_58) == 0) {
            return WideString(mission->Description);
        }
    }
    return WideString(L"");
}


WideString __fastcall User_Tooltip_0073f250(WOL::User const &user);
WideString __fastcall Unknown_0073f550(Wstring const &filename);
void __fastcall Parse_Channel_Info_0073d070(char *,int *,int *,int *,int *,int *,Wstring *);
extern "C" int __cdecl swprintf(wchar_t *,wchar_t const *,...);
extern wchar_t tooltip_separator_007f6520[];
extern char game_tooltip_empty_label_007ea8d0[],game_ping_label_007fd854[],unknown_ping_label_007fd840[];
extern char wol_source_file_007fd62c[];
WideString __fastcall Game_List_Row_Text_0073f5f0(HWND list,int row)
{
    if(row==-1) return WideString(StringTable::Load_String(game_tooltip_empty_label_007ea8d0,0,wol_source_file_007fd62c,0xaa2));
    WOL::Channel channel;
    memset(&channel,0,sizeof(channel));
    int index=SendMessageA(list,0x199,row,0);
    if(index>=0 && index<unknown_games_b29990.Count) {
        channel=unknown_games_b29990.Items[index];
        Wstring host(channel.name);
        host.Truncate_At(char(0x27));
        WOL::User user;
        memset(&user,0,sizeof(user));
        strncpy(user.name,host.Get(),10);
        WideString tooltip=User_Tooltip_0073f250(user);
        tooltip+=WideString(tooltip_separator_007f6520);
        wchar_t ping_text[64];
        if(channel.ping!=-1) {
            swprintf(ping_text,StringTable::Load_String(game_ping_label_007fd854,0,wol_source_file_007fd62c,0xac4),channel.ping);
            tooltip+=ping_text;
        } else tooltip+=StringTable::Load_String(unknown_ping_label_007fd840,0,wol_source_file_007fd62c,0xac7);
        char options[132];
        strncpy(options,channel.exInfo,128);
        int version=0,checksum=0,reserved=0,observers=0,allow_observers=0;
        Wstring filename;
        Parse_Channel_Info_0073d070(options,&version,&checksum,&reserved,&observers,&allow_observers,&filename);
        if(filename.Length()>0) {
            WideString description=Unknown_0073f550(filename);
            if(description.Length()>0) {
                tooltip+=tooltip_separator_007f6520;
                tooltip+=description;
            }
        }
        return tooltip;
    }
    return WideString(StringTable::Load_String(game_tooltip_empty_label_007ea8d0,0,wol_source_file_007fd62c,0xaa9));
}


WideString __fastcall User_List_Description_0073f250(WOL::User const &user);
extern char user_list_empty_label_007ea8e4[];
extern char wol_source_file_007fd62c[];
WideString __fastcall User_List_Row_Text_0073f850(HWND list,int row)
{
    if(row==-1) return WideString(StringTable::Load_String(user_list_empty_label_007ea8e4,0,wol_source_file_007fd62c,0xae1));
    int index=SendMessageA(list,0x199,row,0);
    if(index>=0 && index<unknown_users_b2a728.Count) {
        WOL::User user;
        memset(&user,0,sizeof(user));
        user=unknown_users_b2a728.Items[index];
        return User_List_Description_0073f250(user);
    }
    return WideString(StringTable::Load_String(user_list_empty_label_007ea8e4,0,wol_source_file_007fd62c,0xae6));
}


extern "C" int __cdecl swprintf(wchar_t *,wchar_t const *,...);
long __fastcall unknown_get_window_item_text(HWND,unsigned int,Wstring &);
void __fastcall Parse_Channel_Info_0073d070(char *,int *,int *,int *,int *,int *,Wstring *);
extern bool game_list_refreshing_b2ac80;
extern unsigned int game_name_column_b2a8a8,game_type_column_b2a6f8,game_private_column_b2a6f4,game_observer_column_b299a0,game_ping_column_b292a8;
extern int game_unavailable_color_007a7380;
extern wchar_t game_counts_format_007fd8b8[],game_unknown_ping_text_007fd874[];
extern char game_clan_image_007eb228[],game_tournament_image_007eb25c[],game_normal_image_007eb1fc[],game_private_image_007eb234[];
extern char game_locked_observer_image_007eb218[],game_locked_image_007eafb4[],game_observer_image_007e7444[];
extern char game_clan_label_007fd8a4[],game_tournament_label_007fd890[],game_password_label_007fd87c[],game_ping_label_007fd864[],wol_source_file_007fd62c[];
void __fastcall Refresh_Game_List_0073f8f0()
{
    HWND dialog=(HWND)WS_Find_Dialog_00735940(0x113);
    if(!dialog) return;
    HWND list=GetDlgItem(dialog,0x6ee);
    if(!list) return;
    Wstring previous_selection;
    int selected=SendMessageA(list,0x188,0,0);
    if(selected!=-1) unknown_get_window_item_text(list,selected,previous_selection);
    Wstring previous_top;
    int top=SendMessageA(list,0x18e,0,0);
    if(top!=-1) unknown_get_window_item_text(list,top,previous_top);
    SendMessageA(list,0x49d,0,1);
    game_list_refreshing_b2ac80=true;
    SendMessageA(list,0x184,0,0);
    int count=unknown_games_b29990.Count;
    for(int i=0;i<count;++i) {
        WOL::Channel channel;
        memset(&channel,0,sizeof(channel));
        unknown_games_b29990.Get(channel,i);
        if(Unknown_0073c3e0(&channel) || Unknown_0073c400(&channel)) continue;
        char options[132];
        strncpy(options,channel.exInfo,128);
        int version=0,checksum=0,extra_players=0,observers=0,allow_observers=0;
        Wstring filename;
        Parse_Channel_Info_0073d070(options,&version,&checksum,&extra_players,&observers,&allow_observers,&filename);
        int participants=channel.currentUsers-observers+extra_players;
        int capacity=channel.maxUsers-((observers || allow_observers) ? 1 : 0);
        wchar_t title[80];
        swprintf(title,game_counts_format_007fd8b8,channel.name,participants,capacity);
        int row=SendMessageA(list,0x4c7,-1,(LPARAM)channel.name);
        SendMessageA(list,0x19a,row,i);
        WolLadderListCell cell;
        cell.Flags=1;
        cell.Text.Set(title);
        cell.Secondary.Set(L"");
        cell.unknown_0c=unknown_color_7a7360;
        if(unknown_a72c68) {
            if(!allow_observers || observers) cell.unknown_0c=game_unavailable_color_007a7380;
        } else if(participants==capacity) cell.unknown_0c=game_unavailable_color_007a7380;
        SendMessageA(list,0x4a8,((unsigned long)(unsigned short)game_name_column_b2a8a8|((unsigned long)(unsigned short)row<<16)),(LPARAM)&cell);
        cell.Flags=0;
        cell.Secondary.Set(L"");
        cell.Flags=2;
        cell.Secondary.Set(L"");
        if(Unknown_0073c3c0(&channel)) {
            cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(game_clan_image_007eb228,0);
            cell.Secondary.Set(StringTable::Load_String(game_clan_label_007fd8a4,0,wol_source_file_007fd62c,0xb57));
        } else if(Unknown_0073c340(&channel)) {
            cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(game_tournament_image_007eb25c,0);
            cell.Secondary.Set(StringTable::Load_String(game_tournament_label_007fd890,0,wol_source_file_007fd62c,0xb5c));
        } else {
            cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(game_normal_image_007eb1fc,0);
            cell.Secondary.Set(L"");
        }
        SendMessageA(list,0x4a8,((unsigned long)(unsigned short)game_type_column_b2a6f8|((unsigned long)(unsigned short)row<<16)),(LPARAM)&cell);
        cell.Flags=2;
        cell.Secondary.Set(L"");
        if(channel.flags&0x100) {
            cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(game_private_image_007eb234,0);
            cell.Secondary.Set(StringTable::Load_String(game_password_label_007fd87c,0,wol_source_file_007fd62c,0xb6a));
        } else {
            cell.Flags=0;
            cell.Secondary.Set(L"");
        }
        SendMessageA(list,0x4a8,((unsigned long)(unsigned short)game_private_column_b2a6f4|((unsigned long)(unsigned short)row<<16)),(LPARAM)&cell);
        cell.Flags=0;
        cell.Text.Set(L"");
        cell.Secondary.Set(L"");
        if(channel.unknown_20&0x600) {
            if(!observers && allow_observers) {
                cell.Flags=2;
                cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(game_locked_observer_image_007eb218,0);
            } else {
                cell.Flags=2;
                cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(game_locked_image_007eafb4,0);
            }
        } else if(!observers && allow_observers) {
            cell.Flags=2;
            cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(game_observer_image_007e7444,0);
        }
        SendMessageA(list,0x4a8,((unsigned long)(unsigned short)game_observer_column_b299a0|((unsigned long)(unsigned short)row<<16)),(LPARAM)&cell);
        if(channel.ping==-1) {
            cell.Flags=1;
            cell.Text.Set(game_unknown_ping_text_007fd874);
        } else {
            cell.unknown_14=channel.ping;
            cell.Flags=3;
        }
        swprintf(title,StringTable::Load_String(game_ping_label_007fd864,0,wol_source_file_007fd62c,0xb92),channel.ping);
        cell.Secondary.Set(title);
        SendMessageA(list,0x4a8,((unsigned long)(unsigned short)game_ping_column_b292a8|((unsigned long)(unsigned short)row<<16)),(LPARAM)&cell);
    }
    int restored=SendMessageA(list,0x4c5,0,(LPARAM)previous_selection.Peek_Buffer());
    SendMessageA(list,0x186,restored,0);
    int restored_top=SendMessageA(list,0x4c5,0,(LPARAM)previous_top.Peek_Buffer());
    if(restored_top!=-1) SendMessageA(list,0x197,restored_top,0);
    SendMessageA(list,0x49d,0,0);
    InvalidateRect(list,0,false);
    if(dialog==Unknown_00735B10()) SetFocus(GetDlgItem(dialog,0x540));
}


// 0073FEE0: complete Wonline player-list refresh, independent original bounds5132+4NOP.
void __fastcall Unknown_005c5ff0(HWND list,LobbyNameSet *selection);
void __fastcall Restore_List_Selection_005c6310(HWND list,LobbyNameSet *selection);
WideString __fastcall Locale_Name_0073f1c0(int locale);
extern "C" int __cdecl tolower(int character);
extern int player_list_colors_007e6d94[];
extern wchar_t player_list_locale_00b29738[64];
extern wchar_t player_list_w_00839d4c[];
extern int player_list_column_00b2a81c;
extern int player_list_column_00b292a4;
extern int player_list_column_00b2657c;
extern int player_list_column_00b2ab80;
extern int player_list_column_00b292e0;
extern int player_list_column_00b297b8;
extern int player_list_column_00b26958;

void __fastcall Unknown_0073FEE0(int keep_selection,bool full)
{
    HWND lobby=Find_Lobby_Window_005c2c40();
    if(lobby) { Refresh_Slot_Settings_005ca000(lobby,full); return; }
    if(WS_Find_Dialog_00735940(0x6b)) return;
    HWND window=(HWND)WS_Find_Dialog_00735940(0x116);
    if(window) Unknown_00755180(window);
    else window=(HWND)WS_Find_Dialog_00735940(0x113);
    HWND list=GetDlgItem(window,0x53e);
    if(!window || !list) return;
    LobbyNameSet selections(Hash_Wide_Name_005c2bf0);
    Unknown_005c5ff0(list,&selections);
    int top=SendDlgItemMessageA(window,0x53e,0x18e,0,0);
    SendDlgItemMessageA(window,0x53e,0x49d,0,1);
    SendDlgItemMessageA(window,0x53e,0x184,0,0);
    Wstring name;
    WOL::Ladder ladder;
    memset(&ladder,0,sizeof(ladder));
    WolLadderListCell cell;
    Wstring local_name=unknown_wstring_b26cb8.As_Lower();
    wchar_t info[80],rank[80];
    char count_text[20];
    for(int i=0;i<unknown_users_b2a728.Count;++i) {
        WOL::User user;
        unknown_users_b2a728.Get_Copy(user,i);
        Wstring user_name(user.name);
        if(local_name==user_name.As_Lower()) {
            HWND control=GetDlgItem(window,0x555);
            if(control) {
                LONG style=GetWindowLongA(control,-16);
                if(user.flags & 1) style &= ~0x08000000;
                else style |= 0x08000000;
                SetWindowLongA(control,-16,style);
                InvalidateRect(control,0,0);
            }
            control=GetDlgItem(window,0x587);
            if(control) {
                LONG style=GetWindowLongA(control,-16);
                if(user.flags & 1) style &= ~0x08000000;
                else style |= 0x08000000;
                SetWindowLongA(control,-16,style);
                InvalidateRect(control,0,0);
            }
        }
        if(!unknown_b299c0) {
            swprintf(info,L"%hs",user.name);
            int row=SendDlgItemMessageA(window,0x53e,0x4cc,-1,(LPARAM)info);
            cell.Flags=1;
            cell.Secondary.Set(StringTable::Load_String("TXT_UNRANKED",0,"D:\\Ra2\\wonline.cpp",0xc18));
            Char_To_Wide_006fa0c0(info,user.name,-1);
            cell.Text.Set(info);
            if(unknown_game_mode_a3d298==4 && unknown_a3d2a4==2) {
                if(!(user.flags & 1)) {
                    Wstring team_name=Narrow_Copy_00771280(cell.Text.Get());
                    if(unknown_known_names_b294b8.Contains(team_name)) {
                        int color;
                        int *value=0;
                        if(unknown_known_names_b294b8.Get_Pointer(team_name,&value) && value) color=*value;
                        WideString converted_team(Narrow_To_Wide_006fa180(team_name.Get()));
                        cell.Text=converted_team;
                        cell.unknown_0c=player_list_colors_007e6d94[color];
                    } else cell.unknown_0c=unknown_color_7a7360;
                } else cell.unknown_0c=unknown_color_7a7360;
            } else cell.unknown_0c=unknown_color_7a7360;
            name.Set(user.name);
            name.To_Lower();
            memset(player_list_locale_00b29738,0,sizeof(player_list_locale_00b29738));
            int locale=0;
            if(unknown_channel_names_b293a0.Contains(name) && unknown_chat_a72cfc) {
                int *value=0;
                if(unknown_channel_names_b293a0.Get_Pointer(name,&value) && value) locale=*value;
                if(locale) {
                    WideString locale_name=Locale_Name_0073f1c0(locale);
                    swprintf(player_list_locale_00b29738,L"(%hs)",locale_name.Get());
                }
            }
            if(unknown_ladder_cache_b2ac24->Get_Copy(ladder,name)) {
                swprintf(rank,L"%s %d",StringTable::Load_String("TXT_RANK",0,"D:\\Ra2\\wonline.cpp",0xc5a),ladder.unknown_1c);
                cell.Secondary.Set(rank);
            }
            if(unknown_channel_names_b293a0.Contains(name)) {
                if(locale) cell.Secondary.Insert(L" ",0);
                cell.Secondary.Insert(player_list_locale_00b29738,0);
            }
            SendMessageA(GetDlgItem(window,0x53e),0x19a,row,i);
            SendDlgItemMessageA(window,0x53e,0x4a8,((unsigned short)player_list_column_00b2a81c | ((unsigned int)(unsigned short)i << 16)),(LPARAM)&cell);
            cell.Flags=1;
            name.Set(user.name);
            name.To_Lower();
            rank[0]=0;
            if(strlen(user.unknown_57)) {
                swprintf(rank,L"[%hs]",user.unknown_57);
                wchar_t squad_name[200];
                Char_To_Wide_006fa0c0(squad_name,user.unknown_2e,-1);
                cell.Secondary.Set(squad_name);
            } else if(unknown_ladder_cache_b2ac24->Get_Copy(ladder,name)) {
                swprintf(rank,L"(%s %d)",StringTable::Load_String("TXT_RANK",0,"D:\\Ra2\\wonline.cpp",0xc84),ladder.unknown_1c);
                cell.Secondary.Set(player_list_w_00839d4c);
            }
            cell.Text.Set(rank);
            SendDlgItemMessageA(window,0x53e,0x4a8,((unsigned short)player_list_column_00b292a4 | ((unsigned int)(unsigned short)i << 16)),(LPARAM)&cell);
            cell.Flags=1;
            if(user.flags & 1) {
                cell.Flags=2;
                cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("woloper.pcx",0);
                cell.Secondary.Set(StringTable::Load_String("TXT_OPER",0,"D:\\Ra2\\wonline.cpp",0xc90));
            } else if(user.flags & 2) {
                cell.Flags=2;
                cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("wolvoice.pcx",0);
                cell.Secondary.Set(StringTable::Load_String("TXT_VOICE",0,"D:\\Ra2\\wonline.cpp",0xc96));
            } else {
                cell.Text.Set(player_list_w_00839d4c);
                cell.Secondary.Set(player_list_w_00839d4c);
            }
            SendDlgItemMessageA(window,0x53e,0x4a8,((unsigned short)player_list_column_00b2657c | ((unsigned int)(unsigned short)i << 16)),(LPARAM)&cell);
        } else {
            if(HouseTypes.Count()<=1) return;
            int index;
            { Wstring lookup_name(user.name); index=unknown_users_b2a728.Find_Name(lookup_name); }
            int country=0,star=0;
            if(index!=-1) {
                country=unknown_player_b29cf0[index].unknown_18;
                if(unknown_player_b29cf0[index].unknown_20) star=1;
                if(index==0) { unknown_player_b29cf0[index].unknown_20=1;star=2; }
            }
            country=country>0 ? country : 0;
            name.Set(user.name);
            name.To_Lower();
            swprintf(info,L"%hs",user.name);
            for(int j=0;j<(int)wcslen(info);++j) info[j]=(wchar_t)tolower(info[j]);
            SendDlgItemMessageA(window,0x53e,0x4cc,-1,(LPARAM)info);
            cell.Flags=4;
            cell.unknown_0c=player_list_colors_007e6d94[unknown_player_b29cf0[index].unknown_14];
            cell.Secondary.Set(StringTable::Load_String("TXT_UNRANKED",0,"D:\\Ra2\\wonline.cpp",0xce1));
            if(unknown_ladder_cache_b2ac24->Get_Copy(ladder,name)) {
                swprintf(rank,L"%s %d",StringTable::Load_String("TXT_RANK",0,"D:\\Ra2\\wonline.cpp",0xce4),ladder.unknown_1c);
                cell.Secondary.Set(rank);
            }
            SendDlgItemMessageA(window,0x53e,0x4a8,((unsigned short)player_list_column_00b2a81c | ((unsigned int)(unsigned short)i << 16)),(LPARAM)&cell);
            cell.Flags=1;
            if(strlen(user.unknown_57)) {
                Char_To_Wide_006fa0c0(info,user.unknown_2e,-1);
                cell.Secondary.Set(info);
                Char_To_Wide_006fa0c0(info,user.unknown_57,-1);
            } else {
                if(unknown_ladder_cache_b2ac24->Get_Copy(ladder,name)) {
                    swprintf(info,L"(%s %d)",StringTable::Load_String("TXT_RANK",0,"D:\\Ra2\\wonline.cpp",0xcf7),ladder.unknown_1c);
                    cell.Secondary.Set(player_list_w_00839d4c);
                } else {
                    swprintf(info,player_list_w_00839d4c);
                    cell.Secondary.Set(player_list_w_00839d4c);
                }
            }
            cell.Text.Set(info);
            SendDlgItemMessageA(window,0x53e,0x4a8,((unsigned short)player_list_column_00b2ab80 | ((unsigned int)(unsigned short)i << 16)),(LPARAM)&cell);
            cell.Flags=2;
            switch(country) {
            case 0:
                cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("usai.pcx",0);
                swprintf(info,L"%s",StringTable::Load_String("TXT_GDI",0,"D:\\Ra2\\wonline.cpp",0xd1b));
                break;
            case 1:
                cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("japi.pcx",0);
                swprintf(info,L"%s",StringTable::Load_String("TXT_GDI",0,"D:\\Ra2\\wonline.cpp",0xd1f));
                break;
            case 2:
                cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("frai.pcx",0);
                swprintf(info,L"%s",StringTable::Load_String("TXT_GDI",0,"D:\\Ra2\\wonline.cpp",0xd23));
                break;
            case 3:
                cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("geri.pcx",0);
                swprintf(info,L"%s",StringTable::Load_String("TXT_GDI",0,"D:\\Ra2\\wonline.cpp",0xd27));
                break;
            case 4:
                cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("gbri.pcx",0);
                swprintf(info,L"%s",StringTable::Load_String("TXT_GDI",0,"D:\\Ra2\\wonline.cpp",0xd2b));
                break;
            case 5:
                cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("djbi.pcx",0);
                swprintf(info,L"%s",StringTable::Load_String("TXT_NOD",0,"D:\\Ra2\\wonline.cpp",0xd2f));
                break;
            case 6:
                cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("arbi.pcx",0);
                swprintf(info,L"%s",StringTable::Load_String("TXT_NOD",0,"D:\\Ra2\\wonline.cpp",0xd33));
                break;
            case 7:
                cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("lati.pcx",0);
                swprintf(info,L"%s",StringTable::Load_String("TXT_NOD",0,"D:\\Ra2\\wonline.cpp",0xd37));
                break;
            case 8:
                cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("rusi.pcx",0);
                swprintf(info,L"%s",StringTable::Load_String("TXT_NOD",0,"D:\\Ra2\\wonline.cpp",0xd3b));
                break;
            }
            cell.Secondary.Set(info);
            SendDlgItemMessageA(window,0x53e,0x4a8,((unsigned short)player_list_column_00b292e0 | ((unsigned int)(unsigned short)i << 16)),(LPARAM)&cell);
            cell.Flags=2;
            if(star) {
                if(star==2) cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("wolhost.pcx",0);
                else cell.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40("wolacpt.pcx",0);
                cell.Secondary.Set(StringTable::Load_String("TXT_ACCEPTED",0,"D:\\Ra2\\wonline.cpp",0xd49));
            } else {
                cell.Flags=0;
                cell.Secondary.Set(player_list_w_00839d4c);
            }
            SendDlgItemMessageA(window,0x53e,0x4a8,((unsigned short)player_list_column_00b297b8 | ((unsigned int)(unsigned short)i << 16)),(LPARAM)&cell);
            int ping=0;
            unknown_ladder_client_a72d00->Get_Average_Ping(user.unknown_18,&ping);
            if(ping<0) ping=0;
            cell.Flags=3;
            cell.unknown_14=ping;
            wchar_t ping_text[200];
            swprintf(ping_text,StringTable::Load_String("MSG:PingInfo",0,"D:\\Ra2\\wonline.cpp",0xd59),ping);
            if(ping==0) swprintf(ping_text,StringTable::Load_String("TXT_UNKNOWN_PING",0,"D:\\Ra2\\wonline.cpp",0xd5b));
            cell.Secondary.Set(ping_text);
            Wstring ping_name(user.name);
            if(local_name==ping_name.As_Lower()) cell.Flags=0;
            SendDlgItemMessageA(window,0x53e,0x4a8,((unsigned short)player_list_column_00b26958 | ((unsigned int)(unsigned short)i << 16)),(LPARAM)&cell);
        }
    }
    if(!keep_selection) Restore_List_Selection_005c6310(list,&selections);
    int selected=SendDlgItemMessageA(window,0x53e,0x190,0,0);
    int total=unknown_users_b2a728.Count;
    if(selected>0) sprintf(count_text,"%d  (%d)",total,selected);
    else sprintf(count_text,"%d",total);
    SendDlgItemMessageA(window,0x60b,0x4b4,0,(LPARAM)count_text);
    SendDlgItemMessageA(window,0x53e,0x197,top,0);
    SendDlgItemMessageA(window,0x53e,0x49d,0,0);
    InvalidateRect(GetDlgItem(window,0x53e),0,0);
}


// int __fastcall Startup_Chat_00741300(HINSTANCE instance) (0x00741300)
// Creates the Westwood Online Chat, NetUtil and Download objects, connects an event sink to each,
// checks the installed API version and gives Chat the registry path and product SKU. Returns 1, or
// -2 after a message box when an object is missing or too old. The Tiberian Sun twin is Startup_Chat.
extern unsigned long wol_chat_cookie_b2ac60,wol_net_cookie_b2ac64,wol_download_cookie_b2ac68;
extern GUID const wol_chat_event_iid_007af5b0,wol_net_event_iid_007af5f0,wol_download_event_iid_007af5d0;
extern GUID const wol_chat_clsid_007af640,wol_chat_iid_007af5a0,wol_net_clsid_007af660,wol_net_iid_007af5e0,wol_download_clsid_007af650,wol_download_iid_007af5c0;
extern ATL::CComObject<CChatEventSink> *wol_chat_sink_b2a720;
extern ATL::CComObject<CNetUtilEventSink> *wol_net_sink_b26560;
extern ATL::CComObject<CDownloadEventSink> *wol_download_sink_b28de0;
WIN32_IMPORT LONG WINAPI RegOpenKeyExA(HKEY key,char const *sub_key,unsigned long options,unsigned long access,HKEY *result);
WIN32_IMPORT LONG WINAPI RegQueryValueExA(HKEY key,char const *name,unsigned long *reserved,unsigned long *type,unsigned char *data,unsigned long *size);
extern "C" unsigned int __cdecl strlen(char const *string);
extern "C" char *__cdecl strcpy(char *destination,char const *source);

inline void Clear_Chat_Interfaces()
{
    unknown_chat_a72cfc=0;
    unknown_ladder_client_a72d00=0;
    unknown_download_a72d04=0;
    wol_chat_cookie_b2ac60=0;
    wol_net_cookie_b2ac64=0;
    wol_download_cookie_b2ac68=0;
}

int __fastcall Startup_Chat_00741300(HINSTANCE instance)
{
    HRESULT result=0;
    _Module.Init(0,instance);
    Clear_Chat_Interfaces();

    CoCreateInstance(wol_chat_clsid_007af640,0,1,wol_chat_iid_007af5a0,(void **)&unknown_chat_a72cfc);
    if(unknown_chat_a72cfc) {
        ATL::CComObject<CChatEventSink>::CreateInstance(&wol_chat_sink_b2a720);
        result=AtlAdvise(unknown_chat_a72cfc,wol_chat_sink_b2a720->GetUnknown(),wol_chat_event_iid_007af5b0,&wol_chat_cookie_b2ac60);
    }
    if(unknown_chat_a72cfc==0 || result!=0) {
        Message_Box_005c3230(StringTable::Load_String("TXT_APIMISSING",0,"D:\\Ra2\\wonline.cpp",0x1ec9),0,0,false);
        return -2;
    }

    CoCreateInstance(wol_net_clsid_007af660,0,1,wol_net_iid_007af5e0,(void **)&unknown_ladder_client_a72d00);
    if(unknown_ladder_client_a72d00) {
        ATL::CComObject<CNetUtilEventSink>::CreateInstance(&wol_net_sink_b26560);
        result=AtlAdvise((IUnknown *)unknown_ladder_client_a72d00,wol_net_sink_b26560->GetUnknown(),wol_net_event_iid_007af5f0,&wol_net_cookie_b2ac64);
    }
    if(unknown_ladder_client_a72d00==0 || result!=0) {
        Message_Box_005c3230(StringTable::Load_String("TXT_APIMISSING",0,"D:\\Ra2\\wonline.cpp",0x1ed9),0,0,false);
        return -2;
    }

    CoCreateInstance(wol_download_clsid_007af650,0,1,wol_download_iid_007af5c0,(void **)&unknown_download_a72d04);
    if(unknown_download_a72d04) {
        ATL::CComObject<CDownloadEventSink>::CreateInstance(&wol_download_sink_b28de0);
        result=AtlAdvise(unknown_download_a72d04,wol_download_sink_b28de0->GetUnknown(),wol_download_event_iid_007af5d0,&wol_download_cookie_b2ac68);
    }
    if(unknown_download_a72d04==0 || result!=0) {
        Message_Box_005c3230(StringTable::Load_String("TXT_APIMISSING",0,"D:\\Ra2\\wonline.cpp",0x1ee9),0,0,false);
        return -2;
    }

    unsigned long version;
    unknown_chat_a72cfc->GetVersion(&version);
    if(version<0x1000e) {
        Message_Box_005c3230(StringTable::Load_String("TXT_APIWRONGVERSION",0,"D:\\Ra2\\wonline.cpp",0x1ef5),0,0,false);
        return -2;
    }

    unknown_chat_a72cfc->SetAttributeValue("RegPath",APP_REG_KEY);
    HKEY key;
    char buffer[256];
    if(APP_REG_KEY!=0 && strlen(APP_REG_KEY)) {
        strcpy(buffer,APP_REG_KEY);
        if(RegOpenKeyExA((HKEY)0x80000002,buffer,0,0x20019,&key)==0) {
            unsigned long type;
            unsigned long value;
            unsigned long size=sizeof(value);
            if(RegQueryValueExA(key,"SKU",0,&type,(unsigned char *)&value,&size)==0) unknown_chat_a72cfc->unknown_slot35(value);
            RegCloseKey(key);
        }
    }
    return 1;
}


int __fastcall Compare_Channel_Users_007416d0(WOL::User const *first,WOL::User const *second)
{
    WOL::User const *users[2]={first,second};
    int ranks[2]={0,0};
    for(int i=0;i<2;++i) {
        unsigned int flags=users[i]->flags;
        if(flags&1) ranks[i]+=4;
        if(flags&2) ranks[i]+=2;
    }
    if(ranks[0]<ranks[1]) return 1;
    if(ranks[0]>ranks[1]) return -1;
    return _stricmp(first->name,second->name);
}


extern "C" __declspec(dllimport) DWORD __stdcall WaitForMultipleObjects(DWORD,HANDLE*,BOOL,DWORD);
extern HANDLE wol_wait_events_b264f8[18];
extern char wol_logout_label_007fe478[],wol_source_file_007fd62c[];
void __fastcall Open_Wait_Window_0074d4d0(unsigned int,WideString const &,bool);
void __fastcall Logout_Wonline_00741ea0()
{
    unknown_b2abd8=true;
    if(unknown_game_mode_a3d298==4 && unknown_a3d2a4==2) unknown_known_names_b294b8.Clear();
    if(unknown_chat_a72cfc) {
        for(int i=0;i<18;++i) ResetEvent(wol_wait_events_b264f8[i]);
        Unknown_0073e100();
        if(unknown_chat_a72cfc->unknown_slot13()==0) {
            {
                WideString title(StringTable::Load_String(wol_logout_label_007fe478,0,wol_source_file_007fd62c,0x201c));
                Open_Wait_Window_0074d4d0(0x20,title,false);
            }
            for(int attempt=0;attempt<5;++attempt) {
                DWORD event=WaitForMultipleObjects(18,wol_wait_events_b264f8,0,100);
                while(event==0x102) {
                    Unknown_0073e100();
                    event=WaitForMultipleObjects(18,wol_wait_events_b264f8,0,100);
                }
                ResetEvent(wol_wait_events_b264f8[event]);
                if(event==0 || event==9) break;
            }
        }
        void *dialog=WS_Find_Dialog_00735940(0x11e);
        if(dialog) Unknown_007357E0(dialog,0);
        void *wait=WS_Find_Dialog_00735940(0xc4);
        if(wait && !(unknown_b2abe0&=~0x20)) Unknown_007357E0(wait,0);
        HWND current=Unknown_00735B10();
        HWND edit=GetDlgItem(current,0x540);
        if(edit) SetFocus(edit);
    }
}


extern bool wol_initialized_b2ac81;
extern HANDLE wol_wait_events_b264f8[18];
extern unsigned long wol_chat_cookie_b2ac60,wol_net_cookie_b2ac64,wol_download_cookie_b2ac68;
extern GUID const wol_chat_event_iid_007af5b0,wol_net_event_iid_007af5f0,wol_download_event_iid_007af5d0;
extern _ATL_MODULE wol_module_b292f8;
inline HRESULT __stdcall Atl_Unadvise_Wonline(IUnknown *source,GUID const &event_iid,unsigned long cookie)
{
    CComPtr<IConnectionPointContainer> container;
    CComPtr<IConnectionPoint> point;
    HRESULT result=source->QueryInterface(IID_IConnectionPointContainer,(void **)&container);
    if(SUCCEEDED(result)) result=container->FindConnectionPoint(event_iid,&point);
    if(SUCCEEDED(result)) result=point->Unadvise(cookie);
    return result;
}
void Shutdown_Wonline_00742080()
{
    if(unknown_dialog_preview_a72ce8) {
        delete unknown_dialog_preview_a72ce8;
        unknown_dialog_preview_a72ce8=0;
    }
    void *dialog=WS_Find_Dialog_00735940(0x11e);
    if(dialog) Unknown_007357E0(dialog,0);
    if(wol_initialized_b2ac81) {
        Unknown_Free_Lobby_Entries_005c8950();
        if(unknown_chat_a72cfc) {
            Atl_Unadvise_Wonline(unknown_chat_a72cfc,wol_chat_event_iid_007af5b0,wol_chat_cookie_b2ac60);
            unknown_chat_a72cfc->Release();
        }
        if(unknown_ladder_client_a72d00) {
            Atl_Unadvise_Wonline(unknown_ladder_client_a72d00,wol_net_event_iid_007af5f0,wol_net_cookie_b2ac64);
            unknown_ladder_client_a72d00->Release();
        }
        if(unknown_download_a72d04) {
            Atl_Unadvise_Wonline(unknown_download_a72d04,wol_download_event_iid_007af5d0,wol_download_cookie_b2ac68);
            unknown_download_a72d04->Release();
        }
        unknown_chat_a72cfc=0;
        unknown_ladder_client_a72d00=0;
        unknown_download_a72d04=0;
        for(int i=0;i<18;++i) CloseHandle(wol_wait_events_b264f8[i]);
        AtlModuleTerm(&wol_module_b292f8);
        wol_initialized_b2ac81=false;
    }
}


extern char wol_registry_key_007e6ad8[];
extern char wol_sku_name_007fdad8[];
extern char wol_wdt_debug_007fe48c[];
bool __fastcall Unknown_00742270()
{
    DWORD value=0;
    HKEY key;
    char path[256];
    char const *registry_path=wol_registry_key_007e6ad8;
    if(registry_path && strlen(registry_path)) {
        strcpy(path,registry_path);
        if(RegOpenKeyExA((HKEY)0x80000002,path,0,0x20019,&key)==0) {
            DWORD type;
            DWORD size=sizeof(value);
            RegQueryValueExA(key,wol_sku_name_007fdad8,0,&type,(BYTE *)&value,&size);
            RegCloseKey(key);
        }
    }
    unknown_b2ac5c=value & 0xff;
    if(unknown_chat_a72cfc) unknown_chat_a72cfc->unknown_slot35(value);
    if(unknown_ladder_client_a72d00) {
        Debug_Printf(wol_wdt_debug_007fe48c,unknown_wdt_address_b269ac,unknown_wdt_port_b299bc);
        unknown_ladder_client_a72d00->Request_WDT_State(unknown_wdt_address_b269ac,unknown_wdt_port_b299bc,3);
        return true;
    }
    return false;
}


extern char wol_registry_key_007e6ad8[];
extern char wol_sku_name_007fdad8[];
extern bool wol_initialized_b2ac81;
extern HANDLE wol_wait_events_b264f8[18];
void __fastcall Reset_WOL_Globals_0073e8d0();
int __fastcall Startup_Chat_00741300(HINSTANCE instance);
int __fastcall Unknown_00742390()
{
    if(wol_initialized_b2ac81) return 1;
    wol_initialized_b2ac81=true;
    Init_Lobby_Entries_005c86e0();
    Clear_Slot_Lists_005c8a70();
    Unknown_Reset_Slots_005c8a90();
    DWORD value=0;
    HKEY key;
    char path[256];
    char const *registry_path=wol_registry_key_007e6ad8;
    if(registry_path && strlen(registry_path)) {
        strcpy(path,registry_path);
        if(RegOpenKeyExA((HKEY)0x80000002,path,0,0x20019,&key)==0) {
            DWORD type;
            DWORD size=sizeof(value);
            RegQueryValueExA(key,wol_sku_name_007fdad8,0,&type,(BYTE *)&value,&size);
            RegCloseKey(key);
        }
    }
    value &= 0xff;
    unknown_b2ac5c=value;
    unknown_protocol_7fd370=value+0x2100;
    Reset_WOL_Globals_0073e8d0();
    unknown_b2abd8=true;
    for(int i=0;i<18;++i) wol_wait_events_b264f8[i]=CreateEventA(0,false,false,0);
    return Startup_Chat_00741300(ProgramInstance);
}


// Original complete2185body+7NOP, mapped before candidate. SDK Locale remains represented by its32bit ABI int.
extern "C" __declspec(dllimport) DWORD __stdcall WaitForMultipleObjects(DWORD,HANDLE*,BOOL,DWORD);
extern HANDLE wol_wait_events_b264f8[18];
extern char server_connecting_7fe4d0[],server_cant_connect_7fd740[],server_wdt_error_7fe4c0[],server_source_7fd62c[];
extern unsigned long server_game_version_7fd370;
void __fastcall Open_Wait_Window_0074d4d0(unsigned int,WideString const &,bool);
void __fastcall WOL_Set_Last_Valid_Persona(Wstring const &);
inline void __fastcall Close_Wonline_Wait(unsigned int flags);
int Open_Wonline_Dialog_FE_00751130();
int __fastcall Run_Control_Dialog_006011a0(int id,DLGPROC procedure,OwnerDraw::DialogControl *control);
long __stdcall Wonline_Dialog_FE_00751190(HWND,unsigned,unsigned,long);
static Wstring Server_Display_Name(char const *name);

static int Ensure_Connection_Locale(rc_ptr<PlayerProfile> const &profile)
{
    if(!profile.Get() || profile->Unknown_Value_10()==0) {
        int locale=Open_Wonline_Dialog_FE_00751130();
        if(profile.Get()) {
            profile->Set_Locale_00739950(locale,3);
            profile->Save_007395d0(unknown_wstring_b26cb8);
        }
        return locale;
    }
    return profile->Unknown_Value_10();
}

bool __fastcall Unknown_007424c0(WOL::Server *server)
{
    strcpy(server->nickname,unknown_wstring_b26cb8.Get());
    strcpy(server->password,unknown_password_b2a63c.Get());
    for(int i=0;i<18;++i) ResetEvent(wol_wait_events_b264f8[i]);
    {
        WideString connecting(StringTable::Load_String(server_connecting_7fe4d0,0,server_source_7fd62c,0x20cd));
        Open_Wait_Window_0074d4d0(0x10,connecting,false);
    }
    unknown_chat_a72cfc->RequestConnection(server,20,*(int const*)&unknown_7fd374&0xff);
    int attempts=20;
    for(;;) {
        int remaining=30000;
        DWORD event=WaitForMultipleObjects(18,wol_wait_events_b264f8,0,100);
        while(event==0x102) {
            remaining-=100;
            if(remaining<=0) goto connect_abort;
            Unknown_0073e100();
            event=WaitForMultipleObjects(18,wol_wait_events_b264f8,0,100);
        }
        ResetEvent(wol_wait_events_b264f8[event]);
        if(event==0) { Close_Wonline_Wait(~0u);return false; }
        if(event==5) {
            Popup_005c3230(StringTable::Load_String(server_cant_connect_7fd740,0,server_source_7fd62c,0x20da),0,Unknown_0073c120,0);
            Close_Wonline_Wait(~0u);return false;
        }
        if(event==18) { connect_abort: Close_Wonline_Wait(~0u);return false; }
        if(attempts--<0) {
            Popup_005c3230(StringTable::Load_String(server_cant_connect_7fd740,0,server_source_7fd62c,0x20e1),0,Unknown_0073c120,0);
            Close_Wonline_Wait(~0u);return false;
        }
        if(event==2) break;
    }
    Close_Wonline_Wait(~0u);
    unknown_chat_a72cfc->SetClientVersion(0x1000e);
    unknown_b2abd8=false;
    unknown_session_a3d298.Read_MultiPlayer_Settings();
    WolRegistryOptions options;
    options.Read_004ff200();
    rc_ptr<PlayerProfile> profile;
    if(options.unknown_004ff2c0()) profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
    else profile.Set(unknown_profile_b28cc4);
    if(unknown_a3d2a4==3 || unknown_a3d2a4==4) {
        profile->Set_Server_00739920(Server_Display_Name(server->name));
        if(options.unknown_004ff2c0()) profile->Save_007395d0(unknown_wstring_b26cb8);
    }
    strcpy(unknown_session_a3d298.Handle,unknown_wstring_b26cb8.Get());
    unknown_session_a3d298.Set_Value_00672a30(profile->Country,false);
    unknown_session_a3d298.Set_Value_00672ae0(profile->Color,false);
    unknown_session_a3d298.Write_MultiPlayer_Settings();
    WOL_Set_Last_Valid_Persona(unknown_wstring_b26cb8);
    if(unknown_ladder_client_a72d00 && strlen(unknown_ladder_address_b29018)) {
        unknown_ladder_client_a72d00->Request(unknown_ladder_address_b29018,unknown_ladder_port_b2a8ac,unknown_wstring_b26cb8.Get(),server_game_version_7fd370&0xffff00,-1,0,0);
    }
    for(int j=0;j<18;++j) ResetEvent(wol_wait_events_b264f8[j]);
    WOL::User user;
    memset(&user,0,sizeof(user));
    strcpy(user.name,unknown_wstring_b26cb8.Get());
    unknown_chat_a72cfc->RequestUserLocale(&user);
    for(;;) {
        int remaining=30000;
        DWORD event=WaitForMultipleObjects(18,wol_wait_events_b264f8,0,100);
        while(event==0x102) {
            remaining-=100;
            if(remaining<=0) goto locale_wait_abort;
            Unknown_0073e100();
            event=WaitForMultipleObjects(18,wol_wait_events_b264f8,0,100);
        }
        ResetEvent(wol_wait_events_b264f8[event]);
        if(event==0) { Close_Wonline_Wait(~0u);break; }
        if(event==5) {
            Popup_005c3230(StringTable::Load_String(server_wdt_error_7fe4c0,0,server_source_7fd62c,0x2135),0,Unknown_0073c120,0);
            Close_Wonline_Wait(~0u);break;
        }
        if(event==18) { locale_wait_abort: Close_Wonline_Wait(~0u);break; }
        if(event==14) break;
    }
    Wstring nick=unknown_wstring_b26cb8.As_Lower();
    if(unknown_channel_names_b293a0.Contains(nick)) {
        int cached_locale=0;
        {
            int *value=0;
            if(unknown_channel_names_b293a0.Get_Pointer_00767570(nick,&value) && value) cached_locale=*value;
        }
        if(cached_locale) {
        if(profile.Get() && profile->Unknown_Value_10()!=cached_locale) {
            if(profile->Unknown_Value_10()==0) {
                profile->Set_Locale_00739950(cached_locale,2);
                profile->Save_007395d0(unknown_wstring_b26cb8);
            }
            int chosen_locale=profile->Unknown_Value_10();
            unknown_chat_a72cfc->RequestSetLocale(chosen_locale);
            unknown_chat_a72cfc->RequestUserLocale(&user);
            cached_locale=chosen_locale;
            int removed;
            if(unknown_channel_names_b293a0.Remove_007675e0(nick,&removed)) unknown_channel_names_b293a0.Add_00766070(nick,cached_locale);
        }
            goto connection_done;
        }
    }
    {
        int locale=Ensure_Connection_Locale(profile);
        if(locale) {
            unknown_chat_a72cfc->RequestSetLocale(locale);
            unknown_chat_a72cfc->RequestUserLocale(&user);
            int old_locale=locale;
            int removed;
            if(unknown_channel_names_b293a0.Remove_007675e0(nick,&removed)) unknown_channel_names_b293a0.Add_00766070(nick,old_locale);
        }
    }
connection_done:
    return true;
}


// bool Unknown_00742d50() (0x00742D50)
// Runs 0x007424C0 on the chosen IRC server (unknown_irc_index_b292d8) when it is a valid index above 0.

bool Unknown_00742d50()
{
    if (unknown_irc_index_b292d8 >= 1 && unknown_irc_index_b292d8 < unknown_servers_b2a700.length()) {
        WOL::Server *server = 0;
        unknown_servers_b2a700.Get(server, unknown_irc_index_b292d8);
        return Unknown_007424c0(server);
    }
    return false;
}


bool __fastcall Recall_Stored_Login_00742d80(Wstring const &requested_name)
{
    unknown_password_b2a63c.Release();
    char const *nickname;
    char const *password;
    for(int index=1;index<=32;++index) {
        if(unknown_chat_a72cfc->unknown_slot36(index,&nickname,&password)==0 && nickname && strlen(nickname) && !_stricmp(requested_name.Peek_Buffer(),nickname)) {
            unknown_wstring_b26cb8=nickname;
            unknown_password_b2a63c=password;
            unknown_7fd374=false;
            return true;
        }
    }
    unknown_wstring_b26cb8.Release();
    return false;
}


int __fastcall Find_Server_Suffix_00742e30(Wstring const &name)
{
    if(name.Length()!=0) {
        for(int i=1;i<unknown_servers_b2a700.length();++i) {
            WOL::Server *server=0;
            unknown_servers_b2a700.Get(server,i);
            char *separator=strchr(server->name,':');
            if(separator && *separator && name==(separator+1)) return i;
        }
    }
    return -1;
}


extern bool connection_wait_callback_0073c120();
extern char connection_url_007fe4e8[];
extern char connection_url_007fe524[];
extern char connection_url_007fe564[];
extern char connection_url_007fe5a0[];
extern char connection_url_007fe5d8[];
extern char connection_url_007fe61c[];
bool __fastcall Unknown_00742eb0()
{
    bool handled=false;
    char url[260];
    if(unknown_connection_result_b2ac10==(int)0x80040077 ||
       unknown_connection_result_b2ac10==(int)0x80040078 ||
       unknown_connection_result_b2ac10==(int)0x80040072 ||
       unknown_connection_result_b2ac10==(int)0x80040074 ||
       unknown_connection_result_b2ac10==(int)0x80040075 ||
       unknown_connection_result_b2ac10==(int)0x80040076) {
        handled=true;
        if(Message_Box_005c3230(unknown_connection_error_b2a8ec,4,connection_wait_callback_0073c120,false)==6) {
            int language=unknown_b2ac5c;
            switch(unknown_connection_result_b2ac10) {
            case (int)0x80040077: sprintf(url,connection_url_007fe61c,language); break;
            case (int)0x80040078: sprintf(url,connection_url_007fe5d8,language); break;
            case (int)0x80040072: sprintf(url,connection_url_007fe5a0,language); break;
            case (int)0x80040074: sprintf(url,connection_url_007fe564,language); break;
            case (int)0x80040075: sprintf(url,connection_url_007fe524,language); break;
            case (int)0x80040076: sprintf(url,connection_url_007fe4e8,language); break;
            }
            unknown_open_url_73c580(url,1);
        }
    } else {
        Message_Box_005c3230(unknown_connection_error_b2a8ec,0,connection_wait_callback_0073c120,false);
    }
    return handled;
}


// Login_Wonline 0x00742FD0: fetch the server list, try the stored auto-login, else run the login dialog.
void __fastcall Open_Wait_Window_0074d4d0(unsigned int, WideString const &, bool);
int __fastcall Unknown_00751c30(unsigned int flags);
Wstring __fastcall WOL_Auto_Login_Persona();
Wstring __fastcall WOL_Last_Valid_Persona();
void __fastcall Unknown_007391f0(Wstring const &);
void __fastcall unknown_5159b0(int, bool);
void __fastcall unknown_517370(bool);
HWND __fastcall unknown_create_dialog_5ff610(int, DLGPROC, int);
void __fastcall unknown_dialog_5ff7c0(HWND);
void __fastcall unknown_dialog_5ff6e0(HWND);
int __fastcall Run_Control_Dialog_006011a0(int, DLGPROC, OwnerDraw::DialogControl *);
long __stdcall Wonline_Account_Dialog_007540a0(HWND, UINT, WPARAM, LPARAM);
long __stdcall Wonline_Dialog_00755070(HWND, UINT, WPARAM, LPARAM);
bool __fastcall Get_Auto_Login_00738cb0();
void Debug_Printf(char const *, ...);
void *__fastcall unknown_find_key_by_value(int value);
extern bool remember_password_b2ac74;
extern char wol_source_file_007fd62c[];

bool Unknown_00742d50();

// Fetch_Server_List, Set_Login_Focus and Remember_Login are level hypotheses (owner ruling 2026-11-05): their
// statements are visible in the body; the inline boundaries come from the expansion budget.
inline void Set_Login_Focus(HWND window)
{
    if (unknown_wstring_b26cb8.Length() > 0 && unknown_password_b2a63c.Length() == 0) {
        SetFocus(GetDlgItem(window, 0x72c));
    } else {
        SetFocus(GetDlgItem(window, 0x628));
    }
}

inline void Remember_Login(WolRegistryOptions &saved, rc_ptr<PlayerProfile> &profile)
{
    if (remember_password_b2ac74 && saved.unknown_004ff2c0()) {
        Save_Nickname_00745320(unknown_wstring_b26cb8.Get(), unknown_password_b2a63c.Get(), unknown_7fd374, profile->Unknown_Value_10());
        unknown_session_a3d298.Write_MultiPlayer_Settings();
    }
}

inline void Reset_WOL_Globals()
{
    memset(&unknown_game_channel_b299c0, 0, sizeof(WOL::Channel));
    memset(&unknown_pending_channel_b26a60, 0, sizeof(WOL::Channel));
    unknown_lobbies_b26c88.setEmpty();
    unknown_games_b29990.setEmpty();
    unknown_users_b2a728.setEmpty();
    unknown_b2abe8 = 0;
}

inline int Fetch_Server_List()
{
    {
        WideString title(StringTable::Load_String("TXT_FETCHING_SERVLIST", 0, wol_source_file_007fd62c, 0x56cc));
        Open_Wait_Window_0074d4d0(0x10, title, false);
    }
    unsigned int flags = 0x2300;
    if (unknown_a3d2a4 != 2) {
        flags = 0x2100;
    }
    return Unknown_00751c30(flags);
}

inline void __fastcall Close_Wonline_Wait(unsigned int flags);
int __fastcall Login_Wonline_00742fd0()
{
    bool failed = false;
    unknown_connection_result_b2ac10 = 0;
    int result = Unknown_00742390();
    if (result != 1) {
        return result;
    }
    {
        Reset_WOL_Globals();
        result = 0;
        int error = Fetch_Server_List();
        switch (error) {
        case 0:
            break;
        case 2:
            failed = true;
            result = -1;
            break;
        default:
            failed = true;
            result = -2;
            break;
        }
        WolRegistryOptions options;
        options.Read_004ff200();
        if (!failed) {
            if (options.unknown_004ff2b0()) {
                Wstring persona = WOL_Auto_Login_Persona();
                if (persona.Length() > 0) {
                    if (Recall_Stored_Login_00742d80(persona)) {
                        int index = -1;
                        if (Is_Online_Session()) {
                            rc_ptr<PlayerProfile> profile = PlayerProfile::Load_00738880(persona);
                            if (profile.Get()) {
                                index = Find_Server_Suffix_00742e30(profile->Get_Server());
                            }
                            if (index == -1) {
                                index = unknown_irc_index_b292d8;
                            }
                        } else {
                            rc_ptr<PlayerProfile> profile = PlayerProfile::Load_00738880(persona);
                            if (profile.Get()) {
                                if (profile->Server.Length() == 0) {
                                    index = unknown_irc_index_b292d8;
                                } else {
                                    index = Find_Server_Suffix_00742e30(profile->Get_Server());
                                }
                            }
                        }
                        if (index != -1) {
                            unknown_irc_index_b292d8 = index;
                            if (Unknown_00742d50()) {
                                return 1;
                            }
                            failed = Unknown_00742eb0();
                        }
                    }
                }
            }
            if (unknown_wstring_b26cb8.Length() == 0 && options.unknown_004ff2c0()) {
                unknown_wstring_b26cb8 = WOL_Last_Valid_Persona();
            }
            {
                Wstring empty((char const *)0x83c55c);
                Unknown_007391f0(empty);
            }
            if (failed) {
                return 0;
            }
            Debug_Printf((char const *)0x7fe65c);
            SimpleWonlineDialogControl control;
            int dialog_id = options.unknown_004ff2c0() ? 0x10f : 0x11d;
            unknown_5159b0(0, true);
            HWND window = unknown_create_dialog_5ff610(dialog_id, (DLGPROC)Wonline_Account_Dialog_007540a0, (int)&control);
            unknown_dialog_5ff7c0(window);
            Set_Login_Focus(window);
            while (!control.unknown_slot2() && !Unknown_0073e100() && !unknown_6000d0()) {
                unknown_517370(false);
            }
            unknown_dialog_5ff6e0(window);
            if ((int)control.Get_Window_00601180() == 2) {
                failed = true;
            } else {
                Unknown_0073e100();
                unknown_b2abd8 = false;
                WolRegistryOptions saved;
                saved.Read_004ff200();
                rc_ptr<PlayerProfile> profile;
                if (saved.unknown_004ff2c0()) {
                    profile = PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
                } else {
                    profile.Set(unknown_profile_b28cc4);
                }
                int found = Find_Server_Suffix_00742e30(profile->Get_Server());
                if (found != -1) {
                    unknown_irc_index_b292d8 = found;
                }
                if (unknown_irc_index_b292d8 >= unknown_servers_b2a700.length()) {
                    unknown_irc_index_b292d8 = -1;
                }
                if (unknown_irc_index_b292d8 == -1 && unknown_servers_b2a700.length() > 1) {
                    unknown_irc_index_b292d8 = 1;
                }
                if (Unknown_00742d50()) {
                    Remember_Login(saved, profile);
                    if (!saved.unknown_004ff2b0() || !Get_Auto_Login_00738cb0()) { return 1; }
                    unknown_5159b0(0, true);
                    SimpleWonlineDialogControl next;
                    Run_Control_Dialog_006011a0(0x112, (DLGPROC)Wonline_Dialog_00755070, &next);
                    return 1;
                }
            }
        }
        Close_Wonline_Wait(~0u);
        if (!failed) {
            Unknown_00742eb0();
        }
        unknown_b2abd8 = true;
    }
    return result;
}


// int Join_WOL_Lobby(HWND window, bool first_time) (0x00743720): the online lobby screen (TibSun Join_WOL_Lobby).
extern "C" __declspec(dllimport) DWORD __stdcall WaitForMultipleObjects(DWORD, HANDLE *, BOOL, DWORD);
extern HANDLE wol_wait_events_b264f8[18];
extern bool connection_wait_callback_0073c120();
extern char wol_channel_list_label_007fd754[], wol_joining_label_7fe688[], wol_source_file_007fd62c[];
extern char wol_login_complete_label_7fe694[], wol_motd_separator_7f3830[];
extern unsigned short wol_wide_newline_7e58ec[];
extern bool unknown_wdt_sounds_initialized_b2ac28;
extern int wol_lan_leave_b2abf0;
extern int unknown_7fd300;
extern bool unknown_7fd354;
extern int unknown_time_b2abd0, unknown_time_b2abd4;
extern HWND game_window_00b263d8;
void __fastcall Open_Wait_Window_0074d4d0(unsigned int, WideString const &, bool);
void __fastcall unknown_5159b0(int, bool);
HWND __fastcall unknown_create_dialog_5ff610(int, DLGPROC, int);
int __stdcall Wonline_Main_Dialog_007469c0(HWND, UINT, WPARAM, LPARAM);
bool __fastcall Get_Channel_List_0073d8f0();
bool __fastcall Request_Chat_Channel_List_0073dd20(bool wait);
int __fastcall Join_Selected_Lobby_00744930(bool unknown);
void __fastcall Initialize_WDT_Sounds_00752210();
void __fastcall Discard_WDT_Sound_List_00752890(WolStringArray sounds);
bool __fastcall End_Dialog_007357e0(HWND dialog, int result);
bool __fastcall Unknown_00514EE0();

inline void __fastcall Close_Wonline_Wait(unsigned int flags);

inline void Set_Wait_Dialog_Text(unsigned short const *text)
{
    HWND dialog = (HWND)WS_Find_Dialog_00735940(0xc4);
    if (dialog) {
        SendDlgItemMessageA(dialog, 0x5b2, 0x4b2, 0, (LPARAM)text);
    }
}

inline Wstring Narrow_Text(unsigned short const *text)
{
    WideString wide(text);
    return Narrow_Copy_00771280(wide);
}

inline WideString Wide_Text(char const *text)
{
    Wstring narrow(text);
    return Wstring_To_Wide(narrow);
}

inline void Print_Message_Of_The_Day()
{
    if (unknown_motd_b26954.Length() > 0) {
        char *text = new char[unknown_motd_b26954.Length() + 1];
        if (text) {
            strcpy(text, unknown_motd_b26954.Get());
            char *token = strtok(text, wol_motd_separator_7f3830);
            while (token) {
                WideString line = Wide_Text(token);
                Unknown_005C2C80(unknown_color_7a7360, line.Get());
                token = strtok(0, wol_motd_separator_7f3830);
            }
            Unknown_005C2C80(unknown_color_7a7360, wol_wide_newline_7e58ec);
            unknown_motd_b26954.Release();
            delete text;
        }
    }
}

inline DWORD Wait_For_Join_Event()
{
    DWORD event;
    while ((event = WaitForMultipleObjects(18, wol_wait_events_b264f8, 0, 100)) == 0x102) {
        Unknown_0073e100();
    }
    return event;
}
int __fastcall Unknown_00743720(HWND window, bool first_time)
{
    int result = 0;
    if (unknown_b2abd9 && unknown_game_mode_a3d298 == 4 && unknown_a3d2a4 == 2
        && unknown_session_a3d298.ScoreWinner != -1 && unknown_session_a3d298.ScoreWinner < 8) {
    Wstring winner = Narrow_Text((unsigned short const *)unknown_session_a3d298.Scores[unknown_session_a3d298.ScoreWinner].Name);
    if (_strnicmp(winner.Get(), unknown_name_a3d3a0, 0x14) == 0) {
        if (unknown_a3d3bc == 0) {
            Discard_WDT_Sound_List_00752890(unknown_strings_b26570);
        } else if (unknown_a3d3bc == 1) {
            Discard_WDT_Sound_List_00752890(unknown_strings_b26a50);
        }
    } else if (unknown_a3d3bc == 0) {
        Discard_WDT_Sound_List_00752890(unknown_strings_b29380);
    }
    }
    unknown_b2abd9 = false;
    unknown_b2ac40 = 0;
    if (unknown_game_mode_a3d298 == 4 && unknown_a3d2a4 == 2) {
        unknown_chat_a72cfc->unknown_slot45(unknown_a3d3bc);
        if (!unknown_wdt_sounds_initialized_b2ac28) {
            Initialize_WDT_Sounds_00752210();
        }
    }
    Unknown_00514EE0();
    if (unknown_b2abda) {
        int login = Login_Wonline_00742fd0();
        if (login != 1) {
            return login;
        }
    }
    int i;
    for (i = 0; i < 18; i++) {
        ResetEvent(wol_wait_events_b264f8[i]);
    }
    if (window == 0) {
        unknown_5159b0(0, true);
        window = unknown_create_dialog_5ff610(0x113, (DLGPROC)Wonline_Main_Dialog_007469c0, 0);
        Print_Message_Of_The_Day();
        {
            WideString title(StringTable::Load_String(wol_login_complete_label_7fe694, 0, wol_source_file_007fd62c, 0x2373));
            Open_Wait_Window_0074d4d0(0x10, title, false);
        }
    }
    unknown_query_count_b2abf4++;
    unknown_squad_query_b2abf8 = 1;
    unknown_chat_a72cfc->unknown_slot44(0);
    unknown_7fd300 = 0x21;
    Set_Wait_Dialog_Text(StringTable::Load_String(wol_channel_list_label_007fd754, 0, wol_source_file_007fd62c, 0x2384));
    if (!Request_Chat_Channel_List_0073dd20(true)) {
        goto cleanup;
    }
    Set_Wait_Dialog_Text(StringTable::Load_String(wol_joining_label_7fe688, 0, wol_source_file_007fd62c, 0x238b));
    {
        int joined = Join_Selected_Lobby_00744930(unknown_7fd354 && !first_time);
        unknown_7fd354 = true;
        if (joined == -1) {
            goto cleanup;
        }
    }
    {
        DWORD event = Wait_For_Join_Event();
        ResetEvent(wol_wait_events_b264f8[event]);
        if (event == 0 || event == 5) {
            goto logout;
        }
    }
    wol_lan_leave_b2abf0 = 0;
    PostMessageA(window, 0x593, 0, 0);
    Close_Wonline_Wait(~0u);
    SetFocus(game_window_00b263d8);
    ShowWindow(window, 1);
    SetFocus(window);
    SetFocus(GetDlgItem(window, 0x540));
    unknown_time_b2abd4 = 0;
    unknown_time_b2abd0 = time(0);
    for (;;) {
        DWORD event;
        while ((event = WaitForMultipleObjects(18, wol_wait_events_b264f8, 0, 20)) == 0x102) {
            Unknown_0073e100();
            if (wol_lan_leave_b2abf0) {
                EnableWindow(GetDlgItem(window, 0x53d), FALSE);
                EnableWindow(GetDlgItem(window, 0x6ee), FALSE);
                EnableWindow(GetDlgItem(window, 0x62d), FALSE);
                EnableWindow(GetDlgItem(window, 0x62b), FALSE);
                EnableWindow(GetDlgItem(window, 0x689), FALSE);
                EnableWindow(GetDlgItem(window, 0x558), FALSE);
                EnableWindow(GetDlgItem(window, 0x61f), FALSE);
                EnableWindow(GetDlgItem(window, 0x686), FALSE);
                if (Join_Selected_Lobby_00744930(false) == -1) {
                    EnableWindow(GetDlgItem(window, 0x686), TRUE);
                    SetEvent(wol_wait_events_b264f8[5]);
                } else {
                    PostMessageA(window, 0x593, 0, 0);
                }
                wol_lan_leave_b2abf0 = 0;
            }
        }
        ResetEvent(wol_wait_events_b264f8[event]);
        if (event == 0) {
            goto cleanup;
        }
        if (event != 6) {
            if (event == 15) {
                result = 2;
                break;
            }
            if (event == 5) {
                goto cleanup;
            }
            if (event == 7) {
                goto started;
            }
        } else {
            break;
        }
    }
    unknown_server_error_b2a8ec[0] = 0;
logout:
    End_Dialog_007357e0(window, 0);
    Unknown_0073e100();
    window = 0;
    if (unknown_chat_a72cfc) {
        unknown_chat_a72cfc->unknown_slot09();
    }
cleanup:
    Close_Wonline_Wait(~0u);
    Message_Box_005c3230(unknown_server_error_b2a8ec, 0, connection_wait_callback_0073c120, false);
    if (window) {
        End_Dialog_007357e0(window, 0);
    }
    while (End_Dialog_007357e0(0, 0)) {
    }
    if (result == 1) {
        unknown_b2abd9 = true;
    }
    if (unknown_game_mode_a3d298 == 4 && unknown_a3d2a4 == 2) {
        unknown_known_names_b294b8.Clear_Inline();
    }
    return result;
started:
    result = 1;
    unknown_chat_a72cfc->unknown_slot09();
    unknown_b2ac04 = false;
    unknown_server_error_b2a8ec[0] = 0;
    goto cleanup;
}


extern "C" int __cdecl wcscmp(wchar_t const *,wchar_t const *);
extern "C" int __cdecl wcsncmp(wchar_t const *,wchar_t const *,unsigned);
extern "C" unsigned __cdecl wcslen(wchar_t const *);
extern "C" wchar_t *__cdecl _wcsdup(wchar_t const *);
extern "C" long __cdecl atol(char const *);
rc_ptr<WDTState> const &Get_New_WDT_State_00769510();
void __fastcall Char_To_Wide_006fa0c0(wchar_t *,char const *,int);
extern PKey lobby_fast_key_00839010;
extern wchar_t *lobby_territory_name_00b28cc8;
extern char lobby_prefix_007fd358[],wdt_mix_007fe6dc[],wdt_map_ini_007fc34c[],wdt_territory_format_007fe6cc[];
extern char wdt_north_america_007fe6bc[],wdt_europe_007fe6b4[],wdt_name_key_007ce2a4[],wdt_default_name_007fe6a8[];
int __fastcall Pick_Lobby_00744090(bool require_room)
{
    int count=unknown_lobbies_b26c88.Count;
    WOL::Channel channel;
    memset(&channel,0,sizeof(channel));
    char section[32],entry[32],fallback[32],name[32];
    wchar_t territory[32];
    if(unknown_session_a3d298.Type==4 && *(int*)(unknown_session_a3d298.unknown_08+4)==2) {
        memset(section,0,sizeof(section));
        // Original clears 32 bytes of the widened-name storage before filling it.
        memset(territory,0,32);
        memset(entry,0,sizeof(entry));
        memset(fallback,0,sizeof(fallback));
        MixFileClass *archive=new MixFileClass(wdt_mix_007fe6dc,&lobby_fast_key_00839010);
        INIClass ini;
        CCFileClass file(wdt_map_ini_007fc34c);
        ini.Load(file,false);
        sprintf(entry,wdt_territory_format_007fe6cc,*(int*)unknown_session_a3d298.unknown_08);
        strcpy(fallback,entry);
        if(Get_New_WDT_State_00769510()->unknown_14==0)
            ini.Get_String(wdt_north_america_007fe6bc,entry,fallback,section,sizeof(section));
        else
            ini.Get_String(wdt_europe_007fe6b4,entry,fallback,section,sizeof(section));
        ini.Get_String(section,wdt_name_key_007ce2a4,wdt_default_name_007fe6a8,name,sizeof(name));
        Char_To_Wide_006fa0c0(territory,name,-1);
        lobby_territory_name_00b28cc8=_wcsdup(territory);
        delete archive;
    }
    Wstring lobby_name=Wstring(lobby_prefix_007fd358)+Wstring_From_Integer(unknown_b2abec,1);
    int i;
    for(i=0;i<count;++i) {
        unknown_lobbies_b26c88.Get(channel,i);
        if(unknown_session_a3d298.Type==4 && *(int*)(unknown_session_a3d298.unknown_08+4)==2) {
            if(wcscmp((wchar_t const*)unknown_lobby_names_b26c94[i],territory)!=0) continue;
            if(!require_room || (int)channel.currentUsers<=25) return i;
        }
        Wstring channel_name(channel.name);
        if(channel_name==lobby_name) {
            if(!require_room || (int)channel.currentUsers<=25) return i;
        }
    }
    int best=-1;
    unsigned int smallest=10000;
    for(i=0;i<count;++i) {
        unknown_lobbies_b26c88.Get(channel,i);
        if(unknown_session_a3d298.Type==4 && *(int*)(unknown_session_a3d298.unknown_08+4)==2) {
            if(wcsncmp((wchar_t const*)unknown_lobby_names_b26c94[i],territory,wcslen(territory))!=0) continue;
            if(!require_room || (int)channel.currentUsers<=25) {
                unknown_b2abec=atol(channel.name+strlen(lobby_prefix_007fd358));
                return i;
            }
            if(channel.currentUsers<smallest) { best=i;smallest=channel.currentUsers; }
        }
        if(strncmp(channel.name,lobby_prefix_007fd358,strlen(lobby_prefix_007fd358))==0) {
            if(!require_room || (int)channel.currentUsers<=25) {
                unknown_b2abec=atol(channel.name+strlen(lobby_prefix_007fd358));
                return i;
            }
            if(channel.currentUsers<smallest) { best=i;smallest=channel.currentUsers; }
        }
    }
    if(best!=-1) {
        unknown_lobbies_b26c88.Get(channel,best);
        unknown_b2abec=atol(channel.name+strlen(lobby_prefix_007fd358));
    }
    return best;
}


extern char wol_leave_debug_7fe6f8[],wol_source_file_007fd62c[],wol_leaving_label_7fe6e4[];
void __fastcall Open_Wait_Window_0074d4d0(unsigned int,WideString const &,bool);
inline void __fastcall Close_Wonline_Wait(unsigned int flags);
bool __fastcall Leave_Current_Channel_00744610(unsigned int flags,bool show)
{
    if(unknown_b2abe8) {
        Debug_Printf(wol_leave_debug_7fe6f8,unknown_game_channel_b299c0.name,unknown_game_channel_b299c0.unknown_00);
        unknown_chat_a72cfc->unknown_slot09();
        {
            WideString title(StringTable::Load_String(wol_leaving_label_7fe6e4,0,wol_source_file_007fd62c,0x24a3));
            Open_Wait_Window_0074d4d0(flags,title,show);
        }
        for(int i=0;unknown_b2abe8 && i<150;++i) {
            Sleep(100);
            Unknown_0073e100();
        }
        Close_Wonline_Wait(0x40);
    }
    return unknown_b2abe8==0;
}


extern char wol_leave_debug_7fe6f8[],wol_join_debug_7fe71c[];
extern char wol_source_file_007fd62c[],wol_leaving_label_7fe6e4[];
void __fastcall Open_Wait_Window_0074d4d0(unsigned int,WideString const &,bool);
int __fastcall Request_Channel_Join_00744700(WOL::Channel *channel,Wstring const &password,WideString title)
{
    char const *name=channel->name;
    if(strcmp(name,unknown_game_channel_b299c0.name) && strcmp(name,unknown_pending_channel_b26a60.name)) {
        if(unknown_b2abe8) {
            Debug_Printf(wol_leave_debug_7fe6f8,unknown_game_channel_b299c0.name,unknown_game_channel_b299c0.unknown_00);
            unknown_chat_a72cfc->unknown_slot09();
            {
                WideString leaving(StringTable::Load_String(wol_leaving_label_7fe6e4,0,wol_source_file_007fd62c,0x24a3));
                Open_Wait_Window_0074d4d0(0x40,leaving,true);
            }
            for(int i=0;unknown_b2abe8 && i<150;++i) {
                Sleep(100);
                Unknown_0073e100();
            }
            void *wait=WS_Find_Dialog_00735940(0xc4);
            if(wait && !(unknown_b2abe0&=~0x40)) Unknown_007357E0(wait,0);
            HWND current=Unknown_00735B10();
            HWND edit=GetDlgItem(current,0x540);
            if(edit) SetFocus(edit);
            if(unknown_b2abe8) return 1;
        }
        strcpy(channel->key,password.Peek_Buffer());
        unknown_pending_channel_b26a60=*channel;
        {
            WideString message(title.unknown_text_770430());
            Open_Wait_Window_0074d4d0(2,message,false);
        }
        Debug_Printf(wol_join_debug_7fe71c,name);
        return unknown_chat_a72cfc->unknown_slot08(channel);
    }
    return 0;
}


// Original callers pass a zero low-byte register argument. Its boolean source type
// is a hypothesis: this body never reads the argument, so signedness is unobservable.
int __fastcall Pick_Lobby_00744090(bool require_room);
void __fastcall Refresh_Game_List_0073f8f0();
int __fastcall Unknown_00744700(WOL::Channel const &channel, Wstring const &password, WideString title);
int __fastcall Join_Selected_Lobby_00744930(bool unknown)
{
    int choice=Pick_Lobby_00744090(unknown);
    if(choice!=-1) {
        WOL::Channel channel;
        unknown_lobbies_b26c88.Get(channel,choice);
        int result;
        {
        Wstring password("zotclot9");
        result=Unknown_00744700(channel,password,StringTable::Load_String("TXT_JOINLOB",0,"D:\\Ra2\\wonline.cpp",0x24d4));
        }
        if(!result) {
            HWND dialog=(HWND)WS_Find_Dialog_00735940(0x113);
            if(dialog) SetFocus(GetDlgItem(dialog,0x540));
            Refresh_Game_List_0073f8f0();
            return choice;
        }
    }
    Refresh_Game_List_0073f8f0();
    return -1;
}


extern HANDLE wol_wait_events_b264f8[18];
extern bool connection_wait_callback_0073c120();
void __fastcall Open_Wait_Window_0074d4d0(unsigned int,WideString const &,bool);
inline void __fastcall Close_Wonline_Wait(unsigned int);
void __stdcall unknown_dialog_5ecf40(HWND,int);
long __stdcall Birthdate_Dialog_007494d0(HWND,UINT,WPARAM,LPARAM);
long __stdcall Account_Details_Dialog_007498e0(HWND,UINT,WPARAM,LPARAM);
extern Wstring unknown_wstring_b29ce4;
extern int account_birth_year_b2a6d4,account_birth_day_b26930,account_birth_month_b26894,account_newsletter_b29098;
extern Wstring account_name_b2a640,account_password_b2a71c,account_password_check_b2a724,unknown_account_detail_b2945c;
extern int unknown_account_checkbox_b29ad0;
extern char wol_source_file_007fd62c[];
extern char account_text_7fe82c[];
extern char account_text_7fe814[];
extern char account_text_7fe808[];
extern char account_text_7fe800[];
extern char account_text_7fd740[];
extern char account_text_7fe7e8[];
extern char account_text_7fe7d4[];
extern char account_text_7fe7bc[];
extern char account_text_7fe780[];
extern char account_text_7fe76c[];
extern char account_text_7fe754[];
extern char account_text_7fe744[];
void __fastcall Create_Wonline_Account_00744a00(HWND parent)
{
    OwnerDraw::unknown_5e5150(parent,false);
    unknown_5159b0(0,true);
    char nickname[32];
    char password[32];
    char confirmation[32];
    char email[128];
    char parent_email[128];
    int newsletter,day,year,month,share_info;
    HWND dialog;
    newsletter=1;
    month=1;
    day=0;
    year=0;
    for(int event_index=0;event_index<18;++event_index) ResetEvent(wol_wait_events_b264f8[event_index]);
    memset(nickname,0,sizeof(nickname));
    memset(password,0,sizeof(password));
    memset(confirmation,0,sizeof(confirmation));
    memset(email,0,sizeof(email));
    memset(parent_email,0,sizeof(parent_email));
    {
        WideString message(StringTable::Load_String(account_text_7fe82c,0,wol_source_file_007fd62c,0x251d));
        Open_Wait_Window_0074d4d0(0x80,message,false);
    }
    Debug_Printf(account_text_7fe814);
    unknown_chat_a72cfc->Request_Server_List(unknown_protocol_7fd370,0x10006,account_text_7fe800,account_text_7fe808,35);
    DWORD event;
    do {
        event=WaitForMultipleObjects(18,wol_wait_events_b264f8,false,100);
        while(event==0x102) {
            Unknown_0073e100();
            event=WaitForMultipleObjects(18,wol_wait_events_b264f8,false,100);
        }
        ResetEvent(wol_wait_events_b264f8[event]);
    } while(event!=0 && event!=1);
    Close_Wonline_Wait(~0u);
    if(event==0) {
        ShowWindow(parent,5);
        unknown_dialog_5ecf40(parent,0);
        return;
    }
    if(event==8) {
        SetEvent(wol_wait_events_b264f8[8]);
        ShowWindow(parent,5);
        unknown_dialog_5ecf40(parent,0);
        return;
    }
birthdate_retry:
    unknown_5159b0(0,true);
    {
        dialog=(HWND)unknown_create_dialog_735700(ProgramInstance,0xe7,MainWindow,(UnknownDialogProc)Birthdate_Dialog_007494d0,0);
        unknown_show_dialog_5ff7e0(dialog,0xe7);
        SendDlgItemMessageA(dialog,0x62e,0x14e,month-1,0);
        if(day>0) {
            Wstring text=Wstring_From_Integer(day);
            char const *number=text.Get();
            HWND edit=GetDlgItem(dialog,0x62f);
            SendMessageA(edit,0x4b4,0,(LPARAM)number);
        } else {
            HWND edit=GetDlgItem(dialog,0x62f);
            SendMessageA(edit,0x4b4,0,(LPARAM)unknown_string_83c55c);
        }
        if(year>0) {
            Wstring text=Wstring_From_Integer(year);
            char const *number=text.Get();
            HWND edit=GetDlgItem(dialog,0x630);
            SendMessageA(edit,0x4b4,0,(LPARAM)number);
        } else {
            HWND edit=GetDlgItem(dialog,0x630);
            SendMessageA(edit,0x4b4,0,(LPARAM)unknown_string_83c55c);
        }
        SendDlgItemMessageA(dialog,0x626,0x4b4,0,(LPARAM)email);
        SendDlgItemMessageA(dialog,0x631,0xf1,newsletter,0);
        ShowWindow(parent,0);
        ShowWindow(dialog,1);
        if(Unknown_007359e0(dialog,connection_wait_callback_0073c120,0,1,1)==1) {
            email[0]=0;
            strcpy(email,unknown_wstring_b29ce4.Get());
            month=account_birth_month_b26894+1;
            day=account_birth_day_b26930;
            year=account_birth_year_b2a6d4;
            newsletter=account_newsletter_b29098;
            if(month<=0 || day<=0 || year<=0 || strlen(email)<6) {
                Message_Box_005c3230(StringTable::Load_String(account_text_7fe7d4,0,wol_source_file_007fd62c,0x257b),0,connection_wait_callback_0073c120,false);
                goto birthdate_retry;
            }
            unknown_b2ac70=0;
            unknown_b2ac6c=-1;
            if(unknown_ladder_client_a72d00->Request_Age_Check(month,day,year,email)!=0) {
                Message_Box_005c3230(StringTable::Load_String(account_text_7fd740,0,wol_source_file_007fd62c,0x2583),0,connection_wait_callback_0073c120,false);
                goto account_retry;
            }
            {
                WideString message(StringTable::Load_String(account_text_7fe7e8,0,wol_source_file_007fd62c,0x2586));
                Open_Wait_Window_0074d4d0(0x100,message,true);
            }
            if(unknown_b2ac6c<0) {
                if(unknown_b2ac0c==(int)0x80040100)
                Message_Box_005c3230(StringTable::Load_String(account_text_7fe7d4,0,wol_source_file_007fd62c,0x258a),0,connection_wait_callback_0073c120,false);
                else
                Message_Box_005c3230(StringTable::Load_String(account_text_7fd740,0,wol_source_file_007fd62c,0x258c),0,connection_wait_callback_0073c120,false);
                goto birthdate_retry;
            }
            if(unknown_b2ac6c<13 && !unknown_b2ac70) {
                if(Message_Box_005c3230(StringTable::Load_String(account_text_7fe7bc,0,wol_source_file_007fd62c,0x2595),4,connection_wait_callback_0073c120,false)==6) {
                    char url[256];
                    sprintf(url,account_text_7fe780,unknown_b2ac5c);
                    unknown_open_url_73c580(url,1);
                }
                goto birthdate_retry;
            }
account_retry:
            {
                int resource;
                if(unknown_b2ac6c<13) resource=0xe6;
                else resource=unknown_b2ac6c>=18 ? 0xf4 : 0xf3;
                dialog=(HWND)unknown_create_dialog_735700(ProgramInstance,resource,MainWindow,(UnknownDialogProc)Account_Details_Dialog_007498e0,0);
                unknown_show_dialog_5ff7e0(dialog,resource);
                if(nickname && strlen(nickname)>0) {
                    HWND edit=GetDlgItem(dialog,0x628);
                    SendMessageA(edit,0x4b4,0,(LPARAM)nickname);
                } else {
                    HWND edit=GetDlgItem(dialog,0x628);
                    SendMessageA(edit,0x4b4,0,(LPARAM)unknown_string_83c55c);
                }
                if(parent_email && strlen(parent_email)>0) {
                    HWND edit=GetDlgItem(dialog,0x5e5);
                    SendMessageA(edit,0x4b4,0,(LPARAM)parent_email);
                } else {
                    HWND edit=GetDlgItem(dialog,0x5e5);
                    SendMessageA(edit,0x4b4,0,(LPARAM)unknown_string_83c55c);
                }
                if(password && strlen(password)>0) {
                    HWND edit=GetDlgItem(dialog,0x72c);
                    SendMessageA(edit,0x4b4,0,(LPARAM)password);
                } else {
                    HWND edit=GetDlgItem(dialog,0x72c);
                    SendMessageA(edit,0x4b4,0,(LPARAM)unknown_string_83c55c);
                }
                if(confirmation && strlen(confirmation)>0) {
                    HWND edit=GetDlgItem(dialog,0x627);
                    SendMessageA(edit,0x4b4,0,(LPARAM)confirmation);
                } else {
                    HWND edit=GetDlgItem(dialog,0x627);
                    SendMessageA(edit,0x4b4,0,(LPARAM)unknown_string_83c55c);
                }
                ShowWindow(dialog,1);
                SendMessageA(dialog,0x4a9,0,1);
                if(Unknown_007359e0(dialog,connection_wait_callback_0073c120,0,1,1)!=1) goto birthdate_retry;
                nickname[0]=0;
                strcpy(nickname,account_name_b2a640.Get());
                password[0]=0;
                strcpy(password,account_password_b2a71c.Get());
                confirmation[0]=0;
                strcpy(confirmation,account_password_check_b2a724.Get());
                parent_email[0]=0;
                if(resource==0xf3) strcpy(parent_email,unknown_account_detail_b2945c.Get());
                share_info=0;
                if(resource==0xf4) share_info=unknown_account_checkbox_b29ad0;
                if(strcmp(password,confirmation)) {
                    Message_Box_005c3230(StringTable::Load_String(account_text_7fe76c,0,wol_source_file_007fd62c,0x25ff),0,connection_wait_callback_0073c120,false);
                    goto account_retry;
                }
                if(strlen(password)!=8) {
                    Message_Box_005c3230(StringTable::Load_String(account_text_7fe754,0,wol_source_file_007fd62c,0x2604),0,connection_wait_callback_0073c120,false);
                    goto account_retry;
                }
                if(unknown_ladder_client_a72d00->Request_New_Nick(nickname,password,email,parent_email,newsletter,share_info)!=0) {
                    Message_Box_005c3230(StringTable::Load_String(account_text_7fd740,0,wol_source_file_007fd62c,0x2609),0,connection_wait_callback_0073c120,false);
                } else {
                    {
                        WideString message(StringTable::Load_String(account_text_7fe744,0,wol_source_file_007fd62c,0x260b));
                        Open_Wait_Window_0074d4d0(0x100,message,true);
                    }
                    if(unknown_newnick_result_b2ac08<0) goto account_retry;
                }
            }
            ShowWindow(parent,5);
            unknown_dialog_5ecf40(parent,0);
            return;
        } else {
            ShowWindow(parent,1);
        }
    }
    ShowWindow(parent,5);
    unknown_dialog_5ecf40(parent,0);
}


extern int unknown_last_nickname_slot_a3d554;
int __fastcall Save_Nickname_00745320(char const *name,char const *password,bool mangle,int locale)
{
    int slot=0;
    char const *nick;
    char const *stored_password;
    for(int i=1;i<=32;++i) {
        nick=0;
        stored_password=0;
        if(unknown_chat_a72cfc->unknown_slot36(i,&nick,&stored_password)==0 && nick && !strcmp(nick,name)) {
            slot=i;
            break;
        }
    }
    if(slot==0) {
        for(int i=1;i<=32;++i) {
            nick=0;
            stored_password=0;
            if(unknown_chat_a72cfc->unknown_slot36(i,&nick,&stored_password)!=0 || !nick || strlen(nick)==0) {
                slot=i;
                break;
            }
        }
    }
    if(slot==0) {
        int next=unknown_last_nickname_slot_a3d554;
        if(next==-1) next=1;
        slot=next++;
        unknown_last_nickname_slot_a3d554=next;
        if(next>32) unknown_last_nickname_slot_a3d554=1;
    }
    if(slot) {
        unknown_chat_a72cfc->unknown_slot37(slot,name,password,(unsigned char)mangle);
        unknown_chat_a72cfc->unknown_slot50(slot,locale);
    }
    return slot;
}


// Wstring __fastcall Unknown_00745450(WOL::Server const *server) (0x00745450)
// The server name after its first ':' (the whole name when it has none).

Wstring __fastcall Unknown_00745450(WOL::Server const *server)
{
    Wstring name(server->name);
    Wstring token;
    int position = name.Get_Token(0, ":", token);
    if (position >= 0) name.Remove(0, position + 1);
    return name;
}


void __fastcall Reset_Server_List_00600db0(HWND list);
extern char no_server_text_007fe854[],server_found_debug_007fe840[],server_name_separator_007e6f8c[];
static Wstring Server_Display_Name(char const *name)
{
    Wstring full(name);
    Wstring prefix;
    int split=full.Get_Token(0,server_name_separator_007e6f8c,prefix);
    if(split>=0) full.Remove(0,split+1);
    return full;
}
bool __fastcall Populate_Server_Choices_007454c0(HWND list,bool combo)
{
    if(combo) unknown_reset_combo_6007f0(list);
    else Reset_Server_List_00600db0(list);
    if(unknown_servers_b2a700.Count==1) {
        if(combo) SendMessageA(list,0x4bc,0,(LPARAM)no_server_text_007fe854);
        else SendMessageA(list,0x4c8,0,(LPARAM)no_server_text_007fe854);
        EnableWindow(list,false);
        return false;
    }
    Wstring preferred;
    WolRegistryOptions options;
    options.Read_004ff200();
    rc_ptr<PlayerProfile> profile;
    if(options.unknown_004ff2c0()) profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
    else profile.Set(unknown_profile_b28cc4);
    if(profile.Get()) preferred=profile->Server;
    EnableWindow(list,true);
    int selected=-1;
    for(int i=1;i<unknown_servers_b2a700.Count;++i) {
        WOL::Server *server=0;
        unknown_servers_b2a700.Get(server,i);
        Debug_Printf(server_found_debug_007fe840,server->name);
        Wstring display=Server_Display_Name(server->name);
        int row;
        if(combo) {
            row=SendMessageA(list,0x4bc,0,(LPARAM)display.Peek_Buffer());
            SendMessageA(list,0x151,row,i);
        } else {
            row=SendMessageA(list,0x4c8,0,(LPARAM)display.Peek_Buffer());
            SendMessageA(list,0x19a,row,i);
        }
        if(preferred.Length()>0) {
            bool match=preferred.As_Lower()==display.As_Lower();
            if(match) {
                selected=row;
                unknown_irc_index_b292d8=i;
                unknown_irc_index_b2a7f4=i;
            }
        }
    }
    if(selected==-1) selected=0;
    if(combo) SendMessageA(list,0x14e,selected,0);
    else {
        SendMessageA(list,0x186,selected,0);
        SendMessageA(list,0x197,selected,0);
    }
    return true;
}


bool __fastcall Can_Join_Channel_00755cd0(WOL::Channel const *,bool,bool);
bool __fastcall Test_Display_Mode(int width,int height);
int __fastcall Run_Control_Dialog_006011a0(int,DLGPROC,OwnerDraw::DialogControl *);
long __stdcall Wonline_Password_Dialog_007492d0(HWND,unsigned,unsigned,long);
int __fastcall Join_Channel_00744700(WOL::Channel const *,Wstring const &,WideString);
extern bool connection_wait_callback_0073c120();
extern char wol_source_file_007fd62c[];
extern char wol_screen_800_label_7fe8b8[],wol_screen_limit_label_7fe89c[],wol_joining_label_7fe688[];
extern char wol_password_find_7e613c[],wol_password_replace_7cf730[];
bool __fastcall Join_Game_Channel_00745780(WOL::Channel const *channel,bool observer)
{
    if(!Can_Join_Channel_00755cd0(channel,true,observer)) return false;
    unsigned int mode=channel->unknown_20;
    int maximum=(mode&0x400)?800:((mode&0x200)?640:1024);
    if(unknown_8523e0>maximum) {
        {
            WideString warning((mode&0x400)?
                StringTable::Load_String(wol_screen_800_label_7fe8b8,0,wol_source_file_007fd62c,0x279f):
                StringTable::Load_String(wol_screen_limit_label_7fe89c,0,wol_source_file_007fd62c,0x27a0));
            if(Message_Box_005c3230(warning.Get(),4,connection_wait_callback_0073c120,true)==6) {
                int width=(maximum==800)?800:640;
                int height=(width==800)?600:480;
                if(Test_Display_Mode(width,height)) {
                    Options.ScreenWidth=width;
                    Options.ScreenHeight=height;
                    Options.Save_Settings();
                }
            }
        }
        if(unknown_8523e0>maximum) return false;
    }
    Wstring password;
    // The external WOL channel flags occupy the second DWORD of this still-flat field.
    if(*(unsigned int const *)(channel->unknown_18+4)&0x100) {
        unknown_5159b0(0,true);
        WonlineStringDialogControl control;
        if(Run_Control_Dialog_006011a0(0xe1,(DLGPROC)Wonline_Password_Dialog_007492d0,&control)==2) return false;
        password=control.Text;
        password.Replace(wol_password_find_7e613c,wol_password_replace_7cf730);
    }
    return Join_Channel_00744700(channel,password,StringTable::Load_String(wol_joining_label_7fe688,0,wol_source_file_007fd62c,0x27d7))==0;
}


// bool __fastcall Host_Game_007459a0() (0x007459A0)
// Hosts a WOL game: asks for the game's options (the quick match kinds skip the dialog), creates the game channel,
// rebuilds the session with the local player as host and opens the game setup dialog. Twin: TibSun wonline.cpp's
// IDC_NEWGAME case of WOL_Main_Dialog_Proc. Original 2,203 B + 5 NOP.
extern "C" int __cdecl sprintf(char *,char const *,...);
extern int wol_max_players_b2abdc;
void __fastcall Format_User_Options_0073cff0(char *);
int __fastcall Run_Control_Dialog_006011a0(int,DLGPROC,OwnerDraw::DialogControl *);
long __stdcall Wonline_Create_Game_Dialog_00748e10(HWND,UINT,WPARAM,LPARAM);
int __stdcall WOL_Game_Options_Proc_0074a710(HWND,UINT,WPARAM,LPARAM);
void __fastcall Seat_First_Player_005c8cd0(int slot);   // 0x005C8CD0 (netshare.cpp)
void __fastcall unknown_5159b0(int,bool);
extern char wol_password_find_7e613c[],wol_password_replace_7cf730[],wol_game_name_format_7fe938[];
extern char wol_leave_debug_7fe6f8[],wol_leaving_label_7fe6e4[];
extern char wol_source_file_007fd62c[],wol_bad_key_label_7fe928[],wol_cant_create_label_7fe914[],wol_creating_label_7fe900[];
// Leave_Current_Channel 0x00744610 without its result, one inline level above the wait window close (depth 2 here).
inline void Leave_Game_Channel(unsigned int flags,bool show)
{
    if(unknown_b2abe8) {
        Debug_Printf(wol_leave_debug_7fe6f8,unknown_game_channel_b299c0.name,unknown_game_channel_b299c0.unknown_00);
        unknown_chat_a72cfc->unknown_slot09();
        {
            WideString title(StringTable::Load_String(wol_leaving_label_7fe6e4,0,wol_source_file_007fd62c,0x24a3));
            Open_Wait_Window_0074d4d0(flags,title,show);
        }
        for(int i=0;unknown_b2abe8 && i<150;++i) {
            Sleep(100);
            Unknown_0073e100();
        }
        Close_Wonline_Wait(0x40);
    }
}
// The text of a label of this file (name hypothesis).
inline wchar_t const *Wonline_Text(char *label,int line)
{
    return StringTable::Load_String(label,0,wol_source_file_007fd62c,line);
}
inline void Show_Game_Options()
{
    if(!Is_Online_Session()) {
        HWND window=Unknown_00735B10();
        if(window) {
            OwnerDraw::unknown_5e5150(window,false);
            unknown_5159b0(0,true);
        }
        unknown_5159b0(0,true);
        HWND dialog=unknown_create_dialog_5ff610(0xc2,(DialogProc)WOL_Game_Options_Proc_0074a710,0);
        unknown_session_a3d298.Mode->unknown_slot33();
        unknown_session_a3d298.Mode->unknown_slot8();
        unknown_session_a3d298.Mode->AssignTeamsAtStart();
        unknown_session_a3d298.Mode->UnfixAlliances();
        ShowWindow(dialog,1);
        if(window) ShowWindow(window,0);
    }
}
inline void Reset_Session_Players()
{
    MultiplayerGameMode *mode=MultiplayerGameMode::unknown_first_mode_5b6ec0();
    unknown_session_a3d298.Mode=mode;
    unknown_session_a3d298.GameModeIndex=mode->unknown_28;
    mode->unknown_slot8();
    Select_Scenario_005c7c30(0);
    Free_Pointers_005a7480(&unknown_players_a3fa7c);
    Clear_Slot_Lists_005c8a70();
    Unknown_Reset_Slots_005c8a90();
}
bool __fastcall Host_Game_007459a0()
{
    Wstring password;
    if(unknown_8523e0>=0x400) unknown_session_a3d298.WOLLimitResolution=false;
    CreateGameDialogControl control(unknown_session_a3d298.WOLLimitResolution,unknown_a72c68==1);
    unknown_b2ac04=false;
    if(Is_Online_Session()) wol_max_players_b2abdc=2;
    else {
        unknown_5159b0(0,true);
        if(Run_Control_Dialog_006011a0(0xc3,(DLGPROC)Wonline_Create_Game_Dialog_00748e10,&control)==2) return false;
        password=control.Text;
        password.Replace(wol_password_find_7e613c,wol_password_replace_7cf730);
    }
    unknown_chat_a72cfc->unknown_slot09();
    WOL::Channel channel;
    memset(&channel,0,sizeof(channel));
    sprintf(channel.channel_name,wol_game_name_format_7fe938,unknown_wstring_b26cb8);
    channel.unknown_00=0x21;
    channel.minUsers=1;
    if(wol_max_players_b2abdc<2) wol_max_players_b2abdc=4;
    strcpy(channel.key,password.Get());
    channel.maxUsers=wol_max_players_b2abdc;
    char user_options[80];
    memset(user_options,0,sizeof(user_options));
    Format_User_Options_0073cff0(user_options);
    user_options[40]=0;
    strcpy(channel.exInfo,user_options);
    if(unknown_session_a3d298.Type==4&&unknown_a3d2a4==2) {
        channel.unknown_14=1;
        channel.unknown_20|=0x1000;
    }
    else if(Is_Online_Session()) {
        channel.unknown_14=1;
        channel.unknown_20|=0x800;
    }
    else {
        channel.unknown_14=control.unknown_10;
        if(control.unknown_11) channel.unknown_20|=0x100;
        unknown_session_a3d298.WOLLimitResolution=control.unknown_12;
        if(control.unknown_12) channel.unknown_20|=unknown_8523e0==800?0x400:0x200;
    }
    channel.unknown_20+=unknown_b2abec;
    Leave_Game_Channel(0xc0,false);
    Reset_Session_Players();
    NodeNameType *who=new NodeNameType;
    memset(who,0,sizeof(NodeNameType));
    WideString name=Wstring_To_Wide(unknown_wstring_b26cb8);
    wcscpy(who->Name,name.Get());
    WolRegistryOptions options;
    options.Read_004ff200();
    rc_ptr<PlayerProfile> profile;
    if(options.unknown_004ff2c0()) profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
    else profile.Set(unknown_profile_b28cc4);
    if(profile.Get()) {
        unknown_session_a3d298.Set_Value_00672a30(profile->Country,false);
        unknown_session_a3d298.Set_Value_00672ae0(profile->Color,false);
    }
    who->unknown_66e2d0(unknown_session_a3d298.Value_0124_006729f0());
    who->unknown_66e300(unknown_session_a3d298.Value_011c_00672a10());
    who->unknown_67=1;
    who->Team=0;
    unknown_lobby_flag_7e6d90=7;
    unknown_a72c68=control.unknown_13;
    Unknown_005cd540(0);
    if(unknown_a72c68) {
        unknown_lobby_flag_7e6d90=0;
        who->Team=-1;
        who->unknown_66e2d0(-3);
    }
    unknown_players_a3fa7c.Add(who);
    Seat_First_Player_005c8cd0(0);
    wcscpy(unknown_host_name_a3f618,name.Get());
    HRESULT result=unknown_chat_a72cfc->unknown_slot07(&channel);
    if(result==0x800401f8) {
        Message_Box_005c3230(Wonline_Text(wol_bad_key_label_7fe928,0x28c2),0,Unknown_0073c120,false);
        Close_Wonline_Wait(0x80);
        return false;
    }
    if(result!=0) {
        Message_Box_005c3230(Wonline_Text(wol_cant_create_label_7fe914,0x28c8),0,Unknown_0073c120,false);
        Close_Wonline_Wait(0x80);
        return false;
    }
    WideString title(Wonline_Text(wol_creating_label_7fe900,0x28d0));
    Open_Wait_Window_0074d4d0(4,title,false);
    Close_Wonline_Wait(0x80);
    Open_Wait_Window_0074d4d0(4,title,true);
    int index=unknown_users_b2a728.Find_Name(unknown_wstring_b26cb8);
    if(index<0) return false;
    WOL::User *user=0;
    unknown_users_b2a728.Get(user,index);
    {
        unsigned char net[4];
        unsigned char node[6];
        memset(net,0,sizeof(net));
        memset(node,0,sizeof(node));
        unsigned int ip_addr=ntohl(user->unknown_18);
        node[3]=ip_addr&0xff;
        node[2]=(ip_addr>>8)&0xff;
        node[1]=(ip_addr>>16)&0xff;
        node[0]=(ip_addr>>24)&0xff;
        IPXAddressClass address(net,node,0);
        who->Address=address;
    }
    Show_Game_Options();
    return true;
}


int Gadget_Color_Scheme();
extern "C" int __cdecl wcsncmp(wchar_t const *,wchar_t const *,unsigned int);
extern char online_command_label_007fe950[],missing_name_label_007fe944[],wol_source_file_007fd62c[];
extern char online_last_name_00b29cd8[];
int __fastcall Parse_Online_Command_00746240(wchar_t const *text)
{
    int handled=0;
    wchar_t command[64];
    wcscpy(command,StringTable::Load_String(online_command_label_007fe950,0,wol_source_file_007fd62c,0x2909));
    if(wcsncmp(text,command,wcslen(command))==0) {
    int count=0;
    wchar_t const *cursor=text+wcslen(command);
    while(wcsncmp(cursor,L" ",1)==0) ++cursor;
    wchar_t name[16];
    while(*cursor) {
        if(wcsncmp(cursor,L" ",1)==0 || count>=10) break;
        name[count++]=*cursor++;
    }
    name[count]=0;
    if(wcslen(name)>0) {
    WOL::User user;
    strcpy(user.name,Wide_To_Narrow_006fa120((unsigned short const *)name));
    strcpy(online_last_name_00b29cd8,Wide_To_Narrow_006fa120((unsigned short const *)name));
    unknown_chat_a72cfc->unknown_slot24(&user);
    handled=1;
    } else {
        wchar_t message[256];
        swprintf(message,StringTable::Load_String(missing_name_label_007fe944,0,wol_source_file_007fd62c,0x291d));
        HWND window=Unknown_00735B10();
        if(window) {
            if(GetDlgItem(window,0x53f)) Unknown_005C2C80(unknown_color_7a7360,(unsigned short const *)message);
            if(GetDlgItem(window,0x5b5)) Unknown_005c2d10(unknown_color_7a7360,(unsigned short const *)message);
        }
        if(unknown_session_a3d298.Unknown_00672F10()) {
            int color=Gadget_Color_Scheme();
            unknown_session_a3d298.Messages.Add_Message(0,0,(unsigned short const *)message,color,0x16,Rules->MessageDelay*900,false);
        }
        handled=1;
    }
    }
    return handled;
}


extern "C" int __cdecl _wcsnicmp(wchar_t const *,wchar_t const *,unsigned);
extern "C" int __cdecl wcsncmp(wchar_t const *,wchar_t const *,unsigned);
extern "C" int __cdecl _wcsicmp(wchar_t const *,wchar_t const *);
int Gadget_Color_Scheme();
extern char wol_source_file_007fd62c[];
extern char wol_reply_observer_label_007fe964[];
extern wchar_t wol_reply_space_007d64f4[];
extern char wol_page_label_007dfc7c[];
int __fastcall Parse_Page_Command_00746480(wchar_t const *text)
{
    int result=0;
    wchar_t nickname[16];
    WOL::User recipient;
    wchar_t command[128];
    wcscpy(command,StringTable::Load_String(wol_page_label_007dfc7c,0,wol_source_file_007fd62c,0x293f));
    if(_wcsnicmp(text,command,wcslen(command))==0) {
        int name_length=0;
        unsigned length=wcslen(text)+1;
        wchar_t *copy=(wchar_t *)operator new(length*2);
        if(!copy) return 0;
        copy=wcsncpy(copy,text,length);
        if(!copy) return 0;
        wchar_t *message=copy+wcslen(command);
        while(*message && wcsncmp(message,wol_reply_space_007d64f4,1)==0) ++message;
        while(*message) {
            if(wcsncmp(message,wol_reply_space_007d64f4,1)==0 || name_length>10) break;
            nickname[name_length++]=*message++;
        }
        nickname[name_length]=0;
        while(*message && wcsncmp(message,wol_reply_space_007d64f4,1)==0) ++message;
        if(name_length>10) {
            result=1;
        } else if(name_length==0) {
            result=1;
        } else {
            bool recipient_is_player=false;
            if(unknown_int_a3d548) {
                for(int i=0;i<unknown_players_a3fa7c.Count();++i) {
                    if(_wcsicmp(unknown_players_a3fa7c[i]->Name,nickname)==0) {
                        recipient_is_player=true;
                        break;
                    }
                }
            }
            if(!recipient_is_player) {
                if(wcslen(message)>0) {
                    strcpy(recipient.name,Wide_To_Narrow_006fa120(nickname));
                    unknown_chat_a72cfc->unknown_slot62(&recipient,message);
                }
                result=1;
            } else {
                int color=Gadget_Color_Scheme();
                unknown_session_a3d298.Messages.Add_Message(0,0,
                StringTable::Load_String(wol_reply_observer_label_007fe964,0,wol_source_file_007fd62c,0x297b),
                color,0x16,(int)(Rules->MessageDelay*900.0),false);
                result=2;
            }
        }
        operator delete(copy);
    }
    return result;
}


extern "C" int __cdecl _wcsnicmp(wchar_t const *,wchar_t const *,unsigned);
extern "C" int __cdecl wcsncmp(wchar_t const *,wchar_t const *,unsigned);
extern "C" int __cdecl _wcsicmp(wchar_t const *,wchar_t const *);
int Gadget_Color_Scheme();
extern char wol_source_file_007fd62c[];
extern char wol_reply_label_007dfc14[];
extern char wol_reply_short_label_007dfbfc[];
extern char wol_reply_empty_label_007fe97c[];
extern char wol_reply_observer_label_007fe964[];
extern wchar_t wol_reply_space_007d64f4[];
int __fastcall Reply_To_Last_Pager_007466c0(wchar_t const *text)
{
    int result=0;
    wchar_t reply_command[128];
    WOL::User recipient;
    wchar_t short_command[128];
    wchar_t notice[128];
    wcscpy(reply_command,StringTable::Load_String(wol_reply_label_007dfc14,0,wol_source_file_007fd62c,0x298f));
    wcscpy(short_command,StringTable::Load_String(wol_reply_short_label_007dfbfc,0,wol_source_file_007fd62c,0x2990));
    if(_wcsnicmp(text,reply_command,wcslen(reply_command))==0 ||
    wcsncmp(text,short_command,wcslen(short_command))==0) {
        unsigned length=wcslen(text)+1;
        wchar_t *copy=(wchar_t *)operator new(length*2);
        if(!copy) return 0;
        copy=wcsncpy(copy,text,length);
        if(!copy) return 0;
        wchar_t *message;
        if(wcsncmp(text,reply_command,wcslen(reply_command))==0) message=copy+wcslen(reply_command);
        else message=copy+wcslen(short_command);
        while(*message && wcsncmp(message,wol_reply_space_007d64f4,1)==0) ++message;
        bool recipient_is_player=false;
        if(unknown_int_a3d548) {
            for(int i=0;i<unknown_players_a3fa7c.Count();++i) {
                if(_wcsicmp(unknown_players_a3fa7c[i]->Name,Narrow_To_Wide_006fa180(unknown_last_pager_b2ac48))==0) {
                    recipient_is_player=true;
                    break;
                }
            }
        }
        if(!recipient_is_player) {
            if(strlen(unknown_last_pager_b2ac48)>0) {
                if(wcslen(message)>0) {
                    strcpy(recipient.name,unknown_last_pager_b2ac48);
                    unknown_chat_a72cfc->unknown_slot62(&recipient,message);
                }
                result=1;
            } else {
                wcscpy(notice,StringTable::Load_String(wol_reply_empty_label_007fe97c,0,wol_source_file_007fd62c,0x29c3));
                HWND window=Unknown_00735B10();
                if(window) {
                    if(GetDlgItem(window,0x53f)) Unknown_005C2C80(unknown_color_7a7360,notice);
                    if(GetDlgItem(window,0x5b5)) Unknown_005c2d10(unknown_color_7a7360,notice);
                }
                if(unknown_session_a3d298.Unknown_00672F10()) {
                    int color=Gadget_Color_Scheme();
                    unknown_session_a3d298.Messages.Add_Message(0,0,notice,
                    color,0x16,(int)(Rules->MessageDelay*900.0),false);
                }
                result=2;
            }
        } else {
            int color=Gadget_Color_Scheme();
            unknown_session_a3d298.Messages.Add_Message(0,0,
            StringTable::Load_String(wol_reply_observer_label_007fe964,0,wol_source_file_007fd62c,0x29db),
            color,0x16,(int)(Rules->MessageDelay*900.0),false);
            result=2;
        }
        operator delete(copy);
    }
    return result;
}


extern HANDLE wol_wait_events_b264f8[18];
int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
void __stdcall unknown_dialog_5ecf40(HWND,int);
namespace OwnerDraw {
void __fastcall unknown_5e6390(HWND);
void __fastcall unknown_5e5150(HWND,bool);
void __fastcall unknown_draw_dialog_back_5fee70(HWND);
}
void __fastcall unknown_5159b0(int,bool);
void __fastcall Unknown_005CD3A0();
void __fastcall Unknown_005FE380(long);
WideString __fastcall Limit_Name_Text_005ccf50(HWND);
LRESULT __fastcall unknown_get_window_wide_text(HWND,WideString &);
int __fastcall Unknown_0073cb80(WideString const &);
int __fastcall Unknown_0073ca50(WideString const &);
int __fastcall Parse_Page_Command_00746480(wchar_t const *);
int __fastcall Reply_To_Last_Pager_007466c0(wchar_t const *);
int __fastcall Parse_Online_Command_00746240(wchar_t const *);
void __fastcall Refresh_Lobby_List_0073ee80();
void __fastcall Refresh_Game_List_0073f8f0();
void __fastcall Refresh_Wonline_Players_0073fee0(int,bool);
WideString __fastcall Game_List_Row_Text_0073f5f0(HWND,int);
WideString __fastcall User_List_Row_Text_0073f850(HWND,int);
bool __fastcall Request_Chat_Channel_List_0073dd20(bool wait);
bool __fastcall Request_Game_Channel_List_0073df10(unsigned char autoping,bool wait);
bool __fastcall Can_Join_Channel_00755cd0(WOL::Channel const *,bool,bool);
bool __fastcall Join_Game_Channel_00745780(WOL::Channel const *,bool);
bool __fastcall Host_Game_007459a0();
void __fastcall View_HTML_0073c580(char const *,int);
HWND __fastcall unknown_create_dialog_5ff610(int,int (__stdcall *)(HWND,UINT,WPARAM,LPARAM),int);
int __fastcall Dialog_Loop_007359e0(HWND,bool (*)(),int,bool,bool);
bool connection_wait_callback_0073c120();
int __stdcall Wonline_Options_Dialog_00749b50(HWND,UINT,WPARAM,LPARAM);
extern "C" wchar_t *__cdecl wcsncpy(wchar_t *,wchar_t const *,unsigned);
extern "C" int __cdecl wcsncmp(wchar_t const *,wchar_t const *,unsigned);
extern "C" long __cdecl atol(char const *);
extern wchar_t wol_empty_text_839d4c[];
extern int unknown_color_7a7368,unknown_color_7a736c,unknown_color_7a7370,unknown_color_7a737c;
extern int wol_lan_leave_b2abf0;
extern int unknown_b2abec;
extern int unknown_a3d2a4,unknown_online_option_a3d2a8;
extern unsigned int game_type_column_b2a6f8,game_private_column_b2a6f4,game_observer_column_b299a0,game_name_column_b2a8a8,game_ping_column_b292a8,lobby_title_column_b29acc;
extern int player_list_column_00b2657c,player_list_column_00b2a81c,player_list_column_00b292a4;
extern int wol_pending_allow_page_b292e4,wol_pending_allow_find_b294f0,wol_pending_lang_filter_b29810,wol_pending_lob_music_b297cc;
extern int wol_pending_show_all_b2a644,wol_pending_display_asian_b29730,wol_pending_display_latin_b29978;
extern int last_game_data_7fd378,last_game_data_7fd37c;
extern char last_game_name_b2ac84[];
extern bool game_list_refreshing_b2ac80;
extern char wol_source_file_007fd62c[];
extern char wol_label_7fe9fc[],wol_label_7fe9e8[],wol_label_7dfc7c[],wol_label_7fe9d4[],wol_label_7dfc64[],wol_label_7dfc48[];
extern char wol_label_7dfc14[],wol_label_7dfbfc[],wol_label_7fe998[],wol_label_7fd358[],wol_label_7e1a60[];
extern wchar_t wol_label_7fe2c4[],wol_label_7fe9b8[],wol_label_7fe9a4[],wol_label_7d64f4[];
extern char wol_label_7eb2e4[],wol_label_7eb2d8[],wol_label_7eb2cc[],wol_label_7eb2c0[],wol_label_7fd564[];
void __fastcall Apply_WOL_Settings_0073c990();
inline void Refresh_Game_Channels()
{
    if(!Request_Game_Channel_List_0073df10(1,false)) SetEvent(wol_wait_events_b264f8[5]);
}
inline void Refresh_Chat_Channels()
{
    if(!Request_Chat_Channel_List_0073dd20(false)) SetEvent(wol_wait_events_b264f8[5]);
}
inline void Update_Game_Channels(HWND window)
{
    if(Unknown_00735B10()==window) Refresh_Game_Channels();
}
inline void Update_Chat_Channels(HWND window)
{
    if(Unknown_00735B10()==window) Refresh_Chat_Channels();
}
inline bool Request_Channel_Lists()
{
    return Request_Chat_Channel_List_0073dd20(true) && Request_Game_Channel_List_0073df10(1,false);
}
inline void Refresh_Channel_Lists()
{
    if(!Request_Channel_Lists()) SetEvent(wol_wait_events_b264f8[5]);
}
inline void On_Lobby_Timer(HWND window,WPARAM timer)
{
    switch(timer) {
    case 0x55d:
        KillTimer(window,0x55d);
        EnableWindow(GetDlgItem(window,0x55d),1);
        break;
    case 1:
        Update_Chat_Channels(window);
        break;
    case 0:
        Update_Game_Channels(window);
        break;
    }
}
inline void On_Refresh_Lists(HWND window)
{
    EnableWindow(GetDlgItem(window,0x53d),0);
    Refresh_Channel_Lists();
    EnableWindow(GetDlgItem(window,0x53d),1);
}
int __stdcall Wonline_Main_Dialog_007469c0(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    int handled=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(handled) return handled;
    switch(message) {
    case 2:
        KillTimer(window,0);
        KillTimer(window,0x55d);
        Unknown_005CD3A0();
        return 0;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0xf:
        OwnerDraw::unknown_draw_dialog_back_5fee70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
        return 0;
    case 7:
        SetFocus(GetDlgItem(window,0x540));
        break;
    case 0x14:
        return 1;
    case 0x18:
        return 0;
    case 0x111:
        switch((int)(wparam&0xffff)) {
        case 0x53d: {
            int selection=SendMessageA((HWND)lparam,0x188,0,0);
            if(selection==-1) return 0;
            int data=SendMessageA((HWND)lparam,0x199,selection,0);
            if((unsigned short)(wparam>>16)==1 && data!=-1) {
                WOL::Channel lobby;
                if(unknown_lobbies_b26c88.Get(lobby,data) && strcmp(lobby.name,unknown_channel_b299c0.name) && strcmp(lobby.name,unknown_pending_channel_b26a60.name)) {
                    wol_lan_leave_b2abf0=1;
                    unknown_b2abec=atol(lobby.name+strlen(wol_label_7fd358));
                    SetEvent(wol_wait_events_b264f8[9]);
                }
            }
            return 0;
        }
        case 0x53e:
            return 0;
        case 0x3ed: {
            HWND input=GetDlgItem(window,0x540);
            WideString text=Limit_Name_Text_005ccf50(input);
            SendMessageA(input,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
            SetFocus(input);
            InvalidateRect(input,0,0);
            int color=unknown_color_7a7370;
            if(strlen(unknown_channel_b299c0.name)==0)
                Unknown_005C2C80(-1,StringTable::Load_String(wol_label_7fe9fc,0,wol_source_file_007fd62c,0x2c52));
            else if(text.Length()>0) {
                if(Unknown_0073cb80(text)) color=unknown_color_7a736c;
                Unknown_005C2C80(color,wol_label_7fe2c4,unknown_wstring_b26cb8.Buffer,text.Get());
            } else Unknown_005C2C80(-1,StringTable::Load_String(wol_label_7fe9e8,0,wol_source_file_007fd62c,0x2c5a));
            return 0;
        }
        case 0x540: {
            wchar_t nickname[16],primary_command[64],alternate_command[64];
            HWND input=GetDlgItem(window,0x540);
            WideString text=Limit_Name_Text_005ccf50(input);
            if((unsigned short)(wparam>>16)==0x501) {
                SendMessageA(input,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
                if(text.Length()>0) {
                    int parsed=Parse_Page_Command_00746480(text.Get());
                    if(parsed!=0) {
                        if(parsed==1) {
                            int count=0;
                            unsigned length=text.Length()+1;
                            wchar_t *copy=(wchar_t *)operator new(length*2);
                            wcscpy(primary_command,StringTable::Load_String(wol_label_7dfc7c,0,wol_source_file_007fd62c,0x2bd9));
                            if(!copy) return 0;
                            copy=wcsncpy(copy,text.Get(),length);
                            if(!copy) return 0;
                            wchar_t *body=copy+wcslen(primary_command);
                            while(*body && wcsncmp(body,wol_label_7d64f4,1)==0) ++body;
                            while(*body) {
                                if(wcsncmp(body,wol_label_7d64f4,1)==0 || count>10) break;
                                nickname[count++]=*body++;
                            }
                            nickname[count]=0;
                            while(*body && wcsncmp(body,wol_label_7d64f4,1)==0) ++body;
                            if(count>10) Unknown_005C2C80(-1,StringTable::Load_String(wol_label_7fe9d4,0,wol_source_file_007fd62c,0x2bf7));
                            else if(count==0) Unknown_005C2C80(-1,StringTable::Load_String(wol_label_7dfc64,0,wol_source_file_007fd62c,0x2bf9));
                            else if(wcslen(body)>0) Unknown_005C2C80(unknown_color_7a7368,wol_label_7fe9b8,unknown_wstring_b26cb8.Buffer,nickname,body);
                            else Unknown_005C2C80(-1,StringTable::Load_String(wol_label_7dfc48,0,wol_source_file_007fd62c,0x2bff));
                            operator delete(copy);
                        }
                    } else if(Reply_To_Last_Pager_007466c0(text.Get())) {
                        unsigned length=text.Length()+1;
                        wchar_t *copy=(wchar_t *)operator new(length*2);
                        wcscpy(primary_command,StringTable::Load_String(wol_label_7dfc14,0,wol_source_file_007fd62c,0x2c11));
                        wcscpy(alternate_command,StringTable::Load_String(wol_label_7dfbfc,0,wol_source_file_007fd62c,0x2c12));
                        if(!copy) return 0;
                        copy=wcsncpy(copy,text.Get(),length);
                        if(!copy) return 0;
                        wchar_t *body=copy+(wcsncmp(copy,primary_command,wcslen(primary_command))==0 ? wcslen(primary_command):wcslen(alternate_command));
                        while(*body && wcsncmp(body,wol_label_7d64f4,1)==0) ++body;
                        wcscpy(nickname,Narrow_To_Wide_006fa180(unknown_last_pager_b2ac48));
                        if(wcslen(nickname)>0 && wcslen(body)>0)
                            Unknown_005C2C80(unknown_color_7a7368,wol_label_7fe9b8,unknown_wstring_b26cb8.Buffer,nickname,body);
                        else if(wcslen(nickname)>0)
                            Unknown_005C2C80(-1,StringTable::Load_String(wol_label_7dfc48,0,wol_source_file_007fd62c,0x2c2f));
                        operator delete(copy);
                    } else if(!Parse_Online_Command_00746240(text.Get())) {
                        if(strlen(unknown_channel_b299c0.name)==0) {
                            Unknown_005C2C80(-1,StringTable::Load_String(wol_label_7fe9fc,0,wol_source_file_007fd62c,0x2c38));
                            return 0;
                        }
                        int color=unknown_color_7a737c;
                        if(Unknown_0073ca50(text)==1) color=unknown_color_7a7368;
                        Unknown_005C2C80(color,wol_label_7fe9a4,unknown_wstring_b26cb8.Buffer,text.Get());
                    }
                }
            }
            return 0;
        }
        case 0x55d:
            EnableWindow((HWND)lparam,0);
            SetTimer(window,0x55d,3000,0);
            Refresh_Game_Channels();
            break;
        case 0x558:
            View_HTML_0073c580(wol_label_7fd564,0);
            SetFocus(GetDlgItem(window,0x540));
            break;
        case 0x608: {
            WideString text;
            HWND input=GetDlgItem(window,0x540);
            unknown_get_window_wide_text(input,text);
            SendMessageA(input,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
            SetFocus(input);
            if(unknown_squad_id_b2abfc) {
                WOL::User user;
                memset(&user,0,sizeof(user));
                strcpy(user.name,wol_label_7e1a60);
                unknown_chat_a72cfc->unknown_slot59(&user,text.Get());
            } else Unknown_005C2C80(-1,StringTable::Load_String(wol_label_7fe998,0,wol_source_file_007fd62c,0x2c74));
            return 0;
        }
        case 0x62d: {
            HWND list=GetDlgItem(window,0x6ee);
            int selection=SendMessageA(list,0x188,0,0);
            bool enable=false;
            int data=-1;
            if(selection!=-1) {
                data=SendMessageA(list,0x199,selection,0);
                if(data>=0 && data<unknown_games_b29990.Count) {
                    WOL::Channel game;
                    memset(&game,0,sizeof(game));
                    game=unknown_games_b29990.Items[data];
                    enable=Can_Join_Channel_00755cd0(&game,data!=last_game_data_7fd378,unknown_a72c68!=0);
                    if(enable) {
                        EnableWindow(GetDlgItem(window,0x62b),0);
                        EnableWindow(GetDlgItem(window,0x62d),0);
                        SendMessageA(GetDlgItem(window,0x6ee),0x186,-1,0);
                        EnableWindow(GetDlgItem(window,0x689),0);
                        EnableWindow(GetDlgItem(window,0x558),0);
                        EnableWindow(GetDlgItem(window,0x61f),0);
                        EnableWindow(GetDlgItem(window,0x686),0);
                        bool joined=Join_Game_Channel_00745780(&game,unknown_a72c68==1);
                        EnableWindow(GetDlgItem(window,0x62b),1);
                        EnableWindow(GetDlgItem(window,0x689),1);
                        EnableWindow(GetDlgItem(window,0x558),1);
                        EnableWindow(GetDlgItem(window,0x61f),1);
                        EnableWindow(GetDlgItem(window,0x686),1);
                        if(joined) {
                            OwnerDraw::unknown_5e5150(window,false);
                            unknown_5159b0(0,true);
                            ShowWindow(window,0);
                            unknown_dialog_5ecf40(window,1);
                        }
                    }
                }
            }
            last_game_data_7fd378=data;
            EnableWindow((HWND)lparam,enable);
            SetFocus(GetDlgItem(window,0x540));
            break;
        }
        case 0x62b: {
            EnableWindow((HWND)lparam,0);
            EnableWindow(GetDlgItem(window,0x62d),0);
            SendMessageA(GetDlgItem(window,0x6ee),0x186,-1,0);
            EnableWindow(GetDlgItem(window,0x689),0);
            EnableWindow(GetDlgItem(window,0x558),0);
            EnableWindow(GetDlgItem(window,0x61f),0);
            EnableWindow(GetDlgItem(window,0x686),0);
            bool hosted=Host_Game_007459a0();
            EnableWindow((HWND)lparam,1);
            EnableWindow(GetDlgItem(window,0x689),1);
            EnableWindow(GetDlgItem(window,0x558),1);
            EnableWindow(GetDlgItem(window,0x61f),1);
            EnableWindow(GetDlgItem(window,0x686),1);
            if(!hosted) SetFocus(GetDlgItem(window,0x540));
            break;
        }
        case 0x61f: {
            OwnerDraw::unknown_5e5150(window,false);
            unknown_5159b0(0,true);
            HWND dialog=unknown_create_dialog_5ff610(700,Wonline_Options_Dialog_00749b50,0);
            ShowWindow(dialog,1);
            int result=Dialog_Loop_007359e0(dialog,connection_wait_callback_0073c120,0,true,true);
            InvalidateRect(window,0,0);
            ShowWindow(window,5);
            unknown_dialog_5ecf40(window,0);
            if(result==2) Apply_WOL_Settings_0073c990();
            else {
                unknown_wol_allow_page_b290a0=wol_pending_allow_page_b292e4;
                unknown_wol_allow_find_b2a818=wol_pending_allow_find_b294f0;
                unknown_wol_lang_filter_b2a7e4=wol_pending_lang_filter_b29810;
                unknown_wol_lob_music_b297c8=wol_pending_lob_music_b297cc;
                unknown_wol_show_all_b2a634=wol_pending_show_all_b2a644;
                unknown_wol_display_asian_b2a804=wol_pending_display_asian_b29730;
                unknown_wol_display_latin_b2a7e8=wol_pending_display_latin_b29978;
                Unknown_WOL_Save_Settings_0073C890();
                Apply_WOL_Settings_0073c990();
            }
            SetFocus(GetDlgItem(window,0x540));
            return 0;
        }
        case 0x547: case 0x5c0: case 0x686:
            OwnerDraw::unknown_5e6390(window);
            SetEvent(wol_wait_events_b264f8[6]);
            return 0;
        case 0x689:
            unknown_a3d2a4=4;
            unknown_online_option_a3d2a8=3;
            OwnerDraw::unknown_5e6390(window);
            SetEvent(wol_wait_events_b264f8[6]);
            return 0;
        case 0x6ee: {
            bool activate=false;
            switch(wparam>>16) {
            case 1: break;
            case 2: activate=true; break;
            default: return 0;
            }
            int selection=SendMessageA((HWND)lparam,0x188,0,0);
            int data=-1;
            bool enable=false;
            if(selection!=-1) {
                data=SendMessageA((HWND)lparam,0x199,selection,0);
                if(data>=0 && data<unknown_games_b29990.Count) {
                    WOL::Channel game;
                    memset(&game,0,sizeof(game));
                    game=unknown_games_b29990.Items[data];
                    if(last_game_data_7fd37c==-1) last_game_name_b2ac84[0]=0;
                    int changed=strcmp(last_game_name_b2ac84,game.name);
                    enable=Can_Join_Channel_00755cd0(&game,activate || changed,unknown_a72c68!=0);
                    strcpy(last_game_name_b2ac84,game.name);
                    if(enable && activate) {
                        EnableWindow(GetDlgItem(window,0x62b),0);
                        EnableWindow(GetDlgItem(window,0x62d),0);
                        SendMessageA(GetDlgItem(window,0x6ee),0x186,-1,0);
                        EnableWindow(GetDlgItem(window,0x689),0);
                        EnableWindow(GetDlgItem(window,0x558),0);
                        EnableWindow(GetDlgItem(window,0x61f),0);
                        EnableWindow(GetDlgItem(window,0x686),0);
                        if(Join_Game_Channel_00745780(&game,unknown_a72c68==1)) {
                            OwnerDraw::unknown_5e5150(window,false);
                            unknown_5159b0(0,true);
                            ShowWindow(window,0);
                            unknown_dialog_5ecf40(window,1);
                        }
                        EnableWindow(GetDlgItem(window,0x62b),1);
                        EnableWindow(GetDlgItem(window,0x689),1);
                        EnableWindow(GetDlgItem(window,0x558),1);
                        EnableWindow(GetDlgItem(window,0x61f),1);
                        EnableWindow(GetDlgItem(window,0x686),1);
                    }
                } else last_game_name_b2ac84[0]=0;
            }
            int current=SendMessageA((HWND)lparam,0x188,0,0);
            if(!game_list_refreshing_b2ac80 || current!=-1) last_game_data_7fd37c=data;
            if(game_list_refreshing_b2ac80 && current==-1) game_list_refreshing_b2ac80=false;
            HWND join=GetDlgItem(window,0x62d);
            EnableWindow(join,enable);
            return 0;
        }
        case 0x75f: case 0x760: {
            int checked=0;
            HWND button=GetDlgItem(window,0x760);
            if(button) checked=SendMessageA(button,0xf0,0,0);
            if((unsigned short)wparam==0x75f) checked=!checked;
            unknown_a72c68=checked;
            Unknown_005cd540(0);
            return 0;
        }
        }
        return 0;
    case 0x497: {
        Unknown_005cd540(window);
        SendDlgItemMessageA(window,0x3ed,0x49c,0,(LPARAM)unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_label_7eb2e4,0));
        SendDlgItemMessageA(window,0x3ed,0x4aa,0,(LPARAM)unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_label_7eb2d8,0));
        SendDlgItemMessageA(window,0x55d,0x49c,0,(LPARAM)unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_label_7eb2cc,0));
        SendDlgItemMessageA(window,0x55d,0x4aa,0,(LPARAM)unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_label_7eb2c0,0));
        game_type_column_b2a6f8=2;
        game_private_column_b2a6f4=0x13;
        game_observer_column_b299a0=0x24;
        game_name_column_b2a8a8=0x35;
        game_ping_column_b292a8=200;
        SendDlgItemMessageA(window,0x6ee,0x4a6,game_ping_column_b292a8-game_name_column_b2a8a8,game_name_column_b2a8a8);
        SendDlgItemMessageA(window,0x6ee,0x4a6,0,game_type_column_b2a6f8);
        SendDlgItemMessageA(window,0x6ee,0x4a6,0,game_private_column_b2a6f4);
        SendDlgItemMessageA(window,0x6ee,0x4a6,0,game_ping_column_b292a8);
        SendDlgItemMessageA(window,0x6ee,0x4a6,0,game_observer_column_b299a0);
        lobby_title_column_b29acc=2;
        SendDlgItemMessageA(window,0x53d,0x4a6,0,lobby_title_column_b29acc);
        player_list_column_00b2657c=2;
        player_list_column_00b2a81c=0x12;
        player_list_column_00b292a4=0x73;
        SendDlgItemMessageA(window,0x53e,0x4a6,player_list_column_00b292a4-player_list_column_00b2a81c,player_list_column_00b2a81c);
        SendDlgItemMessageA(window,0x53e,0x4a6,0,player_list_column_00b2657c);
        SendDlgItemMessageA(window,0x53e,0x4a6,0,player_list_column_00b292a4);
        Refresh_Lobby_List_0073ee80();
        Refresh_Game_List_0073f8f0();
        Refresh_Wonline_Players_0073fee0(0,false);
        SetFocus(GetDlgItem(window,0x540));
        SetTimer(window,0,30000,0);
        SetTimer(window,1,53000,0);
        return 0;
    }
    case 0x113:
        On_Lobby_Timer(window,wparam);
        break;
    case 0x4e9: {
        OwnerDraw::UnknownTextRequest *request=(OwnerDraw::UnknownTextRequest *)lparam;
        if(request) {
            switch(GetDlgCtrlID((HWND)request->unknown_00)) {
            case 0x6ee:
                request->text=Game_List_Row_Text_0073f5f0((HWND)request->unknown_00,request->unknown_04);
                break;
            case 0x53e:
                request->text=User_List_Row_Text_0073f850((HWND)request->unknown_00,request->unknown_04);
                break;
            }
        }
        return 0;
    }
    case 0x593:
        On_Refresh_Lists(window);
        break;
    case 0x4ed:
        ShowWindow(window,0);
        unknown_dialog_5ecf40(window,1);
        return 0;
    }
    return 0;
}


LRESULT __fastcall unknown_get_window_text(HWND,Wstring &);
LRESULT __fastcall unknown_get_window_wide_text(HWND,WideString &);
WideString __fastcall Limit_Name_Text_005ccf50(HWND);
int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
bool __fastcall Unknown_005def80(long,bool);
extern int unknown_color_7a737c;
extern unsigned short wol_empty_text_839d4c[];
extern unsigned short wol_page_echo_007fe9a4[];
extern char wol_name_error_007e6c90[],wol_enter_message_007fe9e8[],wol_no_squad_007fe998[];
extern char wol_reply_observer_label_007fe964[],wol_source_file_007fd62c[],wol_squad_user_007e1a60[];
long __stdcall Find_Page_Dialog_007485d0(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    long result=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(result) return result;
    switch(message) {
    case 0x111:
        switch((int)(wparam&0xffff)) {
        case 2:
            Unknown_007357E0(window,2);
            return 0;
        case 0x5c1: {
            Wstring name;
            unknown_get_window_text((HWND)lparam,name);
            WideString text;
            unknown_get_window_wide_text(GetDlgItem(window,0x540),text);
            BOOL enabled=(name.Length()>0 && text.Length()>0)?1:0;
            EnableWindow(GetDlgItem(window,0x5c2),enabled);
            return 0;
        }
        case 0x5c2: {
            HWND edit=GetDlgItem(window,0x540);
            WideString text=Limit_Name_Text_005ccf50(edit);
            SendMessageA(edit,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
            EnableWindow(GetDlgItem(window,0x5c2),false);
            SetFocus(edit);
            if(!unknown_chat_a72cfc) return 0;
            Wstring name;
            unknown_get_window_text(GetDlgItem(window,0x5c1),name);
            name.Truncate(9);
            WOL::User user;
            strcpy(user.name,name.Get());
            if(text.Length()>0 && strlen(user.name)>0) {
                bool recipient_is_player=false;
                if(unknown_int_a3d548) {
                    for(int i=0;i<unknown_players_a3fa7c.Count();++i) {
                        NodeNameType *player=unknown_players_a3fa7c[i];
                        if(_stricmp(Wide_To_Narrow_006fa120(player->Name),user.name)==0) {
                            recipient_is_player=true;
                            break;
                        }
                    }
                }
                if(!recipient_is_player) {
                    unknown_chat_a72cfc->unknown_slot62(&user,text.Get());
                    Unknown_005c2d10(unknown_color_7a737c,wol_page_echo_007fe9a4,unknown_wstring_b26cb8.Buffer,text.Get());
                } else {
                    Unknown_005c2d10(-1,StringTable::Load_String(wol_reply_observer_label_007fe964,0,wol_source_file_007fd62c,0x30ed));
                }
            } else if(strlen(user.name)==0) {
                Unknown_005c2d10(-1,StringTable::Load_String(wol_name_error_007e6c90,0,wol_source_file_007fd62c,0x30f9));
            } else {
                Unknown_005c2d10(-1,StringTable::Load_String(wol_enter_message_007fe9e8,0,wol_source_file_007fd62c,0x30fb));
            }
            return 0;
        }
        case 0x5c3:
            if(unknown_chat_a72cfc) {
                char name[64];
                memset(name,0,sizeof(name));
                SendDlgItemMessageA(window,0x5c1,0x4b5,60,(LPARAM)name);
                name[9]=0;
                WOL::User user;
                strcpy(user.name,name);
                if(strlen(user.name)>0) unknown_chat_a72cfc->unknown_slot24(&user);
                else Unknown_005c2d10(-1,StringTable::Load_String(wol_name_error_007e6c90,0,wol_source_file_007fd62c,0x30bb));
            }
            break;
        case 0x607:
            if(unknown_chat_a72cfc) {
                if(unknown_squad_id_b2abfc==0) {
                    Unknown_005c2d10(-1,StringTable::Load_String(wol_no_squad_007fe998,0,wol_source_file_007fd62c,0x3107));
                    break;
                } else {
                    WideString text;
                    HWND edit=GetDlgItem(window,0x540);
                    unknown_get_window_wide_text(edit,text);
                    SendMessageA(edit,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
                    SetFocus(edit);
                    WOL::User user;
                    strcpy(user.name,wol_squad_user_007e1a60);
                    if(text.Length()>0) unknown_chat_a72cfc->unknown_slot62(&user,text.Get());
                    else Unknown_005c2d10(-1,StringTable::Load_String(wol_enter_message_007fe9e8,0,wol_source_file_007fd62c,0x311a));
                }
            }
            break;
        }
        break;
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
        return 0;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0x497: {
        SendDlgItemMessageA(window,0x5c1,0xc5,9,0);
        HWND name_edit=GetDlgItem(window,0x5c1);
        SendMessageA(name_edit,0x4d1,1,0);
        name_edit=GetDlgItem(window,0x5c1);
        SendMessageA(name_edit,0x4b4,0,(LPARAM)unknown_string_83c55c);
        if(unknown_timer_b292e8.Value()>0) {
            name_edit=GetDlgItem(window,0x5c1);
            SendMessageA(name_edit,0x4b4,0,(LPARAM)unknown_last_pager_b2ac48);
            SetFocus(GetDlgItem(window,0x540));
            unknown_timer_b292e8=CDTimerClass<SystemTimerClass>(0);
        }
        break;
    }
    case 0x4da:
        PostMessageA(GetDlgItem(window,0x5c2),0xf5,0,0);
        break;
    case 0x4db:
        Unknown_005def80(lparam,wparam!=0);
        return 0;
    case 0x540: {
        WideString text=Limit_Name_Text_005ccf50((HWND)lparam);
        Wstring name;
        unknown_get_window_text(GetDlgItem(window,0x5c1),name);
        BOOL enabled=(text.Length()>0 && name.Length()>0)?1:0;
        EnableWindow(GetDlgItem(window,0x5c2),enabled);
        return 0;
    }
    }
    return 0;
}


int __fastcall unknown_dialog_proc_5ffb00(HWND window,UINT message,WPARAM wparam,LPARAM lparam);
void __fastcall Wait_Paint_005e5f20(HWND window);
extern bool connection_wait_callback_0073c120();
extern char wol_source_file_007fd62c[];
extern char wol_download_failed_label_7fea0c[];
long __stdcall Wonline_Download_Dialog_00748cd0(HWND window,unsigned message,unsigned wparam,long lparam)
{
    if(message==0x110 && lparam) SetWindowLongA(window,8,*(long *)(lparam+4));
    int handled=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(handled) return handled;
    switch(message) {
    case 0x111:
        switch(wparam & 0xffff) {
        case 2:
            unknown_download_a72d04->Abort();
            Unknown_007357E0(window,2);
            break;
        default: return 0;
        }
        break;
    case 0x110: break;
    case 0x5f4:
        Unknown_007357E0(window,1);
        break;
    case 0x5f5:
        Message_Box_005c3230(StringTable::Load_String(wol_download_failed_label_7fea0c,0,wol_source_file_007fd62c,0x32f3),0,connection_wait_callback_0073c120,false);
        unknown_download_a72d04->Abort();
        Unknown_007357E0(window,2);
        break;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0x14: return 1;
    case 0xf:
        Unknown_005FEE70(window);
        Wait_Paint_005e5f20(window);
        ValidateRect(window,0);
        return 0;
    default: return 0;
    }
    return 1;
}


int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
LRESULT __fastcall unknown_get_window_text(HWND,Wstring &);
extern "C" __declspec(dllimport) UINT __stdcall IsDlgButtonChecked(HWND,int);
extern "C" __declspec(dllimport) BOOL __stdcall CheckDlgButton(HWND,int,UINT);
extern int wol_max_players_b2abdc;
extern char wol_empty_name_83c55c[],wol_password_forbidden_7fea20[];
long __stdcall Wonline_Create_Game_Dialog_00748e10(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    if(message==0x110 && lparam) SetWindowLongA(window,8,*(long *)(lparam+4));
    long result=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(result) return result;
    CreateGameDialogControl *control=(CreateGameDialogControl *)GetWindowLongA(window,8);
    switch(message) {
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
        return 0;
    case 0x14: return 1;
    case 0x2b: Unknown_005FE380(lparam); return 1;
    case 0x111:
        switch((int)(wparam&0xffff)) {
        case 1: {
            wol_max_players_b2abdc=5;
            Wstring password;
            unknown_get_window_text(GetDlgItem(window,0x72c),password);
            control->Text=password;
            control->Set_Window_006010f0((unsigned)window,(HWND)1);
            return 0;
        }
        case 2:
            control->Set_Window_006010f0((unsigned)window,(HWND)2);
            return 0;
        case 0x629:
            if(SendDlgItemMessageA(window,0x629,0xf0,0,0)==1) {
                SendDlgItemMessageA(window,0x66c,0xf1,0,0);
                control->unknown_11=false;
                control->unknown_10=true;
            } else control->unknown_10=false;
            return 0;
        case 0x66c:
            if(SendDlgItemMessageA(window,0x66c,0xf0,0,0)==1) {
                SendDlgItemMessageA(window,0x629,0xf1,0,0);
                control->unknown_11=true;
                control->unknown_10=false;
            } else control->unknown_11=false;
            return 0;
        case 0x6df:
            if(IsDlgButtonChecked(window,0x6e0)) CheckDlgButton(window,0x6e0,0);
            else CheckDlgButton(window,0x6df,1);
            return 0;
        case 0x72c:
            if(lparam) {
                Wstring password;
                unknown_get_window_text((HWND)lparam,password);
                EnableWindow(GetDlgItem(window,1),!password.Contains_Any(wol_password_forbidden_7fea20));
            }
            return 0;
        case 0x6e0:
            if(IsDlgButtonChecked(window,0x6df)) CheckDlgButton(window,0x6df,0);
            else CheckDlgButton(window,0x6e0,1);
            return 0;
        case 0x71e:
            control->unknown_12=SendMessageA((HWND)lparam,0xf0,0,0)==1;
            return 0;
        case 0x71f:
            control->unknown_13=SendMessageA((HWND)lparam,0xf0,0,0)==1;
            return 0;
        }
        break;
    case 0x497: {
        HWND password=GetDlgItem(window,0x72c);
        SendMessageA(password,0x4d1,1,0);
        password=GetDlgItem(window,0x72c);
        SendMessageA(password,0x4b4,0,(LPARAM)wol_empty_name_83c55c);
        SendDlgItemMessageA(window,0x72c,0xc5,8,0);
        SetFocus(GetDlgItem(window,0x72c));
        CheckDlgButton(window,0x629,control->unknown_10!=false);
        CheckDlgButton(window,0x66c,control->unknown_11!=false);
        if((unknown_game_mode_a3d298!=4 || unknown_a3d2a4!=2) && (int)unknown_squad_id_b2abfc<1)
            EnableWindow(GetDlgItem(window,0x66c),0);
        HWND resolution=GetDlgItem(window,0x71e);
        bool high_resolution=unknown_8523e0>=1024;
        EnableWindow(resolution,!high_resolution);
        SendMessageA(resolution,0xf1,control->unknown_12 && !high_resolution ? 1:0,0);
        HWND unknown_option=GetDlgItem(window,0x71f);
        SendMessageA(unknown_option,0xf1,control->unknown_13!=false,0);
        break;
    }
    case 0x4da:
        PostMessageA(GetDlgItem(window,1),0xf5,0,0);
        return 0;
    }
    return 0;
}


int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
LRESULT __fastcall unknown_get_window_text(HWND,Wstring &);
extern char password_invalid_characters_7fea20[];
long __stdcall Wonline_Password_Dialog_007492d0(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    if(message==0x110 && lparam) SetWindowLongA(window,8,*(long *)(lparam+4));
    int handled=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(handled) return handled;
    WonlineStringDialogControl *control=(WonlineStringDialogControl *)GetWindowLongA(window,8);
    switch(message) {
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
        return 0;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0x111:
        switch(wparam&0xffff) {
        case 1: {
            Wstring password;
            unknown_get_window_text(GetDlgItem(window,0x72c),password);
            control->Text=password;
            control->Set_Window_006010f0((unsigned)window,(HWND)1);
            return 0;
        }
        case 2:
            control->Set_Window_006010f0((unsigned)window,(HWND)2);
            return 0;
        case 0x72c:
            if(lparam) {
                Wstring password;
                unknown_get_window_text((HWND)lparam,password);
                EnableWindow(GetDlgItem(window,1),!password.Contains_Any(password_invalid_characters_7fea20));
            }
            return 0;
        }
        break;
    case 0x497: {
        HWND edit=GetDlgItem(window,0x72c);
        SendMessageA(edit,0x4d1,1,0);
        edit=GetDlgItem(window,0x72c);
        SendMessageA(edit,0x4b4,0,(LPARAM)unknown_string_83c55c);
        SendDlgItemMessageA(window,0x72c,0xc5,8,0);
        SetFocus(GetDlgItem(window,0x72c));
        return 0;
    }
    case 0x4da:
        PostMessageA(GetDlgItem(window,1),0xf5,0,0);
        return 0;
    }
    return 0;
}


void __fastcall unknown_dialog_5e5f20(HWND);
LRESULT __fastcall unknown_get_window_text(HWND,Wstring &);
extern "C" int __cdecl atoi(char const *);
extern Wstring unknown_wstring_b29ce4,unknown_wstring_b269a8,unknown_wstring_b2909c,unknown_wstring_b297c4;
extern int account_birth_year_b2a6d4,account_birth_day_b26930,account_birth_month_b26894,account_newsletter_b29098;
extern char wol_source_file_007fd62c[];
extern char birthdate_privacy_url_7feac0[];
extern char birthdate_january_7feab4[];
extern char birthdate_february_7feaa4[];
extern char birthdate_march_7fea98[];
extern char birthdate_april_7fea8c[];
extern char birthdate_may_7fea84[];
extern char birthdate_june_7fea78[];
extern char birthdate_july_7fea6c[];
extern char birthdate_august_7fea60[];
extern char birthdate_september_7fea50[];
extern char birthdate_october_7fea44[];
extern char birthdate_november_7fea34[];
extern char birthdate_december_7fea24[];
long __stdcall Birthdate_Dialog_007494d0(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    switch(message) {
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
        return 0;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0x111:
        switch(wparam&0xffff) {
        case 1: {
            unknown_wstring_b29ce4.Release();
            HWND edit=GetDlgItem(window,0x626);
            if(edit) unknown_get_window_text(edit,unknown_wstring_b29ce4);
            unknown_wstring_b269a8.Release();
            edit=GetDlgItem(window,0x62e);
            if(edit) {
                unknown_get_window_text(edit,unknown_wstring_b269a8);
                account_birth_month_b26894=atoi(unknown_wstring_b269a8.Get());
            } else account_birth_month_b26894=0;
            unknown_wstring_b2909c.Release();
            edit=GetDlgItem(window,0x62f);
            if(edit) {
                unknown_get_window_text(edit,unknown_wstring_b2909c);
                account_birth_day_b26930=atoi(unknown_wstring_b2909c.Get());
            }
            unknown_wstring_b297c4.Release();
            edit=GetDlgItem(window,0x630);
            if(edit) {
                unknown_get_window_text(edit,unknown_wstring_b297c4);
                account_birth_year_b2a6d4=atoi(unknown_wstring_b297c4.Get());
            }
            account_newsletter_b29098=0;
            edit=GetDlgItem(window,0x631);
            if(edit) account_newsletter_b29098=SendMessageA(edit,0xf0,0,0);
            Unknown_007357E0(window,1);
            return 0;
        }
        case 2:
            Unknown_007357E0(window,2);
            return 0;
        case 0x576:
            unknown_open_url_73c580(birthdate_privacy_url_7feac0,0);
            return 0;
        default:
            return 0;
        }
    case 0x497: {
        HWND edit=GetDlgItem(window,0x626);
        SendMessageA(edit,0x4d1,1,0);
        SendMessageA(GetDlgItem(window,0x62f),0xc5,2,0);
        SendMessageA(GetDlgItem(window,0x630),0xc5,4,0);
        SetFocus(GetDlgItem(window,0x62f));
        HWND month_list=GetDlgItem(window,0x62e);
        unknown_reset_combo_6007f0(month_list);
        SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::Load_String(birthdate_january_7feab4,0,wol_source_file_007fd62c,0x34bc));
        SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::Load_String(birthdate_february_7feaa4,0,wol_source_file_007fd62c,0x34bd));
        SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::Load_String(birthdate_march_7fea98,0,wol_source_file_007fd62c,0x34be));
        SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::Load_String(birthdate_april_7fea8c,0,wol_source_file_007fd62c,0x34bf));
        SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::Load_String(birthdate_may_7fea84,0,wol_source_file_007fd62c,0x34c0));
        SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::Load_String(birthdate_june_7fea78,0,wol_source_file_007fd62c,0x34c1));
        SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::Load_String(birthdate_july_7fea6c,0,wol_source_file_007fd62c,0x34c2));
        SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::Load_String(birthdate_august_7fea60,0,wol_source_file_007fd62c,0x34c3));
        SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::Load_String(birthdate_september_7fea50,0,wol_source_file_007fd62c,0x34c4));
        SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::Load_String(birthdate_october_7fea44,0,wol_source_file_007fd62c,0x34c5));
        SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::Load_String(birthdate_november_7fea34,0,wol_source_file_007fd62c,0x34c6));
        SendMessageA(month_list,0x4c2,0,(LPARAM)StringTable::Load_String(birthdate_december_7fea24,0,wol_source_file_007fd62c,0x34c7));
        SendDlgItemMessageA(window,0x62e,0x14e,0,0);
        SendMessageA(window,0x4a9,0,1);
        return 0;
    }
    case 0x4db:
        Unknown_005def80(lparam,wparam!=0);
        return 0;
    }
    return 0;
}


namespace OwnerDraw { int __fastcall unknown_5eab00(HWND control); }
void __fastcall unknown_dialog_5e5f20(HWND);
LRESULT __fastcall unknown_get_window_text(HWND,Wstring &);
extern Wstring account_name_b2a640,account_password_b2a71c,account_password_check_b2a724,unknown_account_detail_b2945c;
extern int unknown_account_checkbox_b29ad0;
extern unsigned int unknown_account_input_events_b2ac98;
long __stdcall Account_Details_Dialog_007498e0(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    switch(message) {
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
        return 0;
    case 0x2b: Unknown_005FE380(lparam); return 1;
    case 0x111:
        switch(wparam&0xffff) {
        case 1: {
            int kind=OwnerDraw::unknown_5eab00(window);
            account_name_b2a640.Release();
            HWND edit=GetDlgItem(window,0x628);
            if(edit) unknown_get_window_text(edit,account_name_b2a640);
            account_password_b2a71c.Release();
            edit=GetDlgItem(window,0x72c);
            if(edit) unknown_get_window_text(edit,account_password_b2a71c);
            account_password_check_b2a724.Release();
            edit=GetDlgItem(window,0x627);
            if(edit) unknown_get_window_text(edit,account_password_check_b2a724);
            if(kind==0xf3) {
                unknown_account_detail_b2945c.Release();
                edit=GetDlgItem(window,0x5e5);
                if(edit) unknown_get_window_text(edit,unknown_account_detail_b2945c);
            } else if(kind==0xf4) {
                unknown_account_checkbox_b29ad0=0;
                edit=GetDlgItem(window,0x66b);
                if(edit) unknown_account_checkbox_b29ad0=SendMessageA(edit,0xf0,0,0);
            }
            Unknown_007357E0(window,1);
            return 0;
        }
        case 2:
            Unknown_007357E0(window,2);
            return 0;
        }
        break;
    case 0x497: {
        HWND edit=GetDlgItem(window,0x628);
        SendMessageA(edit,0x4d1,1,0);
        edit=GetDlgItem(window,0x72c);
        SendMessageA(edit,0x4d1,1,0);
        edit=GetDlgItem(window,0x627);
        SendMessageA(edit,0x4d1,1,0);
        edit=GetDlgItem(window,0x5e5);
        SendMessageA(edit,0x4d1,1,0);
        SendMessageA(GetDlgItem(window,0x628),0xc5,9,0);
        SendMessageA(GetDlgItem(window,0x72c),0xc5,8,0);
        SendMessageA(GetDlgItem(window,0x627),0xc5,8,0);
        SetFocus(GetDlgItem(window,0x628));
        break;
    }
    case 0x4db:
        ++unknown_account_input_events_b2ac98;
        Unknown_005def80(lparam,wparam!=0);
        return 0;
    }
    return 0;
}


// int __stdcall Wonline_Options_Dialog_00749b50(HWND,UINT,WPARAM,LPARAM) (0x00749B50): the WOL options dialog. Twin: TibSun WOL_Options_Dialog_Proc; original 970 B + 6 NOP.
extern int wol_pending_allow_page_b292e4,wol_pending_allow_find_b294f0,wol_pending_lang_filter_b29810,wol_pending_lob_music_b297cc;
extern int wol_pending_show_all_b2a644,wol_pending_display_asian_b29730,wol_pending_display_latin_b29978;
extern void *unknown_options_b2aca0;
int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
void __fastcall unknown_5159b0(int,bool);
void __stdcall unknown_dialog_5ecf40(HWND,int);
int __stdcall Wonline_Options_Dialog_00749b50(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    long result=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(result) return result;
    switch(message) {
    case 2:
        unknown_flag_a40958=true;
        return 0;
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
        return 0;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0x111:
        switch((int)(wparam&0xffff)) {
        case 1: {
            wol_pending_allow_page_b292e4=0;
            HWND item=GetDlgItem(window,0x5f5);
            if(item) wol_pending_allow_page_b292e4=SendMessageA(item,0xf0,0,0);
            wol_pending_allow_find_b294f0=0;
            item=GetDlgItem(window,0x5f7);
            if(item) wol_pending_allow_find_b294f0=SendMessageA(item,0xf0,0,0);
            wol_pending_lang_filter_b29810=0;
            item=GetDlgItem(window,0x5f6);
            if(item) wol_pending_lang_filter_b29810=SendMessageA(item,0xf0,0,0);
            wol_pending_lob_music_b297cc=0;
            item=GetDlgItem(window,0x5f8);
            if(item) wol_pending_lob_music_b297cc=SendMessageA(item,0xf0,0,0);
            item=GetDlgItem(window,0x5f9);
            if(item) wol_pending_show_all_b2a644=SendMessageA(item,0xf0,0,0);
            wol_pending_display_asian_b29730=0;
            item=GetDlgItem(window,0x5fa);
            if(item) wol_pending_display_asian_b29730=SendMessageA(item,0xf0,0,0);
            wol_pending_display_latin_b29978=0;
            item=GetDlgItem(window,0x5fb);
            if(item) wol_pending_display_latin_b29978=SendMessageA(item,0xf0,0,0);
            operator delete(unknown_options_b2aca0);
            unknown_options_b2aca0=0;
            End_Dialog_007357e0(window,1);
            return 0;
        }
        case 2:
            operator delete(unknown_options_b2aca0);
            unknown_options_b2aca0=0;
            End_Dialog_007357e0(window,2);
            return 0;
        case 0x5f8:
            if((short)(wparam>>16)==1||(short)(wparam>>16)==0) {
                bool checked=SendMessageA((HWND)lparam,0xf0,0,0)==1;
                if(checked && !Theme.Still_Playing()) Restart_Lobby_Music();
                else if(checked) { if(Theme.What_Is_Playing()==Theme.From_Name("INTRO")) Restart_Lobby_Music(); }
                else if(!checked) Theme.Queue_Song(-3);
                break;
            }
            break;
        case 0x63a: {
            OwnerDraw::unknown_5e5150(window,false);
            unknown_5159b0(0,true);
            unknown_flag_a40958=false;
            SoundControlsDialog dialog;
            dialog.Run_00688CD0();
            ShowWindow(window,5);
            unknown_dialog_5ecf40(window,0);
            return 0;
        }
        }
        break;
    case 0x497:
        if(unknown_wol_allow_page_b290a0) SendDlgItemMessageA(window,0x5f5,0xf1,1,0);
        if(unknown_wol_allow_find_b2a818) SendDlgItemMessageA(window,0x5f7,0xf1,1,0);
        if(unknown_wol_lang_filter_b2a7e4) SendDlgItemMessageA(window,0x5f6,0xf1,1,0);
        if(unknown_wol_lob_music_b297c8) SendDlgItemMessageA(window,0x5f8,0xf1,1,0);
        if(unknown_wol_show_all_b2a634) SendDlgItemMessageA(window,0x5f9,0xf1,1,0);
        if(unknown_wol_display_asian_b2a804) SendDlgItemMessageA(window,0x5fa,0xf1,1,0);
        if(unknown_wol_display_latin_b2a7e8) SendDlgItemMessageA(window,0x5fb,0xf1,1,0);
        break;
    case 0x4ed:
        ShowWindow(window,0);
        unknown_dialog_5ecf40(window,1);
        return 0;
    }
    return 0;
}


bool __fastcall Unknown_005c9320(NodeNameType const *entry);
bool __fastcall Unknown_005c9370(NodeNameType const *entry);
void __fastcall Unknown_0073cff0(char *buffer);
extern char channel_mode_prefix_007feae4[],channel_limit_option_007feaec[],channel_command_eol_007d7cec[];
void __fastcall Update_Channel_Player_Limit_00749f20()
{
    int limit=Scenario_Slot_Limit_005c65a0(unknown_session_a3d2b4);
    if(unknown_a72c68==1) ++limit;
    int fixed_slots=0;
    int open_slots=0;
    int player_slots=0;
    int i=1;
    for(;i<limit;++i) {
        NodeNameType *entry=unknown_player_slots_a3fa98[i];
        if(entry!=unknown_open_entry_a72d2c && entry!=unknown_player_a72d44) {
            if(Unknown_005c9320(entry)) ++fixed_slots;
            else if(Unknown_005c9370(entry)) ++player_slots;
        } else ++open_slots;
    }
    for(;i<8;++i) {
        NodeNameType *entry=unknown_player_slots_a3fa98[i];
        if(entry==unknown_player_a72d44) ++open_slots;
        else if(Unknown_005c9320(entry)) ++fixed_slots;
    }
    if(unknown_chat_a72cfc) {
        int capacity=open_slots+fixed_slots+1;
        Wstring command=Wstring(channel_mode_prefix_007feae4)+Wstring(unknown_game_channel_b299c0.name)+Wstring(channel_limit_option_007feaec)+Wstring_From_Integer(capacity)+Wstring(channel_command_eol_007d7cec);
        unknown_chat_a72cfc->unknown_slot39(command.Get());
        char info[80];
        memset(info,0,sizeof(info));
        Unknown_0073cff0(info);
        info[40]=0;
        unknown_chat_a72cfc->unknown_slot42(info);
        unknown_chat_a72cfc->unknown_slot63(1,capacity+player_slots);
    }
}


extern "C" int __cdecl swprintf(wchar_t *,wchar_t const *,...);
extern wchar_t wol_player_tooltip_b290a4[];
extern char wol_source_file_007fd62c[];
extern char tooltip_label_7fd814[];
extern char tooltip_label_7fd828[];
extern char tooltip_label_7feb84[];
extern char tooltip_label_7feb68[];
extern char tooltip_label_7feb4c[];
extern char tooltip_label_7feb30[];
extern char tooltip_label_7feb10[];
extern char tooltip_label_7feaf4[];
wchar_t *__fastcall Player_Info_Tooltip_0074a120(wchar_t const *name)
{
    Wstring player_name;
    WOL::Ladder player_ladder;
    memset(&player_ladder,0,sizeof(player_ladder));
    player_name.Set(Wide_To_Narrow_006fa120(name));
    player_name.To_Lower();
    bool player_found;
    WolLadderCache *cache=unknown_ladder_cache_b2ac24;
    if(cache && cache->Count) {
        WolLadderCache::Node *node=cache->Buckets[cache->Key_Hash_00767550(player_name)];
        if(node) {
            while(node && !(node->Key==player_name)) node=node->Next;
            if(node) {
                WOL::Ladder *value=&node->Value;
                if(value) player_ladder=*value;
                player_found=true;
                goto player_lookup_done;
            }
        }
    }
    player_found=false;
player_lookup_done:
    {
    WOL::Ladder squad_ladder;
    memset(&squad_ladder,0,sizeof(squad_ladder));
    Wstring squad_abbreviation;
    WOL::User user;
    for(int index=0;index<unknown_users_b2a728.Count;++index) {
        unknown_users_b2a728.Get_Copy(user,index);
        if(strcmp(user.name,Wide_To_Narrow_006fa120(name))==0) break;
    }
    bool has_squad=false;
    bool squad_found=false;
    if(strlen(user.unknown_2e)!=0) {
        has_squad=true;
        squad_abbreviation.Set(user.unknown_57);
        squad_abbreviation.To_Lower();
        if(unknown_ladder_b296f8.Count) {
            WolLadderCache::Node *node=unknown_ladder_b296f8.Buckets[unknown_ladder_b296f8.Key_Hash(squad_abbreviation)];
            if(node) {
                while(node && !(node->Key==squad_abbreviation)) node=node->Next;
                if(node) {
                    WOL::Ladder *value=&node->Value;
                    if(value) squad_ladder=*value;
                    squad_found=true;
                }
            }
        }
    }
    if(player_found) {
        if(has_squad) {
            if(squad_found) {
                wchar_t *squad_name=Narrow_To_Wide_006fa180(user.unknown_2e);
                swprintf(wol_player_tooltip_b290a4,StringTable::Load_String(tooltip_label_7feb84,0,wol_source_file_007fd62c,0x36ce),StringTable::Load_String(tooltip_label_7fd814,0,wol_source_file_007fd62c,0x36ce),player_ladder.unknown_1c,squad_name,squad_ladder.unknown_1c);
            } else {
                wchar_t *squad_name=Narrow_To_Wide_006fa180(user.unknown_2e);
                swprintf(wol_player_tooltip_b290a4,StringTable::Load_String(tooltip_label_7feb68,0,wol_source_file_007fd62c,0x36d2),StringTable::Load_String(tooltip_label_7fd814,0,wol_source_file_007fd62c,0x36d2),player_ladder.unknown_1c,squad_name);
            }
        } else {
            swprintf(wol_player_tooltip_b290a4,StringTable::Load_String(tooltip_label_7feb4c,0,wol_source_file_007fd62c,0x36d6),StringTable::Load_String(tooltip_label_7fd814,0,wol_source_file_007fd62c,0x36d6),player_ladder.unknown_1c);
        }
    } else {
        if(has_squad) {
            if(squad_found) {
                wchar_t *squad_name=Narrow_To_Wide_006fa180(user.unknown_2e);
                swprintf(wol_player_tooltip_b290a4,StringTable::Load_String(tooltip_label_7feb30,0,wol_source_file_007fd62c,0x36dc),StringTable::Load_String(tooltip_label_7fd828,0,wol_source_file_007fd62c,0x36dc),squad_name,squad_ladder.unknown_1c);
            } else {
                wchar_t *squad_name=Narrow_To_Wide_006fa180(user.unknown_2e);
                swprintf(wol_player_tooltip_b290a4,StringTable::Load_String(tooltip_label_7feb10,0,wol_source_file_007fd62c,0x36e0),StringTable::Load_String(tooltip_label_7fd828,0,wol_source_file_007fd62c,0x36e0),squad_name);
            }
        } else {
            swprintf(wol_player_tooltip_b290a4,StringTable::Load_String(tooltip_label_7feaf4,0,wol_source_file_007fd62c,0x36e4),StringTable::Load_String(tooltip_label_7fd828,0,wol_source_file_007fd62c,0x36e4));
        }
    }
    }
    return wol_player_tooltip_b290a4;
}


extern int wol_player_pings_b292ac[8];
extern wchar_t wol_ping_tooltip_b28de4[];
extern char wol_source_file_007fd62c[];
extern char wol_label_unknown_ping[];
extern char wol_label_unknown[];
extern char wol_label_no_ping_tooltip[];
extern char wol_label_good[];
extern char wol_label_fair[];
extern char wol_label_poor[];
extern char wol_label_ping_tooltip[];
wchar_t *__fastcall Unknown_0074a4e0(wchar_t const *name)
{
    int ping=wol_player_pings_b292ac[Find_Slot_By_Name_005c8c30(name)];
    if(ping<0) {
        swprintf(wol_ping_tooltip_b28de4,StringTable::Load_String(wol_label_no_ping_tooltip,0,wol_source_file_007fd62c,0x36ef),name,StringTable::Load_String(wol_label_unknown,0,wol_source_file_007fd62c,0x36ef),StringTable::Load_String(wol_label_unknown_ping,0,wol_source_file_007fd62c,0x36ef));
    }
    else if(ping<300) {
        swprintf(wol_ping_tooltip_b28de4,StringTable::Load_String(wol_label_ping_tooltip,0,wol_source_file_007fd62c,0x36f1),name,StringTable::Load_String(wol_label_good,0,wol_source_file_007fd62c,0x36f1),ping);
    }
    else if(ping<500) {
        swprintf(wol_ping_tooltip_b28de4,StringTable::Load_String(wol_label_ping_tooltip,0,wol_source_file_007fd62c,0x36f3),name,StringTable::Load_String(wol_label_fair,0,wol_source_file_007fd62c,0x36f3),ping);
    }
    else {
        swprintf(wol_ping_tooltip_b28de4,StringTable::Load_String(wol_label_ping_tooltip,0,wol_source_file_007fd62c,0x36f5),name,StringTable::Load_String(wol_label_poor,0,wol_source_file_007fd62c,0x36f5),ping);
    }
    return wol_ping_tooltip_b28de4;
}


void __fastcall Enforce_Tournament_Options_0074a640(HWND dialog)
{
    if(Unknown_0073c340(&unknown_game_channel_b299c0)) {
        EnableWindow(GetDlgItem(dialog,0x552),false);
        EnableWindow(GetDlgItem(dialog,0x553),false);
        EnableWindow(GetDlgItem(dialog,0x597),false);
        unknown_session_a3d298.AICount=0;
        unknown_session_a3d298.unknown_a4=false;
        HWND first=GetDlgItem(dialog,0x54e);
        SendMessageA(first,0xf1,1,0);
        EnableWindow(first,false);
        HWND second=GetDlgItem(dialog,0x54b);
        SendMessageA(second,0xf1,1,0);
        EnableWindow(second,false);
        unknown_session_a3d298.unknown_2a=true;
        unknown_session_a3d298.unknown_bases_20=true;
    }
}


namespace OwnerDraw {
WideString __fastcall unknown_5e2160(HWND,int);
WideString __fastcall unknown_5e2260(unsigned int,HWND);
}
int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
void __stdcall unknown_dialog_5ecf40(HWND,int);
void __fastcall Unknown_0073FEE0(int,bool);
wchar_t *__fastcall Player_Info_Tooltip_0074a120(wchar_t const *);
wchar_t *__fastcall Unknown_0074a4e0(wchar_t const *);
int __fastcall Unknown_0073cb80(WideString const &);
int __fastcall Unknown_0073ca50(WideString const &);
int __fastcall Parse_Page_Command_00746480(wchar_t const *);
int __fastcall Reply_To_Last_Pager_007466c0(wchar_t const *);
int __fastcall Parse_Online_Command_00746240(wchar_t const *);
extern "C" wchar_t *__cdecl wcsncpy(wchar_t *,wchar_t const *,unsigned);
extern "C" int __cdecl wcsncmp(wchar_t const *,wchar_t const *,unsigned);
extern wchar_t guest_empty_wide_839d4c[];
extern char guest_last_pager_b2ac48[];
extern int guest_action_color_7a7370,guest_private_action_color_7a736c;
extern int guest_chat_color_7a737c,guest_private_color_7a7368;
extern char guest_label_7fe9fc[];
extern char guest_label_7fe9e8[];
extern wchar_t guest_label_7fe2c4[];
extern char guest_label_7dfc7c[];
extern wchar_t guest_label_7d64f4[];
extern char guest_label_7fe9d4[];
extern char guest_label_7dfc64[];
extern wchar_t guest_label_7fe9b8[];
extern char guest_label_7dfc48[];
extern char guest_label_7dfc14[];
extern char guest_label_7dfbfc[];
extern wchar_t guest_label_7fe9a4[];
extern char guest_label_7e8fe8[];
extern char guest_label_7e8e28[];
extern char guest_label_7eb2e4[];
extern char guest_label_7eb2d8[];
extern char wol_source_file_007fd62c[];
void __fastcall Save_Lobby_Options_005cc890(HWND,SessionOptionsClass *);
void __fastcall Change_Scenario_Slots_005cb8d0(HWND,int,int);
bool __fastcall Unknown_005c9320(NodeNameType *);
void __fastcall Unknown_0073c480();
void __fastcall Unknown_0075b1b0();
void Request_All_User_Pings_0075af80();
void __fastcall Update_Channel_Player_Limit_00749f20();
void Unknown_Enable_Start_005c9700();
bool __fastcall Unknown_005c93a0(NodeNameType *);
bool __fastcall Unknown_Clear_Closed_Slots_005cacb0(HWND);
void __fastcall Unknown_005cca30(HWND,SessionOptionsClass *);
void __fastcall Unknown_005e5150(HWND,bool);
extern "C" char *__stdcall inet_ntoa(unsigned long);

extern bool host_scenario_enabled_7e6dc0;
extern int host_leave_b2abf0;
extern unsigned long host_version_a3d8d4;
extern char host_scenario_file_a3d8f0[];
extern char host_label_7e6b84[];
extern char host_label_7fec6c[];
extern char host_label_7e6b74[];
extern char host_label_7e6b5c[];
extern char host_label_7fec54[];
extern char host_label_7fec3c[];
extern char host_label_7e24cc[];
extern char host_label_7fec24[];
extern char host_label_7fec10[];
extern char ping_request_debug_007fdde0[];
static void Apply_Channel_Restrictions(HWND window)
{
    if(Unknown_0073c370()) {

                EnableWindow(GetDlgItem(window,0x552),0); EnableWindow(GetDlgItem(window,0x553),0); EnableWindow(GetDlgItem(window,0x597),0);
                unknown_session_a3d298.AICount=0; unknown_session_a3d298.unknown_a4=false;
                HWND control=GetDlgItem(window,0x54e); SendMessageA(control,0xf1,1,0); EnableWindow(control,0);
                control=GetDlgItem(window,0x54b); SendMessageA(control,0xf1,1,0); EnableWindow(control,0);
                unknown_session_a3d2c2=1; unknown_flag_a3d2b8=true;
            
    }
}
int __stdcall WOL_Game_Options_Proc_0074a710(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    int handled=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(handled) return handled;
    if(!unknown_b2ac40) {
        switch(message) {
        case 0x497:
            Apply_Channel_Restrictions(window);
            Unknown_Fill_Options_Dialog_005c3550(window,1); break;
        case 0x114:
            if((short)wparam==8) {
                if(GetDlgItem(window,0x54f)) unknown_session_a3d2d0=SendDlgItemMessageA(window,0x54f,0x400,0,0);
                if(GetDlgItem(window,0x402)) unknown_session_a3d2bc=SendDlgItemMessageA(window,0x402,0x400,0,0);
                if(GetDlgItem(window,0x529)) unknown_session_a3d298.GameSpeed_30=6-SendDlgItemMessageA(window,0x529,0x400,0,0);
            for(int accepted=(int)&unknown_player_b29cf0[1].unknown_20;accepted<(int)&unknown_player_b29cf0[8].unknown_20;accepted+=sizeof(WolPlayerData)) *(int *)accepted=0;
            Unknown_Enable_Start_005c9700(); Unknown_0073FEE0(0,false);
            }
            break;
        case 0x111: {
            bool changed=false;
            switch((int)(wparam&0xffff)) {
            case 0x3e8: unknown_session_a3d298.unknown_a8=false; if(SendDlgItemMessageA(window,0x3e8,0xf0,0,0)==1) unknown_session_a3d298.unknown_a8=true; changed=true; break;
            case 0x54d: unknown_flag_a3d2c1=false; if(SendDlgItemMessageA(window,0x54d,0xf0,0,0)==1) unknown_flag_a3d2c1=true; changed=true; break;
            case 0x551: unknown_session_a3d298.unknown_34=false; if(SendDlgItemMessageA(window,0x551,0xf0,0,0)==1) unknown_session_a3d298.unknown_34=true; changed=true; break;
            case 0x596: unknown_session_a3d33d=false; if(SendDlgItemMessageA(window,0x596,0xf0,0,0)==1) unknown_session_a3d33d=true; changed=true; break;
            case 0x597: unknown_session_a3d298.unknown_a4=false; if(SendDlgItemMessageA(window,0x597,0xf0,0,0)==1) unknown_session_a3d298.unknown_a4=true; changed=true; break;
            case 0x599: unknown_session_fog_a3d33f=false; if(SendDlgItemMessageA(window,0x599,0xf0,0,0)==1) unknown_session_fog_a3d33f=true; changed=true; break;
            case 0x624: unknown_session_a3d2c0=false; if(SendDlgItemMessageA(window,0x624,0xf0,0,0)==1) unknown_session_a3d2c0=true; changed=true; break;
            case 0x69a: unknown_session_a3d2c3=false; if(SendDlgItemMessageA(window,0x69a,0xf0,0,0)==1) unknown_session_a3d2c3=true;  break;
            case 0x54b:
                unknown_flag_a3d2b8=false;
                if(SendDlgItemMessageA(window,0x54b,0xf0,0,0)==1) unknown_flag_a3d2b8=true;
                else if(!unknown_flag_a3d2b8) { unknown_session_a3d2c2=0; SendDlgItemMessageA(window,0x54e,0xf1,0,0); }
                changed=true; break;
            case 0x54e:
                unknown_session_a3d2c2=0;
                if(SendDlgItemMessageA(window,0x54e,0xf0,0,0)==1) { unknown_session_a3d2c2=1; if(!unknown_flag_a3d2b8) { unknown_flag_a3d2b8=true; SendDlgItemMessageA(window,0x54b,0xf1,1,0); } }
                changed=true; break;
            case 0x69d:
                unknown_session_a3d298.unknown_2c=false;
                if(SendDlgItemMessageA(window,0x69d,0xf0,0,0)==1) unknown_session_a3d298.unknown_2c=true;
                break;
            }
            if(changed) {
            for(int accepted=(int)&unknown_player_b29cf0[1].unknown_20;accepted<(int)&unknown_player_b29cf0[8].unknown_20;accepted+=sizeof(WolPlayerData)) *(int *)accepted=0;
            Unknown_Enable_Start_005c9700(); Unknown_0073FEE0(0,false);
            }
            break;
        }
        }
    }
    switch(message) {
    case 0x84: {
        HWND tooltip=GetDlgItem(window,0x695);
        if(!tooltip) goto host_unhandled;
        POINT point;
        point.x=lparam&0xffff;
        point.y=(unsigned)lparam>>16;
        RECT bounds;
        GetWindowRect(window,&bounds);
        point.x-=bounds.left;
        point.y-=bounds.top;
        WideString text;
        HWND child=ChildWindowFromPointEx(window,point,1);
        if(child) {
            text=OwnerDraw::unknown_5e2160(window,(int)child);
            if(Compare_Wide_007837d3(text.Get(),guest_empty_wide_839d4c)==0)
                text=OwnerDraw::unknown_5e2260((unsigned)window,child);
        }
        SendMessageA(tooltip,0x4b2,0,(LPARAM)text.unknown_text_770430());
        return 0;
    }
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 2:
        if(unknown_dialog_preview_a72ce8) {
            delete unknown_dialog_preview_a72ce8;
            unknown_dialog_preview_a72ce8=0;
        }
        if(!ProgressScreen.Has_Window()) goto host_unhandled;
        ProgressScreen.Clear_Window();
        return 0;
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        if(unknown_dialog_preview_a72ce8) unknown_dialog_preview_a72ce8->Blit_Preview(window);
        else if(unknown_session_a3d2b4==-1 && unknown_preview_a6fee4) unknown_preview_a6fee4->Blit_Preview(window);
        if(ProgressScreen.Has_Window()) ProgressScreen.Draw_It(Point2D(-1,-1));
        ValidateRect(window,0);
        return 0;
    case 0x111: {
        int command=wparam&0xffff;
        switch(command) {
        case 2:
            if(unknown_b2ac40) goto host_unhandled;
            Save_Lobby_Options_005cc890(window,&unknown_session_a3d298.WonlineOptions);
            unknown_session_a3d298.Write_MultiPlayer_Settings();
            if(unknown_dialog_preview_a72ce8) { delete unknown_dialog_preview_a72ce8; unknown_dialog_preview_a72ce8=0; }
            Unknown_007357E0(window,2); ShowWindow(window,0); unknown_dialog_5ecf40(window,1);
            unknown_chat_a72cfc->unknown_slot09(); host_leave_b2abf0=1;
            return 0;
        case 0x3ed: {
            HWND input=GetDlgItem(window,0x540);
            WideString text=Limit_Name_Text_005ccf50(input);
            SendMessageA(input,0x4b2,0,(LPARAM)guest_empty_wide_839d4c);
            SetFocus(input);
            InvalidateRect(input,0,0);
            int color=guest_action_color_7a7370;
            if(strlen(unknown_channel_b299c0.name)==0)
                Unknown_005C2C80(-1,StringTable::Load_String(guest_label_7fe9fc,0,wol_source_file_007fd62c,0x3ae5));
            else if(text.Length()>0) {
                if(Unknown_0073cb80(text)) color=guest_private_action_color_7a736c;
                Unknown_005C2C80(color,guest_label_7fe2c4,unknown_wstring_b26cb8.Buffer,text.Get());
            } else Unknown_005C2C80(-1,StringTable::Load_String(guest_label_7fe9e8,0,wol_source_file_007fd62c,0x3aec));
            return 0;
        }
        case 0x540: {
            wchar_t nickname[16],primary_command[64],alternate_command[64];
            WideString text=Limit_Name_Text_005ccf50((HWND)lparam);
            if((unsigned short)(wparam>>16)==0x501) {
                SendMessageA((HWND)lparam,0x4b2,0,(LPARAM)guest_empty_wide_839d4c);
                if(text.Length()>0) {
                    int parsed=Parse_Page_Command_00746480(text.Get());
                    if(parsed!=0) {
                        if(parsed==1) {
                            int count=0;
                            unsigned length=text.Length()+1;
                            wchar_t *copy=(wchar_t *)operator new(length*2);
                            wcscpy(primary_command,StringTable::Load_String(guest_label_7dfc7c,0,wol_source_file_007fd62c,0x3a71));
                            if(!copy) return 0;
                            copy=wcsncpy(copy,text.Get(),length);
                            if(!copy) return 0;
                            wchar_t *body=copy+wcslen(primary_command);
                            while(*body && wcsncmp(body,guest_label_7d64f4,1)==0) ++body;
                            while(*body) {
                                if(wcsncmp(body,guest_label_7d64f4,1)==0 || count>10) break;
                                nickname[count++]=*body++;
                            }
                            nickname[count]=0;
                            while(*body && wcsncmp(body,guest_label_7d64f4,1)==0) ++body;
                            if(count>10) Unknown_005C2C80(-1,StringTable::Load_String(guest_label_7fe9d4,0,wol_source_file_007fd62c,0x3a8f));
                            else if(count==0) Unknown_005C2C80(-1,StringTable::Load_String(guest_label_7dfc64,0,wol_source_file_007fd62c,0x3a91));
                            else if(wcslen(body)>0) Unknown_005C2C80(guest_private_color_7a7368,guest_label_7fe9b8,unknown_wstring_b26cb8.Buffer,nickname,body);
                            else Unknown_005C2C80(-1,StringTable::Load_String(guest_label_7dfc48,0,wol_source_file_007fd62c,0x3a97));
                            operator delete(copy);
                        }
                    } else if(Reply_To_Last_Pager_007466c0(text.Get())) {
                        unsigned length=text.Length()+1;
                        wchar_t *copy=(wchar_t *)operator new(length*2);
                        wcscpy(primary_command,StringTable::Load_String(guest_label_7dfc14,0,wol_source_file_007fd62c,0x3aa9));
                        wcscpy(alternate_command,StringTable::Load_String(guest_label_7dfbfc,0,wol_source_file_007fd62c,0x3aaa));
                        if(!copy) return 0;
                        copy=wcsncpy(copy,text.Get(),length);
                        if(!copy) return 0;
                        wchar_t *body=copy+(wcsncmp(copy,primary_command,wcslen(primary_command))==0 ? wcslen(primary_command):wcslen(alternate_command));
                        while(*body && wcsncmp(body,guest_label_7d64f4,1)==0) ++body;
                        wcscpy(nickname,Narrow_To_Wide_006fa180(guest_last_pager_b2ac48));
                        if(wcslen(body)>0 && wcslen(nickname)>0)
                            Unknown_005C2C80(guest_private_color_7a7368,guest_label_7fe9b8,unknown_wstring_b26cb8.Buffer,nickname,body);
                        else if(wcslen(nickname)>0)
                            Unknown_005C2C80(-1,StringTable::Load_String(guest_label_7dfc48,0,wol_source_file_007fd62c,0x3ac7));
                        operator delete(copy);
                    } else if(!Parse_Online_Command_00746240(text.Get())) {
                        int color=guest_chat_color_7a737c;
                        if(Unknown_0073ca50(text)==1) color=guest_private_color_7a7368;
                        Unknown_005C2C80(color,guest_label_7fe9a4,unknown_wstring_b26cb8.Buffer,text.Get());
                    }
                }
            }
            return 0;
        }
        case 0x411: case 0x412: case 0x413: case 0x414:
        case 0x415: case 0x416: case 0x417: case 0x418:
            if((unsigned short)(wparam>>16)!=1 || unknown_b2ac40) goto host_unhandled;
            if(!Is_Host_005c86a0()) goto host_unhandled;
            { NodeNameType *entry=(NodeNameType *)unknown_combo_selected_data_005c99d0(window,command,-1); Unknown_005cbf20(window,command,entry,2); }
            return 0;
        case 0x522: case 0x523: case 0x524: case 0x525: case 0x526: case 0x527: case 0x528: case 0x6a2:
            if((unsigned short)(wparam>>16)!=1 || unknown_b2ac40) goto host_unhandled;
            Set_Slot_Color_005c9ab0(window,command);
            Unknown_005c3810(true,false); Refresh_Slot_Settings_005ca000(window,true);
            return 0;
        case 0x510: case 0x513: case 0x514: case 0x51e: case 0x51f: case 0x520: case 0x521: case 0x6a1:
            if((unsigned short)(wparam>>16)!=1 || unknown_b2ac40) goto host_unhandled;
            Set_Slot_Country_005c9a10(window,command);
            Unknown_005c3810(true,false); Refresh_Slot_Settings_005ca000(window,true);
            return 0;
        case 0x588: {
            SetFocus(GetDlgItem(window,0x540));
            if(unknown_b2ac04==1) goto host_unhandled;
            Save_Lobby_Options_005cc890(window,&unknown_session_a3d298.WonlineOptions);
            unknown_session_a3d298.Write_MultiPlayer_Settings();
            Refresh_Slot_Settings_005ca000(window,true);
            int observers=0;
            for(int i=0;i<unknown_players_a3fa7c.Count();++i) if(unknown_players_a3fa7c[i]->Team==-1) ++observers;
            if(unknown_users_b2a728.Count-observers<2) { Unknown_005C2C80(-1,StringTable::Load_String(host_label_7e6b84,0,wol_source_file_007fd62c,0x397c)); return 0; }
            if(observers && unknown_session_a3d298.AICount) { Unknown_005C2C80(-1,StringTable::Load_String(host_label_7fec6c,0,wol_source_file_007fd62c,0x3987)); return 0; }
            bool missing=false;
            for(int j=1;j<unknown_players_a3fa7c.Count();++j) if(!unknown_players_a3fa7c[j]->unknown_map_state_6f) {
                Unknown_005C2C80(-1,StringTable::Load_String(host_label_7e6b74,0,wol_source_file_007fd62c,0x3992),unknown_players_a3fa7c[j],unknown_map_name_a3d342); missing=true;
            }
            if(missing) return 0;
            for(int k=0;k<unknown_users_b2a728.Count;++k) if(!unknown_player_b29cf0[k].unknown_20) {
                Unknown_005c3b70(); Unknown_005C2C80(-1,StringTable::Load_String(host_label_7e6b5c,0,wol_source_file_007fd62c,0x39a5)); return 0;
            }
            if(unknown_session_a3d298.Type==4 && unknown_a3d2a4==2) {
                if(unknown_users_b2a728.Count<(int)unknown_channel_b299c0.maxUsers) { Unknown_005C2C80(-1,StringTable::Load_String(host_label_7fec54,0,wol_source_file_007fd62c,0x39af),unknown_channel_b299c0.maxUsers); return 0; }
                int team1=0,team2=0;
                for(int q=0;q<unknown_users_b2a728.Count;++q) { if(unknown_player_b29cf0[q].unknown_18==0) ++team2; else ++team1; }
                if(team1!=team2) { Unknown_005C2C80(-1,StringTable::Load_String(host_label_7fec3c,0,wol_source_file_007fd62c,0x39be)); return 0; }
            }
            int limit=Scenario_Slot_Limit_005c65a0(unknown_session_a3d2b4);
            unsigned requested=SendDlgItemMessageA(window,0x552,0x400,0,0);
            if(limit<(int)((requested-observers)+unknown_users_b2a728.Count)) { Unknown_005C2C80(-1,StringTable::Load_String(host_label_7e24cc,0,wol_source_file_007fd62c,0x39c9),limit); return 0; }
            if(unknown_users_b2a728.Count>4) { Unknown_005C2C80(-1,StringTable::Load_String(host_label_7fec24,0,wol_source_file_007fd62c,0x39d1)); return 0; }
            MultiplayerGameMode *mode=unknown_session_a3d298.Mode;
            WideString error;
            if(!mode->CheckStartConditions(&error)) {
                Unknown_005C2C80(-1,error.Get()); EnableWindow(GetDlgItem(Unknown_00735B10(),0x588),1); return 0;
            }
            if(Unknown_0073c3c0(&unknown_game_channel_b299c0)) {
                WOL::User *user=0;
                unsigned squad1=0,squad2=0;
                int count1=0,count2=0;
                char observer[10]="";
                if(unknown_lobby_flag_7e6d90>=0) {
                    NodeNameType *node=unknown_player_slots_a3fa98[unknown_lobby_flag_7e6d90];
                    if(Unknown_005c93a0(node) && node->Team==-1) { char *name=Wide_To_Narrow_006fa120(node->Name); strncpy(observer,name,10); observer[9]=0; }
                }
                for(int u=0;u<unknown_users_b2a728.Count;++u) {
                    unknown_users_b2a728.Get(user,u);
                    if(_stricmp(observer,user->name)) {
                        unsigned squad=user->unknown_14;
                        if(squad<1) { Unknown_005C2C80(-1,StringTable::Load_String(host_label_7fec10,0,wol_source_file_007fd62c,0x39fb)); return 0; }
                        if(!squad1 || squad1==squad) { squad1=squad; ++count1; }
                        else { if(squad2 && squad2!=squad) { Unknown_005C2C80(-1,StringTable::Load_String(host_label_7fec10,0,wol_source_file_007fd62c,0x3a0a)); return 0; } squad2=squad; ++count2; }
                    }
                }
                if(count1!=count2) { Unknown_005C2C80(-1,StringTable::Load_String(host_label_7fec10,0,wol_source_file_007fd62c,0x3a10)); return 0; }
            }
            mode->unknown_slot22(); unknown_session_a3d298.Unknown_00672BB0(); Unknown_0073c480();
            Unknown_005c3810(true,true); mode->unknown_slot21(); mode->unknown_slot9();
            WOL::User user; memset(&user,0,sizeof(user)); WOL::User *head=0;
            for(int v=0;v<unknown_users_b2a728.Count;++v) {
                unknown_users_b2a728.Get_Copy(user,v); WOL::User *next=new WOL::User; *next=user; next->next=head; head=next;
            }
            unknown_chat_a72cfc->RequestGameStart(head); unknown_b2ac40=1; unknown_b2ac04=true;
            WOL::User *current=head;
            while(current) { head=current->next; delete current; current=head; }
            return 0;
        }
        case 0x5aa: {
            if(unknown_b2ac40) goto host_unhandled;
            int old=unknown_session_a3d2b4;
            bool ai=!unknown_session_a3d298.Mode || unknown_session_a3d298.Mode->unknown_slot12();
            bool observers=!unknown_session_a3d298.Mode || unknown_session_a3d298.Mode->unknown_slot40();
            char filename[132]; wchar_t description[130];
            strcpy(filename,host_scenario_file_a3d8f0); wcscpy(description,unknown_map_name_a3d342);
            host_scenario_enabled_7e6dc0=false; Unknown_005e5150(window,false); unknown_5159b0(0,true);
            bool cancelled=Open_Lobby_Dialog_005c6960((HWND)unknown_b263d8)==2;
            ShowWindow(window,5); unknown_dialog_5ecf40(window,0); SetFocus(GetDlgItem(window,0x540));
            if(cancelled) {
                unknown_session_a3d2b4=old; Select_Scenario_005c7c30(old); Unknown_005c7570(window);
                host_scenario_enabled_7e6dc0=true; ShowWindow(window,5); unknown_dialog_5ecf40(window,0);
                Load_Random_Map_Preview_005cc800(window); Unknown_005c3810(true,false); InvalidateRect(window,0,0); return 0;
            }
            if(!Select_Scenario_005c7c30(unknown_session_a3d2b4)) unknown_session_a3d2b4=old;
            else {
                bool new_ai=!unknown_session_a3d298.Mode || unknown_session_a3d298.Mode->unknown_slot12();
                bool new_observers=!unknown_session_a3d298.Mode || unknown_session_a3d298.Mode->unknown_slot40();
                if(observers && !new_observers) Unknown_005cb560(window);
                if(ai && !new_ai) {
                    if(Unknown_Clear_Closed_Slots_005cacb0(window)) {
                        unknown_session_a3d298.AICount=0; Unknown_0073c480(); Unknown_0073FEE0(0,false); Unknown_005c3810(true,false);
                    } else Unknown_0073FEE0(0,true);
                }
                Change_Scenario_Slots_005cb8d0(window,old,unknown_session_a3d2b4);
            }
            Update_Channel_Player_Limit_00749f20(); host_scenario_enabled_7e6dc0=true;
            ShowWindow(window,5); unknown_dialog_5ecf40(window,0); Unknown_005c34b0(window);
            if(strcmp(filename,host_scenario_file_a3d8f0) || Compare_Wide_007837d3(description,unknown_map_name_a3d342)) {
                for(int accepted=(int)&unknown_player_b29cf0[1].unknown_20;accepted<(int)&unknown_player_b29cf0[8].unknown_20;accepted+=sizeof(WolPlayerData)) *(int *)accepted=0;
            }
            Unknown_Enable_Start_005c9700(); Load_Random_Map_Preview_005cc800(window); Refresh_Slot_Settings_005ca000(window,true); Unknown_005c3810(true,false); InvalidateRect(window,0,0);
            return 0;
        }
        }
        return 0;
    }
    case 0x497: {
        SendDlgItemMessageA(window,0x3ed,0x49c,0,(LPARAM)unknown_bitmap_writer_a76378.Get_Image_0068ca40(guest_label_7eb2e4,0));
        SendDlgItemMessageA(window,0x3ed,0x4aa,0,(LPARAM)unknown_bitmap_writer_a76378.Get_Image_0068ca40(guest_label_7eb2d8,0));
        MultiplayerGameMode *mode=MultiplayerGameMode::unknown_first_mode_5b6ec0();
        unknown_session_a3d298.GameModeIndex=mode->unknown_28; unknown_session_a3d298.Mode=mode;
        mode->unknown_slot8(); Unknown_005c3450(window,mode);
        unknown_session_a3d2b4=mode->FirstValidMapIndex(); Select_Scenario_005c7c30(unknown_session_a3d2b4);
        Fill_Side_Combos_4d16d0(window); Init_Color_Entries_4d1ea0(); Fill_Color_Combos_4d2300(window);
        Select_Player_Side_4d1820(window,unknown_lobby_a3fa94); Select_Player_Color_4d23c0(window,unknown_lobby_a3fa94);
        Unknown_005cca30(window,&unknown_session_a3d298.WonlineOptions);
            Apply_Channel_Restrictions(window);
        if(unknown_session_a3d298.Mode && !unknown_session_a3d298.Mode->unknown_slot40()) {
            unknown_lobby_flag_7e6d90=-1; unknown_a72c68=0; Unknown_005cd540(0);
            unknown_players_a3fa7c[0]->Team=0;
            unknown_player_slots_a3fa98[0]->unknown_66e2d0(unknown_session_a3d298.Mode->unknown_slot10());
            unknown_player_slots_a3fa98[0]->unknown_66e300(-2); Fill_Color_Combos_4d2300(window);
        }
        Unknown_005c9b90(window,0,true); Change_Scenario_Slots_005cb8d0(window,-1,unknown_session_a3d2b4);
        Update_Channel_Player_Limit_00749f20(); Refresh_Slot_Images_005c9ec0(window);
        Unknown_005c3450(window,unknown_session_a3d298.Mode); Unknown_005c34b0(window); Load_Random_Map_Preview_005cc800(window);
        host_version_a3d8d4=VerNum.Version_Number(); SetTimer(window,0x14af8,5000,0);
        Unknown_Fill_Options_Dialog_005c3550(window,1); Refresh_Slot_Settings_005ca000(window,true); SetFocus(GetDlgItem(window,0x540));
        if(unknown_b2ac40) goto host_unhandled;
        WOL::User user;
        memset(&user,0,sizeof(user));
        int count=unknown_users_b2a728.Count;
        int handle=0;
        int i=0;
        if(count>0) {
        do {
            unknown_users_b2a728.Get_Copy(user,i);
            unsigned long address=user.unknown_18;
            Debug_Printf(ping_request_debug_007fdde0,user.name,inet_ntoa(address));
            unknown_ladder_client_a72d00->Request_Ping(inet_ntoa(address),1000,&handle);
            ++i;
        } while(i<count);
        return 0;
        }
        goto host_unhandled;
    }
    case 0x114: case 0x115:
        if(!unknown_b2ac40) {
            unknown_session_a3d2d0=SendDlgItemMessageA(window,0x54f,0x400,0,0);
            unknown_session_a3d2bc=SendDlgItemMessageA(window,0x402,0x400,0,0);
            unknown_session_a3d298.GameSpeed_30=6-SendDlgItemMessageA(window,0x529,0x400,0,0);
            return 0;
        }
        goto host_unhandled;
    case 0x113: {
        if(wparam!=0x14af8 || !Is_Host_005c86a0()) goto host_unhandled;
        Unknown_0075b1b0();
        if(unknown_b2ac40) goto host_unhandled;
        WOL::User user;
        memset(&user,0,sizeof(user));
        int count=unknown_users_b2a728.Count;
        int handle=0;
        int i=0;
        if(count<=0) goto host_unhandled;
        do {
            unknown_users_b2a728.Get_Copy(user,i);
            unsigned long address=user.unknown_18;
            Debug_Printf(ping_request_debug_007fdde0,user.name,inet_ntoa(address));
            unknown_ladder_client_a72d00->Request_Ping(inet_ntoa(address),1000,&handle);
            ++i;
        } while(i<count);
        return 0;
    }
    case 0x49b:
        { HWND control=GetDlgItem(window,wparam); SendMessageA(control,0x4b3,0x7f,lparam); return 0; }
    case 0x4e9: {
        OwnerDraw::UnknownTextRequest *request=(OwnerDraw::UnknownTextRequest *)lparam;
        int id;
        {
            WideString &text=request->text;
            text=(wchar_t const *)0;
            if(!request->unknown_00) goto tooltip_done;
            id=GetDlgCtrlID((HWND)request->unknown_00);
            if(request->unknown_04!=-1) {
                if(Control_Slot_005c9800(id)==-1) {
                    if(Side_Combo_Player_4d13b0(id)==-1) {
                        if(Color_Combo_Player_4d1d10(id)!=-1) {
                            text=Color_Name_4d1d80(Color_Combo_Data_4d2900(window,id,request->unknown_04));
                            return 1;
                        }
                    } else {
                        int country=Side_Combo_Data_4d1c70(window,id,request->unknown_04);
                        if(country!=-1) { text=Side_Name_4d1420(country); return 1; }
                    }
                } else {
                    NodeNameType *entry=(NodeNameType *)unknown_combo_selected_data_005c99d0(window,id,request->unknown_04);
                    text=Entry_Type_Name_005c9870(entry);
                    return 1;
                }
                goto tooltip_done;
            }
            if(Control_Slot_005c9800(id)!=-1) {
                NodeNameType *entry=(NodeNameType *)unknown_combo_selected_data_005c99d0(window,id,-1);
                if(entry) {
                    WideString label=OwnerDraw::unknown_5e2260((unsigned)window,(HWND)request->unknown_00);
                    if((Compare_Wide_007837d3(label.Get(),StringTable::Load_String(guest_label_7e8fe8,0,wol_source_file_007fd62c,0x37dd))==0 ||
                        Compare_Wide_007837d3(label.Get(),StringTable::Load_String(guest_label_7e8e28,0,wol_source_file_007fd62c,0x37dd))==0) && Unknown_005c9320(entry))
                        label=Player_Info_Tooltip_0074a120(entry->Name);
                    text=label.Get();
                }
                goto tooltip_done;
            }
        }
        {
            int player;
            if(id==0x754) player=0;
            else if(id==0x755) player=1;
            else if(id==0x756) player=2;
            else if(id==0x757) player=3;
            else if(id==0x758) player=4;
            else if(id==0x759) player=5;
            else if(id==0x75a) player=6;
            else if(id==0x75b) player=7;
            else goto tooltip_done;
            WideString ping;
            int slot=Slot_Control_Id_005c97a0(player);
            if(slot!=-1) {
                NodeNameType *entry=(NodeNameType *)unknown_combo_selected_data_005c99d0(window,slot,-1);
                if(entry && Unknown_005c9320(entry)) {
                    ping=Unknown_0074a4e0(entry->Name);
                    request->text=ping;
                }
            }
        }
tooltip_done:
        return 1;
    }
    case 0x4ed:
        ShowWindow(window,0);
        unknown_dialog_5ecf40(window,1);
host_unhandled:
        return 0;
    case 3: case 0x18: goto host_unhandled;
    case 0x14: return 1;
    default: return 0;
    }
    return 0;
}


extern char leave_channel_label_007fde60[],observer_leave_label_007e6cdc[];
extern char wol_source_file_007fd62c[];
bool connection_wait_callback_0073c120();
void __fastcall Close_Online_Dialog_007357e0(HWND,int);
void __fastcall Leave_Channel_With_Message_0074c5f0(HWND dialog)
{
    if(dialog) {
        if(unknown_a72c68==0) {
            Message_Box_005c3230(StringTable::Load_String(leave_channel_label_007fde60,0,wol_source_file_007fd62c,0x3c0f),0,connection_wait_callback_0073c120,false);
        } else {
            Message_Box_005c3230(StringTable::Load_String(observer_leave_label_007e6cdc,0,wol_source_file_007fd62c,0x3c11),0,connection_wait_callback_0073c120,false);
        }
        if(unknown_chat_a72cfc->unknown_slot09()<0) Close_Online_Dialog_007357e0(dialog,2);
    }
}


namespace OwnerDraw {
WideString __fastcall unknown_5e2160(HWND,int);
WideString __fastcall unknown_5e2260(unsigned int,HWND);
}
int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
void __stdcall unknown_dialog_5ecf40(HWND,int);
void __fastcall Refresh_Wonline_Players_0073fee0(int,bool);
wchar_t *__fastcall Player_Info_Tooltip_0074a120(wchar_t const *);
wchar_t *__fastcall Unknown_0074a4e0(wchar_t const *);
int __fastcall Unknown_0073cb80(WideString const &);
int __fastcall Unknown_0073ca50(WideString const &);
int __fastcall Parse_Page_Command_00746480(wchar_t const *);
int __fastcall Reply_To_Last_Pager_007466c0(wchar_t const *);
int __fastcall Parse_Online_Command_00746240(wchar_t const *);
extern "C" wchar_t *__cdecl wcsncpy(wchar_t *,wchar_t const *,unsigned);
extern "C" int __cdecl wcsncmp(wchar_t const *,wchar_t const *,unsigned);
extern wchar_t guest_empty_wide_839d4c[];
extern char guest_last_pager_b2ac48[];
extern int guest_action_color_7a7370,guest_private_action_color_7a736c;
extern int guest_chat_color_7a737c,guest_private_color_7a7368;
extern char guest_label_7fe9fc[];
extern char guest_label_7fe9e8[];
extern wchar_t guest_label_7fe2c4[];
extern char guest_label_7e6d74[];
extern char guest_label_7dfc7c[];
extern wchar_t guest_label_7d64f4[];
extern char guest_label_7fe9d4[];
extern char guest_label_7dfc64[];
extern wchar_t guest_label_7fe9b8[];
extern char guest_label_7dfc48[];
extern char guest_label_7dfc14[];
extern char guest_label_7dfbfc[];
extern wchar_t guest_label_7fe9a4[];
extern char guest_label_7fec94[];
extern char guest_label_7fec84[];
extern char guest_label_7fdea0[];
extern char guest_label_7e8fe8[];
extern char guest_label_7e8e28[];
extern char guest_label_7eb2e4[];
extern char guest_label_7eb2d8[];
extern char wol_source_file_007fd62c[];
int __stdcall Wonline_Guest_Dialog_0074c660(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    int handled=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(handled) return handled;
    switch(message) {
    case 0x84: {
        HWND tooltip=GetDlgItem(window,0x695);
        if(!tooltip) return 0;
        POINT point;
        point.x=lparam&0xffff;
        point.y=(unsigned)lparam>>16;
        RECT bounds;
        GetWindowRect(window,&bounds);
        point.x-=bounds.left;
        point.y-=bounds.top;
        WideString text;
        HWND child=ChildWindowFromPointEx(window,point,1);
        if(child) {
            text=OwnerDraw::unknown_5e2160(window,(int)child);
            if(Compare_Wide_007837d3(text.Get(),guest_empty_wide_839d4c)==0)
                text=OwnerDraw::unknown_5e2260((unsigned)window,child);
        }
        SendMessageA(tooltip,0x4b2,0,(LPARAM)text.unknown_text_770430());
        return 0;
    }
    case 2:
        if(ProgressScreen.Has_Window()) ProgressScreen.Clear_Window();
        if(unknown_dialog_preview_a72ce8) {
            delete unknown_dialog_preview_a72ce8;
            unknown_dialog_preview_a72ce8=0;
        }
        return 0;
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        if(unknown_dialog_preview_a72ce8) unknown_dialog_preview_a72ce8->Blit_Preview(window);
        if(ProgressScreen.Has_Window()) ProgressScreen.Draw_It(Point2D(-1,-1));
        ValidateRect(window,0);
        return 0;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0x111: {
        int command=wparam&0xffff;
        switch(command) {
        case 2:
            if(!unknown_b2ac40 && unknown_chat_a72cfc->unknown_slot09()<0) Unknown_007357E0(window,2);
            return 0;
        case 0x3ed: {
            HWND input=GetDlgItem(window,0x540);
            WideString text=Limit_Name_Text_005ccf50(input);
            SendMessageA(input,0x4b2,0,(LPARAM)guest_empty_wide_839d4c);
            SetFocus(input);
            InvalidateRect(input,0,0);
            int color=guest_action_color_7a7370;
            if(strlen(unknown_channel_b299c0.name)==0)
                Unknown_005C2C80(-1,StringTable::Load_String(guest_label_7fe9fc,0,wol_source_file_007fd62c,0x3dae));
            else if(text.Length()>0) {
                if(Unknown_0073cb80(text)) color=guest_private_action_color_7a736c;
                Unknown_005C2C80(color,guest_label_7fe2c4,unknown_wstring_b26cb8.Buffer,text.Get());
            } else Unknown_005C2C80(-1,StringTable::Load_String(guest_label_7fe9e8,0,wol_source_file_007fd62c,0x3db6));
            return 0;
        }
        case 0x59f: {
            unknown_players_a3fa7c[0]->unknown_67=1;
            char options[64];
            sprintf(options,guest_label_7e6d74);
            Send_Game_Options_005c3c60(options);
            Unknown_005cd030(window,false);
            HWND accept=GetDlgItem(window,0x59f);
            EnableWindow(accept,0);
            InvalidateRect(accept,0,0);
            SendMessageA(accept,0x4dc,0,0);
            Refresh_Wonline_Players_0073fee0(0,false);
            SetFocus(GetDlgItem(window,0x540));
            return 0;
        }
        case 0x540: {
            wchar_t nickname[16],primary_command[64],alternate_command[64];
            WideString text=Limit_Name_Text_005ccf50((HWND)lparam);
            if((unsigned short)(wparam>>16)==0x501) {
                SendMessageA((HWND)lparam,0x4b2,0,(LPARAM)guest_empty_wide_839d4c);
                if(text.Length()>0) {
                    int parsed=Parse_Page_Command_00746480(text.Get());
                    if(parsed!=0) {
                        if(parsed==1) {
                            int count=0;
                            unsigned length=text.Length()+1;
                            wchar_t *copy=(wchar_t *)operator new(length*2);
                            wcscpy(primary_command,StringTable::Load_String(guest_label_7dfc7c,0,wol_source_file_007fd62c,0x3d3b));
                            if(!copy) return 0;
                            copy=wcsncpy(copy,text.Get(),length);
                            if(!copy) return 0;
                            wchar_t *body=copy+wcslen(primary_command);
                            while(*body && wcsncmp(body,guest_label_7d64f4,1)==0) ++body;
                            while(*body) {
                                if(wcsncmp(body,guest_label_7d64f4,1)==0 || count>10) break;
                                nickname[count++]=*body++;
                            }
                            nickname[count]=0;
                            while(*body && wcsncmp(body,guest_label_7d64f4,1)==0) ++body;
                            if(count>10) Unknown_005C2C80(-1,StringTable::Load_String(guest_label_7fe9d4,0,wol_source_file_007fd62c,0x3d59));
                            else if(count==0) Unknown_005C2C80(-1,StringTable::Load_String(guest_label_7dfc64,0,wol_source_file_007fd62c,0x3d5b));
                            else if(wcslen(body)>0) Unknown_005C2C80(guest_private_color_7a7368,guest_label_7fe9b8,unknown_wstring_b26cb8.Buffer,nickname,body);
                            else Unknown_005C2C80(-1,StringTable::Load_String(guest_label_7dfc48,0,wol_source_file_007fd62c,0x3d61));
                            operator delete(copy);
                        }
                    } else if(Reply_To_Last_Pager_007466c0(text.Get())) {
                        unsigned length=text.Length()+1;
                        wchar_t *copy=(wchar_t *)operator new(length*2);
                        wcscpy(primary_command,StringTable::Load_String(guest_label_7dfc14,0,wol_source_file_007fd62c,0x3d73));
                        wcscpy(alternate_command,StringTable::Load_String(guest_label_7dfbfc,0,wol_source_file_007fd62c,0x3d74));
                        if(!copy) return 0;
                        copy=wcsncpy(copy,text.Get(),length);
                        if(!copy) return 0;
                        wchar_t *body=copy+(wcsncmp(copy,primary_command,wcslen(primary_command))==0 ? wcslen(primary_command):wcslen(alternate_command));
                        while(*body && wcsncmp(body,guest_label_7d64f4,1)==0) ++body;
                        wcscpy(nickname,Narrow_To_Wide_006fa180(guest_last_pager_b2ac48));
                        if(wcslen(nickname)>0 && wcslen(body)>0)
                            Unknown_005C2C80(guest_private_color_7a7368,guest_label_7fe9b8,unknown_wstring_b26cb8.Buffer,nickname,body);
                        else if(wcslen(nickname)>0)
                            Unknown_005C2C80(-1,StringTable::Load_String(guest_label_7dfc48,0,wol_source_file_007fd62c,0x3d91));
                        operator delete(copy);
                    } else if(!Parse_Online_Command_00746240(text.Get())) {
                        int color=guest_chat_color_7a737c;
                        if(Unknown_0073ca50(text)==1) color=guest_private_color_7a7368;
                        Unknown_005C2C80(color,guest_label_7fe9a4,unknown_wstring_b26cb8.Buffer,text.Get());
                    }
                }
            }
            return 0;
        }
        case 0x510: case 0x513: case 0x514:
        case 0x51e: case 0x51f: case 0x520: case 0x521: case 0x522: case 0x523:
        case 0x524: case 0x525: case 0x526: case 0x527: case 0x528:
        case 0x6a1: case 0x6a2:
            if((unsigned short)(wparam>>16)!=1) return 0;
        case 0x7fff: {
            Debug_Printf(guest_label_7fec94);
            int side_player=Side_Combo_Player_4d13b0(command);
            int color_player=Color_Combo_Player_4d1d10(command);
            int player=side_player>color_player?side_player:color_player;
            int side=Player_Side_Combo_ID_4d1350(player);
            int color=Player_Color_Combo_ID_4d1cb0(player);
            if(player==side_player) Set_Slot_Country_005c9a10(window,side);
            else Set_Slot_Color_005c9ab0(window,color);
            int requested_side=Side_Combo_Data_4d1c70(window,side,-1);
            int requested_color=Color_Combo_Data_4d2900(window,color,-1);
            unsigned char network[4],node[6];
            unsigned short socket;
            unknown_address_a3fbc8.Get_Address(network,node,&socket);
            char options[128];
            sprintf(options,guest_label_7fec84,requested_side,requested_color,*(unsigned int *)node,unknown_firewall_a71b18.unknown_00&0xffff,unknown_a40230);
            Debug_Printf(guest_label_7fdea0,options);
            Wstring host=Narrow_Copy_00771280(unknown_host_name_a3f618);
            Unknown_005c3d90(host.Get(),options);
            return 0;
        }
        }
        return 0;
    }
    case 0x497: {
        SetFocus(GetDlgItem(window,0x540));
        SendDlgItemMessageA(window,0x3ed,0x49c,0,(LPARAM)unknown_bitmap_writer_a76378.Get_Image_0068ca40(guest_label_7eb2e4,0));
        SendDlgItemMessageA(window,0x3ed,0x4aa,0,(LPARAM)unknown_bitmap_writer_a76378.Get_Image_0068ca40(guest_label_7eb2d8,0));
        MultiplayerGameMode *mode=MultiplayerGameMode::unknown_first_mode_5b6ec0();
        unknown_session_a3d298.GameModeIndex=mode->unknown_28;
        Unknown_005c3450(window,mode);
        unknown_session_a3d2b4=mode->FirstValidMapIndex();
        Select_Scenario_005c7c30(unknown_session_a3d2b4);
        Fill_Side_Combos_4d16d0(window);
        Init_Color_Entries_4d1ea0();
        Fill_Color_Combos_4d2300(window);
        Select_Scenario_005c7c30(-1);
        unknown_a72d52=false;
        unknown_int_a3db1c=8;
        Unknown_005c34b0(window);
        Unknown_005c7570(window);
        Unknown_Fill_Options_Dialog_005c3550(window,1);
        Refresh_Wonline_Players_0073fee0(0,false);
        return 0;
    }
    case 0x4e9: {
        OwnerDraw::UnknownTextRequest *request=(OwnerDraw::UnknownTextRequest *)lparam;
        int id;
        {
            WideString &text=request->text;
            text=(wchar_t const *)0;
            if(!request->unknown_00) goto tooltip_done;
            id=GetDlgCtrlID((HWND)request->unknown_00);
            if(request->unknown_04!=-1) {
                if(Control_Slot_005c9800(id)==-1) {
                    if(Side_Combo_Player_4d13b0(id)==-1) {
                        if(Color_Combo_Player_4d1d10(id)!=-1) {
                            text=Color_Name_4d1d80(Color_Combo_Data_4d2900(window,id,request->unknown_04));
                            return 1;
                        }
                    } else {
                        int country=Side_Combo_Data_4d1c70(window,id,request->unknown_04);
                        if(country!=-1) { text=Side_Name_4d1420(country); return 1; }
                    }
                } else {
                    NodeNameType *entry=(NodeNameType *)unknown_combo_selected_data_005c99d0(window,id,request->unknown_04);
                    text=Entry_Type_Name_005c9870(entry);
                    return 1;
                }
                goto tooltip_done;
            }
            if(Control_Slot_005c9800(id)!=-1) {
                NodeNameType *entry=(NodeNameType *)unknown_combo_selected_data_005c99d0(window,id,-1);
                if(entry) {
                    WideString label=OwnerDraw::unknown_5e2260((unsigned)window,(HWND)request->unknown_00);
                    if((Compare_Wide_007837d3(label.Get(),StringTable::Load_String(guest_label_7e8fe8,0,wol_source_file_007fd62c,0x3c92))==0 ||
                        Compare_Wide_007837d3(label.Get(),StringTable::Load_String(guest_label_7e8e28,0,wol_source_file_007fd62c,0x3c92))==0) && Unknown_005c9320(entry))
                        label=Player_Info_Tooltip_0074a120(entry->Name);
                    text=label.Get();
                }
            }
        }
        {
            int player;
            if(id==0x754) player=0;
            else if(id==0x755) player=1;
            else if(id==0x756) player=2;
            else if(id==0x757) player=3;
            else if(id==0x758) player=4;
            else if(id==0x759) player=5;
            else if(id==0x75a) player=6;
            else if(id==0x75b) player=7;
            else goto tooltip_done;
            WideString ping;
            int slot=Slot_Control_Id_005c97a0(player);
            if(slot!=-1) {
                NodeNameType *entry=(NodeNameType *)unknown_combo_selected_data_005c99d0(window,slot,-1);
                if(entry && Unknown_005c9320(entry)) {
                    ping=Unknown_0074a4e0(entry->Name);
                    request->text=ping;
                }
            }
        }
tooltip_done:
        return 1;
    }
    case 0x4ed:
        ShowWindow(window,0);
        unknown_dialog_5ecf40(window,1);
        return 0;
    }
    return 0;
}


long __stdcall Wonline_Wait_Dialog_0074d5e0(HWND window,unsigned message,unsigned wparam,long lparam);
extern bool connection_wait_callback_0073c120();
void __fastcall Open_Wait_Window_0074d4d0(unsigned int events,WideString const &message,bool block)
{
    HWND dialog=WS_Find_Dialog_00735940(0xc4);
    if(dialog) {
        unknown_b2abe0 |= events;
        if(message.Length()>0) {
            HWND text=GetDlgItem(dialog,0x5b2);
            SendMessageA(text,0x4b2,0,(LPARAM)message.unknown_text_770430());
        }
    } else if(events) {
        HWND focus=Unknown_00735B10();
        focus=GetDlgItem(focus,0x540);
        dialog=(HWND)unknown_create_dialog_735700(ProgramInstance,0xc4,MainWindow,Wonline_Wait_Dialog_0074d5e0,0);
        unknown_show_dialog_5ff7e0(dialog,0xc4);
        HWND text=GetDlgItem(dialog,0x5b2);
        SendMessageA(text,0x4b2,0,(LPARAM)message.unknown_text_770430());
        unknown_b2abe0=events;
        if(focus) SetFocus(focus);
    }
    if(block && unknown_b2abe0) Unknown_007359e0(dialog,connection_wait_callback_0073c120,0,1,0);
}


void __fastcall Wait_Paint_005e5f20(HWND window);
void __fastcall Unknown_005e6390(HWND window);
extern unsigned int wol_wait_delay_7fd384;
extern char wol_wait_url_7fecb4[];
long __stdcall Wonline_Wait_Dialog_0074d5e0(HWND window,unsigned message,unsigned wparam,long lparam)
{
    switch(message) {
    case 2:
        KillTimer(window,0x45);
        break;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0xf:
        Unknown_005FEE70(window);
        Wait_Paint_005e5f20(window);
        ValidateRect(window,0);
        break;
    case 0x14:
        return 1;
    case 0x110:
        return 0;
    case 0x111:
        switch(wparam & 0xffff) {
        case 0x58d:
            SetEvent(unknown_server_error_event_b264f8);
            Unknown_007357E0(window,1);
            break;
        case 0x58e:
            unknown_open_url_73c580(wol_wait_url_7fecb4,0);
            break;
        }
        break;
    case 0x113:
        if(wparam==0x45) {
            ShowWindow(window,5);
            KillTimer(window,0x45);
        }
        break;
    case 0x497:
        SendMessageA(window,0x4a9,0,1);
        SetTimer(window,0x45,wol_wait_delay_7fd384,0);
        Unknown_005e6390(window);
        break;
    }
    return 0;
}


// int __fastcall Unknown_0074A5D0(int id) (0x0074A5D0)
// The index of one of the eight consecutive ids 0x754..0x75B, else -1.
int __fastcall Unknown_0074A5D0(int id)
{
    if (id == 0x754) {
        return 0;
    }
    if (id == 0x755) {
        return 1;
    }
    if (id == 0x756) {
        return 2;
    }
    if (id == 0x757) {
        return 3;
    }
    if (id == 0x758) {
        return 4;
    }
    if (id == 0x759) {
        return 5;
    }
    if (id == 0x75A) {
        return 6;
    }
    return id == 0x75B ? 7 : -1;
}


// void __fastcall Close_Wonline_Wait(unsigned int flags) (0x0074D780): the unit's inline definition lives in
// Get_Channel_List_0073d8f0.cpp; Host_Game_007459a0 refuses it one inline level down, which emits this body.


extern char wol_broadcast_address_debug_7f1f94[];
extern char wol_session_address_debug_7fecd8[];
void __fastcall Users_To_Session_Players_0074d7d0(WOL::User *users)
{
    unknown_session_a3d55c=1;
    WOL::User user;
    for(int i=0;i<unknown_users_b2a728.Count;++i) {
        unknown_users_b2a728.Get_Copy(user,i);
        NodeNameType *player=Unknown_Find_Lobby_Name_005c8b20(Narrow_To_Wide_006fa180(user.name));
        if(player && Unknown_005c9320(player)) player->unknown_6b=user.unknown_14;
        if(_stricmp(user.name,unknown_wstring_b26cb8.Get())) {
            if(user.flags & 1) {
                NodeNameType *owner=Unknown_Find_Lobby_Name_005c8b20(Narrow_To_Wide_006fa180(user.name));
                NetNumType network;
                NetNodeType node;
                owner->Address.Get_Address(network,node);
                unknown_address_a3db2c.Set_Address(network,node,0);
            }
            int ping=0;
            unknown_ladder_client_a72d00->Get_Average_Ping(user.unknown_18,&ping);
            if(ping<0) ping=0;
            unknown_pings_b2a734[unknown_session_a3d55c++]=ping;
        }
    }
    PacketTransport->Clear_Broadcast_Addresses();
    for(i=1;i<unknown_players_a3fa7c.Count();++i) {
        Debug_Printf(wol_broadcast_address_debug_7f1f94,unknown_players_a3fa7c[i]->Address.As_String());
        PacketTransport->Set_Broadcast_Address((char *)&unknown_players_a3fa7c[i]->Address,0);
    }
    Debug_Printf(wol_session_address_debug_7fecd8,unknown_address_a3db2c.As_String());
}


bool __fastcall User_Is_Channel_Owner_0074d9e0(Wstring const &);
bool Local_User_Is_Channel_Owner_0074d960()
{
    return User_Is_Channel_Owner_0074d9e0(unknown_wstring_b26cb8);
}


bool __fastcall User_Is_Channel_Owner_0074d9e0(Wstring const &name)
{
    WOL::User *owner=Unknown_0074DA60();
    if(!owner) return false;
    if(_stricmp(owner->name,name.Peek_Buffer())==0) return true;
    return false;
}


// WOL::User *Unknown_0074DA60() (0x0074DA60)
// The first user of the user list whose flags have bit 0 set (the game host), copied into
// unknown_user_b2a658; 0 when there is none.

WOL::User *Unknown_0074DA60()
{
    for (int i = 0; i < unknown_users_b2a728.Count; ++i) {
        unknown_users_b2a728.Get_Copy(unknown_user_b2a658, i);
        if (unknown_user_b2a658.flags & 1) return &unknown_user_b2a658;
    }
    return 0;
}


extern int unknown_player_count_a3fa8c;
extern int unknown_player_controls_7fd334[8];
extern bool unknown_player_checked_b28dcc[8];
void __fastcall Mark_Player_Checked_0074dab0(int index)
{
    if(index>=0 && index<unknown_player_count_a3fa8c) {
        HWND dialog=(HWND)WS_Find_Dialog_00735940(0x12c);
        if(dialog) {
            HWND control=GetDlgItem(dialog,unknown_player_controls_7fd334[index]);
            unknown_player_checked_b28dcc[index]=true;
            SendMessageA(control,0xf1,1,0);
            PostMessageA(dialog,0xf,0,0);
        }
    }
}


namespace OwnerDraw { void __fastcall unknown_5e6390(HWND); }
extern int unknown_player_controls_7fd334[8];
extern bool unknown_player_checked_b28dcc[8];
extern bool unknown_player_ready_b28dc4[9];
long __stdcall Player_Readiness_Dialog_0074db40(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    switch(message) {
    case 0xf:
        Unknown_005FEE70(window);
        ValidateRect(window,0);
        return 0;
    case 0x14:
        return 1;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0x111:
        wparam &= 0xffff;
        switch(wparam) {
        case 0x761: case 0x762: case 0x764: case 0x765:
        case 0x766: case 0x767: case 0x768: case 0x769:
            if(unknown_player_ready_b28dc4[wparam-0x761]) {
                if(SendDlgItemMessageA(window,wparam,0xf0,0,0)==0)
                    SendDlgItemMessageA(window,wparam,0xf1,1,0);
            } else {
                if(SendDlgItemMessageA(window,wparam,0xf0,0,0)==1) {
                    SendDlgItemMessageA(window,wparam,0xf1,0,0);
                    return 0;
                }
            }
        }
        break;
    case 0x132: case 0x133: case 0x134: case 0x135:
    case 0x136: case 0x137: case 0x138:
        return (long)GetStockObject(4);
    case 0x497:
        for(int i=0;i<8;++i) {
            HWND control=GetDlgItem(window,unknown_player_controls_7fd334[i]);
            memset(unknown_player_checked_b28dcc,0,8);
            if(i>unknown_players_a3fa7c.Count()-1) DestroyWindow(control);
        }
        for(int player=0;player<unknown_players_a3fa7c.Count();++player) {
            HWND control=GetDlgItem(window,unknown_player_controls_7fd334[player]);
            SendMessageA(control,0x4b2,0,(LPARAM)unknown_players_a3fa7c[player]->Name);
            SendMessageA(control,0xf1,0,0);
        }
        SendMessageA(window,0x4a9,0,1);
        OwnerDraw::unknown_5e6390(window);
        ShowWindow(window,5);
        break;
    }
    return 0;
}


// void Unknown_0074dd00() (0x0074DD00)
// Ends dialog 0x12C (result 0) when it is open.

void Unknown_0074dd00()
{
    void *dialog = WS_Find_Dialog_00735940(0x12C);
    if (dialog) Unknown_007357E0(dialog, 0);
}


extern "C" int __cdecl swprintf(wchar_t *,wchar_t const *,...);
extern int wol_connection_status_b26a2c[];
extern char wol_source_file_007fd62c[];
extern char connect_player_debug_007fee6c[];
extern char connect_player_label_007fee50[];
extern char connect_remote_debug_007fee30[];
extern char connect_remote_label_007fee18[];
extern char connect_proxy_debug_007fedf4[];
extern char connect_proxy_label_007fedd4[];
extern char connect_peer_proxy_debug_007fed9c[];
extern char connect_peer_proxy_label_007fed7c[];
extern char connect_host_debug_007fed3c[];
extern char connect_host_label_007fed24[];
extern char connect_unknown_label_007fed10[];
void Report_Connection_Errors_0074dd50()
{
    wchar_t message[256];
    WOL::User *owner=0;
    int i;
    for(i=0;i<unknown_users_b2a728.Count;++i) {
        unknown_users_b2a728.Get_Copy(unknown_user_b2a658,i);
        if(unknown_user_b2a658.flags&1) {
            owner=&unknown_user_b2a658;
            break;
        }
    }
    if(owner) {
        int owner_index=-1;
        for(i=0;i<unknown_players_a3fa7c.Count();++i) {
            if(!_stricmp(Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name),owner->name)) {
                owner_index=i;
                break;
            }
        }
        bool reported=false;
        for(i=0;i<unknown_players_a3fa7c.Count();++i) {
            switch(wol_connection_status_b26a2c[i]) {
            case 0:
                reported=true;
                Debug_Printf(connect_player_debug_007fee6c,Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name));
                swprintf(message,StringTable::Load_String(connect_player_label_007fee50,0,wol_source_file_007fd62c,0x41fe),unknown_players_a3fa7c[i]->Name);
                Unknown_005C2C80(unknown_color_7a7360,message);
                break;
            case 1:
                reported=true;
                Debug_Printf(connect_remote_debug_007fee30,Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name));
                swprintf(message,StringTable::Load_String(connect_remote_label_007fee18,0,wol_source_file_007fd62c,0x4208),unknown_players_a3fa7c[i]->Name);
                Unknown_005C2C80(unknown_color_7a7360,message);
                break;
            case 2:
                reported=true;
                if(i==owner_index) {
                    Debug_Printf(connect_proxy_debug_007fedf4);
                    Unknown_005C2C80(unknown_color_7a7360,StringTable::Load_String(connect_proxy_label_007fedd4,0,wol_source_file_007fd62c,0x4213));
                } else {
                    Debug_Printf(connect_peer_proxy_debug_007fed9c,Wide_To_Narrow_006fa120(unknown_players_a3fa7c[i]->Name));
                    swprintf(message,StringTable::Load_String(connect_peer_proxy_label_007fed7c,0,wol_source_file_007fd62c,0x4216),unknown_players_a3fa7c[i]->Name);
                    Unknown_005C2C80(unknown_color_7a7360,message);
                }
                break;
            case 3:
                reported=true;
                Debug_Printf(connect_host_debug_007fed3c);
                Unknown_005C2C80(unknown_color_7a7360,StringTable::Load_String(connect_host_label_007fed24,0,wol_source_file_007fd62c,0x4221));
                break;
            }
        }
        if(!reported) Unknown_005C2C80(unknown_color_7a7360,StringTable::Load_String(connect_unknown_label_007fed10,0,wol_source_file_007fd62c,0x422e));
    }
}


extern char blitz_port_debug_007fee9c[];
unsigned short __fastcall Compute_Blitz_Port_0074dfd0(int sender,int base,int unknown,int step)
{
    int port=sender+base;
    if(step!=1) {
      if(step==2) {
        port=base+sender*2;
    } else if(step>1) {
        port=sender%2+base+(sender/2)*step;
    } else if(step<0) {
        if(step==-100) port=base+sender*100;
        else port=sender%2-(sender/2)*step+base;
    }
    }
    Debug_Printf(blitz_port_debug_007fee9c,sender,base,step,port);
    if((unsigned short)port<1024) port=1024;
    return (unsigned short)port;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Adapted for base RA2 0074E070 from ra2-decomp GAMEMD 007937B0,
// pinned 4453ab030c86d8586992bfa7425ef7f39e3fb8d4; shared names/ABI preserved.

// Prioritizes active player slots by firewall weight and writes every unordered pair (0074E070).
namespace seed_005c9320 { unsigned int __fastcall Is_Real_Player_Entry_005c9320(int entry); }

int __fastcall Unknown_0074e070(void *output)
{
	int (*pairs)[2] = (int (*)[2])output;
	int order[8];
	memset(order, 0, sizeof(order));
	int i = 0;
	int * out = order;
	for (; i < 8; i++) {
		if ((unsigned char)seed_005c9320::Is_Real_Player_Entry_005c9320((int)unknown_player_slots_a3fa98[i])) {
			*out++ = i;
		}
	}
	for (int a = 0; a < unknown_players_a3fa7c.Count(); a++) {
		for (int b = 0; b < unknown_players_a3fa7c.Count() - 1; b++) {
			int cur_value = unknown_session_a3d298.unknown_299c.unknown[order[b]];
			int next_value = unknown_session_a3d298.unknown_299c.unknown[order[b + 1]];
			if (unknown_firewall_a71b18.Connection_Weight_005baee0(cur_value) < unknown_firewall_a71b18.Connection_Weight_005baee0(next_value)) {
				int temp = order[b];
				order[b] = order[b + 1];
				order[b + 1] = temp;
			}
		}
	}
	Debug_Printf("Connection order = %d, %d, %d, %d, %d, %d\n", order[0], order[1], order[2], order[3], order[4], order[5]);
	int n = 0;
	Debug_Printf("Connection pairs....\n");
	for (int x = 0; x < unknown_players_a3fa7c.Count(); x++) {
		for (int y = x + 1; y < unknown_players_a3fa7c.Count(); y++) {
			pairs[n][0] = order[x];
			pairs[n][1] = order[y];
			Debug_Printf("\t%d & %d\n", order[x], order[y]);
			n++;
		}
	}
	return n;
}


extern char connection_state_debug_007fef4c[];
extern int unknown_connection_pairs_a3fc54[][2];
extern int unknown_connection_states_a3fe54[][2];
extern int unknown_connection_pair_count_a40054;
void __fastcall Register_Connection_State_0074e1f0(char const *sender,char const *receiver,int state)
{
    int sender_slot=-1;
    int receiver_slot=-1;
    Debug_Printf(connection_state_debug_007fef4c,sender,receiver,state);
    int i=0;
    for(NodeNameType **slot=unknown_player_slots_a3fa98;(long)slot<(long)(unknown_player_slots_a3fa98+8);++slot,++i) {
        if(_stricmp(Wide_To_Narrow_006fa120((*slot)->Name),sender)==0) sender_slot=i;
        if(_stricmp(Wide_To_Narrow_006fa120((*slot)->Name),receiver)==0) receiver_slot=i;
    }
    for(i=0;i<unknown_connection_pair_count_a40054;++i) {
        if(unknown_connection_pairs_a3fc54[i][0]==sender_slot && unknown_connection_pairs_a3fc54[i][1]==receiver_slot) {
            unknown_connection_states_a3fe54[i][0]=state;
            return;
        }
        if(unknown_connection_pairs_a3fc54[i][0]==receiver_slot && unknown_connection_pairs_a3fc54[i][1]==sender_slot) {
            unknown_connection_states_a3fe54[i][1]=state;
            return;
        }
    }
}


extern unsigned short unknown_player_ports_a40058[8];
extern char player_port_message_7fe030[];
void __fastcall Store_Player_Port_0074e2d0(char const *name,unsigned short port)
{
    Debug_Printf(player_port_message_7fe030,name,port);
    for(int i=0;i<8;++i) {
        if(!_stricmp(Wide_To_Narrow_006fa120((wchar_t const *)unknown_player_slots_a3fa98[i]),name)) {
            unknown_player_ports_a40058[i]=port;
            return;
        }
    }
}


extern char private_port_format_007ce9bc[];
extern char private_port_debug_007fef8c[];
void __fastcall Send_User_Port_0074e330(char const *name,unsigned short port)
{
    WOL::User *user=0;
    char text[256];
    for(int i=0;i<unknown_users_b2a728.Count;++i) {
        unknown_users_b2a728.Get(user,i);
        if(_stricmp(name,user->name)==0) {
            text[0]='Z';
            text[1]='4';
            sprintf(text+2,private_port_format_007ce9bc,port);
            Debug_Printf(private_port_debug_007fef8c,text,name);
            unknown_chat_a72cfc->unknown_slot14(user,text);
        }
    }
}


extern char public_options_message_7e71f0[];
void __fastcall Send_Public_Options_0074e3f0(char kind,char const *text)
{
    char options[256];
    options[0]='Z';
    options[1]='1';
    options[2]=kind+'0';
    if(text) strcpy(options+3,text);
    else options[3]=0;
    Debug_Printf(public_options_message_7e71f0,options);
    unknown_chat_a72cfc->unknown_slot15(options);
}


// Resends command 13 to each expired pending destination; operation name is a hypothesis.

extern char const unknown_retry_format_7fefac[];
extern Wstring unknown_wstring_b26cb8;

void Retry_Requests_0074e470()
{
    for (int index = 0; index < unknown_session_a3d298.unknown_2f94; ++index) {
        if (unknown_session_a3d298.unknown_2f34[index] == 0) {
            UnknownGlobalPacket packet;
            memset(&packet, 0, sizeof(packet));
            packet.Command = 13;
            strcpy(packet.Name, unknown_wstring_b26cb8.Get());
            char const *name = Wide_To_Narrow_006fa120((unsigned short const *)unknown_player_slots_a3fa98[unknown_session_a3d298.unknown_2f14[index]]);
            Debug_Printf(unknown_retry_format_7fefac, name);
            unknown_ipx_manager_a40978.unknown_5259a0(&packet, 0x2f, 0,
                (wchar_t const *)unknown_player_slots_a3fa98[unknown_session_a3d298.unknown_2f14[index]]);
            unknown_session_a3d298.unknown_2f34[index] = CDTimerClass<SystemTimerClass>(1200);
        }
    }
}


void __fastcall Queue_Peer_Timeout_0074e590(int peer)
{
    unknown_session_a3d298.unknown_2f14[unknown_session_a3d298.unknown_2f94]=peer;
    unknown_session_a3d298.unknown_2f34[unknown_session_a3d298.unknown_2f94]=CDTimerClass<SystemTimerClass>(1200);
    ++unknown_session_a3d298.unknown_2f94;
}


void Unknown_0074e470();
bool __fastcall Unknown_0074e5e0(int peer,int pair_slot,int step);
extern FirewallSettings wol_firewall_a71b18;
extern int unknown_player_controls_7fd334[8];
extern bool unknown_player_checked_b28dcc[8];
extern char wol_pair_debug_007ff270[],wol_wait_pair_007ff24c[],wol_pair_done_007ff1ac[];
extern char wol_wait_first_007ff218[],wol_wait_second_007ff1e4[],wol_wait_done_007ff1c8[];
bool __fastcall Connect_Player_Pairs_0074ef80()
{
    CDTimerClass<SystemTimerClass> timer;
    unknown_session_a3d298.unknown_2f94=0;
    memset(unknown_session_a3d298.unknown_2ef4,0,sizeof(unknown_session_a3d298.unknown_2ef4));
    int pair_slot=-1;
    unknown_login_state_00b2abe4=0;
    if(unknown_session_a3d298.unknown_2dbc==0) return false;
    unknown_session_a3d298.Allocate_Ports_00673090();
    if(unknown_players_a3fa7c.Count()>0) {
        HWND window=(HWND)WS_Find_Dialog_00735940(300);
        if(window) {
            HWND control=GetDlgItem(window,unknown_player_controls_7fd334[0]);
            unknown_player_checked_b28dcc[0]=true;
            SendMessageA(control,0xf1,1,0);
            PostMessageA(window,0xf,0,0);
        }
    }
    for(int pair=0;pair<unknown_session_a3d298.unknown_2dbc;++pair) {
        int first=unknown_session_a3d298.unknown_29bc[pair*2];
        int second=unknown_session_a3d298.unknown_29bc[pair*2+1];
        int peer=-1;
        NodeNameType *first_player=unknown_player_slots_a3fa98[first];
        NodeNameType *second_player=unknown_player_slots_a3fa98[second];
        char const *first_name=Wide_To_Narrow_006fa120(first_player->Name);
        char const *second_name=Wide_To_Narrow_006fa120(second_player->Name);
        Debug_Printf(wol_pair_debug_007ff270,pair,first_name,second_name);
        char const *name=Wide_To_Narrow_006fa120(unknown_player_slots_a3fa98[first]->Name);
        if(_stricmp(name,unknown_wstring_b26cb8.Get())==0) {
            peer=second;
            pair_slot=pair*2+1;
            unknown_session_a3d298.unknown_2bbc[pair*2]=11;
        }
        name=Wide_To_Narrow_006fa120(unknown_player_slots_a3fa98[second]->Name);
        if(_stricmp(name,unknown_wstring_b26cb8.Get())==0) {
            peer=first;
            pair_slot=pair*2;
            unknown_session_a3d298.unknown_2bbc[pair*2+1]=11;
        }
        if(peer!=-1) {
            int step;
            if(wol_firewall_a71b18.Connection_Step_005baf20(unknown_session_a3d298.unknown_299c.unknown[first]) > wol_firewall_a71b18.Connection_Step_005baf20(unknown_session_a3d298.unknown_299c.unknown[second])) step=wol_firewall_a71b18.Connection_Step_005baf20(unknown_session_a3d298.unknown_299c.unknown[first]);
            else step=wol_firewall_a71b18.Connection_Step_005baf20(unknown_session_a3d298.unknown_299c.unknown[second]);
            if(!Unknown_0074e5e0(peer,pair_slot,step)) return false;
        } else {
            Debug_Printf(wol_wait_pair_007ff24c);
            timer=CDTimerClass<SystemTimerClass>(960);
            int progress=0;
            do {
                Sleep(5);
                Unknown_0073e100();
                Unknown_0074e470();
                int first_status=unknown_session_a3d298.unknown_2bbc[pair*2];
                int second_status=unknown_session_a3d298.unknown_2bbc[pair*2+1];
                if((first_status==11 || first_status==0) && (second_status==11 || second_status==0)) {
                    Debug_Printf(wol_wait_done_007ff1c8);
                    break;
                }
                if(first_status>=1 && first_status<11 && first_status-1>progress) {
                    Debug_Printf(wol_wait_first_007ff218,first_status-1);
                    progress=first_status-1;
                    timer=CDTimerClass<SystemTimerClass>(840);
                }
                if(second_status>=1 && second_status<11 && second_status-1>progress) {
                    Debug_Printf(wol_wait_second_007ff1e4,second_status-1);
                    progress=second_status-1;
                    timer=CDTimerClass<SystemTimerClass>(840);
                }
            } while(timer.Value()!=0);
        }
        if(unknown_session_a3d298.unknown_2bbc[pair*2]!=11) return false;
        if(unknown_session_a3d298.unknown_2bbc[pair*2+1]!=11) return false;
        Debug_Printf(wol_pair_done_007ff1ac);
    }
    return true;
}


long __stdcall Player_Readiness_Dialog_0074db40(HWND,UINT,WPARAM,LPARAM);
bool __fastcall Connect_Player_Pairs_0074ef80();
int __fastcall Sync_Scenario_With_Guests_00750330(bool);
void Report_Connection_Errors_0074dd50();
bool __fastcall Unknown_00514EE0();
extern int wol_connection_status_b26a2c[8];
extern int unknown_b2ac2c,unknown_b2ab88;
extern long unknown_b2a8a4;
extern HANDLE game_exit_event_b26510,game_start_event_b26514;
extern bool use_router_a402d0;
extern int game_port_7f60dc;
extern char start_text_007ff3e8[];
extern char start_text_007ff3c4[];
extern char start_text_007ff3a0[];
extern char start_text_007ff38c[];
extern char start_text_007ff34c[];
extern char start_text_007ff334[];
extern char start_text_007ff314[];
extern char start_text_007d68f4[];
extern char start_text_007ff2fc[];
extern char start_text_007ff2d0[];
extern char start_text_007ff2b8[];
extern char start_text_007ff290[];
extern char start_text_007e38bc[];
extern char start_text_007fd62c[];
extern char start_text_007ec57c[];
extern char start_text_007ce9c0[];
void __fastcall Start_Game_Now_0074f300()
{
    Debug_Printf(start_text_007ff3e8);
    unknown_b2ac44=true;
    PacketTransport->Discard_In_Buffers();
    PacketTransport->Discard_Out_Buffers();
    memset(wol_connection_status_b26a2c,0xff,sizeof(wol_connection_status_b26a2c));
    void *dialog=unknown_create_dialog_735700(ProgramInstance,300,MainWindow,Player_Readiness_Dialog_0074db40,0);
    if(dialog) unknown_show_dialog_5ff7e0(dialog,300);
    for(int n=0;n<100;++n) Service_Game_IO_00481fb0();
    unknown_ipx_manager_a40978.unknown_527830(300);
    bool connected=Connect_Player_Pairs_0074ef80();
    if(!connected) {
        unknown_login_state_00b2abe4=0;
        WOL::User *owner=0;
        for(int i=0;i<unknown_users_b2a728.Count;++i) {
            unknown_users_b2a728.Get(unknown_user_b2a658,i);
            if(unknown_user_b2a658.flags&1) { owner=&unknown_user_b2a658; break; }
        }
        if(!owner || _stricmp(owner->name,unknown_wstring_b26cb8.Peek_Buffer())) Debug_Printf(start_text_007ff3c4);
    }
    int transfer=Sync_Scenario_With_Guests_00750330(connected);
    if(transfer) {
        Debug_Printf(start_text_007ff3a0);
        if(transfer==2) Message_Box_005c3230(StringTable::Load_String(start_text_007ec57c,0,start_text_007fd62c,0x474b),0,Unknown_0073c120,false);
        HWND wait=WS_Find_Dialog_00735940(300);
        if(wait) End_Dialog_007357e0(wait,0);
        unknown_b2ac40=0;
        if(unknown_a3d2a4==2 || unknown_a3d2a4==1) {
            unknown_b2ac2c=0;
            unknown_b26898=11;
            unknown_b2ab88=-1;
            unknown_b2a8a4=time(0);
        }
        unknown_b2ac04=false;
        Report_Connection_Errors_0074dd50();
        Unknown_00514EE0();
        unknown_b2ac45=false;
        unknown_b2ac44=false;
        if(transfer!=2) return;
        unknown_b2abd9=true;
        Debug_Printf(start_text_007ff38c);
        SetEvent(game_exit_event_b26510);
        Debug_Printf(start_text_007ff34c);
        unknown_chat_a72cfc->unknown_slot09();
        return;
    }
    for(int i=0;i<unknown_session_a3d298.Players.Count();++i) {
        if(!unknown_session_a3d298.unknown_3022.unknown[i]) unknown_session_a3d298.unknown_3022.unknown[i]=(unsigned short)unknown_default_port_7f60d0;
    }
    WOL::User *owner=Unknown_0074DA60();
    if(owner) {
        for(int j=0;j<unknown_session_a3d298.Players.Count();++j) {
            if(!_stricmp(Wide_To_Narrow_006fa120(unknown_session_a3d298.Players[j]->Name),owner->name)) {
                unknown_session_a3d298.unknown_292c=unknown_session_a3d298.unknown_3022.unknown[j];
                break;
            }
        }
    }
    HWND wait=WS_Find_Dialog_00735940(300);
    if(wait) End_Dialog_007357e0(wait,0);
    if(unknown_session_a3d298.Type!=4 || unknown_a3d2a4!=2) unknown_session_a3d298.unknown_124=0;
    unknown_session_a3d298.unknown_2cc=5;
    switch(unknown_session_a3d298.GameSpeed_30) {
    case 0: unknown_session_a3d298.MaxAhead_2c8=40;unknown_session_a3d298.unknown_2e8=60;unknown_session_a3d298.unknown_2cc=10;break;
    case 1: unknown_session_a3d298.MaxAhead_2c8=40;unknown_session_a3d298.unknown_2e8=45;unknown_session_a3d298.unknown_2cc=10;break;
    case 2: unknown_session_a3d298.MaxAhead_2c8=30;unknown_session_a3d298.unknown_2e8=30;break;
    case 3: unknown_session_a3d298.MaxAhead_2c8=20;unknown_session_a3d298.unknown_2e8=20;break;
    case 4: unknown_session_a3d298.MaxAhead_2c8=20;unknown_session_a3d298.unknown_2e8=15;break;
    case 5: unknown_session_a3d298.MaxAhead_2c8=20;unknown_session_a3d298.unknown_2e8=12;break;
    default: unknown_session_a3d298.MaxAhead_2c8=10;unknown_session_a3d298.unknown_2e8=10;break;
    }
    if(use_router_a402d0) unknown_session_a3d298.MaxAhead_2c8+=20;
    unknown_session_a3d298.unknown_2e4=0;
    owner=Unknown_0074DA60();
    if(!owner || _stricmp(owner->name,unknown_wstring_b26cb8.Peek_Buffer())) unknown_session_a3d298.unknown_2e8=0;
    int worst_ping=0;
    for(int p=0;p<8;++p) if(unknown_pings_b2a734[p]>worst_ping) worst_ping=unknown_pings_b2a734[p];
    if(use_router_a402d0) worst_ping*=2;
    unknown_session_a3d298.unknown_290c=1;
    if(worst_ping!=0 && worst_ping!=1000) {
        Debug_Printf(start_text_007e38bc,worst_ping);
        if(worst_ping>1000) unknown_session_a3d298.unknown_290c=2;
        else if(worst_ping<600) unknown_session_a3d298.unknown_290c=1;
        int timing=60*worst_ping/1000;
        timing+=120*timing/100;
        unknown_ipx_manager_a40978.Set_Timing(timing,(unsigned long)-1,timing*8,true);
        owner=Unknown_0074DA60();
        if(owner && !_stricmp(owner->name,unknown_wstring_b26cb8.Peek_Buffer())) {
            switch(unknown_session_a3d298.unknown_290c) {
            case 0:case 1:unknown_session_a3d298.unknown_2e4=unknown_session_a3d298.MaxAhead_2c8;break;
            case 2:unknown_session_a3d298.unknown_2e4=unknown_session_a3d298.MaxAhead_2c8+(unknown_session_a3d298.MaxAhead_2c8>>1);break;
            case 3:unknown_session_a3d298.unknown_2e4=unknown_session_a3d298.MaxAhead_2c8*2;break;
            }
            unknown_session_a3d298.unknown_2e4=((unsigned int)unknown_session_a3d298.unknown_2e4/10)*10;
        }
    } else {
        Debug_Printf(start_text_007ff2d0);
        unknown_session_a3d298.unknown_2e4=(unknown_session_a3d298.MaxAhead_2c8/10)*10;
        if(unknown_session_a3d298.unknown_2e4<=10) unknown_session_a3d298.unknown_2e4=10;
    }
    Debug_Printf(start_text_007ff334,unknown_session_a3d298.unknown_2e4);
    Debug_Printf(start_text_007ff314,unknown_session_a3d298.unknown_2e8);
    Debug_Printf(start_text_007d68f4,unknown_session_a3d298.unknown_290c);
    Debug_Printf(start_text_007ff2fc,unknown_session_a3d298.unknown_2cc);
    unknown_session_a3d298.unknown_2e0=unknown_session_a3d298.MaxAhead_2c8;
    memset(unknown_session_a3d298.unknown_2ec,0,sizeof(player_statistics_00a3d584));
    unknown_a40b18=unknown_session_a3d298.GameSpeed_30;
    unknown_default_port_7f60d0=1234;
    unknown_port_7f60d4=1235;
    unknown_alternate_port_7f60d8=1236;
    game_port_7f60dc=1237;
    Frame=0;
    if(use_router_a402d0) unknown_OwnerTalkClass_00ab6d18.Wait_For_Router_Shutdown(2000,true);
    unknown_squad_1_b2a820[0]=0;
    unknown_squad_2_b293d8[0]=0;
    if(unknown_session_a3d298.Type==4) {
        bool squads=Unknown_0073c3a0();
        if(squads) {
            WOL::User *user=0;
            int first_squad=0;
            int second_squad=0;
            int i=0;
            if(unknown_users_b2a728.Count<=0) goto squads_done;
squad_next:
            {
                unknown_users_b2a728.Get(user,i);
                if(first_squad==0 || first_squad==(int)user->unknown_14) {
                    strcat(unknown_squad_1_b2a820,user->name);
                    strcat(unknown_squad_1_b2a820,start_text_007ce9c0);
                    first_squad=user->unknown_14;
                } else if(second_squad==0 || second_squad==(int)user->unknown_14) {
                    strcat(unknown_squad_2_b293d8,user->name);
                    strcat(unknown_squad_2_b293d8,start_text_007ce9c0);
                    second_squad=user->unknown_14;
                }
            
            }
            ++i;
            if(i<unknown_users_b2a728.Count) goto squad_next;
squads_done:;
        }
        if(unknown_a3d2a4==2) {
            WOL::User *user=0;
            for(int i=0;i<unknown_users_b2a728.Count;++i) {
                unknown_users_b2a728.Get(user,i);
                if(unknown_player_b29cf0[i].unknown_18==0) {
                    strcat(unknown_squad_1_b2a820,user->name);
                    strcat(unknown_squad_1_b2a820,start_text_007ce9c0);
                } else {
                    strcat(unknown_squad_2_b293d8,user->name);
                    strcat(unknown_squad_2_b293d8,start_text_007ce9c0);
                }
            }
        }
    }
    Debug_Printf(start_text_007ff2b8);
    SetEvent(game_start_event_b26514);
    if(unknown_a3d2a4==2 || unknown_a3d2a4==1) {
        unknown_b2ac2c=0;
        unknown_b26898=11;
        unknown_b2ab88=-1;
        unknown_b2a8a4=time(0);
    }
    unknown_b2abd9=true;
    Debug_Printf(start_text_007ff290);
    unknown_chat_a72cfc->unknown_slot09();
    Unknown_Pregame_Setup_005c74f0();
    unknown_b2ac45=false;
    unknown_b2ac44=false;
}


extern "C" int __cdecl sscanf(char const *,char const *,...);
typedef int (WINAPI *AddressFarProc)();
WIN32_IMPORT HINSTANCE WINAPI LoadLibraryA(char const *name);
WIN32_IMPORT BOOL WINAPI FreeLibrary(HINSTANCE module);
WIN32_IMPORT AddressFarProc WINAPI GetProcAddress(HINSTANCE module,char const *name);

// Winsock IN_ADDR: exact four-byte SDK union, with named octets.
struct in_addr {
    union {
        struct { unsigned char s_b1,s_b2,s_b3,s_b4; } S_un_b;
        struct { unsigned short s_w1,s_w2; } S_un_w;
        unsigned long S_addr;
    } S_un;
};

// RTTI already names VectorClass<tConnInfoStruct *>; the20B record is
// written column-by-column from the TCP connection MIB in this routine.
class tConnInfoStruct {
public:
    long State;
    unsigned long LocalAddress;
    unsigned short LocalPort;
    unsigned long RemoteAddress;
    unsigned short RemotePort;
};
// SNMP ABI types, checked against Microsoft snmp.h documentation and
// original8B list/24B binding allocations. No reference implementation copied.
struct AsnObjectIdentifier { unsigned int idLength; unsigned int *ids; };
struct AsnOctetString { unsigned char *stream; unsigned int length; BOOL dynamic; };
struct AsnCounter64 { unsigned long low,high; };
struct AsnAny {
    unsigned char asnType;
    union {
        long number;
        unsigned long unsigned32;
        AsnCounter64 counter64;
        AsnOctetString string;
        AsnObjectIdentifier object;
        AsnOctetString address;
    } asnValue;
};
struct SnmpVarBind { AsnObjectIdentifier name; AsnAny value; };
struct SnmpVarBindList { SnmpVarBind *list; unsigned int len; };
typedef BOOL (WINAPI *AddressSnmpInit)(DWORD,HANDLE *,AsnObjectIdentifier *);
typedef BOOL (WINAPI *AddressSnmpQuery)(unsigned char,SnmpVarBindList *,long *,long *);
typedef void *(WINAPI *AddressSnmpAlloc)(unsigned int);
typedef void (WINAPI *AddressSnmpFree)(void *);
extern "C" {
    extern AddressSnmpInit address_snmp_init_b2a754;
    extern AddressSnmpQuery address_snmp_query_b294b0;
    extern AddressSnmpAlloc address_snmp_alloc_b2a718;
    extern AddressSnmpFree address_snmp_free_b2a8dc;
}
extern char address_text_007ce9bc[];
extern char address_text_007e6554[];
extern char address_text_007ff468[];
extern char address_text_007ff488[];
extern char address_text_007ff4bc[];
extern char address_text_007ff4e8[];
extern char address_text_007ff514[];
extern char address_text_007ff530[];
extern char address_text_007ff54c[];
extern char address_text_007ff588[];
extern char address_text_007ff5c0[];
extern char address_text_007ff5e8[];
extern char address_text_007ff60c[];
extern char address_text_007ff628[];
extern char address_text_007ff644[];
extern char address_text_007ff654[];
extern char address_text_007ff668[];
extern char address_text_007ff67c[];
extern char address_text_007ff690[];
extern char address_text_007ff6ac[];
extern char address_text_007ff6b8[];
extern char address_text_007ff6d4[];
extern char address_text_007ff6f4[];
extern char address_text_007ff704[];
extern char address_text_007ff720[];
extern char address_text_007ff740[];
extern char address_text_007ff760[];
extern char address_text_007ff778[];
extern char address_text_007ff798[];

bool __fastcall Find_Local_Address_0074fcb0(IPXAddressClass *address)
{
    DynamicVectorClass<tConnInfoStruct *> connections;
    int server_port=0;
    NetNumType network;
    NetNodeType node;
    if(unknown_irc_index_b292d8<1 || unknown_irc_index_b292d8>=unknown_servers_b2a700.Count) return false;
    Debug_Printf(address_text_007ff798);
    WOL::Server *server=0;
    unknown_servers_b2a700.Get(server,unknown_irc_index_b292d8);
    char hostname[128];
    memset(hostname,0,sizeof(hostname));
    strncpy(hostname,server->connection+4,128);
    char *port=strchr(hostname,';');
    if(port) {
        *port=0;
        sscanf(port+1,address_text_007ce9bc,&server_port);
        Debug_Printf(address_text_007ff778,hostname);
        Debug_Printf(address_text_007ff760,server_port);
        Debug_Printf(address_text_007ff740);
        hostent *host=gethostbyname(hostname);
        if(!host) { Debug_Printf(address_text_007e6554,WSAGetLastError());return false; }
        in_addr remote_address;
        remote_address=*(in_addr *)host->h_addr_list[0];
        remote_address.S_un.S_addr=ntohl(remote_address.S_un.S_addr);
        Debug_Printf(address_text_007ff720,remote_address.S_un.S_un_b.s_b4,remote_address.S_un.S_un_b.s_b3,remote_address.S_un.S_un_b.s_b2,remote_address.S_un.S_un_b.s_b1);
        Debug_Printf(address_text_007ff704);
        HINSTANCE extension=LoadLibraryA(address_text_007ff6f4);
        if(!extension) { Debug_Printf(address_text_007ff6d4);return false; }
        Debug_Printf(address_text_007ff6b8);
        HINSTANCE utility=LoadLibraryA(address_text_007ff6ac);
        if(!utility) { Debug_Printf(address_text_007ff690);FreeLibrary(extension);return false; }
        address_snmp_init_b2a754=(AddressSnmpInit)GetProcAddress(extension,address_text_007ff67c);
        address_snmp_query_b294b0=(AddressSnmpQuery)GetProcAddress(extension,address_text_007ff668);
        address_snmp_alloc_b2a718=(AddressSnmpAlloc)GetProcAddress(utility,address_text_007ff654);
        address_snmp_free_b2a8dc=(AddressSnmpFree)GetProcAddress(utility,address_text_007ff644);
        if(!address_snmp_init_b2a754 || !address_snmp_query_b294b0 || !address_snmp_alloc_b2a718 || !address_snmp_free_b2a8dc) {
            Debug_Printf(address_text_007ff488);FreeLibrary(utility);FreeLibrary(extension);return false;
        }
        SnmpVarBindList *list=(SnmpVarBindList *)address_snmp_alloc_b2a718(sizeof(SnmpVarBindList));
        SnmpVarBind *binding=(SnmpVarBind *)address_snmp_alloc_b2a718(sizeof(SnmpVarBind));
        Debug_Printf(address_text_007ff628);
        HANDLE event;
        AsnObjectIdentifier first_region;
        if(!address_snmp_init_b2a754(GetTickCount(),&event,&first_region)) {
            Debug_Printf(address_text_007ff60c);address_snmp_free_b2a8dc(list);address_snmp_free_b2a8dc(binding);
            FreeLibrary(utility);FreeLibrary(extension);return false;
        }
        unsigned int tcp_connection_oid[10]={1,3,6,1,2,1,6,13,1,1};
        unsigned int *oid_copy=(unsigned int *)address_snmp_alloc_b2a718(sizeof(tcp_connection_oid));
        memcpy(oid_copy,tcp_connection_oid,sizeof(tcp_connection_oid));
        // The original still points the query at the stack OID, not its copy.
        binding->name.idLength=10;
        binding->name.ids=tcp_connection_oid;
        list->list=binding;
        list->len=1;
        int previous_column=1;
        int row=0;
        long error_status,error_index;
        while(true) {
            if(!address_snmp_query_b294b0(0xa1,list,&error_status,&error_index)) {
                // The failure path frees these two objects and leaves oid_copy allocated.
                Debug_Printf(address_text_007ff5e8);
                address_snmp_free_b2a8dc(list);address_snmp_free_b2a8dc(binding);
                FreeLibrary(utility);FreeLibrary(extension);
                return false;
            }
            if(binding->name.idLength<10) break;
            int column=binding->name.ids[9];
            if(previous_column!=column) {row=0;previous_column=column;}
            switch(column) {
            case 1: {
                    tConnInfoStruct *connection=new tConnInfoStruct;
                    connection->State=binding->value.asnValue.number;
                    connections.Add(connection);
                } break;
            case 2: connections[row]->LocalAddress=*(unsigned long *)binding->value.asnValue.address.stream;++row;break;
            case 3: connections[row]->LocalPort=(unsigned short)binding->value.asnValue.number;++row;break;
            case 4: connections[row]->RemoteAddress=*(unsigned long *)binding->value.asnValue.address.stream;++row;break;
            case 5: connections[row]->RemotePort=(unsigned short)binding->value.asnValue.number;++row;break;
            }
        }

        address_snmp_free_b2a8dc(list);address_snmp_free_b2a8dc(binding);address_snmp_free_b2a8dc(oid_copy);
        Debug_Printf(address_text_007ff5c0,connections.Count());
        bool found=false;
        if(connections.Count()) {
            do {
                tConnInfoStruct *connection=connections[0];
                in_addr endpoint;
                endpoint.S_un.S_addr=ntohl(connection->RemoteAddress);
                if(!found && memcmp(&endpoint,&remote_address,sizeof(endpoint))==0) {
                    Debug_Printf(address_text_007ff588);
                    if(server_port && server_port!=connection->RemotePort) {
                        Debug_Printf(address_text_007ff54c,connection->RemotePort,server_port);
                    } else {
                        Debug_Printf(address_text_007ff530);
                        if(connection->State==5) {
                            Debug_Printf(address_text_007ff514);
                            *(unsigned long *)node=connection->LocalAddress;
                            address->Set_Address(network,node,connection->LocalPort);
                            found=true;
                        } else Debug_Printf(address_text_007ff4e8);
                    }
                }
                delete connections[0];
                connections.Delete(0);
            }while(connections.Count());
            if(found) Debug_Printf(address_text_007ff4bc,address->As_String());
        }
        FreeLibrary(utility);FreeLibrary(extension);
        return found;
    } else Debug_Printf(address_text_007ff468);
    return false;
}

// Actual original constructor call and RTTI-backed container vptrs.

// All original virtual targets, read from the two original seven-slot tables.


extern int scenario_packet_length_a3f808;
extern unsigned short scenario_product_a3f818;
extern int wol_connection_status_b26a2c[];
extern int wol_connection_limit_7fd324[];
void __fastcall WideToCharString_006fa0f0(char *,wchar_t const *,int);
extern char scenario_debug_007e6b38[];
extern char scenario_debug_007ff8c0[];
extern char scenario_debug_007ff89c[];
extern char scenario_debug_007ff87c[];
extern char scenario_debug_007ff860[];
extern char scenario_debug_007ff840[];
extern char scenario_debug_007ff828[];
extern char scenario_debug_007ff804[];
extern char scenario_debug_007ff7ec[];
extern char scenario_debug_007ff7d0[];
bool Local_User_Is_Channel_Owner_0074d960();
int __fastcall Sync_Scenario_With_Guests_00750330(bool connected)
{
    UnknownGlobalPacket packet;
    bool failed=!connected;
    bool scenario_mismatch=false;
    int result=0;
    unknown_ipx_manager_a40978.unknown_5277f0();
    unknown_ipx_manager_a40978.Set_Timing(50,(unsigned long)-1,5000,true);
    WOL::User *owner;
    if(Local_User_Is_Channel_Owner_0074d960()) {
        memset(&packet,0,sizeof(packet));
        int responses[8];
        memset(responses,0,sizeof(responses));
        int num_responses=0;
        bool send_scenario=false;
        Debug_Printf(scenario_debug_007e6b38);
        CDTimerClass<SystemTimerClass> response_timer;
        response_timer=900;
        if(failed) response_timer=300;
        do {
            Service_Game_IO_00481fb0();
            if(unknown_ipx_manager_a40978.Get_Global_Message(&unknown_gpacket_a3f640,&scenario_packet_length_a3f808,&unknown_address_a3f80c,&scenario_product_a3f818) && scenario_product_a3f818==0xaa03) {
                Debug_Printf(scenario_debug_007ff8c0);
                for(int i=1;i<unknown_session_a3d298.Players.Count();++i) {
                    if(unknown_session_a3d298.Players[i]->Address.Is_Same_Node(unknown_address_a3f80c) && !_stricmp(Wide_To_Narrow_006fa120(unknown_session_a3d298.Players[i]->Name),unknown_gpacket_a3f640.Name) && !responses[i]) {
                        if(unknown_gpacket_a3f640.Command==16) {
                            Debug_Printf(scenario_debug_007ff89c);
                            responses[i]=unknown_gpacket_a3f640.Command;
                            send_scenario=true;
                            ++num_responses;
                        }
                        if(unknown_gpacket_a3f640.Command==19) {
                            Debug_Printf(scenario_debug_007ff87c);
                            responses[i]=unknown_gpacket_a3f640.Command;
                            ++num_responses;
                        }
                        if(unknown_gpacket_a3f640.Command==42) {
                            Debug_Printf(scenario_debug_007ff860);
                            responses[i]=unknown_gpacket_a3f640.Command;
                            ++num_responses;
                            failed=true;
                            if(i<unknown_session_a3d298.Players.Count() && wol_connection_status_b26a2c[i]<wol_connection_limit_7fd324[i]) wol_connection_status_b26a2c[i]=0;
                        }
                        if(unknown_gpacket_a3f640.Command==20) {
                            Debug_Printf(scenario_debug_007ff840);
                            responses[i]=unknown_gpacket_a3f640.Command;
                            result=2;
                            failed=true;
                            scenario_mismatch=true;
                            ++num_responses;
                        }
                    }
                }
            }
        } while(num_responses<unknown_session_a3d298.Players.Count()-1 && response_timer);
        if(scenario_mismatch) result=2;
        if(!failed && !scenario_mismatch && send_scenario && num_responses==unknown_session_a3d298.Players.Count()-1) {
            while(End_Dialog_007357e0(0,0)==1) {}
            memset(unknown_session_a3d298.ScenarioRequests,0,sizeof(unknown_session_a3d298.ScenarioRequests));
            unknown_session_a3d298.RequestCount=0;
            for(int j=1;j<unknown_session_a3d298.Players.Count();++j) {
                if(responses[j]==16) unknown_session_a3d298.ScenarioRequests[unknown_session_a3d298.RequestCount++]=(char)j;
            }
            if(!Send_Remote_File_0066bb20(unknown_id_source_a3d290->FileName,4,false,true)) {
                result=2;
                failed=false;
            }
        }
        memset(&packet,0,sizeof(packet));
        if(!failed && num_responses==unknown_session_a3d298.Players.Count()-1 && !scenario_mismatch) packet.Command=11;
        else {
            packet.Command=42;
            failed=true;
            if(!result) result=1;
        }
        WideToCharString_006fa0f0(packet.Name,unknown_session_a3d298.Players[0]->Name,-1);
        for(int k=1;k<unknown_session_a3d298.Players.Count();++k) {
            CDTimerClass<SystemTimerClass> send_timer(600);
            if(failed) send_timer=CDTimerClass<SystemTimerClass>(300);
            do {
                Service_Game_IO_00481fb0();
            } while(!unknown_ipx_manager_a40978.unknown_5259a0(&packet,47,1,unknown_session_a3d298.Players[k]->Name) && send_timer);
        }
    } else {
        if(Find_Local_Scenario_0066c5b0(unknown_session_a3d298.ScenarioFileName_658,unknown_session_a3d298.ScenarioFileLength_87c,unknown_session_a3d298.ScenarioDigest_85a,unknown_session_a3d298.ScenarioIsOfficial_880,true)) {
            memset(&packet,0,sizeof(packet));
            packet.Command=19;
            if(failed) packet.Command=42;
            WideToCharString_006fa0f0(packet.Name,unknown_session_a3d298.Players[0]->Name,-1);
            owner=0;
            for(int i=0;i<unknown_users_b2a728.Count;++i) {
                unknown_users_b2a728.Get_Copy(unknown_user_b2a658,i);
                if(unknown_user_b2a658.flags&1) { owner=&unknown_user_b2a658; break; }
            }
            if(owner) unknown_ipx_manager_a40978.unknown_5259a0(&packet,47,1,Narrow_To_Wide_006fa180(owner->name));
            CDTimerClass<SystemTimerClass> send_timer;
            send_timer=1200;
            if(failed) send_timer=300;
            while(unknown_ipx_manager_a40978.Global_Num_Send()>0 && send_timer) Service_Game_IO_00481fb0();
        } else {
            if((unknown_a3d2a4!=2 && unknown_a3d2a4==1) || (unknown_session_a3d298.ScenarioIsOfficial_880 && !unknown_session_a3d298.Unknown_00672140())) {
                memset(&packet,0,sizeof(packet));
                packet.Command=20;
                owner=0;
                for(int i=0;i<unknown_users_b2a728.Count;++i) {
                    unknown_users_b2a728.Get_Copy(unknown_user_b2a658,i);
                    if(unknown_user_b2a658.flags&1) { owner=&unknown_user_b2a658; break; }
                }
                if(owner) unknown_ipx_manager_a40978.unknown_5259a0(&packet,47,1,Narrow_To_Wide_006fa180(owner->name));
                Debug_Printf(scenario_debug_007ff828);
                CDTimerClass<SystemTimerClass> send_timer;
                send_timer=1200;
                while(unknown_ipx_manager_a40978.Global_Num_Send()>0 && send_timer) Service_Game_IO_00481fb0();
                result=2;
                failed=true;
            } else {
                while(End_Dialog_007357e0(0,0)==1) {}
                if(!Get_File_From_Host_0066aef0(unknown_session_a3d298.ScenarioFileName_658,4,true)) { result=2; failed=true; }
            }
        }
        strcpy(unknown_id_source_a3d290->FileName,unknown_session_a3d298.ScenarioFileName_658);
        Debug_Printf(scenario_debug_007ff804);
        CDTimerClass<SystemTimerClass> response_timer;
        response_timer=1200;
        if(failed) response_timer=300;
        do {
            Service_Game_IO_00481fb0();
            if(unknown_ipx_manager_a40978.Get_Global_Message(&unknown_gpacket_a3f640,&scenario_packet_length_a3f808,&unknown_address_a3f80c,&scenario_product_a3f818) && scenario_product_a3f818==0xaa03) {
                Debug_Printf(scenario_debug_007ff8c0);
                owner=0;
                for(int i=0;i<unknown_users_b2a728.Count;++i) {
                    unknown_users_b2a728.Get_Copy(unknown_user_b2a658,i);
                    if(unknown_user_b2a658.flags&1) { owner=&unknown_user_b2a658; break; }
                }
                if(owner && !_stricmp(unknown_gpacket_a3f640.Name,owner->name)) {
                    if(unknown_gpacket_a3f640.Command==11) {
                        Debug_Printf(scenario_debug_007ff7ec);
                        goto finish;
                    }
                    if(unknown_gpacket_a3f640.Command==42) {
                        Debug_Printf(scenario_debug_007ff7d0);
                        failed=true;
                        for(int j=0;j<unknown_session_a3d298.Players.Count();++j) {
                            if(!_stricmp(Wide_To_Narrow_006fa120(unknown_session_a3d298.Players[j]->Name),unknown_user_b2a658.name)) {
                                if(j<unknown_session_a3d298.Players.Count() && wol_connection_status_b26a2c[j]<wol_connection_limit_7fd324[j]) wol_connection_status_b26a2c[j]=3;
                                break;
                            }
                        }
                        break;
                    }
                }
            }
        } while(response_timer);
        if(!response_timer) failed=true;
    }
finish:
    unknown_ipx_manager_a40978.Set_Timing(unknown_ipx_manager_a40978.unknown_526c30()+2,(unsigned long)-1,unknown_ipx_manager_a40978.unknown_526c30()*8<120?120:unknown_ipx_manager_a40978.unknown_526c30()*8,true);
    unknown_ipx_manager_a40978.unknown_527800();
    if(result!=2 && failed) result=1;
    return result;
}


extern bool receiving_host_preview_00b2ac46;
extern char host_preview_debug_007ff8d8[];
// Existing canonical Poke_The_Host_00750f60 uses this same User* interface to 74D7D0.
bool __fastcall Receive_Host_Map_Preview_00750d10()
{
    IPXAddressClass address;
    if(unknown_game_channel_b299c0.unknown_00 && unknown_users_b2a728.Count>=2) {
        WOL::User *owner=0;
        for(int i=0;i<unknown_users_b2a728.Count;++i) {
            unknown_users_b2a728.Get_Copy(unknown_user_b2a658,i);
            if(unknown_user_b2a658.flags&1) {
                owner=&unknown_user_b2a658;
                break;
            }
        }
        if(owner && !_stricmp(owner->name,unknown_wstring_b26cb8.Peek_Buffer())) return false;
        if(unknown_b2ac40==0 && unknown_ipx_manager_a40978.Global_Num_Receive()) {
            Service_Game_IO_00481fb0();
            UnknownGlobalPacket packet;
            int length=sizeof(packet);
            unsigned short product;
            if(unknown_ipx_manager_a40978.unknown_525af0(0,&packet,&length,&address,&product,0) && packet.Command==25) {
                void *dialog=WS_Find_Dialog_00735940(0xc9);
                if(dialog) ShowWindow(GetDlgItem((HWND)dialog,0x468),1);
                WOL::User user;
                unknown_users_b2a728.Get_Copy(user,0);
                user.next=0;
                Unknown_0074d7d0(&user);
                if(unknown_ipx_manager_a40978.Get_Global_Message(&packet,&length,&address,&product) && packet.Command==25 && address.Is_Same_Node(unknown_address_a3db2c) && product==0xaa03) {
                    owner=0;
                    for(int i=0;i<unknown_users_b2a728.Count;++i) {
                        unknown_users_b2a728.Get_Copy(unknown_user_b2a658,i);
                        if(unknown_user_b2a658.flags&1) {
                            owner=&unknown_user_b2a658;
                            break;
                        }
                    }
                    if(owner && !_stricmp(packet.Name,owner->name)) {
                        Debug_Printf(host_preview_debug_007ff8d8);
                        unknown_ipx_manager_a40978.Set_Timing(50,(unsigned long)-1,5000,true);
                        receiving_host_preview_00b2ac46=true;
                        Download_Map_Preview_005c7970();
                        receiving_host_preview_00b2ac46=false;
                        unknown_ipx_manager_a40978.Set_Timing(30,(unsigned long)-1,600,true);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}


extern char wol_host_address_debug_7ff904[];
void __fastcall WideToCharString_006fa0f0(char *destination,wchar_t const *source,int count);
void __fastcall Poke_The_Host_00750f60()
{
    if(unknown_game_mode_a3d298!=4) {
        IPXAddressClass address;
        if(unknown_users_b2a728.Count>=2) {
            WOL::User *owner=0;
            for(int i=0;i<unknown_users_b2a728.Count;++i) {
                unknown_users_b2a728.Get(unknown_user_b2a658,i);
                if(unknown_user_b2a658.flags&1) {
                    owner=&unknown_user_b2a658;
                    break;
                }
            }
            if(owner && !_stricmp(owner->name,unknown_wstring_b26cb8.Peek_Buffer())) return;
            if(unknown_b2ac40==0 && unknown_game_channel_b299c0.unknown_00!=0) {
                WOL::User user;
                unknown_users_b2a728.Get_Copy(user,0);
                user.next=0;
                Unknown_0074d7d0(&user);
                Service_Game_IO_00481fb0();
                unknown_ipx_manager_a40978.Set_Timing(50,(unsigned long)-1,5000,true);
                Debug_Printf(wol_host_address_debug_7ff904,unknown_address_a3db2c.As_String());
                UnknownGlobalPacket packet;
                memset(&packet,0,sizeof(packet));
                packet.Command=13;
                WideToCharString_006fa0f0(packet.Name,unknown_players_a3fa7c[0]->Name,-1);
                owner=0;
                for(int i=0;i<unknown_users_b2a728.Count;++i) {
                    unknown_users_b2a728.Get_Copy(unknown_user_b2a658,i);
                    if(unknown_user_b2a658.flags&1) {
                        owner=&unknown_user_b2a658;
                        break;
                    }
                }
                if(owner) unknown_ipx_manager_a40978.unknown_5259a0(&packet,sizeof(packet),1,Narrow_To_Wide_006fa180(owner->name));
                Service_Game_IO_00481fb0();
            }
        }
    }
}


int __fastcall Run_Control_Dialog_006011a0(int id,DLGPROC procedure,OwnerDraw::DialogControl *control);
long __stdcall Wonline_Dialog_FE_00751190(HWND,unsigned,unsigned,long);
int Open_Wonline_Dialog_FE_00751130()
{
    SimpleWonlineDialogControl control;
    int result=Run_Control_Dialog_006011a0(0xfe,(DLGPROC)Wonline_Dialog_FE_00751190,&control);
    if(result<0) return 0;
    return result;
}


// Locale chooser00751190: native callback ABI and existing dialog-control result carrier.
WideString __fastcall Locale_Name_0073f1c0(int locale);
void __fastcall Unknown_005FF380(HWND window);
void __fastcall unknown_dialog_5e5f20(HWND window);

long __stdcall Wonline_Dialog_FE_00751190(HWND window,unsigned message,unsigned wparam,long lparam)
{
    if(message==0x110) {
        long initialization=lparam;
        if(initialization) SetWindowLongA(window,8,((long *)initialization)[1]);
    }
    int result=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(!result) {
        OwnerDraw::DialogControl *control=(OwnerDraw::DialogControl *)GetWindowLongA(window,8);
        switch(message) {
        case 0x82:
            Unknown_005FF380(window);
            return 0;
        case 0xf:
            Unknown_005FEE70(window);
            unknown_dialog_5e5f20(window);
            ValidateRect(window,0);
            return 0;
        case 0x14:
            return 1;
        case 0x2b:
            Unknown_005FE380(lparam);
            return 1;
        case 0x497: {
            SendDlgItemMessageA(window,0x3f0,0x184,0,0);
            int count=0;
            unknown_chat_a72cfc->unknown_slot52(&count);
            if(count>0) {
                HWND list=GetDlgItem(window,0x3f0);
                for(int locale=count-1;locale>=0;--locale) {
                    WideString name=Locale_Name_0073f1c0(locale);
                    Debug_Printf("Adding country: %hs\n",name.Get());
                    int row;
                    if(locale<2) row=SendMessageA(list,0x4cc,0,(LPARAM)name.Get());
                    else row=SendMessageA(list,0x4cd,0,(LPARAM)name.Get());
                    SendMessageA(list,0x19a,row,locale);
                }
                SendMessageA(list,0x186,-1,0);
            }
            break;
        }
        case 0x111:
            switch(wparam&0xffff) {
            case 1: {
                HWND list=GetDlgItem(window,0x3f0);
                int selected=0;
                int row=SendMessageA(list,0x188,0,0);
                if(row!=-1) selected=SendMessageA(list,0x199,row,0);
                // Preserve the existing interface's HWND-typed numeric result carrier.
                control->Set_Window_006010f0((unsigned int)window,(HWND)selected);
                break;
            }
            case 2:
                control->Set_Window_006010f0((unsigned int)window,0);
                return 0;
            case 0x3f0:
                if((unsigned short)(wparam>>16)==1) {
                    int row=SendMessageA((HWND)lparam,0x188,0,0);
                    EnableWindow(GetDlgItem(window,1),row!=-1);
                    return 0;
                }
                break;
            }
            break;
        }
        result=0;
    }
    return result;
}


#pragma auto_inline(off)
void __fastcall Unknown_007513e0()
{
    NodeNameType *player=new NodeNameType;
    Char_To_Wide_006fa0c0(player->Name,"Ol Cruddy",-1);
    player->unknown_6b=-1;
    player->unknown_4b=0;
    player->unknown_53=3;
    NetNumType network;
    NetNodeType node;
    memset(network,0,sizeof(network));
    node[5]=0;
    node[4]=0;
    node[3]=82;
    node[2]=81;
    node[1]=2;
    node[0]=10;
    player->Address.Set_Address(network,node,0);
    unknown_players_a3fa7c.Add(player);
    unknown_pings_b2a734[unknown_session_a3d55c]=100;
    ++unknown_session_a3d55c;
}

#pragma auto_inline(on)


#pragma auto_inline(off)
void __fastcall Unknown_007514d0()
{
    NodeNameType *player=new NodeNameType;
    Char_To_Wide_006fa0c0(player->Name,"Potato",-1);
    player->unknown_6b=-1;
    player->unknown_4b=0;
    player->unknown_53=0;
    NetNumType network;
    NetNodeType node;
    memset(network,0,sizeof(network));
    node[5]=0;
    node[4]=0;
    node[3]=97;
    node[2]=20;
    node[1]=2;
    node[0]=10;
    player->Address.Set_Address(network,node,0);
    unknown_players_a3fa7c.Add(player);
    unknown_pings_b2a734[unknown_session_a3d55c]=100;
    ++unknown_session_a3d55c;
}

#pragma auto_inline(on)


#pragma auto_inline(off)
void __fastcall Unknown_007515b0()
{
    NodeNameType *player=new NodeNameType;
    Char_To_Wide_006fa0c0(player->Name,"PotatoToo",-1);
    player->unknown_6b=-1;
    player->unknown_4b=1;
    player->unknown_53=2;
    NetNumType network;
    NetNodeType node;
    memset(network,0,sizeof(network));
    node[5]=0;
    node[4]=0;
    node[3]=250;
    node[2]=80;
    node[1]=2;
    node[0]=10;
    player->Address.Set_Address(network,node,0);
    unknown_players_a3fa7c.Add(player);
    unknown_pings_b2a734[unknown_session_a3d55c]=100;
    ++unknown_session_a3d55c;
    unknown_address_a3db2c=player->Address;
}

#pragma auto_inline(on)


WIN32_IMPORT BOOL WINAPI GetComputerNameA(char *,DWORD *);
void __fastcall Unknown_007513e0();
void __fastcall Unknown_007514d0();
void __fastcall Unknown_007515b0();
extern int test_network_ids_007f60d0[];
extern int test_network_end_flag_00a40d2c;
extern char test_host1_007ff96c[],test_host2_007ff960[],test_master_name_007ff954[];
extern char test_start_debug_007fded4[],test_max_ping_debug_007e38bc[],test_no_ping_debug_007e38a0[];
extern char test_precalc_debug_007ff334[],test_frame_rate_debug_007ff314[],test_latency_debug_007d68f4[],test_send_rate_debug_007ff2fc[];
void __fastcall Start_Test_Network_Game_007516c0()
{
    unknown_b2a7fc=0;
    unknown_b2ab84=unknown_protocol_7fd370;
    unknown_b2a7ec=0;
    strcpy(unknown_b2a6d8,unknown_wstring_b26cb8.Get());
    Free_Pointers_005a7480(&unknown_players_a3fa7c);
    // Capacity comes from the original GetComputerNameA size argument (0x3F7).
    char computer_name[1015];
    DWORD computer_name_size=sizeof(computer_name);
    GetComputerNameA(computer_name,&computer_name_size);
    bool precalculate=false;
    if(!_stricmp(computer_name,test_host1_007ff96c)) {
        Unknown_007515b0();Unknown_007513e0();Unknown_007514d0();
        precalculate=true;
    } else {
        if(!_stricmp(computer_name,test_host2_007ff960)) {
            Unknown_007514d0();Unknown_007513e0();
        } else {
            Unknown_007513e0();Unknown_007514d0();
        }
        Unknown_007515b0();
    }
    PacketTransport->Clear_Broadcast_Addresses();
    char address[32]="xxx.xxx.xxx.xxx";
    NetNumType network;
    NetNodeType node;
    int i;
    for(i=1;i<unknown_players_a3fa7c.Count();++i) {
        unknown_players_a3fa7c[i]->Address.Get_Address(network,node);
        PacketTransport->Set_Broadcast_Address(address,0);
    }
    unknown_b2ac40=2;
    Debug_Printf(test_start_debug_007fded4,unknown_session_a3d298.NumPlayers);
    Char_To_Wide_006fa0c0(unknown_session_a3d298.MasterPlayerName,test_master_name_007ff954,-1);
    unknown_session_a3d298.MasterPlayerID=-1;
    unknown_session_a3d298.unknown_124=0;
    unknown_a40d3c=1;
    unknown_house_setting_7d88d8=10;
    unknown_session_a3d298.UnitCount=10;
    unknown_session_a3d298.unknown_24=10000;
    unknown_session_a3d298.unknown_fog_a7=false;
    unknown_session_a3d298.unknown_28=true;
    unknown_session_a3d298.unknown_crates_29=true;
    unknown_session_a3d298.unknown_a8=true;
    unknown_session_a3d298.unknown_a4=true;
    unknown_session_a3d298.unknown_a5=false;
    unknown_session_a3d298.unknown_bases_20=true;
    unknown_session_a3d298.unknown_a6=false;
    unknown_session_a3d298.AICount=0;
    unknown_session_a3d298.unknown_difficulty_40=0;
    unknown_session_a3d298.unknown_2a=true;
    unknown_session_a3d298.unknown_2b=true;
    unknown_session_a3d298.unknown_2c=false;
    unknown_session_a3d298.unknown_34=true;
    unknown_session_a3d298.ScenarioIndex=1;
    unknown_session_a3d298.GameSpeed_30=0;
    Select_Scenario_005c7c30(1);
    unknown_session_a3d298.GameSpeed_30=2;
    unknown_a40b18=2;
    unknown_id_source_a3d290->TechLevel=unknown_session_a3d298.ScenarioIndex;
    unknown_session_a3d298.unknown_2cc=5;
    switch(unknown_session_a3d298.GameSpeed_30) {
    case 0:unknown_session_a3d298.MaxAhead_2c8=40;unknown_session_a3d298.unknown_2e8=60;unknown_session_a3d298.unknown_2cc=10;break;
    case 1:unknown_session_a3d298.MaxAhead_2c8=40;unknown_session_a3d298.unknown_2e8=45;unknown_session_a3d298.unknown_2cc=10;break;
    case 2:unknown_session_a3d298.MaxAhead_2c8=30;unknown_session_a3d298.unknown_2e8=30;break;
    case 3:unknown_session_a3d298.MaxAhead_2c8=20;unknown_session_a3d298.unknown_2e8=20;break;
    case 4:unknown_session_a3d298.MaxAhead_2c8=20;unknown_session_a3d298.unknown_2e8=15;break;
    case 5:unknown_session_a3d298.MaxAhead_2c8=20;unknown_session_a3d298.unknown_2e8=12;break;
    default:unknown_session_a3d298.MaxAhead_2c8=10;unknown_session_a3d298.unknown_2e8=10;break;
    }
    unknown_session_a3d298.unknown_2e4=0;
    if(!precalculate) unknown_session_a3d298.unknown_2e8=0;
    int maximum_ping=0;
    for(i=0;i<unknown_players_a3fa7c.Count();++i) if(unknown_pings_b2a734[i]>maximum_ping) maximum_ping=unknown_pings_b2a734[i];
    unknown_a3fba4=1;
    if(maximum_ping==0 || maximum_ping==1000) {
        unknown_session_a3d298.unknown_2e8=0;
        Debug_Printf(test_no_ping_debug_007e38a0);
    } else {
        Debug_Printf(test_max_ping_debug_007e38bc,maximum_ping);
        if(maximum_ping>1000) unknown_a3fba4=2;
        else if(maximum_ping<600) unknown_a3fba4=1;
        int ticks=60*maximum_ping/1000;
        ticks+=120*ticks/100;
        unknown_ipx_manager_a40978.Set_Timing(ticks,(unsigned long)-1,ticks*8,true);
        if(precalculate) {
            unknown_ipx_manager_a40978.unknown_slot18(5,(unsigned long)-1,120);
            switch(unknown_a3fba4) {
            case 0:case 1:unknown_session_a3d298.unknown_2e4=unknown_session_a3d298.MaxAhead_2c8;break;
            case 2:unknown_session_a3d298.unknown_2e4=(unknown_session_a3d298.MaxAhead_2c8>>1)+unknown_session_a3d298.MaxAhead_2c8;break;
            case 3:unknown_session_a3d298.unknown_2e4=unknown_session_a3d298.MaxAhead_2c8*2;break;
            }
            unknown_session_a3d298.unknown_2e4=unknown_session_a3d298.unknown_2e4/10*10;
        }
    }
    Debug_Printf(test_precalc_debug_007ff334,unknown_session_a3d298.unknown_2e4);
    Debug_Printf(test_frame_rate_debug_007ff314,unknown_session_a3d298.unknown_2e8);
    Debug_Printf(test_latency_debug_007d68f4,unknown_a3fba4);
    Debug_Printf(test_send_rate_debug_007ff2fc,unknown_session_a3d298.unknown_2cc);
    unknown_session_a3d298.unknown_2e0=unknown_session_a3d298.MaxAhead_2c8;
    memset(unknown_session_a3d298.unknown_2ec,0,sizeof(player_statistics_00a3d584));
    test_network_ids_007f60d0[0]=1234;
    unknown_a40b18=unknown_session_a3d298.GameSpeed_30;
    test_network_ids_007f60d0[1]=1235;test_network_ids_007f60d0[2]=1236;test_network_ids_007f60d0[3]=1237;
    test_network_end_flag_00a40d2c=0;
    unknown_squad_1_b2a820[0]=0;unknown_squad_2_b293d8[0]=0;
    Unknown_Pregame_Setup_005c74f0();
}


void __fastcall Open_Wait_Window_0074d4d0(unsigned int,WideString const &,bool);
int __fastcall Unknown_00751c30(unsigned int);
extern char server_list_wait_label_007fe670[];
extern char wol_source_file_007fd62c[];
void __fastcall Request_Server_List_With_Wait_00751bd0()
{
    {
        WideString title(StringTable::Load_String(server_list_wait_label_007fe670,0,wol_source_file_007fd62c,0x56cc));
        Open_Wait_Window_0074d4d0(0x10,title,false);
    }
    unsigned int flags=0x2300;
    if(unknown_a3d2a4!=2) flags=0x2100;
    Unknown_00751c30(flags);
}


// Server list wait00751C30: native two-event completion, error ranges and dialog cleanup.
extern "C" __declspec(dllimport) DWORD __stdcall WaitForMultipleObjects(DWORD,HANDLE*,BOOL,DWORD);
extern HANDLE wol_wait_events_b264f8[18];
extern int server_request_status_b2ac00;
extern unsigned long wol_server_product_007fd370;
extern char account_text_7fe800[],account_text_7fe808[],account_text_7fd740[],wol_source_file_007fd62c[];
extern bool connection_wait_callback_0073c120();
int __fastcall Unknown_00751c30(unsigned int flags)
{
    bool previous=unknown_b2abd8;
    unknown_b2abd8=true;
    unknown_servers_b2a700.Count=0;
    unknown_irc_index_b292d8=0;
    unknown_irc_index_b2a7f4=0;
    unknown_chat_a72cfc->Request_Server_List(unknown_b2ac5c|flags,0x10006,account_text_7fe800,account_text_7fe808,30);
    unknown_chat_a72cfc->unknown_slot35(wol_server_product_007fd370);
    bool servers=false;
    bool products=false;
    int result;
    DWORD event;
    for(;;) {
        event=WaitForMultipleObjects(18,wol_wait_events_b264f8,0,50);
        while(event==0x102) {
            Unknown_0073e100();
            event=WaitForMultipleObjects(18,wol_wait_events_b264f8,0,50);
        }
        ResetEvent(wol_wait_events_b264f8[event]);
        if(event==0) break;
        if(event==8) break;
        if(event==1) {
            servers=true;
            if(products) goto complete;
        } else if(event==9) {
            int status=server_request_status_b2ac00;
            if(status<=(int)0x8004006d) {
                if(status>=(int)0x8004006c || (status>=(int)0x80040067 && status<=(int)0x8004006a)) {
                    Message_Box_005c3230(StringTable::Load_String(account_text_7fd740,0,wol_source_file_007fd62c,0x5739),0,connection_wait_callback_0073c120,false);
                    event=0;
                    break;
                }
            } else if(status==0x4012f) {
                products=true;
                if(servers) goto complete;
            }
        }
    }
    if(event==8) result=2;
    else result=1;
    goto cleanup;
complete:
    result=0;
cleanup:
    void *wait=WS_Find_Dialog_00735940(0xc4);
    if(wait) {
        unknown_b2abe0=0;
        Unknown_007357E0(wait,0);
    }
    HWND current=Unknown_00735B10();
    HWND edit=GetDlgItem(current,0x540);
    if(edit) {
        SetFocus(edit);
        unknown_b2abd8=previous;
        return result;
    }
    unknown_b2abd8=previous;
    return result;
}


extern HANDLE wol_wait_events_b264f8[18];
extern bool connection_wait_callback_0073c120();
void __fastcall Open_Wait_Window_0074d4d0(unsigned int,WideString const &,bool);
inline void __fastcall Close_Wonline_Wait(unsigned int);
extern char wol_wdt_error_007fe4c0[],wol_fetch_wdt_007ff994[],wol_source_file_007fd62c[];
extern char wol_wdt_endpoint_007ff9c8[],wol_wdt_current_007ff9a8[],wol_wdt_previous_007ff978[];
bool __fastcall Request_WDT_Cycle_00751df0()
{
    char const *connection=unknown_wdt_server_b29880.connection;
    if(connection==0) {
        Message_Box_005c3230(StringTable::Load_String(wol_wdt_error_007fe4c0,0,wol_source_file_007fd62c,0x5770),0,connection_wait_callback_0073c120,false);
        return false;
    } else {
    for(int i=0;i<18;++i) ResetEvent(wol_wait_events_b264f8[i]);
    HRESULT request=unknown_ladder_client_a72d00->Request_WDT_State(unknown_wdt_address_b269ac,unknown_wdt_port_b299bc,3);
    Debug_Printf(wol_wdt_endpoint_007ff9c8,unknown_wdt_address_b269ac,unknown_wdt_port_b299bc);
    Debug_Printf(wol_wdt_current_007ff9a8);
    if(request!=0) {
        Message_Box_005c3230(StringTable::Load_String(wol_wdt_error_007fe4c0,0,wol_source_file_007fd62c,0x5780),0,connection_wait_callback_0073c120,false);
        return false;
    } else {
    {
        WideString title(StringTable::Load_String(wol_fetch_wdt_007ff994,0,wol_source_file_007fd62c,0x5784));
        Open_Wait_Window_0074d4d0(0x10,title,false);
    }
    for(;;) {
        DWORD event;
        {
        int remaining=30000;
        event=WaitForMultipleObjects(18,wol_wait_events_b264f8,false,100);
        while(event==0x102) {
            remaining-=100;
            if(remaining<=0) goto timeout;
            Unknown_0073e100();
            event=WaitForMultipleObjects(18,wol_wait_events_b264f8,false,100);
        }
        }
        ResetEvent(wol_wait_events_b264f8[event]);
        if(event==0) {
            Close_Wonline_Wait(~0U);
            return false;
        }
        if(event==5) {
            Message_Box_005c3230(StringTable::Load_String(wol_wdt_error_007fe4c0,0,wol_source_file_007fd62c,0x5797),0,connection_wait_callback_0073c120,false);
            Close_Wonline_Wait(~0U);
            return false;
        }
        if(event==18) {
timeout:
    Message_Box_005c3230(StringTable::Load_String(wol_wdt_error_007fe4c0,0,wol_source_file_007fd62c,0x579d),0,connection_wait_callback_0073c120,false);
    Close_Wonline_Wait(~0U);
            return false;
        }
        if(event==11) {
            request=unknown_ladder_client_a72d00->Request_WDT_State(unknown_wdt_address_b269ac,unknown_wdt_port_b299bc,5);
            Debug_Printf(wol_wdt_previous_007ff978);
            if(request!=0) {
                Close_Wonline_Wait(~0U);
                return false;
            } else {
                while(true) {
                    event=WaitForMultipleObjects(18,wol_wait_events_b264f8,false,100);
                    while(event==0x102) {
                        Unknown_0073e100();
                        event=WaitForMultipleObjects(18,wol_wait_events_b264f8,false,100);
                    }
                    ResetEvent(wol_wait_events_b264f8[event]);
                    if(event==0) {
                        Close_Wonline_Wait(~0U);
                        return false;
                    }
                    if(event==5) goto previous_cycle_error;
                    if(event==11) goto previous_cycle_success;
                }
previous_cycle_success:

                        Close_Wonline_Wait(~0U);
                        return true;
                    

previous_cycle_error:

                        Message_Box_005c3230(StringTable::Load_String(wol_wdt_error_007fe4c0,0,wol_source_file_007fd62c,0x57b8),0,connection_wait_callback_0073c120,false);
                        Close_Wonline_Wait(~0U);
                        return false;
                    
            }
        }
    }
    }
    }
    return false;
}


// WDT sound table initializer; complete1268B+12NOP, mapped before source.
extern bool unknown_wdt_sounds_initialized_b2ac28;
void __fastcall Initialize_WDT_Sounds_00752210()
{
    if(!unknown_wdt_sounds_initialized_b2ac28) {
        static Wstring sounds[16];
        sounds[0].Set("01-W024.V01");
        sounds[1].Set("01-W026.V01");
        sounds[2].Set("01-W028.V01");
        sounds[3].Set("01-W018.V01");
        sounds[4].Set("01-W020.V01");
        sounds[5].Set("01-W022.V01");
        sounds[6].Set("00-W022.V00");
        sounds[7].Set("00-W024.V00");
        sounds[8].Set("00-W026.V00");
        sounds[9].Set("00-W017.V00");
        sounds[10].Set("00-W018.V00");
        sounds[11].Set("00-W020.V00");
        sounds[12].Set("01-W032.V01");
        sounds[13].Set("01-W030.V01");
        sounds[14].Set("00-W031.V00");
        sounds[15].Set("00-W029.V00");
        unknown_strings_b26a50.Clear();
        unknown_strings_b26570.Clear();
        unknown_strings_b299b0.Clear();
        unknown_strings_b29380.Clear();
        unknown_strings_b26960.Clear();
        unknown_strings_b2a6c8.Clear();
        unknown_strings_b26580.Clear();
        unknown_strings_b29000.Clear();
        unknown_strings_b26a50.Add_Head(sounds[0]);
        unknown_strings_b26a50.Add_Head(sounds[1]);
        unknown_strings_b26a50.Add_Head(sounds[2]);
        unknown_strings_b299b0.Add_Head(sounds[3]);
        unknown_strings_b299b0.Add_Head(sounds[4]);
        unknown_strings_b299b0.Add_Head(sounds[5]);
        unknown_strings_b26570.Add_Head(sounds[6]);
        unknown_strings_b26570.Add_Head(sounds[7]);
        unknown_strings_b26570.Add_Head(sounds[8]);
        unknown_strings_b29380.Add_Head(sounds[9]);
        unknown_strings_b29380.Add_Head(sounds[10]);
        unknown_strings_b29380.Add_Head(sounds[11]);
        unknown_strings_b26960.Add_Head(sounds[12]);
        unknown_strings_b26580.Add_Head(sounds[13]);
        unknown_strings_b2a6c8.Add_Head(sounds[14]);
        unknown_strings_b29000.Add_Head(sounds[15]);
        unknown_wdt_sounds_initialized_b2ac28=true;
    }
}


// Emitted naturally by the local static Wstring array in Initialize_WDT_Sounds_00752210.


extern bool unknown_wdt_sounds_initialized_b2ac28;
void Deinitialize_WDT_Sounds_00752730()
{
    unknown_strings_b26a50.Clear();
    unknown_strings_b26570.Clear();
    unknown_strings_b299b0.Clear();
    unknown_strings_b29380.Clear();
    unknown_strings_b26960.Clear();
    unknown_strings_b2a6c8.Clear();
    unknown_strings_b26580.Clear();
    unknown_strings_b29000.Clear();
    unknown_wdt_sounds_initialized_b2ac28=false;
}


// 0x0074dd20: function not yet identified
//
// Body: 35 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __fastcall unknown_set_connection_status_0074dd20(int param_1,int param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_0074dd20 {

extern "C" unsigned int wol_connection_limit_007fd324;
extern "C" int player_count_00a3fa8c;
extern "C" unsigned int wol_connection_status_00b26a2c;

void __fastcall unknown_set_connection_status_0074dd20(int index, int status)
{
    if (index < player_count_00a3fa8c) {
        if ((int)(&wol_connection_status_00b26a2c)[index] < (int)(&wol_connection_limit_007fd324)[index]) {
            (&wol_connection_status_00b26a2c)[index] = status;
        }
    }
    return;
}

}  // namespace seed_0074dd20


// 0x00751120: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void unknown_empty_00751120(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00751120 {

void  unknown_empty_00751120(void)
{
    return;
}

}  // namespace seed_00751120


// RA2 retains destruction of the by-value sound category, with no playback work.
// Original callers construct a 12-byte independent list before calling 0x752890;
// the complete callee destroys Count Wstrings forwards and frees Items.
// __fastcall is an ABI hypothesis: the class argument is stack-passed and RET 12.
void __fastcall Discard_WDT_Sound_List_00752890(WolStringArray sounds)
{
}


extern "C" __declspec(dllimport) DWORD __stdcall WaitForMultipleObjects(DWORD,HANDLE*,BOOL,DWORD);
extern HANDLE wol_wait_events_b264f8[18];
DWORD __fastcall Wait_Wonline_Events_007528c0(int seconds,DWORD count)
{
    int remaining=seconds*1000;
    DWORD result=WaitForMultipleObjects(count,wol_wait_events_b264f8,0,100);
    while(result==0x102) {
        remaining-=100;
        if(remaining<=0) return 18;
        Unknown_0073e100();
        result=WaitForMultipleObjects(count,wol_wait_events_b264f8,0,100);
    }
    ResetEvent(wol_wait_events_b264f8[result]);
    return result;
}


// void Reset_Wait_Events() (0x00752930): inline, defined in Queue_Channel_List_Request_0073db20.cpp.
// Start_Quick_Match 0x00759E20 refuses it, which emits this body.


int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
extern int unknown_online_option_a3d2a8;
extern char wol_standard_icon_007eb1fc[];
extern char wol_tournament_icon_007eb25c[];
extern char wol_clan_icon_007eb228[];
extern char wol_reserved_icon_007eafb4[];
extern char wol_observer_icon_007e7444[];
extern char wol_private_icon_007eb234[];
extern char wol_ping_good_icon_007eafa8[];
extern char wol_ping_medium_icon_007eaf9c[];
extern char wol_ping_poor_icon_007eaf90[];
long __stdcall Wonline_Mode_Menu_Dialog_00752950(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    if(message==0x110 && lparam) SetWindowLongA(window,8,*(long *)(lparam+4));
    long result=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(result) return result;
    OwnerDraw::DialogControl *control=(OwnerDraw::DialogControl *)GetWindowLongA(window,8);
    switch(message) {
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
        return 1;
    case 0x14: return 1;
    case 0x2b: Unknown_005FE380(lparam); return 1;
    case 0x111:
        switch(wparam&0xffff) {
        case 0x6e0:
            unknown_a3d2a4=1;
            control->Set_Window_006010f0((unsigned)window,(HWND)0x6e0);
            return 0;
        case 0x6e1:
            unknown_a3d2a4=3;
            control->Set_Window_006010f0((unsigned)window,(HWND)0x6e1);
            return 0;
        case 0x6e2:
            unknown_a3d2a4=4;
            unknown_online_option_a3d2a8=0;
            control->Set_Window_006010f0((unsigned)window,(HWND)0x6e2);
            return 0;
        case 0x6e3:
            unknown_a3d2a4=2;
            control->Set_Window_006010f0((unsigned)window,(HWND)0x6e3);
            return 0;
        case 0x686:
            unknown_a3d2a4=0;
            control->Set_Window_006010f0((unsigned)window,(HWND)0x686);
            break;
        }
        break;
    case 0x497: {
        HWND image_control=GetDlgItem(window,0x746);
        Surface *image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_standard_icon_007eb1fc,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x747);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_tournament_icon_007eb25c,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x748);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_clan_icon_007eb228,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x749);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_reserved_icon_007eafb4,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x74a);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_observer_icon_007e7444,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x752);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_private_icon_007eb234,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x75c);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_ping_good_icon_007eafa8,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x75d);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_ping_medium_icon_007eaf9c,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x75e);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_ping_poor_icon_007eaf90,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        break;
    }
    }
    return 0;
}


int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
extern int unknown_online_option_a3d2a8;
extern char wol_standard_icon_007eb1fc[];
extern char wol_tournament_icon_007eb25c[];
extern char wol_clan_icon_007eb228[];
extern char wol_reserved_icon_007eafb4[];
extern char wol_observer_icon_007e7444[];
extern char wol_private_icon_007eb234[];
extern char wol_ping_good_icon_007eafa8[];
extern char wol_ping_medium_icon_007eaf9c[];
extern char wol_ping_poor_icon_007eaf90[];
long __stdcall Wonline_Profile_Mode_Menu_Dialog_00752cf0(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    if(message==0x110 && lparam) SetWindowLongA(window,8,*(long *)(lparam+4));
    long result=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(result) return result;
    OwnerDraw::DialogControl *control=(OwnerDraw::DialogControl *)GetWindowLongA(window,8);
    switch(message) {
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
        return 1;
    case 0x14: return 1;
    case 0x2b: Unknown_005FE380(lparam); return 1;
    case 0x111:
        switch(wparam&0xffff) {
        case 0x6e0:
            unknown_a3d2a4=1;
            control->Set_Window_006010f0((unsigned)window,(HWND)0x6e0);
            return 0;
        case 0x6e1:
            unknown_a3d2a4=3;
            control->Set_Window_006010f0((unsigned)window,(HWND)0x6e1);
            return 0;
        case 0x6e2:
            unknown_a3d2a4=4;
            unknown_online_option_a3d2a8=0;
            control->Set_Window_006010f0((unsigned)window,(HWND)0x6e2);
            return 0;
        case 0x6e3:
            unknown_a3d2a4=2;
            control->Set_Window_006010f0((unsigned)window,(HWND)0x6e3);
            return 0;
        case 0x6e4:
            unknown_a3d2a4=5;
            control->Set_Window_006010f0((unsigned)window,(HWND)0x6e4);
            return 0;
        case 0x686:
            unknown_a3d2a4=0;
            control->Set_Window_006010f0((unsigned)window,(HWND)0x686);
            break;
        }
        break;
    case 0x497: {
        if(unknown_wstring_b26cb8.Length()>0) {
            rc_ptr<PlayerProfile> profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
            if(profile.Get()) profile->Populate_Dialog_00739b70(window,unknown_wstring_b26cb8,true);
        }
        HWND image_control=GetDlgItem(window,0x746);
        Surface *image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_standard_icon_007eb1fc,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x747);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_tournament_icon_007eb25c,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x748);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_clan_icon_007eb228,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x749);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_reserved_icon_007eafb4,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x74a);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_observer_icon_007e7444,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x752);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_private_icon_007eb234,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x75c);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_ping_good_icon_007eafa8,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x75d);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_ping_medium_icon_007eaf9c,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        image_control=GetDlgItem(window,0x75e);
        image=unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_ping_poor_icon_007eaf90,0);
        if(image_control && image) OwnerDraw::unknown_5e1f30(image_control,image);
        break;
    }
    }
    return 0;
}


extern unsigned int unknown_game_code_7fd370;
int __fastcall Run_Control_Dialog_006011a0(int,DLGPROC,OwnerDraw::DialogControl *);
long __stdcall Wonline_Selection_Dialog_00752cf0(HWND,unsigned,unsigned,long);
long __stdcall Wonline_Login_Dialog_00752950(HWND,unsigned,unsigned,long);
int Open_Wonline_Selection_00753110()
{
    WolRegistryOptions options;
    options.Read_004ff200();
    unknown_5159b0(0,true);
    if(unknown_ladder_client_a72d00 && strlen(unknown_ladder_address_b29018) && unknown_wstring_b26cb8.Length()>0) {
        unknown_ladder_client_a72d00->Request(unknown_ladder_address_b29018,unknown_ladder_port_b2a8ac,unknown_wstring_b26cb8.Get(),unknown_game_code_7fd370&0xffff00,-1,0,0);
    }
    if(options.unknown_004ff2c0()) {
        SimpleWonlineDialogControl control;
        Run_Control_Dialog_006011a0(0x10e,(DLGPROC)Wonline_Selection_Dialog_00752cf0,&control);
        return unknown_a3d2a4!=0;
    }
    SimpleWonlineDialogControl control;
    Run_Control_Dialog_006011a0(0x11c,(DLGPROC)Wonline_Login_Dialog_00752950,&control);
    return unknown_a3d2a4!=0;
}


WideString __fastcall Locale_Name_0073f1c0(int locale);
void __fastcall Unknown_00753240(HWND combo)
{
    unknown_reset_combo_6007f0(combo);
    int count=0;
    unknown_chat_a72cfc->unknown_slot52(&count);
    WolRegistryOptions options;
    options.Read_004ff200();
    rc_ptr<PlayerProfile> profile;
    if(options.unknown_004ff2c0()) profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
    else profile.Set(unknown_profile_b28cc4);
    if(count>0) {
    for(int locale=count-1;locale>=0;--locale) {
        WideString name=Locale_Name_0073f1c0(locale);
        int item;
        if(locale<2) item=SendMessageA(combo,0x4c1,0,(LPARAM)name.Get());
        else item=SendMessageA(combo,0x4c2,0,(LPARAM)name.Get());
        SendMessageA(combo,0x151,item,locale);
        if(profile.Get() && locale==profile->unknown_10) SendMessageA(combo,0x14e,item,0);
    }
    }
}


int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
void __stdcall unknown_dialog_5ecf40(HWND,int);
void __fastcall unknown_reset_combo_6007f0(HWND);
LRESULT __fastcall unknown_get_window_text(HWND,Wstring &);
LRESULT __fastcall unknown_get_combo_item_wide_text(HWND,WPARAM,WideString &);
void __fastcall Unknown_00753240(HWND);
bool __fastcall Populate_Server_Choices_007454c0(HWND,bool);
void __fastcall Unknown_00739a50(HWND,char);
void __fastcall Create_Wonline_Account_00744a00(HWND);
void __fastcall View_HTML_0073c580(char const *,int);
Wstring __fastcall WOL_Auto_Login_Persona();
Wstring __fastcall WOL_Last_Valid_Persona();
void __fastcall WOL_Set_Auto_Login_Persona(Wstring const &);
void __fastcall WOL_Set_Last_Valid_Persona(Wstring const &);
extern int unknown_online_option_a3d2a8,profile_url_value_b2ac5c;
extern char profile_help_url_7fd564[],profile_registration_url_7fd5a4[],profile_choose_nick_7fd2f0[],wol_source_file_007fd62c[];
long __stdcall Wonline_Profile_Login_Dialog_00753370(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    if(message==0x110 && lparam) SetWindowLongA(window,8,*(long *)(lparam+4));
    long result=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(result) return result;
    OwnerDraw::DialogControl *control=(OwnerDraw::DialogControl *)GetWindowLongA(window,8);
    switch(message) {
    case 0xf:
        Unknown_005FEE70(window);unknown_dialog_5e5f20(window);ValidateRect(window,0);
    painted:return 1;
    case 0x14:goto painted;
    case 0x2b:Unknown_005FE380(lparam);return 1;
    case 0x111:
        switch(wparam&0xffff) {
        case 0x687:
            unknown_a3d2a4=4;unknown_online_option_a3d2a8=5;
            control->Set_Window_006010f0((unsigned)window,(HWND)0x687);return 0;
        case 0x688:View_HTML_0073c580(profile_help_url_7fd564,0);return 0;
        case 0x686:
            unknown_a3d2a4=0;control->Set_Window_006010f0((unsigned)window,(HWND)0x686);return 0;
        case 0x628:
            switch((unsigned short)(wparam>>16)) { case 1: {
                HWND automatic=GetDlgItem(window,0x6ff);
                SendMessageA(automatic,0x49d,0,1);ShowWindow(automatic,0);
                int selected=SendMessageA((HWND)lparam,0x147,0,0);
                int nick_index=-1;
                if(selected!=-1) nick_index=SendMessageA((HWND)lparam,0x150,selected,0);
                unknown_wstring_b26cb8.Release();
                if(nick_index!=-1) {
                    char const *nickname=0,*password=0;
                    if(unknown_chat_a72cfc->unknown_slot36(nick_index,&nickname,&password)==0) unknown_wstring_b26cb8=nickname;
                }
                rc_ptr<PlayerProfile> profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
                Wstring automatic_persona=WOL_Auto_Login_Persona();
                EnableWindow(automatic,nick_index!=-1);
                SendMessageA(automatic,0xf1,(automatic_persona==unknown_wstring_b26cb8)?1:0,0);
                SendMessageA(automatic,0x49d,0,0);ShowWindow(automatic,5);InvalidateRect(automatic,0,0);
                if(profile.Get()) profile->Populate_Dialog_00739b70(window,unknown_wstring_b26cb8,false);
                EnableWindow(GetDlgItem(window,0x6eb),nick_index!=-1 && unknown_wstring_b26cb8.Length()>0);
                EnableWindow(GetDlgItem(window,0x687),nick_index!=-1 && unknown_wstring_b26cb8.Length()>0);
            } break; }
            break;
        case 0x629:
            switch((unsigned short)(wparam>>16)) { case 1: {
                Wstring server;unknown_get_window_text((HWND)lparam,server);
                rc_ptr<PlayerProfile> profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
                if(profile.Get()) {profile->Set_Server_00739920(server);profile->Save_007395d0(unknown_wstring_b26cb8);}
            } break; }
            break;
        case 0x62a:
            switch((unsigned short)(wparam>>16)) { case 1: {
                int locale=0;int selected=SendMessageA((HWND)lparam,0x147,0,0);
                if(selected!=-1) locale=SendMessageA((HWND)lparam,0x150,selected,0);
                rc_ptr<PlayerProfile> profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
                if(profile.Get()) {profile->Set_Locale_00739950(locale,3);profile->Save_007395d0(unknown_wstring_b26cb8);}
            } break; }
            break;
        case 0x6a1:
            switch((unsigned short)(wparam>>16)) { case 1: {
                int value=-2;int selected=SendMessageA((HWND)lparam,0x147,0,0);
                if(selected!=-1) value=SendMessageA((HWND)lparam,0x150,selected,0);
                rc_ptr<PlayerProfile> profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
                if(profile.Get()) {profile->Set_Country_007399b0(value);profile->Save_007395d0(unknown_wstring_b26cb8);}
            } break; }
            break;
        case 0x6a2:
            switch((unsigned short)(wparam>>16)) { case 1: {
                int value=-2;int selected=SendMessageA((HWND)lparam,0x147,0,0);
                if(selected!=-1) value=SendMessageA((HWND)lparam,0x150,selected,0);
                rc_ptr<PlayerProfile> profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
                if(profile.Get()) {profile->Set_Color_007399d0(value);profile->Save_007395d0(unknown_wstring_b26cb8);}
            } break; }
            break;
        case 0x6ff:
            if(SendMessageA((HWND)lparam,0xf0,0,0)==1) WOL_Set_Auto_Login_Persona(unknown_wstring_b26cb8);
            else {
                Wstring automatic=WOL_Auto_Login_Persona();
                if(automatic.As_Lower()==unknown_wstring_b26cb8.As_Lower()) WOL_Set_Auto_Login_Persona(unknown_string_83c55c);
            }
            break;
        case 0x6ea:Create_Wonline_Account_00744a00(window);return 0;
        case 0x6eb: {
            HWND nicknames=GetDlgItem(window,0x628);
            int selected=SendMessageA(nicknames,0x147,0,0);
            if(selected!=-1) {
                WideString wide;unknown_get_combo_item_wide_text(nicknames,selected,wide);
                Wstring nickname=Narrow_Copy_00771280(wide);nickname.To_Lower();
                Wstring automatic=WOL_Auto_Login_Persona();
                if(nickname==automatic.As_Lower()) WOL_Set_Auto_Login_Persona(unknown_string_83c55c);
                Wstring last=WOL_Last_Valid_Persona();
                if(nickname==last.As_Lower()) WOL_Set_Last_Valid_Persona(unknown_string_83c55c);
                int nick_index=SendMessageA(nicknames,0x150,selected,0);
                if(nick_index!=-1) {
                    unknown_chat_a72cfc->unknown_slot37(nick_index,unknown_string_83c55c,unknown_string_83c55c,1);
                    SendMessageA(nicknames,0x144,selected,0);
                }
                SendMessageA(nicknames,0x14e,-1,0);
            }
            Unknown_00739a50(window,1);InvalidateRect(nicknames,0,0);
            EnableWindow(GetDlgItem(window,0x6eb),0);SendMessageA(nicknames,0x14e,0,0);
            rc_ptr<PlayerProfile> profile=PlayerProfile::Load_Default();
            if(profile.Get()) {
                profile->Populate_Dialog_00739b70(window,unknown_string_83c55c,false);
                HWND automatic=GetDlgItem(window,0x6ff);
                SendMessageA(automatic,0x49d,0,1);EnableWindow(automatic,0);
                SendMessageA(automatic,0xf1,0,0);SendMessageA(automatic,0x49d,0,0);InvalidateRect(automatic,0,0);
            }
            return 0;
        }
        case 0x6ec: {
            char url[256];sprintf(url,profile_registration_url_7fd5a4,profile_url_value_b2ac5c);
            View_HTML_0073c580(url,0);return 0;
        }
        }
        break;
    case 0x4ed:ShowWindow(window,0);unknown_dialog_5ecf40(window,1);return 0;
    case 0x497: {
        HWND nicknames=GetDlgItem(window,0x628);
        unknown_reset_combo_6007f0(nicknames);SendMessageA(nicknames,0x141,9,0);
        WideString prompt(StringTable::Load_String(profile_choose_nick_7fd2f0,0,wol_source_file_007fd62c,0x5a2e));
        int item=SendMessageA(nicknames,0x4c2,0,(LPARAM)prompt.unknown_text_770430());
        SendMessageA(nicknames,0x151,item,-1);SendMessageA(nicknames,0x14e,item,0);
        char const *nickname,*password;
        for(int nick_index=1;nick_index<=32;++nick_index) {
            nickname=0;password=0;
            if(unknown_chat_a72cfc->unknown_slot36(nick_index,&nickname,&password)==0 && nickname) {
                if(strlen(nickname)>0) {
                    WideString wide=Wstring_To_Wide(nickname);
                    int entry=SendMessageA(nicknames,0x4c2,0,(LPARAM)wide.unknown_text_770430());
                    SendMessageA(nicknames,0x151,entry,nick_index);
                }
            }
        }
        Unknown_00753240(GetDlgItem(window,0x62a));
        Fill_Side_Combo_4d1560(window,0x6a1);Init_Color_Entries_4d1ea0();Fill_Color_Combo_4d2080(window,0x6a2);
        Populate_Server_Choices_007454c0(GetDlgItem(window,0x629),true);
        rc_ptr<PlayerProfile> profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
        if(profile.Get()) profile->Populate_Dialog_00739b70(window,unknown_wstring_b26cb8,true);
        int selected_nick=-1;int selected=SendMessageA(nicknames,0x147,0,0);
        if(selected!=-1) selected_nick=SendMessageA(nicknames,0x150,selected,0);
        BOOL valid=selected_nick!=-1;
        EnableWindow(GetDlgItem(window,0x6eb),valid);EnableWindow(GetDlgItem(window,0x687),valid);HWND automatic_button=GetDlgItem(window,0x6ff);EnableWindow(automatic_button,valid);
        Wstring automatic=WOL_Auto_Login_Persona();
        SendMessageA(GetDlgItem(window,0x6ff),0xf1,(automatic==unknown_wstring_b26cb8)?1:0,0);
    } break;
    }
    return 0;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Unknown_00753F10() (0x00753F10)
// Clears the WOL channel state: the lobby, game and user lists, the current and pending channel.
// From ra2-decomp 0x00799CD0 (YR twin FUN_00799CD0, src/wonline.cpp).
extern bool unknown_7fd354;

void Unknown_00753F10()
{
    unknown_lobbies_b26c88.setEmpty();
    memset(&unknown_channel_b299c0, 0, sizeof(WOL::Channel));
    memset(&unknown_pending_channel_b26a60, 0, sizeof(WOL::Channel));
    unknown_7fd354 = true;
    unknown_games_b29990.setEmpty();
    unknown_users_b2a728.setEmpty();
    unknown_b2abe8 = 0;
}


extern unsigned int unknown_game_code_7fd370;
extern char wol_source_file_007fd62c[];
extern char wol_wait_label_7fe670[];
int __fastcall Unknown_00742390();
void __fastcall Open_Wait_Window_0074d4d0(unsigned int,WideString const &,bool);
int __fastcall Unknown_00751c30(unsigned int flags);
int __fastcall Run_Control_Dialog_006011a0(int,DLGPROC,OwnerDraw::DialogControl *);
long __stdcall Wonline_Lobby_Dialog_00753370(HWND,unsigned,unsigned,long);
int __fastcall Open_Wonline_Lobby_00753f70()
{
    int initialized=Unknown_00742390();
    if(initialized==1) {
        {
            WideString message(StringTable::Load_String(wol_wait_label_7fe670,0,wol_source_file_007fd62c,0x56cc));
            Open_Wait_Window_0074d4d0(0x10,message,false);
        }
        unsigned int flags=0x2300;
        if(unknown_a3d2a4!=2) flags=0x2100;
        int result=Unknown_00751c30(flags);
        switch(result) {
        case 0: break;
        case 2: return -1;
        default: return -2;
        }
        if(unknown_ladder_client_a72d00 && strlen(unknown_ladder_address_b29018) && unknown_wstring_b26cb8.Length()>0) {
            unknown_ladder_client_a72d00->Request(unknown_ladder_address_b29018,unknown_ladder_port_b2a8ac,unknown_wstring_b26cb8.Get(),unknown_game_code_7fd370&0xffff00,-1,0,0);
        }
        SimpleWonlineDialogControl control;
        Run_Control_Dialog_006011a0(0x114,(DLGPROC)Wonline_Lobby_Dialog_00753370,&control);
        return unknown_a3d2a4!=0;
    }
    return initialized;
}


// long __stdcall Wonline_Account_Dialog_007540a0(HWND window, UINT message, WPARAM wparam, LPARAM lparam) (0x007540A0)
int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
void __stdcall unknown_dialog_5ecf40(HWND,int);
void __fastcall unknown_reset_combo_6007f0(HWND);
LRESULT __fastcall unknown_get_window_text(HWND,Wstring &);
LRESULT __fastcall unknown_get_combo_item_text(HWND,WPARAM,Wstring &);
void __fastcall Unknown_00753240(HWND);
bool __fastcall Populate_Server_Choices_007454c0(HWND,bool);
void __fastcall Create_Wonline_Account_00744a00(HWND);
void __fastcall View_HTML_0073c580(char const *,int);
Wstring __fastcall WOL_Auto_Login_Persona();
Wstring __fastcall WOL_Last_Valid_Persona();
void __fastcall WOL_Set_Auto_Login_Persona(Wstring const &);
void __fastcall WOL_Set_Last_Valid_Persona(Wstring const &);
extern int profile_url_value_b2ac5c;
extern char profile_registration_url_7fd5a4[];
extern bool account_dialog_updating_b2aca8;
extern bool remember_password_b2ac74;

void *__fastcall unknown_find_key_by_value(int value);

// Looks text up among the stored nicknames; a match becomes the current nickname and password.
inline bool Find_Stored_Nick(Wstring const &text)
{
    for (int nick_index = 1; nick_index <= 32; nick_index++) {
        char const *nickname, *password;
        if (unknown_chat_a72cfc->unknown_slot36(nick_index, &nickname, &password) == 0 && nickname != 0
            && strlen(nickname) > 0 && _stricmp(text.Peek_Buffer(), nickname) == 0) {
            unknown_wstring_b26cb8 = nickname;
            unknown_password_b2a63c = password;
            unknown_7fd374 = false;
            return true;
        }
    }
    unknown_wstring_b26cb8.Release();
    return false;
}

long __stdcall Wonline_Account_Dialog_007540a0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == 0x110 && lparam) SetWindowLongA(window, 8, *(long *)(lparam + 4));
    long result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result) return result;
    OwnerDraw::DialogControl *control = (OwnerDraw::DialogControl *)GetWindowLongA(window, 8);
    switch (message) {
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window, 0);
        return 1;
    case 0x14:
        return 1;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0x111:
        switch ((unsigned short)wparam) {
        case 0x539:
            control->Set_Window_006010f0((unsigned)window, (HWND)0x539);
            return 0;
        case 0x686:
            control->Set_Window_006010f0((unsigned)window, (HWND)2);
            return 0;
        case 0x628: {
            bool have_key = unknown_find_key_by_value(0x11d) != 0;
            switch ((unsigned short)(wparam >> 16)) {
            case 1: {
                if (have_key) break;
                account_dialog_updating_b2aca8 = true;
                unknown_wstring_b26cb8.Release();
                unknown_password_b2a63c.Release();
                unknown_7fd374 = true;
                HWND edit = GetDlgItem(window, 0x72c);
                SendMessageA(edit, 0x4b4, 0, (LPARAM)unknown_password_b2a63c.Peek_Buffer());
                bool remember = remember_password_b2ac74;
                int selected = SendMessageA((HWND)lparam, 0x147, 0, 0);
                if (selected != -1) {
                    int nick_index = SendMessageA((HWND)lparam, 0x150, selected, 0);
                    if (nick_index > 0 && nick_index <= 32) {
                        char const *nickname, *password;
                        unknown_get_combo_item_text((HWND)lparam, selected, unknown_wstring_b26cb8);
                        unknown_chat_a72cfc->unknown_slot36(nick_index, &nickname, &password);
                        unknown_password_b2a63c = password;
                        edit = GetDlgItem(window, 0x72c);
                        SendMessageA(edit, 0x4b4, 0, (LPARAM)unknown_password_b2a63c.Peek_Buffer());
                        unknown_7fd374 = false;
                        remember = true;
                    }
                }
                bool valid = unknown_wstring_b26cb8.Length() > 0 && unknown_password_b2a63c.Length() == 8;
                HWND check = GetDlgItem(window, 0x72d);
                SendMessageA(check, 0xf1, remember, 0);
                EnableWindow(check, 0);
                InvalidateRect(check, 0, 0);
                EnableWindow(GetDlgItem(window, 0x6eb), unknown_wstring_b26cb8.Length() > 0);
                EnableWindow(GetDlgItem(window, 0x539), valid);
                account_dialog_updating_b2aca8 = false;
                return 0;
            }
            case 5:
            case 0x300: {
                if ((unsigned short)(wparam >> 16) == 0x300 && !have_key) break;
                if ((unsigned short)(wparam >> 16) == 5 && have_key) break;
                if (account_dialog_updating_b2aca8) break;
                Wstring text;
                unknown_get_window_text((HWND)lparam, text);
                unknown_password_b2a63c.Release();
                bool match = Find_Stored_Nick(text);
                bool remember = match || remember_password_b2ac74;
                if (!match) {
                    unknown_wstring_b26cb8 = text;
                }
                account_dialog_updating_b2aca8 = true;
                HWND edit = GetDlgItem(window, 0x72c);
                SendMessageA(edit, 0x4b4, 0, (LPARAM)unknown_password_b2a63c.Peek_Buffer());
                InvalidateRect(edit, 0, 0);
                account_dialog_updating_b2aca8 = false;
                bool valid = unknown_wstring_b26cb8.Length() > 0 && unknown_password_b2a63c.Length() == 8;
                HWND check = GetDlgItem(window, 0x72d);
                ShowWindow(check, 0);
                SendMessageA(check, 0x49d, 0, 1);
                SendMessageA(check, 0xf1, remember, 0);
                EnableWindow(check, 0);
                SendMessageA(check, 0x49d, 0, 0);
                ShowWindow(check, 5);
                InvalidateRect(check, 0, 0);
                EnableWindow(GetDlgItem(window, 0x6eb), unknown_wstring_b26cb8.Length() > 0);
                EnableWindow(GetDlgItem(window, 0x539), valid);
                return 0;
            }
            }
            break;
        }
        case 0x629:
            if (unknown_find_key_by_value(0x11d) && unknown_profile_b28cc4) {
                Wstring server;
                unknown_get_window_text(GetDlgItem(window, 0x629), server);
                unknown_profile_b28cc4->Set_Server_00739920(server);
                return 0;
            }
            break;
        case 0x62a:
            if (unknown_find_key_by_value(0x11d) && unknown_profile_b28cc4) {
                HWND combo = GetDlgItem(window, 0x62a);
                int selected = SendMessageA(combo, 0x147, 0, 0);
                int locale = 0;
                if (selected != -1) {
                    locale = SendMessageA(combo, 0x150, selected, locale);
                }
                unknown_profile_b28cc4->Set_Locale_00739950(locale, 3);
                return 0;
            }
            break;
        case 0x6a1:
            if (unknown_find_key_by_value(0x11d) && unknown_profile_b28cc4) {
                int value = -2;
                HWND combo = GetDlgItem(window, 0x6a1);
                int selected = SendMessageA(combo, 0x147, 0, 0);
                if (selected != -1) value = SendMessageA(combo, 0x150, selected, 0);
                unknown_profile_b28cc4->Set_Country_007399b0(value);
                return 0;
            }
            break;
        case 0x6a2:
            if (unknown_find_key_by_value(0x11d) && unknown_profile_b28cc4) {
                int value = -2;
                HWND combo = GetDlgItem(window, 0x6a2);
                int selected = SendMessageA(combo, 0x147, 0, 0);
                if (selected != -1) value = SendMessageA(combo, 0x150, selected, 0);
                unknown_profile_b28cc4->Set_Color_007399d0(value);
                return 0;
            }
            break;
        case 0x72c:
            if ((unsigned short)(wparam >> 16) == 0x300 && !account_dialog_updating_b2aca8) {
                unknown_7fd374 = true;
                unknown_get_window_text((HWND)lparam, unknown_password_b2a63c);
                Wstring text;
                unknown_get_window_text(GetDlgItem(window, 0x628), text);
                bool valid = text.Length() > 0 && unknown_password_b2a63c.Length() == 8;
                HWND check = GetDlgItem(window, 0x72d);
                SendMessageA(check, 0x49d, 0, 1);
                EnableWindow(check, valid);
                InvalidateRect(check, 0, 0);
                SendMessageA(check, 0x49d, 0, 0);
                EnableWindow(GetDlgItem(window, 0x6eb), 0);
                EnableWindow(GetDlgItem(window, 0x539), valid);
                return 0;
            }
            break;
        case 0x72d:
            remember_password_b2ac74 = SendMessageA((HWND)lparam, 0xf0, 0, 0) == 1;
            return 0;
        case 0x6eb: {
            account_dialog_updating_b2aca8 = true;
            HWND nicknames = GetDlgItem(window, 0x628);
            int selected = SendMessageA(nicknames, 0x4b9, -1, (LPARAM)unknown_wstring_b26cb8.Peek_Buffer());
            if (selected != -1) {
                Wstring nickname;
                unknown_get_combo_item_text(nicknames, selected, nickname);
                nickname.To_Lower();
                Wstring automatic = WOL_Auto_Login_Persona();
                if (nickname == automatic.As_Lower()) {
                    WOL_Set_Auto_Login_Persona(unknown_string_83c55c);
                }
                Wstring last = WOL_Last_Valid_Persona();
                if (nickname == last.As_Lower()) {
                    WOL_Set_Last_Valid_Persona(unknown_string_83c55c);
                }
                int nick_index = SendMessageA(nicknames, 0x150, selected, 0);
                SendMessageA(nicknames, 0x144, selected, 0);
                unknown_chat_a72cfc->unknown_slot37(nick_index, unknown_string_83c55c, unknown_string_83c55c, 1);
            }
            SendMessageA(nicknames, 0x4b4, 0, (LPARAM)unknown_string_83c55c);
            InvalidateRect(nicknames, 0, 0);
            HWND edit = GetDlgItem(window, 0x72c);
            SendMessageA(edit, 0x4b4, 0, (LPARAM)unknown_string_83c55c);
            unknown_wstring_b26cb8.Release();
            unknown_password_b2a63c.Release();
            unknown_7fd374 = true;
            HWND check = GetDlgItem(window, 0x72d);
            EnableWindow(check, 0);
            InvalidateRect(check, 0, 0);
            EnableWindow(GetDlgItem(window, 0x6eb), 0);
            EnableWindow(GetDlgItem(window, 0x539), 0);
            account_dialog_updating_b2aca8 = false;
            return 0;
        }
        case 0x6ea:
            Create_Wonline_Account_00744a00(window);
            return 0;
        case 0x6ec: {
            char url[256];
            sprintf(url, profile_registration_url_7fd5a4, profile_url_value_b2ac5c);
            View_HTML_0073c580(url, 0);
            return 0;
        }
        }
        break;
    case 0x497: {
        account_dialog_updating_b2aca8 = true;
        HWND nicknames = GetDlgItem(window, 0x628);
        HWND edit = GetDlgItem(window, 0x72c);
        SendMessageA(nicknames, 0x4d1, 1, 0);
        SendMessageA(edit, 0x4d1, 1, 0);
        SendMessageA(nicknames, 0xc5, 9, 0);
        SendMessageA(edit, 0xc5, 8, 0);
        unknown_reset_combo_6007f0(nicknames);
        SendMessageA(nicknames, 0x141, 9, 0);
        for (int nick_index = 1; nick_index < 32; nick_index++) {
            char const *nickname, *password;
            if (unknown_chat_a72cfc->unknown_slot36(nick_index, &nickname, &password) == 0 && nickname != 0 && strlen(nickname) > 0) {
                int entry = SendMessageA(nicknames, 0x4bc, 0, (LPARAM)nickname);
                SendMessageA(nicknames, 0x151, entry, nick_index);
            }
        }
        Wstring saved(unknown_wstring_b26cb8);
        unknown_password_b2a63c.Release();
        bool remember = Find_Stored_Nick(unknown_wstring_b26cb8);
        if (!remember) {
            unknown_wstring_b26cb8 = saved;
        }
        SendMessageA(nicknames, 0x4b4, 0, (LPARAM)unknown_wstring_b26cb8.Peek_Buffer());
        if (unknown_wstring_b26cb8.Length() > 0) {
            SendMessageA(edit, 0x4b4, 0, (LPARAM)unknown_password_b2a63c.Peek_Buffer());
            unknown_7fd374 = false;
        }
        bool valid = unknown_wstring_b26cb8.Length() > 0 && unknown_password_b2a63c.Length() == 8;
        HWND check = GetDlgItem(window, 0x72d);
        SendMessageA(check, 0x49d, 0, 1);
        SendMessageA(check, 0xf1, remember, 0);
        EnableWindow(check, 0);
        InvalidateRect(check, 0, 0);
        SendMessageA(check, 0x49d, 0, 0);
        EnableWindow(GetDlgItem(window, 0x6eb), unknown_wstring_b26cb8.Length() > 0);
        EnableWindow(GetDlgItem(window, 0x539), valid);
        WolRegistryOptions options;
        options.Read_004ff200();
        if (!options.unknown_004ff2c0()) {
            Unknown_00753240(GetDlgItem(window, 0x62a));
            Fill_Side_Combo_4d1560(window, 0x6a1);
            HWND sides = GetDlgItem(window, 0x6a1);
            int count = SendMessageA(sides, 0x146, 0, 0);
            int index;
            for (index = 0; index < count; index++) {
                if (SendMessageA(sides, 0x150, index, 0) == unknown_profile_b28cc4->Country) {
                    SendMessageA(sides, 0x14e, index, 0);
                    break;
                }
            }
            Init_Color_Entries_4d1ea0();
            Fill_Color_Combo_4d2080(window, 0x6a2);
            HWND colors = GetDlgItem(window, 0x6a2);
            int count2 = SendMessageA(colors, 0x146, 0, 0);
            for (index = 0; index < count2; index++) {
                if (SendMessageA(colors, 0x150, index, 0) == unknown_profile_b28cc4->Color) {
                    SendMessageA(colors, 0x14e, index, 0);
                    break;
                }
            }
            Populate_Server_Choices_007454c0(GetDlgItem(window, 0x629), true);
        }
        account_dialog_updating_b2aca8 = false;
        return 0;
    }
    case 0x4da: {
        unknown_get_window_text(GetDlgItem(window, 0x72c), unknown_password_b2a63c);
        Wstring text;
        unknown_get_window_text(GetDlgItem(window, 0x628), text);
        if (text.Length() > 0 && unknown_password_b2a63c.Length() == 8) {
            PostMessageA(GetDlgItem(window, 0x539), 0xf5, 0, 0);
        }
        break;
    }
    case 0x4db: {
        bool flag = wparam != 0;
        HWND child = (HWND)lparam;
        HWND parent = GetParent(child);
        while (parent != 0) {
            if (child == 0) return 0;
            if (parent == window) break;
            child = parent;
            parent = GetParent(child);
        }
        if (child != 0) Unknown_005def80((long)child, flag);
        break;
    }
    case 0x4ed:
        ShowWindow(window, 0);
        unknown_dialog_5ecf40(window, 1);
        break;
    }
    return 0;
}


extern "C" __declspec(dllimport) UINT __stdcall IsDlgButtonChecked(HWND,int);
int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall Wait_Paint_005e5f20(HWND);
void __fastcall Set_Auto_Login_00738da0(bool value);
void __fastcall Unknown_007391f0(Wstring const &name);
long __stdcall Auto_Login_Confirmation_Dialog_00755070(HWND window,unsigned message,unsigned wparam,long lparam)
{
    if(message==0x110 && lparam) SetWindowLongA(window,8,*(long *)(lparam+4));
    int handled=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(handled) return handled;
    OwnerDraw::DialogControl *control=(OwnerDraw::DialogControl *)GetWindowLongA(window,8);
    switch(message) {
    case 0x111:
        switch(wparam & 0xffff) {
        case 1:
            Unknown_007391f0(unknown_wstring_b26cb8);
            control->Set_Window_006010f0((unsigned)window,(HWND)1);
            break;
        case 2:
            {
                bool checked=IsDlgButtonChecked(window,0x6f9)==1;
                Set_Auto_Login_00738da0(!checked);
                control->Set_Window_006010f0((unsigned)window,(HWND)2);
            }
            return 0;
        }
        break;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0x14: return 1;
    case 0xf:
        Unknown_005FEE70(window);
        Wait_Paint_005e5f20(window);
        ValidateRect(window,0);
        return 1;
    }
    return 0;
}


WideString __fastcall unknown_number_text_7710c0(int number);
void __fastcall Update_Buddy_Buttons_007561c0(HWND dialog);
void __fastcall Update_Buddy_Profile_007562b0(HWND dialog);
extern "C" long __cdecl atol(char const *text);
extern "C" int __cdecl strncmp(char const *a,char const *b,unsigned int count);
extern "C" char *__cdecl strncpy(char *out,char const *in,unsigned int count);
extern wchar_t buddy_00839d4c[];
extern char buddy_007fd62c[];
extern char buddy_007ffb1c[];
extern char buddy_007ffb08[];
extern char buddy_007ffae0[];
extern char buddy_007ffaf0[];
extern char buddy_007ffabc[];
extern char buddy_007ffaa4[];
extern char buddy_007fd358[];
extern wchar_t buddy_007d64f4[];
extern wchar_t buddy_007fd8b8[];
extern char buddy_007eb25c[];
extern char buddy_007fd890[];
extern char buddy_007eb228[];
extern char buddy_007fd8a4[];
extern char buddy_007ffad4[];
extern char buddy_007eb218[];
extern char buddy_007eafb4[];
extern char buddy_007e7444[];
extern char buddy_007eb234[];
extern char buddy_007fd87c[];
extern char buddy_007fd864[];
extern int buddy_column_007fd308;
extern int buddy_column_007fd30c;
extern int buddy_column_007fd310;
extern int buddy_column_007fd314;
extern int buddy_column_007fd318;
extern int buddy_column_007fd31c;

void __fastcall Unknown_00755180(HWND dialog)
{
    HWND list=GetDlgItem(dialog,0x689);
    SendMessageA(list,0x49d,0,1);
    for(int i=0;i<unknown_strings_b26920.Count;++i) {
        Wstring buddy;
        unknown_strings_b26920.Get(buddy,i);
        WideString display=Wstring_To_Wide(buddy);
        unsigned int state=0;
        unknown_names_b29848.Get_Value(buddy,state);
        int row=SendMessageA(list,0x4c5,0,(LPARAM)buddy.Peek_Buffer());
        if(row==-1) row=SendMessageA(list,0x4c8,0,(LPARAM)buddy.Peek_Buffer());
        WolLadderListCell name;
        name.Flags=1; name.Text.Set(display.Get()); name.Secondary.Set(buddy_00839d4c);
        WolLadderListCell status;
        status.Flags=1; status.Secondary.Set(buddy_00839d4c);
        WolLadderListCell mode;
        mode.Flags=0; mode.Text.Set(buddy_00839d4c); mode.Secondary.Set(buddy_00839d4c);
        WolLadderListCell password;
        password.Flags=0; password.Text.Set(buddy_00839d4c); password.Secondary.Set(buddy_00839d4c);
        WolLadderListCell gameType;
        gameType.Flags=0; gameType.Text.Set(buddy_00839d4c); gameType.Secondary.Set(buddy_00839d4c);
        WolLadderListCell ping;
        ping.Flags=0; ping.Text.Set(buddy_00839d4c); ping.Secondary.Set(buddy_00839d4c);
        switch(state) {
        case 1:
            status.Text.Set(StringTable::Load_String(buddy_007ffb1c,0,buddy_007fd62c,0x5f37));
            name.unknown_0c=status.unknown_0c=0x808080; break;
        case 2:
            status.Text.Set(StringTable::Load_String(buddy_007ffb08,0,buddy_007fd62c,0x5f3d));
            name.unknown_0c=status.unknown_0c=0x808080; break;
        case 3:
            status.Text.Set(StringTable::Load_String(buddy_007ffae0,0,buddy_007fd62c,0x5f43));
            name.unknown_0c=status.unknown_0c=0xf0f0; break;
        case 7: case 8:
            status.Text.Set(StringTable::Load_String(buddy_007ffaf0,0,buddy_007fd62c,0x5f4a));
            name.unknown_0c=status.unknown_0c=0x80ff00; break;
        case 4: {
            WOL::Channel channel;
            memset(&channel,0,sizeof(channel));
            if(!unknown_channels_b29478.Get_Value(buddy,channel)) {
                status.Text.Set(StringTable::Load_String(buddy_007ffae0,0,buddy_007fd62c,0x5f63));
                name.unknown_0c=status.unknown_0c=0xf0f0;
            } else {
                if(channel.unknown_00==0 && strncmp(channel.channel_name,buddy_007fd358,strlen(buddy_007fd358))==0) {
                    int lobby=atol(channel.channel_name+strlen(buddy_007fd358));
                    int page=lobby/8;
                    WideString location((wchar_t const*)unknown_lobby_names_b26c94[lobby%8]);
                    if(page) {
                        location+=buddy_007d64f4;
                        location+=unknown_number_text_7710c0(page+1);
                    }
                    status.Text.Set(location.Get());
                } else status.Text.Set(Narrow_To_Wide_006fa180(channel.channel_name));
                name.unknown_0c=status.unknown_0c=0xff4000;
            }
            break;
        }
        case 5: {
            WOL::Channel game;
            memset(&game,0,sizeof(game));
            bool found=unknown_channels_b29478.Get_Value(buddy,game);
            if(found) { Wstring game_name(game.channel_name); found=unknown_games_b29990.Find_Name(game_name,game); }
            if(found) {
            char options[132];
            strncpy(options,game.exInfo,128);
            int first=0,second=0,third=0,fourth=0,fifth=0;
            Wstring rest;
            Parse_Channel_Info_0073d070(options,&first,&second,&third,&fourth,&fifth,&rest);
            unsigned int current=game.currentUsers-fourth+third;
            int reserved=(fourth!=0 || fifth!=0);
            unsigned int maximum=game.maxUsers-reserved;
            wchar_t details[80];
            swprintf(details,buddy_007fd8b8,game.channel_name,current,maximum);
            status.Text.Set(details);
            name.unknown_0c=status.unknown_0c=0xffff00;
            mode.Flags=2;
            if(game.unknown_14!=0) {
                mode.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(buddy_007eb25c,0);
                mode.Secondary.Set(StringTable::Load_String(buddy_007fd890,0,buddy_007fd62c,0x5f87));
            } else if(game.unknown_20&0x100) {
                mode.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(buddy_007eb228,0);
                mode.Secondary.Set(StringTable::Load_String(buddy_007fd8a4,0,buddy_007fd62c,0x5f8a));
            } else {
                mode.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(buddy_007ffad4,0);
                mode.Secondary.Set(buddy_00839d4c);
            }
            if(game.unknown_20&0x600) {
                if(fourth==0 && fifth!=0) {
                    gameType.Flags=2;
                    gameType.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(buddy_007eb218,0);
                } else {
                    gameType.Flags=2;
                    gameType.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(buddy_007eafb4,0);
                }
            } else if(fourth==0 && fifth!=0) {
                gameType.Flags=2;
                gameType.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(buddy_007e7444,0);
            }
            if(game.flags&0x100) {
                password.Flags=2;
                password.unknown_10=(int)unknown_bitmap_writer_a76378.Get_Image_0068ca40(buddy_007eb234,0);
                password.Secondary.Set(StringTable::Load_String(buddy_007fd87c,0,buddy_007fd62c,0x5faa));
            }
            ping.Flags=3;
            ping.unknown_14=game.ping;
            swprintf(details,StringTable::Load_String(buddy_007fd864,0,buddy_007fd62c,0x5faf),game.ping);
            ping.Secondary.Set(details);
            } else {
                status.Text.Set(StringTable::Load_String(buddy_007ffae0,0,buddy_007fd62c,0x5fb2));
                name.unknown_0c=status.unknown_0c=0xf0f0;
            }
            break;
        }
        case 6:
            status.Text.Set(StringTable::Load_String(buddy_007ffabc,0,buddy_007fd62c,0x5fb9));
            name.unknown_0c=status.unknown_0c=0x3050ff; break;
        default:
            status.Text.Set(StringTable::Load_String(buddy_007ffaa4,0,buddy_007fd62c,0x5fbf));
            name.unknown_0c=status.unknown_0c=0x808080; break;
        }
        SendMessageA(list,0x4a8,((unsigned short)row<<16) | ((unsigned short)buddy_column_007fd308),(LPARAM)&name);
        SendMessageA(list,0x4a8,((unsigned short)row<<16) | ((unsigned short)buddy_column_007fd30c),(LPARAM)&mode);
        SendMessageA(list,0x4a8,((unsigned short)row<<16) | ((unsigned short)buddy_column_007fd310),(LPARAM)&password);
        SendMessageA(list,0x4a8,((unsigned short)row<<16) | ((unsigned short)buddy_column_007fd314),(LPARAM)&gameType);
        SendMessageA(list,0x4a8,((unsigned short)row<<16) | ((unsigned short)buddy_column_007fd318),(LPARAM)&status);
        SendMessageA(list,0x4a8,((unsigned short)row<<16) | ((unsigned short)buddy_column_007fd31c),(LPARAM)&ping);
    }
    int wanted=-1;
    if(unknown_wstring_b29cd4.Length()>0) wanted=SendMessageA(list,0x4c5,0,(LPARAM)unknown_wstring_b29cd4.Peek_Buffer());
    if(wanted!=SendMessageA(list,0x188,0,0)) SendMessageA(list,0x186,wanted,0);
    SendMessageA(list,0x49d,0,0);
    InvalidateRect(list,0,0);
    Update_Buddy_Buttons_007561c0(dialog);
    Update_Buddy_Profile_007562b0(dialog);
}


int __fastcall Channel_Version_00737af0();
// The existing parser's final4-byte string slot is a Wstring: the caller constructs and destroys it.
unsigned int Unknown_0073cfd0();
extern int unknown_own_squad_b2abfc;
extern char wol_nosquad_label_7ffb44[];
extern char wol_channel_full_label_7ffb34[];
extern char wol_observers_label_7e6cdc[];
extern char wol_wrong_version_label_7e21ac[];
extern char wol_source_file_007fd62c[];
bool __fastcall Can_Join_Channel_00755cd0(WOL::Channel const *channel,bool notify,bool observer)
{
    bool ladder=Unknown_0073c3e0(channel)||Unknown_0073c400(channel);
    int online=Is_Online_Session();
    if(ladder!=online) return false;
    if(Unknown_0073c3c0(channel) && unknown_own_squad_b2abfc<1) {
        if(notify) Unknown_005C2C80(-1,StringTable::Load_String(wol_nosquad_label_7ffb44,0,wol_source_file_007fd62c,0x5fe7));
        return false;
    }
    char info[132];
    strcpy(info,channel->exInfo);
    int version=0;
    int checksum=0;
    int reserved=0;
    int observers=0;
    int allow_observers=0;
    Wstring name;
    Parse_Channel_Info_0073d070(info,&version,&checksum,&reserved,&observers,&allow_observers,&name);
    if(Channel_Version_00737af0()==version && Unknown_0073cfd0()==(unsigned int)checksum) {
        if(!observer) {
            int occupancy=((int)channel->currentUsers-observers)+reserved;
            int observer_slot=(observers==0 && allow_observers==0)?0:1;
            if(occupancy>=(int)channel->maxUsers-observer_slot) {
                if(notify) Unknown_005C2C80(-1,StringTable::Load_String(wol_channel_full_label_7ffb34,0,wol_source_file_007fd62c,0x6016));
            } else return true;
        } else {
            if(allow_observers==0 || observers!=0) {
                if(notify) Unknown_005C2C80(-1,StringTable::Load_String(wol_observers_label_7e6cdc,0,wol_source_file_007fd62c,0x601e));
            } else return true;
        }
    } else if(notify) {
        Unknown_005C2C80(-1,StringTable::Load_String(wol_wrong_version_label_7e21ac,0,wol_source_file_007fd62c,0x600b));
    }
    return false;
}


bool __fastcall Can_Join_Channel_00755cd0(WOL::Channel const *,bool,bool);
bool __fastcall Can_Join_Selected_Buddy_00755ef0()
{
    if(unknown_wstring_b29cd4.Length()==0) return false;
    unsigned int state=0;
    unknown_names_b29848.Get_Value(unknown_wstring_b29cd4,state);
    if(state==4) return true;
    if(state!=5) return false;
    WOL::Channel channel;
    if(!unknown_channels_b29478.Get_Value(unknown_wstring_b29cd4,channel)) return false;
    Wstring game_name(channel.channel_name);
    WOL::Channel game;
    if(!unknown_games_b29990.Find_Name(game_name,game)) return false;
    return Can_Join_Channel_00755cd0(&game,false,unknown_a72c68!=0);
}


void __fastcall Open_Page_Dialog_007560e0()
{
    HWND dialog=(HWND)unknown_create_dialog_735700(ProgramInstance,0x11e,MainWindow,(UnknownDialogProc)Page_Dialog_Proc_00757d80,0);
    unknown_show_dialog_5ff7e0(dialog,0x11e);
    SendMessageA(dialog,0x4a9,0,1);
    ShowWindow(dialog,1);
    Unknown_007359e0(dialog,Unknown_0073e100,0,1,1);
}


bool Selected_Buddy_Unavailable_00756140()
{
    if(unknown_wstring_b29cd4.Length()==0) return false;
    unsigned int status=0;
    unknown_names_b29848.Get_Value(unknown_wstring_b29cd4,status);
    if((int)status>=0 && (int)status<=2) return false;
    return true;
}


bool __fastcall Can_Join_Selected_Buddy_00755ef0();
bool Selected_Buddy_Unavailable_00756140();
void __fastcall Update_Buddy_Buttons_007561c0(HWND dialog)
{
    EnableWindow(GetDlgItem(dialog,0x703),unknown_wstring_b29cd4.Length()>0);
    EnableWindow(GetDlgItem(dialog,0x62d),(unsigned char)Can_Join_Selected_Buddy_00755ef0());
    EnableWindow(GetDlgItem(dialog,0x5c2),(unsigned char)Selected_Buddy_Unavailable_00756140());
}


extern "C" int __cdecl swprintf(wchar_t *,wchar_t const *,...);
WideString __fastcall unknown_number_text_7710c0(int number);
extern char wol_profile_label_7fd2d8[],wol_profile_unranked_7fd2c4[],wol_source_file_007fd62c[];
extern unsigned short wol_empty_text_839d4c[];
void __fastcall Update_Buddy_Profile_007562b0(HWND dialog)
{
    HWND label=GetDlgItem(dialog,0x6ef);
    HWND rank=GetDlgItem(dialog,0x6f4);
    HWND points=GetDlgItem(dialog,0x6f5);
    HWND wins=GetDlgItem(dialog,0x6f0);
    HWND losses=GetDlgItem(dialog,0x6f1);
    HWND disconnects=GetDlgItem(dialog,0x701);
    if(unknown_wstring_b29cd4.Length()==0) {
        SendMessageA(label,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
        SendMessageA(rank,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
        SendMessageA(points,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
        SendMessageA(wins,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
        SendMessageA(losses,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
        SendMessageA(disconnects,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
        return;
    }
    WideString format(StringTable::Load_String(wol_profile_label_7fd2d8,0,wol_source_file_007fd62c,0x607c));
    unsigned length=format.Length()+unknown_wstring_b29cd4.Length();
    wchar_t *text=new wchar_t[length+1];
    swprintf(text,(wchar_t const *)format.Get(),unknown_wstring_b29cd4.Get());
    HWND title=GetDlgItem(dialog,0x6ef);
    SendMessageA(title,0x4b2,0,(LPARAM)text);
    delete [] text;
    WOL::Ladder entry;
    memset(&entry,0,sizeof(entry));
    Wstring name=unknown_wstring_b29cd4.As_Lower();
    bool found;
    {
        WOL::Ladder *value=0;
        found=unknown_ladder_b2ab98.Get_Pointer_007670f0(name,&value);
        if(found && value) entry=*value;
    }
    if(found) {
        SendMessageA(rank,0x4b2,0,(LPARAM)unknown_number_text_7710c0(entry.unknown_1c).unknown_text_770430());
        SendMessageA(points,0x4b2,0,(LPARAM)unknown_number_text_7710c0(entry.unknown_10).unknown_text_770430());
        SendMessageA(wins,0x4b2,0,(LPARAM)unknown_number_text_7710c0(entry.unknown_08).unknown_text_770430());
        SendMessageA(losses,0x4b2,0,(LPARAM)unknown_number_text_7710c0(entry.unknown_0c).unknown_text_770430());
        SendMessageA(disconnects,0x4b2,0,(LPARAM)unknown_number_text_7710c0(entry.unknown_20).unknown_text_770430());
    } else {
        WideString unranked(StringTable::Load_String(wol_profile_unranked_7fd2c4,0,wol_source_file_007fd62c,0x608d));
        SendMessageA(rank,0x4b2,0,(LPARAM)unranked.unknown_text_770430());
        SendMessageA(points,0x4b2,0,(LPARAM)unranked.unknown_text_770430());
        SendMessageA(wins,0x4b2,0,(LPARAM)unranked.unknown_text_770430());
        SendMessageA(losses,0x4b2,0,(LPARAM)unranked.unknown_text_770430());
        SendMessageA(disconnects,0x4b2,0,(LPARAM)unranked.unknown_text_770430());
    }
}


extern "C" long __cdecl time(long *);
void Service_Buddy_Find_Queue_00756610()
{
    if(unknown_wstring_b26944.Length()>0) {
        if((unsigned long)(time(0)-unknown_b2a7f8)<5) return;
        unknown_wstring_b26944.Release();
    }
    if(unknown_strings_b29838.Count!=0) {
        unknown_strings_b29838.Remove_Head(unknown_wstring_b26944);
        unknown_b2a7f8=time(0);
        WOL::User user;
        memset(&user,0,sizeof(user));
        strcpy(user.name,unknown_wstring_b26944.Get());
        unknown_chat_a72cfc->unknown_slot24(&user);
    }
}


extern char wol_invalid_name_chars_7fea20[];
bool __fastcall Unknown_00756790(Wstring const &name)
{
    Wstring lower_name(name);
    lower_name.To_Lower();
    if(name.Length()==0) return false;
    if(lower_name.As_Lower()==unknown_wstring_b26cb8.As_Lower()) return false;
    if(name.Contains_Any(wol_invalid_name_chars_7fea20)) return false;
    for(int i=0;i<unknown_strings_b26920.Count;++i) {
        Wstring other;
        unknown_strings_b26920.Get(other,i);
        other.To_Lower();
        if(other==lower_name) return false;
    }
    return true;
}


extern "C" long __cdecl time(long *timer);
bool __fastcall Unknown_00756790(Wstring const &name);
void __fastcall Unknown_007568c0(Wstring const &name)
{
    if(Unknown_00756790(name)) {
        Wstring friend_name(name);
        unknown_strings_b26920.Add(friend_name);
        unknown_strings_b29838.Add(friend_name);
        unknown_strings_b2a7d8.Add(friend_name);
        bool request=true;
        if(unknown_wstring_b26944.Length()>0) {
            if((unsigned long)(time(0)-unknown_b2a7f8)<5) request=false;
            else unknown_wstring_b26944.Release();
        }
        if(request && unknown_strings_b29838.Count!=0) {
            if(unknown_strings_b29838.Count>0) {
                unknown_strings_b29838.Get(unknown_wstring_b26944,0);
                unknown_strings_b29838.Remove_007661f0(0);
            }
            unknown_b2a7f8=time(0);
            WOL::User query;
            memset(&query,0,sizeof(query));
            strcpy(query.name,unknown_wstring_b26944.Get());
            unknown_chat_a72cfc->unknown_slot24(&query);
        }
    }
}


extern "C" long __cdecl time(long *timer);
bool __fastcall Unknown_00756790(Wstring const &name);
void __fastcall Unknown_00756b40(Wstring const &name)
{
    if(Unknown_00756790(name)) {
        WOL::User user;
        memset(&user,0,sizeof(user));
        strncpy(user.name,name.Peek_Buffer(),9);
        unknown_chat_a72cfc->unknown_slot56(&user);
        if(Unknown_00756790(name)) {
            Wstring friend_name(name);
            unknown_strings_b26920.Add(friend_name);
            unknown_strings_b29838.Add(friend_name);
            unknown_strings_b2a7d8.Add(friend_name);
            bool request=true;
            if(unknown_wstring_b26944.Length()>0) {
                if((unsigned long)(time(0)-unknown_b2a7f8)<5) request=false;
                else unknown_wstring_b26944.Release();
            }
            if(request && unknown_strings_b29838.Count!=0) {
                if(unknown_strings_b29838.Count>0) {
                    unknown_strings_b29838.Get(unknown_wstring_b26944,0);
                    unknown_strings_b29838.Remove_007661f0(0);
                }
                unknown_b2a7f8=time(0);
                WOL::User query;
                memset(&query,0,sizeof(query));
                strcpy(query.name,unknown_wstring_b26944.Get());
                unknown_chat_a72cfc->unknown_slot24(&query);
            }
        }
        Unknown_00757380();
    }
}


void __fastcall Unknown_00756e10(Wstring const &name)
{
    for(int i=0;i<unknown_strings_b26920.Count;++i) {
        Wstring other;
        unknown_strings_b26920.Get(other,i);
        if(other==name) {
            unknown_strings_b26920.Remove(i);
            Wstring key(name);
            WOL::Channel channel;
            unknown_channels_b29478.Remove_00767d60(key,&channel);
            unknown_names_b29848.Remove_Name(key);
            WOL::User user;
            memset(&user,0,sizeof(user));
            strncpy(user.name,name.Peek_Buffer(),9);
            unknown_chat_a72cfc->unknown_slot57(&user);
            return;
        }
    }
}


extern "C" long __cdecl time(long *);
extern HANDLE wol_wait_events_b264f8[18];
extern unsigned long buddy_refresh_time_b2a6f0;
bool __fastcall Queue_Channel_List_Request_0073db20(int channel_type,unsigned char autoping,bool wait);
void Service_Buddy_Find_Queue_00756610();
inline bool Request_Buddy_Channel_List()
{
    return Queue_Channel_List_Request_0073db20(0x21, 1, false);
}

void Refresh_Buddy_Status_007570e0()
{
    buddy_refresh_time_b2a6f0 = time(0);
    if (!Request_Buddy_Channel_List()) {
        SetEvent(wol_wait_events_b264f8[5]);
        return;
    }
    unknown_strings_b29838.Clear();
    for (int i = 0; i < unknown_strings_b26920.Count; i++) {
        Wstring name;
        unknown_strings_b26920.Get(name, i);
        unknown_strings_b29838.Add_Tail(name);
    }
    Service_Buddy_Find_Queue_00756610();
}


extern char wol_buddy_separator_7e6f8c[];
void Unknown_00757380()
{
    int count=unknown_strings_b2a7d8.Count;
    if(count>10) count=10;
    else if(count==0) return;
    Wstring names;
    if(count>0) {
        int remaining=count;
        do {
            Wstring name;
            if(unknown_strings_b2a7d8.Count>0) {
                unknown_strings_b2a7d8.Get(name,0);
                unknown_strings_b2a7d8.Remove(0);
            }
            if(names.Length()>0) names+=wol_buddy_separator_7e6f8c;
            names+=name;
        } while(--remaining);
    }
    unknown_ladder_client_a72d00->Request(unknown_ladder_address_b29018,unknown_ladder_port_b2a8ac,names.Get(),unknown_protocol_7fd370 & 0xffff00,-1,0,0);
}


void Reset_Buddy_Name_List_00757530()
{
    unknown_strings_b2a7d8.Clear();
    for(int i=0;i<unknown_strings_b26920.Count;++i) {
        Wstring name;
        unknown_strings_b26920.Get(name,i);
        unknown_strings_b2a7d8.Add_Tail(name);
    }
    Unknown_00757380();
}


void Refresh_Buddy_Status_007570e0();
void Reset_Buddy_Name_List_00757530();
inline void WolNameTable::Clear_007662f0()
{
    unsigned int index;
    for (index = 0; index < BucketCount; index++) {
        Node *node = Buckets[index];
        while (node != 0) {
            Node *old = node;
            node = node->Next;
            if (old) delete old;
        }
        Buckets[index] = 0;
    }
    Count = 0;
    while (Get_Size() > MinimumBuckets && ResizeLocked == false) Shrink_Table();
}

inline void WolChannelDictionary::Clear_00766400()
{
    unsigned int index;
    for (index = 0; index < BucketCount; index++) {
        Node *node = Buckets[index];
        while (node != 0) {
            Node *old = node;
            node = node->Next;
            if (old) delete old;
        }
        Buckets[index] = 0;
    }
    Count = 0;
    while (Get_Size() > MinimumBuckets && ResizeLocked == false) Shrink_Table();
}

inline void Restart_Buddy_Requests()
{
    unknown_chat_a72cfc->unknown_slot55();
    if (!Is_Online_Session()) Refresh_Buddy_Status_007570e0();
    Reset_Buddy_Name_List_00757530();
}

void __fastcall Reset_Buddy_State_00757720()
{
    unknown_strings_b26920.Clear();
    unknown_strings_b29838.Clear();
    unknown_names_b29848.Clear_007662f0();
    unknown_channels_b29478.Clear_00766400();
    Restart_Buddy_Requests();
}


int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
LRESULT __fastcall unknown_get_window_wide_text(HWND,WideString &);
extern unsigned short wol_empty_text_839d4c[];
long __stdcall Buddy_Message_Dialog_00757950(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    long result=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(result) return result;
    switch(message) {
    case 2: return 0;
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
        return 1;
    case 0x14: return 1;
    case 0x2b: Unknown_005FE380(lparam); return 1;
    case 0x111:
        switch(wparam&0xffff) {
        case 0x540:
            switch(wparam>>16) {
            case 0x300: {
                WideString text;
                unknown_get_window_wide_text((HWND)lparam,text);
                EnableWindow(GetDlgItem(window,0x5c2),0<text.Length());
                break;
            }
            case 0x400: {
                WideString text;
                unknown_get_window_wide_text((HWND)lparam,text);
                text.Trim_Trailing(L"\n\r");
                return 0;
            }
            case 0x501:
                SendMessageA(GetDlgItem(window,0x5c2),0xf5,0,0);
                break;
            }
            break;
        case 0x5c2: {
            WideString text;
            HWND edit=GetDlgItem(window,0x540);
            unknown_get_window_wide_text(edit,text);
            SendMessageA(edit,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
            if(text.Length()>0) {
                text.Trim_Trailing(L"\n\r");
                WOL::User user;
                memset(&user,0,sizeof(user));
                strcpy(user.name,unknown_wstring_b29cd4.Get());
                unknown_chat_a72cfc->unknown_slot62(&user,text.Get());
            }
            InvalidateRect(edit,0,0);
            Unknown_007357E0(window,0x5c2);
            break;
        }
        case 0x686:
            Unknown_007357E0(window,0x686);
            break;
        }
        break;
    case 0x497: {
        HWND edit=GetDlgItem(window,0x540);
        SendMessageA(edit,0xc5,0x78,0);
        SendMessageA(edit,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
        EnableWindow(GetDlgItem(window,0x5c2),0);
        HWND name=GetDlgItem(window,0x712);
        SendMessageA(name,0x4b4,0,(LPARAM)unknown_wstring_b29cd4.Peek_Buffer());
        break;
    }
    }
    return 0;
}


// Wstring __fastcall Unknown_00757cc0(HWND list) (0x00757CC0)
// The first list entry from the selection on whose text starts with '(' and holds a ')': that text between
// the parentheses; unknown_wstring_b29cd4 when there is none.

Wstring __fastcall Unknown_00757cc0(HWND list)
{
    int index = SendMessageA(list, 0x188, 0, 0);
    if (index == -1) index = 0;
    while (index < SendMessageA(list, 0x18B, 0, 0)) {
        WideString text;
        unknown_get_list_item_wide_text(list, index, text);
        if (text.Get_Char(0) == '(' && text.Truncate_At(')')) {
            text.Remove_Character('(');
            return Narrow_Copy_00771280(text);
        }
        index++;
    }
    return unknown_wstring_b29cd4;
}


int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
LRESULT __fastcall unknown_get_window_wide_text(HWND,WideString &);
extern unsigned short wol_empty_text_839d4c[];
int WINAPI Page_Dialog_Proc_00757d80(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    WOL::User user;
    int result=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(result) return result;
    switch(message) {
    case 2: return 0;
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
        return 1;
    case 0x14: return 1;
    case 0x2b: Unknown_005FE380(lparam); return 1;
    case 0x111:
        switch((int)(wparam&0xffff)) {
        case 0x540:
            switch((unsigned short)(wparam>>16)) {
            case 0x300: {
                WideString text;
                unknown_get_window_wide_text((HWND)lparam,text);
                EnableWindow(GetDlgItem(window,0x5c2),0<text.Length());
                return 0;
            }
            case 0x400: {
                WideString text;
                unknown_get_window_wide_text((HWND)lparam,text);
                text.Trim_Trailing(L"\n\r");
                return 0;
            }
            case 0x501: {
                WideString text;
                unknown_get_window_wide_text((HWND)lparam,text);
                text.Trim_Trailing(L"\n\r");
                if(text.Length()>0) SendMessageA(GetDlgItem(window,0x5c2),0xf5,0,0);
                return 0;
            }
            }
            break;
        case 0x53f:
            switch((unsigned short)(wparam>>16)) { case 1: {
                Wstring recipient=Unknown_00757cc0((HWND)lparam);
                HWND target=GetDlgItem(window,0x718);
                SendMessageA(target,0x4b4,0,(LPARAM)recipient.Peek_Buffer());
            } break; }
            break;
        case 0x5c2: {
            WideString text;
            HWND edit=GetDlgItem(window,0x540);
            unknown_get_window_wide_text(edit,text);
            SendMessageA(edit,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
            Wstring recipient=Unknown_00757cc0(GetDlgItem(window,0x53f));
            if(recipient.Length()>0 && text.Length()>0) {
                text.Trim_Trailing(L"\n\r");
                memset(&user,0,sizeof(user));
                strcpy(user.name,recipient.Get());
                unknown_chat_a72cfc->unknown_slot62(&user,text.Get());
                Unknown_005C2C80(unknown_color_7a7364,L"[%hs] %s",unknown_wstring_b26cb8.Buffer,text.Get());
            }
            EnableWindow((HWND)lparam,0);
            InvalidateRect(edit,0,0);
            break;
        }
        case 0x686:
            Unknown_007357E0(window,0x5c2);
            break;
        }
        break;
    case 0x497: {
        HWND edit=GetDlgItem(window,0x540);
        SendMessageA(edit,0xc5,0x78,0);
        SendMessageA(edit,0x4b2,0,(LPARAM)wol_empty_text_839d4c);
        EnableWindow(GetDlgItem(window,0x5c2),0);
        Set_Edit_Text(GetDlgItem(window,0x718),Unknown_00757cc0(GetDlgItem(window,0x53f)));
        if(WWMouse) WWMouse->Release_Mouse();
        break;
    }
    }
    return 0;
}


int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
LRESULT __fastcall unknown_get_window_text(HWND,Wstring &);
extern "C" long __cdecl time(long *timer);
extern char wol_empty_name_83c55c[];
long __stdcall Add_Buddy_Dialog_00758210(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    if(message==0x110 && lparam) SetWindowLongA(window,8,*(long *)(lparam+4));
    long result=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(result) return result;
    OwnerDraw::DialogControl *control=(OwnerDraw::DialogControl *)GetWindowLongA(window,8);
    switch(message) {
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
    case 0x14: return 1;
    case 0x2b: Unknown_005FE380(lparam); return 1;
    case 0x111:
        switch(wparam&0xffff) {
        case 0x540:
            switch((unsigned short)(wparam>>16)) {
            case 0: break;
            default: {
                Wstring name;
                unknown_get_window_text((HWND)lparam,name);
                EnableWindow(GetDlgItem(window,0x702),Unknown_00756790(name));
                break;
            }
            }
            break;
        case 0x686:
            control->Set_Window_006010f0((unsigned)window,(HWND)0x686);
            break;
        case 0x702: {
            Wstring name;
            HWND edit=GetDlgItem(window,0x540);
            unknown_get_window_text(edit,name);
            SendMessageA(edit,0x4b4,0,(LPARAM)wol_empty_name_83c55c);
            if(Unknown_00756790(name)) {
                if(Unknown_00756790(name)) {
                    WOL::User user;
                    memset(&user,0,sizeof(user));
                    strncpy(user.name,name.Peek_Buffer(),9);
                    unknown_chat_a72cfc->unknown_slot56(&user);
                    if(Unknown_00756790(name)) {
                        Wstring friend_name(name);
                        unknown_strings_b26920.Add(friend_name);
                        unknown_strings_b29838.Add(friend_name);
                        unknown_strings_b2a7d8.Add(friend_name);
                        bool request=true;
                        if(unknown_wstring_b26944.Length()>0) {
                            if((unsigned long)(time(0)-unknown_b2a7f8)<5) request=false;
                            else unknown_wstring_b26944.Release();
                        }
                        if(request && unknown_strings_b29838.Count!=0) {
                            if(unknown_strings_b29838.Get_007662c0(unknown_wstring_b26944,0))
                                unknown_strings_b29838.Remove_007661f0(0);
                            unknown_b2a7f8=time(0);
                            WOL::User query;
                            memset(&query,0,sizeof(query));
                            strcpy(query.name,unknown_wstring_b26944.Get());
                            unknown_chat_a72cfc->unknown_slot24(&query);
                        }
                    }
                    Unknown_00757380();
                }
            }
            InvalidateRect(edit,0,0);
            control->Set_Window_006010f0((unsigned)window,(HWND)0x702);
            break;
        }
        }
        break;
    case 0x497: {
        HWND edit=GetDlgItem(window,0x540);
        SendMessageA(edit,0xc5,9,0);
        SendMessageA(edit,0x4d1,1,0);
        SendMessageA(edit,0x4b4,0,(LPARAM)wol_empty_name_83c55c);
        EnableWindow(GetDlgItem(window,0x702),0);
        break;
    }
    case 0x4da:
        PostMessageA(GetDlgItem(window,0x702),0xf5,0,0);
        break;
    }
    return 0;
}


// int Buddy_Dialog_Proc(HWND, UINT, WPARAM, LPARAM) (0x00758750): the buddy list dialog run by 0x00759370.
#ifndef LOWORD
#define LOWORD(l) ((unsigned short)(l))
#define HIWORD(l) ((unsigned short)(((unsigned long)(l) >> 16) & 0xFFFF))
#endif
extern "C" long __cdecl time(long *);
extern HANDLE wol_wait_events_b264f8[18];
extern int buddy_column_b29460, buddy_column_b26cb4;
extern int buddy_columns_7fd308[6];
extern unsigned long buddy_refresh_time_b2a6f0;
extern char wol_label_7eb2cc[], wol_label_7eb2c0[];
extern HWND game_window_00b263d8;
extern void *unknown_instance_b26180;
int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_dialog_5e5f20(HWND);
bool __fastcall Request_Game_Channel_List_0073df10(unsigned char autoping,bool wait);
bool __fastcall Can_Join_Selected_Buddy_00755ef0();
bool Selected_Buddy_Unavailable_00756140();
void __fastcall Update_Buddy_Buttons_007561c0(HWND dialog);
void __fastcall Update_Buddy_Profile_007562b0(HWND dialog);
void Service_Buddy_Find_Queue_00756610();
void Reset_Buddy_Name_List_00757530();
void __fastcall Unknown_00755180(HWND dialog);
void Refresh_Buddy_Status_007570e0();
void __fastcall Reset_Buddy_State_00757720();
void __fastcall Unknown_00756e10(Wstring const &name);
void __fastcall unknown_show_dialog_5ff7e0(void *handle,int kind);
int __fastcall Run_Control_Dialog_006011a0(int,DLGPROC,OwnerDraw::DialogControl *);
LRESULT __fastcall unknown_get_window_text(HWND,Wstring &);
LRESULT __fastcall unknown_get_window_item_text(HWND,WPARAM,Wstring &);
long __stdcall Add_Buddy_Dialog_00758210(HWND,UINT,WPARAM,LPARAM);
int __stdcall Page_Dialog_Proc_00757d80(HWND,UINT,WPARAM,LPARAM);

inline void Run_Add_Buddy_Dialog(HWND window)
{
    SimpleWonlineDialogControl add;
    Run_Control_Dialog_006011a0(0x115, (DLGPROC)Add_Buddy_Dialog_00758210, &add);
    Unknown_00755180(window);
}

inline void Start_Buddy_Refresh(HWND window,HWND refresh)
{
    SendMessageA(refresh, 0x4aa, 0, (LPARAM)unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_label_7eb2c0, 0));
    Reset_Buddy_State_00757720();
    SetTimer(window, 0, 1000, 0);
}

int __stdcall Buddy_Dialog_Proc_00758750(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == 0x110 && lparam) SetWindowLongA(window, 8, *(long *)(lparam + 4));
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result) return result;
    OwnerDraw::DialogControl *control = (OwnerDraw::DialogControl *)GetWindowLongA(window, 8);
    switch (message) {
    case 0x14:
        return 1;
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window, 0);
        return 1;
    case 2:
        KillTimer(window, 0);
        KillTimer(window, 0x704);
        Unknown_005CD3A0();
        return 0;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0x111:
        if (control->unknown_slot2()) return 0;
        switch (LOWORD(wparam)) {
        case 0x5c2: {
            HWND page = (HWND)unknown_create_dialog_735700(unknown_instance_b26180, 0x11e, game_window_00b263d8, (UnknownDialogProc)Page_Dialog_Proc_00757d80, 0);
            unknown_show_dialog_5ff7e0(page, 0x11e);
            SendMessageA(page, 0x4a9, 0, 1);
            ShowWindow(page, 1);
            Dialog_Loop_007359e0(page, Unknown_0073e100, 0, true, true);
            return 0;
        }
        case 0x540:
            if (HIWORD(wparam) == 0x300) {
                Wstring text;
                unknown_get_window_text((HWND)lparam, text);
                EnableWindow(GetDlgItem(window, 0x702), text.Length() != 0);
            }
            return 0;
        case 0x62b:
            control->Set_Window_006010f0((unsigned)window, (HWND)0x62b);
            return 0;
        case 0x686:
            control->Set_Window_006010f0((unsigned)window, (HWND)0x686);
            return 0;
        case 0x62d:
            control->Set_Window_006010f0((unsigned)window, (HWND)0x62d);
            return 0;
        case 0x689:
            switch (wparam >> 16) {
            case 2:
                if (Can_Join_Selected_Buddy_00755ef0()) {
                    SendMessageA(GetDlgItem(window, 0x62d), 0xf5, 0, 0);
                    return 0;
                }
                if (Selected_Buddy_Unavailable_00756140()) {
                    SendMessageA(GetDlgItem(window, 0x5c2), 0xf5, 0, 0);
                }
                return 0;
            case 1: {
                int selected = SendMessageA((HWND)lparam, 0x188, 0, 0);
                if (selected == -1) unknown_wstring_b29cd4.Release();
                else unknown_get_window_item_text((HWND)lparam, selected, unknown_wstring_b29cd4);
                Update_Buddy_Buttons_007561c0(window);
                Update_Buddy_Profile_007562b0(window);
                return 0;
            }
            }
            return 0;
        case 0x760:
        case 0x75f: {
            int checked = 0;
            HWND button = GetDlgItem(window, 0x760);
            if (button) checked = SendMessageA(button, 0xf0, 0, 0);
            if ((short)wparam == 0x75f) checked = !checked;
            unknown_a72c68 = checked;
            Unknown_005cd540(0);
            return 0;
        }
        case 0x702:
            Run_Add_Buddy_Dialog(window);
            return 0;
        case 0x703: {
            HWND list = GetDlgItem(window, 0x689);
            int selected = SendMessageA(list, 0x188, 0, 0);
            if (selected != -1) {
                Wstring name;
                unknown_get_window_item_text(list, selected, name);
                Unknown_00756e10(name);
                SendMessageA(list, 0x182, selected, 0);
                unknown_wstring_b29cd4.Release();
                Unknown_00755180(window);
            }
            return 0;
        }
        case 0x704:
            EnableWindow((HWND)lparam, 0);
            Refresh_Buddy_Status_007570e0();
            SetTimer(window, 0x704, 3000, 0);
            return 0;
        }
        return 0;
    case 0x113:
        switch (wparam) {
        case 0x704:
            KillTimer(window, 0x704);
            EnableWindow(GetDlgItem(window, 0x704), 1);
            return 0;
        case 0:
            if ((unsigned long)(time(0) - buddy_refresh_time_b2a6f0) > 30) {
                Refresh_Buddy_Status_007570e0();
            }
            return 0;
        }
        return 0;
    case 0x497: {
        Unknown_005cd540(window);
        HWND list = GetDlgItem(window, 0x689);
        SendMessageA(list, 0x4a6, buddy_column_b29460, buddy_columns_7fd308[0]);
        SendMessageA(list, 0x4a6, 0, buddy_columns_7fd308[1]);
        SendMessageA(list, 0x4a6, 0, buddy_columns_7fd308[2]);
        SendMessageA(list, 0x4a6, 0, buddy_columns_7fd308[3]);
        SendMessageA(list, 0x4a6, buddy_column_b26cb4, buddy_columns_7fd308[4]);
        SendMessageA(list, 0x4a6, 0, buddy_columns_7fd308[5]);
        HWND refresh = GetDlgItem(window, 0x704);
        SendMessageA(refresh, 0x49c, 0, (LPARAM)unknown_bitmap_writer_a76378.Get_Image_0068ca40(wol_label_7eb2cc, 0));
        Start_Buddy_Refresh(window, refresh);
        return 0;
    }
    }
    return 0;
}


bool __fastcall Find_Channel_By_Name_007592d0(Wstring const &name,WolChannelArray const &channels,WOL::Channel &channel)
{
    memset(&channel,0,sizeof(channel));
    for(int i=0;i<channels.Count;++i) {
        channels.Get(channel,i);
        if(_stricmp(channel.name,name.Peek_Buffer())==0) return true;
    }
    memset(&channel,0,sizeof(channel));
    return false;
}


// int Buddy_List_Main(bool first_time) (0x00759370): the buddy list screen, run from Wonline_Main.
extern "C" __declspec(dllimport) DWORD __stdcall WaitForMultipleObjects(DWORD, HANDLE *, BOOL, DWORD);
extern "C" long __cdecl atol(char const *);
extern HANDLE wol_wait_events_b264f8[18];
extern int wol_lan_leave_b2abf0;
extern int unknown_b2abec;
extern int unknown_online_option_a3d2a8;
extern bool unknown_7fd354;
extern int unknown_a72c68;
extern char wol_source_file_007fd62c[];
extern char wol_youre_discon_label_7fe1fc[];
extern char lobby_prefix_007fd358[];
extern char buddy_host_failed_7ffd50[], buddy_host_abort_7ffd30[], buddy_host_exit_7ffd10[], buddy_host_stop_7ffcfc[];
extern char buddy_guest_list_failed_7ffcd0[], buddy_guest_find_failed_7ffca0[], buddy_guest_join_failed_7ffc7c[];
extern char buddy_guest_abort_7ffc5c[], buddy_guest_exit_7ffc3c[], buddy_guest_error_7ffc1c[], buddy_guest_join_fail_7ffbf0[];
extern char buddy_guest_timeout_7ffbd0[], buddy_guest_done_7ffbb4[], buddy_skirmish_7ffb98[], buddy_host_start_7ffb74[], buddy_guest_start_7ffb50[];
bool __fastcall Queue_Channel_List_Request_0073db20(int channel_type,unsigned char autoping,bool wait);
bool __fastcall Request_Game_Channel_List_0073df10(unsigned char autoping,bool wait);
bool __fastcall Leave_Current_Channel_00744610(unsigned int flags,bool show);
bool __fastcall Join_Game_Channel_00745780(WOL::Channel const *,bool);
bool __fastcall Host_Game_007459a0();
bool __fastcall Find_Channel_By_Name_007592d0(Wstring const &name,WolChannelArray const &channels,WOL::Channel &channel);
bool __fastcall Request_Chat_Channel_List_0073dd20(bool wait);
bool connection_wait_callback_0073c120();
HWND __fastcall unknown_create_dialog_5ff610(int,DLGPROC,int);
void __fastcall unknown_dialog_5ff7c0(HWND);
void __fastcall unknown_dialog_5ff6e0(HWND);
void __fastcall unknown_517370(bool);
bool __fastcall End_Dialog_007357e0(HWND dialog,int result);
int __stdcall Buddy_Dialog_Proc_00758750(HWND,UINT,WPARAM,LPARAM);
inline void __fastcall Close_Wonline_Wait(unsigned int flags);

inline bool Refresh_Game_List()
{
    if (!Request_Game_Channel_List_0073df10(0, true)) {
        Debug_Printf(buddy_guest_list_failed_7ffcd0);
        SetEvent(wol_wait_events_b264f8[5]);
        return false;
    }
    return true;
}

inline void Disconnected(HWND dialog, int line)
{
    Close_Wonline_Wait(~0u);
    unknown_dialog_5ff6e0(dialog);
    unknown_online_option_a3d2a8 = 0;
    Message_Box_005c3230(StringTable::Load_String(wol_youre_discon_label_7fe1fc, 0, wol_source_file_007fd62c, line), 0, connection_wait_callback_0073c120, false);
}

inline bool Request_Chat_Channels()
{
    return Queue_Channel_List_Request_0073db20(0, 0, true);
}

inline void Reset_Wait_Event(DWORD event)
{
    ResetEvent(wol_wait_events_b264f8[event]);
}

int __fastcall Unknown_00759370(bool first_time)
{
    unknown_b2abd9 = false;
    unknown_b2ac40 = 0;
    Unknown_00514EE0();
    if (!Request_Chat_Channels()) {
        unknown_online_option_a3d2a8 = 0;
        return 0;
    }
    Leave_Current_Channel_00744610(0x40, true);
    unknown_channels_b29478.Clear_00766400();
    for (;;) {
        SimpleWonlineDialogControl control;
        Reset_Wait_Events();
        HWND dialog = unknown_create_dialog_5ff610(0x116, Buddy_Dialog_Proc_00758750, (int)&control);
        unknown_dialog_5ff7c0(dialog);
        while (!control.unknown_slot2()) {
            DWORD event = WaitForMultipleObjects(18, wol_wait_events_b264f8, 0, 100);
            while (event == 0x102 && !control.unknown_slot2()) {
                Unknown_0073e100();
                event = WaitForMultipleObjects(18, wol_wait_events_b264f8, 0, 100);
            }
            Reset_Wait_Event(event);
            if (event == 0) {
                Disconnected(dialog, 0x63f1);
                return 0;
            }
            if (event == 5) {
                Disconnected(dialog, 0x63f9);
                return 0;
            }
            unknown_517370(false);
        }
        unknown_dialog_5ff6e0(dialog);
        switch ((int)control.Get_Window_00601180()) {
        case 0x62b:
            Reset_Wait_Events();
            if (!Host_Game_007459a0()) {
                Debug_Printf(buddy_host_failed_7ffd50);
            } else {
                wol_lan_leave_b2abf0 = 0;
                do {
                    DWORD event;
                    do {
                        event = WaitForMultipleObjects(18, wol_wait_events_b264f8, 0, 20);
                        if (event == 0x102) {
                            Unknown_0073e100();
                        }
                    } while (event == 0x102 && !wol_lan_leave_b2abf0);
                    Reset_Wait_Event(event);
                    if (event == 0) {
                        Debug_Printf(buddy_host_abort_7ffd30);
                        wol_lan_leave_b2abf0 = 1;
                    }
                    if (event == 6) {
                        Debug_Printf(buddy_host_exit_7ffd10);
                        wol_lan_leave_b2abf0 = 1;
                    }
                    if (event == 15) {
                        Debug_Printf(buddy_skirmish_7ffb98);
                        unknown_b2ac04 = false;
                        unknown_server_error_b2a8ec[0] = 0;
                        unknown_b2abd9 = false;
                        return 2;
                    } else if (event == 7) {
                        Debug_Printf(buddy_host_start_7ffb74);
                        unknown_b2ac04 = false;
                        unknown_chat_a72cfc->unknown_slot09();
                        unknown_server_error_b2a8ec[0] = 0;
                        unknown_b2abd9 = true;
                        wol_lan_leave_b2abf0 = 0;
                        return 1;
                    }
                } while (!wol_lan_leave_b2abf0);
                Debug_Printf(buddy_host_stop_7ffcfc);
                wol_lan_leave_b2abf0 = 0;
            }
            Close_Wonline_Wait(~0u);
            Reset_Wait_Events();
            if (unknown_server_error_b2a8ec[0]) {
                Message_Box_005c3230(unknown_server_error_b2a8ec, 0, connection_wait_callback_0073c120, false);
            }
            while (End_Dialog_007357e0(0, 0)) {
            }
            unknown_b2abd9 = false;
            break;
        case 0x62d: {
            WOL::Channel channel;
            memset(&channel, 0, sizeof(channel));
            if (unknown_wstring_b29cd4.Length() > 0) {
                bool have_channel;
                {
                    WOL::Channel *found = 0;
                    have_channel = unknown_channels_b29478.Get_Pointer_00767cf0(unknown_wstring_b29cd4, &found);
                    if (have_channel && found) channel = *found;
                }
                if (have_channel) {
                    int kind = 0;
                    bool have_kind;
                    {
                        unsigned int *value = 0;
                        have_kind = unknown_names_b29848.Get_Pointer_00767bb0(unknown_wstring_b29cd4, &value);
                        if (have_kind && value) kind = *value;
                    }
                    if (have_kind) {
                        switch (kind) {
                        case 4: {
                            WOL::Channel lobby;
                            memset(&lobby, 0, sizeof(lobby));
                            unknown_online_option_a3d2a8 = 3;
                            for (int i = 0; i < unknown_lobbies_b26c88.Count; i++) {
                                unknown_lobbies_b26c88.Get(lobby, i);
                                if (!_stricmp(lobby.name, channel.name)) {
                                    unknown_b2abec = atol(channel.name + strlen(lobby_prefix_007fd358));
                                    unknown_7fd354 = false;
                                    return 0;
                                }
                            }
                            return 0;
                        }
                        case 5: {
                            Wstring name(channel.name);
                            WOL::Channel game;
                            if (!Find_Channel_By_Name_007592d0(name, unknown_games_b29990, game)) {
                                bool const &ok = Refresh_Game_List() ? Find_Channel_By_Name_007592d0(name, unknown_games_b29990, game) : false;
                                if (!ok) {
                                    Debug_Printf(buddy_guest_find_failed_7ffca0);
                                    goto guest_done;
                                }
                            }
                            if (!Join_Game_Channel_00745780(&game, unknown_a72c68)) {
                                Debug_Printf(buddy_guest_join_failed_7ffc7c);
                            } else {
                                wol_lan_leave_b2abf0 = 0;
                                int timeout = 500;
                                do {
                                    DWORD event = WaitForMultipleObjects(18, wol_wait_events_b264f8, 0, 20);
                                    while (event == 0x102 && !wol_lan_leave_b2abf0) {
                                        Unknown_0073e100();
                                        event = WaitForMultipleObjects(18, wol_wait_events_b264f8, 0, 20);
                                    }
                                    Reset_Wait_Event(event);
                                    if (event == 0) {
                                        Debug_Printf(buddy_guest_abort_7ffc5c);
                                        wol_lan_leave_b2abf0 = 1;
                                    }
                                    if (event == 6) {
                                        Debug_Printf(buddy_guest_exit_7ffc3c);
                                        wol_lan_leave_b2abf0 = 1;
                                    }
                                    if (event == 5) {
                                        Debug_Printf(buddy_guest_error_7ffc1c);
                                        wol_lan_leave_b2abf0 = 1;
                                    }
                                    if (event == 10) {
                                        Debug_Printf(buddy_guest_join_fail_7ffbf0);
                                        wol_lan_leave_b2abf0 = 1;
                                    }
                                    if (event == 7) {
                                        Debug_Printf(buddy_guest_start_7ffb50);
                                        unknown_b2ac04 = false;
                                        unknown_chat_a72cfc->unknown_slot09();
                                        unknown_server_error_b2a8ec[0] = 0;
                                        unknown_b2abd9 = true;
                                        wol_lan_leave_b2abf0 = 0;
                                        return 1;
                                    }
                                    if (timeout-- < 0) {
                                        Debug_Printf(buddy_guest_timeout_7ffbd0);
                                        break;
                                    }
                                } while (!wol_lan_leave_b2abf0);
                                wol_lan_leave_b2abf0 = 0;
                                Debug_Printf(buddy_guest_done_7ffbb4);
                            }
                            guest_done:
                            break;
                        }
                        }
                    }
                }
            }
            break;
        }
        case 0x5c0:
        case 0x686:
            if (unknown_online_option_a3d2a8 == 3) {
                unknown_7fd354 = false;
            }
            return 0;
        default:
            break;
        }
    }
}


// Start_Quick_Match 0x00759E20: quick or WDT match. Leaves any channel, finds the matching lobby, joins it and waits in
// the waiting dialog for the match server. Returns 1 when the game is starting. Original 2,202 B + 6 NOP.
extern "C" __declspec(dllimport) DWORD __stdcall WaitForMultipleObjects(DWORD,HANDLE *,BOOL,DWORD);
extern HANDLE wol_wait_events_b264f8[18];
extern int unknown_b2ac2c,unknown_b2ab88;
extern long unknown_b2a8a4;
extern Wstring unknown_b2997c;
extern HWND game_window_00b263d8;
extern void *ProgramInstance;
extern char wol_source_file_007fd62c[],wol_leave_debug_7fe6f8[],wol_leaving_label_7fe6e4[],wol_joining_label_7fe688[];
extern char wol_wdt_resolution_7ffe14[],wol_quick_resolution_7ffdf4[],wol_no_agent_7ffdd8[],wol_no_server_7ffdb8[];
extern char wol_join_failure_7ffda0[],wol_match_timeout_7fd768[],wol_match_fail_7ffd8c[],wol_match_starting_7ffd74[];
extern char wol_lobby_prefix_35_7fd368[],wol_lobby_prefix_38_7fd360[],wol_lobby_password_7fe738[];
extern "C" int __cdecl _strnicmp(char const *,char const *,unsigned int);
bool __fastcall Test_Display_Mode(int width,int height);
bool __fastcall Get_Channel_List_0073d8f0();
bool __fastcall Request_Channel_List_0073d760(int,unsigned char);
bool __fastcall Unknown_00514EE0();
void __fastcall Collect_Remote_Player_Names_0075a6c0();
void __fastcall Service_Pending_Player_Pings_0075b2d0();
bool __fastcall Start_Match_0075be10();
int __stdcall Wonline_Waiting_Dialog_0075c350(HWND,UINT,WPARAM,LPARAM);
int __stdcall Wonline_Map_Download_Dialog_0075c830(HWND,UINT,WPARAM,LPARAM);
void __fastcall unknown_show_dialog_5ff7e0(void *,int);
bool __fastcall Queue_Channel_List_Request_0073db20(int channel_type,unsigned char autoping,bool wait);
void __fastcall Reset_Buddy_State_00757720();
inline void __fastcall Set_Matchmaking_State(int state,int timeout,int result)
{
    unknown_b2ac2c=result;
    unknown_b26898=state;
    unknown_b2ab88=timeout;
    unknown_b2a8a4=time(0);
}
inline void Match_Message(char const *label,int line)
{
    Message_Box_005c3230(StringTable::Load_String(label,0,wol_source_file_007fd62c,line),0,Unknown_0073c120,false);
}
int __fastcall Start_Quick_Match_00759e20()
{
    if(unknown_a72c68) {
        unknown_a72c68=0;
        Unknown_005cd540(0);
    }
    unknown_b2abd9=false;
    Set_Matchmaking_State(1,-1,0);
    if(unknown_8523e0>640) {
        WideString query(unknown_session_a3d298.Type==4&&unknown_a3d2a4==2?
            StringTable::Load_String(wol_wdt_resolution_7ffe14,0,wol_source_file_007fd62c,0x64d8):
            StringTable::Load_String(wol_quick_resolution_7ffdf4,0,wol_source_file_007fd62c,0x64d9));
        if(Message_Box_005c3230(query.Get(),4,Unknown_0073c120,true)!=6) return 0;
        if(!Test_Display_Mode(640,480)) return 0;
        Options.ScreenWidth=640;
        Options.ScreenHeight=480;
        Options.Save_Settings();
    }
    unknown_b2ac40=0;
    Unknown_00514EE0();
    Set_Matchmaking_State(2,30,0);
    unknown_lobbies_b26c88.setEmpty();
    unknown_b2997c.Release();
    if(!Request_Chat_Channels()) return 0;
    if(unknown_lobbies_b26c88.Count==0) {
        Match_Message(wol_no_agent_7ffdd8,0x64fb);
        return 0;
    }
    WOL::Channel channel;
    memset(&channel,0,sizeof(channel));
    char const *prefix=unknown_session_a3d298.Type==4&&unknown_a3d2a4==2?wol_lobby_prefix_35_7fd368:wol_lobby_prefix_38_7fd360;
    int i;
    for(i=0;i<unknown_lobbies_b26c88.Count;++i) {
        unknown_lobbies_b26c88.Get(channel,i);
        if(_strnicmp(channel.channel_name,prefix,strlen(prefix))==0) break;
    }
    if(i>=unknown_lobbies_b26c88.Count) {
        Match_Message(wol_no_server_7ffdb8,0x650e);
        return 0;
    }
    HWND dialog=(HWND)unknown_create_dialog_735700(ProgramInstance,0x117,game_window_00b263d8,(UnknownDialogProc)Wonline_Waiting_Dialog_0075c350,0);
    unknown_show_dialog_5ff7e0(dialog,0x117);
    SendMessageA(dialog,0x4a9,0,1);
    ShowWindow(dialog,1);
    Reset_Wait_Events();
    int result=0;
    Reset_Buddy_State_00757720();
    for(;;) {
        if(unknown_b26898==2) {
            int error;
            {
                Wstring password(wol_lobby_password_7fe738);
                error=Request_Channel_Join_00744700(&channel,password,StringTable::Load_String(wol_joining_label_7fe688,0,wol_source_file_007fd62c,0x6526));
            }
            if(error) {
                Set_Matchmaking_State(0,-1,1);
                Match_Message(wol_join_failure_7ffda0,0x6529);
                break;
            }
            Set_Matchmaking_State(3,20,0);
        }
        if(unknown_b26898==3&&unknown_users_b2a728.Count>1&&strcmp(channel.channel_name,unknown_game_channel_b299c0.channel_name)==0) Unknown_0075a920();
        DWORD event;
        while((event=WaitForMultipleObjects(18,wol_wait_events_b264f8,0,20))==0x102) Unknown_0073e100();
        ResetEvent(wol_wait_events_b264f8[event]);
        if(event==0||event==6) break;
        if(event==0x11) {
            Collect_Remote_Player_Names_0075a6c0();
            Service_Pending_Player_Pings_0075b2d0();
        }
        else if(event==5) {
            if(unknown_b26898==0) {
                Leave_Game_Channel(0x40,false);
                switch(unknown_b2ac2c) {
                case 1:
                    Match_Message(wol_no_agent_7ffdd8,0x656c);
                    break;
                case 2:
                    Match_Message(wol_match_timeout_7fd768,0x6570);
                    break;
                default:
                    Match_Message(wol_match_fail_7ffd8c,0x6574);
                    break;
                }
                break;
            }
        }
        else if(event==0x10) {
            if(Start_Match_0075be10()&&unknown_a3d2a4==2) {
                HWND download=unknown_create_dialog_5ff610(0x11f,(DialogProc)Wonline_Map_Download_Dialog_0075c830,0);
                Set_Matchmaking_State(10,180,0);
                SendMessageA(download,0x4a9,0,1);
                ShowWindow(download,1);
            }
        }
        else if(event==7) {
            Unknown_005C2C80(unknown_color_7a7360,StringTable::Load_String(wol_match_starting_7ffd74,0,wol_source_file_007fd62c,0x658d));
            unknown_b2ac04=false;
            result=1;
            break;
        }
    }
    End_Dialog_007357e0(dialog,0);
    HWND download=(HWND)WS_Find_Dialog_00735940(0x11f);
    if(download) End_Dialog_007357e0(download,0);
    if(unknown_b26898!=11) Set_Matchmaking_State(1,-1,0);
    Leave_Game_Channel(0x40,true);
    if(unknown_dialog_preview_a72ce8) {
        delete unknown_dialog_preview_a72ce8;
        unknown_dialog_preview_a72ce8=0;
    }
    return result;
}


extern char wol_user_count_debug_007ffea0[],wol_skip_owner_debug_007ffe74[],wol_skip_local_debug_007ffe50[],wol_queue_name_debug_007ffe2c[];
void __fastcall Collect_Remote_Player_Names_0075a6c0()
{
    WOL::User user;
    memset(&user,0,sizeof(user));
    Debug_Printf(wol_user_count_debug_007ffea0,unknown_users_b2a728.Count);
    for(int i=0;i<unknown_users_b2a728.Count;++i) {
        unknown_users_b2a728.Get_Copy(user,i);
        if(user.flags&1) Debug_Printf(wol_skip_owner_debug_007ffe74,user.name);
        else if(!_stricmp(user.name,unknown_wstring_b26cb8.Get())) Debug_Printf(wol_skip_local_debug_007ffe50,user.name);
        else {
            Wstring name(user.name);
            Debug_Printf(wol_queue_name_debug_007ffe2c,name.Get());
            unknown_strings_b29980.Add_Tail(name);
        }
    }
}


extern Wstring unknown_b2997c;
extern WolStringArray unknown_strings_b29828;
extern int unknown_b2ac2c,unknown_b2ab88;
extern long unknown_b2a8a4;
extern char wol_source_file_007fd62c[];
bool __fastcall Unknown_00514EE0();
int __fastcall Get_Performance_Profile();
void __fastcall Collect_Remote_Player_Names_0075a6c0();
void __fastcall Service_Pending_Player_Pings_0075b2d0();
// Provisional genuine getter ABI: prerequisite rename of the accepted Leaf_769670 placeholder is required before delivery.
rc_ptr<WorldDominationTour::Campaign> const &__fastcall Current_WDT_Campaign_00769670();
// Compiler-generated rc_ptr<Campaign> scalar destructor: original kept copy at 00726B40, independently bounded36+12NOP.
extern char request_match_match_prefix_007fff4c[];
extern char request_match_match_wdt_prefix_007fff40[];
extern char request_match_side_zero_007fff38[];
extern char request_match_side_one_007fff30[];
extern char request_match_territory_option_007fff28[];
extern char request_match_request_match_label_007fff14[];
extern char request_match_country_option_007fff0c[];
extern char request_match_color_option_007fff04[];
extern char request_match_locale_option_007ffefc[];
extern char request_match_rating_option_007ffef4[];
extern char request_match_resolution_option_007ffeec[];
extern char request_match_address_option_007ffee4[];
extern char request_match_nat_option_007ffedc[];
extern char request_match_port_option_007ffed4[];
extern char request_match_user_count_007ffea0[];

bool __fastcall Unknown_0075a920()
{
    unknown_b2997c.Release();
    void *dialog=WS_Find_Dialog_00735940(0x11f);
    if(dialog) Unknown_007357E0(dialog,0);
    unknown_b2ac40=0;
    Unknown_00514EE0();
    WOL::User user,owner;
    int owner_index;
    if(unknown_users_b2a728.Count<2) {
        unknown_b2ac2c=1;
        unknown_b26898=0;
        unknown_b2ab88=-1;
        unknown_b2a8a4=time(0);
        return false;
    }
    unknown_strings_b29980.Clear();
    unknown_strings_b29828.Clear();
    unknown_b26568=0;
    Collect_Remote_Player_Names_0075a6c0();
    memset(&user,0,sizeof(user));
    owner_index=-1;
    Debug_Printf(request_match_user_count_007ffea0,unknown_users_b2a728.Count);
    for(int i=0;i<unknown_users_b2a728.Count;++i) {
        unknown_users_b2a728.Get_Copy(user,i);
        if(user.flags&1) owner_index=i;
    }
    if(owner_index==-1) {
        unknown_b2ac2c=1;
        unknown_b26898=0;
        unknown_b2ab88=-1;
        unknown_b2a8a4=time(0);
        return false;
    }
    memset(&owner,0,sizeof(owner));
    unknown_users_b2a728.Get_Copy(owner,owner_index);
    owner.next=0;
    WolRegistryOptions registry;
    registry.Read_004ff200();
    rc_ptr<PlayerProfile> profile;
    if(registry.unknown_004ff2c0()) profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
    else profile.Set(unknown_profile_b28cc4);
    int country=profile->Country;
    Wstring match(request_match_match_prefix_007fff4c);
    Wstring side;
    if(unknown_a3d2a4==2) {
        match=request_match_match_wdt_prefix_007fff40;
        rc_ptr<WorldDominationTour::Campaign> campaign(Current_WDT_Campaign_00769670());
        if(!campaign.Get()) side=request_match_side_zero_007fff38;
        else {
            switch(campaign->PlayerFaction) {
            case 3:
                side=request_match_side_one_007fff30;
                if(country>=0 && country<=4) country=-1;
                break;
            case 2:
                side=request_match_side_zero_007fff38;
                if(country>4) country=-1;
                break;
            default:
                unknown_b2ac2c=0;
                unknown_b26898=0;
                unknown_b2ab88=-1;
                unknown_b2a8a4=time(0);
                return false;
            }
        }
        side+=request_match_territory_option_007fff28;
        side+=Wstring_From_Integer(unknown_session_a3d2a0);
    }
    Unknown_005C2C80(unknown_color_7a7368,StringTable::Load_String(request_match_request_match_label_007fff14,0,wol_source_file_007fd62c,0x6643));
    Wstring options(match);
    options+=request_match_country_option_007fff0c;
    options+=Wstring_From_Integer(country);
    options+=request_match_color_option_007fff04;
    options+=Wstring_From_Integer(profile->Color);
    Wstring nickname(unknown_wstring_b26cb8);
    nickname.To_Lower();
    if(unknown_channel_names_b293a0.Contains(nickname)) {
        int locale=0;
        int *value;
        if(unknown_channel_names_b293a0.Get_Pointer(nickname,&value) && value) locale=*value;
        options+=request_match_locale_option_007ffefc;
        options+=Wstring_From_Integer(locale);
    }
    options+=request_match_rating_option_007ffef4;
    options+=Wstring_From_Integer(Get_Performance_Profile());
    options+=request_match_resolution_option_007ffeec;
    options+=Wstring_From_Integer(unknown_8523e0);
    NetNumType net;
    NetNodeType node;
    unsigned short socket;
    unknown_address_a3fbc8.Get_Address(net,node,&socket);
    int address=*(int *)node;
    options+=request_match_address_option_007ffee4;
    options+=Wstring_From_Integer(address);
    options+=request_match_nat_option_007ffedc;
    options+=Wstring_From_Integer(unknown_firewall_a71b18.unknown_00&0xffff);
    options+=request_match_port_option_007ffed4;
    options+=Wstring_From_Integer(unknown_a40230);
    options+=side;
    unknown_chat_a72cfc->unknown_slot12(&owner,options.Get());
    unknown_b2ac2c=0;
    unknown_b26898=4;
    unknown_b2ab88=180;
    unknown_b2a8a4=time(0);
    Service_Pending_Player_Pings_0075b2d0();
    return true;
}


extern "C" char *__stdcall inet_ntoa(unsigned long address);
extern char ping_request_debug_007fdde0[];
void Request_All_User_Pings_0075af80()
{
    if(unknown_b2ac40==0) {
        WOL::User user;
        memset(&user,0,sizeof(user));
        int handle=0;
        int count=unknown_users_b2a728.Count;
        for(int i=0;i<count;++i) {
            unknown_users_b2a728.Get_Copy(user,i);
            unsigned long address=user.unknown_18;
            Debug_Printf(ping_request_debug_007fdde0,user.name,inet_ntoa(address));
            unknown_ladder_client_a72d00->Request_Ping(inet_ntoa(address),1000,&handle);
        }
    }
}


void __fastcall Unknown_004d1060(HWND dialog,int slot,int value);
void __fastcall Update_User_Slot_0075b040(WOL::User const *user)
{
    if(user) {
        for(int i=0;i<8;++i) {
            if(unknown_player_slots_a3fa98[i] && !strcmp(Wide_To_Narrow_006fa120((wchar_t const *)unknown_player_slots_a3fa98[i]),user->name)) {
                HWND dialog=Find_Lobby_Window_005c2c40();
                if(dialog) Unknown_004d1060(dialog,i,-1);
                return;
            }
        }
    }
}


extern int wol_pending_pings_b292ac[8];
void __fastcall Store_User_Ping_0075b110(char const *name,int ping);
void Store_Slot_Pings_0075b0c0()
{
    for(int i=0;i<8;++i) {
        if(!unknown_player_slots_a3fa98[i] || !Unknown_005c9320(unknown_player_slots_a3fa98[i])) wol_pending_pings_b292ac[i]=-1;
        // Name is the first member; the original also passes null through the conversion.
        char const *name=Wide_To_Narrow_006fa120((wchar_t const *)unknown_player_slots_a3fa98[i]);
        Store_User_Ping_0075b110(name,wol_pending_pings_b292ac[i]);
    }
}


extern int wol_player_pings_b292ac[8];
extern char wol_store_ping_debug_007fff54[];
void __fastcall Store_User_Ping_0075b110(char const *name,int ping)
{
    Debug_Printf(wol_store_ping_debug_007fff54,name,ping);
    for(int i=0;i<8;++i) {
        if(unknown_player_slots_a3fa98[i] && !strcmp(Wide_To_Narrow_006fa120(unknown_player_slots_a3fa98[i]->Name),name)) {
            wol_player_pings_b292ac[i]=ping;
            HWND dialog=Find_Lobby_Window_005c2c40();
            if(dialog) {
                if(ping<0) Set_Player_Ping_4d1060(dialog,i,-1);
                else if(ping<300) Set_Player_Ping_4d1060(dialog,i,0);
                else if(ping<500) Set_Player_Ping_4d1060(dialog,i,1);
                else Set_Player_Ping_4d1060(dialog,i,2);
            }
        }
    }
}


extern char wol_user_pings_debug_007fff88[];
extern char wol_user_ping_format_007fffa8[];
void __fastcall Store_User_Ping_0075b110(char const *name,int ping);
void __fastcall Unknown_0075b1b0()
{
    int count=unknown_users_b2a728.Count;
    WOL::User user;
    memset(&user,0,sizeof(user));
    char message[256];
    char entry[128];
    int ping;
    message[0]='P';
    message[1]=(char)count+'0';
    message[2]=0;
    if(!unknown_b2ac40) {
        for(int i=0;i<count;++i) {
            unknown_users_b2a728.Get_Copy(user,i);
            unknown_ladder_client_a72d00->Get_Average_Ping(user.unknown_18,&ping);
            sprintf(entry,wol_user_ping_format_007fffa8,user.name,ping);
            strcat(message,entry);
            Store_User_Ping_0075b110(user.name,ping);
        }
        Debug_Printf(wol_user_pings_debug_007fff88,message);
        Send_Game_Options_005c3c60(message);
    }
}


extern "C" char *__stdcall inet_ntoa(unsigned long address);
extern char pending_ping_counts_00800008[];
extern char pending_ping_request_007ffffc[];
extern char pending_ping_started_007fffdc[];
extern char pending_ping_no_ip_007fffc8[];
extern char pending_ping_missing_007fffb0[];
void __fastcall Service_Pending_Player_Pings_0075b2d0()
{
    bool failed=false;
    WOL::User user;
    memset(&user,0,sizeof(user));
    if(unknown_b2ac40) return;
    Debug_Printf(pending_ping_counts_00800008,unknown_strings_b29980.Count,unknown_strings_b29828.Count);
    while(unknown_strings_b29980.Count && unknown_strings_b29828.Count<10 && !failed) {
        Wstring name;
        unknown_strings_b29980.Get(name,0);
        int index=unknown_users_b2a728.Find_Name(name.Get());
        if(index!=-1) {
            unknown_users_b2a728.Get_Copy(user,index);
            if(user.unknown_18) {
                Debug_Printf(pending_ping_request_007ffffc,name.Get());
                int handle;
                unknown_ladder_client_a72d00->Request_Ping(inet_ntoa(user.unknown_18),1000,&handle);
                if(handle>=0) {
                    unknown_strings_b29828.Insert(name,unknown_strings_b29828.Count);
                    if(unknown_strings_b29980.Count>0) {
                        unknown_strings_b29980.Get(name,0);
                        unknown_strings_b29980.Remove_007661f0(0);
                    }
                    Debug_Printf(pending_ping_started_007fffdc,name.Get());
                } else failed=true;
            } else {
                if(unknown_strings_b29980.Count>0) {
                    unknown_strings_b29980.Get(name,0);
                    unknown_strings_b29980.Remove_007661f0(0);
                }
                Debug_Printf(pending_ping_no_ip_007fffc8,name.Get());
            }
        } else {
            if(unknown_strings_b29980.Count>0) {
                unknown_strings_b29980.Get(name,0);
                unknown_strings_b29980.Remove_Inline(0);
            }
            Debug_Printf(pending_ping_missing_007fffb0,name.Get());
        }
    }
}


extern WolStringArray unknown_strings_b29828;
extern char pending_ping_missing_007fffb0[];
extern char pending_ping_no_ip_007fffc8[];
extern char ping_report_title_8000ec[];
extern char ping_report_pending_8000c0[];
extern char ping_report_removed_80009c[];
extern char ping_report_average_800088[];
extern char ping_report_error_80006c[];
extern char ping_report_sent_800054[];
extern char ping_report_separator_7ce9c0[];
void __fastcall Send_Pending_Ping_Report_0075b590()
{
    int sent=0;
    WOL::User user;
    memset(&user,0,sizeof(user));
    if(unknown_b2ac40) return;
    Wstring packet(ping_report_title_8000ec);
    Debug_Printf(ping_report_pending_8000c0,unknown_strings_b29828.Count);
    while(unknown_strings_b29828.Count) {
        Wstring name;
        unknown_strings_b29828.Remove_Tail(name);
        Debug_Printf(ping_report_removed_80009c,name.Get());
        int index=unknown_users_b2a728.Find_Name(name.Get());
        if(index==-1) Debug_Printf(pending_ping_missing_007fffb0,name.Get());
        else {
            unknown_users_b2a728.Get_Copy(user,index);
            if(!user.unknown_18) Debug_Printf(pending_ping_no_ip_007fffc8,name.Get());
            else {
                int average=-1;
                unknown_ladder_client_a72d00->Get_Average_Ping(user.unknown_18,&average);
                if(average!=-1) {
                    Debug_Printf(ping_report_average_800088,name.Get(),average);
                    packet += name+Wstring(ping_report_separator_7ce9c0)+Wstring_From_Integer(average)+Wstring(";");
                    ++sent;
                } else Debug_Printf(ping_report_error_80006c,name.Get());
            }
        }
    }
    unknown_b26568=0;
    if(0<sent) {
        WOL::User *host=Unknown_0074DA60();
        if(host) {
            unknown_chat_a72cfc->unknown_slot12(host,packet.Get());
            Debug_Printf(ping_report_sent_800054,packet.Get());
        }
    }
}


extern "C" int __cdecl _strnicmp(char const *,char const *,unsigned int);
extern HANDLE wol_wait_events_b264f8[18];
extern int unknown_b2ac2c,unknown_b2ab88;
extern long unknown_b2a8a4;
extern char match_start_buffer_00b2658c[];
extern char match_fail_prefix_008001f4[];
extern char match_fail_label_008001e4[];
extern char match_ping_prefix_008001d8[];
extern char match_ping_label_008001c4[];
extern char match_rank_prefix_008001b8[];
extern char match_rank_label_008001a4[];
extern char match_side_prefix_00800198[];
extern char match_side_label_00800180[];
extern char match_exit_prefix_00800178[];
extern char match_exit_label_007ffd8c[];
extern char match_working_prefix_00800170[];
extern char match_working_label_00800158[];
extern char match_info_prefix_00800150[];
extern char match_pool_prefix_00800148[];
extern char match_pool_label_00800130[];
extern char match_start_prefix_00800128[];
extern char match_ack1_00800124[];
extern char match_ack2_0080011c[];
extern char match_need_prefix_00800114[];
extern char match_unknown_label_008000f4[];
extern char match_debug_format_007d5a34[];
extern char wol_source_file_007fd62c[];
void __fastcall Handle_Matchmaking_Command_0075b910(char const *text,char from_server)
{
    if(from_server) {
        if(!_strnicmp(text,match_fail_prefix_008001f4,4)) {
            Unknown_005C2C80(0x40ffff,StringTable::Load_String(match_fail_label_008001e4,0,wol_source_file_007fd62c,0x6753));
            SetEvent(wol_wait_events_b264f8[17]);
            return;
        }
        if(!_strnicmp(text,match_ping_prefix_008001d8,8)) {
            Unknown_005C2C80(0x40ffff,StringTable::Load_String(match_ping_label_008001c4,0,wol_source_file_007fd62c,0x6759));
            SetEvent(wol_wait_events_b264f8[17]);
            return;
        }
        if(!_strnicmp(text,match_rank_prefix_008001b8,8)) {
            Unknown_005C2C80(0x40ffff,StringTable::Load_String(match_rank_label_008001a4,0,wol_source_file_007fd62c,0x675f));
            SetEvent(wol_wait_events_b264f8[17]);
            return;
        }
        if(!_strnicmp(text,match_side_prefix_00800198,11)) {
            Unknown_005C2C80(0x40ffff,StringTable::Load_String(match_side_label_00800180,0,wol_source_file_007fd62c,0x6765));
            SetEvent(wol_wait_events_b264f8[17]);
            return;
        }
    }
    if(!_strnicmp(text,match_exit_prefix_00800178,4)) {
        Unknown_005C2C80(0x40ffff,StringTable::Load_String(match_exit_label_007ffd8c,0,wol_source_file_007fd62c,0x676b));
        unknown_chat_a72cfc->unknown_slot09();
        Message_Box_005c3230(StringTable::Load_String(match_exit_label_007ffd8c,0,wol_source_file_007fd62c,0x676e),0,(bool (*)())Unknown_0073c120,false);
        SetEvent(wol_wait_events_b264f8[0]);
        return;
    }
    if(!_strnicmp(text,match_working_prefix_00800170,7)) {
        if(unknown_b26898==4) {
            unknown_b2ac2c=0;
            unknown_b26898=5;
            unknown_b2ab88=180;
            unknown_b2a8a4=time(0);
        }
        Unknown_005C2C80(unknown_color_7a7360,StringTable::Load_String(match_working_label_00800158,0,wol_source_file_007fd62c,0x677a));
        return;
    }
    if(!_strnicmp(text,match_info_prefix_00800150,5)) {
        Unknown_005C2C80(0xff8080,Narrow_To_Wide_006fa180(text+5));
        return;
    }
    if(!_strnicmp(text,match_pool_prefix_00800148,5)) {
        Unknown_005C2C80(0xff8080,StringTable::Load_String(match_pool_label_00800130,0,wol_source_file_007fd62c,0x6784),text+5);
        return;
    }
    if(from_server && !_strnicmp(text,match_start_prefix_00800128,6)) {
        strncpy(match_start_buffer_00b2658c,text+6,255);
        Debug_Printf(match_debug_format_007d5a34,match_start_buffer_00b2658c);
        SetEvent(wol_wait_events_b264f8[16]);
        return;
    }
    if(_stricmp(text,match_ack1_00800124) && _stricmp(text,match_ack2_0080011c)) {
        if(!_strnicmp(text,match_need_prefix_00800114,5)) {
            Debug_Printf(match_debug_format_007d5a34,text);
            return;
        }
        if(from_server) Unknown_005C2C80(0x40ffff,StringTable::Load_String(match_unknown_label_008000f4,0,wol_source_file_007fd62c,0x67a5));
    }
}


extern Wstring unknown_b2997c;
extern char wol_source_file_007fd62c[];
extern char match_found_label_00800234[];
extern char match_ping_label_0080021c[];
extern char match_no_ping_label_008001fc[];
void __fastcall Report_Match_Player_Ping_0075bc40()
{
    Unknown_005C2C80(0xc0ffff,StringTable::Load_String(match_found_label_00800234,0,wol_source_file_007fd62c,0x67ba),unknown_b2997c.Buffer);
    Wstring lower=unknown_b2997c.As_Lower();
    // The original performs this lookup before resolving the player; its result is unused.
    WOL::Ladder *ladder;
    unknown_ladder_b2ab98.Get_Pointer(lower,&ladder);
    int index=unknown_users_b2a728.Find_Name(unknown_b2997c.Get());
    if(index==-1) {
        Unknown_005C2C80(0xc0ffff,StringTable::Load_String(match_no_ping_label_008001fc,0,wol_source_file_007fd62c,0x67c7));
        return;
    }
    {
        WOL::User user;
        memset(&user,0,sizeof(user));
        unknown_users_b2a728.Get_Copy(user,index);
        if(!user.unknown_18) {
            Unknown_005C2C80(0xc0ffff,StringTable::Load_String(match_no_ping_label_008001fc,0,wol_source_file_007fd62c,0x67ce));
            return;
        }
        int ping=-1;
        unknown_ladder_client_a72d00->Get_Average_Ping(user.unknown_18,&ping);
        if(ping!=-1) {
            Unknown_005C2C80(0xc0ffff,StringTable::Load_String(match_ping_label_0080021c,0,wol_source_file_007fd62c,0x67d4),ping);
        } else {
            Unknown_005C2C80(0xc0ffff,StringTable::Load_String(match_no_ping_label_008001fc,0,wol_source_file_007fd62c,0x67d6));
        }
    }
}


void __fastcall unknown_dialog_5e5f20(HWND);
extern HANDLE wol_wait_events_b264f8[18];
extern char wol_start_command1_00800324[],wol_start_command2_00800378[];
int __stdcall Wonline_Waiting_Dialog_0075c350(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    switch(message) {
    case 2:
        KillTimer(window,0);
        return 0;
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
        return 1;
    case 0x14:
        return 1;
    case 0x2b:
        Unknown_005FE380(lparam);
        return 1;
    case 0x111:
        switch(wparam&0xffff) {
        case 0x5c0:
            if(unknown_b26898==11) Unknown_007357E0(window,0);
            else SetEvent(wol_wait_events_b264f8[0]);
            break;
        case 0x708:
            unknown_chat_a72cfc->unknown_slot11(wol_start_command1_00800324);
            break;
        case 0x709:
            unknown_chat_a72cfc->unknown_slot11(wol_start_command2_00800378);
            break;
        }
        return 0;
    case 0x113:
        if(wparam==0) Add_Chat_Dot_005c3040();
        return 0;
    case 0x497:
        SetTimer(window,0,2000,0);
        return 0;
    }
    return 0;
}


extern "C" int __cdecl sscanf(char const *,char const *,...);
extern char endpoint_number_format_007db4b0[];
extern IPXAddressClass unknown_addresses_a3fbd4[8];
extern int unknown_peer_values_a3fc34[8];
extern int unknown_peer_values_a40238[8];
void __fastcall Set_Player_Endpoint_0075c470(char const *name,char const *address_text,char const *value_text,char const *other_text)
{
    if(name && address_text && value_text && other_text) {
        unsigned int address=0;
        unsigned int value=0;
        unsigned int other=0;
        sscanf(address_text,endpoint_number_format_007db4b0,&address);
        sscanf(value_text,endpoint_number_format_007db4b0,&value);
        sscanf(other_text,endpoint_number_format_007db4b0,&other);
        int i=0;
        for(NodeNameType **slot=unknown_player_slots_a3fa98;(long)slot<(long)(unknown_player_slots_a3fa98+8);++slot,++i) {
            if(_stricmp(name,Wide_To_Narrow_006fa120((*slot)->Name))==0) {
                NetNodeType node;
                NetNumType net;
                memset(node,0,sizeof(node));
                memset(net,0,sizeof(net));
                memcpy(node,&address,4);
                unknown_addresses_a3fbd4[i].Set_Address(net,node,0);
                unknown_peer_values_a3fc34[i]=value;
                unknown_peer_values_a40238[i]=other;
                break;
            }
        }
    }
}


// Apply_Endpoint_Map_0075C570: two peer records followed by random-map setup.
extern "C" long __cdecl atol(char const *text);
void __fastcall Set_Player_Endpoint_0075c470(char const *name, char const *address, char const *value, char const *other);
int __fastcall Set_Channel_Player_Options_0073d550(Wstring const &name, int country, int color);
void __fastcall Do_Random_Map_00577880(HWND dialog, bool (*callback)());
extern char const random_map_file_007e16e8[]; // RandMap.Sed, existing original pool.
extern char const endpoint_delimiters_008003cc[]; // :,
extern char const field_delimiter_007ce9c0[]; // ,
// Same real container operation, with a named capacity value for this element type.
// No additional call or forced-inlining mechanism is introduced.
template<> inline bool DynamicVectorClass<MultiMission *>::Add(MultiMission *const &object)
{
    if (ActiveCount >= Length()) {
        if ((IsAllocated || !VectorMax) && GrowthStep > 0) {
            int new_length = Length();
            new_length += GrowthStep;
            if (!Resize(new_length)) return false;
        } else {
            return false;
        }
    }
    (*this)[ActiveCount++] = object;
    return true;
}
void __fastcall Apply_Endpoint_Map_0075c570(char const *text)
{
    char data[256];
    strcpy(data, text);
    char *name = strtok(data, endpoint_delimiters_008003cc);
    int country = atol(strtok(0, field_delimiter_007ce9c0));
    int color = atol(strtok(0, field_delimiter_007ce9c0));
    char *address = strtok(0, field_delimiter_007ce9c0);
    char *value = strtok(0, field_delimiter_007ce9c0);
    char *other = strtok(0, field_delimiter_007ce9c0);
    Set_Player_Endpoint_0075c470(name, address, value, other);
    Set_Channel_Player_Options_0073d550(name, country, color);
    name = strtok(0, field_delimiter_007ce9c0);
    country = atol(strtok(0, field_delimiter_007ce9c0));
    color = atol(strtok(0, field_delimiter_007ce9c0));
    address = strtok(0, field_delimiter_007ce9c0);
    value = strtok(0, field_delimiter_007ce9c0);
    other = strtok(0, field_delimiter_007ce9c0);
    Set_Player_Endpoint_0075c470(name, address, value, other);
    Set_Channel_Player_Options_0073d550(name, country, color);
    RandomMapGen.SeedData.NumPlayers = 4;
    strcpy(unknown_session_a3d298.ScenarioFileName_658, random_map_file_007e16e8);
    unknown_session_a3d298.ScenarioIndex = -1;
    HWND dialog = (HWND)WS_Find_Dialog_00735940(0x11f);
    Do_Random_Map_00577880(dialog, Unknown_0073c120);
    RandomMapGen.SeedData.Save_Or_Prompt_00578ed0(random_map_file_007e16e8);
    bool found = false;
    int index = 0;
    for (; index < unknown_session_a3d298.Scenarios.Count(); index++) {
        if (unknown_session_a3d298.Scenarios[index]->Unknown_006720C0()) {
            found = true;
            break;
        }
    }
    char *digest = Random_Map_CRC_005c8420();
    if (!found) {
        unknown_session_a3d298.Scenarios.Add(new MultiMission(random_map_file_007e16e8,
            RandomMapGen.SeedData.DescriptionBuffer, digest, true, 0, 2, 4));
    } else {
        unknown_session_a3d298.Scenarios[index]->Set_Digest(digest);
        unknown_session_a3d298.Scenarios[index]->Set_Description(RandomMapGen.SeedData.DescriptionBuffer);
    }
    delete digest;
    if (Select_Scenario_005c7c30(index) == true) unknown_session_a3d298.ScenarioIndex = index;
}


// Wonline_Map_Download_Dialog0075C830: preview and progress while waiting.
extern HANDLE wol_wait_events_b264f8[18];
void __fastcall unknown_dialog_5e5f20(HWND window);
int __stdcall Wonline_Map_Download_Dialog_0075c830(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result != 0) return result;
    if (message <= 0x2b) {
        if (message != 0x2b) {
        switch (message) {
    case 2:
        if (unknown_dialog_preview_a72ce8) {
            delete unknown_dialog_preview_a72ce8;
            unknown_dialog_preview_a72ce8 = 0;
            return 0;
        }
        break;
    case 0xf: {
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        MapPreviewClass *preview = unknown_dialog_preview_a72ce8;
        if (!preview && unknown_session_a3d2b4 == -1) preview = unknown_preview_a6fee4;
        if (preview) preview->Blit_Preview(window);
        if (ProgressScreen.Has_Window()) ProgressScreen.Draw_It(Point2D(-1, -1));
        ValidateRect(window, 0);
        return 1;
    }
    case 0x14:
        return 1;
        }
        } else {
            Unknown_005FE380(lparam);
            return 1;
        }
    } else if (message == 0x111) {
        if ((unsigned short)wparam == 0x5c0) {
            SetEvent(wol_wait_events_b264f8[0]);
            Unknown_007357E0(window, 0x5c0);
        }
    }
    return 0;
}


// void Unknown_0075c960() (0x0075C960)
// Sends "G" through IChat slot 15 when the chat object exists.

void Unknown_0075c960()
{
    if (unknown_chat_a72cfc) unknown_chat_a72cfc->unknown_slot15("G");
}


HWND __fastcall Unknown_00735B10();
void __cdecl Unknown_005C2C80(int color,unsigned short const *format,...);
void __fastcall Unknown_0075c980()
{
    HWND dialog=Unknown_00735B10();
    HWND edit=GetDlgItem(dialog,0x59f);
    if(Unknown_0073d3f0((char const *)0)!=1) {
        SendMessageA(edit,0x4dc,0,1);
        Unknown_005C2C80(-1,StringTable::Load_String("GUI:HostGameStartJoiner",0,"D:\\Ra2\\wonline.cpp",0x695e));
    }
}


void __fastcall Kick_Channel_User_0075ca60(Wstring const &name)
{
    WOL::User user;
    user.flags=0;
    for(int i=0;i<unknown_users_b2a728.Count;++i) {
        unknown_users_b2a728.Get_Copy(user,i);
        if(_stricmp(name.Peek_Buffer(),user.name)==0) {
            unknown_chat_a72cfc->unknown_slot21(&user);
            break;
        }
    }
}


void __fastcall Kick_Channel_User_Wide_0075cae0(wchar_t const *name)
{
    if(name) {
        Wstring narrow=Narrow_Copy_00771280(name);
        WOL::User user;
        user.flags=0;
        for(int i=0;i<unknown_users_b2a728.Count;++i) {
            unknown_users_b2a728.Get_Copy(user,i);
            if(_stricmp(narrow.Peek_Buffer(),user.name)==0) {
                unknown_chat_a72cfc->unknown_slot21(&user);
                break;
            }
        }
    }
}


extern Wstring unknown_b2997c;
bool Unknown_00738eb0();
int __fastcall Run_Control_Dialog_006011a0(int,DLGPROC,OwnerDraw::DialogControl *);
long __stdcall Buddy_Dialog_0075cc10(HWND,unsigned,unsigned,long);
void Open_Buddy_Dialog_0075cba0()
{
    unknown_b2abd9=false;
    if(!unknown_b2abda && Unknown_00738eb0() && Unknown_00756790(unknown_b2997c)) {
        SimpleWonlineDialogControl control;
        Run_Control_Dialog_006011a0(0x122,(DLGPROC)Buddy_Dialog_0075cc10,&control);
    }
}


int __fastcall unknown_dialog_proc_5ffb00(HWND,UINT,WPARAM,LPARAM);
extern Wstring unknown_b2997c;
extern char buddy_query_label_008003d4[],wol_source_file_007fd62c[];
void __fastcall Set_Add_Match_Buddies_00738fa0(bool);
void __fastcall unknown_dialog_5e5f20(HWND);
long __stdcall Buddy_Dialog_0075cc10(HWND window,UINT message,WPARAM wparam,LPARAM lparam)
{
    if(message==0x110 && lparam) SetWindowLongA(window,8,*(long *)(lparam+4));
    long result=unknown_dialog_proc_5ffb00(window,message,wparam,lparam);
    if(result) return result;
    OwnerDraw::DialogControl *control=(OwnerDraw::DialogControl *)GetWindowLongA(window,8);
    switch(message) {
    case 0xf:
        Unknown_005FEE70(window);
        unknown_dialog_5e5f20(window);
        ValidateRect(window,0);
        return 1;
    case 0x14: return 1;
    case 0x2b: Unknown_005FE380(lparam); return 1;
    case 0x111:
        switch(wparam&0xffff) {
        case 1: {
            bool checked=IsDlgButtonChecked(window,0x6f9)==1;
            Set_Add_Match_Buddies_00738fa0(!checked);
            if(Unknown_00756790(unknown_b2997c)) {
                WOL::User buddy;
                memset(&buddy,0,sizeof(buddy));
                strncpy(buddy.name,unknown_b2997c.Peek_Buffer(),9);
                unknown_chat_a72cfc->unknown_slot56(&buddy);
                if(Unknown_00756790(unknown_b2997c)) {
                    Wstring copy(unknown_b2997c);
                    unknown_strings_b26920.Add(copy);
                    unknown_strings_b29838.Add(copy);
                    unknown_strings_b2a7d8.Add(copy);
                    if(unknown_wstring_b26944.Length()>0) {
                        if(time(0)-unknown_b2a7f8<5) goto updated;
                        unknown_wstring_b26944.Release();
                    }
                    if(unknown_strings_b29838.Count) {
                        if(unknown_strings_b29838.Get_007662c0(unknown_wstring_b26944,0)) unknown_strings_b29838.Remove_007661f0(0);
                        unknown_b2a7f8=time(0);
                        WOL::User user;
                        memset(&user,0,sizeof(user));
                        strcpy(user.name,unknown_wstring_b26944.Get());
                        unknown_chat_a72cfc->unknown_slot24(&user);
                    }
updated:;
                }
                Unknown_00757380();
            }
            control->Set_Window_006010f0((unsigned)window,(HWND)1);
            break;
        }
        case 2: {
            bool checked=IsDlgButtonChecked(window,0x6f9)==1;
            Set_Add_Match_Buddies_00738fa0(!checked);
            control->Set_Window_006010f0((unsigned)window,(HWND)2);
            return 0;
        }
        }
        break;
    case 0x497:
        wchar_t query[128];
        swprintf(query,StringTable::Load_String(buddy_query_label_008003d4,0,wol_source_file_007fd62c,0x69b7),unknown_b2997c.Get());
        HWND text=GetDlgItem(window,0x71b);
        SendMessageA(text,0x4b2,0,(LPARAM)query);
        return 0;
    }
    return 0;
}


// void *SimpleWonlineDialogControl::`scalar deleting destructor'(unsigned int flags) (0x0075D0A0, slot 0)
// Compiler-generated with the control's vtable; it inlines the destructor 0x0073C020. No source of its own.


// HRESULT __stdcall AtlAdvise(IUnknown *container, IUnknown *sink, GUID const &iid, unsigned long *cookie) (0x0075D0D0)
// ATL: connects a sink to the container's connection point for iid.

extern "C" HRESULT __stdcall AtlAdvise(IUnknown *container, IUnknown *sink, GUID const &iid, unsigned long *cookie)
{
    CComPtr<IConnectionPointContainer> point_container;
    CComPtr<IConnectionPoint> point;
    HRESULT result = container->QueryInterface(IID_IConnectionPointContainer, (void **)&point_container);
    if (SUCCEEDED(result)) result = point_container->FindConnectionPoint(iid, &point);
    if (SUCCEEDED(result)) result = point->Advise(sink, cookie);
    return result;
}


// HRESULT __stdcall AtlModuleInit(_ATL_MODULE *pM, _ATL_OBJMAP_ENTRY *p, HINSTANCE h) (0x0075D160)
// ATL: initializes the module record and calls each object map entry's ObjectMain(true).

extern "C" HRESULT __stdcall AtlModuleInit(_ATL_MODULE *pM, _ATL_OBJMAP_ENTRY *p, HINSTANCE h)
{
    if (pM == 0) return E_INVALIDARG;
    pM->m_pObjMap = p;
    pM->m_hInst = pM->m_hInstTypeLib = pM->m_hInstResource = h;
    pM->m_nLockCnt = 0;
    pM->m_hHeap = 0;
    InitializeCriticalSection(&pM->m_csTypeInfoHolder);
    InitializeCriticalSection(&pM->m_csWindowCreate);
    InitializeCriticalSection(&pM->m_csObjMap);
    pM->m_pCreateWndList = 0;
    pM->m_bDestroyHeap = true;
    pM->m_dwHeaps = 0;
    pM->m_nHeap = 0;
    pM->m_phHeaps = 0;
    pM->m_pTermFuncs = 0;
    if (pM->m_pObjMap != 0) {
        _ATL_OBJMAP_ENTRY *pEntry = pM->m_pObjMap;
        while (pEntry->pclsid != 0) {
            pEntry->pfnObjectMain(true);
            pEntry = _NextObjectMapEntry(pM, pEntry);
        }
    }
    return S_OK;
}


// CNetUtilEventSink slot4, 0075D330. Original CFG: 2195 body bytes, 13 frozen NOPs.
extern int ladder_column_rung_b2a800, ladder_column_name_b26c80;
extern int ladder_column_points_b297c0, ladder_column_wins_b2aafc;
extern int ladder_column_losses_b2692c, ladder_column_disconnects_b2999c;
extern int ladder_top_rung_b29ce8;
extern wchar_t const ladder_number_format_007d1598[];
extern char const ladder_empty_label_007fdb78[], ladder_source_file_007fd62c[];
void __fastcall Update_Buddy_Profile_007562b0(HWND dialog);

HRESULT __stdcall CNetUtilEventSink::OnLadderList(HRESULT result,WOL::Ladder *list,int count,long timestamp,int keyrung)
{
    if(result != S_OK) list=0;
    if(unknown_b2abd9) return S_OK;
    HWND window=GetDlgItem((HWND)WS_Find_Dialog_00735940(0xc1),0x57f);
    if(window && keyrung == -1 && result == S_OK) window=0;
    if(window) {
        SendMessageA(window,0x184,0,0);
        if(!list) {
            SendMessageA(window,0x4cc,-1,(LPARAM)StringTable::Load_String(ladder_empty_label_007fdb78,0,ladder_source_file_007fd62c,0xe36));
            InvalidateRect(window,0,0);
            return S_OK;
        }
    } else if(!list) return S_OK;
    WOL::Ladder *entry=list;
    wchar_t formatted[128];
    int top_rung=list->unknown_1c;
    WolLadderListCell cell;
    if(list->game_type == 0x802100) {
        if(unknown_ladder_b296f8.Count>2000) unknown_ladder_b296f8.Clear();
    } else if(list->game_type == 0x2300) {
        if(unknown_ladder_b26970.Count>2000) unknown_ladder_b26970.Clear();
    } else {
        if(unknown_ladder_b2ab98.Count>2000) unknown_ladder_b2ab98.Clear();
    }
    if(list && window) ladder_top_rung_b29ce8=list->unknown_1c;
    Wstring local_name=unknown_wstring_b26cb8.As_Lower();
    int row=0;
    while(entry) {
        if(entry->unknown_10 == -1) {
            entry=entry->next;
            continue;
        }
        if(window) {
            cell.Flags=4;
            swprintf(formatted,ladder_number_format_007d1598,entry->unknown_1c);
            SendMessageA(window,0x4cc,-1,(LPARAM)formatted);
            SendMessageA(window,0x4a8,((unsigned short)ladder_column_rung_b2a800 | ((unsigned int)(unsigned short)row << 16)),(LPARAM)&cell);
            cell.Flags=1;
            wchar_t wide_name[5120];
            Char_To_Wide_006fa0c0(wide_name,entry->name,-1);
            cell.Text.Set(wide_name);
            SendMessageA(window,0x4a8,((unsigned short)ladder_column_name_b26c80 | ((unsigned int)(unsigned short)row << 16)),(LPARAM)&cell);
            swprintf(formatted,ladder_number_format_007d1598,entry->unknown_10);
            cell.Text.Set(formatted);
            SendMessageA(window,0x4a8,((unsigned short)ladder_column_points_b297c0 | ((unsigned int)(unsigned short)row << 16)),(LPARAM)&cell);
            swprintf(formatted,ladder_number_format_007d1598,entry->unknown_08);
            cell.Text.Set(formatted);
            SendMessageA(window,0x4a8,((unsigned short)ladder_column_wins_b2aafc | ((unsigned int)(unsigned short)row << 16)),(LPARAM)&cell);
            swprintf(formatted,ladder_number_format_007d1598,entry->unknown_0c);
            cell.Text.Set(formatted);
            SendMessageA(window,0x4a8,((unsigned short)ladder_column_losses_b2692c | ((unsigned int)(unsigned short)row << 16)),(LPARAM)&cell);
            swprintf(formatted,ladder_number_format_007d1598,entry->unknown_20);
            cell.Text.Set(formatted);
            SendMessageA(window,0x4a8,((unsigned short)ladder_column_disconnects_b2999c | ((unsigned int)(unsigned short)row << 16)),(LPARAM)&cell);
            ++row;
        }
        Wstring name(entry->name);
        name.To_Lower();
        if(list->game_type == 0x802100) {
            WOL::Ladder old_value;
            unknown_ladder_b296f8.Remove_00767160(name,&old_value);
            unknown_ladder_b296f8.Add(name,*entry);
        }
        else if(list->game_type == 0x2300) {
            WOL::Ladder old_value;
            unknown_ladder_b26970.Remove_00767160(name,&old_value);
            unknown_ladder_b26970.Add(name,*entry);
        }
        else {
            WOL::Ladder old_value;
            unknown_ladder_b2ab98.Remove_00767160(name,&old_value);
            unknown_ladder_b2ab98.Add(name,*entry);
        }
        if(local_name == name) {
            rc_ptr<PlayerProfile> profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
            if(profile.Get()) {
                profile->Set_Ladder_007399f0(entry);
                profile->Save_007395d0(name);
                HWND dialog=(HWND)WS_Find_Dialog_00735940(0x10e);
                if(!dialog) dialog=(HWND)WS_Find_Dialog_00735940(0x114);
                if(dialog) profile->Populate_Dialog_00739b70(dialog,unknown_wstring_b26cb8,false);
            }
        }
        entry=entry->next;
    }
    if(window) {
        SendMessageA(window,0x186,keyrung-top_rung,0);
        InvalidateRect(window,0,0);
    } else Unknown_0073FEE0(0,false);
    Unknown_00757380();
    HWND buddy=(HWND)WS_Find_Dialog_00735940(0x116);
    if(buddy) Update_Buddy_Profile_007562b0(buddy);
    return S_OK;
}


HRESULT __stdcall CNetUtilEventSink::OnWDTState(HRESULT result, unsigned char *data, int length)
{
    if (result >= 0 && length > 0) {
        rc_ptr<WDTState> state = WDTState::Create(data);
        if (state.Get() && state->Is_Valid()) {
            if (state->Is_Previous_Cycle()) {
                Debug_Printf("OnWDTState: Previous cycle data received\n");
                WDT_Store_Previous_007695a0(state);
                SetEvent(unknown_b26524);
            } else {
                Debug_Printf("OnWDTState: Current cycle data received\n");
                unknown_b292dc = time(0) + state->unknown_28;
                WDT_Store_Current_00769520(state);
                SetEvent(unknown_b26524);
            }
        } else {
            Debug_Printf("OnWDTState: NACK received from server\n");
            SetEvent(unknown_b26524);
        }
        return S_OK;
    }
    SetEvent(unknown_b2650c);
    return S_OK;
}


// HRESULT __stdcall CNetUtilEventSink::OnGameresSent(HRESULT result) (0x0075D300, slot 5)
// In game mode 4 with online state 2, a pending game-results flag
// (0x00B2A7EC) is set to 1 and 0x00698D00 is told.

HRESULT __stdcall CNetUtilEventSink::OnGameresSent(HRESULT result)
{
    if (unknown_game_mode_a3d298 == 4 && unknown_a3d2a4 == 2 && unknown_b2a7ec) {
        unknown_b2a7ec = 1;
        Unknown_00698D00(1);
    }
    return S_OK;
}


HRESULT __stdcall CNetUtilEventSink::OnNewNick(HRESULT result, char const *message, char const *nick, char const *password)
{
    wchar_t error[5120];
    Debug_Printf("OnNewNick\n");
    unknown_newnick_result_b2ac08 = result;
    if (result == S_OK) {
        rc_ptr<PlayerProfile> profile = PlayerProfile::Load_00738880(nick);
        int nick_slot = 0;
        WolRegistryOptions options;
        options.Read_004ff200();
        if (options.unknown_004ff2c0()) {
            nick_slot = Save_Nickname_00745320(nick, password, true, profile->Unknown_Value_10());
            unknown_session_a3d298.Write_MultiPlayer_Settings();
        }
        bool login_dialog = true;
        bool alternate = false;
        HWND dialog = (HWND)WS_Find_Dialog_00735940(0x10f);
        if (!dialog) {
            dialog = (HWND)WS_Find_Dialog_00735940(0x11d);
            alternate = dialog != 0;
            if (!dialog) {
                login_dialog = false;
                dialog = (HWND)WS_Find_Dialog_00735940(0x114);
            }
        }
        if (dialog) {
            HWND list = GetDlgItem(dialog, 0x628);
            if (alternate) {
                SendMessageA(list, 0x4b4, 0, (LPARAM)nick);
            } else {
                int index = SendMessageA(list, 0x4bc, 0, (LPARAM)nick);
                SendMessageA(list, 0x151, index, nick_slot);
                SendMessageA(list, 0x14e, index, 0);
            }
            if (login_dialog) {
                unknown_wstring_b26cb8 = nick;
                unknown_password_b2a63c = password;
                HWND edit = GetDlgItem(dialog, 0x72c);
                SendMessageA(edit, 0x4b4, 0, (LPARAM)unknown_password_b2a63c.Peek_Buffer());
                unknown_7fd374 = true;
                EnableWindow(GetDlgItem(dialog, 0x539), true);
                if (!alternate) {
                    EnableWindow(GetDlgItem(dialog, 0x72d), false);
                    SendMessageA(GetDlgItem(dialog, 0x72d), 0xf1, 1, 0);
                }
            } else {
                SendMessageA(GetDlgItem(dialog, 0x6ff), 0xf1, 0, 0);
                EnableWindow(GetDlgItem(dialog, 0x6ff), true);
            }
            if (!alternate) {
                EnableWindow(GetDlgItem(dialog, 0x6eb), true);
                Wstring nickname(nick);
                profile->Populate_Dialog_00739b70(dialog, nickname, true);
            }
        }
    }
    if (message) {
        Char_To_Wide_006fa0c0(error, message, -1);
        Popup_005c3230(error, 0, Unknown_0073c120, 0);
    } else {
        Popup_005c3230(L"Unknown Status", 0, Unknown_0073c120, 0);
    }
    void *dialog = WS_Find_Dialog_00735940(0xc4);
    if (dialog && (unknown_b2abe0 &= ~0x100u) == 0) Unknown_007357E0(dialog, 0);
    HWND window = Unknown_00735B10();
    window = GetDlgItem(window, 0x540);
    if (window) SetFocus(window);
    return S_OK;
}


// HRESULT __stdcall CNetUtilEventSink::OnAgeCheck(HRESULT result, int years, int consent) (0x0075DEB0, slot 7)
// Records the age-check answer, clears its pending bit (closing dialog 0xC4
// when nothing else is pending) and focuses control 0x540.

HRESULT __stdcall CNetUtilEventSink::OnAgeCheck(HRESULT result, int years, int consent)
{
    Debug_Printf("OnAgeCheck\n");
    unknown_b2ac0c = result;
    if (result == S_OK) {
        unknown_b2ac6c = years;
        unknown_b2ac70 = consent;
    }
    void *dialog = WS_Find_Dialog_00735940(0xC4);
    if (dialog) {
        unknown_b2abe0 &= ~0x100;
        if (!unknown_b2abe0) {
            Unknown_007357E0(dialog, 0);
        }
    }
    HWND dialog_window = Unknown_00735B10();
    HWND window = GetDlgItem(dialog_window, 0x540);
    if (window) {
        SetFocus(window);
    }
    return S_OK;
}


CChatEventSink::CChatEventSink() {}


// HRESULT __stdcall CChatEventSink::OnUpdateList(HRESULT result, WOL::Update *updates) (0x0075EA50, slot 4)
// Offers the required patches; on acceptance downloads each one (showing "x of y" in the download
// dialog), then asks for a restart and signals unknown_b26518. Without patches, on refusal or on a
// cancelled download it signals unknown_b264f8.

HRESULT __stdcall CChatEventSink::OnUpdateList(HRESULT result, WOL::Update *updates)
{
    int count = 0;
    if (result >= 0 && updates) {
        unknown_login_state_00b2abe4 = 1;
        WOL::Update *update;
        for (update = updates; update; update = update->next) ++count;
        void *list = WS_Find_Dialog_00735940(0xc4);
        if (list) {
            if (!(unknown_b2abe0 &= ~0x10)) Unknown_007357E0(list, 0);
        }
        HWND current = Unknown_00735B10();
        HWND edit = GetDlgItem(current, 0x540);
        if (edit) SetFocus(edit);
        if (Popup_005c3230(StringTable::Load_String("TXT_UPGRADEREQUIRED", 0, "D:\\Ra2\\wonline.cpp", 0x10de), 4, Unknown_0073c120, 0) == 7) {
            SetEvent(unknown_b264f8);
            return S_OK;
        }
        char directory[260];
        GetCurrentDirectoryA(256, directory);
        int index = 0;
        update = updates;
        while (update) {
            char file[260];
            char local[260];
            wchar_t text[200];
            sprintf(file, "%s/%s", update->patchpath, update->patchfile);
            sprintf(local, "%s\\%s", update->localpath, update->patchfile);
            CreateDirectoryA(update->localpath, 0);
            unknown_download_a72d04->DownloadFile(update->server, update->login, update->password, file, local, "SOFTWARE\\Westwood\\Red Alert 2");
            ++index;
            swprintf(text, StringTable::Load_String("TXT_DOWNLOADING_X_OF_Y", 0, "D:\\Ra2\\wonline.cpp", 0x10f4), index, count);
            WideString message(text);
            SimpleWonlineDialogControl control;
            HWND dialog = unknown_create_dialog_5ff610(0xd3, Download_Dialog_Proc_00748cd0, 0);
            HWND item = GetDlgItem(dialog, 0x5c7);
            SendMessageA(item, 0x4b2, 0, (LPARAM)message.unknown_text_770430());
            if (Unknown_007359e0(dialog, (bool (*)())Unknown_0073c120, 0, 1, 1) == 2) {
                SetEvent(unknown_b264f8);
                SetCurrentDirectoryA(directory);
                return S_OK;
            }
            update = update->next;
        }
        Popup_005c3230(StringTable::Load_String("TXT_GAME_RESTART", 0, "D:\\Ra2\\wonline.cpp", 0x1114), 0, Unknown_0073c120, 0);
        SetEvent(unknown_b26518);
        SetCurrentDirectoryA(directory);
        return S_OK;
    }
    SetEvent(unknown_b264f8);
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnSquadInfo(HRESULT result, unsigned long id, WOL::Squad *squad) (0x0075ED00, slot 32)
// Stores the squad's name and abbreviation in the users of that squad (and, when it was the
// local player's lookup, the local squad), then requests the squad's ladder entry if uncached.

HRESULT __stdcall CChatEventSink::OnSquadInfo(HRESULT result, unsigned long id, WOL::Squad *squad)
{
    --unknown_query_count_b2abf4;
    if (unknown_squad_query_b2abf8) {
        unknown_squad_query_b2abf8 = 0;
        unknown_squad_id_b2abfc = id;
        if (result != S_OK) return S_OK;
        strcpy(unknown_squad_name_b2a8b0, squad->abbreviation);
    } else if (result != S_OK) {
        return S_OK;
    }
    for (int index = 0; index < unknown_users_b2a728.Count; ++index) {
        WOL::User *user = 0;
        unknown_users_b2a728.Get(user, index);
        if (user->unknown_14 == id) {
            strcpy(user->unknown_57, squad->abbreviation);
            strcpy(user->unknown_2e, squad->name);
        }
    }
    Wstring name(squad->abbreviation);
    name.To_Lower();
    if (!unknown_ladder_b296f8.Contains(name)) {
        unknown_ladder_client_a72d00->Request(unknown_ladder_address_b29018, unknown_ladder_port_b2a8ac, squad->abbreviation, 0x802100, -1, 0, 0);
    }
    if (unknown_query_count_b2abf4 == 0) Unknown_0073FEE0(0, false);
    return S_OK;
}


// int __stdcall unknown_callback_741750(int arg1, int result, UnknownTarget_0075eeb0 *target) -- 0x0075EEB0
// Purpose: a callback: when result is not negative, runs 0x00741750 on
// target; returns 0. arg1 is unused.
// Assumptions: the target class is unidentified (WOL area).

struct UnknownTarget_0075eeb0 {
    void unknown_call_741750();             // 0x00741750
};

int __stdcall unknown_callback_741750(int arg1, int result, UnknownTarget_0075eeb0 *target)
{
    if (result >= 0) {
        target->unknown_call_741750();
    }
    return 0;
}


// HRESULT __stdcall CChatEventSink::OnServerError(HRESULT result, char const *message) (0x0075EED0, slot 5)
// Unless 0x00B2ABD9 is set, shows "TXT_SERVER_ERROR: <message>" in the chat.

HRESULT __stdcall CChatEventSink::OnServerError(HRESULT result, char const *message)
{
    if (!unknown_b2abd9) {
        wchar_t buffer[512];
        swprintf(buffer, L"%s : %hs", StringTable::Load_String("TXT_SERVER_ERROR", 0, "D:\\Ra2\\wonline.cpp", 4456), message);
        Unknown_005C2C80(-1, buffer);
    }
    return S_OK;
}


extern "C" long __cdecl atol(char const *text);
extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
HRESULT __stdcall CChatEventSink::OnConnection(HRESULT result, char const *motd)
{
    unknown_connection_result_b2ac10 = result;
    if (result < 0) unknown_b2abd8 = true;
    switch (result) {
    case (HRESULT)0x80040064:
        wcscpy(unknown_connection_error_b2a8ec, StringTable::Load_String("TXT_LOGIN_USED", 0, "D:\\Ra2\\wonline.cpp", 0x11ba));
        SetEvent(unknown_b264f8);
        break;
    case (HRESULT)0x80040065:
        wcscpy(unknown_connection_error_b2a8ec, StringTable::Load_String("TXT_BADPASS", 0, "D:\\Ra2\\wonline.cpp", 0x11bf));
        SetEvent(unknown_b264f8);
        break;
    case (HRESULT)0x80040072:
        wcscpy(unknown_connection_error_b2a8ec, StringTable::Load_String("TXT_BANNED", 0, "D:\\Ra2\\wonline.cpp", 0x11c4));
        SetEvent(unknown_b264f8);
        break;
    case (HRESULT)0x80040074:
        wcscpy(unknown_connection_error_b2a8ec, StringTable::Load_String("TXT_DISABLED", 0, "D:\\Ra2\\wonline.cpp", 0x11ca));
        SetEvent(unknown_b264f8);
        break;
    case (HRESULT)0x80040075:
        wcscpy(unknown_connection_error_b2a8ec, StringTable::Load_String("TXT_BANNED", 0, "D:\\Ra2\\wonline.cpp", 0x11cf));
        SetEvent(unknown_b264f8);
        break;
    case (HRESULT)0x80040076:
        wcscpy(unknown_connection_error_b2a8ec, StringTable::Load_String("TXT_SERIALDUP", 0, "D:\\Ra2\\wonline.cpp", 0x11d4));
        SetEvent(unknown_b264f8);
        break;
    case (HRESULT)0x80040077:
        wcscpy(unknown_connection_error_b2a8ec, StringTable::Load_String("TXT_SERIALUNKNOWN", 0, "D:\\Ra2\\wonline.cpp", 0x11d9));
        SetEvent(unknown_b264f8);
        break;
    case (HRESULT)0x80040078:
        wcscpy(unknown_connection_error_b2a8ec, StringTable::Load_String("TXT_SKUSERIALMISMATCH", 0, "D:\\Ra2\\wonline.cpp", 0x11de));
        SetEvent(unknown_b264f8);
        break;
    default:
        wcscpy(unknown_connection_error_b2a8ec, StringTable::Load_String("TXT_CANT_CONNECT", 0, "D:\\Ra2\\wonline.cpp", 0x11e4));
        SetEvent(unknown_b264f8);
        break;
    case S_OK:
        if (!unknown_icons_b2acc4) {
            char const *types;
            unknown_chat_a72cfc->Game_Types(&types);
            char *copy = new char[strlen(types) + 32];
            strcpy(copy, types);
            strcat(copy, ",-1,0,18");
            char *token = strtok(copy, ",");
            while (token) {
                long id = atol(token);
                char const *game_name, *url;
                unsigned char *bitmap;
                int bitmap_bytes;
                unknown_chat_a72cfc->GetGametypeInfo(id, unknown_icon_format_7fd304, &bitmap, &bitmap_bytes, &game_name, &url);
                char file[64];
                sprintf(file, "gt%d.bmp", id);
                unknown_bitmap_writer_a76378.CacheBMP_0068c040(file, bitmap, bitmap_bytes, 2);
                token = strtok(0, ",");
            }
            delete [] copy;
            unknown_icons_b2acc4 = 1;
        }
        if (unknown_7fd2fc) unknown_motd_b26954 = motd;
        else unknown_motd_b26954.Release();
        unknown_b2abda = false;
        unknown_b2abe8 = 0;
        memset(&unknown_channel_b299c0, 0, sizeof(unknown_channel_b299c0));
        memset(&unknown_pending_channel_b26a60, 0, sizeof(unknown_pending_channel_b26a60));
        SetEvent(unknown_b26500);
        Find_Local_Address_0074fcb0(&unknown_address_a3fbc8);
        break;
    }
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnMessageOfTheDay(HRESULT result, char const *motd) (0x0075F290, slot 7)
// Shows the message of the day line by line, then an empty line.

HRESULT __stdcall CChatEventSink::OnMessageOfTheDay(HRESULT result, char const *motd)
{
    wchar_t buffer[1024];
    if (result < 0) {
        return S_OK;
    }
    char *copy = _strdup(motd);
    char *line = strtok(copy, "\n");
    while (line) {
        Char_To_Wide_006fa0c0(buffer, line, -1);
        Unknown_005C2C80(unknown_color_7a7360, buffer);
        line = strtok(0, "\n");
    }
    free(copy);
    Unknown_005C2C80(unknown_color_7a7360, L"\n");
    return S_OK;
}


static inline void Complete_Channel_Create_Request()
{
    void *dialog = WS_Find_Dialog_00735940(0xc4);
    if (dialog && (unknown_b2abe0 &= ~4u) == 0) Unknown_007357E0(dialog, 0);
    HWND window = Unknown_00735B10();
    window = GetDlgItem(window, 0x540);
    if (window) SetFocus(window);
}
HRESULT __stdcall CChatEventSink::OnChannelCreate(HRESULT result, WOL::Channel *channel)
{
    char user_options[80];
    if (result < 0) {
        Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_CHANCREATE_FAILED", 0, "D:\\Ra2\\wonline.cpp", 0x1212));
        unknown_b2ac58 = 0;
        unknown_channel_b299c0.name[0] = 0;
        Complete_Channel_Create_Request();
        return S_OK;
    } else {
        if (channel->unknown_00 == 0) {
            Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_CREATED_CHAN", 0, "D:\\Ra2\\wonline.cpp", 0x121c), Narrow_To_Wide_006fa180(channel->name));
        } else {
            memset(unknown_player_b29cf0, 0, sizeof(unknown_player_b29cf0));
            char const *name = unknown_wstring_b26cb8.Get();
            strcpy(unknown_player_b29cf0[0].name, name);
            unknown_player_b29cf0[0].unknown_18 = unknown_a3d3bc;
            unknown_player_b29cf0[0].unknown_1c = 0;
            unknown_player_b29cf0[0].unknown_14 = unknown_a3d3b4;
            unknown_player_b29cf0[0].unknown_20 = 1;
        }
        unknown_channel_b299c0 = *channel;
        unknown_b2ac58 = 1;
        ++unknown_b2abe8;
        Debug_Printf("Created channel\n");
        if (unknown_chat_a72cfc && channel->unknown_00 != 0) {
            memset(user_options, 0, sizeof(user_options));
            Format_User_Options_0073cff0(user_options);
            user_options[40] = 0;
            unknown_chat_a72cfc->unknown_slot42(user_options);
        }
        Complete_Channel_Create_Request();
    }
    return S_OK;
}


// Not exact (RA2_NON_MATCHING): CChatEventSink::OnChannelJoin at 0x0075F530, best score 75.54, 2026-11-13; differs: 1753 instructions in the original, 1832 in the draft (shape 96.29)
#ifdef RA2_NON_MATCHING
// HRESULT __stdcall CChatEventSink::OnChannelJoin(HRESULT result, WOL::Channel *channel, WOL::User *user) (0x0075F530, slot 10)
extern HANDLE unknown_b26508;
extern HANDLE unknown_b26520;
extern int unknown_b2abf0;
extern int unknown_b292ac[8];
extern char unknown_b2658c[];
extern char lobby_prefix_7fd358[];
extern wchar_t const lobby_name_format_7d9c94[];
extern wchar_t const lobby_number_format_7fd91c[];
extern MultiplayerGameMode *unknown_mode_a3d29c;
extern int unknown_7e6d90;
extern void *ProgramInstance;
void __fastcall Decode_Pub_Gameopt_005c3ff0(char const *, char const *);
void __fastcall Unknown_0073c480();
bool __fastcall Can_Join_Channel_00755cd0(WOL::Channel const *channel, bool notify, bool observer);
void __stdcall unknown_dialog_5ecf40(HWND, int);
int __stdcall Wonline_Guest_Dialog_0074c660(HWND, UINT, WPARAM, LPARAM);
void __fastcall Update_Channel_Player_Limit_00749f20();
void __fastcall Broadcast_User_Pings_0075b1b0();
void __fastcall Apply_Endpoint_Map_0075c570(char const *);
extern "C" char *__stdcall inet_ntoa(unsigned long address);

// wlib ArrayList::remove, expanded where it is used.
inline bool WolChannelArray::Remove(int index)
{
    if (Count == 0) return false;
    if (index < 0) index = 0;
    if (index >= Count) index = Count - 1;
    memmove(Items + index, Items + index + 1, (Count - (index + 1)) * sizeof(WOL::Channel));
    --Count;
    if (Count * 3 <= Capacity) Shrink_00766dc0();
    return true;
}

static inline void Close_Wait_Flag(unsigned int flag)
{
    void *dialog = WS_Find_Dialog_00735940(0xc4);
    if (dialog && (unknown_b2abe0 &= ~flag) == 0) Unknown_007357E0(dialog, 0);
    HWND window = Unknown_00735B10();
    window = GetDlgItem(window, 0x540);
    if (window) SetFocus(window);
}
// The address of a user from its IP (the same body as 0x0073C130, expanded here).
inline IPXAddressClass __fastcall User_Address(WOL::User const *user)
{
    NetNumType net;
    NetNodeType node;
    memset(net, 0, sizeof(net));
    memset(node, 0, sizeof(node));
    unsigned long ip = ntohl(user->unknown_18);
    node[3] = (unsigned char)ip;
    node[2] = (unsigned char)(ip >> 8);
    node[1] = (unsigned char)(ip >> 16);
    node[0] = (unsigned char)(ip >> 24);
    IPXAddressClass address(net, node, 0);
    return address;
}
static inline NodeNameType *New_Player(WOL::User *user)
{
    NodeNameType *node = new NodeNameType;
    memset(node, 0, sizeof(NodeNameType));
    wcscpy(node->Name, Narrow_To_Wide_006fa180(user->name));
    node->Address = User_Address(user);
    return node;
}
static inline void Request_Squad(WOL::User *user)
{
    if (user->unknown_14 && strlen(user->unknown_57) == 0) {
        ++unknown_query_count_b2abf4;
        unknown_chat_a72cfc->unknown_slot44(user->unknown_14);
    }
}
static inline void Make_Observer(NodeNameType *node)
{
    node->unknown_66e2d0(-3);
    node->unknown_66e300(-2);
    node->Team = -1;
}

HRESULT __stdcall CChatEventSink::OnChannelJoin(HRESULT result, WOL::Channel *channel, WOL::User *user)
{
    if (result == 0x8004006C) return S_OK;
    wchar_t channel_name[64];
    channel_name[0] = 0;
    if (channel) {
        Char_To_Wide_006fa0c0(channel_name, channel->name, -1);
        if (strncmp(channel->name, lobby_prefix_7fd358, 7) == 0) {
            int number = atol(channel->name + strlen(lobby_prefix_7fd358));
            int page = number / 8;
            int lobby = number % 8;
            if (page == 0) swprintf(channel_name, lobby_name_format_7d9c94, unknown_lobby_names_b26c94[lobby]);
            else swprintf(channel_name, lobby_number_format_7fd91c, unknown_lobby_names_b26c94[lobby], page + 1);
        }
    }
    if (result < 0) {
        if (result == 0x80040070) {
            WOL::Channel *entry = 0;
            int i;
            for (i = 0; i < unknown_lobbies_b26c88.Count; ++i) {
                unknown_lobbies_b26c88.Get(entry, i);
                if (strcmp(unknown_pending_channel_b26a60.name, entry->name) == 0) {
                    unknown_lobbies_b26c88.Remove(i);
                    break;
                }
            }
            for (i = 0; i < unknown_games_b29990.Count; ++i) {
                unknown_games_b29990.Get(entry, i);
                if (strcmp(unknown_pending_channel_b26a60.name, entry->name) == 0) {
                    unknown_games_b29990.Remove(i);
                    break;
                }
            }
            if (Is_Online_Session()) {
                Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_GAME_CLOSED", 0, "D:\\Ra2\\wonline.cpp", 0x128a));
                SetEvent(unknown_b2650c);
            } else {
                Popup_005c3230(StringTable::Load_String("TXT_GAME_CLOSED", 0, "D:\\Ra2\\wonline.cpp", 0x128d), 0, Unknown_0073c120, 0);
            }
        } else if (result == 0x80040071) {
            if (Is_Online_Session()) {
                Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_BADPASS", 0, "D:\\Ra2\\wonline.cpp", 0x1294));
                SetEvent(unknown_b2650c);
            } else {
                Popup_005c3230(StringTable::Load_String("TXT_BADPASS", 0, "D:\\Ra2\\wonline.cpp", 0x1297), 0, Unknown_0073c120, 0);
            }
        } else if (result == 0x8004006E) {
            if (Is_Online_Session()) {
                Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_CHANNEL_FULL", 0, "D:\\Ra2\\wonline.cpp", 0x129e));
                SetEvent(unknown_b2650c);
            } else {
                Popup_005c3230(StringTable::Load_String("TXT_CHANNEL_FULL", 0, "D:\\Ra2\\wonline.cpp", 0x12a1), 0, Unknown_0073c120, 0);
                if (unknown_pending_channel_b26a60.unknown_00 == 0) SetEvent(unknown_b2650c);
            }
        } else if (result == 0x80040072) {
            Popup_005c3230(StringTable::Load_String("TXT_JOINBAN", 0, "D:\\Ra2\\wonline.cpp", 0x12aa), 0, Unknown_0073c120, 0);
            SetEvent(unknown_b2650c);
            ++unknown_b2abec;
        } else {
            Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_CANT_JOINCHAN", 0, "D:\\Ra2\\wonline.cpp", 0x12b1), result);
            SetEvent(unknown_b2650c);
        }
        unknown_b2abf0 = unknown_pending_channel_b26a60.unknown_00 != 0;
        unknown_b2ac58 = 0;
        unknown_channel_b299c0.name[0] = 0;
        memset(&unknown_pending_channel_b26a60, 0, sizeof(unknown_pending_channel_b26a60));
        Close_Wait_Flag(2);
        unknown_pending_channel_b26a60.name[0] = 0;
        Unknown_0073f8f0();
        Unknown_0073FEE0(0, false);
        SetEvent(unknown_b26520);
        HWND window = (HWND)WS_Find_Dialog_00735940(0x113);
        if (window) {
            ShowWindow(window, 5);
            unknown_dialog_5ecf40(window, 0);
        }
        return S_OK;
    }
    unknown_pending_channel_b26a60.name[0] = 0;
    if (user->flags & 0x8000) {
        unknown_channel_b299c0 = *channel;
        memset(&unknown_pending_channel_b26a60, 0, sizeof(unknown_pending_channel_b26a60));
        Close_Wait_Flag(2);
        ++unknown_b2abe8;
        Debug_Printf("Joined Channel\n");
        if (!Is_Online_Session())
            Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_JOINED_S", 0, "D:\\Ra2\\wonline.cpp", 0x12e1), channel_name);
        if (channel->unknown_00 == 0) {
            unknown_b2abec = atol(channel->name + strlen(lobby_prefix_7fd358));
            HWND main = (HWND)WS_Find_Dialog_00735940(0x113);
            if (main) {
                EnableWindow(GetDlgItem(main, 0x53d), 1);
                bool can_join = false;
                bool can_observe = false;
                HWND list = GetDlgItem(main, 0x6ee);
                int selection = SendMessageA(list, 0x188, 0, 0);
                if (selection != -1) {
                    int index = SendMessageA(list, 0x199, selection, 0);
                    if (index >= 0 && index < unknown_games_b29990.Count) {
                        WOL::Channel game;
                        memset(&game, 0, sizeof(game));
                        game = unknown_games_b29990.Items[index];
                        can_join = Can_Join_Channel_00755cd0(&game, false, false);
                        can_observe = Can_Join_Channel_00755cd0(&game, false, true);
                    }
                }
                EnableWindow(GetDlgItem(main, 0x6ee), 1);
                EnableWindow(GetDlgItem(main, 0x62d), can_join || can_observe);
                EnableWindow(GetDlgItem(main, 0x62b), 1);
                EnableWindow(GetDlgItem(main, 0x689), 1);
                EnableWindow(GetDlgItem(main, 0x558), 1);
                EnableWindow(GetDlgItem(main, 0x61f), 1);
                EnableWindow(GetDlgItem(main, 0x686), 1);
                SetFocus(GetDlgItem(main, 0x540));
            }
        }
        if (channel->unknown_00 == 0x21) {
            MultiplayerGameMode *mode = MultiplayerGameMode::unknown_first_mode_5b6ec0();
            unknown_mode_a3d29c = mode;
            unknown_int_a3db1c = 8;
            unknown_7e6d90 = 7;
            if (mode) mode->unknown_slot9();
            Free_Pointers_005a7480(&unknown_players_a3fa7c);
            Clear_Slot_Lists_005c8a70();
            Unknown_Reset_Slots_005c8a90();
            NodeNameType *node = New_Player(user);
            node->unknown_66e2d0(-2);
            node->unknown_66e300(-2);
            if (!Is_Online_Session() && unknown_a72c68) {
                node->unknown_66e2d0(-3);
                node->Team = -1;
            }
            unknown_session_a3d298.Players.Add(node);
            unknown_host_name_a3f618[0] = 0;
            if (!Is_Online_Session()) {
                unknown_5159b0(0, true);
                void *guest = unknown_create_dialog_735700(ProgramInstance, 0xc9, (HWND)unknown_b263d8, (UnknownDialogProc)Wonline_Guest_Dialog_0074c660, 0);
                unknown_show_dialog_5ff7e0(guest, 0xc9);
                HWND main = (HWND)WS_Find_Dialog_00735940(0x113);
                if (main) {
                    ShowWindow(main, 0);
                    unknown_dialog_5ecf40(main, 0);
                }
                SendMessageA((HWND)guest, 0x4a9, 0, 1);
                ShowWindow((HWND)guest, 1);
                memset(unknown_b292ac, 0xff, sizeof(unknown_b292ac));
                Request_Squad(user);
            }
        }
    } else {
        if (unknown_channel_b299c0.unknown_00 == 0x21) Voc_Play(Rules->PlayerJoined, 1.0f, 0x2000, 0);
        if (channel->unknown_00 != 0 && (unknown_b2ac04 || unknown_b2ac40 || unknown_b2ac45)) {
            unknown_chat_a72cfc->unknown_slot21(user);
            return S_OK;
        }
        Request_Squad(user);
        {
        Wstring name;
        name.Set(user->name);
        name.To_Lower();
        if (unknown_game_mode_a3d298 == 4 && unknown_a3d2a4 == 2) {
            Wstring other;
            int i;
            bool found = false;
            for (i = 0; i < unknown_game_names_b2a648.Count; ++i) {
                unknown_game_names_b2a648.Get(other, i);
                if (other == name) {
                    found = true;
                    break;
                }
            }
            if (!found) unknown_game_names_b2a648.Prepend(name);
        }
        if (!unknown_channel_names_b293a0.Contains(name)) {
            Wstring other;
            int i;
            bool found = false;
            for (i = 0; i < unknown_pending_names_b29468.Count; ++i) {
                unknown_pending_names_b29468.Get(other, i);
                if (other == name) {
                    found = true;
                    break;
                }
            }
            if (!found) unknown_pending_names_b29468.Prepend(name);
        }
        if (!unknown_ladder_cache_b2ac24->Contains(name)) {
            unknown_ladder_client_a72d00->Request(unknown_ladder_address_b29018, unknown_ladder_port_b2a8ac, name.Get(), unknown_protocol_7fd370 & 0xffff00, -1, 0, 0);
        }
        }
        if (channel->unknown_00 != 0) {
            for (int *accepted = &unknown_player_b29cf0[1].unknown_20; (unsigned)accepted < (unsigned)&unknown_player_b29cf0[16].unknown_20; accepted += sizeof(WolPlayerData) / sizeof(int)) {
                *accepted = 0;
                EnableWindow(GetDlgItem(Find_Lobby_Window_005c2c40(), 0x59f), 1);
            }
            if (unknown_b2ac58 == 0) {
                HWND guest = (HWND)WS_Find_Dialog_00735940(0xc9);
                Unknown_005cd030(guest, true);
            }
            Unknown_005c9700();
        }
    }
    int position = unknown_users_b2a728.Count;
    WOL::User other;
    for (int entry_index = 0; entry_index < unknown_users_b2a728.Count; ++entry_index) {
        unknown_users_b2a728.Get_Copy(other, entry_index);
        if (Compare_Users_007416d0(*user, other) < 0) {
            position = entry_index;
            break;
        }
    }
    WOL::User copy = *user;
    copy.next = 0;
    unknown_users_b2a728.Insert(copy, position);
    if (!(user->flags & 0x8000) && channel->unknown_00 == 0 && Is_Online_Session()) {
        Wstring pending(user->name);
        unknown_strings_b29980.Add(pending);
        if (unknown_b29828 == 0) Unknown_0075B2D0();
    }
    int index = 0;
    int players = unknown_users_b2a728.Count;
    if (channel->unknown_00 != 0) {
        char const *name = user->name;
        index = unknown_users_b2a728.Find_Name(name);
        memmove(unknown_player_b29cf0 + index + 1, unknown_player_b29cf0 + index, sizeof(WolPlayerData) * (15 - index));
        memset(unknown_player_b29cf0 + index, 0, sizeof(WolPlayerData));
        strcpy(unknown_player_b29cf0[index].name, name);
        if (unknown_b2ac58 == 1) {
            if (!Is_Online_Session()) Update_Channel_Player_Limit_00749f20();
            if (Slots_Filled_005c9440() && No_Observer_Slot_005c9490()) {
                Wstring kicked(name);
                WOL::User entry;
                entry.flags = 0;
                for (int i = 0; i < unknown_users_b2a728.Count; ++i) {
                    unknown_users_b2a728.Get_Copy(entry, i);
                    if (_stricmp(kicked.Peek_Buffer(), entry.name) == 0) {
                        unknown_chat_a72cfc->unknown_slot21(&entry);
                        break;
                    }
                }
            } else {
                int color = 0;
                int taken = 0;
                while (true) {
                    unknown_player_b29cf0[index].unknown_14 = color;
                    taken = 0;
                    for (int i = 0; i < players; ++i) {
                        if (i != index && unknown_player_b29cf0[i].unknown_14 == color) {
                            ++color;
                            taken = 1;
                        }
                    }
                    if (taken == 0) break;
                }
                NodeNameType *node = New_Player(user);
                if (unknown_mode_a3d29c) node->unknown_66e2d0(unknown_mode_a3d29c->unknown_slot10());
                else node->unknown_66e2d0(-2);
                node->unknown_66e300(-2);
                if (!Is_Online_Session() && Slots_Filled_005c9440()) Make_Observer(node);
                if ((user->flags & 0x8000) && !Is_Online_Session() && unknown_a72c68) Make_Observer(node);
                unknown_session_a3d298.Players.Add(node);
                Assign_Slot_005c9170(node);
                if (Is_Online_Session() && channel->unknown_00 != 0) {
                    if (unknown_a3d2a4 != 2) Decode_Pub_Gameopt_005c3ff0(unknown_b2658c, 0);
                    else Apply_Endpoint_Map_0075c570(strchr(unknown_b2658c, ':'));
                    int i;
                    for (i = 0; i < unknown_multi_missions_a3d8d8.Count(); ++i) {
                        char const *scenario = unknown_multi_missions_a3d8d8[i]->unknown_58;
                        if (_stricmp(unknown_session_a3d298.ScenarioFileName_658, scenario) == 0) {
                            Select_Scenario_005c7c30(i);
                            break;
                        }
                    }
                    if (i >= unknown_multi_missions_a3d8d8.Count()) Select_Scenario_005c7c30(0);
                }
                Unknown_005c9700();
                Unknown_005c34f0();
                char text[3];
                text[0] = 'O';
                int observers = unknown_7e6d90;
                if (observers < 0) observers = 8;
                text[1] = (char)observers + '0';
                text[2] = 0;
                Debug_Printf("Sending observer slot of %c\n", text[1]);
                unknown_chat_a72cfc->unknown_slot15(text);
                Unknown_0073c480();
                Unknown_005c3810(true, false);
                if (Is_Host_005c86a0()) {
                    Broadcast_User_Pings_0075b1b0();
                    if (unknown_b2ac40 == 0) {
                        WOL::User entry;
                        memset(&entry, 0, sizeof(entry));
                        int count = unknown_users_b2a728.Count;
                        int handle = 0;
                        for (int i = 0; i < count; ++i) {
                            unknown_users_b2a728.Get_Copy(entry, i);
                            unsigned long address = entry.unknown_18;
                            Debug_Printf("Requesting ping for %s at %s\n", entry.name, inet_ntoa(address));
                            unknown_ladder_client_a72d00->Request_Ping(inet_ntoa(address), 1000, &handle);
                        }
                    }
                }
            }
        } else if (!(user->flags & 0x8000)) {
            NodeNameType *node = New_Player(user);
            node->unknown_66e2d0(-2);
            node->unknown_66e300(-2);
            unknown_session_a3d298.Players.Add(node);
            HWND guest = (HWND)WS_Find_Dialog_00735940(0xc9);
            Unknown_005cd030(guest, true);
            Unknown_005c9700();
        }
    }
    SetEvent(unknown_b26508);
    Unknown_0073FEE0(0, false);
    return S_OK;
}
#endif

// HRESULT __stdcall CChatEventSink::OnChannelLeave(HRESULT result, WOL::Channel *channel, WOL::User *user) (0x00760BD0, slot 11)
// Logs failures; otherwise passes the leaving user to 0x00741750.

HRESULT __stdcall CChatEventSink::OnChannelLeave(HRESULT result, WOL::Channel *channel, WOL::User *user)
{
    if (result < 0) {
        Debug_Printf("OnChannelLeave with error!\n");
        return S_OK;
    }
    Unknown_00741750(user);
    return S_OK;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// HRESULT CChatEventSink::OnUserList(HRESULT, WOL::Channel *, WOL::User *) (0x00760C00)
// Rebuilds the WOL channel user list and queues missing profile lookups.
// From ra2-decomp 0x007A7110 (YR twin FUN_007A7110, src/wonline.cpp): the session player list is read through
// the session object (Session.Players) and a user's address comes from Unknown_0073c130 (YR FUN_0077D720).
// Built on codex4's pass7_760c00 draft (Scout696 keep/ul_c4_9972.cpp).
extern char ladder_names_separator_7e6f8c[], user_reply_format_7fdec4[],user_reply_debug_7fdea0[];
extern unsigned int local_socket_a71b18;
HRESULT __stdcall CChatEventSink::OnUserList(HRESULT result,WOL::Channel *channel,WOL::User *user)
{
    if (unknown_b2abd9) return S_OK;
    memset(unknown_query_b2b420,0,sizeof(char)*512);
    Wstring ladder_name;
    if (FAILED(result)) return S_OK;
    SendDlgItemMessageA(Unknown_00735B10(),0x53e,0x184,0,0);
    unknown_users_b2a728.Count=0;
    int batch_count=0;
    int user_count=0;
    bool saw_owner=false;
    while (user) {
        char name_buffer[128];
        ++user_count;
        if (user->unknown_14 != 0 && strlen(user->unknown_57) == 0) {
            ++unknown_query_count_b2abf4;
            unknown_chat_a72cfc->unknown_slot44(user->unknown_14);
        } else if (unknown_ladder_client_a72d00 && user->unknown_14 != 0 && strlen(user->unknown_57) != 0) {
            Wstring squad(user->unknown_57);
            squad.To_Lower();
            if (!unknown_ladder_b296f8.Contains(squad))
                unknown_ladder_client_a72d00->Request(unknown_ladder_address_b29018,unknown_ladder_port_b2a8ac,user->unknown_57,0x802100,-1,0,0);
        }
        unsigned flags=user->flags;
        strcpy(name_buffer,user->name);
        WOL::User entry=*user;
        entry.next=0;
        unknown_users_b2a728.Insert_00765e90(entry,unknown_users_b2a728.Count);
        if (flags & 1) saw_owner=true;
        ladder_name.Set(user->name);
        ladder_name.To_Lower();
        if (unknown_game_mode_a3d298 == 4 && unknown_a3d2a4 == 2 && !unknown_known_names_b294b8.Contains(ladder_name))
            unknown_game_names_b2a648.Insert(ladder_name,0);
        if (!unknown_channel_names_b293a0.Contains(ladder_name)) unknown_pending_names_b29468.Insert(ladder_name,0);
        if (!unknown_ladder_cache_b2ac24->Contains(ladder_name) && user_count < 100) {
            ladder_name.Set(user->name);
            ladder_name.To_Lower();
            strcat(unknown_query_b2b420,ladder_name.Get());
            strcat(unknown_query_b2b420,ladder_names_separator_7e6f8c);
            ++batch_count;
        }
        if (batch_count > 20) {
            unknown_ladder_client_a72d00->Request(unknown_ladder_address_b29018,unknown_ladder_port_b2a8ac,unknown_query_b2b420,unknown_protocol_7fd370 & 0xffff00,-1,0,0);
            batch_count=0;
            memset(unknown_query_b2b420,0,512);
        }
        if (unknown_channel_b299c0.unknown_00 != 0 && !unknown_b2ac58) {
            Wstring name(user->name);
            int index=unknown_users_b2a728.Find_Name(name);
            memset(unknown_player_b29cf0+index,0,sizeof(WolPlayerData));
            strcpy(unknown_player_b29cf0[index].name,user->name);
            if (_stricmp(user->name,unknown_wstring_b26cb8.Get()) != 0) {
                if (unknown_session_a3d298.Players.Count() == 1) wcscpy(unknown_session_a3d298.HostName_2380,Narrow_To_Wide_006fa180(user->name));
                NodeNameType *node=new NodeNameType;
                memset(node,0,sizeof(NodeNameType));
                WideString wide_name=Wstring_To_Wide(user->name);
                wcscpy(node->Name,wide_name.Get());
                node->Address=Unknown_0073c130(user);
                node->unknown_66e2d0(-2);
                node->unknown_66e300(-2);
                unknown_session_a3d298.Players.Add(node);
            }
        }
        user=user->next;
    }
    if (strlen(unknown_query_b2b420))
        unknown_ladder_client_a72d00->Request(unknown_ladder_address_b29018,unknown_ladder_port_b2a8ac,unknown_query_b2b420,unknown_protocol_7fd370 & 0xffff00,-1,0,0);
    if (unknown_channel_b299c0.unknown_00 != 0) {
        if (!saw_owner) {
            WOL::User entry;
            for (int index=0;index<unknown_users_b2a728.Count;++index) {
                if (unknown_users_b2a728.Get_Copy(entry,index) && !_stricmp(entry.name,unknown_wstring_b26cb8.Get())) {
                    Unknown_00741750(&entry);
                    break;
                }
            }
        }
        if (unknown_channel_b299c0.unknown_00 != 0) {
            if (!unknown_b2ac58 && unknown_users_b2a728.Count == 1 && !_stricmp(user->name,unknown_wstring_b26cb8.Get())) Unknown_00741750(user);
            if (unknown_channel_b299c0.unknown_00 != 0) {
                unknown_a72d08=1;
                if (!unknown_b2ac58) {
                    HWND window=WS_Find_Dialog_00735940(0xc9);
                    Unknown_005cd030(window,true);
                }
                Unknown_005c9700();
                WolRegistryOptions options;
                options.Read_004ff200();
                rc_ptr<PlayerProfile> profile;
                if (options.unknown_004ff2c0()) profile=PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
                else profile.Set(unknown_profile_b28cc4);
                unknown_session_a3d298.Set_Value_00672a30(profile->Country,false);
                unknown_session_a3d298.Set_Value_00672ae0(profile->Color,false);
                if (unknown_a72c68) {
                    if (!Is_Online_Session()) {
                        unknown_session_a3d298.Set_Value_00672a30(-3,false);
                        unknown_session_a3d298.Set_Value_00672ae0(-2,false);
                    }
                }
                if (!unknown_a72c68 && !Is_Online_Session() && unknown_session_a3d298.Value_0124_006729f0() == -3) {
                    if (unknown_session_a3d298.Players.Count() != 0) {
                        unknown_session_a3d298.Set_Value_00672a30(unknown_session_a3d298.Players[0]->unknown_66e290(),false);
                        unknown_session_a3d298.Set_Value_00672ae0(unknown_session_a3d298.Players[0]->unknown_66e2a0(),false);
                    } else {
                        unknown_session_a3d298.Set_Value_00672a30(-2,false);
                        unknown_session_a3d298.Set_Value_00672ae0(-2,false);
                    }
                }
                char buffer[128];
                {
                NetNumType network;
                NetNodeType address;
                unsigned short socket;
                unknown_address_a3fbc8.Get_Address(network,address,&socket);
                sprintf(buffer,user_reply_format_7fdec4,unknown_session_a3d298.Value_0124_006729f0(),unknown_session_a3d298.Value_011c_00672a10(),*(unsigned *)address,local_socket_a71b18&0xffff,unknown_a40230);
                Debug_Printf(user_reply_debug_7fdea0,buffer);
                }
                WOL::User host;
                if (unknown_users_b2a728.Get_Copy(host,0)) unknown_chat_a72cfc->unknown_slot14(&host,buffer);
            }
        }
    }
    if (Is_Online_Session() && unknown_channel_b299c0.unknown_00 == 0 && !Unknown_0075a920()) SetEvent(unknown_b2650c);
    Unknown_0073FEE0(0,false);
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnGameStart(HRESULT result, WOL::Channel *channel, WOL::User *users, int game_id) (0x00761570, slot 23)
// Starts the game once every listed player is in the user list: records the game id, protocol,
// host and start time, then sets up the session's players.

HRESULT __stdcall CChatEventSink::OnGameStart(HRESULT result, WOL::Channel *channel, WOL::User *users, int game_id)
{
    bool all_found = true;
    int count = unknown_users_b2a728.Count;
    int listed = 0;
    unknown_timer_b292e8 = CDTimerClass<SystemTimerClass>(0);
    if (unknown_b2ac45) return S_OK;
    unknown_login_state_00b2abe4 = 1;
    WOL::User *user = users;
    while (user) {
        ++listed;
        Wstring name(user->name);
        name.To_Lower();
        int i;
        for (i = 0; i < count; ++i) {
            WOL::User entry;
            unknown_users_b2a728.Get_Copy(entry, i);
            Wstring other(entry.name);
            other.To_Lower();
            if (name == other) break;
        }
        if (i >= count) {
            all_found = false;
            break;
        }
        user = user->next;
    }
    bool ok = listed == count && all_found;
    if (!ok) {
        unknown_b2ac04 = false;
        unknown_b2ac40 = 0;
    } else {
        if (unknown_a3d2a4 == 2 || unknown_a3d2a4 == 1) {
            void *dialog = WS_Find_Dialog_00735940(0x117);
            Unknown_007357E0(dialog, 0);
        }
        unknown_b2a7fc = game_id;
        unknown_b2ab84 = unknown_protocol_7fd370;
        unknown_b2668c = 0x2300;
        unknown_b2a7ec = unknown_b299d4;
        if (Unknown_0073c3a0()) unknown_b2a7ec = 2;
        strcpy(unknown_b2a6d8, unknown_wstring_b26cb8.Get());
        WOL::User *first = 0;
        unknown_users_b2a728.Get(first, 0);
        if (first) strcpy(unknown_b2a808, first->name);
        unknown_game_start_time_b2a638 = time(0);
        Debug_Printf("OnGameStart\n");
        if (channel) unknown_b2abec = channel->unknown_20 & 0xff;
        unknown_b2ac44 = false;
        unknown_b2ac45 = true;
        Unknown_0074d7d0(users);
        unknown_b2ac40 = 2;
        Debug_Printf("Starting a %d player game.\n", unknown_session_a3d55c);
        WOL::User *host = Unknown_0074DA60();
        if (host) Char_To_Wide_006fa0c0(unknown_session_a3d298.MasterPlayerName, host->name, -1);
        unknown_session_a3d298.MasterPlayerID = -1;
        unknown_session_a40054 = Unknown_0074e070(unknown_session_a3fc54);
    }
    return S_OK;
}


HRESULT __stdcall CChatEventSink::OnPublicMessage(HRESULT result, WOL::Channel *channel, WOL::User *user, char const *message)
{
    if (!unknown_b2abd9 && result >= 0) {
        if (Is_Online_Session()) {
            if (user->flags & 1) {
                Unknown_0075B910(message, false);
                return S_OK;
            }
            return S_OK;
        }
        bool show;
        {
            // Unused copy of the text (hypothesis: left over from a removed check).
            Wstring copy(message);
            if (!user) {
                show = false;
            } else if (user->flags & 1) {
                show = true;
            } else if (WS_Find_Dialog_00735940(0xC2)) {
                show = true;
            } else if (WS_Find_Dialog_00735940(0xC9)) {
                show = true;
            } else {
                show = unknown_b2a7e8 != 0;
            }
        }
        if (!show) return S_OK;
        WideString text = Wstring_To_Wide(message);
        int color = unknown_color_7a7364;
        if (user->flags & 1) color = unknown_color_7a7374;
        if (_stricmp(unknown_name_a3d3a0, user->name)) {
            Unknown_005C2C80(color, L"[%hs] : %s", user->name, text.Get());
        }
    }
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnPublicUnicodeMessage(HRESULT result, WOL::Channel *channel, WOL::User *user, unsigned short const *message) (0x007626B0, slot 40)
// Shows "[<nick>] : <message>" for a visible wide-text channel message from another user.

HRESULT __stdcall CChatEventSink::OnPublicUnicodeMessage(HRESULT result, WOL::Channel *channel, WOL::User *user, unsigned short const *message)
{
    if (unknown_b2abd9) return S_OK;
    if (result < 0) return S_OK;
    WideString text(message);
    bool show;
    if (!user) {
        show = false;
    } else if (user->flags & 1) {
        show = true;
    } else if (WS_Find_Dialog_00735940(0xC2)) {
        show = true;
    } else if (WS_Find_Dialog_00735940(0xC9)) {
        show = true;
    } else {
        show = (text.Is_Narrow() ? unknown_wol_display_latin_b2a7e8 : unknown_wol_display_asian_b2a804) != 0;
    }
    if (!show) return S_OK;
    int color = unknown_color_7a7364;
    if (user->flags & 1) color = unknown_color_7a7374;
    if (_stricmp(unknown_name_a3d3a0, user->name)) {
        Unknown_005C2C80(color, L"[%hs] : %s", user->name, text.Get());
    }
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnPrivateMessage(HRESULT result, WOL::User *user, char const *message) (0x00762790, slot 17)
// In an online game session a message from the channel owner is an in-game message; otherwise
// shows "<nick> : <message>" for a visible private message from another user.

HRESULT __stdcall CChatEventSink::OnPrivateMessage(HRESULT result, WOL::User *user, char const *message)
{
    if (!unknown_b2abd9 && result >= 0) {
        if (Is_Online_Session()) {
            if (user->flags & 1) {
                Unknown_0075B910(message, true);
                return S_OK;
            }
            Debug_Printf("Received message from non-channel owner %s: %s\n", user->name, message);
            return S_OK;
        }
        bool show;
        {
            // Unused copy of the text (hypothesis: left over from a removed check).
            Wstring copy(message);
            if (!user) {
                show = false;
            } else if (user->flags & 1) {
                show = true;
            } else if (WS_Find_Dialog_00735940(0xC2)) {
                show = true;
            } else if (WS_Find_Dialog_00735940(0xC9)) {
                show = true;
            } else {
                show = unknown_b2a7e8 != 0;
            }
        }
        if (!show) return S_OK;
        WideString text = Wstring_To_Wide(message);
        int color = unknown_color_7a7368;
        if (user->flags & 1) color = unknown_color_7a7374;
        if (_stricmp(unknown_name_a3d3a0, user->name)) {
            Unknown_005C2C80(color, L"%hs : %s", user->name, text.Get());
        }
    }
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnPrivateUnicodeMessage(HRESULT result, WOL::User *user, unsigned short const *message) (0x007628D0, slot 41)
// Shows "<nick> : <message>" for a visible wide-text private message from another user.

HRESULT __stdcall CChatEventSink::OnPrivateUnicodeMessage(HRESULT result, WOL::User *user, unsigned short const *message)
{
    if (unknown_b2abd9) return S_OK;
    if (result < 0) return S_OK;
    WideString text(message);
    bool show;
    if (!user) {
        show = false;
    } else if (user->flags & 1) {
        show = true;
    } else if (WS_Find_Dialog_00735940(0xC2)) {
        show = true;
    } else if (WS_Find_Dialog_00735940(0xC9)) {
        show = true;
    } else {
        show = (text.Is_Narrow() ? unknown_wol_display_latin_b2a7e8 : unknown_wol_display_asian_b2a804) != 0;
    }
    if (!show) return S_OK;
    int color = unknown_color_7a7368;
    if (user->flags & 1) color = unknown_color_7a7374;
    if (_stricmp(unknown_name_a3d3a0, user->name)) {
        Unknown_005C2C80(color, L"%hs : %s", user->name, text.Get());
    }
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnPaged(HRESULT result, WOL::User *user, char const *message) (0x007629B0, slot 28)
// Remembers the pager's nick and shows "(<nick>) <message>": in game as a message and in the list
// (restarting the page timer); otherwise in the chat (or list) of the open dialogs, or, with none
// showing chat, in a new page reply dialog. Then plays the incoming-message sound.

HRESULT __stdcall CChatEventSink::OnPaged(HRESULT result, WOL::User *user, char const *message)
{
    wchar_t text[256];
    if (result < 0) return S_OK;
    WideString wide = Wstring_To_Wide(message);
    if (user) strcpy(unknown_last_pager_b2ac48, user->name);
    if (unknown_b2abd9) {
        swprintf(text, L"(%hs) %s", user->name, wide.Get());
        int color = Gadget_Color_Scheme();
        unknown_session_a3d298.Messages.Add_Message(0, 0, text, color, 0x16, Rules->MessageDelay * 900, false);
        Map.Flag_To_Redraw(false);
        Unknown_005c2d10(unknown_color_7a7378, L"(%hs) %s", user->name, wide.Get());
        unknown_timer_b292e8 = CDTimerClass<SystemTimerClass>(300);
    } else {
        HWND dialog = Unknown_00735B10();
        while (dialog) {
            if (GetDlgItem(dialog, 0x53f)) {
                Unknown_005C2C80(unknown_color_7a7378, L"(%hs) %s", user->name, wide.Get());
                break;
            }
            if (GetDlgItem(dialog, 0x5b5)) Unknown_005c2d10(unknown_color_7a7378, L"(%hs) %s", user->name, wide.Get());
            dialog = (HWND)unknown_previous_key((int)dialog);
        }
        if (!dialog) {
            ReleaseCapture();
            unknown_5159b0(0, true);
            dialog = unknown_create_dialog_5ff610(0x11e, Page_Dialog_Proc_00757d80, 0);
            if (dialog) {
                SetFocus(dialog);
                Unknown_005C2C80(unknown_color_7a7378, L"(%hs) %s", user->name, wide.Get());
                ShowWindow(dialog, 1);
            }
        }
        Set_Edit_Text(GetDlgItem(dialog, 0x718), Unknown_00757cc0(GetDlgItem(dialog, 0x53f)));
    }
    Voc_Play(Rules->IncomingMessage, 1.0f, 0x2000, 0);
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnPagedUnicode(HRESULT result, WOL::User *user, unsigned short const *message) (0x00762C20, slot 44)
// Shows a received page "(<nick>) <message>": in game as a message and in the list; otherwise in
// the chat (or list) of the open dialogs, or, with none showing chat, in a new page reply dialog.
// Then plays the incoming-message sound.

HRESULT __stdcall CChatEventSink::OnPagedUnicode(HRESULT result, WOL::User *user, unsigned short const *message)
{
    wchar_t text[256];
    if (result < 0) return S_OK;
    if (unknown_b2abd9) {
        swprintf(text, L"(%hs) %s", user->name, message);
        int color = Gadget_Color_Scheme();
        unknown_session_a3d298.Messages.Add_Message(0, 0, text, color, 0x16, Rules->MessageDelay * 900, false);
        Map.Flag_To_Redraw(false);
        Unknown_005c2d10(unknown_color_7a7378, L"(%hs) %s", user->name, message);
    } else {
        HWND dialog = Unknown_00735B10();
        while (dialog) {
            if (GetDlgItem(dialog, 0x53f)) {
                Unknown_005C2C80(unknown_color_7a7378, L"(%hs) %s", user->name, message);
                break;
            }
            if (GetDlgItem(dialog, 0x5b5)) Unknown_005c2d10(unknown_color_7a7378, L"(%hs) %s", user->name, message);
            dialog = (HWND)unknown_previous_key((int)dialog);
        }
        if (!dialog) {
            ReleaseCapture();
            unknown_5159b0(0, true);
            dialog = unknown_create_dialog_5ff610(0x11e, Page_Dialog_Proc_00757d80, 0);
            SetFocus(dialog);
            Unknown_005C2C80(unknown_color_7a7378, L"(%hs) %s", user->name, message);
            ShowWindow(dialog, 1);
        }
        Set_Edit_Text(GetDlgItem(dialog, 0x718), Unknown_00757cc0(GetDlgItem(dialog, 0x53f)));
    }
    Voc_Play(Rules->IncomingMessage, 1.0f, 0x2000, 0);
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnPageSend(HRESULT result) (0x00762E00, slot 27)
// Reports the outcome of a page into the current dialog's chat or list, or as an in-game message.

HRESULT __stdcall CChatEventSink::OnPageSend(HRESULT result)
{
    wchar_t text[128];
    if (result == S_OK) {
        Unknown_005c2d10(unknown_color_7a7360, StringTable::Load_String("TXT_PAGESENT", 0, "D:\\Ra2\\wonline.cpp", 0x1b20));
        return S_OK;
    }
    if (result == 0x40134) {
        swprintf(text, StringTable::Load_String("TXT_PAGEDISABLED", 0, "D:\\Ra2\\wonline.cpp", 0x1b24));
    } else if (result == 0x40133) {
        swprintf(text, StringTable::Load_String("TXT_PAGEUSERGONE", 0, "D:\\Ra2\\wonline.cpp", 0x1b26));
    } else {
        swprintf(text, StringTable::Load_String("TXT_PAGEFAILED", 0, "D:\\Ra2\\wonline.cpp", 0x1b28));
    }
    HWND dialog = Unknown_00735B10();
    if (dialog) {
        if (GetDlgItem(dialog, 0x53f)) {
            Unknown_005C2C80(unknown_color_7a7360, text);
            return S_OK;
        }
        if (GetDlgItem(dialog, 0x5b5)) {
            Unknown_005c2d10(unknown_color_7a7360, text);
            return S_OK;
        }
    }
    if (unknown_session_a3d298.Unknown_00672F10()) {
        int color = Gadget_Color_Scheme();
        unknown_session_a3d298.Messages.Add_Message(0, 0, text, color, 0x16, Rules->MessageDelay * 900, false);
    }
    return S_OK;
}


extern "C" int __cdecl _strnicmp(char const *,char const *,unsigned int);
extern "C" long __cdecl atol(char const *);
extern "C" long __cdecl time(long *);
int Gadget_Color_Scheme();
WideString __fastcall unknown_number_text_7710c0(int value);
extern char wol_find_requested_name_b29cd8[];
extern char wol_lobby_prefix_7fd358[];
extern char wol_lobby_prefix_7fd360[];
extern char wol_lobby_prefix_7fd368[];
extern char wol_find_string_format_7e7f2c[];
extern char wol_find_numbered_format_7fe1c0[];
extern char wol_find_ok_label_7fe1b4[];
extern char wol_find_none_label_7fe1a4[];
extern char wol_find_gone_label_7fe194[];
extern char wol_find_disabled_label_7fe180[];
extern char wol_find_failed_label_7fe170[];
extern char wol_user_found_label_7fe160[];
extern char wol_user_nochannel_label_7fe148[];
extern char wol_user_disabled_label_7fe130[];
extern char wol_user_offline_label_7fe11c[];
extern char wol_source_file_007fd62c[];
extern unsigned short wol_find_space_7d64f4[];
HRESULT __stdcall CChatEventSink::OnFind(HRESULT result,WOL::Channel *channel)
{
    unsigned int status=0;
    if(unknown_wstring_b26944.Length()!=0) {
        unsigned int *previous=0;
        unknown_names_b29848.Get_Pointer(unknown_wstring_b26944,&previous);
        if(previous) status=*previous;
        char name[64];
        name[0]=0;
        WOL::User user;
        if(channel) {
            if(channel->unknown_00==0) {
                if(!_strnicmp(channel->name,wol_lobby_prefix_7fd360,strlen(wol_lobby_prefix_7fd360))) status=7;
                else if(!_strnicmp(channel->name,wol_lobby_prefix_7fd368,strlen(wol_lobby_prefix_7fd368))) status=8;
                else status=4;
            } else {
                status=5;
                Wstring lookup_name(channel->name);
                WOL::Channel game;
                if(unknown_games_b29990.Find_Name(lookup_name,game)) {
                    if(Unknown_0073c400(&game)) status=8;
                    else if(Unknown_0073c3e0(&game)) status=7;
                }
            }
            if(unknown_wstring_b26944.Length()>0) unknown_channels_b29478.Add(unknown_wstring_b26944,*channel);
            strcpy(name,channel->name);
            if(!strncmp(name,wol_lobby_prefix_7fd358,7)) {
                int value=atol(name+strlen(wol_lobby_prefix_7fd358));
                int division=value/8;
                int remainder=value%8;
                if(division==0) sprintf(name,wol_find_string_format_7e7f2c,unknown_lobby_names_b26c94[remainder]);
                else sprintf(name,wol_find_numbered_format_7fe1c0,unknown_lobby_names_b26c94[remainder],division+1);
            }
            Unknown_005c2d10(unknown_color_7a7360,StringTable::Load_String(wol_find_ok_label_7fe1b4,0,wol_source_file_007fd62c,0x1bae),name);
        } else {
            if(unknown_wstring_b26944.Length()>0) unknown_channels_b29478.Remove_Channel(unknown_wstring_b26944);
            if(result==0x40131) {
                status=3;
                Unknown_005c2d10(unknown_color_7a7360,StringTable::Load_String(wol_find_none_label_7fe1a4,0,wol_source_file_007fd62c,0x1bb7));
            } else if(result==0x40130) {
                status=2;
                Unknown_005c2d10(unknown_color_7a7360,StringTable::Load_String(wol_find_gone_label_7fe194,0,wol_source_file_007fd62c,0x1bbb));
            } else if(result==0x40132) {
                status=1;
                Unknown_005c2d10(unknown_color_7a7360,StringTable::Load_String(wol_find_disabled_label_7fe180,0,wol_source_file_007fd62c,0x1bbf));
            } else {
                Unknown_005c2d10(unknown_color_7a7360,StringTable::Load_String(wol_find_failed_label_7fe170,0,wol_source_file_007fd62c,0x1bc2));
            }
        }
        if(unknown_wstring_b26944.Length()>0) {
            unknown_names_b29848.Add(unknown_wstring_b26944,status);
            unknown_wstring_b26944.Release();
            HWND dialog=WS_Find_Dialog_00735940(0x116);
            if(dialog) Unknown_00755180(dialog);
        }
        if(unknown_wstring_b26944.Length()>0) {
            if((unsigned long)(time(0)-unknown_b2a7f8)<5) return 0;
            unknown_wstring_b26944.Release();
        }
        if(unknown_strings_b29838.Count!=0) {
            if(unknown_strings_b29838.Count>0) {
                unknown_strings_b29838.Get(unknown_wstring_b26944,0);
                unknown_strings_b29838.Remove_Inline(0);
            }
            unknown_b2a7f8=time(0);
            memset(&user,0,sizeof(user));
            strcpy(user.name,unknown_wstring_b26944.Get());
            unknown_chat_a72cfc->unknown_slot24(&user);
        }
        return 0;
    }
    else {
    wchar_t text[128];
    if(channel) {
        if(channel->unknown_00==0 && !strncmp(channel->name,wol_lobby_prefix_7fd358,strlen(wol_lobby_prefix_7fd358))) {
            int value=atol(channel->name+strlen(wol_lobby_prefix_7fd358));
            int division=value/8;
            int remainder=value%8;
            WideString lobby((wchar_t const *)unknown_lobby_names_b26c94[remainder]);
            if(division!=0) {
                lobby+=wol_find_space_7d64f4;
                lobby+=unknown_number_text_7710c0(division+1);
            }
            wchar_t const *user_name=Narrow_To_Wide_006fa180(wol_find_requested_name_b29cd8);
            wchar_t const *lobby_name=lobby.Get();
            swprintf(text,StringTable::Load_String(wol_user_found_label_7fe160,0,wol_source_file_007fd62c,0x1b5b),user_name,lobby_name);
        } else {
            wchar_t const *user_name=Narrow_To_Wide_006fa180(wol_find_requested_name_b29cd8);
            wchar_t const *channel_name=Narrow_To_Wide_006fa180(channel->name);
            swprintf(text,StringTable::Load_String(wol_user_found_label_7fe160,0,wol_source_file_007fd62c,0x1b5d),user_name,channel_name);
        }
    } else if(result==0x40130) {
        swprintf(text,StringTable::Load_String(wol_user_offline_label_7fe11c,0,wol_source_file_007fd62c,0x1b64),Narrow_To_Wide_006fa180(wol_find_requested_name_b29cd8));
    } else if(result==0x40131) {
        swprintf(text,StringTable::Load_String(wol_user_nochannel_label_7fe148,0,wol_source_file_007fd62c,0x1b66),Narrow_To_Wide_006fa180(wol_find_requested_name_b29cd8));
    } else if(result==0x40132) {
        swprintf(text,StringTable::Load_String(wol_user_disabled_label_7fe130,0,wol_source_file_007fd62c,0x1b68),Narrow_To_Wide_006fa180(wol_find_requested_name_b29cd8));
    } else {
        swprintf(text,StringTable::Load_String(wol_user_offline_label_7fe11c,0,wol_source_file_007fd62c,0x1b6a),Narrow_To_Wide_006fa180(wol_find_requested_name_b29cd8));
    }
    HWND dialog=Unknown_00735B10();
    if(dialog) {
        if(GetDlgItem(dialog,0x53f)) {
            Unknown_005C2C80(unknown_color_7a7360,text);
            return 0;
        }
        if(GetDlgItem(dialog,0x5b5)) {
            Unknown_005c2d10(unknown_color_7a7360,text);
            return 0;
        }
    }
    if(unknown_session_a3d298.Unknown_00672F10()) {
        int color=Gadget_Color_Scheme();
        unknown_session_a3d298.Messages.Add_Message(0,0,text,color,0x16,(int)(Rules->MessageDelay*900.0),false);
    }
    wol_find_requested_name_b29cd8[0]=0;
    return 0;
    }
}


// 0x00763ab0: function not yet identified
//
// Body: 47 bytes (frozen inventory bound).
// Referenced from 2 sites (data).
// Ghidra prototype: undefined4 unknown_print_message_00763ab0(undefined4 param_1,int param_2,undefined4 param_3)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00763ab0 {

extern void  Print_Chat_Line_005c2c80(int, int, int);
extern "C" unsigned int unknown_color_007a7360;
extern "C" char g_PlayingNetGame_00b2abd9;
extern "C" unsigned short u______hs_007fe1c8[];

unsigned int __stdcall unknown_print_message_00763ab0(unsigned int sink, int code, unsigned int text)
{
    if ((g_PlayingNetGame_00b2abd9 == '\0') && (code >= 0)) {
        Print_Chat_Line_005c2c80((int)(unknown_color_007a7360), (int)(u______hs_007fe1c8), (int)(text));
    }
    return 0;
}

}  // namespace seed_00763ab0


// HRESULT __stdcall CChatEventSink::OnNetStatus(HRESULT result) (0x00763AE0, slot 19)
// While disconnecting, records the status and signals; on a lost connection reports it (in game
// as a message, otherwise in a popup) and clears the current and pending channels.

HRESULT __stdcall CChatEventSink::OnNetStatus(HRESULT result)
{
    if (unknown_b2abd8) {
        SetEvent(unknown_b2651c);
        unknown_net_status_b2ac00 = result;
        return S_OK;
    }
    if (result == 0x4012f) {
        if (unknown_b2abd9) {
            if (!unknown_b2abda) {
                wchar_t text[256];
                text[0] = 0;
                swprintf(text, StringTable::Load_String("TXT_WOL_CONNLOST", 0, "D:\\Ra2\\wonline.cpp", 0x1c09));
                int color = Gadget_Color_Scheme();
                unknown_session_a3d298.Messages.Add_Message(0, 0, text, color, 0x16, Rules->MessageDelay * 900, false);
                Map.Flag_To_Redraw(false);
                unknown_b2abda = true;
                unknown_b2abe8 = 0;
                memset(&unknown_channel_b299c0, 0, sizeof(unknown_channel_b299c0));
                memset(&unknown_pending_channel_b26a60, 0, sizeof(unknown_pending_channel_b26a60));
                Debug_Printf("NetStatus disconnect ingame\n");
            }
        } else {
            if (WS_Find_Dialog_00735940(0x113)) {
                Popup_005c3230(StringTable::Load_String("TXT_YOURE_DISCON", 0, "D:\\Ra2\\wonline.cpp", 0x1c1b), 0, Unknown_0073c120, 0);
            }
            unknown_b2abd8 = true;
            unknown_b2abda = true;
            SetEvent(unknown_b264f8);
            unknown_b2abe8 = 0;
            memset(&unknown_channel_b299c0, 0, sizeof(unknown_channel_b299c0));
            memset(&unknown_pending_channel_b26a60, 0, sizeof(unknown_pending_channel_b26a60));
            Debug_Printf("NetStatus disconnect not in game \n");
        }
    }
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnPrivateAction(HRESULT result, WOL::User *user, char const *action) (0x00764360, slot 13)
// Shows "<nick> <action>" for a visible action from another user.

HRESULT __stdcall CChatEventSink::OnPrivateAction(HRESULT result, WOL::User *user, char const *action)
{
    if (!unknown_b2abd9 && result >= 0) {
        bool show;
        {
            // Unused copy of the text (hypothesis: left over from a removed check).
            Wstring copy(action);
            if (!user) {
                show = false;
            } else if (user->flags & 1) {
                show = true;
            } else if (WS_Find_Dialog_00735940(0xC2)) {
                show = true;
            } else if (WS_Find_Dialog_00735940(0xC9)) {
                show = true;
            } else {
                show = unknown_b2a7e8 != 0;
            }
        }
        if (show) {
            WideString text = Wstring_To_Wide(action);
            if (_stricmp(unknown_name_a3d3a0, user->name)) {
                Unknown_005C2C80(unknown_color_7a736c, L"%hs %s", user->name, text.Get());
            }
        }
    }
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnPrivateUnicodeAction(HRESULT result, WOL::User *user, unsigned short const *action) (0x00764440, slot 42)
// Shows "<nick> <action>" for a visible wide-text action from another user.

HRESULT __stdcall CChatEventSink::OnPrivateUnicodeAction(HRESULT result, WOL::User *user, unsigned short const *action)
{
    if (!unknown_b2abd9 && result >= 0) {
        bool show;
        {
            WideString text(action);
            if (!user) {
                show = false;
            } else if (user->flags & 1) {
                show = true;
            } else if (WS_Find_Dialog_00735940(0xC2)) {
                show = true;
            } else if (WS_Find_Dialog_00735940(0xC9)) {
                show = true;
            } else {
                show = (text.Is_Narrow() ? unknown_wol_display_latin_b2a7e8 : unknown_wol_display_asian_b2a804) != 0;
            }
        }
        if (show) {
            if (_stricmp(unknown_name_a3d3a0, user->name)) {
                Unknown_005C2C80(unknown_color_7a736c, L"%hs %s", user->name, action);
            }
        }
    }
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnPublicAction(HRESULT result, WOL::Channel *channel, WOL::User *user, char const *action) (0x00764500, slot 14)
// Shows "<nick> <action>" for a visible action from another user.

HRESULT __stdcall CChatEventSink::OnPublicAction(HRESULT result, WOL::Channel *channel, WOL::User *user, char const *action)
{
    if (!unknown_b2abd9 && result >= 0) {
        bool show;
        {
            // Unused copy of the text (hypothesis: left over from a removed check).
            Wstring copy(action);
            if (!user) {
                show = false;
            } else if (user->flags & 1) {
                show = true;
            } else if (WS_Find_Dialog_00735940(0xC2)) {
                show = true;
            } else if (WS_Find_Dialog_00735940(0xC9)) {
                show = true;
            } else {
                show = unknown_b2a7e8 != 0;
            }
        }
        if (show) {
            WideString text = Wstring_To_Wide(action);
            if (_stricmp(unknown_name_a3d3a0, user->name)) {
                Unknown_005C2C80(unknown_color_7a7370, L"%hs %s", user->name, text.Get());
            }
        }
    }
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnPublicUnicodeAction(HRESULT result, WOL::Channel *channel, WOL::User *user, unsigned short const *action) (0x007645E0, slot 43)
// Shows "<nick> <action>" for a visible wide-text action from another user.

HRESULT __stdcall CChatEventSink::OnPublicUnicodeAction(HRESULT result, WOL::Channel *channel, WOL::User *user, unsigned short const *action)
{
    if (!unknown_b2abd9 && result >= 0) {
        bool show;
        {
            WideString text(action);
            if (!user) {
                show = false;
            } else if (user->flags & 1) {
                show = true;
            } else if (WS_Find_Dialog_00735940(0xC2)) {
                show = true;
            } else if (WS_Find_Dialog_00735940(0xC9)) {
                show = true;
            } else {
                show = (text.Is_Narrow() ? unknown_wol_display_latin_b2a7e8 : unknown_wol_display_asian_b2a804) != 0;
            }
        }
        if (show) {
            if (_stricmp(unknown_name_a3d3a0, user->name)) {
                Unknown_005C2C80(unknown_color_7a7370, L"%hs %s", user->name, action);
            }
        }
    }
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnChannelTopic(HRESULT result, WOL::Channel *channel, char const *topic) (0x007646A0, slot 12)
// Ignored.

HRESULT __stdcall CChatEventSink::OnChannelTopic(HRESULT result, WOL::Channel *channel, char const *topic)
{
    return S_OK;
}


// 0x007646b0: function not yet identified
//
// Body: 5 bytes (frozen inventory bound).
// Referenced from 2 sites (data).
// Ghidra prototype: undefined4 unknown_return_zero_007646b0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_007646b0 {

unsigned int __stdcall unknown_return_zero_007646b0(int unused_0, int unused_1, int unused_2)
{
    return 0;
}

}  // namespace seed_007646b0


// HRESULT __stdcall CChatEventSink::OnServerBannedYou(HRESULT result, long banned_till) (0x007646C0, slot 29)
// Tells the player until when the server bans them.

HRESULT __stdcall CChatEventSink::OnServerBannedYou(HRESULT result, long banned_till)
{
    if (!unknown_b2abd9 && result == S_OK) {
        wchar_t text[200];
        swprintf(text, StringTable::Load_String("TXT_BANNEDTILL", 0, "D:\\Ra2\\wonline.cpp", 0x1d54), ctime(&banned_till));
        Popup_005c3230(text, 0, Unknown_0073c120, 0);
    }
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnChannelBan(HRESULT result, char const *name, int banned) (0x00764730, slot 31)
// Reports that a user was banned from or unbanned in the channel.

HRESULT __stdcall CChatEventSink::OnChannelBan(HRESULT result, char const *name, int banned)
{
    if (!unknown_b2abd9) {
        if (banned) {
            Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_CHAN_BAN", 0, "D:\\Ra2\\wonline.cpp", 0x1d61), Narrow_To_Wide_006fa180(name));
        } else {
            Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_CHAN_UNBAN", 0, "D:\\Ra2\\wonline.cpp", 0x1d64), Narrow_To_Wide_006fa180(name));
        }
    }
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnUserFlags(HRESULT result, char const *name, unsigned int flags, unsigned int mask) (0x007647B0, slot 30)
// Updates the flags of the named user in the user list (keeping the bits in mask, adding flags),
// re-sorting the entry, then refreshes the user list display.

HRESULT __stdcall CChatEventSink::OnUserFlags(HRESULT result, char const *name, unsigned int flags, unsigned int mask)
{
    if (result == 0) {
        for (int i = 0; i < unknown_users_b2a728.Count; ++i) {
            WOL::User user;
            if (unknown_users_b2a728.Get_Copy(user, i) && !_stricmp(user.name, name)) {
                user.flags = (user.flags & mask) | flags;
                unknown_users_b2a728.Remove(i);
                unknown_users_b2a728.Add_Sorted(user);
            }
        }
        Unknown_0073FEE0(0, false);
    }
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnUserKick(HRESULT result, WOL::Channel *channel, WOL::User *kicked, WOL::User *kicker) (0x00764B00, slot 24)
// Reports a kick: to the kicked local user as a popup (or in the game lobby's list), to others in
// the chat unless the game slots are full; then drops the kicked user and refreshes the lists.

HRESULT __stdcall CChatEventSink::OnUserKick(HRESULT result, WOL::Channel *channel, WOL::User *kicked, WOL::User *kicker)
{
    if (unknown_b2abd9) return S_OK;
    if (result < 0) return S_OK;
    memset(unknown_kick_channel_b2b3c0, 0, sizeof(unknown_kick_channel_b2b3c0));
    char const *name = channel->name;
    if (!_strnicmp(name, "Lob_33_", strlen("Lob_33_"))) {
        int number = atol(channel->name + strlen("Lob_33_"));
        int group = number / 8;
        int index = number % 8;
        if (group == 0) {
            sprintf(unknown_kick_channel_b2b3c0, "%s", unknown_lobby_names_b26c94[index]);
        } else {
            sprintf(unknown_kick_channel_b2b3c0, "%s %d", unknown_lobby_names_b26c94[index], group + 1);
        }
    } else {
        sprintf(unknown_kick_channel_b2b3c0, "%s", name);
    }
    if (kicked->flags & 0x8000) {
        WideString text;
        if (unknown_b299c0 && Channel_Full_005c9510()) {
            text = StringTable::Load_String("TXT_CHANNEL_FULL", 0, "D:\\Ra2\\wonline.cpp", 0x1db7);
        } else {
            wchar_t buffer[1024];
            memset(buffer, 0, sizeof(buffer));
            swprintf(buffer, StringTable::Load_String("TXT_YOURE_KICKED", 0, "D:\\Ra2\\wonline.cpp", 0x1dbb), Narrow_To_Wide_006fa180(name), Narrow_To_Wide_006fa180(kicker->name));
            text = buffer;
        }
        if (!unknown_b299c0) {
            Popup_005c3230(text.Get(), 0, Unknown_0073c120, 0);
            ++unknown_b2abec;
        } else {
            HWND dialog = (HWND)WS_Find_Dialog_00735940(0x113);
            if (dialog) _DrawMessage_005c2da0(unknown_color_7a7360, text.Get(), GetDlgItem(dialog, 0x53f));
        }
    } else if (!Slots_Filled_005c9440()) {
        Unknown_005C2C80(unknown_color_7a7360, StringTable::Load_String("TXT_USER_KICKED", 0, "D:\\Ra2\\wonline.cpp", 0x1dd3), Narrow_To_Wide_006fa180(kicked->name), Narrow_To_Wide_006fa180(name), Narrow_To_Wide_006fa180(kicker->name));
    }
    Unknown_00741750(kicked);
    Unknown_0073FEE0(0, false);
    Unknown_0073f8f0();
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnUserLocale(HRESULT result, WOL::User *users) (0x00764D70, slot 33)
// Records each user's locale by lower-case name; for the local player also stores it in the
// profile (saved to the registry when profiles are kept there) and signals the waiting request.
// The local-player test and the stored locale read the first user of the list, not the current one.

HRESULT __stdcall CChatEventSink::OnUserLocale(HRESULT result, WOL::User *users)
{
    bool local = false;
    if (result < 0) {
        Debug_Printf("OnUserLocale callback failed!\n");
        return S_OK;
    }
    WOL::User *user = users;
    while (user) {
        Wstring name;
        name.Set(user->name);
        name.To_Lower();
        int old_locale;
        unknown_channel_names_b293a0.Remove_007675e0(name, &old_locale);
        int locale = user->locale;
        WolChannelNameSet::Node *node = new WolChannelNameSet::Node;
        node->Key = name;
        node->Value = locale;
        node->Next = 0;
        unsigned int bucket = unknown_channel_names_b293a0.Key_Hash_007679a0(name);
        WolChannelNameSet::Node *head = unknown_channel_names_b293a0.Buckets[bucket];
        unknown_channel_names_b293a0.Buckets[bucket] = node;
        if (head) node->Next = head;
        ++unknown_channel_names_b293a0.Count;
        if ((float)unknown_channel_names_b293a0.Count / (float)unknown_channel_names_b293a0.Get_Size() >= unknown_channel_names_b293a0.GrowLimit) {
            unknown_channel_names_b293a0.Grow_00767900();
        }
        if (!_stricmp(users->name, unknown_wstring_b26cb8.Get())) {
            WolRegistryOptions options;
            options.Read_004ff200();
            rc_ptr<PlayerProfile> profile;
            if (options.unknown_004ff2c0()) {
                profile = PlayerProfile::Load_00738880(unknown_wstring_b26cb8);
            } else {
                profile.Set(unknown_profile_b28cc4);
            }
            profile->Set_Locale_00739950(users->locale, 2);
            if (options.unknown_004ff2c0()) profile->Save_007395d0(unknown_wstring_b26cb8);
            Debug_Printf("OnUserLocale callback - Locale is %d\n", users->locale);
            local = true;
        }
        user = user->next;
    }
    if (local) SetEvent(unknown_b26530);
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnSetLocale(HRESULT result, int locale) (0x00764F80, slot 35)
// Records the local player's new locale under the lower-case nickname (replacing an older entry).

HRESULT __stdcall CChatEventSink::OnSetLocale(HRESULT result, int locale)
{
    Wstring name;
    char nick[64];
    strcpy(nick, unknown_name_a3d3a0);
    name.Set(nick);
    name.To_Lower();
    if (unknown_channel_names_b293a0.Contains(name)) unknown_channel_names_b293a0.Remove(name);
    unknown_channel_names_b293a0.Add(name, locale);
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnUserTeam(HRESULT result, WOL::User *users) (0x007651D0, slot 34)
// Records each user's team by name (replacing an older entry), then continues the pending queries.

HRESULT __stdcall CChatEventSink::OnUserTeam(HRESULT result, WOL::User *users)
{
    if (result >= 0) {
        WOL::User *user = users;
        Wstring name;
        while (user) {
            name.Set(user->name);
            if (unknown_known_names_b294b8.Contains(name)) unknown_known_names_b294b8.Remove(name);
            unknown_known_names_b294b8.Add(name, user->team);
            user = user->next;
        }
        Unknown_0073FEE0(0, false);
        return S_OK;
    }
    Debug_Printf("OnUserTeam FAILED!\n");
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnSetTeam(HRESULT result, int team) (0x00765430, slot 36)
// Stores the local player's new team and records it under the lower-case nickname.
// On failure it prints the team, not the result (original behaviour).

HRESULT __stdcall CChatEventSink::OnSetTeam(HRESULT result, int team)
{
    if (result == S_OK) {
        unknown_a3d3bc = team;
        Wstring name;
        char nick[64];
        strcpy(nick, unknown_name_a3d3a0);
        name.Set(nick);
        name.To_Lower();
        if (unknown_known_names_b294b8.Contains(name)) unknown_known_names_b294b8.Remove(name);
        unknown_known_names_b294b8.Add(name, team);
        return S_OK;
    }
    Debug_Printf("OnSetTeam FAILED: %d\n", team);
    return S_OK;
}


// HRESULT __stdcall CChatEventSink::OnBuddyList(HRESULT result, WOL::User *buddies) (0x007656B0, slot 37)
// Records each acceptable buddy name in the three buddy lists and sends the next pending request
// (at most one every five seconds), then refreshes the buddy dialogs.

HRESULT __stdcall CChatEventSink::OnBuddyList(HRESULT result, WOL::User *buddies)
{
    if (result >= 0) {
        for (WOL::User *buddy = buddies; buddy; buddy = buddy->next) {
            Wstring name(buddy->name);
            if (Unknown_00756790(name)) {
                Wstring copy(name);
                unknown_strings_b26920.Insert(copy, unknown_strings_b26920.Count);
                unknown_strings_b29838.Insert(copy, unknown_strings_b29838.Count);
                unknown_strings_b2a7d8.Add(copy);
                if (unknown_wstring_b26944.Length() > 0) {
                    if (time(0) - unknown_b2a7f8 < 5) continue;
                    unknown_wstring_b26944.Release();
                }
                if (unknown_strings_b29838.Count) {
                    if (unknown_strings_b29838.Get_007662c0(unknown_wstring_b26944, 0)) unknown_strings_b29838.Remove_007661f0(0);
                    unknown_b2a7f8 = time(0);
                    WOL::User user;
                    memset(&user, 0, sizeof(user));
                    strcpy(user.name, unknown_wstring_b26944.Get());
                    unknown_chat_a72cfc->unknown_slot24(&user);
                }
            }
        }
        Unknown_00757380();
        void *list = WS_Find_Dialog_00735940(0xc4);
        if (list) {
            if (!(unknown_b2abe0 &= ~0x200)) Unknown_007357E0(list, 0);
        }
        HWND current = Unknown_00735B10();
        HWND edit = GetDlgItem(current, 0x540);
        if (edit) SetFocus(edit);
        HWND dialog = (HWND)WS_Find_Dialog_00735940(0x116);
        if (dialog) {
            Unknown_00755180(dialog);
            KillTimer(dialog, 0x704);
            EnableWindow(GetDlgItem(dialog, 0x704), 1);
        }
    }
    return S_OK;
}


HRESULT __stdcall CChatEventSink::OnServerTime(HRESULT result,long server_time)
{
    return 0;
}


HRESULT __stdcall CChatEventSink::OnInsiderStatus(HRESULT result,WOL::User *users)
{
    return 0;
}


HRESULT __stdcall CChatEventSink::OnSetLocalIP(HRESULT result,char const *message)
{
    return 0;
}


class Leaf_765930 { public: unsigned int get(unsigned int a1, unsigned int a2, unsigned int a3); };
unsigned int Leaf_765930::get(unsigned int a1, unsigned int a2, unsigned int a3) { unsigned int self = (unsigned int)this; unsigned int result; unsigned int spare; result = 0; return result; }


class Leaf_765940 { public: unsigned int get(unsigned int a1, unsigned int a2, unsigned int a3); };
unsigned int Leaf_765940::get(unsigned int a1, unsigned int a2, unsigned int a3) { unsigned int self = (unsigned int)this; unsigned int result; unsigned int spare; result = 0; return result; }


// HRESULT __stdcall AtlModuleTerm(_ATL_MODULE *pM) (0x00765990)
// ATL: releases the class factories, calls each ObjectMain(false), runs the term functions and destroys the heaps.

extern "C" HRESULT __stdcall AtlModuleTerm(_ATL_MODULE *pM)
{
    if (pM == 0) return E_INVALIDARG;
    if (pM->m_pObjMap != 0) {
        _ATL_OBJMAP_ENTRY *pEntry = pM->m_pObjMap;
        while (pEntry->pclsid != 0) {
            if (pEntry->pCF != 0) pEntry->pCF->Release();
            pEntry->pCF = 0;
            pEntry->pfnObjectMain(false);
            pEntry = _NextObjectMapEntry(pM, pEntry);
        }
    }
    DeleteCriticalSection(&pM->m_csTypeInfoHolder);
    DeleteCriticalSection(&pM->m_csWindowCreate);
    DeleteCriticalSection(&pM->m_csObjMap);
    _ATL_TERMFUNC_ELEM *pElem = pM->m_pTermFuncs;
    _ATL_TERMFUNC_ELEM *pNext = 0;
    while (pElem != 0) {
        pElem->pFunc(pElem->dw);
        pNext = pElem->pNext;
        delete pElem;
        pElem = pNext;
    }
    if (pM->m_hHeap != 0 && pM->m_bDestroyHeap) {
        if (pM->m_phHeaps != 0) {
            for (unsigned long i = 0; i <= pM->m_dwHeaps; i++) HeapDestroy(pM->m_phHeaps[i]);
        }
        HeapDestroy(pM->m_hHeap);
    }
    return S_OK;
}


// The real inline WonlineStringDialogControl destructor is defined with its
// Wstring member in wol_sink.hpp. Its vtable emits this deleting destructor.


// WonlineStringDialogControl::WonlineStringDialogControl() (0x00765AA0, 32 B): the class's inline constructor;
// CreateGameDialogControl's constructor in Host_Game_007459a0 refuses it, which emits this body.


// CreateGameDialogControl deleting destructor (0x00765AC0, slot 0 of vtable 0x007AF94C): compiler generated from the
// class's inline destructor (shared/create_game_dialog_control.hpp).


// ATL::CComPtr<IConnectionPointContainer>::~CComPtr (0x00765B00), the template in shared/com_api.hpp.
// Emitted by the refused destructor site in Startup_Chat_00741300's second AtlAdvise; no source of its own.


// ATL::CComPtr<IConnectionPoint>::~CComPtr (0x00765B10), the template in shared/com_api.hpp.
// Emitted by the refused destructor site in Startup_Chat_00741300's second AtlAdvise; no source of its own.


bool WolChannelNameSet::Add_00766070(Wstring const &key, int const &value)
{
    Node *node = new Node;
    node->Key = key;
    node->Value = value;
    node->Next = 0;
    unsigned int bucket = Key_Hash(key);
    Node *head = Buckets[bucket];
    Buckets[bucket] = node;
    if (head) node->Next = head;
    ++Count;
    if ((float)Count / (float)Get_Size() >= GrowLimit) Grow_Table();
    return true;
}


// void WolArrayList<Wstring>::Clear() (0x007661C0): the inline in shared/wol_array_list.hpp;
// Reset_Buddy_Name_List_00757530 refuses it inside the dialog proc 0x00758750, which emits this body.


// bool WolArrayList<Wstring>::Remove(int index) (0x007661F0): the inline in shared/wol_array_list.hpp;
// Remove_Head inside the dialog proc 0x00758750 refuses it, which emits this body. Older rows call it by address.


// bool WolArrayList<Wstring>::Get(Wstring &text, int index) (0x007662C0): the inline in shared/wol_array_list.hpp;
// Reset_Buddy_State_00757720 refuses it two inline levels down, which emits this body.


// void WolNameTable::Clear_007662f0() (0x007662F0): inline, defined in Reset_Buddy_State_00757720.cpp.
// The dialog proc 0x00758750 refuses it, which emits this body.


// void WolChannelDictionary::Clear_00766400() (0x00766400): inline, defined in Reset_Buddy_State_00757720.cpp.
// The dialog proc 0x00758750 refuses it, which emits this body.


// WolDictionary<Wstring, WOL::Ladder>::Node scalar deleting destructor (0x00766780): compiler generated;
// Reset_WOL_Globals_0073e8d0 refuses `delete old` in WolLadderCache::Clear, which emits this body.


// WolDictionary<Wstring, WOL::Channel>::Node scalar deleting destructor (0x007667E0): compiler generated;
// Reset_WOL_Globals_0073e8d0 refuses `delete old` in WolChannelDictionary::Clear_00766400, which emits this body.


extern "C" unsigned char vtable_766800[]; extern "C" void __cdecl release_buffer_766800(void*); extern "C" void __cdecl release_object_766800(void*);
class Leaf_766800 { public: void* get(unsigned int flags); };
void* Leaf_766800::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_766800; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_766800((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_766800(this); return this; }


extern "C" unsigned char vtable_766850[]; extern "C" void __cdecl release_buffer_766850(void*); extern "C" void __cdecl release_object_766850(void*);
class Leaf_766850 { public: void* get(unsigned int flags); };
void* Leaf_766850::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_766850; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_766850((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_766850(this); return this; }


// ATL::CComObject<CDownloadEventSink>::CComObject(void *) (0x007668A0), the inline constructor in
// shared/wol_sink.hpp. Emitted by the refused new in Startup_Chat_00741300's CreateInstance; no source of its own.


HRESULT __stdcall CDownloadEventSink::OnStatusUpdate(int status)
{
    wchar_t text[200];
    text[0]=0;
    switch(status) {
    case 2:
        swprintf(text,StringTable::Load_String("TXT_CONNECTING",0,"D:\\Ra2\\wonline.cpp",0xfa5));
        break;
    case 4:
        swprintf(text,StringTable::Load_String("TXT_FINDING_PATCH",0,"D:\\Ra2\\wonline.cpp",0xfa9));
        break;
    }
    HWND label=GetDlgItem(Unknown_00735B10(),0x5c8);
    SendMessageA(label,0x4b2,0,(LPARAM)text);
    return 0;
}


// HRESULT __stdcall CDownloadEventSink::OnEnd() (0x007668D0, slot 3)
// Tells the current dialog the download finished (message 0x5F4).

HRESULT __stdcall CDownloadEventSink::OnEnd()
{
    SendMessageA(Unknown_00735B10(), 0x5F4, 0, 0);
    return S_OK;
}


// HRESULT __stdcall CDownloadEventSink::OnError(int error) (0x007668F0, slot 4)
// Tells the current dialog the download failed (message 0x5F5, the error).

HRESULT __stdcall CDownloadEventSink::OnError(int error)
{
    SendMessageA(Unknown_00735B10(), 0x5F5, 0, error);
    return S_OK;
}


// HRESULT __stdcall CDownloadEventSink::OnProgressUpdate(int bytes_read, int total_size, int time_taken, int time_left) (0x00766910, slot 5)
// Moves the progress bar (control 0x5C6) and, at most once a second,
// shows the byte counts (and the time left when known) in control 0x5C8.

HRESULT __stdcall CDownloadEventSink::OnProgressUpdate(int bytes_read, int total_size, int time_taken, int time_left)
{
    wchar_t text[200];
    text[0] = 0;
    SendDlgItemMessageA(Unknown_00735B10(), 0x5C6, 0x402, (unsigned)(bytes_read * 100) / (unsigned)total_size, 0);
    if (time_left > 0) {
        swprintf(text, StringTable::Load_String("TXT_BYTES_W_TIME", 0, "D:\\Ra2\\wonline.cpp", 3981), bytes_read, total_size, time_left);
    } else {
        swprintf(text, StringTable::Load_String("TXT_BYTES_WO_TIME", 0, "D:\\Ra2\\wonline.cpp", 3986), bytes_read, total_size);
    }
    if (time(0) - unknown_b2acc8) {
        HWND window = GetDlgItem(Unknown_00735B10(), 0x5C8);
        SendMessageA(window, 0x4B2, 0, (LPARAM)text);
        unknown_b2acc8 = time(0);
    }
    return S_OK;
}


// HRESULT __stdcall CDownloadEventSink::OnQueryResume() (0x00766AA0, slot 6)
// Always S_OK.

HRESULT __stdcall CDownloadEventSink::OnQueryResume()
{
    return S_OK;
}


namespace ATL {
template<> unsigned long __stdcall CComObject<CDownloadEventSink>::Release()
{
    unsigned long count = InterlockedDecrement(&m_dwRef);
    if (count == 0) delete this;
    return count;
}
}


namespace ATL {
extern InterfaceMapEntry const CDownloadEventSinkInterfaceMap[]; // @ref ?CDownloadEventSinkInterfaceMap@ATL@@=007af708
template<> HRESULT __stdcall CComObject<CDownloadEventSink>::QueryInterface(GUID const &iid, void **object)
{
    return InternalQueryInterface(this, CDownloadEventSinkInterfaceMap, iid, object);
}
}


bool WolChannelArray::Shrink_00766dc0()
{
    if (Count * 3 > Capacity) return false;
    int capacity = Capacity / 2;
    if (capacity < 10) capacity = 10;
    if (capacity >= Capacity) return false;
    WOL::Channel *items = (WOL::Channel *)operator new(capacity * sizeof(WOL::Channel));
    if (Items) memcpy(items, Items, Count * sizeof(WOL::Channel));
    operator delete(Items);
    Items = items;
    Capacity = capacity;
    return true;
}


bool WolIntArray::Insert_00766f90(int const &value, int index)
{
    if (index > Count) index = Count;
    if (index >= Capacity) Grow();
    Grow();
    if (index < Count) memmove(Items + index + 1, Items + index, (Count - index) * sizeof(int));
    new (Items + index) int(value);
    ++Count;
    return true;
}


void WolLadderCache::Grow_007674b0()
{
    if (ResizeLocked != true) {
        int old_count = BucketCount;
        Node **old_buckets = Buckets;
        BucketCount *= 2;
        ++Bits;
        Buckets = new Node *[BucketCount];
        memset(Buckets, 0, BucketCount * sizeof(Node *));
        if (old_count > 0) {
            Node **bucket = old_buckets;
            int remaining = old_count;
            do {
                Node *node = *bucket;
                while (node) {
                    unsigned int hash = Hash(node->Key);
                    unsigned int index = ((1u << Bits) - 1) & hash;
                    Node *head = Buckets[index];
                    Buckets[index] = node;
                    Node *next = node->Next;
                    node->Next = head;
                    node = next;
                }
                ++bucket;
            } while (--remaining);
        }
        delete [] old_buckets;
    }
}


bool WolChannelNameSet::Remove_007675e0(Wstring const &key, int *old_value)
{
    if (Count == 0) return false;
    float load = (float)(Count - 1);
    load /= (float)BucketCount;
    unsigned int bucket = Key_Hash(key);
    Node *head = Buckets[bucket];
    Node *previous = head;
    if (!head) return false;
    if (head->Key == key) {
        *old_value = head->Value;
        Node *removed = Buckets[bucket];
        Node *next = removed->Next;
        delete removed;
        Buckets[bucket] = next;
        --Count;
        if (load <= ShrinkLimit) Shrink_Table();
        return true;
    }
    Node *node = head->Next;
    bool removed = false;
    while (node) {
        if (node->Key == key) {
            *old_value = node->Value;
            previous->Next = node->Next;
            --Count;
            delete node;
            removed = true;
            break;
        }
        previous = node;
        node = node->Next;
    }
    if (load <= ShrinkLimit) Shrink_Table();
    return removed;
}


void WolChannelNameSet::Grow_00767900()
{
    Grow_Table();
}


// void WolDictionary<Wstring,unsigned int>::Shrink_Table() (0x00767C20): the wlib template in shared/wol_dictionary.hpp;
// Reset_Buddy_State_00757720 refuses it inside Clear_007662f0, which emits this body. Older rows call it by address.


bool WolChannelDictionary::Remove_00767d60(Wstring const &key, WOL::Channel *old_value)
{
    if (Count == 0) return false;
    float load = (float)(Count - 1);
    load /= (float)BucketCount;
    unsigned int bucket = Key_Hash(key);
    Node *head = Buckets[bucket];
    Node *previous = head;
    if (!head) return false;
    if (head->Key == key) {
        *old_value = head->Value;
        Node *removed = Buckets[bucket];
        Node *next = removed->Next;
        delete removed;
        Buckets[bucket] = next;
        --Count;
        if (load <= ShrinkLimit) Shrink_Table();
        return true;
    }
    Node *node = head->Next;
    bool removed = false;
    while (node) {
        if (node->Key == key) {
            *old_value = node->Value;
            previous->Next = node->Next;
            --Count;
            delete node;
            removed = true;
            break;
        }
        previous = node;
        node = node->Next;
    }
    if (load <= ShrinkLimit) Shrink_Table();
    return removed;
}


// void WolDictionary<Wstring,WOL::Channel>::Shrink_Table() (0x00768010): the wlib template in shared/wol_dictionary.hpp;
// Reset_Buddy_State_00757720 refuses it inside Clear_00766400, which emits this body. Older rows call it by address.


extern "C" unsigned char table_7680c0[]; extern "C" void* __cdecl storage_7680c0(unsigned int);
class Storage_7680c0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_7680c0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_7680c0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_7680c0(requested << 2); owns=1; } return this; }


bool WolServerArray::Grow_00768110()
{
    if (Count < Capacity) return false;
    int capacity = Count * 2;
    if (capacity < 10) capacity = 10;
    WOL::Server *items = (WOL::Server *)operator new(capacity * sizeof(WOL::Server));
    memset(items, 0, capacity * sizeof(WOL::Server));
    if (Items) memcpy(items, Items, Count * sizeof(WOL::Server));
    operator delete(Items);
    Items = items;
    Capacity = capacity;
    return true;
}


bool WolIntArray::Remove_007681a0(int index)
{
    if (Count == 0) return false;
    if (index < 0) index = 0;
    if (index >= Count) index = Count - 1;
    memmove(Items + index, Items + index + 1, (Count - (index + 1)) * sizeof(int));
    --Count;
    Shrink();
    return true;
}


bool WolIntArray::Grow_00768290()
{
    return Grow();
}
