#!/usr/bin/env python3
"""Run a rebuilt game image under the project's Wine host for N seconds and report how it ended.

    run_wine.py IMAGE RUN_DIR SECONDS LOG [WINEDEBUG [KEEP_REGEX]]

Copies IMAGE to RUN_DIR/game.exe and starts it there (RUN_DIR holds the game data the image
needs: the MIX files, binkw32.dll beside it). Reports "still running (killed)" when the game
reached a steady state, else its exit status, and the files it created. WINEDEBUG defaults to
errors only; with "+relay" and KEEP_REGEX the log keeps only matching lines (a relay trace is
about half a million lines for a start-up). Wine and the prefix come from the private config
(wine, wine_prefix); RA2_CONFIG names another config. Used by the shift test
(docs/SOURCE_FORM_TRIAL.md); the exact and the shifted image should behave the same."""
import json, os, re, shutil, signal, subprocess, sys, time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]


def wine_settings():
    config = json.loads(Path(os.environ.get('RA2_CONFIG', ROOT / 'private/original-target.local.json')).read_text())
    return config['wine'], config.get('wine_prefix')


def main():
    image, run_dir, seconds, log = sys.argv[1], sys.argv[2], float(sys.argv[3]), sys.argv[4]
    debug = sys.argv[5] if len(sys.argv) > 5 else 'err+all,fixme-all'
    keep = sys.argv[6] if len(sys.argv) > 6 else None
    wine, prefix = wine_settings()
    env = dict(os.environ, WINEDEBUG=debug)
    if prefix:
        env['WINEPREFIX'] = prefix
    before = set(os.listdir(run_dir))
    shutil.copyfile(image, os.path.join(run_dir, 'game.exe'))
    with open(log, 'w') as out:
        started = time.time()
        proc = subprocess.Popen([wine, './game.exe'], cwd=run_dir, env=env, stdout=out, stderr=subprocess.STDOUT)
        status = None
        while time.time() - started < seconds:
            status = proc.poll()
            if status is not None:
                break
            time.sleep(1)
        alive = proc.poll() is None
        if alive:
            proc.send_signal(signal.SIGTERM)
            try:
                proc.wait(10)
            except subprocess.TimeoutExpired:
                proc.kill()
        out.write('\n--- runner: %s after %.0fs; new files: %s\n' % (
            'still running (killed)' if alive else 'exited with %s' % status, time.time() - started,
            sorted(set(os.listdir(run_dir)) - before)))
    if keep:
        lines = Path(log).read_text(errors='replace').split('\n')
        kept = [l for l in lines if re.search(keep, l)]
        Path(log).write_text('\n'.join(kept[-4000:]) + '\n' + lines[-1] + '\n')
        print('kept %d of %d lines' % (len(kept), len(lines)))
    print(Path(log).read_text(errors='replace')[-3000:])


if __name__ == '__main__':
    main()
