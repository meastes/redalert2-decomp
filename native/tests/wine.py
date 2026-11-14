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
import re
import shutil
import subprocess

SKIP = 77
# Wine's macOS driver (with MoltenVK) sometimes dies at a launch with an Objective-C
# exception ("-[__NSArrayM lock]: unrecognized selector", C15). The Windows program cannot
# raise one: an Objective-C exception is the Mac host's, never the game's.
# A prefix wineboot left without its system folder ("could not load kernel32.dll") is the
# host's too; Prefix.make makes it again.
HOST_EXCEPTION = re.compile(r'Terminating app due to uncaught exception|uncaught exception of type NSException'
                            r'|^wine: could not load kernel32\.dll', re.M)
HOST_RETRIES = 2
PREFIX_TRIES = 3


def host_failure(output, timed_out=False):
    """Why a launch under Wine failed in the Mac host rather than in the program, or ''.
    An Objective-C exception anywhere, a prefix Wine could not start in, or a launch that
    hung before the program's first start-up milestone (`run-to: reached main`). Anything
    else is the program's own result."""
    m = HOST_EXCEPTION.search(output or '')
    if m:
        line = next((l for l in output.splitlines() if 'reason:' in l), m.group(0))
        return 'Wine host failure: ' + line.strip()[:160]
    if timed_out and 'run-to: reached main' not in (output or ''):
        return 'Wine host stalled before the program started'
    return ''


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
        self.make()

    def make(self):
        """A fresh prefix, made again (at most PREFIX_TRIES times) while wineboot leaves it
        without its system folder: under load wineboot sometimes ends early, and every
        launch in such a prefix fails ("could not load kernel32.dll", C15)."""
        for _ in range(PREFIX_TRIES):
            self.close()
            shutil.rmtree(self.folder, ignore_errors=True)
            os.makedirs(self.folder)
            # The prefix's own wineserver, started first and persistent: a server that a
            # program run started would inherit (and hold open) that run's output pipe.
            if self.server:
                subprocess.run([self.server, '-p'], env=self.env(), stdin=subprocess.DEVNULL,
                               stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            subprocess.run([self.wine, 'wineboot', '-i'], env=self.env(), stdin=subprocess.DEVNULL,
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
            if self.ready():
                return
            print('wine: prefix %s not made (no kernel32.dll after wineboot); making it again' % self.folder)

    def ready(self):
        return os.path.isfile(os.path.join(self.folder, 'drive_c', 'windows', 'system32', 'kernel32.dll'))

    def env(self, base=None):
        e = dict(os.environ if base is None else base)
        # No Mono/Gecko install prompts; Wine's own diagnostics off (the game's log is stderr).
        e.update(WINEPREFIX=self.folder, WINEDEBUG='-all', WINEDLLOVERRIDES='mscoree,mshtml=')
        return e

    def command(self, argv):
        if not self.ready():          # a launch's host failure left the prefix unusable
            self.make()
        return [self.wine] + list(argv)

    def close(self):
        if self.server:
            subprocess.run([self.server, '-k'], env=self.env(), stdin=subprocess.DEVNULL,
                           stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
