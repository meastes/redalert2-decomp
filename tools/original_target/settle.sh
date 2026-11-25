#!/bin/zsh
# settle.sh UNIT: alternate symbol learning and vtable-slot binding until the unit's bindings settle.
cd "${0:A:h}/../.." || exit 1
P=${RA2_PYTHON:-python3}
for i in 1 2 3; do
  $P -B tools/original_target/rebuild.py check $1 --learn | grep "learned\|conflict" 
  $P -B tools/original_target/rebuild.py catalog >/dev/null
  (cd tools/original_target && $P vtables.py $1 --learn | grep "  bound\|no main")
done
$P -B tools/original_target/rebuild.py check $1 | grep "rows exact"
$P -B tools/original_target/rebuild.py catalog
