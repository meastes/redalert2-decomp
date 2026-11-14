// init.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/init.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x00511400: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00511400-0x005115E1); the other
// drafts of this plan are emitted by these includes.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"


// Dynamic initializer (0x00511420, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00511400.cpp; no source of its own.


// Dynamic initializer (0x00511450, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00511400.cpp; no source of its own.


// Dynamic initializer (0x00511470, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00511400.cpp; no source of its own.


// Dynamic initializer (0x00511490, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00511400.cpp; no source of its own.


// Dynamic initializer (0x005114B0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00511400.cpp; no source of its own.


// Dynamic initializer (0x005114D0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00511400.cpp; no source of its own.


// Dynamic initializer (0x005114F0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00511400.cpp; no source of its own.


// Dynamic initializer (0x00511530, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00511400.cpp; no source of its own.


// Dynamic initializer (0x00511560, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00511400.cpp; no source of its own.


// Dynamic initializer (0x00511590, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00511400.cpp; no source of its own.


// Dynamic initializer (0x005115C0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00511400.cpp; no source of its own.


// Dynamic initializer (0x005115D0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00511400.cpp; no source of its own.


// Centres the dialog's movie control 0x71A in the 800x600 area of the screen
// (unknown_8523e0 x unknown_8523e4), starts it (0x4E3) and loads the large or, at
// 640 wide, the small Ra2ts clip (0x4E4) (0x005115F0; called by 0x005EAB60).
#include "shared/ms_message_box.hpp"
#include "shared/unknown_globals.hpp"

void __fastcall unknown_5115f0(HWND dialog)
{
    HWND control = GetDlgItem(dialog, 0x71A);
    if (control != 0) {
        int x = unknown_8523e0 <= 800 ? 0 : (unknown_8523e0 - 800) / 2;
        int y = unknown_8523e4 <= 600 ? 0 : (unknown_8523e4 - 600) / 2;
        SetWindowPos(control, 0, x, y, -1, -1, 0xD);     // SWP_NOSIZE | SWP_NOZORDER | SWP_NOREDRAW
        SendMessageA(control, 0x4E3, 1, 0);
        if (unknown_8523e0 == 640) {
            SendMessageA(control, 0x4E4, 0, (LPARAM)"Ra2ts_s");
        } else {
            SendMessageA(control, 0x4E4, 0, (LPARAM)"Ra2ts_l");
        }
    }
}


// int __fastcall Init_Game() -- 0x005116A0
// Body 0x005116A0-0x005127E3 (4420 bytes). Tiberian Sun's Init_Game with its Bootstrap, Init_Mouse and
// Init_CDROM_Access expanded inline: scenario and rules, the bootstrap mixfiles, palettes and drawers,
// the CD check, the secondary mixfiles, SOUND/EVA/THEME.INI, the bulk data and the rules, the intro
// movies and the multiplayer score records.
#include "shared/crt_memory.hpp"
#include "shared/house.hpp"
#include "shared/id_source.hpp"
#include "shared/rules.hpp"
#include "shared/keyboard.hpp"
#include "shared/mono_class.hpp"
#include "shared/text_print.hpp"
#include "shared/audio.hpp"
#include "shared/mix_file_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/ccini_class.hpp"
#include "shared/convert.hpp"
#include "shared/matrix3d.hpp"
#include "shared/mouse.hpp"
#include "shared/wwmouse.hpp"
#include "shared/gadget.hpp"
#include "shared/dsurface.hpp"
#include "shared/display.hpp"
#include "shared/mouse_class.hpp"
#include "shared/cd.hpp"
#include "shared/cd_file_class.hpp"
#include "shared/special.hpp"
#include "shared/wol_registry_options.hpp"
#include "shared/ww_message_box.hpp"
#include "shared/string_table.hpp"
#include "shared/options.hpp"
#include "shared/movie.hpp"
#include "shared/tactical.hpp"
#include "shared/voc_class.hpp"
#include "shared/vox_class.hpp"
#include "shared/vox_speech.hpp"
#include "shared/theme.hpp"
#include "shared/session_state.hpp"
#include "shared/multiplayer_game_mode.hpp"
#include "shared/progress_screen.hpp"
#include "shared/exit.hpp"
#include "shared/ms_engine.hpp"
#include "shared/debug_printf.hpp"


extern PaletteClass unknown_palette_837510;     // 0x00837510 black (TS BlackPalette)
extern PaletteClass unknown_palette_837810;     // 0x00837810 WAYPOINT.PAL
extern PaletteClass unknown_palette_838110;     // 0x00838110 the theater palette as loaded (TS OriginalPalette)
extern PaletteClass unknown_palette_838410;     // 0x00838410 (TS WhitePalette)
extern PaletteClass unknown_palette_838a10;     // 0x00838A10 the unit palette (TS SchemePalette)
extern PaletteClass unknown_palette_838d10;     // 0x00838D10 the voxel palette
extern RGBClass unknown_voxel_colors_ae3008[256];   // 0x00AE3008 voxels.vpl's colours
extern bool unknown_hicolor_7ce008;             // 0x007CE008 (TS HicolorFlag)
extern int unknown_audio_7f4cc8;                // 0x007F4CC8 Audio_Init's argument
extern bool unknown_use_voxel_cache_839a58;     // 0x00839A58
extern float unknown_camera_angle_839a5c;       // 0x00839A5C
extern Matrix3D unknown_voxel_transform_839a70; // 0x00839A70
extern float unknown_camera_angle_79acf4;       // 0x0079ACF4 default
extern float unknown_light_angle_79acf0;        // 0x0079ACF0 default
extern int unknown_af6614;                      // 0x00AF6614
extern int unknown_af6610;                      // 0x00AF6610
extern MouseClass Map;                          // 0x008324E0
extern unsigned __int64 unknown_loading_time_a41668;
extern ConvertClass *unknown_drawer_8323c8;     // 0x008323C8 (TS MouseDrawer)    // 0x00A41668 when the loading screen may go

MonoClass *__fastcall Unknown_0059DA50(int page);       // 0x0059DA50
bool __fastcall Init_Bootstrap_Mixfiles();              // 0x00515C90
void __fastcall Unknown_005EAC10();                     // 0x005EAC10
void __fastcall Unknown_006F1B60();                     // 0x006F1B60
void __fastcall Draw_Loading_Screen();                  // 0x00516560
int __fastcall unknown_run_716f40(void *file);          // 0x00716F40 (TS VoxelDrawSystem::Load_VPL_File)
void __stdcall Unknown_00717FB0(float angle);           // 0x00717FB0 (TS Set_Voxel_Light_Angle)
void __fastcall Unknown_00718060();                     // 0x00718060 (TS Init_Voxel_Matrices)
bool Init_Expansion_Files();                            // 0x00515AF0
bool __fastcall Init_Secondary_Mixfiles();              // 0x00515E70
void __fastcall Read_Battle_Files();                    // 0x005127F0
bool __fastcall Init_Bulk_Data();                       // 0x00516900
bool __fastcall Init_Rules();                           // 0x005129C0
void __fastcall Play_Movie_005A02B0(char const *name, int theme, bool a, bool b, bool c);          // 0x005A02B0
void __fastcall Play_Movie_005A02D0(char const *name, int theme, bool a, bool b, bool c, bool d);  // 0x005A02D0
void __fastcall Init_Commands_005173C0();
void __fastcall Init_Random();                          // 0x00515710               // 0x005173C0

// Tiberian Sun's Set_Voxel_Camera_Angle.
inline void Set_Voxel_Camera_Angle(float angle)
{
    unknown_camera_angle_839a5c = angle;
    unknown_voxel_transform_839a70.Make_Identity();
}

static void Init_Heaps() {}
static void Init_Threads() {}
static bool Bootstrap();
static void Init_Mouse();
static bool Init_CDROM_Access();

int __fastcall Init_Game()
{
    Debug_Printf("Init Game\n");

    unknown_id_source_a3d290 = new ScenarioClass;
    if (unknown_id_source_a3d290 == 0) {
        Debug_Printf("Failed to instantiate Scenario!\n");
        return -1;
    }

    Rules = new RulesClass;
    if (Rules == 0) {
        Debug_Printf("Failed to instantiate Rules!\n");
        return -1;
    }

    Debug_Printf("Bootstrap.....");
    if (!Bootstrap()) {
        Unknown_00496460(" ..Failed to bootstrap!\n");
        return -1;
    }
    Unknown_00496460(" ...OK\n");

    Debug_Printf("Init Mouse\n");
    Init_Mouse();

    Debug_Printf("Init CDROM\n");
    if (!Init_CDROM_Access()) {
        Debug_Printf("Failed to initialize CDROM access!\n");
        return 1;
    }

    Debug_Printf("Init Secondary Mixfiles.....");
    if (!Init_Secondary_Mixfiles()) {
        Unknown_00496460(" ...Failed!!!\n");
        return -1;
    }
    Unknown_00496460(" ...OK\n");

    Debug_Printf("Init Campaigns\n");
    Read_Battle_Files();

    Debug_Printf("Init Heaps\n");
    Init_Heaps();

    Debug_Printf("Init Threads\n");
    Init_Threads();

    Debug_Printf("Reading Game Settings\n");
    Options.Load_Settings();

    Debug_Printf("Init Anim System\n");
    VQA_Unknown_0071DB20();
    VQA_Unknown_0071DB60(0);

    Debug_Printf("Init Color Remap Tables\n");
    Gadget_Scheme_Name = "Grey";

    Debug_Printf("Creating TacticalMap\n");
    delete unknown_tactical_83996c;
    unknown_tactical_83996c = new Tactical;
    if (unknown_tactical_83996c == 0) {
        Debug_Printf("Failed to create TacticalMap!\n");
        return -1;
    }

    Surface *hidden = unknown_surface_839954;
    unknown_surface_83995c = hidden;

    Debug_Printf("Reading SOUND.INI\n");
    CCFileClass voc_file("SOUND.INI");
    if (voc_file.Is_Available() == false) {
        Debug_Printf("Failed to find SOUND.INI!\n");
        return -1;
    }
    CCINIClass voc_ini;
    if (!voc_ini.Load(voc_file, false)) {
        Debug_Printf("Failed to load SOUND.INI!\n");
        return -1;
    }
    Unknown_00714860();
    VocClass::Process(voc_ini);

    Debug_Printf("Reading EVA.INI\n");
    CCFileClass eva_file("EVA.INI");
    if (eva_file.Is_Available() == false) {
        Debug_Printf("Failed to find EVA.INI!\n");
        return -1;
    }
    CCINIClass eva_ini;
    if (!eva_ini.Load(eva_file, false)) {
        Debug_Printf("Failed to load EVA.INI!\n");
        return -1;
    }
    Speech_Free_All();
    VoxClass::Process(eva_ini);

    Debug_Printf("Reading THEME.INI\n");
    CCFileClass theme_file("THEME.INI");
    if (theme_file.Is_Available() == false) {
        Debug_Printf("Failed to find THEME.INI!\n");
        return -1;
    }
    CCINIClass theme_ini;
    if (!theme_ini.Load(theme_file, false)) {
        Debug_Printf("Failed to load THEME.INI!\n");
        return -1;
    }
    Theme.Unknown_006E7A70();
    Theme.Unknown_006E7890(theme_ini);
    Theme.Scan();

    Debug_Printf("Init Bulk Data\n");
    if (!Init_Bulk_Data()) {
        return -1;
    }

    Debug_Printf("Init Rules\n");
    if (!Init_Rules()) {
        Debug_Printf("Failed to initialize Rules!\n");
        return -1;
    }

    while (Audio_Now() < unknown_loading_time_a41668) {
    }

    if (unknown_special_a40920.IsFromInstall == true) {
        Debug_Printf("Playing first time intro sequence.\n");
        if (unknown_special_a40920.EscapeMovies) {
            Play_Movie_005A02D0("WESTLOGO", -1, true, true, true, false);
            Play_Movie_005A02D0("A00_F00E", -1, false, true, true, false);
        } else {
            Play_Movie_005A02B0("WESTLOGO", -1, true, true, true);
            Play_Movie_005A02B0("A00_F00E", -1, false, true, true);
        }
    } else {
        Debug_Printf("Playing startup movie.\n");
        Play_Movie_005A02D0("WESTLOGO", -1, true, true, true, false);
    }
    unknown_5159b0(0, true);
    unknown_481fb0();

    unknown_max_players_a3d558 = Rules->MaxPlayers;
    Gadget_Scheme_Name = "Grey";

    unknown_session_a3d298.unknown_22f4 = 0;
    unknown_session_a3d298.ScoreCount = 0;
    unknown_session_a3d298.unknown_2300 = 0;
    for (int i = 0; i < 8; i++) {
        unknown_session_a3d298.Scores[i].Name[0] = 0;
        unknown_session_a3d298.Scores[i].Winner = 0;
        for (int j = 0; j < 4; j++) {
            unknown_session_a3d298.Scores[i].KillCounts[j] = -1;
            unknown_session_a3d298.Scores[i].LostCounts[j] = -1;
            unknown_session_a3d298.Scores[i].BuiltCounts[j] = -1;
            unknown_session_a3d298.Scores[i].ScoreCounts[j] = -1;
        }
    }
    memset(unknown_flags_a3f118, true, sizeof(unknown_flags_a3f118));

    Init_Random();
    Debug_Printf("Init Commands\n");
    Init_Commands_005173C0();
    Debug_Printf("Game Init Completed.\n");
    MultiplayerGameMode::unknown_init_modes_5b8bd0();
    return 0;
}

static bool Bootstrap()
{
    int index;

    if (!unknown_hicolor_7ce008) {
        Unknown_004955D0(&unknown_palette_837510, 0, 0);
    }

    if (CD::IsLocal == true) {
        CD::Set_Required_CD(-2);
    }

    do {
        WWKeyboard->Check();
    } while (!unknown_flag_a40d28);

    unknown_redraw_8523dc = false;
    Unknown_0059DA50(4)->Clear();

    if (!Init_Bootstrap_Mixfiles()) {
        Debug_Printf("Failed to initialize bootstrap mixfiles!\n");
        return false;
    }

    BitText = new BitTextClass;
    Unknown_005EAC10();
    Unknown_006F1B60();
    Audio_Init(unknown_audio_7f4cc8);
    Draw_Loading_Screen();
    WWKeyboard->Clear();

    memmove(&unknown_palette_838a10[0], MixFileClass::Retrieve("UNITSNO.PAL"), sizeof(unknown_palette_838a10));
    for (index = 0; index < 256; index++) {
        unknown_palette_838a10[index] = RGBClass(
            (unsigned char)(unknown_palette_838a10[index].Red << 2),
            (unsigned char)(unknown_palette_838a10[index].Green << 2),
            (unsigned char)(unknown_palette_838a10[index].Blue << 2));
    }

    memmove(&unknown_palette_837e10[0], MixFileClass::Retrieve("TEMPERAT.PAL"), sizeof(unknown_palette_837e10));
    for (index = 0; index < 256; index++) {
        unknown_palette_837e10[index] = RGBClass(
            (unsigned char)(unknown_palette_837e10[index].Red << 2),
            (unsigned char)(unknown_palette_837e10[index].Green << 2),
            (unsigned char)(unknown_palette_837e10[index].Blue << 2));
    }
    unknown_palette_838110 = unknown_palette_837e10;
    unknown_palette_838710 = unknown_palette_837e10;
    unknown_palette_838410[0] = unknown_palette_837510[0];

    memmove(&unknown_palette_837810[0], MixFileClass::Retrieve("WAYPOINT.PAL"), sizeof(unknown_palette_837810));
    for (index = 0; index < 256; index++) {
        unknown_palette_837810[index] = RGBClass(
            (unsigned char)(unknown_palette_837810[index].Red << 2),
            (unsigned char)(unknown_palette_837810[index].Green << 2),
            (unsigned char)(unknown_palette_837810[index].Blue << 2));
    }

    CCFileClass vplfile("voxels.vpl");
    unknown_run_716f40(&vplfile);
    for (int i = 0; i < 256; i++) {
        unknown_palette_838d10[i] = RGBClass(unknown_voxel_colors_ae3008[i].Red, unknown_voxel_colors_ae3008[i].Green,
            unknown_voxel_colors_ae3008[i].Blue);
    }
    unknown_use_voxel_cache_839a58 = true;
    Set_Voxel_Camera_Angle(unknown_camera_angle_79acf4);
    Unknown_00717FB0(unknown_light_angle_79acf0);
    unknown_af6614 = 1;
    unknown_af6610 = 0;
    Unknown_00718060();

    unknown_drawer_8323b8 = new ConvertClass(unknown_palette_837e10, unknown_palette_837e10, unknown_surface_839950, 0x35, false);

    PaletteClass pal;
    memmove(&pal[0], MixFileClass::Retrieve("ANIM.PAL"), sizeof(pal));
    for (index = 0; index < 256; index++) {
        pal[index] = RGBClass((unsigned char)(pal[index].Red << 2), (unsigned char)(pal[index].Green << 2),
            (unsigned char)(pal[index].Blue << 2));
    }
    Unit_Draw_Convert_008323C0 = new ConvertClass(pal, unknown_palette_837e10, unknown_surface_839950, 0x35, false);

    memmove(&pal[0], MixFileClass::Retrieve("PALETTE.PAL"), sizeof(pal));
    for (index = 0; index < 256; index++) {
        pal[index] = RGBClass((unsigned char)(pal[index].Red << 2), (unsigned char)(pal[index].Green << 2),
            (unsigned char)(pal[index].Blue << 2));
    }
    unknown_drawer_8323c4 = new ConvertClass(pal, unknown_palette_837e10, unknown_surface_839950, 0x35, false);

    memmove(&pal[0], MixFileClass::Retrieve("UNITSNO.PAL"), sizeof(pal));
    for (index = 0; index < 256; index++) {
        pal[index] = RGBClass((unsigned char)(pal[index].Red << 2), (unsigned char)(pal[index].Green << 2),
            (unsigned char)(pal[index].Blue << 2));
    }
    unknown_drawer_8323b4 = new ConvertClass(pal, unknown_palette_837e10, unknown_surface_839950, 0x35, false);

    memmove(&pal[0], MixFileClass::Retrieve("CAMEO.PAL"), sizeof(pal));
    for (index = 0; index < 256; index++) {
        pal[index] = RGBClass((unsigned char)(pal[index].Red << 2), (unsigned char)(pal[index].Green << 2),
            (unsigned char)(pal[index].Blue << 2));
    }
    unknown_drawer_8323b0 = new ConvertClass(pal, unknown_palette_837e10, unknown_surface_839950, 0x35, false);

    memmove(&pal[0], MixFileClass::Retrieve("MOUSEPAL.PAL"), sizeof(pal));
    for (index = 0; index < 256; index++) {
        pal[index] = RGBClass((unsigned char)(pal[index].Red << 2), (unsigned char)(pal[index].Green << 2),
            (unsigned char)(pal[index].Blue << 2));
    }
    unknown_drawer_8323c8 = new ConvertClass(pal, unknown_palette_837e10, unknown_surface_839950, 1, false);

    unknown_drawer_8323bc = unknown_drawer_8323b4;

    CCFileClass palfile("GRFXTXT.PAL");
    if (palfile.Is_Available()) {
        unknown_grfxtxt_palette_a41670 = Load_Alloc_Data(palfile);
        palfile.Close();
        for (index = 0; index < 256; index++) {
            (*(PaletteClass *)unknown_grfxtxt_palette_a41670)[index] = RGBClass(
                (unsigned char)((*(PaletteClass *)unknown_grfxtxt_palette_a41670)[index].Red << 2),
                (unsigned char)((*(PaletteClass *)unknown_grfxtxt_palette_a41670)[index].Green << 2),
                (unsigned char)((*(PaletteClass *)unknown_grfxtxt_palette_a41670)[index].Blue << 2));
        }
    }
    if (unknown_grfxtxt_palette_a41670 != 0) {
        unknown_grfxtxt_drawer_a41678 = new ConvertClass(*(PaletteClass *)unknown_grfxtxt_palette_a41670,
            unknown_palette_837e10, unknown_surface_839950, 1, false);
    }

    CCFileClass shpfile("GRFXTXT.SHP");
    if (shpfile.Is_Available()) {
        unknown_grfxtxt_shape_a41674 = Load_Alloc_Data(shpfile);
        shpfile.Close();
    }

    Init_Expansion_Files();
    return true;
}

static void Init_Mouse()
{
    unknown_block_abd488 = (void *)Unknown_Load_Shape_00495270("MOUSE.SHP", unknown_flag_abd48c);
    if (unknown_block_abd488 != 0) {
        WWMouse->Hide_Mouse();
        Point2D pt = Point2D(0, 0);
        WWMouse->Set_Cursor(pt, (ShapeFileStruct const *)unknown_block_abd488, 0);
    }
    Map.Set_Default_Mouse(0, false);
    unknown_481fb0();
}

static bool Init_CDROM_Access()
{
    if (CDFileClass::First == 0) {
        Debug_Printf("Calling Force_CD_Available\n");
        if (!CD().Force_Available(-1)) {
            Debug_Printf("Failed to find disk (user cancel)!\n");
            return false;
        }

        int error;
        do {
            error = CDFileClass::Set_Search_Drives("?:\\");
            switch (error) {
                case 1:
                    Unknown_004955D0(&unknown_palette_837e10, 0, 0);
                    WWMessageBox(L" ").Process(StringTable::Load_String("TXT_CD_ERROR1", 0, "D:\\Ra2\\Init.CPP", 0x104D),
                        StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\Init.CPP", 0x104D), 0, 0, false);
                    return false;

                case 2:
                    Unknown_004955D0(&unknown_palette_837e10, 0, 0);
                    if (WWMessageBox(L" ").Process(StringTable::Load_String("TXT_CD_DIALOG_1", 0, "D:\\Ra2\\Init.CPP", 0x1056),
                            StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\Init.CPP", 0x1056),
                            StringTable::Load_String("TXT_CANCEL", 0, "D:\\Ra2\\Init.CPP", 0x1056), 0, false) == 1) {
                        Debug_Printf("Failed to find disk (user cancel)!\n");
                        return false;
                    }
                    break;

                default:
                    if (!CD().Force_Available(CD::RequiredCD)) {
                        Debug_Printf("Failed to find disk (user cancel)!\n");
                        return false;
                    }
                    break;
            }
        } while (error);

        CD::Set_Required_CD(-1);
        if (!VolumeCheck()) {
            Debug_Printf("VolumeCheck FAILED!!\n");
            WWMessageBox(L" ").Process(StringTable::Load_String("TXT_CD_ERROR1", 0, "D:\\Ra2\\Init.CPP", 0x106F),
                StringTable::Load_String("TXT_OK", 0, "D:\\Ra2\\Init.CPP", 0x106F), 0, 0, false);
            return false;
        }

        int disk = Unknown_0046E4C0(60);
        if (!unknown_special_a40920.IsFromInstall && disk > -1) {
            unknown_special_a40920.IsFromInstall = unknown_ini_839740.Get_Bool("Intro", "Play", true);
        }

        WolRegistryOptions options;
        options.Read_004ff200();
        if (!options.unknown_004ff2b0() && !options.unknown_004ff2d0() && !options.unknown_004ff2c0()) {
            unknown_special_a40920.EscapeMovies = true;
        }

        if (unknown_special_a40920.IsFromInstall == true) {
            CCFileClass ra2_ini("RA2.INI");
            if (disk > -1) {
                unknown_special_a40920.IsFromInstall = unknown_ini_839740.Put_Bool("Intro", "Play", false);
            }
            unknown_ini_839740.Save(ra2_ini, false);
        }
    } else {
        Debug_Printf("-CD parameter was specified. No CD init - all files must be local\n");
        CD::Set_Required_CD(-2);
    }
    return true;
}


// void __fastcall Read_Battle_Files() -- 0x005127F0
// __fastcall (no parameters): as __cdecl VC6 schedules the object address loads after the argument pushes.
#include "shared/win32_find.hpp"
#include "shared/campaign.hpp"
#include "shared/crt_string.hpp"


// Reads the [Battles] of every BATTLE*.INI in the game folder (by its short name when it
// has one); BATTLE.INI itself is read from the mixfiles when the folder has none.
void __fastcall Read_Battle_Files()
{
    bool found = false;
    WIN32_FIND_DATA data;
    HANDLE handle = FindFirstFileA("BATTLE*.INI", &data);
    if (handle != INVALID_HANDLE_VALUE) {
        do {
            if ((data.dwFileAttributes & 0x116) == 0) {
                char const *name = data.cAlternateFileName;
                if (strlen(name) == 0) {
                    name = data.cFileName;
                }
                CCFileClass file(name);
                CCINIClass *ini = new CCINIClass;
                ini->Load(file, false, false);
                if (_stricmp(name, "BATTLE.INI") == 0) {
                    found = true;
                }
                CampaignClass::Read_Battles(*ini);
                delete ini;
            }
        } while (FindNextFileA(handle, &data));
        FindClose(handle);
    }
    if (!found) {
        CCFileClass file("BATTLE.INI");
        CCINIClass *ini = new CCINIClass;
        if (ini != 0) {
            ini->Load(file, false, false);
            CampaignClass::Read_Battles(*ini);
            delete ini;
        }
    }
}


// bool __fastcall Init_Rules() -- 0x005129C0
// Body 0x005129C0-0x00513070 (1713 bytes, ret at 0x00513070, nop padding to 0x00513080). Drafted by
// Init181 (then 45.98: the INIClass constructor 0x0051AD50 was not yet a row of this unit).
#include "shared/vector_class.hpp"
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"

// The VectorClass<CCINIClass *> / DynamicVectorClass<CCINIClass *> members this unit emits (the vtables
// 0x007A3C04 / 0x007A3C24 instantiate them) are the original's rows at 0x0051E0A0-0x0051E710:

int __stdcall Unknown_00513080(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

// Reads RULES.INI (or lets the player choose among the RULE*.INI files in the game folder,
// RULES.INI first), ART.INI, LANGRULE.INI and AI.INI, and copies the multiplayer dialog
// defaults into the session.
bool __fastcall Init_Rules()
{
    DynamicVectorClass<CCINIClass *> files;
    bool found = false;
    WIN32_FIND_DATA data;
    HANDLE handle = FindFirstFileA("RULE*.INI", &data);
    if (handle != INVALID_HANDLE_VALUE) {
        do {
            if ((data.dwFileAttributes & 0x116) == 0) {
                char const *name = data.cAlternateFileName;
                if (data.cAlternateFileName[0] == 0) {
                    name = data.cFileName;
                }
                CCFileClass file(name);
                CCINIClass *ini = new CCINIClass;
                ini->Load(file, false, false);
                if (_stricmp(name, "RULES.INI") == 0) {
                    found = true;
                    files.Add_Head(ini);
                } else {
                    files.Add(ini);
                }
            }
        } while (FindNextFileA(handle, &data));
        FindClose(handle);
        if (found) {
            goto have_rules;
        }
    }
    {
        CCFileClass file("RULES.INI");
        CCINIClass *ini = new CCINIClass;
        ini->Load(file, false, false);
        files.Add_Head(ini);
    }
have_rules:
    if (files.Count() <= 0) {
        return false;
    }

    CCFileClass artfile("ART.INI");
    if (ArtINI.Load(artfile, false, false) == 0) {
        Debug_Printf("Failed to load ART.INI!\n");
        return false;
    }

    CCINIClass *rules;
    if (files.Count() == 1) {
        rules = files[0];
    } else {
        int choice = DialogBoxParamA(ProgramInstance, MAKEINTRESOURCEA(0x93), MainWindow, Unknown_00513080, (LPARAM)&files);
        if (choice == -1) {
            choice = 0;
        }
        rules = files[choice];
    }
    unknown_ini_8396d0 = rules;
    Rules->Colors(*rules);
    Rules->Movies(ArtINI);
    Rules->Audio_Visual(*unknown_ini_8396d0);
    Rules->MPlayer(*unknown_ini_8396d0);

    unknown_session_a3d298.unknown_24 = Rules->Money;
    unknown_session_a3d298.UnitCount = Rules->UnitCount;
    unknown_house_setting_7d88d8 = Rules->TechLevel;
    unknown_session_a3d298.GameSpeed_30 = Rules->GameSpeed;
    unknown_session_a3d298.unknown_difficulty_40 = Rules->AIDifficulty;
    unknown_session_a3d298.AICount = Rules->AIPlayers;
    unknown_session_a3d298.unknown_28 = Rules->BridgeDestruction;
    unknown_session_a3d298.unknown_bases_20 = Rules->Bases;
    unknown_session_a3d298.unknown_crates_29 = Rules->Crates;
    unknown_session_a3d298.unknown_a6 = Rules->CaptureTheFlag;
    unknown_session_a3d298.unknown_a5 = Rules->HarvesterTruce;
    unknown_session_a3d298.unknown_34 = Rules->MultiEngineer;
    unknown_session_a3d298.unknown_a4 = Rules->unknown_111d;
    unknown_session_a3d298.unknown_2a = Rules->ShortGame;
    unknown_session_a3d298.unknown_2b = Rules->SuperWeaponsAllowed;
    unknown_session_a3d298.unknown_2c = Rules->BuildOffAlly;
    unknown_session_a3d298.unknown_fog_a7 = Rules->FogOfWar;
    unknown_session_a3d298.unknown_a8 = Rules->MCVRedeploys;

    CCFileClass langfile("LANGRULE.INI");
    if (langfile.Is_Available() == true) {
        CCINIClass lang;
        if (lang.Load(langfile, true, false) > 1) {
            return false;
        }
        Rules->Addition(lang);
    }

    for (int index = 0; index < files.Count(); index++) {
        if (files[index] != unknown_ini_8396d0) {
            delete files[index];
        }
    }

    CCFileClass aifile("AI.INI");
    unknown_ini_8397a0.Load(aifile, true, false);
    return true;
}


// int __stdcall Unknown_00513080(HWND, UINT, WPARAM, LPARAM) -- 0x00513080


void __fastcall On_WM_HELP_00736880(LPARAM lparam);            // 0x00736880 WM_HELP
void __fastcall unknown_dialog_context_menu_7368a0(WPARAM wparam);    // 0x007368A0 WM_CONTEXTMENU
void __fastcall unknown_dialog_init_737060(HWND window);              // 0x00737060

// The battle file chooser's dialog procedure (pointer only, 0x005129C0): the list 0x4A3 shows
// each INI's [General] Name; cancel (2) or 0x4A4 ends the dialog with the selection.
int __stdcall Unknown_00513080(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    char buffer[128];
    switch (message) {
    case 0x7B:
        unknown_dialog_context_menu_7368a0(wparam);
        return 0;
    case 0x53:
        On_WM_HELP_00736880(lparam);
        return 0;
    case 0x110:
        {
            unknown_dialog_init_737060(window);
            HWND list = GetDlgItem(window, 0x4A3);
            DynamicVectorClass<CCINIClass *> *files = (DynamicVectorClass<CCINIClass *> *)lparam;
            for (int index = 0; index < files->Count(); index++) {
                (*files)[index]->Get_String("General", "Name", "", buffer, sizeof(buffer));
                SendMessageA(list, 0x180, 0, (LPARAM)buffer);
            }
            SendMessageA(list, 0x186, 0, 0);
        }
        return 0;
    case 0x216:
        return unknown_moving_736d80(window, wparam, (Rect *)lparam);
    case 0x111:
        switch (wparam & 0xFFFF) {
        case 2:
        case 0x4A4:
            if ((unsigned short)(wparam >> 16) == 0) {
                HWND list = GetDlgItem(window, 0x4A3);
                EndDialog(window, SendMessageA(list, 0x188, 0, 0));
                DestroyWindow(window);
            }
            break;
        }
        break;
    }
    return 0;
}


// LRESULT __stdcall Unknown_00513200(HWND, UINT, WPARAM, LPARAM) -- 0x00513200
// A dialog procedure (no references found): ends the dialog with 0 for
// control 2, 1 for control 0x53A and 2 for control 0x53B.

int __stdcall Unknown_00513200(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    if (message == 0x111) {
        switch (wparam) {
        case 2:
            EndDialog(window, 0);
            return 0;
        case 0x53A:
            EndDialog(window, 1);
            return 0;
        case 0x53B:
            EndDialog(window, 2);
            return 0;
        }
    }
    return 0;
}


// 0x00513260: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_00513260(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00513260 {

void  FUN_00513260(void)
{
    return;
}

}  // namespace seed_00513260


// 0x00513270: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_00513270(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00513270 {

void  FUN_00513270(void)
{
    return;
}

}  // namespace seed_00513270


// 0x00513280: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_00513280(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00513280 {

void  FUN_00513280(void)
{
    return;
}

}  // namespace seed_00513280


// int __stdcall Unknown_00513290(HWND, UINT, WPARAM, LPARAM) -- 0x00513290
#include "shared/map_seed.hpp"

// A dialog procedure (pointer only): the common handler 0x005FFB00 first; WM_PAINT sends 0x4F0
// to item 0x71A; the init message enables item 0x689 only when a saved game exists; WM_COMMAND
// ids 0x688, 0x689, 0x68A, 0x579 and 0x686 store 8, 9, 0xA, 0xB and 0x12 in the int the window's
// long 8 points at.
int __stdcall Unknown_00513290(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    int *choice = (int *)GetWindowLongA(window, 8);
    switch (message) {
    case 0xF:
        {
            HWND item = GetDlgItem(window, 0x71A);
            SendMessageA(item, 0x4F0, 0, 0);
        }
        break;
    case 0x111:
        switch ((unsigned short)wparam) {
        case 0x688:
            *choice = 8;
            return 0;
        case 0x689:
            *choice = 9;
            return 0;
        case 0x68A:
            *choice = 0xA;
            return 0;
        case 0x579:
            *choice = 0xB;
            return 0;
        case 0x686:
            *choice = 0x12;
            return 0;
        }
        break;
    case 0x497:
        {
            HWND item = GetDlgItem(window, 0x689);
            if (item) {
                bool saved = LoadOptionsClass().Has_Saved_Games_0053c5c0();
                EnableWindow(item, saved);
            }
        }
        break;
    }
    return 0;
}


// int __stdcall Unknown_005133E0(HWND, UINT, WPARAM, LPARAM) -- 0x005133E0
// Body 0x005133E0-0x005134B5: code to 0x00513495, then the WM_COMMAND jump table (5 dwords at
// 0x00513498) and its byte index table (10 bytes at 0x005134AC).

// A dialog procedure (pointer only): the common handler 0x005FFB00 first; WM_PAINT sends 0x4F0
// to item 0x71A; WM_COMMAND ids 0x68D, 0x68E, 0x68F and 0x686 store 0xD, 0xE, 0xF and 0x12 in
// the int the window's long 8 points at.
int __stdcall Unknown_005133E0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    int *choice = (int *)GetWindowLongA(window, 8);
    switch (message) {
    case 0xF:
        {
            HWND item = GetDlgItem(window, 0x71A);
            SendMessageA(item, 0x4F0, 0, 0);
        }
        break;
    case 0x111:
        switch (wparam & 0xFFFF) {
        case 0x68D:
            *choice = 0xD;
            return 0;
        case 0x68E:
            *choice = 0xE;
            return 0;
        case 0x68F:
            *choice = 0xF;
            return 0;
        case 0x686:
            *choice = 0x12;
            return 0;
        }
        break;
    }
    return 0;
}


// int __stdcall Unknown_005134C0(HWND, UINT, WPARAM, LPARAM) -- 0x005134C0


// The movie list's last selection (0x007DB558).
extern int unknown_movie_choice_7db558;

int __stdcall Unknown_005134C0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    bool chosen = false;
    HWND list = GetDlgItem(window, 0x744);
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    int *choice = (int *)GetWindowLongA(window, 8);
    switch (message) {
    case 0x497:
        if (list != 0) {
            Options.Fill_Movie_List(list);
            if (SendMessageA(list, 0x186, unknown_movie_choice_7db558, 0) == -1) {
                SendMessageA(list, 0x186, 0, 0);
            }
        } else {
            if (choice != 0) {
                *choice = -1;
            }
            return 0;
        }
        break;
    case 0x111:
        switch (wparam & 0xFFFF) {
        case 0x686:
            if (choice != 0) {
                *choice = -1;
            }
            break;
        case 0x744:
            if ((unsigned short)(wparam >> 16) == 0) {
                chosen = true;
            }
            break;
        case 0x745:
            chosen = true;
            break;
        }
        break;
    }
    if (list != 0 && chosen) {
        int index = SendMessageA(list, 0x188, 0, 0);
        unknown_movie_choice_7db558 = index;
        if (index != -1) {
            int movie = SendMessageA(list, 0x199, index, 0);
            if (movie != 0 && choice != 0) {
                *choice = movie;
            }
        }
    }
    return 0;
}


// bool __fastcall Select_Game(bool fade) -- 0x005135F0
// Body 0x005135F0-0x005146F4 (code) plus its five jump tables 0x005146F4-0x00514794 (40 entries):
// 4516 bytes, padding to 0x005147A0. Callers 0x00481D03 and 0x00481EDA (Main_Game) pass the fade flag in cl.
// The main menu loop (Red Alert's Select_Game): resets the per-game globals, runs the main menu and its
// sub-dialogs until a game is chosen, then starts the scenario (retrying from the top on failure).
// Its campaign picker and cheat reset are file statics that /Ob2 inlines, in the shape of ra2-decomp
// Choose_Campaign and Cheat_Disable beside Select_Game 0x0052D9A0 (YR fuzzy twin, src/init.cpp):
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Draft by codex4 (init_select_game_current, 99.92); helpers split out by Scout855.
#include "shared/main_opt.hpp"
#include "shared/queue.hpp"
#include "shared/campaign_score.hpp"
#include "shared/house_type.hpp"
#include "shared/map_view.hpp"
#include "shared/tool_tip.hpp"
#include "shared/sound_play.hpp"
#include "shared/comm_buffer.hpp"
#include "shared/t_timer.hpp"
#include "shared/system_timer.hpp"
#include "shared/scenario_checkpoint.hpp"
#include "shared/modem_guest.hpp"
#include "shared/modem_host.hpp"
#include "shared/serial_progress.hpp"
#include "shared/null_modem.hpp"
#include "shared/ipx_address.hpp"
#include "shared/ipx_interface.hpp"
#include "shared/ipx_manager.hpp"
#include "shared/winsock_interface.hpp"
#include "shared/connection.hpp"
#include "shared/owner_talk.hpp"
#include "shared/lobby.hpp"
#include "shared/movie_globals.hpp"
#include "shared/movie_list.hpp"
#include "shared/campaign_choice.hpp"

#define Max(a, b) ((a) < (b) ? (b) : (a))

// The cheat codes the main menu types into (0x007DB500; 0x00516F00 matches them): a code whose
// unknown_c is clear has its typed text emptied before a multiplayer game.
#include "shared/init_cheat_codes.hpp"
// What the movie dialog (0x005134C0) returns: a movie's file name and the CD it is on.
struct UnknownMovieEntry {
    char const *Name;       // +0x00
    int unknown_4;          // +0x04
    int CD;                 // +0x08
};
extern char unknown_flag_a35db0;            // 0x00A35DB0
extern char unknown_flag_a35db1;            // 0x00A35DB1
extern char unknown_flag_a40c78;            // 0x00A40C78
extern bool unknown_flag_00866250;          // 0x00866250
extern int unknown_833680;                  // 0x00833680
extern int unknown_833684;                  // 0x00833684
extern int unknown_833688;                  // 0x00833688
extern int unknown_a3d570;                  // 0x00A3D570
extern int unknown_a3d574;                  // 0x00A3D574
extern int unknown_a3d258;                  // 0x00A3D258
extern int unknown_a3d23c;                  // 0x00A3D23C
extern bool unknown_flag_a40d10;            // 0x00A40D10
extern int unknown_a41698;                  // 0x00A41698
extern int unknown_7db55c;                  // 0x007DB55C
extern bool unknown_flag_7ce008;            // 0x007CE008
extern PaletteClass unknown_palette_837510; // 0x00837510
extern bool unknown_redraw_8523dc;

int __fastcall Unknown_00516F00(int timeout);
bool __fastcall Unknown_00514EE0();
int __stdcall Unknown_00513290(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
int __stdcall Unknown_005133E0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
int __stdcall Unknown_005134C0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
void __fastcall Read_Battle_Files();
bool __fastcall Load_Recording_Values(CCFileClass &file);
bool __fastcall Save_Recording_Values(CCFileClass &file);
void __fastcall Init_Random();
void __fastcall Unknown_005A02D0(char const *name, int theme, int unknown3, int unknown4, int unknown5, int unknown6);
void __fastcall Unknown_006F2C40();
void __fastcall Unknown_006F2D00();
bool __fastcall Unknown_00683BF0();
int __fastcall Unknown_0073A300(bool unknown1);
bool __fastcall Unknown_005BD070();
void __fastcall Unknown_00613E50();

void __fastcall Audio_Event_Handle_Clear(AudioEventHandle *handle);

static void Cheat_Disable()
{
    for (int code = 0; code < 4; code++) {
        if (!unknown_cheat_codes_7db500[code].AllowedInMultiplayer) {
            *unknown_cheat_codes_7db500[code].Flag = false;
        }
    }
}

static int Choose_Campaign()
{
    UnknownCampaignChoice choice;
    choice.Chosen = false;
    choice.Campaign = -1;
    if (Campaigns.Count() == 0) {
        Read_Battle_Files();
        if (Campaigns.Count() == 0) {
            return -1;
        }
    }
    Unknown_006F2C40();
    Audio_Event_Handle_Clear(&unknown_side_sound_a411e8);
    unknown_hover_time_a41680 = 0;
    unknown_hover_id_a41688 = 0;
    HWND window = unknown_create_dialog_5ff610(0x94, Unknown_005147A0, 0);
    if (window != 0) {
        SetWindowLongA(window, 8, (LONG)&choice);
        Unknown_00737080(window, MainWindow);
        unknown_dialog_5ff7c0(window);
        while (!choice.Chosen && unknown_6000d0() != true) {
            if (unknown_side_voc_7db4f8 != -1 && (unsigned __int64)unknown_hover_time_a41680 < (unsigned __int64)Audio_Now()) {
                Audio_Event_Handle_Stop(&unknown_side_sound_a411e8);
                Voc_Play(unknown_side_voc_7db4f8, 1.0f, 0x2000, &unknown_side_sound_a411e8);
                unknown_side_voc_7db4f8 = -1;
            }
            if (unknown_redraw_8523dc) {
                unknown_redraw_8523dc = false;
                unknown_5159b0(0, true);
                Unknown_004E0C90(false, unknown_surface_839954, 0);
            }
        }
        unknown_dialog_5ff6e0(window);
    }
    unknown_hover_time_a41680 = (unsigned __int64)Audio_Now() + 3000;
    while (Audio_Event_Handle_Event(&unknown_side_sound_a411e8) != 0 && (unsigned __int64)unknown_hover_time_a41680 > (unsigned __int64)Audio_Now()) {
        unknown_481fb0();
    }
    Unknown_006F2D00();
    return choice.Campaign;
}

bool __fastcall Select_Game(bool fade)
{
    Theme.Play_Song((ThemeType)Theme.From_Name("INTRO"));
top:
    {
        bool gameloaded = false;
        int selection;
        bool process = true;

        unknown_flag_a40958 = true;
        unknown_player_a35db4 = 0;
        DoList.Init();
        OutList.Init();
        Frame = 0;
        unknown_id_source_a3d290->MissionTimer = 0;
        unknown_id_source_a3d290->MissionTimer.Stop();
        unknown_id_source_a3d290->unknown_610 = 1;
        unknown_id_source_a3d290->Difficulty = 1;
        unknown_flag_a35db1 = 0;
        unknown_flag_a40c78 = 0;
        unknown_flag_00866250 = false;
        unknown_flag_a35db0 = 0;
        unknown_session_a3d298.unknown_2b0 = 0;
        Map.Unknown_00498F10(0);
        unknown_833680 = 0;
        unknown_833684 = 0;
        unknown_833688 = -1;
        unknown_a3d570 = 0;
        unknown_a3d574 = 0;
        unknown_a3d568 = 30;
        unknown_a3d258 = 0;
        unknown_a3d23c = 0;
        for (int i = 0; i < 4; i++) {
            unknown_session_a3d298.unknown_1fb4[i][unknown_session_a3d298.unknown_2300] = -1;
        }
        Map.Set_Default_Mouse(0, false);
        if (ToolTips != 0) {
            ToolTips->Enable(false);
        }
        if (unknown_session_a3d298.Type == 0) {
            selection = 0x12;
        } else {
            selection = (unknown_session_a3d298.Type == 4) ? 0x11 : 0x10;
        }
        if (!Debug_Map) {
            if (unknown_session_a3d298.Play && unknown_session_a3d298.RecordFile.Is_Available()) {
                if (unknown_session_a3d298.RecordFile.Open(1)) {
                    Load_Recording_Values(unknown_session_a3d298.RecordFile);
                    process = false;
                    Theme.Stop(true);
                } else {
                    unknown_session_a3d298.Play = false;
                }
            }
            while (process) {
                if (PacketTransport != 0) {
                    if (unknown_session_a3d298.ViaPacketRouter) {
                        if (unknown_connection_a409c4 != 0) {
                            unknown_OwnerTalkClass_00ab6d18.Unknown_0063FC90();
                        }
                        unknown_session_a3d298.ViaPacketRouter = false;
                    }
                    delete PacketTransport;
                    PacketTransport = 0;
                    Unknown_00514EE0();
                }
                if (selection == 0x12 && !unknown_flag_a40d05) {
                    selection = Unknown_00516F00(3600);
                }
                if (unknown_flag_a40d05) {
                    selection = 8;
                }
                if (selection != 7) {
                    Theme.Play_Song((ThemeType)Theme.From_Name("INTRO"));
                }
                unknown_481fb0();
                switch (selection) {
                case -1:
                    if (unknown_session_a3d298.Attract && unknown_session_a3d298.RecordFile.Is_Available()) {
                        unknown_session_a3d298.Play = true;
                        if (unknown_session_a3d298.RecordFile.Open(1)) {
                            Load_Recording_Values(unknown_session_a3d298.RecordFile);
                            process = false;
                            Theme.Stop(true);
                            break;
                        }
                        unknown_session_a3d298.Play = false;
                        selection = 0x12;
                    } else {
                        selection = 0x12;
                    }
                    break;
                case 1:
                    unknown_a72c68 = 0;
                    selection = unknown_dialog_box_5eab60(0x100, Unknown_00513290, 1);
                    break;
                case 2:
                    selection = 0x10;
                    unknown_session_a3d298.Type = 4;
                    IPXAddressClass::Set_Protocol(2);
                    break;
                case 3:
                    selection = 0x10;
                    unknown_session_a3d298.Type = 3;
                    IPXAddressClass::Set_Protocol(1);
                    break;
                case 4:
                    selection = unknown_dialog_box_5eab60(0x101, Unknown_005133E0, 1);
                    break;
                case 5:
                    selection = 0x12;
                    Main_Options_Dialog();
                    break;
                case 6: {
                    unsigned short const *text = StringTable::Load_String("GUI:ExitAreYouSure", 0, "D:\\Ra2\\Init.CPP", 0x96B);
                    unsigned short const *ok = StringTable::Load_String("GUI:Ok", 0, "D:\\Ra2\\Init.CPP", 0x96C);
                    unsigned short const *cancel = StringTable::Load_String("GUI:Cancel", 0, "D:\\Ra2\\Init.CPP", 0x96D);
                    unknown_5159b0(0, true);
                    if (WWMessageBox(L" ").Process(text, ok, cancel, 0, false) == 0) {
                        selection = 7;
                        Options.Save_Settings();
                    } else {
                        unknown_5159b0(0, true);
                        selection = 0x12;
                    }
                    break;
                }
                case 7: {
                    Theme.Stop(true);
                    long timeout = TickCount.Time() + 3000;
                    while (Theme.Still_Playing() && Speech_Is_Playing() && timeout > TickCount.Time()) {
                        unknown_481fb0();
                    }
                    Theme.Stop(false);
                    if (!unknown_flag_7ce008) {
                        Unknown_004955D0(&unknown_palette_837510, 30, 0);
                    }
                    return false;
                }
                case 13:
                    selection = 4;
                    if (unknown_flag_a40d10) {
                        if (unknown_a41698 <= 0) {
                            unknown_a41698 = unknown_names_a71050.Count();
                        }
                        if (unknown_a41698 == unknown_names_a71050.Count()) {
                            unknown_a41698--;
                        }
                        Unknown_005A07F0(unknown_a41698--, -1, 1, 1, 1);
                        unknown_5159b0(0, true);
                    } else {
                        Unknown_005A02D0("EMPRTRLR.BIK", -1, 1, 1, 1, 0);
                        unknown_5159b0(0, true);
                    }
                    break;
                case 14: {
                    UnknownMovieEntry *movie = (UnknownMovieEntry *)unknown_dialog_box_5eab60(0x129, Unknown_005134C0, 1);
                    if (movie != (UnknownMovieEntry *)-1 && movie != 0) {
                        if (!CD().Force_Available(movie->CD)) {
                            break;
                        }
                        Unknown_005A02D0(movie->Name, -1, 1, 1, 1, 0);
                        unknown_5159b0(0, true);
                    } else {
                        selection = 4;
                    }
                    break;
                }
                case 15:
                    selection = 4;
                    Unknown_004B37C0();
                    Theme.Queue_Song((ThemeType)Theme.From_Name("INTRO"));
                    unknown_5159b0(0, true);
                    break;
                case 8: {
                    new (&unknown_scenario_checkpoint_00855a90) EnvironmentClass;
                    unknown_id_source_a3d290->unknown_224c = Choose_Campaign();
                    if (unknown_id_source_a3d290->unknown_224c == -1) {
                        process = true;
                        selection = 1;
                        break;
                    }
                    switch (unknown_difficulty_a40b1c) {
                    case 0:
                        unknown_id_source_a3d290->unknown_610 = 2;
                        unknown_id_source_a3d290->Difficulty = 0;
                        break;
                    case 1:
                        unknown_id_source_a3d290->unknown_610 = 2;
                        unknown_id_source_a3d290->Difficulty = 1;
                        break;
                    case 2:
                        unknown_id_source_a3d290->unknown_610 = 1;
                        unknown_id_source_a3d290->Difficulty = 1;
                        break;
                    case 3:
                        unknown_id_source_a3d290->unknown_610 = 0;
                        unknown_id_source_a3d290->Difficulty = 1;
                        break;
                    case 4:
                        unknown_id_source_a3d290->unknown_610 = 0;
                        unknown_id_source_a3d290->Difficulty = 2;
                        break;
                    }
                    unknown_session_a3d298.Type = 0;
                    process = false;
                    break;
                }
                case 9:
                    unknown_a40774++;
                    Read_Battle_Files();
                    unknown_a40774--;
                    if (LoadOptionsClass().Load_Dialog_0053b190()) {
                        gameloaded = true;
                        process = false;
                    } else {
                        selection = 1;
                    }
                    break;
                case 10:
                    selection = 1;
                    break;
                case 11:
                    unknown_session_a3d298.Type = 5;
                case 16:
                case 17: {
                    Unknown_0066F3A0(&unknown_session_a3d298);
                    for (int house = 0; house < HouseTypes.Count(); house++) {
                        HouseTypes[house]->Read_INI(*unknown_ini_8396d0);
                    }
                    unknown_counter_a3fabc = 0;
                    switch (unknown_session_a3d298.Type) {
                    case 0:
                        selection = 0x12;
                        continue;
                    case 5:
                        if (!Unknown_00683BF0()) {
                            unknown_session_a3d298.Type = 0;
                            selection = 1;
                        }
                        break;
                    case 1:
                    case 2: {
                        Cheat_Disable();
                        unknown_session_a3f616 = true;
                        if (serial_manager_00a40a28.Num_Connections()) {
                            serial_manager_00a40a28.Init_Send_Queue();
                            ModemGuest guest;
                            if ((unknown_session_a3d298.Type == 2 && serial_local_player_00a3fb38 == 0) || (unknown_session_a3d298.Type == 1 && serial_local_player_00a3fb38 == 2)) {
                                if (!ModemHost().Dialog_00598e10()) {
                                    unknown_session_a3d298.Type = 0;
                                    selection = 0x12;
                                }
                            } else {
                                if (!guest.Dialog_00596010()) {
                                    unknown_session_a3d298.Type = 0;
                                    selection = 0x12;
                                }
                            }
                        } else {
                            selection = 0x12;
                            unknown_session_a3d298.Type = 0;
                            continue;
                        }
                        break;
                    }
                    case 4: {
                        Cheat_Disable();
                        Debug_Printf("Init.cpp - GAME_INTERNET\n");
                        memset(&unknown_session_a3d298.unknown_299c, 0, sizeof(unknown_session_a3d298.unknown_299c));
                        unknown_session_a3d298.unknown_3018 = 0;
                        bool reset = Unknown_00514EE0();
                        int result = Unknown_0073A300(selection == 0x11);
                        selection = 0x10;
                        unknown_session_a3f616 = false;
                        switch (result) {
                        case -2:
                        case 0:
                            delete PacketTransport;
                            PacketTransport = 0;
                            selection = 0x12;
                            unknown_session_a3d298.Type = 0;
                            Theme.Play_Song((ThemeType)Theme.From_Name("INTRO"));
                            break;
                        case -1:
                            Theme.Stop(true);
                            if (!unknown_flag_7ce008) {
                                Unknown_004955D0(&unknown_palette_837510, 30, 0);
                            }
                            delete PacketTransport;
                            PacketTransport = 0;
                            return false;
                        case 1:
                            Debug_Printf("Return from WOL to play a %d player game\n", unknown_session_a3d298.Players.Count());
                            if (reset) {
                                process = false;
                                unknown_7db55c = 2;
                            } else {
                                unknown_session_a3d298.Type = 0;
                                selection = 0x12;
                                delete PacketTransport;
                                PacketTransport = 0;
                                unknown_7db55c = -1;
                            }
                            break;
                        case 2:
                            delete PacketTransport;
                            PacketTransport = 0;
                            unknown_session_a3d298.Type = 0;
                            selection = 11;
                            break;
                        }
                        break;
                    }
                    }
                    switch (unknown_session_a3d298.Type) {
                    case 3: {
                        Cheat_Disable();
                        Unknown_0066F3A0(&unknown_session_a3d298);
                        if (PacketTransport != 0) {
                            delete PacketTransport;
                            PacketTransport = 0;
                        }
                        for (int house = 0; house < HouseTypes.Count(); house++) {
                            HouseTypes[house]->Read_INI(*unknown_ini_8396d0);
                        }
                        unknown_session_a3d298.Type = 3;
                        unknown_session_a3d2ac = 2;
                        if (PacketTransport == 0) {
                            PacketTransport = new IPXInterfaceClass;
                            PacketTransport->Init();
                            unknown_5159b0(0, true);
                            if (Init_Network_005bc460() && Unknown_005BD070()) {
                                process = false;
                            } else {
                                unknown_session_a3d298.Type = 0;
                                selection = 0x12;
                                delete PacketTransport;
                                PacketTransport = 0;
                            }
                        }
                        break;
                    }
                    case 1:
                    case 2:
                    case 4:
                    case 5:
                        process = false;
                        break;
                    }
                    break;
                }
                }
            }
        }
        WWKeyboard->Clear();
        Init_Random();
        Unknown_00613E50();
        if (!gameloaded && unknown_a3d8c8 == 0) {
            Debug_Printf("About to load a %d player game.\n", unknown_session_a3d298.Players.Count());
            WWMouse->Hide_Mouse();
            if (selection != 8) {
                if (!unknown_flag_7ce008) {
                    Unknown_004955D0(&unknown_palette_837510, 15, unknown_481fb0);
                }
                unknown_surface_839954->Fill(0);
                Unknown_004E0C90(true, unknown_surface_839954, 0);
            }
            WWMouse->Show_Mouse();
            if (unknown_session_a3d298.Type == 4) {
                for (int player = 0; player < 8; player++) {
                    unknown_session_a3d298.unknown_2ff4[player] = TickCount.Time();
                }
            }
            if (unknown_session_a3d298.Type == 0 && !unknown_flag_a40d05 && !unknown_session_a3d298.Play) {
                if (!Start_Scenario_0065bc70(Campaigns[unknown_id_source_a3d290->unknown_224c]->Scenario, true, unknown_id_source_a3d290->unknown_224c)) {
                    if (Debug_Map) {
                        return false;
                    }
                    goto top;
                }
            } else if (!Start_Scenario_0065bc70(unknown_id_source_a3d290->FileName, true, -1)) {
                if (Debug_Map) {
                    return false;
                }
                goto top;
            }
            if (unknown_session_a3d298.Record) {
                if (unknown_session_a3d298.RecordFile.Open(2)) {
                    Save_Recording_Values(unknown_session_a3d298.RecordFile);
                } else {
                    unknown_session_a3d298.Record = false;
                }
            }
            unknown_special_a40920.IsFromInstall = false;
        }
        Debug_Printf("IsTGrowth = %d\n", unknown_special_a40920.TiberiumGrows);
        Debug_Printf("IsTSpread = %d\n", unknown_special_a40920.TiberiumSpreads);
        if (unknown_session_a3d298.Type != 0 && unknown_session_a3d298.Type != 5 && !unknown_session_a3d298.Play) {
            unknown_session_a3d298.Create_Connections();
            if (unknown_session_a3d298.Type == 3) {
                unknown_ipx_manager_a40978.Set_Timing(Max(unknown_ipx_manager_a40978.unknown_526c30() + 2, 15), -1, 600, true);
                unknown_ipx_manager_a40978.unknown_slot14(Max(unknown_ipx_manager_a40978.unknown_526c30() + 2, 60), -1, 600);
            } else if (unknown_session_a3d298.Type == 4 && unknown_a3d580 == 0) {
                unknown_ipx_manager_a40978.Set_Timing(Max(unknown_ipx_manager_a40978.unknown_526c30() + 2, 60), -1, 600, true);
            }
        }
        unknown_481fb0();
        WWMouse->Hide_Mouse();
        if (!unknown_flag_7ce008) {
            Unknown_004955D0(&unknown_palette_837510, 15, unknown_481fb0);
        }
        unknown_surface_839954->Fill(0);
        Unknown_004E0C90(true, unknown_surface_839954, 0);
        WWMouse->Show_Mouse();
        unknown_surface_83995c = unknown_surface_839954;
        Map.Override_Mouse_Shape(0x13, false);
        Map.Revert_Mouse_Shape();
        if (!Debug_Map) {
            Map.unknown_slot54(1);
        }
        Map.Flag_To_Redraw(false);
        unknown_481fb0();
        WWMouse->Hide_Mouse();
        return true;
    }
}

#undef Max


// int __stdcall Unknown_005147A0(HWND, UINT, WPARAM, LPARAM) -- 0x005147A0
// Body 0x005147A0-0x00514ED2 (1842 bytes, linear sweep to the last ret 0x10, padding to 0x00514EE0;
// no jump tables: compare chains). The dialog procedure pointer is pushed at 0x00513BCE (Select_Game).
#include "shared/game_controls_dialog.hpp"


// The first scenario of the Allied, Soviet and boot camp campaigns ("all1", "sov1", "tut1"; 0x007DB540).
extern char const *unknown_campaign_names_7db540[3];
// The side sound the hovered button plays when chosen (-1 none; 0x007DB4F8) and the three
// side sounds (0x007DB54C Soviet, 0x007DB550 Allied, 0x007DB554 boot camp).
extern int unknown_side_voc_7db4f8;
extern int unknown_soviet_voc_7db54c;
extern int unknown_allied_voc_7db550;
extern int unknown_bootcamp_voc_7db554;
extern __int64 unknown_hover_time_a41680;     // 0x00A41680
extern int unknown_hover_id_a41688;           // 0x00A41688 the highlighted side button
extern char unknown_campaign_list_a4168d;     // 0x00A4168D show the campaign list
extern HWND unknown_pressed_a41690;           // 0x00A41690 the button the mouse went down on
extern AudioEventHandle unknown_side_sound_a411e8;   // 0x00A411E8
extern bool unknown_flag_a6ebd0;              // 0x00A6EBD0

// What the dialog's long 8 points at: the chosen campaign (-1 cancel) and whether one was chosen.

// The campaign choice dialog (pointer only): the Allied (0x6EA), Soviet (0x6EC) and boot camp
// (0x6EB) buttons highlight under the mouse and play their side sound when clicked; list 0x455
// holds the campaigns, slider 0x50F the difficulty.
int __stdcall Unknown_005147A0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    switch (message) {
    case 0x114:
        if ((HWND)lparam == GetDlgItem(window, 0x50F)) {
            char const *label = Difficulty_Labels[wparam >> 16];
            HWND text = GetDlgItem(window, 0x670);
            SendMessageA(text, 0x4B2, 0, (LPARAM)StringTable::Load_String(label, 0, "D:\\Ra2\\Init.CPP", 0x4A9));
            return 0;
        }
        break;
    case 0x111:
        switch (wparam & 0xFFFF) {
        case 0x40E:
            if ((unsigned short)(wparam >> 16) == 0) {
                UnknownCampaignChoice *choice = (UnknownCampaignChoice *)GetWindowLongA(window, 8);
                if (choice != 0) {
                    HWND list = GetDlgItem(window, 0x455);
                    if (list != 0) {
                        choice->Campaign = SendMessageA(list, 0x188, 0, 0);
                        choice->Chosen = true;
                    }
                }
                HWND slider = GetDlgItem(window, 0x50F);
                if (slider != 0) {
                    unknown_difficulty_a40b1c = SendMessageA(slider, 0x400, 0, 0);
                    return 0;
                }
            }
            break;
        case 0x686:
            {
                UnknownCampaignChoice *choice = (UnknownCampaignChoice *)GetWindowLongA(window, 8);
                if (choice != 0) {
                    choice->Campaign = -1;
                    choice->Chosen = true;
                    return 0;
                }
            }
            break;
        }
        break;
    case 0x84:
        if (unknown_pressed_a41690 == 0) {
            POINT point;
            point.x = lparam & 0xFFFF;
            point.y = (unsigned)lparam >> 16;
            RECT rect;
            GetWindowRect(window, &rect);
            point.x -= rect.left;
            point.y -= rect.top;
            HWND child = ChildWindowFromPointEx(window, point, 1);
            if (child != 0) {
                int id = GetDlgCtrlID(child);
                if (unknown_hover_id_a41688 != id) {
                    SendDlgItemMessageA(window, unknown_hover_id_a41688, 0x4D5, 0, 0);
                    SendDlgItemMessageA(window, unknown_hover_id_a41688, 0x4D4, 0, 0);
                    unknown_hover_time_a41680 = Audio_Now() + 500;
                    unknown_side_voc_7db4f8 = -1;
                    unknown_hover_id_a41688 = 0;
                }
                if (id == 0x6EA) {
                    if (unknown_hover_id_a41688 != id) {
                        unknown_side_voc_7db4f8 = unknown_allied_voc_7db550;
                    }
                    SendDlgItemMessageA(window, 0x6EA, 0x4D3, 0, 0);
                    unknown_hover_id_a41688 = 0x6EA;
                    return 0;
                }
                if (id == 0x6EC) {
                    if (unknown_hover_id_a41688 != id) {
                        unknown_side_voc_7db4f8 = unknown_soviet_voc_7db54c;
                    }
                    SendDlgItemMessageA(window, 0x6EC, 0x4D3, 0, 0);
                    unknown_hover_id_a41688 = 0x6EC;
                    return 0;
                }
                if (id == 0x6EB) {
                    if (unknown_hover_id_a41688 != id) {
                        unknown_side_voc_7db4f8 = unknown_bootcamp_voc_7db554;
                    }
                    SendDlgItemMessageA(window, 0x6EB, 0x4D3, 0, 0);
                    unknown_hover_id_a41688 = 0x6EB;
                    return 0;
                }
            } else if (unknown_hover_id_a41688 != 0) {
                SendDlgItemMessageA(window, unknown_hover_id_a41688, 0x4D5, 0, 0);
                SendDlgItemMessageA(window, unknown_hover_id_a41688, 0x4D4, 0, 0);
                return 0;
            }
        }
        break;
    case 0x201:
        {
            POINT point;
            point.x = lparam & 0xFFFF;
            point.y = (unsigned)lparam >> 16;
            RECT rect;
            GetWindowRect(window, &rect);
            point.x -= rect.left;
            point.y -= rect.top;
            HWND child = ChildWindowFromPointEx(window, point, 1);
            if (child != 0) {
                int id = GetDlgCtrlID(child);
                if (id == 0x6EA || id == 0x6EC || id == 0x6EB) {
                    unknown_pressed_a41690 = child;
                    SetCapture(window);
                    return 0;
                }
            }
        }
        break;
    case 0x202:
        {
            POINT point;
            point.x = lparam & 0xFFFF;
            point.y = (unsigned)lparam >> 16;
            RECT rect;
            GetWindowRect(window, &rect);
            point.x -= rect.left;
            point.y -= rect.top;
            HWND child = ChildWindowFromPointEx(window, point, 1);
            if (child != 0) {
                int id = GetDlgCtrlID(child);
                if (child == unknown_pressed_a41690) {
                    UnknownCampaignChoice *choice = (UnknownCampaignChoice *)GetWindowLongA(window, 8);
                    if (choice != 0) {
                        if (id == 0x6EA) {
                            choice->Campaign = CampaignClass::Find_Index(unknown_campaign_names_7db540[0]);
                            choice->Chosen = true;
                        } else if (id == 0x6EC) {
                            choice->Campaign = CampaignClass::Find_Index(unknown_campaign_names_7db540[1]);
                            choice->Chosen = true;
                        } else if (id == 0x6EB) {
                            choice->Campaign = CampaignClass::Find_Index(unknown_campaign_names_7db540[2]);
                            choice->Chosen = true;
                        }
                        if (choice->Chosen) {
                            unknown_flag_a6ebd0 = false;
                            SendDlgItemMessageA(window, unknown_hover_id_a41688, 0x4D5, 0, 0);
                            SendDlgItemMessageA(window, unknown_hover_id_a41688, 0x4D4, 0, 0);
                            HWND slider = GetDlgItem(window, 0x50F);
                            if (slider != 0) {
                                unknown_difficulty_a40b1c = SendMessageA(slider, 0x400, 0, 0);
                            }
                            if (unknown_side_voc_7db4f8 != -1) {
                                Audio_Event_Handle_Stop(&unknown_side_sound_a411e8);
                                Voc_Play(unknown_side_voc_7db4f8, 1.0f, 0x2000, &unknown_side_sound_a411e8);
                                unknown_side_voc_7db4f8 = -1;
                            }
                        }
                    }
                } else if (id == 0x6EA || id == 0x6EC || id == 0x6EB) {
                    if (unknown_hover_id_a41688 != 0) {
                        SendDlgItemMessageA(window, unknown_hover_id_a41688, 0x4D5, 0, 0);
                        SendDlgItemMessageA(window, unknown_hover_id_a41688, 0x4D4, 0, 0);
                    }
                    SendDlgItemMessageA(window, id, 0x4D5, 0, 0);
                    SendDlgItemMessageA(window, id, 0x4D4, 0, 0);
                    unknown_hover_id_a41688 = id;
                } else if (unknown_hover_id_a41688 != 0) {
                    SendDlgItemMessageA(window, unknown_hover_id_a41688, 0x4D5, 0, 0);
                    SendDlgItemMessageA(window, unknown_hover_id_a41688, 0x4D4, 0, 0);
                    unknown_hover_id_a41688 = 0;
                }
            } else if (unknown_hover_id_a41688 != 0) {
                SendDlgItemMessageA(window, unknown_hover_id_a41688, 0x4D5, 0, 0);
                SendDlgItemMessageA(window, unknown_hover_id_a41688, 0x4D4, 0, 0);
                unknown_hover_id_a41688 = 0;
            }
            if (GetCapture() == window) {
                ReleaseCapture();
            }
            unknown_pressed_a41690 = 0;
        }
        break;
    case 0x497:
        {
            unknown_soviet_voc_7db54c = VocClass::From_Name("SovietCampaignSelect");
            unknown_allied_voc_7db550 = VocClass::From_Name("AlliedCampaignSelect");
            unknown_bootcamp_voc_7db554 = VocClass::From_Name("BootCampSelect");
            HWND list = GetDlgItem(window, 0x455);
            if (unknown_campaign_list_a4168d) {
                if (list != 0) {
                    for (int i = 0; i < Campaigns.Count(); i++) {
                        SendMessageA(list, 0x4CD, 0, (LPARAM)Campaigns[i]->Description);
                    }
                    SendMessageA(list, 0x186, 0, 0);
                }
            } else {
                ShowWindow(list, 0);
                HWND button = GetDlgItem(window, 0x40E);
                ShowWindow(button, 0);
                EnableWindow(button, 0);
            }
            HWND slider = GetDlgItem(window, 0x50F);
            if (slider != 0) {
                PostMessageA(slider, 0x4AC, 0, 0);
                PostMessageA(slider, 0x406, 1, 0x20000);
                PostMessageA(slider, 0x405, 1, unknown_difficulty_a40b1c);
            }
            SendDlgItemMessageA(window, 0x6EA, 0x4D5, 0, 0);
            SendDlgItemMessageA(window, 0x6EA, 0x4D4, 0, 0);
            SendDlgItemMessageA(window, 0x6EC, 0x4D5, 0, 0);
            SendDlgItemMessageA(window, 0x6EC, 0x4D4, 0, 0);
            SendDlgItemMessageA(window, 0x6EB, 0x4D5, 0, 0);
            SendDlgItemMessageA(window, 0x6EB, 0x4D4, 0, 0);
            return 0;
        }
    }
    return 0;
}


// bool Unknown_00514EE0() -- 0x00514EE0
// Purpose: resets the network layer before a LAN or internet game: replaces the
// packet transport with a UDP interface, restarts listening, clears the session's
// connection tables and steps PortBase, then saves PortBase/PortPool to RA2.INI.
// Returns the result of 0x005BB540 (hypothesis: the network code initialised).
#include "shared/raw_file_class.hpp"
#include "shared/udp_interface.hpp"

extern int unknown_a71b28;                  // 0x00A71B28 RA2.INI [MultiPlayer] PortPool
bool __fastcall Unknown_005BB540();

bool __fastcall Unknown_00514EE0()
{
    if (PacketTransport != 0) {
        delete PacketTransport;
        PacketTransport = 0;
    }
    for (int house = 0; house < HouseTypes.Count(); house++) {
        HouseTypes[house]->Read_INI(*unknown_ini_8396d0);
    }
    if (PacketTransport == 0) {
        PacketTransport = new UDPInterfaceClass;
    }
    unknown_session_a3d2ac = 2;
    PacketTransport->Open_Socket(0);
    unknown_ipx_manager_a40978.Set_Timing(30, -1, 600, true);
    Debug_Printf("About to initialise the network code\n");
    bool result = Unknown_005BB540();
    PacketTransport->Start_Listening();
    memset(&unknown_session_a3d298.unknown_3022, 0, sizeof(unknown_session_a3d298.unknown_3022));
    unknown_session_a3d298.unknown_3020 = false;
    unknown_session_a3d298.unknown_3014 = false;
    unknown_session_a3d298.unknown_292c = 0;
    unknown_session_a3d298.unknown_3016 = 0;
    unknown_session_a3d298.unknown_3034 = 0;
    unknown_session_a3d298.unknown_303c = -1;
    unknown_session_a3d298.unknown_3044 = 20;
    unknown_session_a3d298.unknown_3048 = 0;
    unknown_session_a3d298.unknown_304c = 0;
    unknown_session_a3d298.unknown_3050 = -1;
    memset(unknown_session_a3d298.unknown_29bc, 0, sizeof(unknown_session_a3d298.unknown_29bc));
    memset(unknown_session_a3d298.unknown_2bbc, -1, sizeof(unknown_session_a3d298.unknown_2bbc));
    memset(unknown_session_a3d298.unknown_2dc0, 0, sizeof(unknown_session_a3d298.unknown_2dc0));
    memset(&unknown_session_a3d298.unknown_2fa0, 0, sizeof(unknown_session_a3d298.unknown_2fa0));
    unknown_session_a3d298.unknown_2dbc = 0;
    if (unknown_session_a3d298.unknown_2f9c != 0) {
        unknown_session_a3d298.PortBase = unknown_session_a3d298.unknown_2f9c;
    } else if (unknown_session_a3d298.unknown_2fc0) {
        unknown_session_a3d298.PortBase += 5;
        if (unknown_session_a3d298.PortBase >= 1750) {
            unknown_session_a3d298.PortBase = 1250;
        }
        unknown_session_a3d298.unknown_2fc0 = false;
    }
    RawFileClass file("RA2.INI");
    unknown_ini_839740.Put_Int("MultiPlayer", "PortBase", unknown_session_a3d298.PortBase);
    unknown_ini_839740.Put_Int("MultiPlayer", "PortPool", unknown_a71b28);
    unknown_ini_839740.Save(file, false);
    unknown_OwnerTalkClass_00ab6d18.Reset_0063f070();
    unknown_session_a3f616 = true;
    return result;
}


// void Unknown_00515100() -- 0x00515100
// Resets the movie options to their defaults and sets option 0.

void __cdecl Unknown_00515100()
{
    VQA_Unknown_0071DB20();
    VQA_Unknown_0071DB60(0);
}


// bool __fastcall Parse_Command_Line(int argc, char *argv[]) -- 0x00515110
// Body 0x00515110-0x0051570A (1531 bytes, nop padding to 0x00515710). TS's Parse_Command_Line with
// RA2's switches (-jabber/-str/-nostr, -noaudio, -record/-play, -DROP=, router and -WIN switches).
#include "shared/gscreen.hpp"
#include "shared/crt_tokens.hpp"


extern "C" char *__cdecl _strupr(char *string);
extern "C" char *__cdecl strstr(char const *string, char const *find);
extern "C" int __cdecl sscanf(char const *buffer, char const *format, ...);
extern "C" int __cdecl atoi(char const *string);
extern "C" int __cdecl isdigit(int character);
extern "C" int __cdecl toupper(int character);
extern "C" int __cdecl _putws(wchar_t const *string);

long __fastcall Obfuscate(char const *string);             // 0x005D8390
void __fastcall Set_Text_Option_006f9fb0(int value);        // 0x006F9FB0 (-jabber)
void __fastcall Set_Text_Option_006f9fe0(int value);        // 0x006F9FE0 (-str / -nostr)
extern int unknown_audio_7f4cc8;                // 0x007F4CC8 cleared by -noaudio
extern int unknown_record_mode_a41664;          // 0x00A41664 1 = -record, 2 = -play
extern bool unknown_extra_sync_info_ab62e8;     // 0x00AB62E8 set by -MPDEBUG
extern int unknown_router_b2ac30;               // 0x00B2AC30 1 = -DLINK1, 2 = -NETGEAR
extern bool unknown_router_a402d0;              // 0x00A402D0 cleared by -NOROUTER
extern bool unknown_session_a3f615;             // 0x00A3F615 cleared by -MESSAGES (TS's Session.NetProtect)
extern bool unknown_hicolor_7ce008;             // 0x007CE008 set by -16 (TS's HicolorFlag)
extern char unknown_flag_a40d0c;                // 0x00A40D0C set by -XQ (TS's Debug_Quiet)

#define PARM_INSTALL 0xD95C68A2

bool __fastcall Parse_Command_Line(int argc, char *argv[])
{
    unknown_special_a40920.Init();

    Debug_Map = false;

    for (int index = 1; index < argc; index++) {
        char *string;
        string = _strupr(argv[index]);

        if (_stricmp("-jabber", string) == 0) {
            Set_Text_Option_006f9fb0(1);
        } else if (_stricmp("-str", string) == 0) {
            Set_Text_Option_006f9fe0(1);
        } else if (_stricmp("-nostr", string) == 0) {
            Set_Text_Option_006f9fe0(0);
        } else if (_stricmp("-noaudio", string) == 0) {
            unknown_audio_7f4cc8 = 0;
        } else if (_stricmp("-record", string) == 0) {
            unknown_record_mode_a41664 = 1;
        } else if (_stricmp("-play", string) == 0) {
            unknown_record_mode_a41664 = 2;
        } else {
            if (_stricmp("/?", string) == 0 || _stricmp("-?", string) == 0 || _stricmp("-h", string) == 0 || _stricmp("/h", string) == 0) {
                _putws(StringTable::Load_String("TXT_OPTION_HELP_01", 0, "D:\\Ra2\\Init.CPP", 0xD0E));
                _putws(StringTable::Load_String("TXT_OPTION_HELP_02", 0, "D:\\Ra2\\Init.CPP", 0xD0F));
                _putws(StringTable::Load_String("TXT_OPTION_HELP_03", 0, "D:\\Ra2\\Init.CPP", 0xD10));
                _putws(StringTable::Load_String("TXT_OPTION_HELP_04", 0, "D:\\Ra2\\Init.CPP", 0xD11));
                _putws(StringTable::Load_String("TXT_OPTION_HELP_05", 0, "D:\\Ra2\\Init.CPP", 0xD12));
                _putws(StringTable::Load_String("TXT_OPTION_HELP_06", 0, "D:\\Ra2\\Init.CPP", 0xD13));
                return false;
            }

            bool processed = true;
            long ob = Obfuscate(string);
            switch (ob) {
                case PARM_INSTALL:
                    unknown_special_a40920.IsFromInstall = true;
                    break;

                default:
                    processed = false;
                    break;
            }
            if (processed) continue;

            if (strstr(string, "-CD")) {
                CD::IsLocal = true;
                CDFileClass::Set_Search_Drives(&string[3]);
                continue;
            }

            if (strstr(string, "-DESTNET")) {
                NetNumType net;
                NetNodeType node;

                int i = 0;
                char *p = strtok(string + 8, ".");
                while (p) {
                    int x;
                    sscanf(p, "%x", &x);
                    if (i < 4) {
                        net[i] = (char)x;
                    } else {
                        node[i - 4] = (char)x;
                    }
                    i++;
                    p = strtok(0, ".");
                }

                if (i >= 4) {
                    unknown_dest_net_set_a3f604 = 1;
                    memset(node, 0xFF, 6);
                    unknown_dest_net_a3f608 = IPXAddressClass(net, node, 0);
                }
                continue;
            }

            if (strstr(string, "-SOCKET")) {
                unsigned short socket;
                socket = (unsigned short)(atoi(string + strlen("SOCKET")));
                socket += (unsigned short)0x4000;
                if (socket >= 0x4000 && socket < 0x8000) {
                    unknown_ipx_manager_a40978.Set_Socket(socket);
                }
                continue;
            }

            WinsockInterfaceClass::unknown_76b9e0(0);

            if (strstr(string, "-MPDEBUG")) {
                unknown_extra_sync_info_ab62e8 = true;
                unknown_flag_a3d8c4 = true;
            }

            if (strstr(string, "-DROP=")) {
                WinsockInterfaceClass::unknown_76b9e0((unsigned short)atoi(string + strlen("-DROP=")));
                continue;
            }

            if (strstr(string, "-SPEEDCONTROL")) {
                unknown_flag_a40d84 = true;
            }
            if (strstr(string, "-DLINK1")) {
                unknown_router_b2ac30 = 1;
            }
            if (strstr(string, "-NETGEAR")) {
                unknown_router_b2ac30 = 2;
            }
            if (strstr(string, "-NOROUTER")) {
                unknown_router_a402d0 = false;
            }

            if (strstr(string, "-STEALTH")) {
                unknown_session_a3f614 = true;
                continue;
            }

            if (strstr(string, "-MESSAGES")) {
                unknown_session_a3f615 = false;
                continue;
            }

            if (strstr(string, "-ATTRACT")) {
                unknown_session_a3d298.Attract = true;
                continue;
            }

            if (isdigit(string[1])) {
                sscanf(string, "-%dX%d", &Options.ScreenWidth, &Options.ScreenHeight);
                continue;
            }

            if (strcmp(string, "-480") == 0) {
                Options.ScreenHeight = 480;
                continue;
            }

            if (strcmp(string, "-16") == 0) {
                unknown_hicolor_7ce008 = true;
                continue;
            }

            if (_stricmp(string, "-WIN") == 0) {
                unknown_flag_851cb0 = true;
                continue;
            }

            if (_strnicmp(string, "-X", strlen("-O")) == 0) {
                string += strlen("-X");
                while (*string) {
                    char code = *string++;
                    switch (toupper(code)) {
                        case 'Q':
                            unknown_flag_a40d0c = true;
                            break;

                        default:
                            _putws(StringTable::Load_String("TXT_INVALID", 0, "D:\\Ra2\\Init.CPP", 0xE4C));
                            return false;
                    }
                }
                continue;
            }
        }
    }
    return true;
}


// void __fastcall Init_Random() -- 0x00515710
// __fastcall (no parameters): as __cdecl VC6 schedules the object address loads after the argument pushes.
#include "shared/random_straw.hpp"
#include "shared/random2.hpp"


// Seeds the scenario's generator and the shared one (0x00839218) from Seed; a new
// single-player or skirmish game first picks Seed (Red Alert's Init_Random).
void __fastcall Init_Random()
{
    Debug_Printf("Init random number\n");
    if (unknown_a3d8c8 != 0 || (unknown_flags_a3f600 & 2) != 0 || unknown_function_522e80() != 0) {
        unknown_id_source_a3d290->RandomNumber = Seed;
        unknown_random_839218 = Seed;
        Debug_Printf("Seed is %08x\n", Seed);
        return;
    }
    if (unknown_game_mode_a3d298 == 0 || unknown_game_mode_a3d298 == 5) {
        SYSTEMTIME t;
        GetSystemTime(&t);
        CryptRandom.Seed_Byte(t.wMilliseconds);
        CryptRandom.Seed_Bit(t.wSecond);
        CryptRandom.Seed_Bit(t.wSecond >> 1);
        CryptRandom.Seed_Bit(t.wSecond >> 2);
        CryptRandom.Seed_Bit(t.wSecond >> 3);
        CryptRandom.Seed_Bit(t.wSecond >> 4);
        CryptRandom.Seed_Bit(t.wMinute);
        CryptRandom.Seed_Bit(t.wMinute >> 1);
        CryptRandom.Seed_Bit(t.wMinute >> 2);
        CryptRandom.Seed_Bit(t.wMinute >> 3);
        CryptRandom.Seed_Bit(t.wMinute >> 4);
        CryptRandom.Seed_Bit(t.wHour);
        CryptRandom.Seed_Bit(t.wDay);
        CryptRandom.Seed_Bit(t.wDayOfWeek);
        CryptRandom.Seed_Bit(t.wMonth);
        CryptRandom.Seed_Bit(t.wYear);
        if (CustomSeed != 0) {
            Seed = CustomSeed;
        } else {
            CryptRandom.Get(&Seed, sizeof(Seed));
            Seed = GetTickCount();
        }
    }
    Debug_Printf("Seed is %08x\n", Seed);
    unknown_id_source_a3d290->RandomNumber = Seed;
    unknown_random_839218 = Seed;
}


// void __fastcall unknown_5159b0(int unknown1, bool unknown2) -- 0x005159B0
// Body 0x005159B0-0x00515A79 (202 bytes, nop padding to 0x00515A80). unknown1 is really the surface to draw
// on (0: unknown_surface_839954) and unknown2 whether to blit it afterwards; the declaration in
// shared/unknown_globals.hpp keeps its int parameter until a rename spec changes the signature.
#include "shared/surface.hpp"
#include "shared/rect.hpp"


void __fastcall Fill_Surface_Rect_006f3a20(Surface *surface, Rect const &window, bool arg3);   // 0x006F3A20

// Clears the surface and redraws the menu background into it through a 16-bit work surface of the same
// size (0x006F3A20 draws it), then blits it when asked (name hypothesis: the main menu background).
void __fastcall unknown_5159b0(int unknown1, bool unknown2)
{
    Surface *surface = (Surface *)unknown1;
    if (surface == 0) {
        surface = unknown_surface_839954;
    }
    surface->Fill(0);
    int width = surface->Get_Width();
    int height = surface->Get_Height();
    Rect rect(0, 0, width, height);
    BSurface *work = new BSurface(width, height, 2);
    Fill_Surface_Rect_006f3a20(work, rect, false);
    surface->Copy_From(*work, false, true);
    delete work;
    if (unknown2) {
        Unknown_004E0C90(false, surface, 0);
    }
}


// void __fastcall Unknown_00515A80(char const *filename, bool blit) -- 0x00515A80
// Clears the hidden surface, draws the named picture on it with the palette
// 0x00838710 (0x00738190) and, when asked, blits it with the mouse.
#include "shared/palette.hpp"

extern PaletteClass unknown_palette_838710;
// Reads a picture file onto a surface with a palette (0x00738190; hypothesis).
void __fastcall Unknown_00738190(char const *filename, Surface *surface, PaletteClass *palette);

void __fastcall Unknown_00515A80(char const *filename, bool blit)
{
    unknown_surface_839954->Fill(0);
    Unknown_00738190(filename, unknown_surface_839954, &unknown_palette_838710);
    if (blit) {
        GScreenClass::Blit(true, unknown_surface_839954);
    }
}


// void __fastcall Unknown_00515AC0(bool blit) -- 0x00515AC0
// Fills the hidden surface with colour 0 and, when asked, blits it with the mouse.

void __fastcall Unknown_00515AC0(bool blit)
{
    unknown_surface_839954->Fill(0);
    if (blit) {
        GScreenClass::Blit(true, unknown_surface_839954);
    }
}


// bool Init_Expansion_Files() -- 0x00515AF0
// Opens every ECACHE*.MIX (cached) and ELOCAL*.MIX found in the game folder
// with the fast key and lists them (as Red Alert's Init_Expansion_Files with
// SC*.MIX and SS*.MIX). Body 0x00515AF0-0x00515C85 (406 bytes): the inventory
// start 0x00515B40 is inside it (mid-instruction; no reference).

bool __cdecl Init_Expansion_Files()
{
    WIN32_FIND_DATA data;
    HANDLE handle = FindFirstFileA("ECACHE*.MIX", &data);
    if (handle != INVALID_HANDLE_VALUE) {
        do {
            if ((data.dwFileAttributes & 0x116) == 0) {
                MixFileClass *mix = new MixFileClass(data.cFileName, &FastKey);
                if (mix != 0) {
                    unknown_expansion_mixes_837450.Add(mix);
                    mix->Cache();
                }
            }
        } while (FindNextFileA(handle, &data));
        FindClose(handle);
    }
    handle = FindFirstFileA("ELOCAL*.MIX", &data);
    if (handle != INVALID_HANDLE_VALUE) {
        do {
            if ((data.dwFileAttributes & 0x116) == 0) {
                MixFileClass *mix = new MixFileClass(data.cFileName, &FastKey);
                if (mix != 0) {
                    unknown_expansion_mixes_837450.Add(mix);
                }
            }
        } while (FindNextFileA(handle, &data));
        FindClose(handle);
    }
    return true;
}


// bool __fastcall Init_Bootstrap_Mixfiles() -- 0x00515C90
// __fastcall (no parameters): as __cdecl VC6 schedules the object address loads after the argument pushes.
// Body 0x00515C90-0x00515E6B (476 bytes; the inventory start 0x00515D00 is inside it).


// Opens the EXPAND99.MIX..EXPAND00.MIX found on disk, then RA2.MIX, CACHE.MIX (cached) and
// LOCAL.MIX, with any disk allowed meanwhile (Red Alert's Init_Bootstrap_Mixfiles).
bool __fastcall Init_Bootstrap_Mixfiles()
{
    int cd = CD::RequiredCD;
    CD::Set_Required_CD(-2);
    for (int index = 99; index >= 0; index--) {
        char name[64];
        sprintf(name, "EXPAND%02d.MIX", index);
        if (RawFileClass(name).Is_Available()) {
            MixFileClass *mix = new MixFileClass(name, &FastKey);
            if (mix != 0) {
                unknown_expansion_mixes_837450.Add(mix);
                Unknown_00496460(" %s", name);
            }
        }
    }

    unknown_ra2_mix_8374b8 = new MixFileClass("RA2.MIX", &FastKey);
    if (unknown_ra2_mix_8374b8 == 0) {
        return false;
    }
    Unknown_00496460(" CACHE.MIX");

    unknown_cache_mix_8374e8 = new MixFileClass("CACHE.MIX", &FastKey);
    if (unknown_cache_mix_8374e8 == 0 || !MixFileClass::Cache("CACHE.MIX")) {
        return false;
    }
    Unknown_00496460(" CACHE.MIX");

    unknown_local_mix_8374ec = new MixFileClass("LOCAL.MIX", &FastKey);
    if (unknown_local_mix_8374ec == 0) {
        return false;
    }
    Unknown_00496460(" LOCAL.MIX");
    CD::Set_Required_CD(cd);
    return true;
}


// bool __fastcall Init_Secondary_Mixfiles() -- 0x00515E70
// __fastcall (no parameters, like Init_Bootstrap_Mixfiles).


// Opens CONQUER.MIX, CAMEO.MIX, the maps (every MAPS*.MIX on a local install, else
// MAPS<disk>.MIX), MULTI.MIX and THEME.MIX, scans the themes and opens the movies the same way
// as the maps (Red Alert's Init_Secondary_Mixfiles).
bool __fastcall Init_Secondary_Mixfiles()
{
    if (CCFileClass("CONQUER.MIX").Is_Available()) {
        ConquerMix = new MixFileClass("CONQUER.MIX", &FastKey);
    }
    Unknown_00496460(" CONQUER.MIX");
    if (ConquerMix == 0) {
        return false;
    }

    if (CCFileClass("CAMEO.MIX").Is_Available()) {
        CameoMix = new MixFileClass("CAMEO.MIX", &FastKey);
    }
    Unknown_00496460(" CAMEO.MIX");
    if (CameoMix == 0) {
        return false;
    }

    int cd = Unknown_0046E4C0(60) + 1;
    char name[260];
    if (CD::IsLocal == true) {
        strcpy(name, "MAPS*.MIX");
        if (CDFileClass::Find_First_File(name) == true) {
            Unknown_00496460(" %s", name);
            MapsMix = new MixFileClass(name, &FastKey);
            while (CDFileClass::Find_Next_File(name) == true) {
                Unknown_00496460(" %s", name);
                MixFileClass *mix = new MixFileClass(name, &FastKey);
                if (mix != 0) {
                    MixFileClass *added = mix;
                    unknown_maps_mixes_837468.Add(added);
                }
            }
            CDFileClass::Find_Close();
        }
    } else {
        sprintf(name, "MAPS%02d.MIX", cd);
        if (CCFileClass(name).Is_Available()) {
            MapsMix = new MixFileClass(name, &FastKey);
        }
        Unknown_00496460(" %s", name);
    }
    if (MapsMix == 0) {
        return false;
    }

    if (CCFileClass("MULTI.MIX").Is_Available()) {
        MultiMix = new MixFileClass("MULTI.MIX", &FastKey);
    }
    Unknown_00496460(" MULTI.MIX");
    if (MultiMix == 0) {
        return false;
    }

    if (CCFileClass("THEME.MIX").Is_Available()) {
        ThemeMix = new MixFileClass("THEME.MIX", &FastKey);
    }
    Unknown_00496460(" THEME.MIX");
    unknown_sound_enabled_a40c18 = true;
    Theme.Scan();

    if (CD::IsLocal == true) {
        strcpy(name, "u:\\mixfiles\\MOVIES*.MIX");
        if (RawFileClass("MOVIES01.MIX").Is_Available()) {
            strcpy(name, "MOVIES*.MIX");
        }
        if (RawFileClass("MIXFILES\\MOVIES01.MIX").Is_Available()) {
            strcpy(name, "MIXFILES\\MOVIES*.MIX");
        }
        if (CDFileClass::Find_First_File(name) == true) {
            Unknown_00496460(" %s", name);
            MoviesMix = new MixFileClass(name, &FastKey);
            while (CDFileClass::Find_Next_File(name) == true) {
                Unknown_00496460(" %s", name);
                MixFileClass *mix = new MixFileClass(name, &FastKey);
                if (mix != 0) {
                    MixFileClass *added = mix;
                    unknown_movies_mixes_8374a0.Add(added);
                }
            }
            CDFileClass::Find_Close();
        }
    } else {
        sprintf(name, "MOVIES%02d.MIX", cd);
        if (CCFileClass(name).Is_Available()) {
            MoviesMix = new MixFileClass(name, &FastKey);
        }
        Unknown_00496460(" %s", name);
    }
    return MoviesMix ? true : false;
}


// void __fastcall Load_Title_Page() -- 0x00516560


// When the title page was shown plus five seconds (0x00A41668, Audio_Now time).
extern __int64 unknown_title_time_a41668;

// Shows the title shape (GLSS.SHP at 640 wide, else GLSL.SHP, in GLS.PAL) centred on the hidden
// surface with the copyright, brand and loading texts, and blits it (Red Alert's
// Load_Title_Page).
void __fastcall Load_Title_Page()
{
    CCFileClass file("GLS.PAL");
    ShapeFileStruct *shape = 0;
    PaletteClass *palette = 0;
    ConvertClass *drawer = 0;
    if (file.Is_Available()) {
        palette = (PaletteClass *)Load_Alloc_Data(file);
        file.Close();
        for (int index = 0; index < 256; index++) {
            RGBClass &color = (*palette)[index];
            color = RGBClass(color.Red << 2, color.Green << 2, color.Blue << 2);
        }
        if (palette != 0) {
            drawer = new ConvertClass(*palette, unknown_palette_837e10, unknown_surface_839950, 1, false);
            if (drawer != 0) {
                CCFileClass shapefile(unknown_8523e0 == 640 ? "GLSS.SHP" : "GLSL.SHP");
                if (shapefile.Is_Available()) {
                    shape = (ShapeFileStruct *)Load_Alloc_Data(shapefile);
                    shapefile.Close();
                }
            }
        }
    }

    RECT client;
    GetClientRect(MainWindow, &client);
    Rect area(0, 0, client.right - client.left, client.bottom - client.top);
    if (shape != 0) {
        Point2D xy((area.Width - shape->Width) / 2, (area.Height - shape->Height) / 2);
        CC_Draw_Shape(unknown_surface_839954, drawer, shape, 0, xy, area, 0, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    }

    wchar_t const *text = StringTable::Load_String("TXT_COPYRIGHT", 0, "D:\\Ra2\\Init.CPP", 4700);
    BitFontClass *font = Font_For_Style(0x16);
    Point2D xy(0, area.Height - 60);
    if (font != 0) {
        xy.X = area.Width - font->String_Pixel_Width(text, 0) - 50;
    }
    Plain_Text_Print(text, unknown_surface_839954, &area, &xy, 0xFFFF, 0, 0x16);

    text = StringTable::Load_String("GUI:WWBrand", 0, "D:\\Ra2\\Init.CPP", 4711);
    if (font != 0) {
        xy.X = area.Width - font->String_Pixel_Width(text, 0) - 50;
        xy.Y += font->Height + 3;
    }
    Plain_Text_Print(text, unknown_surface_839954, &area, &xy, 0xFFFF, 0, 0x16);

    text = StringTable::Load_String("GUI:LoadingEx", 0, "D:\\Ra2\\Init.CPP", 4721);
    xy.X = 10;
    xy.Y = 10;
    Plain_Text_Print(text, unknown_surface_839954, &area, &xy, 0xFFFF, 0, 0x16);

    GScreenClass::Blit(true, unknown_surface_839954);
    unknown_title_time_a41668 = Audio_Now() + 5000;
    if (palette != 0) {
        operator delete(palette);
    }
    if (shape != 0) {
        operator delete(shape);
    }
    delete drawer;
}


// bool __fastcall Init_Bulk_Data() -- 0x00516900
// Body 0x00516900-0x00516AF8 (505 bytes, nop padding to 0x00516B00). Called once from Init_Game (0x005125BE).
// Init_One_Time_Systems is TS's file-local helper; VC6 inlines it here (written out in this body the
// constant zero takes esi instead of ebx). 0x00521A90 is __fastcall: as __cdecl the CCFileClass address load
// follows the string push.
#include "shared/object_type.hpp"
#include "shared/unit.hpp"
#include "shared/aircraft_type.hpp"


extern UnknownObjectPair unknown_drop_pod_voxel_a40c80;    // 0x00A40C80 the drop pod voxel and motion (TS's DropPodVoxel)
void unknown_481fb0();                  // 0x00481FB0 once per frame
void Unknown_00458370();                // 0x00458370 empty (a type class One_Time in TS's order)
void Unknown_004E2E30();                // 0x004E2E30 empty (TS: HouseClass::One_Time)
void __fastcall Unknown_005DD760();                // 0x005DD760 (TS order: SpotLightClass::One_Time)
void __fastcall Unknown_00521A90();                // 0x00521A90 (TS order: IonBlastClass::One_Time)

// Runs the one-time system initialisations and loads the drop pod voxel (TS's Init_One_Time_Systems;
// inlined into its only caller).
static bool Init_One_Time_Systems()
{
    unknown_481fb0();
    Map.One_Time();
    Options.One_Time();
    unknown_session_a3d298.One_Time();
    ObjectTypeClass::One_Time();
    Unknown_00458370();
    ObjectTypeClass::One_Time();
    ObjectTypeClass::One_Time();
    ObjectTypeClass::One_Time();
    UnitTypeClass::One_Time();
    ObjectTypeClass::One_Time();
    ObjectTypeClass::One_Time();
    AircraftTypeClass::Init();
    Unknown_004E2E30();
    Unknown_005DD760();
    Unknown_00521A90();

    CCFileClass dropvxl("DPOD.VXL");
    unknown_drop_pod_voxel_a40c80.unknown_00 = new VoxelLibrary(&dropvxl, 0);
    if (unknown_drop_pod_voxel_a40c80.unknown_00 == 0) {
        Debug_Printf("Failed to create VoxLib!\n");
        return false;
    }
    CCFileClass dropmot("DPOD.HVA");
    unknown_drop_pod_voxel_a40c80.unknown_04 = new UnknownPairSecond(&dropmot);
    if (unknown_drop_pod_voxel_a40c80.unknown_04 == 0) {
        Debug_Printf("Failed to create MotLib!\n");
        return false;
    }
    return true;
}

// Caches CONQUER.MIX and CAMEO.MIX, then runs the one-time system initialisations
// (Red Alert's Init_Bulk_Data).
bool __fastcall Init_Bulk_Data()
{
    if (ConquerMix == 0 || !ConquerMix->Cache()) {
        return false;
    }
    if (CameoMix == 0 || !CameoMix->Cache()) {
        return false;
    }
    unknown_481fb0();
    return Init_One_Time_Systems();
}


// bool __fastcall Save_Recording_Values(CCFileClass &file) -- 0x00516B00
// Writes the values a recording starts with (as Red Alert's Save_Recording_Values; the mirror of
// Load_Recording_Values 0x00516BA0): the house setting, two unnamed globals, the scenario's tech
// level and name, the special and the options. Select_Game 0x005135F0 inlines it.

bool __fastcall Save_Recording_Values(CCFileClass &file)
{
    Debug_Printf("Saving recording values for scenario : %s\n", unknown_id_source_a3d290->FileName);
    file.Write(&unknown_house_setting_7d88d8, sizeof(unknown_house_setting_7d88d8));
    file.Write(&unknown_a40d3c, sizeof(unknown_a40d3c));
    file.Write(&unknown_id_source_a3d290->TechLevel, sizeof(unknown_id_source_a3d290->TechLevel));
    file.Write(unknown_id_source_a3d290->FileName, sizeof(unknown_id_source_a3d290->FileName));
    file.Write(&unknown_a40c38, sizeof(unknown_a40c38));
    file.Write(&unknown_special_a40920, sizeof(unknown_special_a40920));
    file.Write(&Options, sizeof(Options));
    return true;
}


// bool __fastcall Load_Recording_Values(CCFileClass &file) -- 0x00516BA0
// Reads the values a recording starts with (as Red Alert's
// Load_Recording_Values): the house setting, two unnamed globals, the
// scenario's tech level and name, the special and the options.

bool __fastcall Load_Recording_Values(CCFileClass &file)
{
    file.Read(&unknown_house_setting_7d88d8, sizeof(unknown_house_setting_7d88d8));
    file.Read(&unknown_a40d3c, sizeof(unknown_a40d3c));
    file.Read(&unknown_id_source_a3d290->TechLevel, sizeof(unknown_id_source_a3d290->TechLevel));
    file.Read(unknown_id_source_a3d290->FileName, sizeof(unknown_id_source_a3d290->FileName));
    file.Read(&unknown_a40c38, sizeof(unknown_a40c38));
    file.Read(&unknown_special_a40920, sizeof(unknown_special_a40920));
    file.Read(&Options, sizeof(Options));
    Debug_Printf("Loaded recording values for scenario : %s\n", unknown_id_source_a3d290->FileName);
    return true;
}


// int __stdcall Unknown_00516C40(HWND, UINT, WPARAM, LPARAM) -- 0x00516C40
#include "shared/win_stub.hpp"
#include "shared/cpu_info.hpp"


char *__fastcall unknown_build_machine_text_737ad0(char *buffer, int size);                   // 0x00737AD0
char *__fastcall Build_By_String_00737b00(char *buffer, int size);                      // 0x00737B00
void __fastcall unknown_clear_text_4955a0(char *text);                                        // 0x004955A0
extern char unknown_cpu_text_8015a5[];

// The version dialog's procedure (pointer only, 0x00516E70's dialog 0xD4): the common handler
// first; 0x497 fills item 0x5B5 with the title, the versions, the build, the system texts and
// the CPU line; WM_COMMAND ids 1 and 2 are stored in the int the window's long 8 points at.
int __stdcall Unknown_00516C40(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    char buffer[256];
    char time[128];
    char machine[128];
    char user[128];
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    int *choice = (int *)GetWindowLongA(window, 8);
    switch (message) {
    case 0x497:
        {
            HWND item = GetDlgItem(window, 0x5B5);
            SendMessageA(item, 0x4CD, 0, (LPARAM)StringTable::Load_String("TXT_SHORT_TITLE", 0, "D:\\Ra2\\Init.CPP", 5428));
            sprintf(buffer, "Version %s", Version_Name());
            SendMessageA(item, 0x4C8, 0, (LPARAM)buffer);
            sprintf(buffer, "Internal Version %s", VerNum.Version_Name());
            SendMessageA(item, 0x4C8, 0, (LPARAM)buffer);
            time[0] = 0;
            Build_Time_Text(time, sizeof(time));
            sprintf(buffer, "Release Build: %s by %s - %s", unknown_build_machine_text_737ad0(machine, sizeof(machine)),
                Build_By_String_00737b00(user, sizeof(user)), time);
            SendMessageA(item, 0x4C8, 0, (LPARAM)buffer);
            int family = 5;
            unsigned char mmx = 0;
            SendMessageA(item, 0x4C8, 0, (LPARAM)unknown_profile_cpu_text_a411fc);
            SendMessageA(item, 0x4C8, 0, (LPARAM)unknown_profile_ram_text_a41388);
            SendMessageA(item, 0x4C8, 0, (LPARAM)unknown_profile_vram_text_a41308);
            SendMessageA(item, 0x4C8, 0, (LPARAM)unknown_profile_blit_text_a41408);
            SendMessageA(item, 0x4C8, 0, (LPARAM)unknown_profile_text_a4127c);
            Get_CPU_Info(&family, &mmx, 0, 0);
            sprintf(buffer, "MMX %s, Vendor: %s", mmx ? "Yes" : "No", unknown_cpu_text_8015a5);
            SendMessageA(item, 0x4C8, 0, (LPARAM)buffer);
            unknown_clear_text_4955a0(buffer);
            SendMessageA(item, 0x4C8, 0, (LPARAM)buffer);
        }
        return 0;
    case 0x111:
        if ((int)(wparam & 0xFFFF) > 0 && (int)(wparam & 0xFFFF) <= 2) {
            *choice = wparam & 0xFFFF;
        }
        break;
    }
    return 0;
}


// void __fastcall Unknown_00516E70() -- 0x00516E70

extern bool unknown_redraw_8523dc;
int __stdcall Unknown_00516C40(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
void __fastcall Unknown_00517370(bool force);

void __fastcall Unknown_00516E70()
{
    int result = 0;
    HWND window = unknown_create_dialog_5ff610(0xD4, Unknown_00516C40, 0);
    if (window) {
        SetWindowLongA(window, 8, (LONG)&result);
        unknown_dialog_5ff7c0(window);
        while (!result) {
            if (unknown_6000d0() == true) {
                break;
            }
        Unknown_00517370(false);
        }
        unknown_dialog_5ff6e0(window);
    }
}


// int __fastcall Unknown_00516F00(int unknown_timeout) -- 0x00516F00
#include "shared/wstring.hpp"
#include "shared/win_util.hpp"


extern "C" int __cdecl isalnum(int character);
extern bool unknown_redraw_8523dc;
int __stdcall Unknown_005171D0(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
void __fastcall Unknown_00517370(bool force);

// A typed code (0x007DB500, four entries: PENGO, THETEAM, HIRES, TOGGLE) and the flag it toggles.
extern char unknown_cheat_text_a4169c[32];   // 0x00A4169C the letters typed so far

// Feeds a typed letter to the code recognizer: toggles a code's flag the moment its text appears in
// what was typed; anything but a letter or digit (or '~') clears it (Tiberian Sun's
// Cheat_Key_Process; inlined into its only caller).
static bool Cheat_Key_Process(char chr)
{
    if (!isalnum(chr) || chr == '~') {
        memset(unknown_cheat_text_a4169c, 0, sizeof(unknown_cheat_text_a4169c));
        return false;
    }

    char tmp[2];
    tmp[0] = chr;
    tmp[1] = 0;
    Unknown_00496460(tmp);

    int len = strlen(unknown_cheat_text_a4169c);
    if (len >= 31) {
        len = 0;
        unknown_cheat_text_a4169c[0] = 0;
    }
    unknown_cheat_text_a4169c[len] = (char)toupper(chr);

    for (int index = 0; index < 4; index++) {
        if (strstr(unknown_cheat_text_a4169c, unknown_cheat_codes_7db500[index].Code) != 0) {
            *unknown_cheat_codes_7db500[index].Flag = !*unknown_cheat_codes_7db500[index].Flag;
            memset(unknown_cheat_text_a4169c, 0, sizeof(unknown_cheat_text_a4169c));
            return true;
        }
    }
    return false;
}

// Runs the main menu dialog 0xE2 (procedure 0x005171D0) until a button sets the choice; letters
// typed meanwhile toggle the flags of the codes they spell. 7 when the disk or dialog fails.
// Select_Game005135F0 loads ECX=3600 at005138E4 and calls00516F00 at005138E9.
// This implementation ignores the argument; its timeout meaning is a hypothesis.
int __fastcall Unknown_00516F00(int unknown_timeout)
{
    int choice = 0x12;
    CD::Set_Required_CD(-1);
    if (!CD().Force_Available(CD::RequiredCD)) {
        return 7;
    }
    HWND window = unknown_create_dialog_5ff610(0xE2, Unknown_005171D0, 0);
    if (window != 0) {
        SetWindowLongA(window, 8, (LONG)&choice);
        unknown_center_window_737080(window, MainWindow);
        unknown_dialog_5ff7c0(window);
        SetFocus(MainWindow);
        unknown_5115f0(window);
        do {
            if (unknown_6000d0()) {
                choice = 7;
            }
            Unknown_00517370(false);
            if (WWKeyboard->Check()) {
                int key = (unsigned short)WWKeyboard->Get();
                if ((key & 0x800) == 0) {
                    if (Cheat_Key_Process((char)key)) {
                        Voc_Play(Rules->OptionsChanged, 1.0f, 0x2000, 0);
                        Unknown_00517370(true);
                    }
                }
            }
        } while (choice == 0x12);
        unknown_dialog_5ff6e0(window);
        SYSTEMTIME time;
        GetSystemTime(&time);
        CryptRandom.Seed_Byte((char)time.wMilliseconds);
    } else {
        choice = 7;
    }
    SetFocus(MainWindow);
    return choice;
}


// int __stdcall Unknown_005171D0(HWND, UINT, WPARAM, LPARAM) -- 0x005171D0



// The main menu dialog's procedure (pointer only, 0x00516F00's dialog 0xE2): the common handler
// first; WM_PAINT sends 0x4F0 to item 0x71A; 0x497 shows the version in item 0x71D; the
// WM_COMMAND buttons store their choice in the int the window's long 8 points at.
int __stdcall Unknown_005171D0(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    wchar_t buffer[128];
    int result = unknown_dialog_proc_5ffb00(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    int *choice = (int *)GetWindowLongA(window, 8);
    switch (message) {
    case 0x497:
        {
            HWND item = GetDlgItem(window, 0x71D);
            swprintf(buffer, L"%s %s", StringTable::Load_String("GUI:Version", 0, "D:\\Ra2\\Init.CPP", 5723),
                Narrow_To_Wide_006fa180(VerNum.Version_Name()));
            SendMessageA(item, 0x4B2, 0, (LPARAM)buffer);
        }
        return 0;
    case 0x111:
        switch ((int)(wparam & 0xFFFF)) {
        case 0x578:
            *choice = 3;
            return 0;
        case 0x55C:
            *choice = 5;
            return 0;
        case 0x3EE:
            *choice = 6;
            return 0;
        case 0x683:
            *choice = 1;
            return 0;
        case 0x686:
            *choice = 4;
            return 0;
        case 0x684:
            *choice = 2;
            return 0;
        }
        break;
    case 0xF:
        {
            HWND item = GetDlgItem(window, 0x71A);
            SendMessageA(item, 0x4F0, 0, 0);
        }
        break;
    }
    return 0;
}


// void __fastcall Unknown_00517370(bool force) -- 0x00517370
// When the screen is marked for a redraw (0x008523DC) or force: clears the
// mark, clears the hidden surface (0x005159B0) and blits it.

extern bool unknown_redraw_8523dc;

void __fastcall Unknown_00517370(bool force)
{
    if (unknown_redraw_8523dc || force) {
        unknown_redraw_8523dc = false;
        unknown_5159b0(0, true);
        GScreenClass::Blit(false, unknown_surface_839954);
    }
}


class Leaf_5173a0 { public: void get(); };
void Leaf_5173a0::get() {  }


// 0x005173b0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_005173b0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005173b0 {

void  FUN_005173b0(void)
{
    return;
}

}  // namespace seed_005173b0


// void unknown_init_commands_5173c0() -- 0x005173C0
// Body 0x005173C0-0x0051923F (7807 bytes, linear sweep to the ret, padding to 0x00519240; no jump tables).
// The 46 first Adds expand inline; the rest call DynamicVectorClass<CommandClass const *>::Add
// (0x0051E360) on the inline budget, which also leaves CreateTeamCommandClass(1) and the implicit
// constructors of SetInfantryTab and ScreenCapture inline (Westwood's int operator[] and
// `(*this)[ActiveCount++] = object` Add are what reproduce that budget).
#include "shared/command.hpp"

// Reads KEYBOARD.INI's hotkeys (0x00519240; defined later in this unit).
bool __fastcall Load_Keyboard_Hotkeys();

// Creates and registers every keyboard command (the two multiplayer debug commands only with
// unknown_flag_a3d8c4), loads KEYBOARD.INI and then binds Delete, Escape and Space to the
// Delete, Options and Center-on-event commands.
void unknown_init_commands_5173c0()
{
    if (unknown_flag_a3d8c4) {
        unknown_commands_832358.Add(new MultiplayerDebugCommandClass);
        unknown_commands_832358.Add(new MultiplayerSyncCommandClass);
    }
    unknown_commands_832358.Add(new FollowCommandClass);
    unknown_commands_832358.Add(new View1CommandClass);
    unknown_commands_832358.Add(new View2CommandClass);
    unknown_commands_832358.Add(new View3CommandClass);
    unknown_commands_832358.Add(new View4CommandClass);
    unknown_commands_832358.Add(new SetView1CommandClass);
    unknown_commands_832358.Add(new SetView2CommandClass);
    unknown_commands_832358.Add(new SetView3CommandClass);
    unknown_commands_832358.Add(new SetView4CommandClass);
    CommandClass *options = new OptionsCommandClass;
    unknown_commands_832358.Add(options);
    unknown_commands_832358.Add(new SidebarUpCommandClass);
    unknown_commands_832358.Add(new LSidebarUpCommandClass);
    unknown_commands_832358.Add(new RSidebarUpCommandClass);
    unknown_commands_832358.Add(new SidebarDownCommandClass);
    unknown_commands_832358.Add(new LSidebarDownCommandClass);
    unknown_commands_832358.Add(new RSidebarDownCommandClass);
    unknown_commands_832358.Add(new SidebarPageUpCommandClass);
    unknown_commands_832358.Add(new LSidebarPageUpCommandClass);
    unknown_commands_832358.Add(new RSidebarPageUpCommandClass);
    unknown_commands_832358.Add(new SidebarPageDownCommandClass);
    unknown_commands_832358.Add(new LSidebarPageDownCommandClass);
    unknown_commands_832358.Add(new RSidebarPageDownCommandClass);
    CommandClass *center_event = new CenterREventCommandClass;
    unknown_commands_832358.Add(center_event);
    unknown_commands_832358.Add(new BeaconPlacementCommandClass);
    unknown_commands_832358.Add(new ToggleSellCommandClass);
    unknown_commands_832358.Add(new ToggleRepairCommandClass);
    unknown_commands_832358.Add(new AllianceCommandClass);
    unknown_commands_832358.Add(new CenterBaseCommandClass);
    unknown_commands_832358.Add(new CenterViewCommandClass);
    unknown_commands_832358.Add(new ScatterCommandClass);
    unknown_commands_832358.Add(new GuardCommandClass);
    unknown_commands_832358.Add(new StopCommandClass);
    unknown_commands_832358.Add(new AllToCheerCommandClass);
    unknown_commands_832358.Add(new DeployCommandClass);
    unknown_commands_832358.Add(new PrevObjectCommandClass);
    unknown_commands_832358.Add(new NextObjectCommandClass);
    unknown_commands_832358.Add(new PlanningModeCommandClass);
    unknown_commands_832358.Add(new CombatantSelectCommandClass);
    unknown_commands_832358.Add(new TypeSelectCommandClass);
    unknown_commands_832358.Add(new HealthNavCommandClass);
    unknown_commands_832358.Add(new VeterancyNavCommandClass);
    unknown_commands_832358.Add(new SetStructureTabCommandClass);
    unknown_commands_832358.Add(new SetDefenseTabCommandClass);
    unknown_commands_832358.Add(new SetUnitTabCommandClass);
    unknown_commands_832358.Add(new SetInfantryTabCommandClass);
    int team_number_1 = 1;
    unknown_commands_832358.Add(new CreateTeamCommandClass(team_number_1));
    int team_number_2 = 2;
    unknown_commands_832358.Add(new CreateTeamCommandClass(team_number_2));
    int team_number_3 = 3;
    unknown_commands_832358.Add(new CreateTeamCommandClass(team_number_3));
    int team_number_4 = 4;
    unknown_commands_832358.Add(new CreateTeamCommandClass(team_number_4));
    int team_number_5 = 5;
    unknown_commands_832358.Add(new CreateTeamCommandClass(team_number_5));
    unknown_commands_832358.Add(new CreateTeamCommandClass(6));
    unknown_commands_832358.Add(new CreateTeamCommandClass(7));
    unknown_commands_832358.Add(new CreateTeamCommandClass(8));
    unknown_commands_832358.Add(new CreateTeamCommandClass(9));
    unknown_commands_832358.Add(new CreateTeamCommandClass(10));
    unknown_commands_832358.Add(new SelectTeamCommandClass(1));
    unknown_commands_832358.Add(new SelectTeamCommandClass(2));
    unknown_commands_832358.Add(new SelectTeamCommandClass(3));
    unknown_commands_832358.Add(new SelectTeamCommandClass(4));
    unknown_commands_832358.Add(new SelectTeamCommandClass(5));
    unknown_commands_832358.Add(new SelectTeamCommandClass(6));
    unknown_commands_832358.Add(new SelectTeamCommandClass(7));
    unknown_commands_832358.Add(new SelectTeamCommandClass(8));
    unknown_commands_832358.Add(new SelectTeamCommandClass(9));
    unknown_commands_832358.Add(new SelectTeamCommandClass(10));
    unknown_commands_832358.Add(new AddTeamCommandClass(1));
    unknown_commands_832358.Add(new AddTeamCommandClass(2));
    unknown_commands_832358.Add(new AddTeamCommandClass(3));
    unknown_commands_832358.Add(new AddTeamCommandClass(4));
    unknown_commands_832358.Add(new AddTeamCommandClass(5));
    unknown_commands_832358.Add(new AddTeamCommandClass(6));
    unknown_commands_832358.Add(new AddTeamCommandClass(7));
    unknown_commands_832358.Add(new AddTeamCommandClass(8));
    unknown_commands_832358.Add(new AddTeamCommandClass(9));
    unknown_commands_832358.Add(new AddTeamCommandClass(10));
    unknown_commands_832358.Add(new CenterTeamCommandClass(1));
    unknown_commands_832358.Add(new CenterTeamCommandClass(2));
    unknown_commands_832358.Add(new CenterTeamCommandClass(3));
    unknown_commands_832358.Add(new CenterTeamCommandClass(4));
    unknown_commands_832358.Add(new CenterTeamCommandClass(5));
    unknown_commands_832358.Add(new CenterTeamCommandClass(6));
    unknown_commands_832358.Add(new CenterTeamCommandClass(7));
    unknown_commands_832358.Add(new CenterTeamCommandClass(8));
    unknown_commands_832358.Add(new CenterTeamCommandClass(9));
    unknown_commands_832358.Add(new CenterTeamCommandClass(10));
    unknown_commands_832358.Add(new TauntCommandClass(1));
    unknown_commands_832358.Add(new TauntCommandClass(2));
    unknown_commands_832358.Add(new TauntCommandClass(3));
    unknown_commands_832358.Add(new TauntCommandClass(4));
    unknown_commands_832358.Add(new TauntCommandClass(5));
    unknown_commands_832358.Add(new TauntCommandClass(6));
    unknown_commands_832358.Add(new TauntCommandClass(7));
    unknown_commands_832358.Add(new TauntCommandClass(8));
    unknown_commands_832358.Add(new ScreenCaptureCommandClass);
    unknown_commands_832358.Add(new PageUserCommandClass);
    unknown_commands_832358.Add(new CursorPositionCommandClass);
    CommandClass *del = new DeleteCommandClass;
    unknown_commands_832358.Add(del);
    Load_Keyboard_Hotkeys();
    if (unknown_hotkeys_832380.Is_Present(0x2E)) {
        unknown_hotkeys_832380.Remove_Index(0x2E);
    }
    unknown_hotkeys_832380.Add_Index(0x2E, del);
    if (unknown_hotkeys_832380.Is_Present(0x1B)) {
        unknown_hotkeys_832380.Remove_Index(0x1B);
    }
    unknown_hotkeys_832380.Add_Index(0x1B, options);
    if (unknown_hotkeys_832380.Is_Present(0x20)) {
        unknown_hotkeys_832380.Remove_Index(0x20);
    }
    unknown_hotkeys_832380.Add_Index(0x20, center_event);
}


// bool __fastcall Load_Keyboard_Hotkeys() -- 0x00519240
// __fastcall (no parameters): as __cdecl VC6 schedules the object address loads after the argument pushes.
// Body 0x00519240-0x00519471 (562 bytes; the inventory's 552 stops before the failure path's epilogue).


extern "C" int __cdecl strcmp(char const *left, char const *right);

// Reads the [Hotkey] section of KEYBOARD.INI (command name = key) into the hotkey table,
// replacing it; false when the file cannot be read.
bool __fastcall Load_Keyboard_Hotkeys()
{
    CCINIClass ini;
    CCFileClass file("KEYBOARD.INI");
    if (ini.Load(file, false, false)) {
        unknown_hotkeys_832380.Clear();
        for (int index = 0; index < ini.Entry_Count("Hotkey"); index++) {
            char const *entry = ini.Get_Entry("Hotkey", index);
            int key = ini.Get_Int("Hotkey", entry, 0);
            for (int command = 0; command < unknown_commands_832358.Count(); command++) {
                if (strcmp(unknown_commands_832358[command]->Get_Name(), entry) == 0) {
                    CommandClass *found = (CommandClass *)unknown_commands_832358[command];
                    if (found != 0 && key != 0) {
                        unknown_hotkeys_832380.Add_Index(key, found);
                    }
                    break;
                }
            }
    }
    return true;
    }
    Debug_Printf("Unable to load KEYBOARD.INI\n");
    return false;
}


// void __fastcall Execute_Command_By_Name(char const *name) -- 0x00519480
// Executes (key 0) the first registered command whose name is name.

extern "C" int __cdecl strcmp(char const *left, char const *right);

void __fastcall Execute_Command_By_Name(char const *name)
{
    for (int index = 0; index < unknown_commands_832358.Count(); index++) {
        if (strcmp(unknown_commands_832358[index]->Get_Name(), name) == 0) {
            unknown_commands_832358[index]->Execute(0);
            return;
        }
    }
}


// bool __fastcall Allocate_Surfaces(Rect *hidden, Rect *composite, Rect *tile, Rect *sidebar, bool hidden_first)
// -- 0x00519500. Body 0x00519500-0x0051995D (1118 bytes, nop padding to 0x00519960).
// Declared in shared/main_opt.hpp (its last parameter is named `clear` there).


extern bool unknown_tile_sysmem_7f4cc4;     // 0x007F4CC4 composite and tile surfaces may live in video memory

// The DSurface constructor's flag as the original passes it: an int 0 or 1 taken through an if.
static inline int Memory_Flag(bool system_memory)
{
    if (system_memory) {
        return 1;
    }
    return 0;
}

// Frees the five off-screen surfaces and makes new ones for the given rectangles: the hidden surface
// (before the others when hidden_first, else after the sidebar), composite and tile (both moved to system
// memory when only one of them got video memory), sidebar (video memory only with AllowVRAMSidebar)
// and the alternate surface in system memory (TS's Allocate_Surfaces).
bool __fastcall Allocate_Surfaces(Rect *hidden, Rect *composite, Rect *tile, Rect *sidebar, bool hidden_first)
{
    Debug_Printf("Allocating new surfaces\n");

    if (unknown_surface_839958 != 0) {
        Debug_Printf("Deleting AlternateSurface\n");
        delete unknown_surface_839958;
        unknown_surface_839958 = 0;
    }
    if (unknown_surface_839954 != 0) {
        Debug_Printf("Deleting HiddenSurface\n");
        delete unknown_surface_839954;
        unknown_surface_839954 = 0;
    }
    if (unknown_surface_839964 != 0) {
        Debug_Printf("Deleting CompositeSurface\n");
        delete unknown_surface_839964;
        unknown_surface_839964 = 0;
    }
    if (unknown_surface_839944 != 0) {
        Debug_Printf("Deleting TileSurface\n");
        delete unknown_surface_839944;
        unknown_surface_839944 = 0;
    }
    if (unknown_surface_839948 != 0) {
        Debug_Printf("Deleting SidebarSurface\n");
        delete unknown_surface_839948;
        unknown_surface_839948 = 0;
    }

    bool tile_sysmem = !unknown_tile_sysmem_7f4cc4;
    bool sidebar_sysmem = !Options.AllowVRAMSidebar;

    if (hidden_first && hidden->Is_Valid()) {
        unknown_surface_839954 = new DSurface(hidden->Width, hidden->Height, false);
        unknown_surface_839954->Fill(0);
        Debug_Printf("HiddenSurface (%dx%d) %s\n", hidden->Width, hidden->Height,
                     ((DSurface *)unknown_surface_839954)->Is_Video_Ram() ? "VRAM" : "SYSTEM MEMORY");
    }

    if (composite->Is_Valid()) {
        unknown_surface_839964 = new DSurface(composite->Width, composite->Height, Memory_Flag(tile_sysmem));
        unknown_surface_839964->Fill(0);
        Debug_Printf("CompositeSurface (%dx%d) %s\n", composite->Width, composite->Height,
                     ((DSurface *)unknown_surface_839964)->Is_Video_Ram() ? "VRAM" : "SYSTEM MEMORY");
    }

    if (tile->Is_Valid()) {
        unknown_surface_839944 = new DSurface(tile->Width, tile->Height, Memory_Flag(tile_sysmem));
        unknown_surface_839944->Fill(0);
        Debug_Printf("TileSurface (%dx%d) %s\n", tile->Width, tile->Height,
                     ((DSurface *)unknown_surface_839944)->Is_Video_Ram() ? "VRAM" : "SYSTEM MEMORY");
    }

    if (unknown_surface_839964 != 0 && unknown_surface_839944 != 0) {
        bool composite_vram = ((DSurface *)unknown_surface_839964)->Is_Video_Ram();
        bool tile_vram = ((DSurface *)unknown_surface_839944)->Is_Video_Ram();
        if (composite_vram ^ tile_vram) {
            delete unknown_surface_839964;
            delete unknown_surface_839944;
            Debug_Printf("Moving TileSurface and CompositeSurface into system memory\n");
            unknown_surface_839964 = new DSurface(composite->Width, composite->Height, true);
            unknown_surface_839944 = new DSurface(tile->Width, tile->Height, true);
        }
    }

    if (sidebar->Is_Valid()) {
        unknown_surface_839948 = new DSurface(sidebar->Width, sidebar->Height, Memory_Flag(sidebar_sysmem));
        unknown_surface_839948->Fill(0);
        Debug_Printf("SidebarSurface (%dx%d) %s\n", sidebar->Width, sidebar->Height,
                     ((DSurface *)unknown_surface_839948)->Is_Video_Ram() ? "VRAM" : "SYSTEM MEMORY");
    }

    if (!hidden_first && hidden->Is_Valid()) {
        unknown_surface_839954 = new DSurface(hidden->Width, hidden->Height, false);
        unknown_surface_839954->Fill(0);
        Debug_Printf("HiddenSurface (%dx%d) %s\n", hidden->Width, hidden->Height,
                     ((DSurface *)unknown_surface_839954)->Is_Video_Ram() ? "VRAM" : "SYSTEM MEMORY");
    }

    if (hidden->Is_Valid()) {
        unknown_surface_839958 = new DSurface(hidden->Width, hidden->Height, true);
        unknown_surface_839958->Fill(0);
        Debug_Printf("AlternateSurface (%dx%d) %s\n", hidden->Width, hidden->Height,
                     ((DSurface *)unknown_surface_839958)->Is_Video_Ram() ? "VRAM" : "SYSTEM MEMORY");
    }

    return true;
}


// void __fastcall Delete_All_Objects_00519960() (0x00519960)
// Deletes every game object (name not recovered): the indexed objects, then each class list
// (deleting its first entry until it is empty; bullets through Release) with Unknown_006ecf10
// after each, the bolts, line trails and laser draws, the abstract types, the tactical view and
// the map's cells (MapClass slot 23), with the live-object counter raised meanwhile.
// __fastcall: as cdecl the epilogue's pop ebx is scheduled before the counter's dec.
// With Add visible the unit also emits the radar tracking vector vtable (bound to the Radar copy):
#include "shared/object_lists.hpp"
#include "shared/object.hpp"
#include "shared/terrain_object_index.hpp"
#include "shared/bullet.hpp"
#include "shared/tag.hpp"
#include "shared/trigger.hpp"
#include "shared/tube.hpp"
#include "shared/building_light.hpp"
#include "shared/overlay.hpp"
#include "shared/particle_system.hpp"
#include "shared/wave.hpp"
#include "shared/factory_class.hpp"
#include "shared/side.hpp"
#include "shared/team.hpp"
#include "shared/anim.hpp"
#include "shared/script.hpp"
#include "shared/radsite.hpp"
#include "shared/light_source.hpp"
#include "shared/abstract.hpp"
#include "shared/empulse_class.hpp"
#include "shared/capture_manager.hpp"
#include "shared/parasite.hpp"
#include "shared/temporal.hpp"
#include "shared/spawn_manager.hpp"
#include "shared/spawn_manager_list.hpp"
#include "shared/spot_light.hpp"
#include "shared/fogged_object.hpp"
#include "shared/alpha_shape.hpp"
#include "shared/alpha_shape_list.hpp"
#include "shared/terrain.hpp"
#include "shared/abstract_type.hpp"
#include "shared/ebolt.hpp"
#include "shared/line_trail.hpp"
#include "shared/laser_draw.hpp"

void __fastcall Delete_All_Objects_00519960()
{
    unknown_a40774++;
    unknown_objects_ac0cd8.Clear();
    while (unknown_object_index_abff58.Count()) {
        delete unknown_object_index_abff58.Fetch_By_Position(0);
    }
    Unknown_006ecf10();
    while (unknown_BulletClass_list_00a40ce8.Count()) {
        unknown_BulletClass_list_00a40ce8[0]->Release();
    }
    Unknown_006ecf10();
    while (unknown_objects_a40368.Count()) {
        delete unknown_objects_a40368[0];
    }
    Unknown_006ecf10();
    while (Tags.Count()) {
        delete Tags[0];
    }
    Unknown_006ecf10();
    while (Triggers.Count()) {
        delete Triggers[0];
    }
    Unknown_006ecf10();
    while (unknown_TubeClass_list_008661c8.Count()) {
        delete unknown_TubeClass_list_008661c8[0];
    }
    Unknown_006ecf10();
    while (BuildingLights.Count()) {
        delete BuildingLights[0];
    }
    Unknown_006ecf10();
    while (unknown_OverlayClass_list_00a40c00.Count()) {
        delete unknown_OverlayClass_list_00a40c00[0];
    }
    Unknown_006ecf10();
    while (ParticleSystems.Count()) {
        delete ParticleSystems[0];
    }
    Unknown_006ecf10();
    while (Waves.Count()) {
        delete Waves[0];
    }
    Unknown_006ecf10();
    while (Factories.Count()) {
        delete Factories[0];
    }
    Unknown_006ecf10();
    while (Sides.Count()) {
        delete Sides[0];
    }
    Unknown_006ecf10();
    while (Teams.Count()) {
        delete Teams[0];
    }
    Unknown_006ecf10();
    while (Houses.Count()) {
        delete Houses[0];
    }
    Unknown_006ecf10();
    while (Anims.Count()) {
        delete Anims[0];
    }
    Unknown_006ecf10();
    while (Scripts.Count()) {
        delete Scripts[0];
    }
    Unknown_006ecf10();
    while (unknown_RadSiteClass_list_00ab6620.Count()) {
        delete unknown_RadSiteClass_list_00ab6620[0];
    }
    Unknown_006ecf10();
    while (unknown_LightSourceClass_list_00a6e818.Count()) {
        delete unknown_LightSourceClass_list_00a6e818[0];
    }
    Unknown_006ecf10();
    while (EMPulseClass::Array.Count()) {
        delete EMPulseClass::Array[0];
    }
    Unknown_006ecf10();
    while (unknown_CaptureManagerClass_list_008505a0.Count()) {
        delete unknown_CaptureManagerClass_list_008505a0[0];
    }
    Unknown_006ecf10();
    while (unknown_ParasiteClass_list_00a76430.Count()) {
        delete unknown_ParasiteClass_list_00a76430[0];
    }
    Unknown_006ecf10();
    while (unknown_TemporalClass_list_00ac02e0.Count()) {
        delete unknown_TemporalClass_list_00ac02e0[0];
    }
    Unknown_006ecf10();
    while (unknown_SpawnManagerClass_list_00abd028.Count()) {
        delete unknown_SpawnManagerClass_list_00abd028[0];
    }
    Unknown_006ecf10();
    while (unknown_BombClass_list_0084eb78.Count()) {
        delete unknown_BombClass_list_0084eb78[0];
    }
    Unknown_006ecf10();
    while (unknown_SpotLightClass_list_00a731c0.Count()) {
        delete unknown_SpotLightClass_list_00a731c0[0];
    }
    Unknown_006ecf10();
    while (FoggedObjectClass::Array.Count()) {
        delete FoggedObjectClass::Array[0];
    }
    Unknown_006ecf10();
    while (unknown_AlphaShapeClass_list_0083c648.Count()) {
        delete unknown_AlphaShapeClass_list_0083c648[0];
    }
    Unknown_006ecf10();
    while (Terrains.Count()) {
        delete Terrains[0];
    }
    Unknown_006ecf10();
    EBolt::Delete_All_004B23B0();
    LineTrail::Delete_All();
    LaserDrawClass::Clear_All();
    while (AbstractTypes.Count()) {
        delete AbstractTypes[0];
    }
    Unknown_006ecf10();
    delete unknown_tactical_83996c;
    unknown_tactical_83996c = 0;
    Map.unknown_slot23();
    Unknown_006ecf10();
    unknown_object_84e8d0.Unknown_0042E150();
    unknown_a40774--;
}


// void __fastcall Init_Theater_00519e70(int theater) -- 0x00519E70
// Body 0x00519E70-0x0051A263 (1012 bytes). Tiberian Sun's Init_Theater: reopens the theater mixfiles,
// loads the theater palette (unknown_palette_837e10, the game palette), copies it to the original
// palette (0x00838110), loads UNIT<suffix>.PAL into the scheme palette and rebuilds every colour
// scheme's converters from the two.
#include "shared/anim_type.hpp"
#include "shared/color_scheme.hpp"
#include "shared/techno_type.hpp"


extern PaletteClass unknown_palette_838110;     // 0x00838110 the theater palette as loaded (TS OriginalPalette)
extern PaletteClass unknown_palette_838a10;     // 0x00838A10 the unit palette (TS SchemePalette)
void __fastcall Unknown_00603750(int theater);  // 0x00603750

template<class T> inline T min(T a, T b) { return (a < b) ? a : b; }

void __fastcall Init_Theater_00519e70(int theater)
{
    char fullname[16];
    char shortname[16];
    char isofullname[16];

    wsprintfA(fullname, "%s.MIX", Theaters[theater].Root);
    wsprintfA(isofullname, "%s.MIX", Theaters[theater].IsoRoot);
    wsprintfA(shortname, "%s.MIX", Theaters[theater].Suffix);
    Debug_Printf("Init theater %s\n", Theaters[theater].Name);

    unknown_id_source_a3d290->Theater = theater;
    unknown_session_a3d298.Update_Progress(8);

    if (unknown_id_source_a3d290->Theater != 1
        && (unknown_loaded_theater_7d88dc == 1 || unknown_loaded_theater_7d88dc == -1)) {
        if (unknown_theater_mix_8374c8 != 0) {
            delete unknown_theater_mix_8374c8;
        }
        unknown_theater_mix_8374c8 = new MixFileClass("GENERIC.MIX", &FastKey);
        unknown_theater_mix_8374c8->Cache();
    }

    if (unknown_id_source_a3d290->Theater != unknown_loaded_theater_7d88dc) {
        if (unknown_theater_mix_8374c0 != 0) {
            delete unknown_theater_mix_8374c0;
        }
        unknown_theater_mix_8374c0 = new MixFileClass(fullname, &FastKey);

        if (unknown_theater_mix_8374c4 != 0) {
            delete unknown_theater_mix_8374c4;
        }
        unknown_theater_mix_8374c4 = new MixFileClass(shortname, &FastKey);
        unknown_theater_mix_8374c4->Cache();
        unknown_theater_mix_8374c0->Cache();
        unknown_session_a3d298.Update_Progress(6);

        if (unknown_theater_mix_8374cc != 0) {
            delete unknown_theater_mix_8374cc;
        }
        unknown_theater_mix_8374cc = new MixFileClass(isofullname, &FastKey);

        if (unknown_theater_mix_8374d0 != 0) {
            delete unknown_theater_mix_8374d0;
        }
        unknown_theater_mix_8374d0 = new MixFileClass("ISOGEN.MIX", &FastKey);
        unknown_session_a3d298.Update_Progress(12);

        wsprintfA(fullname, "%s.PAL", Theaters[theater].Root);
        unsigned char const *ptr = (unsigned char const *)MixFileClass::Retrieve(fullname);
        if (ptr != 0) {
            for (int color = 0; color < 256; color++) {
                unsigned char r = (unsigned char)((*ptr++) << 2);
                unsigned char g = (unsigned char)((*ptr++) << 2);
                unsigned char b = (unsigned char)((*ptr++) << 2);
                unknown_palette_837e10[color] = RGBClass(r, g, b);
            }
        } else {
            for (int color = 0; color < 256; color++) {
                unknown_palette_837e10[color] = RGBClass(color, 255 - color, (color << 2) & 0xFF);
            }
        }
        unknown_palette_838110 = unknown_palette_837e10;

        wsprintfA(fullname, "UNIT%s.PAL", Theaters[theater].Suffix);
        PaletteClass const *unitpal = (PaletteClass const *)MixFileClass::Retrieve(fullname);
        unknown_481fb0();
        if (unitpal != 0) {
            unknown_palette_838a10 = *unitpal;
        }
        for (int index = 0; index < 256; index++) {
            unknown_palette_838a10[index] = RGBClass(
                (unsigned char)(unknown_palette_838a10[index].Red << 2),
                (unsigned char)(unknown_palette_838a10[index].Green << 2),
                (unsigned char)(unknown_palette_838a10[index].Blue << 2));
        }

        int last_percent = 12;
        int prog_step = ColorSchemes.Count() / (25 - 12);
        for (int s = 0; s < ColorSchemes.Count(); s++) {
            ColorSchemes[s]->Unknown_00663B40(&unknown_palette_838a10, &unknown_palette_837e10, 1000, 1000, 1000);
            int percent = (min(12 + (s / prog_step), 25));
            if (percent != last_percent) {
                unknown_session_a3d298.Update_Progress(percent);
                last_percent = percent;
            }
            unknown_481fb0();
        }
        Unknown_00603750(theater);
        TechnoTypeClass::Load_Palettes();
        unknown_session_a3d298.Update_Progress(25);
    }
}


// bool Unknown_0051A270() -- 0x0051A270
// Releases the archive at 0x008374F0 (as CD::Swap_Disk releases its mixfiles).

bool __cdecl Unknown_0051A270()
{
    if (unknown_mix_8374f0 != 0) {
        Debug_Printf("     Releasing %s\n", unknown_mix_8374f0->Filename);
        delete unknown_mix_8374f0;
        unknown_mix_8374f0 = 0;
    }
    return true;
}


// bool Unknown_0051A2B0() -- 0x0051A2B0
// Releases the archive at 0x008374F0 and opens NEUTRAL.MIX in its place.
// Body 0x0051A2B0-0x0051A32B (124 bytes; the inventory's 59 stops at the
// operator new call).


bool __cdecl Unknown_0051A2B0()
{
    if (unknown_mix_8374f0 != 0) {
        Debug_Printf("     Releasing %s\n", unknown_mix_8374f0->Filename);
        delete unknown_mix_8374f0;
        unknown_mix_8374f0 = 0;
    }
    unknown_mix_8374f0 = new MixFileClass("NEUTRAL.MIX", &FastKey);
    if (unknown_mix_8374f0 == 0) {
        return false;
    }
    Unknown_00496460("LOADED NEUTRAL.MIX");
    return true;
}


// bool __fastcall Unknown_0051a330(int side) -- 0x0051A330


// Opens the side's interface archives (SIDEC and SIDENC, numbered from 1; side -1 asks the
// CD, which must be disk one or two) and reads UI.INI's advanced command bar.
bool __fastcall Unknown_0051a330(int side)
{
    if (side == -1) {
        int disk = Get_CD_Index(CDFileClass::CurrentCDDrive, 60);
        if (disk != 0 && disk != 1) {
            Debug_Printf("     FAILED!  This was not disk one or two!!!\n");
            return false;
        }
        side = disk;
    }
    Debug_Printf("Preparing Mixfiles for Side %02d.\n", side);

    if (unknown_mix_8374f8 != 0) {
        Debug_Printf("     Releasing %s\n", unknown_mix_8374f8->Filename);
        delete unknown_mix_8374f8;
        unknown_mix_8374f8 = 0;
    }
    if (unknown_side_mix_837500 != 0) {
        Debug_Printf("     Releasing %s\n", unknown_side_mix_837500->Filename);
        delete unknown_side_mix_837500;
        unknown_side_mix_837500 = 0;
    }
    if (unknown_side_nc_mix_837504 != 0) {
        Debug_Printf("     Releasing %s\n", unknown_side_nc_mix_837504->Filename);
        delete unknown_side_nc_mix_837504;
        unknown_side_nc_mix_837504 = 0;
    }

    int number = side + 1;
    Unknown_007168C0(side);

    char filename[64];
    sprintf(filename, "SIDEC%02d.MIX", number);
    Debug_Printf("     Initilizing %s\n", filename);
    if (CCFileClass(filename).Is_Available()) {
        unknown_side_mix_837500 = new MixFileClass(filename, &FastKey);
    }
    if (unknown_side_mix_837500 == 0) {
        Debug_Printf("     FAILED!\n");
        return false;
    }
    unknown_side_mix_837500->Cache();

    sprintf(filename, "SIDENC%02d.MIX", number);
    Debug_Printf("     Initilizing %s\n", filename);
    if (CCFileClass(filename).Is_Available()) {
        unknown_side_nc_mix_837504 = new MixFileClass(filename, &FastKey);
    }

    Unknown_Reload_Art_006F44D0();
    unknown_select_color(side);

    CCFileClass file("UI.INI");
    if (unknown_ini_839860.Load(file, false, false)) {
        Rules->Advanced_Command_Bar(unknown_ini_839860, unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5);
        Map.Init_For_House();
        return true;
    }
    Debug_Printf("Failed to load UI.INI!\n");
    return false;
}


// ObjectClass *__fastcall Unknown_0051A690(Cell const *cell, ObjectClass *object) -- 0x0051A690
// Body 0x0051A690-0x0051A864 (469 bytes, nop padding to 0x0051A870). Called from 0x00630049/0x00630063
// and 0x00669984/0x006699A5.
#include "shared/techno.hpp"
#include "shared/coord.hpp"
#include "shared/coord_distance.hpp"

// The selected object that best answers an order at the cell (or, without one, at the object): the
// highest priority wins, ties go to the nearest. Priority: 0 an object without AbstractFlags.IsTechno, 1 a
// techno with unknown_slot201, 2 the other flagged technos, 3 with unknown_slot163, 4 that and not a
// building (Kind_Of 6), 5 that and unknown_6c16f0(-1) > 0. The separate Count() > 0 guard is the
// original's (the count is tested twice before the loop).
ObjectClass *__fastcall Unknown_0051A690(Cell const *cell, ObjectClass *object)
{
    ObjectClass *best = 0;
    int best_priority = -1;
    int best_distance = 0x7FFFFFFF;
    if (unknown_objects_a40c60.Count() > 0) {
        for (int index = 0; index < unknown_objects_a40c60.Count(); index++) {
            ObjectClass *candidate = unknown_objects_a40c60[index];
            int priority = 0;
            bool flagged = false;
            if (candidate != 0) {
                flagged = candidate->AbstractFlags.IsTechno;
            }
            if (flagged) {
                TechnoClass *techno = (TechnoClass *)candidate;
                priority = 2;
                if (techno->unknown_slot201()) {
                    priority = 1;
                } else if (techno->unknown_slot163()) {
                    priority = 3;
                    if (techno->Kind_Of() != 6) {
                        priority = 4;
                        if (techno->unknown_6c16f0(-1) > 0) {
                            priority = 5;
                        }
                    }
                }
            }
            if (priority >= best_priority) {
                int distance = 0x7FFFFFFF;
                bool at_cell = cell != 0;
                if (at_cell || object != 0) {
                    CoordStruct coord = candidate->Center_Coord();
                    CoordStruct target;
                    if (at_cell) {
                        target = Cell_Coord(*cell);
                    } else {
                        target = object->Center_Coord();
                    }
                    distance = Coord_Distance(coord, target);
                }
                if (priority > best_priority) {
                    best_priority = priority;
                    best = candidate;
                    best_distance = distance;
                } else if (distance < best_distance) {
                    best = candidate;
                    best_distance = distance;
                }
            }
        }
    }
    return best;
}


// 0x0051a870: function not yet identified
//
// Body: 16 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_0051a870(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_0051a870 {

struct Callee_006e8510 { unsigned int From_Name_006e8510(char *theme_name); };
extern "C" unsigned int DAT_00a35d78;
extern "C" char s_INTRO_007dbc90[];

void  FUN_0051a870(void)
{
    ((Callee_006e8510*)(&DAT_00a35d78))->From_Name_006e8510((char *)(s_INTRO_007dbc90));
    return;
}

}  // namespace seed_0051a870


// int __fastcall Get_Performance_Profile() -- 0x0051A880
#include "shared/directdraw.hpp"
#include "shared/event.hpp"


// Rates the machine once from 1 to 10 by the lowest of its CPU, RAM, video memory and video
// blit speed ratings, writing each finding into the system texts; later calls return the rating.
int __fastcall Get_Performance_Profile()
{
    if (unknown_performance_profile_7db564 == -1) {
        unknown_profile_text_a4127c[0] = 0;
        unknown_profile_cpu_text_a411fc[0] = 0;
        unknown_profile_ram_text_a41388[0] = 0;
        unknown_profile_vram_text_a41308[0] = 0;
        unknown_profile_blit_text_a41408[0] = 0;

        int type = Get_CPU_Type();
        int mhz = Get_RDTSC_CPU_Speed();
        int cpu = 5;
        switch (type) {
        case -1:
            break;
        case 3:
        case 5:
            cpu = mhz / 100 + 1;
            break;
        case 4:
        case 7:
            cpu = mhz / 100 + 2;
            break;
        case 6:
            cpu = mhz / 100 + 3;
            break;
        default:
            cpu = 1;
            break;
        }
        Debug_Printf("Profile: CPU:%d (%dMhz %s)\n", cpu, mhz, Get_CPU_Type_Name());
        sprintf(unknown_profile_cpu_text_a411fc, "CPU: %d Mhz %s", mhz, Get_CPU_Type_Name());

        MEMORYSTATUS status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatus(&status);
        int ram = 2;
        unsigned int megabytes = (status.dwTotalPhys + 0xA0000) >> 20;
        if (megabytes >= 64) {
            ram = 5;
        }
        if (megabytes >= 128) {
            ram += 2;
        }
        if (megabytes == 256) {
            ram += 2;
        } else if (megabytes > 256) {
            ram = 10;
        }
        Debug_Printf("Profile: RAM:%d (%dMb)\n", ram, megabytes);
        sprintf(unknown_profile_ram_text_a41388, "RAM reported: %d Mb", megabytes);

        megabytes = 4;
        int vram = 5;
        if (DirectDrawObject != 0) {
            DDCAPS caps;
            caps.dwSize = sizeof(caps);
            if (DirectDrawObject->GetCaps(&caps, 0) == 0) {
                unsigned int memory = caps.dwVidMemTotal > caps.dwVidMemFree ? caps.dwVidMemTotal : caps.dwVidMemFree;
                megabytes = (memory + 0xFFFFF) >> 20;
            }
        }
        if (megabytes >= 32) {
            vram = 10;
        } else if (megabytes >= 16) {
            vram = 9;
        } else if (megabytes >= 8) {
            vram = 7;
        } else if (megabytes >= 4) {
            vram = 5;
        } else if (megabytes <= 2) {
            vram = 3;
        }
        Debug_Printf("Profile: VRAM:%d (%dMb)\n", vram, megabytes);
        sprintf(unknown_profile_vram_text_a41308, "VRAM reported: %d Mb", megabytes);

        int speed = 5;
        int blits = 0;
        if (unknown_surface_839944 != 0 && ((DSurface *)unknown_surface_839944)->Is_Video_Ram()
            && !((DSurface *)unknown_surface_839958)->Is_Video_Ram()) {
            Rect area = unknown_surface_839944->Get_Rect();
            Rect other = unknown_surface_839958->Get_Rect();
            if (area.Width >= other.Width) {
                area.Width = other.Width;
            }
            if (area.Height >= other.Height) {
                area.Height = other.Height;
            }
            if (area.Width >= 480) {
                area.Width = 480;
            }
            if (area.Height >= 400) {
                area.Height = 400;
            }
            for (int index = 0; index < 4; index++) {
                unknown_surface_839944->Copy_From(area, *unknown_surface_839958, area, false, true);
                unknown_surface_839958->Copy_From(area, *unknown_surface_839944, area, false, true);
                Sleep(0);
            }
            DWORD start = timeGetTime();
            if ((start & 0xFF000000) != 0xFF) {
                unknown_surface_839944->Copy_From(area, *unknown_surface_839958, area, false, true);
                unknown_surface_839958->Copy_From(area, *unknown_surface_839944, area, false, true);
                blits++;
                while (timeGetTime() - start <= 1000 && blits <= 5000) {
                    unknown_surface_839944->Copy_From(area, *unknown_surface_839958, area, false, true);
                    unknown_surface_839958->Copy_From(area, *unknown_surface_839944, area, false, true);
                    blits++;
                }
                speed = blits / 4 < 1 ? 1 : blits / 4;
            }
        }
        Debug_Printf("Profile: VRAM speed:%d (%d blits per second)\n", speed, blits * 2);
        sprintf(unknown_profile_blit_text_a41408, "%d blits per second\n", blits * 2);

        unknown_performance_profile_7db564 = cpu;
        if (unknown_performance_profile_7db564 >= ram) {
            unknown_performance_profile_7db564 = ram;
        }
        if (unknown_performance_profile_7db564 >= vram) {
            unknown_performance_profile_7db564 = vram;
        }
        if (unknown_performance_profile_7db564 >= speed) {
            unknown_performance_profile_7db564 = speed;
        }
        Debug_Printf("Overall performance profile = %d\n", unknown_performance_profile_7db564);
        sprintf(unknown_profile_text_a4127c, "Profile: %d\n", unknown_performance_profile_7db564);
    }
    return unknown_performance_profile_7db564;
}


// CDFileClass::~CDFileClass() (0x0051AD10)
// Inline destructor of shared/cd_file_class.hpp, emitted as a COMDAT by Init.CPP: Init_Game's inline
// expansion of ~CCFileClass on the SOUND.INI failure path calls it out of line.


// CCFileClass::~CCFileClass() (0x0051AD20)
// Inline destructor of shared/cc_file_class.hpp, emitted as a COMDAT by Init.CPP: Init_Game and its
// inline Bootstrap/Init_CDROM_Access call it out of line once their inline budget is spent.


// INIClass::INIClass() (0x0051AD50)
// Inline constructor of shared/ini_class.hpp, emitted as a COMDAT by Init.CPP: Init_Game's inline
// CCINIClass constructor (SOUND.INI) calls it out of line.
#include "shared/ini_class.hpp"


// CCINIClass::CCINIClass() (0x0051ADE0)
// Inline constructor of shared/ccini_class.hpp, emitted as a COMDAT by Init.CPP: Init_Game constructs
// the EVA.INI and THEME.INI objects out of line (the SOUND.INI one is expanded inline).


// bool CommandClass::unknown_slot5(int key) const -- 0x0051AE70
// The default: no modifier condition.
#include "shared/tab.hpp"

bool CommandClass::unknown_slot5(int key) const
{
    return false;
}


// bool CommandClass::unknown_slot6(int key) const -- 0x0051AE80
// The default: true unless the key carries the release bit (0x800).

bool CommandClass::unknown_slot6(int key) const
{
    return ((unsigned)~key >> 11) & 1;
}


// bool CommandClass::unknown_slot7(int key) const -- 0x0051AE90
// The default: false.

bool CommandClass::unknown_slot7(int key) const
{
    return false;
}


// CreateTeamCommandClass::CreateTeamCommandClass(int number) -- 0x0051AEA0

CreateTeamCommandClass::CreateTeamCommandClass(int number) : TeamNumber(number)
{
}


// char const *CreateTeamCommandClass::Get_Name() const -- 0x0051AEC0
// The command's key name, "TeamCreate_<n>" with the number, formatted into the shared name
// buffer (0x00A4149C).

extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);

char const *CreateTeamCommandClass::Get_Name() const
{
    sprintf(unknown_command_name_a4149c, "TeamCreate_%d", TeamNumber);
    return unknown_command_name_a4149c;
}


// wchar_t const *CreateTeamCommandClass::Get_UI_Name() const -- 0x0051AEE0
// The localized name (TXT_CREATE_TEAM) formatted with the team number
// into the shared command text buffer (0x00A4153C).

wchar_t const *CreateTeamCommandClass::Get_UI_Name() const
{
    swprintf(unknown_command_text_a4153c, StringTable::Load_String("TXT_CREATE_TEAM", 0, "D:\\Ra2\\Init.CPP", 5910), TeamNumber);
    return unknown_command_text_a4153c;
}


// wchar_t const *CreateTeamCommandClass::Get_Category() const -- 0x0051AF10
// The localized category (TXT_TEAM).

wchar_t const *CreateTeamCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_TEAM", 0, "D:\\Ra2\\Init.CPP", 5912);
}


// wchar_t const *CreateTeamCommandClass::Get_Description() const -- 0x0051AF30
// The localized description (TXT_CREATE_TEAM_DESC) formatted with the team number
// into the shared command text buffer (0x00A4153C).

wchar_t const *CreateTeamCommandClass::Get_Description() const
{
    swprintf(unknown_command_text_a4153c, StringTable::Load_String("TXT_CREATE_TEAM_DESC", 0, "D:\\Ra2\\Init.CPP", 5914), TeamNumber);
    return unknown_command_text_a4153c;
}


// void CreateTeamCommandClass::Execute(int key) const -- 0x0051AF60
// Makes team TeamNumber of the selection (0x006F5D90).

void CreateTeamCommandClass::Execute(int key) const
{
    Unknown_006F5D90(TeamNumber);
}


// CreateTeamCommandClass scalar deleting destructor (0x0051AF70, ??_G): emitted with
// the vtable by the constructor in this unit; the destructor (implicit) expands inline.


// wchar_t const *CommandClass::Get_UI_Name() const -- 0x0051AF90
// The command's display name: by default its description (slot 4).

wchar_t const *CommandClass::Get_UI_Name() const
{
    return Get_Description();
}


// CommandClass::`scalar deleting destructor' (0x0051AFA0, ??_G; vtable slot 0): emitted by the class
// declaration in Init.CPP; no source of its own.


// char const *SetStructureTabCommandClass::Get_Name() const -- 0x0051AFC0
// The command's key name.

char const *SetStructureTabCommandClass::Get_Name() const
{
    return "StructureTab";
}


// wchar_t const *SetStructureTabCommandClass::Get_UI_Name() const -- 0x0051AFD0
// The localized name (TXT_STRUCTURE_TAB).

wchar_t const *SetStructureTabCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_STRUCTURE_TAB", 0, "D:\\Ra2\\Init.CPP", 5931);
}


// wchar_t const *SetStructureTabCommandClass::Get_Category() const -- 0x0051AFF0
// The localized category (TXT_INTERFACE).

wchar_t const *SetStructureTabCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 5933);
}


// wchar_t const *SetStructureTabCommandClass::Get_Description() const -- 0x0051B010
// The localized description (TXT_STRUCTURE_TAB_DESC).

wchar_t const *SetStructureTabCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_STRUCTURE_TAB_DESC", 0, "D:\\Ra2\\Init.CPP", 5935);
}


// void SetStructureTabCommandClass::Execute(int key) const -- 0x0051B030
// Switches the sidebar to tab 0 when that column has anything to build,
// then hands the remembered building (0x006F8A50) to the player's house
// (0x004E7070) with the building its slot 94 names.

BuildingClass *Unknown_006F8A50();

void SetStructureTabCommandClass::Execute(int key) const
{
    if (Map.Column[0].Unknown_00681730()) {
        Map.Select_Tab(0);
        BuildingClass *building = Unknown_006F8A50();
        if (building != 0) {
            unknown_player_a35db4->unknown_func_4e7070(building->Who_Can_Build_Me(false, false), building);
        }
    }
}


// char const *SetDefenseTabCommandClass::Get_Name() const -- 0x0051B080
// The command's key name.

char const *SetDefenseTabCommandClass::Get_Name() const
{
    return "DefenseTab";
}


// wchar_t const *SetDefenseTabCommandClass::Get_UI_Name() const -- 0x0051B090
// The localized name (TXT_DEFENSE_TAB).

wchar_t const *SetDefenseTabCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_DEFENSE_TAB", 0, "D:\\Ra2\\Init.CPP", 5957);
}


// wchar_t const *SetDefenseTabCommandClass::Get_Category() const -- 0x0051B0B0
// The localized category (TXT_INTERFACE).

wchar_t const *SetDefenseTabCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 5959);
}


// wchar_t const *SetDefenseTabCommandClass::Get_Description() const -- 0x0051B0D0
// The localized description (TXT_DEFENSE_TAB_DESC).

wchar_t const *SetDefenseTabCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_DEFENSE_TAB_DESC", 0, "D:\\Ra2\\Init.CPP", 5961);
}


// void SetDefenseTabCommandClass::Execute(int key) const -- 0x0051B0F0
// Switches the sidebar to tab 1 when that column has anything to build,
// then hands the remembered building (0x006F8A60) to the player's house
// (0x004E7070) with the building its slot 94 names.

BuildingClass *Unknown_006F8A60();

void SetDefenseTabCommandClass::Execute(int key) const
{
    if (Map.Column[1].Unknown_00681730()) {
        Map.Select_Tab(1);
        BuildingClass *building = Unknown_006F8A60();
        if (building != 0) {
            unknown_player_a35db4->unknown_func_4e7070(building->Who_Can_Build_Me(false, false), building);
        }
    }
}


// char const *SetInfantryTabCommandClass::Get_Name() const -- 0x0051B140
// The command's key name.

char const *SetInfantryTabCommandClass::Get_Name() const
{
    return "InfantryTab";
}


// wchar_t const *SetInfantryTabCommandClass::Get_UI_Name() const -- 0x0051B150
// The localized name (TXT_INFANTRY_TAB).

wchar_t const *SetInfantryTabCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_INFANTRY_TAB", 0, "D:\\Ra2\\Init.CPP", 5983);
}


// wchar_t const *SetInfantryTabCommandClass::Get_Category() const -- 0x0051B170
// The localized category (TXT_INTERFACE).

wchar_t const *SetInfantryTabCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 5985);
}


// wchar_t const *SetInfantryTabCommandClass::Get_Description() const -- 0x0051B190
// The localized description (TXT_INFANTRY_TAB_DESC).

wchar_t const *SetInfantryTabCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_INFANTRY_TAB_DESC", 0, "D:\\Ra2\\Init.CPP", 5987);
}


// void SetInfantryTabCommandClass::Execute(int key) const -- 0x0051B1B0
// Switches the sidebar to tab 2 when that column has anything to build
// (StripClass at 0x008356E8, Column[2] of the screen at 0x008324E0).
#include "shared/sidebar_screen.hpp"

void SetInfantryTabCommandClass::Execute(int key) const
{
    if (Map.Column[2].Unknown_00681730()) {
        Map.Select_Tab(2);
    }
}


// char const *SetUnitTabCommandClass::Get_Name() const -- 0x0051B1D0
// The command's key name.

char const *SetUnitTabCommandClass::Get_Name() const
{
    return "UnitTab";
}


// wchar_t const *SetUnitTabCommandClass::Get_UI_Name() const -- 0x0051B1E0
// The localized name (TXT_UNIT_TAB).

wchar_t const *SetUnitTabCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_UNIT_TAB", 0, "D:\\Ra2\\Init.CPP", 6004);
}


// wchar_t const *SetUnitTabCommandClass::Get_Category() const -- 0x0051B200
// The localized category (TXT_INTERFACE).

wchar_t const *SetUnitTabCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 6006);
}


// wchar_t const *SetUnitTabCommandClass::Get_Description() const -- 0x0051B220
// The localized description (TXT_UNIT_TAB_DESC).

wchar_t const *SetUnitTabCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_UNIT_TAB_DESC", 0, "D:\\Ra2\\Init.CPP", 6008);
}


// void SetUnitTabCommandClass::Execute(int key) const -- 0x0051B240
// Switches the sidebar to tab 3 when that column has anything to build
// (StripClass at 0x0083654C, Column[3] of the screen at 0x008324E0).

void SetUnitTabCommandClass::Execute(int key) const
{
    if (Map.Column[3].Unknown_00681730()) {
        Map.Select_Tab(3);
    }
}


// SelectTeamCommandClass::SelectTeamCommandClass(int number) -- 0x0051B260

SelectTeamCommandClass::SelectTeamCommandClass(int number) : TeamNumber(number)
{
}


// char const *SelectTeamCommandClass::Get_Name() const -- 0x0051B280
// The command's key name, "TeamSelect_<n>" with the number, formatted into the shared name
// buffer (0x00A4149C).

extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);

char const *SelectTeamCommandClass::Get_Name() const
{
    sprintf(unknown_command_name_a4149c, "TeamSelect_%d", TeamNumber);
    return unknown_command_name_a4149c;
}


// wchar_t const *SelectTeamCommandClass::Get_UI_Name() const -- 0x0051B2A0
// The localized name (TXT_SELECT_TEAM) formatted with the team number
// into the shared command text buffer (0x00A4153C).

wchar_t const *SelectTeamCommandClass::Get_UI_Name() const
{
    swprintf(unknown_command_text_a4153c, StringTable::Load_String("TXT_SELECT_TEAM", 0, "D:\\Ra2\\Init.CPP", 6037), TeamNumber);
    return unknown_command_text_a4153c;
}


// wchar_t const *SelectTeamCommandClass::Get_Category() const -- 0x0051B2D0
// The localized category (TXT_TEAM).

wchar_t const *SelectTeamCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_TEAM", 0, "D:\\Ra2\\Init.CPP", 6039);
}


// wchar_t const *SelectTeamCommandClass::Get_Description() const -- 0x0051B2F0
// The localized description (TXT_SELECT_TEAM_DESC) formatted with the team number
// into the shared command text buffer (0x00A4153C).

wchar_t const *SelectTeamCommandClass::Get_Description() const
{
    swprintf(unknown_command_text_a4153c, StringTable::Load_String("TXT_SELECT_TEAM_DESC", 0, "D:\\Ra2\\Init.CPP", 6041), TeamNumber);
    return unknown_command_text_a4153c;
}


// void SelectTeamCommandClass::Execute(int key) const -- 0x0051B320
// Selects team TeamNumber (0x006F5F00).

void SelectTeamCommandClass::Execute(int key) const
{
    Unknown_006F5F00(TeamNumber);
}


// SelectTeamCommandClass scalar deleting destructor (0x0051B330, ??_G): emitted with
// the vtable by the constructor in this unit; the destructor (implicit) expands inline.


// AddTeamCommandClass::AddTeamCommandClass(int number) -- 0x0051B350

AddTeamCommandClass::AddTeamCommandClass(int number) : TeamNumber(number)
{
}


// char const *AddTeamCommandClass::Get_Name() const -- 0x0051B370
// The command's key name, "TeamAddSelect_<n>" with the number, formatted into the shared name
// buffer (0x00A4149C).

extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);

char const *AddTeamCommandClass::Get_Name() const
{
    sprintf(unknown_command_name_a4149c, "TeamAddSelect_%d", TeamNumber);
    return unknown_command_name_a4149c;
}


// wchar_t const *AddTeamCommandClass::Get_UI_Name() const -- 0x0051B390
// The localized name (TXT_ADD_SELECT_TEAM) formatted with the team number
// into the shared command text buffer (0x00A4153C).

wchar_t const *AddTeamCommandClass::Get_UI_Name() const
{
    swprintf(unknown_command_text_a4153c, StringTable::Load_String("TXT_ADD_SELECT_TEAM", 0, "D:\\Ra2\\Init.CPP", 6071), TeamNumber);
    return unknown_command_text_a4153c;
}


// wchar_t const *AddTeamCommandClass::Get_Category() const -- 0x0051B3C0
// The localized category (TXT_TEAM).

wchar_t const *AddTeamCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_TEAM", 0, "D:\\Ra2\\Init.CPP", 6073);
}


// wchar_t const *AddTeamCommandClass::Get_Description() const -- 0x0051B3E0
// The localized description (TXT_ADD_SELECT_TEAM_DESC) formatted with the team number
// into the shared command text buffer (0x00A4153C).

wchar_t const *AddTeamCommandClass::Get_Description() const
{
    swprintf(unknown_command_text_a4153c, StringTable::Load_String("TXT_ADD_SELECT_TEAM_DESC", 0, "D:\\Ra2\\Init.CPP", 6075), TeamNumber);
    return unknown_command_text_a4153c;
}


// void AddTeamCommandClass::Execute(int key) const -- 0x0051B410
// Adds the selection to team TeamNumber (0x006F61F0).
void __fastcall Unknown_006F61F0(int group);

void AddTeamCommandClass::Execute(int key) const
{
    Unknown_006F61F0(TeamNumber);
}


// AddTeamCommandClass scalar deleting destructor (0x0051B420, ??_G): emitted with
// the vtable by the constructor in this unit; the destructor (implicit) expands inline.


// CenterTeamCommandClass::CenterTeamCommandClass(int number) -- 0x0051B440

CenterTeamCommandClass::CenterTeamCommandClass(int number) : TeamNumber(number)
{
}


// char const *CenterTeamCommandClass::Get_Name() const -- 0x0051B460
// The command's key name, "TeamCenter_<n>", formatted into the shared name
// buffer (0x00A4149C).

extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);

char const *CenterTeamCommandClass::Get_Name() const
{
    sprintf(unknown_command_name_a4149c, "TeamCenter_%d", TeamNumber);
    return unknown_command_name_a4149c;
}


// wchar_t const *CenterTeamCommandClass::Get_UI_Name() const -- 0x0051B480
// The localized name (TXT_CENTER_TEAM) formatted with the team number
// into the shared command text buffer (0x00A4153C).

wchar_t const *CenterTeamCommandClass::Get_UI_Name() const
{
    swprintf(unknown_command_text_a4153c, StringTable::Load_String("TXT_CENTER_TEAM", 0, "D:\\Ra2\\Init.CPP", 6106), TeamNumber);
    return unknown_command_text_a4153c;
}


// wchar_t const *CenterTeamCommandClass::Get_Category() const -- 0x0051B4B0
// The localized category (TXT_TEAM).

wchar_t const *CenterTeamCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_TEAM", 0, "D:\\Ra2\\Init.CPP", 6108);
}


// wchar_t const *CenterTeamCommandClass::Get_Description() const -- 0x0051B4D0
// The localized description (TXT_CENTER_TEAM_DESC) formatted with the team number
// into the shared command text buffer (0x00A4153C).

wchar_t const *CenterTeamCommandClass::Get_Description() const
{
    swprintf(unknown_command_text_a4153c, StringTable::Load_String("TXT_CENTER_TEAM_DESC", 0, "D:\\Ra2\\Init.CPP", 6110), TeamNumber);
    return unknown_command_text_a4153c;
}


// void CenterTeamCommandClass::Execute(int key) const -- 0x0051B500
// Centres the view on team TeamNumber (0x006F60D0).

void CenterTeamCommandClass::Execute(int key) const
{
    Unknown_006F60D0(TeamNumber);
}


// CenterTeamCommandClass scalar deleting destructor (0x0051B510, ??_G): emitted with
// the vtable by the constructor in this unit; the destructor (implicit) expands inline.


// TauntCommandClass::TauntCommandClass(int number) -- 0x0051B530

TauntCommandClass::TauntCommandClass(int number) : TauntNumber(number)
{
}


// char const *TauntCommandClass::Get_Name() const -- 0x0051B550
// The command's key name, "Taunt_<n>" with the number, formatted into the shared name
// buffer (0x00A4149C).

extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);

char const *TauntCommandClass::Get_Name() const
{
    sprintf(unknown_command_name_a4149c, "Taunt_%d", TauntNumber);
    return unknown_command_name_a4149c;
}


// wchar_t const *TauntCommandClass::Get_UI_Name() const -- 0x0051B570
// The localized name (TXT_TAUNT_NUMBER) formatted with the taunt number
// into the shared command text buffer (0x00A4153C).

wchar_t const *TauntCommandClass::Get_UI_Name() const
{
    swprintf(unknown_command_text_a4153c, StringTable::Load_String("TXT_TAUNT_NUMBER", 0, "D:\\Ra2\\Init.CPP", 6145), TauntNumber);
    return unknown_command_text_a4153c;
}


// wchar_t const *TauntCommandClass::Get_Category() const -- 0x0051B5A0
// The localized category (TXT_TAUNT).

wchar_t const *TauntCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_TAUNT", 0, "D:\\Ra2\\Init.CPP", 6150);
}


// wchar_t const *TauntCommandClass::Get_Description() const -- 0x0051B5C0
// The localized description (TXT_TAUNT_DESC) formatted with the taunt number
// into the shared command text buffer (0x00A4153C).

wchar_t const *TauntCommandClass::Get_Description() const
{
    swprintf(unknown_command_text_a4153c, StringTable::Load_String("TXT_TAUNT_DESC", 0, "D:\\Ra2\\Init.CPP", 6154), TauntNumber);
    return unknown_command_text_a4153c;
}


// TauntCommandClass scalar deleting destructor (0x0051B820, ??_G): emitted with
// the vtable by the constructor in this unit; the destructor (implicit) expands inline.


// char const *NextObjectCommandClass::Get_Name() const -- 0x0051B840
// The command's key name.

char const *NextObjectCommandClass::Get_Name() const
{
    return "NextObject";
}


// wchar_t const *NextObjectCommandClass::Get_UI_Name() const -- 0x0051B850
// The localized name (TXT_NEXT_OBJECT).

wchar_t const *NextObjectCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_NEXT_OBJECT", 0, "D:\\Ra2\\Init.CPP", 6271);
}


// wchar_t const *NextObjectCommandClass::Get_Category() const -- 0x0051B870
// The localized category (TXT_SELECTION).

wchar_t const *NextObjectCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_SELECTION", 0, "D:\\Ra2\\Init.CPP", 6273);
}


// wchar_t const *NextObjectCommandClass::Get_Description() const -- 0x0051B890
// The localized description (TXT_NEXT_OBJECT_DESC).

wchar_t const *NextObjectCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_NEXT_OBJECT_DESC", 0, "D:\\Ra2\\Init.CPP", 6275);
}


// void NextObjectCommandClass::Execute(int key) const -- 0x0051B8B0
// Leaves the screen modes, then selects the object after the first selected
// one (DisplayClass::Next_Object; the first selectable when none is selected)
// and centres the view on it.

void __cdecl Clear_Selected_Objects_00482bc0();

void NextObjectCommandClass::Execute(int key) const
{
    Map.Unknown_0049C980(0);
    Map.Unknown_0049C860(0, false);
    Map.Unknown_0049CA20(0);
    Map.Unknown_0049C7C0(0);
    ObjectClass *object = Map.Next_Object(unknown_objects_a40c60.Count() ? unknown_objects_a40c60[0] : 0);
    if (object != 0) {
        Clear_Selected_Objects_00482bc0();
        object->unknown_slot80();
        Map.Unknown_0049E3B0();
        Map.Flag_To_Redraw(true);
    }
}


// char const *PlanningModeCommandClass::Get_Name() const -- 0x0051B940
// The command's key name.

char const *PlanningModeCommandClass::Get_Name() const
{
    return "PlanningMode";
}


// wchar_t const *PlanningModeCommandClass::Get_UI_Name() const -- 0x0051B950
// The localized name (Cmnd:PlanningMode).

wchar_t const *PlanningModeCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("Cmnd:PlanningMode", 0, "D:\\Ra2\\Init.CPP", 6325);
}


// wchar_t const *PlanningModeCommandClass::Get_Category() const -- 0x0051B970
// The localized category (TXT_CONTROL).

wchar_t const *PlanningModeCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_CONTROL", 0, "D:\\Ra2\\Init.CPP", 6327);
}


// wchar_t const *PlanningModeCommandClass::Get_Description() const -- 0x0051B990
// The localized description (Cmnd:PlanningModeDesc).

wchar_t const *PlanningModeCommandClass::Get_Description() const
{
    return StringTable::Load_String("Cmnd:PlanningModeDesc", 0, "D:\\Ra2\\Init.CPP", 6329);
}


// True with any of the shift, ctrl or alt key bits (0x100, 0x200, 0x400) (0x0051B9B0; VC6 folds the
// three tests into one `test ah, 7`, giving the original's branch with a full-register 1).
bool PlanningModeCommandClass::unknown_slot5(int key) const
{
    return (key & 0x100) || (key & 0x200) || (key & 0x400);
}


// bool PlanningModeCommandClass::unknown_slot6(int key) const -- 0x0051B9D0
// Always.

bool PlanningModeCommandClass::unknown_slot6(int key) const
{
    return true;
}


// bool PlanningModeCommandClass::unknown_slot7(int key) const -- 0x0051B9E0
// True unless the key carries the release bit (0x800).

bool PlanningModeCommandClass::unknown_slot7(int key) const
{
    return ((unsigned)~key >> 11) & 1;
}


// void PlanningModeCommandClass::Execute(int key) const -- 0x0051B9F0
// Planning mode off (0x006F64F0) with the 0x800 key bit, on (0x006F64D0) without.

void PlanningModeCommandClass::Execute(int key) const
{
    if (key & 0x800) {
        Unknown_006F64F0(false);
    } else {
        Unknown_006F64D0(false);
    }
}


// char const *CombatantSelectCommandClass::Get_Name() const -- 0x0051BA10
// The command's key name.

char const *CombatantSelectCommandClass::Get_Name() const
{
    return "CombatantSelect";
}


// wchar_t const *CombatantSelectCommandClass::Get_UI_Name() const -- 0x0051BA20
// The localized name (Cmnd:CombatantSelect).

wchar_t const *CombatantSelectCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("Cmnd:CombatantSelect", 0, "D:\\Ra2\\Init.CPP", 6409);
}


// wchar_t const *CombatantSelectCommandClass::Get_Category() const -- 0x0051BA40
// The localized category (TXT_SELECTION).

wchar_t const *CombatantSelectCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_SELECTION", 0, "D:\\Ra2\\Init.CPP", 6411);
}


// wchar_t const *CombatantSelectCommandClass::Get_Description() const -- 0x0051BA60
// The localized description (Cmnd:CombatantSelectDesc).

wchar_t const *CombatantSelectCommandClass::Get_Description() const
{
    return StringTable::Load_String("Cmnd:CombatantSelectDesc", 0, "D:\\Ra2\\Init.CPP", 6413);
}


// bool CombatantSelectCommandClass::unknown_slot5(int key) const -- 0x0051BA80
// True with the shift bit (0x100).

bool CombatantSelectCommandClass::unknown_slot5(int key) const
{
    return ((unsigned)key >> 8) & 1;
}


// void CombatantSelectCommandClass::Execute(int key) const -- 0x0051BA90
// Runs the UICmnds.cpp handler with whether the 0x100 key bit is clear.

void __fastcall Unknown_006F6D00(bool arg);

void CombatantSelectCommandClass::Execute(int key) const
{
    Unknown_006F6D00(!(key & 0x100));
}


// char const *TypeSelectCommandClass::Get_Name() const -- 0x0051BAB0
// The command's key name.

char const *TypeSelectCommandClass::Get_Name() const
{
    return "TypeSelect";
}


// wchar_t const *TypeSelectCommandClass::Get_UI_Name() const -- 0x0051BAC0
// The localized name (Cmnd:TypeSelect).

wchar_t const *TypeSelectCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("Cmnd:TypeSelect", 0, "D:\\Ra2\\Init.CPP", 6435);
}


// wchar_t const *TypeSelectCommandClass::Get_Category() const -- 0x0051BAE0
// The localized category (TXT_SELECTION).

wchar_t const *TypeSelectCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_SELECTION", 0, "D:\\Ra2\\Init.CPP", 6437);
}


// wchar_t const *TypeSelectCommandClass::Get_Description() const -- 0x0051BB00
// The localized description (Cmnd:TypeSelectDesc).

wchar_t const *TypeSelectCommandClass::Get_Description() const
{
    return StringTable::Load_String("Cmnd:TypeSelectDesc", 0, "D:\\Ra2\\Init.CPP", 6439);
}


// bool TypeSelectCommandClass::unknown_slot5(int key) const -- 0x0051BB20
// True with the shift bit (0x100).

bool TypeSelectCommandClass::unknown_slot5(int key) const
{
    return ((unsigned)key >> 8) & 1;
}


// bool TypeSelectCommandClass::unknown_slot6(int key) const -- 0x0051BB30
// Always.

bool TypeSelectCommandClass::unknown_slot6(int key) const
{
    return true;
}


// bool TypeSelectCommandClass::unknown_slot7(int key) const -- 0x0051BB40
// True unless the key carries the release bit (0x800).

bool TypeSelectCommandClass::unknown_slot7(int key) const
{
    return ((unsigned)~key >> 11) & 1;
}


// void TypeSelectCommandClass::Execute(int key) const -- 0x0051BB50
// On release (0x800) redraws the stopwatch (0x006F7670), else starts it (0x006F7650).
void __fastcall Unknown_006F7670();
void Unknown_006F7650();

void TypeSelectCommandClass::Execute(int key) const
{
    if (key & 0x800) {
        Unknown_006F7670();
        return;
    }
    Unknown_006F7650();
}


// char const *HealthNavCommandClass::Get_Name() const -- 0x0051BB70
// The command's key name.

char const *HealthNavCommandClass::Get_Name() const
{
    return "HealthNav";
}


// wchar_t const *HealthNavCommandClass::Get_UI_Name() const -- 0x0051BB80
// The localized name (Cmnd:HealthNavigation).

wchar_t const *HealthNavCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("Cmnd:HealthNavigation", 0, "D:\\Ra2\\Init.CPP", 6473);
}


// wchar_t const *HealthNavCommandClass::Get_Category() const -- 0x0051BBA0
// The localized category (TXT_SELECTION).

wchar_t const *HealthNavCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_SELECTION", 0, "D:\\Ra2\\Init.CPP", 6475);
}


// wchar_t const *HealthNavCommandClass::Get_Description() const -- 0x0051BBC0
// The localized description (Cmnd:HealthNavigationDesc).

wchar_t const *HealthNavCommandClass::Get_Description() const
{
    return StringTable::Load_String("Cmnd:HealthNavigationDesc", 0, "D:\\Ra2\\Init.CPP", 6477);
}


// bool HealthNavCommandClass::unknown_slot5(int key) const -- 0x0051BBE0
// True with the shift bit (0x100).

bool HealthNavCommandClass::unknown_slot5(int key) const
{
    return ((unsigned)key >> 8) & 1;
}


// void HealthNavCommandClass::Execute(int key) const -- 0x0051BBF0
// Runs the UICmnds.cpp handler with whether the 0x100 key bit is clear.

void __fastcall Unknown_006F7B80(bool arg);

void HealthNavCommandClass::Execute(int key) const
{
    Unknown_006F7B80(!(key & 0x100));
}


// char const *VeterancyNavCommandClass::Get_Name() const -- 0x0051BC10
// The command's key name.

char const *VeterancyNavCommandClass::Get_Name() const
{
    return "VeterancyNav";
}


// wchar_t const *VeterancyNavCommandClass::Get_UI_Name() const -- 0x0051BC20
// The localized name (Cmnd:VetNavigation).

wchar_t const *VeterancyNavCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("Cmnd:VetNavigation", 0, "D:\\Ra2\\Init.CPP", 6499);
}


// wchar_t const *VeterancyNavCommandClass::Get_Category() const -- 0x0051BC40
// The localized category (TXT_SELECTION).

wchar_t const *VeterancyNavCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_SELECTION", 0, "D:\\Ra2\\Init.CPP", 6501);
}


// wchar_t const *VeterancyNavCommandClass::Get_Description() const -- 0x0051BC60
// The localized description (Cmnd:VetNavigationDesc).

wchar_t const *VeterancyNavCommandClass::Get_Description() const
{
    return StringTable::Load_String("Cmnd:VetNavigationDesc", 0, "D:\\Ra2\\Init.CPP", 6503);
}


// bool VeterancyNavCommandClass::unknown_slot5(int key) const -- 0x0051BC80
// True with the shift bit (0x100).

bool VeterancyNavCommandClass::unknown_slot5(int key) const
{
    return ((unsigned)key >> 8) & 1;
}


// void VeterancyNavCommandClass::Execute(int key) const -- 0x0051BC90
// Runs the UICmnds.cpp handler with whether the 0x100 key bit is clear.

void __fastcall Unknown_006F7EC0(bool arg);

void VeterancyNavCommandClass::Execute(int key) const
{
    Unknown_006F7EC0(!(key & 0x100));
}


// char const *PrevObjectCommandClass::Get_Name() const -- 0x0051BCB0
// The command's key name.

char const *PrevObjectCommandClass::Get_Name() const
{
    return "PreviousObject";
}


// wchar_t const *PrevObjectCommandClass::Get_UI_Name() const -- 0x0051BCC0
// The localized name (TXT_PREV_OBJECT).

wchar_t const *PrevObjectCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_PREV_OBJECT", 0, "D:\\Ra2\\Init.CPP", 6562);
}


// wchar_t const *PrevObjectCommandClass::Get_Category() const -- 0x0051BCE0
// The localized category (TXT_SELECTION).

wchar_t const *PrevObjectCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_SELECTION", 0, "D:\\Ra2\\Init.CPP", 6564);
}


// wchar_t const *PrevObjectCommandClass::Get_Description() const -- 0x0051BD00
// The localized description (TXT_PREV_OBJECT_DESC).

wchar_t const *PrevObjectCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_PREV_OBJECT_DESC", 0, "D:\\Ra2\\Init.CPP", 6566);
}


// void PrevObjectCommandClass::Execute(int key) const -- 0x0051BD20
// As NextObjectCommandClass::Execute with DisplayClass::Prev_Object.

void __cdecl Clear_Selected_Objects_00482bc0();

void PrevObjectCommandClass::Execute(int key) const
{
    Map.Unknown_0049C980(0);
    Map.Unknown_0049C860(0, false);
    Map.Unknown_0049CA20(0);
    Map.Unknown_0049C7C0(0);
    ObjectClass *object = Map.Prev_Object(unknown_objects_a40c60.Count() ? unknown_objects_a40c60[0] : 0);
    if (object != 0) {
        Clear_Selected_Objects_00482bc0();
        object->unknown_slot80();
        Map.Unknown_0049E3B0();
        Map.Flag_To_Redraw(true);
    }
}


// char const *StopCommandClass::Get_Name() const -- 0x0051BDB0
// The command's key name.

char const *StopCommandClass::Get_Name() const
{
    return "StopObject";
}


// wchar_t const *StopCommandClass::Get_UI_Name() const -- 0x0051BDC0
// The localized name (TXT_STOP_OBJECT).

wchar_t const *StopCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_STOP_OBJECT", 0, "D:\\Ra2\\Init.CPP", 6603);
}


// wchar_t const *StopCommandClass::Get_Category() const -- 0x0051BDE0
// The localized category (TXT_CONTROL).

wchar_t const *StopCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_CONTROL", 0, "D:\\Ra2\\Init.CPP", 6605);
}


// wchar_t const *StopCommandClass::Get_Description() const -- 0x0051BE00
// The localized description (TXT_STOP_OBJECT_DESC).

wchar_t const *StopCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_STOP_OBJECT_DESC", 0, "D:\\Ra2\\Init.CPP", 6607);
}


// void StopCommandClass::Execute(int key) const -- 0x0051BE20
// Stops the selection (0x006F5BC0).

void StopCommandClass::Execute(int key) const
{
    Unknown_006F5BC0();
}


// char const *AllToCheerCommandClass::Get_Name() const -- 0x0051BE30
// The command's key name.

char const *AllToCheerCommandClass::Get_Name() const
{
    return "AllToCheer";
}


// wchar_t const *AllToCheerCommandClass::Get_UI_Name() const -- 0x0051BE40
// The localized name (Cmnd:AllCheer).

wchar_t const *AllToCheerCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("Cmnd:AllCheer", 0, "D:\\Ra2\\Init.CPP", 6621);
}


// wchar_t const *AllToCheerCommandClass::Get_Category() const -- 0x0051BE60
// The localized category (TXT_CONTROL).

wchar_t const *AllToCheerCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_CONTROL", 0, "D:\\Ra2\\Init.CPP", 6623);
}


// wchar_t const *AllToCheerCommandClass::Get_Description() const -- 0x0051BE80
// The localized description (Cmnd:AllCheerDesc).

wchar_t const *AllToCheerCommandClass::Get_Description() const
{
    return StringTable::Load_String("Cmnd:AllCheerDesc", 0, "D:\\Ra2\\Init.CPP", 6625);
}


// void AllToCheerCommandClass::Execute(int key) const -- 0x0051BEA0
// Makes the selection cheer (0x006F5C50).

void AllToCheerCommandClass::Execute(int key) const
{
    Unknown_006F5C50();
}


// char const *DeployCommandClass::Get_Name() const -- 0x0051BEB0
// The command's key name.

char const *DeployCommandClass::Get_Name() const
{
    return "DeployObject";
}


// wchar_t const *DeployCommandClass::Get_UI_Name() const -- 0x0051BEC0
// The localized name (TXT_DEPLOY_OBJECT).

wchar_t const *DeployCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_DEPLOY_OBJECT", 0, "D:\\Ra2\\Init.CPP", 6648);
}


// wchar_t const *DeployCommandClass::Get_Category() const -- 0x0051BEE0
// The localized category (TXT_CONTROL).

wchar_t const *DeployCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_CONTROL", 0, "D:\\Ra2\\Init.CPP", 6650);
}


// wchar_t const *DeployCommandClass::Get_Description() const -- 0x0051BF00
// The localized description (TXT_DEPLOY_OBJECT_DESC).

wchar_t const *DeployCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_DEPLOY_OBJECT_DESC", 0, "D:\\Ra2\\Init.CPP", 6652);
}


// void DeployCommandClass::Execute(int key) const -- 0x0051BF20
// Deploys the selection (0x006F5940).

void DeployCommandClass::Execute(int key) const
{
    Unknown_006F5940();
}


// char const *GuardCommandClass::Get_Name() const -- 0x0051BF30
// The command's key name.

char const *GuardCommandClass::Get_Name() const
{
    return "GuardObject";
}


// wchar_t const *GuardCommandClass::Get_UI_Name() const -- 0x0051BF40
// The localized name (TXT_GUARD).

wchar_t const *GuardCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_GUARD", 0, "D:\\Ra2\\Init.CPP", 6676);
}


// wchar_t const *GuardCommandClass::Get_Category() const -- 0x0051BF60
// The localized category (TXT_CONTROL).

wchar_t const *GuardCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_CONTROL", 0, "D:\\Ra2\\Init.CPP", 6678);
}


// wchar_t const *GuardCommandClass::Get_Description() const -- 0x0051BF80
// The localized description (TXT_GUARD_DESC).

wchar_t const *GuardCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_GUARD_DESC", 0, "D:\\Ra2\\Init.CPP", 6680);
}


// void GuardCommandClass::Execute(int key) const -- 0x0051BFA0
// Puts the selection on guard (0x006F5B10).

void GuardCommandClass::Execute(int key) const
{
    Unknown_006F5B10();
}


// char const *ScatterCommandClass::Get_Name() const -- 0x0051BFB0
// The command's key name.

char const *ScatterCommandClass::Get_Name() const
{
    return "ScatterObject";
}


// wchar_t const *ScatterCommandClass::Get_UI_Name() const -- 0x0051BFC0
// The localized name (TXT_SCATTER).

wchar_t const *ScatterCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SCATTER", 0, "D:\\Ra2\\Init.CPP", 6705);
}


// wchar_t const *ScatterCommandClass::Get_Category() const -- 0x0051BFE0
// The localized category (TXT_CONTROL).

wchar_t const *ScatterCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_CONTROL", 0, "D:\\Ra2\\Init.CPP", 6707);
}


// wchar_t const *ScatterCommandClass::Get_Description() const -- 0x0051C000
// The localized description (TXT_SCATTER_DESC).

wchar_t const *ScatterCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SCATTER_DESC", 0, "D:\\Ra2\\Init.CPP", 6709);
}


// void ScatterCommandClass::Execute(int key) const -- 0x0051C020
// Scatters the selection (0x006F5D10).
void Unknown_006F5D10();

void ScatterCommandClass::Execute(int key) const
{
    Unknown_006F5D10();
}


// char const *CenterViewCommandClass::Get_Name() const -- 0x0051C030
// The command's key name.

char const *CenterViewCommandClass::Get_Name() const
{
    return "CenterView";
}


// wchar_t const *CenterViewCommandClass::Get_UI_Name() const -- 0x0051C040
// The localized name (TXT_CENTER_VIEW).

wchar_t const *CenterViewCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_CENTER_VIEW", 0, "D:\\Ra2\\Init.CPP", 6732);
}


// wchar_t const *CenterViewCommandClass::Get_Category() const -- 0x0051C060
// The localized category (TXT_SELECTION).

wchar_t const *CenterViewCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_SELECTION", 0, "D:\\Ra2\\Init.CPP", 6734);
}


// wchar_t const *CenterViewCommandClass::Get_Description() const -- 0x0051C080
// The localized description (TXT_CENTER_VIEW_DESC).

wchar_t const *CenterViewCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_CENTER_VIEW_DESC", 0, "D:\\Ra2\\Init.CPP", 6736);
}


// void CenterViewCommandClass::Execute(int key) const -- 0x0051C0A0
// Centres the view on the selected objects (0x0049E3B0) and redraws.

void CenterViewCommandClass::Execute(int key) const
{
    if (unknown_objects_a40c60.Count() > 0) {
        Map.Unknown_0049E3B0();
        Map.Flag_To_Redraw(true);
    }
}


// char const *CenterBaseCommandClass::Get_Name() const -- 0x0051C0D0
// The command's key name.

char const *CenterBaseCommandClass::Get_Name() const
{
    return "CenterBase";
}


// wchar_t const *CenterBaseCommandClass::Get_UI_Name() const -- 0x0051C0E0
// The localized name (TXT_CENTER_BASE).

wchar_t const *CenterBaseCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_CENTER_BASE", 0, "D:\\Ra2\\Init.CPP", 6763);
}


// wchar_t const *CenterBaseCommandClass::Get_Category() const -- 0x0051C100
// The localized category (TXT_SELECTION).

wchar_t const *CenterBaseCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_SELECTION", 0, "D:\\Ra2\\Init.CPP", 6765);
}


// wchar_t const *CenterBaseCommandClass::Get_Description() const -- 0x0051C120
// The localized description (TXT_CENTER_BASE_DESC).

wchar_t const *CenterBaseCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_CENTER_BASE_DESC", 0, "D:\\Ra2\\Init.CPP", 6767);
}


// void CenterBaseCommandClass::Execute(int key) const -- 0x0051C140
// Centres the view on the player's construction yard (the first one with
// +0x32B set, else the last found), else on another building, else on a
// base unit of the player's side (Rules BaseUnit).
#include "shared/building_type_class.hpp"

void CenterBaseCommandClass::Execute(int key) const
{
    CoordStruct yard = unknown_geometry_l;
    CoordStruct other = unknown_geometry_l;
    if (unknown_player_a35db4->CurBuildings) {
        for (int index = 0; index < unknown_BuildingClass_list_00a40af8.Count(); index++) {
            BuildingClass *building = unknown_BuildingClass_list_00a40af8[index];
            if (building != 0 && !building->unknown_76 && building->House->Is_Player_Control()) {
                if (building->Type->ConstructionYard) {
                    yard = building->Center_Coord();
                    if (building->unknown_32b) {
                        break;
                    }
                } else if (other == unknown_geometry_l) {
                    other = building->Center_Coord();
                }
            }
        }
    }
    if (other == unknown_geometry_l && yard == unknown_geometry_l && unknown_player_a35db4->unknown_228) {
        for (int index = 0; index < Units.Count(); index++) {
            UnitClass *unit = Units[index];
            if (unit != 0 && !unit->unknown_76 && unit->House->Is_Player_Control()
                && unit->Type == unknown_player_a35db4->Unknown_004EF6F0(Rules->BaseUnit)) {
                yard = unit->Center_Coord();
                break;
            }
        }
    }
    if (yard != unknown_geometry_l) {
        unknown_tactical_83996c->Set_Tactical_Position_006a66b0(yard);
    } else if (other != unknown_geometry_l) {
        unknown_tactical_83996c->Set_Tactical_Position_006a66b0(other);
    }
    if (Map.unknown_11a4 != 0) {
        Map.Unknown_004994D0(unknown_geometry_k);
    }
    Map.Unknown_0049EC60(0);
    Map.Flag_To_Redraw(true);
}


// char const *AllianceCommandClass::Get_Name() const -- 0x0051C3C0
// The command's key name.

char const *AllianceCommandClass::Get_Name() const
{
    return "ToggleAlliance";
}


// wchar_t const *AllianceCommandClass::Get_UI_Name() const -- 0x0051C3D0
// The localized name (TXT_ALLIANCE).

wchar_t const *AllianceCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_ALLIANCE", 0, "D:\\Ra2\\Init.CPP", 6848);
}


// wchar_t const *AllianceCommandClass::Get_Category() const -- 0x0051C3F0
// The localized category (TXT_CONTROL).

wchar_t const *AllianceCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_CONTROL", 0, "D:\\Ra2\\Init.CPP", 6850);
}


// wchar_t const *AllianceCommandClass::Get_Description() const -- 0x0051C410
// The localized description (TXT_ALLIANCE_DESC).

wchar_t const *AllianceCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_ALLIANCE_DESC", 0, "D:\\Ra2\\Init.CPP", 6852);
}


// void AllianceCommandClass::Execute(int key) const -- 0x0051C430
// Queues the alliance event for the object under the cursor (0x006F6380).
void Unknown_006F6380();

void AllianceCommandClass::Execute(int key) const
{
    Unknown_006F6380();
}


// char const *ToggleRepairCommandClass::Get_Name() const -- 0x0051C440
// The command's key name.

char const *ToggleRepairCommandClass::Get_Name() const
{
    return "ToggleRepair";
}


// wchar_t const *ToggleRepairCommandClass::Get_UI_Name() const -- 0x0051C450
// The localized name (TXT_REPAIR_MODE).

wchar_t const *ToggleRepairCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_REPAIR_MODE", 0, "D:\\Ra2\\Init.CPP", 6906);
}


// wchar_t const *ToggleRepairCommandClass::Get_Category() const -- 0x0051C470
// The localized category (TXT_INTERFACE).

wchar_t const *ToggleRepairCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 6908);
}


// wchar_t const *ToggleRepairCommandClass::Get_Description() const -- 0x0051C490
// The localized description (TXT_REPAIR_MODE_DESC).

wchar_t const *ToggleRepairCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_REPAIR_MODE_DESC", 0, "D:\\Ra2\\Init.CPP", 6910);
}


// void ToggleRepairCommandClass::Execute(int key) const -- 0x0051C4B0
// Toggles the repair mode (0x0049CA20, -1).

void ToggleRepairCommandClass::Execute(int key) const
{
    Map.Unknown_0049CA20(-1);
}


// char const *ToggleSellCommandClass::Get_Name() const -- 0x0051C4C0
// The command's key name.

char const *ToggleSellCommandClass::Get_Name() const
{
    return "ToggleSell";
}


// wchar_t const *ToggleSellCommandClass::Get_UI_Name() const -- 0x0051C4D0
// The localized name (TXT_SELL_MODE).

wchar_t const *ToggleSellCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SELL_MODE", 0, "D:\\Ra2\\Init.CPP", 6932);
}


// wchar_t const *ToggleSellCommandClass::Get_Category() const -- 0x0051C4F0
// The localized category (TXT_INTERFACE).

wchar_t const *ToggleSellCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 6934);
}


// wchar_t const *ToggleSellCommandClass::Get_Description() const -- 0x0051C510
// The localized description (TXT_SELL_MODE_DESC).

wchar_t const *ToggleSellCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SELL_MODE_DESC", 0, "D:\\Ra2\\Init.CPP", 6936);
}


// void ToggleSellCommandClass::Execute(int key) const -- 0x0051C530
// Toggles the sell mode (0x0049C7C0, -1).

void ToggleSellCommandClass::Execute(int key) const
{
    Map.Unknown_0049C7C0(-1);
}


// char const *BeaconPlacementCommandClass::Get_Name() const -- 0x0051C540
// The command's key name.

char const *BeaconPlacementCommandClass::Get_Name() const
{
    return "PlaceBeacon";
}


// wchar_t const *BeaconPlacementCommandClass::Get_UI_Name() const -- 0x0051C550
// The localized name (TXT_PLACE_BEACON).

wchar_t const *BeaconPlacementCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_PLACE_BEACON", 0, "D:\\Ra2\\Init.CPP", 6975);
}


// wchar_t const *BeaconPlacementCommandClass::Get_Category() const -- 0x0051C570
// The localized category (TXT_INTERFACE).

wchar_t const *BeaconPlacementCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 6977);
}


// wchar_t const *BeaconPlacementCommandClass::Get_Description() const -- 0x0051C590
// The localized description (TXT_PLACE_BEACON_DESC).

wchar_t const *BeaconPlacementCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_PLACE_BEACON_DESC", 0, "D:\\Ra2\\Init.CPP", 6979);
}


// void BeaconPlacementCommandClass::Execute(int key) const -- 0x0051C5B0
// Beacon placement (0x006F64B0) outside the campaign and skirmish (game modes 0 and 5).

void BeaconPlacementCommandClass::Execute(int key) const
{
    if (unknown_game_mode_a3d298 != 0 && unknown_game_mode_a3d298 != 5) {
        Unknown_006F64B0();
    }
}


// char const *CenterREventCommandClass::Get_Name() const -- 0x0051C5D0
// The command's key name.

char const *CenterREventCommandClass::Get_Name() const
{
    return "CenterOnRadarEvent";
}


// wchar_t const *CenterREventCommandClass::Get_UI_Name() const -- 0x0051C5E0
// The localized name (TXT_RADAR_EVENT).

wchar_t const *CenterREventCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_RADAR_EVENT", 0, "D:\\Ra2\\Init.CPP", 6998);
}


// wchar_t const *CenterREventCommandClass::Get_Category() const -- 0x0051C600
// The localized category (TXT_INTERFACE).

wchar_t const *CenterREventCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7000);
}


// wchar_t const *CenterREventCommandClass::Get_Description() const -- 0x0051C620
// The localized description (TXT_RADAR_EVENT_DESC).

wchar_t const *CenterREventCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_RADAR_EVENT_DESC", 0, "D:\\Ra2\\Init.CPP", 7002);
}


// void CenterREventCommandClass::Execute(int key) const -- 0x0051C640
// Jumps to the last radar event (0x006F88F0).
void Unknown_006F88F0();

void CenterREventCommandClass::Execute(int key) const
{
    Unknown_006F88F0();
}


// char const *SidebarUpCommandClass::Get_Name() const -- 0x0051C650
// The command's key name.

char const *SidebarUpCommandClass::Get_Name() const
{
    return "SidebarUp";
}


// wchar_t const *SidebarUpCommandClass::Get_UI_Name() const -- 0x0051C660
// The localized name (TXT_SIDEBAR_UP).

wchar_t const *SidebarUpCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SIDEBAR_UP", 0, "D:\\Ra2\\Init.CPP", 7052);
}


// wchar_t const *SidebarUpCommandClass::Get_Category() const -- 0x0051C680
// The localized category (TXT_INTERFACE).

wchar_t const *SidebarUpCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7054);
}


// wchar_t const *SidebarUpCommandClass::Get_Description() const -- 0x0051C6A0
// The localized description (TXT_SIDEBAR_UP_DESC).

wchar_t const *SidebarUpCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SIDEBAR_UP_DESC", 0, "D:\\Ra2\\Init.CPP", 7056);
}


// void SidebarUpCommandClass::Execute(int key) const -- 0x0051C6C0
// Scrolls the current column up by a row (SidebarClass::Scroll).

void SidebarUpCommandClass::Execute(int key) const
{
    Map.Scroll(true, -1);
}


// char const *LSidebarUpCommandClass::Get_Name() const -- 0x0051C6E0
// The command's key name.

char const *LSidebarUpCommandClass::Get_Name() const
{
    return "LeftSidebarUp";
}


// wchar_t const *LSidebarUpCommandClass::Get_UI_Name() const -- 0x0051C6F0
// The localized name (TXT_LSIDEBAR_UP).

wchar_t const *LSidebarUpCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_LSIDEBAR_UP", 0, "D:\\Ra2\\Init.CPP", 7078);
}


// wchar_t const *LSidebarUpCommandClass::Get_Category() const -- 0x0051C710
// The localized category (TXT_INTERFACE).

wchar_t const *LSidebarUpCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7080);
}


// wchar_t const *LSidebarUpCommandClass::Get_Description() const -- 0x0051C730
// The localized description (TXT_LSIDEBAR_UP_DESC).

wchar_t const *LSidebarUpCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_LSIDEBAR_UP_DESC", 0, "D:\\Ra2\\Init.CPP", 7082);
}


// void LSidebarUpCommandClass::Execute(int key) const -- 0x0051C750
// Scrolls the left column up by a row (SidebarClass::Scroll).

void LSidebarUpCommandClass::Execute(int key) const
{
    Map.Scroll(true, 0);
}


// char const *RSidebarUpCommandClass::Get_Name() const -- 0x0051C770
// The command's key name.

char const *RSidebarUpCommandClass::Get_Name() const
{
    return "RightSidebarUp";
}


// wchar_t const *RSidebarUpCommandClass::Get_UI_Name() const -- 0x0051C780
// The localized name (TXT_RSIDEBAR_UP).

wchar_t const *RSidebarUpCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_RSIDEBAR_UP", 0, "D:\\Ra2\\Init.CPP", 7105);
}


// wchar_t const *RSidebarUpCommandClass::Get_Category() const -- 0x0051C7A0
// The localized category (TXT_INTERFACE).

wchar_t const *RSidebarUpCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7107);
}


// wchar_t const *RSidebarUpCommandClass::Get_Description() const -- 0x0051C7C0
// The localized description (TXT_RSIDEBAR_UP_DESC).

wchar_t const *RSidebarUpCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_RSIDEBAR_UP_DESC", 0, "D:\\Ra2\\Init.CPP", 7109);
}


// void RSidebarUpCommandClass::Execute(int key) const -- 0x0051C7E0
// Scrolls the right column up by a row (SidebarClass::Scroll).

void RSidebarUpCommandClass::Execute(int key) const
{
    Map.Scroll(true, 1);
}


// char const *SidebarPageUpCommandClass::Get_Name() const -- 0x0051C800
// The command's key name.

char const *SidebarPageUpCommandClass::Get_Name() const
{
    return "SidebarPageUp";
}


// wchar_t const *SidebarPageUpCommandClass::Get_UI_Name() const -- 0x0051C810
// The localized name (TXT_SIDEBAR_PGUP).

wchar_t const *SidebarPageUpCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SIDEBAR_PGUP", 0, "D:\\Ra2\\Init.CPP", 7131);
}


// wchar_t const *SidebarPageUpCommandClass::Get_Category() const -- 0x0051C830
// The localized category (TXT_INTERFACE).

wchar_t const *SidebarPageUpCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7133);
}


// wchar_t const *SidebarPageUpCommandClass::Get_Description() const -- 0x0051C850
// The localized description (TXT_SIDEBAR_PGUP_DESC).

wchar_t const *SidebarPageUpCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SIDEBAR_PGUP_DESC", 0, "D:\\Ra2\\Init.CPP", 7135);
}


// void SidebarPageUpCommandClass::Execute(int key) const -- 0x0051C870
// Scrolls the current column up by a page (SidebarClass::Scroll_Page).

void SidebarPageUpCommandClass::Execute(int key) const
{
    Map.Scroll_Page(true, -1);
}


// char const *LSidebarPageUpCommandClass::Get_Name() const -- 0x0051C890
// The command's key name.

char const *LSidebarPageUpCommandClass::Get_Name() const
{
    return "LeftSidebarPageUp";
}


// wchar_t const *LSidebarPageUpCommandClass::Get_UI_Name() const -- 0x0051C8A0
// The localized name (TXT_LSIDEBAR_PGUP).

wchar_t const *LSidebarPageUpCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_LSIDEBAR_PGUP", 0, "D:\\Ra2\\Init.CPP", 7149);
}


// wchar_t const *LSidebarPageUpCommandClass::Get_Category() const -- 0x0051C8C0
// The localized category (TXT_INTERFACE).

wchar_t const *LSidebarPageUpCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7151);
}


// wchar_t const *LSidebarPageUpCommandClass::Get_Description() const -- 0x0051C8E0
// The localized description (TXT_LSIDEBAR_PGUP_DESC).

wchar_t const *LSidebarPageUpCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_LSIDEBAR_PGUP_DESC", 0, "D:\\Ra2\\Init.CPP", 7153);
}


// void LSidebarPageUpCommandClass::Execute(int key) const -- 0x0051C900
// Scrolls the left column up by a page (SidebarClass::Scroll_Page).

void LSidebarPageUpCommandClass::Execute(int key) const
{
    Map.Scroll_Page(true, 0);
}


// char const *RSidebarPageUpCommandClass::Get_Name() const -- 0x0051C920
// The command's key name.

char const *RSidebarPageUpCommandClass::Get_Name() const
{
    return "RightSidebarPageUp";
}


// wchar_t const *RSidebarPageUpCommandClass::Get_UI_Name() const -- 0x0051C930
// The localized name (TXT_RSIDEBAR_PGUP).

wchar_t const *RSidebarPageUpCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_RSIDEBAR_PGUP", 0, "D:\\Ra2\\Init.CPP", 7167);
}


// wchar_t const *RSidebarPageUpCommandClass::Get_Category() const -- 0x0051C950
// The localized category (TXT_INTERFACE).

wchar_t const *RSidebarPageUpCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7169);
}


// wchar_t const *RSidebarPageUpCommandClass::Get_Description() const -- 0x0051C970
// The localized description (TXT_RSIDEBAR_PGUP_DESC).

wchar_t const *RSidebarPageUpCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_RSIDEBAR_PGUP_DESC", 0, "D:\\Ra2\\Init.CPP", 7171);
}


// void RSidebarPageUpCommandClass::Execute(int key) const -- 0x0051C990
// Scrolls the right column up by a page (SidebarClass::Scroll_Page).

void RSidebarPageUpCommandClass::Execute(int key) const
{
    Map.Scroll_Page(true, 1);
}


// char const *SidebarDownCommandClass::Get_Name() const -- 0x0051C9B0
// The command's key name.

char const *SidebarDownCommandClass::Get_Name() const
{
    return "SidebarDown";
}


// wchar_t const *SidebarDownCommandClass::Get_UI_Name() const -- 0x0051C9C0
// The localized name (TXT_SIDEBAR_DOWN).

wchar_t const *SidebarDownCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SIDEBAR_DOWN", 0, "D:\\Ra2\\Init.CPP", 7193);
}


// wchar_t const *SidebarDownCommandClass::Get_Category() const -- 0x0051C9E0
// The localized category (TXT_INTERFACE).

wchar_t const *SidebarDownCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7195);
}


// wchar_t const *SidebarDownCommandClass::Get_Description() const -- 0x0051CA00
// The localized description (TXT_SIDEBAR_DOWN_DESC).

wchar_t const *SidebarDownCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SIDEBAR_DOWN_DESC", 0, "D:\\Ra2\\Init.CPP", 7197);
}


// void SidebarDownCommandClass::Execute(int key) const -- 0x0051CA20
// Scrolls the current column down by a row (SidebarClass::Scroll).

void SidebarDownCommandClass::Execute(int key) const
{
    Map.Scroll(false, -1);
}


// char const *LSidebarDownCommandClass::Get_Name() const -- 0x0051CA40
// The command's key name.

char const *LSidebarDownCommandClass::Get_Name() const
{
    return "LeftSidebarDown";
}


// wchar_t const *LSidebarDownCommandClass::Get_UI_Name() const -- 0x0051CA50
// The localized name (TXT_LSIDEBAR_DOWN).

wchar_t const *LSidebarDownCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_LSIDEBAR_DOWN", 0, "D:\\Ra2\\Init.CPP", 7219);
}


// wchar_t const *LSidebarDownCommandClass::Get_Category() const -- 0x0051CA70
// The localized category (TXT_INTERFACE).

wchar_t const *LSidebarDownCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7221);
}


// wchar_t const *LSidebarDownCommandClass::Get_Description() const -- 0x0051CA90
// The localized description (TXT_LSIDEBAR_DOWN_DESC).

wchar_t const *LSidebarDownCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_LSIDEBAR_DOWN_DESC", 0, "D:\\Ra2\\Init.CPP", 7223);
}


// void LSidebarDownCommandClass::Execute(int key) const -- 0x0051CAB0
// Scrolls the left column down by a row (SidebarClass::Scroll).

void LSidebarDownCommandClass::Execute(int key) const
{
    Map.Scroll(false, 0);
}


// char const *RSidebarDownCommandClass::Get_Name() const -- 0x0051CAD0
// The command's key name.

char const *RSidebarDownCommandClass::Get_Name() const
{
    return "RightSidebarDown";
}


// wchar_t const *RSidebarDownCommandClass::Get_UI_Name() const -- 0x0051CAE0
// The localized name (TXT_RSIDEBAR_DOWN).

wchar_t const *RSidebarDownCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_RSIDEBAR_DOWN", 0, "D:\\Ra2\\Init.CPP", 7246);
}


// wchar_t const *RSidebarDownCommandClass::Get_Category() const -- 0x0051CB00
// The localized category (TXT_INTERFACE).

wchar_t const *RSidebarDownCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7248);
}


// wchar_t const *RSidebarDownCommandClass::Get_Description() const -- 0x0051CB20
// The localized description (TXT_RSIDEBAR_DOWN_DESC).

wchar_t const *RSidebarDownCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_RSIDEBAR_DOWN_DESC", 0, "D:\\Ra2\\Init.CPP", 7250);
}


// void RSidebarDownCommandClass::Execute(int key) const -- 0x0051CB40
// Scrolls the right column down by a row (SidebarClass::Scroll).

void RSidebarDownCommandClass::Execute(int key) const
{
    Map.Scroll(false, 1);
}


// char const *SidebarPageDownCommandClass::Get_Name() const -- 0x0051CB60
// The command's key name.

char const *SidebarPageDownCommandClass::Get_Name() const
{
    return "SidebarPageDown";
}


// wchar_t const *SidebarPageDownCommandClass::Get_UI_Name() const -- 0x0051CB70
// The localized name (TXT_SIDEBAR_PGDN).

wchar_t const *SidebarPageDownCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SIDEBAR_PGDN", 0, "D:\\Ra2\\Init.CPP", 7272);
}


// wchar_t const *SidebarPageDownCommandClass::Get_Category() const -- 0x0051CB90
// The localized category (TXT_INTERFACE).

wchar_t const *SidebarPageDownCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7274);
}


// wchar_t const *SidebarPageDownCommandClass::Get_Description() const -- 0x0051CBB0
// The localized description (TXT_SIDEBAR_PGDN_DESC).

wchar_t const *SidebarPageDownCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SIDEBAR_PGDN_DESC", 0, "D:\\Ra2\\Init.CPP", 7276);
}


// void SidebarPageDownCommandClass::Execute(int key) const -- 0x0051CBD0
// Scrolls the current column down by a page (SidebarClass::Scroll_Page).

void SidebarPageDownCommandClass::Execute(int key) const
{
    Map.Scroll_Page(false, -1);
}


// char const *LSidebarPageDownCommandClass::Get_Name() const -- 0x0051CBF0
// The command's key name.

char const *LSidebarPageDownCommandClass::Get_Name() const
{
    return "LeftSidebarPageDown";
}


// wchar_t const *LSidebarPageDownCommandClass::Get_UI_Name() const -- 0x0051CC00
// The localized name (TXT_LSIDEBAR_PGDN).

wchar_t const *LSidebarPageDownCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_LSIDEBAR_PGDN", 0, "D:\\Ra2\\Init.CPP", 7290);
}


// wchar_t const *LSidebarPageDownCommandClass::Get_Category() const -- 0x0051CC20
// The localized category (TXT_INTERFACE).

wchar_t const *LSidebarPageDownCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7292);
}


// wchar_t const *LSidebarPageDownCommandClass::Get_Description() const -- 0x0051CC40
// The localized description (TXT_LSIDEBAR_PGDN_DESC).

wchar_t const *LSidebarPageDownCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_LSIDEBAR_PGDN_DESC", 0, "D:\\Ra2\\Init.CPP", 7294);
}


// void LSidebarPageDownCommandClass::Execute(int key) const -- 0x0051CC60
// Scrolls the left column down by a page (SidebarClass::Scroll_Page).

void LSidebarPageDownCommandClass::Execute(int key) const
{
    Map.Scroll_Page(false, 0);
}


// char const *RSidebarPageDownCommandClass::Get_Name() const -- 0x0051CC80
// The command's key name.

char const *RSidebarPageDownCommandClass::Get_Name() const
{
    return "RightSidebarPageDown";
}


// wchar_t const *RSidebarPageDownCommandClass::Get_UI_Name() const -- 0x0051CC90
// The localized name (TXT_RSIDEBAR_PGDN).

wchar_t const *RSidebarPageDownCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_RSIDEBAR_PGDN", 0, "D:\\Ra2\\Init.CPP", 7308);
}


// wchar_t const *RSidebarPageDownCommandClass::Get_Category() const -- 0x0051CCB0
// The localized category (TXT_INTERFACE).

wchar_t const *RSidebarPageDownCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7310);
}


// wchar_t const *RSidebarPageDownCommandClass::Get_Description() const -- 0x0051CCD0
// The localized description (TXT_RSIDEBAR_PGDN_DESC).

wchar_t const *RSidebarPageDownCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_RSIDEBAR_PGDN_DESC", 0, "D:\\Ra2\\Init.CPP", 7312);
}


// void RSidebarPageDownCommandClass::Execute(int key) const -- 0x0051CCF0
// Scrolls the right column down by a page (SidebarClass::Scroll_Page).

void RSidebarPageDownCommandClass::Execute(int key) const
{
    Map.Scroll_Page(false, 1);
}


// char const *OptionsCommandClass::Get_Name() const -- 0x0051CD10
// The command's key name.

char const *OptionsCommandClass::Get_Name() const
{
    return "Options";
}


// wchar_t const *OptionsCommandClass::Get_UI_Name() const -- 0x0051CD20
// The localized name (TXT_OPTIONS).

wchar_t const *OptionsCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_OPTIONS", 0, "D:\\Ra2\\Init.CPP", 7335);
}


// wchar_t const *OptionsCommandClass::Get_Category() const -- 0x0051CD40
// The localized category (TXT_INTERFACE).

wchar_t const *OptionsCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7337);
}


// wchar_t const *OptionsCommandClass::Get_Description() const -- 0x0051CD60
// The localized description (TXT_OPTIONS_DESC).

wchar_t const *OptionsCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_OPTIONS_DESC", 0, "D:\\Ra2\\Init.CPP", 7339);
}


// void OptionsCommandClass::Execute(int key) const -- 0x0051CD80
// Opens the options menu (Queue_Options 0x00623540).

void OptionsCommandClass::Execute(int key) const
{
    Queue_Options();
}


// char const *View1CommandClass::Get_Name() const -- 0x0051CD90
// The command's key name.

char const *View1CommandClass::Get_Name() const
{
    return "View1";
}


// wchar_t const *View1CommandClass::Get_UI_Name() const -- 0x0051CDA0
// The localized name (TXT_VIEW_BOOKMARK1).

wchar_t const *View1CommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_VIEW_BOOKMARK1", 0, "D:\\Ra2\\Init.CPP", 7512);
}


// wchar_t const *View1CommandClass::Get_Category() const -- 0x0051CDC0
// The localized category (TXT_INTERFACE).

wchar_t const *View1CommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7514);
}


// wchar_t const *View1CommandClass::Get_Description() const -- 0x0051CDE0
// The localized description (TXT_VIEW_BOOKMARK1_DESC).

wchar_t const *View1CommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_VIEW_BOOKMARK1_DESC", 0, "D:\\Ra2\\Init.CPP", 7516);
}


// void View1CommandClass::Execute(int key) const -- 0x0051CE00
// Moves the tactical view to remembered view 1 (the scenario's Views[0]) at
// its ground height.

void View1CommandClass::Execute(int key) const
{
    CoordStruct coord(unknown_id_source_a3d290->Views[0].X * 256 + 128, unknown_id_source_a3d290->Views[0].Y * 256 + 128, 0);
    coord.Z = Map.Get_Height_GL(coord);
    unknown_tactical_83996c->Set_Tactical_Position_006a66b0(coord);
    if (Map.unknown_11a4 != 0) {
        Map.Unknown_004994D0(unknown_geometry_k);
    }
}


// char const *View2CommandClass::Get_Name() const -- 0x0051CE80
// The command's key name.

char const *View2CommandClass::Get_Name() const
{
    return "View2";
}


// wchar_t const *View2CommandClass::Get_UI_Name() const -- 0x0051CE90
// The localized name (TXT_VIEW_BOOKMARK2).

wchar_t const *View2CommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_VIEW_BOOKMARK2", 0, "D:\\Ra2\\Init.CPP", 7548);
}


// wchar_t const *View2CommandClass::Get_Category() const -- 0x0051CEB0
// The localized category (TXT_INTERFACE).

wchar_t const *View2CommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7550);
}


// wchar_t const *View2CommandClass::Get_Description() const -- 0x0051CED0
// The localized description (TXT_VIEW_BOOKMARK2_DESC).

wchar_t const *View2CommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_VIEW_BOOKMARK2_DESC", 0, "D:\\Ra2\\Init.CPP", 7552);
}


// void View2CommandClass::Execute(int key) const -- 0x0051CEF0
// Moves the tactical view to remembered view 2 (the scenario's Views[1]) at
// its ground height.

void View2CommandClass::Execute(int key) const
{
    CoordStruct coord(unknown_id_source_a3d290->Views[1].X * 256 + 128, unknown_id_source_a3d290->Views[1].Y * 256 + 128, 0);
    coord.Z = Map.Get_Height_GL(coord);
    unknown_tactical_83996c->Set_Tactical_Position_006a66b0(coord);
    if (Map.unknown_11a4 != 0) {
        Map.Unknown_004994D0(unknown_geometry_k);
    }
}


// char const *View3CommandClass::Get_Name() const -- 0x0051CF70
// The command's key name.

char const *View3CommandClass::Get_Name() const
{
    return "View3";
}


// wchar_t const *View3CommandClass::Get_UI_Name() const -- 0x0051CF80
// The localized name (TXT_VIEW_BOOKMARK3).

wchar_t const *View3CommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_VIEW_BOOKMARK3", 0, "D:\\Ra2\\Init.CPP", 7584);
}


// wchar_t const *View3CommandClass::Get_Category() const -- 0x0051CFA0
// The localized category (TXT_INTERFACE).

wchar_t const *View3CommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7586);
}


// wchar_t const *View3CommandClass::Get_Description() const -- 0x0051CFC0
// The localized description (TXT_VIEW_BOOKMARK3_DESC).

wchar_t const *View3CommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_VIEW_BOOKMARK3_DESC", 0, "D:\\Ra2\\Init.CPP", 7588);
}


// void View3CommandClass::Execute(int key) const -- 0x0051CFE0
// Moves the tactical view to remembered view 3 (the scenario's Views[2]) at
// its ground height.

void View3CommandClass::Execute(int key) const
{
    CoordStruct coord(unknown_id_source_a3d290->Views[2].X * 256 + 128, unknown_id_source_a3d290->Views[2].Y * 256 + 128, 0);
    coord.Z = Map.Get_Height_GL(coord);
    unknown_tactical_83996c->Set_Tactical_Position_006a66b0(coord);
    if (Map.unknown_11a4 != 0) {
        Map.Unknown_004994D0(unknown_geometry_k);
    }
}


// char const *View4CommandClass::Get_Name() const -- 0x0051D060
// The command's key name.

char const *View4CommandClass::Get_Name() const
{
    return "View4";
}


// wchar_t const *View4CommandClass::Get_UI_Name() const -- 0x0051D070
// The localized name (TXT_VIEW_BOOKMARK4).

wchar_t const *View4CommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_VIEW_BOOKMARK4", 0, "D:\\Ra2\\Init.CPP", 7620);
}


// wchar_t const *View4CommandClass::Get_Category() const -- 0x0051D090
// The localized category (TXT_INTERFACE).

wchar_t const *View4CommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7622);
}


// wchar_t const *View4CommandClass::Get_Description() const -- 0x0051D0B0
// The localized description (TXT_VIEW_BOOKMARK4_DESC).

wchar_t const *View4CommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_VIEW_BOOKMARK4_DESC", 0, "D:\\Ra2\\Init.CPP", 7624);
}


// void View4CommandClass::Execute(int key) const -- 0x0051D0D0
// Moves the tactical view to remembered view 4 (the scenario's Views[3]) at
// its ground height.

void View4CommandClass::Execute(int key) const
{
    CoordStruct coord(unknown_id_source_a3d290->Views[3].X * 256 + 128, unknown_id_source_a3d290->Views[3].Y * 256 + 128, 0);
    coord.Z = Map.Get_Height_GL(coord);
    unknown_tactical_83996c->Set_Tactical_Position_006a66b0(coord);
    if (Map.unknown_11a4 != 0) {
        Map.Unknown_004994D0(unknown_geometry_k);
    }
}


// char const *SetView1CommandClass::Get_Name() const -- 0x0051D150
// The command's key name.

char const *SetView1CommandClass::Get_Name() const
{
    return "SetView1";
}


// wchar_t const *SetView1CommandClass::Get_UI_Name() const -- 0x0051D160
// The localized name (TXT_SET_BOOKMARK1).

wchar_t const *SetView1CommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SET_BOOKMARK1", 0, "D:\\Ra2\\Init.CPP", 7656);
}


// wchar_t const *SetView1CommandClass::Get_Category() const -- 0x0051D180
// The localized category (TXT_INTERFACE).

wchar_t const *SetView1CommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7658);
}


// wchar_t const *SetView1CommandClass::Get_Description() const -- 0x0051D1A0
// The localized description (TXT_SET_BOOKMARK1_DESC).

wchar_t const *SetView1CommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SET_BOOKMARK1_DESC", 0, "D:\\Ra2\\Init.CPP", 7660);
}


// void SetView1CommandClass::Execute(int key) const -- 0x0051D1C0
// Remembers the cell under the view position as view 1.

void SetView1CommandClass::Execute(int key) const
{
    Point2D point = unknown_tactical_83996c->Unknown_006A67D0();
    unknown_id_source_a3d290->Views[0] = unknown_tactical_83996c->Pixel_To_Cell(point);
}


// char const *SetView2CommandClass::Get_Name() const -- 0x0051D210
// The command's key name.

char const *SetView2CommandClass::Get_Name() const
{
    return "SetView2";
}


// wchar_t const *SetView2CommandClass::Get_UI_Name() const -- 0x0051D220
// The localized name (TXT_SET_BOOKMARK2).

wchar_t const *SetView2CommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SET_BOOKMARK2", 0, "D:\\Ra2\\Init.CPP", 7683);
}


// wchar_t const *SetView2CommandClass::Get_Category() const -- 0x0051D240
// The localized category (TXT_INTERFACE).

wchar_t const *SetView2CommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7685);
}


// wchar_t const *SetView2CommandClass::Get_Description() const -- 0x0051D260
// The localized description (TXT_SET_BOOKMARK2_DESC).

wchar_t const *SetView2CommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SET_BOOKMARK2_DESC", 0, "D:\\Ra2\\Init.CPP", 7687);
}


// void SetView2CommandClass::Execute(int key) const -- 0x0051D280
// Remembers the cell under the view position as view 2.

void SetView2CommandClass::Execute(int key) const
{
    Point2D point = unknown_tactical_83996c->Unknown_006A67D0();
    unknown_id_source_a3d290->Views[1] = unknown_tactical_83996c->Pixel_To_Cell(point);
}


// char const *SetView3CommandClass::Get_Name() const -- 0x0051D2D0
// The command's key name.

char const *SetView3CommandClass::Get_Name() const
{
    return "SetView3";
}


// wchar_t const *SetView3CommandClass::Get_UI_Name() const -- 0x0051D2E0
// The localized name (TXT_SET_BOOKMARK3).

wchar_t const *SetView3CommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SET_BOOKMARK3", 0, "D:\\Ra2\\Init.CPP", 7710);
}


// wchar_t const *SetView3CommandClass::Get_Category() const -- 0x0051D300
// The localized category (TXT_INTERFACE).

wchar_t const *SetView3CommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7712);
}


// wchar_t const *SetView3CommandClass::Get_Description() const -- 0x0051D320
// The localized description (TXT_SET_BOOKMARK3_DESC).

wchar_t const *SetView3CommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SET_BOOKMARK3_DESC", 0, "D:\\Ra2\\Init.CPP", 7714);
}


// void SetView3CommandClass::Execute(int key) const -- 0x0051D340
// Remembers the cell under the view position as view 3.

void SetView3CommandClass::Execute(int key) const
{
    Point2D point = unknown_tactical_83996c->Unknown_006A67D0();
    unknown_id_source_a3d290->Views[2] = unknown_tactical_83996c->Pixel_To_Cell(point);
}


// char const *SetView4CommandClass::Get_Name() const -- 0x0051D390
// The command's key name.

char const *SetView4CommandClass::Get_Name() const
{
    return "SetView4";
}


// wchar_t const *SetView4CommandClass::Get_UI_Name() const -- 0x0051D3A0
// The localized name (TXT_SET_BOOKMARK4).

wchar_t const *SetView4CommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SET_BOOKMARK4", 0, "D:\\Ra2\\Init.CPP", 7737);
}


// wchar_t const *SetView4CommandClass::Get_Category() const -- 0x0051D3C0
// The localized category (TXT_INTERFACE).

wchar_t const *SetView4CommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7739);
}


// wchar_t const *SetView4CommandClass::Get_Description() const -- 0x0051D3E0
// The localized description (TXT_SET_BOOKMARK4_DESC).

wchar_t const *SetView4CommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SET_BOOKMARK4_DESC", 0, "D:\\Ra2\\Init.CPP", 7741);
}


// void SetView4CommandClass::Execute(int key) const -- 0x0051D400
// Remembers the cell under the view position as view 4.

void SetView4CommandClass::Execute(int key) const
{
    Point2D point = unknown_tactical_83996c->Unknown_006A67D0();
    unknown_id_source_a3d290->Views[3] = unknown_tactical_83996c->Pixel_To_Cell(point);
}


// char const *FollowCommandClass::Get_Name() const -- 0x0051D450
// The command's key name.

char const *FollowCommandClass::Get_Name() const
{
    return "Follow";
}


// wchar_t const *FollowCommandClass::Get_UI_Name() const -- 0x0051D460
// The localized name (TXT_FOLLOW).

wchar_t const *FollowCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_FOLLOW", 0, "D:\\Ra2\\Init.CPP", 7764);
}


// wchar_t const *FollowCommandClass::Get_Category() const -- 0x0051D480
// The localized category (TXT_INTERFACE).

wchar_t const *FollowCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7766);
}


// wchar_t const *FollowCommandClass::Get_Description() const -- 0x0051D4A0
// The localized description (TXT_FOLLOW_DESC).

wchar_t const *FollowCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_FOLLOW_DESC", 0, "D:\\Ra2\\Init.CPP", 7768);
}


// void FollowCommandClass::Execute(int key) const -- 0x0051D4C0
// Follows the first selected object unless the view already follows one;
// stops following otherwise.

void FollowCommandClass::Execute(int key) const
{
    if (unknown_objects_a40c60.Count() != 0 && Map.Unknown_0049EC40() == 0) {
        Map.Unknown_0049EC60(unknown_objects_a40c60[0]);
    } else {
        Map.Unknown_0049EC60(0);
    }
}


// char const *ScreenCaptureCommandClass::Get_Name() const -- 0x0051D500
// The command's key name.

char const *ScreenCaptureCommandClass::Get_Name() const
{
    return "ScreenCapture";
}


// wchar_t const *ScreenCaptureCommandClass::Get_Category() const -- 0x0051D510
// The localized category (TXT_INTERFACE).

wchar_t const *ScreenCaptureCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7876);
}


// wchar_t const *ScreenCaptureCommandClass::Get_UI_Name() const -- 0x0051D530
// The localized name (TXT_SCRNCAP).

wchar_t const *ScreenCaptureCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SCRNCAP", 0, "D:\\Ra2\\Init.CPP", 7877);
}


// wchar_t const *ScreenCaptureCommandClass::Get_Description() const -- 0x0051D550
// The localized description (TXT_SCRNCAP_DESC).

wchar_t const *ScreenCaptureCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SCRNCAP_DESC", 0, "D:\\Ra2\\Init.CPP", 7878);
}


// void ScreenCaptureCommandClass::Execute(int key) const -- 0x0051D570
// Copies the game window's client area into the 0x00839954 surface and
// writes it to the first free SCRN%04d.pcx (Write_PCX_File).

extern "C" int __cdecl sprintf(char *buffer, char const *format, ...);
int __fastcall Write_PCX_File(FileClass &file, Surface &pic, PaletteClass *palette);

void ScreenCaptureCommandClass::Execute(int key) const
{
    RECT client;
    if (GetClientRect(MainWindow, &client)) {
        POINT topleft;
        topleft.x = client.left;
        topleft.y = client.top;
        if (ClientToScreen(MainWindow, &topleft)) {
            POINT bottomright;
            bottomright.x = client.right;
            bottomright.y = client.bottom;
            if (ClientToScreen(MainWindow, &bottomright)) {
                Rect area(topleft.x, topleft.y, client.right + 1, client.bottom + 1);
                if (area.Width >= unknown_surface_839954->Get_Width()) {
                    area.Width = unknown_surface_839954->Get_Width();
                }
                if (area.Height >= unknown_surface_839954->Get_Height()) {
                    area.Height = unknown_surface_839954->Get_Height();
                }
                WWMouse->Hide_Mouse();
                int width = unknown_surface_839954->Get_Width();
                int height = unknown_surface_839954->Get_Height();
                Rect full(0, 0, width, height);
                unknown_surface_839954->Copy_From(full, *unknown_surface_839950, area, false, true);
                WWMouse->Show_Mouse();
                char filename[128];
                int index = -1;
                bool exists;
                do {
                    index++;
                    sprintf(filename, "SCRN%04d.pcx", index);
                    exists = CCFileClass(filename).Is_Available();
                } while (exists);
                CCFileClass file(filename);
                Write_PCX_File(file, *unknown_surface_839954, &unknown_palette_837e10);
            }
        }
    }
}


// char const *MultiplayerDebugCommandClass::Get_Name() const -- 0x0051D780
// The command's key name.

char const *MultiplayerDebugCommandClass::Get_Name() const
{
    return "MultiplayerDebug";
}


// wchar_t const *MultiplayerDebugCommandClass::Get_Category() const -- 0x0051D790
// The command's category (not localized).

wchar_t const *MultiplayerDebugCommandClass::Get_Category() const
{
    return L"Debug";
}


// wchar_t const *MultiplayerDebugCommandClass::Get_UI_Name() const -- 0x0051D7A0
// The command's name (not localized).

wchar_t const *MultiplayerDebugCommandClass::Get_UI_Name() const
{
    return L"Multiplayer debug info";
}


// wchar_t const *MultiplayerDebugCommandClass::Get_Description() const -- 0x0051D7B0
// The command's description (not localized).

wchar_t const *MultiplayerDebugCommandClass::Get_Description() const
{
    return L"Turns multiplayer debug on or off.";
}


// void MultiplayerDebugCommandClass::Execute(int key) const -- 0x0051D7C0
// Toggles the multiplayer debug display (0x00A3D8C5) when debugging is
// allowed (0x00A3D8C4). Flag names are hypotheses.

extern bool unknown_debug_allowed_a3d8c4;
extern bool unknown_debug_display_a3d8c5;

void MultiplayerDebugCommandClass::Execute(int key) const
{
    if (unknown_debug_allowed_a3d8c4) {
        unknown_debug_display_a3d8c5 = !unknown_debug_display_a3d8c5;
    }
}


// char const *MultiplayerSyncCommandClass::Get_Name() const -- 0x0051D7E0
// The command's key name.

char const *MultiplayerSyncCommandClass::Get_Name() const
{
    return "ExtraSyncChecking";
}


// wchar_t const *MultiplayerSyncCommandClass::Get_Category() const -- 0x0051D7F0
// The command's category (not localized).

wchar_t const *MultiplayerSyncCommandClass::Get_Category() const
{
    return L"Debug";
}


// wchar_t const *MultiplayerSyncCommandClass::Get_UI_Name() const -- 0x0051D800
// The command's name (not localized).

wchar_t const *MultiplayerSyncCommandClass::Get_UI_Name() const
{
    return L"Game sync logging";
}


// wchar_t const *MultiplayerSyncCommandClass::Get_Description() const -- 0x0051D810
// The command's description (not localized).

wchar_t const *MultiplayerSyncCommandClass::Get_Description() const
{
    return L"Turns extra game sync logging on or off.";
}


// void MultiplayerSyncCommandClass::Execute(int key) const -- 0x0051D820
// Toggles the extra sync information (0x00AB62E8) and announces the new
// state in the message list (0x00A3DC70, Add_Message 0x005B4DA0).

extern bool unknown_extra_sync_info_ab62e8;
// An ASCII string widened into a rotating static buffer (as in shared/ipx_manager.hpp).
wchar_t *__fastcall Narrow_To_Wide_006fa180(char const *text);

// The in-game message list (MessageListClass in Westwood's code; only the
// call used here).
class UnknownMessageList_0051d820 {
public:
    void *Add_Message(wchar_t const *name, int id, wchar_t const *text, int color, int style, int timeout,
                      bool single_player);
};
extern UnknownMessageList_0051d820 unknown_message_list_a3dc70;

void MultiplayerSyncCommandClass::Execute(int key) const
{
    unknown_extra_sync_info_ab62e8 = !unknown_extra_sync_info_ab62e8;
    if (unknown_extra_sync_info_ab62e8) {
        unknown_message_list_a3dc70.Add_Message(0, 0, Narrow_To_Wide_006fa180("Extra sync info ON"), 3, 0x4046, 150, false);
    } else {
        unknown_message_list_a3dc70.Add_Message(0, 0, Narrow_To_Wide_006fa180("Extra sync info OFF"), 3, 0x4046, 150, false);
    }
}


// PageUserCommandClass::PageUserCommandClass() -- 0x0051D870

PageUserCommandClass::PageUserCommandClass()
{
}


// char const *PageUserCommandClass::Get_Name() const -- 0x0051D880
// The command's key name.

char const *PageUserCommandClass::Get_Name() const
{
    return "PageUser";
}


// wchar_t const *PageUserCommandClass::Get_UI_Name() const -- 0x0051D890
// The localized name (TXT_PAGEUSER).

wchar_t const *PageUserCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_PAGEUSER", 0, "D:\\Ra2\\Init.CPP", 7368);
}


// wchar_t const *PageUserCommandClass::Get_Category() const -- 0x0051D8B0
// The localized category (TXT_INTERFACE).

wchar_t const *PageUserCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 7370);
}


// wchar_t const *PageUserCommandClass::Get_Description() const -- 0x0051D8D0
// The localized description (TXT_PAGEUSER_DESC).

wchar_t const *PageUserCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_PAGEUSER_DESC", 0, "D:\\Ra2\\Init.CPP", 7372);
}


// void PageUserCommandClass::Execute(int key) const -- 0x0051D8F0
// Pages a user (Queue_Event_22 0x00623600) in an internet game (mode 4).

void PageUserCommandClass::Execute(int key) const
{
    if (unknown_game_mode_a3d298 == 4) {
        Queue_Event_22();
    }
}


// CursorPositionCommandClass::CursorPositionCommandClass() -- 0x0051D910

CursorPositionCommandClass::CursorPositionCommandClass()
{
}


// char const *CursorPositionCommandClass::Get_Name() const -- 0x0051D920
// The command's key name.

char const *CursorPositionCommandClass::Get_Name() const
{
    return "CursorCheat";
}


// wchar_t const *CursorPositionCommandClass::Get_UI_Name() const -- 0x0051D930
// The localized name (GUI:CursorCheat).

wchar_t const *CursorPositionCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("GUI:CursorCheat", 0, "D:\\Ra2\\Init.CPP", 8040);
}


// wchar_t const *CursorPositionCommandClass::Get_Category() const -- 0x0051D950
// The localized category (GUI:Debug).

wchar_t const *CursorPositionCommandClass::Get_Category() const
{
    return StringTable::Load_String("GUI:Debug", 0, "D:\\Ra2\\Init.CPP", 8041);
}


// wchar_t const *CursorPositionCommandClass::Get_Description() const -- 0x0051D970
// The localized description (DESC:CursorCheat).

wchar_t const *CursorPositionCommandClass::Get_Description() const
{
    return StringTable::Load_String("DESC:CursorCheat", 0, "D:\\Ra2\\Init.CPP", 8042);
}


// void CursorPositionCommandClass::Execute(int key) const -- 0x0051D990
// Toggles the cursor position display (flag 0x00A416BC; name hypothesis).

extern bool unknown_show_cursor_position_a416bc;

void CursorPositionCommandClass::Execute(int key) const
{
    unknown_show_cursor_position_a416bc = !unknown_show_cursor_position_a416bc;
}


// DeleteCommandClass::DeleteCommandClass() -- 0x0051D9B0

DeleteCommandClass::DeleteCommandClass()
{
}


// char const *DeleteCommandClass::Get_Name() const -- 0x0051D9C0
// The command's key name.

char const *DeleteCommandClass::Get_Name() const
{
    return "Delete";
}


// wchar_t const *DeleteCommandClass::Get_UI_Name() const -- 0x0051D9D0
// The localized name (Cmnd:Delete).

wchar_t const *DeleteCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("Cmnd:Delete", 0, "D:\\Ra2\\Init.CPP", 6304);
}


// wchar_t const *DeleteCommandClass::Get_Category() const -- 0x0051D9F0
// The localized category (TXT_INTERFACE).

wchar_t const *DeleteCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_INTERFACE", 0, "D:\\Ra2\\Init.CPP", 6306);
}


// wchar_t const *DeleteCommandClass::Get_Description() const -- 0x0051DA10
// The localized description (Cmnd:DeleteDesc).

wchar_t const *DeleteCommandClass::Get_Description() const
{
    return StringTable::Load_String("Cmnd:DeleteDesc", 0, "D:\\Ra2\\Init.CPP", 6308);
}


// void DeleteCommandClass::Execute(int key) const -- 0x0051DA30
// Deletes the selection (0x006F6490).
void unknown_dispatch();

void DeleteCommandClass::Execute(int key) const
{
    unknown_dispatch();
}


// MultiplayerDebugCommandClass scalar deleting destructor (0x0051DA40, ??_G; vtable 0x7A43C4 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// MultiplayerSyncCommandClass scalar deleting destructor (0x0051DA60, ??_G; vtable 0x7A439C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// FollowCommandClass scalar deleting destructor (0x0051DA80, ??_G; vtable 0x7A4374 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// View1CommandClass scalar deleting destructor (0x0051DAA0, ??_G; vtable 0x7A434C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// View2CommandClass scalar deleting destructor (0x0051DAC0, ??_G; vtable 0x7A4324 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// View3CommandClass scalar deleting destructor (0x0051DAE0, ??_G; vtable 0x7A42FC slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// View4CommandClass scalar deleting destructor (0x0051DB00, ??_G; vtable 0x7A42D4 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// SetView1CommandClass scalar deleting destructor (0x0051DB20, ??_G; vtable 0x7A42AC slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// SetView2CommandClass scalar deleting destructor (0x0051DB40, ??_G; vtable 0x7A4284 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// SetView3CommandClass scalar deleting destructor (0x0051DB60, ??_G; vtable 0x7A425C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// SetView4CommandClass scalar deleting destructor (0x0051DB80, ??_G; vtable 0x7A4234 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// OptionsCommandClass scalar deleting destructor (0x0051DBA0, ??_G; vtable 0x7A420C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// SidebarUpCommandClass scalar deleting destructor (0x0051DBC0, ??_G; vtable 0x7A41E4 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// LSidebarUpCommandClass scalar deleting destructor (0x0051DBE0, ??_G; vtable 0x7A41BC slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// RSidebarUpCommandClass scalar deleting destructor (0x0051DC00, ??_G; vtable 0x7A4194 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// SidebarDownCommandClass scalar deleting destructor (0x0051DC20, ??_G; vtable 0x7A416C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// LSidebarDownCommandClass scalar deleting destructor (0x0051DC40, ??_G; vtable 0x7A4144 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// RSidebarDownCommandClass scalar deleting destructor (0x0051DC60, ??_G; vtable 0x7A411C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// SidebarPageUpCommandClass scalar deleting destructor (0x0051DC80, ??_G; vtable 0x7A40F4 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// LSidebarPageUpCommandClass scalar deleting destructor (0x0051DCA0, ??_G; vtable 0x7A40CC slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// RSidebarPageUpCommandClass scalar deleting destructor (0x0051DCC0, ??_G; vtable 0x7A40A4 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// SidebarPageDownCommandClass scalar deleting destructor (0x0051DCE0, ??_G; vtable 0x7A407C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// LSidebarPageDownCommandClass scalar deleting destructor (0x0051DD00, ??_G; vtable 0x7A4054 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// RSidebarPageDownCommandClass scalar deleting destructor (0x0051DD20, ??_G; vtable 0x7A402C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// CenterREventCommandClass scalar deleting destructor (0x0051DD40, ??_G; vtable 0x7A4004 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// BeaconPlacementCommandClass scalar deleting destructor (0x0051DD60, ??_G; vtable 0x7A3FDC slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// ToggleSellCommandClass scalar deleting destructor (0x0051DD80, ??_G; vtable 0x7A3FB4 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// ToggleRepairCommandClass scalar deleting destructor (0x0051DDA0, ??_G; vtable 0x7A3F8C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// AllianceCommandClass scalar deleting destructor (0x0051DDC0, ??_G; vtable 0x7A3F64 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// CenterBaseCommandClass scalar deleting destructor (0x0051DDE0, ??_G; vtable 0x7A3F3C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// CenterViewCommandClass scalar deleting destructor (0x0051DE00, ??_G; vtable 0x7A3F14 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// ScatterCommandClass scalar deleting destructor (0x0051DE20, ??_G; vtable 0x7A3EEC slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// GuardCommandClass scalar deleting destructor (0x0051DE40, ??_G; vtable 0x7A3EC4 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// StopCommandClass scalar deleting destructor (0x0051DE60, ??_G; vtable 0x7A3E9C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// AllToCheerCommandClass scalar deleting destructor (0x0051DE80, ??_G; vtable 0x7A3E74 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// DeployCommandClass scalar deleting destructor (0x0051DEA0, ??_G; vtable 0x7A3E4C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// PrevObjectCommandClass scalar deleting destructor (0x0051DEC0, ??_G; vtable 0x7A3E24 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// NextObjectCommandClass scalar deleting destructor (0x0051DEE0, ??_G; vtable 0x7A3DFC slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// PlanningModeCommandClass scalar deleting destructor (0x0051DF00, ??_G; vtable 0x7A3DD4 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// CombatantSelectCommandClass scalar deleting destructor (0x0051DF20, ??_G; vtable 0x7A3DAC slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// TypeSelectCommandClass scalar deleting destructor (0x0051DF40, ??_G; vtable 0x7A3D84 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// HealthNavCommandClass scalar deleting destructor (0x0051DF60, ??_G; vtable 0x7A3D5C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// VeterancyNavCommandClass scalar deleting destructor (0x0051DF80, ??_G; vtable 0x7A3D34 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// SetStructureTabCommandClass scalar deleting destructor (0x0051DFA0, ??_G; vtable 0x7A3D0C slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// SetDefenseTabCommandClass scalar deleting destructor (0x0051DFC0, ??_G; vtable 0x7A3CE4 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// SetUnitTabCommandClass scalar deleting destructor (0x0051DFE0, ??_G; vtable 0x7A3CBC slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// SetInfantryTabCommandClass scalar deleting destructor (0x0051E000, ??_G; vtable 0x7A3C94 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// ScreenCaptureCommandClass scalar deleting destructor (0x0051E020, ??_G; vtable 0x7A3C44 slot 0): emitted with the vtable by
// Init_Commands (0x005173C0) in this unit; the destructor (implicit) expands inline.


// PageUserCommandClass scalar deleting destructor (0x0051E040, ??_G): emitted with
// the vtable by the constructor in this unit; the destructor (implicit) expands inline.


// CursorPositionCommandClass scalar deleting destructor (0x0051E060, ??_G): emitted with
// the vtable by the constructor in this unit; the destructor (implicit) expands inline.


// DeleteCommandClass scalar deleting destructor (0x0051E080, ??_G): emitted with
// the vtable by the constructor in this unit; the destructor (implicit) expands inline.


extern "C" void __cdecl release_51e0a0(void*);
class Cleanup_51e0a0 { unsigned char pad0[4]; unsigned int* items; int count; unsigned char pad12[1]; unsigned char owns; unsigned char pad14[2]; int capacity; public: void get(); };
void Cleanup_51e0a0::get() { unsigned int* pointer=items; capacity=0; if(pointer && owns) { release_51e0a0(pointer); items=0; } owns=0; count=0; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<CCINIClass *>::Fetch (0x0051E0F0; VectorClass<CCINIClass *> vtable 0x007a3c04 slot 6).
// VectorClass<CCINIClass *>::operator equal (0x0051E100; VectorClass<CCINIClass *> vtable 0x007a3c04 slot 1).
// VectorClass<CCINIClass *>::Resize (0x0051E140; VectorClass<CCINIClass *> vtable 0x007a3c04 slot 2).
// VectorClass<CCINIClass *>::Clear (0x0051E1F0; VectorClass<CCINIClass *> vtable 0x007a3c04 slot 3).
// VectorClass<CCINIClass *>::ID_object (0x0051E220; VectorClass<CCINIClass *> vtable 0x007a3c04 slot 4).
// VectorClass<CCINIClass *>::ID_pointer (0x0051E260; VectorClass<CCINIClass *> vtable 0x007a3c04 slot 5).
// DynamicVectorClass<CCINIClass *>::Resize (0x0051E280; DynamicVectorClass<CCINIClass *> vtable 0x007A3C24 slot 2, named by its RTTI).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<CommandClass const *>::Add (0x0051E360): emitted by this unit's use of its list.


// IndexClass<int, CommandClass *>::Add_Index (0x0051E3B0): the hotkey table's template member,
// emitted in this unit for unknown_init_commands_5173c0 (0x005173C0), its only caller.
// Layout and behaviour: shared/ini_class.hpp.

template bool IndexClass<int, CommandClass *>::Add_Index(int const &id, CommandClass *const &data);


// IndexClass<int, CommandClass *>::Remove_Index (0x0051E480): the hotkey table's template member,
// emitted in this unit for unknown_init_commands_5173c0 (0x005173C0), its only caller.
// Layout and behaviour: shared/ini_class.hpp.

template bool IndexClass<int, CommandClass *>::Remove_Index(int const &id);


// IndexClass<int, CommandClass *>::Is_Present (0x0051E560): the hotkey table's template member,
// emitted in this unit for unknown_init_commands_5173c0 (0x005173C0), its only caller.
// Layout and behaviour: shared/ini_class.hpp.

template bool IndexClass<int, CommandClass *>::Is_Present(int const &id) const;


extern "C" unsigned char vtable_51e670[]; extern "C" void __cdecl release_buffer_51e670(void*); extern "C" void __cdecl release_object_51e670(void*);
class Leaf_51e670 { public: void* get(unsigned int flags); };
void* Leaf_51e670::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_51e670; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_51e670((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_51e670(this); return this; }


extern "C" unsigned char vtable_51e6c0[]; extern "C" void __cdecl release_buffer_51e6c0(void*); extern "C" void __cdecl release_object_51e6c0(void*);
class Leaf_51e6c0 { public: void* get(unsigned int flags); };
void* Leaf_51e6c0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_51e6c0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_51e6c0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_51e6c0(this); return this; }


extern "C" unsigned char table_51e710[]; extern "C" void* __cdecl storage_51e710(unsigned int);
class Storage_51e710 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_51e710::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_51e710; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_51e710(requested << 2); owns=1; } return this; }


// IndexClass<int, CommandClass *>::Increase_Table_Size (0x0051E760): the hotkey table's
// template member, emitted in this unit for Load_Keyboard_Hotkeys (0x00519240).
// Layout and behaviour: shared/ini_class.hpp.

template bool IndexClass<int, CommandClass *>::Increase_Table_Size(int amount);
