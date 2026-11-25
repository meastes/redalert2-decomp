#!/bin/zsh
# reinit.sh UNIT...: re-learn each unit's per-unit bindings from a clean table (reset_refs.py), name
# its initializers from the .CRT$XCU table, re-learn; units without the header statics then try the
# prologue orders; one summary line per unit, then one catalog (lane R2).
cd "${0:A:h}/../.." || exit 1
P=${RA2_PYTHON:-python3}
LOCK=build/r/batch.lock
mkdir $LOCK 2>/dev/null || { echo "batch lock held ($(cat $LOCK/owner 2>/dev/null)); not starting"; exit 1; }
echo "pid $$ reinit $(date -u +%H:%MZ)" > $LOCK/owner
trap 'rm -rf $LOCK' EXIT INT TERM
for u in "$@"; do
  (cd tools/original_target && $P reset_refs.py $u >/dev/null)
  $P -B tools/original_target/rebuild.py check $u --learn >/dev/null 2>&1
  (cd tools/original_target && $P xcu.py $u >/dev/null 2>&1)
  $P -B tools/original_target/rebuild.py check $u --learn >/dev/null 2>&1
  echo "$u: $($P -B tools/original_target/rebuild.py check $u --learn 2>&1 | grep -o '[0-9]*/[0-9]* rows exact')"
done
$P -B tools/original_target/rebuild.py catalog | head -1
