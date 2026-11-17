#!/bin/bash
# SPDX-License-Identifier: GPL-3.0-or-later
# One command per platform for the native ports (native/README.md "Building the apps").
#
#   native/build.sh mac                 Red Alert 2 for the Mac:            build/release-mac/RA2.app
#   native/build.sh mac-one             RA2 + Yuri's Revenge for the Mac:   build/release-mac-one/RA2YR.app
#   native/build.sh ios [simulator|device]   the iPhone/iPad one-game app:  build/ios-one-<platform>/RA2YR.app
#   native/build.sh windows             Red Alert 2 for Windows (x86_64):   build/release-win/RA2Native.exe
#   native/build.sh all                 mac, mac-one, ios simulator and windows in turn
#
# Each configures its own build folder (Release where the platform has build types), builds the
# game, packages it and prints the result's path. Rerunning only rebuilds what changed. Extra
# arguments after the platform go to CMake's configure step. Nothing is downloaded or installed;
# no game file is read or bundled (the apps ask for the player's installed game at run time).
#
# Prerequisites (README "Building the apps"): a Mac on Apple silicon with Xcode (or its command
# line tools for mac/windows), CMake 3.20+, Ninja, Python 3; for mac and mac-one Homebrew's sdl3;
# for mac-one, ios and windows the llvm-mingw 20261020 macOS release extracted (RA2_LLVM_MINGW, or
# private/toolchains/llvm-mingw-20261020/ in this checkout); for ios and windows the SDL 3.4.2
# source (private/vendor/SDL3-3.4.2.tar.gz or RA2_SDL_SOURCE); for an iPhone/iPad device build
# RA2_IOS_TEAM=<your Apple team id> (see build-ios.command).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
root=$(cd "$here/.." && pwd)
cd "$root"

# private/ (toolchains, SDL source) lives in the main checkout; a git worktree under
# .claude/worktrees uses the main checkout's, as the toolchain file does.
private_dir() {
  local main="${root%/.claude/worktrees/*}"
  for r in "$root" "$main"; do
    if [[ -d "$r/private" ]]; then echo "$r/private"; return; fi
  done
  echo "$root/private"
}
PRIVATE=$(private_dir)
LLVM_MINGW_RELEASE=llvm-mingw-20261020-ucrt-macos-universal
SDL_VERSION=3.4.2
SDL_SHA256=ef39a2e3f9a8a78296c40da701967dd1b0d0d6e267e483863ce70f8a03b4050c

die() { echo "build.sh: $*" >&2; exit 1; }
need() { command -v "$1" >/dev/null 2>&1 || die "$1 not found: $2"; }

common_checks() {
  [[ "$(uname -s)" == Darwin ]] || die "the ports build on macOS (the Windows build is cross-compiled on the Mac)"
  need cmake "install CMake (brew install cmake)"
  need ninja "install Ninja (brew install ninja)"
  need python3 "install Python 3"
  need clang "install Xcode or its command line tools (xcode-select --install)"
}

llvm_mingw() {
  local d="${RA2_LLVM_MINGW:-$PRIVATE/toolchains/llvm-mingw-20261020/$LLVM_MINGW_RELEASE}"
  [[ -x "$d/bin/x86_64-w64-mingw32-clang" ]] || die "llvm-mingw not found: extract $LLVM_MINGW_RELEASE (github.com/mstorsjo/llvm-mingw, release 20261020) and set RA2_LLVM_MINGW to its folder"
  echo "$d"
}

sdl_source() {
  local d="${RA2_SDL_SOURCE:-$PRIVATE/vendor/SDL3-$SDL_VERSION}"
  if [[ ! -f "$d/CMakeLists.txt" ]]; then
    local tarball="$PRIVATE/vendor/SDL3-$SDL_VERSION.tar.gz"
    [[ -f "$tarball" ]] || die "no SDL $SDL_VERSION source: put SDL3-$SDL_VERSION.tar.gz (SHA-256 $SDL_SHA256, from libsdl.org) in $PRIVATE/vendor or set RA2_SDL_SOURCE"
    [[ "$(shasum -a 256 "$tarball" | awk '{print $1}')" == "$SDL_SHA256" ]] || die "$tarball is not SDL $SDL_VERSION (SHA-256 differs)"
    tar -xzf "$tarball" -C "$PRIVATE/vendor"
  fi
  (cd "$d" && pwd)
}

mac_sdl_check() {
  # The Mac apps link Homebrew's SDL3 dylib and bundle it (packaging/make_app.sh).
  if ! { command -v brew >/dev/null 2>&1 && brew --prefix sdl3 >/dev/null 2>&1 && [[ -d "$(brew --prefix sdl3)/lib" ]]; }; then
    [[ -n "${SDL3_DIR:-}" ]] || die "SDL3 not found: brew install sdl3 (or set SDL3_DIR to an SDL3 CMake package)"
  fi
}

build_mac() {
  common_checks; mac_sdl_check
  local b="${RA2_BUILD_DIR:-build/release-mac}"
  cmake -S native -B "$b" -G Ninja -DCMAKE_BUILD_TYPE=Release -DRA2_NATIVE_SUBSYSTEM=all \
        -DRA2_NATIVE_LINK_CLOSURE=ON "$@"
  cmake --build "$b" --target ra2-native
  sh native/packaging/make_app.sh "$b/ra2-native" "$b"
  echo "Built: $b/RA2.app"
}

build_mac_one() {
  common_checks; mac_sdl_check
  local mingw; mingw=$(llvm_mingw)
  local b="${RA2_BUILD_DIR:-build/release-mac-one}"
  cmake -S native -B "$b" -G Ninja -DCMAKE_BUILD_TYPE=Release -DRA2_NATIVE_SUBSYSTEM=all \
        -DRA2_NATIVE_LINK_CLOSURE=ON -DRA2_NATIVE_YR=ON -DRA2_NATIVE_COMBINED=ON \
        -DRA2_NATIVE_YR_MINGW="$mingw/generic-w64-mingw32/include" "$@"
  cmake --build "$b" --target ra2yr-native ra2-engine yr-engine
  sh native/packaging/make_app.sh --one-game "$b/ra2yr-native" "$b"
  echo "Built: $b/RA2YR.app"
}

build_ios() {
  common_checks
  local platform="${1:-simulator}"
  if [[ $# -gt 0 ]]; then shift; fi
  [[ "$platform" == simulator || "$platform" == device ]] || die "ios takes simulator or device"
  local mingw; mingw=$(llvm_mingw)
  local sdl; sdl=$(sdl_source)
  RA2_SDL_SOURCE="$sdl" bash native/build-ios.command "$platform" \
    -DRA2_NATIVE_YR_MINGW="$mingw/generic-w64-mingw32/include" "$@"
}

build_windows() {
  common_checks
  local mingw; mingw=$(llvm_mingw)
  local sdl; sdl=$(sdl_source)
  local b="${RA2_BUILD_DIR:-build/release-win}"
  RA2_LLVM_MINGW="$mingw" cmake -S native -B "$b" -G Ninja \
        -DCMAKE_TOOLCHAIN_FILE="$root/native/cmake/llvm-mingw-x86_64.cmake" -DRA2_LLVM_MINGW="$mingw" \
        -DRA2_NATIVE_SDL_SOURCE="$sdl" -DRA2_NATIVE_SUBSYSTEM=all -DRA2_NATIVE_LINK_CLOSURE=ON "$@"
  cmake --build "$b" --target RA2Native ra2-native   # ra2-native.exe: the console build the Wine tests run
  echo "Built: $b/RA2Native.exe (self-contained: copy it to the Windows PC on its own)"
}

what="${1:-}"
if [[ $# -gt 0 ]]; then shift; fi
case "$what" in
  mac) build_mac "$@" ;;
  mac-one) build_mac_one "$@" ;;
  ios) build_ios "$@" ;;
  windows) build_windows "$@" ;;
  all) build_mac; build_mac_one; build_ios simulator; build_windows ;;
  *) sed -n '3,9p' "$0" | sed 's/^# \{0,1\}//' >&2; exit 64 ;;
esac
