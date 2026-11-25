#!/bin/zsh
# settle_all.sh: re-check every unit with learning, then one catalog, vtable-slot binding and catalog
# pass (after header changes or an interrupted batch) (lane R).
cd "${0:A:h}/../.." || exit 1
P=${RA2_PYTHON:-python3}
LOCK=build/r/batch.lock
mkdir $LOCK 2>/dev/null || { echo "batch lock held ($(cat $LOCK/owner 2>/dev/null)); not starting"; exit 1; }
echo "pid $$ settle_all $(date -u +%H:%MZ)" > $LOCK/owner
trap 'rm -rf $LOCK' EXIT INT TERM
units=($($P -c "import json;print(' '.join(json.load(open('manifests/units.json'))))"))
for u in $units; do echo "$u: $($P -B tools/original_target/rebuild.py check $u --learn 2>&1 | grep 'rows exact\|error' | head -1)"; done
$P -B tools/original_target/rebuild.py catalog >/dev/null 2>&1
for u in $units; do (cd tools/original_target && $P vtables.py $u --learn >/dev/null 2>&1); done
for u in $units; do $P -B tools/original_target/rebuild.py check $u --learn >/dev/null 2>&1; done
$P -B tools/original_target/rebuild.py catalog 2>&1 | head -1
