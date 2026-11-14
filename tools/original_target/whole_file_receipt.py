#!/usr/bin/env python3
"""The last whole-file gate's result at one fixed path (review request 2026-11-14).

Every whole-file gate (./build.command through build.py, join_gate.py) calls record() once it
knows the result. It overwrites

    <repository>/build/whole-file-receipt.json
        {"commit": HEAD sha, "dirty": true when tracked files differ from HEAD (omitted otherwise),
         "whole_file_exact": bool, "output_sha256": sha or null, "time_utc": ISO 8601 Z,
         "gate": "build.command" | "join_gate.py" | ..., "worktree": the checkout's path, ...extra}

and appends the same object as one line to the main checkout's build/whole-file-receipts.jsonl
(the main checkout is the parent of `git rev-parse --git-common-dir`, so a worktree's gate also
lands in the shared history). A failure to write either file is reported on stderr and never
fails the gate itself.

    whole_file_receipt.py [--repository DIR]   print the receipt at the fixed path
"""
import argparse
from datetime import datetime, timezone
import json
from pathlib import Path
import subprocess
import sys

REPOSITORY = Path(__file__).resolve().parents[2]
RECEIPT = 'build/whole-file-receipt.json'
HISTORY = 'build/whole-file-receipts.jsonl'


def git(repository, *args):
    return subprocess.run(['git', '-C', str(repository), *args], capture_output=True, text=True, check=True).stdout


def checkout_state(repository):
    """(HEAD sha, dirty, main checkout root): dirty means a tracked file differs from HEAD."""
    commit = git(repository, 'rev-parse', 'HEAD').strip()
    dirty = bool(git(repository, 'status', '--porcelain', '--untracked-files=no').strip())
    common = Path(git(repository, 'rev-parse', '--git-common-dir').strip())
    if not common.is_absolute():
        common = (Path(repository) / common).resolve()
    return commit, dirty, common.parent


def receipt(gate, whole_file_exact, output_sha256, commit, dirty, worktree, now=None, **extra):
    now = now or datetime.now(timezone.utc)
    out = dict(commit=commit)
    if dirty:
        out['dirty'] = True
    out.update(whole_file_exact=bool(whole_file_exact), output_sha256=output_sha256,
               time_utc=now.strftime('%Y-%m-%dT%H:%M:%SZ'), gate=gate, worktree=str(worktree))
    out.update(extra)
    return out


def write(repository, main_root, document):
    """Overwrite the checkout's fixed receipt; append one line to the main checkout's history."""
    path = Path(repository) / RECEIPT
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(document, indent=1) + '\n')
    history = Path(main_root) / HISTORY
    history.parent.mkdir(parents=True, exist_ok=True)
    with history.open('a') as stream:
        stream.write(json.dumps(document, sort_keys=True) + '\n')
    return path, history


def record(gate, whole_file_exact, output_sha256, repository=REPOSITORY, **extra):
    """Write the receipt for a finished gate; never raises."""
    try:
        commit, dirty, main_root = checkout_state(repository)
        document = receipt(gate, whole_file_exact, output_sha256, commit, dirty, Path(repository).resolve(), **extra)
        write(repository, main_root, document)
        return document
    except Exception as error:     # the receipt is a record of the gate, not part of it
        print('whole-file receipt not written: %s' % error, file=sys.stderr)
        return None


def main():
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('--repository', type=Path, default=REPOSITORY)
    args = parser.parse_args()
    path = args.repository / RECEIPT
    if not path.exists():
        raise SystemExit('no receipt at %s' % path)
    print(path.read_text(), end='')


if __name__ == '__main__':
    main()
