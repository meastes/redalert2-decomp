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
| Unreached rewrites | Partly done | 124 of 311 rewritten functions were reached by no test or play. N1 labelled each: 110 reachable in the base game and checked statically, 6 unreachable in the base game, 8 unreachable in the port. Coverage runs have since reached 33 of the 110 (lane N4: 8; lane N6: 10, among them the sidebar's on-hold check, the defence tab's hot key, the missile silo's launch, the psychic sensor and range circles, the random map dialog's seed save; lane N8: 4, the seed list's save message, load and delete; lane N9: 3, a Crazy Ivan bomb taken off by an engineer's capture and its detonate-cursor check, and unselecting every unit of a type; lane N10: 8, a player leaving a LAN game, the command bar's Beacon in a LAN game, the guard-area cursor, the Delete key, a sound with an audio controller, three default slots; lane N11: 4, the audio cache freed at Exit Game, the Iron Curtain over a tree, a LAN host's Choose Map, the attack-move cursor). Lane N11 also showed from game.exe's instructions and the shipped rules.ini that 23 cannot run: 2 Westwood Online only, 21 not in the base game (relabelled). | 53 left (`reach` "reviewed"): default virtual slots with many callers (ObjectClass slots 35, 50, 51, 93, 111, 112, 115 and the like), network-dialog branches that need a missing map or another game mode (Unholy Alliance, observers, Cooperative), audio voices, the 8-bit and backward blitters, fog of war. |
| Drafts behaviour-checked | Done | All 311 rewrites have a verdict against the original (lane N2): 299 the same, 4 fixed, 1 different by design, 3 kept, 2 partly fixed, 2 not fixed. | Optional: the 2 partly fixed menu-control procedures (combo box paint, rare edit-control messages). |
| One-command builds | Done | `native/build.sh` with `mac`, `mac-one`, `ios`, `windows`, `yr-windows` or `windows-one`. | Nothing (no signing or notarisation, by choice). |
| iPad | Partly done | Simulator walk passes: import, menus, skirmish, touch play, 62 frames/s. | Not verified on a real iPad. |
| LAN | Done | Mac with Windows (your PC), Mac with iPhone (you), Mac with Windows under Wine for 30,000 frames, two Macs for 18,000+ frames. | Internet play stays held. |

## Yuri's Revenge

| Item | State | What is done | What is left |
|---|---|---|---|
| Skirmish stability | Mostly done | A 45-minute game against three hard AIs on the Mac has no fault, and a 10-minute ctest runs. Lane N6: the skirmish lobby's Choose Map stopped the game (its dialog procedure was missing); fixed, ctest `yr-native-choose-map`. | 89 functions still missing at the Mac link (100 before lane N8; the rest are Westwood Online, coop, modem, Tiberian Sun-only or editor paths; lane N10 re-checked the ones with a gamemd.exe caller outside those families and found none reachable in skirmish or campaign play). Lane N8: Choose Map's Create Random Map works (the random map generator written as stand-ins; ctest `yr-native-choose-map` makes, previews and plays a random map). Longer and varied games will find more. |
| Campaign trace and forced-win chain | Done | Both campaigns pass a test that forces each win in turn: all 14 missions, both endings, the credits and the return to the main menu. | Real wins are yours. |
| Campaign stand-ins | Partly done | Stand-ins reached in play: Psychic Dominator, Lightning Storm, the beams and waves, the spy, the hijacker, the Lasher tank's drive tracks; lane N3: the trigger actions "create building at", "ion lightning strike" and "ion storm stop", range rings, beacons, the HealthNav/VeterancyNav keys (ctest `yr-native-standins-triggers`), planning mode from the command bar (ctest `yr-native-planning`; picking up planned waypoints with the mouse not written). | Bridge repair has a test since lane N4 (`yr-native-bridge-repair`; it found engineers vanishing without a repair, fixed). The War Factory's unload has a test (lane N3, `yr-native-war-factory`; it found vehicles staying inside the factory, fixed). The trace was static (callers of the missing functions); no mission was played through. |
| Windows build | Partly done | `YRNative.exe` and the both-games `RA2YRNative.exe` are cross-built, pass under Wine, and are packaged on your PC. | Your run on the real PC. |
| LAN | Partly done | Two Macs played a 12,000-frame game in step; Mac with Windows under Wine played 3,000 frames. | Not yet tried between two real machines or with your Windows PC. |
| Saves | Mostly done | Saves are written in the original format and load back in the port. Five of your own Yuri's Revenge saves from the PC (2023) load and play in the port (lane N5). | Your installed Yuri's Revenge loading the port's save. |

## Your checks (once)

1. **On the Duo** (checked by the user 2026-11-18):
   - working: team-button hold, MCV deploy and Grizzly first move, the Skip button, unit animations, the campaign Difficulty label;
   - faults reported 2026-11-19 ~04:15Z, fixed by lane N4, please check: GIs sent into a building vanished (now they
     garrison it, and the building's deploy lets them out); a Mirage tank's camouflage drew as corrupted graphics (now a
     tree); oil derricks lacked their pump and flame (no building animation was drawn; power plants, refineries and
     the rest should animate again too);
   - fault, fixed by lane N3 (2026-11-19), please check again: Yuri's Revenge's command bar "select all units of this type" button crashed the game;
   - found and fixed by lane N5 (2026-11-19), please check: in a Yuri's Revenge skirmish, Tab (the Diplomacy screen) and
     the Delete key stopped the game; loading a game while explosions were lighting the map stopped it too;
   - found and fixed by lane N6 (2026-11-19), please check: in a Yuri's Revenge skirmish lobby, Choose Map stopped the
     game; now the map list opens and the chosen map is played. Its Create Random Map button stopped the game too:
     fixed by lane N8 (2026-11-19), please check: Create Random Map, Preview, OK, Start Game plays the generated map
     (lane N9 walked it in the Simulator: the generated map plays);
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
     - loading a save made by the original Yuri's Revenge crashing: **not reproduced** in the current build (lane N5):
       five of your own saves from the PC's D:\Games\RA2 load and play, also through the load dialog and after the
       switch from Red Alert 2. Loading while a combat light was alive did stop the game; fixed. If it still crashes,
       please say which save (its name in D:\Games\RA2) and from where you loaded it (main menu or in a game).
     Please check all four again on the new build.
2. **On the Windows PC:** `C:\ra2\for-user\RA2YRNative`:
   - switch games;
   - a skirmish in each game;
   - a Yuri's Revenge LAN game against the Mac.
3. **Yuri's Revenge saves:** the installed game loads the port's save, and the reverse.
4. **Campaign missions:** play them per [CAMPAIGN_PLAY_LIST.md](CAMPAIGN_PLAY_LIST.md), the finals first.

## Suggested order for the next port lane

0. Done (lane N3): the two Duo faults above; the user re-checks them on the Duo.
0a. Done (lane N4, 2026-11-19): the user's three new Duo faults in Yuri's Revenge, each fixed on the Mac with a ctest
   (details and checks in PORTS_FACTS.md): GIs now garrison buildings (`yr-native-garrison`); the Mirage tank's
   camouflage draws as a tree in the tree's colours (`yr-native-mirage`); building animations, the oil derrick's pump
   and flame among them, are drawn again (`yr-native-building-anims`). The user re-checks them on the Duo.
1. Trace the Yuri's Revenge campaign missions for missing functions, and write stand-ins for them (lane N5: 110 -> 101,
   Tab's Diplomacy screen, the Delete key, combat lights at a load, the scold sound, line trails; ctest
   `yr-native-keys-load`).
2. Done (lane N4): the bridge repair test, `yr-native-bridge-repair`; it found engineers vanishing into the hut without a
   repair (fixed).
3. Reach Red Alert 2's unreached rewrites, most-called first (lane N11: 80 -> 53, 4 reached with scenarios `n11-exit-audio`, `n11-terrain`, `n11-lan` and `n11-ui`, 23 shown unreachable from game.exe and the shipped rules; lane N10: 88 -> 80, scenarios `n10-leave` and `n10-bar`; lane N9: 91 -> 88, scenarios `n9-bomb` and `n9-ui`; lane N8: 95 -> 91, scenario `n8-seeds`; lane N6: 105 -> 95, scenarios `n6-place`, `n6-sidebar` and `n6-seeds`, `n5-bomb` run; lane N4 started: a coverage scenario `n4-deaths` reached
   3 more, Shake_The_Screen, Audio_Event_Handle_Set and Planning_Active, `n4-ui` one, FootClass slot 271, and `n4-randmap` two of the random map generator (the skirmish then played 4,000+ frames on a generated map), and a chat line in `lan_pair.py` two (MessageListClass::Get_Edit_Buf, IME_Stub1); 105 left as "reviewed").
4. Yuri's Revenge LAN on two real machines once you can run the PC side.
5. Done (lane N8, 2026-11-19): Yuri's Revenge's random map generator (Choose Map > Create Random Map): stand-ins from
   gamemd.exe's instructions in the shape of Red Alert 2's twins (glue/mapgen_native.inc and two more), and a fault it
   uncovered (MapClass::Zone_Reset's result: one random map in three had no start points). ctest `yr-native-choose-map`.
   MapPreviewClass::Blit_Preview stays missing (only the modem dialogs use it).

## User checks on the Duo (2026-11-19 ~19:45Z)
- Pass: GIs garrison and leave buildings; selling plays the sell sound; Yuri's Revenge random maps (Choose Map, Create Random Map) play correctly.
- Fault: the Mirage tank's camouflage still shows corrupted graphics on its first animation; every later animation is correct (same first-draw pattern as the MCV/Grizzly fix in lane Y14). First item for the next port lane.
  Lane N12 (2026-11-19): not reproduced on the Mac or in the Simulator (the first morph is pixel for pixel the
  second, on five maps, a factory-built tank and a re-disguise); `yr-native-mirage` now compares the first animation
  with the second. Needs from the user: a screenshot or description of the first animation, and whether the Duo's
  app was built after lane N4.
- Mirage camouflage (update 2026-11-19 ~20:40Z): the user saw blue trees once and cannot reproduce it; lane N12 could not either (Mac and Simulator, five maps, frame-by-frame). Blue trees are the pre-N4 house-colour symptom. Closed as not reproduced; yr-native-mirage now compares the first animation with the second, so a recurrence would show there. Reopen if seen again.
