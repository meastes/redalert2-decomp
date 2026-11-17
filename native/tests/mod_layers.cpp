// SPDX-License-Identifier: GPL-3.0-or-later
// ctest native-mod-layers (lane C16, mod support level 1): the port's read layers. In a fresh
// folder an installation, a write folder and a mod folder are made; the checks are what the game's
// file calls see:
//   * a file the mod has (any case) is read from the mod, ahead of the write folder and the
//     installation; a file it lacks from the write folder, else the installation;
//   * a folder stays the installation's, and FindFirstFileA lists the three merged (the first
//     spelling of a name wins): expand##.mix from the mod beside the installation's, and a mod's
//     map beside the installation's maps;
//   * writes never go into the mod: a write over a mod's file lands in the write folder;
//   * without a mod (None) every answer is the installation's or the write folder's, as before.
//   native-mod-layers <work folder>
#include "ra2port/win32_port.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <set>
#include <sys/stat.h>
#include <unistd.h>

struct FindData {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime, ftLastAccessTime, ftLastWriteTime;
    DWORD nFileSizeHigh, nFileSizeLow, dwReserved0, dwReserved1;
    char cFileName[260];
    char cAlternateFileName[14];
};
extern "C" {
HANDLE FindFirstFileA(const char *spec, FindData *data);
BOOL FindNextFileA(HANDLE handle, FindData *data);
BOOL FindClose(HANDLE handle);
}

// The game side's answer the port's PeekMessageA asks for (link/app_hooks.cpp in the game).
extern "C" int ra2port_game_redraw_pending(void) { return 0; }

static int failures = 0;
static void expect(bool ok, const char *what, const std::string &got = std::string())
{
    if (!ok) {
        ++failures;
        fprintf(stderr, "mod-layers: FAILED %s%s%s\n", what, got.empty() ? "" : ": got ", got.c_str());
    }
}
static void put(const std::string &path, const char *text)
{
    for (size_t i = 1; i < path.size(); ++i)
        if (path[i] == '/') mkdir(path.substr(0, i).c_str(), 0755);
    FILE *f = fopen(path.c_str(), "w");
    fputs(text, f);
    fclose(f);
}
static std::string read_back(const std::string &path)
{
    FILE *f = fopen(path.c_str(), "r");
    if (!f) return "(missing)";
    char buf[256] = {0};
    size_t n = fread(buf, 1, sizeof buf - 1, f);
    fclose(f);
    return std::string(buf, n);
}
static std::string resolved(const char *game_path)
{
    char out[1024];
    return read_back(ra2port::host_path(game_path, out, sizeof out));
}
static std::set<std::string> listed(const char *spec)
{
    std::set<std::string> names;
    FindData d;
    HANDLE h = FindFirstFileA(spec, &d);
    if (h == RA2_INVALID_HANDLE) return names;
    do {
        std::string n = d.cFileName;
        for (auto &c : n) c = (char)tolower((unsigned char)c);
        names.insert(n);
    } while (FindNextFileA(h, &d));
    FindClose(h);
    return names;
}
static std::string joined(const std::set<std::string> &s)
{
    std::string j;
    for (auto &n : s) j += (j.empty() ? "" : ",") + n;
    return j;
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "usage: native-mod-layers <work folder>\n");
        return 64;
    }
    std::string work = argv[1];
    std::string data = work + "/game", write = work + "/write", mod = work + "/Mods/Test";
    put(data + "/RULES.INI", "game rules");
    put(data + "/ART.INI", "game art");
    put(data + "/RA2.INI", "game settings");
    put(data + "/expandmd01.mix", "game expansion");
    put(data + "/Maps/one.map", "game map");
    put(write + "/ra2.ini", "player settings");
    put(mod + "/rules.ini", "mod rules");
    put(mod + "/Expandmd99.mix", "mod expansion");
    put(mod + "/MAPS/two.map", "mod map");
    ra2port::set_data_dir(data.c_str());
    ra2port::set_write_dir(write.c_str());

    // None: the installation and the write folder, as before.
    expect(resolved("RULES.INI") == "game rules", "no mod: RULES.INI", resolved("RULES.INI"));
    expect(resolved("ra2.ini") == "player settings", "no mod: RA2.INI from the write folder", resolved("ra2.ini"));
    expect(joined(listed("EXPAND*.MIX")) == "expandmd01.mix", "no mod: EXPAND*.MIX", joined(listed("EXPAND*.MIX")));
    expect(joined(listed("MAPS\\*.MAP")) == "one.map", "no mod: MAPS\\*.MAP", joined(listed("MAPS\\*.MAP")));

    ra2port::set_mod_dir(mod.c_str());
    expect(resolved("RULES.INI") == "mod rules", "mod: RULES.INI from the mod", resolved("RULES.INI"));
    expect(resolved("C:\\Rules.ini") == "mod rules", "mod: a drive path from the mod", resolved("C:\\Rules.ini"));
    expect(resolved("ART.INI") == "game art", "mod: ART.INI (not in the mod) from the installation", resolved("ART.INI"));
    expect(resolved("RA2.INI") == "player settings", "mod: RA2.INI (not in the mod) from the write folder", resolved("RA2.INI"));
    expect(joined(listed("EXPAND*.MIX")) == "expandmd01.mix,expandmd99.mix", "mod: EXPAND*.MIX merged",
           joined(listed("EXPAND*.MIX")));
    expect(joined(listed("MAPS\\*.MAP")) == "one.map,two.map", "mod: MAPS\\*.MAP merged", joined(listed("MAPS\\*.MAP")));
    expect(resolved("Maps\\two.map") == "mod map", "mod: a mod map read through the installation's folder",
           resolved("Maps\\two.map"));
    {
        char out[1024];
        std::string w = ra2port::host_path_write("RULES.INI", out, sizeof out);
        expect(w.compare(0, write.size(), write) == 0, "mod: a write over the mod's file goes to the write folder", w);
    }
    ra2port::set_mod_dir(nullptr);
    expect(resolved("RULES.INI") == "game rules", "back to None: RULES.INI", resolved("RULES.INI"));
    fprintf(stderr, "mod-layers: %s\n", failures ? "FAILED" : "ok");
    return failures ? 1 : 0;
}
