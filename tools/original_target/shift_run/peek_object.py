#!/usr/bin/env python3
"""Start a rebuilt game image under Wine, attach winedbg and read memory: direct evidence of
which code ran. The shift test reads VerNum (0xA40C88): its first word is the vtable pointer
the VersionClass constructor stored, 0x7A2ABC in the exact image and the relocated copy in a
shifted one.

    peek_object.py IMAGE RUN_DIR [ADDRESS=0xa40c88] [WAIT_SECONDS=10]

Needs the same run folder as run_wine.py. Wine and the prefix come from the private config."""
import os, re, shutil, subprocess, sys, time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from run_wine import wine_settings


def main():
    image, run_dir = sys.argv[1], sys.argv[2]
    address = int(sys.argv[3], 16) if len(sys.argv) > 3 else 0xa40c88
    wait = float(sys.argv[4]) if len(sys.argv) > 4 else 10
    wine, prefix = wine_settings()
    env = dict(os.environ, WINEDEBUG='-all')
    if prefix:
        env['WINEPREFIX'] = prefix
    shutil.copyfile(image, os.path.join(run_dir, 'game.exe'))
    game = subprocess.Popen([wine, './game.exe'], cwd=run_dir, env=env, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    try:
        time.sleep(wait)
        listing = subprocess.run([wine, 'winedbg', '--command', 'info process'], env=env, capture_output=True,
                                 text=True, timeout=60).stdout
        pid = next((re.findall(r'[0-9a-fA-F]{8}', line)[0] for line in listing.splitlines() if 'game.exe' in line), None)
        if pid is None:
            print('game.exe is not running'); return 1
        script = Path(run_dir) / 'peek.dbg'
        script.write_text('attach 0x%x\nx /8x 0x%x\ndetach\nquit\n' % (int(pid, 16), address))
        out = subprocess.run([wine, 'winedbg', '--file', 'peek.dbg'], cwd=run_dir, env=env, capture_output=True,
                             text=True, timeout=120)
        for line in (out.stdout + out.stderr).splitlines():
            if ('%x' % address)[:6] in line.lower() or 'attached' in line.lower() or 'rror' in line:
                print(line)
    finally:
        game.terminate()
        try:
            game.wait(10)
        except subprocess.TimeoutExpired:
            game.kill()
    return 0


if __name__ == '__main__':
    raise SystemExit(main())
