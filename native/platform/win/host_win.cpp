// SPDX-License-Identifier: GPL-3.0-or-later
// Windows implementations of the host's operating-system services
// (platform/sdl/host_os.hpp; host_apple.mm is the macOS/iOS file). The only host
// file with Win32 calls; no SDL and no Win32 port headers here (the port's
// win32_port.hpp declares the game's own Win32 types). x86_64 Windows build
// (README "Windows build").
#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>

#include "../sdl/host_os.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

namespace {

// The test runs' temporary folders (main.cpp: $TMPDIR, "/tmp" without it) and the
// timer resolution the frame pacing sleeps at (1 ms, as SDL and games set it).
struct WindowsStartup {
    WindowsStartup()
    {
        if (!getenv("TMPDIR")) {
            char temp[MAX_PATH];
            DWORD n = GetTempPathA(sizeof temp, temp);
            if (n > 0 && n < sizeof temp) {
                if (temp[n - 1] == '\\') temp[n - 1] = 0;
                _putenv_s("TMPDIR", temp);
            }
        }
        timeBeginPeriod(1);
    }
} windows_startup;

std::string known_folder(REFKNOWNFOLDERID id, const char *fallback_variable)
{
    PWSTR wide = nullptr;
    std::string path;
    if (SUCCEEDED(SHGetKnownFolderPath(id, KF_FLAG_CREATE, nullptr, &wide)) && wide) {
        char buffer[MAX_PATH * 3];
        int n = WideCharToMultiByte(CP_UTF8, 0, wide, -1, buffer, sizeof buffer, nullptr, nullptr);
        if (n > 0) path = buffer;
    }
    CoTaskMemFree(wide);
    if (path.empty()) {
        const char *v = getenv(fallback_variable);
        path = v && *v ? v : ".";
    }
    return path;
}

}  // namespace

namespace hostos {

std::string executable_path()
{
    char path[MAX_PATH * 2];
    DWORD n = GetModuleFileNameA(nullptr, path, sizeof path);
    return n > 0 && n < sizeof path ? std::string(path, n) : std::string();
}

// Started from Explorer (no console of its own to write to), not from a terminal.
bool launched_as_app() { return GetConsoleWindow() == nullptr; }

std::string settings_dir() { return known_folder(FOLDERID_RoamingAppData, "APPDATA") + "\\RA2Native"; }
std::string game_write_dir() { return settings_dir() + "\\Game"; }
std::string log_dir() { return known_folder(FOLDERID_LocalAppData, "LOCALAPPDATA") + "\\RA2Native\\Logs"; }

// A fresh process shows the crash dialog: this executable with --crash-report.
bool start_crash_report(const char *self, const char *log)
{
    std::string command = std::string("\"") + self + "\" --crash-report \"" + (log && *log ? log : "-") + "\"";
    STARTUPINFOA si = {};
    si.cb = sizeof si;
    PROCESS_INFORMATION pi = {};
    std::string mutable_command = command;
    if (!CreateProcessA(self, &mutable_command[0], nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) return false;
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return true;
}
bool crash_report_at_next_start() { return false; }

void reveal_file(const char *path)
{
    std::string args = std::string("/select,\"") + path + "\"";
    ShellExecuteA(nullptr, "open", "explorer.exe", args.c_str(), nullptr, SW_SHOWNORMAL);
}

// The CONTEXT platform/win/posix_win.cpp's sigaction passes.
bool fault_pc(void *context, uintptr_t *pc)
{
    if (!context) return false;
    *pc = (uintptr_t)((const CONTEXT *)context)->Rip;
    return true;
}

// Test input through the system's queue: not on Windows yet (SDL's own events are used).
bool post_system_click(void *, double, double) { return false; }
bool post_system_keys(void *, const char *) { return false; }

bool imports_game_files() { return false; }
bool pick_import_folder(void *, void (*)(void *, const char *), void *) { return false; }
bool usable_rect(void *, Rect *) { return false; }
void watch_audio_interruptions(void (*)(bool)) {}
bool toolbar_create(void *, ToolbarItem const *, int, void (*)(int)) { return false; }
void toolbar_set(int, bool, const char *) {}
void toolbar_place(Rect) {}
void skip_button_create(void *, void (*)()) {}
void skip_button_show(bool) {}
void toolbar_notice(const char *text) { fprintf(stderr, "host: notice \"%s\"\n", text); }
bool app_active() { return false; }
std::string device_report() { return ""; }
void exclude_from_backup(const char *) {}

}  // namespace hostos
