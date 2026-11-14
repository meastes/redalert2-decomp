# SPDX-License-Identifier: GPL-3.0-or-later
"""Running the Windows cross build's ra2-native.exe under Wine on the Mac (lane C7).

The Windows build's checks (census, enhancements frames, enhancements settings) run the
.exe through Wine when one is found, in a fresh prefix of their own under the build
folder, and are skipped (77) when none is: Wine is a local test host only, never shipped
and never needed for the build. Wine is found as --wine <path>, else $RA2_NATIVE_WINE,
else `wine` on PATH. Unix paths given to the program work as they are (Wine's Z: drive
is the Mac's root).

  prefix = Prefix(wine, folder)   # makes the prefix (wineboot), starts its wineserver
  subprocess.run(prefix.command([exe, args...]), env=prefix.env(env))
  prefix.close()                  # stops its wineserver
"""
import os
import shutil
import subprocess

SKIP = 77


def find_wine(path=''):
    """The wine executable, or '' when there is none."""
    for candidate in (path, os.environ.get('RA2_NATIVE_WINE', '')):
        if candidate and os.path.isfile(candidate) and os.access(candidate, os.X_OK):
            return candidate
    return shutil.which('wine') or ''


class Prefix:
    def __init__(self, wine, folder):
        self.wine = wine
        self.folder = os.path.abspath(folder)
        # wineserver beside wine (the Wine bundle's bin folder), else on PATH.
        server = os.path.join(os.path.dirname(wine), 'wineserver')
        self.server = server if os.path.isfile(server) else (shutil.which('wineserver') or '')
        shutil.rmtree(self.folder, ignore_errors=True)
        os.makedirs(self.folder)
        # The prefix's own wineserver, started first and persistent: a server that a
        # program run started would inherit (and hold open) that run's output pipe.
        if self.server:
            subprocess.run([self.server, '-p'], env=self.env(), stdin=subprocess.DEVNULL,
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        subprocess.run([wine, 'wineboot', '-i'], env=self.env(), stdin=subprocess.DEVNULL,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)

    def env(self, base=None):
        e = dict(os.environ if base is None else base)
        # No Mono/Gecko install prompts; Wine's own diagnostics off (the game's log is stderr).
        e.update(WINEPREFIX=self.folder, WINEDEBUG='-all', WINEDLLOVERRIDES='mscoree,mshtml=')
        return e

    def command(self, argv):
        return [self.wine] + list(argv)

    def close(self):
        if self.server:
            subprocess.run([self.server, '-k'], env=self.env(), stdin=subprocess.DEVNULL,
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
