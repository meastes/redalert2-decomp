#!/bin/zsh
# reguard.sh UNIT...: guard the unit's remaining drafts (guard.py), re-learn and refine, as the tail of
# land.sh; one summary line per unit, then one catalog (lane R2).
cd "${0:A:h}/../.." || exit 1
P=${RA2_PYTHON:-python3}
LOCK=build/r/batch.lock
mkdir $LOCK 2>/dev/null || { echo "batch lock held ($(cat $LOCK/owner 2>/dev/null)); not starting"; exit 1; }
echo "pid $$ reguard $(date -u +%H:%MZ)" > $LOCK/owner
trap 'rm -rf $LOCK' EXIT INT TERM
for u in "$@"; do
  $P -B tools/original_target/rebuild.py check $u --learn >/dev/null 2>&1
  before=$($P -B tools/original_target/rebuild.py check $u --learn 2>&1 | grep -o "[0-9]*/[0-9]* rows exact" | head -1)
  /bin/cp -f build/r/last/$u.json build/r/last/$u.preguard.json; /bin/cp -f src/$u.cpp build/r/$u.preguard.cpp
  g=$(cd tools/original_target && $P -B guard.py $u 2>&1 | head -1)
  for i in 1 2 3; do
    $P -B tools/original_target/rebuild.py check $u --learn >/dev/null 2>&1; (cd tools/original_target && $P xcu.py $u >/dev/null 2>&1)
    $P -B tools/original_target/rebuild.py check $u --learn >/dev/null 2>&1
    r=$(cd tools/original_target && $P -B guard.py $u --refine ../../build/r/last/$u.preguard.json 2>&1 | tail -1)
    [[ $r == *" 0 drafts unguarded"* ]] && break
  done
  after=$($P -B tools/original_target/rebuild.py check $u --learn 2>&1 | grep -o "[0-9]*/[0-9]* rows exact\|error C[0-9]*: .*" | head -1)
  # Guards that break the compile or cost exact rows are taken back: the drafts stay unguarded.
  if [[ $after == error* || ${after%%/*} -lt ${before%%/*} ]]; then
    /bin/cp -f build/r/$u.preguard.cpp src/$u.cpp
    $P -B tools/original_target/rebuild.py check $u --learn >/dev/null 2>&1; (cd tools/original_target && $P xcu.py $u >/dev/null 2>&1)
    after="$($P -B tools/original_target/rebuild.py check $u --learn 2>&1 | grep -o "[0-9]*/[0-9]* rows exact" | head -1) (guards taken back)"
  fi
  echo "$u: before $before; $g; $r; final $after"
done
$P -B tools/original_target/rebuild.py catalog 2>&1 | head -1
