#!/bin/zsh
# Unit tests: the shared and frozen-tree tools (tests/), then the rebuild's own tools (rebuild/tests/).
set -eu
cd "${0:A:h}"
py="${RA2_PYTHON:-python3}"
"$py" -B -m unittest discover -s tests -p 'test*.py' "$@"
RA2_TREE=rebuild "$py" -B -m unittest discover -s rebuild/tests -t rebuild/tests -p 'test*.py' "$@"
