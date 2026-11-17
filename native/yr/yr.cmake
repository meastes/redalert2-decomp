# The native Yuri's Revenge game, yr-native (lane Y1; user decision 2026-11-15, AGENTS.md;
# docs/YR_PORT_SIZING.md plan 1, stage 1). Built from the pinned ra2-decomp tree (yr/) through
# native/yr/tools (generated units with native/yr/fixups.json, the Win32/DirectX headers from the
# local llvm-mingw toolchain rewritten for the VC6 widths), with native/'s platform layer: the
# SDL3 host and the ra2_port seams. Included by native/CMakeLists.txt with -DRA2_NATIVE_YR=ON on
# the Mac build and the iOS build (lane Y7; the RA2 targets are unchanged). Unrecovered code and
# unsourced data link as traps and zero placeholders (tools/link_closure.py): gen-yr/link_closure.json is the work queue.
set(RA2_NATIVE_YR_GEN "${CMAKE_CURRENT_BINARY_DIR}/gen-yr")
file(GLOB ra2_yr_mingw_default "${CMAKE_CURRENT_SOURCE_DIR}/../private/toolchains/llvm-mingw-*/llvm-mingw-*-macos-*/generic-w64-mingw32/include")
set(ra2_yr_mingw_first "")
if(ra2_yr_mingw_default)
  list(GET ra2_yr_mingw_default 0 ra2_yr_mingw_first)
endif()
set(RA2_NATIVE_YR_MINGW "${ra2_yr_mingw_first}" CACHE PATH "mingw-w64 headers (llvm-mingw <root>/generic-w64-mingw32/include) for the YR build's Win32/DirectX layer")
if(NOT IS_DIRECTORY "${RA2_NATIVE_YR_MINGW}")
  message(FATAL_ERROR "RA2_NATIVE_YR: set RA2_NATIVE_YR_MINGW to llvm-mingw's generic-w64-mingw32/include")
endif()
execute_process(
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/yr_sdk.py"
          --mingw "${RA2_NATIVE_YR_MINGW}" --out "${RA2_NATIVE_YR_GEN}/mingw"
  RESULT_VARIABLE ra2_yr_rc)
if(ra2_yr_rc)
  message(FATAL_ERROR "yr_sdk.py failed")
endif()
execute_process(
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/yr_units.py" --out "${RA2_NATIVE_YR_GEN}"
  RESULT_VARIABLE ra2_yr_rc)
if(ra2_yr_rc)
  message(FATAL_ERROR "yr_units.py failed")
endif()
file(GLOB ra2_yr_appends "${CMAKE_CURRENT_SOURCE_DIR}/yr/glue/*.inc")   # fixups.json "append" files
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${ra2_yr_appends}
  "${CMAKE_CURRENT_SOURCE_DIR}/yr/fixups.json" "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/yr_units.py"
  "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/yr_sdk.py")
include("${RA2_NATIVE_YR_GEN}/units.cmake")
file(GLOB ra2_yr_glue CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/yr/glue/*.cpp")

# The YR units: the RA2 tree's VC6 dialect options (ra2_dialect), the YR prelude, the generated
# header directory and the rewritten Win32 headers after the host's own.
get_target_property(ra2_yr_dialect_options ra2_dialect INTERFACE_COMPILE_OPTIONS)
add_library(yr_units_objs OBJECT ${YR_NATIVE_UNITS} ${ra2_yr_glue})
target_compile_options(yr_units_objs PRIVATE ${ra2_yr_dialect_options}
  "SHELL:-include ${CMAKE_CURRENT_SOURCE_DIR}/yr/sdk/yr_prelude.h"
  "SHELL:-idirafter ${RA2_NATIVE_YR_GEN}/mingw"
  # Header constants defined in every unit (geometry.h's CELL_NONE, COORD_NONE, ...; VC6 emitted a
  # copy per unit): zero-filled storage cannot carry the weak definitions weaken_duplicates.py
  # gives the repeats, so it goes in __DATA, where ld64 keeps the first.
  -fno-zero-initialized-in-bss)
target_compile_definitions(yr_units_objs PRIVATE WIN32 _WIN32 __GNUC__=4 __GNUC_MINOR__=2 RA2_NATIVE_YR=1)
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  # The iOS build has no build type (build-ios.command): the tree's asserts stay out, as in
  # gamemd.exe (a release build) and the Mac Release build the skirmish tests run (lane Y7; with
  # them, Assert_Failure was one more trap).
  target_compile_definitions(yr_units_objs PRIVATE NDEBUG)
endif()
target_include_directories(yr_units_objs BEFORE PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/yr/sdk"
  "${RA2_NATIVE_YR_GEN}/hdr" "${RA2_NATIVE_YR_GEN}/hdr/vqalib")

# The host (native/platform/sdl), compiled again for this game.
add_library(yr_host_objs OBJECT ${ra2_host_sources})
target_link_libraries(yr_host_objs PRIVATE SDL3::SDL3 ra2_port)
target_compile_definitions(yr_host_objs PRIVATE RA2_NATIVE_GAME=1 RA2_NATIVE_YR=1)

file(GENERATE OUTPUT "${RA2_NATIVE_YR_GEN}/link_inputs.txt"
     CONTENT "$<JOIN:$<TARGET_OBJECTS:yr_units_objs>;$<TARGET_FILE:ra2_port>,\n>\n")
# The tree's address names bound to the definitions at those addresses (yr_aliases.py; ld64 aliases).
add_custom_command(
  OUTPUT "${RA2_NATIVE_YR_GEN}/aliases.tsv"
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/yr_aliases.py"
          --inputs "${RA2_NATIVE_YR_GEN}/link_inputs.txt" --out "${RA2_NATIVE_YR_GEN}/aliases.tsv"
  DEPENDS yr_units_objs "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/yr_aliases.py" "${CMAKE_CURRENT_SOURCE_DIR}/yr/data/globals.tsv" "${CMAKE_CURRENT_SOURCE_DIR}/yr/data/name_aliases.tsv"
          $<TARGET_OBJECTS:yr_units_objs>
  COMMAND_EXPAND_LISTS VERBATIM)
# The YR globals no source defines: storage and the port's load table (yr_data.py, from the
# undefined data of the units and native/yr/data/globals.tsv).
add_custom_command(
  OUTPUT "${RA2_NATIVE_YR_GEN}/yr_original_data.cpp" "${RA2_NATIVE_YR_GEN}/yr_original_data.json"
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/yr_data.py"
          --inputs "${RA2_NATIVE_YR_GEN}/link_inputs.txt" --host $<TARGET_OBJECTS:yr_host_objs>
          --gen "${RA2_NATIVE_YR_GEN}" --out "${RA2_NATIVE_YR_GEN}/yr_original_data.cpp"
          --aliases "${RA2_NATIVE_YR_GEN}/aliases.tsv"
          "--sdk=${CMAKE_OSX_SYSROOT}" --system-libs ${RA2_NATIVE_CLOSURE_SYSTEM_LIBS} --frameworks ${ra2_closure_frameworks}
  DEPENDS yr_units_objs ra2_port yr_host_objs "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/yr_data.py" "${RA2_NATIVE_YR_GEN}/aliases.tsv"
          "${CMAKE_CURRENT_SOURCE_DIR}/yr/data/globals.tsv" "${CMAKE_CURRENT_SOURCE_DIR}/tools/link_closure.py"
          $<TARGET_FILE:ra2_port> $<TARGET_OBJECTS:yr_host_objs> $<TARGET_OBJECTS:yr_units_objs>
  COMMAND_EXPAND_LISTS VERBATIM)
# Field maps for saves in gamemd.exe's format (lane Y10; README "Saves in gamemd.exe's format"): the i686
# and arm64 record layouts of the YR units for the types in yr/data/original_save_types.txt
# (tools/original_layouts.py --game yr); the vtables come from the user's gamemd.exe at run time.
file(GLOB ra2_yr_i686_shims "${CMAKE_CURRENT_SOURCE_DIR}/yr/sdk/i686/*")
add_custom_command(
  OUTPUT "${RA2_NATIVE_YR_GEN}/original_layouts.cpp"
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/original_layouts.py" --game yr
          --build "${CMAKE_CURRENT_BINARY_DIR}" --out "${RA2_NATIVE_YR_GEN}/original_layouts.cpp"
          --report "${RA2_NATIVE_YR_GEN}/original_layouts.json"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/original_layouts.py" "${CMAKE_CURRENT_SOURCE_DIR}/yr/data/original_save_types.txt"
          "${CMAKE_CURRENT_SOURCE_DIR}/port/include/ra2port/prelude.hpp" "${CMAKE_CURRENT_SOURCE_DIR}/yr/sdk/yr_prelude.h"
          ${ra2_yr_i686_shims} "${RA2_NATIVE_YR_GEN}/units.cmake" ${YR_NATIVE_UNITS}
  VERBATIM)
add_library(yr_data_obj OBJECT "${RA2_NATIVE_YR_GEN}/yr_original_data.cpp" "${RA2_NATIVE_YR_GEN}/original_layouts.cpp")
file(GENERATE OUTPUT "${RA2_NATIVE_YR_GEN}/closure_inputs.txt"
     CONTENT "$<JOIN:$<TARGET_OBJECTS:yr_units_objs>;$<TARGET_OBJECTS:yr_data_obj>;$<TARGET_FILE:ra2_port>,\n>\n")
add_custom_command(
  OUTPUT "${RA2_NATIVE_YR_GEN}/link_closure.c" "${RA2_NATIVE_YR_GEN}/link_closure.json" "${RA2_NATIVE_YR_GEN}/alias_list.txt"
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/link_closure.py"
          --inputs "${RA2_NATIVE_YR_GEN}/closure_inputs.txt" --host $<TARGET_OBJECTS:yr_host_objs>
          --out "${RA2_NATIVE_YR_GEN}/link_closure.c" --system-libs ${RA2_NATIVE_CLOSURE_SYSTEM_LIBS}
          "--sdk=${CMAKE_OSX_SYSROOT}" --frameworks ${ra2_closure_frameworks} --data-from 0x7E1000
          --aliases "${RA2_NATIVE_YR_GEN}/aliases.tsv" --alias-list "${RA2_NATIVE_YR_GEN}/alias_list.txt"
  DEPENDS yr_units_objs yr_data_obj ra2_port yr_host_objs "${CMAKE_CURRENT_SOURCE_DIR}/tools/link_closure.py"
          $<TARGET_FILE:ra2_port> $<TARGET_OBJECTS:yr_host_objs> $<TARGET_OBJECTS:yr_units_objs> $<TARGET_OBJECTS:yr_data_obj>
  COMMAND_EXPAND_LISTS VERBATIM)
add_executable(yr-native $<TARGET_OBJECTS:yr_host_objs> $<TARGET_OBJECTS:yr_units_objs> $<TARGET_OBJECTS:yr_data_obj>
               "${RA2_NATIVE_YR_GEN}/link_closure.c")
target_link_libraries(yr-native PRIVATE SDL3::SDL3 ra2_port ${ra2_host_frameworks})
target_link_options(yr-native PRIVATE "LINKER:-alias_list,${RA2_NATIVE_YR_GEN}/alias_list.txt")
set_property(TARGET yr-native APPEND PROPERTY LINK_DEPENDS "${RA2_NATIVE_YR_GEN}/alias_list.txt")
target_compile_definitions(yr-native PRIVATE RA2_NATIVE_GAME=1 RA2_NATIVE_YR=1)
# Repeated definitions across units (explicit template instantiations in several units, as VC6's
# COMDATs) get select-any semantics, as in the RA2 game (tools/weaken_duplicates.py).
# The units' static initialisers run after the original data is in place (defer_initializers.py).
add_custom_command(TARGET yr-native PRE_LINK
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/weaken_duplicates.py"
          --inputs "${RA2_NATIVE_YR_GEN}/link_inputs.txt" --report "${RA2_NATIVE_YR_GEN}/duplicates.json"
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/defer_initializers.py"
          --inputs "${RA2_NATIVE_YR_GEN}/link_inputs.txt"
  VERBATIM)

# The iOS app (lane Y7; README "yr-native on iOS"): YR.app, its own bundle id and home-screen name,
# the same bundle shape as RA2.app (native/CMakeLists.txt's ra2_ios_app). The ctests below run the
# Mac build; on iOS the Simulator runs (yr/tests/ios_sim_skirmish.py) are the check.
if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
  ra2_ios_app(yr-native YR "io.github.ra2native.YR" "Yuri's Revenge" --yr)
  include("${CMAKE_CURRENT_SOURCE_DIR}/yr/combined.cmake")   # lane Y8: ra2yr-one (--one)
  return()
endif()

# ctest yr-native-skirmish-smoke (lane Y3): YR's menus to a skirmish, about 20 s of play, the
# in-game options' Abort Mission and back to the skirmish setup, through scripted input
# (yr/tests/yr_skirmish_smoke.py). Only in a -DRA2_NATIVE_YR=ON build; skipped (77) without the
# installed game (RA2_NATIVE_YR_DATA_DIR: the RA2 folder with Yuri's Revenge, read only).
set(RA2_NATIVE_YR_DATA_DIR "$ENV{RA2_NATIVE_YR_DATA_DIR}" CACHE PATH "Installed RA2 + Yuri's Revenge folder for the yr-native skirmish smoke test (read only)")
add_test(NAME yr-native-skirmish-smoke COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_skirmish_smoke.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-skirmish-smoke")
set_tests_properties(yr-native-skirmish-smoke PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 300)

# ctest yr-native-skirmish-score (lane Y4): a lost skirmish (the MCV deployed, the construction yard
# sold in a short game), the Skirmish Score screen, Continue and Back to the main menu
# (yr/tests/yr_skirmish_score.py); same conditions as the smoke test.
add_test(NAME yr-native-skirmish-score COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_skirmish_score.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-skirmish-score")
set_tests_properties(yr-native-skirmish-score PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 360)

# ctest yr-native-skirmish-long (lane Y6): ten minutes of a skirmish against three hard computer
# opponents (the write folder's RA2MD.INI sets the setup) without a trap or a fault
# (yr/tests/yr_skirmish_long.py); same conditions as the smoke test.
add_test(NAME yr-native-skirmish-long COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_skirmish_long.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-skirmish-long" --seconds 600 --opponents 3 --difficulty hard)
set_tests_properties(yr-native-skirmish-long PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 900)

# ctest yr-native-original-layouts (lane Y10): every type in yr/data/original_save_types.txt has an i686
# layout of gamemd.exe's size whose members all pair with the arm64 layout, and every vtable owner's name
# could be mangled (tools/original_layouts.py --game yr --check).
add_test(NAME yr-native-original-layouts COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/original_layouts.py"
         --game yr --build "${CMAKE_CURRENT_BINARY_DIR}" --out "${CMAKE_CURRENT_BINARY_DIR}/yr_original_layouts_check.cpp" --check)

# ctest yr-native-save (lane Y10): a skirmish saved in gamemd.exe's format at frame 500, loaded from the
# menus and played 300 frames, saved again after the load and that loaded and played; a save of the
# user's installed Yuri's Revenge loaded too when the data folder has one (yr/tests/yr_save_roundtrip.py).
add_test(NAME yr-native-save COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_save_roundtrip.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-save")
set_tests_properties(yr-native-save PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 700)

# ctest yr-native-controls (lane Y8): Controls Classic / Modern and Queue five with Shift in a
# skirmish (glue/yr_enhancements.cpp), one run with each setting
add_test(NAME yr-native-controls COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_controls.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-controls")
set_tests_properties(yr-native-controls PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 400)

# One game (lane Y5, -DRA2_NATIVE_COMBINED=ON): native/yr/combined.cmake.
include("${CMAKE_CURRENT_SOURCE_DIR}/yr/combined.cmake")
