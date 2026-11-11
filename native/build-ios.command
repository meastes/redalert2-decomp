#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-or-later
# The iOS app (README "iOS"; Scout1247), in the shape of Outpost 2's
# build-ios.command: CMake for CMAKE_SYSTEM_NAME=iOS, arm64, SDL3 3.4.2 built
# statically from source, the same native tree as the Mac app.
#   native/build-ios.command [simulator|device] [CMake options]
# Xcode is used through DEVELOPER_DIR for this command only (never xcode-select,
# never a licence acceptance). The Simulator build is signed ad hoc. A device
# build is signed with the user's team id in RA2_IOS_TEAM, given on the command
# line and never stored in Git (packaging/ios_sign.py: a development profile and
# identity Xcode already made; none is created here). Install with
#   xcrun devicectl device install app --device <udid> build/ios-device/RA2.app
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
  *) echo 'usage: native/build-ios.command [simulator|device] [CMake options]' >&2; exit 2 ;;
esac
build="${RA2_IOS_BUILD_DIR:-build/ios-$platform}"

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
cmake "${config[@]}" "$@"
cmake --build "$build" --target ra2-native
app="$build/RA2.app"
if [[ "$platform" == simulator ]]; then
  codesign --force --sign - --timestamp=none "$app"
elif [[ -n "${RA2_IOS_TEAM:-}" ]]; then
  # The user's own device: a development profile of the team and its identity
  # (packaging/ios_sign.py; RA2_IOS_PROFILE names another profile).
  python3 -I "$here/packaging/ios_sign.py" "$app" --team "$RA2_IOS_TEAM" ${RA2_IOS_PROFILE:+--profile "$RA2_IOS_PROFILE"}
else
  echo "build-ios: device build left unsigned (set RA2_IOS_TEAM on the command line for the device pass)"
fi
python3 -I "$here/packaging/audit_ios_bundle.py" "$app" ${RA2_GAME_EXE:+--game-exe "$RA2_GAME_EXE"}
echo "Built: $app"
