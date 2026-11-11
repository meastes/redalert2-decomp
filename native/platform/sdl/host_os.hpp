// SPDX-License-Identifier: GPL-3.0-or-later
// The host's operating-system services that are not SDL, standard C++ or POSIX
// (Scout1247, the Mac-port preparation for iOS; the standing portability rule:
// Apple-only calls live in one file per layer). host_apple.mm implements them on
// macOS and iOS; a Windows or other host supplies its own host_<platform> file
// with the same functions. Nothing here includes SDL: callers pass the native
// window handles SDL reports (SDL_PROP_WINDOW_COCOA_WINDOW_POINTER and
// SDL_PROP_WINDOW_UIKIT_WINDOW_POINTER) as void *.
#pragma once
#include <stdint.h>
#include <string>

namespace hostos {

// This process's executable, resolved (empty when unknown).
std::string executable_path();
// Started as an app (macOS: the executable of an .app bundle, as Finder starts
// RA2.app; iOS: always), not as a plain program from a terminal.
bool launched_as_app();
// Where the app keeps things (each created on demand by the caller):
//   settings_dir: the settings file (macOS ~/Library/Application Support/RA2Native,
//                 iOS the container's Library/Application Support/RA2Native);
//   game_write_dir: the game's own writes, RA2.INI and saved games (macOS
//                 <settings_dir>/Game; iOS Documents/Game, visible in Files so
//                 saves can move between the Mac and the phone);
//   log_dir:      the log (macOS ~/Library/Logs/RA2Native; iOS Library/Logs/RA2Native).
std::string settings_dir();
std::string game_write_dir();
std::string log_dir();

// The crash dialog of the session that just faulted. Where the platform can
// start another process (macOS), start_crash_report starts this executable
// again with "--crash-report <log>" (async-signal-safe: called from the fault
// handler) and returns true; elsewhere (iOS: no fork/exec) it returns false and
// the app shows the report at its next start (crash_report_at_next_start).
bool start_crash_report(const char *self, const char *log);
bool crash_report_at_next_start();
// Shows a file in the system's file manager (macOS Finder; nothing on iOS).
void reveal_file(const char *path);
// A fault handler's context: the faulting instruction's address (false when the
// layout is not known).
bool fault_pc(void *ucontext, uintptr_t *pc);

// Test input through the system's own event queue (macOS; RA2_NATIVE_NSEVENT_KEYS,
// packaging/README.md): a left click at a window point (points from the window's
// top left), and key presses for the characters a-z ('<' Backspace, '|' Return).
// False where not supported.
bool post_system_click(void *native_window, double x, double y);
bool post_system_keys(void *native_window, const char *text);

// The app copies the game files into its own storage once (iOS: an app cannot
// read a folder in place from one start to the next) instead of reading the
// installation where it is (macOS).
bool imports_game_files();
// iOS one-time import of the game files (README "iOS"): the folder picker. The
// callback receives the chosen folder's path (empty: cancelled) while the
// folder is readable (inside the picker's security scope), on the main thread.
// False when no picker can be shown (macOS uses SDL's dialogs).
bool pick_import_folder(void *native_window, void (*done)(void *user, const char *path), void *user);
// The part of the window the game may draw in and take touches from (Scout1248):
// the largest landscape rectangle, in window points, clear of the safe-area
// insets and the device's reserved regions (the iPhone Duo's hinge and its
// camera occlusion; UIView reserved regions, iOS 27.1). False where the whole
// window is usable (macOS).
struct Rect { double x, y, w, h; };
bool usable_rect(void *native_window, Rect *out);
// Audio-session interruptions (iOS: a call, Siri, another app's audio session):
// the callback gets true when one begins and false when it ends, on the main
// thread. Nothing elsewhere.
void watch_audio_interruptions(void (*callback)(bool interrupted));
// The touch controls' toolbar (Scout1249, iOS): a Liquid Glass "…" button at the
// usable area's top left that slides a glass capsule of items in and out (the
// look of the user's duo-vnc toolbar: UIGlassEffect, capsule corners, dark glass,
// light SF Symbols). `tapped` gets an item's index on the main thread. False where
// there is no system toolbar (macOS: the host draws its own toggles for tests).
struct ToolbarItem { const char *symbol; const char *title; };
bool toolbar_create(void *native_window, ToolbarItem const *items, int count, void (*tapped)(int index));
// An item's state: on (accent tint) or off, and its title (the scale: "1x").
void toolbar_set(int index, bool on, const char *title);
// The usable area moved (window points): the toolbar keeps to its top left.
void toolbar_place(Rect usable);
// The device's memory footprint and thermal state, for the log (iOS: phys_footprint,
// NSProcessInfo.thermalState); empty elsewhere.
std::string device_report();
// Excludes a file or folder from device backups (iOS: the imported game files,
// which the user can import again). Nothing elsewhere.
void exclude_from_backup(const char *path);

}  // namespace hostos
