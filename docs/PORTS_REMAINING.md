# Ports: what is done and what is left

The costed ports list as of 2026-11-18, item by item, so that a port lane can start from it.
Facts and how each was checked are in [PORTS_FACTS.md](PORTS_FACTS.md).

"N1 and Y12 finished" meant this much:
- **N1** (2026-11-17) ran the iPad Simulator walk and labelled each unreached Red Alert 2 rewrite by
  whether the game can reach it.
- **Y12** wired Yuri's Revenge's Modern controls on iPhone and checked them in the Simulator.

Neither lane widened test coverage, played the campaigns, or tested anything on real devices beyond the Duo.
The YR lanes after them, Y13 to Y16, and lane P1 did the rest shown below.

## Red Alert 2

| Item | State | What is done | What is left |
|---|---|---|---|
| Unreached rewrites | Partly done | 124 of 311 rewritten functions are reached by no test or play. N1 labelled each: 110 reachable in the base game and checked statically, 6 unreachable in the base game, 8 unreachable in the port. | Tests or scripted play that reach the 110, starting with the most-called. |
| Drafts behaviour-checked | Done | All 311 rewrites have a verdict against the original (lane N2): 299 the same, 4 fixed, 1 different by design, 3 kept, 2 partly fixed, 2 not fixed. | Optional: the 2 partly fixed menu-control procedures (combo box paint, rare edit-control messages). |
| One-command builds | Done | `native/build.sh` with `mac`, `mac-one`, `ios`, `windows`, `yr-windows` or `windows-one`. | Nothing (no signing or notarisation, by choice). |
| iPad | Partly done | Simulator walk passes: import, menus, skirmish, touch play, 62 frames/s. | Not verified on a real iPad. |
| LAN | Done | Mac with Windows (your PC), Mac with iPhone (you), Mac with Windows under Wine for 30,000 frames, two Macs for 18,000+ frames. | Internet play stays held. |

## Yuri's Revenge

| Item | State | What is done | What is left |
|---|---|---|---|
| Skirmish stability | Mostly done | A 45-minute game against three hard AIs on the Mac has no fault, and a 10-minute ctest runs. | 110 functions still missing at the Mac link. Each is written as a labelled stand-in when play reaches it. Longer and varied games will find more. |
| Campaign trace and forced-win chain | Done | Both campaigns pass a test that forces each win in turn: all 14 missions, both endings, the credits and the return to the main menu. | Real wins are yours. |
| Campaign stand-ins | Partly done | Stand-ins reached in play: Psychic Dominator, Lightning Storm, the beams and waves, the spy, the hijacker, the Lasher tank's drive tracks; lane N3: the trigger actions "create building at", "ion lightning strike" and "ion storm stop", range rings, beacons, the HealthNav/VeterancyNav keys (ctest `yr-native-standins-triggers`), planning mode from the command bar (ctest `yr-native-planning`; picking up planned waypoints with the mouse not written). | Not covered by a test: bridge repair. The War Factory's unload has a test (lane N3, `yr-native-war-factory`; it found vehicles staying inside the factory, fixed). The trace was static (callers of the missing functions); no mission was played through. |
| Windows build | Partly done | `YRNative.exe` and the both-games `RA2YRNative.exe` are cross-built, pass under Wine, and are packaged on your PC. | Your run on the real PC. |
| LAN | Partly done | Two Macs played a 12,000-frame game in step; Mac with Windows under Wine played 3,000 frames. | Not yet tried between two real machines or with your Windows PC. |
| Saves | Partly done | Saves are written in the original format and load back in the port. | Your installed Yuri's Revenge loading the port's save, and the reverse. |

## Your checks (once)

1. **On the Duo** (checked by the user 2026-11-18):
   - working: team-button hold, MCV deploy and Grizzly first move, the Skip button, unit animations, the campaign Difficulty label;
   - fault, fixed by lane N3 (2026-11-19), please check again: Yuri's Revenge's command bar "select all units of this type" button crashed the game;
   - fault, fixed by lane N3 (2026-11-19), please check again: the on-screen keyboard didn't come up in Yuri's Revenge's text fields (the skirmish player name). Now a tap on the field raises it, as in Red Alert 2.
   - Mac app checks (the user, 2026-11-19, on build/mac-app/RA2YR.app, a stale build from 2026-11-17 11:20). Lane N3
     rebuilt the current app (`native/build.sh mac-one`) and walked each one through the menus in the packaged app
     (app mode, its own settings and save folder, under a scratch HOME):
     - Yuri's Revenge skirmish ending in an immediate defeat: not reproduced in the current build (a fresh YR start
       and after switching from Red Alert 2: the player's 5 tanks still there at frame 600);
     - entering Yuri's Revenge's Network screen crashing: not reproduced in the current build (the lobby comes up,
       fresh and after switching from Red Alert 2);
     - saving in Yuri's Revenge failing: reproduced and **fixed** by lane N3 (the Save dialog's file name was lost;
       ctest `yr-native-save-dialog` saves through the options menu);
     - loading a save made by the original Yuri's Revenge crashing: **not checked** (no save from the original Yuri's
       Revenge on the owner's Mac: the saves in the game folder are Red Alert 2's). Needs the user's save file, or the
       user's retry on the new build; next port lane.
     Please check all four again on the new build.
2. **On the Windows PC:** `C:\ra2\for-user\RA2YRNative`:
   - switch games;
   - a skirmish in each game;
   - a Yuri's Revenge LAN game against the Mac.
3. **Yuri's Revenge saves:** the installed game loads the port's save, and the reverse.
4. **Campaign missions:** play them per [CAMPAIGN_PLAY_LIST.md](CAMPAIGN_PLAY_LIST.md), the finals first.

## Suggested order for the next port lane

0. Done (lane N3): the two Duo faults above; the user re-checks them on the Duo.
0a. New Duo faults in Yuri's Revenge (user, 2026-11-19 ~04:15Z, on the build with lane N3's fixes, which all checked out):
   - GIs sent into a building disappear instead of garrisoning it;
   - Mirage tanks' camouflage draws as corrupted graphics;
   - oil derricks are missing some of their graphics.
   First items for the next port lane: reproduce each on the Mac and in the "RA2 Duo Validation" simulator, fix, and add a ctest.
1. Trace the Yuri's Revenge campaign missions for missing functions, and write stand-ins for them.
2. Add the bridge repair test (the War Factory unload test is done, lane N3).
3. Reach Red Alert 2's 110 unreached rewrites, most-called first.
4. Yuri's Revenge LAN on two real machines once you can run the PC side.
