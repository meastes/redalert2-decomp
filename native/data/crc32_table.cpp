// 0x007D5504: the CRC-32 table (reflected polynomial 0xEDB88320, the zlib/PKZIP
// CRC) that CRC_Memory (0x00494190), the CRC pipes/straws and the MIX file ids
// use. Behaviour-written generator, evaluated at compile time; compared
// privately with the pinned target (all 256 words equal). No executable bytes.
namespace {
struct Crc32Table {
    unsigned int v[256];
    constexpr Crc32Table() : v()
    {
        for (unsigned int n = 0; n < 256; ++n) {
            unsigned int c = n;
            for (int k = 0; k < 8; ++k) c = (c & 1) ? (c >> 1) ^ 0xEDB88320u : c >> 1;
            v[n] = c;
        }
    }
};
constexpr Crc32Table crc32_values;
}
// The recovered code declares it `unsigned long const[256]`, which the native
// view spells as the 32-bit RA2_ULONG; this file is not port-rewritten, so it
// says unsigned int (same 4-byte elements, same symbol name).
extern unsigned int const unknown_crc_table_007d5504[256];
unsigned int const unknown_crc_table_007d5504[256] = {
#define R(i) crc32_values.v[i], crc32_values.v[i + 1], crc32_values.v[i + 2], crc32_values.v[i + 3]
#define R16(i) R(i), R(i + 4), R(i + 8), R(i + 12)
    R16(0), R16(16), R16(32), R16(48), R16(64), R16(80), R16(96), R16(112),
    R16(128), R16(144), R16(160), R16(176), R16(192), R16(208), R16(224), R16(240),
#undef R16
#undef R
};
