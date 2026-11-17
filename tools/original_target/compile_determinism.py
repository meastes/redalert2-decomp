#!/usr/bin/env python3
"""Compile module units repeatedly at the fixed compile folder and print what comes out.

    compile_determinism.py UNIT [UNIT ...] [--runs N] [--gr] [--cache]

Each run compiles the unit's compile group exactly as the build does (build.compile_units:
the fixed compile folder, docs/ORIGINAL_TARGET_BUILD.md "Fixed compile folder") with the
object cache off (unless --cache), and prints one line per run: the unit, the run, the
sha256 of the object with the COFF time stamp zeroed, and the attributes word of every
class hierarchy descriptor (??_R3) the object defines. --gr compiles the unit's population
with /GR added (a measurement only; nothing is written to the catalog). Identical lines for
every run of a unit mean the compile is deterministic on this host; the sha256 values are
comparable across hosts with the same pinned compiler.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import sys

REPOSITORY = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(Path(__file__).resolve().parent))

import catalog_io
from coff import Coff


def object_digest(data):
    data = bytearray(data)
    data[4:8] = b'\0\0\0\0'          # IMAGE_FILE_HEADER.TimeDateStamp
    return hashlib.sha256(bytes(data)).hexdigest()


def hierarchy_attributes(coff):
    return {s['name']: int.from_bytes(bytes(coff.sections[s['section'] - 1]['data'])[4:8], 'little')
            for s in coff.symbols.values() if s['name'].startswith('??_R3') and s['section'] > 0}


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('units', nargs='+')
    parser.add_argument('--runs', type=int, default=20)
    parser.add_argument('--gr', action='store_true')
    parser.add_argument('--cache', action='store_true', help='leave the object cache on')
    parser.add_argument('--config', type=Path, default=catalog_io.config_path(REPOSITORY))
    args = parser.parse_args()
    if not args.cache:
        os.environ['RA2_OBJECT_CACHE'] = 'off'
    import refile_unit
    config = json.loads(args.config.read_text())
    catalog = catalog_io.load(catalog_io.locate(REPOSITORY))
    modules = json.loads(refile_unit.MODULES.read_text())
    for unit in args.units:
        module = modules[unit]['source']
        pid = next(r['population'] for r in catalog['regions'] if r['source'] == module)
        if args.gr:
            for row in catalog['regions']:
                if row['population'] == pid and '/GR' not in row['options']:
                    row['options'] = list(row['options']) + ['/GR']
        text = (REPOSITORY / module).read_text(errors='replace')
        for run in range(args.runs):
            folder = REPOSITORY / 'build/determinism' / unit / str(run)
            if folder.exists():
                shutil.rmtree(folder)
            obj, log, _ = refile_unit.screen_compile(config, catalog, pid, module, text, folder)
            if obj is None:
                raise SystemExit('%s: compile failed (%s)' % (unit, folder / 'compile.log'))
            data = obj.read_bytes()
            attributes = sorted(set(hierarchy_attributes(Coff(data)).values()))
            print('%s %d %s %s' % (unit, run, object_digest(data), attributes), flush=True)
            shutil.rmtree(folder)


if __name__ == '__main__':
    main()
