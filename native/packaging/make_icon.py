#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-or-later
# A placeholder app icon for RA2.app, drawn here from plain shapes (no game or
# third-party art): a red rounded square with a white five-pointed star and a
# dark ring. Writes one 1024x1024 PNG; make_app.sh scales it into the iconset.
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


def shade(x, y, star):
    # macOS icon grid: the body is about 824 of 1024, centred.
    m = 100
    if not inside_rounded(x, y, m, m, SIZE - m, SIZE - m, 185):
        return (0, 0, 0, 0)
    d = math.hypot(x - SIZE / 2, y - SIZE / 2)
    if inside_polygon(x, y, star):
        return (250, 246, 238, 255)
    if 300 <= d <= 330:
        return (60, 8, 8, 255)
    # red, darker towards the bottom
    t = (y - m) / (SIZE - 2 * m)
    return (int(200 - 70 * t), int(24 - 10 * t), int(20 - 8 * t), 255)


def main(out):
    star = star_points(SIZE / 2, SIZE / 2 + 18, 270, 108)
    rows = []
    for py in range(SIZE):
        row = bytearray([0])
        for px in range(SIZE):
            acc = [0, 0, 0, 0]
            for sy in range(SS):
                for sx in range(SS):
                    c = shade(px + (sx + 0.5) / SS, py + (sy + 0.5) / SS, star)
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

    png = b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", SIZE, SIZE, 8, 6, 0, 0, 0))
    png += chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b"")
    with open(out, "wb") as f:
        f.write(png)


if __name__ == "__main__":
    if len(sys.argv) != 2:
        sys.exit("usage: make_icon.py <out.png>")
    main(sys.argv[1])
