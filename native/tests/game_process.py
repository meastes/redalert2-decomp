# SPDX-License-Identifier: GPL-3.0-or-later
"""Starting ra2-native / yr-native from a test harness without leaving processes behind (lane C19).

Every harness that launches the game goes through here:

  - the game starts in a session (process group) of its own, and when the harness is done with it
    (a normal end, a failure, a timeout, an exception) the whole group is ended: SIGTERM, then
    SIGKILL after GRACE seconds. A child the game started (the Mac crash dialog,
    `--crash-report`, or Wine's own processes) goes with it;
  - its environment says RA2_NATIVE_NO_CRASH_DIALOG=1 (the crash handler logs the crash and ends,
    without starting the crash dialog that would wait for a user who is not there) and
    RA2_NATIVE_NO_RELAUNCH=1 (an app that would restart itself, for a new mod, ends instead: the
    restart leaves the session).

  p = game_process.run(cmd, env=env, stdout=..., timeout=...)   # as subprocess.run
  with game_process.Popen(cmd, env=env, stdout=...) as g:       # as subprocess.Popen
      ...                                                      # the group ends on leaving

Only the group this module started is signalled. On Windows (a harness run there) the game gets
a new process group and Popen.kill ends it; there is no group signal.
"""
import os
import signal
import subprocess
import time

GRACE = 3.0
TEST_ENV = {'RA2_NATIVE_NO_CRASH_DIALOG': '1', 'RA2_NATIVE_NO_RELAUNCH': '1'}


def game_env(env=None):
    """env (default os.environ) with the test run's variables (TEST_ENV) set."""
    e = dict(os.environ if env is None else env)
    e.update(TEST_ENV)
    return e


def group_alive(pgid):
    try:
        os.killpg(pgid, 0)
    except ProcessLookupError:
        return False
    except PermissionError:      # a member that is not ours any more: nothing of ours is left
        return False
    return True


def end_group(pgid, grace=GRACE):
    """SIGTERM to the group, SIGKILL after grace seconds if any of it is still there."""
    if os.name != 'posix' or not pgid:
        return
    for sig, wait in ((signal.SIGTERM, grace), (signal.SIGKILL, grace)):
        if not group_alive(pgid):
            return
        try:
            os.killpg(pgid, sig)
        except (ProcessLookupError, PermissionError):
            return
        deadline = time.monotonic() + wait
        while time.monotonic() < deadline and group_alive(pgid):
            time.sleep(0.05)


class Popen(subprocess.Popen):
    """subprocess.Popen in a session of its own, with the test environment; end() (or leaving a
    with block) ends the whole group."""

    def __init__(self, args, env=None, test_env=True, **kwargs):
        if test_env:
            env = game_env(env)
        if os.name == 'posix':
            kwargs['start_new_session'] = True
        else:
            kwargs['creationflags'] = kwargs.get('creationflags', 0) | subprocess.CREATE_NEW_PROCESS_GROUP
        super().__init__(args, env=env, **kwargs)
        self.pgid = self.pid     # the session's leader: its group id is its pid

    def end(self, grace=GRACE):
        """End the game and everything in its group; returns the game's exit code."""
        if os.name == 'posix':
            end_group(self.pgid, grace)     # the game, and whatever of its group outlived it
        elif self.poll() is None:
            self.kill()
        try:
            return self.wait(timeout=grace)
        except subprocess.TimeoutExpired:
            self.kill()
            return self.wait()

    def __exit__(self, exc_type, value, traceback):
        try:
            self.end()
        finally:
            super().__exit__(exc_type, value, traceback)


def run(args, input=None, timeout=None, check=False, **kwargs):
    """subprocess.run for the game: TimeoutExpired as subprocess.run raises it (with the output
    so far), and the game's group is ended whatever happens. A child that outlives the game and
    holds its output open (a crash dialog) is ended GRACE seconds after the game's own end."""
    if input is not None:
        kwargs['stdin'] = subprocess.PIPE
    deadline = None if timeout is None else time.monotonic() + timeout
    with Popen(args, **kwargs) as p:
        ended_at = None
        try:
            while True:
                step = 0.5 if deadline is None else max(0.0, min(0.5, deadline - time.monotonic()))
                try:
                    out, err = p.communicate(input, timeout=step)
                    break
                except subprocess.TimeoutExpired:
                    input = None       # sent with the first call
                    if deadline is not None and time.monotonic() >= deadline:
                        raise
                    if p.poll() is not None:
                        ended_at = ended_at or time.monotonic()
                        if time.monotonic() - ended_at > GRACE:
                            p.end()
        except subprocess.TimeoutExpired as e:
            p.end()
            out, err = p.communicate()
            raise subprocess.TimeoutExpired(args, timeout, out, err) from e
        except BaseException:
            p.end()
            raise
        code = p.poll()
    if check and code:
        raise subprocess.CalledProcessError(code, args, out, err)
    return subprocess.CompletedProcess(args, code, out, err)
