#!/bin/zsh
# finish.sh [UNIT...]: for every unit (default: all in units.json) bind vtable slots, try the
# header-static prologue where initializer rows are missing, re-learn, then one catalog (lane R).
cd "${0:A:h}/../.." || exit 1
P=${RA2_PYTHON:-python3}
LOCK=build/r/batch.lock
mkdir $LOCK 2>/dev/null || { echo "batch lock held ($(cat $LOCK/owner 2>/dev/null)); not starting"; exit 1; }
echo "pid $$ finish $(date -u +%H:%MZ)" > $LOCK/owner
trap 'rm -rf $LOCK' EXIT INT TERM
units=("$@"); [[ ${#units} -eq 0 ]] && units=($($P -c "import json;print(' '.join(json.load(open('manifests/units.json'))))"))
$P -B tools/original_target/rebuild.py catalog >/dev/null 2>&1
for u in $units; do (cd tools/original_target && $P vtables.py $u --learn >/dev/null 2>&1); done
for u in $units; do
  missing=$($P -B tools/original_target/rebuild.py check $u --learn 2>/dev/null | grep -c "missing  _\$E\|differs  _\$E")
  if (( missing > 0 )); then tools/original_target/prologue.sh $u; fi
done
$P -B tools/original_target/rebuild.py catalog 2>&1 | head -1
