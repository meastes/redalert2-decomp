// SPDX-License-Identifier: GPL-3.0-or-later
// The app mode of the SDL host (packaging/README.md): what RA2.app adds to the
// plain ra2-native binary when it is launched from Finder (and, later, the iOS app).
//   - First-run setup: the user picks the installed game folder (validated:
//     its MIX files), and the supported game.exe is located (the folder's own,
//     or one the user picks when the folder carries another build). The choice
//     is kept in the settings folder (hostos::settings_dir; on the Mac
//     ~/Library/Application Support/RA2Native/settings).
//   - Writes (RA2.INI, saved games) go to hostos::game_write_dir (on the Mac
//     ~/Library/Application Support/RA2Native/Game), never into the installation.
//   - stdout/stderr go to hostos::log_dir()/ra2-native.log (on the Mac
//     ~/Library/Logs/RA2Native/ra2-native.log).
//   - A trap (unrecovered code reached) or a fault shows a dialog naming the
//     function and the log, instead of a silent exit.
// Included once, by main.cpp. Platform services (paths, the crash dialog's new
// process, the file manager) come from host_os.hpp (Scout1247): no Apple-only
// calls here.
#pragma once
#include <SDL3/SDL.h>
#include <cxxabi.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <ctype.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "ra2port/game_builds.hpp"
#include "host_os.hpp"

namespace app {

// The supported game.exe (AGENTS.md "Fixed constraints"; the same check as
// port/src/original_image.cpp, which reads its data).
using ra2build::kTargetSize;
using ra2build::kTargetSha256;

std::string support_dir() { return hostos::settings_dir(); }
std::string game_write_dir() { return hostos::game_write_dir(); }
std::string log_dir() { return hostos::log_dir(); }
std::string settings_path()
{
    // RA2_NATIVE_SETTINGS: another settings file (tests of the first run).
    const char *s = getenv("RA2_NATIVE_SETTINGS");
    return s && *s ? s : support_dir() + "/settings";
}

void make_dirs(std::string const &path)
{
    for (size_t i = 1; i <= path.size(); ++i) {
        if (i == path.size() || path[i] == '/') mkdir(path.substr(0, i).c_str(), 0755);
    }
}
bool is_dir(std::string const &path)
{
    struct stat st;
    return !path.empty() && stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}
bool is_file(std::string const &path)
{
    struct stat st;
    return !path.empty() && stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

std::string exe_path() { return hostos::executable_path(); }
// Launched as an app (RA2.app's executable, Contents/MacOS/; the iOS app).
bool in_bundle() { return hostos::launched_as_app(); }

// ---- settings: "key=value" lines ---------------------------------------
struct Settings {
    std::map<std::string, std::string> values;
    std::string get(const char *key) const
    {
        auto i = values.find(key);
        return i == values.end() ? "" : i->second;
    }
    void load()
    {
        FILE *f = fopen(settings_path().c_str(), "r");
        if (!f) return;
        char line[4096];
        while (fgets(line, sizeof line, f)) {
            size_t n = strlen(line);
            while (n && (line[n - 1] == '\n' || line[n - 1] == '\r')) line[--n] = 0;
            if (!*line || *line == '#') continue;
            char *eq = strchr(line, '=');
            if (!eq) continue;
            *eq = 0;
            values[line] = eq + 1;
        }
        fclose(f);
    }
    void save() const
    {
        std::string path = settings_path();
        size_t slash = path.rfind('/');
        if (slash != std::string::npos) make_dirs(path.substr(0, slash));
        std::string tmp = path + ".tmp";
        FILE *f = fopen(tmp.c_str(), "w");
        if (!f) return;
        fprintf(f, "# Red Alert 2 (native) settings: written by the app\n");
        for (auto const &kv : values) fprintf(f, "%s=%s\n", kv.first.c_str(), kv.second.c_str());
        fclose(f);
        rename(tmp.c_str(), path.c_str());
    }
};

// ---- the installation ---------------------------------------------------
// A file in dir by name, any case (installations differ: RA2.MIX, ra2.mix).
std::string find_file(std::string const &dir, const char *name)
{
    DIR *d = opendir(dir.c_str());
    if (!d) return "";
    std::string found;
    while (struct dirent *e = readdir(d)) {
        if (!strcasecmp(e->d_name, name)) { found = dir + "/" + e->d_name; break; }
    }
    closedir(d);
    return found;
}
// The MIX files a base Red Alert 2 installation has: the missing ones, or "".
std::string missing_mix(std::string const &dir)
{
    static const char *const required[] = {"ra2.mix", "language.mix", "multi.mix", "theme.mix"};
    std::string missing;
    for (const char *name : required) {
        if (!is_file(find_file(dir, name))) missing += (missing.empty() ? "" : ", ") + std::string(name);
    }
    return missing;
}

enum class Exe { missing, supported, other };
// Why the last checked game.exe was refused (ra2build::classify).
std::string &exe_reason() { static std::string r; return r; }
Exe check_exe(std::string const &path, std::string &digest, long long &size)
{
    digest.clear();
    size = 0;
    FILE *f = path.empty() ? nullptr : fopen(path.c_str(), "rb");
    if (!f) return Exe::missing;
    ra2build::Sha256 whole;
    unsigned char buf[65536];
    size_t n;
    std::vector<unsigned char> file;
    while ((n = fread(buf, 1, sizeof buf, f)) > 0) {
        whole.update(buf, n);
        file.insert(file.end(), buf, buf + n);
        size += (long long)n;
    }
    fclose(f);
    char hex[65];
    whole.hex(hex);
    digest = hex;
    // The target, or another build with the target's data (ra2port/game_builds.hpp).
    ra2build::Kind kind = ra2build::classify(file.data(), file.size(), digest, [](unsigned char const *p, size_t len) {
        ra2build::Sha256 part;
        part.update(p, len);
        char h[65];
        part.hex(h);
        return std::string(h);
    }, &exe_reason());
    return kind == ra2build::Kind::other ? Exe::other : Exe::supported;
}

// ---- dialogs (SDL: NSAlert and NSOpenPanel on macOS) --------------------
// RA2_NATIVE_SETUP_ANSWERS (tests of the first run without a person): the
// answers in order, separated by '|': a button number for each message box, a
// path for each file or folder dialog ("" cancels). Every dialog is logged.
bool scripted_answer(std::string &answer)
{
    static std::vector<std::string> answers;
    static bool loaded = false;
    static size_t next = 0;
    const char *s = getenv("RA2_NATIVE_SETUP_ANSWERS");
    if (!s) return false;
    if (!loaded) {
        loaded = true;
        std::string all = s;
        size_t start = 0;
        for (;;) {
            size_t bar = all.find('|', start);
            answers.push_back(all.substr(start, bar == std::string::npos ? std::string::npos : bar - start));
            if (bar == std::string::npos) break;
            start = bar + 1;
        }
    }
    answer = next < answers.size() ? answers[next++] : "";
    return true;
}

int ask(const char *title, std::string const &message, std::vector<const char *> const &buttons, bool warning = false)
{
    // SDL lays buttons out right to left on macOS: the first is the default.
    std::vector<SDL_MessageBoxButtonData> data;
    for (size_t i = 0; i < buttons.size(); ++i) {
        SDL_MessageBoxButtonData b = {};
        b.buttonID = (int)i;
        b.text = buttons[i];
        if (i == 0) b.flags |= SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT;
        if (i + 1 == buttons.size() && buttons.size() > 1) b.flags |= SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT;
        data.push_back(b);
    }
    SDL_MessageBoxData box = {};
    box.flags = warning ? SDL_MESSAGEBOX_WARNING : SDL_MESSAGEBOX_INFORMATION;
    box.title = title;
    box.message = message.c_str();
    box.numbuttons = (int)data.size();
    box.buttons = data.data();
    int pressed = -1;
    std::string answer;
    if (scripted_answer(answer)) pressed = answer.empty() ? -1 : atoi(answer.c_str());
    else if (!SDL_ShowMessageBox(&box, &pressed)) fprintf(stderr, "app: message box: %s\n", SDL_GetError());
    std::string flat = message;
    for (char &c : flat) if (c == '\n') c = ' ';
    fprintf(stderr, "app: dialog \"%s\" -> %s\n", flat.c_str(), pressed >= 0 && pressed < (int)buttons.size() ? buttons[pressed] : "(closed)");
    return pressed;
}

struct Pick { bool done = false; std::string path; };
void SDLCALL picked(void *user, const char *const *files, int)
{
    Pick *p = (Pick *)user;
    if (files && files[0]) p->path = files[0];
    else if (!files) fprintf(stderr, "app: file dialog: %s\n", SDL_GetError());
    p->done = true;
}
std::string wait_pick(Pick &p)
{
    while (!p.done) {
        SDL_Event e;
        SDL_WaitEventTimeout(&e, 50);
    }
    return p.path;
}
std::string pick_folder(std::string const &start)
{
    Pick p;
    std::string answer;
    if (scripted_answer(answer)) p.path = answer, p.done = true;
    else SDL_ShowOpenFolderDialog(picked, &p, nullptr, start.empty() ? "/Applications" : start.c_str(), false);
    std::string path = wait_pick(p);
    fprintf(stderr, "app: folder chosen: %s\n", path.empty() ? "(cancelled)" : path.c_str());
    return path;
}
std::string pick_exe(std::string const &start)
{
    Pick p;
    static const SDL_DialogFileFilter filters[] = {{"Windows programs (game.exe)", "exe"}, {"All files", "*"}};
    std::string answer;
    if (scripted_answer(answer)) p.path = answer, p.done = true;
    else SDL_ShowOpenFileDialog(picked, &p, nullptr, filters, 2, start.c_str(), false);
    std::string path = wait_pick(p);
    fprintf(stderr, "app: file chosen: %s\n", path.empty() ? "(cancelled)" : path.c_str());
    return path;
}

const char kTitle[] = "Red Alert 2";

// First-run setup (and every start: the saved choice is checked again).
// Returns false when the user quits. RA2_NATIVE_SETUP=1 (or --setup) asks for
// the folder even when one is saved.
bool setup(Settings &settings, bool force, std::string &data_dir, std::string &game_exe)
{
    std::string dir = settings.get("data_dir");
    std::string note;
    if (!force && !dir.empty() && !is_dir(dir))
        note = "The game folder chosen before is not available:\n" + dir + "\n(Is its drive connected?)\n\n";
    while (force || dir.empty() || !is_dir(dir) || !missing_mix(dir).empty()) {
        if (!force && is_dir(dir) && note.empty())
            note = dir + "\nno longer looks like a Red Alert 2 folder (missing " + missing_mix(dir) + ").\n\n";
        force = false;
        int b = ask(kTitle, note + "Choose the folder where Red Alert 2 is installed (it holds ra2.mix and game.exe).\n\n"
                    "The game reads its files from there and never changes them; settings and saved games are "
                    "kept in your Library.", {"Choose Folder…", "Quit"});
        if (b != 0) return false;
        std::string chosen = pick_folder(is_dir(dir) ? dir : "");
        if (chosen.empty()) return false;
        while (chosen.size() > 1 && chosen.back() == '/') chosen.pop_back();
        dir = chosen;
        std::string missing = missing_mix(dir);
        note = missing.empty() ? "" : dir + "\ndoes not look like a Red Alert 2 installation: " + missing +
                                          (missing.find(',') != std::string::npos ? " are" : " is") + " missing.\n\n";
        if (!missing.empty()) {
            force = true;
            dir.clear();
        }
    }
    data_dir = dir;
    settings.values["data_dir"] = dir;

    // game.exe: the one chosen before, else the folder's own, else ask.
    std::string digest;
    long long size = 0;
    std::string exe = settings.get("game_exe");
    if (!exe.empty() && check_exe(exe, digest, size) == Exe::supported) {
        game_exe = exe;
        settings.save();
        return true;
    }
    std::string installed = find_file(dir, "game.exe");
    Exe state = check_exe(installed, digest, size);
    if (state == Exe::supported) {
        game_exe = installed;
        settings.values.erase("game_exe");
        settings.save();
        return true;
    }
    auto bytes = [](long long n) {
        std::string digits = std::to_string(n), out;
        for (size_t i = 0; i < digits.size(); ++i) {
            if (i && (digits.size() - i) % 3 == 0) out += ',';
            out += digits[i];
        }
        return out + " bytes";
    };
    std::string sizes = bytes(size);
    std::string why = state == Exe::missing
        ? "There is no game.exe in\n" + dir + "\n\n"
        : "The game.exe in this folder is a build of Red Alert 2 the native game cannot read (" + sizes + ", SHA-256 " +
              digest.substr(0, 16) + "…: " + exe_reason() + ").\n\n";
    std::string supported = std::string("The native game reads its tables and dialogs from ") +
                            ra2build::supported_builds_text() +
                            ". Any other build is refused: its data may sit at other addresses, and reading it there "
                            "would give wrong tables.\n\nIf you have a supported game.exe elsewhere (for example a "
                            "backup), choose it. Otherwise the game cannot start.";
    for (;;) {
        int b = ask(kTitle, why + supported, {"Choose game.exe…", "Quit"}, true);
        if (b != 0) return false;
        std::string chosen = pick_exe(dir);
        if (chosen.empty()) return false;
        Exe c = check_exe(chosen, digest, size);
        if (c == Exe::supported) {
            game_exe = chosen;
            settings.values["game_exe"] = chosen;
            settings.save();
            return true;
        }
        sizes = bytes(size);
        why = c == Exe::missing ? chosen + "\ncannot be read.\n\n"
                                : chosen + "\nis not a supported game.exe either (" + sizes + ", SHA-256 " +
                                      digest.substr(0, 16) + "…: " + exe_reason() + ").\n\n";
    }
}

// ---- iOS: the game files imported once into the app (Scout1247) ----------
// An iOS app cannot read a folder in place from one start to the next, so the
// first start copies the files the native game reads from the folder the user
// picks (Files: a Mac share, a USB drive, On My iPhone) into the app's own
// Library/Application Support/RA2Native/GameData, names lower-cased (the
// container's volume is case-sensitive; the port's lookups ignore case), then
// checks them: the four base MIX files, a supported game.exe and BINKW32.DLL.
// The copy is built beside the old one and swapped in by rename, so a failed or
// cancelled import keeps the previous files. Later starts skip the picker.
// Only these names are copied (base Red Alert 2; Yuri's Revenge files and
// everything else stay behind): never code to run, only data and the two
// Windows files the port reads data from (game.exe's tables and dialogs,
// BINKW32.DLL's Bink tables).
bool import_wanted(std::string const &lower)
{
    auto ends = [&](const char *suffix) {
        size_t n = strlen(suffix);
        return lower.size() > n && lower.compare(lower.size() - n, n, suffix) == 0;
    };
    auto numbered = [&](const char *stem, const char *ext) {   // stem + digits + ext
        size_t a = strlen(stem), b = strlen(ext);
        if (lower.size() <= a + b || lower.compare(0, a, stem) != 0 || !ends(ext)) return false;
        for (size_t i = a; i < lower.size() - b; ++i) if (lower[i] < '0' || lower[i] > '9') return false;
        return true;
    };
    if (lower == "game.exe" || lower == "binkw32.dll") return true;
    for (const char *name : {"ra2.mix", "language.mix", "multi.mix", "theme.mix", "wdt.mix"}) if (lower == name) return true;
    for (const char *stem : {"maps", "movies", "expand", "ecache", "elocal"}) if (numbered(stem, ".mix")) return true;
    for (const char *ext : {".mmx", ".map", ".mpr", ".ini", ".csf", ".pkt"}) if (ends(ext)) return true;
    return false;
}

std::string game_data_dir() { return support_dir() + "/GameData"; }

// The imported files are complete: "" or why not.
std::string imported_problem(std::string const &dir)
{
    if (!is_dir(dir)) return "no game files have been imported";
    std::string missing = missing_mix(dir);
    if (!missing.empty()) return "missing " + missing;
    if (!is_file(dir + "/binkw32.dll")) return "missing BINKW32.DLL";
    std::string digest;
    long long size = 0;
    Exe exe = check_exe(dir + "/game.exe", digest, size);
    if (exe == Exe::missing) return "missing game.exe";
    if (exe == Exe::other) return "game.exe is a build the native game cannot read (" + exe_reason() + ")";
    return "";
}

bool copy_file(std::string const &from, std::string const &to)
{
    int in = open(from.c_str(), O_RDONLY);
    if (in < 0) return false;
    int out = open(to.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    bool ok = out >= 0;
    static char buf[1 << 20];
    while (ok) {
        ssize_t n = read(in, buf, sizeof buf);
        if (n == 0) break;
        if (n < 0) { if (errno == EINTR) continue; ok = false; break; }
        for (ssize_t done = 0; ok && done < n;) {
            ssize_t w = write(out, buf + done, (size_t)(n - done));
            if (w < 0 && errno == EINTR) continue;
            if (w <= 0) ok = false;
            else done += w;
        }
    }
    if (out >= 0 && close(out) != 0) ok = false;
    close(in);
    return ok;
}

void remove_tree(std::string const &path)
{
    if (DIR *d = opendir(path.c_str())) {
        while (struct dirent *e = readdir(d)) {
            if (!strcmp(e->d_name, ".") || !strcmp(e->d_name, "..")) continue;
            std::string child = path + "/" + e->d_name;
            if (is_dir(child)) remove_tree(child);
            else unlink(child.c_str());
        }
        closedir(d);
    }
    rmdir(path.c_str());
}

// A start after an import that stopped between its two renames finds the
// previous files under GameData.previous: they are put back.
void recover_import()
{
    std::string dir = game_data_dir(), previous = dir + ".previous";
    if (!is_dir(dir) && is_dir(previous)) rename(previous.c_str(), dir.c_str());
}

// Copies the wanted files of source into GameData; "" or the reason it failed
// (the previous files are kept then).
std::string import_game_files(std::string source)
{
    while (source.size() > 1 && source.back() == '/') source.pop_back();
    std::string dir = game_data_dir(), staging = dir + ".importing", previous = dir + ".previous";
    char real_source[PATH_MAX], real_support[PATH_MAX];
    make_dirs(support_dir());
    if (realpath(source.c_str(), real_source) && realpath(support_dir().c_str(), real_support) &&
        !strncmp(real_source, real_support, strlen(real_support)))
        return "choose a folder outside the app's own files";
    remove_tree(staging);
    make_dirs(staging);
    DIR *d = opendir(source.c_str());
    if (!d) return "the folder cannot be read (" + std::string(strerror(errno)) + ")";
    std::vector<std::string> names;
    std::map<std::string, std::string> by_lower;
    std::string problem;
    while (struct dirent *e = readdir(d)) {
        std::string lower = e->d_name;
        for (char &c : lower) c = (char)tolower((unsigned char)c);
        if (lower.compare(0, 2, "._") == 0 || !import_wanted(lower)) continue;
        std::string from = source + "/" + e->d_name;
        struct stat st;
        if (lstat(from.c_str(), &st) != 0 || !S_ISREG(st.st_mode)) continue;     // ordinary files only
        if (!by_lower.emplace(lower, e->d_name).second) { problem = "two files are named " + lower + " apart from case"; break; }
    }
    closedir(d);
    long long bytes = 0;
    int files = 0;
    for (auto const &kv : by_lower) {
        if (!problem.empty()) break;
        if (!copy_file(source + "/" + kv.second, staging + "/" + kv.first)) problem = "copying " + kv.second + " failed (" + strerror(errno) + ")";
        struct stat st;
        if (stat((staging + "/" + kv.first).c_str(), &st) == 0) bytes += st.st_size;
        ++files;
    }
    if (problem.empty()) problem = imported_problem(staging);
    if (!problem.empty()) {
        remove_tree(staging);
        fprintf(stderr, "app: import from %s failed: %s\n", source.c_str(), problem.c_str());
        return problem;
    }
    remove_tree(previous);
    if (is_dir(dir) && rename(dir.c_str(), previous.c_str()) != 0) { remove_tree(staging); return "the old files could not be moved aside"; }
    if (rename(staging.c_str(), dir.c_str()) != 0) {
        rename(previous.c_str(), dir.c_str());
        remove_tree(staging);
        return "the new files could not be put in place";
    }
    remove_tree(previous);
    hostos::exclude_from_backup(dir.c_str());     // the user can import them again
    fprintf(stderr, "app: imported %d files (%lld bytes) from %s into %s\n", files, bytes, source.c_str(), dir.c_str());
    return "";
}

struct ImportPick { bool done = false; std::string path, error; };
void import_picked(void *user, const char *path)
{
    ImportPick *p = (ImportPick *)user;
    // The picker's folder is readable only during this call: the copy runs here.
    if (path && *path) {
        p->path = path;
        p->error = import_game_files(path);
    }
    p->done = true;
}

// The setup of an app that imports the game files (hostos::imports_game_files:
// iOS). RA2_NATIVE_IMPORT=<folder> (tests) imports from that folder without the
// picker. Returns false when the user gives up.
bool setup_imported(void *native_window, bool force, std::string &data_dir, std::string &game_exe)
{
    recover_import();
    std::string dir = game_data_dir();
    const char *scripted = getenv("RA2_NATIVE_IMPORT");
    if (scripted && *scripted && (force || !imported_problem(dir).empty())) {
        // A relative folder is in the app's Documents (a device test copies the files
        // there: xcrun devicectl device copy to ... --destination "Documents/<folder>").
        std::string source = scripted;
        if (source[0] != '/') {
            std::string writes = game_write_dir();
            source = writes.substr(0, writes.rfind('/') + 1) + source;
        }
        std::string error = import_game_files(source);
        if (!error.empty()) fprintf(stderr, "app: RA2_NATIVE_IMPORT failed: %s\n", error.c_str());
    }
    std::string problem = force ? "import requested" : imported_problem(dir);
    std::string note;
    while (!problem.empty()) {
        fprintf(stderr, "app: game files: %s\n", problem.c_str());
        int b = ask(kTitle, note + "Red Alert 2 needs your game files once. Choose the folder where Red Alert 2 is "
                    "installed (it holds ra2.mix and game.exe), for example on a Mac shared in Files or copied to "
                    "On My iPhone.\n\nThe app copies the game's data files (about 1 GB) into its own storage and never "
                    "changes the folder.", {"Choose Folder…", "Not Now"});
        if (b != 0) return false;
        ImportPick pick;
        std::string answer;
        if (scripted_answer(answer)) import_picked(&pick, answer.c_str());
        else if (!hostos::pick_import_folder(native_window, import_picked, &pick)) return false;
        while (!pick.done) {
            SDL_Event e;
            SDL_WaitEventTimeout(&e, 50);
        }
        // Cancelled: the files imported before, when complete, are used; else ask again.
        problem = pick.path.empty() || pick.error.empty() ? imported_problem(dir) : pick.error;
        note = problem.empty() ? "" : "The import did not work: " + problem + ".\n\n";
    }
    data_dir = dir;
    game_exe = dir + "/game.exe";
    return true;
}

// ---- iOS: the in-game resolution from the screen (Scout1248) ----------------
// The game's resolution in play is RA2.INI's [Video] ScreenWidth/ScreenHeight (the
// menus are 800x600 whatever it says). On a phone it is the usable part of the
// screen (hostos::usable_rect, points) divided by the interface scale the player
// chose (1, 1.25 or 1.5), the width a multiple of 8, but never below 800x600: the
// game keeps its 800x600 menus on the same primary surface, and a smaller one cut
// them off (Scout1248, a 656x480 trial). Below that the same aspect is enlarged and
// the picture scaled down to fit (on the iPhone Duo, 594x432 points clear: 824x600,
// then 800x600 below).
void game_resolution(double usable_w, double usable_h, double scale, int &w, int &h)
{
    if (scale < 1) scale = 1;
    double gw = usable_w / scale, gh = usable_h / scale;
    if (gh < 600) { gw *= 600 / gh; gh = 600; }
    if (gw < 800) { gh *= 800 / gw; gw = 800; }
    w = ((int)(gw + 0.5)) & ~7;
    h = ((int)(gh + 0.5)) & ~1;
    // Just wider than 800 at 600 high, the menus (800 wide) leave strips the game never
    // paints, which showed old surface contents as coloured noise beside the shell
    // (Scout1249, the Duo's 824x600): 800x600 then, the picture letterboxed by a few points.
    if (h == 600 && w < 864) w = 800;
}
// Writes ScreenWidth/ScreenHeight into the game's RA2.INI in its write folder
// (made from the imported ra2.ini the first time), keeping every other line.
void set_ini_resolution(std::string const &write_dir, std::string const &data_dir, int w, int h)
{
    std::string path = write_dir + "/RA2.INI";
    std::vector<std::string> lines;
    FILE *f = fopen(path.c_str(), "r");
    if (!f) f = fopen((data_dir + "/ra2.ini").c_str(), "r");
    if (f) {
        char line[1024];
        while (fgets(line, sizeof line, f)) {
            size_t n = strlen(line);
            while (n && (line[n - 1] == '\n' || line[n - 1] == '\r')) line[--n] = 0;
            lines.push_back(line);
        }
        fclose(f);
    }
    std::string width = "ScreenWidth=" + std::to_string(w), height = "ScreenHeight=" + std::to_string(h);
    bool in_video = false, have_video = false, wrote_w = false, wrote_h = false;
    std::vector<std::string> out;
    auto finish_video = [&]() {
        if (!wrote_w) out.push_back(width);
        if (!wrote_h) out.push_back(height);
        wrote_w = wrote_h = true;
    };
    for (std::string const &line : lines) {
        if (!line.empty() && line[0] == '[') {
            if (in_video) finish_video();
            in_video = !strcasecmp(line.c_str(), "[Video]");
            have_video = have_video || in_video;
        } else if (in_video && !strncasecmp(line.c_str(), "ScreenWidth=", 12)) { out.push_back(width); wrote_w = true; continue; }
        else if (in_video && !strncasecmp(line.c_str(), "ScreenHeight=", 13)) { out.push_back(height); wrote_h = true; continue; }
        out.push_back(line);
    }
    if (in_video) finish_video();
    if (!have_video) { out.push_back("[Video]"); finish_video(); }
    std::string tmp = path + ".tmp";
    if (FILE *o = fopen(tmp.c_str(), "w")) {
        for (std::string const &line : out) fprintf(o, "%s\r\n", line.c_str());
        fclose(o);
        rename(tmp.c_str(), path.c_str());
        fprintf(stderr, "app: in-game resolution %dx%d (RA2.INI [Video])\n", w, h);
    }
}

// ---- log file and crash dialog -----------------------------------------
char log_path[1024];
char self_path[1024];
const void *main_image = nullptr;     // the executable's load address (dladdr's dli_fbase)

// stdout and stderr to ~/Library/Logs/RA2Native/ra2-native.log (the previous
// session's log kept as ra2-native.previous.log).
void open_log()
{
    std::string dir = log_dir();
    make_dirs(dir);
    snprintf(log_path, sizeof log_path, "%s/ra2-native.log", dir.c_str());
    std::string previous = dir + "/ra2-native.previous.log";
    rename(log_path, previous.c_str());
    int fd = open(log_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) return;
    fflush(stdout);
    fflush(stderr);
    dup2(fd, 1);
    dup2(fd, 2);
    close(fd);
    setvbuf(stdout, nullptr, _IOLBF, 0);
    time_t now = time(nullptr);
    char when[64];
    strftime(when, sizeof when, "%Y-%m-%d %H:%M:%S", localtime(&now));
    fprintf(stderr, "ra2-native: session started %s (%s)\n", when, self_path);
}

const char *signal_name(int sig)
{
    switch (sig) {
    case SIGSEGV: return "SIGSEGV, a bad memory access";
    case SIGBUS: return "SIGBUS, a bad memory access";
    case SIGILL: return "SIGILL, an illegal instruction";
    case SIGFPE: return "SIGFPE, an arithmetic fault";
    case SIGABRT: return "SIGABRT, an abort";
    case SIGTRAP: return "SIGTRAP, a trap";
    }
    return "a signal";
}

// The fault handler: names the function (the faulting instruction's, or for
// an abort the first frame in the game that called it), writes the backtrace
// to the log, then starts this executable again as the crash dialog
// (--crash-report; a fresh process, as a signal handler cannot run Cocoa; where
// no process can be started, iOS, the next start shows it) and ends.
// Async-signal-safe apart from dladdr/backtrace, as such handlers are.
void on_crash(int sig, siginfo_t *, void *context)
{
    static volatile sig_atomic_t entered = 0;
    if (entered++) _exit(128 + sig);
    void *frames[64];
    int n = backtrace(frames, 64);
    void *candidates[66];
    int count = 0;
    uintptr_t pc = 0;
    if (sig != SIGABRT && hostos::fault_pc(context, &pc)) candidates[count++] = (void *)pc;
    for (int i = 0; i < n; ++i) candidates[count++] = frames[i];
    const char *name = "an unknown function";
    unsigned long offset = 0;
    for (int i = 0; i < count; ++i) {
        Dl_info info = {};
        if (!dladdr(candidates[i], &info) || info.dli_fbase != main_image || !info.dli_sname) continue;
        if (strstr(info.dli_sname, "on_crash") || strstr(info.dli_sname, "ra2_unrecovered") ||
            strstr(info.dli_sname, "ra2_trap_") || strstr(info.dli_sname, "ra2port_trap_reached")) continue;
        name = info.dli_sname;
        offset = (unsigned long)((char *)candidates[i] - (char *)info.dli_saddr);
        break;
    }
    char line[1024];
    int len = snprintf(line, sizeof line, "ra2-native: crash: signal %d (%s) in %s+%#lx\n", sig, signal_name(sig), name, offset);
    if (len > 0) write(2, line, (size_t)len < sizeof line ? (size_t)len : sizeof line - 1);
    backtrace_symbols_fd(frames, n, 2);
    hostos::start_crash_report(self_path, log_path[0] ? log_path : "-");
    _exit(128 + sig);
}

void install_crash_handler()
{
    snprintf(self_path, sizeof self_path, "%s", exe_path().c_str());
    Dl_info self = {};
    if (dladdr((void *)&install_crash_handler, &self)) main_image = self.dli_fbase;
    static char alt_stack[256 * 1024];
    stack_t ss = {};
    ss.ss_sp = alt_stack;
    ss.ss_size = sizeof alt_stack;
    sigaltstack(&ss, nullptr);
    struct sigaction action = {};
    action.sa_sigaction = on_crash;
    action.sa_flags = SA_SIGINFO | SA_ONSTACK;
    sigemptyset(&action.sa_mask);
    for (int sig : {SIGSEGV, SIGBUS, SIGILL, SIGFPE, SIGABRT, SIGTRAP}) sigaction(sig, &action, nullptr);
}

std::string demangle(std::string const &name)
{
    std::string mangled = name;
    int status = 0;
    char *d = abi::__cxa_demangle(mangled.c_str(), nullptr, nullptr, &status);
    if (!d && !mangled.empty() && mangled[0] == '_') d = abi::__cxa_demangle(mangled.c_str() + 1, nullptr, nullptr, &status);
    std::string out = d ? d : name;
    free(d);
    return out;
}

// --crash-report <log>: the dialog for the session that just ended. Reads the
// log's last trap line ("ra2-native: unrecovered NAME reached", the link
// closure's) and crash line (on_crash's).
int crash_report(const char *path)
{
    std::string text;
    if (FILE *f = fopen(path, "rb")) {
        fseek(f, 0, SEEK_END);
        long size = ftell(f);
        long from = size > 1 << 20 ? size - (1 << 20) : 0;
        fseek(f, from, SEEK_SET);
        text.resize((size_t)(size - from));
        text.resize(fread(&text[0], 1, text.size(), f));
        fclose(f);
    }
    size_t crash_at = text.rfind("ra2-native: crash: ");
    size_t trap_at = text.rfind("ra2-native: unrecovered ");
    // A trap aborts at once: its line stands just before the crash line.
    bool trap_abort = crash_at != std::string::npos && trap_at != std::string::npos && trap_at < crash_at &&
                      crash_at - trap_at < 4096;
    auto last_line = [&](const char *prefix) -> std::string {
        size_t at = text.rfind(prefix);
        if (at == std::string::npos) return "";
        size_t end = text.find('\n', at);
        return text.substr(at + strlen(prefix), end == std::string::npos ? std::string::npos : end - at - strlen(prefix));
    };
    std::string crash = last_line("ra2-native: crash: ");
    std::string trap = last_line("ra2-native: unrecovered ");
    std::string message;
    if (trap_abort && crash.find("signal 6 ") == 0) {
        size_t r = trap.rfind(" reached");
        if (r != std::string::npos) trap.resize(r);
        message = "The game reached a part of Red Alert 2 that has not been rebuilt yet, and had to stop:\n\n" + trap + "\n\n";
    } else if (!crash.empty()) {
        // "signal N (what) in NAME+0xOFF"
        size_t open = crash.find('('), close = crash.find(')'), in = crash.find(") in ");
        std::string what = open != std::string::npos && close != std::string::npos ? crash.substr(open + 1, close - open - 1) : "a fault";
        std::string where = in != std::string::npos ? crash.substr(in + 5) : "an unknown function";
        size_t plus = where.rfind('+');
        std::string offset = plus != std::string::npos ? where.substr(plus) : "";
        if (plus != std::string::npos) where.resize(plus);
        message = "The game stopped after an error (" + what + ") in:\n\n" + demangle(where) + " " + offset + "\n\n";
    } else {
        message = "The game stopped unexpectedly.\n\n";
    }
    if (!strcmp(path, "-")) message += "The details were printed to the terminal.";
    else message += "The log has the details:\n" + std::string(path);
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        fprintf(stderr, "%s\n", message.c_str());
        return 1;
    }
    int b = ask("Red Alert 2 stopped", message, {"OK", "Show Log"}, true);
    if (b == 1 && strcmp(path, "-")) hostos::reveal_file(path);
    SDL_Quit();
    return 0;
}

}  // namespace app
