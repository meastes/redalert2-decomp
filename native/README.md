# Native tree (arm64 macOS)

The shipped game: recovered C++ from `src/recovered` and `src/shared`, compiled
by modern clang for arm64 macOS, on an SDL3 platform layer. No Wine, Rosetta,
emulation, original code or original bytes. Game data is read from the user's
installation at run time (`--data-dir` or `RA2_DATA_DIR`) and never stored here.
This project is separate from the root `CMakeLists.txt` (the exact original-target
build) and never changes `src/`, the catalog or `tools/original_target`.

```
cmake -S native -B build/native -G Ninja && cmake --build build/native
cmake -S native -B build/native -DRA2_NATIVE_SUBSYSTEM=all   # whole tree (3,279 groups) + drafts + ra2-native game link
cmake -S native -B build/native -DRA2_NATIVE_SUBSYSTEM=all -DRA2_NATIVE_LINK_CLOSURE=ON   # link with traps; gen/link_closure.json
ctest --test-dir build/native            # native-core-check, native-asm-check, native-view-audit
build/native/ra2-native --smoke-test     # SDL3 host window
build/native/ra2-native --data-dir <installation> --run-to=menu   # start-up test run (below)
```

**AddressSanitizer runs.** Configure a separate build with
`-DCMAKE_C_FLAGS="-O1 -g -fsanitize=address -fsanitize-recover=address -fno-omit-frame-pointer"`
(same for `CMAKE_CXX_FLAGS`, plus `-DCMAKE_EXE_LINKER_FLAGS=-fsanitize=address`);
`link_closure.py` treats the sanitizer runtime's exports as system symbols. Run
with `ASAN_OPTIONS=detect_leaks=0:halt_on_error=0:symbolize=0` (in-process
symbolisation hangs under the SDL host) and symbolise the `ra2-native:arm64+0x...`
offsets afterwards with `atos -o ra2-native -l 0x100000000` (the printed offset
already includes the 0x100000000 base). Under the SDL host `lldb` hangs at `run`;
use the ASan build, `RA2_NATIVE_CRASH_TRACE=1`, the macOS crash report
(`~/Library/Logs/DiagnosticReports/ra2-native-*.ips`, for SIGTRAP/heap aborts the
host does not catch) or temporary `dprintf(2, ...)` in a scratch worktree (sources
are copied into `gen/units` at configure time: reconfigure after editing them).

Full-tree smoke test: all 3,610 original static initialisers run (in `_initterm`
order), main is reached and prints `initterm: main reached ...`, and exit-time
destructors complete. Diagnostics: `RA2_NATIVE_INIT_TRACE=1` prints each unit's
first original initterm index as it starts, `RA2_NATIVE_CRASH_TRACE=1` prints the
last index and a backtrace on a fault, `RA2_NATIVE_TRAP_CONTINUE=1` lists every
link-closure trap on the path instead of stopping at the first,
`RA2_NATIVE_PRESENT_DUMP=<file.bmp>` writes every presented 16-bit primary frame
there as a 24-bit BMP (the file holds the last one; test runs only).

## Start-up test runs

`ra2-native --run-to=<milestone> [--run-seconds=N] [--write-dir=D] --data-dir <installation>`
starts the game (WinMain, `native/drafts.json` 0x68E280) and logs each start-up
milestone to stderr as the port sees it: `main`, `winmain`, `ini` (RA2.INI opened),
`mix` (first MIX), `strings` (ra2.csf as a loose file; from a MIX it is implied by
`window`), `window`, `ddraw`, `primary` (first DirectDraw primary surface),
`surfaces` (off-screen surfaces after it), `dialog` (first dialog requested),
`present` (first frame presented), `menu` (the first dialog built from a game.exe
template is shown: Select_Game's main menu on the start-up path; the detail names
the template). Dialog templates (RT_DIALOG, DLGTEMPLATE and DLGTEMPLATEEX) and
FindResourceA/LoadResource/LockResource read the supported game.exe's resource
directory at run time (`ra2port::original_resource`); CreateDialog*/DialogBox*
build the dialog and its controls as port windows and send WM_INITDIALOG; a modal
box pumps messages until EndDialog. The
run ends by itself at the requested milestone (exit 0), the first link-closure
trap (3), a fault (4, with a backtrace), WinMain returning, or the time limit
(default 5 s; exit 2, with the main thread's backtrace), and prints a summary:
milestones with times, files opened/written, windows, surfaces, dialogs, message
boxes, unported imports and traps. The installation is never written: every
create/write/delete goes to `--write-dir` (default a fresh `$TMPDIR/ra2-native-run-*`)
and reads look there first. Message boxes print to stderr; no modal UI is shown.
The host plays the launcher's part (the launch mutex and event RA2.EXE created).

### Skirmish harness

`ra2-native --skirmish=<map> [--skirmish-frames=N] --run-to=<milestone> --game-args="-noaudio -CD." --data-dir <installation>`
exercises the skirmish path before the menu dialogs exist (a test harness, not
game behaviour; `link/harness_skirmish.cpp`). Main_Game calls Select_Game and
Main_Loop through native seams (`port/fixups.json`) that call them unchanged
without `--skirmish`. With it, the seam does what the skirmish dialog's start
button stores (Skirmish_Command 0x006829F0: session type 5, the rules'
countries/sides/house types, one human and one computer player, the saved
options; `<map>` is `#<n>` for the n-th scenario description, a description's
file name, or any file name) and calls Select_Game, which skips its menu loop
through a seam (`Native_Skirmish_Menu_Bypass`) and starts the scenario. It must
not set `Debug_Map` (0x00A40D13, Westwood's scenario editor
flag), under which the multiplayer setup 0x0065E8C0 makes no start units and the
human house is defeated at once (Scout1091). The cursor is parked in the
tactical area (the host's (0, 0) edge-scrolls the map every frame). Bootstrap's interface-art load runs: its 19 file names are read from game.exe (data/original_data.cpp; the seam skips it only while they are null).
Milestones: `init` (Init_Game succeeded), `scenario` (Start_Scenario loaded the
map), `frame` (first Main_Loop iteration), `frames` (N of them); notes
`harness-art`, `harness-map`, `harness-stop`. With `--skirmish='#0'` (MP02T2.MAP) the
run reaches `scenario`, `frame` and `frames` (60 checked) without traps; the human
house gets its start units (MCV, tanks, infantry) and is not defeated. The
tactical view draws the map tiles under the shroud, the start units (voxel
vehicles, infantry), trees and cliffs around the base, and the sidebar credits
count up (Scout1098; best with `'#5'`, MP14T2.MAP).
`--skirmish-order=<kind>[@<frame>]` (`link/harness_orders.cpp`, test scaffolding)
queues the orders a player's click would, as MEGAMISSION events
(Queue_Mission 0x006233A0), for the human player's start units at that frame
(default 10): `move` (infantry and vehicles 6 cells east), `deploy` (the MCV
unloads), `both`; the last frame logs `harness-units` (kind:mission@cell of the
player's technos). With `both`, `'#0'` and `'#5'` run 600 frames without traps:
infantry walk (Walk 0x0071E220 draft), tanks drive (path search 0x00427370
draft), the MCV deploys into a construction yard (Scout1103). Longer runs need
`--run-seconds=300` or more (about 55 s per 1,000 frames). Scout1108 (M11): the
human house was blown up at frame 1,024 by the launcher check (Main_Loop_End
0x0053F840 queues a DESTRUCT event every 1,024 frames unless Validate_Launcher
0x00491A10 saw the launcher's message; the host now plays that handshake), and
BuildingClass slot 52's in-file cell view wrote CellClass +0x122 at its VC6
offset, inside the arm64 AltOccupierPtr (Evaluate_Cell 0x006C6130 then faulted
on 0x0000000000FF0000); `'#0'` and `'#5'` run 3,000 frames with `both`.
Scout1111 (M12): `build` (the MCV deploys, then `link/harness_build.cpp`'s build order)
and `all` (`both` and `build`): at each step the harness queues the events a click
would, PRODUCE (0x0E, BuildingTypeClass kind 7, the type's index) as the sidebar's
cameo click does and PLACE (0x0B with a cell) once the factory holds the finished
building, trying cells around the construction yard; the order is a power plant
(GAPOWR/NAPOWR) and a refinery (GAREFN/NAREFN), which brings its free harvester.
Notes `harness-build-<n>` per step and `harness-houses` (buildings and units per
house) at the last frame. The sidebar shows the cameos (the build options were
dropped by Who_Can_Build_Me's padded views and the cameo pointer by Get_Cameo_Data's
int return).
Scout1115 (M13): the placement cell is the nearest one to the yard that the
original's own tests accept (DisplayClass::Passes_Proximity_Check 0x004991D0 and
CellClass::Is_Clear_To_Build 0x004718B0 for each cell of the type's occupy list);
the power plant and the refinery stand at the first try.
`RA2_NATIVE_DEBUG_LOG=1`
prints the game's Debug_Printf lines (the original's body is empty). The menu run
(`--run-to=menu`) and the skirmish runs also work without `-noaudio` (60 frames
checked on `'#0'` and `'#5'`). `--game-args` is WinMain's command
line: `-noaudio` skips the audio device, `-CD.` makes every file local (no CD
check; Parse_Command_Line's `-CD<path>`). The native game skips the CD check
(user decision 2026-11-08): the host adds `-CD.` unless the command line names
`-CD<path>` itself or `--cd-check` asks for the original check. Test runs play audio into SDL's dummy
device (silent, consumed in real time) unless `SDL_AUDIO_DRIVER` names one.

### Playing (interactive skirmish) and scripted input

`ra2-native --play=<map> --data-dir <installation>` starts a skirmish on `<map>`
(as `--skirmish` names it: the harness's start without the menu dialogs, Russians
as the computer player) with no frame limit and no scripted orders: the player's
mouse and keyboard drive the game until the window is closed (Scout1115, M13).
The SDL host turns input into the Win32 messages the game's window procedure
reads (Windows_Procedure 0x007375A0: ScrollClass 0x0066A400 for the map,
WWKeyboardClass::Message_Handler 0x00532A60 for the gadgets and hotkeys): left
and right buttons, motion, the wheel (WM_MOUSEWHEEL scrolls the sidebar) and
keys, with mouse positions converted from the window to the game's pixels (the
presented surface letterboxed into the window, which takes the game's
resolution). DirectDraw's SetDisplayMode makes the screen that size and the
main window cover it (WM_MOVE/WM_SIZE), as on Windows: the mouse's confining
rectangle (WWMouseClass::Calc_Confining_Rect 0x00771C50) was the 800x600 window
before, which clamped every gadget click (the sidebar) to x < 800.
Select (click, drag a band), move/attack (click a target), deploy (click a
selected MCV), build (click a sidebar cameo; click it again when ready, then a
map cell) and the hotkeys (S stop, G guard, X scatter, D deploy, T type select,
team keys, the sidebar scroll commands) work.

`--input-script=<file>` (or `RA2_NATIVE_INPUT_SCRIPT`; any skirmish run) plays
scripted input through the same SDL event path, one event per line
`<frame> <action> <args>` at the start of that Main_Loop frame: `move P`,
`ldown P`, `lup P`, `rdown P`, `rup P`, `click P` (move, then down and up on the
next two frames), `rclick P`, `drag P0 P1`, `key NAME` / `keydown NAME` /
`keyup NAME` (SDL key names). A point P is `X Y` in game pixels or
`@ID[:n][+dx,dy]`: the human player's n-th object of type ID, moved by dx, dy
cells (`link/harness_input.cpp` answers its screen pixel). The last frame logs
`harness-input` (the selection count and each of the player's objects as
ID[*selected]:mission@cell); `RA2_NATIVE_INPUT_LOG=<n>` also every n frames.
Example (`'#5'`, 900 frames, no order): click `@MTNK`, click `@MTNK+5,0` (it
drives there), `key S`, double-click `@AMCV` (it deploys), click the power plant
cameo (908, 252), click it again at frame 720 and click `@GACNST+4,0`: the
power plant stands at the clicked cell.

### Menus (Scout1119/Scout1121, M14)

Without `--skirmish`/`--play` the game runs its own menus. The port's USER32
(`port/src/user32.cpp`) now gives the owner-draw dialog layer what it needs:
GWL_WNDPROC subclassing and GWL_USERDATA pointer-sized
(`ra2port_set/get_window_ptr`, native_units.py rules), Win32 control procedures
for the predefined classes (Button click to WM_COMMAND/BN_CLICKED, check state,
ListBox/ComboBox items and selection, Edit text, trackbar/progress/tab/hotkey
state, WM_GETTEXT/WM_SETTEXT), the dialog manager procedure under a subclass,
WM_SHOWWINDOW, WM_PAINT generated for dirty dialogs and controls after posted
input and before WM_TIMER, screen coordinates for child windows, and mouse input
routed to the control under the cursor of the topmost shown dialog (template
order is the Z order; static controls without SS_NOTIFY and group boxes are
transparent) or to the capture. The owner-draw procedures with no recovered body
are behaviour rows (`link/behaviour_m14_ownerdraw.cpp`; input exact to the
decompiles, painting simplified). Scripted input reaches the menus: a script
line's first field `t<ms>` (milliseconds after the window opened) or
`d<k>+<ms>` (after the k-th dialog was shown) instead of a frame number; timed
lines play in the order written. `RA2_NATIVE_MSG_LOG=1` logs messages to dialogs
and controls, each shown dialog's controls (id, class, screen rectangle, text)
and every routed click (`=2` adds paint, timer and motion). `--frames=N` reports
`frames` after N Main_Loop frames of a game started from the menus; the
milestone `menu-return` is the main menu shown again after a game. Scout1121 (M14b): the
dialogs draw (their background art, the sidebar buttons with their labels, the
skirmish dialog's controls) and their controls sit where the original's
placement puts them (main menu buttons on the sidebar slots at x 756). Four
native causes: GetWindowLongA(GWL_HWNDPARENT) answered 0 (the layout pass
0x00735BC0 then used an unset RECT); UnknownControlRecord's first 16 bytes are
an int and two HWNDs in the combo drop-list unit only (24 bytes on arm64), so
the dialog kind 0x005EAAA0 stored read back wrong elsewhere (fixup pads the
other variants); a PeekMessage without PM_REMOVE cleared a dialog's pending
WM_PAINT, so the owner-draw paint (0x005EE460 draft; its slide-in state read at
the VC6 record offset +0x1FC, now RA2_REC_TAIL) never started; the background
art pointers (+0x74, +0xE0, +0xE4) were kept in ints. A drop-down combo box is
its selection field (the template height is the dropped height,
CB_GETDROPPEDCONTROLRECT; CB_SETITEMHEIGHT(-1) sizes it); the owner-draw combo
procedure (behaviour row 0x005F4BA0) keeps the original's item records and
opens the recovered drop list 0x005EAD20 ("ComboDropWin") on a click on its
arrow; the drop list holds the capture and a click picks the item
(CBN_SELCHANGE). Keys go to the focus window when it belongs to a shown dialog
(arrow keys move a combo's selection). WM_CREATE carries a CREATESTRUCT.
Example (`'#0'`'s map is the skirmish default, Isle of War): main menu Single
Player `d1+1500 click 834 304`, Skirmish `d2+1500 click 834 388`, the first AI
slot's arrow `d3+2500 click 385 197`, Medium Enemy `d3+3500 click 300 251`, Start
Game `d3+4500 click 834 346`, then `--frames=N`. `--skirmish-order` also works
in a game started from the menus; `hunt` (the MCV deploys, every other unit
hunts) and `rush` (the MCV drives to the enemy, the others hunt) are scripted
aggression for the menu-to-score loop.

### Win, score screen, save game (Scout1125, M15)

`--skirmish-order=win` (`link/harness_army.cpp`, test scaffolding): the MCV
deploys, harness_build.cpp's long build order (power plant, refinery, barracks,
war factory, second refinery, second power plant), then one tank (Grizzly or
Rhino) at a time from the war factory (PRODUCE kind 0x28) and, once twelve
stand (or at `RA2_NATIVE_ARMY_ATTACK`, default frame 9,000), every combat unit
hunts; later tanks join. Notes `harness-army-<n>` and `harness-houses-<k>k`
(buildings, units, the player's money every 2,000 frames). Every game logs
`frames-<k>k` (game frame per 1,000 loop frames) and `game-over` (win, lose,
restart, abort) as Main_Loop saw it: Main_Loop returns at once while a
player_wins/loses flag is set (no Frame++, no Sync_Delay), which is the "frames
run 50x faster" Scout1121 saw; Main_Loop_End then runs Do_Win/Do_Lose and the
score dialog (MP_Score_Dialog_Proc 0x005AAE40, modal). The default skirmish map,
Isle of War, has no land route between the bases (Regular findpath failure);
a menu run picks El Dorado in Customize Battle (Choose Map, M16). The real
loop (menus, Medium Enemy, win at frame ~25,000, the score screen, Continue,
Back, Back, `menu-return`) takes about 8 minutes.
`RA2_NATIVE_FORCE_END=<frame>[,lose]` sets the win (or loss) flag at that frame
(test scaffolding for the score screens). Native causes fixed on the way: the
lparam `(long)` casts of window messages truncated pointers (native_units.py
rule: `(LPARAM)`), the combo box never cleared its item colours (OD_SUBCLASSED,
so the drop list drew its rows in black), the depth read/write darken and
translucent blitters and a few leaves were unrecovered.

Save game: `RA2_NATIVE_SAVE_AT=<frame>[,<file>]` (`link/harness_save.cpp`) calls
Save_Game 0x006554A0 as the options menu's Save ends in; with
`RA2_NATIVE_SAVE_LEAVE=1` the game is then left as the menu's Abort does. Ole32
structured storage is ported (`port/src/storage.cpp`: IStorage, IStream,
IPropertySetStorage, OleSaveToStream/OleLoadFromStream; one host file per
storage in the port's own container format, `RA2NSTG1`; `RA2_NATIVE_STG_LOG=1`
logs it). LZO1X's dictionary keeps 32-bit offsets natively (its 64 KB work
memory held 16,384 VC6 pointers), swizzle IDs are pointer-wide (the saved
object address is 8 bytes natively), FindFirstFileA also lists the write
directory (saved games), and the load dialog's list shows the saves' descriptions.
Load from the menu (Single Player, Load Saved Game, the save, Load) reads the
whole game back and plays on (M16, below).

### Load, Choose Map, campaign start (Scout1133, M16)

Load: SwizzleManagerClass::Process_Tables 0x0069FC30 spun for ever because its
deliberate 1 / 0 for an unmatched pointer does not trap on arm64. A fixup bounds
the walk and reports each unmatched field (`swizzle: unmatched id ... field ...`
with the nearest registered objects' vtables; `RA2_NATIVE_SWIZZLE_TRACE=1` adds
the Load that asked) and then faults as the original
(`RA2_NATIVE_SWIZZLE_CONTINUE=1` nulls them instead). The cause: save/load
streams wrote and read pointers with a literal 4 (RadioClass contacts,
FactoryClass queue, StormOwner); native_units.py rewrites `Read/Write(&x, 4, ...)`
to `ra2port::stream_width(x)` (8 for a pointer, else 4). The scenario's NoInit
constructor 0x0065B780 is a full behaviour row (generator, countdown, the three
TypeLists, the mission timer text), so a save loads in a fresh process: save at
frame 1,500 in one run, load from the menu in another, 1,000 more frames.
Choose Map: Lobby_Dialog_Proc 0x005C69E0 is a draft (93.62); the mode list
called through two placeholder vtables (MultiplayerManBattle,
MultiplayerObserverTeam: their key functions are behaviour rows). Campaign: Single
Player, New Campaign, the Allied emblem starts Allied 1 (New York). Movies run
through a native binkw32 without a decoder (`port/src/bink.cpp`: BinkOpen reads
the header from a file or the mix file handle, frames advance one per decode
pass, nothing is drawn), so every movie plays out at once with no picture. Native
causes on the way: the loading screen kept its message texts in ints
(Unknown535910 +0x38/+0x3C), ILocomotion's by-value matrix slots
(RA2_LOCOMOTION_COM_VALUE_TYPES, AircraftClass::Draw) pass the hidden return
pointer explicitly natively (arm64 returns large structs through x8), and the
bridge trigger's Damage_High_Bridge 0x559000 and ShipLocomotionClass::Draw_Matrix
0x676010 are drafts.

### Campaign view, map preview, movie timing (Scout1138, M17)

Allied 1's tactical view drew black terrain wherever a reveal queued cells for
redraw (the upper-right rectangle the view then scrolled around): the cell
overload `Tactical::Draw_Tiles(Cell, Rect)` 0x006A8560 was linked as an alias
of the area overload 0x006A7BA0, because `tools/address_aliases.py` bound names
by qualified name only and the catalog gives `Tactical::Draw_Tiles` both
addresses. The tool now refuses a same-named defined overload with other
parameters when the name has several addresses, and 0x006A8560 is a draft
(Scout876, 63.04). A pointer cast of an int cast, `(T *)(int)x`, keeps the
pointer whole (native_units.py rule; TechnoClass::unknown_slot27 0x006CCCE0
faulted when a selected unit's cursor was asked over a cell). The port's
DirectDraw Blt stretches when the destination and source rectangles differ, so
the Choose Map / skirmish map preview (MapPreviewClass 0x0061D460) is scaled
into its frame. MSFrameAnim and WorldDominationTour::MapSizeGameOption have
their key functions as behaviour rows (`link/behaviour_m17.cpp`), so their
vtables are real. Movies: BinkWait answers from a clock at the header's frame
rate (BinkPause stops it) and BinkCopyToBuffer writes black, so each movie
shows black for its own length (no decoder yet); `RA2_NATIVE_MOVIE_SKIP=1`
keeps the M16 pace for test runs. `RA2_NATIVE_PRESENT_DUMP_EVERY=<n>` also keeps
every n-th presented frame as `<file>.<count>.bmp`.

### Allied 1 objective, water, movie sound (Scout1142, M18)

Allied 1's first objective (destroy the four Dreadnoughts) is reached: Tanya swims
to the ships and sinks them. Three native causes kept her on land. A cell view's
`bool` stood over CellClass's ramp byte (+0x11C, techno.hpp's UnknownCellClass):
VC6 tests the whole byte, clang takes a bool's byte as 0 or 1 (`!b` is `b ^ 1`),
so FootClass::Can_Reach 0x004C8730 read ramp type 2 as "no ramp" and refused every
slope (Tanya's island search closed after 159 nodes). `view_audit.py` now lists a
view `bool` over a real one-byte non-bool member (`bools`; `--write` records them
as `bytes` in `port/view_layouts.json`, a missing row fails the test) and
`native_units.py` declares those members `unsigned char` (4 members: the cell's
ramp in two cell views, BulletTypeClass +0x2C4/+0x2C5). Land_Type 0x00529300 (and
the extent 0x0052DFA0) read the tile type's image through an unowned view that
lists the VC6 vtable as 40 plain virtuals; natively ObjectTypeClass slot 39 is
entry 45, so the view called another function, got no image and answered land
type 0 (clear) for every tile: no cell was water, ships could not move and
amphibious paths never entered water. A fixup calls the real virtual
(`link/behaviour_m18.cpp`) and the views' record slots are `ra2port::IsoTileRef`.
With water, Tanya's weapon choice against ships (NavalTargeting 4: the
secondary) works, so the attack cursor and order follow.
Movies: `port/src/bink.cpp` reads the whole Bink file (container, frame index,
audio tracks) and decodes the first audio track (clean-room, from the format's
public description: Bark band quantizers, run-length coefficient widths, inverse
real DFT or DCT-III, 1/16 overlap) into a port mixer voice
(`ra2port::audio_stream_*`, `ddraw_dsound.cpp`) when the game set a sound system;
video is still black (no decoder). `RA2_NATIVE_BINK_WAV=<file>` writes the last
movie's decoded sound as a WAV.

### Data the original stores (game.exe at run time)

User decision (2026-11-08, AGENTS.md "Native phase"): file names, tables and
constants the original keeps in its .data/.rdata are read from the user's
installed `game.exe` at run time, never copied here. `tools/original_data.py`
writes `data/original_data.cpp` (names, addresses, extents and shapes only) from
`data/unsourced_initialised.tsv` and the globals' declarations in `src/`: one
storage per original address, shape `bytes` (scalars and scalar arrays: copied),
`ptr`/`ptrs` (data pointers: the native storage of the global pointed at (a `ptr`
place covers its one pointer, a `ptrs` place its declared elements, pointer slots
scaled to 8 bytes; the TSV extent runs on into the literals that follow), from
this file and `bss_globals.cpp`, else the mapped image's initialised data, else
null). `port/src/original_image.cpp` maps the executable's sections (after
checking size and SHA-256 against the pinned target) and fills them before
WinMain (milestone `original-data`; static initialisers run before main and see
zeros). `RA2_GAME_EXE=<path>` names the supported executable when the
installation carries another build. Structs of scalars and fixed scalar arrays
only (TheaterTypeStruct, the track-control tables, GUIDs) load as `bytes`: VC6
(/Zp8) and arm64 lay them out alike; such a struct may be defined in the user
source itself (one body across `src/recovered`), and multi-dimensional arrays
count every dimension (Scout1098: the voxel view table, the infantry spot
sequences, the path and quadrant tables). Unsized arrays of records of data
pointers and 32-bit scalars take their count from the TSV extent when it is whole
records (Scout1103: DriveRawTracks/ShipRawTracks, the track point pointers).
File statics are copied the same way (Scout1115): Tactical.CPP's
`unknown_scale_abe490` (0x00ABE490, pixels per lepton of height, a dynamic
initialiser) is read elsewhere as the extern `unknown_abe490`; zero there, every
object's Z adjust (Unknown_006A2970) lost its height, the depth test hid units
and infantry on raised ground and drew vehicles as two strips (the "doubled
Grizzly").
Header statics (a shared header's file statics, one copy per original unit at
its own address, e.g. unknown_geometry.hpp's `unknown_geometry_j` at 0x00AD057C)
are copied into the zeroed storage of the extern names other units read at the
same address (`unknown_bridge_height_ad057c`) right after the header-statics
row's own initialisers (native_units.py `header_static_copies`, Scout1103).
Other class-typed globals and pointers to pointers are not loaded (`--report`).

## Layers

1. **Native view** (`tools/native_units.py`, run at configure time). Reads the
   catalog (read-only, via `tools/original_target/catalog_io.py`) and writes,
   under the build directory, one translation unit per catalog compile group in
   source order (the exact build concatenates a group the same way, and later
   files rely on earlier ones), plus a port-rewritten copy of every `src/shared`
   header. Rewrites are mechanical and keep VC6 x86 meaning on LP64:
   `long` to a 32-bit `RA2_LONG`, `L` literal suffixes dropped, `operator new(unsigned int`
   to `size_t`, 32-bit layout size checks disabled, VC6 for-init scope hoisted,
   VC6 wide CRT calls renamed `ra2_*`, opaque enums given VC6's fixed `int`
   (`enum X;` and its definition become `enum X : int`), reference casts of
   rvalues wrapped in `ra2port::as_lvalue` (VC6 reinterprets a temporary).
   Pointers carried in VC6 ints get pointer width in the native view by named rules
   (native_units.py: `new <Name>Class(` allocates twice the size a light stand-in
   sees, Bit_Blit/RLE_Blit's ring locals, the ZBuffer/ABuffer Wrap_ helpers, the
   RadiusCount table; Scout1091; the drawer value slot 115
   hands to building and overlay animations, the audio event's waiting source and
   sample data addresses; Scout1098; the click orders' pointer arguments of
   ObjectClass slots 77/78 and TechnoClass slot 200, Scout1115). VC6 variadic forwarding
   (`vswprintf(buf, fmt, (char *)(&last + 1))`) becomes a real va_list from the
   same parameter (Plain_Text_Print, Fancy_Text_Printf, MonoClass::Printf,
   JShell_Fatal, the chat lines).
   Slot 67/68's point/rect, the blitters' ring and depth-shape arguments,
   CC_Draw_Shape's remap and depth shape, unknown_blit_49f3a0's remap/blitter and
   the voxel shadow mask are real pointer types in src/ (Scout1102).
   Rows with `__asm` are skipped and listed. `port/fixups.json` holds the
   per-file substitutions clang needs (sources and `src/shared` headers, each
   with the VC6 meaning it keeps and its evidence) plus qualified-name
   `renames`; each should move into `src/` once the exact build proves the
   edit byte-neutral. `--win64-params` (on: `RA2_NATIVE_WIN64_PARAMS`) makes WPARAM/LPARAM/LRESULT/*_PTR
   pointer-sized and respells the (HWND, unsigned, unsigned, long) procedure shape.
   CRT declarations whose count is VC6's `size_t` (`memset(void *, int, unsigned
   int)`, memcpy, strncpy, qsort, malloc, ...) get `size_t` (an arm64 32-bit
   argument's upper half is unspecified; libc reads 64 bits).
   Calls whose libc namesake differs (Winsock, rand, fopen, time, printf/scanf
   family, wide CRT) are renamed `ra2_*` and supplied by the port.
   **Static initialisers**: `initterm.json` (from `tools/initterm_manifest.py`,
   addresses and names only) gives the original `_initterm` order; units are
   emitted in that order and linked as objects, so `__mod_init_func` runs as the
   original did; explicit init functions are called from generated
   `units/initterm_NNNN.cpp` at their positions (seed/scaffold ones skipped, listed
   in `gen/report.json`).
   `--drafts` appends the non-exact launch-path drafts listed in `drafts.json`
   (entry, score, draft path, host group; only those marked `compiles`) and
   writes `gen/drafts.json`; a draft with `host_source: null` is its own unit,
   linked after the initialiser units (WinMain: no accepted Startup.CPP row hosts
   it); an `adapted` draft is a native adaptation of the scored draft (its private
   headers inlined, non-portable statements marked NATIVE). Each included draft is
   tracked as `native/drafts/<entry>.cpp` (header: entry, score, origin lane or
   worker, NON-EXACT DRAFT; `origin`/`author` in drafts.json are provenance only),
   so the build reads no ignored path. Recovered source text by our lanes, never
   original bytes; raw Ghidra seeds are not tracked. On by default with
   `RA2_NATIVE_SUBSYSTEM=all` (`-DRA2_NATIVE_DRAFTS=OFF` builds without them).
   Refresh a draft by copying the lane's newer text over the file, keeping the header.
2. **Dialect** (`port/include/ra2port/prelude.hpp`, forced include, plus flags
   `-fms-extensions -fms-compatibility -fdelayed-template-parsing -Xclang -fno-wchar
   -fshort-wchar -fwrapv -fno-strict-aliasing -mms-bitfields -fwritable-strings -Wno-register -Wno-c++11-narrowing`):
   calling conventions vanish, `wchar_t` is VC6's `unsigned short`, signed overflow
   wraps, string literals are writable (below), bit-fields take whole units of their
   declared type as VC6 lays them out (`-mms-bitfields`, Scout1103: the TMP cell
   record's ramp byte at +0x2A after an `unsigned :1` group; bases and vtables keep
   the arm64 layout).

   **Writable string literals.** VC6 put string literals in .data, one copy per
   occurrence (the exact build has no /GF), and recovered code writes through
   pointers that callers pass literals to. Clang's `-fwritable-strings` (Apple clang
   21 accepts it) gives every narrow and wide literal its own `__DATA,__data` object,
   the same meaning; native-core-check proves it (an in-place upper-casing of a
   literal and a store into `L""`). A static scan (Scout1061, 2026-11-08: functions
   that write through a char/wide pointer parameter, directly, through an alias,
   through a CRT mutator or transitively, joined with the call sites that pass
   literals there) found 71 literal sites, all one chain: `MixFileClass::Offset`
   0x595AA0 upper-cases its name in place (`_strupr`), reached by
   `MixFileClass::Retrieve` 0x5956C0 (53 sites: palettes, shapes, sounds in
   Init_Game, the One_Time functions, the sidebar), `Load_Cached_Or_Allocated_File`
   0x495270 (12: MOUSE.SHP, sidebar shapes), `ProgressScreenClass::Load_Shape`
   0x61EF40 (4), plus `_strupr("TUR")` twice in
   `BuildingTypeClass::Load_Turret_Voxels` 0x459640. CCFileClass names are copied
   (`Set_Name`) before `Offset` sees them. Literals stored in pointers and written
   later, and name tables in native data, are covered by the same flag (it applies
   to every unit with the dialect, `native/data` and `native/link` included); the
   platform layer (`ra2_port`) keeps read-only literals.
3. **Recovered code** (`ra2_recovered`): accepted sources; unmatched rows enter as
   their best parked draft (flagged non-exact with its score) or a behaviour-written
   implementation, never empty stubs.
   **Link adapters** (`link/`, compiled with the dialect, never original code):
   `address_wrappers.cpp` and `data/address_aliases.tsv` (`tools/address_aliases.py`)
   bind a name to the recovered body at the same original address (catalog
   references and region entries) when the native ABI signatures (LLVM IR of the
   units) agree, or through a converting wrapper when only a float/double return
   differs (x87 st0), or through a converting adapter (`signature_adapters.cpp`)
   when the declarations differ only in integral widths (VC6 passed a 32-bit slot;
   the callee read its own width), a Win32 handle carried in an int (port handles are
   small tokens), pointer-sized message parameters, a `this` the free definition
   never reads, or a result the caller ignores; the remaining mismatches (int for
   pointer, parameter count, struct by value) are listed with a reason in
   `gen/address_aliases.json`: one side's declaration is wrong and src/ settles it.
   The generators are idempotent (each recomputes its queue from the build's link
   inputs without its own outputs, or keeps what it wrote before); after a landing
   re-run `data_globals.py --build <dir>`, rebuild, `address_aliases.py --build <dir>`,
   rebuild, `template_instances.py --build <dir>`, rebuild.
   `data/member_aliases.tsv` and `data/owner_sizes.tsv` (`tools/member_aliases.py`): interior
names of class-typed globals and of pointer/scalar arrays bound to the member or
element at their native offset (a class-typed name at the start of a class-typed
member is that member), and the native size of every class-typed global and
array, which `data_globals.py` gives its BSS storage at least. A class defined in
several headers takes its layout from the header named after it (MouseClass:
`mouse_class.hpp`, not `map_view.hpp`'s light view). The tool keeps the rows it
wrote before, so a layout change needs a fresh pass: header-only TSV, rebuild,
`member_aliases.py --fresh`, `data_globals.py`, `original_data.py`, rebuild, then
repeat the last three until `bss_globals.cpp` no longer changes.
   **Light views** (`map_view.hpp`'s screen layers, `house_screen.hpp`'s HouseClass,
   `unit.hpp`'s house views, `map_techno.hpp`/`map_overlay.hpp`'s Map-unit stand-ins and
   `techno.hpp`'s UnknownCellClass, Scout1098): their VC6 padding is respelled by
   `port/fixups.json` to the full classes' arm64 offsets under `#if defined(__LP64__)` (the
   i686 branch keeps the VC6 text for `member_aliases.py`); `link/check_light_views*.cpp`
   and the behaviour files static_assert both sides. `tools/view_audit.py` (the
   `native-view-audit` test, Scout1108) checks every cell view (the cast targets of
   `Map[...]`/`Cell_At(...)`, other definitions of `CellClass`, classes whose comment
   calls them a view of the cell; in-file ones included) member by member: the clang
   record layouts for i686-pc-windows-msvc and arm64 of each unit that defines or uses
   one, the real class's member at the view member's VC6 offset, and its arm64 offset.
   Members of a light view start in
   the base's tail padding natively (AbstractClass's data size 0x31), so the padding
   arrays are measured from there. Since Scout1111 (M12) the audit covers every padded
   view of the game object tree (AbstractClass and its RTTI-derived classes): the real
   class is the longest definition of each polymorphic (RTTI) class among the shared
   headers, and a view is another definition of that name, or a padded record whose
   leading comment calls it a view of a real class (`view of X`, `X as ... reads it`),
   whose name names one (`Unknown<owner><target>View`: the class named last), or which
   derives from a real base (the derived classes the name names first); the first
   candidate whose members all sit on real members wins (`NOT_VIEWS`/`VIEW_OF` in the
   tool hold the exceptions). `view_audit.py --write` records, per view, the arm64 size
   of each padding member before a mismatching member in `port/view_layouts.json`;
   `native_units.py` rewrites those arrays to `RA2_NATIVE_PAD(arm64, vc6)` (port prelude:
   the VC6 size on i686, so the audit and `member_aliases.py` still see VC6's layout).
   It respelled 49 views (65 padding members): e.g. Who_Can_Build_Me 0x005D5FC0's
   BuildingTypeClass `Factory` and BuildingClass +0x530 views, which kept the sidebar
   empty. Members padding cannot move are listed as `stuck` (fix them in
   `port/fixups.json`); padded stand-ins outside that tree (the MapClass screen layers) are
   listed as suspect, not checked; since Scout1115 `CHECKED` adds WinModemClass and
   MessageListClass (a non-RTTI real class, `REAL`), `VIEW_OF` names
   UnknownRadarObject37B a TechnoClass view (Technos' +0x37B flag, which the radar
   rebuild cleared at a wrong native offset) and `NOT_VIEWS` lists five records
   named after ListClass/EditClass that are not views of them. After changing views:
   reconfigure, `view_audit.py --build <b> --write`, reconfigure again; a stale row stops
   the configure (`--fresh` from an empty file rebuilds the list). **COM class factories** (`TClassFactory<X>::CreateInstance`,
   74 rows) allocate their light in-file view through `ra2port::factory_alloc` (twice the
   VC6 size; `native_units.py` RULES). **IsoTileImageStruct::Tiles** is `ra2port::IsoTileRef`
   natively (tagged self-relative record offsets instead of 32-bit pointers).
`member_aliases.cpp`: names of members of class-typed globals (an interior
   address such as 0x00A3D8D8, the session's Scenarios) bound to the member
   through a reference, with a fixup renaming the declaration in the native view.
   `template_instances*.cpp` (`tools/template_instances.py`)
   explicitly instantiate the recovered VectorClass/DynamicVectorClass bodies for
   element types whose owner unit is missing; `initterm_rewrites.cpp` rewrites the
   seed/scaffold explicit initialisers (`Initterm_<entry>`, called at their
   positions); `behaviour_*.cpp` hold behaviour-written bodies of unmatched rows on
   the start-up/exit path, each citing its evidence (`behaviour_winmain.cpp`,
   `behaviour_surface.cpp`, `behaviour_map.cpp`: the WinMain path;
   `behaviour_launch.cpp`: rows the skirmish harness reaches and launch-path rows
   without a usable draft, listed in `behaviour_rewrites.json`). Fixups run
   before the inline-assembly check, so a fixup can give an evidenced `__asm` block
   its native meaning (Main_Game's exception-return capture).
   **Handwritten assembly** (`behaviour_asm_vqa.cpp`, `behaviour_asm_draw.cpp`,
   `behaviour_asm_lib.cpp`; listed in `behaviour_rewrites.json`): portable C++
   for the functions the original implements in assembly (the archive procedures
   in `src/recovered/asm`: AudioUnzap, VQA_LCW_Uncompress, the VQA SOS ADPCM
   decoder, the four ASM_UnVQ drawers, Brighten_Color/MMX_Brighten_Color in four
   pixel formats, the three Asm_Interpolate copies, Get_CPU_Clock) and for the
   accepted rows whose body is evidenced `__asm` (LCW_Comp, BlitTransXlat<unsigned
   short>::BlitForward, RDTSC, Get_CPU_Speed). Each is written from the adopted
   archive text, not exact; the TSC is a virtual 1 GHz counter (Get_CPU_Speed
   returns 1000). `native-asm-check` (tests/asm_check.cpp) runs each on synthetic
   inputs against references built from the algorithm (encoders, per-pixel
   formulas, the textbook IMA ADPCM decoder) and compares the ADPCM tables with
   the adopted `.inc` text. Aliases are applied with ld64
   `-alias_list` (link_closure.py filters them to the current link).
4. **Native data** (`data/`): globals the code reads from the original's .data/.rdata.
   Each comes from adopted source text (TibSun / ra2-decomp, with attribution), a
   behaviour-written generator, or the game files at run time; never copied bytes.
   `bss_globals.cpp` (`tools/data_globals.py`) defines the zero-initialised ones as
   storage (original extent doubled for LP64); initialised ones without a permitted
   source are listed in `unsourced_initialised.tsv`, never defined from the executable.
   One storage per original address: other names of a BSS address are assembler
   aliases (`.set`), names whose address another input defines go to `bss_aliases.tsv`.
   WinMain path data: `com_guids.cpp` (COM class/interface ids: TibSun MIDL output
   and public Microsoft ids), `startup_winmain.cpp` (key text, launcher names,
   flags; TibSun / ra2-decomp text), `base64_tables.cpp` (TibSun base64.cpp),
   `crc32_table.cpp` (CRC-32 table generated at compile time); each compared
   privately with the pinned target, nothing copied. Data files are not
   port-rewritten: spell VC6 `long` as `int` there.
   **Generated data**: `tools/fast_math_tables.py` computes the fast-math tables
   (sin/cos 10,241, asin 4,097, tan 4,097, atan 4,097 + step, sqrt mantissas 16,384)
   and their scale constants at configure time: TibSun fastmath.cpp's init loops run
   as on the chopping x87 (FSIN/FPTAN reduced by the FPU's 66-bit pi and rounded to
   64 bits, every float store truncated, arctangent limit tan(M_FPI/2 - 0.01)).
   Checked privately against the pinned target: all 38,927 words identical.
5. **Platform seams** (`port/src`, `ra2_port`): the imported Win32 API (366 imports:
   KERNEL32, USER32, GDI32, WSOCK32, ole32, binkw32, ...) implemented on POSIX and
   SDL3 under the same C names; DirectDraw/DirectSound as C++ classes with the COM
   interface shapes the recovered code calls, backed by SDL3 textures and an SDL3
   audio stream (the host's `open_audio`: playing secondary buffers resampled by
   frequency, scaled by volume and pan, 8/16-bit mono/stereo, play cursors moved by
   what the device consumed). GetVolumeInformationA reports the host volume
   holding the drive's folder (a mounted game disc reports its own label).
   Blowfish.dll's COM server (IBlockCipher, MIX header decryption) is native
   (`port/src/blowfish.cpp`, TibSun blowfish.cpp); comctl32's DllGetVersion
   reports 5.81; FindWindowA, OpenMutexA, RegisterActiveObject,
   MessageBoxIndirectA and `_controlfp` are ported; the host plays RA2.EXE's
   launcher handshake (`ra2port::set_launcher`: when the game signals the launcher
   event, message 0xBEEF hands it a mapping whose view holds the text
   Validate_Launcher 0x00491A10 expects, read from game.exe); showing the first top-level
   window activates it (WM_ACTIVATEAPP, WM_ACTIVATE) as Windows does.
   Unported imports are logged no-ops returning their Win32 failure value
   (`port/src/win32_unimplemented.cpp` and the `ra2port::unimplemented` calls).
6. **Host** (`platform/sdl`, `ra2-native`): process, window, event pump
   (`host.cpp`: SDL input to Win32 messages, primary-surface presentation); with
   `-DRA2_NATIVE_SUBSYSTEM=all` it links every recovered object and calls `WinMain`
   (`--smoke-test` only opens the window). Repeated definitions across units get
   VC6 select-any semantics at link (`tools/weaken_duplicates.py`). Until the tree
   is closed, `RA2_NATIVE_LINK_CLOSURE` adds traps/placeholders for what is still
   unrecovered (`tools/link_closure.py`, never shipped).

`historical/native-reconstruction/` stays preserved as reference (SDL3 choice,
MIX/SHP readers usable as test oracles); its own simulation is not reused.
