#!/bin/zsh
# fix_xcu.sh UNIT...: re-name each unit's initializer rows from its .CRT$XCU table and re-learn
# (after a header change renumbers _$E), then one catalog (lane R).
cd "${0:A:h}/../.." || exit 1
P=${RA2_PYTHON:-python3}
LOCK=build/r/batch.lock
mkdir $LOCK 2>/dev/null || { echo "batch lock held ($(cat $LOCK/owner 2>/dev/null)); not starting"; exit 1; }
echo "pid $$ fix_xcu $(date -u +%H:%MZ)" > $LOCK/owner
trap 'rm -rf $LOCK' EXIT INT TERM
for u in "$@"; do
  (cd rebuild/tools && $P xcu.py $u | head -1 | sed "s/^/$u: /")
  $P -B rebuild/tools/rebuild.py check $u --learn >/dev/null 2>&1
done
$P -B rebuild/tools/rebuild.py catalog 2>&1 | head -1
