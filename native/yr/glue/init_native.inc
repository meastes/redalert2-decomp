
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y2), appended to the init unit: Allocate_Surfaces, YR 0x00533FD0
// (1,137 bytes, unmatched in ra2-decomp). From the RA2 tree's Allocate_Surfaces
// (src/recovered/init.cpp, 0x00519500; TibSun's) with gamemd.exe's changes read from its code:
// YR's DSurface takes a second flag (its 3D-device surface: composite and tile only), and the
// composite and tile surfaces go to system memory unless [Video] VideoBackBuffer (0x00840A6C).
// Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
extern bool DAT_00840A6C;                         // 0x00840A6C [Video] VideoBackBuffer

// DSurface's video-memory flag (+0x19, protected; the original reads the field).
struct YRSurfaceMemory : DSurface {
	static bool Video_Ram(Surface *surface) { return ((YRSurfaceMemory *)surface)->IsVideoRam; }
};

static inline char const *YR_Memory_Text(Surface *surface)
{
	return YRSurfaceMemory::Video_Ram(surface) ? "VRAM" : "SYSTEM MEMORY";
}

bool Allocate_Surfaces(const Rect &hidden, const Rect &composite, const Rect &tile, const Rect &sidebar, bool hidden_first)
{
	FUN_004068E0("Allocating new surfaces\n");
	if (AlternateSurface != NULL) {
		FUN_004068E0("Deleting AlternateSurface\n");
		delete AlternateSurface;
		AlternateSurface = NULL;
	}
	if (HiddenSurface != NULL) {
		FUN_004068E0("Deleting HiddenSurface\n");
		delete HiddenSurface;
		HiddenSurface = NULL;
	}
	if (DSurface::Composite != NULL) {
		FUN_004068E0("Deleting CompositeSurface\n");
		delete DSurface::Composite;
		DSurface::Composite = NULL;
	}
	if (TileSurface != NULL) {
		FUN_004068E0("Deleting TileSurface\n");
		delete TileSurface;
		TileSurface = NULL;
	}
	if (SidebarSurface != NULL) {
		FUN_004068E0("Deleting SidebarSurface\n");
		delete SidebarSurface;
		SidebarSurface = NULL;
	}

	bool tile_sysmem = !DAT_00840A6C;
	bool sidebar_sysmem = !Options.AllowVRAMSidebar;

	if (hidden_first && hidden.Width > 0 && hidden.Height > 0) {
		HiddenSurface = new DSurface(hidden.Width, hidden.Height, false, false);
		HiddenSurface->Fill(0);
		FUN_004068E0("HiddenSurface (%dx%d) %s\n", hidden.Width, hidden.Height, YR_Memory_Text(HiddenSurface));
	}
	if (composite.Width > 0 && composite.Height > 0) {
		DSurface::Composite = new DSurface(composite.Width, composite.Height, tile_sysmem, true);
		DSurface::Composite->Fill(0);
		FUN_004068E0("CompositeSurface (%dx%d) %s\n", composite.Width, composite.Height, YR_Memory_Text(DSurface::Composite));
	}
	if (tile.Width > 0 && tile.Height > 0) {
		TileSurface = new DSurface(tile.Width, tile.Height, tile_sysmem, true);
		TileSurface->Fill(0);
		FUN_004068E0("TileSurface (%dx%d) %s\n", tile.Width, tile.Height, YR_Memory_Text(TileSurface));
	}
	if (DSurface::Composite != NULL && TileSurface != NULL
		&& (YRSurfaceMemory::Video_Ram(TileSurface) ^ YRSurfaceMemory::Video_Ram(DSurface::Composite))) {
		delete DSurface::Composite;
		delete TileSurface;
		FUN_004068E0("Moving TileSurface and CompositeSurface into system memory\n");
		DSurface::Composite = new DSurface(composite.Width, composite.Height, true, true);
		TileSurface = new DSurface(tile.Width, tile.Height, true, true);
	}
	if (sidebar.Width > 0 && sidebar.Height > 0) {
		SidebarSurface = new DSurface(sidebar.Width, sidebar.Height, sidebar_sysmem, false);
		SidebarSurface->Fill(0);
		FUN_004068E0("SidebarSurface (%dx%d) %s\n", sidebar.Width, sidebar.Height, YR_Memory_Text(SidebarSurface));
	}
	if (!hidden_first && hidden.Width > 0 && hidden.Height > 0) {
		HiddenSurface = new DSurface(hidden.Width, hidden.Height, false, false);
		HiddenSurface->Fill(0);
		FUN_004068E0("HiddenSurface (%dx%d) %s\n", hidden.Width, hidden.Height, YR_Memory_Text(HiddenSurface));
	}
	if (hidden.Width > 0 && hidden.Height > 0) {
		AlternateSurface = new DSurface(hidden.Width, hidden.Height, true, false);
		AlternateSurface->Fill(0);
		FUN_004068E0("AlternateSurface (%dx%d) %s\n", hidden.Width, hidden.Height, YR_Memory_Text(AlternateSurface));
	}
	return true;
}

// NON-EXACT NATIVE DRAFT: ConvertClass::CreateFromFile, YR 0x0072ADE0 (303 bytes; declared by the
// tree as FUN_0072ADE0), from gamemd.exe's code: a 6-bit palette file read whole into a new
// palette (each channel << 2) and a drawer for it on the alternate surface.
void __fastcall FUN_0072ADE0(char const *name, PaletteClass **palette, ConvertClass **convert)
{
	CCFileClass file(name);
	unsigned char *data = (unsigned char *)Load_Alloc_Data(file);
	if (data == NULL) {
		return;
	}
	*palette = new PaletteClass;
	for (int index = 0; index < 256; index++) {
		(**palette)[index] = RGBClass((unsigned char)(data[index * 3] << 2), (unsigned char)(data[index * 3 + 1] << 2),
			(unsigned char)(data[index * 3 + 2] << 2));
	}
	delete [] data;
	*convert = new ConvertClass(**palette, **palette, *AlternateSurface, 1, false);
}

// NON-EXACT NATIVE DRAFT: FUN_00534E50 (330 bytes, unnamed, unmatched), from gamemd.exe's code:
// the neutral mixfiles reloaded: NEUTRAL.MIX (0x00884E5C) and NTRLMD.MIX (0x00884E58) freed, then
// NTRLMD.MIX and NEUTRAL.MIX made again with the fast key. The original returns whether both
// loaded (the tree declares it void; its callers ignore the result).
extern MixFileClass *DAT_00884E58;
extern MixFileClass *DAT_00884E5C;

void FUN_00534E50(void)
{
	if (DAT_00884E5C != NULL) {
		FUN_004068E0("Deleting %s\n", DAT_00884E5C->Filename);
		delete DAT_00884E5C;
	}
	DAT_00884E5C = NULL;
	if (DAT_00884E58 != NULL) {
		FUN_004068E0("Deleting %s\n", DAT_00884E58->Filename);
		delete DAT_00884E58;
		DAT_00884E58 = NULL;
	}
	DAT_00884E58 = new MixFileClass("NTRLMD.MIX", &FastKey);
	if (DAT_00884E58 == NULL) {
		return;
	}
	DebugStringNoPrefix("LOADED NTRLMD.MIX");
	DAT_00884E5C = new MixFileClass("NEUTRAL.MIX", &FastKey);
	if (DAT_00884E5C == NULL) {
		return;
	}
	DebugStringNoPrefix("LOADED NEUTRAL.MIX");
}
