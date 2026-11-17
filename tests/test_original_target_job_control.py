"""job_control.py, gate_scope.py and native_gate.py (lane P1): stage timings, the full-build lock,
the heavy-job slots, the gate split and the native gate's receipt parsing."""
import json
import os
import pathlib
import subprocess
import sys
import tempfile
import textwrap
import threading
import time
import unittest

TOOLS = pathlib.Path(__file__).resolve().parents[1] / 'tools' / 'original_target'
sys.path.insert(0, str(TOOLS))
import gate_scope
import job_control
import native_gate


class StagesTests(unittest.TestCase):
    def test_marks_and_context(self):
        now = [0.0]
        stages = job_control.Stages(clock=lambda: now[0])
        now[0] = 2.0
        stages.mark('setup')
        with stages('compile'):
            now[0] = 7.5
        self.assertEqual([(i['stage'], i['seconds']) for i in stages.items], [('setup', 2.0), ('compile', 5.5)])
        table = stages.table('t')
        self.assertIn('setup', table)
        self.assertIn('7.5s', table)


class FullBuildLockTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.dir = pathlib.Path(self.tmp.name)

    def tearDown(self):
        self.tmp.cleanup()

    def test_acquire_release(self):
        with job_control.full_build_lock(self.dir, log=None):
            self.assertEqual(int((self.dir / 'fullbuild.lock/pid').read_text()), os.getpid())
        self.assertFalse((self.dir / 'fullbuild.lock').exists())

    def test_stale_holder_removed(self):
        lock = self.dir / 'fullbuild.lock'
        lock.mkdir()
        dead = subprocess.Popen(['true']); dead.wait()
        (lock / 'pid').write_text('%d\n' % dead.pid)
        with job_control.full_build_lock(self.dir, poll=0.01, log=None) as waited:
            self.assertLess(waited, 5)

    def test_reentrant_under_holding_ancestor(self):
        # This process holds the lock; a child taking it goes on at once and leaves it in place.
        script = textwrap.dedent('''
            import sys, time; sys.path.insert(0, %r)
            import job_control
            with job_control.full_build_lock(%r, poll=0.05, log=None) as waited:
                print(waited)
        ''' % (str(TOOLS), str(self.dir)))
        with job_control.full_build_lock(self.dir, log=None):
            out = subprocess.run([sys.executable, '-c', script], capture_output=True, text=True, timeout=30)
            self.assertEqual(out.returncode, 0, out.stderr)
            self.assertLess(float(out.stdout), 1.0)
            self.assertTrue((self.dir / 'fullbuild.lock').exists())
        self.assertFalse((self.dir / 'fullbuild.lock').exists())

    def test_waits_for_live_holder(self):
        holder = subprocess.Popen(['sleep', '30'])
        try:
            lock = self.dir / 'fullbuild.lock'
            lock.mkdir(); (lock / 'pid').write_text('%d\n' % holder.pid)
            done = []
            def take():
                with job_control.full_build_lock(self.dir, poll=0.05, log=None) as waited:
                    done.append(waited)
            thread = threading.Thread(target=take); thread.start()
            time.sleep(0.4)
            self.assertEqual(done, [])
            holder.kill(); holder.wait()
            thread.join(10)
            self.assertEqual(len(done), 1)
        finally:
            holder.kill()


class HeavySlotTests(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.dir = pathlib.Path(self.tmp.name)
        self.saved = {k: os.environ.pop(k, None) for k in ('RA2_HEAVY_SLOTS', job_control.HELD)}

    def tearDown(self):
        for key, value in self.saved.items():
            os.environ.pop(key, None)
            if value is not None:
                os.environ[key] = value
        self.tmp.cleanup()

    def hold(self, weight, seconds):
        """A child process holding `weight` slots for `seconds`."""
        script = textwrap.dedent('''
            import sys, time; sys.path.insert(0, %r)
            import job_control
            with job_control.heavy_slots('child', %d, %r, poll=0.02, log=None):
                print('held', flush=True); time.sleep(%f)
        ''' % (str(TOOLS), weight, str(self.dir), seconds))
        child = subprocess.Popen([sys.executable, '-c', script], stdout=subprocess.PIPE, text=True)
        self.assertEqual(child.stdout.readline().strip(), 'held')
        return child

    def test_count_from_file_and_environment(self):
        (self.dir / 'heavy').mkdir()
        (self.dir / 'heavy/slots').write_text('3\n')
        self.assertEqual(job_control.slot_count(self.dir), 3)
        os.environ['RA2_HEAVY_SLOTS'] = '5'
        self.assertEqual(job_control.slot_count(self.dir), 5)

    def test_weight_waits_for_enough_slots(self):
        os.environ['RA2_HEAVY_SLOTS'] = '3'
        child = self.hold(2, 1.0)
        try:
            started = time.monotonic()
            with job_control.heavy_slots('one', 1, self.dir, poll=0.02, log=None) as waited:
                self.assertLess(waited, 0.5)          # one slot is still free
            with job_control.heavy_slots('two', 2, self.dir, poll=0.02, log=None) as waited:
                self.assertGreater(time.monotonic() - started, 0.5)
            self.assertEqual(os.environ.get(job_control.HELD), None)
        finally:
            child.wait()
        busy = [s for s in job_control.status(self.dir)['slots'] if s['busy']]
        self.assertEqual(busy, [])

    def test_held_by_parent_and_off(self):
        os.environ['RA2_HEAVY_SLOTS'] = '1'
        with job_control.heavy_slots('outer', 1, self.dir, log=None):
            self.assertEqual(os.environ[job_control.HELD], 'outer')
            with job_control.heavy_slots('inner', 1, self.dir, poll=0.02, log=None) as waited:
                self.assertEqual(waited, 0.0)       # not a deadlock on the one slot
        os.environ['RA2_HEAVY_SLOTS'] = '0'
        with job_control.heavy_slots('off', 4, self.dir, log=None) as waited:
            self.assertEqual(waited, 0.0)


class GateScopeTests(unittest.TestCase):
    def test_classify(self):
        self.assertEqual(gate_scope.scope(['native/platform/sdl/main.cpp', 'docs/HANDOFF.md']), 'native')
        self.assertEqual(gate_scope.scope(['native/CMakeLists.txt', 'src/recovered/a.cpp']), 'exact')
        self.assertEqual(gate_scope.scope(['tools/original_target/build.py']), 'exact')
        self.assertEqual(gate_scope.scope(['manifests/original_target/index.json']), 'exact')
        self.assertEqual(gate_scope.scope(['docs/STATUS.md', 'AGENTS.md']), 'none')
        self.assertEqual(gate_scope.scope([]), 'none')

    def test_changed_paths_and_resting_receipt(self):
        with tempfile.TemporaryDirectory() as tmp:
            repo = pathlib.Path(tmp)
            def git(*args):
                return subprocess.run(['git', '-C', str(repo), '-c', 'user.name=t', '-c', 'user.email=t@example.invalid',
                                       *args], check=True, capture_output=True, text=True).stdout.strip()
            git('init', '-q', '-b', 'main')
            (repo / 'docs').mkdir(); (repo / 'docs/a.md').write_text('a\n')
            git('add', '.'); git('commit', '-q', '-m', 'a')
            base = git('rev-parse', 'HEAD')
            (repo / 'native').mkdir(); (repo / 'native/x.cpp').write_text('x\n')
            git('add', '.'); git('commit', '-q', '-m', 'b')
            (repo / 'src').mkdir(); (repo / 'src/new.cpp').write_text('y\n')
            self.assertEqual(gate_scope.changed_paths(base, repo), ['native/x.cpp', 'src/new.cpp'])
            (repo / 'build').mkdir()
            rows = [dict(commit=base, whole_file_exact=True, time_utc='t1', gate='g'),
                    dict(commit='0' * 40, whole_file_exact=True, time_utc='t2', gate='g'),
                    dict(commit=git('rev-parse', 'HEAD'), whole_file_exact=False, time_utc='t3', gate='g')]
            (repo / 'build/whole-file-receipts.jsonl').write_text(''.join(json.dumps(r) + '\n' for r in rows))
            self.assertEqual(gate_scope.resting_receipt(repo)['commit'], base)


class NativeGateParsingTests(unittest.TestCase):
    def test_junit_and_ninja_log(self):
        with tempfile.TemporaryDirectory() as tmp:
            tmp = pathlib.Path(tmp)
            (tmp / 'r.xml').write_text('<testsuite><testcase name="a" time="1.5" status="run"/>'
                                       '<testcase name="b" time="0.2"><failure/></testcase></testsuite>')
            self.assertEqual(native_gate.junit_tests(tmp / 'r.xml'),
                             [dict(name='a', seconds=1.5, status='run'), dict(name='b', seconds=0.2, status='failed')])
            old = '# ninja log v5\n0\t100\t0\told.o\t1\n'
            (tmp / '.ninja_log').write_text(old + '10\t4000\t0\ta.o\t2\n20\t1020\t0\tb.o\t3\n')
            self.assertEqual(native_gate.ninja_steps(tmp)['steps'], 3)
            self.assertEqual(native_gate.ninja_steps(tmp, 10 ** 6)['steps'], 3)     # recompacted: whole log
            steps = native_gate.ninja_steps(tmp, len(old))
            self.assertEqual(steps['steps'], 2)
            self.assertEqual(steps['slowest'][0], dict(output='a.o', seconds=4.0))
            self.assertEqual(native_gate.link_closure('x link-closure baseline: 12 undefined (baseline 12) ok'), 12)

    def test_unconfigured_folder_refuses(self):
        with tempfile.TemporaryDirectory() as tmp:
            receipt = native_gate.gate(pathlib.Path(tmp) / 'none', pathlib.Path(tmp) / 'logs')
            self.assertFalse(receipt['ok'])
            self.assertIn('not a configured native build', receipt['reason'])


class GateConfigTests(unittest.TestCase):
    def test_gate_prefix_and_lock(self):
        import importlib
        build = importlib.import_module('build')
        base = dict(wine_kind='wine', wine_prefix='/p/shared', lock='/l/compiler.lock')
        self.assertIs(build.gate_config(base), base)
        gate = build.gate_config(dict(base, gate_wine_prefix='/p/gate', gate_lock='/l/gate.lock'))
        self.assertEqual((gate['wine_prefix'], gate['lock']), ('/p/gate', '/l/gate.lock'))
        with self.assertRaises(Exception):
            build.gate_config(dict(base, gate_wine_prefix='/p/gate', gate_lock='/elsewhere/gate.lock'))
        crossover = dict(base, wine_kind='crossover', gate_wine_prefix='/p/gate', gate_lock='/l/gate.lock')
        self.assertIs(build.gate_config(crossover), crossover)


class BuildLockWaitTests(unittest.TestCase):
    def test_lock_waits_from_command_receipts(self):
        import importlib
        build = importlib.import_module('build')
        with tempfile.TemporaryDirectory() as tmp:
            tmp = pathlib.Path(tmp)
            (tmp / 'compile-command.json').write_text(json.dumps(dict(lock_wait_seconds=785.2)))
            (tmp / 'link-command.json').write_text(json.dumps(dict(lock_wait_seconds=0.0)))
            (tmp / 'assemble-command.json').write_text('not json')
            self.assertEqual(build.lock_waits(tmp), {'compile': 785.2})


if __name__ == '__main__':
    unittest.main()
