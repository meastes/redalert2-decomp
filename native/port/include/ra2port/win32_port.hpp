// Platform side of the Win32 seam (ra2_port). The recovered code declares the
// imports itself (shared/win32_api.hpp and friends, extern "C"); this header
// gives the port the same Win32 widths on LP64 arm64 without the VC6 dialect:
// DWORD/UINT/LONG/BOOL are 32 bits, handles and pointers 64, and WPARAM/LPARAM/
// LRESULT pointer-sized (the tree is built with --win64-params).
//
// Handles are small tokens (0x100 + slot), not heap pointers, so recovered code
// that keeps a HANDLE/HWND in a 32-bit field still works (Scout1052 port rule).
#ifndef RA2PORT_WIN32_PORT_HPP
#define RA2PORT_WIN32_PORT_HPP

#include <stddef.h>
#include <stdint.h>

typedef int32_t BOOL;
typedef uint32_t DWORD;
typedef uint32_t UINT;
typedef int32_t LONG;
typedef uint32_t ULONG;
typedef uint16_t WORD;
typedef uint8_t BYTE;
typedef int32_t HRESULT;
typedef void *HANDLE;
typedef void *HWND;
typedef void *HDC;
typedef void *HINSTANCE;
typedef void *HGDIOBJ;
typedef uintptr_t WPARAM;
typedef intptr_t LPARAM;
typedef intptr_t LRESULT;
typedef uint16_t WCHAR;   // VC6 wchar_t: UTF-16 code unit

typedef LRESULT (*WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef intptr_t (*DLGPROC)(HWND, UINT, WPARAM, LPARAM);

struct POINT { LONG x, y; };
struct RECT { LONG left, top, right, bottom; };
struct MSG { HWND hwnd; UINT message; WPARAM wParam; LPARAM lParam; DWORD time; POINT pt; };
struct FILETIME { DWORD dwLowDateTime, dwHighDateTime; };
struct SYSTEMTIME { WORD wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds; };

#define RA2_INVALID_HANDLE ((HANDLE)(intptr_t)-1)

namespace ra2port {

// Logged once per name: an import the port does not implement (or implements
// as a no-op). Returns nothing; the caller returns the Win32 failure value.
void unimplemented(const char *name);
void trace(const char *fmt, ...);

// Error codes (GetLastError) for the calling thread.
void set_last_error(DWORD code);

// Handle table. Each kind owns its payload; close() deletes it.
enum HandleKind { H_FILE = 1, H_FIND, H_EVENT, H_MUTEX, H_THREAD, H_WINDOW, H_DC, H_GDI, H_MODULE, H_PROCESS, H_REGKEY, H_MAPPING };
struct HandleObject { virtual ~HandleObject() {} int kind = 0; };
HANDLE handle_new(HandleObject *object);
HandleObject *handle_get(HANDLE handle, int kind = 0);
bool handle_close(HANDLE handle);

// Paths: DOS-style game paths ("DATA\\RULES.INI", "C:\\WESTWOOD\\RA2\\...")
// mapped onto the user's installation (--data-dir / RA2_DATA_DIR), with
// case-insensitive component lookup as on FAT/NTFS.
void set_data_dir(const char *dir);
const char *data_dir();
// Writes the host path for a game path into out (size n); returns out.
char *host_path(const char *game_path, char *out, size_t n);
// Writes (create, write access, delete, rename) go through host_path_write:
// with a write directory set (--write-dir, always under --run-to) they land in
// that overlay and the installation is never modified.
void set_write_dir(const char *dir);
const char *write_dir();
char *host_path_write(const char *game_path, char *out, size_t n);

// Start-up progress (--run-to, platform/sdl/main.cpp): named milestones and
// counters logged as the game reaches them; reaching the requested one, a trap
// or the time limit prints the summary and ends the process.
void progress(const char *milestone, const char *detail = nullptr);
void progress_count(const char *counter, const char *detail = nullptr);

// UTF-16 helpers (VC6 wchar_t).
size_t wlen(const WCHAR *s);

// Host hooks (platform/sdl): main window, event pump into the Win32 queue,
// frame presentation for the DirectDraw primary surface, audio output (the
// host opens a stereo 16-bit stream at rate and calls mix for every block it
// plays; mix runs on the host's audio thread; false when no device opens).
struct Host {
    void *(*create_window)(const char *title, int width, int height);
    void (*pump_events)();          // translate host events into PostMessage calls
    void (*present)(const void *pixels, int width, int height, int pitch, int bits);
    void (*set_relative_cursor)(bool show);
    bool (*open_audio)(int rate, void (*mix)(short *out, int frames));
    // A game frame (Main_Loop iteration) began: the host plays scripted input
    // due at that frame (--input-script, test runs). May be null.
    void (*frame)(int frame);
};
void set_host(const Host *host);
const Host *host();

// The window that receives host input (the first top-level window created).
HWND main_window();
BOOL post_message(HWND window, UINT message, WPARAM wparam, LPARAM lparam);
// DirectDraw set a display mode: the screen and the main window take its size.
void display_mode_changed(int width, int height);

// The launcher's handshake (platform/sdl/main.cpp plays RA2.EXE's part). The
// game signals the named launcher event (Notify_Launcher 0x00491930); the
// launcher then posts message 0xBEEF to the game thread with a file mapping
// whose view holds its message, which Receive_Launcher_Message 0x004919B0 maps
// (MapViewOfFileEx) and Validate_Launcher 0x00491A10 compares with the text
// the game expects. Without it the game queues a DESTRUCT event for the player
// every 1,024 frames (Main_Loop_End 0x0053F840) and checks it before the score
// screen (Do_Win 0x0065D7F0). `message` is the text the launcher sends (the
// expected text, read from game.exe at run time).
void set_launcher(const char *event_name, const char *message);

}  // namespace ra2port

#endif
