# Native ports: facts for an update

Plain facts about the native Mac, iPhone and Windows ports as of 2026-11-18 (lanes Y14-Y16, written by lane P1), each with how
it was checked. "Not verified" marks anything nobody has confirmed. Numbers for the exact
decompilation are in [STATUS.md](STATUS.md); this file covers only the ports.

## Platforms

| Platform | Minimum | How checked |
|---|---|---|
| Mac (Apple silicon, arm64) | macOS 26.0 | The app binary's build version (`otool -l`, minos 26.0); played on the owner's Mac |
| iPhone | iOS 17.0 (build default) | Built and installed on the user's foldable iPhone ("Duo"); played there. One app holds both games (see "One app, two games") |
| iPad | iOS 17.0 | Simulator only (lane N1, 2026-11-17, iPad Air 11-inch (M4) Simulator, iOS 27): the one-game app imports the game, plays at 1040x720, the main menu, Options and back, Single Player, Skirmish, Start Game, then touch play (pan, hold-and-drag box, two-finger tap, the toolbar's Deploy, a sidebar build) at 62 frames/s with no fault. Not verified on a real iPad |
| Windows (x86_64) | Windows 10 or later (needs the UCRT that ships with Windows 10) | Cross-built on the Mac; Red Alert 2 played by the user on a real Windows PC (LAN game, 2026-11-15/16); test runs under Wine. Yuri's Revenge and the both-games app run only under Wine so far: packages for the user's PC are in `C:\ra2\for-user` (lane P1, 2026-11-18); needs a user check on the device |

No Rosetta, Wine, virtual machine or emulation is used by the shipped ports: the game code is
the recovered C/C++ compiled natively for each platform.

## What the player supplies

- Their own copy of Red Alert 2 (and Yuri's Revenge for the YR port). Nothing from the game is
  in the repository or in a build: no data, no executables, no movies.
- The ports read the game files from the player's installation at run time. The Mac reads the
  folder in place; the iPhone app copies the files into its own storage once (a folder
  picker on first start). Data the original executable carries (dialog layouts, tables) is
  read from the player's own `game.exe` at run time, and movie tables from their
  `BINKW32.DLL`. Checked: the bundle audit on every iOS build reports "no game files".
- The CD check is skipped.

## What works

| Area | State | How checked |
|---|---|---|
| Campaigns | All 24 missions start. Played to a win on the Mac: Allied 7 of 12, Soviet 2 of 12. Every mission's end screen and both endings are reached in a test that forces each win in turn | Played (Mac); ctests `native-campaign-allied`/`-soviet` on the Mac and the Windows build under Wine |
| Skirmish | Works on Mac, iPhone and Windows | Played; ctests |
| LAN | On by default (Network on the main menu) | See the LAN table below |
| Saves | The port writes only the original game's save format | ctests on Mac, iPhone simulator and Windows; the user's installed Windows game listed, loaded and played the port's saves, and a save made in the real game loaded and played in the port (2026-11-16) |
| Old port saves | Saves from earlier port builds still load for one release and are re-saved in the original format | ctest `native-port-save-transition` |
| Movies and sound | Intro and mission movies play; sound and music work | ctests run with audio on; played |
| Touch (iPhone) | Tap to select and move, box select, pan, pinch zoom, an on-screen toolbar, a Keys button for the keyboard | Played on the Duo |
| Team buttons on touch (both games) | Holding a command-bar team button (400 ms) makes that team the selection; a tap and a mouse are unchanged (lane Y15) | ctests `native-touch-team-hold` (RA2) and `yr-native-touch-team` on the Mac; the Simulator walk `ios_sim_skirmish.py --team-hold` on "RA2 Duo Validation" passed. Needs a user check on the device |

### LAN pairings

| Pairing | Result | How checked |
|---|---|---|
| Mac + Windows (real PC) | Worked, no issues | The user, 2026-11-16 |
| Mac + iPhone | Worked; lobby, hosting and joining both ways after a fix on 2026-11-16. One "Reconnection Error" when the Mac ran an old build; none with the current build | The user, 2026-11-16 |
| Mac + Windows under Wine | 30,000 frames in step, no desync | ctest-style run (lane C16) |
| Yuri's Revenge, two Mac instances | The host seats the guest, chat both ways, both load the map; 3,000 frames with orders (the ctest) and a 12,000-frame game (6 min 20 s: both sides' vehicles grouped as team 1, 36 group moves a side, then every combat unit hunting for the last 2,000 frames) with every frame's game CRC equal (lanes YW2, Y16) | ctest `yr-native-lan-pair` (`--frames 12000` for the long game) |
| Yuri's Revenge, Mac + Windows under Wine | 3,000 frames in step, CRCs equal (lane YW2) | ctest `yr-native-lan-pair-wine`. Not tried between real machines |
| Two Mac instances | 18,000+ frames in step | ctest `native-lan-pair` |
| Lobby chat | Fixed (sends on Return) | ctest `native-lan-lobby`; the user's Mac + iPhone retest on 2026-11-16 passed, chat included (HANDOFF) |
| Tank damage in LAN games | Fixed 2026-11-16 (tank shells did no damage; a renamed table had lost its native rule) | ctest `native-tank-damage` (commit de761bc78); confirmed by the user on the iPhone, 2026-11-16 (commit 9594b9353); guard ctest `native-name-rules` |

## Enhancements (all off by default; the original's look and behaviour are unchanged when off)

| Enhancement | What it does |
|---|---|
| VSync, scaling, interface size | Picture timing and scaling; sidebar and menus at a chosen size |
| Map zoom | 0.5x to 3x (pinch on iPhone) |
| Menu fill | Menus fill the window instead of a 4:3 box |
| Queue Five | Queue up to five of a unit with one click |
| Modern controls | Modern-RTS mouse and touch controls (Classic remains the default) |
| HD vehicles | Vehicles drawn at 2x or 3x detail; drops to 2x by itself if a frame takes too long. In network games 3x plays at 2x (commit adde789a9; ctest `native-lan-background` runs a LAN game with HD vehicles on) |
| Mods | Load mod folders (iPhone: import a zip) |

Checked: ctest `native-enhancements-frames` confirms frames are identical to the original with
every enhancement off; each enhancement has its own ctest.

## One app, two games

On iPhone the one-game app replaced the separate Red Alert 2 app: a single app holds both games,
and the main menu carries a "Play Yuri's Revenge" button (in Yuri's Revenge, "Play Red Alert 2")
in the slot above Exit Game. It keeps the Red Alert 2 app's identity, so an existing player's
imported files, saves and settings carry over; switching to Yuri's Revenge the first time asks
only for its own files. The same build runs on the Mac (ctests `one-game-toggle`,
`one-game-saves`, `one-game-menu-backdrop`). Checked: native/yr/README.md (lanes Y9 and Y11);
the Simulator route `ios_sim_one_game.py`; installed on the Duo.

## Yuri's Revenge

A separate native build made from the ra2-decomp project's source (GPL), sharing the ports'
platform layer. It earns no exact-match credit.

- Mac: skirmishes play from the main menu through a full game to the score screen; a
  45-minute game against three hard AI players (Russia, Russia, Yuri) runs without a fault
  (lane Y13; ctest `yr-native-skirmish-long` runs 10 minutes). Combat: TechnoClass::Fire_At is
  written (a clearly labelled stand-in, not exact) and a forced attack deals damage and kills
  (ctest `yr-native-combat`).
- Campaigns: both campaigns pass a test that forces each win in turn: all 14 missions start
  and end, both endings, the credits and the return to the main menu (ctests
  `yr-native-campaign-allied`/`-soviet`). Real wins are not played yet (left to the user).
- Saves: Yuri's Revenge saves in the original game's format (gamemd.exe's), as Red Alert 2 does
  (ctest `yr-native-save`: save, load from the menus, play on, save again). Not verified: the
  installed Yuri's Revenge on Windows loading a save the port wrote, and the reverse.
- iPhone (in the one-game app): skirmish, sound, touch and building placement; Modern controls
  are wired (two-finger tap gives Modern's order; checked on the simulator, lane Y12); the
  loading screen draws the map preview, the country's name and texts, and the campaign
  loading screen its picture and briefing.
- Windows: a Yuri's Revenge build (`YRNative.exe`, lane Y13) and the both-games app
  (`RA2YRNative.exe` with `ra2-engine.dll`, `yr-engine.dll` and `SDL3.dll` beside it, lane Y16; the
  toggle as on the Mac, each start loads a fresh copy of the game's DLL) are cross-built on the Mac
  (`native/build.sh yr-windows`, `native/build.sh windows-one`). Checked under Wine only: ctests
  `yr-native-skirmish-smoke` (menus, a skirmish, abort) and `one-game-toggle-wine` (RA2 -> YR -> RA2
  with a skirmish, RA2 -> YR with a YR skirmish, the game chosen last starts next). Packages are on
  the user's PC (`C:\ra2\for-user\RA2Native`, `C:\ra2\for-user\RA2YRNative`, lane P1); needs a
  user check on the device.
- Fixed 2026-11-17 (lane Y14), from the user's iPhone reports:
  - graphics noise in place of a deploying MCV and of a Grizzly on its first move (a vehicle
    drawn at a new facing before the game had cached that picture was drawn with 16-bit pixels
    on the 8-bit surface): ctest `yr-native-graphics` watches the tactical view for 900 frames
    while an MCV deploys and a Grizzly first moves (0-24 noise-coloured pixels a frame now,
    75-210 before). Needs a user check on the device;
  - the bottom command bar was missing: it is drawn now (ctest `yr-native-graphics` checks the
    layout and the bar's rows on screen; the Simulator walk `ios_sim_menus.py` on "RA2 Duo
    Validation", interface size 1.25x, HD vehicles off). Needs a user check on the device;
  - the Skip button showed on the start-up loading screen: it now shows only while a movie
    plays (a Mac run and the same Simulator walk, which checks the button is gone before the
    main menu). Needs a user check on the device.
- Fixed 2026-11-17 (lane Y15): the campaign dialog's Difficulty label showed `MISSING:''`
  (three pointer tables were loaded with the wrong extent); checked by lane Y15 on the Mac (no
  ctest names it). Needs a user check on the device.
- Fixed 2026-11-18 (lane Y16): the Lasher tank (LTNK) driven off the map in Allied 2 and Soviet 7.
  Yuri's Revenge has 72 drive turn tracks and 15 raw tracks where the tree had Tiberian Sun's 67
  and 13; a unit leaving a factory diagonally used a track past the table's end. The full tables
  are now loaded from the player's gamemd.exe. Checked: ctests `yr-native-campaign-allied/-soviet`
  now fail on any off-map line, and pass.
- Stand-in code (clearly labelled, not exact) for functions not recovered yet, reached in play:
  ctest `yr-native-standins` (lane Y16) has a Magnetron, a Prism Tank and a Mirage Tank fire on
  enemy Apocalypse tanks, makes sonic and magnetron waves, sends a spy into a power plant and
  fires the Psychic Dominator and the Lightning Storm through the map click's path: no fault, the
  tanks destroyed, the spy inside. Since lane P1 the same test hijacks a vehicle: no unit in Yuri's
  Revenge's rules is a vehicle thief, so the harness makes a civilian one and sends it at an enemy
  truck, which becomes the player's (3 of 3 runs). Not covered by a test yet: bridge repair. The War Factory's unload is covered since lane N3 (ctest
  `yr-native-war-factory`).
- Fixed 2026-11-19 (lane N3), from the user's Duo reports:
  - the command bar's "select all units of this type" button (and the T key) crashed the game: it called a function
    not recovered yet (gamemd.exe 0x00732950), now a clearly labelled stand-in written from gamemd.exe's
    instructions. Checked: ctest `yr-native-type-select` (one tank selected, the button selects every tank; nothing
    selected, nothing; the T key the same) and the Simulator walk `ios_sim_menus.py` on "RA2 Duo Validation" (a tap on
    the button selects all five tanks). Needs a user check on the device;
  - the on-screen keyboard did not come up in Yuri's Revenge's text fields: the player-name field is a list box
    standing in for an edit, which the port did not recognise in Yuri's Revenge. A tap on the field now raises the
    keyboard, as in Red Alert 2 (both games need the tap; neither raises it on entering the screen). Checked: the same
    Simulator walk taps the player name field in both games ("on-screen keyboard shown", then "hidden" after a tap
    away). Needs a user check on the device.
- Stand-ins added 2026-11-19 (lane N3) for functions not recovered yet that a campaign trigger, the keyboard or a
  button reaches (found by tracing the functions still missing at the Mac link from their callers): the trigger
  actions "create building at" (a building forced onto a cell, the units there moved aside), "ion lightning strike"
  and "ion storm stop"; a selected Gap Generator's or Psychic Sensor's range ring; the HealthNav and VeterancyNav
  keys; placing a beacon (the command bar's Beacon button in network games) and drawing it. Before, each of these
  stopped the game. Checked: ctest `yr-native-standins-triggers` (each one through the harness, no fault) and a Mac
  run that placed a beacon with a map click.
- Fixed 2026-11-19 (lane N3): the command bar's PlanningMode button stopped the game as soon as it was pressed with
  units selected (planning mode's drawing and orders were functions not recovered yet; now clearly labelled
  stand-ins written from gamemd.exe's instructions, about 7 KB). Checked: ctest `yr-native-planning` (all tanks
  selected, planning mode on, two waypoints planned, the plan committed with the button, the tanks follow it).
  Picking up or linking planned waypoints by hand with the mouse is not written yet (it does nothing). Functions
  still missing at the Mac link: 110. Needs a user check on the device.
- Fixed 2026-11-19 (lane N3): in Yuri's Revenge a finished vehicle stayed inside the War Factory, and a soldier
  from a barracks was not given its move from the door (the factory drafts' orders were dropped). Checked: ctest
  `yr-native-war-factory` (a tank produced in a War Factory drives out and the factory returns to guard; a GI produced
  in a Barracks walks from the door). Needs a user check on the device (build a tank and a soldier).
- Fixed 2026-11-19 (lane N3): saving through Yuri's Revenge's Save dialog failed (the user's Mac report): the new
  save's file name was lost before the save. Checked: ctest `yr-native-save-dialog` (a skirmish, Escape, Save Game, a
  typed description, Save: a SAVE????.SAV in the save folder) and the same walk in the packaged Mac app. The user's
  other Mac reports (an immediate defeat in a Yuri's Revenge skirmish, a crash on the Network screen) came from a
  stale app build and do not happen in the current one (walked through the menus in the packaged app). Loading a
  save made by the original Yuri's Revenge: not verified.
- Fixed 2026-11-19 (lane N4), the user's three Duo reports in Yuri's Revenge:
  - GIs sent into a building vanished: the soldiers' cell-arrival code (a draft from Tiberian Sun's text) had no
    garrisons and deleted any soldier that was neither an engineer nor a spy. It now garrisons as gamemd.exe does.
    Checked: ctest `yr-native-garrison` (a GI clicked onto a civilian house walks in, the house becomes the player's
    with one occupant, the GI is inside; the house's deploy lets it out again);
  - Mirage tanks' camouflage drew as corrupted graphics: the disguise (a tree) was drawn in the tank's house
    colours. It now takes the tree's cell lighting, as gamemd.exe does. Checked: ctest `yr-native-mirage` (counts
    leaf-green against blue/white pixels where the tree stands: before the fix about 85 green to 615 blue, after
    about 730 to 5) and screen dumps of the tank blinking between itself and a green tree;
  - oil derricks were missing graphics: no building animation was drawn at all (pumps, flames, power plant
    turbines, ...). The layer names the game reads animation types' `Layer=` with were empty in the port (a data
    table loaded from gamemd.exe was cut short by a neighbouring table sized wrongly), so every building animation
    went into no display layer. Checked: ctest `yr-native-building-anims` (the derrick's pump and flame and a power
    plant's animation playing in the ground layer) and screen dumps (the pumpjack moves, the flare burns).
  All three also checked in the Simulator ("RA2 Duo Validation", `ios_sim_menus.py`: the GI garrisons, the
  camouflage counts 271 green to 0 blue/white, the derrick's animations are in the ground layer). Also found and fixed
  by the rerun of the YR ctests: the sonic, laser and Magnetron beam drafts kept depth-buffer addresses in 32 bits
  and crashed when a beam ran one way (`yr-native-standins`).
  Needs a user check on the device: garrison a building, watch a Mirage tank stand still, look at an oil derrick and
  other buildings' animations.
- Fixed 2026-11-19 (lane N4): engineers did not repair bridges in Yuri's Revenge (one sent into a bridge repair hut
  vanished and the bridge stayed broken). The soldiers' cell-arrival draft called the wrong game function where Tiberian
  Sun's text asks whether a building counts as a vehicle; the same mistake let bullets, particles and falling debris
  through every building and kept the player's sell sound from playing. All five places now call the right one.
  Checked: ctest `yr-native-bridge-repair` (on the skirmish map Dead Man's, whose bridges start broken: an engineer
  repairs the bridge, C4 on the hut destroys it again, a second engineer repairs it). Needs a user check on the
  device: repair a bridge with an engineer.
- Checked 2026-11-19 (lane N5): N4's fixes in the Simulator ("RA2 Duo Validation", `ios_sim_menus.py`, current main):
  both games pass the walk, and two new checks: tanks' shells damage an enemy power plant (750 to 590) and an engineer
  captures an enemy Tesla Reactor.
- Checked 2026-11-19 (lane N9): current main (lane N8's random map generator and lane N6's spawn offsets) in the
  Simulator ("RA2 Duo Validation", `ios_sim_menus.py --games yr-random,yr,ra2`): both games pass the walk (60-62
  frames/s, no trap or fault; the enemy power plant 750 -> 616 under the tanks' fire, the reactor captured), and a
  new run, `yr-random`, takes Yuri's Revenge's skirmish lobby through Choose Map, Create Random Map, Preview and OK:
  the generated map (RANDMAP.SED) plays, the player has 13 units at frame 40 and the game reaches frame 600. The
  first Yuri's Revenge run of that walk failed in the lobby: the Simulator delivered the tap away from the name field
  and the Start Game tap 635 ms late in one batch, read as a two-finger right click (a host stall with the Mac's swap
  nearly full, not a game fault); the rerun passed.
- Saves of the original Yuri's Revenge (lane N5): five of the user's own saves from the PC (D:\Games\RA2, made in 2023,
  campaign and skirmish) load in the port and play 1,500 frames; one also through the load dialog at 1024x768 and in
  the one-game app after the switch from Red Alert 2. The user's reported crash did not reproduce. Checked: those runs,
  and ctest `yr-native-save` with `--original-saves` pointed at them (the saves stay outside the repository). Not
  verified: the installed Yuri's Revenge loading a save the port wrote.
- Fixed 2026-11-19 (lane N5), found by tracing the functions still missing at the Mac link (110 -> 101) to keys and
  play: in a Yuri's Revenge skirmish Tab (the Diplomacy screen: players, flags, alliances, kills) and the Delete key
  stopped the game; loading a game, or starting a new one, while an explosion's light was on the map stopped it; hover
  and mech units refusing a destination, and types with line trails, would have stopped it. Each is now a labelled
  stand-in from gamemd.exe's instructions or the tree's own function under its other name. Checked: ctest
  `yr-native-keys-load` (Tab shows the Diplomacy screen with both players and it closes, Delete, two lights alive, then
  a load from the options menu that deletes them and plays on) and a screen dump of the Diplomacy screen. Needs a user
  check on the device: Tab and Delete in a skirmish on the Mac (no keyboard Tab on the Duo).
- Fixed 2026-11-19 (lane N6), found by tracing the functions still missing at the Mac link (101 -> 100): in a Yuri's
  Revenge skirmish the lobby's Choose Map stopped the game as its dialog opened (its window procedure, gamemd.exe
  0x005E6920, is declared and never defined in the pinned ra2-decomp tree). Now in glue/netshare_native.inc, text
  adopted from a later ra2-decomp. Checked: ctest `yr-native-choose-map` (the lobby's map started once, then Choose
  Map, a row clicked, OK, Start Game: the game starts on the other map, no trap). Needs a user check: Choose Map in a
  Yuri's Revenge skirmish on the Mac and the Duo.
- Fixed 2026-11-19 (lane N8): that dialog's Create Random Map stopped the game: Yuri's Revenge's random map generator
  (MapGeneratorClass::Generate_Random_Map and nine more, MapClass::Pick_Random_Tile_Variant, the preview tiles) is
  missing from the YR tree (the pinned and the latest ra2-decomp). Clearly labelled stand-ins, written from gamemd.exe's
  instructions in the shape of Red Alert 2's twins (most have the same size and calls as Red Alert 2's). Writing them
  uncovered a fault in an older stand-in (MapClass::Zone_Reset's result): about one random map in three had no start
  points, and both players began with nothing and lost at once. Checked: ctest `yr-native-choose-map` (Create Random
  Map, Preview, OK, Start Game: the generated map is played with the player's units, past frame 600, no trap); 6 of 6
  runs with both players' units after the Zone_Reset fix. Link closure 100 -> 89. Needs a user check: Create Random Map
  in a Yuri's Revenge skirmish on the Mac and the Duo (Preview, OK, Start Game).
- The yr-native-standins hijack flake (lane N6): two causes, both in the test. The truck's offset was lost (the YR
  harness kept 4 words of an order; now 8) and it could land 11 cells from the thief at some start positions (2 of 4
  runs failed, the same corner both times), and
  under load the 170 s run reached frame 1250, where it is checked, only just or not at all. The thief is now placed
  3,3 from the base and the truck 3 cells from the thief (harness `spawn ... near CIV1`), and the run is 220 s. Checked: four runs after the change
  (see native/yr/README.md, lane N6).
- Need a user check on the device: the unit animation fixes from lane Y13 (walk frames and
  idle animations), the lane Y14 fixes above, the team-button hold, and the lane N3 and N4 fixes above.
- Still open: longer games may still reach functions not recovered yet (written as clearly
  labelled stand-in code when found; the Mac link closure, functions still missing, was 175
  after lane Y14, 126 after lane Y16, 110 after lane N3, 101 after lane N5 and 100 after lane N6); Yuri's Revenge LAN between separate machines and with Windows on a real PC is untested.

## Not done or not supported

- Internet play: held. The Internet button is disabled.
- Open faults: none known in Red Alert 2 or Yuri's Revenge beyond the untested items above; the
  start crash seen once in the simulator (1 of 10 runs, lane Y7) has not recurred in 12 later
  starts (lane Y12).
- Unchecked code: 124 rewritten functions are not reached by any test or play so far (lane C21). Lane N1
  (2026-11-17) labelled each from game.exe's callers: 110 reachable in the base game but on paths no
  test takes yet (each statically checked), 6 unreachable in the base game, 8 unreachable in the port
  (Westwood Online only, or a branch the port never takes); `native/behaviour_rewrites.json` `reach`.
  Lane N4 (2026-11-19) reached 8 of the reviewed ones on a coverage build (new scenarios in
  `native/tests/coverage_runs.py`: `n4-deaths`, a demolition truck and two tanks destroyed; `n4-ui`, modifier
  clicks and a closed waypoint path; `n4-randmap`, a random map generated and played 4,000+ frames; and a chat
  line typed in `lan_pair.py`'s game): Shake_The_Screen, Audio_Event_Handle_Set, Planning_Active, a FootClass
  slot, two of the random map generator, MessageListClass::Get_Edit_Buf and IME_Stub1 (`reach`: "reached in a
  coverage run"). Lane N6 (2026-11-19) reached 10 more: the scenarios `n5-bomb` (a bomb planted on the player's own
  vehicle and a save while it is planted: BombClass::Save and GetClassID; the laser beams), `n6-place` (a psychic
  sensor, a missile silo and a gap generator spawned for the player and clicked: the sensor array's cell marks, the
  range circles' Draw_Ellipse, the silo's missile mission when the nuke is fired) and `n6-sidebar` (a cameo clicked
  in the sidebar: the factory's on-hold check; the defence tab's hot key, bound in a KEYBOARD.INI of the run's own)
  and `n6-seeds` (the random map dialog's Save seed: MapSeedClass::Save_Menu_Name). Lane N8 (2026-11-19) reached 4
  more with `n8-seeds` (a seed saved through the save dialog's Save, then loaded and deleted through the random map
  dialog's Load seed and Delete seed: MapSeedClass::Saved_Message, Load_Menu_Name, Delete_Menu_Name, Delete_File;
  n6-seeds had clicked the save dialog's list and Back). Lane N9 (2026-11-19) reached 3 more: `n9-bomb` (a Crazy
  Ivan bombs the player's engineer and an enemy missile silo spawned beside the base, then the engineer captures the
  silo: BombClass::Disarm; the bombed engineer alone selected with the mouse over it: the bomb's kind asked for the
  detonate cursor, BombClass::Unknown_00435FF0) and `n9-ui` (T held and a Shift click on a selected tank: every tank unselected,
  Unknown_006F7020; the input log shows 5 tanks selected, then none). 88 remain reviewed but unreached.
  Every one of the 311 rewrite rows now carries a behaviour verdict against the original (lane N2,
  2026-11-17: the last 31, the hand-written-assembly ones among them, checked statically): 299 the
  same (some apart from named, harmless edge cases), 4 fixed, 1 different by design, 3 kept (the
  original loops forever there), 2 partly fixed (two menu-control procedures: the combo box's paint and
  some rarely used edit-control messages), 2 not fixed (an unreachable tab control; LCW_Comp on 0-1
  bytes, where the original reads past its data).
- No signed or notarised builds: no TestFlight, no Mac notarisation, no Windows code signing.
  Gatekeeper and SmartScreen will warn.

## How to get it today

From source only; there are no binary downloads.
`native/build.sh mac`, `mac-one`, `ios`, `windows`, `yr-windows` and `windows-one` build each app with
one command (native/README.md "Building the apps"). The repository's
[native/README.md](../native/README.md) has the build steps for Mac, iPhone and Windows (the
Windows build is cross-compiled on a Mac). The iPhone build needs the builder's own Apple
developer signing.
