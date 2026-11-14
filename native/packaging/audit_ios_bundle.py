#!/usr/bin/env python3
"""Audit the iOS app bundle (build-ios.command; Scout1247): it holds only the
rebuilt game, its Info.plist, the SDL3 licence, the generated placeholder icon
(packaging/make_icon.py: plain shapes) and the signature; never game
data, a game executable or original code.

    python3 native/packaging/audit_ios_bundle.py build/ios-simulator/RA2.app [--game-exe <game.exe>]

Checks: an allowlist of the bundle's files; no symbolic links; no file with a
game-data or Windows-program extension; the executable is an arm64 Mach-O that
links no library outside the system (SDL3 is static). With --game-exe (the
user's own game.exe, read only, never stored), it also samples the original's
code (.text) and resources (.rsrc) in 64-byte pieces (those with at least 24
different byte values, every 1 KB) and requires that none of them appears in
the executable: the native game reads game.exe's data at run time and carries
none of its code. (.data/.rdata are not sampled: tables the native tree
generates at compile time, such as the CRC-32 and fast-math tables, are equal
to the original's by construction.)
"""
import argparse
import pathlib
import plistlib
import struct
import subprocess
import sys

REQUIRED = {"Info.plist", "RA2", "licenses/LICENSE.txt",
            "AppIcon60x60@2x.png", "AppIcon60x60@3x.png", "AppIcon76x76@2x~ipad.png", "AppIcon83.5x83.5@2x~ipad.png"}
OPTIONAL = {"PkgInfo", "_CodeSignature/CodeResources", "embedded.mobileprovision"}
GAME_EXTENSIONS = {".mix", ".mmx", ".exe", ".dll", ".tlb", ".ini", ".map", ".mpr", ".csf", ".pkt", ".bik",
                   ".vqa", ".shp", ".pal", ".aud", ".wav", ".sav", ".str", ".yrm", ".vxl", ".hva", ".tem", ".sno",
                   ".urb", ".des", ".lun", ".ubn", ".dsk"}


def pe_sections(data):
    pe = struct.unpack_from("<I", data, 0x3C)[0]
    count, opt = struct.unpack_from("<HH", data, pe + 6)[0], struct.unpack_from("<H", data, pe + 20)[0]
    table = pe + 24 + opt
    for i in range(count):
        name = data[table + 40 * i: table + 40 * i + 8].rstrip(b"\0").decode("latin-1")
        raw_size, raw_ptr = struct.unpack_from("<II", data, table + 40 * i + 16)
        yield name, data[raw_ptr: raw_ptr + raw_size]


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("bundle", type=pathlib.Path)
    ap.add_argument("--game-exe", type=pathlib.Path, help="the user's game.exe: sample its code and resources")
    args = ap.parse_args()
    bundle = args.bundle
    failures = []
    if any(p.is_symlink() for p in bundle.rglob("*")):
        failures.append("the bundle has symbolic links")
    files = {p.relative_to(bundle).as_posix() for p in bundle.rglob("*") if p.is_file()}
    if REQUIRED - files:
        failures.append("missing: %s" % sorted(REQUIRED - files))
    if files - REQUIRED - OPTIONAL:
        failures.append("not on the allowlist: %s" % sorted(files - REQUIRED - OPTIONAL))
    games = sorted(f for f in files if pathlib.PurePosixPath(f).suffix.lower() in GAME_EXTENSIONS)
    if games:
        failures.append("game files in the bundle: %s" % games)
    info = plistlib.loads((bundle / "Info.plist").read_bytes()) if "Info.plist" in files else {}
    if info.get("CFBundleExecutable") != "RA2" or info.get("CFBundleIdentifier") != "io.github.ra2native.RA2":
        failures.append("Info.plist names another executable or bundle id")
    exe = bundle / "RA2"
    if exe.is_file():
        kind = subprocess.run(["file", "-b", str(exe)], capture_output=True, text=True).stdout
        if "Mach-O 64-bit executable arm64" not in kind:
            failures.append("the executable is not an arm64 Mach-O: %s" % kind.strip())
        libs = subprocess.run(["otool", "-L", str(exe)], capture_output=True, text=True).stdout.splitlines()[1:]
        foreign = [l.split()[0] for l in libs if l.strip() and not l.split()[0].startswith(("/usr/lib/", "/System/"))]
        if foreign:
            failures.append("links libraries outside the system: %s" % foreign)
    sampled = found = 0
    if args.game_exe and exe.is_file():
        binary = exe.read_bytes()
        original = args.game_exe.read_bytes()
        for name, section in pe_sections(original):
            if name not in (".text", ".rsrc"):
                continue
            for at in range(0, len(section) - 64, 1024):
                piece = section[at: at + 64]
                if len(set(piece)) < 24:
                    continue
                sampled += 1
                if binary.find(piece) >= 0:
                    found += 1
        if not sampled:
            failures.append("no pieces of game.exe's code or resources could be sampled")
        elif found:
            failures.append("%d of %d sampled pieces of game.exe's code and resources are in the executable" % (found, sampled))
    if failures:
        sys.exit("iOS bundle audit FAILED:\n  " + "\n  ".join(failures))
    print("PASS iOS bundle audit: %d files (%s), SDK %s, no game files, no libraries outside the system%s" % (
        len(files), ", ".join(sorted(files)), info.get("DTSDKName", "?"),
        "; none of %d sampled pieces of game.exe's code and resources in the executable" % sampled if sampled else ""))


if __name__ == "__main__":
    main()
