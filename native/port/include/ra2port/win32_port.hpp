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
// After this thread's last host_path: 0 the file is there (or was not looked
// up), 2 its last component is missing, 3 a folder on its way is missing (the
// cached listings already know; no open is needed to find out).
int host_path_missing();
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
    // The game's main thread is about to wait (Sleep, the frame floor): the host
    // shows the primary surface as the game left it (Scout1209). On Windows the
    // primary is the screen, so what was drawn before a Sleep is visible during
    // it; the host otherwise presents at its next event pump. May be null.
    void (*wait)();
    // The game's main thread is busy without reading messages (file reads, a
    // busy wait, a Sleep): the host lets the system see the window respond
    // (macOS shows the spinning cursor after a few seconds otherwise) and keeps
    // any input queued for the game's next PeekMessage. The port calls it at
    // most every 50 ms (keep_alive below); off the main thread the host does
    // nothing. May be null (Scout1209).
    void (*keep_alive)();
    // The port's pause hold began (true) or ended (false): see request_pause. The
    // host stops and restarts its audio device (the mixer's play cursors stand
    // still with the game's clocks). May be null (Scout1247).
    void (*held)(bool on);
};
void set_host(const Host *host);

// Host pause (Scout1247): the host asks for it (the Mac app when its window loses
// the focus; the iOS app when it goes to the background) and lifts it again. The
// game is not told and no game code changes: the port holds the game's main thread
// inside its next read of an empty message queue (PeekMessageA: every menu, movie
// and mission loop reads its queue), pumping the host's events until the pause is
// lifted, so the game stops wherever it was. While held, the game's clocks
// (timeGetTime, GetTickCount, QueryPerformanceCounter, the virtual TSC, the Bink
// movie clock, silent DirectSound play cursors) stand still and the multimedia
// timers do not fire; afterwards they go on from where they stopped, without a
// catch-up burst. The pause takes effect only at that hold: until the main thread
// reaches it the clocks run (a loading screen's busy wait on them still ends).
void request_pause(bool on);
bool pause_requested();
bool held();                 // the main thread is in the hold now
void hold_if_paused();       // the queue reads call it when they find nothing
// The game's clock: the monotonic clock less the time spent held (nanoseconds).
uint64_t game_clock_ns();
// Checkpoint saves (Scout1248): the host asks for one (the iOS app when it goes to
// the background, and periodically); the game side takes the request at the end of
// its next Main_Loop pass (ra2port_take_checkpoint, link/harness_save.cpp) and saves
// a single-player or skirmish game. A pause requested with a checkpoint waits for it
// up to one second before it holds the game.
void request_checkpoint();
// When the last checkpoint was saved (os monotonic ns; 0: none this run). The game side
// reports each save (ra2port_checkpoint_saved); the iOS host compares it with the
// time it went to the background (lane I).
uint64_t last_checkpoint_saved_ns();
// Resume at start (lane I): the host asks, before WinMain, for a saved game to be
// loaded as the menus would load it. The port's dialogs then press Single Player in
// the main menu and Load Saved Game in the single-player menu (resume_dialog_command:
// the control to send a dialog as it opens, 0 for none), and the Load dialog's entry
// loads the file instead of listing the saves (ra2port_take_resume, once).
// A cutscene is playing (lane I; port/src/bink.cpp): a Bink movie with sound, at least
// 400 pixels wide, is open (Play_Movie 0x005A02D0's BinkClass; the menus' silent
// animations and the radar's small in-game movies do not count). The touch host shows
// its Skip button meanwhile. RA2 plays no VQA movies on this path.
bool movie_playing();
void request_resume(const char *file);
bool resume_pending();
int resume_dialog_command(unsigned dialog);

// File-system operations on the game's behalf (Scout1209 diagnostics): counted
// per kind for RA2_NATIVE_PERF_LOG, and slowed by RA2_NATIVE_FS_LATENCY_US
// microseconds each (test runs: a network volume's round trip).
enum FsOp { FS_STAT, FS_LIST, FS_OPEN, FS_READ, FS_SEEK, FS_OP_KINDS };
void fs_op(FsOp kind);
unsigned long fs_op_count(FsOp kind);
// Calls Host::keep_alive at most every 50 ms (from any thread; cheap otherwise).
void keep_alive();
const Host *host();

// A streamed 16-bit PCM voice in the DirectSound mixer (port/src/ddraw_dsound.cpp;
// the native binkw32's movie sound, Scout1142): written ahead of its play cursor
// as frames are decoded, played until closed; silent without a host device.
void *audio_stream_open(int rate, int channels);
void audio_stream_write(void *stream, short const *samples, int frames);
void audio_stream_pause(void *stream, bool pause);
void audio_stream_volume(void *stream, int volume);   // 0 .. 32768 (Bink's scale)
void audio_stream_close(void *stream);

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
