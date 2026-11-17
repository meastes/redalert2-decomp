#!/bin/zsh
# prologue.sh UNIT: the unit's header-static initializers (the _$E rows before its functions) come
# from geometry.h and rect.h in one of two orders; try both after always.h and keep the one that
# makes the most rows exact (lane R; the order is evidence read from the original's table).
cd "${0:A:h}/../.." || exit 1
P=${RA2_PYTHON:-python3}
u=$1; src=rebuild/src/$u.cpp
grep -q '#include "geometry.h"' $src && { echo "$u: already has geometry.h"; exit 0; }
/bin/cp -f $src build/r/prologue-$u.orig
count() { (cd rebuild/tools && $P reset_refs.py $u >/dev/null); $P -B rebuild/tools/rebuild.py check $u --learn >/dev/null 2>&1; (cd rebuild/tools && $P xcu.py $u >/dev/null 2>&1); $P -B rebuild/tools/rebuild.py check $u --learn 2>/dev/null | grep -o "[0-9]*/[0-9]* rows" | cut -d/ -f1; }
base=$(count); best=$base; bestv=none
for v in gr rg; do
  /bin/cp -f build/r/prologue-$u.orig $src
  if [[ $v == gr ]]; then ins='#include "geometry.h"\n#include "rect.h"'; else ins='#include "rect.h"\n#include "geometry.h"'; fi
  $P - $src "$ins" <<'PY'
import sys
from pathlib import Path
p = Path(sys.argv[1]); t = p.read_text(); ins = sys.argv[2].replace('\\n', '\n')
note = ' // RA2: the header statics, in the order of the unit\'s initializer table'
i = t.find('#include "always.h"')
t = t[:t.index('\n', i) + 1] + ins.split('\n')[0] + note + '\n' + '\n'.join(ins.split('\n')[1:]) + '\n' + t[t.index('\n', i) + 1:] if i >= 0 else ins + '\n' + t
p.write_text(t)
PY
  n=$(count); if (( n > best )); then best=$n; bestv=$v; /bin/cp -f $src build/r/prologue-$u.best; fi
done
if [[ $bestv == none ]]; then /bin/cp -f build/r/prologue-$u.orig $src; $P -B rebuild/tools/rebuild.py check $u --learn >/dev/null 2>&1; else /bin/cp -f build/r/prologue-$u.best $src; count >/dev/null; fi
echo "$u: prologue $bestv ($base -> $best rows)"
