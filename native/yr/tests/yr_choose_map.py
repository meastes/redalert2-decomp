"""ctest yr-native-choose-map (lane N6; built with -DRA2_NATIVE_YR=ON): the skirmish lobby's Choose Map, as a player
uses it: Skirmish, Choose Map (control 0x5AA), a click on a row of the map list (0x553), OK (0x6C5), Start Game.
Lane N8: and its Create Random Map (0x583): Preview (0x620), OK (0x6C5), Start Game on the generated map.

Choose Map's dialog procedure (gamemd.exe 0x005E6920) was missing from the pinned YR tree, a link-closure trap: the
click stopped the game as the dialog was made. glue/netshare_native.inc now has it (text adopted from a later
ra2-decomp).

Two runs: the lobby's own map started at once (harness order `scenario` logs its file), then the map chosen through
the dialog. Checks: both start, the chosen map is another one, no trap or fault.

Lane N8, a third run: Create Random Map stopped the game (the random map generator, MapGeneratorClass, was missing
from the YR tree; glue/mapgen_native.inc). The random map dialog's Preview makes a map and draws it, its OK makes it
again and saves the seed (RandMap.Sed, RandMap.img in the write folder), Start Game plays RANDMAP.SED. Checks: the
seed and preview files written, the game is RANDMAP.SED, the player has units at frame 40 (harness order `list`),
the game plays on to frame 600 (not lost at once), no trap or fault.

  yr_choose_map.py --binary <yr-native> --data-dir <game folder> --work <dir>
Skipped (77) without the installed game (gamemd.exe in --data-dir) and for the Windows build.
"""
import argparse
import importlib
import os
import re
import shutil
import subprocess
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import game_process  # noqa: E402

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
standins = importlib.import_module('yr_standins')
# standins.SCRIPT: Single Player, Skirmish, then (s3) Start Game; here Choose Map comes before it.
LOBBY = standins.SCRIPT.splitlines()[:2]
START = 'click 720 262'
RUNS = {'default': LOBBY + ['s3+2000 ' + START],
        'chosen': LOBBY + ['s3+2000 click @ctl=1450', 's4+3000 click @ctl=1363', 's4+4000 click @ctl=1733',
                           's5+3000 ' + START],
        # Lane N8: Create Random Map (0x583) opens the random map dialog; Preview (0x620) makes a map and draws its
        # preview, OK (0x6C5) accepts it (the map is made again in full) and closes both dialogs; then Start Game.
        'random': LOBBY + ['s3+2000 click @ctl=1450', 's4+3000 click @ctl=1411', 's5+3000 click @ctl=1568',
                           's5+20000 click @ctl=1733', 's6+3000 ' + START]}


ORDERS = {'random': '30 scenario; 40 list; 600 scenario'}


def play(a, name):
    work = os.path.join(a.work, name)
    writes = os.path.join(work, 'w')
    os.makedirs(writes)
    with open(os.path.join(writes, 'RA2MD.INI'), 'w') as f:
        f.write(standins.SETTINGS)
    script = os.path.join(work, 'choose.script')
    with open(script, 'w') as f:
        f.write('\n'.join(RUNS[name]) + '\n')
    env = dict(os.environ, RA2_NATIVE_MOVIE_SKIP='1', RA2_NATIVE_IGNORE_FOCUS='1', RA2_NATIVE_MSG_LOG='0',
               RA2_NATIVE_ORDERS=ORDERS.get(name, '30 scenario'))
    env.pop('RA2_NATIVE_FORCE_END', None)
    seconds = a.random_seconds if name == 'random' else a.seconds
    cmd = [a.binary, '--data-dir', a.data_dir, '--run-to=never', '--run-seconds=%d' % seconds,
           '--write-dir=' + writes, '--input-script=' + script, '--game-args=-noaudio -CD.']
    with open(os.path.join(work, 'run.log'), 'w') as log:
        game_process.run(cmd, env=env, stdout=log, stderr=subprocess.STDOUT, timeout=seconds + 120)
    text = open(os.path.join(work, 'run.log'), errors='replace').read()
    scenario = (re.findall(r'^harness-scenario (\S+)', text, re.M) or [None])[0]
    bad = sorted(set(l for l in text.splitlines() if standins.BAD.search(l)))
    print('%s: scenario %s' % (name, scenario))
    if name == 'random':
        units = len(re.findall(r'^harness-list h\d+p ', text, re.M))
        late = re.search(r'^harness-order 600 scenario', text, re.M) is not None
        files = [f for f in ('RandMap.Sed', 'RandMap.img') if not os.path.exists(os.path.join(writes, f))]
        print('random: %d player units at frame 40, frame 600 %s, files missing %s' % (units, 'reached' if late else 'not reached', files or 'none'))
        bad += ['no player units at frame 40'] if units == 0 else []
        bad += ['the game did not reach frame 600 (lost at once?)'] if not late else []
        bad += ['not written: %s' % ', '.join(files)] if files else []
    return scenario, bad


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--data-dir', default='')
    ap.add_argument('--work', required=True)
    ap.add_argument('--seconds', type=int, default=40)
    ap.add_argument('--random-seconds', type=int, default=90)
    a = ap.parse_args()
    if not a.data_dir or not os.path.exists(os.path.join(a.data_dir, 'gamemd.exe')):
        print('skipped: no Yuri\'s Revenge install (gamemd.exe) in --data-dir')
        return 77
    if a.binary.lower().endswith('.exe'):
        print('skipped: run on the Mac build (the Windows build has the same game code)')
        return 77
    shutil.rmtree(a.work, ignore_errors=True)
    failures = []
    default, bad = play(a, 'default')
    failures += ['default: traps or faults: %s' % '; '.join(bad[:5])] if bad else []
    chosen, bad = play(a, 'chosen')
    failures += ['chosen: traps or faults: %s' % '; '.join(bad[:5])] if bad else []
    random_map, bad = play(a, 'random')
    failures += ['random: %s' % '; '.join(bad[:5])] if bad else []
    if (random_map or '').upper() != 'RANDMAP.SED':
        failures.append('random: the generated map was not played (%s)' % random_map)
    if not default or not chosen:
        failures.append('a game did not start (no harness-scenario line)')
    elif default == chosen:
        failures.append('the map chosen in the dialog was not played (%s both times)' % chosen)
    for f in failures:
        print('FAIL: ' + f)
    print('%s: Choose Map in the skirmish lobby (%s)' % ('FAIL' if failures else 'OK', a.work))
    return 1 if failures else 0


if __name__ == '__main__':
    sys.exit(main())
