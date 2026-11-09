// SPDX-License-Identifier: GPL-3.0-or-later
// The Mac app mode of the SDL host (README "Mac app"): what RA2.app adds to the
// plain ra2-native binary when it is launched from Finder.
//   - First-run setup: the user picks the installed game folder (validated:
//     its MIX files), and the supported game.exe is located (the folder's own,
//     or one the user picks when the folder carries another build). The choice
//     is kept in ~/Library/Application Support/RA2Native/settings.
//   - Writes (RA2.INI, saved games) go to ~/Library/Application Support/
//     RA2Native/Game, never into the installation.
//   - stdout/stderr go to ~/Library/Logs/RA2Native/ra2-native.log.
//   - A trap (unrecovered code reached) or a fault shows a dialog naming the
//     function and the log, instead of a silent exit.
// Included once, by main.cpp (no separate unit, so native/CMakeLists.txt is
// unchanged); macOS only.
#pragma once
#include <SDL3/SDL.h>
#include <CommonCrypto/CommonDigest.h>
#include <mach-o/dyld.h>
#include <mach-o/loader.h>
#include <cxxabi.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>
#include <signal.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/ucontext.h>
#include <sys/wait.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <map>
#include <string>
#include <vector>

extern char **environ;

namespace app {

// The supported game.exe (AGENTS.md "Fixed constraints"; the same check as
// port/src/original_image.cpp, which reads its data).
const long long kTargetSize = 4485392;
const char kTargetSha256[] = "e6418111c2c36081a1795fb000e0927beea1630170b334c5013d12102602e70f";

std::string home()
{
    const char *h = getenv("HOME");
    return h && *h ? h : "/tmp";
}
std::string support_dir() { return home() + "/Library/Application Support/RA2Native"; }
std::string log_dir() { return home() + "/Library/Logs/RA2Native"; }
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

std::string exe_path()
{
    char buf[4096];
    uint32_t n = sizeof buf;
    if (_NSGetExecutablePath(buf, &n) != 0) return "";
    char real[PATH_MAX];
    return realpath(buf, real) ? real : buf;
}
// Launched as RA2.app's executable (Contents/MacOS/).
bool in_bundle() { return exe_path().find(".app/Contents/MacOS/") != std::string::npos; }

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
Exe check_exe(std::string const &path, std::string &digest, long long &size)
{
    digest.clear();
    size = 0;
    FILE *f = path.empty() ? nullptr : fopen(path.c_str(), "rb");
    if (!f) return Exe::missing;
    CC_SHA256_CTX ctx;
    CC_SHA256_Init(&ctx);
    unsigned char buf[65536];
    size_t n;
    while ((n = fread(buf, 1, sizeof buf, f)) > 0) {
        CC_SHA256_Update(&ctx, buf, (CC_LONG)n);
        size += (long long)n;
    }
    fclose(f);
    unsigned char md[CC_SHA256_DIGEST_LENGTH];
    CC_SHA256_Final(md, &ctx);
    char hex[2 * CC_SHA256_DIGEST_LENGTH + 1];
    for (int i = 0; i < CC_SHA256_DIGEST_LENGTH; ++i) snprintf(hex + 2 * i, 3, "%02x", md[i]);
    digest = hex;
    return size == kTargetSize && digest == kTargetSha256 ? Exe::supported : Exe::other;
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
        : "The game.exe in this folder is a different build of Red Alert 2 (" + sizes + ", SHA-256 " +
              digest.substr(0, 16) + "…).\n\n";
    std::string supported = "This version of the native game reads its tables and dialogs from one build only: the base "
                            "Red Alert 2 game.exe of 4,485,392 bytes (SHA-256 e6418111c2c36081…). Other builds are not "
                            "supported yet.\n\nIf you have that game.exe elsewhere (for example a backup), choose it. "
                            "Otherwise the game cannot start.";
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
                                : chosen + "\nis not the supported game.exe either (" + sizes + ", SHA-256 " +
                                      digest.substr(0, 16) + "…).\n\n";
    }
}

// ---- log file and crash dialog -----------------------------------------
char log_path[1024];
char self_path[1024];
const struct mach_header *main_image = nullptr;

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
// (--crash-report; a fresh process, as a signal handler cannot run Cocoa) and
// ends. Async-signal-safe apart from dladdr/backtrace, as such handlers are.
void on_crash(int sig, siginfo_t *, void *context)
{
    static volatile sig_atomic_t entered = 0;
    if (entered++) _exit(128 + sig);
    void *frames[64];
    int n = backtrace(frames, 64);
    void *candidates[66];
    int count = 0;
#if defined(__arm64__)
    if (sig != SIGABRT && context) candidates[count++] = (void *)((ucontext_t *)context)->uc_mcontext->__ss.__pc;
#elif defined(__x86_64__)
    if (sig != SIGABRT && context) candidates[count++] = (void *)((ucontext_t *)context)->uc_mcontext->__ss.__rip;
#endif
    for (int i = 0; i < n; ++i) candidates[count++] = frames[i];
    const char *name = "an unknown function";
    unsigned long offset = 0;
    for (int i = 0; i < count; ++i) {
        Dl_info info = {};
        if (!dladdr(candidates[i], &info) || info.dli_fbase != (void *)main_image || !info.dli_sname) continue;
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
    pid_t child = fork();
    if (child == 0) {
        execl(self_path, self_path, "--crash-report", log_path[0] ? log_path : "-", (char *)nullptr);
        _exit(1);
    }
    _exit(128 + sig);
}

void install_crash_handler()
{
    snprintf(self_path, sizeof self_path, "%s", exe_path().c_str());
    main_image = _dyld_get_image_header(0);
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
    if (b == 1 && strcmp(path, "-")) {
        pid_t pid;
        const char *argv[] = {"/usr/bin/open", "-R", path, nullptr};
        if (posix_spawn(&pid, argv[0], nullptr, nullptr, (char *const *)argv, environ) == 0) waitpid(pid, nullptr, 0);
    }
    SDL_Quit();
    return 0;
}

}  // namespace app
