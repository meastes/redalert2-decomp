# Native ports: facts for an update

Plain facts about the native Mac, iPhone and Windows ports as of 2026-11-16, each with how
it was checked. "Not verified" marks anything nobody has confirmed. Numbers for the exact
decompilation are in [STATUS.md](STATUS.md); this file covers only the ports.

## Platforms

| Platform | Minimum | How checked |
|---|---|---|
| Mac (Apple silicon, arm64) | macOS 26.0 | The app binary's build version (`otool -l`, minos 26.0); played on the owner's Mac |
| iPhone | iOS 17.0 (build default) | Built and installed on the user's foldable iPhone ("Duo"); played there |
| iPad | iOS 17.0 | Not verified: the app carries iPad icons but has not been run on an iPad |
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
| Lobby chat | Fixed 2026-11-16 | ctest `native-lan-lobby`; not yet re-checked by the user on real devices |

## Enhancements (all off by default; the original's look and behaviour are unchanged when off)

| Enhancement | What it does |
|---|---|
| VSync, scaling, interface size | Picture timing and scaling; sidebar and menus at a chosen size |
| Map zoom | 0.5x to 3x (pinch on iPhone) |
| Menu fill | Menus fill the window instead of a 4:3 box |
| Queue Five | Queue up to five of a unit with one click |
| Modern controls | Modern-RTS mouse and touch controls (Classic remains the default) |
| HD vehicles | Vehicles drawn at 2x or 3x detail; drops to 2x by itself if a frame takes too long. A cap at 2x in network games was decided on 2026-11-16 and is being added |
| Mods | Load mod folders (iPhone: import a zip) |

Checked: ctest `native-enhancements-frames` confirms frames are identical to the original with
every enhancement off; each enhancement has its own ctest.

## Yuri's Revenge

A separate native build made from the ra2-decomp project's source (GPL), sharing the ports'
platform layer.

- Mac: skirmishes play from the main menu through a full game to the score screen; a
  10-minute game against three hard AI players runs without a fault (ctest
  `yr-native-skirmish-long`). An optional one-app build switches between Red Alert 2 and Yuri's
  Revenge from a toggle at the bottom left of the main menu (ctest `one-game-toggle`).
- iPhone: a standalone Yuri's Revenge app runs on the user's Duo (skirmish, sound, touch,
  building placement).
- Not working or not verified yet: campaigns not tried; Modern controls not wired on iPhone
  (Classic only); one crash at game start seen in 1 of 10 simulator runs; longer games may reach
  functions not recovered yet (they are written as clearly labelled stand-in code); no Windows
  build yet; the one-app build is Mac only.

## Not done or not supported

- Internet play: held. The Internet button is disabled.
- Open faults: in a 2026-11-16 LAN game tanks did no damage to each other (being investigated);
  the Yuri's Revenge iPhone start crash above.
- Unchecked code: 126 rewritten functions are not reached by any test or play so far (lane C21);
  most belong to features the base game does not use.
- No signed or notarised builds: no TestFlight, no Mac notarisation, no Windows code signing.
  Gatekeeper and SmartScreen will warn.

## How to get it today

From source only; there are no binary downloads. The repository's
[native/README.md](../native/README.md) has the build steps for Mac, iPhone and Windows (the
Windows build is cross-compiled on a Mac). The iPhone build needs the builder's own Apple
developer signing.
