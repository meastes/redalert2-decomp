#!/usr/bin/env python3
"""The landing gate's whole-file step for a change (one tree since the layout move, lane O88, 2026-11-25):

    join_gate.py --base REV [--output DIR]

gate_scope.py decides what REV..HEAD needs:
- "frozen" (manifests/original_target/, the earlier tree's catalog kept as evidence at src-final): refused;
- "none" (documentation only): nothing to build;
- "exact": the whole-file build (./build.command, build.py takes the full-build lock itself) must be exact
  (target SHA-256), and the result is recorded as a whole-file receipt.

The rest of a landing (catalog of the touched units, structure gate, progress.csv) is the landing script's
(private/tools/land_rebuild_lane.sh). The earlier own-bound join steps (module maps, RTTI, module_check)
and the native ctest step belonged to the retired src/ tree and the native ports (now
github.com/meastes/redalert2-modern); they are at the tag src-final.
Writes OUTPUT/join-gate.json (with stage timings) and exits 1 on any failure.
"""
import argparse
import json
from pathlib import Path
import shutil
import subprocess
import sys

REPOSITORY = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))
TARGET_SHA256 = 'e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f'


def run(command, log, cwd=REPOSITORY):
    with open(log, 'w') as handle:
        return subprocess.run(command, cwd=cwd, stdout=handle, stderr=subprocess.STDOUT).returncode


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--base', required=True, help='the revision before the change')
    parser.add_argument('--output', type=Path, default=REPOSITORY / 'build/join-gate')
    args = parser.parse_args()
    out = args.output.resolve()
    out.mkdir(parents=True, exist_ok=True)
    import gate_scope
    from job_control import Stages
    stages = Stages()
    scope = gate_scope.scope(gate_scope.changed_paths(args.base))
    if scope == 'frozen':
        print('refused: the change touches manifests/original_target/, frozen at src-final (read-only evidence)')
        raise SystemExit(1)
    receipt = dict(base=args.base, scope=scope, checks={}, stages=stages.items)
    ok = True
    if scope == 'exact':
        build_dir = out / 'build'
        shutil.rmtree(build_dir, ignore_errors=True)
        with stages('whole-file build'):
            code = run(['./build.command', '--output', str(build_dir)], out.parent / 'join-gate-build.log')
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
        shutil.rmtree(build_dir, ignore_errors=True)
    receipt['ok'] = bool(ok)
    receipt['seconds'] = stages.total()
    (out / 'join-gate.json').write_text(json.dumps(receipt, indent=1) + '\n')
    print(json.dumps({k: v for k, v in receipt.items() if k != 'stages'}, indent=1))
    raise SystemExit(0 if ok else 1)


if __name__ == '__main__':
    main()
