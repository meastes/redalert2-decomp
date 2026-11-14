# Cross-compile the native tree for x86_64 Windows on macOS with llvm-mingw
# (mstorsjo/llvm-mingw, the macOS universal release; README "Windows build").
# Nothing is installed: the toolchain is the extracted release folder, found as
#   -DRA2_LLVM_MINGW=<folder>, else $RA2_LLVM_MINGW, else the pinned release
#   under the main checkout's private/toolchains (the user-approved download).
#
#   cmake -S native -B build/native-win -G Ninja \
#     -DCMAKE_TOOLCHAIN_FILE=native/cmake/llvm-mingw-x86_64.cmake \
#     -DRA2_NATIVE_SDL_SOURCE=<SDL3 3.4.2 source> -DRA2_NATIVE_SUBSYSTEM=all
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

set(ra2_llvm_mingw_release llvm-mingw-20261020-ucrt-macos-universal)
if(NOT RA2_LLVM_MINGW AND DEFINED ENV{RA2_LLVM_MINGW})
  set(RA2_LLVM_MINGW "$ENV{RA2_LLVM_MINGW}")
endif()
if(NOT RA2_LLVM_MINGW)
  # native/cmake -> the checkout; a git worktree under .claude/worktrees uses the
  # main checkout's private/ (ignored, never in a worktree).
  get_filename_component(ra2_checkout "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)
  string(REGEX REPLACE "/\\.claude/worktrees/[^/]+$" "" ra2_main_checkout "${ra2_checkout}")
  foreach(root IN ITEMS "${ra2_checkout}" "${ra2_main_checkout}")
    set(candidate "${root}/private/toolchains/llvm-mingw-20261020/${ra2_llvm_mingw_release}")
    if(EXISTS "${candidate}/bin/x86_64-w64-mingw32-clang")
      set(RA2_LLVM_MINGW "${candidate}")
      break()
    endif()
  endforeach()
endif()
if(NOT EXISTS "${RA2_LLVM_MINGW}/bin/x86_64-w64-mingw32-clang")
  message(FATAL_ERROR "llvm-mingw not found: set RA2_LLVM_MINGW to the extracted ${ra2_llvm_mingw_release} folder")
endif()
# Try-compile projects see the same folder.
set(RA2_LLVM_MINGW "${RA2_LLVM_MINGW}" CACHE PATH "Extracted llvm-mingw release folder")
list(APPEND CMAKE_TRY_COMPILE_PLATFORM_VARIABLES RA2_LLVM_MINGW)

set(ra2_mingw_bin "${RA2_LLVM_MINGW}/bin")
set(CMAKE_C_COMPILER "${ra2_mingw_bin}/x86_64-w64-mingw32-clang")
set(CMAKE_CXX_COMPILER "${ra2_mingw_bin}/x86_64-w64-mingw32-clang++")
set(CMAKE_ASM_COMPILER "${ra2_mingw_bin}/x86_64-w64-mingw32-clang")
set(CMAKE_RC_COMPILER "${ra2_mingw_bin}/x86_64-w64-mingw32-windres")
set(CMAKE_AR "${ra2_mingw_bin}/llvm-ar" CACHE FILEPATH "")
set(CMAKE_RANLIB "${ra2_mingw_bin}/llvm-ranlib" CACHE FILEPATH "")
set(CMAKE_NM "${ra2_mingw_bin}/llvm-nm" CACHE FILEPATH "")

set(CMAKE_FIND_ROOT_PATH "${RA2_LLVM_MINGW}/x86_64-w64-mingw32")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# One self-contained executable: libc++, libunwind and winpthreads linked in, no
# toolchain DLLs beside it (the UCRT is part of Windows 10 and later).
set(CMAKE_EXE_LINKER_FLAGS_INIT "-static")
