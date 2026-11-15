
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT, PARTIAL (lane Y3), appended to the load-progress unit:
// LoadProgressManager::Draw, YR 0x00552D60 (5,010 bytes, unmatched in ra2-decomp, no RA2 twin).
// Behaviour-written from gamemd.exe's code for the skirmish/multiplayer screen: a fresh
// render-size surface (ProgressSurface) filled with the black colour, then the country's loading
// picture "ls<640|800><country>.shp" (the observer's in IPX/Internet games when the first player
// has no house) through FUN_0072B500's drawer at LoadScreenSHPRect. NOT YET DRAWN (open in
// native/yr/README.md): the campaign screen (Session.Type GAME_NORMAL: the scenario's pictures and
// briefing), the coop-campaign picture, the map preview, the player names, colours and sides, the
// title text. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
ConvertClass * __fastcall FUN_0072B500(void);
void *YR_LoadWholeFileEx_ld(char const *name, bool &allocated) __asm__("__ZN10FileSystem15LoadWholeFileExEPKcRb");
#include "bsurface.h"
#include "draw.h"

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
}
