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

# Non-system libraries (SDL3 from Homebrew) into Frameworks, rewired to the bundle.
otool -L "$binary" | tail -n +2 | awk '{print $1}' | while read -r lib; do
    case "$lib" in
    /usr/lib/*|/System/*|@*) continue ;;
    esac
    name=$(basename "$lib")
    /bin/cp -f "$lib" "$contents/Frameworks/$name"
    chmod 644 "$contents/Frameworks/$name"
    install_name_tool -id "@rpath/$name" "$contents/Frameworks/$name"
    install_name_tool -change "$lib" "@executable_path/../Frameworks/$name" "$contents/MacOS/RA2"
    # The library's licence beside it (SDL3: zlib).
    prefix=$(cd "$(dirname "$lib")/.." && pwd)
    for licence in LICENSE.txt LICENSE COPYING.txt; do
        if [ -f "$prefix/$licence" ]; then
            /bin/cp -f "$prefix/$licence" "$contents/Resources/${name%%.*}-$licence"
            break
        fi
    done
done
if otool -L "$contents/MacOS/RA2" | tail -n +2 | awk '{print $1}' | grep -v -e '^/usr/lib/' -e '^/System/' -e '^@executable_path/' >/dev/null; then
    echo "make_app: unbundled library left:" >&2
    otool -L "$contents/MacOS/RA2" >&2
    exit 1
fi

# Never game data or a game executable in the bundle.
if find "$app" -type f \( -iname '*.mix' -o -iname '*.exe' -o -iname '*.mmx' -o -iname '*.tlb' -o -iname '*.dll' -o -iname '*.ini' \) | grep -q .; then
    echo "make_app: game files found in the bundle" >&2
    exit 1
fi

# Ad-hoc signature (this Mac only): the libraries first, then the app.
for lib in "$contents/Frameworks"/*; do
    [ -f "$lib" ] && codesign --force --sign - "$lib"
done
codesign --force --sign - --identifier io.github.ra2native.RA2 "$app"
codesign --verify --strict --deep "$app"
echo "make_app: $app"
