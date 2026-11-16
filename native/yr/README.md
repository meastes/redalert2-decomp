# yr-native: the native Yuri's Revenge game (lane Y1)

User decision 2026-11-15 (AGENTS.md); plan 1 of `docs/YR_PORT_SIZING.md`. The game is built from
the pinned ra2-decomp tree in `yr/` (never edited) with native/'s platform layer. RA2's `src/`,
its exact build and the `ra2-native` targets are unchanged; `yr-native` is built only with
`-DRA2_NATIVE_YR=ON` (Mac).

    cmake -DRA2_NATIVE_YR=ON -DRA2_NATIVE_YR_MINGW=<llvm-mingw>/generic-w64-mingw32/include <native build dir>
    cmake --build <native build dir> --target yr-native

## Layout

| Path | What |
| --- | --- |
| `yr/ra2-decomp`, `yr/tibsun` | pinned copies (commits, licences: `yr/README.md`, `THIRD_PARTY.md`) |
| `native/yr/tools/yr_units.py` | one native unit per `src/**/*.cpp` and the merged header directory (TibSun headers, ra2-decomp's `include/` over them, as their build), with `fixups.json` and the RA2 tree's port rewrites (`native_units.port`: long, literal suffixes, ra2_ CRT names, window data, ...) |
| `native/yr/tools/yr_sdk.py` | the Win32/DirectX headers: the local llvm-mingw toolchain's mingw-w64 headers rewritten for the VC6 widths (long -> 32 bits) into the build directory, with the few clashes with the host's C library resolved (BSD socket types renamed, real `__uuidof`, `DLGPROC` returning BOOL, Win32 window-data names kept); never copied into the repository |
| `native/yr/sdk` | the YR prelude (host C types first), the VC6 CRT declarations as the port supplies them, and `comdef.h`/`io.h`/`mbstring.h` wrappers |
| `native/yr/fixups.json` | text fixups where clang refuses VC6 dialect, cut functions (assembly) and left-out units (Westwood Online), each with its reason |
| `native/yr/glue` | native code for what is cut or not matched: the RA2 native tree's C for the TibSun assembly (LCW_Comp, the time-stamp counter), the VC6 CRT names, the host's RA2 hooks, and YR functions written from RA2 twins (`yr_twins.cpp`) |
| `native/yr/yr.cmake` | the `yr_units_objs`, `yr_host_objs` and `yr-native` targets and the link closure |

## Stage 1 result (2026-11-15)

- All 435 YR units compile for arm64 at -O2 (3 Westwood Online units left out: Internet play).
  33 text fixups, 4 cut functions with native glue.
- `yr-native` links (11 MB). Link closure: **2,534** entries (functions 811: 611 C++, 197 C,
  3 Win32 [VariantInit/VariantClear/VariantChangeType]; data 1,723: 1,294 address-named
  `DAT_`/`PTR_` globals, 284 C++ and 139 C globals, 6 vtables). All but 10 are referenced
  by the YR units (9 by the shared host, 1 by the port: RA2 hooks, supplied by
  `glue/yr_host_glue.cpp`). Of the 811 functions, 269 carry ra2-decomp's address names
  (`FUN_xxxxxxxx`, their unmatched and library code); WinMain itself (0x006BB9A0, 10,261
  bytes) is unmatched there and has an RA2 twin (RA2 0x0068E280).
- Run: the process starts and faults in the static initialisers, before main: MapClass's
  constructor chain (TabClass) compares a sidebar name from a `DAT_` global that is still
  zero. The YR globals have no values yet: the original keeps them in gamemd.exe's data.

## The Win32 headers (ruling 2026-11-15)

The Mac YR build needs the mingw-w64 Win32/DirectX headers of the local llvm-mingw toolchain
(`-DRA2_NATIVE_YR_MINGW`, `private/toolchains/llvm-mingw-*`), the same toolchain the Windows
native build uses. `tools/yr_sdk.py` rewrites them into the build directory at configure time;
nothing from them is copied into the repository.

## Stage 2 (lane Y2, 2026-11-15): from the data load into Init_Game

* gamemd.exe 1.001 is recognised by its code sections (the user's copy with its 4 KB overlay;
  the 2-byte no-CD patch at 0x0052F949 is undone for the .text check); `data/globals.tsv`
  (from ra2-decomp's pins, `tools/yr_pins.py`) and the address names give each global its
  address. `tools/yr_data.py` makes storage for every global no unit defines and the table the
  port fills from gamemd.exe (546 globals) before the YR static initialisers, which
  `tools/defer_initializers.py` holds back until then. Class-typed globals no unit defines
  (the INI objects, the random generator, BlubCell) are constructed in `glue/yr_globals.cpp`.
* Address names bound to the definitions at those addresses (`tools/yr_aliases.py`), and the
  few functions the tree defines under one name and calls under another
  (`data/name_aliases.tsv`, `glue/uicmnds2_native.inc`, `glue/ccini_native.inc`).
* Window procedures take full-width message parameters (ruling: WPARAM/LPARAM as the RA2
  tree's host passes them). teleport.cpp's bridge test is `true`: gamemd.exe's 0x00718260 makes
  no call to 0x00483610.
* Native drafts of unmatched functions, appended to their units (`fixups.json` "append"; each
  file is labelled NON-EXACT): WinMain (`glue/startup_winmain.inc`), Allocate_Surfaces,
  ConvertClass::CreateFromFile and FUN_00534E50 (`glue/init_native.inc`), Init_Game
  (`glue/init_game_native.inc`); the import thunks the tree calls by address
  (`glue/yr_import_thunks.cpp`); the CPU probes (`glue/yr_asm.cpp`).
* Fixed on the way: Darwin defines BIG_ENDIAN, so base64.cpp's packet union took the
  big-endian layout and the encryption key decoded to nothing (LANGUAGE.MIX then failed
  intermittently); GUID globals (CLSIDs, IIDs) are now loaded.

Run: `yr-native --data-dir <RA2+YR install> --run-to=menu` creates the "Yuri's Revenge" window
(800x600, 16 bpp), its surfaces and mouse, loads the bootstrap and language mixfiles and the
palettes, and stops in Init_Game at FUN_00754CB0 (3,290 bytes, TS Init_Voxel_Matrices; RA2
twin `src/recovered/voxel.cpp` Unknown_00718060).

## Stage 2, lane Y3 (2026-11-15): to the main menu

* The pin moved to ra2-decomp `8719494` (`yr/README.md`, `THIRD_PARTY.md`): 17 of our stops had been
  matched upstream since `f8d8d53` (BuildingTypeClass::Read_INI, Main_Loop, Message_Input,
  VoxelLibrary::Read_File, ...).
* `tools/yr_aliases.py` also binds calls made by a matched function's `functions.csv` name
  (Game::AudioGetTime, INIClass::GetSection, ...; 22 bindings, listed in the generated
  `aliases.tsv`); `data/name_aliases.tsv` gained the ones read from gamemd.exe's calls
  (Toggle_Cameo_Text, ObjectTypeClass::One_Time, IonStormClass::Is_Ion_Storm_Active,
  Vox_From_Name, Fill_Surface_Rect, OwnerDraw::Dialog_Message_Handler, FUN_0053D330).
* Native drafts (NON-EXACT, `fixups.json` "append"): FUN_00754CB0 (voxel matrices, RA2 twin),
  SidebarClass::Reposition_Sidebar, IonBlastClass::One_Time, the TechnoTypeClass constructor
  (gamemd.exe's stores mapped to field offsets), Read_PCX_File (TibSun text), the Adjust_Color
  tables (`yr_asm.cpp`), and the owner-draw layer the menu needs (`glue/ownrdraw_native.inc`):
  UI::StandardWndProc, OwnerDraw::DefaultHandler (TibSun CtrlProc via the RA2 draft),
  OwnerDrawButtonHandler, StaticHandler, ResetControlDrawModeAndTimerProc, FUN_0060C0C0,
  FUN_0060B950, DrawCampaignMenuTransition (the RA2 tree's matched twin). Bindings:
  Game::CallBack, Game::PlayMovie / FUN_005BF260, the game-mode initialisers' Allocate.
* Pointer-width fixes: the audio memory pool and stream casts, ToolTip::Text (intptr_t), and the
  owner-draw record (OwnerDraw::WinData): the unit reaches fields both by name and at raw 32-bit
  offsets; natively the raw ones go through the union's pointer-free Dialog view, the record
  constructor's font by name, and the statics' pointer fields (+0x58, +0x60, +0x74, +0x78, +0xA4)
  sit in a side table by window.
* Research helpers: `tools/yr_dis.py` (gamemd.exe disassembly with the tree's names),
  `tools/yr_layout.py` (32-bit field offsets of a YR class), `tools/yr_ctor_stores.py`
  (a constructor's stores by field), `tools/yr_run.sh` (a run with window dumps).

Run: the "Yuri's Revenge" main menu shows (Single Player, Internet, Network, Movies & Credits,
Options, Exit Game, Version 1.001) with its TV movie playing, and Main_Menu's loop runs. Link closure: 343 entries.

## Stage 2, lane Y3 (2026-11-15): first playable, a skirmish on the Mac

Main menu -> Single Player -> Skirmish setup -> Start Game -> the skirmish loads and runs: the
tactical view (tiles, shroud, overlays, terrain, buildings, infantry, voxel vehicles), the
sidebar, the radar frame and the credits; the units move, take selection, the H (base) and D
(deploy) keys; Escape opens Game Options, Abort Mission and its question return to the skirmish
setup. `ctest -R yr-native-skirmish-smoke` (yr/tests/yr_skirmish_smoke.py; a -DRA2_NATIVE_YR=ON
build, RA2_NATIVE_YR_DATA_DIR set to the RA2 + YR folder) runs that route with scripted input.

Fixes of note on the way (all NATIVE, native/yr only): yr_data.py loads enumerations, the theater
table and VoxelRenderOrientations from gamemd.exe and sizes arrays bounded by enumerators (the
RadiusTable overran MainWindow); tile sets' record slots (ra2port::IsoTileRef), the ABuffer/ZBuffer
ring positions and RLE_Blit's buffer positions at pointer width; the radar animation shape and the
command-bar button walk no longer go through ints; the positional-sound pool sized from the native
SoundNode; the voxel span steps through ra2port::x87_ftol. Drafts (NON-EXACT, from the RA2 twins,
TibSun's text or gamemd.exe's code): Tactical::Render (YR's foreground pass), Draw_Tile, the
sidebar and strip AI/Draw_It/Recalc, Techno_Draw_Object, the voxel render, the zone and subzone
spans, Execute_DoList, the EVA speech service, the display cursor and Mouse_Left_Up, and others in
yr/glue/*.inc; many traps were gamemd names of functions the tree has under another name
(data/name_aliases.tsv).

Known gaps: a skirmish abort returns to the setup without a score screen (YR does that; lane Y4
reaches the score screen with a lost game). Partial drafts: Wait_For_Players_To_Load (the network wait),
FUN_0063AB60/FUN_0063A5A0 (planning mode), LoadProgressManager::Draw (campaign screen),
StripClass::Draw_It (the observer list), the strip's production-abandon events in Recalc.
(Can_Deploy_Now and the GeneratePalette fault: fixed by lane Y4, below.)

## Stage 2, lane Y4 (2026-11-15): a lost skirmish and the score screen

Main menu -> Skirmish (Short Game) -> a band selection and D (the MCV deploys into a construction
yard) -> H -> Sell -> the construction yard sold -> the game is lost -> the Skirmish Score screen ->
Continue -> setup -> Back -> Single Player -> Back -> main menu. `ctest -R yr-native-skirmish-score`
(yr/tests/yr_skirmish_score.py) runs that route (same build and data conditions as the smoke test).

Fixes of note (all NATIVE, native/yr only):
* No unit ran its AI: the TechnoTypeClass constructor draft stored gamemd.exe's +0x234 into
  IsHasRadialIndicator; gamemd.exe's ObjectClass::Unlimbo reads IsSentient there, so units never
  joined the logic list and queued missions never started. The same draft left PaletteFile unset:
  the intermittent ColorScheme::GeneratePalette fault during a load (garbage palette names whose
  negative scheme hash indexed outside the bucket table).
* The view scrolled north-west for good: Blit_Sidebar called the mouse through a stand-in class
  whose MSVC Draw_Mouse slot is Set_Mouse_Point in the Itanium layout (two destructor slots).
* The drive tracks were empty (RawTracks holds pointers yr_data.py cannot load): TibSun's tables.
* Address names of members of unit-defined globals (DAT_00886F94 is SidebarRect.Y) had storage of
  their own: fixups map them to the members; yr_data.py lets an unbounded scalar array run over
  the address names of its later elements (DAT_007ED3CC / MapClass::RadiusCount).
* The score screen sorted its pointer table with a 4-byte element size.
* Vtable-slot and second-name bindings read from gamemd.exe's vtables and calls
  (data/name_aliases.tsv): TechnoClass slots 0x3D0 and 0x3E0-0x3EC, GetFireErrorWithoutRange,
  ObjectClass::Direction, SidebarClass::Add, UnselectAll, the area damage's three names.

NON-EXACT drafts (each labelled in its file): from gamemd.exe's code TechnoClass::Can_Deploy_Now
(YR's 891-byte version, replacing TibSun's text), InfantryClass::Assign_Destination (TibSun's text
with YR's changes), FootClass::Active_Click_With (TibSun's text with YR's cases); from TibSun's
text WalkLocomotionClass::Movement_AI, DriveLocomotionClass::While_Moving, AStarClass::
Find_Path_Regular, InfantryClass::Per_Cell_Process, BuildingClass::Exit_Object and
Do_MISSION_DECONSTRUCTION, HouseClass::Can_Build (with the RA2 twin's YR-era tests),
Where_To_Place_Building and Can_Build_Here, CellClass::Reduce_Tiberium, the area damage
(Explosion_Damage), XSurface::Plot_Line, BounceClass::AI, Tactical::Draw_Tiles(Cell).

Known gaps: the selection brackets are not drawn; TibSun-text drafts are not compared with
gamemd.exe yet (YR's sell survivors, YR's area damage details); the AI opponent's further play
will reach more unmatched functions.

## Next

1. The buttons' targets (input not yet exercised; the runs are unattended): each submenu
   will stop at its unmatched handlers (ListBoxHandler 14.7 KB, ComboBoxHandler 6.9 KB,
   ComboDropWindowHandler 7.6 KB, SysTabHandler, NewEditHandler, PrepareDialogChildControls,
   UI::RegisterWindow; RA2 counterparts in native/link/behaviour_m14_ownerdraw.cpp).
2. The owner-draw unit's other 32-bit pointer assumptions: globals declared `int` that hold
   pointers (FUN_00603870's shapes, DAT_00833674), `(char *)p + 0x74`-style raw offsets on other
   variables, union views with pointer members read at the same offsets.
3. Init_Game's CD search when no search path is set (not drafted: the host always passes -CD.).
4. Variant* and the other Win32 imports the port does not yet supply (the run lists them).
