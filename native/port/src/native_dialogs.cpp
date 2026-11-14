// SPDX-License-Identifier: GPL-3.0-or-later
// Native dialog templates (enhancements menu, C3; not original). Two RT_DIALOG
// resources the port answers before the original's resource directory
// (original_image.cpp, ra2port::original_resource):
//   - 0xD5, the shell Options dialog, is the original template read from game.exe at
//     run time with one more control appended: the ENHANCEMENTS button (id 0x7F80) in
//     the free slot under Keyboard and Network, the same size and style as those
//     buttons (owner-drawn #128 0x5000000B, 108x23 dialog units).
//   - the Enhancements page, built here in the Options dialog's size, font and control
//     styles and answered under 0xD5 while the page is being created (ra2port_native_page)
//     so the game's dialog code (0x005FF610, the owner-draw handler 0x005FFB00) draws and
//     drives it as the Options dialog: backdrop, title box, hint line, button column.
// Nothing here is copied into the repository: the 0xD5 bytes come from the user's
// game.exe when the game asks for them (AGENTS.md "Native phase").
#include <stdint.h>
#include <string.h>
#include <string>
#include <vector>

namespace ra2port {
const void *original_resource_image(uintptr_t type, uintptr_t name, size_t *size);
namespace native_dialogs {
namespace {

struct Builder {
    std::vector<unsigned char> b;
    void align4() { while (b.size() % 4) b.push_back(0); }
    void u16(uint16_t v) { b.push_back(v & 0xFF); b.push_back(v >> 8); }
    void u32(uint32_t v) { u16(v & 0xFFFF); u16(v >> 16); }
    void text(const char *s) { for (; *s; ++s) u16((unsigned char)*s); u16(0); }
    // One DLGITEMTEMPLATEEX: class is an atom (0x80 button, 0x82 static).
    void item(uint32_t id, uint16_t atom, uint32_t style, int x, int y, int cx, int cy, const char *title)
    {
        align4();
        u32(0); u32(0); u32(style);
        u16((uint16_t)x); u16((uint16_t)y); u16((uint16_t)cx); u16((uint16_t)cy);
        u32(id);
        u16(0xFFFF); u16(atom);
        text(title);
        u16(0);                                     // no creation data
    }
};

constexpr uint32_t kButton = 0x5000000B;           // WS_CHILD|WS_VISIBLE|BS_OWNERDRAW (the Options buttons)
constexpr uint32_t kLabel = 0x50000200;            // the Options labels (SS_CENTERIMAGE)
constexpr uint32_t kValue = 0x50000202;            // right-aligned (as GUI:HigherDetail)
constexpr uint32_t kTitle = 0x50020001;            // the page title (as GUI:OptionsMenu)
constexpr uint32_t kHeading = 0x50000000;          // a section heading (as GUI:DisplayOptions)

// The Options dialog with the ENHANCEMENTS button appended.
std::vector<unsigned char> options_dialog()
{
    size_t size = 0;
    const unsigned char *d = (const unsigned char *)original_resource_image(5, 0xD5, &size);
    if (!d || size < 26 || d[2] != 0xFF || d[3] != 0xFF) return {};
    Builder out;
    out.b.assign(d, d + size);
    uint16_t count = (uint16_t)(d[16] | d[17] << 8);
    out.item(0x7F80, 0x80, kButton, 317, 176, 108, 23, "");
    ++count;
    out.b[16] = count & 0xFF;
    out.b[17] = count >> 8;
    return out.b;
}

// The Enhancements page. It is created under the Options dialog's resource id (0xD5,
// while ra2port_native_page is set), so the game's dialog code gives it the Options
// look: the backdrop and frame, the title box (control 0x694), the animated box in the
// button column (0x71C) and the hint line along the bottom (0x695). Its own controls sit
// in the main panel: one row per setting (name, value, CHANGE), the preset line with
// ORIGINAL and ENHANCED, and ACCEPT and CANCEL. Texts are set by the page's procedure
// (link/native_enhancements.cpp); the template's are empty.
std::vector<unsigned char> enhancements_dialog()
{
    Builder out;
    out.u16(1); out.u16(0xFFFF);                    // DLGTEMPLATEEX
    out.u32(0); out.u32(0); out.u32(0x40000040);    // helpID, exStyle, style (WS_CHILD|DS_SETFONT, as 0xD5)
    size_t count_at = out.b.size();
    out.u16(0);
    out.u16(0); out.u16(0); out.u16(426); out.u16(295);
    out.u16(0); out.u16(0); out.text("");           // no menu, default class, no title
    out.u16(8); out.u16(0); out.b.push_back(0); out.b.push_back(0); out.text("MS Sans Serif");
    int n = 0;
    auto item = [&](uint32_t id, uint16_t atom, uint32_t style, int x, int y, int cx, int cy, const char *t) {
        out.item(id, atom, style, x, y, cx, cy, t);
        ++n;
    };
    item(0x694, 0x82, kTitle, 318, 1, 108, 10, "");                    // the title box
    item(0x7F7E, 0x82, kHeading, 10, 10, 298, 10, "");
    for (int i = 0; i < 6; ++i) {                   // up to six rows; unused ones are hidden
        int y = 34 + 26 * i;
        item(0x7F20 + i, 0x82, kLabel, 25, y, 110, 10, "");
        item(0x7F30 + i, 0x82, kValue, 135, y, 70, 10, "");
        item(0x7F10 + i, 0x80, kButton, 218, y - 4, 80, 18, "");
    }
    item(0x7F40, 0x82, kLabel, 25, 205, 100, 10, "");
    item(0x7F70, 0x82, kLabel, 25, 229, 280, 10, "");                  // what the last row changed does
    item(0x7F00, 0x80, kButton, 128, 201, 80, 18, "");
    item(0x7F01, 0x80, kButton, 218, 201, 80, 18, "");
    item(0x7F02, 0x80, kButton, 128, 250, 80, 18, "");
    item(0x7F03, 0x80, kButton, 218, 250, 80, 18, "");
    item(0x695, 0x82, kLabel, 10, 282, 303, 12, "");                   // the hint line
    item(0x71C, 0x82, 0x50000007, 339, 29, 61, 33, "");               // the animated box (as 0xD5)
    out.b[count_at] = n & 0xFF;
    out.b[count_at + 1] = n >> 8;
    return out.b;
}

bool page_open = false;

}  // namespace

// The template the port answers for (type, id), or null for the original's own.
const void *resource(uintptr_t type, uintptr_t name, size_t *size)
{
    if (type != 5) return nullptr;
    static std::vector<unsigned char> options, page;
    std::vector<unsigned char> *t = nullptr;
    if (name == 0xD5 && page_open) {
        if (page.empty()) page = enhancements_dialog();
        t = &page;
    } else if (name == 0xD5) {
        if (options.empty()) options = options_dialog();
        t = &options;
    }
    if (!t || t->empty()) return nullptr;
    if (size) *size = t->size();
    return t->data();
}

}  // namespace native_dialogs
}  // namespace ra2port

// The Enhancements page asks for its template under 0xD5 while this is set
// (link/native_enhancements.cpp, around its dialog creation).
extern "C" void ra2port_native_page(int on) { ra2port::native_dialogs::page_open = on != 0; }
