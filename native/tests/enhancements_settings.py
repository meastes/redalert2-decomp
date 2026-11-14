#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-enhancements-settings (lane C7): the Enhancements page's rows persist and
ORIGINAL resets them (tests/enhancements_settings.cpp), in two processes so the second
is a fresh start reading the settings file the first wrote. A Windows build's .exe runs
under Wine (tests/wine.py), skipped (77) without one.

  enhancements_settings.py --binary <native-enhancements-settings> --work <dir>
                           [--wine-prefix <dir> [--wine <wine>]]
"""
import argparse
import os
import shutil
import subprocess
import sys

import wine as winehost


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--work', required=True)
    ap.add_argument('--wine-prefix', default='')
    ap.add_argument('--wine', default='')
    a = ap.parse_args()
    prefix = None
    if a.wine_prefix:
        wine = winehost.find_wine(a.wine)
        if not wine:
            print('enhancements-settings: Windows build and no Wine (set RA2_NATIVE_WINE); skipped')
            return winehost.SKIP
        prefix = winehost.Prefix(wine, a.wine_prefix)
    shutil.rmtree(a.work, ignore_errors=True)
    os.makedirs(a.work)
    settings = os.path.join(os.path.abspath(a.work), 'settings')
    env = dict(os.environ)
    env.pop('RA2_NATIVE_ENHANCEMENTS', None)   # it would set the values for one run, unsaved
    bad = 0
    try:
        for phase in ('change', 'reload'):
            cmd = [a.binary, phase, settings]
            run_env = env
            if prefix:
                cmd, run_env = prefix.command(cmd), prefix.env(env)
            p = subprocess.run(cmd, env=run_env, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True,
                               errors='replace')
            sys.stdout.write(p.stdout)
            if p.returncode != 0:
                print('enhancements-settings: %s exited %d' % (phase, p.returncode))
                bad += 1
                break
    finally:
        if prefix:
            prefix.close()
    return 1 if bad else 0


if __name__ == '__main__':
    sys.exit(main())
