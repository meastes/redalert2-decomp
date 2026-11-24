#!/bin/zsh
# Whole-file exact build of the original target.
#   ./build.command [build.py options]              rebuild/, the tree of record (user, 2026-11-24): its catalog
#                                                   rebuild/manifest/catalog with the build config the rebuild gate
#                                                   writes (build/r/rebuild-config.json, never tracked)
#   ./build.command --tree src [build.py options]   src/, frozen at the tag src-final: main's catalog
#                                                   (manifests/original_target), unchanged; the native ports build from it
set -eu
cd "${0:A:h}"
py="${RA2_PYTHON:-python3}"
tree=rebuild
if [[ ${1:-} == --tree ]]; then
  tree=${2:?--tree takes rebuild or src}; shift 2
elif [[ ${1:-} == --tree=* ]]; then
  tree=${1#--tree=}; shift
fi
case $tree in
  src|main)
    export RA2_TREE=main
    exec "$py" -B tools/original_target/build.py "$@" ;;
  rebuild)
    export RA2_TREE=rebuild
    config=build/r/rebuild-config.json
    if [[ ! -f $config ]]; then
      print -u2 "build.command: $config is missing. The rebuild gate writes it (rebuild/tools/rebuild.py catalog,"
      print -u2 "  after rebuild.py check of every unit); a fresh worktree can copy it with build/r/{check,last,xcu-table.json}"
      print -u2 "  from a gated checkout (docs/ORIGINAL_TARGET_BUILD.md). The frozen tree builds with: ./build.command --tree src"
      exit 2
    fi
    exec "$py" -B tools/original_target/build.py --config $config --manifest rebuild/manifest/catalog "$@" ;;
  *)
    print -u2 "build.command: --tree must be rebuild or src"; exit 2 ;;
esac
