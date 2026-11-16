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
if(NOT RA2_NATIVE_LINK_CLOSURE)
  message(FATAL_ERROR "RA2_NATIVE_COMBINED needs RA2_NATIVE_LINK_CLOSURE=ON (the engines link as the games do)")
endif()

set(ra2_engine_link_options "LINKER:-exported_symbol,_ra2_engine_main")

# Red Alert 2: ra2-native's objects (they are already weakened by its PRE_LINK: built after it).
add_library(ra2_engine_host_objs OBJECT ${ra2_host_sources})
target_link_libraries(ra2_engine_host_objs PRIVATE SDL3::SDL3 ra2_port)
target_compile_definitions(ra2_engine_host_objs PRIVATE RA2_NATIVE_GAME=1 RA2_NATIVE_ENGINE=1)
add_library(ra2-engine MODULE $<TARGET_OBJECTS:ra2_engine_host_objs> $<TARGET_OBJECTS:ra2_recovered_objs>
            $<TARGET_OBJECTS:ra2_data> "${RA2_NATIVE_GEN}/link_closure.c")
target_link_libraries(ra2-engine PRIVATE SDL3::SDL3 ra2_port ${ra2_host_frameworks})
target_compile_definitions(ra2-engine PRIVATE RA2_NATIVE_GAME=1)
target_link_options(ra2-engine PRIVATE ${ra2_engine_link_options} "LINKER:-alias_list,${RA2_NATIVE_GEN}/alias_list.txt")
set_property(TARGET ra2-engine APPEND PROPERTY LINK_DEPENDS "${RA2_NATIVE_GEN}/alias_list.txt")
set_target_properties(ra2-engine PROPERTIES PREFIX "" SUFFIX ".so")
add_dependencies(ra2-engine ra2-native)

# Yuri's Revenge: yr-native's objects (weakened and their initialisers deferred by its PRE_LINK).
add_library(yr_engine_host_objs OBJECT ${ra2_host_sources})
target_link_libraries(yr_engine_host_objs PRIVATE SDL3::SDL3 ra2_port)
target_compile_definitions(yr_engine_host_objs PRIVATE RA2_NATIVE_GAME=1 RA2_NATIVE_YR=1 RA2_NATIVE_ENGINE=1)
add_library(yr-engine MODULE $<TARGET_OBJECTS:yr_engine_host_objs> $<TARGET_OBJECTS:yr_units_objs>
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
