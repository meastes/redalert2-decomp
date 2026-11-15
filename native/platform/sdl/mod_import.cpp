// SPDX-License-Identifier: GPL-3.0-or-later
// Mods in the app (lane C16, mod support level 3): a .zip put in a Mods folder (on iOS the app's
// Documents/Mods, which the Files app shows) is unpacked into a folder of its name at the app's
// start, and a mod's optional mod.ini names and describes it for the Enhancements page's Mod row.
// Native app code, not game code.
//
// mod.ini (optional, in the mod's folder, any case):
//   [Mod]
//   Name=Shown on the Mod row (else the folder's name)
//   Description=Shown under the page when the row is chosen
#include "mod_import.hpp"

#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <string>
#include <vector>
#ifdef __APPLE__
#include <zlib.h>
#endif

namespace {
bool is_dir(const std::string &p)
{
    struct stat st;
    return stat(p.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}
bool make_dirs(const std::string &path)
{
    std::string at;
    for (size_t i = 0; i <= path.size(); ++i) {
        if (i == path.size() || path[i] == '/') {
            if (!at.empty() && !is_dir(at) && mkdir(at.c_str(), 0755) != 0) return false;
        }
        if (i < path.size()) at += path[i];
    }
    return true;
}
std::string trim(std::string s)
{
    size_t a = 0, b = s.size();
    while (a < b && (s[a] == ' ' || s[a] == '\t' || s[a] == '\r' || s[a] == '\n')) ++a;
    while (b > a && (s[b - 1] == ' ' || s[b - 1] == '\t' || s[b - 1] == '\r' || s[b - 1] == '\n')) --b;
    return s.substr(a, b - a);
}
uint32_t rd32(const unsigned char *p) { return p[0] | p[1] << 8 | p[2] << 16 | (uint32_t)p[3] << 24; }
uint16_t rd16(const unsigned char *p) { return (uint16_t)(p[0] | p[1] << 8); }

// A path inside the archive that stays inside the folder: no absolute path, no "..", no drive.
bool safe_name(const std::string &name)
{
    if (name.empty() || name[0] == '/' || name[0] == '\\' || name.find(':') != std::string::npos) return false;
    size_t start = 0;
    while (start <= name.size()) {
        size_t end = name.find_first_of("/\\", start);
        if (end == std::string::npos) end = name.size();
        if (name.compare(start, end - start, "..") == 0 && end - start == 2) return false;
        start = end + 1;
    }
    return true;
}

#ifdef __APPLE__
// Unpacks a .zip (stored and deflated entries; no ZIP64, no encryption) into folder. When every entry
// sits in one top folder, that folder's contents go to folder (a zipped mod folder). False on an
// archive it cannot read; nothing half written is left as the mod (the caller unpacks into a temporary
// folder and renames it).
bool unzip(const std::string &zip, const std::string &folder)
{
    FILE *f = fopen(zip.c_str(), "rb");
    if (!f) return false;
    std::vector<unsigned char> data;
    unsigned char buf[65536];
    size_t n;
    while ((n = fread(buf, 1, sizeof buf, f)) > 0) data.insert(data.end(), buf, buf + n);
    fclose(f);
    if (data.size() < 22) return false;
    size_t eocd = std::string::npos;
    for (size_t i = data.size() - 22 + 1; i-- > 0 && data.size() - i <= 22 + 65535;) {
        if (rd32(&data[i]) == 0x06054b50) { eocd = i; break; }
    }
    if (eocd == std::string::npos) return false;
    uint32_t entries = rd16(&data[eocd + 10]), dir_size = rd32(&data[eocd + 12]), dir_at = rd32(&data[eocd + 16]);
    if ((uint64_t)dir_at + dir_size > data.size()) return false;
    struct Entry { std::string name; uint32_t method, comp, size, local; };
    std::vector<Entry> list;
    size_t p = dir_at;
    for (uint32_t i = 0; i < entries; ++i) {
        if (p + 46 > data.size() || rd32(&data[p]) != 0x02014b50) return false;
        uint16_t flags = rd16(&data[p + 8]);
        Entry e;
        e.method = rd16(&data[p + 10]);
        e.comp = rd32(&data[p + 20]);
        e.size = rd32(&data[p + 24]);
        uint16_t name_len = rd16(&data[p + 28]), extra = rd16(&data[p + 30]), comment = rd16(&data[p + 32]);
        e.local = rd32(&data[p + 42]);
        if (p + 46 + name_len > data.size()) return false;
        e.name.assign((const char *)&data[p + 46], name_len);
        p += 46 + name_len + extra + comment;
        if (flags & 1) return false;                                           // encrypted
        if (e.comp == 0xFFFFFFFFu || e.size == 0xFFFFFFFFu || e.local == 0xFFFFFFFFu) return false;   // ZIP64
        for (char &c : e.name) if (c == '\\') c = '/';
        if (e.name.compare(0, 9, "__MACOSX/") == 0) continue;               // Finder's resource forks
        if (!safe_name(e.name)) return false;
        list.push_back(e);
    }
    // One top folder holding everything: strip it.
    std::string top;
    bool one_top = !list.empty();
    for (const Entry &e : list) {
        size_t slash = e.name.find('/');
        std::string first = slash == std::string::npos ? std::string() : e.name.substr(0, slash + 1);
        if (first.empty() || (!top.empty() && first != top)) { one_top = false; break; }
        top = first;
    }
    if (!make_dirs(folder)) return false;
    for (const Entry &e : list) {
        std::string name = one_top ? e.name.substr(top.size()) : e.name;
        if (name.empty()) continue;
        std::string out = folder + "/" + name;
        if (name.back() == '/') {
            if (!make_dirs(out.substr(0, out.size() - 1))) return false;
            continue;
        }
        size_t slash = out.find_last_of('/');
        if (!make_dirs(out.substr(0, slash))) return false;
        if ((uint64_t)e.local + 30 > data.size() || rd32(&data[e.local]) != 0x04034b50) return false;
        size_t at = e.local + 30 + rd16(&data[e.local + 26]) + rd16(&data[e.local + 28]);
        if ((uint64_t)at + e.comp > data.size()) return false;
        std::vector<unsigned char> bytes(e.size);
        if (e.method == 0) {
            if (e.comp != e.size) return false;
            if (e.size) memcpy(bytes.data(), &data[at], e.size);
        } else if (e.method == 8) {
            z_stream z = {};
            if (inflateInit2(&z, -15) != Z_OK) return false;
            z.next_in = &data[at];
            z.avail_in = e.comp;
            z.next_out = bytes.data();
            z.avail_out = e.size;
            int r = inflate(&z, Z_FINISH);
            inflateEnd(&z);
            if (r != Z_STREAM_END || z.total_out != e.size) return false;
        } else {
            return false;                                                      // another method
        }
        FILE *o = fopen(out.c_str(), "wb");
        if (!o) return false;
        bool ok = fwrite(bytes.data(), 1, bytes.size(), o) == bytes.size();
        fclose(o);
        if (!ok) return false;
    }
    return true;
}
#endif

void remove_tree(const std::string &path)
{
    if (DIR *d = opendir(path.c_str())) {
        while (struct dirent *e = readdir(d)) {
            if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
            std::string p = path + "/" + e->d_name;
            if (is_dir(p)) remove_tree(p); else remove(p.c_str());
        }
        closedir(d);
    }
    rmdir(path.c_str());
}
}  // namespace

namespace mods {

int import_zips(const std::string &mods_dir)
{
    int imported = 0;
#ifdef __APPLE__
    DIR *d = opendir(mods_dir.c_str());
    if (!d) return 0;
    std::vector<std::string> zips;
    while (struct dirent *e = readdir(d)) {
        size_t len = strlen(e->d_name);
        if (e->d_name[0] != '.' && len > 4 && !strcasecmp(e->d_name + len - 4, ".zip")) zips.push_back(e->d_name);
    }
    closedir(d);
    for (const std::string &zip : zips) {
        std::string name = zip.substr(0, zip.size() - 4);
        std::string folder = mods_dir + "/" + name;
        if (is_dir(folder)) continue;                                          // already unpacked
        std::string temp = mods_dir + "/.importing-" + name;
        remove_tree(temp);
        if (unzip(mods_dir + "/" + zip, temp) && rename(temp.c_str(), folder.c_str()) == 0) {
            fprintf(stderr, "mods: %s unpacked into Mods/%s\n", zip.c_str(), name.c_str());
            ++imported;
        } else {
            remove_tree(temp);
            fprintf(stderr, "mods: %s could not be unpacked (a .zip of stored or deflated files is needed)\n", zip.c_str());
        }
    }
#else
    (void)mods_dir;
#endif
    return imported;
}

bool manifest(const std::string &mod_dir, std::string *name, std::string *description)
{
    std::string path;
    if (DIR *d = opendir(mod_dir.c_str())) {
        while (struct dirent *e = readdir(d))
            if (!strcasecmp(e->d_name, "mod.ini")) { path = mod_dir + "/" + e->d_name; break; }
        closedir(d);
    }
    FILE *f = path.empty() ? nullptr : fopen(path.c_str(), "r");
    if (!f) return false;
    char line[1024];
    bool in_mod = false, found = false;
    while (fgets(line, sizeof line, f)) {
        std::string l = trim(line);
        if (l.size() >= 3 && (unsigned char)l[0] == 0xEF && (unsigned char)l[1] == 0xBB && (unsigned char)l[2] == 0xBF) l = trim(l.substr(3));
        if (l.empty() || l[0] == ';' || l[0] == '#') continue;
        if (l[0] == '[') { in_mod = !strcasecmp(l.c_str(), "[Mod]"); continue; }
        size_t eq = l.find('=');
        if (!in_mod || eq == std::string::npos) continue;
        std::string key = trim(l.substr(0, eq)), value = trim(l.substr(eq + 1));
        if (!strcasecmp(key.c_str(), "Name") && !value.empty()) { if (name) *name = value; found = true; }
        else if (!strcasecmp(key.c_str(), "Description") && !value.empty()) { if (description) *description = value; found = true; }
    }
    fclose(f);
    return found;
}

}  // namespace mods
