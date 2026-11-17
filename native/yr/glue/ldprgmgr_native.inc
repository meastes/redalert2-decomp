
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT, PARTIAL (lane Y3), appended to the load-progress unit:
// LoadProgressManager::Draw, YR 0x00552D60 (5,010 bytes, unmatched in ra2-decomp, no RA2 twin).
// Behaviour-written from gamemd.exe's code for the skirmish/multiplayer screen: a fresh
// render-size surface (ProgressSurface) filled with the black colour, then the country's loading
// picture "ls<640|800><country>.shp" (the observer's in IPX/Internet games when the first player
// has no house) through FUN_0072B500's drawer at LoadScreenSHPRect. Lane Y11 added the map preview
// and the texts (country, unit, briefing, Loading); lane Y12 the campaign screen (Draw_Campaign).
// NOT YET DRAWN: the coop-campaign picture. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
ConvertClass * __fastcall FUN_0072B500(void);
void *YR_LoadWholeFileEx_ld(char const *name, bool &allocated) __asm__("__ZN10FileSystem15LoadWholeFileExEPKcRb");
#include "bsurface.h"
#include "draw.h"
#include "preview.h"
#include "progress.h"
#include "scheme.h"
#include "multiplayer.h"
#include "stringtable.h"
#include "dialog.h"

char const * __fastcall FUN_0072B2F0(int width, int height, int side);   // txtlabel2.cpp: the title band's file
char const * __fastcall FUN_0072B310(int width, int height, int side);   // the load bar's file
ConvertClass * FUN_0072B340(void);                                       // their drawers
ConvertClass * FUN_0072B360(void);
ConvertClass * FUN_0072B380(void);
void __fastcall FUN_0072B390(char const * name);                         // the campaign picture's palette

// The campaign's loading screen (lane Y12, gamemd.exe 0x00552E65-0x005532FD, the GAME_NORMAL branch;
// it was not drawn): the side's title band, the scenario's picture (ScenarioClass LS640BkgdName or
// LS800BkgdName with the LS800BkgdPal palette) and the side's load bar, each at its rect (a missing
// band or bar leaves its rect black); the mission's name (LSLoadMessage) at the title band's corner in
// the side's colour scheme, and its briefing (LSBrief) on a dimmed box at the scenario's
// LS640BriefLoc / LS800BriefLoc from the picture's rect, 400 pixels wide. Nothing is drawn without a
// picture name; no briefing ends it there.
static void Draw_Campaign(LoadProgressManager & self, BSurface & surface, bool small)
{
	char const * picture = small ? Scen->LS640BkgdName : Scen->LS800BkgdName;
	if (picture[0] == '\0') {
		return;
	}
	FUN_0072B390(Scen->LS800BkgdPal);
	self.LoadMessage = NULL;
	if (Scen->LSLoadMessage[0] != '\0') {
		self.LoadMessage = (wchar_t *)StringTable::LoadString(Scen->LSLoadMessage, NULL, "D:\\ra2mdpost\\LdPrgMgr.cpp", 0x297);
	}
	self.LoadBriefing = NULL;
	if (Scen->LSBrief[0] != '\0') {
		self.LoadBriefing = (wchar_t *)StringTable::LoadString(Scen->LSBrief, NULL, "D:\\ra2mdpost\\LdPrgMgr.cpp", 0x29B);
	}
	int side = Progress.Get_Side();
	char const * title_name = FUN_0072B2F0(Drawing::RenderWidth, Drawing::RenderHeight, side);
	char const * bar_name = FUN_0072B310(Drawing::RenderWidth, Drawing::RenderHeight, side);
	self.TitleBarSHP = (SHPStruct *)YR_LoadWholeFileEx_ld(title_name, self.TitleBarSHP_loaded);
	self.LoadBarSHP = (SHPStruct *)YR_LoadWholeFileEx_ld(bar_name, self.LoadBarSHP_loaded);
	self.LoadScreenSHP = (SHPStruct *)YR_LoadWholeFileEx_ld(picture, self.LoadScreenSHP_loaded);
	ConvertClass * title_drawer = FUN_0072B340();
	ConvertClass * picture_drawer = FUN_0072B360();
	ConvertClass * bar_drawer = FUN_0072B380();
	Rect whole(0, 0, Drawing::RenderWidth, Drawing::RenderHeight);
	int black = DSurface::Build_Hicolor_Pixel(BlackColor);
	// gamemd.exe draws all three at the title band's X, each at its own rect's Y.
	Point2D point(self.TitleBarRect.X, self.TitleBarRect.Y);
	if (self.TitleBarSHP != NULL && title_drawer != NULL) {
		Draw_Shape(surface, *title_drawer, (ShapeSet const *)self.TitleBarSHP, 0, point, whole, (ShapeFlags_Type)0x400);
	} else {
		surface.Fill_Rect(self.TitleBarRect, black);
	}
	point.Y = self.LoadBarSHPRect.Y;
	if (self.LoadScreenSHP != NULL && picture_drawer != NULL) {
		Draw_Shape(surface, *picture_drawer, (ShapeSet const *)self.LoadScreenSHP, 0, point, whole, (ShapeFlags_Type)0x400);
	}
	point.Y = self.LoadScreenSHPRect.Y;
	if (self.LoadBarSHP != NULL && bar_drawer != NULL) {
		Draw_Shape(surface, *bar_drawer, (ShapeSet const *)self.LoadBarSHP, 0, point, whole, (ShapeFlags_Type)0x400);
	} else {
		surface.Fill_Rect(self.LoadScreenSHPRect, black);
	}
	ColorScheme * scheme = Progress.Side_Scheme();
	if (self.LoadMessage != NULL) {
		Point2D ret;
		Point2D at(self.TitleBarRect.X + 10, self.TitleBarRect.Y + 10);
		Fancy_Text_Print_Wide(&ret, self.LoadMessage, &surface, &whole, &at, scheme, 0, (TextPrintType)0x19);
	}
	if (self.LoadBriefing == NULL) {
		return;
	}
	Point2D at(self.LoadBarSHPRect.X + (small ? Scen->LS640BriefLocX : Scen->LS800BriefLocX),
		self.LoadBarSHPRect.Y + (small ? Scen->LS640BriefLocY : Scen->LS800BriefLocY));
	wchar_t brief[1024];
	wcsncpy(brief, self.LoadBriefing, 1023);
	brief[1023] = 0;
	int pixel = DSurface::Build_Hicolor_Pixel((RGBClass)scheme->HSV);
	int width = 0;
	int height = 0;
	BitFont::Instance->GetTextDimension(brief, &width, &height, 400);
	FUN_005541C0(brief, &surface, at, 400, 0, false);
	Rect box(at.X, at.Y, width, height);
	FUN_00554280(brief, &surface, &box, pixel, 0);
}

void LoadProgressManager::Draw(void)
{
	bool small = Drawing::RenderWidth == DAT_007F5BE0;
	BSurface *surface = new BSurface(Drawing::RenderWidth, Drawing::RenderHeight, 2);
	ProgressSurface = (DSurface *)surface;
	Rect whole(0, 0, Drawing::RenderWidth, Drawing::RenderHeight);
	surface->Fill_Rect(whole, DSurface::Build_Hicolor_Pixel(BlackColor));
	if (Session.Type == GAME_NORMAL) {
		Draw_Campaign(*this, *surface, small);
		return;
	}
	static char const * const pictures[10] = {
		"ls%sustates.shp", "ls%skorea.shp", "ls%sfrance.shp", "ls%sgermany.shp", "ls%sukingdom.shp",
		"ls%slibya.shp", "ls%siraq.shp", "ls%scuba.shp", "ls%srussia.shp", "ls%syuri.shp"
	};
	int country = Session.Players[0]->Get_Country();
	char name[64];
	name[0] = '\0';
	char const *resolution = small ? "640" : "800";
	if ((Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && *(int *)((char *)Session.Players[0] + 0x6b) == -1) {
		sprintf(name, "ls%sobs.shp", resolution);
	} else if (country >= 0 && country < 10) {
		sprintf(name, pictures[country], resolution);
	}
	ConvertClass *drawer = FUN_0072B500();
	LoadScreenSHP = (SHPStruct *)YR_LoadWholeFileEx_ld(name, LoadScreenSHP_loaded);
	if (drawer != NULL && LoadScreenSHP != NULL) {
		Point2D point = FUN_0072A9C0(&LoadScreenSHPRect);
		Draw_Shape(*surface, *drawer, (ShapeSet const *)LoadScreenSHP, 0, point, surface->Get_Rect(), SHAPE_WIN_REL);
	}
	// The map's preview (lane Y11, gamemd.exe 0x00553568-0x005536A0; user report 2026-11-17: the
	// loading screen's box was empty): a random map's preview picture, else the scenario file's
	// [Preview] pixels, drawn by MapPreviewClass::DrawMap into the screen's preview box.
	MapPreviewClass *preview = new MapPreviewClass;
	if (Scen->IsRandom) {
		preview->Read_PCX_Preview("RandMap.img");
	} else {
		preview->Read_INI_Preview(Session.ScenarioFileName);
	}
	preview->DrawMap(*surface);
	delete preview;

	// The texts (lane Y11, gamemd.exe 0x005536A0-0x005540E7; user report 2026-11-17: the loading screen
	// lacked its texts), as Red Alert 2's loading screen draws them (src/recovered/ldprgmgr.cpp,
	// UnknownLoadProgress::Unknown_00535BF0) with Yuri's Revenge's tenth country and places: the
	// country's name right-aligned on a dimmed strip, the country's unit in black on the title band,
	// the country's briefing on a dimmed box (placed lower in a cooperative game), then "Loading".
	// An observer gets only the name and "Loading". Colour: the loading colour scheme's.
	RGBClass rgb = (RGBClass)Progress.Loading_Scheme()->HSV;
	int pixel = DSurface::Build_Hicolor_Pixel(rgb);
	bool observer = (Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && *(int *)((char *)Session.Players[0] + 0x6b) == -1;
	static char const * const names[10] = {"Name:Americans", "Name:Alliance", "Name:French", "Name:Germans", "Name:British",
		"Name:Africans", "Name:Arabs", "Name:Confederation", "Name:Russians", "Name:YuriCountry"};
	static char const * const units[10] = {"Name:Para", "Name:BEAGLE", "Name:GTGCAN", "Name:TNKD", "Name:SNIPE",
		"Name:DTRUCK", "Name:DESO", "Name:TERROR", "Name:TTNK", "Name:YURI"};
	static char const * const briefs[10] = {"LoadBrief:USA", "LoadBrief:Korea", "LoadBrief:French", "LoadBrief:Germans",
		"LoadBrief:British", "LoadBrief:Lybia", "LoadBrief:Iraq", "LoadBrief:Cuba", "LoadBrief:Russia", "LoadBrief:YuriCountry"};
	wchar_t const *country_name = NULL;
	if (observer) {
		country_name = StringTable::LoadString("Name:Observer", NULL, "D:\\ra2mdpost\\LdPrgMgr.cpp", 0xB2);
	} else if (country >= 0 && country < 10) {
		country_name = StringTable::LoadString(names[country], NULL, "D:\\ra2mdpost\\LdPrgMgr.cpp", 0xB7 + 2 * country);
	}
	if (country_name != NULL) {
		Rect area = small ? Rect(0x181, 0x1B4, 200, 20) : Rect(0x21C, 0x136, 200, 20);
		FUN_00554100(&area);
		FUN_005541C0(country_name, surface, FUN_0072A9C0(&area), area.Width, 2, false);
		FUN_00554280(country_name, surface, &area, pixel, 2);
	}
	if (!observer && country >= 0 && country < 10) {
		wchar_t unit_name[128];
		wcscpy(unit_name, StringTable::LoadString(units[country], NULL, "D:\\ra2mdpost\\LdPrgMgr.cpp", 0xF7 + 2 * country));
		for (wchar_t *c = unit_name; *c != 0; ++c) {
			if ((*c >= 'a' && *c <= 'z') || (*c >= 0xE0 && *c <= 0xFE)) *c -= 0x20;
		}
		Rect area = small ? Rect(0x10, 0x48, 200, 20) : Rect(0x14, 0x5A, 200, 20);
		FUN_00554100(&area);
		FUN_00554280(unit_name, surface, &area, DSurface::Build_Hicolor_Pixel(BlackColor), 0);

		wchar_t const *brief = StringTable::LoadString(briefs[country], NULL, "D:\\ra2mdpost\\LdPrgMgr.cpp", 0x12A + 2 * country);
		bool coop = Session.MPGameMode != NULL && Session.MPGameMode->vt_entry_08();
		Rect box = small ? Rect(0x10, 0x7E, 0x13E, 0x68) : Rect(0x14, coop ? 0x17C : 0x9E, 0x18E, 0x82);
		FUN_00554100(&box);
		FUN_005541C0(brief, surface, FUN_0072A9C0(&box), box.Width, 0, true);
		FUN_00554280(brief, surface, &box, pixel, 0);
	}
	wchar_t const *loading = StringTable::LoadString("GUI:LoadingEx", NULL, "D:\\ra2mdpost\\LdPrgMgr.cpp", 0x365);
	Rect area = FUN_00554150();
	FUN_005541C0(loading, surface, FUN_0072A9C0(&area), area.Width, 0, false);
	FUN_00554280(loading, surface, &area, pixel, 0);
}
