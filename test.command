#!/bin/zsh
# Unit tests of the tools (tests/).
set -eu
cd "${0:A:h}"
py="${RA2_PYTHON:-python3}"
exec "$py" -B -m unittest discover -s tests -p 'test*.py' "$@"
