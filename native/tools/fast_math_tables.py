#!/usr/bin/env python3
"""Generate the fast-math lookup tables (0x00801DC4-0x00827DFC) and their scale constants.

The original carries these tables as initialised .data (the TibSun archive's
fastmathtab.asm has the same values for Tiberian Sun); the code that reads
them is recovered (src/recovered/unowned/Fast_*_004ba*.cpp) and the Remap_*
functions rebuild some of them later. Natively they are computed here, the way
Westwood's builder evidently ran: the TibSun fastmath.cpp init_*_tab loops
(sizes, steps and the M_FPI / 0.01 arctangent limit), evaluated on the x87
with the rounding control set to chop. The model, each part checked against the
original (see native/README.md, "Generated data"):

  * angles and inputs in double; sin/cos and tan by the x87 FSIN/FPTAN, which
    reduce the argument by the FPU's 66-bit pi (P87 below) and round the result
    to the 64-bit significand; asin/atan/sqrt correctly rounded;
  * every result stored to float truncated toward zero (round-to-zero);
  * the arctangent table accumulates its input in a float (also truncated) by a
    step of tan(M_FPI/2 - 0.01) / 4096, the step kept in atan_tab[4097].

Output: an assembly file defining the tables under the recovered code's names
(one storage per original address: names at an interior address, such as the
arctangent limit sample tan_tab[1017], are labels inside the table).

    python3 native/tools/fast_math_tables.py --out gen/fast_math_tables.s
"""
import argparse
import math
import struct
from decimal import Decimal, getcontext
from fractions import Fraction
from pathlib import Path

getcontext().prec = 60
PI = math.pi                                   # M_PI as the compiler rounds it
FPI = struct.unpack('<f', struct.pack('<f', 3.141592654))[0]   # fastmath.h M_FPI (3.141592654f)
P87 = Fraction(0xC90FDAA22168C234C, 1 << 66)   # x87 internal pi for FSIN/FCOS/FPTAN reduction

SIN_TABLE_SIZE = 8192
SIN_TABLE_EXTENT = SIN_TABLE_SIZE + SIN_TABLE_SIZE // 4
ASIN_TABLE_SIZE = TAN_TABLE_SIZE = ATAN_TABLE_SIZE = 4096
SQRT_BITS = 13
ATAN_LIMIT_INDEX = 1017                        # the tan sample Remap_Atan_Table reads (0x00810DC8)


def _series(r, start):
    r = Decimal(r.numerator) / Decimal(r.denominator)
    total, term, k = Decimal(0), (r if start else Decimal(1)), start
    eps = Decimal('1e-58')
    while abs(term) > eps:
        total += term
        term = -term * r * r / ((k + 1) * (k + 2))
        k += 2
    return total


def _reduce(x):
    x = Fraction(x)
    half = P87 / 2
    n = round(x / half)
    return x - n * half, n


def x87_sin(x):
    r, n = _reduce(x)
    s, c = _series(r, 1), _series(r, 0)
    return (s, c, -s, -c)[n % 4]


def x87_tan(x):
    r, n = _reduce(x)
    s, c = _series(r, 1), _series(r, 0)
    return s / c if n % 2 == 0 else -c / s


def ext64(value):
    """Round to the x87 64-bit significand (nearest)."""
    f = Fraction(value)
    if f == 0:
        return f
    sign = -1 if f < 0 else 1
    f = abs(f)
    e = f.numerator.bit_length() - f.denominator.bit_length()
    if Fraction(2) ** e > f:
        e -= 1
    scale = Fraction(2) ** (63 - e)
    return sign * Fraction(round(f * scale)) / scale


def chop_float(value):
    """IEEE single bits of value truncated toward zero."""
    f = Fraction(value)
    if f == 0:
        return 0
    bits = struct.unpack('<I', struct.pack('<f', float(f)))[0]

    def val(b):
        return Fraction(struct.unpack('<f', struct.pack('<I', b))[0])
    if abs(val(bits)) > abs(f):
        bits -= 1
    if abs(val(bits + 1)) <= abs(f):
        bits += 1
    return bits


def as_float(bits):
    return struct.unpack('<f', struct.pack('<I', bits))[0]


def tables():
    half = SIN_TABLE_SIZE // 2
    sin_tab = [chop_float(ext64(x87_sin(i * (PI / half)))) for i in range(SIN_TABLE_EXTENT + 1)]
    half = ASIN_TABLE_SIZE // 2
    asin_tab = [chop_float(math.asin(i / half - 1)) for i in range(ASIN_TABLE_SIZE + 1)]
    half = TAN_TABLE_SIZE // 2
    tan_tab = [chop_float(ext64(x87_tan(i * (PI / half)))) for i in range(TAN_TABLE_SIZE + 1)]
    limit = Fraction(FPI) / 2 - Fraction(0.01)
    step = chop_float(ext64(x87_tan(limit)) / ATAN_TABLE_SIZE)
    atan_tab, value = [], Fraction(0)
    for _ in range(ATAN_TABLE_SIZE + 1):
        atan_tab.append(chop_float(math.atan(float(value))))
        value = Fraction(as_float(chop_float(value + Fraction(as_float(step)))))
    atan_tab.append(step)
    # sqrt_tab: init_sqrt_tab's two halves (even / odd exponent), mantissa of the root.
    sqrt_tab = [0] * (1 << (SQRT_BITS + 1))
    for i in range(1 << SQRT_BITS):
        for base, exponent in ((0, 127), (1 << SQRT_BITS, 128)):
            x = Fraction(as_float((i << (23 - SQRT_BITS)) | (exponent << 23)))
            # exact square root, truncated: isqrt of the scaled value
            scale = 1 << 120
            root = Fraction(math.isqrt(x.numerator * scale * scale // x.denominator), scale)
            sqrt_tab[i + base] = chop_float(root) & 0x7FFFFF
    return sin_tab, asin_tab, tan_tab, atan_tab, sqrt_tab


def words(name, values, labels=None, pad=0, glob=True):
    out = ['', '.globl _%s' % name if glob else '', '.p2align 4', '_%s:' % name]
    labels = labels or {}
    for i in range(0, len(values), 8):
        for j in range(i, min(i + 8, len(values))):
            if j in labels:
                out.append('.globl _%s' % labels[j])
                out.append('_%s = _%s + %d' % (labels[j], name, 4 * j))
        out.append('.long ' + ', '.join('0x%08X' % v for v in values[i:i + 8]))
    if pad:
        out.append('.space %d' % pad)
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--out', type=Path, required=True)
    args = ap.parse_args()
    sin_tab, asin_tab, tan_tab, atan_tab, sqrt_tab = tables()
    f2b = lambda x: struct.unpack('<I', struct.pack('<f', x))[0]
    d2q = lambda x: struct.unpack('<Q', struct.pack('<d', x))[0]
    lines = ['// generated by native/tools/fast_math_tables.py: fast-math tables and constants,',
             '// computed (TibSun fastmath.cpp init loops on the chopping x87); no executable bytes.',
             '.section __DATA,__data']
    lines += words('unknown_sin_table_801dc4', sin_tab, pad=12)
    lines += words('unknown_asin_table_80bdd4', asin_tab, pad=12)
    lines += words('unknown_sin_table_80fde4', tan_tab, labels={ATAN_LIMIT_INDEX: 'unknown_atan_sample_00810dc8'}, pad=12)
    lines += words('unknown_atan_table_813df4', atan_tab, labels={ATAN_TABLE_SIZE + 1: 'unknown_atan_step_817df8'})
    lines += words('unknown_sqrt_table_817dfc', sqrt_tab)
    # fastmath.cpp `static float _s = SIN_TABLE_SIZE / M_PI;` (sinf and cosf each own one).
    scale = f2b(SIN_TABLE_SIZE / PI)
    lines += words('unknown_sin_scale_7d8048', [scale]) + words('unknown_cos_scale_7d804c', [scale])
    # Compiler constants of the table builders and tanf (double).
    for name, value in (('unknown_sin4096_scale_007a0f80', TAN_TABLE_SIZE / 2 / PI),
                        ('unknown_sine_step_007a0f98', PI / (SIN_TABLE_SIZE // 2)),
                        ('unknown_asin_step_007a0fa0', 1 / (ASIN_TABLE_SIZE // 2)),
                        ('unknown_sin4096_step_007a0fa8', PI / (TAN_TABLE_SIZE // 2))):
        q = d2q(value)
        lines += words(name, [q & 0xFFFFFFFF, q >> 32])
    text = '\n'.join(l for l in lines if l != '') + '\n'
    if not args.out.exists() or args.out.read_text() != text:
        args.out.write_text(text)


if __name__ == '__main__':
    main()
