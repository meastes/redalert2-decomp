# Native arm64 macOS tree built from src/recovered + src/shared (see README.md).
# Separate from the root CMakeLists (the exact original-target build), which
# this project never touches. Configure:
#   cmake -S native -B build/native -G Ninja && cmake --build build/native
cmake_minimum_required(VERSION 3.20)
project(ra2_native VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(APPLE AND NOT CMAKE_OSX_ARCHITECTURES)
  set(CMAKE_OSX_ARCHITECTURES arm64)
endif()

set(RA2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(RA2_NATIVE_SUBSYSTEM "core" CACHE STRING "Compile groups to build (native_units.py --subsystem)")
option(RA2_NATIVE_SDL "Build the SDL3 host executable" ON)
option(RA2_NATIVE_DRAFTS "Append the non-exact launch-path drafts (native/drafts.json) to their groups" OFF)
option(RA2_NATIVE_WIN64_PARAMS "Pointer-sized WPARAM/LPARAM/LRESULT/*_PTR, as on Win64 (native_units.py --win64-params)" ON)
set(RA2_NATIVE_DRAFT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." CACHE PATH "Checkout whose build/class-work holds the lane drafts")

# 1. Generate the native view: port-rewritten shared headers and one TU per
#    catalog compile group (the exact build's unit structure). Re-run by
#    reconfiguring after src/ or the catalog changes.
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(ra2_draft_args "")
if(RA2_NATIVE_WIN64_PARAMS)
  list(APPEND ra2_draft_args --win64-params)
endif()
if(RA2_NATIVE_DRAFTS)
  list(APPEND ra2_draft_args --drafts --draft-root "${RA2_NATIVE_DRAFT_ROOT}")
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
  "${CMAKE_CURRENT_SOURCE_DIR}/drafts.json")

# 2. The VC6 dialect on clang/arm64 (port/include/ra2port/prelude.hpp explains
#    each choice). -fwrapv and -fno-strict-aliasing restore what VC6 x86 code
#    relies on: wrapping signed arithmetic and type punning through pointers.
add_library(ra2_dialect INTERFACE)
target_compile_options(ra2_dialect INTERFACE
  -fms-extensions -fms-compatibility -fdelayed-template-parsing
  "SHELL:-Xclang -fno-wchar" -fshort-wchar
  -fwrapv -fno-strict-aliasing
  -Wno-error=address-of-temporary -Wno-register -Wno-c++11-narrowing -w
  "SHELL:-include ${CMAKE_CURRENT_SOURCE_DIR}/port/include/ra2port/prelude.hpp")
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
file(GLOB RA2_NATIVE_DATA CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp")
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
          OUTPUT "${RA2_NATIVE_GEN}/link_closure.c" "${RA2_NATIVE_GEN}/link_closure.json"
          COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/link_closure.py"
                  --inputs "${RA2_NATIVE_GEN}/link_inputs.txt" --host $<TARGET_OBJECTS:ra2_host_objs>
                  --out "${RA2_NATIVE_GEN}/link_closure.c"
          DEPENDS ra2_recovered_objs ra2_data ra2_port ra2_host_objs "${CMAKE_CURRENT_SOURCE_DIR}/tools/link_closure.py"
                  $<TARGET_OBJECTS:ra2_data> $<TARGET_FILE:ra2_port> $<TARGET_OBJECTS:ra2_host_objs>
                  "${RA2_NATIVE_GEN}/units.cmake"
          COMMAND_EXPAND_LISTS VERBATIM)
        set(ra2_game_inputs $<TARGET_OBJECTS:ra2_host_objs> $<TARGET_OBJECTS:ra2_recovered_objs>
            $<TARGET_OBJECTS:ra2_data> "${RA2_NATIVE_GEN}/link_closure.c")
      endif()
      add_executable(ra2-native ${ra2_game_inputs})
      target_link_libraries(ra2-native PRIVATE SDL3::SDL3 ra2_port)
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
