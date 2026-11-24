"""Which source tree the canonical tools read (lane R, rebuild trial, 2026-11-17).

The default is main's tree: sources under src/recovered, shared headers under src/shared,
quoted includes spelled relative to src/ ("shared/x.hpp").

RA2_TREE=rebuild selects the rebuild trial's unit-file tree: one source per original
translation unit under rebuild/src, one header per class under rebuild/include, quoted
includes spelled relative to rebuild/include ("facing.h"), and every unit compiled with the
pinned VC6 system headers searched after rebuild/include (no RA2_SDK_HEADERS switch: the
original units included <windows.h> and the C runtime headers directly).
"""
import os

NAME = os.environ.get('RA2_TREE', 'main')
if NAME not in ('main', 'rebuild'):
    raise ValueError('RA2_TREE must be main or rebuild')
REBUILD = NAME == 'rebuild'
SOURCES = 'rebuild/src' if REBUILD else 'src/recovered'        # catalogued sources
HEADERS = 'rebuild/include' if REBUILD else 'src/shared'       # catalogued headers
INCLUDE_ROOT = 'rebuild/include' if REBUILD else 'src'         # the compile's /I directory
HEADER_SUFFIXES = ('.h', '.hpp', '.hh', '.c') if REBUILD else ('.h', '.hpp')     # .c: MIDL interface-id files a unit includes
SOURCE_SUFFIXES = ('.cpp', '.c') if REBUILD else ('.cpp',)
MIN_SOURCE_PARTS = 3                                            # rebuild/src/x.cpp, src/recovered/x.cpp
# Handwritten assembly sources and their ledger (tree-of-record switch, 2026-11-24): the rebuild assembles its own
# copy; src/recovered/asm stays as it was (frozen at src-final; native/CMakeLists.txt's asm check reads it).
ASM = 'rebuild/asm' if REBUILD else 'src/recovered/asm'
ASSEMBLY = 'rebuild/manifest/assembly.json' if REBUILD else 'manifests/original_target/assembly.json'
