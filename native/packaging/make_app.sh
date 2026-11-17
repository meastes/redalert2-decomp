#!/bin/sh
# SPDX-License-Identifier: GPL-3.0-or-later
# Packages a built ra2-native as RA2.app (README.md in this folder).
#   native/packaging/make_app.sh <path/to/ra2-native> [<output folder>]
# The bundle holds the executable, the SDL3 library it links (zlib licence,
# with its LICENSE), a generated placeholder icon and Info.plist; it is signed
# ad hoc for this Mac only (no notarisation, never published). It never holds
# game data or a game.exe: the app asks for the installed game on first run.
set -eu

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
    echo "usage: $0 <path/to/ra2-native> [<output folder>]" >&2
    exit 64
fi
binary=$1
out=${2:-$(dirname "$binary")}
here=$(cd "$(dirname "$0")" && pwd)
[ -x "$binary" ] || { echo "make_app: $binary is not an executable" >&2; exit 1; }
# Only the game build (RA2_NATIVE_SUBSYSTEM=all) is an app.
if ! nm "$binary" 2>/dev/null | grep -q ' T _WinMain$'; then
    echo "make_app: $binary has no WinMain: configure with -DRA2_NATIVE_SUBSYSTEM=all" >&2
    exit 1
fi

app=$out/RA2.app
contents=$app/Contents
work=$(mktemp -d "${TMPDIR:-/tmp}/ra2-app.XXXXXX")
trap 'rm -rf "$work"' EXIT

rm -rf "$app"
mkdir -p "$contents/MacOS" "$contents/Resources" "$contents/Frameworks"
/bin/cp -f "$binary" "$contents/MacOS/RA2"
chmod 755 "$contents/MacOS/RA2"

# Info.plist: the version from git when the source is a checkout.
version=0.1
build=$(git -C "$here" rev-parse --short HEAD 2>/dev/null || echo local)
minos=$(otool -l "$binary" | awk '/LC_BUILD_VERSION/{f=1} f&&/minos/{print $2; exit}')
[ -n "$minos" ] || minos=13.0
sed -e "s/@VERSION@/$version/" -e "s/@BUILD@/$build/" -e "s/@MINOS@/$minos/" \
    "$here/Info.plist.in" > "$contents/Info.plist"
plutil -lint "$contents/Info.plist" >/dev/null
printf 'APPL????' > "$contents/PkgInfo"

# The icon (make_icon.py: plain shapes) at every size the iconset takes.
python3 -I "$here/make_icon.py" "$work/icon.png"
iconset=$work/RA2.iconset
mkdir -p "$iconset"
for size in 16 32 128 256 512; do
    sips -z $size $size "$work/icon.png" --out "$iconset/icon_${size}x${size}.png" >/dev/null
    double=$((size * 2))
    sips -z $double $double "$work/icon.png" --out "$iconset/icon_${size}x${size}@2x.png" >/dev/null
done
iconutil -c icns "$iconset" -o "$contents/Resources/RA2.icns"

# Non-system libraries (SDL3 from Homebrew) into Frameworks, rewired to the bundle,
# with the non-system libraries they link in turn (Scout1247). A build that links
# SDL3 statically (as the iOS build does) has none: Frameworks is then left out.
non_system() {
    otool -L "$1" | tail -n +2 | awk '{print $1}' | while read -r dep; do
        case "$dep" in
        /usr/lib/*|/System/*|@*) ;;
        *) echo "$dep" ;;
        esac
    done
}
bundle_lib() {
    lib=$1
    name=$(basename "$lib")
    [ -f "$contents/Frameworks/$name" ] && return 0
    /bin/cp -f "$lib" "$contents/Frameworks/$name"
    chmod 644 "$contents/Frameworks/$name"
    install_name_tool -id "@rpath/$name" "$contents/Frameworks/$name"
    # The library's licence beside it (SDL3: zlib).
    prefix=$(cd "$(dirname "$lib")/.." && pwd)
    for licence in LICENSE.txt LICENSE COPYING.txt; do
        if [ -f "$prefix/$licence" ]; then
            /bin/cp -f "$prefix/$licence" "$contents/Resources/${name%%.*}-$licence"
            break
        fi
    done
    for dep in $(non_system "$lib"); do
        bundle_lib "$dep"
        install_name_tool -change "$dep" "@loader_path/$(basename "$dep")" "$contents/Frameworks/$name"
    done
}
for lib in $(non_system "$binary"); do
    bundle_lib "$lib"
    install_name_tool -change "$lib" "@executable_path/../Frameworks/$(basename "$lib")" "$contents/MacOS/RA2"
done
for file in "$contents/MacOS/RA2" "$contents/Frameworks"/*; do
    [ -f "$file" ] || continue
    if [ -n "$(non_system "$file")" ]; then
        echo "make_app: unbundled library left in $file:" >&2
        otool -L "$file" >&2
        exit 1
    fi
done
rmdir "$contents/Frameworks" 2>/dev/null || true

# Never game data or a game executable in the bundle.
if find "$app" -type f \( -iname '*.mix' -o -iname '*.exe' -o -iname '*.mmx' -o -iname '*.tlb' -o -iname '*.dll' -o -iname '*.ini' \) | grep -q .; then
    echo "make_app: game files found in the bundle" >&2
    exit 1
fi

# Ad-hoc signature (this Mac only): the libraries first, then the app.
for lib in "$contents/Frameworks"/*; do
    if [ -f "$lib" ]; then codesign --force --sign - "$lib"; fi
done
codesign --force --sign - --identifier io.github.ra2native.RA2 "$app"
codesign --verify --strict --deep "$app"
echo "make_app: $app"
