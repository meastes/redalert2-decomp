
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT, PARTIAL (lane Y3), appended to the load-progress unit:
// LoadProgressManager::Draw, YR 0x00552D60 (5,010 bytes, unmatched in ra2-decomp, no RA2 twin).
// Behaviour-written from gamemd.exe's code for the skirmish/multiplayer screen: a fresh
// render-size surface (ProgressSurface) filled with the black colour, then the country's loading
// picture "ls<640|800><country>.shp" (the observer's in IPX/Internet games when the first player
// has no house) through FUN_0072B500's drawer at LoadScreenSHPRect. Lane Y11 added the map preview
// and the texts (country, unit, briefing, Loading). NOT YET DRAWN (open in native/yr/README.md): the
// campaign screen (Session.Type GAME_NORMAL: the scenario's pictures and briefing) and the
// coop-campaign picture. Behaviour code for the native build; never part of an exact build.
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

void LoadProgressManager::Draw(void)
{
	bool small = Drawing::RenderWidth == DAT_007F5BE0;
	BSurface *surface = new BSurface(Drawing::RenderWidth, Drawing::RenderHeight, 2);
	ProgressSurface = (DSurface *)surface;
	Rect whole(0, 0, Drawing::RenderWidth, Drawing::RenderHeight);
	surface->Fill_Rect(whole, DSurface::Build_Hicolor_Pixel(BlackColor));
	if (Session.Type == GAME_NORMAL) {
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
