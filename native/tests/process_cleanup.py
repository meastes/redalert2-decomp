#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
"""ctest native-process-cleanup (lane C19): no game process and no crash dialog outlives a test run.

Test runs used to leave processes behind: the Mac crash dialog (`ra2-native --crash-report`, a
modal waiting for a user who is not there) and games whose harness timed out. Checked here:

  1. tests/game_process.py ends the whole process group (a stand-in game: sh with children):
     - a timeout (game_process.run raises TimeoutExpired, every child is gone);
     - a child that ignores SIGTERM (SIGKILL after the grace);
     - a game that ended while its child holds the output open (the child is ended, run returns);
     - an exception in the harness inside `with game_process.Popen(...)`.
  2. A forced crash of the real app (--app, SIGSEGV once its crash handler is installed), once
     with the harnesses' RA2_NATIVE_NO_CRASH_DIALOG=1 and once with only an input script: the
     crash is logged, "crash dialog not started" follows, and when the app has ended nothing of
     its process group is left (no --crash-report process was started).
  3. A forced timeout of the real app (stopped with SIGSTOP, so only SIGKILL ends it): the harness
     ends its group and nothing is left.

Needs only the binary (the app's crash handler is installed before its setup). macOS/Linux.

  process_cleanup.py --binary <ra2-native> --work <dir>
"""
import argparse
import os
import shutil
import signal
import subprocess
import sys
import time

import game_process

SKIP = 77


def ps():
    """[(pid, pgid, command)] of every process."""
    out = subprocess.run(['ps', '-Ao', 'pid=,pgid=,command='], capture_output=True, text=True).stdout
    rows = []
    for line in out.splitlines():
        parts = line.split(None, 2)
        if len(parts) == 3:
            rows.append((int(parts[0]), int(parts[1]), parts[2]))
    return rows


def left(token=None, pgid=None):
    """processes still there with token in their command line, or in group pgid"""
    return [r for r in ps() if (token and token in r[2]) or (pgid and r[1] == pgid)]


def stand_in(work, problems):
    token = '%d%05d' % (os.getpid() % 1000, int(time.time()) % 100000)

    def sleeper(n):      # a unique, harmless child: "sleep 30<n>.<token>"
        return 'sleep 30%d.%s' % (n, token)

    # Timeout: three children of the stand-in; all go.
    try:
        game_process.run(['sh', '-c', '%s & %s & %s; wait' % (sleeper(1), sleeper(2), sleeper(3))], timeout=1)
        problems.append('stand-in timeout: run returned without TimeoutExpired')
    except subprocess.TimeoutExpired:
        pass
    if left(token):
        problems.append('stand-in timeout: left %s' % left(token))
    # A child that ignores SIGTERM (ignored signals are kept across exec): SIGKILL ends it.
    t = time.monotonic()
    try:
        game_process.run(['sh', '-c', 'trap "" TERM; %s & %s; wait' % (sleeper(4), sleeper(5))], timeout=1)
    except subprocess.TimeoutExpired:
        pass
    if left(token):
        problems.append('stand-in SIGTERM ignored: left %s' % left(token))
    print('process-cleanup: stand-in ignoring SIGTERM ended in %.1f s' % (time.monotonic() - t))
    # The game ends at once; its child (a crash dialog's place) holds the output pipe open.
    t = time.monotonic()
    p = game_process.run(['sh', '-c', '(%s &); echo started; exit 3' % sleeper(6)], stdout=subprocess.PIPE,
                         text=True, timeout=60)
    took = time.monotonic() - t
    if p.returncode != 3 or 'started' not in p.stdout:
        problems.append('stand-in pipe holder: exit %s, output %r' % (p.returncode, p.stdout))
    if took > game_process.GRACE + 10:
        problems.append('stand-in pipe holder: run took %.1f s' % took)
    if left(token):
        problems.append('stand-in pipe holder: left %s' % left(token))
    # An exception in the harness while the game runs.
    try:
        with game_process.Popen(['sh', '-c', '%s & %s; wait' % (sleeper(7), sleeper(8))]):
            time.sleep(0.5)
            raise RuntimeError('harness failure')
    except RuntimeError:
        pass
    if left(token):
        problems.append('stand-in exception: left %s' % left(token))


def start_app(work, name, env):
    """the app started (--app, a fresh HOME), once its crash handler is installed (the line it
    prints before its log takes stderr over); (process, log path)"""
    home = os.path.join(work, name)
    shutil.rmtree(home, ignore_errors=True)
    os.makedirs(home)
    env = dict(env, HOME=home)
    for k in ('RA2_NATIVE_LOG_STDERR', 'RA2_DATA_DIR', 'RA2_GAME_EXE', 'RA2_NATIVE_SETUP'):
        env.pop(k, None)
    return env, os.path.join(home, 'Library', 'Logs', 'RA2Native', 'ra2-native.log')


def wait_line(g, marker='app: logging to', timeout=60):
    """the app's first output line with marker in it ('' if it ended or the time ran out first)"""
    import threading
    got = []

    def read():
        for line in g.stdout:
            if marker in line:
                got.append(line)
                return
    reader = threading.Thread(target=read, daemon=True)
    reader.start()
    reader.join(timeout)
    return got[0] if got else ''


def forced_crash(binary, work, name, problems, scripted_only):
    env = dict(os.environ)
    argv = [binary, '--app', '--game-args=-noaudio']
    if scripted_only:
        # The harness variables off: the input script alone keeps the dialog away.
        for k in game_process.TEST_ENV:
            env.pop(k, None)
        script = os.path.join(work, name + '.script')
        with open(script, 'w') as f:
            f.write('600000 quit\n')
        argv.append('--input-script=' + script)
    env, log = start_app(work, name, env)
    with game_process.Popen(argv, env=env, test_env=not scripted_only, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, text=True, errors='replace') as g:
        line = wait_line(g)
        if 'app: logging to' not in line:
            problems.append('%s: the app did not start its log (%r)' % (name, line))
            return
        # The crash once the log has taken stderr over (its first line written).
        deadline = time.monotonic() + 30
        while time.monotonic() < deadline and 'session started' not in (
                open(log, errors='replace').read() if os.path.isfile(log) else ''):
            time.sleep(0.02)
        os.kill(g.pid, signal.SIGSEGV)
        try:
            code = g.wait(timeout=60)
        except subprocess.TimeoutExpired:
            problems.append('%s: the app did not end after its crash' % name)
            return
        time.sleep(1.0)      # a dialog the handler started would be running by now
        rest = left(pgid=g.pgid)
        if rest:
            problems.append('%s: after the crash its group still has %s' % (name, rest))
    text = open(log, errors='replace').read() if os.path.isfile(log) else ''
    if code != 128 + signal.SIGSEGV:
        problems.append('%s: exit %s (wanted %d, the crash handler\'s)' % (name, code, 128 + signal.SIGSEGV))
    if 'ra2-native: crash: signal %d' % signal.SIGSEGV not in text:
        problems.append('%s: no crash line in %s' % (name, log))
    if 'crash dialog not started (scripted run)' not in text:
        problems.append('%s: the log does not say the crash dialog was not started' % name)
    if left(token=log):
        problems.append('%s: a --crash-report process for this log is running' % name)
    print('process-cleanup: %s: exit %s, %s' % (name, code, 'FAILED' if any(x.startswith(name + ':') for x in problems)
                                                   else 'crash logged, no crash dialog started'))


def forced_timeout(binary, work, problems):
    env, _ = start_app(work, 'timeout', dict(os.environ))
    code = None
    with game_process.Popen([binary, '--app', '--game-args=-noaudio'], env=env, stdout=subprocess.PIPE,
                            stderr=subprocess.STDOUT, text=True, errors='replace') as g:
        line = wait_line(g)
        if 'app: logging to' not in line:
            problems.append('timeout: the app did not start its log (%r)' % line)
        os.kill(g.pid, signal.SIGSTOP)     # hung: it answers nothing now
        try:
            g.wait(timeout=2)
            problems.append('timeout: the stopped app ended by itself')
        except subprocess.TimeoutExpired:
            code = g.end()
        pgid = g.pgid
    rest = left(pgid=pgid)
    if rest:
        problems.append('timeout: its group still has %s' % rest)
    print('process-cleanup: timeout: the stopped app was ended (exit %s)' % code)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--binary', required=True)
    ap.add_argument('--work', required=True)
    a = ap.parse_args()
    if os.name != 'posix':
        print('process-cleanup: POSIX only; skipped')
        return SKIP
    work = os.path.abspath(a.work)
    shutil.rmtree(work, ignore_errors=True)
    os.makedirs(work)
    problems = []
    stand_in(work, problems)
    forced_crash(a.binary, work, 'crash-harness', problems, scripted_only=False)
    forced_crash(a.binary, work, 'crash-script', problems, scripted_only=True)
    forced_timeout(a.binary, work, problems)
    for p in problems:
        print('FAIL: ' + p)
    print('process-cleanup: %s' % ('FAILED' if problems else 'OK'))
    return 1 if problems else 0


if __name__ == '__main__':
    sys.exit(main())
