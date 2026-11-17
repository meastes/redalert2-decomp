#!/usr/bin/env python3
"""yr_adopt_tibsun.py <glue .inc> <TibSun file> <signature prefix>... : append TibSun archive
functions (the text from a line starting with each prefix to its closing brace at column 0) to a
native/yr/glue include, under an attribution header written once (lane Y3). The archive is read
from $TIBSUN_ARCHIVE (default: private/reference/TibSun under the main checkout)."""
import os, sys
from pathlib import Path
W = Path(__file__).resolve().parents[3]
root = os.environ.get('TIBSUN_ARCHIVE') or next((str(r / 'private/reference/TibSun') for r in [W] + list(W.parents)
                                                 if (r / 'private/reference/TibSun').exists()), '')
out, name, sigs = Path(sys.argv[1]), sys.argv[2], sys.argv[3:]
src = (Path(root) / 'code' / name).read_text(encoding='latin-1').split('\n')
text = '' if not out.exists() else out.read_text(encoding='latin-1')
if 'TibSun archive code/%s' % name not in text:
    text += ('\n// ' + '-' * 93 + '\n// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun\'s\n'
             '// (TibSun archive code/%s; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and\n'
             '// OpenTS contributors, EA\'s GPL section 7 terms; THIRD_PARTY.md); native changes are marked\n'
             '// NATIVE. Behaviour code for the native build; never part of an exact build.\n// ' % name + '-' * 93 + '\n')
for sig in sigs:
    starts = [i for i, l in enumerate(src) if l.startswith(sig)]
    if not starts:
        sys.exit('no %r in %s' % (sig, name))
    i = starts[-1]
    j = i + next(k for k, l in enumerate(src[i:]) if l.startswith('}'))
    text += '\n' + '\n'.join(src[i:j + 1]) + '\n'
    print('adopted %s (%d lines)' % (sig, j - i + 1))
out.write_text(text, encoding='latin-1')
