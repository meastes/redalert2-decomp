# Native ports: facts for an update

Plain facts about the native Mac, iPhone and Windows ports as of 2026-11-17 (lane Y14), each with how
it was checked. "Not verified" marks anything nobody has confirmed. Numbers for the exact
decompilation are in [STATUS.md](STATUS.md); this file covers only the ports.

## Platforms

| Platform | Minimum | How checked |
|---|---|---|
| Mac (Apple silicon, arm64) | macOS 26.0 | The app binary's build version (`otool -l`, minos 26.0); played on the owner's Mac |
| iPhone | iOS 17.0 (build default) | Built and installed on the user's foldable iPhone ("Duo"); played there. One app holds both games (see "One app, two games") |
| iPad | iOS 17.0 | Simulator only (lane N1, 2026-11-17, iPad Air 11-inch (M4) Simulator, iOS 27): the one-game app imports the game, plays at 1040x720, the main menu, Options and back, Single Player, Skirmish, Start Game, then touch play (pan, hold-and-drag box, two-finger tap, the toolbar's Deploy, a sidebar build) at 62 frames/s with no fault. Not verified on a real iPad |
| Windows (x86_64) | Windows 10 or later (needs the UCRT that ships with Windows 10) | Cross-built on the Mac; played by the user on a real Windows PC (LAN game, 2026-11-15/16); test runs under Wine |

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

### LAN pairings

| Pairing | Result | How checked |
|---|---|---|
| Mac + Windows (real PC) | Worked, no issues | The user, 2026-11-16 |
| Mac + iPhone | Worked; lobby, hosting and joining both ways after a fix on 2026-11-16. One "Reconnection Error" when the Mac ran an old build; none with the current build | The user, 2026-11-16 |
| Mac + Windows under Wine | 30,000 frames in step, no desync | ctest-style run (lane C16) |
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
- Windows: a Yuri's Revenge build exists (cross-built on the Mac, `YRNative.exe`; lane Y13).
  Checked only under Wine (ctest `yr-native-skirmish-smoke`: menus, a skirmish, abort); not
  run on a real PC yet. No one-game (both games in one app) build on Windows yet.
- Fixed 2026-11-17 (lane Y14), from the user's iPhone reports:
  - noise in place of a deploying MCV and of a Grizzly on its first move (any vehicle drawn
    at a new facing before the game had cached that picture): checked on the Mac by ctest
    `yr-native-graphics` (no noise while an MCV deploys and a Grizzly first moves); not yet
    checked by the user on the device;
  - the bottom command bar was missing: it is drawn now (ctest `yr-native-graphics`, and the
    simulator walk `ios_sim_menus.py` on "RA2 Duo Validation", with the user's interface size
    1.25x and HD vehicles off);
  - the Skip button showed on the start-up loading screen: it now shows only while a movie
    plays (Mac run and the same simulator walk).
- Need a user check on the device: the unit animation fixes from lane Y13 (walk frames and
  idle animations: WalkRate, the infantry action table MasterDoControls) and the lane Y14
  fixes above; the on-screen keyboard in Yuri's Revenge's text fields.
- Still open: the campaign dialog's Difficulty label shows `MISSING:''`; a Lasher tank (LTNK)
  in two campaign missions gets a garbage position from its drive locomotor (guarded: the map
  ignores the bad cell and logs it); LAN play (lane YW); longer games may still reach
  functions not recovered yet (written as clearly labelled stand-in code when found; about
  40 such functions remain on play paths, native/yr/README.md lane Y14).

## Not done or not supported

- Internet play: held. The Internet button is disabled.
- Open faults: the Yuri's Revenge items above (the Difficulty label, the Lasher tank's position);
  the start crash seen once in the simulator (1 of 10 runs, lane Y7) has not recurred in 12 later
  starts (lane Y12).
- Unchecked code: 124 rewritten functions are not reached by any test or play so far (lane C21). Lane N1
  (2026-11-17) labelled each from game.exe's callers: 110 reachable in the base game but on paths no
  test takes yet (each statically checked), 6 unreachable in the base game, 8 unreachable in the port
  (Westwood Online only, or a branch the port never takes); `native/behaviour_rewrites.json` `reach`.
  Every one of the 311 rewrite rows now carries a behaviour verdict against the original (lane N2,
  2026-11-17: the last 31, the hand-written-assembly ones among them, checked statically): 299 the
  same (some apart from named, harmless edge cases), 4 fixed, 1 different by design, 3 kept (the
  original loops forever there), 2 partly fixed (two menu-control procedures: the combo box's paint and
  some rarely used edit-control messages), 2 not fixed (an unreachable tab control; LCW_Comp on 0-1
  bytes, where the original reads past its data).
- No signed or notarised builds: no TestFlight, no Mac notarisation, no Windows code signing.
  Gatekeeper and SmartScreen will warn.

## How to get it today

From source only; there are no binary downloads. The repository's
[native/README.md](../native/README.md) has the build steps for Mac, iPhone and Windows (the
Windows build is cross-compiled on a Mac). The iPhone build needs the builder's own Apple
developer signing.
