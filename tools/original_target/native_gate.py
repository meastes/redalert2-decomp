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

Tiers and the identical-build rule (lane P2, 2026-11-16):

    native_gate.py ... [--tier auto|fast|full] [--names-only [--explained TEXT]] [--no-identity]

* --tier picks the tests by their ctest labels (native/CMakeLists.txt "Gate tiers"): "fast" runs
  the static checks, the fast played checks and the slow tests whose area the change touches
  (gate_scope.py AREAS, from --base); "full" runs every test; "auto" (the default with --base)
  is gate_scope.tier: fast, or full when the history has no full pass from the last
  FULL_TIER_HOURS hours. Without --base and --tier the gate runs every test, as before.
* After the compile the gate records the build's identity (native_identity.py) and compares it
  with the builds whose played tests passed (HISTORY, in the main checkout). A build identical or
  equivalent to one whose run covered the chosen tier, with the same test scripts, runs the static
  checks only; the receipt's `played_skipped` names the passing receipt it rests on.
* --names-only declares a names-only landing: a build that is not identical or equivalent to the
  last passing build fails the gate (`failed: names-only build differs`, with the differing inputs)
  until --explained TEXT says why; then the full set runs and the explanation is kept.
* Every passing run that played tests appends a row to HISTORY with its identity and receipt.
"""
import argparse
import gzip
import json
import os
from pathlib import Path
import re
import shlex
import subprocess
import sys
import xml.etree.ElementTree as ElementTree

sys.path.insert(0, str(Path(__file__).resolve().parent))
from job_control import Stages, WEIGHTS, control_directory, heavy_slots  # noqa: E402

REPOSITORY = Path(__file__).resolve().parents[2]
HISTORY_DIR = 'native-gate'          # in build/owner-watch of the main checkout
HISTORY_ROWS = 40                    # rows a build is compared with (newest first)


def history_dir():
    return control_directory(REPOSITORY) / HISTORY_DIR


def read_history():
    """The native gate history rows, oldest first."""
    rows = []
    try:
        text = (history_dir() / 'history.jsonl').read_text()
    except OSError:
        return rows
    for line in text.splitlines():
        try:
            rows.append(json.loads(line))
        except ValueError:
            continue
    return rows


def load_identity(path):
    with (gzip.open if str(path).endswith('.gz') else open)(path, 'rt') as handle:
        return json.load(handle)


def commit_state():
    def git(*args):
        return subprocess.run(['git', '-C', str(REPOSITORY), *args], capture_output=True, text=True).stdout.strip()
    return git('rev-parse', 'HEAD'), bool(git('status', '--porcelain', '--untracked-files=no'))


def record_history(receipt, identity_file):
    """Append a passing played run (its identity and receipt copied beside the history)."""
    import time
    directory = history_dir()
    directory.mkdir(parents=True, exist_ok=True)
    commit, dirty = commit_state()
    stamp = '%d-%d' % (int(time.time()), os.getpid())
    kept_identity = directory / ('identity-%s.json.gz' % stamp)
    with open(identity_file, 'rb') as source, gzip.open(kept_identity, 'wb') as target:
        target.write(source.read())
    kept_receipt = directory / ('receipt-%s.json' % stamp)
    kept_receipt.write_text(json.dumps(receipt, indent=1) + '\n')
    row = dict(time=time.time(), commit=commit, dirty=dirty, ok=True, tier=receipt.get('tier'),
               slow_tests=receipt.get('slow_tests_run', []), identity=kept_identity.name, receipt=kept_receipt.name,
               worktree=str(REPOSITORY))
    with open(directory / 'history.jsonl', 'a') as handle:
        handle.write(json.dumps(row) + '\n')
    return row


def covers(row_tier, wanted):
    return row_tier == 'full' or row_tier == wanted


def resting_build(identity, wanted_tier, history=None):
    """(row, comparison) of the newest passing played run whose build is identical or equivalent
    to `identity`, with the same test scripts and a tier covering `wanted_tier`; else (None, the
    comparison with the newest passing run, or None)."""
    import native_identity
    rows = [r for r in (history if history is not None else read_history()) if r.get('ok') and r.get('identity')]
    newest = None
    for row in reversed(rows[-HISTORY_ROWS:]):
        try:
            old = load_identity(history_dir() / row['identity'])
        except (OSError, ValueError):
            continue
        result = native_identity.compare(old, identity)
        if newest is None:
            newest = (row, result)
        if result['verdict'] != 'different' and result['tests_same'] and covers(row.get('tier'), wanted_tier):
            return row, result
    return None, newest


def ctest_tests(build_dir):
    """{name: [labels]} of the configured tests."""
    out = subprocess.run(['ctest', '--test-dir', str(build_dir), '--show-only=json-v1'], capture_output=True, text=True)
    try:
        data = json.loads(out.stdout)
    except ValueError:
        return {}
    tests = {}
    for test in data.get('tests', []):
        labels = []
        for prop in test.get('properties', []):
            if prop.get('name') == 'LABELS':
                labels = prop.get('value') or []
        tests[test['name']] = labels
    return tests


def select_tests(tests, tier, areas=(), played=True):
    """Names to run: the static checks always; the played ones by tier (fast: the fast tier and the
    slow tests of the touched areas; full: all)."""
    chosen = []
    for name, labels in tests.items():
        if 'static' in labels:
            chosen.append(name)
        elif not played:
            continue
        elif tier == 'full' or 'fast' in labels or not labels:
            chosen.append(name)
        elif 'slow' in labels and any('area:' + a in labels for a in areas):
            chosen.append(name)
    return chosen


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


def gate(build_dir, logs, configure=(), tests=None, jobs=None, played=None, stages=None, ctest_jobs=None,
         tier=None, areas=(), names_only=False, explained=None, identity=True, record=True):
    """Run the native gate; returns its receipt (receipt['ok'] tells the result).

    tier None (and tests None): every test, as before the tiers; 'fast' or 'full': by label.
    identity: compare the build with the history and skip the played tests of an equal build."""
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
    identity_file = logs / 'native-identity.json'
    played_run = True
    selected = None
    if tests is None and (identity or names_only):
        import native_identity
        with stages('identity'):
            try:
                record_now = native_identity.snapshot(build_dir)
                identity_file.write_text(json.dumps(record_now, separators=(',', ':')) + '\n')
            except Exception as error:          # an unreadable build never skips anything
                record_now = None
                receipt['identity_error'] = '%s: %s' % (type(error).__name__, error)
            row, result = (None, None)
            if record_now is not None:
                row, result = resting_build(record_now, tier or 'full')
        if row is not None:
            receipt['played_skipped'] = dict(verdict=result['verdict'], rests_on=dict(
                receipt=row['receipt'], commit=row.get('commit'), time=row.get('time'), tier=row.get('tier'),
                slow_tests=row.get('slow_tests', [])), history=str(history_dir()))
            played_run = False
        elif names_only:
            newest = result
            verdict = newest[1] if newest else None
            receipt['names_only'] = dict(compared_with=newest and newest[0].get('receipt'),
                                         verdict=verdict and verdict['verdict'],
                                         reasons=verdict and verdict['reasons'][:20], explained=explained)
            if verdict is None or verdict['verdict'] == 'different':
                if not explained:
                    receipt['failed'] = 'names-only build differs' if verdict else 'names-only: no passing build to compare'
                    return receipt
                tier = 'full'
    if tests is None and tier is not None:
        listed = ctest_tests(build_dir)
        selected = select_tests(listed, tier, areas, played=played_run)
        receipt.update(tier=tier if played_run else 'static only', areas=list(areas),
                       slow_tests_run=sorted(n for n in selected if 'slow' in listed.get(n, [])),
                       tests_selected=len(selected), tests_configured=len(listed))
    elif tests is None and not played_run:
        listed = ctest_tests(build_dir)
        selected = select_tests(listed, 'full', played=False)
        receipt.update(tier='static only', tests_selected=len(selected), tests_configured=len(listed))
    else:
        receipt.update(tier='full' if tests is None else 'regex', slow_tests_run=[])
    if tests is None and selected is None:
        receipt['slow_tests_run'] = sorted(n for n, labels in ctest_tests(build_dir).items() if 'slow' in labels)
    if selected is not None and not selected:
        receipt['failed'] = 'no tests selected'
        return receipt
    regex = tests if tests else ('^(%s)$' % '|'.join(re.escape(n) for n in selected) if selected else None)
    junit = logs / 'native-ctest.xml'
    with heavy_slots('native ctest ' + build_dir.name, WEIGHTS['ctest'] * ((max(1, ctest_jobs or 1) + 1) // 2)):
        stages.mark('ctest slot wait')
        test = ['ctest', '--test-dir', str(build_dir), '--output-junit', str(junit)] + (['-R', regex] if regex else []) \
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
    if tests is None and closure is None and (selected is None or 'native-link-closure' in selected):
        receipt['failed'] = 'link closure baseline not reported'
        return receipt
    if played:
        with stages('played check'):
            code = run(['/bin/zsh', '-c', played], logs / 'native-played.log')
        if code:
            receipt['failed'] = 'played check'
            return receipt
    receipt['ok'] = True
    receipt['played_tests_ran'] = played_run
    if record and played_run and tests is None and identity_file.exists():
        receipt['history_row'] = record_history(dict(receipt, stages=list(stages.items)), identity_file)
    return receipt


def table(receipt, stages, top=8):
    lines = [stages.table('native gate stages')]
    if receipt.get('tier'):
        lines.append('  tier: %s%s' % (receipt['tier'], (' (slow: %s)' % ', '.join(receipt['slow_tests_run']))
                                         if receipt.get('slow_tests_run') else ''))
    skipped = receipt.get('played_skipped')
    if skipped:
        lines.append('  played tests skipped: build %s to the passing run of %s (%s, receipt %s)' % (
            skipped['verdict'], (skipped['rests_on'].get('commit') or '?')[:10], skipped['rests_on'].get('tier'),
            skipped['rests_on'].get('receipt')))
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


def tier_for(base, requested):
    """(tier or None, areas, scope record or None) for a gate on BASE..HEAD."""
    import gate_scope
    if base is None and requested is None:
        return None, [], None
    paths = gate_scope.changed_paths(base) if base else []
    chosen, touched, reason = gate_scope.tier(paths, requested or 'auto', read_history())
    info = dict(base=base, scope=gate_scope.scope(paths) if base else None, tier=chosen, areas=touched, reason=reason)
    return chosen, touched, info


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
    parser.add_argument('--base', default=None, help='record gate_scope for BASE..HEAD (and pick the tier from it)')
    parser.add_argument('--tier', choices=['auto', 'fast', 'full'], default=None,
                        help='tests by label (default: auto with --base, else every test)')
    parser.add_argument('--names-only', action='store_true', help='a names-only landing: a differing build fails')
    parser.add_argument('--explained', default=None, help='why a names-only build differs (then the full set runs)')
    parser.add_argument('--no-identity', action='store_true', help='always run the played tests')
    args = parser.parse_args()
    stages = Stages()
    output = (args.output or args.build_dir / 'native-gate.json').resolve()
    import gate_scope
    tier, areas, scope_info = tier_for(args.base, args.tier)
    receipt = gate(args.build_dir, output.parent, args.configure, args.tests, args.jobs, args.played, stages, args.ctest_jobs,
                   tier=tier, areas=areas, names_only=args.names_only, explained=args.explained,
                   identity=not args.no_identity)
    if scope_info:
        receipt['scope'] = scope_info
    receipt['rests_on_whole_file_receipt'] = gate_scope.resting_receipt()
    receipt['seconds'] = stages.total()
    output.parent.mkdir(parents=True, exist_ok=True)
    output.write_text(json.dumps(receipt, indent=1) + '\n')
    print(table(receipt, stages), file=sys.stderr)
    print(json.dumps(dict(ok=receipt['ok'], failed=receipt.get('failed'), tests=receipt.get('tests'),
                          tier=receipt.get('tier'), played_skipped=receipt.get('played_skipped'),
                          link_closure=receipt.get('link_closure'), seconds=receipt['seconds'], receipt=str(output))))
    raise SystemExit(0 if receipt['ok'] else 1)


if __name__ == '__main__':
    main()
