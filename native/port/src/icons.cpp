// Icons from the supported game.exe at run time (lane C15d): the game registers its
// window class with LoadIconA(instance, 0x5D) (winstub.cpp), its RT_GROUP_ICON. The
// port decodes the group's largest picture (a DIB: 1, 4, 8, 24 or 32 bits, with its AND
// mask) to 0xAARRGGBB so the host can show it on the window (user32.cpp CreateWindowExA).
// Nothing of the original is kept in the repository: the pixels come from the user's
// game.exe each run (AGENTS.md "Native phase").
#include "ra2port/win32_port.hpp"

#include <map>
#include <memory>
#include <mutex>
#include <string.h>

namespace ra2port {
const void *original_resource(uintptr_t type, uintptr_t name, size_t *size);

namespace {
std::mutex lock;
std::map<void *, std::unique_ptr<IconImage>> images;   // handle -> picture

uint32_t rd16(const uint8_t *p) { return p[0] | (p[1] << 8); }
uint32_t rd32(const uint8_t *p) { return p[0] | (p[1] << 8) | (p[2] << 16) | ((uint32_t)p[3] << 24); }

// One RT_ICON resource (BITMAPINFOHEADER, palette, XOR rows, AND rows; bottom-up rows,
// each padded to 4 bytes; the header's height counts both bitmaps). PNG pictures (Vista
// icons) are not read: game.exe's are DIBs.
bool decode(const uint8_t *d, size_t n, IconImage &out)
{
    if (n < 40 || rd32(d) < 40) return false;
    uint32_t header = rd32(d);
    int w = (int)rd32(d + 4), h = (int)rd32(d + 8) / 2;
    uint32_t bits = rd16(d + 14), used = rd32(d + 32);
    if (w <= 0 || h <= 0 || w > 256 || h > 256 || rd32(d + 16) != 0) return false;   // BI_RGB only
    if (bits != 1 && bits != 4 && bits != 8 && bits != 24 && bits != 32) return false;
    uint32_t colours = bits <= 8 ? (used ? used : 1u << bits) : 0;
    size_t palette = header, xor_rows = palette + 4 * (size_t)colours;
    size_t xor_stride = (((size_t)w * bits + 31) / 32) * 4, and_stride = (((size_t)w + 31) / 32) * 4;
    size_t and_rows = xor_rows + xor_stride * h;
    if (and_rows + and_stride * h > n) return false;
    out.width = w;
    out.height = h;
    out.argb.assign((size_t)w * h, 0);
    bool any_alpha = false;
    for (int y = 0; y < h; ++y) {
        const uint8_t *row = d + xor_rows + xor_stride * (h - 1 - y);
        const uint8_t *mask = d + and_rows + and_stride * (h - 1 - y);
        for (int x = 0; x < w; ++x) {
            uint32_t c;
            if (bits == 32) {
                c = rd32(row + 4 * x);
                any_alpha |= (c >> 24) != 0;
            } else if (bits == 24) {
                c = row[3 * x] | (row[3 * x + 1] << 8) | (row[3 * x + 2] << 16);
            } else {
                uint32_t index = bits == 8 ? row[x] : bits == 4 ? (row[x / 2] >> (x % 2 ? 0 : 4)) & 15
                                                                : (row[x / 8] >> (7 - x % 8)) & 1;
                if (index >= colours) index = 0;
                c = rd32(d + palette + 4 * index) & 0xFFFFFFu;   // RGBQUAD: blue, green, red
            }
            bool transparent = (mask[x / 8] >> (7 - x % 8)) & 1;
            if (bits != 32) c = (c & 0xFFFFFFu) | (transparent ? 0u : 0xFF000000u);
            out.argb[(size_t)y * w + x] = c;
        }
    }
    if (bits == 32 && !any_alpha) {     // an old 32-bit picture without alpha: the mask's
        for (int y = 0; y < h; ++y) {
            const uint8_t *mask = d + and_rows + and_stride * (h - 1 - y);
            for (int x = 0; x < w; ++x) {
                bool transparent = (mask[x / 8] >> (7 - x % 8)) & 1;
                out.argb[(size_t)y * w + x] |= transparent ? 0u : 0xFF000000u;
            }
        }
    }
    return true;
}
}  // namespace

void *load_original_icon(unsigned group_id)
{
    size_t n = 0;
    const uint8_t *g = (const uint8_t *)original_resource(14, group_id, &n);   // RT_GROUP_ICON
    if (!g || n < 6 || rd16(g + 2) != 1) return nullptr;
    unsigned count = rd16(g + 4), best = count;
    int best_size = 0, best_bits = 0;
    for (unsigned i = 0; i < count && 6 + 14 * (i + 1) <= n; ++i) {
        const uint8_t *e = g + 6 + 14 * i;
        int size = e[0] ? e[0] : 256, bits = (int)rd16(e + 6);
        if (size > best_size || (size == best_size && bits > best_bits)) best = i, best_size = size, best_bits = bits;
    }
    if (best == count) return nullptr;
    size_t m = 0;
    const uint8_t *d = (const uint8_t *)original_resource(3, rd16(g + 6 + 14 * best + 12), &m);   // RT_ICON
    std::unique_ptr<IconImage> image(new IconImage);
    if (!d || !decode(d, m, *image)) return nullptr;
    std::lock_guard<std::mutex> g2(lock);
    void *handle = (void *)(uintptr_t)(0x7300 + group_id);   // a distinct handle per group
    images[handle] = std::move(image);
    return handle;
}

const IconImage *icon_image(void *icon)
{
    std::lock_guard<std::mutex> g(lock);
    auto it = images.find(icon);
    return it == images.end() ? nullptr : it->second.get();
}
}  // namespace ra2port
