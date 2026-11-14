#!/usr/bin/env python3
"""The landing gate for own-bound joins (docs/DECISIONS.md "Own-bound joins widened"):

    join_gate.py --base REV [--native-build DIR] [--output DIR]

1. the whole-file build (build.py --keep-intermediates) must be exact (target SHA-256);
2. the module maps of that build are written (module_ledger.py --write); every unit whose
   joined rows changed since REV must still prove each RTTI structure it emits at its
   original address: no `??_R` symbol unproven that REV's map did not already list, none
   proven at REV missing now (a unit that fails is listed: its join is not taken; undo it
   with `refile_unit.py unjoin UNIT --base REV`);
3. module_check.py must pass;
4. the native tree must configure, build and pass ctest (the configured build folder is
   reused: it carries the machine's census paths), with the link closure at its baseline.
Writes OUTPUT/join-gate.json and exits 1 on any failure. No join lands without it.
"""
import argparse
import json
from pathlib import Path
import re
import subprocess
import sys

REPOSITORY = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))
TARGET_SHA256 = 'e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f'
MAPS = REPOSITORY / 'manifests/original_target/modules'
MODULES = REPOSITORY / 'manifests/original_target/modules.json'


def run(command, log, cwd=REPOSITORY):
    with open(log, 'w') as handle:
        return subprocess.run(command, cwd=cwd, stdout=handle, stderr=subprocess.STDOUT).returncode


def git_json(revision, path):
    result = subprocess.run(['git', 'show', '%s:%s' % (revision, path)], cwd=REPOSITORY, capture_output=True)
    return json.loads(result.stdout) if result.returncode == 0 else None


def joined_units(base):
    """Units whose `joined` list differs from REV's."""
    now = json.loads(MODULES.read_text())
    then = git_json(base, MODULES.relative_to(REPOSITORY).as_posix()) or {}
    return sorted(u for u, item in now.items() if item.get('joined', []) != then.get(u, {}).get('joined', []))


def rtti_sets(entry):
    proven = {c['symbol'] for c in entry.get('data', []) + entry.get('kept_elsewhere', []) if c.get('kind') == 'rtti'}
    unproven = {f.get('symbol') for f in entry.get('data_unproven', []) + entry.get('failures', [])
                if (f.get('symbol') or '').startswith('??_R')}
    return proven, unproven


def rtti_check(units, base):
    """{unit: reason} for units whose RTTI is no longer all proven."""
    out = {}
    for unit in units:
        path = MAPS / (unit + '.json')
        if not path.exists():
            out[unit] = 'no module map'
            continue
        now = json.loads(path.read_text())
        then = git_json(base, path.relative_to(REPOSITORY).as_posix())
        proven, unproven = rtti_sets(now)
        was_proven, was_unproven = rtti_sets(then) if then else (set(), set())
        new = sorted(unproven - was_unproven)
        lost = sorted(was_proven - proven - unproven)
        if new or (was_proven - proven):
            out[unit] = 'RTTI not proven: %s' % ', '.join((new + sorted(was_proven - proven))[:4])
        elif lost:
            out[unit] = 'RTTI no longer emitted: %s' % ', '.join(lost[:4])
    return out


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--base', required=True, help='the revision before the joins')
    parser.add_argument('--native-build', type=Path, default=REPOSITORY / 'build/native-play')
    parser.add_argument('--output', type=Path, default=REPOSITORY / 'build/join-gate')
    args = parser.parse_args()
    out = args.output.resolve()
    out.parent.mkdir(parents=True, exist_ok=True)
    python = sys.executable
    receipt = dict(base=args.base, checks={})
    ok = True
    # 1. whole-file build
    build_dir = out / 'build'
    import shutil
    shutil.rmtree(build_dir, ignore_errors=True)
    code = run(['./build.command', '--keep-intermediates', '--output', str(build_dir)], out.parent / 'join-gate-build.log')
    found = None
    if (build_dir / 'build-receipt.json').exists():
        found = json.loads((build_dir / 'build-receipt.json').read_text())
    exact = code == 0 and found is not None and found.get('whole_file_exact') and found.get('output_sha256') == TARGET_SHA256
    receipt['checks']['whole_file_exact'] = dict(ok=bool(exact), sha256=found and found.get('output_sha256'),
                                                 tracked_sources=found and found.get('tracked_sources'))
    ok &= bool(exact)
    # 2. module maps and the RTTI condition
    if exact:
        run([python, '-B', 'tools/original_target/module_ledger.py', str(build_dir), '--write'], out.parent / 'join-gate-maps.log')
        units = joined_units(args.base)
        failed = rtti_check(units, args.base)
        receipt['checks']['rtti'] = dict(ok=not failed, units=len(units), failed=failed)
        ok &= not failed
    # 3. module agreement
    code = run([python, '-B', 'tools/original_target/module_check.py'], out.parent / 'join-gate-module-check.log')
    receipt['checks']['module_check'] = dict(ok=code == 0)
    ok &= code == 0
    # 4. native configure, build, ctest, link closure
    native = args.native_build.resolve()
    if not (native / 'CMakeCache.txt').exists():
        receipt['checks']['native'] = dict(ok=False, reason='%s is not a configured native build' % native)
        ok = False
    else:
        steps = [('configure', ['cmake', '-S', 'native', '-B', str(native)]), ('build', ['cmake', '--build', str(native)]),
                 ('ctest', ['ctest', '--test-dir', str(native)])]
        result = {}
        for name, command in steps:
            code = run(command, out.parent / ('join-gate-native-%s.log' % name))
            result[name] = code == 0
            if code:
                break
        log = (out.parent / 'join-gate-native-ctest.log')
        text = log.read_text() if log.exists() else ''
        passed = re.search(r'\d+% tests passed(?:, \d+ tests failed)? out of \d+', text)
        closure = re.search(r'link-closure baseline: (\d+) undefined \(baseline (\d+)\) ok', text)
        if closure is None and (native / 'Testing/Temporary/LastTest.log').exists():
            closure = re.search(r'link-closure baseline: (\d+) undefined \(baseline (\d+)\) ok',
                                (native / 'Testing/Temporary/LastTest.log').read_text())
        result.update(tests=passed.group(0) if passed else None,
                      link_closure=int(closure.group(1)) if closure else None)
        native_ok = all(result.get(n) for n, _ in steps) and closure is not None
        receipt['checks']['native'] = dict(ok=native_ok, **result)
        ok &= native_ok
    receipt['ok'] = bool(ok)
    out.mkdir(parents=True, exist_ok=True)
    (out / 'join-gate.json').write_text(json.dumps(receipt, indent=1) + '\n')
    shutil.rmtree(build_dir, ignore_errors=True)      # the kept intermediates are large; the maps are written
    print(json.dumps(receipt, indent=1))
    raise SystemExit(0 if ok else 1)


if __name__ == '__main__':
    main()
