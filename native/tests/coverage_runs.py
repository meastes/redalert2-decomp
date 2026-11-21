#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""Coverage runs for the behaviour rewrites (lane C21; after lane C16's scratch scripts).

A coverage build of ra2-native writes the functions a run executed:

  cmake -S native -B build/native-cov \\
      -DCMAKE_C_FLAGS='-fsanitize-coverage=func,trace-pc-guard -fno-inline' \\
      -DCMAKE_CXX_FLAGS='-fsanitize-coverage=func,trace-pc-guard -fno-inline' \\
      -DCMAKE_EXE_LINKER_FLAGS=-fsanitize=undefined
  ninja -C build/native-cov ra2-native

(-fno-inline keeps the small leaves as functions of their own: with inlining a leaf called
directly is folded into its caller and never shows as reached.) The run summary dumps the
coverage (port/src/run_progress.cpp) when UBSAN_OPTIONS=coverage=1:coverage_dir=<dir>.

  coverage_runs.py run --binary B --data-dir D --game-exe E --out DIR [SCENARIO...]
      plays the named scenarios (default: all) one after another, each into DIR/<name>/
      (run.log, cov/*.sancov, w/ for its writes); several at once with --jobs N.
  coverage_runs.py report --binary B --out DIR [--json FILE] [--rows unchecked|all]
      maps each run's PCs to the binary's functions and lists every native/behaviour_rewrites.json
      row with the runs that reached it.
  coverage_runs.py list
      the scenarios.

Scenarios are test scaffolding over the harness switches the README documents
(--skirmish, RA2_NATIVE_ORDERS, RA2_NATIVE_FORCE_END, RA2_NATIVE_SAVE_AT, --load-save,
input scripts through the menus). Every launch goes through game_process. Game files are
read in place; writes go to DIR/<name>/w.
"""
import argparse
import bisect
import collections
import glob
import json
import os
import re
import struct
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import game_process  # noqa: E402

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(HERE))
ROWS = os.path.join(REPO, 'native', 'behaviour_rewrites.json')

NOAUDIO = '--game-args=-noaudio -CD.'
AUDIO = '--game-args=-CD.'

# Menu points at 1024x768 (campaign_chain.py, controls_input.py, the README's menu examples).
SINGLE_PLAYER = 'click 829 302'
NEW_CAMPAIGN = 'click 831 305'
SKIRMISH = 'click 834 388'
START_GAME = 'click 834 346'


def skirmish(name, frames, order='all', extra=(), env=None, seconds=None, audio=False, movies=False, map_='#5',
             script=None, files=None):
    seconds = seconds or max(120, frames // 15 + 60)
    return dict(name=name, args=['--skirmish=' + map_, '--skirmish-frames=%d' % frames, '--skirmish-order=' + order,
                                 '--run-to=frames', '--run-seconds=%d' % seconds, AUDIO if audio else NOAUDIO] + list(extra),
                env=env or {}, seconds=seconds + 60, movies=movies,
                **({'script': script} if script else {}), **({'files': files} if files else {}))


def menus(name, script, seconds, env=None, audio=False, movies=False, extra=()):
    return dict(name=name, args=['--run-to=seconds', '--run-seconds=%d' % seconds, AUDIO if audio else NOAUDIO] + list(extra),
                env=env or {}, seconds=seconds + 60, script=script, movies=movies)


# Each super weapon on an enemy, then a save while their effects (bombs, pulses, the
# iron curtain, radiation) are on the map, then the load of that save.
SUPERS = '; '.join('%d super %d enemy' % (300 + 150 * k, k) for k in range(7))
# The harness's build order puts up the barracks by about frame 1,300 and the war factory by
# about 2,800 (GAPOWR, GAPILE, GAREFN, GAWEAP); each order is repeated in case its factory is late.
SPECIALS = ('1400 train SPY; 1450 train E1; 1500 train E1; 1700 train SPY; 1750 train ENGINEER; '
            '3000 train FV; 3300 train FV; 3700 board E1 FV; 4100 unload FV; 4300 board E1 FV; 4700 hunt')

SCENARIOS = [
    # Audio on (SDL's dummy device) and the movies played: the start-up audio, the audio thread,
    # the VQA player's drawers.
    menus('audio-menu', ['# the intro movie, then the main menu'], 40, audio=True, movies=True),
    skirmish('audio-skirmish', 1500, audio=True, movies=True,
             env=dict(RA2_NATIVE_ORDERS='600 super 0 enemy; 900 super 2 enemy; 1100 wave')),
    # Super weapons, then a save holding their effects, then the game left (SAVE_LEAVE).
    skirmish('supers-save', 1800, order='win', env=dict(RA2_NATIVE_ORDERS=SUPERS, RA2_NATIVE_SAVE_AT='1500,SUPERS.SAV'),
             seconds=400),
    # The Allied specials: spy, IFV with a gunner, engineers.
    skirmish('specials', 5000, order='win', seconds=700, env=dict(
        RA2_NATIVE_ORDERS=SPECIALS, RA2_NATIVE_BUILD_LIST='GAPOWR,GAPILE,GAREFN,GAWEAP,GAPOWR')),
    # Lane N4 (item 3, Red Alert 2's unreached rewrites): a demolition truck spawned and destroyed (UnitClass::Explode
    # shakes the screen), other vehicles spawned and destroyed beside it.
    skirmish('n4-deaths', 1500, order='both', seconds=300, env=dict(RA2_NATIVE_ORDERS=(
        '300 spawn DTRUCK 4 0; 320 spawn APOC 6 2; 340 spawn HTK 6 -2; 400 kill DTRUCK; 600 kill APOC; 700 kill HTK'))),
    # Lane N5: a Crazy Ivan's bomb on the player's own Flak Track (BombClass), a save while it is planted (its Save
    # and GetClassID), the bomb going off; then the laser beams (DSurface::Draw_Depth_Antialiased_Line).
    skirmish('n5-bomb', 1500, order='both', seconds=300, env=dict(RA2_NATIVE_SAVE_AT='520,BOMB.SAV', RA2_NATIVE_ORDERS=(
        '300 spawn IVAN 2 2; 310 spawn HTK 5 0; 330 fire IVAN HTK; 500 health; 700 laser; 900 health'))),
    # Lane N9: BombClass::Disarm, both ways an engineer's capture takes it: a Crazy Ivan bombs the player's engineer and
    # an enemy missile silo spawned beside the base (the harness's `spawn <ID> enemy`), then the engineer captures the
    # silo (`capture <ID> <TYPE>`): the engineer is deleted with its bomb (ObjectClass::Delete_Me) and the silo changes
    # hands with its own (BuildingClass::Captured).
    skirmish('n9-bomb', 1100, order='both', seconds=300, env=dict(RA2_NATIVE_ORDERS=(
        '300 spawn NAMISL enemy -6 2; 310 spawn IVAN -2 2; 312 spawn ENGINEER -2 3; 330 fire IVAN ENGINEER; '
        '420 fire IVAN NAMISL; 520 capture ENGINEER NAMISL; 700 health; 1000 health')),
        # the bombed engineer alone selected and the mouse over it: TechnoClass::What_Action asks the bomb its kind
        # (BombClass::Unknown_00435FF0) for the detonate cursor
        script=['440 click @ENGINEER', '450 move @ENGINEER', '460 move @ENGINEER', '480 move @ENGINEER']),
    # Lane N5: a soldier's production put on hold (a right click on its cameo: SUSPEND), the sidebar's strip asking
    # its factory whether it is on hold (UnknownFactory::Is_On_Hold). Lane N6: it does not: only the sidebar's own
    # click marks a cameo building, and the strip asks only then (n6-sidebar reaches it); kept for the SUSPEND path.
    skirmish('n5-hold', 2400, order='win', seconds=400, env=dict(RA2_NATIVE_BUILD_LIST='GAPOWR,GAPILE,GAREFN',
        RA2_NATIVE_ORDERS='1400 train E1; 1401 train E1; 1420 hold E1; 1800 train E1; 1820 hold E1; 2200 train E1')),
    # Lane N6: buildings spawned for the player: a psychic sensor (its sensor array marks the cells), a missile silo
    # and a gap generator, each selected by a click (the range circle: XSurface::Draw_Ellipse); the nuke fired with
    # the silo standing (the silo's missile mission), the sensor destroyed (the cells unmarked).
    skirmish('n6-place', 1500, order='both', seconds=320, env=dict(RA2_NATIVE_ORDERS=(
        '300 spawn NAPSIS 7 5; 310 spawn NAMISL -7 5; 320 spawn GAGAP 0 8; 600 super 0 enemy; 1100 kill NAPSIS')),
        script=['360 click @NAPSIS', '380 move @NAPSIS+2,0', '420 click @GAGAP', '440 move @GAGAP+2,0',
                 '500 click @NAMISL', '520 move @NAMISL+2,0']),
    # Lane N6: production started by a left click on the barracks' cameo (the strip marks it building and asks
    # its factory whether it is on hold: UnknownFactory::Is_On_Hold), held, resumed; then the defence tab's hot key
    # (KEYBOARD.INI in the run's write folder binds DefenseTab to W) with a pillbox buildable
    # (SetDefenseTabCommandClass::Execute asks for the building waiting for placement, 0x006F8A60).
    skirmish('n6-sidebar', 1400, order='build', seconds=320, env=dict(RA2_NATIVE_BUILD_LIST='GAPOWR,GAPILE'),
             script=['900 click @tab=2', '935 click @cameo=E1', '1000 rclick @cameo=E1',
                     '1060 click @cameo=E1', '1200 key W', '1250 click @tab=0', '1300 key W'],
             files={'KEYBOARD.INI': '[Hotkey]\nDefenseTab=87\n'}),
    # Lane N4: the clicks the in-game runs did not make: a Ctrl+Alt click (the area-guard cursor), a waypoint path
    # that comes back to its first waypoint (waypoint mode's path cell), a Shift click and release on a selected unit.
    dict(name='n4-ui', args=['--skirmish=#5', '--skirmish-frames=900', '--skirmish-order=both', '--run-to=frames',
                             '--run-seconds=200', NOAUDIO], env={}, seconds=260, script=[
        '60 drag 150 150 750 650',
        '80 keydown Left_Ctrl', '81 keydown Left_Option', '83 click 520 430', '86 keyup Left_Option', '87 keyup Left_Ctrl',
        '100 drag 150 150 750 650', '110 key Z', '115 click 300 300', '120 click 360 320', '125 click 420 300',
        '130 click 300 300', '135 click 300 300', '140 key Z',
        '160 drag 150 150 750 650', '170 keydown Left_Shift', '172 click 450 400', '175 keyup Left_Shift',
        '190 keydown Left_Option', '192 click 450 400', '195 keyup Left_Option',
    ]),
    # Lane N9: the type-select key held (T: TypeSelectCommandClass::Execute starts its stopwatch, the flag 0x00AC133D)
    # while a Shift click lands on a tank: on an unselected one Mouse_Left_Release selects every tank (Unknown_006F71E0),
    # on a selected one (action 8) it unselects every tank (Unknown_006F7020). The input log shows 5 tanks selected
    # after the first click and none after the second.
    dict(name='n9-ui', args=['--skirmish=#5', '--skirmish-frames=600', '--skirmish-order=both', '--run-to=frames',
                             '--run-seconds=200', NOAUDIO], env=dict(RA2_NATIVE_INPUT_LOG='10'), seconds=260, script=[
        '80 keydown T', '82 keydown Left_Shift', '84 click @MTNK', '88 keyup Left_Shift', '90 keyup T',
        '120 keydown T', '122 keydown Left_Shift', '124 click @MTNK', '128 keyup Left_Shift', '130 keyup T',
    ]),
    # Lane N10: the command bar's buttons (an input script's "@cmd=NAME", link/team_button_hook.cpp) and hover cursors.
    # A tank selected; Ctrl+Alt held with the mouse resting on open ground (force fire and force move: guard area,
    # action 0x1A, its cursor Unknown_006F6740; reached); Delete (DeleteCommandClass::Execute asks for the planning
    # mode's node, unknown_find_617a00; reached). Not reached: the bar's Attack Move and Beacon (UI.INI's single-player
    # [AdvancedCommandBar] has only Deploy, Guard, PlanningMode, TypeSelect and the teams; the run logs "no command bar
    # button": lan_pair.py --extras presses the multiplayer bar's Beacon), and waypoint mode (Z) with the mouse on an
    # empty cell (HouseClass::Can_Add_Waypoint_To_Path: not asked; not traced further).
    dict(name='n10-bar', args=['--skirmish=#5', '--skirmish-frames=700', '--skirmish-order=both', '--run-to=frames',
                               '--run-seconds=240', NOAUDIO], env=dict(RA2_NATIVE_INPUT_LOG='10'), seconds=300, script=[
        '80 click @MTNK', '90 click @cmd=AttackMove', '95 move @MTNK+4,0', '100 move @MTNK+5,0', '106 move @MTNK+5,1',
        '112 rclick @MTNK+5,1',
        '130 click @MTNK', '140 keydown Left_Ctrl', '141 keydown Left_Option', '143 move @MTNK+4,1', '147 move @MTNK+5,1',
        '152 move @MTNK+5,2', '156 keyup Left_Option', '157 keyup Left_Ctrl',
        '200 click @cmd=Beacon', '210 move @MTNK+3,3', '215 click @MTNK+3,3', '260 key Delete',
        '300 click @MTNK', '310 key Z', '320 click @MTNK+4,0', '330 move @MTNK+6,2', '336 move @MTNK+7,2',
        '342 move @MTNK+7,3', '350 key Z', '400 key Delete',
    ]),
    # Lane N11: terrain objects (the harness's `super <index> terrain [mine]` and `terrain kill [mine]`): the Iron
    # Curtain (index 1) over the tree nearest the base reaches the terrain object in the cell's occupier list
    # (ObjectClass::IronCurtain, the default TerrainClass keeps; reached). The ore mine and the tree then take no
    # damage: the ore mines are Immune=yes and the C4 warhead is not Wood (so Cell::Chain_Reaction_Damage, which a
    # destroyed ore mine would set off, stays unreached: unreachable with the shipped rules).
    skirmish('n11-terrain', 1000, order='both', seconds=300, env=dict(RA2_NATIVE_ORDERS=(
        '300 super 1 terrain; 600 terrain kill mine; 700 terrain kill; 800 health'))),
    # Lane N11: the main menu's Exit Game with audio on: the audio shuts down and frees its cache
    # (AudioSource::Finish).
    menus('n11-exit-audio', ['s1+1000 click 834 640', 'd2+1500 click 660 485'], 60, audio=True),
    # Lane N11: a tank that can attack on the move (MTNK: OpportunityFire) selected, Ctrl+Shift held (the 0x00A40BAC and
    # 0x00A40BB4 key pairs, which shared/keyboard.hpp calls force fire and force move; Ctrl+Alt is guard area) with the
    # mouse resting on open ground: TechnoClass::What_Action drops both and answers a move (1), which becomes
    # attack-move's 0x3E while 0x006F6670 holds (Unknown_006D8F60), whose cursor is 0x006F6730's. Then the same over
    # an enemy tank spawned beside it (an input script's "@!ID", harness_input.cpp: attack 5 becomes 0x3F).
    dict(name='n11-ui', args=['--skirmish=#5', '--skirmish-frames=500', '--skirmish-order=both', '--run-to=frames',
                              '--run-seconds=200', NOAUDIO], env=dict(RA2_NATIVE_INPUT_LOG='10', RA2_NATIVE_ORDERS='84 spawn HTNK enemy 4 2'),
         seconds=260, script=[
        '80 click @MTNK', '90 keydown Left_Ctrl', '91 keydown Left_Shift', '93 move @MTNK+3,0', '97 move @MTNK+4,1',
        '101 move @MTNK+4,2', '105 move @!HTNK', '109 move @!HTNK', '113 keyup Left_Shift', '114 keyup Left_Ctrl',
    ]),
    # Lane N14: the mouse resting on a tree and then on an ore mine (both TerrainClass objects in their cell's occupier
    # list) with a tank selected, the view centred on each by the harness's `terrain view [mine]`: the tooltip's
    # DisplayClass::Help_Text asks the object under the mouse for its name (ObjectClass slot 35, "No name", the default
    # TerrainClass keeps) and the tank's TechnoClass::What_Action(ObjectClass *) answers for it.
    dict(name='n14-hover', args=['--skirmish=#5', '--skirmish-frames=420', '--skirmish-order=both', '--run-to=frames',
                                 '--run-seconds=200', NOAUDIO],
         env=dict(RA2_NATIVE_INPUT_LOG='20', RA2_NATIVE_ORDERS='90 terrain view; 250 terrain view mine'), seconds=260, script=[
        '80 click @MTNK', '100 move @terrain', '104 move @terrain', '108 move @terrain',
        '260 move @terrain=mine', '264 move @terrain=mine', '268 move @terrain=mine',
    ]),
    # Lane N17: a war factory spawned for the player and selected, Alt (Options' KeyForceMove, VK_MENU) held with the
    # mouse over open ground and then over the player's tank: BuildingClass::What_Action (the cell's, 0x00443BA0, and
    # the object's, slot 28 0x004438A0) asks TechnoClass::Is_In_Same_Zone on the factory, which asks the factory's
    # TechnoClass slot 189 (0x00453BA0, the default BuildingClass keeps).
    dict(name='n17-factory', args=['--skirmish=#5', '--skirmish-frames=500', '--skirmish-order=both', '--run-to=frames',
                                   '--run-seconds=200', NOAUDIO],
         env=dict(RA2_NATIVE_INPUT_LOG='10', RA2_NATIVE_ORDERS='84 spawn GAWEAP 7 4'), seconds=260, script=[
        '120 click @GAWEAP', '130 keydown Left_Option', '133 move @GAWEAP+5,0', '137 move @GAWEAP+6,1',
        '141 move @GAWEAP+6,2', '145 move @MTNK', '149 move @MTNK', '153 keyup Left_Option',
    ]),
    # Lane N22: the Next Object and Previous Object keys (KEYBOARD.INI in the run's write folder binds them to N and
    # M): DisplayClass::Next_Object and Prev_Object ask every object of the ground, air and top layers whether it is
    # the player's army (ObjectClass slot 24); the trees and animations there keep the default, ObjectClass::
    # Is_Players_Army, and are passed over.
    skirmish('n22-next', 500, order='both', seconds=200,
             script=['100 key N', '110 key N', '120 key N', '130 key M', '140 key M', '150 key N'],
             files={'KEYBOARD.INI': '[Hotkey]\nNextObject=78\nPreviousObject=77\n'}),
    # Lane N24: a sound that loops forever out of earshot and back. A terror drone spawned beside the base drives to
    # and fro (its MoveSound, TerrorDroneMoveLoop: Control=loop); the view is moved 40 cells away (out of the sound's
    # range: Voc_Update_At 0x007141B0 stops the event) and back, where the handle has a looping type and no event:
    # a new event is made and flagged, Audio_Event_Set_Flag_8. Audio on. Before lane N24's data fix no sound had
    # its Control keywords (the keyword table was read as one pointer), so nothing looped and this never ran.
    skirmish('n24-earshot', 1200, order='deploy', audio=True, seconds=300,
             env=dict(RA2_NATIVE_ORDERS='50 spawn DRON; 60 view GACNST 0 0; 100 goto DRON ~-10 ~0; 180 goto DRON ~10 ~0; 260 goto DRON ~-10 ~0; 340 goto DRON ~10 ~0; 420 goto DRON ~-10 ~0; 500 goto DRON ~10 ~0; 580 goto DRON ~-10 ~0; 660 goto DRON ~10 ~0; 740 goto DRON ~-10 ~0; 820 goto DRON ~10 ~0; 900 goto DRON ~-10 ~0; 980 goto DRON ~10 ~0; 1060 goto DRON ~-10 ~0; 300 view GACNST 40 -40; 600 view GACNST 0 0; 800 view GACNST 40 -40; 1000 view GACNST 0 0')),
    # Lane N4: the skirmish's Choose Map (control 0x5AA), its Create Random Map (0x583), the random map dialog's
    # Preview (0x620: the map generated) and its accept (0x6C5).
    menus('n4-randmap', ['s1+1500 ' + SINGLE_PLAYER, 's2+1500 ' + SKIRMISH, 's3+2500 click @ctl=1450',
                         's4+2500 click @ctl=1411', 's5+3000 click @ctl=1568', 's5+40000 click @ctl=1733', 's6+3000 ' + START_GAME], 200),
    # Lane N22: the random map dialog's Surprise Me (0x621: a random map type 1-4 among the rest) before Preview: a wet
    # map type (3 or 4, half the runs) takes MapGeneratorClass's wet-map steps (MapRegionClass::Make_Ice_Regions,
    # Split_Region).
    menus('n22-wetmap', ['s1+1500 ' + SINGLE_PLAYER, 's2+1500 ' + SKIRMISH, 's3+2500 click @ctl=1450',
                         's4+2500 click @ctl=1411', 's5+3000 click @ctl=1569', 's5+4500 click @ctl=1568',
                         's5+40000 click @ctl=1733', 's6+3000 ' + START_GAME], 120),
    # Lane N22: the single-player menu's Load Saved Game (Select_Game: LoadOptionsClass::Load_Dialog_0053b190, Dialog
    # 0x0053B770 with the load style: the title from LoadOptionsClass::Load_Menu_Name, slot 5), then its Back.
    menus('n22-load', ['s1+1500 ' + SINGLE_PLAYER, 's2+1500 click 834 346', 's3+3000 click @ctl=1670'], 60),
    # Lane N22: the in-game menu's Save Game (0x51F: the save dialog, LoadOptionsClass::Save_Menu_Name; its Save 0x6C7
    # and the message's OK 0x5AE: Saved_Message), then Delete Game (0x520: Delete_Menu_Name; the list's first row
    # 0x528, Delete 0x6C8 and the question's OK).
    skirmish('n22-saves', 900, order='both', seconds=300, script=[
        '100 key Escape', 'd1+2500 click @ctl=1311', 'd2+3000 click @ctl=1735', 'd3+2000 click @ctl=1454',
        '500 key Escape', 'd4+2500 click @ctl=1312', 'd5+3000 click @ctl=1320', 'd5+4000 click @ctl=1736',
        'd6+2000 click @ctl=1454']),
    # Lane N22: Tab in the LAN lobby (the main menu's Network): the owner-drawn edit line with the focus sends the
    # dialog 0x4DB, and Join_Dialog_Proc moves the focus to the next tab stop (0x005DEF80).
    menus('n22-tab', ['s1+1000 click 834 388', 's2+2500 click @ctl=1344', 's2+3000 key Tab', 's2+3500 key Tab',
                      's2+4000 click @ctl=1344', 's2+4500 keydown Left_Shift', 's2+4600 key Tab', 's2+4800 keyup Left_Shift'], 40),
    # Lane N6: the random map dialog's Save seed button (0x6C3) after a preview opens the seed list's save dialog
    # (MapSeedClass, a LoadOptionsClass kind: MapSeedClass::Save_Menu_Name). It saved nothing: 0x527 is the dialog's
    # list and 0x686 its Back (lane N8; n8-seeds below saves, loads and deletes a seed).
    menus('n6-seeds', ['s1+1500 ' + SINGLE_PLAYER, 's2+1500 ' + SKIRMISH, 's3+2500 click @ctl=1450',
                       's4+2500 click @ctl=1411', 's5+3000 click @ctl=1568', 's5+40000 click @ctl=1731',
                       's6+3000 click @ctl=1319', 's6+6000 click @ctl=1670'], 200),
    # Lane N8: why n6-seeds saved nothing: 0x527 (1319) is the save dialog's list (a click there copies a row's
    # description into the edit field) and 0x686 (1670) its Cancel; Save is 0x6C7 (1735, LoadOptionsClass::
    # Save_Dialog_On_WM_COMMAND). Here: the list, Save (MapSeedClass::Save_File, Saved_Message); then the random map
    # dialog's Load seed (0x6C2: the load dialog, Load_Menu_Name; its list 0x525, its Load 0x40F) and Delete seed (0x6C4:
    # the delete dialog, Delete_Menu_Name; its list 0x528, its Delete 0x6C8 and the question's OK 0x5AE: MapSeedClass::
    # Delete_File). The save shows a message (Saved_Message, s8) that its OK (0x5AE) closes.
    menus('n8-seeds', ['s1+1500 ' + SINGLE_PLAYER, 's2+1500 ' + SKIRMISH, 's3+2500 click @ctl=1450',
                       's4+2500 click @ctl=1411', 's5+3000 click @ctl=1568', 's5+40000 click @ctl=1731',
                       's6+3000 click @ctl=1319', 's6+5000 click @ctl=1735',
                       's8+1500 click @ctl=1454', 's8+4000 click @ctl=1730', 's9+2000 click @ctl=1317',
                       's9+3500 click @ctl=1039', 's9+45000 click @ctl=1732', 's10+2000 click @ctl=1320',
                       's10+3500 click @ctl=1736', 's11+1500 click @ctl=1454'], 200),
    # Seven computer opponents on a four-player map, longer: their specials and super weapons.
    skirmish('ai-crowd', 9000, order='all', map_='#10', seconds=900,
             env=dict(RA2_NATIVE_SKIRMISH_AI='3', RA2_NATIVE_SKIRMISH_AI_LEVEL='2')),
    # The shell's option pages (controls at 1024x768, RA2_NATIVE_MSG_LOG): the Options page's
    # check boxes, sliders and resolution combo (opened and closed), Keyboard (its category
    # combo, a command), Network (Cancel), back to the main menu; Movies and Credits: Sneak
    # Peeks, Play Movies, View Credits.
    menus('options', [
        's1+1000 click 834 472',
        's2+800 click 235 437', 's2+1200 click 235 413', 's2+1600 click 235 461',
        's2+2000 click 300 228', 's2+2400 click 600 440', 's2+2800 click 380 334',
        's2+3200 click 300 563', 's2+3600 click 450 563', 's2+4000 click 600 563',
        's2+4400 click 620 228', 's2+5400 click 620 228',
        's2+6000 click 834 304',
        's3+1000 click 404 236', 's3+2000 click 300 270', 's3+3000 click 500 260', 's3+3500 click 500 280',
        's3+4500 click 834 640',
        's4+1000 click 834 346', 's5+1500 click 834 640',
        's6+1000 click 834 640',
        's7+1000 click 834 430', 's8+1000 click 834 304', 's9+1500 click 834 640',
        's10+1000 click 834 346', 's11+1500 click 300 240', 's11+2500 click 834 640',
        's12+1000 click 834 388',
    ], 150),
    # Choose Map's game types, each row clicked in turn, then the third used for a skirmish that
    # a forced win ends (the score screen).
    menus('modes', ['s1+1000 click 834 304', 's2+1000 click 834 388', 's3+1500 click 834 388'] +
          ['s4+%d click 300 %d' % (1000 + 700 * k, 280 + 18 * k) for k in range(14)] +
          ['s4+11000 click 300 316', 's4+11800 click 500 290', 's4+12600 click 834 304', 's5+1500 click 834 346',
           'd6+3000 click 831 640'],
          240, env=dict(RA2_NATIVE_FORCE_END='800')),
    # The in-game shell: right button, team keys, waypoint mode, chat, cheer, beacon and sidebar
    # tab keys, then the in-game menu's Save (a typed description), Load and Delete.
    dict(name='ingame-ui', args=['--skirmish=#5', '--skirmish-frames=2400', '--skirmish-order=both', '--run-to=frames',
                                 '--run-seconds=300', NOAUDIO], env={}, seconds=360, script=[
        '60 drag 200 200 700 600', '70 keydown LCtrl', '71 key 1', '72 keyup LCtrl', '80 key 1',
        '90 rdown 500 400', '96 rup 500 400', '100 rclick 450 380',
        '110 key Z', '115 click 300 300', '120 click 350 320', '125 click 400 300', '130 key Z',
        '140 key Return', '145 key H', '146 key I', '150 key Return',
        '160 keydown LCtrl', '161 key C', '162 keyup LCtrl', '170 key C',
        '180 key B', '185 click 420 420', '190 key Q', '195 key W', '200 key E', '205 key R',
        '210 key Tab', '215 key Home', '220 key P', '225 key Delete', '230 key F1', '235 key F2',
        '400 key Escape', 'd1+1500 click 940 239', 'd2+1500 click 834 640',
        '600 key Escape', 's3+1500 click 940 289', 's4+1000 click 400 520', 's4+1500 key A', 's4+1700 key B',
        's4+2000 key Left', 's4+2200 key Delete', 's4+2400 key C', 's4+2800 click 940 239', 's7+1000 click 660 517',
        '1200 key Escape', 's8+1500 click 940 264', 's9+1500 click 400 240', 's9+2500 click 940 239',
    ]),
    # The in-game commands with audio on: a band selection, Ctrl+1 then 1 twice (team, reselect),
    # T (type select), G (guard), Ctrl+Shift click (attack move), the right button, waypoint mode,
    # chat, cheer, beacon, the diplomacy screen (Tab) and its Return, a cameo built, held and
    # cancelled (right clicks), the window losing and regaining the focus.
    dict(name='ingame-keys', args=['--skirmish=#5', '--skirmish-frames=1200', '--skirmish-order=both', '--run-to=frames',
                                   '--run-seconds=420', AUDIO], env={}, seconds=480, movies=True, script=[
        '60 drag 150 150 750 650', '70 keydown Left_Ctrl', '71 key 1', '72 keyup Left_Ctrl', '80 key 1', '85 key 1',
        '90 key T', '95 key T', '100 key G',
        '110 drag 150 150 750 650', '120 keydown Left_Ctrl', '121 keydown Left_Shift', '123 click 500 420',
        '127 keyup Left_Shift', '128 keyup Left_Ctrl',
        '135 rdown 500 400', '141 rup 500 400', '145 rclick 450 380',
        '150 key Z', '155 click 300 300', '160 click 350 320', '165 click 400 300', '170 key Z', '172 key Return',
        '180 keydown Left_Ctrl', '181 key C', '182 keyup Left_Ctrl', '185 key C', '190 key B', '193 click 420 420',
        '200 key Tab', 'd1+2500 click 940 714',
        '300 click 908 252', '330 rclick 908 252', '360 click 908 252', '390 rclick 908 252', '392 rclick 908 252',
        '420 blur', '440 focus', '460 key Return', '462 key H', '463 key I', '466 key Return',
        '480 key Delete', '490 key Home', '500 key Space', '510 key F5', '520 key F6', '530 key F7', '540 key F8',
    ]),
    # The main menu's Exit Game and its confirmation: the game's own end (Prog_End).
    menus('exit-game', ['s1+1000 click 834 640', 'd2+1500 click 660 485'], 60),
    # A LAN game of two instances on this Mac (lan_pair.py): the lobby, the host and join screens,
    # the game's network frames (CRC, events), the in-game menus of a network game.
    dict(name='lanpair', harness='lan_pair.py', args=['--binary', '{binary}', '--data-dir', '{data}', '--game-exe', '{exe}',
                                                      '--work', '{work}', '--frames', '4000'], env={}, seconds=900),
    # Lane N10: a player leaving a LAN game. In lan_pair.py --background, Bravo (as the iOS app) leaves the game when its
    # background time runs low (the EXIT event); Alpha takes the EXIT (Execute_DoList) and plays on: Destroy_Connection
    # 0x005BB5D0 removes Bravo from the session's player list (SessionClass::Unknown_00672F20).
    # --extras: in the game the host's command bar Beacon (Unknown_006F64B0), a beacon placed, its text line closed,
    # Delete over it.
    dict(name='n10-leave', harness='lan_pair.py', args=['--background', '--extras', '--binary', '{binary}', '--data-dir', '{data}',
                                                        '--game-exe', '{exe}', '--work', '{work}', '--frames', '9000'],
         env={}, seconds=1800),
    # Lane N11: lan_pair.py --extras without the background run, shorter: the host's Choose Map before Bravo joins
    # (Scenario_Select_Callback in a LAN game: 0x005BBD10) and the command bar's Attack Move (0x006F6730's cursor).
    dict(name='n11-lan', harness='lan_pair.py', args=['--extras', '--binary', '{binary}', '--data-dir', '{data}',
                                                      '--game-exe', '{exe}', '--work', '{work}', '--frames', '4000'],
         env={}, seconds=900),
    # Lane N22: a LAN game made to part (lan_pair.py --desync: Alpha alone gets a tank at frame 1200): the CRC check
    # of Execute_DoList finds the frames differ, Print_CRCs writes SYNC<n>.TXT (every object's Compute_CRC through
    # WWCRCEngine, the build name Build_By_String_00737b00) and the out-of-sync box comes up.
    dict(name='n22-desync', harness='lan_pair.py', args=['--desync', '1200', '--hd', '0', '--binary', '{binary}', '--data-dir',
                                                         '{data}', '--game-exe', '{exe}', '--work', '{work}', '--frames', '3000'],
         env={}, seconds=900),
    # Lane N22: a LAN game on a generated random map (lan_pair.py --randmap: the host makes it after Bravo has joined).
    dict(name='n22-lanrand', harness='lan_pair.py', args=['--randmap', '--hd', '0', '--binary', '{binary}', '--data-dir',
                                                          '{data}', '--game-exe', '{exe}', '--work', '{work}', '--frames', '3000'],
         env={}, seconds=1200),
    # Lane N22: a LAN game of the Cooperative game type (lan_pair.py --mode 2, the host's game type list's third row):
    # the mode object's slots (MPCooperative). Rows 3, 4 and 7 gave Megawealth, Duel and a Battle map: Unholy Alliance
    # was not found in the LAN list.
    dict(name='n22-coop', harness='lan_pair.py', args=['--mode', '2', '--hd', '0', '--binary', '{binary}', '--data-dir',
                                                       '{data}', '--game-exe', '{exe}', '--work', '{work}', '--frames', '2000'],
         env={}, seconds=900),
    # Lane N24: a LAN game of the Unholy Alliance game type (lan_pair.py --mode 3, the game type list's fourth row at
    # 16 pixels a row): at Start Game the host asks UnholyAlliance::CheckStartConditions (0x005AC390: bases on and the
    # base mode's check); both houses get an MCV of each side (UnholyAlliance::SpawnBaseUnits).
    dict(name='n24-unholy', harness='lan_pair.py', args=['--mode', '3', '--hd', '0', '--binary', '{binary}', '--data-dir',
                                                         '{data}', '--game-exe', '{exe}', '--work', '{work}', '--frames', '1500'],
         env={}, seconds=900),
    # The game binary's own self tests (test scaffolding, not a game path): the VQA player's
    # reset around a mod's .VQA movie and the drawing checks.
    dict(name='vqa-selftest', args=[], env=dict(RA2_NATIVE_VQA_SELFTEST='1'), seconds=120),
    dict(name='draw-selftest', args=[], env=dict(RA2_NATIVE_DRAW_SELFTEST='1'), seconds=120),
]
# The original game's saves in the installation (lane C16's load runs).
for save in ('ALL01T.sav', 'ALL02S.sav', 'ALL03U.sav', 'SAVE397A.SAV', 'SAVE4FBD.SAV'):
    SCENARIOS.append(dict(name='load-' + save.split('.')[0], args=['--load-save=' + save, '--run-to=frames', '--frames=1500',
                                                                 '--run-seconds=300', NOAUDIO], env={}, seconds=360))
# Each side's campaign through the menus, every mission won by a forced end (campaign_chain.py).
for side, emblem in (('allied', '433 179'), ('soviet', '429 448')):
    SCENARIOS.append(menus('camp-' + side, ['s1+1500 ' + SINGLE_PLAYER, 's2+1500 ' + NEW_CAMPAIGN, 's3+2000 click ' + emblem] +
                           ['d%d+3000 click 831 640' % (4 + k) for k in range(12)], 580,
                           env=dict(RA2_NATIVE_FORCE_END=','.join(str(300 + 600 * k) for k in range(12)))))
BY_NAME = {s['name']: s for s in SCENARIOS}


def play(s, a):
    d = os.path.join(a.out, s['name'])
    for sub in ('cov', 'w'):
        os.makedirs(os.path.join(d, sub), exist_ok=True)
    for f in glob.glob(os.path.join(d, 'cov', '*.sancov')):
        os.remove(f)
    env = dict(os.environ)
    env.update(RA2_GAME_EXE=a.game_exe, RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_DIALOG_LOG='1',
               UBSAN_OPTIONS='coverage=1:coverage_dir=' + os.path.join(d, 'cov'))
    if not s.get('movies'):
        env['RA2_NATIVE_MOVIE_SKIP'] = '1'
    env.update(s['env'])
    cmd = [a.binary, '--data-dir', a.data_dir, '--write-dir=' + os.path.join(d, 'w')] + s['args']
    if 'harness' in s:      # another test harness that starts the games itself (its children inherit UBSAN_OPTIONS)
        cmd = [sys.executable, '-B', os.path.join(HERE, s['harness'])] + [
            x.format(binary=a.binary, data=a.data_dir, exe=a.game_exe, work=os.path.join(d, 'w')) for x in s['args']]
    for rel, text in s.get('files', {}).items():     # files the run reads ahead of the installation's (lane N6)
        with open(os.path.join(d, 'w', rel), 'w') as f:
            f.write(text)
    if 'script' in s:
        path = os.path.join(d, 'input.script')
        with open(path, 'w') as f:
            f.write('\n'.join(s['script']) + '\n')
        cmd.append('--input-script=' + path)
    for k, v in s.get('cmd_env', {}).items():
        env[k] = v
    t0 = time.monotonic()
    # The binary's functions as this run has them: a rebuilt binary moves every address, so the
    # report maps this run's counters with this table, not the binary's later one.
    with open(os.path.join(d, 'functions.txt'), 'w') as f:
        f.write(subprocess.run(['nm', '-n', '-U', a.binary], capture_output=True, text=True).stdout)
    with open(os.path.join(d, 'run.log'), 'w') as log:
        log.write('# ' + ' '.join(cmd) + '\n# env ' + ' '.join('%s=%s' % kv for kv in s['env'].items()) + '\n')
        log.flush()
        try:
            p = game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=s['seconds'])
            code = p.returncode
        except subprocess.TimeoutExpired:
            code = 'timeout'
    text = open(os.path.join(d, 'run.log'), errors='replace').read()
    dumps = len(glob.glob(os.path.join(d, 'cov', '*.sancov')))
    stop = (re.findall(r'run-to: .*stopped.*', text) or [''])[0][:100]
    return '%s: exit %s, %d s, %d unrecovered, %d dumps %s' % (
        s['name'], code, time.monotonic() - t0, text.count('unrecovered'), dumps, stop)


def functions(binary=None, table=None):
    nm = (open(table).read() if table else
          subprocess.run(['nm', '-n', '-U', binary], capture_output=True, text=True).stdout).splitlines()
    syms = []
    for line in nm:
        p = line.split(None, 2)
        if len(p) == 3 and p[1] in 'tT':
            syms.append((int(p[0], 16), p[2]))
    return syms


def reached(binary, out):
    """(the binary's functions, {function: runs}): each run's counters mapped with the function
    table saved with it (a run without one is left out: its binary may have been rebuilt)."""
    syms = functions(binary)
    runs = collections.defaultdict(set)
    tables = {}
    for f in sorted(glob.glob(os.path.join(out, '*', 'cov', '*.sancov'))):
        run = f.split(os.sep)[-3]
        table = os.path.join(out, run, 'functions.txt')
        if not os.path.isfile(table):
            print('report: %s left out (no functions.txt: run it again)' % run, file=sys.stderr)
            continue
        if run not in tables:
            t = functions(table=table)
            tables[run] = (t, [x for x, _ in t])
        t, addrs = tables[run]
        b = open(f, 'rb').read()
        for pc in set(struct.unpack_from('<%dQ' % ((len(b) - 8) // 8), b, 8)):
            i = bisect.bisect_right(addrs, pc) - 1
            # Every name at that address (lane N1): the link's aliases (alias_list.txt, e.g.
            # Planning_Mode_Active_614840 = Planning_Active) share one address, and nm lists one
            # of them last; a row named by another alias was reported as not reached.
            if i < 0:
                continue
            j = bisect.bisect_left(addrs, addrs[i])
            while j < len(addrs) and addrs[j] == addrs[i]:
                runs[t[j][1]].add(run)
                j += 1
    return syms, runs


def row_symbols(rows, syms):
    """Each row's candidate mangled symbols: its `symbol`, else the demangled base name, else a
    function whose name carries the row's address."""
    mangled = sorted({m for _, m in syms})
    dem = subprocess.run(['c++filt'], input='\n'.join(mangled), capture_output=True, text=True).stdout.split('\n')
    base_of = collections.defaultdict(set)
    addr_of = collections.defaultdict(set)
    for m, d in zip(mangled, dem):
        base_of[d.split('(')[0].strip()].add(m)
        for h in re.findall(r'(?<![0-9a-fA-F])0{0,2}([0-9a-fA-F]{6})(?![0-9a-fA-F])', d):
            addr_of[h.lower()].add(m)
    have = set(mangled)
    out = []
    for r in rows:
        got = set()
        if r.get('symbol'):
            got = {r['symbol']} & have or {'_' + r['symbol']} & have
        if not got:
            for part in re.split(r'\s*/\s*', r['name']):
                n = re.sub(r'\s*\(.*$', '', part).strip()
                got |= base_of.get(n, set())
        e = r.get('entry')
        if not got and isinstance(e, str) and e.startswith('0x'):
            got = set(addr_of.get('%06x' % int(e, 16), set()))
        out.append(got)
    return out


def report(a):
    rows = json.load(open(ROWS))['rows']
    syms, runs = reached(a.binary, a.out)
    cand = row_symbols(rows, syms)
    result = []
    earlier = collections.defaultdict(set)
    for path in a.merge:
        for r in json.load(open(path)):
            earlier[(r['entry'], r['name'])].update(r['reached'])
    for r, c in zip(rows, cand):
        hit = sorted({x for s in c for x in runs.get(s, ())} | earlier.get((r.get('entry'), r['name']), set()))
        unchecked = not r.get('behaviour_verdict') and not r.get('test')
        if a.rows == 'unchecked' and not unchecked:
            continue
        result.append(dict(entry=r.get('entry'), name=r['name'], file=r['file'], symbols=sorted(c), reached=hit,
                           verdict=r.get('behaviour_verdict', '')[:60]))
    n_hit = sum(1 for x in result if x['reached'])
    for x in result:
        if x['reached'] or a.all:
            print('%-12s %-60s %s' % (x['entry'], x['name'][:60], ','.join(x['reached']) or '-'))
    print('%d rows, %d reached, %d not reached, %d without a symbol' % (
        len(result), n_hit, len(result) - n_hit, sum(1 for x in result if not x['symbols'])), file=sys.stderr)
    if a.json:
        json.dump(result, open(a.json, 'w'), indent=1)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest='cmd', required=True)
    r = sub.add_parser('run')
    r.add_argument('--binary', required=True)
    r.add_argument('--data-dir', default=os.environ.get('RA2_NATIVE_CENSUS_DATA_DIR', ''))
    r.add_argument('--game-exe', default=os.environ.get('RA2_NATIVE_CENSUS_GAME_EXE', ''))
    r.add_argument('--out', required=True)
    r.add_argument('--jobs', type=int, default=1)
    r.add_argument('scenarios', nargs='*')
    p = sub.add_parser('report')
    p.add_argument('--binary', required=True)
    p.add_argument('--out', required=True)
    p.add_argument('--json', default='')
    p.add_argument('--rows', choices=('unchecked', 'all'), default='unchecked')
    p.add_argument('--all', action='store_true', help='list the rows not reached too')
    p.add_argument('--merge', action='append', default=[], help='an earlier report (--json) whose runs are added')
    sub.add_parser('list')
    a = ap.parse_args()
    if a.cmd == 'list':
        for s in SCENARIOS:
            print('%-16s %s %s' % (s['name'], ' '.join(s['args']), ' '.join('%s=%s' % kv for kv in s['env'].items())))
        return 0
    if a.cmd == 'report':
        report(a)
        return 0
    names = a.scenarios or [s['name'] for s in SCENARIOS]
    bad = [n for n in names if n not in BY_NAME]
    if bad:
        ap.error('unknown scenario(s): ' + ', '.join(bad))
    with ThreadPoolExecutor(max_workers=max(1, a.jobs)) as pool:
        for line in pool.map(lambda n: play(BY_NAME[n], a), names):
            print(line, flush=True)
    return 0


if __name__ == '__main__':
    sys.exit(main())
