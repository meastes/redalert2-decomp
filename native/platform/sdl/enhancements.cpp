// SPDX-License-Identifier: GPL-3.0-or-later
// Enhancements (native additions, not in the original; the Outpost 2 port's model,
// docs/HANDOFF.md "ENHANCEMENTS MENU MODEL"). Every setting is off by default: with
// the defaults (and with the ORIGINAL preset) the app looks and behaves as before
// (tests/enhancements_frames.py proves the frames identical). Settings live in the
// native app's own settings file (main.cpp, key=value lines beside RA2.INI's folder),
// never in RA2.INI; RA2_NATIVE_ENHANCEMENTS="key=value,..." sets them for one run
// (tests; not saved). The page itself is a native dialog reached from the shell
// Options dialog (link/native_enhancements.cpp; port/src/native_dialogs.cpp).
//
// Rows appear only when something backs them: map zoom and interface size come with
// their own work item, not as dead controls.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <map>
#include <vector>

void ra2_host_set_picture_scaling(int mode);
void ra2_host_set_vsync(bool on);
void ra2_host_set_fullscreen(bool on);
bool ra2_host_is_fullscreen();
bool ra2_host_touch();

// The host's fullscreen shortcut (host.cpp): Alt+Return on Windows, the Command key's on the Mac.
#if defined(_WIN32)
#define RA2_FULLSCREEN_KEYS "Alt+Return"
#else
#define RA2_FULLSCREEN_KEYS "Cmd+Return or Cmd+F"
#endif

namespace {

struct Row {
    const char *key;
    const char *label;
    const char *help;
    std::vector<std::string> values;   // the first is the default (the original look)
    std::vector<std::string> names;    // shown on the page
    bool preset;                       // set by the presets (fullscreen is a choice they leave alone)
};

const std::vector<Row> &rows()
{
    static const std::vector<Row> r = {
        {"fullscreen", "Fullscreen", "Fill the screen (also " RA2_FULLSCREEN_KEYS ").",
         {"off", "on"}, {"Off", "On"}, false},
        {"vsync", "Vertical sync", "Draw in step with the display to avoid tearing.",
         {"off", "on"}, {"Off", "On"}, true},
        {"uisize", "Game interface size", "The sidebar in play, larger than the map. From the next game.",
         {"1", "1.25", "1.5", "2"}, {"1x", "1.25x", "1.5x", "2x"}, true},
        {"mapzoom", "Map zoom", "The map in play shown larger. From the next game.",
         {"1", "1.25", "1.5", "2"}, {"1x", "1.25x", "1.5x", "2x"}, true},
        {"scaling", "Picture scaling", "Sharp keeps pixel edges, Nearest is blocky, Smooth softens.",
         {"sharp", "nearest", "smooth"}, {"Sharp", "Nearest", "Smooth"}, true},
    };
    return r;
}

const std::map<std::string, std::string> &preset_values(bool enhanced)
{
    static const std::map<std::string, std::string> original = {{"vsync", "off"}, {"scaling", "sharp"}, {"uisize", "1"}, {"mapzoom", "1"}};
    static const std::map<std::string, std::string> better = {{"vsync", "on"}, {"scaling", "smooth"}};
    return enhanced ? better : original;
}

std::map<std::string, std::string> current, pending;
std::string (*store_get)(const char *key) = nullptr;
void (*store_set)(const char *key, const char *value) = nullptr;
bool loaded = false;

const Row *find_row(const std::string &key)
{
    for (const Row &r : rows()) if (key == r.key) return &r;
    return nullptr;
}

bool valid(const Row &r, const std::string &v)
{
    for (const std::string &x : r.values) if (x == v) return true;
    return false;
}

void set_value(std::map<std::string, std::string> &s, const std::string &key, const std::string &value)
{
    if (key == "preset") {
        if (value == "original" || value == "enhanced")
            for (auto &kv : preset_values(value == "enhanced")) s[kv.first] = kv.second;
        return;
    }
    const Row *r = find_row(key);
    if (r && valid(*r, value)) s[key] = value;
}

void apply()
{
    const std::string &scaling = current["scaling"];
    ra2_host_set_picture_scaling(scaling == "nearest" ? 1 : scaling == "smooth" ? 2 : 0);
    ra2_host_set_vsync(current["vsync"] == "on");
}

void load()
{
    if (loaded) return;
    loaded = true;
    for (const Row &r : rows()) current[r.key] = r.values[0];
    if (store_get)
        for (const Row &r : rows())
            if (r.preset) set_value(current, r.key, store_get((std::string("enh_") + r.key).c_str()));
    if (const char *env = getenv("RA2_NATIVE_ENHANCEMENTS")) {    // tests: one run, not saved
        std::string s = env;
        size_t at = 0;
        while (at <= s.size()) {
            size_t end = s.find(',', at);
            if (end == std::string::npos) end = s.size();
            std::string item = s.substr(at, end - at);
            size_t eq = item.find('=');
            if (eq != std::string::npos) set_value(current, item.substr(0, eq), item.substr(eq + 1));
            at = end + 1;
        }
        store_set = nullptr;
    }
    fprintf(stderr, "enhancements: vsync %s, scaling %s, interface %sx, map zoom %sx\n", current["vsync"].c_str(),
            current["scaling"].c_str(), current["uisize"].c_str(), current["mapzoom"].c_str());
}

bool automatic(const Row &r) { return std::string(r.key) == "fullscreen" && ra2_host_touch(); }

std::string value_of(const std::map<std::string, std::string> &s, const Row &r)
{
    if (std::string(r.key) == "fullscreen") {
        auto it = s.find("fullscreen");
        return it == s.end() ? (ra2_host_is_fullscreen() ? "on" : "off") : it->second;
    }
    auto it = s.find(r.key);
    return it == s.end() ? r.values[0] : it->second;
}

std::string preset_of(const std::map<std::string, std::string> &s)
{
    for (int e = 0; e < 2; ++e) {
        bool all = true;
        for (auto &kv : preset_values(e == 1)) {
            auto it = s.find(kv.first);
            if (it == s.end() || it->second != kv.second) all = false;
        }
        if (all) return e ? "Enhanced" : "Original";
    }
    return "Custom";
}

}  // namespace

// Where the app keeps its settings (main.cpp, app mode); without it nothing is saved.
void ra2_enhancements_store(std::string (*get)(const char *), void (*set)(const char *, const char *))
{
    store_get = get;
    store_set = set;
}
// At the host's start (create_window): the saved (or RA2_NATIVE_ENHANCEMENTS) settings.
void ra2_enhancements_start()
{
    load();
    apply();
}

// The page (link/native_enhancements.cpp) works on a pending copy: CHANGE and the
// presets edit it, ACCEPT applies and saves it, CANCEL drops it.
extern "C" {
int ra2_enh_rows(void) { return (int)rows().size(); }
const char *ra2_enh_label(int i) { return i >= 0 && i < ra2_enh_rows() ? rows()[i].label : ""; }
const char *ra2_enh_help(int i)
{
    if (i < 0 || i >= ra2_enh_rows())
        return "Native additions, not in the original. Each is off in the ORIGINAL preset.";
    if (automatic(rows()[i])) return "Fullscreen follows the display here.";
    return rows()[i].help;
}
int ra2_enh_automatic(int i) { return i >= 0 && i < ra2_enh_rows() && automatic(rows()[i]); }
const char *ra2_enh_value(int i)
{
    static std::string text;
    if (i < 0 || i >= ra2_enh_rows()) return "";
    const Row &r = rows()[i];
    if (automatic(r)) return "Automatic";
    std::string v = value_of(pending, r);
    for (size_t k = 0; k < r.values.size(); ++k) if (r.values[k] == v) return (text = r.names[k]).c_str();
    return (text = v).c_str();
}
const char *ra2_enh_preset(void)
{
    static std::string text;
    return (text = "Preset: " + preset_of(pending)).c_str();
}
void ra2_enh_begin(void)
{
    load();
    pending = current;
    pending["fullscreen"] = ra2_host_is_fullscreen() ? "on" : "off";
}
void ra2_enh_change(int i)
{
    if (i < 0 || i >= ra2_enh_rows() || automatic(rows()[i])) return;
    const Row &r = rows()[i];
    std::string v = value_of(pending, r);
    size_t k = 0;
    while (k < r.values.size() && r.values[k] != v) ++k;
    pending[r.key] = r.values[k + 1 < r.values.size() ? k + 1 : 0];
}
void ra2_enh_set_preset(int enhanced) { set_value(pending, "preset", enhanced ? "enhanced" : "original"); }
void ra2_enh_accept(void)
{
    bool full = pending["fullscreen"] == "on";
    pending.erase("fullscreen");
    current = pending;
    apply();
    if (!ra2_host_touch() && full != ra2_host_is_fullscreen()) ra2_host_set_fullscreen(full);
    if (store_set)
        for (const Row &r : rows())
            if (r.preset) store_set((std::string("enh_") + r.key).c_str(), current[r.key].c_str());
    fprintf(stderr, "enhancements: accepted vsync %s, scaling %s, interface %sx, map zoom %sx, fullscreen %s\n",
            current["vsync"].c_str(), current["scaling"].c_str(), current["uisize"].c_str(), current["mapzoom"].c_str(),
            full ? "on" : "off");
}
}

// ---- Game interface size (v2 part 1, C3): the sidebar larger than the map ------------
// The game draws the sidebar on its own surface (Allocate_Surfaces' callers, the sidebar
// layout 0x006F4C90). Natively that surface may be shorter than the screen (native
// fixups ask ra2_enh_sidebar_height), and the host shows the sidebar column that many
// times larger than the picture's base scale (host.cpp, the split layout), so it fills the
// height again. The map's own scale is the separate Map zoom below (v2 part 2): the two
// are independent, each relative to the picture's base scale.
namespace {
constexpr int kSidebarMinHeight = 360;     // three cameo rows below the radar and tabs (tried at 384: plays)
int sidebar_for = 0, sidebar_rows = 0;     // the last screen height asked about and the answer
}
extern "C" int ra2_enh_sidebar_height(int screen_height)
{
    load();
    double ratio = atof(current["uisize"].c_str());
    int h = screen_height;
    if (ratio > 1.0) {
        h = (int)(screen_height / ratio + 0.5) & ~1;
        if (h < kSidebarMinHeight) h = kSidebarMinHeight;
        if (h > screen_height) h = screen_height;
    }
    if (h != sidebar_rows || screen_height != sidebar_for)
        fprintf(stderr, "enhancements: sidebar %d of %d screen rows (interface %sx)\n", h, screen_height, current["uisize"].c_str());
    sidebar_for = screen_height;
    sidebar_rows = h;
    return h;
}
// The host's question (host.cpp): for a picture this tall, is the sidebar column shorter
// than the screen, and by what ratio is it shown larger? False when it fills the height.
bool ra2_enh_sidebar_split(int picture_height, double *ratio)
{
    if (sidebar_for != picture_height || sidebar_rows <= 0 || sidebar_rows >= picture_height) return false;
    *ratio = (double)picture_height / sidebar_rows;
    return true;
}

// ---- Map zoom (v2 part 2, C4): the map larger than the interface ------------------------
// The game's screen keeps its size (so every dialog, the menus and the sidebar work as in
// the original); only the tactical view is made smaller: Set_View_Dimensions 0x00498C80
// (native fixup) keeps the view rectangle divided by the zoom (link/native_enhancements.cpp
// Native_Map_View), and the host shows that smaller view over the whole map area, the zoom
// times larger (host.cpp, the zoom layout). The zoom a game uses is taken when it starts
// (Start_Scenario, a load); with 1x nothing changes.
namespace {
int view_rect[4] = {0, 0, 0, 0}, full_rect[4] = {0, 0, 0, 0};   // the view and the tactical rectangle it stands for
bool in_play = false;          // a game frame ran since the last scenario start or load
}
// The zoom for the game that starts now (the setting; 1 without one).
extern "C" double ra2_enh_map_zoom(void)
{
    load();
    double z = atof(current["mapzoom"].c_str());
    return z >= 1.0 && z <= 4.0 ? z : 1.0;
}
// The game's view rectangle as Set_View_Dimensions keeps it, and the tactical rectangle it
// was asked for (equal at 1x).
extern "C" void ra2_enh_map_view(int x, int y, int w, int h, int full_x, int full_y, int full_w, int full_h)
{
    int v[4] = {x, y, w, h}, f[4] = {full_x, full_y, full_w, full_h};
    if (memcmp(v, view_rect, sizeof v) || memcmp(f, full_rect, sizeof f))
        fprintf(stderr, "enhancements: map view %dx%d at %d,%d for %dx%d (map zoom %.2fx)\n", w, h, x, y, full_w, full_h,
                w > 0 ? (double)full_w / w : 1.0);
    memcpy(view_rect, v, sizeof v);
    memcpy(full_rect, f, sizeof f);
}
// In play (a game frame ran) or not (a scenario starting or loading: its loading screen
// and movies are shown whole).
extern "C" void ra2_enh_in_play(int on) { in_play = on != 0; }
// The host's question: is the map shown zoomed now, for a picture this size? view and full
// get the two rectangles (game pixels).
bool ra2_enh_map_zoomed(int picture_width, int picture_height, int view[4], int full[4])
{
    if (!in_play || view_rect[2] <= 0 || view_rect[3] <= 0) return false;
    if (view_rect[2] >= full_rect[2] && view_rect[3] >= full_rect[3]) return false;
    if (full_rect[0] < 0 || full_rect[1] < 0 || full_rect[0] + full_rect[2] > picture_width || full_rect[1] + full_rect[3] > picture_height)
        return false;
    memcpy(view, view_rect, sizeof view_rect);
    memcpy(full, full_rect, sizeof full_rect);
    return true;
}
