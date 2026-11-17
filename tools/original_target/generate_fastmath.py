#!/usr/bin/env python3
"""Generate the fastmath table module (a generated data module; docs/DECISIONS.md
"fastmath tables as a generated data module").

    generate_fastmath.py OUT.asm

The five lookup tables of the fast math unit (TibSun fastmath.cpp and fastmathtab.asm:
sin_tab, asin_tab, tan_tab, atan_tab, sqrt_tab, one MASM .data module) are written out as
MASM `dd` lines, computed here with integer and exact rational arithmetic only (no host
floating point), so the text is the same on every host. Each entry is what the x87 gives
for the table's formula when the result is stored to a float under round-toward-zero
(the game runs with the FPU control word set to chop):

  sin_tab[i]   = sin(i * (pi / 4096)),   i = 0 .. 10240   (TibSun init_sin_tab; 8192 per turn
                                                           plus a quarter for cosine)
  asin_tab[i]  = asin(i / 2048 - 1),     i = 0 .. 4096    (TibSun init_asin_tab)
  tan_tab[i]   = tan(i * (pi / 2048)),   i = 0 .. 4096    (TibSun init_tan_tab)
  atan_tab[i]  = atan(v_i), v_0 = 0, v_(i+1) = float(v_i + step), i = 0 .. 4096
                                                          (TibSun init_atan_tab; RA2
                                                           Remap_Atan_Table 0x004BAB30)
  atan_tab[4097] = step = 0x3CC7FE84                      (flagged constant, see below)
  sqrt_tab[i]  = mantissa of sqrt(float with mantissa i << 10, exponent 127),
  sqrt_tab[i + 8192] = the same with exponent 128,  i = 0 .. 8191
                                                          (TibSun init_sqrt_tab, Graphics
                                                           Gems III; RA2 0x004BA6F0)

Transcendental results are the exact value rounded to nearest at the x87's 64-bit
extended mantissa, then chopped to float when stored; square roots chop directly (fsqrt
rounds by the control word). pi in the arguments is the double nearest pi (M_PI); the argument is formed exactly.
fsin and fptan reduce their argument by multiples of pi/2 using the x87's internal
66-bit pi, which decides the entries at multiples of pi/2 (sin(pi) is not zero); the
other functions are evaluated exactly. Every table ends in the zero entries the original
allocates (sin 3, asin 3, tan 3, sqrt 2).

FLAGGED CONSTANT: atan_tab[4097], the arctangent input step, is 0x3CC7FE84 (about
99.997 / 4096). The derivation is not known: TibSun's pi/size/pi * tan_tab[1017] and RA2's
Remap_Atan_Table tan_tab[1017] / 4096 both give another value. It is written as the named
hex constant ATAN_STEP_BITS and every atan_tab entry is generated from it.

The symbols are the decorated names the accepted C++ uses for these tables, so the
references of the accepted rows bind the module (build.py proves the assembled .data
section as one whole block at the original addresses before it replaces the scaffold).
"""
import sys
from fractions import Fraction

P = 320                                   # fixed-point fraction bits
ONE = 1 << P
ATAN_STEP_BITS = 0x3CC7FE84               # flagged: derivation unknown (see the docstring)
PI_DOUBLE = Fraction(0x1921FB54442D18, 1 << 51)            # M_PI, 0x400921FB54442D18
PI_X87 = Fraction(0xC90FDAA22168C234C >> 2, 1 << 64)       # the x87's 66-bit internal pi

SYMBOLS = [   # (label, decorated name, entries computed, zero entries after them)
    ('sin_tab', '?unknown_sin_table_801dc4@@3PAMA', 10241, 3),
    ('asin_tab', '?unknown_asin_table_80bdd4@@3PAMA', 4097, 3),
    ('tan_tab', '?unknown_sin_table_80fde4@@3PAMA', 4097, 3),
    ('atan_tab', '?unknown_atan_table_813df4@@3PAMA', 4097, 0),
    ('sqrt_tab', '?unknown_sqrt_table_817dfc@@3PAIA', 16384, 2),
]
# Other names the accepted code uses inside the module: (decorated name, label, entry index).
INNER = [('?unknown_atan_sample_00810dc8@@3MB', 'tan_tab', 1017),
         ('?unknown_atan_step_817df8@@3MA', 'atan_tab', 4097)]


def fixed(value):
    """Fraction -> fixed point (floor)."""
    return value.numerator * ONE // value.denominator


def mul(a, b):
    return a * b >> P


def div(a, b):
    return (a << P) // b


def isqrt_fixed(a):
    from math import isqrt
    return isqrt(a << P)


def atan_series(x):
    """atan(x) for |x| <= 1/16 (fixed)."""
    total, power, square, n = 0, x, mul(x, x), 0
    while power:
        term = power // (2 * n + 1)
        total += -term if n & 1 else term
        power = mul(power, square)
        n += 1
    return total


def atan_fixed(x):
    """atan of a fixed-point value (any sign/size)."""
    if x < 0:
        return -atan_fixed(-x)
    if x > ONE:
        return (PI_FIXED >> 1) - atan_fixed(div(ONE, x))
    halvings = 0
    while x > ONE >> 4:                    # atan(x) = 2 atan(x / (1 + sqrt(1 + x^2)))
        x = div(x, ONE + isqrt_fixed(ONE + mul(x, x)))
        halvings += 1
    return atan_series(x) << halvings


def pi_machin():
    def arctan_inverse(n):
        total, power, k = 0, ONE // n, 0
        while power:
            term = power // (2 * k + 1)
            total += -term if k & 1 else term
            power //= n * n
            k += 1
        return total
    return 16 * arctan_inverse(5) - 4 * arctan_inverse(239)


PI_FIXED = pi_machin()


def sin_cos(r):
    """(sin r, cos r) for |r| <= 1 (fixed)."""
    s, c, term, n = 0, 0, ONE, 0
    while term:
        if n % 4 == 0: c += term
        elif n % 4 == 1: s += term
        elif n % 4 == 2: c -= term
        else: s -= term
        n += 1
        term = mul(term, r) // n
    return s, c


def reduce_x87(x):
    """x87 argument reduction by its pi/2: (k, r) with x = k * (pi66 / 2) + r."""
    half = PI_X87 / 2
    k = (x / half + Fraction(1, 2)).__floor__()
    return k, fixed(x - k * half)


def sin_x87(x):
    k, r = reduce_x87(x)
    s, c = sin_cos(r)
    return [s, c, -s, -c][k % 4]


def tan_x87(x):
    k, r = reduce_x87(x)
    s, c = sin_cos(r)
    return div(s, c) if k % 2 == 0 else -div(c, s)


def chop_float_fixed(y, margin=1 << 40):
    """Bits of the float an x87 transcendental result stores under round-toward-zero: the
    exact value y (fixed) is first rounded to nearest at the 64-bit extended mantissa (the
    result fsin/fptan/fpatan deliver), then chopped to 24 bits. Refuses when y lies within
    margin (fixed units) of an extended rounding midpoint (the evaluation error must not
    decide)."""
    if y == 0:
        return 0
    sign = 0x80000000 if y < 0 else 0
    a = abs(y)
    if a.bit_length() > 64:
        shift = a.bit_length() - 64
        q = a >> shift
        rest = a - (q << shift)
        half = 1 << (shift - 1)
        if abs(rest - half) < margin:
            raise ValueError('evaluation too close to an extended rounding midpoint')
        a = (q + (rest > half)) << shift
    e = a.bit_length() - 1 - P                 # floor(log2 a)
    shift = P + e - 23                          # a / 2^shift has 24 integer bits
    m = a >> shift if shift >= 0 else a << -shift
    assert 1 << 23 <= m < 1 << 24 and -126 <= e <= 127
    return sign | (e + 127) << 23 | (m - (1 << 23))


def chop_float_exact(value):
    """Bits of the float nearest zero-ward to an exact Fraction."""
    if value == 0:
        return 0
    sign = 0x80000000 if value < 0 else 0
    a = abs(value)
    e = a.numerator.bit_length() - a.denominator.bit_length()
    if Fraction(2) ** e > a:
        e -= 1
    elif Fraction(2) ** (e + 1) <= a:
        e += 1
    m = (a / Fraction(2) ** (e - 23)).__floor__()
    assert 1 << 23 <= m < 1 << 24
    return sign | (e + 127) << 23 | (m - (1 << 23))


def chop_sqrt_exact(x):
    """Bits of the float nearest zero-ward to sqrt(x), x a positive Fraction (exact)."""
    from math import isqrt
    e = (x.numerator.bit_length() - x.denominator.bit_length()) // 2
    while Fraction(2) ** (2 * e) > x:
        e -= 1
    while Fraction(2) ** (2 * e + 2) <= x:
        e += 1
    m = isqrt((x * Fraction(2) ** (46 - 2 * e)).__floor__())
    assert 1 << 23 <= m < 1 << 24
    return (e + 127) << 23 | (m - (1 << 23))


def float_value(bits):
    sign = -1 if bits >> 31 else 1
    e = (bits >> 23) & 0xff
    m = bits & 0x7fffff
    if e == 0:
        return sign * Fraction(m, 1 << 149)
    return sign * Fraction(m | 1 << 23) * Fraction(2) ** (e - 150)


def tables():
    out = {}
    out['sin_tab'] = [chop_float_fixed(sin_x87(i * PI_DOUBLE / 4096)) for i in range(10241)]
    asin = []
    for i in range(4097):
        v = Fraction(i, 2048) - 1
        if abs(v) == 1:
            y = (PI_FIXED >> 1) * (1 if v > 0 else -1)
        else:
            y = atan_fixed(div(fixed(v), isqrt_fixed(fixed(1 - v * v))))
        asin.append(chop_float_fixed(y))
    out['asin_tab'] = asin
    out['tan_tab'] = [chop_float_fixed(tan_x87(i * PI_DOUBLE / 2048)) for i in range(4097)]
    step = float_value(ATAN_STEP_BITS)
    atan, v = [], Fraction(0)
    for i in range(4097):
        atan.append(chop_float_fixed(atan_fixed(fixed(v))))
        v = float_value(chop_float_exact(v + step))
    out['atan_tab'] = atan + [ATAN_STEP_BITS]
    sqrt = []
    for exponent in (127, 128):
        for i in range(8192):
            x = float_value((i << 10) | (exponent << 23))
            sqrt.append(chop_sqrt_exact(x) & 0x7fffff)
    out['sqrt_tab'] = sqrt
    for label, _, count, zeros in SYMBOLS:
        assert len(out[label]) == count + (1 if label == 'atan_tab' else 0)
        out[label] += [0] * zeros
    return out


COMMENTS = {
    'sin_tab': 'sin(i * (pi / 4096)), i = 0 .. 10240; 3 zero entries (TibSun init_sin_tab)',
    'asin_tab': 'asin(i / 2048 - 1), i = 0 .. 4096; 3 zero entries (TibSun init_asin_tab)',
    'tan_tab': 'tan(i * (pi / 2048)), i = 0 .. 4096; 3 zero entries (TibSun init_tan_tab)',
    'atan_tab': 'atan(v_i), v_(i+1) = float(v_i + step), i = 0 .. 4096; [4097] = step '
                '(flagged constant ATAN_STEP_BITS) (TibSun init_atan_tab)',
    'sqrt_tab': 'mantissa of sqrt(1.m) for m = i << 10, exponent 127 then 128, i = 0 .. 8191; '
                '2 zero entries (TibSun init_sqrt_tab)',
}


def module_text():
    data = tables()
    lines = [';; fastmath tables: a generated data module (tools/original_target/generate_fastmath.py).',
             ';; Shape after TibSun fastmathtab.asm (GPL-3.0-or-later, OpenTS contributors; THIRD_PARTY.md):',
             ';; one .data module of five dword tables. Values computed under x87 round-toward-zero.',
             ';; ATAN_STEP_BITS = 0%08Xh is a flagged constant (derivation unknown).' % ATAN_STEP_BITS,
             '', '.586', '.model flat', '']
    for label, name, _, _ in SYMBOLS:
        lines.append('PUBLIC %s' % name)
    for name, _, _ in INNER:
        lines.append('PUBLIC %s' % name)
    lines += ['', '.data']
    inner = {(label, index): name for name, label, index in INNER}
    for label, name, _, _ in SYMBOLS:
        lines += ['', ';; %s: %s' % (label, COMMENTS[label]), '%s LABEL DWORD' % name]
        values = data[label]
        row = []
        for index, value in enumerate(values):
            if (label, index) in inner:
                if row:
                    lines.append('\tdd ' + ', '.join(row)); row = []
                lines.append('%s LABEL DWORD' % inner[(label, index)])
            row.append('0%08Xh' % value)
            if len(row) == 8:
                lines.append('\tdd ' + ', '.join(row)); row = []
        if row:
            lines.append('\tdd ' + ', '.join(row))
    lines += ['', 'END', '']
    return '\r\n'.join(lines)


def table_bytes():
    import struct
    data = tables()
    return b''.join(struct.pack('<%dI' % len(data[label]), *data[label]) for label, _, _, _ in SYMBOLS)


def main():
    if len(sys.argv) != 2:
        raise SystemExit(__doc__)
    with open(sys.argv[1], 'w', newline='') as handle:
        handle.write(module_text())


if __name__ == '__main__':
    main()
