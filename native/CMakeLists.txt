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
set(RA2_NATIVE_DRAFT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/.." CACHE PATH "Checkout whose build/class-work holds the lane drafts")

# 1. Generate the native view: port-rewritten shared headers and one TU per
#    catalog compile group (the exact build's unit structure). Re-run by
#    reconfiguring after src/ or the catalog changes.
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(ra2_draft_args "")
if(RA2_NATIVE_DRAFTS)
  set(ra2_draft_args --drafts --draft-root "${RA2_NATIVE_DRAFT_ROOT}")
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
#    never bytes from the executable).
add_library(ra2_recovered STATIC ${RA2_NATIVE_UNITS} data/random3_tables.cpp)
target_link_libraries(ra2_recovered PUBLIC ra2_dialect)

# 4. Platform seams: Win32 / DirectDraw / DirectSound / Winsock APIs the
#    recovered code imports, implemented on POSIX and SDL3.
add_library(ra2_port STATIC port/src/win32_time.cpp)

# 5. Proof: recovered RandomClass / Random3Class run natively.
add_executable(native-core-check tests/core_check.cpp)
target_link_libraries(native-core-check PRIVATE ra2_recovered)
target_link_options(native-core-check PRIVATE -Wl,-dead_strip)
enable_testing()
add_test(NAME native-core-check COMMAND native-core-check)

if(RA2_NATIVE_SDL)
  find_package(SDL3 3.2 CONFIG)
  if(SDL3_FOUND)
    add_executable(ra2-native platform/sdl/main.cpp)
    target_link_libraries(ra2-native PRIVATE SDL3::SDL3 ra2_port)
  else()
    message(STATUS "SDL3 not found: ra2-native host skipped")
  endif()
endif()
