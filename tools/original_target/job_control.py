#!/usr/bin/env python3
"""Shared job control for the Mac's heavy work (lane P1, 2026-11-15).

Two locks, both kept in the main checkout (the parent of `git rev-parse --git-common-dir`), so
every worktree sees the same ones:

* the full-build lock, build/owner-watch/fullbuild.lock: one whole-file exact build at a time.
  It is the same mkdir lock private/tools/fullbuild_lock.sh takes (holder pid in `pid`; a lock
  whose holder is gone is removed). A process whose ancestor already holds it (a wrapper script
  that sourced fullbuild_lock.sh) goes on without waiting.
* the heavy-job slots, build/owner-watch/heavy/slot-N.lock: a counting semaphore on flock'd files
  for whole-file builds, native compiles and ctest runs. A job takes `weight` slots; the count
  is RA2_HEAVY_SLOTS, else the integer in build/owner-watch/heavy/slots, else DEFAULT_SLOTS.
  While a job holds slots RA2_HEAVY_HELD is set, so the tools it starts (build.py under
  join_gate.py, ctest under native_gate.py) do not wait for slots their parent already holds.
  RA2_HEAVY_SLOTS=0 turns the slots off. Order: the full-build lock, then the slots; a job never
  waits for the full-build lock while it holds slots, so the two cannot deadlock.

    job_control.py run [--weight N] [--name TEXT] [--full-build] -- COMMAND...
    job_control.py status

`Stages` times the stages of a gate for its receipt.
"""
import argparse
from contextlib import contextmanager
import fcntl
import json
import os
from pathlib import Path
import subprocess
import sys
import time

REPOSITORY = Path(__file__).resolve().parents[2]
# Measured 2026-11-15 on the 10-core 16 GB Mac (docs/ORIGINAL_TARGET_BUILD.md, "Heavy-job cap"):
# about 11 GB is in use before any heavy job; a whole-file build peaks near 2.9 GB, a cold native
# compile (ninja -j10) near 1.2 GB of clang, a ctest game about 0.25 GB. Four slots hold two
# heavy jobs at once (a whole-file build and a native compile, or either and a ctest -j4 run).
DEFAULT_SLOTS = 4
WEIGHTS = dict(full_build=2, native_build=2, ctest=1)    # ctest: per two parallel tests
HELD = 'RA2_HEAVY_HELD'


def main_root(repository=REPOSITORY):
    """The main checkout of `repository` (itself when it is not a worktree)."""
    try:
        common = subprocess.run(['git', '-C', str(repository), 'rev-parse', '--git-common-dir'],
                                capture_output=True, text=True, check=True).stdout.strip()
    except (OSError, subprocess.CalledProcessError):
        return Path(repository)
    common = Path(common)
    if not common.is_absolute():
        common = (Path(repository) / common).resolve()
    return common.parent


def control_directory(repository=REPOSITORY):
    return main_root(repository) / 'build/owner-watch'


class Stages:
    """Ordered stage timings: `with stages('compile'):` or stages.mark('compile')."""

    def __init__(self, clock=time.monotonic):
        self.clock = clock
        self.started = self.last = clock()
        self.items = []

    def mark(self, name, **extra):
        now = self.clock()
        self.items.append(dict(stage=name, seconds=round(now - self.last, 3), **extra))
        self.last = now
        return self.items[-1]

    @contextmanager
    def __call__(self, name, **extra):
        self.last = self.clock()
        try:
            yield
        finally:
            self.mark(name, **extra)

    def total(self):
        return round(self.clock() - self.started, 3)

    def table(self, title='stage timings'):
        total = sum(item['seconds'] for item in self.items) or 1.0
        width = max([len(item['stage']) for item in self.items] + [5])
        lines = ['%s:' % title]
        for item in self.items:
            lines.append('  %-*s %9.1fs %5.1f%%' % (width, item['stage'], item['seconds'], 100.0 * item['seconds'] / total))
        lines.append('  %-*s %9.1fs' % (width, 'total', sum(item['seconds'] for item in self.items)))
        return '\n'.join(lines)


def pid_alive(pid):
    try:
        os.kill(pid, 0)
    except ProcessLookupError:
        return False
    except PermissionError:
        return True
    return True


def ancestors(pid=None):
    """Pids of this process and its ancestors."""
    pid = os.getpid() if pid is None else pid
    chain = []
    while pid > 1 and pid not in chain:
        chain.append(pid)
        try:
            out = subprocess.run(['ps', '-o', 'ppid=', '-p', str(pid)], capture_output=True, text=True).stdout.strip()
            pid = int(out) if out else 0
        except (OSError, ValueError):
            break
    return chain


@contextmanager
def full_build_lock(directory=None, poll=5.0, log=sys.stderr):
    """Hold the full-build lock (mkdir lock compatible with fullbuild_lock.sh); yields seconds waited.
    Reentrant for a process whose ancestor holds it."""
    lock = Path(directory or control_directory()) / 'fullbuild.lock'
    lock.parent.mkdir(parents=True, exist_ok=True)
    started, announced, mine = time.monotonic(), False, False
    while True:
        try:
            lock.mkdir()
            (lock / 'pid').write_text('%d\n' % os.getpid())
            mine = True
            break
        except FileExistsError:
            try:
                holder = int((lock / 'pid').read_text().strip() or 0)
            except (OSError, ValueError):
                holder = 0
            if holder and holder in ancestors():
                break                                    # a wrapper of this process holds it
            if holder and not pid_alive(holder):
                _remove(lock)
                continue
            if not holder:
                try:
                    if time.time() - lock.stat().st_mtime > 120:
                        _remove(lock)
                        continue
                except FileNotFoundError:
                    continue
            if not announced and log:
                print('waiting for the full-build lock (held by pid %s)' % (holder or '?'), file=log, flush=True)
                announced = True
            time.sleep(poll)
    try:
        yield time.monotonic() - started
    finally:
        if mine:
            try:
                if int((lock / 'pid').read_text().strip()) == os.getpid():
                    _remove(lock)
            except (OSError, ValueError):
                pass


def _remove(lock):
    for item in lock.glob('*'):
        try:
            item.unlink()
        except OSError:
            pass
    try:
        lock.rmdir()
    except OSError:
        pass


def slot_count(directory=None):
    value = os.environ.get('RA2_HEAVY_SLOTS')
    if value is None:
        path = Path(directory or control_directory()) / 'heavy/slots'
        try:
            value = path.read_text().strip()
        except OSError:
            value = None
    try:
        return max(0, int(value)) if value is not None else DEFAULT_SLOTS
    except ValueError:
        return DEFAULT_SLOTS


@contextmanager
def heavy_slots(name, weight=1, directory=None, poll=2.0, log=sys.stderr):
    """Hold `weight` of the shared heavy-job slots; yields seconds waited. No-op when slots are off
    or a parent process already holds slots (RA2_HEAVY_HELD)."""
    root = Path(directory or control_directory()) / 'heavy'
    count = slot_count(root.parent)
    if count == 0 or os.environ.get(HELD):
        yield 0.0
        return
    weight = max(1, min(int(weight), count))
    root.mkdir(parents=True, exist_ok=True)
    started, announced, held = time.monotonic(), False, []
    try:
        while True:
            for index in range(count):
                if len(held) == weight:
                    break
                if any(i == index for i, _ in held):
                    continue
                handle = open(root / ('slot-%d.lock' % index), 'a+')
                try:
                    fcntl.flock(handle, fcntl.LOCK_EX | fcntl.LOCK_NB)
                except OSError:
                    handle.close()
                    continue
                handle.seek(0); handle.truncate()
                handle.write(json.dumps(dict(pid=os.getpid(), name=name, weight=weight, since=time.time())) + '\n')
                handle.flush()
                held.append((index, handle))
            if len(held) == weight:
                break
            # Not enough free slots: give back what we took so two jobs cannot deadlock on halves.
            for _, handle in held:
                fcntl.flock(handle, fcntl.LOCK_UN); handle.close()
            held = []
            if not announced and log:
                print('waiting for %d of %d heavy-job slots (%s)' % (weight, count, name), file=log, flush=True)
                announced = True
            time.sleep(poll)
        waited = time.monotonic() - started
        os.environ[HELD] = name
        try:
            yield waited
        finally:
            os.environ.pop(HELD, None)
    finally:
        for _, handle in held:
            try:
                handle.seek(0); handle.truncate()
                fcntl.flock(handle, fcntl.LOCK_UN)
            finally:
                handle.close()


def status(directory=None):
    root = Path(directory or control_directory())
    lock = root / 'fullbuild.lock'
    holder = None
    if lock.exists():
        try:
            holder = int((lock / 'pid').read_text().strip())
        except (OSError, ValueError):
            holder = '?'
    slots = []
    for index in range(slot_count(root)):
        path = root / 'heavy' / ('slot-%d.lock' % index)
        busy, info = False, None
        if path.exists():
            with open(path, 'a+') as handle:
                try:
                    fcntl.flock(handle, fcntl.LOCK_EX | fcntl.LOCK_NB)
                    fcntl.flock(handle, fcntl.LOCK_UN)
                except OSError:
                    busy = True
                    handle.seek(0)
                    text = handle.read().strip()
                    info = json.loads(text) if text else None
        slots.append(dict(slot=index, busy=busy, holder=info))
    return dict(full_build_lock=holder, slots=slots)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = parser.add_subparsers(dest='action', required=True)
    run = sub.add_parser('run', help='run COMMAND holding heavy-job slots (and the full-build lock)')
    run.add_argument('--weight', type=int, default=1)
    run.add_argument('--name', default=None)
    run.add_argument('--full-build', action='store_true', help='also hold the full-build lock')
    run.add_argument('command', nargs=argparse.REMAINDER)
    sub.add_parser('status')
    args = parser.parse_args()
    if args.action == 'status':
        print(json.dumps(status(), indent=1))
        return
    command = args.command[1:] if args.command[:1] == ['--'] else args.command
    if not command:
        parser.error('no command')
    name = args.name or ' '.join(command)[:80]
    # Order: the full-build lock first, then the slots (a job never holds slots while it waits for
    # the full-build lock, so the two cannot deadlock).
    from contextlib import nullcontext
    with full_build_lock() if args.full_build else nullcontext():
        with heavy_slots(name, args.weight) as waited:
            code = subprocess.run(command).returncode
    if waited > 1:
        print('heavy-job slots waited %.1fs' % waited, file=sys.stderr)
    raise SystemExit(code)


if __name__ == '__main__':
    main()
