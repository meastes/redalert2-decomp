#!/bin/zsh
# Whole-file exact build of the original target.
#   ./build.command [build.py options]   the tree (src/, include/, asm/): its catalog manifests/catalog with the
#                                        build config the gate writes (build/r/rebuild-config.json, never tracked)
# The earlier tree (src/recovered, manifests/original_target) is the tag src-final; build it from a checkout
# of that tag with its own build.command.
set -eu
cd "${0:A:h}"
py="${RA2_PYTHON:-python3}"
config=build/r/rebuild-config.json
if [[ ! -f $config ]]; then
  print -u2 "build.command: $config is missing. The gate writes it (tools/original_target/rebuild.py catalog,"
  print -u2 "  after rebuild.py check of every unit); a fresh worktree can copy it with build/r/{check,last,xcu-table.json}"
  print -u2 "  from a gated checkout (docs/ORIGINAL_TARGET_BUILD.md)."
  exit 2
fi
exec "$py" -B tools/original_target/build.py --config $config --manifest manifests/catalog "$@"
