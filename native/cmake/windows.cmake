# The x86_64 Windows build of the native tree (README "Windows build"), included
# by CMakeLists.txt when the toolchain targets Windows (cmake/llvm-mingw-x86_64.cmake),
# after the dialect target is made. The recovered units, the port and the SDL3 host
# are the macOS build's sources; platform/win supplies what mingw-w64 lacks and the
# Windows host services.
set(ra2_win_dir "${CMAKE_CURRENT_SOURCE_DIR}/platform/win")

# The VC6 dialect on mingw-w64 headers (dialect_win.hpp explains each line); before
# the prelude, which it must precede.
target_compile_options(ra2_dialect BEFORE INTERFACE
  "$<$<COMPILE_LANGUAGE:CXX>:SHELL:-include ${ra2_win_dir}/dialect_win.hpp>")

# The generated units, the native data and the link sources name symbols in the
# Mach-O spelling (a leading underscore) in their assembler labels: each is compiled
# from a COFF copy (tools/coff_asm_labels.py) in gen/coff. The units' copies are
# made here (the units are generated at configure time); RA2_NATIVE_UNITS names them.
execute_process(
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/coff_asm_labels.py"
          --units "${RA2_NATIVE_GEN}" "${RA2_NATIVE_GEN}/coff_units.cmake"
  RESULT_VARIABLE ra2_coff_result)
if(NOT ra2_coff_result EQUAL 0)
  message(FATAL_ERROR "coff_asm_labels.py failed")
endif()
include("${RA2_NATIVE_GEN}/coff_units.cmake")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/coff_asm_labels.py")
# <out_var> gets the compiled list of the tracked sources given.
function(ra2_windows_coff_sources out_var)
  set(compiled "")
  foreach(source IN LISTS ARGN)
    if(NOT source MATCHES "\\.cpp$")
      list(APPEND compiled "${source}")
      continue()
    endif()
    file(RELATIVE_PATH relative "${CMAKE_CURRENT_SOURCE_DIR}" "${source}")
    set(copy "${RA2_NATIVE_GEN}/coff/${relative}")
    add_custom_command(OUTPUT "${copy}"
      COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/coff_asm_labels.py" "${source}" "${copy}"
      DEPENDS "${source}" "${CMAKE_CURRENT_SOURCE_DIR}/tools/coff_asm_labels.py"
      VERBATIM)
    list(APPEND compiled "${copy}")
  endforeach()
  set(${out_var} "${compiled}" PARENT_SCOPE)
endfunction()

# The port: the POSIX pieces mingw-w64 does not have (port_compat.h, posix/), on Win32
# (posix_win.cpp), and the port's operating-system services (os_win.cpp).
function(ra2_windows_port)
  target_sources(ra2_port PRIVATE "${ra2_win_dir}/posix_win.cpp" "${ra2_win_dir}/os_win.cpp")
  target_include_directories(ra2_port BEFORE PRIVATE "${ra2_win_dir}/posix")
  target_compile_options(ra2_port PRIVATE "SHELL:-include ${ra2_win_dir}/port_compat.h")
  target_compile_definitions(ra2_port PRIVATE _FILE_OFFSET_BITS=64)
  # winpthreads (pthread.h, nanosleep, clock_gettime): llvm-mingw's libc++ itself
  # uses Win32 threads, so the driver does not add it.
  target_link_libraries(ra2_port PUBLIC pthread ws2_32 iphlpapi)
endfunction()

# The SDL3 host's sources see the same layer (whichever target compiles them;
# host_compat.h adds the one SDL name clash).
set_source_files_properties(
  platform/sdl/main.cpp platform/sdl/host.cpp platform/sdl/touch.cpp platform/sdl/enhancements.cpp
  PROPERTIES COMPILE_OPTIONS "-include${ra2_win_dir}/host_compat.h"
             INCLUDE_DIRECTORIES "${ra2_win_dir}/posix")
set_source_files_properties(platform/win/host_win.cpp
  PROPERTIES COMPILE_OPTIONS "-include${ra2_win_dir}/port_compat.h")
# The files that include <windows.h> keep Winsock's own names.
set_source_files_properties(platform/win/host_win.cpp "${ra2_win_dir}/posix_win.cpp" "${ra2_win_dir}/os_win.cpp"
  PROPERTIES COMPILE_DEFINITIONS RA2_WIN32_HEADERS)

# The game's link. The exact build's units may each carry a copy of one definition
# (VC6 COMDATs, repeated callees: weaken_duplicates.py on macOS); lld keeps the
# first in link order (the original's unit order) with --allow-multiple-definition,
# which also lets the port's definitions of VC6 C runtime names (crt.cpp) stand
# before the UCRT's.
# RA2_WINDOWS_GUI: the GUI subsystem (no console window; the test runs' logs need
# the console subsystem, the default).
option(RA2_WINDOWS_GUI "Link ra2-native for the Windows GUI subsystem" OFF)
function(ra2_windows_game_link target)
  target_link_options(${target} PRIVATE "LINKER:--allow-multiple-definition")
  # The main thread's stack: 8 MiB, as on macOS (Windows' default 1 MiB overflows in
  # the start-up path).
  target_link_options(${target} PRIVATE "LINKER:--stack,8388608")
  # The static initialisers in link order, the original's _initterm order (the units'
  # order), as on macOS: the mingw-w64 runtime runs them backwards (ctors_win.c).
  target_sources(${target} PRIVATE "${ra2_win_dir}/ctors_win.c")
  target_link_libraries(${target} PRIVATE ws2_32 iphlpapi winmm shell32 ole32 uuid)
  if(RA2_WINDOWS_GUI)
    target_link_options(${target} PRIVATE -mwindows)
  endif()
endfunction()

# The game checks on the Mac (CMakeLists.txt: native-census, native-enhancements-frames,
# native-enhancements-settings): the .exe runs under Wine in a fresh prefix of each test's
# own under the build folder (tests/wine.py), and the test is skipped when no Wine is
# found. Wine is a local test host, never shipped: RA2_NATIVE_WINE (cache or environment),
# else `wine` on PATH, else the Wine bundle kept under the main checkout's private/tools.
set(RA2_NATIVE_WINE "$ENV{RA2_NATIVE_WINE}" CACHE FILEPATH "Wine for the Windows build's checks on the Mac (optional; tests skip without it)")
if(NOT RA2_NATIVE_WINE)
  find_program(ra2_wine_found wine)
  if(ra2_wine_found)
    set(RA2_NATIVE_WINE "${ra2_wine_found}")
  else()
    get_filename_component(ra2_checkout "${CMAKE_CURRENT_SOURCE_DIR}/.." ABSOLUTE)
    string(REGEX REPLACE "/\\.claude/worktrees/[^/]+$" "" ra2_main_checkout "${ra2_checkout}")
    foreach(root IN ITEMS "${ra2_checkout}" "${ra2_main_checkout}")
      set(candidate "${root}/private/tools/wine-11.18/Wine Devel.app/Contents/Resources/wine/bin/wine")
      if(EXISTS "${candidate}")
        set(RA2_NATIVE_WINE "${candidate}")
        break()
      endif()
    endforeach()
  endif()
endif()
foreach(check census frames settings newgame)
  set(ra2_wine_args_${check} --wine-prefix "${CMAKE_CURRENT_BINARY_DIR}/wine/${check}" --wine "${RA2_NATIVE_WINE}")
endforeach()
# The link-closure baseline is the Mac link's: Win64's long long is the Mac's long.
set(ra2_closure_check_args --target=windows)
