#!/bin/zsh
# land.sh UNIT...: the adoption pipeline for units whose text is staged in build/r/stage7 (or already in
# rebuild/src): conform externs, init, learn, header-static prologue, vtable slots (per-unit inline
# copies), guard what is not exact, re-learn; one summary line per unit, then one catalog (lane R2).
cd "${0:A:h}/../.." || exit 1
P=${RA2_PYTHON:-python3}
LOCK=build/r/batch.lock
mkdir $LOCK 2>/dev/null || { echo "batch lock held ($(cat $LOCK/owner 2>/dev/null)); not starting"; exit 1; }
echo "pid $$ land $(date -u +%H:%MZ)" > $LOCK/owner
trap 'rm -rf $LOCK' EXIT INT TERM
for u in "$@"; do
  [[ -f rebuild/src/$u.cpp ]] || /bin/cp -f build/r/stage7/$u.cpp rebuild/src/$u.cpp
  /bin/rm -rf build/r/inc.snap; /bin/cp -R rebuild/include build/r/inc.snap
  (cd rebuild/tools && $P -B move_externs.py $u >/dev/null 2>&1)
  $P -B rebuild/tools/rebuild.py init $u --auto >/dev/null 2>&1 || { echo "$u: init failed"; continue; }
  $P - $u <<'PY'
import json, sys
u = json.load(open('rebuild/manifest/units.json'))
info = u[sys.argv[1]]
gr = any('/GR' in o for o in info['main_options'])
info['options'] = ['/O2', '/Ob2', '/G5', '/Gy', '/Gr'] + (['/GR'] if gr else [])
open('rebuild/manifest/units.json', 'w').write(json.dumps(u, indent=1) + '\n')
PY
  first=$($P -B rebuild/tools/rebuild.py check $u --learn 2>&1 | grep -o "[0-9]*/[0-9]* rows exact\|error C[0-9]*" | head -1)
  if [[ $first == error* ]]; then
    # Park the unit: the headers go back to what they were, the text back to the stage.
    err=$($P -B rebuild/tools/rebuild.py check $u 2>&1 | grep -m1 "error C" | sed 's/.*\\src\\//' | cut -c1-120)
    /bin/rm -rf rebuild/include; /bin/cp -R build/r/inc.snap rebuild/include
    /bin/rm -f rebuild/src/$u.cpp
    $P - $u <<'PY'
import json, sys
p = 'rebuild/manifest/units.json'; u = json.load(open(p)); u.pop(sys.argv[1], None)
open(p, 'w').write(json.dumps(u, indent=1) + '\n')
PY
    echo "$u: parked, compile error: $err"; continue
  fi
  (cd rebuild/tools && $P xcu.py $u >/dev/null 2>&1; $P databind.py $u >/dev/null 2>&1)
  missing=$($P -B rebuild/tools/rebuild.py check $u --learn 2>/dev/null | grep -c "missing  _\$E\|differs  _\$E")
  if (( missing > 0 )); then rebuild/tools/prologue.sh $u >/dev/null 2>&1; fi
  for i in 1 2; do (cd rebuild/tools && $P vtables.py $u --learn >/dev/null 2>&1); $P -B rebuild/tools/rebuild.py check $u --learn >/dev/null 2>&1; done
  (cd rebuild/tools && $P xcu.py $u >/dev/null 2>&1)
  before=$($P -B rebuild/tools/rebuild.py check $u --learn 2>&1 | grep -o "[0-9]*/[0-9]* rows exact" | head -1)
  /bin/cp -f build/r/last/$u.json build/r/last/$u.preguard.json
  g=$(cd rebuild/tools && $P -B guard.py $u 2>&1 | head -1)
  for i in 1 2 3; do
    $P -B rebuild/tools/rebuild.py check $u --learn >/dev/null 2>&1; (cd rebuild/tools && $P xcu.py $u >/dev/null 2>&1)
    $P -B rebuild/tools/rebuild.py check $u --learn >/dev/null 2>&1
    r=$(cd rebuild/tools && $P -B guard.py $u --refine ../../build/r/last/$u.preguard.json 2>&1 | tail -1)
    [[ $r == *" 0 drafts unguarded"* ]] && break
  done
  g="$g; $r"
  $P -B rebuild/tools/rebuild.py check $u --learn >/dev/null 2>&1; (cd rebuild/tools && $P xcu.py $u >/dev/null 2>&1)
  after=$($P -B rebuild/tools/rebuild.py check $u --learn 2>&1 | grep -o "[0-9]*/[0-9]* rows exact\|error C[0-9]*: .*" | head -1)
  echo "$u: first $first; settled $before; $g; final $after"
done
$P -B rebuild/tools/rebuild.py catalog 2>&1 | head -1
