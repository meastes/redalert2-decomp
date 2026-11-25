#!/bin/zsh
# batch.sh UNIT...: adopt, initialise and bind each unit, one summary line per unit; then one catalog
# pass with vtable-slot binding for the whole batch (lane R).
cd "${0:A:h}/../.." || exit 1
P=${RA2_PYTHON:-python3}
# One writer of manifests at a time: an atomic lock directory, released on exit.
LOCK=build/r/batch.lock
mkdir -p build/r
mkdir $LOCK 2>/dev/null || { echo "batch lock held ($LOCK: $(cat $LOCK/owner 2>/dev/null)); not starting"; exit 1; }
echo "pid $$ started $(date -u +%H:%MZ)" > $LOCK/owner
trap 'rm -rf $LOCK' EXIT INT TERM
for u in "$@"; do
  $P -B tools/original_target/rebuild.py adopt $u >/dev/null 2>&1
  $P -B tools/original_target/rebuild.py init $u --auto >/dev/null 2>&1 || { echo "$u: init failed"; continue; }
  $P - $u <<'PY'
import json, sys
u = json.load(open('manifests/units.json'))
info = u[sys.argv[1]]
gr = any('/GR' in o for o in info['main_options'])
info['options'] = ['/O2', '/Ob2', '/G5', '/Gy', '/Gr'] + (['/GR'] if gr else [])
open('manifests/units.json', 'w').write(json.dumps(u, indent=1) + '\n')
PY
  $P -B tools/original_target/rebuild.py check $u --learn >/dev/null 2>&1
  (cd tools/original_target && $P xcu.py $u >/dev/null 2>&1; $P databind.py $u >/dev/null 2>&1)
  r=$($P -B tools/original_target/rebuild.py check $u --learn 2>&1 | grep "rows exact\|error" | head -2 | tr '\n' ' ')
  echo "$u: $r"
done
$P -B tools/original_target/rebuild.py catalog >/dev/null 2>&1
for u in "$@"; do (cd tools/original_target && $P vtables.py $u --learn >/dev/null 2>&1); done
for u in "$@"; do $P -B tools/original_target/rebuild.py check $u --learn >/dev/null 2>&1; done
$P -B tools/original_target/rebuild.py catalog 2>&1 | head -1
