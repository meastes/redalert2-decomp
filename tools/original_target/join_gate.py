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
   reused: it carries the machine's census paths), with the link closure at its baseline
   (native_gate.py).
A native-only change (gate_scope.py, or --scope native) runs step 4 alone and records the exact
whole-file receipt it rests on. Step 4 runs the native test tier gate_scope.tier picks (--tier;
lane P2): fast, or full once the history has no recent full pass; a native build identical or
equivalent to a passing one runs the static checks only (native_gate.py, native_identity.py), and
--names-only fails a names-only landing whose native build differs until --explained says why. build.py takes the full-build lock for step 1 only, so a wrapper
should not hold it around the whole gate. Writes OUTPUT/join-gate.json (with stage timings) and
exits 1 on any failure. No join lands without it.
"""
import argparse
import json
from pathlib import Path
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
    parser.add_argument('--ctest-jobs', type=int, default=4, help='parallel ctest jobs (failures rerun one at a time)')
    parser.add_argument('--output', type=Path, default=REPOSITORY / 'build/join-gate')
    parser.add_argument('--tier', choices=['auto', 'fast', 'full'], default='auto',
                        help='native tests by label (native_gate.py --tier; auto: gate_scope.tier on BASE..HEAD)')
    parser.add_argument('--names-only', action='store_true',
                        help='a names-only landing: a native build that differs from the last passing one fails')
    parser.add_argument('--explained', default=None, help='why a names-only native build differs (the full set runs)')
    parser.add_argument('--scope', choices=['auto', 'exact', 'native'], default='auto',
                        help='auto (gate_scope.py on BASE..HEAD): a native-only change skips steps 1-3')
    args = parser.parse_args()
    out = args.output.resolve()
    out.parent.mkdir(parents=True, exist_ok=True)
    python = sys.executable
    import gate_scope
    import native_gate
    from job_control import Stages
    stages = Stages()
    scope = args.scope
    if scope == 'auto':
        scope = gate_scope.scope(gate_scope.changed_paths(args.base))
        scope = 'exact' if scope == 'none' else scope
    if scope == 'rebuild':
        # Only the rebuild tree (rebuild/) and docs changed: neither main's whole-file gate nor the
        # native tests apply; the rebuild gate does (private/tools/land_rebuild_lane.sh).
        receipt = dict(base=args.base, scope=scope, ok=True, checks={},
                       note='rebuild-only change: run the rebuild gate (catalog, structure gate, rebuild whole-file build)')
        out.mkdir(parents=True, exist_ok=True)
        (out / 'join-gate.json').write_text(json.dumps(receipt, indent=1) + '\n')
        print(receipt['note'])
        raise SystemExit(0)
    receipt = dict(base=args.base, scope=scope, checks={}, stages=stages.items)
    ok = True
    import shutil
    build_dir = out / 'build'
    if scope == 'native':
        # Nothing the exact build reads changed: no full-build lock, no whole-file build; the gate
        # rests on the last exact whole-file receipt for an ancestor of HEAD.
        resting = gate_scope.resting_receipt()
        receipt['rests_on_whole_file_receipt'] = resting
        receipt['checks']['whole_file_exact'] = dict(ok=resting is not None, skipped='native-only change',
                                                     rests_on=resting and resting['commit'])
        ok &= resting is not None
    else:
        # 1. whole-file build (build.py takes the full-build lock and heavy-job slots itself)
        shutil.rmtree(build_dir, ignore_errors=True)
        with stages('whole-file build'):
            code = run(['./build.command', '--keep-intermediates', '--output', str(build_dir)], out.parent / 'join-gate-build.log')
        found = None
        if (build_dir / 'build-receipt.json').exists():
            found = json.loads((build_dir / 'build-receipt.json').read_text())
        exact = code == 0 and found is not None and found.get('whole_file_exact') and found.get('output_sha256') == TARGET_SHA256
        receipt['checks']['whole_file_exact'] = dict(ok=bool(exact), sha256=found and found.get('output_sha256'),
                                                     tracked_sources=found and found.get('tracked_sources'),
                                                     build_stages=found and found.get('stages'))
        ok &= bool(exact)
        import whole_file_receipt
        whole_file_receipt.record('join_gate.py', bool(exact), found and found.get('output_sha256'), base=args.base)
        # 2. module maps and the RTTI condition
        if exact:
            with stages('module maps and RTTI'):
                run([python, '-B', 'tools/original_target/module_ledger.py', str(build_dir), '--write'], out.parent / 'join-gate-maps.log')
                units = joined_units(args.base)
                failed = rtti_check(units, args.base)
            receipt['checks']['rtti'] = dict(ok=not failed, units=len(units), failed=failed)
            ok &= not failed
        # 3. module agreement
        with stages('module check'):
            code = run([python, '-B', 'tools/original_target/module_check.py'], out.parent / 'join-gate-module-check.log')
        receipt['checks']['module_check'] = dict(ok=code == 0)
        ok &= code == 0
    # 4. native configure, build, ctest, link closure (heavy-job slots, never the full-build lock)
    tier, areas, tier_info = native_gate.tier_for(args.base, args.tier)
    receipt['native_tier'] = tier_info
    native = native_gate.gate(args.native_build, out.parent, stages=stages, ctest_jobs=args.ctest_jobs, tier=tier,
                              areas=areas, names_only=args.names_only, explained=args.explained)
    receipt['checks']['native'] = dict(ok=native['ok'], **{k: v for k, v in native.items() if k not in ('ok', 'stages')})
    ok &= native['ok']
    receipt['ok'] = bool(ok)
    receipt['seconds'] = stages.total()
    out.mkdir(parents=True, exist_ok=True)
    (out / 'join-gate.json').write_text(json.dumps(receipt, indent=1) + '\n')
    shutil.rmtree(build_dir, ignore_errors=True)      # the kept intermediates are large; the maps are written
    print(native_gate.table(native, stages), file=sys.stderr)
    print(json.dumps({k: v for k, v in receipt.items() if k != 'stages'}, indent=1))
    raise SystemExit(0 if ok else 1)


if __name__ == '__main__':
    main()
