# Native arm64 macOS tree built from src/recovered + src/shared (see README.md).
# Separate from the root CMakeLists (the exact original-target build), which
# this project never touches. Configure:
#   cmake -S native -B build/native -G Ninja && cmake --build build/native
cmake_minimum_required(VERSION 3.20)
project(ra2_native VERSION 0.1.0 LANGUAGES C CXX ASM)
if(APPLE)
  # The host's Apple services (platform/sdl/host_apple.mm). Before any subdirectory
  # (the iOS build's SDL3), so every directory sees the language.
  enable_language(OBJCXX)
endif()

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
  message(FATAL_ERROR "native_units.py failed (see its message above: a stale native/drafts.json, "
                      "native/port/view_layouts.json or native/initterm.json fails the configure)")
endif()
include("${RA2_NATIVE_GEN}/units.cmake")
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/tools/native_units.py" "${CMAKE_CURRENT_SOURCE_DIR}/port/fixups.json"
  "${CMAKE_CURRENT_SOURCE_DIR}/drafts.json" "${CMAKE_CURRENT_SOURCE_DIR}/link/initterm_rewrites.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/initterm.json" "${CMAKE_CURRENT_SOURCE_DIR}/port/view_layouts.json"
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
  # VC6 code tests `this` against null (AbstractClass::As_Techno 0x0040DC20 `if (this)`, called on
  # ScrollClass::Resolve_Point's object, null over open ground); clang at -O2 deletes such tests as
  # always true. The x86_64 Windows build at -O2 (lane C16) called a null object's Kind_Of at the
  # first game frame. -fno-delete-null-pointer-checks keeps the tests, as VC6 did.
  "$<$<COMPILE_LANGUAGE:CXX>:-fno-delete-null-pointer-checks>"
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
  # The original computes with the x87 rounding toward zero (WinMain's
  # _controlfp(_RC_CHOP, _MCW_RC); port/src/crt.cpp applies it to the game
  # thread). -frounding-math keeps clang from assuming round-to-nearest when it
  # folds or reorders floating-point code (Scout1274).
  "$<$<COMPILE_LANGUAGE:CXX>:-frounding-math>"
  "$<$<COMPILE_LANGUAGE:CXX>:-Wno-error=address-of-temporary;-Wno-register;-Wno-c++11-narrowing;-w>"
  "$<$<COMPILE_LANGUAGE:CXX>:SHELL:-include ${CMAKE_CURRENT_SOURCE_DIR}/port/include/ra2port/prelude.hpp>")
target_include_directories(ra2_dialect INTERFACE "${RA2_NATIVE_GEN}/overlay")
# The x86_64 Windows build (cmake/llvm-mingw-x86_64.cmake, README "Windows build"):
# the dialect's, the native sources' and the port's Windows layer (platform/win).
if(WIN32)
  include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/windows.cmake")
endif()

# 3. Recovered code (accepted sources and, later, parked drafts) and the
#    native data it needs (adopted source text or behaviour-written generators,
#    never bytes from the executable). The units are an OBJECT library: the
#    game links every object in the order units.cmake gives (the original's
#    _initterm order, so static initialisers run as they did); the static
#    archive is only for the small checks.
add_library(ra2_recovered_objs OBJECT ${RA2_NATIVE_UNITS})
target_link_libraries(ra2_recovered_objs PUBLIC ra2_dialect)
add_library(ra2_recovered STATIC $<TARGET_OBJECTS:ra2_recovered_objs>)
target_link_libraries(ra2_recovered PUBLIC ra2_dialect)

# Native definitions of the data the recovered code reads but does not define
# (data/README.md): zero-initialised globals by size/alignment from the catalog,
# adopted tables. Initialised data without a permitted source is listed, not defined.
file(GLOB RA2_NATIVE_DATA CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/data/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/link/*.cpp")
if(WIN32)
  ra2_windows_coff_sources(RA2_NATIVE_DATA ${RA2_NATIVE_DATA})
endif()
# Generated data: computed by behaviour-written generators (tools/*_tables.py),
# each checked against the original privately (README "Generated data").
set(ra2_tables_args "")
if(WIN32)
  set(ra2_tables_args --coff)
endif()
execute_process(
  COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/fast_math_tables.py"
          --out "${RA2_NATIVE_GEN}/fast_math_tables.s" ${ra2_tables_args}
  RESULT_VARIABLE ra2_tables_result)
if(NOT ra2_tables_result EQUAL 0)
  message(FATAL_ERROR "fast_math_tables.py failed")
endif()
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/fast_math_tables.py")
list(APPEND RA2_NATIVE_DATA "${RA2_NATIVE_GEN}/fast_math_tables.s")
# The arm64 game (macOS, and iOS, where CMAKE_SYSTEM_PROCESSOR is empty and the architecture
# comes from CMAKE_OSX_ARCHITECTURES: before lane C17 the iOS app had no field maps).
if(NOT WIN32 AND (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64" OR CMAKE_OSX_ARCHITECTURES MATCHES "arm64"))
  set(RA2_NATIVE_ARM64 ON)
else()
  set(RA2_NATIVE_ARM64 OFF)
endif()
# Field maps for reading the original game's saves (C6, README "Original saves"):
# i686 and native record layouts of the units that load each type in
# data/original_save_types.txt (tools/original_layouts.py): the arm64 game, and the
# x86_64 Windows game (lane C15: LLP64, its own compiler and dialect header); without
# them (other builds) port/src/original_save.cpp has none.
set(ra2_original_layouts_target "")
if(WIN32)
  set(ra2_original_layouts_target --target windows "--cxx=${CMAKE_CXX_COMPILER}")
endif()
if(RA2_NATIVE_SUBSYSTEM STREQUAL "all" AND ((RA2_NATIVE_ARM64 AND NOT WIN32) OR WIN32))
  add_custom_command(
    OUTPUT "${RA2_NATIVE_GEN}/original_layouts.cpp"
    COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/original_layouts.py"
            --build "${CMAKE_CURRENT_BINARY_DIR}" --out "${RA2_NATIVE_GEN}/original_layouts.cpp"
            --report "${RA2_NATIVE_GEN}/original_layouts.json" --members "${RA2_NATIVE_GEN}/original_layouts_members.json"
            ${ra2_original_layouts_target}
    DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/tools/original_layouts.py" "${CMAKE_CURRENT_SOURCE_DIR}/data/original_save_types.txt"
            "${CMAKE_CURRENT_SOURCE_DIR}/port/include/ra2port/prelude.hpp" "${RA2_NATIVE_GEN}/units.cmake" ${RA2_NATIVE_UNITS}
    VERBATIM)
  list(APPEND RA2_NATIVE_DATA "${RA2_NATIVE_GEN}/original_layouts.cpp")
endif()
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

if(WIN32)
  ra2_windows_port()
endif()

# The iOS build (build-ios.command, Scout1247) makes the game app only: the checks
# below run on the Mac build. The Windows build is cross-compiled: the unit checks
# below are the Mac build's; its game checks (census, enhancements, link closure,
# vtable views) are registered further down and run its .exe under Wine when one is
# found (cmake/windows.cmake, RA2_NATIVE_WINE), skipped otherwise.
if(CMAKE_SYSTEM_NAME STREQUAL "iOS" OR WIN32)
  set(RA2_NATIVE_CHECKS OFF)
else()
  set(RA2_NATIVE_CHECKS ON)
endif()
enable_testing()
if(RA2_NATIVE_CHECKS)
# 5. Proof: recovered RandomClass / Random3Class run natively.
add_executable(native-core-check tests/core_check.cpp "${RA2_NATIVE_GEN}/fast_math_tables.s")   # the arctangent table (C5 checks)
target_link_libraries(native-core-check PRIVATE ra2_recovered)
target_link_options(native-core-check PRIVATE -Wl,-dead_strip)
add_test(NAME native-core-check COMMAND native-core-check)
# Mods (lane C16): the port's read layers (a mod ahead of the write folder and the installation;
# merged listings; writes never into the mod) in a made-up installation (tests/mod_layers.cpp).
if(NOT WIN32)
  add_executable(native-mod-layers tests/mod_layers.cpp)
  target_link_libraries(native-mod-layers PRIVATE ra2_port)
  add_test(NAME native-mod-layers COMMAND native-mod-layers "${CMAKE_CURRENT_BINARY_DIR}/mod-layers")
endif()

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
# 7b. Original saves (C6): every type in data/original_save_types.txt has an i686
#     layout of the original's size whose members all pair with the arm64 layout.
if(RA2_NATIVE_SUBSYSTEM STREQUAL "all" AND RA2_NATIVE_ARM64 AND NOT WIN32)
  add_test(NAME native-original-layouts COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/original_layouts.py"
           --build "${CMAKE_CURRENT_BINARY_DIR}" --out "${CMAKE_CURRENT_BINARY_DIR}/original_layouts_check.cpp" --check)
endif()
endif()
# The same check for the x86_64 Windows layouts (C15; LLP64, the build's own compiler).
if(WIN32 AND RA2_NATIVE_SUBSYSTEM STREQUAL "all")
  add_test(NAME native-original-layouts COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/original_layouts.py"
           --build "${CMAKE_CURRENT_BINARY_DIR}" --out "${CMAKE_CURRENT_BINARY_DIR}/original_layouts_check.cpp" --check
           ${ra2_original_layouts_target})
endif()

# Libraries whose exports the link closure counts as system (default: Homebrew's
# SDL3 dylib; the iOS build passes its static SDL3).
set(RA2_NATIVE_CLOSURE_SYSTEM_LIBS "" CACHE STRING "Libraries whose exports link_closure.py treats as system")
option(RA2_NATIVE_LINK_CLOSURE "Close the ra2-native link with generated trap stubs for still-unrecovered code and placeholders for unsourced data (never shipped; lists them in gen/link_closure.json)" OFF)

# SDL3 from a source folder, built here as a static library (the iOS build: no
# dylib may ship in an iOS app; build-ios.command passes SDL 3.4.2's source),
# else the installed SDL3 package (Homebrew's dylib on the Mac).
set(RA2_NATIVE_SDL_SOURCE "" CACHE PATH "SDL3 source folder to build statically (the iOS build)")
if(RA2_NATIVE_SDL AND RA2_NATIVE_SDL_SOURCE)
  set(SDL_SHARED OFF CACHE BOOL "" FORCE)
  set(SDL_STATIC ON CACHE BOOL "" FORCE)
  set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
  set(SDL_TESTS OFF CACHE BOOL "" FORCE)
  set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
  add_subdirectory("${RA2_NATIVE_SDL_SOURCE}" "${CMAKE_CURRENT_BINARY_DIR}/sdl3" EXCLUDE_FROM_ALL)
  set(SDL3_FOUND TRUE)
  set(RA2_NATIVE_CLOSURE_SYSTEM_LIBS "$<TARGET_FILE:SDL3-static>")
elseif(RA2_NATIVE_SDL)
  find_package(SDL3 3.2 CONFIG)
endif()
if(RA2_NATIVE_SDL)
  if(SDL3_FOUND)
    # The host's platform services (platform/sdl/host_os.hpp): host_apple.mm on
    # macOS and iOS, the host's only file with Apple-only calls (Scout1247).
    set(ra2_host_sources platform/sdl/main.cpp platform/sdl/host.cpp platform/sdl/touch.cpp platform/sdl/enhancements.cpp
        platform/sdl/mod_import.cpp)
    set(ra2_host_frameworks "")
    set(ra2_closure_frameworks "")
    if(APPLE)
      list(APPEND ra2_host_sources platform/sdl/host_apple.mm)
      set_source_files_properties(platform/sdl/host_apple.mm PROPERTIES COMPILE_OPTIONS "-fobjc-arc")
      if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        set(ra2_closure_frameworks Foundation UIKit UniformTypeIdentifiers AVFAudio CoreGraphics)
      else()
        set(ra2_closure_frameworks Foundation AppKit)
      endif()
      list(APPEND ra2_host_frameworks -lz)    # mod_import.cpp's .zip unpacking (the system's zlib)
      foreach(f IN LISTS ra2_closure_frameworks)
        list(APPEND ra2_host_frameworks "-framework ${f}")
      endforeach()
    elseif(WIN32)
      list(APPEND ra2_host_sources platform/win/host_win.cpp)
    endif()
    # How link_closure.py reads the objects and which libraries are the system's.
    if(WIN32)
      set(ra2_closure_target_args --target windows "--nm=${CMAKE_NM}" "--sysroot=${RA2_LLVM_MINGW}/x86_64-w64-mingw32")
    else()
      set(ra2_closure_target_args "--sdk=${CMAKE_OSX_SYSROOT}" --frameworks ${ra2_closure_frameworks})
    endif()
    # The Enhancements page's settings persist and ORIGINAL resets them (lane C7): the
    # page's code against a stand-in host, two runs (a fresh start). Windows: under Wine.
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
      add_executable(native-enhancements-settings tests/enhancements_settings.cpp platform/sdl/enhancements.cpp)
      if(WIN32)
        target_link_libraries(native-enhancements-settings PRIVATE SDL3::SDL3 ra2_port)   # host_compat.h's headers
        target_link_options(native-enhancements-settings PRIVATE -static)
      endif()
      add_test(NAME native-enhancements-settings COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/enhancements_settings.py"
               --binary $<TARGET_FILE:native-enhancements-settings> --work "${CMAKE_CURRENT_BINARY_DIR}/enhancements-settings"
               ${ra2_wine_args_settings})
      set_tests_properties(native-enhancements-settings PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 300)
    endif()
    if(RA2_NATIVE_SUBSYSTEM STREQUAL "all")
      # The game: SDL3 host + every recovered object (initialiser order) +
      # native data + the platform seams.
      set(ra2_game_inputs ${ra2_host_sources}
          $<TARGET_OBJECTS:ra2_recovered_objs> $<TARGET_OBJECTS:ra2_data>)
      file(GENERATE OUTPUT "${RA2_NATIVE_GEN}/link_inputs.txt"
           CONTENT "$<JOIN:$<TARGET_OBJECTS:ra2_recovered_objs>;$<TARGET_OBJECTS:ra2_data>;$<TARGET_FILE:ra2_port>,\n>\n")
      if(RA2_NATIVE_LINK_CLOSURE)
        # Undefined symbols after the real inputs, as trap functions and zero
        # placeholders (tools/link_closure.py). The JSON report is the work
        # queue: every entry is unrecovered code or unsourced data.
        add_library(ra2_host_objs OBJECT ${ra2_host_sources})
        target_link_libraries(ra2_host_objs PRIVATE SDL3::SDL3 ra2_port)
        target_compile_definitions(ra2_host_objs PRIVATE RA2_NATIVE_GAME=1)
        add_custom_command(
          OUTPUT "${RA2_NATIVE_GEN}/link_closure.c" "${RA2_NATIVE_GEN}/link_closure.json" "${RA2_NATIVE_GEN}/alias_list.txt"
          COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/link_closure.py"
                  --inputs "${RA2_NATIVE_GEN}/link_inputs.txt" --host $<TARGET_OBJECTS:ra2_host_objs>
                  --out "${RA2_NATIVE_GEN}/link_closure.c"
                  --aliases "${CMAKE_CURRENT_SOURCE_DIR}/data/address_aliases.tsv" "${CMAKE_CURRENT_SOURCE_DIR}/data/bss_aliases.tsv"
                  --alias-list "${RA2_NATIVE_GEN}/alias_list.txt"
                  ${ra2_closure_target_args}
                  --system-libs ${RA2_NATIVE_CLOSURE_SYSTEM_LIBS}
          DEPENDS ra2_recovered_objs ra2_data ra2_port ra2_host_objs "${CMAKE_CURRENT_SOURCE_DIR}/tools/link_closure.py" ${RA2_NATIVE_CLOSURE_SYSTEM_LIBS}
                  "${CMAKE_CURRENT_SOURCE_DIR}/data/address_aliases.tsv"
                  $<TARGET_OBJECTS:ra2_data> $<TARGET_FILE:ra2_port> $<TARGET_OBJECTS:ra2_host_objs>
                  "${RA2_NATIVE_GEN}/units.cmake"
          COMMAND_EXPAND_LISTS VERBATIM)
        set(ra2_game_inputs $<TARGET_OBJECTS:ra2_host_objs> $<TARGET_OBJECTS:ra2_recovered_objs>
            $<TARGET_OBJECTS:ra2_data> "${RA2_NATIVE_GEN}/link_closure.c")
      endif()
      add_executable(ra2-native ${ra2_game_inputs})
      target_link_libraries(ra2-native PRIVATE SDL3::SDL3 ra2_port ${ra2_host_frameworks})
      if(RA2_NATIVE_LINK_CLOSURE AND NOT WIN32)
        # One original address, several names: ld64 aliases (data/address_aliases.tsv).
        # (Windows: -alternatename directives in link_closure.c.)
        target_link_options(ra2-native PRIVATE "LINKER:-alias_list,${RA2_NATIVE_GEN}/alias_list.txt")
        set_property(TARGET ra2-native APPEND PROPERTY LINK_DEPENDS "${RA2_NATIVE_GEN}/alias_list.txt")
      endif()
      target_compile_definitions(ra2-native PRIVATE RA2_NATIVE_GAME=1)
      if(WIN32)
        # Select-any for repeated definitions there: cmake/windows.cmake. The players'
        # GUI-subsystem RA2Native.exe from the same objects (lane C15d).
        ra2_windows_game_link(ra2-native)
        ra2_windows_gui_exe("${ra2_game_inputs}" "SDL3::SDL3;ra2_port;${ra2_host_frameworks}")
      else()
        # Repeated definitions across units get VC6's select-any semantics.
        add_custom_command(TARGET ra2-native PRE_LINK
          COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/weaken_duplicates.py"
                  --inputs "${RA2_NATIVE_GEN}/link_inputs.txt" --report "${RA2_NATIVE_GEN}/duplicates.json"
          VERBATIM)
      endif()
      if(CMAKE_SYSTEM_NAME STREQUAL "iOS")
        # The iOS app (build-ios.command, Scout1247): RA2.app with the executable, its
        # Info.plist and the SDL3 licence; never game data (packaging/audit_ios_bundle.py).
        # The team id for a device build comes from the command line, never from Git.
        set(RA2_IOS_TEAM "" CACHE STRING "Apple Development team id (device builds only; never in Git)")
        set(RA2_IOS_BUILD "1" CACHE STRING "CFBundleVersion of the iOS app")
        # One iOS app bundle (lane Y7: also yr-native's, native/yr/yr.cmake): TARGET's executable
        # named EXE in EXE.app, bundle id ID, home-screen name NAME, icon drawn by make_icon.py
        # with ICON_ARGS. Files in the build folder's ios/ (RA2) or ios-<exe>/ (others).
        function(ra2_ios_app target exe id name)
          set(icon_args ${ARGN})
          if(CMAKE_OSX_SYSROOT MATCHES "[Ss]imulator")
            set(RA2_IOS_PLATFORM iPhoneSimulator)
            set(RA2_IOS_PLATFORM_NAME iphonesimulator)
          else()
            set(RA2_IOS_PLATFORM iPhoneOS)
            set(RA2_IOS_PLATFORM_NAME iphoneos)
          endif()
          execute_process(COMMAND xcrun --sdk ${RA2_IOS_PLATFORM_NAME} --show-sdk-version
                          OUTPUT_VARIABLE ra2_ios_sdk_version OUTPUT_STRIP_TRAILING_WHITESPACE)
          set(RA2_IOS_SDK_NAME "${RA2_IOS_PLATFORM_NAME}${ra2_ios_sdk_version}")
          set(RA2_IOS_APP_NAME "${name}")
          if(exe STREQUAL "RA2")
            set(dir "${CMAKE_CURRENT_BINARY_DIR}/ios")
          else()
            set(dir "${CMAKE_CURRENT_BINARY_DIR}/ios-${exe}")
          endif()
          configure_file("${CMAKE_CURRENT_SOURCE_DIR}/packaging/ios/Info.plist.in" "${dir}/Info.plist.in" @ONLY)
          set(sdl_licence "${RA2_NATIVE_SDL_SOURCE}/LICENSE.txt")
          target_sources(${target} PRIVATE "${sdl_licence}")
          set_source_files_properties("${sdl_licence}" PROPERTIES MACOSX_PACKAGE_LOCATION "licenses")
          set_target_properties(${target} PROPERTIES
            MACOSX_BUNDLE TRUE
            OUTPUT_NAME ${exe}
            MACOSX_BUNDLE_INFO_PLIST "${dir}/Info.plist.in"
            MACOSX_BUNDLE_EXECUTABLE_NAME ${exe}
            MACOSX_BUNDLE_GUI_IDENTIFIER "${id}"
            MACOSX_BUNDLE_BUNDLE_VERSION "${RA2_IOS_BUILD}"
            MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}")
          # The home-screen icon (lane I): make_icon.py's plain-shape placeholder drawn full
          # bleed (iOS masks it), at the sizes Info.plist's CFBundleIcons names, copied into
          # the bundle's root after each link (build-ios.command signs afterwards).
          set(icon_dir "${dir}/icons")
          set(icons AppIcon60x60@2x.png:120 AppIcon60x60@3x.png:180
              AppIcon76x76@2x~ipad.png:152 AppIcon83.5x83.5@2x~ipad.png:167)
          set(icon_commands)
          set(icon_files)
          foreach(icon IN LISTS icons)
            string(REPLACE ":" ";" icon "${icon}")
            list(GET icon 0 icon_name)
            list(GET icon 1 icon_size)
            list(APPEND icon_commands COMMAND sips -z ${icon_size} ${icon_size}
                 "${icon_dir}/icon.png" --out "${icon_dir}/${icon_name}")
            list(APPEND icon_files "${icon_dir}/${icon_name}")
          endforeach()
          add_custom_command(OUTPUT ${icon_files}
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${icon_dir}"
            COMMAND "${Python3_EXECUTABLE}" -I "${CMAKE_CURRENT_SOURCE_DIR}/packaging/make_icon.py"
                    --full-bleed ${icon_args} "${icon_dir}/icon.png"
            ${icon_commands}
            DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/packaging/make_icon.py"
            COMMENT "iOS app icon (${exe})" VERBATIM)
          if(exe STREQUAL "RA2")
            add_custom_target(ra2-ios-icons DEPENDS ${icon_files})
            add_dependencies(${target} ra2-ios-icons)
          else()
            add_custom_target(${target}-ios-icons DEPENDS ${icon_files})
            add_dependencies(${target} ${target}-ios-icons)
          endif()
          add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy ${icon_files} "$<TARGET_BUNDLE_DIR:${target}>"
            VERBATIM)
        endfunction()
        ra2_ios_app(ra2-native RA2 "io.github.ra2native.RA2" "Red Alert 2")
      endif()
      # 8. Native census (Scout1209): the game to 600 skirmish frames with all
      #    harness orders; any trap, fault or unrecovered call fails it (the
      #    smoke test only opens the window). Needs the installed game: set the
      #    two cache variables (or the environment variables of the same names),
      #    otherwise ctest reports it skipped.
      set(RA2_NATIVE_CENSUS_DATA_DIR "$ENV{RA2_NATIVE_CENSUS_DATA_DIR}" CACHE PATH "Installed Red Alert 2 folder for the native-census test (read only)")
      set(RA2_NATIVE_CENSUS_GAME_EXE "$ENV{RA2_NATIVE_CENSUS_GAME_EXE}" CACHE FILEPATH "game.exe for the native-census test")
      add_test(NAME native-census COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/census.py"
               --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
               --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --map "#5" --frames 600 --seconds 420
               --log "${CMAKE_CURRENT_BINARY_DIR}/native-census.log" ${ra2_wine_args_census})
      # The run's own limit (420 s; about 40 s alone) ends it before ctest's timeout, so a
      # stalled run still prints its milestones and the main thread's backtrace (C5).
      set_tests_properties(native-census PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 480)
      # 8a'. Campaign chains (C16): each side's new campaign through the menus, its twelve missions
      #      won in turn (RA2_NATIVE_FORCE_END) through their score screens, the ending and the main
      #      menu again (tests/campaign_chain.py; about 7 minutes a side alone).
      foreach(ra2_campaign_side allied soviet)
        add_test(NAME native-campaign-${ra2_campaign_side} COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/campaign_chain.py"
                 --side ${ra2_campaign_side} --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                 --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --log "${CMAKE_CURRENT_BINARY_DIR}/native-campaign-${ra2_campaign_side}.log"
                 ${ra2_wine_args_campaign${ra2_campaign_side}})
        set_tests_properties(native-campaign-${ra2_campaign_side} PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 1500)
      endforeach()
      # 8f. Pointers moved by negative unsigned 32-bit offsets (C17 pitch sweep; codex4 R749/R751 SAVE0029):
      #     link/selftest_draw.cpp in the game binary; under Wine on the Windows build.
      if(WIN32)
        if(RA2_NATIVE_WINE)
          add_test(NAME native-draw-selftest COMMAND "${CMAKE_COMMAND}" -E env RA2_NATIVE_DRAW_SELFTEST=1
                   "WINEPREFIX=${CMAKE_CURRENT_BINARY_DIR}/wine/draw" WINEDEBUG=-all
                   "${RA2_NATIVE_WINE}" $<TARGET_FILE:ra2-native>)
        endif()
      elseif(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
        add_test(NAME native-draw-selftest COMMAND "${CMAKE_COMMAND}" -E env RA2_NATIVE_DRAW_SELFTEST=1 $<TARGET_FILE:ra2-native>)
      endif()
      # 8g. The VQA player's path around a mod's .VQA movie (C16): the library's default
      #     configuration from game.exe and VQA_Reset, in the game binary (link/selftest_vqa.cpp).
      if(NOT WIN32 AND NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
        add_test(NAME native-vqa-selftest COMMAND "${CMAKE_COMMAND}" -E env RA2_NATIVE_VQA_SELFTEST=1
                 "RA2_GAME_EXE=${RA2_NATIVE_CENSUS_GAME_EXE}" $<TARGET_FILE:ra2-native>)
        set_tests_properties(native-vqa-selftest PROPERTIES SKIP_RETURN_CODE 77)
      endif()
      # 8h. No game process and no crash dialog outlives a test run (lane C19): the harnesses'
      #     launcher (tests/game_process.py) ends the game's whole process group, and a scripted
      #     run's crash is logged without the crash dialog (tests/process_cleanup.py).
      if(NOT WIN32 AND NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
        add_test(NAME native-process-cleanup COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/process_cleanup.py"
                 --binary $<TARGET_FILE:ra2-native> --work "${CMAKE_CURRENT_BINARY_DIR}/process-cleanup")
        set_tests_properties(native-process-cleanup PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 300)
      endif()
      # 8i. Paths the coverage runs found unreached by the other tests (lane C21,
      #     tests/coverage_runs.py): the lobby name edit's character filter (NewEdit 0x005F22A0),
      #     the Options page's check boxes reaching RA2.INI (Main_Options_Proc 0x00542590), and the
      #     in-game commands with the diplomacy screen (tests/shell_paths.py).
      if(NOT WIN32 AND NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
        foreach(ra2_shell_case name-filter options ingame-keys)
          add_test(NAME native-shell-${ra2_shell_case} COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/shell_paths.py"
                   --case ${ra2_shell_case} --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                   --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/shell-${ra2_shell_case}")
          set_tests_properties(native-shell-${ra2_shell_case} PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 400)
        endforeach()
      endif()
      # 8e. A loaded game abandoned and the game ended by the menus' Exit (C15f, codex4
      #     R710/R719): WinMain and the exit-time teardown end cleanly (tests/exit_route.py).
      add_test(NAME native-exit-route COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/exit_route.py"
               --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
               --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/exit-route"
               ${ra2_wine_args_exitroute})
      set_tests_properties(native-exit-route PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 1200)
      # 8c. The players' RA2Native.exe (C15d): GUI subsystem, our icon, the app by itself
      #     under Wine with its log in the prefix's AppData (tests/windows_gui.py).
      if(WIN32 AND RA2_WINDOWS_GUI)
        add_test(NAME native-windows-gui COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/windows_gui.py"
                 --gui $<TARGET_FILE:RA2Native> --console $<TARGET_FILE:ra2-native>
                 --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}" --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}"
                 --work "${CMAKE_CURRENT_BINARY_DIR}/windows-gui" ${ra2_wine_args_gui})
        set_tests_properties(native-windows-gui PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 900)
      endif()
      # 8d. A truncated save is refused with the game's error box and the menus stay
      #     usable; the untruncated save loads (C15e, codex4 R684/R714; tests/truncated_save.py).
      add_test(NAME native-truncated-save COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/truncated_save.py"
               --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
               --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/truncated-save"
               ${ra2_wine_args_truncated})
      set_tests_properties(native-truncated-save PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 1200)
      # 8b. Original saves (C6, C15): one of the user's saves from the original game
      #     (Allied 1, in the installation) loads through the menus (--load-save) with
      #     the field maps and plays 300 frames; on Windows the .exe under Wine.
      if(RA2_NATIVE_SUBSYSTEM STREQUAL "all" AND ((RA2_NATIVE_ARM64 AND NOT WIN32) OR WIN32))
        add_test(NAME native-original-save COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/original_save_load.py"
                 --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                 --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --save ALL01T.sav --frames 300 --seconds 240
                 --log "${CMAKE_CURRENT_BINARY_DIR}/native-original-save.log" ${ra2_wine_args_original})
        # Room for the Wine host retries (tests/wine.py host_failure: at most two more launches).
        set_tests_properties(native-original-save PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 900)
        # 8b'. Writing the original's format (C17; the only format written since C20): a fresh
        #      skirmish and each of the user's five saves, saved that way, load back and play.
        add_test(NAME native-original-save-write COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/original_save_write.py"
                 --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                 --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/original-save-write"
                 --save ALL01T.sav --save ALL02S.sav --save ALL03U.sav --save SAVE397A.SAV --save SAVE4FBD.SAV
                 ${ra2_wine_args_originalwrite})
        set_tests_properties(native-original-save-write PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 2400)
        # 8b'''. TRANSITION (C20; remove in release N+1 with the port container reader): a port
        #        save made at test time (test hook RA2_NATIVE_TEST_PORT_SAVE) loads, is saved
        #        again in the original's format, and that save loads and plays.
        add_test(NAME native-port-save-transition COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/original_save_write.py"
                 --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                 --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/port-save-transition"
                 --port-transition ${ra2_wine_args_porttransition})
        set_tests_properties(native-port-save-transition PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 900)
        # 8b''. The round trip (C17 stage E): each of the user's saves loaded and saved again at
        #       once in the original's format, the decoded streams compared (original_save_compare.py);
        #       fails on a differing vtable pointer or indexes that never meet again.
        add_test(NAME native-original-save-roundtrip COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/original_save_roundtrip.py"
                 --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                 --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --members "${RA2_NATIVE_GEN}/original_layouts_members.json"
                 --work "${CMAKE_CURRENT_BINARY_DIR}/original-save-roundtrip"
                 --save ALL01T.sav --save ALL02S.sav --save ALL03U.sav --save SAVE397A.SAV --save SAVE4FBD.SAV
                 ${ra2_wine_args_roundtrip})
        set_tests_properties(native-original-save-roundtrip PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 2400)
      endif()
      # 9. Plain-virtual views against the real classes' arm64 vtables (Scout1288,
      #    tools/vtable_views.py, data/vtable_views.tsv): every view classified, and each
      #    slot the sources call through a view reaches the real class's entry for that
      #    VC6 slot (clang vtable dumps of the built units). Needs the linked ra2-native.
      add_test(NAME native-vtable-views COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tools/vtable_views.py"
               --build "${CMAKE_CURRENT_BINARY_DIR}")
      set_tests_properties(native-vtable-views PROPERTIES TIMEOUT 600)
      # 10. Link-closure baseline (C3): the undefined set of the link closure may not
      #     grow past data/link_closure_baseline.json (total, per-kind counts and the
      #     symbols; the failure names each new symbol). The baseline is the Mac game
      #     link's, so the iOS build skips it. A self-test proves the check fails on a
      #     grown set.
      if(RA2_NATIVE_LINK_CLOSURE AND NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
        add_test(NAME native-link-closure COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/link_closure_baseline.py"
                 --build "${CMAKE_CURRENT_BINARY_DIR}" ${ra2_closure_check_args})
        set_tests_properties(native-link-closure PROPERTIES SKIP_RETURN_CODE 77)
        add_test(NAME native-link-closure-selftest COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/link_closure_baseline.py"
                 --self-test)
      endif()
      # 11. Enhancements (C3): the ORIGINAL preset draws the default frames byte for byte
      #     outside the menu's animated pixels, and the ENHANCED preset does not (the check
      #     sees the scaling). Needs the installed game, as native-census.
      if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
        add_test(NAME native-enhancements-frames COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/enhancements_frames.py"
                 --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                 --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/enhancements-frames"
                 ${ra2_wine_args_frames})
        # Room for the Wine host retries (tests/wine.py host_failure), one launch hung at most.
        set_tests_properties(native-enhancements-frames PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 900)
        # A Game interface size accepted on the page applies to the next new game in the
        # same process (C7, codex4 R700): the app through the real menus.
        add_test(NAME native-enhancements-newgame COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/enhancements_newgame.py"
                 --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                 --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/enhancements-newgame"
                 ${ra2_wine_args_newgame})
        set_tests_properties(native-enhancements-newgame PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 600)
        # A Movies-menu movie at the iPhone Duo's width with the interface and map at 1.25x is
        # shown whole, and the main menu after a submenu and a focus change has its buttons (C11).
        # (The Mac app's settings folder; the Windows build is not run under Wine here.)
        if(NOT WIN32)
          add_test(NAME native-menu-movie COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/menu_movie.py"
                   --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                   --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/menu-movie")
          set_tests_properties(native-menu-movie PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 400)
        endif()
        # A Network (LAN) game between two instances on this Mac with no switch set (C17w: LAN on by
        # default): about five minutes of play with orders and reused control groups, the Sound
        # dialog opened twice in the game, equal per-frame CRCs and the edge-scroll rate against a
        # skirmish's (tests/lan_pair.py). The Mac app's settings folders; not under Wine here.
        if(NOT WIN32)
          add_test(NAME native-lan-pair COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/lan_pair.py"
                   --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                   --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/lan-pair")
          set_tests_properties(native-lan-pair PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 3600 RUN_SERIAL TRUE)
          # The LAN lobby (C20, the user's reports of 2026-11-16): chat lines sent with Return
          # arrive both ways, and the host's game is seen and joined, with either side unable to
          # broadcast as the iOS app is (RA2_NATIVE_TEST_NO_BROADCAST); the main menu's Internet
          # button is disabled and a click on it does nothing (tests/lan_lobby.py).
          add_test(NAME native-lan-lobby COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/lan_lobby.py"
                   --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                   --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/lan-lobby")
          set_tests_properties(native-lan-lobby PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 900 RUN_SERIAL TRUE)
        endif()
        # 12. Touch input (C8): scripted fingers with the touch controls on; a two-finger tap
        #     deselects; A-Move is a toggle: two taps give two attack-move orders, and after
        #     A-Move is turned off a tap is a plain move (C10); with a tank selected a tap on a
        #     second tank selects it, in Classic and Modern; in Allied mission 2 (the original's
        #     ALL02S.sav) a tap on Tanya, the Germans' under the player's control, selects her and a
        #     tap on a GI selects it again, a mouse click keeps the original's move (C18).
        foreach(ra2_touch_case two-finger attack-move modern second-classic second-modern cross-classic cross-modern cross-mouse)
          add_test(NAME native-touch-${ra2_touch_case} COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/touch_input.py"
                   --case ${ra2_touch_case} --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                   --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --log "${CMAKE_CURRENT_BINARY_DIR}/native-touch-${ra2_touch_case}.log")
          set_tests_properties(native-touch-${ra2_touch_case} PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 400)
        endforeach()
        # 12b. Control enhancements (C16): scripted clicks in a skirmish, checked by game state.
        #      Queue five with Shift on and off (the barracks' factory after Shift-clicks on the
        #      GI's build icon); Modern controls and the same clicks with Classic (the selection,
        #      the tank's mission, the MCV's deploy); on Windows the .exe under Wine.
        foreach(ra2_controls_case queue-five queue-five-off modern classic)
          add_test(NAME native-controls-${ra2_controls_case} COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/controls_input.py"
                   --case ${ra2_controls_case} --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                   --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --log "${CMAKE_CURRENT_BINARY_DIR}/native-controls-${ra2_controls_case}.log"
                   ${ra2_wine_args_controls})
          set_tests_properties(native-controls-${ra2_controls_case} PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 900)
        endforeach()
        # 12c. HD vehicles (C16): a skirmish with HD vehicles 2x and Map zoom 1.5x, units moving and
        #      turrets turning; every picture shows units at 2x and no 2x group disagrees with its
        #      1x pixel (tests/hd_vehicles.py).
        add_test(NAME native-hd-vehicles COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/hd_vehicles.py"
                 --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                 --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --log "${CMAKE_CURRENT_BINARY_DIR}/native-hd-vehicles.log"
                 ${ra2_wine_args_hdvox})
        set_tests_properties(native-hd-vehicles PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 900)
        add_test(NAME native-hd-vehicles-zoom2 COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/hd_vehicles.py"
                 --zoom 2 --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                 --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --log "${CMAKE_CURRENT_BINARY_DIR}/native-hd-vehicles-zoom2.log"
                 ${ra2_wine_args_hdvox})
        set_tests_properties(native-hd-vehicles-zoom2 PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 900)
        # 12c'. HD vehicles 3x (C17): the same skirmish at 3x (the 1x colour fill in the gaps the thirds
        #      leave), and the air-base case at 2x and 3x: a Patriot's voxel turret and two paradrops'
        #      cargo plane on screen, each kind shown at Nx (tests/hd_vehicles.py --case air-base).
        #      vehicles-3x-budget: 3x over a 0.5 ms frame budget drops to 2x for the rest of the game.
        foreach(ra2_hd_case IN ITEMS "vehicles-3x;units;3;0" "vehicles-3x-budget;units;3;0.5" "air-base;air-base;2;0"
                                     "air-base-3x;air-base;3;0")
          list(GET ra2_hd_case 0 ra2_hd_name)
          list(GET ra2_hd_case 1 ra2_hd_kind)
          list(GET ra2_hd_case 2 ra2_hd_scale)
          list(GET ra2_hd_case 3 ra2_hd_budget)
          add_test(NAME native-hd-${ra2_hd_name} COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/hd_vehicles.py"
                   --case ${ra2_hd_kind} --scale ${ra2_hd_scale} --budget-ms ${ra2_hd_budget} --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                   --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --log "${CMAKE_CURRENT_BINARY_DIR}/native-hd-${ra2_hd_name}.log"
                   ${ra2_wine_args_hdvox})
          set_tests_properties(native-hd-${ra2_hd_name} PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 900)
        endforeach()
        # 12e. Zoom gestures (C16): scripted zoom steps (the wheel's path) with the window mapping checked
        #      both ways, clicks on given tanks at each zoom and the sidebar left in place; a scripted
        #      two-finger pinch that zooms only; the in-game menu at render scale 2, clicked through the
        #      window (tests/zoom_input.py).
        foreach(ra2_zoom_case steps menu menu-ui score abort pinch)
          add_test(NAME native-zoom-${ra2_zoom_case} COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/zoom_input.py"
                   --case ${ra2_zoom_case} --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                   --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --log "${CMAKE_CURRENT_BINARY_DIR}/native-zoom-${ra2_zoom_case}.log")
          set_tests_properties(native-zoom-${ra2_zoom_case} PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 400)
        endforeach()
        # 12d. Mods (C16, level 1): a mod made at test time (the game's rules with the Grizzly's
        #      cost changed) is read ahead of the installation; without a mod nothing changes.
        add_test(NAME native-mod-rules COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/mod_rules.py"
                 --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                 --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/mod-rules" ${ra2_wine_args_mods})
        set_tests_properties(native-mod-rules PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 600)
        #      Level 2: the Enhancements page's Mod row chooses a mod (and None again) across app
        #      starts, each mod's writes in their own folder (tests/mod_switch.py).
        add_test(NAME native-mod-switch COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/mod_switch.py"
                 --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                 --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/mod-switch" ${ra2_wine_args_modswitch})
        set_tests_properties(native-mod-switch PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 1000)
        # 13. Touch controls (C10): the app's movies, menus and a game with the touch controls on;
        #     Skip alone during a movie, the Keys button alone in the menus (its tap turns the
        #     keyboard on), the toolbar alone in play.
        add_test(NAME native-touch-controls COMMAND "${Python3_EXECUTABLE}" -B "${CMAKE_CURRENT_SOURCE_DIR}/tests/touch_controls.py"
                 --binary $<TARGET_FILE:ra2-native> --data-dir "${RA2_NATIVE_CENSUS_DATA_DIR}"
                 --game-exe "${RA2_NATIVE_CENSUS_GAME_EXE}" --work "${CMAKE_CURRENT_BINARY_DIR}/touch-controls")
        set_tests_properties(native-touch-controls PROPERTIES SKIP_RETURN_CODE 77 TIMEOUT 400)
      endif()
      # The native Yuri's Revenge game (lane Y1, -DRA2_NATIVE_YR=ON, Mac; lane Y7: also iOS,
      # build-ios.command --yr): native/yr/yr.cmake.
      option(RA2_NATIVE_YR "Also build yr-native, the Yuri's Revenge game from the pinned ra2-decomp tree (yr/)" OFF)
      if(RA2_NATIVE_YR AND APPLE)
        include("${CMAKE_CURRENT_SOURCE_DIR}/yr/yr.cmake")
      endif()
    else()
      add_executable(ra2-native platform/sdl/main.cpp)
      target_link_libraries(ra2-native PRIVATE SDL3::SDL3 ra2_port)
    endif()
  else()
    message(STATUS "SDL3 not found: ra2-native host skipped")
  endif()
endif()
