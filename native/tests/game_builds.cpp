// The builds the port reads its stored data from (ra2port/game_builds.hpp; lane N15): the Steam
// game.exe and gamemd.exe (The Ultimate Collection) are recognised, and against the builds the
// port already reads (the target game.exe, the retail gamemd.exe 1.001) they differ, in what the
// port reads, only by what game_builds.hpp records: the .rdata/.data bytes that differ are the
// known few and lie outside every global the port fills (data/original_data.cpp,
// gen-yr/yr_original_data.cpp), and every resource but the icons is byte-identical.
//
//   native-game-builds <private dir> <target game.exe> <retail gamemd.exe> <original_data.cpp>
//                      <yr_original_data.cpp>
// The executables are the user's (private/steam/, never in the repository); a missing Steam copy
// skips the test (exit 77), a missing comparison build skips that comparison.
#include "ra2port/game_builds.hpp"

#include <stdio.h>
#include <stdlib.h>

#include <map>
#include <string>
#include <utility>
#include <vector>

namespace {

int failures = 0;
void check(bool ok, std::string const &what)
{
    printf("%s %s\n", ok ? "ok  " : "FAIL", what.c_str());
    if (!ok) ++failures;
}

bool read_file(std::string const &path, std::vector<unsigned char> &out)
{
    FILE *f = fopen(path.c_str(), "rb");
    if (!f) return false;
    unsigned char buf[65536];
    size_t n;
    out.clear();
    while ((n = fread(buf, 1, sizeof buf, f)) > 0) out.insert(out.end(), buf, buf + n);
    fclose(f);
    return true;
}

std::string sha(const unsigned char *p, size_t n)
{
    ra2build::Sha256 s;
    s.update(p, n);
    char h[65];
    s.hex(h);
    return h;
}

ra2build::Kind classify(std::vector<unsigned char> const &f, ra2build::Game game, std::string *why = nullptr)
{
    return ra2build::classify(f.data(), f.size(), sha(f.data(), f.size()), sha, why, game);
}

// The globals the port fills: (address, extent) from a generated table's rows, whose first two
// numbers are the address (0x...) and the extent ("{storage, 0x0079A6A0u, 88u, 0}" or
// "{0x0079A6A0u, 88u, storage, 0}").
std::vector<std::pair<uint32_t, uint32_t>> table_rows(std::string const &path)
{
    std::vector<std::pair<uint32_t, uint32_t>> rows;
    FILE *f = fopen(path.c_str(), "r");
    if (!f) return rows;
    char line[4096];
    while (fgets(line, sizeof line, f)) {
        char *p = line;
        while (*p == ' ') ++p;
        if (*p != '{') continue;
        char *x = strstr(p, "0x");
        if (!x) continue;
        char *end;
        unsigned long address = strtoul(x, &end, 16);
        while (*end == 'u' || *end == ',' || *end == ' ') ++end;
        unsigned long extent = strtoul(end, nullptr, 10);
        rows.push_back({(uint32_t)address, (uint32_t)extent});
    }
    fclose(f);
    return rows;
}

struct Sections {
    uint32_t va[4], vsize[4], raw[4], rawsize[4];
    uint32_t rsrc_rva = 0;
};
Sections sections(std::vector<unsigned char> const &f)
{
    Sections s{};
    uint32_t pe = ra2build::rd32(f.data() + 0x3C);
    const unsigned char *o = f.data() + pe + 24, *h = o + ra2build::rd16(f.data() + pe + 20);
    s.rsrc_rva = ra2build::rd32(o + 96 + 16);
    for (int i = 0; i < 4; ++i, h += 40) {
        s.vsize[i] = ra2build::rd32(h + 8);
        s.va[i] = ra2build::rd32(h + 12);
        s.rawsize[i] = ra2build::rd32(h + 16);
        s.raw[i] = ra2build::rd32(h + 20);
    }
    return s;
}

// Every resource leaf: (type/name/language) -> its bytes.
std::map<std::string, std::string> resources(std::vector<unsigned char> const &f)
{
    std::map<std::string, std::string> out;
    Sections s = sections(f);
    auto file_at = [&](uint32_t rva) -> size_t {
        for (int i = 0; i < 4; ++i)
            if (rva >= s.va[i] && rva < s.va[i] + s.rawsize[i]) return s.raw[i] + (rva - s.va[i]);
        return 0;
    };
    size_t base = file_at(s.rsrc_rva);
    if (!base) return out;
    struct Walk {
        std::vector<unsigned char> const &f;
        size_t base;
        std::map<std::string, std::string> &out;
        decltype(file_at) &at;
        std::string name(uint32_t e)
        {
            if (!(e & 0x80000000u)) return std::to_string(e);
            size_t p = base + (e & 0x7FFFFFFFu);
            std::string n;
            for (uint16_t k = 0, len = ra2build::rd16(f.data() + p); k < len; ++k) n += (char)f[p + 2 + 2 * k];
            return n;
        }
        void dir(size_t p, std::string const &path, int depth)
        {
            if (depth > 3 || p + 16 > f.size()) return;
            unsigned count = ra2build::rd16(f.data() + p + 12) + ra2build::rd16(f.data() + p + 14);
            for (unsigned k = 0; k < count; ++k) {
                uint32_t n = ra2build::rd32(f.data() + p + 16 + 8 * k), e = ra2build::rd32(f.data() + p + 20 + 8 * k);
                std::string here = path + "/" + name(n);
                if (e & 0x80000000u) dir(base + (e & 0x7FFFFFFFu), here, depth + 1);
                else {
                    const unsigned char *leaf = f.data() + base + e;
                    size_t from = at(ra2build::rd32(leaf)), size = ra2build::rd32(leaf + 4);
                    out[here] = from && from + size <= f.size() ? std::string((const char *)f.data() + from, size) : std::string();
                }
            }
        }
    } walk{f, base, out, file_at};
    walk.dir(base, "", 0);
    return out;
}

// The Steam build against the build the port already reads: data and resources.
void compare(char const *label, std::vector<unsigned char> const &known, std::vector<unsigned char> const &steam,
             std::vector<std::pair<uint32_t, uint32_t>> const &rows, std::vector<std::pair<uint32_t, uint32_t>> const &expected)
{
    Sections a = sections(known), b = sections(steam);
    for (int i = 0; i < 3; ++i) check(a.va[i] == b.va[i] && a.raw[i] == b.raw[i] && a.rawsize[i] == b.rawsize[i] && a.vsize[i] == b.vsize[i],
                                      std::string(label) + ": section " + std::to_string(i) + " at the same place");
    check(a.va[3] == b.va[3] && a.raw[3] == b.raw[3], std::string(label) + ": .rsrc starts at the same place");
    // .rdata and .data: the runs of differing bytes.
    std::vector<std::pair<uint32_t, uint32_t>> runs;
    for (int i = 1; i <= 2; ++i)
        for (uint32_t k = 0; k < a.rawsize[i]; ++k) {
            if (known[a.raw[i] + k] == steam[b.raw[i] + k]) continue;
            uint32_t va = ra2build::kTargetImageBase + a.va[i] + k;
            if (!runs.empty() && runs.back().first + runs.back().second == va) ++runs.back().second;
            else runs.push_back({va, 1});
        }
    check(runs == expected, std::string(label) + ": .rdata/.data differ by " + std::to_string(runs.size()) + " known runs only");
    for (auto const &r : runs) {
        printf("     differs at 0x%08X, %u bytes\n", r.first, r.second);
        bool read = false;
        for (auto const &row : rows)
            if (r.first < row.first + (row.second ? row.second : 4) && row.first < r.first + r.second) read = true;
        check(!read, std::string(label) + ": no global the port fills covers it");
    }
    check(!rows.empty(), std::string(label) + ": the port's table was read (" + std::to_string(rows.size()) + " rows)");
    // Resources: all but the icons identical.
    std::map<std::string, std::string> ra = resources(known), rb = resources(steam);
    size_t same = 0, icons = 0;
    bool others_same = !ra.empty();
    for (auto const &r : ra) {
        bool icon = r.first.rfind("/3/", 0) == 0 || r.first.rfind("/14/", 0) == 0;   // RT_ICON, RT_GROUP_ICON
        if (icon) { ++icons; continue; }
        auto it = rb.find(r.first);
        if (it != rb.end() && it->second == r.second) ++same;
        else { others_same = false; printf("     resource %s differs\n", r.first.c_str()); }
    }
    for (auto const &r : rb)
        if (!ra.count(r.first) && r.first.rfind("/3/", 0) != 0) { others_same = false; printf("     resource %s added\n", r.first.c_str()); }
    check(others_same, std::string(label) + ": " + std::to_string(same) + " dialogs, menus, cursors and version resources identical (" +
                           std::to_string(icons) + " icon resources not compared)");
}

}  // namespace

int main(int argc, char **argv)
{
    if (argc < 6) { fprintf(stderr, "usage: %s <private dir> <target game.exe> <gamemd.exe> <original_data.cpp> <yr_original_data.cpp>\n", argv[0]); return 2; }
    std::string dir = argv[1];
    std::vector<unsigned char> steam_ra2, steam_yr, target, retail_yr;
    bool have_ra2 = read_file(dir + "/steam/game.exe", steam_ra2), have_yr = read_file(dir + "/steam/gamemd.exe", steam_yr);
    if (!have_ra2 && !have_yr) { printf("skip: no Steam game.exe or gamemd.exe in %s/steam\n", dir.c_str()); return 77; }
    if (have_ra2) {
        std::string why;
        check(steam_ra2.size() == 5077312, "Steam game.exe: 5,077,312 bytes");
        check(classify(steam_ra2, ra2build::Game::ra2, &why) == ra2build::Kind::same_data, "Steam game.exe read at the target's addresses " + why);
        check(classify(steam_ra2, ra2build::Game::yr) == ra2build::Kind::other, "Steam game.exe refused as gamemd.exe");
        // One changed byte of read data (a table in .data) is refused.
        std::vector<unsigned char> changed = steam_ra2;
        changed[0x3c9000 + 0x1000] ^= 1;
        check(classify(changed, ra2build::Game::ra2, &why) == ra2build::Kind::other && why == "different data in .data",
              "Steam game.exe with one .data byte changed refused (" + why + ")");
        if (read_file(argv[2], target)) {
            check(classify(target, ra2build::Game::ra2) == ra2build::Kind::target, "target game.exe still the target");
            compare("Steam game.exe vs target", target, steam_ra2, table_rows(argv[4]), {{0x0079A619, 2}, {0x007F9930, 3}});
        } else printf("skip: no target game.exe at %s\n", argv[2]);
    } else printf("skip: no Steam game.exe\n");
    if (have_yr) {
        std::string why;
        check(steam_yr.size() == 5286208, "Steam gamemd.exe: 5,286,208 bytes");
        check(classify(steam_yr, ra2build::Game::yr, &why) == ra2build::Kind::same_data, "Steam gamemd.exe read at 1.001's addresses " + why);
        check(classify(steam_yr, ra2build::Game::ra2) == ra2build::Kind::other, "Steam gamemd.exe refused as game.exe");
        std::vector<unsigned char> changed = steam_yr;
        changed[0x412000 + 0x1000] ^= 1;
        check(classify(changed, ra2build::Game::yr, &why) == ra2build::Kind::other && why == "different data in .data",
              "Steam gamemd.exe with one .data byte changed refused (" + why + ")");
        if (read_file(argv[3], retail_yr)) {
            check(classify(retail_yr, ra2build::Game::yr) == ra2build::Kind::same_data, "retail gamemd.exe still read");
            std::vector<std::pair<uint32_t, uint32_t>> rows = table_rows(argv[5]);
            if (rows.empty()) printf("skip: no YR table at %s (YR not configured)\n", argv[5]);
            else compare("Steam gamemd.exe vs retail 1.001", retail_yr, steam_yr, rows, {{0x007E1629, 2}, {0x00846370, 3}});
        } else printf("skip: no retail gamemd.exe at %s\n", argv[3]);
    } else printf("skip: no Steam gamemd.exe\n");
    printf("%s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
