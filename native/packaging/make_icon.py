#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
# A placeholder app icon for RA2.app, drawn here from plain shapes (no game or
# third-party art): a red rounded square with a white five-pointed star and a
# dark ring. Writes one 1024x1024 PNG; make_app.sh scales it into the iconset.
# --full-bleed (the iOS app, native/CMakeLists.txt): the red square fills the
# picture, opaque, since iOS draws its own rounded mask over an app icon.
# --yr (Yuri's Revenge's iOS app, lane Y7): the same shapes on purple.
# Standard library only.
import math
import struct
import sys
import zlib

SIZE = 1024
SS = 3  # supersamples per axis (antialiasing)


def star_points(cx, cy, outer, inner, n=5):
    pts = []
    for i in range(2 * n):
        r = outer if i % 2 == 0 else inner
        a = -math.pi / 2 + i * math.pi / n
        pts.append((cx + r * math.cos(a), cy + r * math.sin(a)))
    return pts


def inside_polygon(x, y, pts):
    c = False
    j = len(pts) - 1
    for i in range(len(pts)):
        xi, yi = pts[i]
        xj, yj = pts[j]
        if (yi > y) != (yj > y) and x < (xj - xi) * (y - yi) / (yj - yi) + xi:
            c = not c
        j = i
    return c


def inside_rounded(x, y, x0, y0, x1, y1, r):
    if x < x0 or x > x1 or y < y0 or y > y1:
        return False
    cx = min(max(x, x0 + r), x1 - r)
    cy = min(max(y, y0 + r), y1 - r)
    return (x - cx) ** 2 + (y - cy) ** 2 <= r * r


PURPLE = False   # --yr


def shade(x, y, star, full_bleed):
    # macOS icon grid: the body is about 824 of 1024, centred; iOS: all of it.
    m = 0 if full_bleed else 100
    if not full_bleed and not inside_rounded(x, y, m, m, SIZE - m, SIZE - m, 185):
        return (0, 0, 0, 0)
    d = math.hypot(x - SIZE / 2, y - SIZE / 2)
    if inside_polygon(x, y, star):
        return (250, 246, 238, 255)
    if 300 <= d <= 330:
        return (60, 8, 8, 255)
    # red, darker towards the bottom
    t = (y - m) / (SIZE - 2 * m)
    if PURPLE:
        return (int(120 - 45 * t), int(30 - 14 * t), int(170 - 60 * t), 255)
    return (int(200 - 70 * t), int(24 - 10 * t), int(20 - 8 * t), 255)


def render(n=SIZE, full_bleed=False):
    """The icon as an n x n PNG (the 1024 design scaled; n == SIZE draws it as designed)."""
    star = star_points(SIZE / 2, SIZE / 2 + 18, 270, 108)
    k = SIZE / n
    rows = []
    for py in range(n):
        row = bytearray([0])
        for px in range(n):
            acc = [0, 0, 0, 0]
            for sy in range(SS):
                for sx in range(SS):
                    c = shade((px + (sx + 0.5) / SS) * k, (py + (sy + 0.5) / SS) * k, star, full_bleed)
                    a = c[3]
                    acc[0] += c[0] * a
                    acc[1] += c[1] * a
                    acc[2] += c[2] * a
                    acc[3] += a
            a = acc[3]
            if a:
                row += bytes((acc[0] // a, acc[1] // a, acc[2] // a, a // (SS * SS)))
            else:
                row += b"\0\0\0\0"
        rows.append(bytes(row))
    raw = b"".join(rows)

    def chunk(tag, data):
        return struct.pack(">I", len(data)) + tag + data + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)

    png = b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", n, n, 8, 6, 0, 0, 0))
    png += chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b"")
    return png


def main(out, full_bleed=False):
    with open(out, "wb") as f:
        f.write(render(SIZE, full_bleed))


# The Windows exe's file icon (lane C15d, cmake/windows.cmake): an .ico of PNG pictures
# (Windows Vista and later read PNG entries) at the sizes Explorer and the taskbar use.
ICO_SIZES = (16, 24, 32, 48, 64, 256)


def write_ico(out):
    pictures = [render(n) for n in ICO_SIZES]
    head = struct.pack("<HHH", 0, 1, len(pictures))
    offset = len(head) + 16 * len(pictures)
    entries, data = b"", b""
    for n, png in zip(ICO_SIZES, pictures):
        entries += struct.pack("<BBBBHHII", n % 256, n % 256, 0, 0, 1, 32, len(png), offset + len(data))
        data += png
    with open(out, "wb") as f:
        f.write(head + entries + data)


if __name__ == "__main__":
    args = sys.argv[1:]
    if len(args) == 2 and args[0] == "--ico":
        write_ico(args[1])
        sys.exit(0)
    full = "--full-bleed" in args
    PURPLE = "--yr" in args
    args = [a for a in args if a not in ("--full-bleed", "--yr")]
    if len(args) != 1:
        sys.exit("usage: make_icon.py [--full-bleed] [--yr] <out.png> | make_icon.py --ico <out.ico>")
    main(args[0], full)
