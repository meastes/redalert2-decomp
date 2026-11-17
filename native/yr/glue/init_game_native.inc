
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y2), appended to the init unit: Init_Game, YR 0x0052BA60
// (4,333 bytes, unmatched in ra2-decomp). Written from the RA2 tree's Init_Game
// (src/recovered/init.cpp, 0x005116A0: TibSun's with Bootstrap, Init_Mouse and Init_CDROM_Access
// inline) and gamemd.exe's code read instruction by instruction: the bootstrap mixfiles, the
// palettes and their drawers (UNITSNO, TEMPERAT, WAYPOINT, voxels.vpl, ANIM, PALETTE, CAMEO,
// MOUSEPAL, GRFXTXT), the expansion files, the mouse shapes, the CD search, the secondary
// mixfiles, the EA_WWLOGO movie, the options, the tactical map, SOUNDMD/EVAMD/THEMEMD.INI, the
// bulk data and rules, the art of animations and buildings, the multiplayer score records.
// The CD search when no search path is set (the native host always passes -CD.) is not drafted:
// it requires no disk, as the search-path case does. Debug prints are kept where they mark
// stages. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
#include "scenario.h"
#include "rules.h"
#include "bitfont.h"
#include "convert.h"
#include "tactical.h"
#include "theme.h"
#include "options.h"
#include "session.h"
#include "cdfile.h"
#include "ccfile.h"
#include "matrix3d.h"
#include "animtype.h"
#include "builtype.h"
#include "debugpage.h"

struct VirtualObject;
extern VirtualObject *DAT_00887640;               // 0x00887640 the mouse (WinMain)
void yr_wwmouse_hide(VirtualObject *mouse);       // native/yr/glue/wwmous_native.inc
void yr_wwmouse_set_cursor(VirtualObject *mouse, void const *cursor);
void __cdecl DebugStringNoPrefix(char const *format, ...);
DebugPage *__fastcall FUN_005BC450(int index);
bool __fastcall Init_Bootstrap_Mixfiles(void);
void __fastcall FUN_0060D430(void);
void __fastcall FUN_0072AA40(void);
int __fastcall FUN_00406B10(int value);
bool __fastcall Init_Expansion_Files(void);
bool __fastcall Init_Secondary_Mixfiles(void);
void __fastcall FUN_005312A0(void);
void __fastcall FUN_0052CB90(void);
void __fastcall FUN_007510D0(INIClass &ini);
void __fastcall FUN_007531A0(void);
void __fastcall FUN_00753000(INIClass &ini);
bool __fastcall Init_Bulk_Data(void);
bool __fastcall FUN_0052CD70(void);
bool __fastcall FUN_0052CB50(void);
void __fastcall Init_Commands(void);
void __fastcall FUN_00754CB0(void);
void Initialize_Options(void);
void Set_Option(int option);
void Free_Vocs(void);
void Init_Game_Modes(void);
void Set_Voxel_Light_Angle(float angle);
TimeStamp AudioGetTime(void);
// FileSystem's other members (this unit's struct FileSystem names only two; one symbol each).
extern ConvertClass *YR_ANIM_PAL __asm__("__ZN10FileSystem8ANIM_PALE");
extern ConvertClass *YR_CAMEO_PAL __asm__("__ZN10FileSystem9CAMEO_PALE");
extern ConvertClass *YR_GRFXTXT_Convert __asm__("__ZN10FileSystem15GRFXTXT_ConvertE");
extern SHPStruct *YR_GRFXTXT_SHP __asm__("__ZN10FileSystem11GRFXTXT_SHPE");
extern void *YR_GRFXTXT_PAL __asm__("__ZN10FileSystem11GRFXTXT_PALE");
void *YR_LoadWholeFileEx(char const *name, bool &allocated) __asm__("__ZN10FileSystem15LoadWholeFileExEPKcRb");
extern bool YR_ChatMask[8] __asm__("__ZN4Game8ChatMaskE");   // 0x00A8D108 Game::ChatMask
extern PaletteClass DAT_00886380;                 // 0x00886380 UNITSNO.PAL
extern PaletteClass DAT_00885A80;                 // 0x00885A80
extern VirtualObject *DAT_0087F6B4;               // 0x0087F6B4 the unit drawer (UNITSNO.PAL)
extern VirtualObject *DAT_0087F6B8;               // 0x0087F6B8 the game palette's drawer
extern VirtualObject *DAT_0087F6C4;               // 0x0087F6C4 PALETTE.PAL's drawer
extern VirtualObject *DAT_0087F6C8;               // 0x0087F6C8 MOUSEPAL.PAL's drawer
extern SHPStruct *DAT_00B0BCF0;                   // 0x00B0BCF0 MOUSE.SHA
extern bool DAT_00B0BCF4;                         // 0x00B0BCF4 MOUSE.SHA was allocated
extern unsigned char DAT_008175B0;
extern unsigned char DAT_0089E3A0;
extern char DAT_00A8ED80;
extern unsigned __int64 DAT_00A8F788;             // 0x00A8F788 when the loading screen may go
extern int DAT_00840A70;
extern float DAT_0088741C;                        // 0x0088741C the voxel camera angle
extern const float DAT_007E1E6C;                  // its default
extern const float DAT_007E1E68;                  // the voxel light angle's default
extern int DAT_00B43180;
extern bool UseVoxelCache;
extern Matrix3D VoxelCameraMatrix;
namespace VoxelDrawSystem { int Load_VPL_File(FileClass &file); extern int EnableLighting; }
extern unsigned char DAT_00B2FB78[0x300];                // 0x00B2FB78 voxels.vpl's colours
extern PaletteClass VoxelPalette;
extern PaletteClass WaypointPalette;
extern PaletteClass CCPalette;
extern PaletteClass WhitePalette;

// A 6-bit palette file read into a palette: each channel scaled to 8 bits (<< 2).
static void YR_Load_Palette(PaletteClass &palette, char const *name)
{
	memmove(&palette, FileSystem::LoadFile(name, false), 0x300);
	for (int index = 0; index < 256; index++) {
		palette[index] = RGBClass((unsigned char)(palette[index].Get_Red() << 2),
			(unsigned char)(palette[index].Get_Green() << 2), (unsigned char)(palette[index].Get_Blue() << 2));
	}
}

static ConvertClass *YR_Drawer(PaletteClass const &artpal, int shades)
{
	return new ConvertClass(artpal, GamePalette, *(Surface *)VisibleSurface, shades, false);
}

int Init_Game(int argc, char *argv[])
{
	FUN_004068E0("Init_Game - Creating the scenario and the rules\n");
	Scen = new ScenarioClass;
	if (Scen == NULL) {
		FUN_004068E0("Init_Game - Failed to create the scenario\n");
		return -1;
	}
	Rule = new RulesClass;
	if (Rule == NULL) {
		FUN_004068E0("Init_Game - Failed to create the rules\n");
		return -1;
	}

	FUN_004068E0("Bootstrap.....");
	if (!DAT_008175B0) {
		Set_Palette(BlackPalette, 0, NULL);
	}
	if (DAT_0089E3A0 == 1) {
		CD::Set_Required_Disk((DiskID)-2);
	}
	do {
		Keyboard->Check();
	} while (!DAT_00A8ED80);
	DAT_008A00A0 = false;
	FUN_005BC450(4)->Reset();
	if (!Init_Bootstrap_Mixfiles()) {
		FUN_004068E0("Init_Game - Failed to load the bootstrap mixfiles\n");
		DebugStringNoPrefix("Failed\n");
		return -1;
	}
	BitText::Instance = new BitText;
	FUN_0060D430();
	FUN_0072AA40();
	FUN_00406B10(DAT_00840A70);
	Keyboard->Clear();

	YR_Load_Palette(DAT_00886380, "UNITSNO.PAL");
	YR_Load_Palette(GamePalette, "TEMPERAT.PAL");
	DAT_00885A80 = GamePalette;
	CCPalette = GamePalette;
	WhitePalette[0] = BlackPalette[0];
	YR_Load_Palette(WaypointPalette, "WAYPOINT.PAL");

	CCFileClass vplfile("voxels.vpl");
	VoxelDrawSystem::Load_VPL_File(vplfile);
	memmove(&VoxelPalette, DAT_00B2FB78, 0x300);       // voxels.vpl's colours, copied entry by entry
	DAT_0088741C = DAT_007E1E6C;
	UseVoxelCache = true;
	VoxelCameraMatrix.Make_Identity();
	Set_Voxel_Light_Angle(DAT_007E1E68);
	VoxelDrawSystem::EnableLighting = 1;
	DAT_00B43180 = 0;
	FUN_00754CB0();

	DAT_0087F6B8 = (VirtualObject *)YR_Drawer(GamePalette, 0x35);
	PaletteClass palette;
	YR_Load_Palette(palette, "ANIM.PAL");
	YR_ANIM_PAL = YR_Drawer(palette, 0x35);
	YR_Load_Palette(palette, "PALETTE.PAL");
	DAT_0087F6C4 = (VirtualObject *)YR_Drawer(palette, 0x35);
	YR_Load_Palette(palette, "UNITSNO.PAL");
	DAT_0087F6B4 = (VirtualObject *)YR_Drawer(palette, 0x35);
	YR_Load_Palette(palette, "CAMEO.PAL");
	YR_CAMEO_PAL = YR_Drawer(palette, 0x35);
	YR_Load_Palette(palette, "MOUSEPAL.PAL");
	DAT_0087F6C8 = (VirtualObject *)YR_Drawer(palette, 1);
	TiberiumDrawer = (ConvertClass *)DAT_0087F6B4;

	CCFileClass grfxpal("GRFXTXT.PAL");
	if (grfxpal.Is_Available(0)) {
		YR_GRFXTXT_PAL = Load_Alloc_Data(grfxpal);
		grfxpal.Close();
	}
	if (YR_GRFXTXT_PAL != NULL) {
		YR_GRFXTXT_Convert = YR_Drawer(*(PaletteClass *)YR_GRFXTXT_PAL, 0x35);
	}
	CCFileClass grfxshp("GRFXTXT.SHP");
	if (grfxshp.Is_Available(0)) {
		YR_GRFXTXT_SHP = (SHPStruct *)Load_Alloc_Data(grfxshp);
		grfxshp.Close();
	}
	Init_Expansion_Files();
	DebugStringNoPrefix("OK\n");

	FUN_004068E0("Init_Game - Mouse\n");
	DAT_00B0BCF0 = (SHPStruct *)YR_LoadWholeFileEx("MOUSE.SHA", DAT_00B0BCF4);
	if (DAT_00B0BCF0 != NULL) {
		yr_wwmouse_hide(DAT_00887640);
		yr_wwmouse_set_cursor(DAT_00887640, DAT_00B0BCF0);
	}
	Map.Set_Default_Mouse(MOUSE_NORMAL, false);
	Game::CallBack();

	FUN_004068E0("Init_Game - CD-ROM access\n");
	if (!CDFileClass::Is_There_Search_Drives()) {
		FUN_004068E0("Init_Game - no search path: the native build requires no disk\n");
	}
	CD::Set_Required_Disk((DiskID)-2);
	FUN_004068E0("Init Secondary Mixfiles.....");
	if (!Init_Secondary_Mixfiles()) {
		DebugStringNoPrefix("Failed\n");
		return -1;
	}
	DebugStringNoPrefix("OK\n");

	Game::PlayMovie("EA_WWLOGO", -1, 1, 1, 1, 0);
	FUN_005312A0();
	FUN_0052CB90();
	FUN_004068E0("Init_Game - Options\n");
	Options.Load_Settings();
	Initialize_Options();
	Set_Option(0);
	GadgetClass::CurrentColorScheme = "Grey";

	if (TacticalMap != NULL) {
		delete TacticalMap;
	}
	TacticalMap = new Tactical;
	if (TacticalMap == NULL) {
		FUN_004068E0("Init_Game - Failed to create the tactical map\n");
		return -1;
	}
	LogicalSurface = HiddenSurface;

	FUN_004068E0("Init_Game - SOUNDMD.INI\n");
	CCFileClass soundfile("SOUNDMD.INI");
	if (!soundfile.Is_Available(0)) {
		FUN_004068E0("Init_Game - SOUNDMD.INI is missing\n");
		return -1;
	}
	CCINIClass soundini;
	if (!soundini.ReadCCFile(soundfile, false, false)) {
		FUN_004068E0("Init_Game - SOUNDMD.INI did not load\n");
		return -1;
	}
	Free_Vocs();
	FUN_007510D0(soundini);

	FUN_004068E0("Init_Game - EVAMD.INI\n");
	CCFileClass evafile("EVAMD.INI");
	if (!evafile.Is_Available(0)) {
		FUN_004068E0("Init_Game - EVAMD.INI is missing\n");
		return -1;
	}
	CCINIClass evaini;
	if (!evaini.ReadCCFile(evafile, false, false)) {
		FUN_004068E0("Init_Game - EVAMD.INI did not load\n");
		return -1;
	}
	FUN_007531A0();
	FUN_00753000(evaini);

	FUN_004068E0("Init_Game - THEMEMD.INI\n");
	CCFileClass themefile("THEMEMD.INI");
	CCINIClass themeini;
	if (!themefile.Is_Available(0)) {
		FUN_004068E0("Init_Game - THEMEMD.INI is missing\n");
		return -1;
	}
	if (!themeini.ReadCCFile(themefile, false, false)) {
		FUN_004068E0("Init_Game - THEMEMD.INI did not load\n");
		return -1;
	}
	Theme.Free_Themes();
	Theme.Init_Themes(themeini);
	Theme.Scan();

	FUN_004068E0("Init_Game - Bulk data\n");
	if (!Init_Bulk_Data()) {
		return -1;
	}
	FUN_004068E0("Init_Game - Rules\n");
	if (!FUN_0052CD70()) {
		FUN_004068E0("Init_Game - Rules failed\n");
		return -1;
	}
	Rule->Do_AnimTypes(*RuleINI);
	for (int anim = 0; anim < AnimTypes.Count(); anim++) {
		AnimTypes[anim]->Read_INI(ArtINI);
	}
	Rule->Do_BuildingTypes(*RuleINI);
	for (int building = 0; building < BuildingTypes.Count(); building++) {
		BuildingTypes[building]->Read_INI(*RuleINI);
	}

	while (AudioGetTime() < DAT_00A8F788) {
	}
	FUN_0052CB50();

	GadgetClass::CurrentColorScheme = "Grey";
	Session.MaxPlayers = Rule->MaxPlayers;
	Session.GamesPlayed = 0;
	Session.NumScores = 0;
	Session.CurGame = 0;
	for (int index = 0; index < MAX_MULTI_NAMES; index++) {
		Session.Score[index].Name[0] = 0;
		Session.Score[index].Wins = 0;
		for (int game = 0; game < MAX_MULTI_GAMES; game++) {
			Session.Score[index].Lost[game] = -1;
			Session.Score[index].Kills[game] = -1;
			Session.Score[index].Built[game] = -1;
			Session.Score[index].Score[game] = -1;
		}
	}
	memset(YR_ChatMask, true, 8);
	Game::InitRandom();
	FUN_004068E0("Init Commands\n");
	Init_Commands();
	FUN_004068E0("Game Init Completed.\n");
	Init_Game_Modes();
	return 0;
}
