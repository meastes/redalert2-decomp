# One game (lane Y5; native/yr/README.md "One game"; user design 2026-11-15): Red Alert 2 and
# Yuri's Revenge in one app, one engine at a time, a toggle at the bottom left of each main menu.
# Off by default (-DRA2_NATIVE_COMBINED=ON with -DRA2_NATIVE_YR=ON and RA2_NATIVE_LINK_CLOSURE, Mac):
# ra2-native, yr-native and their tests are unchanged. Each engine is a bundle of the same objects
# as its game's executable, with the SDL host compiled again with RA2_NATIVE_ENGINE (main.cpp's
# main becomes ra2_engine_main) and only that symbol exported, so the two games' identical names
# stay inside their bundles; ra2yr-native is the thin host (combined/one_game.cpp).
option(RA2_NATIVE_COMBINED "Also build ra2yr-native, Red Alert 2 and Yuri's Revenge in one app (engine bundles + thin host)" OFF)
if(NOT RA2_NATIVE_COMBINED)
  return()
endif()
if(NOT RA2_NATIVE_LINK_CLOSURE)   # (iOS: build-ios.command --one sets both)
  message(FATAL_ERROR "RA2_NATIVE_COMBINED needs RA2_NATIVE_LINK_CLOSURE=ON (the engines link as the games do)")
endif()

set(ra2_engine_link_options "LINKER:-exported_symbol,_ra2_engine_main")

# Each game's SDL host compiled as an engine (main.cpp's main becomes ra2_engine_main). The Apple
# host services (host_apple.mm) are compiled once, outside the engines (lane Y9): ra2yr-one links
# that one copy with the thin host and both engines call it (its Objective-C classes are global in
# the runtime); each Mac bundle links it in as before (no class there: the classes are iOS's).
set(ra2_engine_host_sources ${ra2_host_sources})
set(ra2_engine_hostos "")
if(APPLE)
  list(REMOVE_ITEM ra2_engine_host_sources platform/sdl/host_apple.mm)
  add_library(ra2_engine_hostos_objs OBJECT platform/sdl/host_apple.mm)
  set(ra2_engine_hostos $<TARGET_OBJECTS:ra2_engine_hostos_objs>)
endif()
add_library(ra2_engine_host_objs OBJECT ${ra2_engine_host_sources})
target_link_libraries(ra2_engine_host_objs PRIVATE SDL3::SDL3 ra2_port)
target_compile_definitions(ra2_engine_host_objs PRIVATE RA2_NATIVE_GAME=1 RA2_NATIVE_ENGINE=1)
add_library(yr_engine_host_objs OBJECT ${ra2_engine_host_sources})
target_link_libraries(yr_engine_host_objs PRIVATE SDL3::SDL3 ra2_port)
target_compile_definitions(yr_engine_host_objs PRIVATE RA2_NATIVE_GAME=1 RA2_NATIVE_YR=1 RA2_NATIVE_ENGINE=1)

if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")   # bundles: the Mac's (iOS loads no bundle)
# Red Alert 2: ra2-native's objects (they are already weakened by its PRE_LINK: built after it).
add_library(ra2-engine MODULE $<TARGET_OBJECTS:ra2_engine_host_objs> ${ra2_engine_hostos} $<TARGET_OBJECTS:ra2_recovered_objs>
            $<TARGET_OBJECTS:ra2_data> "${RA2_NATIVE_GEN}/link_closure.c")
target_link_libraries(ra2-engine PRIVATE SDL3::SDL3 ra2_port ${ra2_host_frameworks})
target_compile_definitions(ra2-engine PRIVATE RA2_NATIVE_GAME=1)
target_link_options(ra2-engine PRIVATE ${ra2_engine_link_options} "LINKER:-alias_list,${RA2_NATIVE_GEN}/alias_list.txt")
set_property(TARGET ra2-engine APPEND PROPERTY LINK_DEPENDS "${RA2_NATIVE_GEN}/alias_list.txt")
set_target_properties(ra2-engine PROPERTIES PREFIX "" SUFFIX ".so")
add_dependencies(ra2-engine ra2-native)

# Yuri's Revenge: yr-native's objects (weakened and their initialisers deferred by its PRE_LINK).
add_library(yr-engine MODULE $<TARGET_OBJECTS:yr_engine_host_objs> ${ra2_engine_hostos} $<TARGET_OBJECTS:yr_units_objs>
            $<TARGET_OBJECTS:yr_data_obj> "${RA2_NATIVE_YR_GEN}/link_closure.c")
target_link_libraries(yr-engine PRIVATE SDL3::SDL3 ra2_port ${ra2_host_frameworks})
target_compile_definitions(yr-engine PRIVATE RA2_NATIVE_GAME=1 RA2_NATIVE_YR=1)
target_link_options(yr-engine PRIVATE ${ra2_engine_link_options} "LINKER:-alias_list,${RA2_NATIVE_YR_GEN}/alias_list.txt")
set_property(TARGET yr-engine APPEND PROPERTY LINK_DEPENDS "${RA2_NATIVE_YR_GEN}/alias_list.txt")
set_target_properties(yr-engine PROPERTIES PREFIX "" SUFFIX ".so")
add_dependencies(yr-engine yr-native)

# The thin host: the process, SDL, the window and the swap; the engines beside it.
add_executable(ra2yr-native "${CMAKE_CURRENT_SOURCE_DIR}/yr/combined/one_game.cpp")
target_link_libraries(ra2yr-native PRIVATE SDL3::SDL3)
add_dependencies(ra2yr-native ra2-engine yr-engine)

# ctest one-game-toggle (lane Y5): start -> the RA2 main menu -> toggle -> the YR main menu (no
# start-up movie) -> toggle -> the RA2 main menu, then a skirmish started in each game after a
# swap; the selection kept in the settings; no game process left (yr/tests/one_game_toggle.py).
# Needs RA2_NATIVE_YR_DATA_DIR (RA2 + YR folder) and RA2_NATIVE_CENSUS_GAME_EXE (game.exe).
add_test(NAME one-game-toggle COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/one_game_toggle.py"
         --binary $<TARGET_FILE:ra2yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/one-game-toggle")
set_tests_properties(one-game-toggle PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 600)
# ctest one-game-saves (lane Y10): each game saves in its original's format after the toggle (RA2 -> YR,
# YR -> RA2) and each save loads back in its game (yr/tests/one_game_saves.py); the same conditions.
add_test(NAME one-game-saves COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/one_game_saves.py"
         --binary $<TARGET_FILE:ra2yr-native> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
         --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/one-game-saves")
set_tests_properties(one-game-saves PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 700)

endif()

# One game in one executable (lane Y8; native/yr/README.md "iOS in one executable", stage 1 on the
# Mac): the same two engines as relocatable objects instead of bundles, linked with the thin host
# into ra2yr-one. Each engine object is `ld -r` of its objects between two markers
# (combined/engine_marker.c) with only its entry (renamed ra2_engine_main_<game>) and its markers
# exported, so the games' identical names stay apart as in the bundles; its initialisers move to
# __DATA,__<game>e_init (defer_initializers.py) and the host runs them at each start of the game,
# after copying the game's pristine __data/__bss back (a fresh start without a fresh image).
option(RA2_NATIVE_ONE_EXECUTABLE "Also build ra2yr-one: both engines linked into one executable (lane Y8)" OFF)
if(RA2_NATIVE_ONE_EXECUTABLE)
  add_library(ra2_one_port_objs OBJECT ${RA2_PORT_SOURCES})
  target_include_directories(ra2_one_port_objs PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/port/include")
  target_compile_options(ra2_one_port_objs PRIVATE -Wall -Wno-unused-parameter -Wno-unused-function)
  target_link_libraries(ra2_one_port_objs PRIVATE SDL3::SDL3)
  set(ra2_one_engine_objects "")
  foreach(engine ra2 yr)
    foreach(side begin end)
      add_library(${engine}_one_marker_${side} OBJECT "${CMAKE_CURRENT_SOURCE_DIR}/yr/combined/engine_marker.c")
      target_compile_definitions(${engine}_one_marker_${side} PRIVATE ENGINE=${engine} SIDE=${side})
      target_compile_options(${engine}_one_marker_${side} PRIVATE -fno-common)
    endforeach()
  endforeach()
  add_library(ra2_one_closure OBJECT "${RA2_NATIVE_GEN}/link_closure.c")
  target_compile_definitions(ra2_one_closure PRIVATE RA2_NATIVE_GAME=1)
  add_library(yr_one_closure OBJECT "${RA2_NATIVE_YR_GEN}/link_closure.c")
  target_compile_definitions(yr_one_closure PRIVATE RA2_NATIVE_GAME=1 RA2_NATIVE_YR=1)
  # No commons: an engine's zero placeholders must be its own storage inside its bss span, not
  # tentative definitions the final link would merge with the other engine's of the same name.
  target_compile_options(ra2_one_closure PRIVATE -fno-common)
  target_compile_options(yr_one_closure PRIVATE -fno-common)
  set(ra2_one_inputs_ra2 $<TARGET_OBJECTS:ra2_one_marker_begin> $<TARGET_OBJECTS:ra2_engine_host_objs>
      $<TARGET_OBJECTS:ra2_recovered_objs> $<TARGET_OBJECTS:ra2_data> $<TARGET_OBJECTS:ra2_one_closure>
      $<TARGET_OBJECTS:ra2_one_port_objs> $<TARGET_OBJECTS:ra2_one_marker_end>)
  set(ra2_one_inputs_yr $<TARGET_OBJECTS:yr_one_marker_begin> $<TARGET_OBJECTS:yr_engine_host_objs>
      $<TARGET_OBJECTS:yr_units_objs> $<TARGET_OBJECTS:yr_data_obj> $<TARGET_OBJECTS:yr_one_closure>
      $<TARGET_OBJECTS:ra2_one_port_objs> $<TARGET_OBJECTS:yr_one_marker_end>)
  set(ra2_one_alias_ra2 "${RA2_NATIVE_GEN}/alias_list.txt")
  set(ra2_one_alias_yr "${RA2_NATIVE_YR_GEN}/alias_list.txt")
  set(ra2_one_after_ra2 ra2-native)
  set(ra2_one_after_yr yr-native)
  set(ra2_one_r_target "")
  if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    if(CMAKE_OSX_SYSROOT MATCHES "[Ss]imulator")
      set(ra2_one_r_target "--target=arm64-apple-ios${CMAKE_OSX_DEPLOYMENT_TARGET}-simulator")
    else()
      set(ra2_one_r_target "--target=arm64-apple-ios${CMAKE_OSX_DEPLOYMENT_TARGET}")
    endif()
    execute_process(COMMAND xcrun --sdk "${CMAKE_OSX_SYSROOT}" --show-sdk-path OUTPUT_VARIABLE ra2_one_sdk OUTPUT_STRIP_TRAILING_WHITESPACE)
    list(APPEND ra2_one_r_target -isysroot "${ra2_one_sdk}")
  endif()
  foreach(engine ra2 yr)
    set(out "${CMAKE_CURRENT_BINARY_DIR}/${engine}-engine-one.o")
    file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${engine}-engine-one.filelist"
         CONTENT "$<JOIN:${ra2_one_inputs_${engine}},\n>\n")
    add_custom_command(OUTPUT "${out}"
      COMMAND "${CMAKE_CXX_COMPILER}" -arch arm64 ${ra2_one_r_target} -r -nostdlib -o "${out}"
              "-Wl,-filelist,${CMAKE_CURRENT_BINARY_DIR}/${engine}-engine-one.filelist"
              "-Wl,-alias_list,${ra2_one_alias_${engine}}"
              "-Wl,-alias,_ra2_engine_main,_ra2_engine_main_${engine}"
              "-Wl,-exported_symbol,_ra2_engine_main_${engine}"
              "-Wl,-exported_symbol,_${engine}_*_begin" "-Wl,-exported_symbol,_${engine}_*_end"
      COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tools/defer_initializers.py"
              --object "${out}" --section "__${engine}e_init"
      DEPENDS ${ra2_one_inputs_${engine}} "${ra2_one_alias_${engine}}" ${ra2_one_after_${engine}}
              "${CMAKE_CURRENT_BINARY_DIR}/${engine}-engine-one.filelist"
      COMMENT "One executable: the ${engine} engine object"
      COMMAND_EXPAND_LISTS VERBATIM)
    list(APPEND ra2_one_engine_objects "${out}")
  endforeach()
  set_source_files_properties(${ra2_one_engine_objects} PROPERTIES EXTERNAL_OBJECT TRUE GENERATED TRUE)
  add_executable(ra2yr-one "${CMAKE_CURRENT_SOURCE_DIR}/yr/combined/one_game.cpp" ${ra2_engine_hostos} ${ra2_one_engine_objects})
  target_compile_definitions(ra2yr-one PRIVATE RA2_NATIVE_ONE_EXECUTABLE=1)
  target_link_libraries(ra2yr-one PRIVATE SDL3::SDL3 ${ra2_host_frameworks})
  if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    # The iOS app (build-ios.command, the default since lane Y9): it replaces the Red Alert 2 app,
    # with its bundle id and name (owner ruling 2026-11-16), so the player's Red Alert 2 saves,
    # settings and imported files carry over; it asks only for Yuri's Revenge's files.
    ra2_ios_app(ra2yr-one RA2YR "io.github.ra2native.RA2" "Red Alert 2")
  endif()
endif()
if(RA2_NATIVE_ONE_EXECUTABLE AND NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
  # ctest one-exe-toggle (lane Y8): the one-game-toggle route on ra2yr-one (the second Red Alert 2
  # start is a reset of its data in place, not a fresh image).
  add_test(NAME one-exe-toggle COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/one_game_toggle.py"
           --binary $<TARGET_FILE:ra2yr-one> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
           --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/one-exe-toggle")
  set_tests_properties(one-exe-toggle PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 600)
  # ctest one-exe-saves (lane Y10): the one-game-saves route on ra2yr-one.
  add_test(NAME one-exe-saves COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/yr/tests/one_game_saves.py"
           --binary $<TARGET_FILE:ra2yr-one> --data-dir "${RA2_NATIVE_YR_DATA_DIR}"
           --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/one-exe-saves")
  set_tests_properties(one-exe-saves PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 700)
endif()
