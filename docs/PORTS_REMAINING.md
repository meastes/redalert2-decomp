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
| Unreached rewrites | Done | 124 of 311 rewritten functions were reached by no test or play. N1 labelled each: 110 reachable in the base game and checked statically, 6 unreachable in the base game, 8 unreachable in the port. Coverage runs have since reached 48 of the 110 (lane N4: 8; lane N6: 10, among them the sidebar's on-hold check, the defence tab's hot key, the missile silo's launch, the psychic sensor and range circles, the random map dialog's seed save; lane N8: 4, the seed list's save message, load and delete; lane N9: 3, a Crazy Ivan bomb taken off by an engineer's capture and its detonate-cursor check, and unselecting every unit of a type; lane N10: 8, a player leaving a LAN game, the command bar's Beacon in a LAN game, the guard-area cursor, the Delete key, a sound with an audio controller, three default slots; lane N11: 4, the audio cache freed at Exit Game, the Iron Curtain over a tree, a LAN host's Choose Map, the attack-move cursor; lane N14: 1, a tree's name in the tooltip; lane N17: 1, a selected war factory's zone check with Alt held over the ground; lane N22: 15, the Next/Previous Object keys, a LAN desync's sync log (CRC engine, build line), a wet random map, the load/save/delete captions, Tab in the LAN lobby, a LAN Cooperative game, a LAN random map's start; lane N24: 2, a looping sound out of earshot and back, a LAN Unholy Alliance game). Lane N11 also showed from game.exe's instructions and the shipped rules.ini that 23 cannot run: 2 Westwood Online only, 21 not in the base game (relabelled). | None left (lane N24, 2026-11-21: the last 7 settled, 2 reached and 5 shown unreachable; it found and fixed a data fault that kept every sound from looping and gave every sound the lowest priority; details in PORTS_FACTS.md). Lane N26: block coverage inside the reached rewrites (`coverage_runs.py blocks`): 1,770 of 3,015 edges taken; base defences' attack mission (prism support, delayed fire) and the spy, C4 and Chrono Legionnaire bridges now run in `n26-defence` and `n26-infiltrate`, same as the original. |
| Drafts behaviour-checked | Done | All 311 rewrites have a verdict against the original (lane N2): 299 the same, 4 fixed, 1 different by design, 3 kept, 2 partly fixed, 2 not fixed. | Optional: the 2 partly fixed menu-control procedures (combo box paint, rare edit-control messages). |
| One-command builds | Done | `native/build.sh` with `mac`, `mac-one`, `ios`, `windows`, `yr-windows` or `windows-one`. | Nothing (no signing or notarisation, by choice). |
| iPad | Partly done | Simulator walk passes: import, menus, skirmish, touch play, 62 frames/s. | Not verified on a real iPad. |
| LAN | Done | Mac with Windows (your PC), Mac with iPhone (you), Mac with Windows under Wine for 30,000 frames, two Macs for 18,000+ frames. | Internet play stays held. |
| Campaign missions played | Done (soak) | Lane N25: every mission, Allied and Soviet, reached through the menus and played 4,000 game frames headless with scripted orders (the MCV deploys, the army hunts every 1,200 frames), twice with one seed: all 24 run with no trap, fault, unrecovered call or "not written" path, their triggers fire (3 to 32 one-shot triggers each), the two runs agree frame for frame. Allied 12 and Soviet 8 are lost on the way (the hunt sends the player's few units to die). ctests `mission-soak-ra2-allied`/`-soviet`, label `soak` (native/README.md "Mission soak"). | Real wins are yours. Runs at different game speeds part by design (the speed sets normalized animations' rates; native/README.md "Mission soak"); nothing in play reads the clock. |

## Yuri's Revenge

| Item | State | What is done | What is left |
|---|---|---|---|
| Skirmish stability | Mostly done | A 45-minute game against three hard AIs on the Mac has no fault, and a 10-minute ctest runs. Lane N6: the skirmish lobby's Choose Map stopped the game (its dialog procedure was missing); fixed, ctest `yr-native-choose-map`. | 89 functions still missing at the Mac link (100 before lane N8). Lane N23 checked all 89 from their callers' guards in the units and in gamemd.exe (native/yr/README.md, lane N23): 42 Westwood Online, 3 coop, 1 modem, 17 VQA movies (YR ships Bink), 8 Tiberian Sun only (veins, tunnels, laser fences), 9 editor, debug or recording, 5 unused COM template members, 4 default slots and the version dialog; none is reachable in skirmish, LAN or campaign play with the shipped data. Lane N8: Choose Map's Create Random Map works (the random map generator written as stand-ins; ctest `yr-native-choose-map` makes, previews and plays a random map). Longer and varied games will find more. |
| Campaign trace and forced-win chain | Done | Both campaigns pass a test that forces each win in turn: all 14 missions, both endings, the credits and the return to the main menu. | Real wins are yours. |
| Campaign stand-ins | Partly done | Stand-ins reached in play: Psychic Dominator, Lightning Storm, the beams and waves, the spy, the hijacker, the Lasher tank's drive tracks; lane N3: the trigger actions "create building at", "ion lightning strike" and "ion storm stop", range rings, beacons, the HealthNav/VeterancyNav keys (ctest `yr-native-standins-triggers`), planning mode from the command bar (ctest `yr-native-planning`); lane N23: planned waypoints picked up with the mouse, a path closed into a loop by hand, a press on a node that cannot close one refused (the same ctest). | Bridge repair has a test since lane N4 (`yr-native-bridge-repair`; it found engineers vanishing without a repair, fixed). The War Factory's unload has a test (lane N3, `yr-native-war-factory`; it found vehicles staying inside the factory, fixed). The trace was static (callers of the missing functions). Lane N25 played every mission: all 14, reached through the menus and played 4,000 game frames with scripted orders, twice with one seed, with no trap, fault or unrecovered call and the two runs the same (ctests `mission-soak-yr-allied`/`-soviet`, label `soak`). |
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
   - new in lane N23 (2026-11-21), please check: in Yuri's Revenge's planning mode (the command bar's PlanningMode
     button), with units selected and three or more waypoints planned, clicking the path's first waypoint closes it
     into a loop (the units patrol it after the commit); before, the click planned another waypoint.
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
   - found and fixed by lane N24 (2026-11-21), please listen in a Red Alert 2 game (any platform): looping sounds
     played once and stopped (a Kirov's or terror drone's engine, an oil derrick, burning buildings, the map's ambient
     birds, wind and water); sound.ini's other Control settings (random sample choice, fade in and out) were ignored
     too, and every sound had the lowest priority. They should now loop and play as on the PC.
2. **On the Windows PC:** `C:\ra2\for-user\RA2YRNative`:
   - switch games;
   - a skirmish in each game;
   - a Yuri's Revenge LAN game against the Mac;
   - a Red Alert 2 LAN game on a random map (the host: Choose Map > Create Random Map; the joining side could not start
     one before lane N22's fix, 2026-11-21).
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
3. Reach Red Alert 2's unreached rewrites, most-called first (lane N22: 40 -> 7, scenarios `n22-next`, `n22-desync`, `n22-wetmap`, `n22-load`, `n22-saves`, `n22-tab`, `n22-coop`, `n22-lanrand`, and a bug found: a LAN guest could not start a random map game (SessionClass 0x00672140 compared the data label "RandMap_Sed"), fixed, ctest `native-lan-random-map`; lane N17: 48 -> 40, TechnoClass slot 189 reached with scenario `n17-factory`, ObjectClass slots 50 and 51 (GetDisguise, GetDisguiseHouse) and TechnoClass slots 62 (Exit_Object), 195, 202 and 224 shown unreachable with the shipped rules and maps, DisplayClass slot 43 with no caller; lane N14: 52 -> 48, ObjectClass slot 35 reached with scenario `n14-hover`, slots 93, 111 and 112 shown unreachable with the shipped rules (each call site's receiver read in game.exe); lane N13: 53 -> 52, ObjectClass slot 115 shown unreachable (all 12 calls on technos); lane N11: 80 -> 53, 4 reached with scenarios `n11-exit-audio`, `n11-terrain`, `n11-lan` and `n11-ui`, 23 shown unreachable from game.exe and the shipped rules; lane N10: 88 -> 80, scenarios `n10-leave` and `n10-bar`; lane N9: 91 -> 88, scenarios `n9-bomb` and `n9-ui`; lane N8: 95 -> 91, scenario `n8-seeds`; lane N6: 105 -> 95, scenarios `n6-place`, `n6-sidebar` and `n6-seeds`, `n5-bomb` run; lane N4 started: a coverage scenario `n4-deaths` reached
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
- Fault (user, Duo, 2026-11-19 ~21:30Z): Yuri's Revenge Guardian GIs (GGI) will not deploy; in the first Allied campaign mission they are meant to deploy automatically and instead stand and die. First item for the next port lane.
  Lane N13 (2026-11-19): fixed. Reproduced on the Mac (a GGI ordered to deploy, and the first Allied mission ALL01UMD:
  0 of 10 GGIs deployed, all stuck in the deploy animation). Cause: InfantryClass::MasterDoControls (the infantry
  actions' rates) was sized from TibSun's DO_COUNT (21) and only that much was read from gamemd.exe; YR has 42, so the
  deploy action's rate read 0. The GI and the Desolator had the same fault; MouseClass::MouseControl the same (86
  cursors, 66 read). fixups.json gives both the original's size. Checked: ctest `yr-native-deployers` (GGI, GI, DESO
  deploy and undeploy, SCHP deploys, the mission's 10 GGIs deploy; fails on the old build). Needs a user check on the
  Duo: the first Allied mission's GGIs deploy and fight.
- Pass (user, Duo, 2026-11-19, reported to the owner): Yuri's Revenge Guardian GIs deploy and undeploy (lane N13's fix
  confirmed); the bottom toolbar's (the command bar's) buttons all work.
- Requests (user, Duo, 2026-11-19), lane N18: in Yuri's Revenge (1) the main menu's buttons do not slide out when
  Enhancements is chosen, (2) the Enhancements page lacks most of Red Alert 2's rows, (3) the Internet button should be
  disabled as in Red Alert 2.
  Lane N18 (2026-11-19), please check on the Duo: (1) fixed: the Options column now slides out before the
  Enhancements page shows and back in after it, as in Red Alert 2 (the original's own slide, gamemd.exe 0x00608070);
  (3) fixed: Yuri's Revenge's Internet button is disabled when the main menu opens, as Red Alert 2's (ctest
  `one-game-menu-backdrop` checks both in both games); (2) partly: Map zoom and the zoom row (Pinch zoom on the Duo)
  now work in Yuri's Revenge (glue/yr_map_zoom.cpp, ctest `yr-native-map-zoom`; the gestures zoom between 1x and 3x,
  Red Alert 2's zoom-out below 1x is not carried over). Still Red Alert 2 only: Game interface size and HD vehicles,
  each a separate piece of Yuri's Revenge work (native/yr/README.md "Not shared, and why"). Mod is shown in both games.
- Pass (user, Duo, 2026-11-20, Yuri's Revenge: "Everything else passes on the Duo"): lanes N18-N20's Enhancements
  (Map zoom and the gestures down to 0.5x, Game interface size, HD vehicles), the derricks' and buildings'
  animations, Tab / Delete / loading during explosions, and the on-screen keyboard.
- Faults (user, Duo, 2026-11-20, Yuri's Revenge), lane N21, please check on the Duo:
  (1) Abort Mission with Game interface size above 1x (and Zoom gestures) showed the map small in the screen's top
  left corner with the rest black. Cause: when Main_Loop ended the game only the play layout was dropped, so the
  zoomed game's last frame (its view in a corner of the larger screen) was shown whole, through Do_Abort's wait for
  EVA's "Battle control terminated" (seconds with sound); Red Alert 2 clears it at that point. Fixed: the end of
  Main_Loop, and the abort and restart the in-game menu asks for, clear it as the win and loss did
  (glue/yr_app_hooks.cpp, two fixups; ctest `yr-native-zoom-abort` checks every picture after the menu, fails on the
  old build). Check: Abort Mission at 1.5x with and without Zoom gestures; the screen goes black, then the menus.
  (2) The "Game Over" banner was far too dark. Cause: Init_Game's draft loaded GRFXTXT.PAL without making its 6-bit
  colours 8-bit (gamemd.exe 0x0052C233 does, as for every palette), so the banner was drawn at a quarter of its
  brightness. Fixed (glue/init_game_native.inc; ctest `yr-native-end-banner`: the banner's middle 120 of 255 against
  27 before). Check: lose a skirmish (or a mission); "Game Over" (and a won game's "Mission Accomplished") as bright
  as in Red Alert 2. Red Alert 2 has neither fault (its Init_Game is the matched original's; its game end clears
  the zoomed frame).
- Lane N27 (2026-11-22): YR command bar reopen button under Zoom gestures — validated by the user on the Duo.
- Fault (user, Duo, 2026-11-21): with Menu fill = Scale to fit, Red Alert 2's menus (main menu, Options, Enhancements)
  were letterboxed on all four sides while Yuri's Revenge's filled the screen. Lane N28, please check on the Duo:
  fixed. Cause: with Zoom gestures on, Red Alert 2's screen is twice as large in its map column (render scale 2) and
  the host's render-scale layout showed the menus at that screen's nominal size, skipping Scale to fit
  (host.cpp update_layout_scaled); it now fits the 800x600 menu there as update_layout does (ctest
  `one-game-menu-backdrop`: a 1136x800 window with the phone's settings, Red Alert 2's menu the size of Yuri's
  Revenge's; fails on the old build). Check: Red Alert 2's main menu and Enhancements fill the screen's height as
  Yuri's Revenge's do, with Scale to fit and Pinch zoom on; Off keeps the original look.
