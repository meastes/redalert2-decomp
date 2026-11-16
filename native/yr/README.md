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

## Stage 2, lane Y6 (2026-11-16): longer skirmishes

`ctest -R yr-native-skirmish-long` (yr/tests/yr_skirmish_long.py): ten minutes of a skirmish against
three hard computer opponents without a trap or a fault (same build and data conditions as the
smoke test). The skirmish setup comes from the write folder's RA2MD.INI, which the test writes:
`[Skirmish] SlotNN=<type>,-2,-2` with type 4 hard, 5 normal, 6 easy and 1 closed (the setup's
own encoding, skirmish.cpp Skirmish_Slot_Type); `--opponents` and `--difficulty` choose them.
`RA2_NATIVE_TRAP_CONTINUE=1` lets a research run go on past traps and lists them all.

The trap in HouseClass::AI_Build_Defense about 15 s into a game was not caused by RA2MD.INI: the
function was simply unmatched, and which unmatched function an AI reaches first changes from game
to game (the start positions and the random seed differ per run); the runs that kept a write
folder happened to reach it first, runs without one reached UnitClass::Do_MISSION_HARVEST first.

NON-EXACT drafts (each labelled in its file), in the order long games reached them:
* from the RA2 twins' text in the YR tree's names, with YR's changes read from gamemd.exe:
  HouseClass::AI_Build_Defense (YR weighs the candidates by their defense value alone),
  UnitClass::Do_MISSION_HARVEST (a slave miner hands the mission to its slave manager; an idle
  harvester goes for repair when a repair bay is reachable), ParticleSystemClass::Smoke_AI and
  Spark_AI, SuperClass::Launch (the RA2 twin's Place plus YR's psychic dominator, spy plane,
  genetic mutator, force shield and psychic reveal, and YR's chrono warp, iron curtain and paradrop
  changes), the area damage (Explosion_Damage: now the RA2 twin's warhead spread instead of
  TibSun's fixed 1.5 cells; TibSun's deforming, cliff collapse and fire spread left out);
* from gamemd.exe's code alone (YR-only): AircraftTrackerClass::FillCurrentVector, the slave
  miner's two state machines (SlaveManagerClass FUN_006AF6C0 and FUN_006AFD60);
* bindings: BuildingClass::Fetch_Super_Weapon(2) are the tree's FirstActiveSWIdx and
  SecondActiveSWIdx (data/name_aliases.tsv).

Fixes: the radio's message parameter is pointer-sized natively (fixups.json; it carries an object
pointer for RADIO_MOVE_HERE, which faulted when a harvester docked at a refinery); LParam is
defined in glue/yr_globals.cpp.

Selection: YR has no TibSun-style brackets for units; a selected unit shows the pip board
(pipbrd.shp) under its health pips, a selected building its pip bar along the foundation's top
edge inside the white corner brackets. Lane Y3's TibSun text drew from ObjectTypeClass::SelectShapes
and PipShapes, which YR never loads, and placed the building bar at a zero length.
TechnoClass::Draw_Health_Bar is now written from gamemd.exe (FileSystem::PIPS_SHP, PIPBRD_SHP,
PixelSelectionBracketDelta, YR's pip frames 16-18 and its pip rules).

TibSun-text drafts compared with gamemd.exe (task list of lane Y4):
* BuildingClass::Exit_Object: YR passes the preferred cell to Find_Exit_Cell for vehicles only;
  infantry and the other war factory get no preferred cell. Fixed.
* The sell survivors (Do_MISSION_DECONSTRUCTION): YR leaves survivors unless the building both has
  an archive target and undeploys (TibSun: unless either), so a sold construction yard leaves
  survivors; a building that absorbs units or infantry lets them out, and a garrisoned one its
  occupants. Fixed.
* The area damage: TibSun's one cell and neighbours vs YR's warhead spread. Redone (above).
* HouseClass::Can_Build: YR also allows what a secret lab of the house gives, and refuses a
  building whose super weapon can be switched off from the setup when super weapons are off
  (unless it is one of the rules' BuildTech). Added; the rest (prerequisites, the computer's
  shortcut, build limits) matches.

## Next: yr-native on iOS on its own (notes, lane Y6; not built yet)

What a standalone `yr-native` iPhone/iPad app needs before the one-game iOS work (the RA2 app's
`build-ios.command` shape: CMake for `CMAKE_SYSTEM_NAME=iOS`, arm64, SDL3 built statically, Ninja):

1. **CMake gate.** `native/CMakeLists.txt` includes `yr/yr.cmake` only for `APPLE AND NOT iOS`.
   Allow iOS there, and give `build-ios.command` a target choice (`--target yr-native`). Every
   generation step runs on the build host and stays as it is: `yr_sdk.py` (the llvm-mingw headers),
   `yr_units.py`, `yr_aliases.py`, `yr_data.py`, `link_closure.py` (with the iOS SDK's system
   libraries and frameworks, as the RA2 iOS build passes them), `weaken_duplicates.py` and
   `defer_initializers.py` (object-level Mach-O edits; same format for iOS objects).
2. **The app bundle.** The RA2 app's iOS block in `native/CMakeLists.txt` (MACOSX_BUNDLE,
   Info.plist, icon set, `packaging/ios_sign.py`, `RA2_IOS_TEAM`/`RA2_IOS_BUILD`) is written for
   `ra2-native`; make it a function and call it for `yr-native` with its own bundle id, display name
   ("Yuri's Revenge"), icon and Info.plist. The data reader does not change (gamemd.exe read at run
   time, never shipped).
3. **Initialisers with chained fixups.** iOS 17 targets link with chained fixups: check that the
   deferred initialiser section (`__DATA,__yr_init_func`, called by `glue/yr_original_data.cpp`)
   still holds rebased pointers when the YR units' static initialisers are called after the data
   load (dyld rebases every pointer in `__DATA`, so it should; check on the Simulator first).
4. **Importing the game files.** `platform/sdl/app_mode.hpp` already builds per game (`kHostGame`,
   `exe_name`, the `yr_game_exe` settings key) but its import filter and checks are RA2's: it
   copies `game.exe`, `ra2.mix`, `language.mix`, `multi.mix`, `theme.mix`, `maps*/movies*/expand*`
   and the loose `.mmx/.map/.mpr/.ini/.csf/.pkt` files. A YR import must also take `gamemd.exe`
   (with its 4 KB overlay; the no-CD byte is undone when it is checked), `ra2md.mix`, `langmd.mix`,
   `multimd.mix`, `thememd.mix`, `expandmd*.mix`, `mapsmd*.mix`, `movmd*.mix` and `BINKW32.DLL`, and
   its required list and message must name Yuri's Revenge (the RA2 files are needed too: YR reads
   both games' mixfiles). YR's writes (RA2MD.INI, saves) go to the app's own Documents folder.
5. **The screen.** YR's menus are 800x600 like RA2's; the iOS resolution rule (720 or 600 lines by
   the screen) and the touch layer (`touch.cpp`, the toolbar) are the host's and apply as they are;
   check the YR sidebar width and the command bar with the toolbar on a phone.
6. **Tests.** Run the YR smoke, score and long skirmish routes on the Simulator "RA2 Duo Validation"
   (`RA2_NATIVE_IMPORT=<folder>` imports without the picker), then on a device.
7. **Traps.** The link closure's traps
   stay in the build until the remaining unmatched functions have drafts (a trap ends the game, as
   on the Mac).

## One game (lane Y5, 2026-11-16): Red Alert 2 and Yuri's Revenge in one app

User design (2026-11-15): one game. A Red Alert 2 / Yuri's Revenge toggle at the bottom left of the
main menu, away from its buttons; the deeper menus are the selected game's; both games in one app
with one engine active at a time; the toggle swaps engines in the process and lands on the other
game's main menu without its intro; the intro plays once at start-up, for the game chosen last,
which the settings keep. Built with `-DRA2_NATIVE_COMBINED=ON` (with `-DRA2_NATIVE_YR=ON` and
`RA2_NATIVE_LINK_CLOSURE`; Mac); off by default, and `ra2-native`, `yr-native` and their tests
are unchanged.

    cmake -DRA2_NATIVE_YR=ON -DRA2_NATIVE_COMBINED=ON <native build dir>
    cmake --build <native build dir> --target ra2yr-native     # with ra2-engine.so, yr-engine.so

### How the two engines live in one process

* **Symbols.** Each game is an engine bundle (`MH_BUNDLE`, `ra2-engine.so`, `yr-engine.so`): the
  same objects as its executable (`ra2-native`: src/recovered's units, native data, the port;
  `yr-native`: the YR units, `yr_data`, the port), with its own copy of the platform layer
  (`ra2_port`, the SDL host). The host is compiled once more per engine with
  `RA2_NATIVE_ENGINE`: `main.cpp`'s `main` becomes `ra2_engine_main` (`platform/sdl/engine_api.h`).
  ld64's `-exported_symbol _ra2_engine_main` makes every other symbol private to its bundle, so
  the names both games define (`WinMain`, `MapClass`, the C++ templates and vtables, every port
  function) never meet, and weak definitions are never coalesced across the games. No source is
  renamed; neither tree knows about the other.
* **Globals.** All of a game's globals are its bundle's. Each engine start maps a fresh copy of
  its bundle (copied to a temporary file and unlinked once mapped: dyld maps a file once per
  process), so the game starts from its static initialisers with the original's data loaded,
  exactly as a process start would. Images that used to depend on being image 0 now find their
  own image (the YR deferred initialisers' section, `original_save.cpp`'s in-image test).
* **Shared.** SDL (Homebrew's dylib on the Mac: one copy) and the window belong to the thin host
  (`combined/one_game.cpp`, `ra2yr-native`). An engine takes the window it is given
  (`ra2_sdl_host_use_window`; the game's `CreateWindowExA` takes it over) and makes its own
  renderer, textures and audio stream on it. The settings file is shared: the thin host owns its
  `game=ra2|yr` line; YR's own keys are separate (`yr_game_exe`), and in the app YR writes its
  RA2MD.INI and saves under `Game/YR`, so neither game's Load menu lists the other's saves. Each
  game reads its own executable variable (`RA2_GAME_EXE`, `RA2_GAMEMD_EXE`).
* **A swap.** The toggle (below) asks the port to end the game the way a player does: the port
  presses Exit Game in the main menu and OK in its question as those dialogs open; the host
  freezes the picture first, so the window keeps the menu's last frame meanwhile. The game's
  own shutdown runs (`Main_Game` returns, `WinMain` frees its art, surfaces, mixfiles and network
  and returns). The engine then gives the window back and destroys what it made with SDL (renderer
  with its textures, audio stream, event watch: `ra2_sdl_host_release`). The thin host runs the
  ended engine's exit handlers at once (`__cxa_finalize_ranges` over its image: `Prog_End`, the C++
  static destructors, the app settings' save), as `dlclose` or `exit` would, and starts the other
  engine with `skip_intro`: the port opens no Bink movie before the first dialog (the logos and
  the intro), so the other game's main menu comes up in about 5 s. The ended image stays mapped
  and is never called again (a thread of it may still be parked there; unmapping would pull the
  code from under it). Cost: its memory (code and what the game did not free) per swap.
* **The time limit of a test run** is the run's: each engine gets what is left; an ended engine's
  watchdog stands down (`ra2port_run_retire`).

### The toggle

`ra2port::set_game_toggle` (port/src/user32.cpp): the main menu (the dialog with Exit Game,
0x3EE "GUI:ExitGame", the same in both games) gets a clone of its version line (0x71D), clickable
(SS_NOTIFY), labelled "Play Yuri's Revenge" / "Play Red Alert 2" and placed, once the game has laid
the menu out, at the left end of the menu's bottom bar (the tool-tip line 0x695, which starts that
much further right). The game's own static code draws it, in its menu font and colour; the deeper
menus are simply the running game's. The selection is the settings' `game=` line (the thin host
writes it at every start and swap); the next start runs that game with its start-up movies.

ctest `one-game-toggle` (yr/tests/one_game_toggle.py, needs RA2_NATIVE_YR_DATA_DIR and
RA2_NATIVE_CENSUS_GAME_EXE): RA2 menu -> toggle -> YR menu (no start-up movie) -> toggle -> RA2
menu -> a skirmish in RA2; the game chosen last (RA2) -> YR -> a skirmish in YR; then YR, chosen
last, starts with its movies. Checks the engine order, the skipped movies, the toggle in each
menu, game frames in both skirmishes, the settings and that no process of the run is left.

Fixed on the way: YR's exit freed the menu drawers with the vector deleting destructor
(gamemd.exe 0x0072B230, harmless at exit on Windows, an abort natively; fixups.json).

### Next: iOS and Windows

* **iOS.** No `dlopen` of app-supplied code on a device (and no loose bundles in an app): both
  games must be linked into the one executable. That needs the symbol separation done at the
  object level instead of the bundle level: link each game's objects into one relocatable object
  (`ld -r`) with only its entry exported (`-exported_symbol`, which `ld -r` honours by making
  the rest private), then link the two objects and the thin host into the app. A fresh start of
  a game then cannot remap its image: its data segment would be reset from a saved copy of the
  pristine `__DATA`/`__bss` (taken before its initialisers run) and its initialisers run again,
  or the swap restarts the app process at the other game's menu (iOS allows `exit` only as a
  crash; not acceptable). Data reset is the plan; the threads of the ended game must be stopped
  first (the mouse timer, audio; the network thread is parked). The window: the iOS host's
  `app_window` already hands the game a window; the touch toolbar and the import flow are per
  app and move to the thin host.
* **Windows.** Two DLLs (`ra2-engine.dll`, `yr-engine.dll`) built from the same objects with
  only `ra2_engine_main` exported (PE exports are explicit: nothing else is visible), loaded with
  `LoadLibrary` from a fresh copy per start, `RA2Native.exe` as the thin host. The engines' C
  runtime must be the shared UCRT DLL (each DLL with a static CRT would keep its own heap and
  `atexit` list; the DLL's own list runs at `FreeLibrary`, which is not called for the reason
  above: the thin host calls the engine's exported finaliser instead). The window and SDL3.dll
  are shared as on the Mac.

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
