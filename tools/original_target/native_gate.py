#!/usr/bin/env python3
"""The native gate with stage timings (lane P1, 2026-11-15).

    native_gate.py [--build-dir DIR] [--configure ARG...] [--tests REGEX] [--jobs N]
                   [--ctest-jobs N] [--played COMMAND] [--output JSON] [--base REV]

Runs, in order: cmake configure (the configured folder is reused: it carries the machine's
census paths; --configure adds -D arguments), cmake --build, ctest (one JUnit file, so each
test's time is recorded), and optionally a played check (any command, e.g. a scripted play
run). Every stage's seconds, each test's seconds and status, the slowest compile steps (from
.ninja_log) and the heavy-job slot waits go to OUTPUT (default DIR/native-gate.json), and a
table goes to stderr. Build and ctest each hold heavy-job slots (job_control.py). The native
gate never takes the full-build lock.

A native-only change (gate_scope.py) needs only this gate; the receipt records the whole-file
exact receipt it rests on. Exits 1 on the first failing stage.
"""
import argparse
import json
from pathlib import Path
import re
import shlex
import subprocess
import sys
import xml.etree.ElementTree as ElementTree

sys.path.insert(0, str(Path(__file__).resolve().parent))
from job_control import Stages, WEIGHTS, heavy_slots  # noqa: E402

REPOSITORY = Path(__file__).resolve().parents[2]


def run(command, log, cwd=REPOSITORY):
    with open(log, 'w') as handle:
        return subprocess.run(command, cwd=cwd, stdout=handle, stderr=subprocess.STDOUT).returncode


def junit_tests(path):
    """[{name, seconds, status}] from a ctest --output-junit file."""
    try:
        root = ElementTree.parse(path).getroot()
    except (OSError, ElementTree.ParseError):
        return []
    out = []
    for case in root.iter('testcase'):
        status = case.get('status') or ('failed' if case.find('failure') is not None else
                                         'skipped' if case.find('skipped') is not None else 'run')
        out.append(dict(name=case.get('name'), seconds=round(float(case.get('time') or 0), 3), status=status))
    return out


def ninja_log_size(build_dir):
    try:
        return (Path(build_dir) / '.ninja_log').stat().st_size
    except OSError:
        return 0


def ninja_steps(build_dir, offset=0, top=10):
    """Steps ninja logged after byte `offset` of .ninja_log (the size before this build; the whole
    log when ninja recompacted it): their count, summed seconds and the slowest `top`."""
    path = Path(build_dir) / '.ninja_log'
    try:
        data = path.read_bytes()
    except OSError:
        return dict(steps=0, summed_seconds=0.0, slowest=[])
    if offset > len(data):
        offset = 0
    rows = []
    for line in data[offset:].decode(errors='replace').splitlines():
        parts = line.split('\t')
        if len(parts) >= 4 and parts[0].isdigit() and parts[1].isdigit():
            rows.append((int(parts[0]), int(parts[1]), parts[3]))
    rows.sort(key=lambda r: r[1] - r[0], reverse=True)
    return dict(steps=len(rows), summed_seconds=round(sum(r[1] - r[0] for r in rows) / 1000.0, 1),
                slowest=[dict(output=r[2], seconds=round((r[1] - r[0]) / 1000.0, 1)) for r in rows[:top]])


def link_closure(text):
    match = re.search(r'link-closure baseline: (\d+) undefined \(baseline (\d+)\) ok', text)
    return int(match.group(1)) if match else None


def gate(build_dir, logs, configure=(), tests=None, jobs=None, played=None, stages=None, ctest_jobs=None):
    """Run the native gate; returns its receipt (receipt['ok'] tells the result)."""
    stages = stages or Stages()
    build_dir, logs = Path(build_dir).resolve(), Path(logs).resolve()
    logs.mkdir(parents=True, exist_ok=True)
    receipt = dict(build_dir=str(build_dir), stages=stages.items, ok=False)
    if not (build_dir / 'CMakeCache.txt').exists() and not configure:
        receipt['reason'] = '%s is not a configured native build' % build_dir
        return receipt
    command = ['cmake', '-S', str(REPOSITORY / 'native'), '-B', str(build_dir), *configure]
    with stages('configure'):
        code = run(command, logs / 'native-configure.log')
    if code:
        receipt['failed'] = 'configure'
        return receipt
    with heavy_slots('native build ' + build_dir.name, WEIGHTS['native_build']) as waited:
        stages.mark('build slot wait')
        build = ['cmake', '--build', str(build_dir)] + (['-j', str(jobs)] if jobs else [])
        offset = ninja_log_size(build_dir)
        with stages('compile and link'):
            code = run(build, logs / 'native-build.log')
    receipt['ninja'] = ninja_steps(build_dir, offset)
    if code:
        receipt['failed'] = 'build'
        return receipt
    junit = logs / 'native-ctest.xml'
    with heavy_slots('native ctest ' + build_dir.name, WEIGHTS['ctest'] * ((max(1, ctest_jobs or 1) + 1) // 2)):
        stages.mark('ctest slot wait')
        test = ['ctest', '--test-dir', str(build_dir), '--output-junit', str(junit)] + (['-R', tests] if tests else []) \
            + (['-j', str(ctest_jobs)] if ctest_jobs and ctest_jobs > 1 else [])
        with stages('ctest'):
            code = run(test, logs / 'native-ctest.log')
    text = (logs / 'native-ctest.log').read_text(errors='replace')
    passed = re.search(r'\d+% tests passed(?:, \d+ tests failed)? out of \d+', text)
    closure = link_closure(text)
    if closure is None and (build_dir / 'Testing/Temporary/LastTest.log').exists():
        closure = link_closure((build_dir / 'Testing/Temporary/LastTest.log').read_text(errors='replace'))
    receipt.update(tests=passed.group(0) if passed else None, link_closure=closure,
                   test_seconds=sorted(junit_tests(junit), key=lambda t: -t['seconds']))
    if code and ctest_jobs and ctest_jobs > 1:
        # Several tests drive the game by frame and fail when other games share the cores (the
        # known load flakes): a parallel run's failures are run again alone, one at a time, and
        # the gate passes only if each then passes. Both results stay in the receipt.
        failed = [t['name'] for t in receipt['test_seconds'] if t['status'] not in ('run', 'skipped', 'notrun')]
        receipt['parallel_failures'] = failed
        if failed:
            again = logs / 'native-ctest-rerun.xml'
            with heavy_slots('native ctest rerun ' + build_dir.name, WEIGHTS['ctest']):
                with stages('ctest serial rerun'):
                    code = run(['ctest', '--test-dir', str(build_dir), '--output-junit', str(again), '-R',
                                '^(%s)$' % '|'.join(re.escape(n) for n in failed)], logs / 'native-ctest-rerun.log')
            receipt['serial_rerun'] = junit_tests(again)
            rerun = (logs / 'native-ctest-rerun.log').read_text(errors='replace')
            match = re.search(r'\d+% tests passed(?:, \d+ tests failed)? out of \d+', rerun)
            receipt['serial_rerun_tests'] = match.group(0) if match else None
            if code == 0 and len(receipt['serial_rerun']) != len(failed):
                code = 1
    if code:
        receipt['failed'] = 'ctest'
        return receipt
    if tests is None and closure is None:
        receipt['failed'] = 'link closure baseline not reported'
        return receipt
    if played:
        with stages('played check'):
            code = run(['/bin/zsh', '-c', played], logs / 'native-played.log')
        if code:
            receipt['failed'] = 'played check'
            return receipt
    receipt['ok'] = True
    return receipt


def table(receipt, stages, top=8):
    lines = [stages.table('native gate stages')]
    tests = receipt.get('test_seconds') or []
    if tests:
        lines.append('  slowest tests:')
        lines += ['    %-36s %7.1fs %s' % (t['name'], t['seconds'], '' if t['status'] == 'run' else t['status'])
                  for t in tests[:top]]
        lines.append('    (%d tests, %.1fs summed)' % (len(tests), sum(t['seconds'] for t in tests)))
    ninja = receipt.get('ninja') or {}
    if ninja.get('steps'):
        lines.append('  compile steps: %d, %.1fs summed; slowest %s' % (
            ninja['steps'], ninja['summed_seconds'],
            ', '.join('%s %.1fs' % (Path(s['output']).name, s['seconds']) for s in ninja['slowest'][:3])))
    return '\n'.join(lines)


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--build-dir', type=Path, default=REPOSITORY / 'build/native-play')
    parser.add_argument('--configure', action='append', default=[], help='an extra cmake configure argument')
    parser.add_argument('--tests', default=None, help='ctest -R REGEX (default: every test)')
    parser.add_argument('--jobs', type=int, default=None, help='cmake --build -j (default: ninja\'s)')
    parser.add_argument('--ctest-jobs', type=int, default=None,
                        help='ctest -j N; failures of the parallel run are run again alone (default: serial)')
    parser.add_argument('--played', default=None, help='a played-check command, run by zsh from the repository')
    parser.add_argument('--output', type=Path, default=None)
    parser.add_argument('--base', default=None, help='record gate_scope for BASE..HEAD')
    args = parser.parse_args()
    stages = Stages()
    output = (args.output or args.build_dir / 'native-gate.json').resolve()
    receipt = gate(args.build_dir, output.parent, args.configure, args.tests, args.jobs, args.played, stages, args.ctest_jobs)
    import gate_scope
    if args.base:
        paths = gate_scope.changed_paths(args.base)
        receipt['scope'] = dict(base=args.base, scope=gate_scope.scope(paths))
    receipt['rests_on_whole_file_receipt'] = gate_scope.resting_receipt()
    receipt['seconds'] = stages.total()
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(receipt, indent=1) + '\n')
    print(table(receipt, stages), file=sys.stderr)
    print(json.dumps(dict(ok=receipt['ok'], failed=receipt.get('failed'), tests=receipt.get('tests'),
                          link_closure=receipt.get('link_closure'), seconds=receipt['seconds'], receipt=str(output))))
    raise SystemExit(0 if receipt['ok'] else 1)


if __name__ == '__main__':
    main()
