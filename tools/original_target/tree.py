"""The repository's one source tree (layout move at port parity, user decision 2026-11-24/25, lane O88).

Sources are one file per original translation unit under src/, one header per class under include/,
quoted includes spelled relative to include/ ("facing.h"), and every unit is compiled with the pinned
VC6 system headers searched after include/ (the original units included <windows.h> and the C runtime
headers directly). Handwritten assembly lives under asm/ with its ledger manifests/assembly.json.

The former two-tree switch (RA2_TREE=main|rebuild, 2026-11-17 to 2026-11-25) is gone: the earlier tree
(src/recovered + src/shared, catalog manifests/original_target/) survives only at the tag src-final, and
its catalog stays under manifests/original_target/ as read-only evidence the catalog rows cite.
REBUILD stays True for the code paths that once branched on the tree.
"""
REBUILD = True
SOURCES = 'src'                                     # catalogued sources
HEADERS = 'include'                                 # catalogued headers
INCLUDE_ROOT = 'include'                            # the compile's /I directory
HEADER_SUFFIXES = ('.h', '.hpp', '.hh', '.c')       # .c: MIDL interface-id files a unit includes
SOURCE_SUFFIXES = ('.cpp', '.c')
SOURCE_PARTS = len(SOURCES.split('/'))              # leading path parts naming the source tree
HEADER_PARTS = len(HEADERS.split('/'))
MIN_SOURCE_PARTS = SOURCE_PARTS + 1                 # src/x.cpp
ASM = 'asm'
ASSEMBLY = 'manifests/assembly.json'
