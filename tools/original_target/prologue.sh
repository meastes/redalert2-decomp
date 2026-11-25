#!/bin/zsh
# prologue.sh UNIT: the unit's header-static initializers (the _$E rows before its functions) come
# from geometry.h and rect.h: one of them, both in either order, or neither. Try each after always.h
# and keep the one that makes the most rows exact; a tie keeps the fewer includes (lane R; variants
# geometry-only and rect-only, and redoing an earlier prologue, lane R3). The order is evidence read
# from the original's table.
cd "${0:A:h}/../.." || exit 1
P=${RA2_PYTHON:-python3}
u=$1; src=src/$u.cpp
NOTE="RA2: the header statics, in the order of the unit's initializer table"
if grep -q '#include "geometry.h"' $src && ! grep -q "$NOTE" $src; then echo "$u: already has geometry.h"; exit 0; fi
# Strip an earlier prologue: the noted line and, right after it, a rect.h or geometry.h line.
$P - $src "$NOTE" <<'PY'
import sys, re
from pathlib import Path
p = Path(sys.argv[1]); lines = p.read_text().split('\n'); out = []; i = 0
while i < len(lines):
    if sys.argv[2] in lines[i]:
        if i + 1 < len(lines) and re.match(r'#include "(rect|geometry)\.h"\s*$', lines[i + 1]): i += 1
        i += 1; continue
    out.append(lines[i]); i += 1
p.write_text('\n'.join(out))
PY
/bin/cp -f $src build/r/prologue-$u.orig
count() { (cd tools/original_target && $P reset_refs.py $u >/dev/null); $P -B tools/original_target/rebuild.py check $u --learn >/dev/null 2>&1; (cd tools/original_target && $P xcu.py $u >/dev/null 2>&1); $P -B tools/original_target/rebuild.py check $u --learn >/dev/null 2>&1; $P -B tools/original_target/rebuild.py check $u --learn >/dev/null 2>&1; (cd tools/original_target && $P -B xcu.py $u --score 2>/dev/null || echo -999); }
base=$(count); best=$base; bestv=none
for v in g r gr rg; do
  /bin/cp -f build/r/prologue-$u.orig $src
  case $v in
    g) ins='#include "geometry.h"';;
    r) ins='#include "rect.h"';;
    gr) ins='#include "geometry.h"\n#include "rect.h"';;
    rg) ins='#include "rect.h"\n#include "geometry.h"';;
  esac
  $P - $src "$ins" "$NOTE" <<'PY'
import sys
from pathlib import Path
p = Path(sys.argv[1]); t = p.read_text(); ins = sys.argv[2].replace('\\n', '\n').split('\n')
note = ' // ' + sys.argv[3]
i = t.find('#include "always.h"')
block = ins[0] + note + '\n' + ''.join(l + '\n' for l in ins[1:])
t = t[:t.index('\n', i) + 1] + block + t[t.index('\n', i) + 1:] if i >= 0 else block + t
p.write_text(t)
PY
  n=$(count); if (( n > best )); then best=$n; bestv=$v; /bin/cp -f $src build/r/prologue-$u.best; fi
done
if [[ $bestv == none ]]; then /bin/cp -f build/r/prologue-$u.orig $src; count >/dev/null; else /bin/cp -f build/r/prologue-$u.best $src; count >/dev/null; fi
echo "$u: prologue $bestv (score $base -> $best: exact rows less stray initializers)"
