# SPDX-License-Identifier: GPL-3.0-or-later
"""The YR tests' Windows runs (lane YW): with --wine-prefix the test runs yr-native.exe under Wine in
that fresh prefix (native/tests/wine.py: a local test host, never shipped), and the prefix is deleted
afterwards (Wine prefixes are large). Without Wine the test is skipped (77).

    prefix = yr_wine.open_prefix(a)        # None for the Mac build; SKIP when no Wine
    if prefix == yr_wine.SKIP: return 77
    cmd, env = yr_wine.wrap(prefix, cmd, env)
    try: ... game_process.run(cmd, env=env, ...)
    finally: yr_wine.close(prefix)
"""
import os
import shutil
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), '..', '..', 'tests'))
import wine as winehost  # noqa: E402

SKIP = 'skip'


def add_arguments(ap):
    ap.add_argument('--wine-prefix', default='', help='the Windows build: run the .exe under Wine in this fresh prefix')
    ap.add_argument('--wine', default='', help='the wine executable (else RA2_NATIVE_WINE or PATH)')


def open_prefix(a):
    if not getattr(a, 'wine_prefix', ''):
        return None
    wine = winehost.find_wine(a.wine)
    if not wine:
        print('skipped: no Wine for the Windows build')
        return SKIP
    return winehost.Prefix(wine, a.wine_prefix)


def wrap(prefix, cmd, env):
    if prefix is None:
        return cmd, env
    return prefix.command(cmd), prefix.env(env)


def close(prefix):
    if prefix is None or prefix == SKIP:
        return
    prefix.close()
    shutil.rmtree(prefix.folder, ignore_errors=True)
