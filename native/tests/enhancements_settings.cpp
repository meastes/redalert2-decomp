// SPDX-License-Identifier: GPL-3.0-or-later
// The Enhancements page's settings (platform/sdl/enhancements.cpp, lane C7): every row is
// there, CHANGE and ACCEPT save it in the app's settings, a fresh start reads it back and
// applies it, and the ORIGINAL preset sets every preset row back to the original look.
// Runs the page's own entry points (ra2_enh_*, as link/native_enhancements.cpp calls
// them) against a stand-in host and a settings file; tests/enhancements_settings.py runs
// the two phases as two processes (a fresh start), on the Mac and, under Wine, on Windows.
//
//   native-enhancements-settings change <settings file>   CHANGE each row once, ACCEPT
//   native-enhancements-settings reload <settings file>   read back; ORIGINAL; ACCEPT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <string>

extern "C" {
int ra2_enh_rows(void);
const char *ra2_enh_label(int i);
const char *ra2_enh_value(int i);
const char *ra2_enh_help(int i);
const char *ra2_enh_preset(void);
int ra2_enh_automatic(int i);
void ra2_enh_begin(void);
void ra2_enh_change(int i);
void ra2_enh_set_preset(int enhanced);
void ra2_enh_accept(void);
double ra2_enh_map_zoom(void);
int ra2_enh_sidebar_height(int screen_height);
}
void ra2_enhancements_store(std::string (*get)(const char *), void (*set)(const char *, const char *));
void ra2_enhancements_start();

// The host as the page sees it (platform/sdl/host.cpp on a desktop: not a touch host).
static int picture_scaling = -1;
static int vsync = -1;
static bool fullscreen = false;
void ra2_host_set_picture_scaling(int mode) { picture_scaling = mode; }
void ra2_host_set_vsync(bool on) { vsync = on; }
void ra2_host_set_fullscreen(bool on) { fullscreen = on; }
bool ra2_host_is_fullscreen() { return fullscreen; }
bool ra2_host_touch() { return false; }

// The app's settings file (main.cpp's app::Settings: key=value lines).
static std::map<std::string, std::string> settings;
static const char *settings_path;
static void read_settings()
{
    FILE *f = fopen(settings_path, "r");
    if (!f) return;
    char line[512];
    while (fgets(line, sizeof line, f)) {
        line[strcspn(line, "\r\n")] = 0;
        if (char *eq = strchr(line, '=')) {
            *eq = 0;
            settings[line] = eq + 1;
        }
    }
    fclose(f);
}
static void write_settings()
{
    FILE *f = fopen(settings_path, "w");
    for (auto &kv : settings) fprintf(f, "%s=%s\n", kv.first.c_str(), kv.second.c_str());
    fclose(f);
}
static std::string get(const char *key)
{
    auto it = settings.find(key);
    return it == settings.end() ? std::string() : it->second;
}
static void set(const char *key, const char *value)
{
    settings[key] = value;
    write_settings();
}

static int failures = 0;
static void expect(bool ok, const char *what, const std::string &got = std::string())
{
    if (!ok) {
        ++failures;
        fprintf(stderr, "enhancements-settings: FAILED %s%s%s\n", what, got.empty() ? "" : ": got ", got.c_str());
    }
}

// The rows the page must have, in its order, and each one's value after one CHANGE.
struct Want { const char *label, *original, *changed, *key, *saved_original, *saved_changed; };
static const Want rows[] = {
    {"Fullscreen", "Off", "On", nullptr, nullptr, nullptr},
    {"Vertical sync", "Off", "On", "enh_vsync", "off", "on"},
    {"Game interface size", "1x", "1.25x", "enh_uisize", "1", "1.25"},
    {"Map zoom", "1x", "1.25x", "enh_mapzoom", "1", "1.25"},
    {"Picture scaling", "Sharp", "Nearest", "enh_scaling", "sharp", "nearest"},
};
static const int kRows = sizeof rows / sizeof rows[0];

static int row_of(const char *label)
{
    for (int i = 0; i < ra2_enh_rows(); ++i)
        if (!strcmp(ra2_enh_label(i), label)) return i;
    return -1;
}

static void check_values(bool changed, const char *when)
{
    for (const Want &w : rows) {
        int i = row_of(w.label);
        if (i < 0) continue;
        std::string what = std::string(when) + ": " + w.label;
        expect(!strcmp(ra2_enh_value(i), changed ? w.changed : w.original), what.c_str(), ra2_enh_value(i));
    }
}

int main(int argc, char **argv)
{
    if (argc != 3 || (strcmp(argv[1], "change") && strcmp(argv[1], "reload"))) {
        fprintf(stderr, "usage: native-enhancements-settings change|reload <settings file>\n");
        return 64;
    }
    settings_path = argv[2];
    read_settings();
    ra2_enhancements_store(get, set);
    ra2_enhancements_start();
    bool change = !strcmp(argv[1], "change");

    // Every row is on the page, none is left Automatic on a desktop host.
    for (const Want &w : rows) {
        int i = row_of(w.label);
        std::string what = std::string("row ") + w.label + " on the page";
        expect(i >= 0, what.c_str());
        if (i >= 0) expect(!ra2_enh_automatic(i), (std::string("row ") + w.label + " not Automatic").c_str());
    }
    expect(ra2_enh_rows() == kRows, "row count", std::to_string(ra2_enh_rows()));
    // The Fullscreen row names this platform's shortcut (host.cpp).
#if defined(_WIN32)
    const char *keys = "Alt+Return";
#else
    const char *keys = "Cmd+Return";
#endif
    if (row_of("Fullscreen") >= 0)
        expect(strstr(ra2_enh_help(row_of("Fullscreen")), keys) != nullptr, "Fullscreen help names the shortcut",
               ra2_enh_help(row_of("Fullscreen")));

    if (change) {
        // A first start: the original look, applied.
        expect(picture_scaling == 0 && vsync == 0, "defaults applied at start");
        ra2_enh_begin();
        check_values(false, "first start");
        expect(!strcmp(ra2_enh_preset(), "Preset: Original"), "first start preset", ra2_enh_preset());
        for (const Want &w : rows) {
            int i = row_of(w.label);
            if (i >= 0) ra2_enh_change(i);
        }
        check_values(true, "after CHANGE");
        expect(!strcmp(ra2_enh_preset(), "Preset: Custom"), "preset after CHANGE", ra2_enh_preset());
        ra2_enh_accept();
        for (const Want &w : rows)
            if (w.key) expect(get(w.key) == w.saved_changed, (std::string("saved ") + w.key).c_str(), get(w.key));
        expect(fullscreen, "ACCEPT turned fullscreen on");
        expect(picture_scaling == 1 && vsync == 1, "changed settings applied at ACCEPT");
        // The next game in this same process (codex4 R700): what a new game's start asks
        // (link/native_enhancements.cpp Native_Sidebar_New_Game, native_map_zoom.cpp) answers
        // the accepted interface size and map zoom, without a restart; and back at 1x.
        expect(ra2_enh_sidebar_height(768) == 614, "same process: next game's interface size",
               std::to_string(ra2_enh_sidebar_height(768)));
        expect(ra2_enh_map_zoom() == 1.25, "same process: next game's map zoom");
        ra2_enh_begin();
        for (const char *label : {"Game interface size", "Map zoom"})
            for (int k = 0; k < 3; ++k) ra2_enh_change(row_of(label));     // 1.25x -> 1.5x -> 2x -> 1x
        ra2_enh_accept();
        expect(ra2_enh_sidebar_height(768) == 768 && ra2_enh_map_zoom() == 1.0, "same process: back to 1x",
               std::to_string(ra2_enh_sidebar_height(768)));
        ra2_enh_begin();
        ra2_enh_change(row_of("Game interface size"));
        ra2_enh_change(row_of("Map zoom"));
        ra2_enh_accept();
        expect(ra2_enh_sidebar_height(768) == 614 && ra2_enh_map_zoom() == 1.25, "same process: 1.25x again");
        // main.cpp keeps the window's fullscreen state in the same file at exit.
        set("fullscreen", fullscreen ? "1" : "0");
    } else {
        // A fresh start reads the saved values back and applies them; the window starts in
        // fullscreen from the file (main.cpp, ra2_sdl_host_options).
        fullscreen = get("fullscreen") == "1";
        expect(picture_scaling == 1 && vsync == 1, "saved settings applied at start");
        expect(ra2_enh_map_zoom() == 1.25, "map zoom read back");
        expect(ra2_enh_sidebar_height(768) == 614, "interface size read back", std::to_string(ra2_enh_sidebar_height(768)));
        ra2_enh_begin();
        check_values(true, "fresh start");
        // ORIGINAL sets every preset row back; fullscreen is the player's choice, which the
        // presets leave alone (README "Enhancements menu").
        ra2_enh_set_preset(0);
        expect(!strcmp(ra2_enh_preset(), "Preset: Original"), "preset after ORIGINAL", ra2_enh_preset());
        for (const Want &w : rows) {
            int i = row_of(w.label);
            if (i < 0) continue;
            const char *want = w.key ? w.original : w.changed;
            expect(!strcmp(ra2_enh_value(i), want), (std::string("ORIGINAL: ") + w.label).c_str(), ra2_enh_value(i));
        }
        ra2_enh_accept();
        for (const Want &w : rows)
            if (w.key) expect(get(w.key) == w.saved_original, (std::string("saved after ORIGINAL ") + w.key).c_str(), get(w.key));
        expect(picture_scaling == 0 && vsync == 0, "ORIGINAL applied at ACCEPT");
        expect(ra2_enh_map_zoom() == 1.0 && ra2_enh_sidebar_height(768) == 768, "ORIGINAL map zoom and interface size");
        // ENHANCED is recognised as such.
        ra2_enh_begin();
        ra2_enh_set_preset(1);
        expect(!strcmp(ra2_enh_preset(), "Preset: Enhanced"), "preset after ENHANCED", ra2_enh_preset());
    }
    fprintf(stderr, "enhancements-settings %s: %s\n", argv[1], failures ? "FAILED" : "ok");
    return failures ? 1 : 0;
}
