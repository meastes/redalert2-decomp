#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-or-later
# The iOS app (README "iOS"; Scout1247), in the shape of Outpost 2's
# build-ios.command: CMake for CMAKE_SYSTEM_NAME=iOS, arm64, SDL3 3.4.2 built
# statically from source, the same native tree as the Mac app.
#   native/build-ios.command [simulator|device] [--ra2|--yr] [CMake options]
# The default (lane Y9): the one-game app, Red Alert 2 and Yuri's Revenge in one executable
# (native/yr/README.md "One game"), build/ios-one-<platform>/RA2YR.app. It replaces the Red Alert 2
# app: bundle id io.github.ra2native.RA2, home-screen name "Red Alert 2", so the player's saves,
# settings and imported Red Alert 2 files carry over (it asks only for Yuri's Revenge's files). It
# needs llvm-mingw's headers as --yr does. (--one names it too.)
# --ra2: the Red Alert 2 app on its own, build/ios-<platform>/RA2.app (same bundle id: kept until
# the owner retires it).
# --yr (lane Y7, native/yr/README.md "yr-native on iOS"): Yuri's Revenge's own app, YR.app
# (bundle id io.github.ra2native.YR), built from the pinned ra2-decomp tree as yr-native is on the
# Mac, in build/ios-yr-<platform>; it needs llvm-mingw's headers (RA2_NATIVE_YR_MINGW, found under
# private/toolchains by default). Signed and audited as RA2.app is.
# Xcode is used through DEVELOPER_DIR for this command only (never xcode-select,
# never a licence acceptance). The Simulator build is signed ad hoc. A device
# build is signed with the user's team id in RA2_IOS_TEAM, given on the command
# line and never stored in Git (packaging/ios_sign.py: a development profile and
# identity Xcode already made; none is created here). Install with
#   xcrun devicectl device install app --device <udid> build/ios-one-device/RA2YR.app   (default)
#   xcrun devicectl device install app --device <udid> build/ios-device/RA2.app         (--ra2)
#   xcrun devicectl device install app --device <udid> build/ios-yr-device/YR.app       (--yr)
#
# Ninja, not Xcode's generator: the link closure and the duplicate weakening
# read the objects' paths at build time (native/CMakeLists.txt), which Xcode's
# generator leaves as $(CURRENT_ARCH)/$(EFFECTIVE_PLATFORM_NAME) placeholders.
#
# Optimised with -O2 (RA2_IOS_OPT; Scout1248: the Mac tree at -O2 passes the census
# and the skirmish like the -O0 build); no build type, so no NDEBUG.
#
# SDL: RA2_SDL_SOURCE, else private/vendor/SDL3-3.4.2 (extracted from
# private/vendor/SDL3-3.4.2.tar.gz after its SHA-256 is checked). This script
# downloads nothing.
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$here/.." && pwd)
cd "$root"
export DEVELOPER_DIR="${DEVELOPER_DIR:-/Applications/Xcode.app/Contents/Developer}"
platform="${1:-simulator}"
if [[ $# -gt 0 ]]; then shift; fi
case "$platform" in
  simulator) sdk=iphonesimulator ;;
  device) sdk=iphoneos ;;
  *) echo 'usage: native/build-ios.command [simulator|device] [--ra2|--yr] [CMake options]' >&2; exit 2 ;;
esac
# The default: the one-game app (lane Y9).
game_options=(-DRA2_NATIVE_YR=ON -DRA2_NATIVE_COMBINED=ON -DRA2_NATIVE_ONE_EXECUTABLE=ON)
target=ra2yr-one
exe=RA2YR
bundle_id=io.github.ra2native.RA2
build_default="build/ios-one-$platform"
audit_exe="${RA2_GAME_EXE:-}"
if [[ "${1:-}" == --one ]]; then
  shift
elif [[ "${1:-}" == --ra2 ]]; then
  # The Red Alert 2 app on its own (kept until the owner retires it).
  shift
  game_options=()
  target=ra2-native
  exe=RA2
  bundle_id=io.github.ra2native.RA2
  build_default="build/ios-$platform"
elif [[ "${1:-}" == --yr ]]; then
  shift
  game_options=(-DRA2_NATIVE_YR=ON)
  target=yr-native
  exe=YR
  bundle_id=io.github.ra2native.YR
  build_default="build/ios-yr-$platform"
  audit_exe="${RA2_GAMEMD_EXE:-}"
fi
build="${RA2_IOS_BUILD_DIR:-$build_default}"

sdl_version=3.4.2
sdl_sha256=ef39a2e3f9a8a78296c40da701967dd1b0d0d6e267e483863ce70f8a03b4050c
sdl="${RA2_SDL_SOURCE:-private/vendor/SDL3-$sdl_version}"
if [[ ! -f "$sdl/CMakeLists.txt" ]]; then
  tarball="private/vendor/SDL3-$sdl_version.tar.gz"
  if [[ ! -f "$tarball" ]]; then
    echo "build-ios: no SDL $sdl_version source: put SDL3-$sdl_version.tar.gz (SHA-256 $sdl_sha256) in private/vendor or set RA2_SDL_SOURCE" >&2
    exit 1
  fi
  if [[ "$(shasum -a 256 "$tarball" | awk '{print $1}')" != "$sdl_sha256" ]]; then
    echo "build-ios: $tarball is not SDL $sdl_version (SHA-256 differs)" >&2
    exit 1
  fi
  tar -xzf "$tarball" -C private/vendor
fi
sdl=$(cd "$sdl" && pwd)

config=(-S native -B "$build" -G Ninja -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_SYSROOT="$sdk"
  -DCMAKE_OSX_ARCHITECTURES=arm64 -DCMAKE_OSX_DEPLOYMENT_TARGET="${RA2_IOS_MIN:-17.0}"
  -DRA2_NATIVE_SUBSYSTEM=all -DRA2_NATIVE_LINK_CLOSURE=ON -DRA2_NATIVE_SDL_SOURCE="$sdl"
  -DCMAKE_C_FLAGS="${RA2_IOS_OPT--O2}" -DCMAKE_CXX_FLAGS="${RA2_IOS_OPT--O2}" -DCMAKE_OBJCXX_FLAGS="${RA2_IOS_OPT--O2}")
cmake "${config[@]}" ${game_options[@]+"${game_options[@]}"} "$@"
cmake --build "$build" --target "$target"
app="$build/$exe.app"
if [[ "$platform" == simulator ]]; then
  codesign --force --sign - --timestamp=none "$app"
elif [[ -n "${RA2_IOS_TEAM:-}" ]]; then
  # The user's own device: a development profile of the team and its identity
  # (packaging/ios_sign.py; RA2_IOS_PROFILE names another profile).
  python3 -I "$here/packaging/ios_sign.py" "$app" --team "$RA2_IOS_TEAM" ${RA2_IOS_PROFILE:+--profile "$RA2_IOS_PROFILE"}
else
  echo "build-ios: device build left unsigned (set RA2_IOS_TEAM on the command line for the device pass)"
fi
python3 -I "$here/packaging/audit_ios_bundle.py" "$app" --executable "$exe" --bundle-id "$bundle_id" ${audit_exe:+--game-exe "$audit_exe"}
echo "Built: $app"
