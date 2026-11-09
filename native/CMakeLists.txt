# Native arm64 macOS tree built from src/recovered + src/shared (see README.md).
# Separate from the root CMakeLists (the exact original-target build), which
# this project never touches. Configure:
#   cmake -S native -B build/native -G Ninja && cmake --build build/native
cmake_minimum_required(VERSION 3.20)
project(ra2_native VERSION 0.1.0 LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# address_aliases.py and template_instances.py read the compile commands.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(APPLE AND NOT CMAKE_OSX_ARCHITECTURES)
  set(CMAKE_OSX_ARCHITECTURES arm64)
endif()

set(RA2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(RA2_NATIVE_SUBSYSTEM "core" CACHE STRING "Compile groups to build (native_units.py --subsystem)")
option(RA2_NATIVE_SDL "Build the SDL3 host executable" ON)
# The game's start-up path needs the non-exact drafts (WinMain among them), so
# they are on by default in the whole-tree build. They are tracked text
# (native/drafts/<entry>.cpp); the build reads no ignored path.
if(RA2_NATIVE_SUBSYSTEM STREQUAL "all")
  set(ra2_drafts_default ON)
else()
  set(ra2_drafts_default OFF)
endif()
option(RA2_NATIVE_DRAFTS "Append the non-exact launch-path drafts (native/drafts.json, native/drafts/) to their groups" ${ra2_drafts_default})
option(RA2_NATIVE_WIN64_PARAMS "Pointer-sized WPARAM/LPARAM/LRESULT/*_PTR, as on Win64 (native_units.py --win64-params)" ON)

# 1. Generate the native view: port-rewritten shared headers and one TU per
#    catalog compile group (the exact build's unit structure). Re-run by
#    reconfiguring after src/ or the catalog changes.
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(ra2_draft_args "")
if(RA2_NATIVE_WIN64_PARAMS)
  list(APPEND ra2_draft_args --win64-params)
endif()
if(RA2_NATIVE_DRAFTS)
  list(APPEND ra2_draft_args --drafts)
endif()
set(RA2_NATIVE_GEN "${CMAKE_CURRENT_BINARY_DIR}/gen")
execute_process(
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/native_units.py"
          --out "${RA2_NATIVE_GEN}" --subsystem "${RA2_NATIVE_SUBSYSTEM}"
          ${ra2_draft_args}
  RESULT_VARIABLE ra2_gen_result)
if(NOT ra2_gen_result EQUAL 0)
  message(FATAL_ERROR "native_units.py failed")
endif()
include("${RA2_NATIVE_GEN}/units.cmake")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/tools/native_units.py" "${CMAKE_CURRENT_SOURCE_DIR}/port/fixups.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/drafts.json" "${CMAKE_CURRENT_SOURCE_DIR}/link/initterm_rewrites.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/data/member_aliases.tsv")
file(GLOB ra2_tracked_drafts CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/drafts/*.cpp")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${ra2_tracked_drafts})

# 2. The VC6 dialect on clang/arm64 (port/include/ra2port/prelude.hpp explains
#    each choice). -fwrapv and -fno-strict-aliasing restore what VC6 x86 code
#    relies on: wrapping signed arithmetic and type punning through pointers.
add_library(ra2_dialect INTERFACE)
target_compile_options(ra2_dialect INTERFACE
  "$<$<COMPILE_LANGUAGE:CXX>:-fms-extensions;-fms-compatibility;-fdelayed-template-parsing>"
  "$<$<COMPILE_LANGUAGE:CXX>:SHELL:-Xclang -fno-wchar>" "$<$<COMPILE_LANGUAGE:CXX>:-fshort-wchar>"
  "$<$<COMPILE_LANGUAGE:CXX>:-fwrapv;-fno-strict-aliasing>"
  # VC6 bit-field layout (Scout1103): a bit-field takes a whole unit of its
  # declared type and the next non-bit-field member starts after that unit
  # (IsoTileCellStruct's `unsigned :1` flags at +0x24 put the ramp byte at
  # +0x2A; clang's default packed it into +0x27 and read the TMP's 0xCD fill:
  # an MCV could not deploy). -mms-bitfields changes bit-fields only; vtables
  # and bases keep the arm64 layout (hence -Wno-incompatible-ms-struct).
  "$<$<COMPILE_LANGUAGE:CXX>:-mms-bitfields;-Wno-incompatible-ms-struct>"
  # VC6 string literals are writable data (.data, one copy per occurrence: the
  # exact build has no /GF); recovered code writes through char */wchar_t *
  # parameters callers pass literals to (MixFileClass::Offset upper-cases its
  # name in place; README "Writable string literals"). Clang's -fwritable-strings
  # emits every literal, narrow and wide, as its own __DATA,__data object.
  "$<$<COMPILE_LANGUAGE:CXX>:-fwritable-strings>"
  "$<$<COMPILE_LANGUAGE:CXX>:-Wno-error=address-of-temporary;-Wno-register;-Wno-c++11-narrowing;-w>"
  "$<$<COMPILE_LANGUAGE:CXX>:SHELL:-include ${CMAKE_CURRENT_SOURCE_DIR}/port/include/ra2port/prelude.hpp>")
target_include_directories(ra2_dialect INTERFACE "${RA2_NATIVE_GEN}/overlay")

# 3. Recovered code (accepted sources and, later, parked drafts) and the
#    native data it needs (adopted source text or behaviour-written generators,
#    never bytes from the executable). The units are an OBJECT library: the
#    game links every object in the order units.cmake gives (the original's
#    _initterm order, so static initialisers run as they did); the static
#    archive is only for the small checks.
add_library(ra2_recovered_objs OBJECT ${RA2_NATIVE_UNITS})
target_link_libraries(ra2_recovered_objs PUBLIC ra2_dialect)
add_library(ra2_recovered STATIC $<TARGET_OBJECTS:ra2_recovered_objs> data/random3_tables.cpp)
target_link_libraries(ra2_recovered PUBLIC ra2_dialect)

# Native definitions of the data the recovered code reads but does not define
# (data/README.md): zero-initialised globals by size/alignment from the catalog,
# adopted tables. Initialised data without a permitted source is listed, not defined.
file(GLOB RA2_NATIVE_DATA CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/link/*.cpp")
# Generated data: computed by behaviour-written generators (tools/*_tables.py),
# each checked against the original privately (README "Generated data").
execute_process(
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/fast_math_tables.py"
          --out "${RA2_NATIVE_GEN}/fast_math_tables.s"
  RESULT_VARIABLE ra2_tables_result)
if(NOT ra2_tables_result EQUAL 0)
  message(FATAL_ERROR "fast_math_tables.py failed")
endif()
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/fast_math_tables.py")
list(APPEND RA2_NATIVE_DATA "${RA2_NATIVE_GEN}/fast_math_tables.s")
add_library(ra2_data OBJECT ${RA2_NATIVE_DATA})
target_link_libraries(ra2_data PUBLIC ra2_dialect)

# 4. Platform seams: Win32 / DirectDraw / DirectSound / Winsock / OLE and the
#    VC6 CRT surface the recovered code imports, implemented on POSIX and SDL3
#    under the same C names (or the ra2_ names native_units.py gives calls
#    whose libc namesakes differ). Built without the VC6 dialect.
file(GLOB RA2_PORT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/port/src/*.cpp")
add_library(ra2_port STATIC ${RA2_PORT_SOURCES})
target_include_directories(ra2_port PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/port/include")
target_compile_options(ra2_port PRIVATE -Wall -Wno-unused-parameter -Wno-unused-function)

# 5. Proof: recovered RandomClass / Random3Class run natively.
add_executable(native-core-check tests/core_check.cpp)
target_link_libraries(native-core-check PRIVATE ra2_recovered)
target_link_options(native-core-check PRIVATE -Wl,-dead_strip)
enable_testing()
add_test(NAME native-core-check COMMAND native-core-check)

# 6. The behaviour rewrites of the handwritten-assembly functions
#    (link/behaviour_asm_*.cpp, behaviour_rewrites.json) against references
#    built from each algorithm; the ADPCM tables against the adopted .inc text.
add_executable(native-asm-check tests/asm_check.cpp link/behaviour_asm_vqa.cpp
               link/behaviour_asm_draw.cpp link/behaviour_asm_lib.cpp)
target_link_libraries(native-asm-check PRIVATE ra2_dialect)
target_compile_definitions(native-asm-check PRIVATE RA2_ASM_DIR="${RA2_ROOT}/src/recovered/asm")
add_test(NAME native-asm-check COMMAND native-asm-check)

# 7. Light views of real classes (cell views first) against the real classes'
#    arm64 layout: i686-msvc and arm64 record layouts of every unit that defines
#    or uses one (tools/view_audit.py, Scout1108). Needs the generated units.
add_test(NAME native-view-audit COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/view_audit.py"
         --build "${CMAKE_CURRENT_BINARY_DIR}")

option(RA2_NATIVE_LINK_CLOSURE "Close the ra2-native link with generated trap stubs for still-unrecovered code and placeholders for unsourced data (never shipped; lists them in gen/link_closure.json)" OFF)

if(RA2_NATIVE_SDL)
  find_package(SDL3 3.2 CONFIG)
  if(SDL3_FOUND)
    if(RA2_NATIVE_SUBSYSTEM STREQUAL "all")
      # The game: SDL3 host + every recovered object (initialiser order) +
      # native data + the platform seams.
      set(ra2_game_inputs platform/sdl/main.cpp platform/sdl/host.cpp
          $<TARGET_OBJECTS:ra2_recovered_objs> $<TARGET_OBJECTS:ra2_data>)
      file(GENERATE OUTPUT "${RA2_NATIVE_GEN}/link_inputs.txt"
           CONTENT "$<JOIN:$<TARGET_OBJECTS:ra2_recovered_objs>;$<TARGET_OBJECTS:ra2_data>;$<TARGET_FILE:ra2_port>,\n>\n")
      if(RA2_NATIVE_LINK_CLOSURE)
        # Undefined symbols after the real inputs, as trap functions and zero
        # placeholders (tools/link_closure.py). The JSON report is the work
        # queue: every entry is unrecovered code or unsourced data.
        add_library(ra2_host_objs OBJECT platform/sdl/main.cpp platform/sdl/host.cpp)
        target_link_libraries(ra2_host_objs PRIVATE SDL3::SDL3 ra2_port)
        target_compile_definitions(ra2_host_objs PRIVATE RA2_NATIVE_GAME=1)
        add_custom_command(
          OUTPUT "${RA2_NATIVE_GEN}/link_closure.c" "${RA2_NATIVE_GEN}/link_closure.json" "${RA2_NATIVE_GEN}/alias_list.txt"
          COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/link_closure.py"
                  --inputs "${RA2_NATIVE_GEN}/link_inputs.txt" --host $<TARGET_OBJECTS:ra2_host_objs>
                  --out "${RA2_NATIVE_GEN}/link_closure.c"
                  --aliases "${CMAKE_CURRENT_SOURCE_DIR}/data/address_aliases.tsv" "${CMAKE_CURRENT_SOURCE_DIR}/data/bss_aliases.tsv"
                  --alias-list "${RA2_NATIVE_GEN}/alias_list.txt"
          DEPENDS ra2_recovered_objs ra2_data ra2_port ra2_host_objs "${CMAKE_CURRENT_SOURCE_DIR}/tools/link_closure.py"
                  "${CMAKE_CURRENT_SOURCE_DIR}/data/address_aliases.tsv"
                  $<TARGET_OBJECTS:ra2_data> $<TARGET_FILE:ra2_port> $<TARGET_OBJECTS:ra2_host_objs>
                  "${RA2_NATIVE_GEN}/units.cmake"
          COMMAND_EXPAND_LISTS VERBATIM)
        set(ra2_game_inputs $<TARGET_OBJECTS:ra2_host_objs> $<TARGET_OBJECTS:ra2_recovered_objs>
            $<TARGET_OBJECTS:ra2_data> "${RA2_NATIVE_GEN}/link_closure.c")
      endif()
      add_executable(ra2-native ${ra2_game_inputs})
      target_link_libraries(ra2-native PRIVATE SDL3::SDL3 ra2_port)
      if(RA2_NATIVE_LINK_CLOSURE)
        # One original address, several names: ld64 aliases (data/address_aliases.tsv).
        target_link_options(ra2-native PRIVATE "LINKER:-alias_list,${RA2_NATIVE_GEN}/alias_list.txt")
        set_property(TARGET ra2-native APPEND PROPERTY LINK_DEPENDS "${RA2_NATIVE_GEN}/alias_list.txt")
      endif()
      target_compile_definitions(ra2-native PRIVATE RA2_NATIVE_GAME=1)
      # Repeated definitions across units get VC6's select-any semantics.
      add_custom_command(TARGET ra2-native PRE_LINK
        COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/weaken_duplicates.py"
                --inputs "${RA2_NATIVE_GEN}/link_inputs.txt" --report "${RA2_NATIVE_GEN}/duplicates.json"
        VERBATIM)
    else()
      add_executable(ra2-native platform/sdl/main.cpp)
      target_link_libraries(ra2-native PRIVATE SDL3::SDL3 ra2_port)
    endif()
  else()
    message(STATUS "SDL3 not found: ra2-native host skipped")
  endif()
endif()
