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
# The x86_64 Windows build (lane Y13): the toolchain's own headers are the system headers, so the
# rewritten Win32 headers are searched ahead of them (-isystem) and leave out every header the Mac's
# SDK has (the C library's: the toolchain's own are used, as the Mac uses its SDK's).
set(ra2_yr_sdk_args "")
if(WIN32)
  execute_process(COMMAND xcrun --sdk macosx --show-sdk-path OUTPUT_VARIABLE ra2_yr_mac_sdk OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(NOT IS_DIRECTORY "${ra2_yr_mac_sdk}/usr/include")
    message(FATAL_ERROR "RA2_NATIVE_YR (Windows): the macOS SDK (xcrun --sdk macosx) names the C library headers to leave out")
  endif()
  set(ra2_yr_sdk_args --host-headers "${ra2_yr_mac_sdk}/usr/include")
endif()
execute_process(
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/yr_sdk.py"
          --mingw "${RA2_NATIVE_YR_MINGW}" --out "${RA2_NATIVE_YR_GEN}/mingw" ${ra2_yr_sdk_args}
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
  "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/yr_sdk.py" "${CMAKE_CURRENT_SOURCE_DIR}/yr/data/member_aliases.tsv")
include("${RA2_NATIVE_YR_GEN}/units.cmake")
file(GLOB ra2_yr_glue CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/yr/glue/*.cpp")
set(ra2_yr_headers "SHELL:-idirafter ${RA2_NATIVE_YR_GEN}/mingw")
if(WIN32)
  # COFF spellings of the Mach-O assembler labels in the units and the glue (cmake/windows.cmake).
  execute_process(
    COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/yr_coff.py"
            --gen "${RA2_NATIVE_YR_GEN}" --out "${RA2_NATIVE_YR_GEN}/coff_units.cmake"
    RESULT_VARIABLE ra2_yr_rc)
  if(ra2_yr_rc)
    message(FATAL_ERROR "yr_coff.py failed")
  endif()
  include("${RA2_NATIVE_YR_GEN}/coff_units.cmake")
  ra2_windows_coff_sources(ra2_yr_glue ${ra2_yr_glue})
  set(ra2_yr_glue_dir "${CMAKE_CURRENT_SOURCE_DIR}/yr/glue")   # the copies' "" includes
  set(ra2_yr_headers "SHELL:-isystem ${RA2_NATIVE_YR_GEN}/mingw")
endif()

# The YR units: the RA2 tree's VC6 dialect options (ra2_dialect), the YR prelude, the generated
# header directory and the rewritten Win32 headers after the host's own.
get_target_property(ra2_yr_dialect_options ra2_dialect INTERFACE_COMPILE_OPTIONS)
add_library(yr_units_objs OBJECT ${YR_NATIVE_UNITS} ${ra2_yr_glue})
target_compile_options(yr_units_objs PRIVATE ${ra2_yr_dialect_options}
  "SHELL:-include ${CMAKE_CURRENT_SOURCE_DIR}/yr/sdk/yr_prelude.h"
  ${ra2_yr_headers}
  # Header constants defined in every unit (geometry.h's CELL_NONE, COORD_NONE, ...; VC6 emitted a
  # copy per unit): zero-filled storage cannot carry the weak definitions weaken_duplicates.py
  # gives the repeats, so it goes in __DATA, where ld64 keeps the first.
  -fno-zero-initialized-in-bss)
target_compile_definitions(yr_units_objs PRIVATE WIN32 _WIN32 __GNUC__=4 __GNUC_MINOR__=2 RA2_NATIVE_YR=1)
if(WIN32)
  target_compile_options(yr_units_objs BEFORE PRIVATE "SHELL:-include ${CMAKE_CURRENT_SOURCE_DIR}/yr/sdk/yr_win_first.h")
endif()
if(CMAKE_SYSTEM_NAME STREQUAL "iOS" OR WIN32)
  # The iOS build (and the Windows build, lane Y13) has no build type (build-ios.command): the tree's asserts stay out, as in
  # gamemd.exe (a release build) and the Mac Release build the skirmish tests run (lane Y7; with
  # them, Assert_Failure was one more trap).
  target_compile_definitions(yr_units_objs PRIVATE NDEBUG)
endif()
target_include_directories(yr_units_objs BEFORE PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/yr/sdk"
  "${RA2_NATIVE_YR_GEN}/hdr" "${RA2_NATIVE_YR_GEN}/hdr/vqalib" ${ra2_yr_glue_dir})

# The host (native/platform/sdl), compiled again for this game.
add_library(yr_host_objs OBJECT ${ra2_host_sources})
target_link_libraries(yr_host_objs PRIVATE SDL3::SDL3 ra2_port)
target_compile_definitions(yr_host_objs PRIVATE RA2_NATIVE_GAME=1 RA2_NATIVE_YR=1)

# How the link tools read the objects (lane Y13: COFF on Windows, read in the Mach-O spelling).
if(WIN32)
  set(ra2_yr_obj_args --target windows "--nm=${CMAKE_NM}")
  set(ra2_yr_sys_args --target windows "--nm=${CMAKE_NM}" "--sysroot=${RA2_LLVM_MINGW}/x86_64-w64-mingw32")
else()
  set(ra2_yr_obj_args "")
  set(ra2_yr_sys_args "--sdk=${CMAKE_OSX_SYSROOT}" --frameworks ${ra2_closure_frameworks})
endif()
file(GENERATE OUTPUT "${RA2_NATIVE_YR_GEN}/link_inputs.txt"
     CONTENT "$<JOIN:$<TARGET_OBJECTS:yr_units_objs>;$<TARGET_FILE:ra2_port>,\n>\n")
# The tree's address names bound to the definitions at those addresses (yr_aliases.py; ld64 aliases).
add_custom_command(
  OUTPUT "${RA2_NATIVE_YR_GEN}/aliases.tsv"
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/yr_aliases.py"
          --inputs "${RA2_NATIVE_YR_GEN}/link_inputs.txt" --out "${RA2_NATIVE_YR_GEN}/aliases.tsv" ${ra2_yr_obj_args}
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
          ${ra2_yr_sys_args} --system-libs ${RA2_NATIVE_CLOSURE_SYSTEM_LIBS}
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
          --build "${CMAKE_CURRENT_BINARY_DIR}" --out "${RA2_NATIVE_YR_GEN}/original_layouts.cpp" ${ra2_original_layouts_target}
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
          ${ra2_yr_sys_args} --data-from 0x7E1000
          --aliases "${RA2_NATIVE_YR_GEN}/aliases.tsv" --alias-list "${RA2_NATIVE_YR_GEN}/alias_list.txt"
  DEPENDS yr_units_objs yr_data_obj ra2_port yr_host_objs "${CMAKE_CURRENT_SOURCE_DIR}/tools/link_closure.py"
          $<TARGET_FILE:ra2_port> $<TARGET_OBJECTS:yr_host_objs> $<TARGET_OBJECTS:yr_units_objs> $<TARGET_OBJECTS:yr_data_obj>
  COMMAND_EXPAND_LISTS VERBATIM)
add_executable(yr-native $<TARGET_OBJECTS:yr_host_objs> $<TARGET_OBJECTS:yr_units_objs> $<TARGET_OBJECTS:yr_data_obj>
               "${RA2_NATIVE_YR_GEN}/link_closure.c")
target_link_libraries(yr-native PRIVATE SDL3::SDL3 ra2_port ${ra2_host_frameworks})
target_compile_definitions(yr-native PRIVATE RA2_NATIVE_GAME=1 RA2_NATIVE_YR=1)
if(WIN32)
  # The x86_64 Windows build (lane Y13; cmake/windows.cmake as the RA2 game's): repeated definitions
  # select-any (--allow-multiple-definition), the 8 MiB stack, the initialisers in link order, the
  # icon; the address aliases are -alternatename directives in link_closure.c. The units' initialisers
  # move to .yrini$m (defer_initializers.py), run by yr_host_glue.cpp once the data is loaded; the
  # glue's too (their header constants call fastmath::atan on gamemd.exe's tables: before the load
  # the tables are zero, and x86-64's conversion of the NaN gives an index far outside the table).
  ra2_windows_game_link(yr-native)
  set_target_properties(yr-native PROPERTIES OUTPUT_NAME yr-native)
  add_custom_command(TARGET yr-native PRE_LINK
    COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/defer_initializers.py"
            --inputs "${RA2_NATIVE_YR_GEN}/link_inputs.txt" --only /yr_units_objs.dir/
    VERBATIM)
  # YRNative.exe: the players' GUI-subsystem build of the same objects (as RA2Native.exe).
  if(RA2_WINDOWS_GUI)
    add_executable(YRNative WIN32 $<TARGET_OBJECTS:yr_host_objs> $<TARGET_OBJECTS:yr_units_objs> $<TARGET_OBJECTS:yr_data_obj>
                   "${RA2_NATIVE_YR_GEN}/link_closure.c")
    target_link_libraries(YRNative PRIVATE SDL3::SDL3 ra2_port)
    target_compile_definitions(YRNative PRIVATE RA2_NATIVE_GAME=1 RA2_NATIVE_YR=1)
    ra2_windows_game_link(YRNative)
    add_dependencies(YRNative yr-native)   # after yr-native's PRE_LINK has moved the initialisers
  endif()
  # ctest yr-native-skirmish-smoke on the Windows build: the same route, yr-native.exe under Wine in a
  # fresh prefix (cmake/windows.cmake RA2_NATIVE_WINE; skipped without Wine or the installed game).
  set(RA2_NATIVE_YR_DATA_DIR "$ENV{RA2_NATIVE_YR_DATA_DIR}" CACHE PATH "Installed RA2 + Yuri's Revenge folder for the yr-native tests (read only)")
  add_test(NAME yr-native-skirmish-smoke COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_skirmish_smoke.py"
           --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
           --work "${CMAKE_CURRENT_BINARY_DIR}/yr-skirmish-smoke"
           --wine-prefix "${CMAKE_CURRENT_BINARY_DIR}/wine/yr-smoke" --wine "${RA2_NATIVE_WINE}")
  set_tests_properties(yr-native-skirmish-smoke PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 420)
  # yr-native-skirmish-score and yr-native-combat on the Windows build (lane YW): the Mac routes with
  # yr-native.exe under Wine, each in a fresh prefix deleted after the run (yr/tests/yr_wine.py).
  add_test(NAME yr-native-skirmish-score COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_skirmish_score.py"
           --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
           --work "${CMAKE_CURRENT_BINARY_DIR}/yr-skirmish-score"
           --wine-prefix "${CMAKE_CURRENT_BINARY_DIR}/wine/yr-score" --wine "${RA2_NATIVE_WINE}")
  set_tests_properties(yr-native-skirmish-score PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 600)
  add_test(NAME yr-native-combat COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_combat.py"
           --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
           --work "${CMAKE_CURRENT_BINARY_DIR}/yr-combat"
           --wine-prefix "${CMAKE_CURRENT_BINARY_DIR}/wine/yr-combat" --wine "${RA2_NATIVE_WINE}")
  set_tests_properties(yr-native-combat PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 600)
  include("${CMAKE_CURRENT_SOURCE_DIR}/yr/combined.cmake")   # lane Y16: RA2YRNative.exe and the engine DLLs
  return()   # the other ctests below run the Mac build
endif()
target_link_options(yr-native PRIVATE "LINKER:-alias_list,${RA2_NATIVE_YR_GEN}/alias_list.txt")
set_property(TARGET yr-native APPEND PROPERTY LINK_DEPENDS "${RA2_NATIVE_YR_GEN}/alias_list.txt")
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
# ctest yr-native-member-aliases (lane Y15): yr/data/member_aliases.tsv (gamemd.exe globals inside a defined
# object, bound to its members) is what this build's layouts and undefined globals give.
add_test(NAME yr-native-member-aliases COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/yr_member_aliases.py"
         --build "${CMAKE_CURRENT_BINARY_DIR}" --check)

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

# ctest yr-native-map-zoom (lanes N18, N20): Map zoom and the zoom gestures in a skirmish (glue/yr_map_zoom.cpp,
# Red Alert 2's link/native_map_zoom.cpp carried over), down to 0.5x (the render scale) and the in-game menu at
# the window's layout, against a run with the defaults
add_test(NAME yr-native-map-zoom COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_map_zoom.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-map-zoom")
set_tests_properties(yr-native-map-zoom PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 400)

# ctest yr-native-zoom-abort (lane N21): Abort Mission with Game interface size 1.5x and Zoom gestures on (render
# scale 2): no picture after the in-game menu shows the game's screen whole (the zoomed map in its top left corner,
# the user's Duo report); Main_Loop's end clears it (glue/yr_app_hooks.cpp, glue/yr_map_zoom.cpp Yr_Map_Zoom_End)
add_test(NAME yr-native-zoom-abort COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_zoom_abort.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-zoom-abort")
set_tests_properties(yr-native-zoom-abort PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 300)

# ctest yr-native-end-banner (lane N21): the end-of-game banner ("Game Over", GRFXTXT.SHP) at the original's
# brightness after a defeat flagged in a skirmish (Init_Game's draft scales GRFXTXT.PAL to 8 bits as gamemd.exe does)
add_test(NAME yr-native-end-banner COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_end_banner.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-end-banner")
set_tests_properties(yr-native-end-banner PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 300)

# ctest yr-native-interface-size (lane N19): Game interface size set on the Enhancements page applies to the
# next game (glue/yr_interface_size.cpp, Red Alert 2's link/native_enhancements.cpp carried over): the sidebar
# surface shorter, play laid out split, the in-game menu shown whole; against a run with the defaults
add_test(NAME yr-native-interface-size COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_interface_size.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-interface-size")
set_tests_properties(yr-native-interface-size PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 500)

# ctest yr-native-hd-vehicles (lane N19): HD vehicles 2x (with Map zoom 1.5x) and 3x in a skirmish
# (glue/yr_hd_vehicles.cpp, Red Alert 2's link/native_hd_vehicles.cpp carried over), checked by the Nx layer's counts
add_test(NAME yr-native-hd-vehicles COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_hd_vehicles.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-hd-vehicles")
set_tests_properties(yr-native-hd-vehicles PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 400)

# ctest yr-native-combat (lane Y12): a vehicle force-fires on another of the player's (glue/yr_harness.cpp
# RA2_NATIVE_ORDERS): TechnoClass::Fire_At runs, damage is dealt and the target dies (yr/tests/yr_combat.py)
add_test(NAME yr-native-combat COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_combat.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-combat")
set_tests_properties(yr-native-combat PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 360)

# ctest yr-native-standins (lane Y16): the newer stand-ins at work in one skirmish: Magnetron, Prism and Mirage
# tanks fire on spawned enemies, sonic and magnetron waves (WaveClass::FUN_007602E0), a spy goes into an
# enemy power plant, the Psychic Dominator and the Lightning Storm are placed, a thief hijacks a truck (lane P1)
# (yr/tests/yr_standins.py)
add_test(NAME yr-native-standins COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_standins.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-standins" --seconds 120)
set_tests_properties(yr-native-standins PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 400)

# ctest yr-native-graphics (lane Y14): no noise drawn while the MCV deploys and a Grizzly first moves
# (a vehicle's uncached voxel picture through the eight-bit drawer; yr/tests/yr_graphics.py)
add_test(NAME yr-native-graphics COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_graphics.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-graphics")
set_tests_properties(yr-native-graphics PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 240)

# ctest yr-native-touch-team (lane Y15): a hold on the command bar's Team 1 sets the team to the selection
# and a tap selects it (platform/sdl/touch.cpp; yr/tests/yr_touch_team.py)
add_test(NAME yr-native-touch-team COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_touch_team.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-touch-team")
set_tests_properties(yr-native-touch-team PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 240)

# ctest yr-native-type-select (lane N3): the command bar's TypeSelect button and the TypeSelect key select
# every unit of the selected type (glue/uicmnds_native.inc's stand-in; yr/tests/yr_type_select.py)
add_test(NAME yr-native-type-select COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_type_select.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-type-select")
set_tests_properties(yr-native-type-select PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 240)

# ctest yr-native-command-bar-thumb (lane N27): the command bar's close thumb closes it and its open thumb, inside
# the part of the bar the host shows (Zoom gestures on: glue/yr_map_zoom.cpp Yr_Command_Bar_Right), opens it again
add_test(NAME yr-native-command-bar-thumb COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_command_bar_thumb.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-command-bar-thumb")
set_tests_properties(yr-native-command-bar-thumb PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 300)

# ctest yr-native-locked-input (lane N32): touch pan and the zoom gestures wait for a mission intro's input lock
# (IgnoreInput 0x00A8ED9C: glue/yr_app_hooks.cpp ra2_native_scroll_map, glue/yr_map_zoom.cpp ra2_native_zoom_by;
# Allied mission 1's intro, yr/tests/yr_locked_input.py)
add_test(NAME yr-native-locked-input COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_locked_input.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-locked-input")
set_tests_properties(yr-native-locked-input PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 420)

# ctest yr-native-standins-triggers (lane N3): stand-ins the campaign triggers and keys reach (ForceCreate, the
# lightning bolt and storm stop, the Gap Generator's range ring, HealthNav/VeterancyNav; yr/tests/yr_standins_triggers.py)
add_test(NAME yr-native-standins-triggers COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_standins_triggers.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-standins-triggers")
set_tests_properties(yr-native-standins-triggers PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 360)

# ctest yr-native-planning (lane N3): the command bar's PlanningMode button: two planned waypoints, the commit, the
# tanks follow the plan (glue/planmgr_planning_native.inc; yr/tests/yr_planning.py)
add_test(NAME yr-native-planning COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_planning.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-planning")
set_tests_properties(yr-native-planning PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 300)

# ctest yr-native-war-factory (lane N3): a vehicle leaves a War Factory (Exit_Object, Do_MISSION_UNLOAD) and a soldier
# leaves a Barracks (yr/tests/yr_war_factory.py)
add_test(NAME yr-native-war-factory COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_war_factory.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-war-factory")
set_tests_properties(yr-native-war-factory PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 360)

# ctest yr-native-garrison (lane N4): a GI garrisons a civilian house and unloads from it (InfantryClass::Per_Cell_Process's
# occupier branch; yr/tests/yr_garrison.py)
add_test(NAME yr-native-garrison COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_garrison.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-garrison")
set_tests_properties(yr-native-garrison PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 240)

# ctest yr-native-mirage (lane N4): a Mirage tank's camouflage draws as a tree in the tree's colours (Techno_Draw_Object's
# disguise converter; yr/tests/yr_mirage.py)
add_test(NAME yr-native-mirage COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_mirage.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-mirage")
set_tests_properties(yr-native-mirage PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 240)

# ctest yr-native-deployers (lane N13): the Guardian GI, the GI and the Desolator deploy and undeploy on command, the
# Siege Chopper deploys, and the first Allied mission's Guardian GIs deploy (MasterDoControls' 42 actions;
# yr/tests/yr_deployers.py)
add_test(NAME yr-native-deployers COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_deployers.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-deployers")
set_tests_properties(yr-native-deployers PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 400)

# ctest yr-native-building-anims (lane N4): an oil derrick's pump and flame and a power plant's animation play in the
# ground display layer (the layer names' data, yr_data.py's namespace members; yr/tests/yr_building_anims.py)
add_test(NAME yr-native-building-anims COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_building_anims.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-building-anims")
set_tests_properties(yr-native-building-anims PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 200)

# ctest yr-native-bridge-repair (lane N4): an engineer repairs a broken bridge from its repair hut, a hut blown with C4
# destroys it again (IsStrange for TibSun's Considered_Vehicle; yr/tests/yr_bridge_repair.py)
add_test(NAME yr-native-bridge-repair COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_bridge_repair.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-bridge-repair")
set_tests_properties(yr-native-bridge-repair PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 240)

# ctest yr-native-keys-load (lane N5): Tab opens the Diplomacy dialog, Delete with no planned node, and a load from the
# options menu with combat lights alive: functions the tree had not recovered (yr/tests/yr_keys_load.py)
add_test(NAME yr-native-keys-load COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_keys_load.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-keys-load")
set_tests_properties(yr-native-keys-load PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 240)

# ctest yr-native-save-dialog (lane N3): a save through the options menu's Save dialog (yr/tests/yr_save_dialog.py)
add_test(NAME yr-native-save-dialog COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_save_dialog.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-save-dialog")
set_tests_properties(yr-native-save-dialog PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 240)

# ctest yr-native-choose-map (lane N6): the skirmish lobby's Choose Map, a row clicked, OK, the game started on that
# map (yr/tests/yr_choose_map.py; the dialog's procedure was a link-closure trap). Lane N8: and Create Random Map,
# Preview, OK, the generated map played (the random map generator was a link-closure trap; glue/mapgen_native.inc)
add_test(NAME yr-native-choose-map COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_choose_map.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-choose-map")
set_tests_properties(yr-native-choose-map PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 600)

# ctest yr-native-campaign-allied / -soviet (lane Y12): each side's campaign through the menus, its seven
# missions won in turn (RA2_NATIVE_FORCE_END) through their score screens, the final movie, the credits
# and the main menu again (yr/tests/yr_campaign_chain.py; about 6 minutes a side).
foreach(yr_campaign_side allied soviet)
  add_test(NAME yr-native-campaign-${yr_campaign_side} COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_campaign_chain.py"
           --side ${yr_campaign_side} --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
           --work "${CMAKE_CURRENT_BINARY_DIR}/yr-campaign-${yr_campaign_side}" --seconds 600)
  set_tests_properties(yr-native-campaign-${yr_campaign_side} PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 900)
endforeach()

# ctest mission-soak-yr-allied / -soviet (lane N25; label soak, not in the default gate; named outside yr-native so
# the gate's -R yr-native leaves them out): every mission of the side reached through the menus and forced wins,
# then played 4,000 game frames with the soak's scripted orders, twice with one seed (tests/mission_soak.py).
foreach(yr_soak_side allied soviet)
  add_test(NAME mission-soak-yr-${yr_soak_side} COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/mission_soak.py"
           --game yr --side ${yr_soak_side} --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
           --work "${CMAKE_CURRENT_BINARY_DIR}/mission-soak/yr-${yr_soak_side}" --twice --jobs 4)
  set_tests_properties(mission-soak-yr-${yr_soak_side} PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 3600 LABELS soak)
endforeach()

# ctest yr-native-lan-lobby (lane YW): two games on this Mac over UDP through the Network screens: the
# host screen, the guest's lobby listing the host's game and its join request (first stage: the join's
# answer and the game are not reached yet; yr/tests/yr_lan_lobby.py).
add_test(NAME yr-native-lan-lobby COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_lan_lobby.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-lan-lobby")
set_tests_properties(yr-native-lan-lobby PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 300 RUN_SERIAL TRUE)

# ctest yr-native-lan-pair (lane YW2): two games on this Mac play a LAN game: the host seats the guest,
# chat both ways in the setup screens, Accept and Start Game, then 3,000 frames in step with every
# frame's game CRC equal (yr/tests/yr_lan_pair.py). yr-native-lan-pair-wine: the guest is the Windows
# build under Wine (RA2_NATIVE_YR_WINDOWS_EXE: its yr-native.exe, from native/build.sh yr-windows; the
# Wine prefix is deleted after the run); skipped without it or without Wine.
add_test(NAME yr-native-lan-pair COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_lan_pair.py"
         --binary $<TARGET_FILE:yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-lan-pair" --frames 3000)
set_tests_properties(yr-native-lan-pair PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 1200 RUN_SERIAL TRUE)
if(NOT RA2_NATIVE_WINE)
  # The Mac build's Wine for the guest (as cmake/windows.cmake looks for it): RA2_NATIVE_WINE, else `wine` on
  # PATH, else the Wine bundle kept under the main checkout's private/tools.
  find_program(yr_wine_found wine)
  get_filename_component(yr_checkout "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
  string(REGEX REPLACE "/\\.claude/worktrees/[^/]+$" "" yr_main_checkout "${yr_checkout}")
  set(yr_wine_bundle "${yr_main_checkout}/private/tools/wine-11.18/Wine Devel.app/Contents/Resources/wine/bin/wine")
  if(yr_wine_found)
    set(RA2_NATIVE_WINE "${yr_wine_found}")
  elseif(EXISTS "${yr_wine_bundle}")
    set(RA2_NATIVE_WINE "${yr_wine_bundle}")
  endif()
endif()
set(RA2_NATIVE_YR_WINDOWS_EXE "" CACHE FILEPATH "The Windows build's yr-native.exe for yr-native-lan-pair-wine (optional)")
add_test(NAME yr-native-lan-pair-wine COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/yr_lan_pair.py"
         --binary $<TARGET_FILE:yr-native> --binary-b "${RA2_NATIVE_YR_WINDOWS_EXE}" --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --work "${CMAKE_CURRENT_BINARY_DIR}/yr-lan-pair-wine" --frames 3000
         --wine-prefix-b "${CMAKE_CURRENT_BINARY_DIR}/wine/yr-lan-pair" --wine "${RA2_NATIVE_WINE}")
set_tests_properties(yr-native-lan-pair-wine PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 1800 RUN_SERIAL TRUE)

# One game (lane Y5, -DRA2_NATIVE_COMBINED=ON): native/yr/combined.cmake.
include("${CMAKE_CURRENT_SOURCE_DIR}/yr/combined.cmake")
