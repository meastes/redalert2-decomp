// options.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/options.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#define INDEX_DECLARE_FETCH_ID_BY_POSITION
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/options.hpp"
#include "shared/debug_printf.hpp"
#include "shared/theme.hpp"
#include "shared/audio.hpp"
#include "shared/rules.hpp"
#include "shared/techno.hpp"
#include "shared/vox_class.hpp"
#include "shared/hicolor.hpp"
#include "shared/map_view.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/string_table.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/ccini_class.hpp"
#include "shared/crt_string.hpp"
#include "shared/crt_memory.hpp"
#include "shared/command.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/dialog_default_proc.hpp"
#include "shared/owner_draw_dialog_control.hpp"
#include "shared/dialog_create.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/abstract_type.hpp"


// Header-static dynamic initializers of the unit at 0x005D85E0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x005D85E0-0x005D87C6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x005D8610, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_005d85e0.cpp; no source of its own.


// Dynamic initializer (0x005D8630, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_005d85e0.cpp; no source of its own.


// Dynamic initializer (0x005D8650, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_005d85e0.cpp; no source of its own.


// Dynamic initializer (0x005D8670, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_005d85e0.cpp; no source of its own.


// Dynamic initializer (0x005D8690, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_005d85e0.cpp; no source of its own.


// Dynamic initializer (0x005D86B0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_005d85e0.cpp; no source of its own.


// Dynamic initializer (0x005D86F0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_005d85e0.cpp; no source of its own.


// Dynamic initializer (0x005D8720, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_005d85e0.cpp; no source of its own.


// Dynamic initializer (0x005D8750, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_005d85e0.cpp; no source of its own.


// Dynamic initializer (0x005D8780, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_005d85e0.cpp; no source of its own.


// Dynamic initializer (0x005D8790, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_005d85e0.cpp; no source of its own.


// Dynamic initializer (0x005D87B0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_005d85e0.cpp; no source of its own.


// int __fastcall Hotkey_To_Key(int hotkey) (0x005D87D0)
// A hot key control value as a key number: the virtual key with the Alt, Ctrl and Shift
// bits; an extended key gives 0. First source of Options.CPP: its keyboard dialog expands
// the IndexClass and vector accessors inline (the defines must precede the first include).

int __fastcall Hotkey_To_Key(int hotkey)
{
    int key = hotkey & 0xFF;
    int flags = (hotkey >> 8) & 0xFF;
    if (flags & 4) key |= 0x400;
    if (flags & 2) key |= 0x200;
    if (flags & 1) key |= 0x100;
    if (flags & 8) key = 0;
    return key;
}


// unsigned short __fastcall Key_To_Hotkey(unsigned short key) (0x005D8800)
// A key number as a hot key control value (HOTKEYF_ALT, CONTROL and SHIFT in the high
// byte, HOTKEYF_EXT for the 0x1000 bit).

unsigned short __fastcall Key_To_Hotkey(unsigned short key)
{
    unsigned int flags = 0;
    if (key & 0x400) flags = 4;
    if (key & 0x200) flags |= 2;
    if (key & 0x100) flags |= 1;
    if (key & 0x1000) flags |= 8;
    return (flags << 8) | (key & 0xFF);
}


// OptionsClass::OptionsClass() (0x005D8840)
// The default settings (RA2.INI overrides them in Load_Settings).

OptionsClass::OptionsClass() :
    GameSpeed(3),
    Difficulty(1),
    ScrollMethod(0),
    ScrollRate(3),
    AutoScroll(true),
    DetailLevel(2),
    SidebarOn(true),
    SidebarCameoText(true),
    UnitActionLines(true),
    ShowHidden(false),
    ToolTips(true),
    ScreenWidth(-1),
    ScreenHeight(-1),
    StretchMovies(false),
    AllowHiResModes(false),
    AllowVRAMSidebar(false),
    SoundVolume(0.7f),
    VoiceVolume(1.0f),
    ScoreVolume(0.5f),
    IsScoreRepeat(false),
    InGameMusic(true),
    IsScoreShuffle(false),
    SoundLatency(9),
    Socket(0xFFFF),
    NetCard(0),
    KeyForceMove1(0x12),
    KeyForceMove2(0x12),
    KeyForceAttack1(0x11),
    KeyForceAttack2(0x11),
    KeySelect1(0x10),
    KeySelect2(0x10),
    KeyQueueMove1(0x51),
    KeyQueueMove2(0x51)
{
    UnlockedMovie[0] = -1;
    UnlockedMovie[1] = -1;
    DestNet[0] = 0;
}


// void OptionsClass::One_Time() (0x005D8910)
// Called once at startup; empty in this build (name after TS).

void OptionsClass::One_Time()
{
}


// void OptionsClass::Set_Shuffle(bool on) (0x005D8920)

void OptionsClass::Set_Shuffle(bool on)
{
    IsScoreShuffle = on;
    Theme.IsShuffle = on;
    Debug_Printf("ScoreShuffle is %s\n", IsScoreShuffle == true ? "ON" : "OFF");
}


// void OptionsClass::Set_Repeat(bool on) (0x005D8950)

void OptionsClass::Set_Repeat(bool on)
{
    IsScoreRepeat = on;
    Theme.IsRepeat = on;
    Debug_Printf("ScoreRepeat is %s\n", IsScoreRepeat == true ? "ON" : "OFF");
}


// void OptionsClass::Set_Score_Volume(float volume, bool feedback) (0x005D8980)

void OptionsClass::Set_Score_Volume(float volume, bool feedback)
{
    ScoreVolume = volume < 1.0 ? volume : 1.0;
    Audio_Global_Levels[2]->Volume.Set((int)(ScoreVolume * 16384.0f));
    Theme.Set_Volume((int)(ScoreVolume * 255.0));
    Debug_Printf("ScoreVolume = %f\n", ScoreVolume);
}


// void OptionsClass::Set_Sound_Volume(float volume, bool feedback) (0x005D89F0)

void OptionsClass::Set_Sound_Volume(float volume, bool feedback)
{
    SoundVolume = volume < 1.0 ? volume : 1.0;
    Audio_Global_Levels[3]->Volume.Set((int)(SoundVolume * 16384.0f));
    if (feedback) {
        Voc_Play(Rules->GenericBeep, 1.0f, 0x2000, 0);
    }
    Debug_Printf("SoundVolume = %f\n", SoundVolume);
}


// void OptionsClass::Set_Voice_Volume(float volume, bool feedback) (0x005D8A70)

void OptionsClass::Set_Voice_Volume(float volume, bool feedback)
{
    VoiceVolume = volume < 1.0 ? volume : 1.0;
    VoxClass::Set_Volume((int)(VoiceVolume * 255.0));
    Audio_Global_Levels[1]->Volume.Set((int)(VoiceVolume * 16384.0f));
    if (feedback) {
        Voc_Play(Rules->GenericBeep, VoiceVolume, 0x2000, 0);
    }
    Debug_Printf("VoiceVolume = %f\n", VoiceVolume);
}


// void OptionsClass::Load_Settings() (0x005D8B00)
// Reads the settings from RA2.INI ([Options], [Video], [Audio], [Network]),
// clamping the difficulty (0-4), detail level (0-2) and volumes (at most 1)
// and logging each; NetID holds two numbers stored with inverted characters.

void OptionsClass::Load_Settings()
{
    Debug_Printf("--------- Loading SUN.INI settings ---------------\n");

    GameSpeed = unknown_ini_839740.Get_Int("Options", "GameSpeed", GameSpeed);
    Debug_Printf("GameSpeed = %d\n", GameSpeed);
    Difficulty = unknown_ini_839740.Get_Int("Options", "Difficulty", Difficulty);
    Difficulty = Difficulty < 4 ? Difficulty : 4;
    Difficulty = Difficulty > 0 ? Difficulty : 0;
    Debug_Printf("Difficulty = %d\n", Difficulty);
    ScrollMethod = unknown_ini_839740.Get_Int("Options", "ScrollMethod", ScrollMethod);
    Debug_Printf("ScrollMethod = %d\n", ScrollMethod);
    ScrollRate = unknown_ini_839740.Get_Int("Options", "ScrollRate", ScrollRate);
    Debug_Printf("ScrollRate = %d\n", ScrollRate);
    AutoScroll = unknown_ini_839740.Get_Bool("Options", "AutoScroll", AutoScroll);
    Debug_Printf("AutoScroll is %s\n", AutoScroll == true ? "ON" : "OFF");
    DetailLevel = unknown_ini_839740.Get_Int("Options", "DetailLevel", DetailLevel);
    DetailLevel = DetailLevel < 2 ? DetailLevel : 2;
    DetailLevel = DetailLevel > 0 ? DetailLevel : 0;
    Debug_Printf("DetailLevel = %d\n", DetailLevel);
    SidebarOn = true;
    Debug_Printf("SideBar on %s\n", "RIGHT");
    SidebarCameoText = unknown_ini_839740.Get_Bool("Options", "SidebarCameoText", SidebarCameoText);
    Debug_Printf("Sidebar Text is %s\n", SidebarCameoText == true ? "ON" : "OFF");
    UnitActionLines = unknown_ini_839740.Get_Bool("Options", "UnitActionLines", UnitActionLines);
    Debug_Printf("ActionLines are %s\n", UnitActionLines == true ? "ON" : "OFF");
    ShowHidden = unknown_ini_839740.Get_Bool("Options", "ShowHidden", ShowHidden);
    Debug_Printf("ShowHidden is %s\n", ShowHidden == true ? "ON" : "OFF");
    ToolTips = unknown_ini_839740.Get_Bool("Options", "ToolTips", ToolTips);
    Debug_Printf("ToolTips are %s\n", ToolTips == true ? "ON" : "OFF");

    ScreenWidth = unknown_ini_839740.Get_Int("Video", "ScreenWidth", ScreenWidth);
    ScreenHeight = unknown_ini_839740.Get_Int("Video", "ScreenHeight", ScreenHeight);
    Debug_Printf("Resolution = %d X %d\n", ScreenWidth, ScreenHeight);
    StretchMovies = unknown_ini_839740.Get_Bool("Video", "StretchMovies", StretchMovies);
    StretchMovies = StretchMovies == true && AllowStretchBlits_852fd6 == true;
    Debug_Printf("StretchMovies is %s\n", StretchMovies == true ? "ON" : "OFF");
    AllowHiResModes = unknown_ini_839740.Get_Bool("Video", "AllowHiResModes", AllowHiResModes);
    Debug_Printf("AllowHiRes is %s\n", AllowHiResModes == true ? "ON" : "OFF");
    unknown_allow_mode_toggle_a40d0b = unknown_ini_839740.Get_Bool("Video", "AllowModeToggle", unknown_allow_mode_toggle_a40d0b);
    AllowVRAMSidebar = unknown_ini_839740.Get_Bool("Video", "AllowVRAMSidebar", AllowVRAMSidebar);

    Set_Sound_Volume(unknown_ini_839740.Get_Double("Audio", "SoundVolume", SoundVolume), false);
    Set_Voice_Volume(unknown_ini_839740.Get_Double("Audio", "VoiceVolume", VoiceVolume), false);
    Set_Score_Volume(unknown_ini_839740.Get_Double("Audio", "ScoreVolume", ScoreVolume), false);
    Set_Repeat(unknown_ini_839740.Get_Bool("Audio", "IsScoreRepeat", IsScoreRepeat));
    Set_Shuffle(unknown_ini_839740.Get_Bool("Audio", "IsScoreShuffle", IsScoreShuffle));
    InGameMusic = unknown_ini_839740.Get_Bool("Audio", "InGameMusic", InGameMusic);

    UnlockedMovie[0] = -1;
    UnlockedMovie[1] = -1;
    unsigned short buffer[200];
    unknown_ini_839740.Get_Unicode("Network", "NetID", unknown_empty_wide_839d4c, buffer, 200);
    if (buffer[0] != 0) {
        for (unsigned short *letter = buffer; *letter != 0; letter++) {
            *letter = ~*letter;
            if (*letter == 0) {
                *letter = 0xFFFF;
            }
        }
        unsigned short *token = wcstok(buffer, L" ");
        if (token != 0) {
            UnlockedMovie[0] = wcstol(token, 0, 10) - 1;
            token = wcstok(0, L" ");
            if (token != 0) {
                UnlockedMovie[1] = wcstol(token, 0, 10) - 1;
            }
        }
    }
    if (UnlockedMovie[0] < -1 || UnlockedMovie[0] >= 12) {
        UnlockedMovie[0] = -1;
    }
    if (UnlockedMovie[1] < -1 || UnlockedMovie[1] >= 12) {
        UnlockedMovie[1] = -1;
    }

    SoundLatency = unknown_ini_839740.Get_Int("Audio", "SoundLatency", SoundLatency);
    Debug_Printf("Emulated sound card latency default = %d\n", SoundLatency);
    Socket = unknown_ini_839740.Get_Int("Network", "Socket", Socket);
    NetCard = unknown_ini_839740.Get_Int("Network", "NetCard", NetCard);
    unknown_ini_839740.Get_String("Network", "DestNet", "", DestNet, sizeof(DestNet));
    Debug_Printf("--------- Complete -------------------------------\n");

    Map.Unknown_0067B880(SidebarCameoText);
    TechnoClass::unknown_6d79c0(UnitActionLines);
    Setup_Network();
}


// void OptionsClass::Save_Settings() (0x005D91B0)
// Writes the settings back to RA2.INI (NetID with inverted characters) and
// saves the file.

void OptionsClass::Save_Settings()
{
    CCFileClass file("RA2.INI");
    Debug_Printf("Saving game settings\n");

    unknown_ini_839740.Put_Int("Options", "GameSpeed", GameSpeed);
    unknown_ini_839740.Put_Int("Options", "Difficulty", Difficulty);
    unknown_ini_839740.Put_Int("Options", "ScrollMethod", ScrollMethod);
    unknown_ini_839740.Put_Int("Options", "ScrollRate", ScrollRate);
    unknown_ini_839740.Put_Bool("Options", "AutoScroll", AutoScroll);
    unknown_ini_839740.Put_Int("Options", "DetailLevel", DetailLevel);
    unknown_ini_839740.Put_Bool("Options", "SidebarCameoText", SidebarCameoText);
    unknown_ini_839740.Put_Bool("Options", "UnitActionLines", UnitActionLines);
    unknown_ini_839740.Put_Bool("Options", "ShowHidden", ShowHidden);
    unknown_ini_839740.Put_Bool("Options", "ToolTips", ToolTips);

    unknown_ini_839740.Put_Int("Video", "ScreenWidth", ScreenWidth);
    unknown_ini_839740.Put_Int("Video", "ScreenHeight", ScreenHeight);
    unknown_ini_839740.Put_Bool("Video", "StretchMovies", StretchMovies);

    unknown_ini_839740.Put_Double("Audio", "SoundVolume", SoundVolume);
    unknown_ini_839740.Put_Double("Audio", "VoiceVolume", VoiceVolume);
    unknown_ini_839740.Put_Double("Audio", "ScoreVolume", ScoreVolume);
    unknown_ini_839740.Put_Bool("Audio", "IsScoreRepeat", IsScoreRepeat);
    unknown_ini_839740.Put_Bool("Audio", "IsScoreShuffle", IsScoreShuffle);
    unknown_ini_839740.Put_Int("Audio", "SoundLatency", SoundLatency);
    unknown_ini_839740.Put_Bool("Audio", "InGameMusic", InGameMusic);

    unknown_ini_839740.Put_Int("Network", "Socket", Socket);
    unknown_ini_839740.Put_Int("Network", "NetCard", NetCard);
    unknown_ini_839740.Put_String("Network", "DestNet", DestNet);
    unsigned short buffer[200];
    swprintf(buffer, L"%d %d", UnlockedMovie[0] + 1, UnlockedMovie[1] + 1);
    for (unsigned short *letter = buffer; *letter != 0; letter++) {
        *letter = ~*letter;
        if (*letter == 0) {
            *letter = 0xFFFF;
        }
    }
    unknown_ini_839740.Put_Unicode("Network", "NetID", buffer);

    unknown_ini_839740.Save(file, false);
    Setup_Network();
}


// void OptionsClass::Setup_Network() (0x005D94D0)
// Sets the IPX socket (0x8813 by default, else Socket moved above 0x4000)
// and, when DestNet holds at least four dot-separated hex numbers, the
// destination network (with the broadcast node).

void OptionsClass::Setup_Network()
{
    if (Socket == 0xFFFF) {
        unknown_ipx_manager_a40978.Set_Socket(0x8813);
    } else if (Socket < 0x4000) {
        unknown_ipx_manager_a40978.Set_Socket(Socket + 0x4000);
    }
    int length = strlen(DestNet);
    if (length != 0) {
        char *buffer = new char[length + 1];
        strcpy(buffer, DestNet);
        NetNumType net;
        NetNodeType node;
        int index = 0;
        char *token = strtok(buffer, ".");
        while (token != 0) {
            int value;
            sscanf(token, "%x", &value);
            if (index < 4) {
                net[index] = value;
            } else {
                node[index - 4] = value;
            }
            index++;
            token = strtok(0, ".");
        }
        if (index >= 4) {
            memset(node, 0xFF, sizeof(node));
            unknown_dest_net_set_a3f604 = 1;
            unknown_dest_net_a3f608 = IPXAddressClass(net, node, 0);
        }
    }
}


// void OptionsClass::Set() (0x005D95E0)
// Applies the current settings: volumes, score repeat/shuffle, sidebar cameo
// text and unit action lines (name after TS's OptionsClass::Set).

void OptionsClass::Set()
{
    Set_Sound_Volume(SoundVolume, false);
    Set_Voice_Volume(VoiceVolume, false);
    Set_Score_Volume(ScoreVolume, false);
    Set_Repeat(IsScoreRepeat);
    Set_Shuffle(IsScoreShuffle);
    Map.Unknown_0067B880(SidebarCameoText);
    TechnoClass::unknown_6d79c0(UnitActionLines);
}


// int OptionsClass::Normalize_Delay(int delay) const (0x005D9760)
// A delay adjusted for the game speed: from a table for delays 1-4, else
// scaled by 8 / (GameSpeed + 1) (name after TS).

int OptionsClass::Normalize_Delay(int delay) const
{
    if (delay) {
        if (delay < 5) {
            delay = Delay_Adjust[delay - 1][GameSpeed];
        } else {
            delay = (delay * 8) / (GameSpeed + 1);
        }
    }
    return delay;
}


// int OptionsClass::Normalize_Volume(int volume) (0x005D9790)
// A volume scaled by the sound volume (name after TS).

int OptionsClass::Normalize_Volume(int volume)
{
    return volume * SoundVolume;
}


// int __stdcall Unknown_Options_Dialog_Proc_005d97a0(HWND, UINT, WPARAM, LPARAM) (0x005D97A0)
// The keyboard dialog (0xA3, run by OptionsClass::Unknown_005DA360; TS Hotkey_Dialog_Proc): category
// combo 0x4C7, command list 0x4C8, description 0x4CB, current shortcut 0x63C, hot key control 0x4CD,
// assigned-to 0x63B, error text 0x63D. It posts itself 0x464 (fill the command list), 0x465 (show the
// selected command), 0x466 (assign the hot key) and 0x467 (refill the categories; also sent for 0x497).
// 0x686 writes KEYBOARD.INI ([Hotkey] name = key), Cancel reloads it, 0x4D0 deletes it.
// Needs INDEX_DECLARE_FETCH_ID_BY_POSITION and the vector accessors (defined in the unit's first source,
// Options_Hotkey_To_Key_005d87d0.cpp).

bool __fastcall Load_Keyboard_Hotkeys();
bool __fastcall Command_Allows_Modified_Key_00480b20(CommandClass const &command, unsigned int key);
bool __fastcall Command_Allows_Key_00480b40(unsigned int key);
extern "C" int __cdecl _wcsicmp(wchar_t const *left, wchar_t const *right);

int __stdcall Unknown_Options_Dialog_Proc_005d97a0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    wchar_t buffer[64];
    int *retval;
    static int current_selection = -1;

    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result) {
        return result;
    }

    retval = (int *)GetWindowLongA(window, 8);

    switch (message) {
        case 0x466: {
            HWND list_commands = GetDlgItem(window, 0x4C8);
            int selection = SendMessageA(list_commands, 0x188, 0, 0);
            if (selection != -1) {
                CommandClass *cmd = (CommandClass *)SendMessageA(list_commands, 0x199, selection, 0);
                HWND hotkey = GetDlgItem(window, 0x4CD);
                int key = Hotkey_To_Key(SendMessageA(hotkey, 0x402, 0, 0));
                if (!Command_Allows_Modified_Key_00480b20(*cmd, key)) {
                    HWND error = GetDlgItem(window, 0x63D);
                    SendMessageA(error, 0x4B2, 0, (LPARAM)StringTable::Load_String("Error:CannotMap", 0, "D:\\Ra2\\Options.CPP", 987));
                } else if (!Command_Allows_Key_00480b40(key)) {
                    HWND error = GetDlgItem(window, 0x63D);
                    SendMessageA(error, 0x4B2, 0, (LPARAM)StringTable::Load_String("Error:CannotRemap", 0, "D:\\Ra2\\Options.CPP", 995));
                    return TRUE;
                } else {
                    for (int i = 0; i < unknown_hotkeys_832380.Count(); i++) {
                        if (unknown_hotkeys_832380.Fetch_By_Position(i) == cmd) {
                            unknown_hotkeys_832380.Remove_Index(unknown_hotkeys_832380.Fetch_ID_By_Position(i));
                            break;
                        }
                    }
                    if (key != 0) {
                        unknown_hotkeys_832380.Remove_Index(key);
                        unknown_hotkeys_832380.Add_Index(key, cmd);
                        return TRUE;
                    }
                }
            }
            return TRUE;
        }

        case 0x465: {
            HWND list_commands = GetDlgItem(window, 0x4C8);
            int selection = SendMessageA(list_commands, 0x188, 0, 0);
            if (selection != -1) {
                CommandClass const *cmd = (CommandClass const *)SendMessageA(list_commands, 0x199, selection, 0);
                HWND description = GetDlgItem(window, 0x4CB);
                SendMessageA(description, 0x4B2, 0, (LPARAM)cmd->Get_Description());

                int key = 0;
                for (int i = 0; i < unknown_hotkeys_832380.Count(); i++) {
                    if (unknown_hotkeys_832380.Fetch_By_Position(i) == cmd) {
                        key = unknown_hotkeys_832380.Fetch_ID_By_Position(i);
                        break;
                    }
                }

                HWND key_label = GetDlgItem(window, 0x63C);
                OwnerDraw::unknown_hotkey_text_5fc600(Key_To_Hotkey(key), buffer);
                SendMessageA(key_label, 0x4B2, 0, (LPARAM)buffer);

                HWND hotkey = GetDlgItem(window, 0x4CD);
                SendMessageA(hotkey, 0x401, 0, 0);

                HWND hotkey_name = GetDlgItem(window, 0x63B);
                SendMessageA(hotkey_name, 0x4B2, 0, (LPARAM)L"");
                return TRUE;
            }
            return TRUE;
        }

        case 0x464: {
            HWND cmb_category = GetDlgItem(window, 0x4C7);
            if (SendMessageA(cmb_category, 0x147, 0, 0) != current_selection) {
                current_selection = SendMessageA(cmb_category, 0x147, 0, 0);
                SendMessageA(cmb_category, 0x4B3, 64, (LPARAM)buffer);
                HWND list_commands = GetDlgItem(window, 0x4C8);
                SendMessageA(list_commands, 0x184, 0, 0);
                for (int i = 0; i < unknown_commands_832358.Count(); i++) {
                    CommandClass const *cmd = unknown_commands_832358[i];
                    if (_wcsicmp(cmd->Get_Category(), buffer) == 0) {
                        int index = SendMessageA(list_commands, 0x4CD, 0, (LPARAM)cmd->Get_UI_Name());
                        if (index != -1) {
                            SendMessageA(list_commands, 0x19A, index, (LPARAM)cmd);
                        }
                    }
                }
                HWND description = GetDlgItem(window, 0x4CB);
                SendMessageA(description, 0x4B2, 0, (LPARAM)L"");
                HWND key_label = GetDlgItem(window, 0x63C);
                SendMessageA(key_label, 0x4B2, 0, (LPARAM)L"");
                HWND hotkey_name = GetDlgItem(window, 0x63B);
                SendMessageA(hotkey_name, 0x4B2, 0, (LPARAM)L"");
                SendMessageA(hotkey_name, 0x186, 0, 0);
                SendMessageA(window, 0x465, 0, 0);
                return TRUE;
            }
            return TRUE;
        }

        case 0x467: {
            HWND cmb_category = GetDlgItem(window, 0x4C7);
            SendMessageA(cmb_category, 0x14B, 0, 0);
            for (int i = 0; i < unknown_commands_832358.Count(); i++) {
                CommandClass const *cmd = unknown_commands_832358[i];
                if (SendMessageA(cmb_category, 0x4BE, 0, (LPARAM)cmd->Get_Category()) == -1) {
                    SendMessageA(cmb_category, 0x4C2, 0, (LPARAM)cmd->Get_Category());
                }
            }
            SendMessageA(cmb_category, 0x14E, 0, 0);
            current_selection = -1;
            SendMessageA(window, 0x464, 0, 0);
            return TRUE;
        }

        case 0x497:
            SendMessageA(window, 0x467, 0, 0);
            return FALSE;
        case 0x111:
            switch ((unsigned short)wparam) {
                case 0x686:
                    if ((unsigned short)(wparam >> 16) == 0) {
                        CCINIClass ini;
                        ini.Clear();
                        for (int i = 0; i < unknown_hotkeys_832380.Count(); i++) {
                            CommandClass const *cmd = unknown_hotkeys_832380.Fetch_By_Position(i);
                            int key = unknown_hotkeys_832380.Fetch_ID_By_Position(i);
                            ini.Put_Int("Hotkey", cmd->Get_Name(), key);
                        }
                        RawFileClass file("Keyboard.ini");
                        ini.Save(file, false);
                        *retval = 1;
                        return TRUE;
                    }
                    break;

                case 2:
                    if ((unsigned short)(wparam >> 16) == 0) {
                        Load_Keyboard_Hotkeys();
                        *retval = 2;
                        return TRUE;
                    }
                    break;

                case 0x4C8:
                    if ((unsigned short)(wparam >> 16) == 1) {
                        SendMessageA(window, 0x465, 0, 0);
                        HWND hotkey = GetDlgItem(window, 0x4CD);
                        if (hotkey != 0) {
                            SetFocus(hotkey);
                            return TRUE;
                        }
                    }
                    break;

                case 0x4CF:
                    SendMessageA(window, 0x466, 0, 0);
                    SendMessageA(window, 0x465, 0, 0);
                    return TRUE;

                case 0x4CD:
                    if ((unsigned short)(wparam >> 16) == 0x300) {
                        HWND error = GetDlgItem(window, 0x63D);
                        SendMessageA(error, 0x4B2, 0, (LPARAM)L"");
                        int key = Hotkey_To_Key(SendMessageA((HWND)lparam, 0x402, 0, 0));
                        wchar_t const *key_name;
                        if (unknown_hotkeys_832380.Is_Present(key)) {
                            key_name = unknown_hotkeys_832380[key]->Get_UI_Name();
                            if (key_name == 0) {
                                key_name = L"";
                            }
                        } else {
                            key_name = L"";
                        }
                        HWND hotkey_name = GetDlgItem(window, 0x63B);
                        SendMessageA(hotkey_name, 0x4B2, 0, (LPARAM)key_name);
                        return TRUE;
                    }
                    break;

                case 0x4D0:
                    if ((unsigned short)(wparam >> 16) == 0) {
                        Debug_Printf("Deleting users KEYBOARD.INI\n");
                        CCFileClass file("KEYBOARD.INI");
                        file.Delete();
                        Load_Keyboard_Hotkeys();
                        SendMessageA(window, 0x467, 0, 0);
                        return TRUE;
                    }
                    break;

                case 0x4C7:
                    if ((unsigned short)(wparam >> 16) == 1) {
                        SendMessageA(window, 0x464, 0, 0);
                        return TRUE;
                    }
                    break;
            }
            return TRUE;

    }

    return FALSE;
}


// bool OptionsClass::Unknown_005DA360() (0x005DA360)
// Runs dialog 0xA3 (procedure 0x005D97A0) until it sets its result or the dialog handler ends the wait;
// outside a game the title screen keeps being serviced. Does not use its receiver; always true.

void __fastcall unknown_dialog_5ff7c0(HWND window);      // 0x005FF7C0 (shared/ms_message_box.hpp)
void __fastcall unknown_dialog_5ff6e0(HWND window);      // 0x005FF6E0
void __fastcall unknown_517370(bool unknown1);           // 0x00517370
int __stdcall Unknown_Options_Dialog_Proc_005d97a0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

bool OptionsClass::Unknown_005DA360()
{
    int rc = -1;
    HWND dialog = unknown_create_dialog_5ff610(0xA3, Unknown_Options_Dialog_Proc_005d97a0, 0);
    if (dialog) {
        SetWindowLongA(dialog, 8, (LONG)&rc);
        unknown_dialog_5ff7c0(dialog);
        while (rc < 0) {
            if (unknown_6000d0() == true) {
                rc = 2;
            }
            if (!unknown_flag_a40958) {
                unknown_517370(false);
            }
        }
        unknown_dialog_5ff6e0(dialog);
    }
    return true;
}


// int OptionsClass::Get_Scroll_Method() const (0x005DA3E0)

int OptionsClass::Get_Scroll_Method() const
{
    return ScrollMethod;
}


// void OptionsClass::Unlock_Movie(char const *name) (0x005DA3F0)
// A campaign movie that has been played raises its campaign's last unlocked
// movie (Soviet list first, then Allied).

// The index of a named movie in a list, or -1.
inline int Movie_Index(MovieEntryStruct const *movies, char const *name)
{
    for (int index = 0; movies->Name != 0; index++, movies++) {
        if (_stricmp(name, movies->Name) == 0) {
            return index;
        }
    }
    return -1;
}

void OptionsClass::Unlock_Movie(char const *name)
{
    int index = Movie_Index(SovietMovies, name);
    if (index > UnlockedMovie[0]) {
        UnlockedMovie[0] = index;
    }
    index = Movie_Index(AlliedMovies, name);
    if (index > UnlockedMovie[1]) {
        UnlockedMovie[1] = index;
    }
}


// void OptionsClass::Fill_Movie_List(HWND list) (0x005DA470)
// The intro and every unlocked campaign movie go into the list box with
// their entries as item data.

// Adds a movie (its label's text) to the list box.
inline void Add_Movie(HWND list, MovieEntryStruct const *movie)
{
    if (movie->Name != 0 && movie->Label != 0) {
        int index = SendMessageA(list, 0x4CD, 0, (LPARAM)StringTable::Load_String(movie->Label, 0, "D:\\Ra2\\Options.CPP", 1248));
        if (index != -1) {
            SendMessageA(list, 0x19A, index, (LPARAM)movie);
        }
    }
}

void OptionsClass::Fill_Movie_List(HWND list)
{
    Add_Movie(list, &IntroMovie);
    if (UnlockedMovie[0] >= 0) {
        int count = UnlockedMovie[0] + 1;
        for (MovieEntryStruct *movie = SovietMovies; movie->Name != 0 && count > 0; movie++, count--) {
            Add_Movie(list, movie);
        }
    }
    if (UnlockedMovie[1] >= 0) {
        int count = UnlockedMovie[1] + 1;
        for (MovieEntryStruct *movie = AlliedMovies; movie->Name != 0 && count > 0; movie++, count--) {
            Add_Movie(list, movie);
        }
    }
}


// Binary_Search<IndexClass<int, CommandClass *>::NodeElement> (0x005DA590)
// Template instantiation; the definition is in shared/ini_class.hpp and the keyboard
// dialog 0x005D97A0 (its two Remove_Index expansions) instantiates it out of line.
