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

## yr-native on iOS on its own (lane Y7, 2026-11-16)

`native/build-ios.command [simulator|device] --yr` builds Yuri's Revenge's own iPhone/iPad app,
`build/ios-yr-<platform>/YR.app` (bundle id `io.github.ra2native.YR`, home-screen name "Yuri's
Revenge", the placeholder icon on purple: `make_icon.py --yr`), from the same tree as the Mac
`yr-native` (`-DRA2_NATIVE_YR=ON`; llvm-mingw's headers from `RA2_NATIVE_YR_MINGW`, found under
`private/toolchains` by default). RA2.app is unchanged: the iOS bundle block of
`native/CMakeLists.txt` is the function `ra2_ios_app(target exe id name [icon args])`, called for
`ra2-native` and (in `yr.cmake`) for `yr-native`. Signing and the bundle audit are RA2.app's
(`ios_sign.py` takes the bundle id from the app's Info.plist; `audit_ios_bundle.py --executable YR
--bundle-id io.github.ra2native.YR`, with `RA2_GAMEMD_EXE` sampling gamemd.exe). Device:

    RA2_IOS_TEAM=<team id> native/build-ios.command device --yr
    xcrun devicectl device install app --device <udid> build/ios-yr-device/YR.app

What it took:

* **The system's exports.** `yr_data.py` and the YR `link_closure.py` call now get the iOS SDK
  (`--sdk`): without it they read the Mac SDK, UIKit's `NSFontAttributeName` and
  `UIFontWeightSemibold` became trap stubs, and SDL's launch screen crashed before `main`.
* **Asserts.** The iOS build has no build type: the YR units get `NDEBUG`, as gamemd.exe and the
  Mac Release build (else `Assert_Failure` was one more trap).
* **Chained fixups.** Nothing to do: `__DATA,__yr_init_func` holds rebased pointers on iOS 17
  (281 initialisers run after the data load on the Simulator).
* **Import.** `app_mode.hpp` builds per game: YR's app copies gamemd.exe (not game.exe),
  `ra2md/langmd/multimd/thememd.mix`, `expandmd*`, `mapsmd*`, `movmd*`, `ecachemd*`, `elocalmd*`
  beside RA2's MIX files (28 files, 1.88 GB from the user's installation), requires the eight base
  MIX files and names Yuri's Revenge in its messages; the screen's resolution goes into RA2MD.INI;
  writes go to `Documents/Game/YR`.
* **Sound.** `DAT_00816514` (the sound system master's pointer, passed to AudioLoadSystem) is bound
  to drv_dsnd.cpp's `AudioSystemDSD` (`data/name_aliases.tsv`): gamemd.exe's copy held 32-bit
  addresses, so YR crashed in Init_Game whenever sound was on (every Mac test passes `-noaudio`;
  the Mac app had the same crash). YR now plays with sound.
* **Touch.** The RA2 touch layer is the host's and applies as it is; YR needed the game-side
  hooks RA2 has in native/link: `glue/yr_app_hooks.cpp` gives the map pan
  (`ra2_native_scroll_map`, Tactical::Set_Tactical_Position) and the settings' save, and the
  game loop (conquer.cpp, fixups.json) calls `yr_native_main_loop`, which marks play
  (`ra2_enh_in_play`: the toolbar shows in play only) and runs the host's frame (`ra2port_frame`:
  held buttons, pans, frame-numbered script lines, the stats line). Classic/Modern controls are
  the Enhancements page's (RA2's native/link): not wired into YR yet (YR plays Classic; lane Y8
  wired them).
* **Player input traps.** A tap on a sidebar icon reached `SelectClass::Action` (a trap: the Mac
  runs never built anything by hand): adopted from a newer ra2-decomp commit where it matches
  exactly (`glue/sidebar_native.inc`, with `SuperClass::Impatient_Click`). An aircraft flying to a
  destination reached `FlyLocomotionClass::Nearing_Target`: a draft from the RA2 twin and
  gamemd.exe (`glue/fly_native.inc`, non-exact). Computer players' units reached
  `HouseClass::Random_Cell_In_Zone` (Where_To_Go; TibSun's text with gamemd.exe's changes,
  `glue/house_native.inc`), deployed slave miners `TechnoClass::ScanForTiberium` (bound to the
  tree's `TechnoClass::FUN_0070F8F0`, gamemd.exe's BuildingClass vtable entry) and their slaves
  `SlaveManagerClass::FUN_006B0880` (from gamemd.exe, `glue/slavemanager_native.inc`).
* **Map's members named as globals.** The tree declares the pending placement and the interface
  modes (0x0088098C..0x008809A0: Map 0x0087F7E8 + 0x11A4 PendingObjectPtr ... +0x11B8
  IsTargettingMode) and the placement copy (0x00880978..0x00880980) as separate `int` globals;
  natively those are other storage than Map's members, so a building picked up for placement never
  reached the placement code (and its pointers were cut to 32 bits). fixups.json points
  Manual_Place, TAction_CLEAR_MODES, the coop setup, super.cpp and Tactical at the members. Others of
  the kind remain (the radar's 0x008809F4 LastDrawRect in sidebar.cpp, movie.cpp's 0x008809D8): not
  on the play path checked here.

Simulator route (`yr/tests/ios_sim_skirmish.py`, not a ctest: "RA2 Duo Validation" only, booted
and shut down by the script): the first start imports the files (`RA2_NATIVE_IMPORT`), then taps
through the menus to a skirmish and plays with the touch gestures (swipe pan, hold-and-drag box,
the toolbar's Deploy, Guard and A-Move, taps on sidebar icons and the map); `--score` adds a
second start that loses a short game and goes through the Skirmish Score screen back to the main
menu. Window dumps are the pictures (`simctl io screenshot` shows black for the game's Metal layer).
Results (2026-11-16, Duo Simulator, 872x600, sound on, about 62 frames a second, 101-106 MB): the
import, the menus, the skirmish, the score route back to the main menu; 300-360 s of play without a
trap after the fixes above, with the MCV deployed and power plants built and placed by taps. Open:
one SIGBUS in CellClass::Draw_Shroud_Or_Fog_Shape (glue/cell_native.inc, the shape copy) at a
game's start in one run of ten, not seen again (found and fixed by lane Y8); computer players reach more unrecovered functions
the longer a game runs (each a trap, as on the Mac).

## yr-native on iOS, second stage (lane Y8, 2026-11-16)

* **The SIGBUS at a game's start** (1 run in 10 on the Simulator, CellClass::Draw_Shroud_Or_Fog_Shape):
  `FileSystem::LoadFile` returns an `SHPReference` for a `.SHP` name (gamemd.exe 0x005B40B0), not the
  file; lane Y3's draft (TibSun's text) read that object as a ShapeSet, so its frame rectangles came
  from the reference's own pointers and the heap past it, and its pixel pointer was the object plus a
  garbage offset. Rewritten from gamemd.exe 0x0047EFE0 (`glue/cell_native.inc`, NON-EXACT):
  `SHPStruct::GetFrameBounds` and `GetPixels`, which load the file behind the reference, and the two
  loops as gamemd.exe has them. The shroud's soft edges are now drawn (they never were).
* **Controls: Classic / Modern and Queue five with Shift** (`glue/yr_enhancements.cpp`, native
  additions): RA2's rules (native/link/native_modern_controls.cpp, native_queue_five.cpp) on the YR
  tree's classes, hooked by fixups in ScrollClass::Message_Handler 0x006930A0 (left and right
  release), DisplayClass::Mouse_Left_Release 0x004AB9B0 (the band's walk), RadarClass::RTacticalClass::
  Action 0x006539D0 (the radar's buttons) and the sidebar's two PRODUCE adds (`glue/sidebar_native.inc`).
  Yuri's Revenge's own Shift right click on a build icon already abandons every queued copy
  (ABANDON_ALL), so Queue five changes only the left click. The touch layer needs nothing new: a tap
  is a left click, a two-finger tap or a held release a right click, and the toolbar's Fire, Move,
  Add and A-Move hold the keys the game reads. RA2's tap across player-controlled houses (lane C18,
  a campaign case) is not carried over.
* **The Options dialog and the Enhancements page.** The main menu's Options was a link-closure trap
  (FUN_0055FDB0, the dialog procedure of template 0xD5): drafted from gamemd.exe with the RA2 twin's
  behaviour row as the text (`glue/mainopt_native.inc`, NON-EXACT; YR sets the chosen resolution
  without RA2's test dialog). The port already appends the ENHANCEMENTS button to 0xD5 (gamemd.exe's
  template is read the same way); two fixups in ownrdraw.cpp put it and the page's Ok, Original,
  Enhanced and Cancel in the right-hand column. The page (`Yr_Run_Enhancements`) runs RA2's page
  logic on the YR dialog code and lists the rows Yuri's Revenge uses: Fullscreen, Vertical sync,
  Picture scaling, Menu fill (all the host's), Queue five with Shift and Controls. The settings are
  the app's own file, so on iOS YR.app keeps its own choices.
* **Not shared, and why.** Game interface size, Map zoom, Zoom gestures and HD vehicles are RA2's
  native/link code on RA2's classes and addresses, reached by 20-40 native fixups each in RA2's
  units (the sidebar surface's allocations and layout, Set_View_Dimensions and every site that takes
  the view's width as the sidebar's x, the in-game dialog loop's screen size, the voxel renderer's
  passes, Bit_Blit and the voxel caches). Yuri's Revenge has its own versions of those functions
  (several still drafts in glue/, gamemd.exe's voxel renderer differs from RA2's), so each needs its
  own set of YR fixups and a YR copy of the module; none is wired, and the YR page leaves the rows
  off. The host's half of each (the split layout, the zoom region, the HD overlay) is shared and
  would apply unchanged once the game side answers.
* **Long games had one opponent.** The skirmish setup's map 0 (The Alamo) is a two-player map, and
  the setup closes every slot past the first opponent: lane Y6's ten-minute Mac runs and lane Y7's
  Simulator runs played one computer player. `yr_skirmish_long.py` and `ios_sim_skirmish.py
  --opponents N` now pick map 30 (a four-player map) for more than one opponent; `--trap-continue`
  lists every trap of a research run, `--enhancements` sets the settings for one run.

* **wpaudio's device, EVA and music (sound on).** Every Mac test passes `-noaudio`; with sound on,
  Exit Game crashed in AudioCloseDown (and so did the one-game app's swap away from Yuri's
  Revenge). drv_dsnd.cpp's header (auddsnd.h) spelled the members of AudioSystem, AudioDevice,
  AudioChannel, AudioSample, AudioServiceInfo and AudioSearch it does not use as 32-bit gaps, so
  natively its load wrote numUnits = 1 over the low half of AudioSystem::master and the real
  numUnits stayed 0: wpaudio's device never opened natively, so the EVA voice (VoxClass's stream)
  and the music (ThemeClass's stream) were silent, and AudioUnloadAllSystems called through
  0x1_00000001. Fixed (fixups.json, yr/data/name_aliases.tsv), each found by running with sound
  on (lldb, guard malloc):
  * auddsnd.h's views and audplay.cpp's own AudioChannel view (byte pads) spelled as the real
    members at native widths, and audchan.h naming the three members the driver keeps in its
    padding (a probe compiled both views: equal offsets);
  * theme.cpp and vox.cpp declared the streamers' attribute setters and the attribute globals as
    int (the pointers were cut to 32 bits: AudioAttribsApply faulted);
  * Is_Speaking (Select_Game waits on it at Exit Game) is the tree's FUN_007529E0 (bound);
  * the speech queue's VoxNode was allocated with its 32-bit size 0x20 (vox.cpp);
  * ShapeSet's Get_Data, Get_Rect, Get_Color, Is_Transparent and Is_RLE_Compressed are SHPStruct's
    GetPixels, GetFrameBounds, GetColor, HasTransparency and HasCompression (gamemd.exe calls those;
    the tree's inlines and lane Y3's Get_Color draft read an SHPReference's own bytes as frame
    records: BuildingTypeClass::Get_Draw_Rect and the overlays' radar colours read past it). The
    same cause as the shroud's SIGBUS above.
  With sound on and guard malloc, the smoke route (a skirmish, Abort Mission, back to the setup)
  runs clean, and Exit Game returns from WinMain. Simulator (RA2YR.app, `--game=yr`, sound on):
  600 s against three hard computer players, no trap, 62 frames a second, 122 MB; the toggle
  from YR's main menu to Red Alert 2 and a skirmish there, with sound.

ctest `yr-native-controls` (yr/tests/yr_controls.py): a skirmish with Modern and Queue five (a double
click on a tank selects the tanks on the screen, a right click on open ground gives a move, Shift
with a click on the build icon queues five), the same with the defaults (none of those), and the
Options dialog -> Enhancements -> Enhanced -> Ok -> Main Menu.

Simulator ("RA2 Duo Validation", 872x600, sound on): 600 s against three hard computer players on
map 30 without a trap or a fault (`--trap-continue`: none reached), about 62 frames a second,
107-113 MB; 300 s with Modern and Queue five (the two-finger tap's move logged); two earlier 600 s
runs (one opponent, map 0) also clean. Mac: `yr-native-skirmish-long` with three real opponents,
600 s, no trap.

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
0x3EE "GUI:ExitGame", the same in both games) gets a clone of its Exit Game button (id 0x7F5E),
labelled "Play Yuri's Revenge" / "Play Red Alert 2" and placed, once the game has laid the menu out,
just above the left end of the menu's bottom bar (the tool-tip line 0x695), away from the
right-hand column. Lane C23 (user requests 2026-11-16: the version-line clone of lane Y5 read as
help text; then: it should look like the dialogs' push buttons, Keyboard Options' Assign and Reset
All, and not flicker) made it such a push button: the game's own owner-draw button code paints it
as those buttons (kind 3, WinData +0xB0: the 126x25 push button shape, red with yellow text, its
lit and pressed frames, the click sound), because the id is given that kind beside the listed
dialogs' ids: RA2 in port/fixups.json (unknown_5e7eb0 0x005E7EB0), YR in yr/fixups.json
(ClassifyLayoutBand, its twin); it takes that shape's size. The menu's movie (the TV screen,
632x570 at the top left) covers everything above the bar on the left, so the toggle stands over
it: the movie's timer draws the toggle on each new frame before the frame is shown (both games'
static procedures; the first version drew it after the frame, and a picture without it could be
shown: the flicker the user saw on the Duo). YR's button paint also takes the button's BST_PUSHED
as pressed, as RA2's does (the port sends no WM_DRAWITEM), so both games' buttons show their press. The deeper menus are simply the running game's. The selection is the settings' `game=` line (the thin host
writes it at every start and swap); the next start runs that game with its start-up movies.

ctest `one-game-toggle` (yr/tests/one_game_toggle.py, needs RA2_NATIVE_YR_DATA_DIR and
RA2_NATIVE_CENSUS_GAME_EXE): RA2 menu -> toggle -> YR menu (no start-up movie) -> toggle -> RA2
menu -> a skirmish in RA2; the game chosen last (RA2) -> YR -> a skirmish in YR; then YR, chosen
last, starts with its movies. Checks the engine order, the skipped movies, the toggle in each
menu, game frames in both skirmishes, the settings and that no process of the run is left. The
toggle is clicked by its id (input script `@ctl=32606`), and window dumps check it: six pictures
120 ms apart over the running movie must each show it red (at least 30% of its rectangle: 70-74%
measured; the menu-button art of the first version 27%, the version-line clone 0%) and the same
(no flicker), and the picture while it is held must differ (the pressed frame); and with
`RA2_NATIVE_TEST_TOGGLE_WATCH=1` the port checks the toggle's rectangle at every present of the screen
until the press: at most 2 changes (measured 2 in 310 presents; the after-the-frame version 78-80)
(lane C23).

Fixed on the way: YR's exit freed the menu drawers with the vector deleting destructor
(gamemd.exe 0x0072B230, harmless at exit on Windows, an abort natively; fixups.json).

### Lane Y13 (2026-11-17): Yuri's Revenge for Windows, data the x86-64 traps found, the rocket

* **The Windows build** (llvm-mingw cross-build on the Mac, as Red Alert 2's: `cmake -S native -B build/yr-win -G Ninja
  -DCMAKE_TOOLCHAIN_FILE=native/cmake/llvm-mingw-x86_64.cmake -DRA2_NATIVE_SDL_SOURCE=<SDL 3.4.2> -DRA2_NATIVE_SUBSYSTEM=all
  -DRA2_NATIVE_LINK_CLOSURE=ON -DRA2_NATIVE_YR=ON -DRA2_NATIVE_YR_MINGW=<llvm-mingw>/generic-w64-mingw32/include`, then
  `ninja -C build/yr-win YRNative`): `YRNative.exe` (GUI subsystem, for players) and `yr-native.exe` (console, the tests),
  self-contained (system DLLs and the UCRT only). What differs from the Mac build (yr.cmake `if(WIN32)` branches):
  - headers: the rewritten Win32 headers are searched ahead of the toolchain's (`-isystem`), leaving out every header
    the macOS SDK has (`yr_sdk.py --host-headers`), so the C library is the toolchain's as the Mac's is its SDK's;
    `long` stays `long` (32 bits on Win64, VC6's width, the type clang's MS intrinsics use): `sdk/yr_win_first.h` reads
    `_mingw.h` before `dialect_win.hpp` defines `__LP64__`, and `yr_prelude.h` spells the units' `RA2_LONG` as `long`;
  - COFF spellings of the units' and glue's assembler labels (`tools/yr_coff.py`, also the multi-literal `.globl`
    aliases), `yr_aliases.py`/`yr_data.py --target windows`, the link closure's `-alternatename` directives;
  - the units' and glue's static initialisers move from `.ctors` to `.yrini$m` (`defer_initializers.py`, COFF) and
    `yr_host_glue.cpp` runs them between its `.yrini$a`/`.yrini$z` markers after the original data is loaded;
  - Red Alert 2's link options (`ra2_windows_game_link`: select-any, 8 MiB stack, icon).
  Link closure 179 on Windows (the Mac's 184 less a few Win32 calls the system supplies; plus `XMP_Init` and two
  `UnVQ` tables, mangled with `unsigned long`: Westwood Online and VQA only). ctest `yr-native-skirmish-smoke` runs
  `yr-native.exe` under Wine (the Mac route: menus, a skirmish, Abort, the setup again): passes. Not tried on a real PC.
* **Data the Windows build's divide traps found** (x86-64 faults on an integer division by zero; arm64 quietly gives 0,
  so each was a silent fault on the Mac and iPhone too):
  - `ZGradients` (Bit_Blit's depth steps), `MouseClass::MouseControl` (every cursor's frames) and
    `InfantryClass::MasterDoControls` (the infantry actions' rates and flags) were never loaded from gamemd.exe (their
    structures were not on yr_data.py's list of pointer-free structures); now they are;
  - `DAT_00ABDE88` in the map unit is that unit's copy of geometry.h's `LEVEL_LEPTON_H` (gamemd.exe's initialiser
    0x005617E0); nothing set it, so `Is_Shrouded` and the cell-height reads used 0 (fixups.json);
  - the TechnoTypeClass constructor draft stored 1 in `IdleRate`; gamemd.exe stores it in `WalkRate` (+0x294). With
    `WalkRate` 0 the walk frames never advanced and with `IdleRate` 1 every unit ran its idle animation each frame:
    a likely cause of the user's unit animation glitches (to be checked by the user).
  The Mac window dumps' black tactical view (lane Y12) is not seen in the fixed build's smoke run.
* **NON-EXACT** `RocketLocomotionClass::Process` (gamemd.exe 0x006622C0; glue/rocket_native.inc): Red Alert 2's text
  with gamemd.exe's cruise missile (CMisl), its launch state 6, the layer re-submit and the legal-cell move.
  Link closure (Mac) 186 -> 184.
* **Long game**: 45 minutes against three hard opponents (Russia, Russia, Yuri; `yr_skirmish_long.py --countries 8,8,9`,
  new) on map 30: no trap, no fault.
* **LAN, first look**: the main menu's Network button reaches `IPXManagerClass::Service` (a trap) and then a fault in
  `Select_Game`; YR's LAN also needs the `MultiplayerGameMode` entries the closure lists (about 40). Not started.

### Lane YW (2026-11-17): the Windows tests and build command, LAN to the join request

* **Windows**: `native/build.sh yr-windows` (build/release-yr-win: `YRNative.exe`, and `yr-native.exe` for the
  tests). ctests on the Windows build, each under Wine in a fresh prefix deleted after the run
  (`tests/yr_wine.py`): `yr-native-skirmish-smoke` (lane Y13), `yr-native-skirmish-score`, `yr-native-combat`:
  all pass.
* **LAN** (Network): Mac to Mac and Mac to Windows-under-Wine, the host screen and the guest's lobby listing the
  host's game, its selection and the Join request. ctest `yr-native-lan-lobby` (`tests/yr_lan_lobby.py`, two
  Mac instances; `--binary-b <yr-native.exe> --wine-prefix-b <dir>` runs the guest under Wine). On the way
  (all NATIVE/NON-EXACT, native/yr only):
  - drafts: `IPXManagerClass::Service` (0x00541820, from the RA2 tree's twin; the Internet game's NAT and
    router branches left out, Westwood Online is not built), `UI::RegisterWindow` (0x00622820, the network
    dialogs' preparation), `LBSaveSelections` (0x005E5F10), `FUN_005EC9F0` (0x005EC9F0, a slot's entry on the
    host screen; the RA2 tree's Unknown_005cbf20 with YR's start/team arrays), `FUN_005EA3E0` (0x005EA3E0, the
    setup screens' slot controls refresh);
  - `WinsockInterfaceClass::Build_Packet_CRC` / `Passes_CRC_Check`: members calling the matched free functions
    (wsproto.cpp); MultiplayerGameMode's 24 base virtuals from gamemd.exe's vtable 0x007EED60;
  - `OriginalCheats` (0x00825C28) read from gamemd.exe after the data load (Cheat_Disable faulted on the
    Network button); Westwood Online's tournament tests `FUN_0077D940`/`FUN_0077D970` false (glue/yr_lan.cpp);
  - name aliases: `Build_Number` = FUN_00777B50, `OwnerDraw::Draw_Dialog_Back` = OwnerDraw::Paint, wstring.cpp's
    `Bit_Stuff`/`Bit_Unstuff_String` = bfiofile2.cpp's.
  Link closure (Mac) 183 -> 151.
* **LAN, next**: the host does not seat the joining guest yet (the guest's Join shows its waiting box; the
  host's slot stays Open): follow the join packet through `Process_Global_Packet` (netdlg2.cpp) on the host
  and the guest's `MPlayer_Guest_Dialog_Proc`; then Start Game, the game in step (RA2's `native-lan-pair`
  checks: per-frame CRCs, RA2_NATIVE_CRC_LOG) and the Mac + Windows pair. Closure entries likely on that path:
  `DecodePubGameopt`, `FUN_005E6920`, `Send_Statistics_Packet`, `Draw_Sync_Bars`, `FUN_00637E00`.
### Lane Y14 (2026-11-17): the user's graphics reports, the command bar, the Skip button, stand-ins

* **Vehicle noise on a new picture** (the user's iPhone reports: a deploying MCV and a Grizzly's first move drawn as
  pink, green and white noise, gone once the vehicle had moved): a vehicle composites its voxels on the eight-bit
  surface (UnitClass::Unit_Draw_Voxel); a picture not yet in the voxel cache is rendered directly by
  TechnoClass::Techno_Render_Voxel_Object (glue/techno_native.inc, NON-EXACT, gamemd.exe 0x00706ED0), whose draft drew
  it through the house's 16-bit converter: 16-bit pixels written into an 8-bit surface. It now picks EightBitDrawer
  while compositing, as gamemd.exe (0x0070717D) and the tree's Techno_Blit_Voxel do; Techno_Draw_Object's TibSun text
  had lost the same choice. ctest `yr-native-graphics` (yr/tests/yr_graphics.py): the harness watches the composed
  tactical view for noise-coloured pixels for 900 frames while the MCV deploys and a Grizzly first moves (0-24 pixels a
  frame now; 75-210 with the old draft).
* **The command bar** (the user's "missing bottom bar"): TabClass::Draw_It draws it on the composite surface and sets
  Map's TabClass::unknown_byte_5546 (0x0087F7E8 + 0x5546 = gamemd.exe's DAT_00884D2E); Update_Visible_Surface copies
  the bar's 32 rows only while that flag is set. Natively DAT_00884D2E was a separate global that stayed false, so the
  bar never reached the screen. fixups.json: gscreen.cpp reads the member. `yr-native-graphics` checks the layout (the
  tactical view ends where the bar starts) and the bar's rows on the screen; `ios_sim_menus.py` does the same on
  "RA2 Duo Validation" (interface size 1.25x, HD vehicles off: tactical 656x568, bar 656x32 at 568, 29% drawn).
  Likely also the cause of lane Y10's "a loaded game redraws its command bar only once the mouse passes over it".
* **The Skip button on the loading screen** (the user's screenshot): Yuri's Revenge keeps its intro movie open through
  the start-up load, and the touch controls counted any open full movie as playing. port/src/bink.cpp: a full movie
  counts only while the game polls it (BinkWait/BinkDoFrame within the last second of the game clock); host.cpp also
  updates the touch controls when presenting (the load presents without pumping events). `ios_sim_menus.py` checks the
  Skip button gives way before the main menu in Yuri's Revenge.
* **Stand-ins** (link closure 184 -> 175):
  - NON-EXACT HouseClass::Fire_PsyDom (glue/house_native.inc, gamemd.exe 0x0050A150, in the shape of its matched
    siblings);
  - the team script's ion storm actions call the matched FUN_00539EB0 / FUN_0053A090 (LightningStorm::Start /
    RequestStop) with their real types (fixups.json; team.cpp had (int center, bool flag) where gamemd.exe pushes a
    cell and a null owner);
  - bindings (data/name_aliases.tsv): TechnoClass::GotHijacked is the slot gamemd.exe fills with the tree's empty
    Scatter_Incoming_Infantry (0x0070D980); BuildingClass::Spied_By is Infiltrate (0x004571E0, what gamemd.exe's
    Per_Cell_Process calls); BuildingClass::Power_Off is DisableStuff (0x00452480, what Turn_Off calls);
  - BuildingClass::Captured's draft asks IsPowerOnline (vtable slot 0x350, as gamemd.exe 0x00448C01), not the
    undefined Is_Powered_On;
  - NON-EXACT MapClass::Repair_Train_Bridge and Repair_Bridge (TibSun's text, glue/map_native.inc; not compared with
    gamemd.exe's RepairWoodBridgeAt 0x00570050 / RepairConcreteBridgeAt 0x00573540; no test reaches them yet).
  Still open on play paths: BuildingClass::Do_MISSION_UNLOAD (0x0044D880, 2,848 bytes; RA2 twin 0x00449290 in
  src/recovered/building.cpp), ParticleClass::Gas_Behavior_AI, TechnoClass::Draw_A_Link_To (needs
  DSurface::Draw_Depth_Antialiased_Line, also open), RadBeam FUN_00659AC0/FUN_00659FF0, WaveClass::Draw_Laser /
  Draw_Sonic / FUN_007602E0, MechLocomotionClass::Movement_AI, UnitClass::Is_Route_Broken.
* **Harness** (glue/yr_harness.cpp): `deploy <ID>`, `move <ID> <dx> <dy>`, `where <ID>`, `rects`, `noise <frames>`.
* **Open from Y12**: the campaign dialog's Difficulty label `MISSING:''` (StringTable::LoadString given an empty
  label; the dialog's code was not found in the time box) and the LTNK position (not investigated).

### Lane YW2 (2026-11-17): a LAN game in step

* **The game**: two Macs (or two games on one Mac) play a Network game: the host seats the guest, chat works
  both ways in the setup screens, the guest's Accept and the host's Start Game load the map on both, and the
  frames run in step: 3,000 frames with every frame's game CRC equal. ctest `yr-native-lan-pair`
  (`tests/yr_lan_pair.py`; RA2_NATIVE_CRC_LOG gives each frame's "crc <frame> <value>", fixups.json's
  queue.cpp row). `yr-native-lan-pair-wine`: the guest is the Windows build under Wine
  (`-DRA2_NATIVE_YR_WINDOWS_EXE=<build/release-yr-win/yr-native.exe>`; skipped without it; Wine from
  RA2_NATIVE_WINE, PATH or the main checkout's private/tools bundle): passes, 3,000 frames, CRCs equal.
* **Why the guest was never seated**: the host refuses a player whose serial it already has
  (TXT_SERIAL_DUP), and every native copy read the same empty serial from the registry the installer would
  have written. Without one, the installation keeps its own 22-digit serial, made once at random, in its
  settings folder (`YRSERIAL.TXT`; glue/yr_lan.cpp `Yr_Installation_Serial`, called from Decrypt_Serial by a
  fixup).
* **Globals that are Session's fields**: gamemd.exe names some of Session's fields as globals
  (`GameModeOptionsClass::Instance` = Session.Options.MPModeIndex, `Game::PlayerColor` = PrefColor,
  `Game::PlayerCount` = NumPlayers, `Game::ChatMask` = PlayerChatEnabled,
  `Game::Network::PreCalcFrameRate` = PrecalcDesiredFrameRate); yr_data.py gave each its own storage, so the
  host's game mode never reached the options it sent (mode 0: the guest faulted in Read_Scenario_INI). Fixups
  name the fields in netshare, netdlg2, netdlg, session, house and scenario. Still separate storage, not on the
  LAN path: `Unsorted::SWAllowed` (Options.SWAllowed, house.cpp), `Unsorted::MuteSWLaunches` (0x00A8B538,
  Session+0x300, many units), `Game::RecordingFlag` (0x00A8D5F8, init/sidebar): a general fix belongs in
  yr_data.py (an address inside a defined object becomes that object's field).
* **NON-EXACT drafts** (native/yr only): `DecodePubGameopt` (0x005E3D10; the RA2 twin's draft and TibSun's
  shape; Westwood Online's Z/P/C options and chat-channel parts left out), `FUN_006475F0` Queue_AI_Multiplayer
  and `Wait_For_Players` (0x00648710) from the RA2 tree's exact text (gamemd.exe repeats it) with YR's names,
  `FUN_0064BDD0` Extract_Compressed_Events (the RA2 draft's text and fixes; same 414 instructions),
  `Draw_Sync_Bars` (0x0064A7B0), `FUN_00650A90` Game::LogFrameCRC (PARTIAL: the -MPDEBUG sync record keeps only
  the CRC, random number and frame). OwnerDraw::NewEditHandler (Y? draft): a chat line's Return now also sends
  the 0x501 notification the setup screens answer by sending the line (the RA2 rewrite's C20 fix).
* Link closure (Mac) 151 -> 137 (with Y14's landing).
* The Windows build with lane Y14's landing (harness orders, drafts, bink.cpp, host.cpp) cross-builds and links
  under llvm-mingw unchanged.
* **Next**: the -MPDEBUG sync record's object lists; longer games with orders (RA2's native-lan-pair gives orders
  and control groups); the both-games app on Windows (engines as DLLs).

### Lane Y15 (2026-11-17): globals that are members, team buttons on touch, stand-ins

* **Globals inside a defined object are its members** (tools/yr_member_aliases.py, data/member_aliases.tsv): gamemd.exe
  names some fields of its global objects as globals (Game::PlayerColor is Session.PrefColor, DAT_00884D2E is Map's
  TabClass::unknown_byte_5546, DAT_00B054D4 is ColorSchemes' item array). The tool finds every global yr_data.py would
  give storage whose address lies inside a unit-defined object (owners from globals.tsv, template types included),
  maps the offset to the member by the owner unit's clang layouts (i686-msvc against arm64 and x86-64 Windows,
  original_layouts.py --game yr), and yr_units.py appends to the owner's unit one assembler alias per name (owner +
  the member's native offset; `#if __x86_64__` for Windows). 48 names, 15 owners: Session (Unsorted::SWAllowed =
  Options.SWAllowed, Unsorted::MuteSWLaunches, Game::RecordingFlag = the Record bit-field's byte, Game::PlayerColor /
  PlayerCount / ChatMask / PreCalcFrameRate, GameModeOptionsClass::Instance, ...), Map (20: the pending placement,
  the radar, the command bar's flags), the vectors' raw counts and arrays (Houses, ColorSchemes, CurrentObject,
  IngameVQ, VoxArray), TargetTracker, CDList, WinVersion, RandomMapGen, RadarStatics' two buttons, DAT_00ABFEC8. Not
  merged (the tree declares an int where the member is a pointer): DAT_0088097C, DAT_0088098C, DAT_00880990 keep their
  lane Y7 fixups (narrowed). The per-name fixups of lanes YW2, Y14 and Y7 that the rule covers are gone. Found on the
  way: the superweapon timer's flash read a null DAT_00B054D4 (yr-native-skirmish-long's fault in
  Tactical::DrawTimer once a timer reached zero), and Delete_All_Objects sorted TargetTracker's table with the 8-byte
  entry size (fixups: sizeof, Archive declared a pointer). ctest `yr-native-member-aliases`: the table is what the
  build gives (rerun the tool after a header change, then build again).
* **Pointer tables declared without a bound** (fixups.json): DAT_00822774 (TXT_EASY/NORMAL/HARD), DAT_0081B538,
  DAT_00823FD4 hold their pointers followed by the strings; with the extent to the next global the strings were loaded
  as pointer slots and the pointers into them reached converted storage: the campaign dialog's Difficulty label read
  `MISSING:''`. Their bounds are the tables' own.
* **Team buttons on touch** (both games; platform/sdl/touch.cpp, host.cpp, link/team_button_hook.cpp,
  glue/yr_app_hooks.cpp): the command bar (TabClass::AI) gives an empty team the selection on a click, selects a team
  with members, and disbands a team on a right click; Ctrl is not read there, which is why the toolbar's Fire toggle
  did nothing. A hold (400 ms) on a team button now sends the right click and, 150 ms later, the click: the team
  becomes the selection. A tap is unchanged; a mouse is unchanged. Each game answers `ra2_native_team_button_at`
  from its bar's buttons; input scripts take `@team=N`. ctests `native-touch-team-hold` (RA2) and
  `yr-native-touch-team`; `ios_sim_skirmish.py --team-hold` on "RA2 Duo Validation" passed.
* **Stand-ins** (NON-EXACT, native/yr only): BuildingClass::Do_MISSION_UNLOAD (gamemd.exe 0x0044D880, read
  instruction by instruction on the RA2 twin's shape: the Bio Reactor's and unit absorbers' unload, the war factory,
  the gap generator); RadBeam FUN_00659AC0/FUN_00659FF0 (RA2's Compute_Pulse_Step/Compute_Swing_Step, fields checked
  against gamemd.exe); from Red Alert 2's text (mischa85/RedAlert2, attributed): MechLocomotionClass::Movement_AI,
  WaveClass::Draw_Sonic and Draw_Laser, ParticleClass::Gas_Behavior_AI, DSurface::Draw_Depth_Antialiased_Line;
  UnitClass::Is_Route_Broken (TibSun's text); TechnoClass::Draw_A_Link_To (gamemd.exe 0x00704E40, the mind control
  link, read instruction by instruction). Still open: WaveClass::FUN_007602E0 (wave type 3, 1,777 bytes). Link
  closure (Mac) 137 -> 127.
* **Open**: the LTNK off-map place-down/pick-up in the campaign chains (Allied 2, Soviet 7) is deterministic (the
  same garbage coordinates every run, e.g. -1447125004,-1459498755), so it is not a pointer's low half (those vary
  with ASLR); not found in the time box.

### Lane Y16 (2026-11-18): the LTNK off the map, the Magnetron's beam, stand-in tests, a longer LAN game, one game on Windows

* **The LTNK driven off the map** (Allied 2, Soviet 7; lane Y12's `place-down:`/`pick-up:` lines with the same garbage
  coordinates every run): gamemd.exe's drive locomotor has 72 turn tracks and 15 raw tracks; the tree has TibSun's 67
  and 13. YR's turn tracks 67-71 (raw tracks 14, 16 steps, and 15, 17 steps) are what Force_Track gives a unit leaving
  a factory diagonally; with the 67-entry table TrackControl[70] read past it, its raw track was RawTracks[-1] and
  Smooth_Turn added garbage offsets. The tables are pointer-free, so `TrackControl[72]`, `Track14_007E78A8[16]` and
  `Track15_007E7968[17]` are declared only (hdr/drive.h's fixup) and loaded from gamemd.exe (yr_data.py:
  `TurnTrackType`/`TrackType` are plain structures); `RawTracks[15]` (glue/drive_native.inc) keeps its pointers. On the
  way, yr_data.py reads a static member's bound from its own class's declaration (`Class::member` index), not from a
  same-named member of another class (ship.h's `TrackControl[67]` had given the drive's its bound; CDFileClass::First
  and LastCDDrive now get their own types too). `yr-native-campaign-allied/-soviet` fail on any off-map line.
* **NON-EXACT WaveClass::FUN_007602E0** (gamemd.exe 0x007602E0, 1,777 bytes; Draw_It's wave type 3, the Magnetron's
  beam): gamemd.exe's routine is Draw_Sonic with FUN_00760190 (matched) called where Draw_Sonic calls
  Set_Sonic_Pixel; the two disassemblies differ in those four call targets only (yr_dis.py; ra2-decomp's attempts.csv
  says the same). glue/wave_native.inc: the Draw_Sonic draft with the call swapped. Link closure (Mac) 127 -> 126.
* **ctest `yr-native-standins`** (yr/tests/yr_standins.py; harness orders `spawn <ID> [enemy|dx dy]`, `fire`, `enter`,
  `owner`, `waves`, `wave <type>`, `super <name> enemy`, `money`): a Magnetron (TELE), a Prism Tank (SREF) and a
  Mirage Tank (MGTK) fire on two enemy Apocalypses spawned beside them, sonic and magnetron waves are made directly,
  a spy goes into an enemy power plant, the Psychic Dominator and the Lightning Storm are placed through
  SuperClass::Discharged (the map click's path): no trap, the waves in flight, the Apocalypses destroyed, the spy in.
  Not covered: a hijacker (no rules ID found here; TechnoClass::GotHijacked is gamemd.exe's empty slot anyway, lane
  Y14), bridge repair (needs a map with a broken bridge) and the War Factory's unload (production through the
  harness is not written; the AI's factories run Do_MISSION_UNLOAD in every long skirmish).
* **A longer LAN game with orders** (`yr-native-lan-pair`, also `--frames 12000`): both sides' fighting vehicles are
  selected (`select * all`), made team 1 (Ctrl+1), moved as a group (`go`: one move event each in one frame), the team
  recalled and sent back every 600 frames, as RA2's native-lan-pair; from 6,000 frames on every combat unit hunts for
  the last 2,000. 3,000 frames (the ctest) and 12,000 frames (6 min 20 s, 36 group moves a side, the hunt): every
  frame's CRC equal.
* **One game on Windows** (`-DRA2_NATIVE_COMBINED=ON` in the Windows build; yr/combined.cmake's WIN32 branch):
  `ra2-engine.dll` and `yr-engine.dll` are the same objects as ra2-native.exe and yr-native.exe with the engine host
  (main.cpp's `ra2_engine_main`, `__declspec(dllexport)`: the DLLs' one export), libc++/libunwind/winpthreads inside
  each (-static); the build's SDL is then the one shared `SDL3.dll` (copied beside the executables), owning the window
  both games draw in. `RA2YRNative.exe` (GUI) and `ra2yr-native.exe` (console, the tests) are the thin host
  (combined/one_game.cpp's `_WIN32` parts): each start copies the engine's DLL to %TEMP% under a new name and
  LoadLibrary's it (a fresh image: the game's static initialisers again), an ended engine's exit handlers run through
  its DLL entry point with DLL_PROCESS_DETACH (what FreeLibrary would run: that DLL's atexit table, the static
  destructors, Prog_End, the settings save) and its image stays loaded; copies left by earlier runs are deleted at
  the next start. The host's environment changes (`_putenv_s`) reach the engines (one ucrtbase.dll). The last engine's
  exit handlers also run before the host returns (ExitProcess would run them in its DLL detach after ending every
  other thread), and the port's and host's `_exit` is TerminateProcess on Windows (platform/win/port_compat.h): the
  UCRT's goes through ExitProcess and the running engine's detach, and a test run's time limit never ended the
  process. Also: link()
  for the one-game imports' hard links (CreateHardLinkA, platform/win). ctest `one-game-toggle-wine`: the
  one-game-toggle route (RA2 -> YR -> RA2 and a skirmish, RA2 -> YR and a YR skirmish, YR chosen last) on
  ra2yr-native.exe under Wine (`one_game_toggle.py --wine-prefix`; time limits doubled). Not tried on a real PC.
* **native-basics' shroud flake** (RA2, link/harness_orders.cpp): the scouts' destinations were picked among the
  least-mapped directions without asking whether they could be reached; the start position is random, and a game
  whose three picks were all in another movement zone (cliff tops, water: the path finder refuses them, the scout
  never moves) failed. Destinations are now in the scout's own zone (MapClass::Zone with its MovementZone).

### Lane P1 (2026-11-18): packages for a real PC, build commands, the hijacker

* **Builds**: `native/build.sh windows-one` (new) builds the both-games Windows app and gathers what a PC needs in
  `build/release-win-one/RA2YRNative/` (RA2YRNative.exe, ra2-engine.dll, yr-engine.dll, SDL3.dll, SDL's licence;
  only system DLLs, the UCRT and SDL3.dll imported). `one-game-toggle-wine` passed on that build (374 s).
  `native/build.sh mac-one` (lane N1) already produced the owner's hand-made RA2YR.app: the same layout, SDL paths
  and ad-hoc signature (identifier local.ra2native.ra2yr); both engines reach the main menu from the bundle.
* **The hijacker** (ctest `yr-native-standins`, now also a hijack): no infantry in Yuri's Revenge's rules
  (RULESMD.INI in ra2md.mix and the patch's expandmd01.mix, read in place) sets `VehicleThief=` or `Thief=`; only the
  comment block describing the keys names them, so there is no rules ID to spawn. New harness orders: `thief <ID>`
  (sets IsThief, which InfantryClass::Theft_AI tests, and IsVehicleThief, which movement and the click's action test),
  `enter <ID> <TARGET> hijack` (MISSION_CAPTURE with the vehicle as the destination only, as
  FootClass::Active_Click_With's capture click) and `spawn <ID> enemy <dx> <dy>`. The test makes a civilian (CIV1) a
  thief nine cells from the base and sends it at an enemy truck (TRUCKA) thirteen cells out: by frame 1250 the truck
  is the player's and the thief has gone into it (3 of 3 runs). Found on the way: a thief spawned on the vehicle's
  own cell never hijacks (WalkLocomotionClass::Movement_AI's no-path branch clears its destination, and the vehicle
  then drives over it); a walking thief is the normal case. An enemy MCV as the target was flaky (the AI deploys or
  loses it).
* **Bridge repair**: not started. Many multiplayer maps carry a CABHUT (multimd.mix); a test needs a harness order
  that breaks the bridge next to the hut and the skirmish map index of such a map.

### Lane N3 (2026-11-19): the user's two Duo faults

* **The command bar's "select all units of this type" crashed the game.** TabClass::AI (gamemd.exe 0x006D0680) calls
  FUN_00732950 for the bar's TypeSelect button, and the TypeSelect key's quick press (FUN_00732CC0) calls it too; the
  YR tree has not recovered it, so it was the link closure's trap. glue/uicmnds_native.inc: NON-EXACT stand-in from
  gamemd.exe's 729 bytes (yr_dis.py) in the shape of its matched sibling FUN_00732280: the selected objects' type names
  (FUN_00732D10), the player's units on the screen of those types selected; when all are already selected, or on a
  second press (NavCycleMode 2), across the map; the "selected across screen/map" message. Link closure (Mac) 126 -> 125.
  ctest `yr-native-type-select` (yr/tests/yr_type_select.py): with one tank selected the button selects every tank,
  with nothing selected it selects nothing, and the T key does the same as the button. Input scripts take
  `@cmd=TypeSelect` (the bar button's middle; glue/yr_app_hooks.cpp registers the point resolver).
* **No on-screen keyboard in Yuri's Revenge's text fields.** The player-name field is a dialog Edit that OwnerDraw's
  FUN_0060F320 hides and replaces with a ListBox carrying a NewEditLink ("NewEdit") in GWL_USERDATA; port/src/user32.cpp
  answered the host only for windows of the classes Edit and NewEdit, and the YR engine had no
  `ra2_native_new_edit_focused` (Red Alert 2's is native/link/native_text_entry.cpp). glue/yr_app_hooks.cpp now answers
  it (the focus is a shown list box whose link says NewEdit) and `ra2_native_message_edit_open` (Session.Messages'
  edit line: chat and beacon text). As in Red Alert 2, the keyboard comes up when the field is tapped.
* **Checked in the Simulator** ("RA2 Duo Validation", `ios_sim_menus.py`, now 120 s of play by default): in both games
  a tap on the player name field (control 1696, `@ctl=1696`) gives "host: on-screen keyboard shown" and the tap away
  "hidden"; in Yuri's Revenge a tap on the TypeSelect button with one tank selected selects all five. The walk's old
  tap point missed Red Alert 2's field (further right), so it never checked Red Alert 2's keyboard.

* **Stand-ins the campaign triggers, the command bar and the keys reach** (item 1 of PORTS_REMAINING: a static trace of
  the 125 functions still missing at the Mac link, from their callers in the tree; link closure 125 -> 116):
  - BuildingClass::ForceCreate (gamemd.exe 0x00458A80; vtable slot 0x490): the trigger action "create building at"
    and HouseClass::RespawnStartingForces. glue/building_native.inc, from gamemd.exe's instructions on the matched
    FootClass::ForceCreate's shape (foundation cells' overlay cleared, the units there lifted and put down nearby);
  - bindings (data/name_aliases.tsv): IonStormClass::Lightning_Bolt is LightningStorm::Strike (0x0053A140, the
    trigger action "ion lightning strike"), Ion_Storm_End is RequestStop (0x0053A090, "ion storm stop"),
    TechnoTypeClass::Get_Weapon is GetWeapon (0x007177C0, an upgraded building's weapon);
  - XSurface::Draw_Ellipse: a selected Gap Generator's or Psychic Sensor's range ring
    (BuildingClass::Draw_Radial_Indicator). glue/xsurface_native.inc: Red Alert 2's draft of its twin
    (src/recovered/xsurface.cpp, not exact), not compared with gamemd.exe;
  - FUN_00733380 / FUN_007336C0, the HealthNav and VeterancyNav keys (glue/uicmnds_native.inc, from gamemd.exe's
    instructions with the matched helpers they inline);
  - BeaconManagerClass::PlaceBeacon (0x00430BA0) and Drawing::Intersect (glue/beacon_native.inc): the Beacon
    command's placement mode (the command bar's Beacon button in network games; a skirmish's bar has none), then a
    click on the map, then every frame's beacon draw. PlaceBeacon from gamemd.exe's instructions on Red Alert 2's
    accepted twin's shape.
  ctest `yr-native-standins-triggers` (yr/tests/yr_standins_triggers.py; harness orders `create <BLDG> on <ID>`,
  `bolt enemy`, `stormstart`, `stormend`, `beaconmode`, `beacons`, `cmd <name>`): a selected Gap Generator, a power
  plant forced onto the player's tanks (none lost), a bolt, a storm started and stopped, a beacon placed by a map
  click, three presses of each nav key selecting tanks. Input scripts also take `@cmd=Beacon` (network games).
  Still missing (callers found, reach not
  checked in play): the laser fence's and firestorm wall's placement drawing
  (Tactical::Draw_Fence_Placement / Draw_Firestorm_Wall_Placement, for building types with IsLaserFencePost or
  IsFirestormWall); Speech_Name (a trigger's description text); the score font's Print_String; LineTrail's
  constructor (types with line trails); Tunnel_AI (Tiberian Sun's subterranean movement); the random map generator
  (MapGeneratorClass) and the World Domination Tour (WorldDominationTour::*), both Westwood Online.

* **Planning mode** (the command bar's PlanningMode button stopped the game at its first draw with units selected):
  glue/planmgr_planning_native.inc, NON-EXACT stand-ins from gamemd.exe's instructions for 0x00637E00 (the planning
  events: commit, connect, node delete, a planned order), 0x00638120 (a planned order, Red Alert 2's
  Planning_Add_Order), 0x00634E10 (a member's branch), 0x00639CB0 (a closed loop's attack orders become attack-move),
  0x0063B550 (a unit's plan in outline, the tree's filled twin Draw_Filled_Unit_Waypoints in outline) and
  0x0063C690 / 0x0063CAE0 (a node in outline and filled). A fixup in FUN_0063B0A0 empties the drawn-nodes list
  (PlanningNodeClass::Unknown2) through the list itself: gamemd.exe's names for its fields were separate storage, the
  list was never emptied, and the hover test read a deleted node once a plan was committed. Link closure 116 -> 110.
  ctest `yr-native-planning` (yr/tests/yr_planning.py; script point `@cmd=PlanningMode`, harness orders `plan <dx> <dy>`
  (an order through Player_Assign_Mission, a map click's path) and `plans`): every tank selected, planning mode on,
  two waypoints planned, the commit, the tanks follow. Still partial from lane Y3:
  FUN_0063AB60 / FUN_0063A5A0 (the mouse over and clicks on planned nodes in planning mode) return false, so nodes
  cannot be picked up or linked by hand yet.

* **Production left inside the factory** (found by the new War Factory test): glue/building_native.inc's drafts
  (Exit_Object and its neighbours, TibSun's text) gave missions with Assign_Mission, which in the YR tree is
  ObjectClass's empty virtual (it backs the Mission property): a finished vehicle stayed inside the war factory
  (the factory never switched to MISSION_UNLOAD) and every other mission these drafts gave (the soldier's move
  from a barracks door, a harvester's harvest, ...) was dropped. They now call QueueMission(mission, false), what
  gamemd.exe calls there (vtable slot 0x1E8, MissionClass::QueueMission 0x005B35E0; 0x004445F0 for the unload).
  ctest `yr-native-war-factory` (yr/tests/yr_war_factory.py; harness orders `produce <FACTORY> <ID>`, `produced`,
  `mission <ID>`): a Grizzly produced in a spawned War Factory is unloaded (mission 16) and drives out, the factory
  returns to guard; a GI produced in a Barracks walks from the door.

## Next: iOS and Windows

* **iOS.** No `dlopen` of app-supplied code on a device (and no loose bundles in an app): both
  games must be linked into the one executable. The design and its size: "iOS in one executable"
  below (lane Y8).
* **Windows** (built by lane Y16: "One game on Windows" above). Two DLLs (`ra2-engine.dll`, `yr-engine.dll`) built from the same objects with
  only `ra2_engine_main` exported (PE exports are explicit: nothing else is visible), loaded with
  `LoadLibrary` from a fresh copy per start, `RA2Native.exe` as the thin host. The engines' C
  runtime must be the shared UCRT DLL (each DLL with a static CRT would keep its own heap and
  `atexit` list; the DLL's own list runs at `FreeLibrary`, which is not called for the reason
  above: the thin host calls the engine's exported finaliser instead). The window and SDL3.dll
  are shared as on the Mac.

### iOS in one executable: design (lane Y8; stage 1 built)

The Mac's engines are bundles mapped afresh per start; iOS allows neither `dlopen` of app code nor
writable-executable pages, so both games are linked into `RA2YR` (one app, bundle id to choose:
keep `io.github.ra2native.RA2` so RA2.app's saves and settings carry over) and a game's fresh start
becomes a reset of its own data in place.

1. **Symbols, at the object level.** Each game's objects (`ra2_recovered_objs` + `ra2_data` + its
   link_closure.c + its copy of `ra2_port` and of the SDL host compiled with `RA2_NATIVE_ENGINE`)
   are linked into one relocatable object, `ld -r -d -exported_symbol _ra2_engine_main_<game>`
   (`-d` defines commons; ld64 turns the symbols not exported into static ones in a `-r` output
   unless `-keep_private_externs` is given), with `-alias_list` applied there. The two objects and
   the thin host link into the app; nothing else of either game is visible to the other, as with
   the bundles. Each `-r` input list starts and ends with a marker object (`engine_begin_<game>.c`,
   `engine_end_<game>.c`: one symbol in each of `__text`, `__data`, `__bss`, `__common`,
   `__const`), so the game's share of every section is contiguous and bounded by known symbols.
2. **What stays single.** SDL (static on iOS, with its Objective-C classes) and the Apple host
   (`host_apple.mm`: `RA2ImportPicker`, `RA2PassView`, `RA2Toolbar`, the glass buttons) are left out
   of the engine objects and linked once: Objective-C class names are global in the runtime whatever
   the symbol visibility, so two copies would collide. The engines' references to `SDL_*` and
   `hostos::*` stay undefined in their `-r` objects and bind to the one copy. hostos keeps no game
   state across a swap except the toolbar, which the thin host owns (as the window already is).
   The import flow (RA2 files plus YR's 28 files, one folder picker) and the settings' `game=` line
   move to the thin host (`combined/one_game.cpp` gains the iOS app start that `main.cpp`'s app mode
   has now).
3. **Initialisers.** Both games' static initialisers must run at that game's start, not at launch:
   the YR units already have theirs moved to `__DATA,__yr_init_func` (defer_initializers.py); the
   same pass runs over the RA2 game's objects and the per-engine port and host objects into
   `__ra2_init_func` / `__yr_init_func`. On iOS 17 the linker emits `__TEXT,__init_offsets`
   (32-bit offsets) for the app's own initialisers; the moved sections keep plain pointers, rebased
   by the chained fixups like YR's today.
4. **A fresh start.** The first start of an engine copies its data range (the markers' `__data`,
   `__bss`, `__common` spans, about 5 MB for YR) to a pristine buffer before its initialisers run
   (pointers are already rebased: the copy is taken after dyld). Each later start copies the
   pristine bytes back, runs the engine's initialisers again and loads the original's data again
   (yr_data / native data, as now), so it starts exactly as a process start would. `__DATA_CONST`
   (`__got`, read-only after launch) is never touched.
5. **An end.** The game's own shutdown runs as on the Mac (Exit Game, `WinMain` returns). Its exit
   handlers run through `__cxa_finalize_ranges` over its `__text` span (the markers), as the Mac
   host does over a bundle's image range. Unlike the Mac, the code stays live for the next start of
   the same game, so its threads must have ended first: the port gains `ra2port_stop_threads()`
   (the multimedia timers' `std::thread`s, `CreateThread`'s pthreads: the mouse thread, the
   network's async thread, run_progress's watchdog) called before the reset, each joined or
   signalled and waited for. Memory the game did not free stays allocated (as on the Mac).
6. **Toggle, movies, saves.** Unchanged from the Mac (`ra2port::set_game_toggle`, `skip_intro`,
   `Documents/Game` and `Documents/Game/YR`).

**Built so far (lane Y8).** Stage 1, `-DRA2_NATIVE_ONE_EXECUTABLE=ON` (with COMBINED): `ra2yr-one`
on the Mac and `native/build-ios.command simulator --one` (`RA2YR.app`, bundle id
`io.github.ra2native.RA2YR` for now). Items 1, 3, 4 and 5 as above, except that the thread stop is
not done (a swap back to a game whose threads still run has not failed yet) and that item 2 is
an interim: SDL is linked once (the engines leave it undefined), but each engine still has its
own `host_apple.mm`, with Yuri's Revenge's three Objective-C classes renamed (`YRImportPicker`,
`YRPassView`, `YRToolbar`) so the runtime has no duplicate. The markers: `__data`, `__bss` and
`__common` (ld -r puts zero-fill there too: the closure's placeholders are built `-fno-common` so
none stays a tentative definition the final link would merge between the games). ctest
`one-exe-toggle` runs lane Y5's route on `ra2yr-one` (Red Alert 2 -> Yuri's Revenge -> Red Alert 2,
the third start a reset of its data, then a skirmish): passes. Simulator: `ios_sim_skirmish.py
--app build/ios-one-simulator/RA2YR.app --bundle io.github.ra2native.RA2YR --launch-arg=--game=yr
[--toggle]`: a 180 s Yuri's Revenge skirmish against three computer players passes; `--toggle`
(the YR main menu's toggle, then the play in Red Alert 2) found YR's exit crashing with sound on
(lane Y8 section: wpaudio's device, fixed). The import is one flow: each engine of the one-game app takes both games' files
(app_mode.hpp `import_wanted` under RA2_NATIVE_ENGINE; an import replaces the whole GameData, so
before this the second game's import threw the first game's files away): a fresh RA2YR.app started
in Red Alert 2 imported 29 files (1.89 GB) once, and Yuri's Revenge after the toggle found its own
(`--game=ra2 --toggle`: the swap with sound, Yuri's Revenge's menus and a skirmish at 62 frames a
second, 140 MB). Not done: the thread stop, one host_apple copy, a device build of RA2YR.app.

Size (lane time, one lane): the two `-r` objects with markers and the app link in CMake (iOS and
the Mac, so the Mac can test it without bundles) 1 day; RA2's initialisers deferred, the pristine
copy and reset, finalisers by range 1.5 days; the port's thread stop 0.5 day; SDL and hostos once,
the thin host's iOS start (window, toolbar, import of both games, settings) 1 day; the Simulator
route (toggle both ways, a skirmish in each, a swap during the menus) and a device build with the
bundle audit 1 day. About 5 days (35-45 hours), the main risks being state that survives a reset
outside the data range (the port's heap-held singletons are inside it; SDL's and hostos's are not
and must be released per swap) and the executable's size (YR.app's is 14 MB, 7 MB of it code; two games about twice that).

### The one-game app as the iPhone app (lane Y9, 2026-11-16)

* **The app.** `native/build-ios.command simulator|device` builds the one-game app by default
  (`build/ios-one-<platform>/RA2YR.app`; `--ra2` the Red Alert 2 app alone, `--yr` Yuri's Revenge's).
  Owner ruling: it replaces the Red Alert 2 app, with its bundle id `io.github.ra2native.RA2` and
  name "Red Alert 2", so the Red Alert 2 app's import (GameData), saves (Documents/Game), settings
  and RA2.INI carry over; Yuri's Revenge writes under Documents/Game/YR. The standalone YR app
  (`io.github.ra2native.YR`) stays buildable.
* **Import.** In the one-game app an import adds to what was imported before
  (`app_mode.hpp import_game_files` under RA2_NATIVE_ENGINE): a file the folder has the same
  (same size; small files the same bytes) is hard-linked from the old GameData, a file it lacks is
  kept (linked), a file it has otherwise replaces the old one; the staging folder is checked and
  swapped in by rename as before. So a player who had the Red Alert 2 app is asked, at the toggle to
  Yuri's Revenge, only for Yuri's Revenge's files (the prompt says the Red Alert 2 files are already
  there; picking the full installation copies only Yuri's Revenge's ~0.9 GB); a fresh install
  imports both games in one pick (the prompt names both). "Not Now" at Yuri's Revenge's import
  goes back to Red Alert 2's main menu (`app::other_game_ready`, engine->swap) instead of leaving
  the app. After a swap the engine takes the window the previous one used (it made a second
  full-screen window before).
* **Threads before a reset.** `ra2port_stop_threads` (port/src/run_progress.cpp), handed to the
  thin host as `ra2_engine_host::stop_threads` (API version 2) and called after the ended engine's
  exit handlers: those end the game's own threads (Prog_End's Kill_Mouse_Thread, YR's
  FUN_007B86B0); then the multimedia timers it left are stopped and joined, the WSAAsyncSelect
  thread joined (it was detached and looped for ever), the run's watchdog woken and joined (it
  slept on a plain sleep), and the CreateThread threads still alive are waited for (3 s) and named
  in the log. The port's state those use is never destroyed (function-local heap objects), since
  the exit handlers' static destructors run first. Mac: `one-exe-toggle` and `one-game-toggle`
  show "0 timer(s), the watchdog, 0 game thread(s) still running" at every swap (before the order
  change the mouse threads of both games were still running at the stop).
* **One Apple host.** `host_apple.mm` is compiled once (`ra2_engine_hostos_objs`): linked into
  `ra2yr-one` beside the thin host and into each Mac bundle; the engines' `-r` objects leave
  `hostos::*` undefined and bind to it. The YR class renames are gone. What it keeps across a swap
  calls the engine that started last: `toolbar_create`, `skip_button_create`,
  `keys_button_create` and `watch_audio_interruptions` rebind their callbacks when the toolbar or
  observer already exists.
* **Saves.** Red Alert 2's saves work in the one-game app as in RA2.app. (Since lane Y10 Yuri's Revenge
  saves in gamemd.exe's format: "Saves in gamemd.exe's format" below.) Until then Yuri's Revenge could not save: the port writes saves only in the original's compound format, from the field maps of Red
  Alert 2's classes (gen/original_layouts.cpp), and the YR build has none, so YR's Save_Game fails
  cleanly ("stg: ... no field maps"). With the port's own container (the test hook
  RA2_NATIVE_TEST_PORT_SAVE=1) YR's Put_All writes a 762 KB save on the Mac without a trap; loading
  it is untested. Test harness: RA2_NATIVE_SAVE_AT=<frame>[,<file>] in the YR build too
  (glue/yr_app_hooks.cpp).
* **Simulator route** `yr/tests/ios_sim_one_game.py` (ctest-style, "RA2 Duo Validation" only):
  the carry-over route (a Red Alert 2 import only -> an RA2 skirmish and save -> the toggle, YR's
  files added, a YR skirmish and save attempt -> YR chosen last -> RA2 -> YR again, a reset -> a YR
  skirmish -> RA2 loads its save) and `--fresh` (one import of both games, then the toggle).

### Saves in gamemd.exe's format (lane Y10, 2026-11-16)

Owner ruling (lane Y9's open item): Yuri's Revenge saves only in the original's format, gamemd.exe's, as
Red Alert 2 does game.exe's (native/README.md "Original saves"): a compound file whose CONTENTS stream
holds the i686 object images, pointers as 4-byte swizzle ids, vtables as gamemd.exe's.

* **Field maps.** `tools/original_layouts.py --game yr` (a build step: `gen-yr/original_layouts.cpp`,
  linked with `yr_data_obj`) lays out the YR units for i686-pc-windows-msvc and for arm64 and pairs them
  member by member, for the types in `yr/data/original_save_types.txt` (95 rows; each size is gamemd.exe
  1.001's: Fetch_Object_Size, vtable slot 12, a locomotor's slot 9, or the push before a sizeof read) and
  every record of the units that read streams: 3,000 maps, 0 problems. The i686 dump has no C++ library
  and no host headers: `yr/sdk/i686` stands in for the few C++ headers the tree includes, and the tool
  makes i686 preludes in the build (ra2port/prelude.hpp with RA2_LONG as long, the YR prelude without the
  host's C types and _WIN64, mingw's `__LONG32` as long). ctest `yr-native-original-layouts` checks it.
* **Vtables at run time.** There is no gamemd.exe manifest, so a vtable pointer becomes a VPTR_NAMED
  operation naming its class by its MSVC RTTI type descriptor (`.?AVUnitClass@@`,
  `.?AV?$DynamicVectorClass@PBVAircraftTypeClass@@@@`; the tool mangles the names, 853 of them); the port
  finds that class's vtable at that offset among the complete-object locators of the user's gamemd.exe
  (`original_vtable_named`, port/src/original_image.cpp: vtables in data sections whose first slot is
  code). Every type of the save list resolves in gamemd.exe 1.001.
* **The streams.** The Win64 headers' ISequentialStream gets the RA2 tree's typed Read/Write overloads
  (`tools/yr_sdk.py`, objidlbase.h/objidl.h, in an extern "C++" block).
* **Fixes in the YR units** (`fixups.json`, lane Y10): swizzle ids are intptr_t (the tree's LONG truncated
  addresses; the Win64 LONG_PTR is long long, which the typed reads would take as 8 bytes) and an object's
  own id is written as its address, a pointer, so it gets the id every pointer to it gets; the swizzle
  walk is bounded and reports unmatched fields as the RA2 tree's does; LZO1X-1's dictionary keeps 32-bit
  offsets (8-byte pointers ran 64 KB past CStreamClass's buffer: the CONTENTS stream would not
  decompress); LocomotionClass::Load reads its image through its type, not void *; HouseClass's starting
  units write their Type through the member. In the port a pointer written with VC6's size 4
  (LayerClass::Save's sizeof(int)) is written as its id. Draft: FUN_004E02C0 (the ILocomotion pointer's
  _Release, glue/foot_native.inc).
* **Loading for tests.** `--load-save=<file>` works in yr-native too (LoadOptionsClass::Load takes the
  host's resume request and runs the load dialog's steps with that row); `--run-to=frames --frames=N`
  ends N frames into the game (glue/yr_app_hooks.cpp).

ctest `yr-native-save` (yr/tests/yr_save_roundtrip.py): a skirmish saved at frame 500, loaded from the
menus and played 300 frames, saved again at frame 50 (a save written after a load keeps the ids) and that
loaded and played 300 frames; a save of the user's installed Yuri's Revenge is loaded too when the data
folder holds one (by its internal version, 178328; read in place, never copied). The data folder here
has none (its saves are Red Alert 2's), so that part is reported as skipped; the user can make one on
Windows. ctest `one-game-saves` / `one-exe-saves` (yr/tests/one_game_saves.py): in the one-game app each
game saves after the toggle (RA2 -> YR, YR -> RA2) and each save loads back in its game.

iOS: the one-game app's Simulator route (`yr/tests/ios_sim_one_game.py`, "RA2 Duo Validation") now
expects Yuri's Revenge's save in Documents/Game/YR after the toggle and loads it (step 3b): all checks
pass (the save at frame 900, the loaded game at 62 frames a second; the iOS maps: 2,999).

Not checked yet: the installed gamemd.exe loading a save the port wrote (needs Windows). A loaded game
redraws its command bar only once the mouse passes over it.

## Lane Y11 (2026-11-17): stability, the shell's backdrop, the toggle as a menu button

* **TechnoClass::Destroy** (ObjectClass vtable slot 0x170) is the tree's TechnoClass::FUN_00710460
  (gamemd.exe's TechnoClass, FootClass, BuildingClass, InfantryClass and AircraftClass vtables hold
  0x00710460 there; data/name_aliases.tsv). A unit dying while mind-controlling others reached the
  trap in `yr-native-skirmish-long` under ctest -j4 (496 s); the test passes under -j4 since.
* **NON-EXACT drafts** (each labelled in its file): TeamClass::AI (glue/team_native.inc, gamemd.exe
  0x006E9140: the RA2 twin's text, the script step through the tree's own TMission_* handlers by
  gamemd.exe's jump table), BulletClass::Unlimbo, AI and Shrapnel and Projectile_Motion
  (glue/bullet_native.inc: the RA2 twins and TibSun's text with gamemd.exe's YR changes). A 20-minute
  game against three hard opponents first reached TechnoClass::Fire_At (gamemd.exe 0x006FDD50, 7,167
  bytes) about 16 minutes in; it is still a trap (the RA2 twin, src/recovered/techno.cpp, is exact and
  the natural text for the draft).
* **The shell dialogs' backdrop** (user report 2026-11-17, the Duo: Options black, Enhancements noise,
  Set Game Resolution blank): FUN_0060CF00 keeps each dialog's backdrop art (a converter and two shapes)
  as ints at the record's 32-bit offsets 0x74/0xE0/0xE4, read back by FUN_0060C4E0/FUN_0060CE40/
  FUN_0060CEA0 and passed by OwnerDraw::Paint to FUN_0072E730 through an int declaration. Natively the
  pointers were cut to 32 bits (and the dialog id was read off the record start, not the Dialog view),
  so the art was drawn from garbage. fixups.json keeps them by window at pointer width. Every shell
  dialog now has its own backdrop (the skirmish setup its parachute picture, which had been the red
  map). ctest `one-game-menu-backdrop` (yr/tests/menu_backdrop.py) walks main menu -> Options ->
  Enhancements -> Options -> main menu in both games and checks the backdrop region.
* **Set Game Resolution**: Yuri's Revenge runs its shell at 800x600 and only the game at the host's
  play size, so the port's mode list (current mode plus the usual ones) lacked the play size; the host
  now gives it to the port (ra2port_set_play_mode) and the list carries it.
* **Focus loss in the menus**: the one-game app's YR engine had a link-closure trap for
  ra2port_game_redraw_pending and aborted when the app went to the background (the Duo's log);
  glue/yr_app_hooks.cpp answers it from SurfacesRestored as RA2's hook does.
* **Touch**: YR's own "NewEdit" text fields raise the on-screen keyboard (port: ra2port_text_input_wanted),
  and the game cursor is hidden on touch (glue/yr_app_hooks.cpp ra2_native_hide_game_cursor) as in RA2.
* **The game toggle** is a regular main-menu sidebar button in the slot above Exit Game in both games
  ("Play Red Alert 2" / "Play Yuri's Revenge"; port/src/user32.cpp place_toggle, the game-side hooks give
  it Exit Game's kind); the corner push button and its movie-frame redraw are gone. one-game-toggle
  still clicks it by id.
* Simulator walk: `yr/tests/ios_sim_menus.py` (not a ctest) runs both games' shell, a skirmish's setup,
  load and first play with the same settings on "RA2 Duo Validation" and checks the backdrops.

## Lane Y11b (2026-11-17): the loading screen, Fire_At, the menus' size

* **The loading screen** (glue/ldprgmgr_native.inc, NON-EXACT): the map preview (MapPreviewClass, the
  scenario's [Preview] or a random map's picture) and the texts: the country's name on a dimmed strip,
  its special unit in black on the title band, its briefing (lower in a cooperative game) and Loading,
  from Red Alert 2's loading-screen text with YR's places and tenth country (gamemd.exe
  0x005536A0-0x005540E7). The campaign screen is still not drawn.
* **NON-EXACT drafts**: TechnoClass::Fire_At (glue/fire_at_native.inc, gamemd.exe 0x006FDD50: Red
  Alert 2's text with YR's drain weapon, spray attack, area fire, disk laser, damage multipliers,
  occupant cycling, berserk rearm and magnetron; the railgun's beam damage, gamemd.exe 0x0070C690, left
  out), BuildingClass::Drop_Debris (glue/building_native.inc, 0x00442D90: TibSun's text with YR's
  release of absorbed units) and ParticleClass::Spark_Behavior_AI (glue/particle_native.inc,
  0x0062C6E0). Two 20-minute games (three hard opponents on map 30, one on The Alamo) reached no trap.
* **The menus' size** (user rule 2026-11-17: the original by default, filling as an enhancement):
  Yuri's Revenge runs its menus in a separate display mode, GameOptionsClass::ShellWidth x ShellHeight
  (800x600 from its constructor, read from no INI; gamemd.exe WinMain 0x006BDB7E-0x006BDB90 passes
  Options+0x2C/+0x30 to Set_Video_Mode, and ScenarioClass returns to it after a game, ra2-decomp
  src/scenario.cpp:605), so its menu fills the screen; Red Alert 2 runs its menus at the game
  resolution with the 800x600 menu centred (game.exe 0x005115F0). Both stay so by default. Menu fill's
  new choice Scale to fit shows the 800x600 menu as large as the screen holds in both games (Red Alert
  2's then fills as Yuri's Revenge's does; off in the Original preset; the Enhanced preset keeps Blurred
  backdrop). one-game-menu-backdrop checks both looks at 1024x768.
* More traps off the play paths: HouseClass::Is_Build_Limited (a second unit queued in a factory),
  MindControlBuildings and RelinquishBuildings are the tree's FUN_0050B370, FUN_0050D290 and
  FUN_0050D2D0 (data/name_aliases.tsv, from gamemd.exe's callers); the TibSun-text drafts called
  TibSun's Speak, Sound_Effect, Harvested and Remove_Target, which YR does not have: they call
  VoxClass::Play, VocClass::PlayAt/PlayGlobal, Give_Tiberium and AbstractClass::BecomeUntargetable as
  gamemd.exe does there (selling, repairing bridges, walls and bridges destroyed, barrels); NON-EXACT
  BuildingClass::FUN_00457DE0 (gamemd.exe 0x00457DE0: a garrisoned building lets its occupants out).
  Link closure 236 (lane Y11 start) -> 219.
* Fix on main: lane CB s2's native rekey had renamed the YR glue's DisplayClass::Set_Cursor_Shape to
  RA2's Set_Copy_Cursor_Shape (yr-native did not compile).

## Lane Y12 (2026-11-17): combat, long games, both campaigns to their endings

* **Test harness** (`glue/yr_harness.cpp`, test scaffolding as RA2's link/harness_orders.cpp): RA2_NATIVE_ORDERS
  (`list`, `health`, `shoot <ID|*>`, `watch`, `hunt`) and RA2_NATIVE_FORCE_END (`<f1>,<f2>,...[,lose]`: PlayerWins /
  PlayerLoses at those loop frames), called from yr_native_main_loop.
* **ctest `yr-native-combat`** (yr/tests/yr_combat.py): a Grizzly force-fires on another of the player's (the order a
  force-fire click queues): TechnoClass::Fire_At runs, the target's strength drops and it dies (about frame 1,300).
* **ctests `yr-native-campaign-allied` / `-soviet`** (yr/tests/yr_campaign_chain.py, as RA2's campaign_chain.py):
  Single Player, New Campaign, the side's emblem, seven missions won in turn through their score screens, the final
  movie, the credits and the main menu. Both pass (ALL01UMD..ALL07SMD and SOV01UMD..SOV07TMD: all 14 missions start and
  end, both endings, the credits' 565 lines, menu-return; about 6 minutes a side).
* **The campaign loading screen** (glue/ldprgmgr_native.inc Draw_Campaign, gamemd.exe 0x00552E65-0x005532FD): the
  side's title band, the scenario's LS640/LS800 picture with its palette, the load bar, the mission name and the
  briefing box. The load bar's drawer was cut to 32 bits on its way to Draw_Bar (scenario.cpp declared FUN_0072B380
  as returning int; ProgressScreenClass Field74 and Set_Graphic_Data's argument were int): fixups.json.
* **NON-EXACT drafts** (each labelled in its file), in the order the runs reached them: the spotlight's brightening
  blitters (glue/yr_spotlight.cpp, gamemd.exe 0x007DEEFA-0x007DF640, hand-written assembly the tree declares);
  the railgun's beam damage (TechnoClass::Railgun_Beam_Damage, 0x0070C690, called from Fire_At); the credits
  (Show_Who_Was_Responsible, 0x004C3E30, glue/egos_native.inc: RA2's draft with CREDITSMD.TXT); OwnerDraw::ListBoxHandler
  (0x00618D40, glue/ownrdraw_list_native.inc: the campaign dialog's list; columns and cells not yet);
  BuildingClass::Captured (0x00448260), BuildingClass::Fire_Direction (0x0044D7D0), the rocking from an impact
  (TechnoClass vtable slot 0x3D8, 0x0070B280), MapClass::Reveal (0x00577D90); the ship locomotor (glue/ship_native.inc:
  gamemd.exe's ShipLocomotionClass is a clone of the drive locomotor, Draw_Matrix compared instruction by instruction,
  so the drive drafts' text under the ship's name); the hover locomotor's Process, While_Moving and Start_Of_Move
  (TibSun's text, glue/hover_native.inc, not yet compared with gamemd.exe); off the chains' paths but on the campaign's
  play paths: TActionClass::TAction_IRON_CURTAIN (0x006E36E0) and the voxel debris' VoxelAnimClass constructor
  (0x007493B0, TibSun's text with gamemd.exe's audio and particle changes).
* **Bindings** (data/name_aliases.tsv): CellClass::On_Ground = Is_Tile_Not_Water (0x004867E0), FUN_004C3E30 =
  Show_Who_Was_Responsible, Submit_Radar_Event = RadarEventClass::Create (0x0065FA70), FUN_00622B50 =
  UI::StandardWndProc, FUN_005BF390 = Play_Ingame_Movie, FUN_0041C610 = IndexClass<int, AbstractClass *>::search_compfunc.
  The rocket locomotor's Save, Do_Turn and Mark_All_Occupation_Bits declarations go (fixups.json): gamemd.exe's
  vtables hold LocomotionClass's.
* **Modern controls on touch**: RA2's tap across player-controlled houses (lane C18) carried over
  (glue/yr_enhancements.cpp Tap_Selects_Across_Houses); the rest was wired by lane Y8.
* **Simulator** ("RA2 Duo Validation", RA2YR.app, `--game=yr`, yr/tests/ios_sim_skirmish.py): 12 starts of a skirmish
  (30 s of play each) all reached the time limit without a trap or a fault (the one start crash in 10 seen by lane Y7
  did not recur); a 90 s run with `controls=modern` passed, its two-finger tap giving Modern's order.
* **Long game**: 30 minutes against three hard opponents on map 30 (Mac, `RA2_NATIVE_TRAP_CONTINUE=1`): no trap, no
  fault.
* Link closure 219 (lane Y11b) -> 186. Of the 186: Westwood Online and internet 36, LAN / multiplayer / coop 42,
  the random map generator 11, TibSun leftovers YR does not use (veins, fences, tunnels, ion storms, VQA) 40, shell
  and Win32 16, Speech_Name (the trigger INI writer) 1, play paths 40 (planning mode, the rocket's Process, radiation beams, gas particles, hijackers, spies,
  psychic dominator AI) (lane Y12 trace, by caller).
* **Open**: an LTNK (Allied 2, Soviet 7) gets a garbage position from its drive locomotor; MapClass::Place_Down and
  Pick_Up now log a cell off the cell array and leave it alone (`place-down:` / `pick-up:` lines, five each) instead
  of faulting. The Mac window dumps of a skirmish show a black tactical view with only health bars and the
  `Primary` tag (also on main, lane N1's build): not investigated. The campaign dialog's Difficulty label reads
  `MISSING:''` on its right end. The rocket locomotor's Process (0x006622C0) is still a trap (not reached by the chains).

## Next

0. The Windows builds on a real PC (RA2YRNative.exe with its two DLLs and SDL3.dll is on the user's PC, lane P1;
   YRNative.exe alone is not); the stand-ins not yet seen at work (bridge repair; the War Factory's unload through
   the harness; the hijacker is in yr-native-standins since lane P1); the -MPDEBUG sync
   record's object lists.

1. The buttons' targets (input not yet exercised; the runs are unattended): each submenu
   will stop at its unmatched handlers (ListBoxHandler 14.7 KB, ComboBoxHandler 6.9 KB,
   ComboDropWindowHandler 7.6 KB, SysTabHandler, NewEditHandler, PrepareDialogChildControls,
   UI::RegisterWindow; RA2 counterparts in native/link/behaviour_m14_ownerdraw.cpp).
2. The owner-draw unit's other 32-bit pointer assumptions: globals declared `int` that hold
   pointers (FUN_00603870's shapes, DAT_00833674), `(char *)p + 0x74`-style raw offsets on other
   variables, union views with pointer members read at the same offsets.
3. Init_Game's CD search when no search path is set (not drafted: the host always passes -CD.).
4. Variant* and the other Win32 imports the port does not yet supply (the run lists them).
5. (lane Y10) Yuri's Revenge's saves: the installed gamemd.exe on Windows loading a save the port wrote,
   and the port loading one the installed game wrote (yr-native-save picks it up from the data folder).
6. (lane Y8) Map zoom, Zoom gestures, Game interface size and HD vehicles for Yuri's Revenge: a YR
   copy of each native/link module and its fixups in the YR units (lane Y8 section: why).
