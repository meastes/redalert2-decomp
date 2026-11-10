# RA2.app (Mac app bundle)

`make_app.sh` turns the native game build into `RA2.app`, a normal Mac app you
start from Finder. The bundle holds only the game program, the SDL3 library it
uses (with its zlib licence), a placeholder icon drawn by `make_icon.py` and
`Info.plist`. It never contains game data or a game.exe. It is signed ad hoc for
use on this Mac only: it is not notarised and must not be published.

```
cmake -S native -B build/native -G Ninja -DRA2_NATIVE_SUBSYSTEM=all -DRA2_NATIVE_LINK_CLOSURE=ON
cmake --build build/native
native/packaging/make_app.sh build/native/ra2-native build/native   # writes build/native/RA2.app
```

## How to launch

1. Double-click **RA2.app** (or drag it to Applications first). Because it is
   signed for this Mac only, the first launch of a copy from somewhere else may
   need right-click, **Open**.
2. The first time, the app asks for your Red Alert 2 folder: click
   **Choose Folder…** and pick the folder where the game is installed (the one
   with `ra2.mix` and `game.exe`). The app reads the game's files from there
   and never changes them.
3. The app needs the base Red Alert 2 `game.exe` of 4,485,392 bytes, or a copy
   of that build whose data is unchanged (for example the signed 4,489,184-byte
   `game.exe` with no-CD patches; `port/include/ra2port/game_builds.hpp`). If the
   folder has any other build, it says so (with the reason, e.g. "different data
   in .data") and lets you pick a supported `game.exe` from somewhere else (for
   example a backup); otherwise the game cannot start.
4. The game starts at its main menu. Your choice is remembered; to choose
   again, delete `~/Library/Application Support/RA2Native/settings`.

While playing:

| Keys | |
| --- | --- |
| Cmd+Return or Cmd+F (also Alt+Return) | fullscreen on and off (remembered) |
| Cmd+Q | quit (asks first once a game has started) |
| Two-finger trackpad swipe over the map | scroll the map |
| Mouse wheel, or two-finger swipe over the sidebar | scroll the sidebar |
| Two-finger click (trackpad) | right click |
| Ctrl+click | force fire |
| Ctrl+Shift+click | attack-move |
| Ctrl+Alt (Option)+click | guard (defend) |
| Alt (Option)+click | force move |
| Shift+click | add to the selection |

Map scrolling by trackpad and saving RA2.INI on Cmd+Q use two small hooks on
the game side (`ra2_native_scroll_map`, `ra2_native_save_settings`, declared
weak in `platform/sdl/host.cpp`); in a build without them the swipe scrolls
the sidebar and Cmd+Q quits without writing RA2.INI.

In fullscreen the picture keeps its shape (black bars at the sides on a wide
display) and the mouse stays inside the game, so pushing it against any screen
edge scrolls the map.

Where things are kept:

- settings: `~/Library/Application Support/RA2Native/settings`
- the game's own files it writes (RA2.INI, saved games):
  `~/Library/Application Support/RA2Native/Game`
- the log: `~/Library/Logs/RA2Native/ra2-native.log` (the session before it:
  `ra2-native.previous.log`)

If the game stops with an error, a window names the part of the game that
failed and points at the log. Parts of the game that are not rebuilt yet stop
it this way.

## LAN games (experimental, off by default)

Network (LAN) play between Macs on the same local network works in tests
(two and three games, thousands of frames in step) but is off by default.
To turn it on, quit the game and add this line to
`~/Library/Application Support/RA2Native/settings`:

    lan=1

(A test run can set `RA2_NATIVE_LAN=1` in the environment instead.) Every Mac
that plays needs it. The game uses UDP ports 26300-26307 and broadcasts on the
local network; up to eight games fit on one Mac or network.

What to try, two Macs on the same Wi-Fi or Ethernet network:

1. Both: Network. Each lobby lists both players by their names
   (RA2.INI [MultiPlayer] Handle, or the name typed in the lobby).
2. Mac A: New (host). Mac B: select A's game in the Games list, Join, Accept.
3. Mac A: Start Game. Both load (a progress bar per player) and play.
4. Play a few minutes with orders on both sides. An "out of sync" message or a
   player dropped at the start ("failed to make it into the game") is a bug:
   please send both Macs' logs.

The macOS firewall: when it is on (System Settings, Network, Firewall), the
first LAN game asks "Do you want the application Red Alert 2 to accept
incoming network connections?". Choose Allow; with Deny the other Mac's packets
are blocked and the lobby stays empty. Because this build is signed for local
use only, macOS may ask again after each new build of the app. With the
firewall off there is no question.

Not supported: Internet (Westwood Online) games, games with Windows PCs (not
tested), the packet router and NAT.

## For testing

`ra2-native --app` runs the same app mode from a terminal. Environment:
`RA2_NATIVE_SETTINGS=<file>` (another settings file), `RA2_NATIVE_SETUP=1` or
`--setup` (ask for the folder again), `RA2_NATIVE_SETUP_ANSWERS="0|<folder>|0|<game.exe>"`
(answers for the setup dialogs, in order: a button number per message, a path
per file dialog), `RA2_NATIVE_LOG_STDERR=1` (keep the terminal instead of the
log file), `RA2_NATIVE_WINDOW_DUMP=<file>` (every 2 s the rendered window as
`<file>.<n>.bmp`). Input scripts (`--input-script`) also take `quit`,
`key cmd+F` / `key alt+Return`, and `wheel DX DY [X Y]` (a trackpad swipe).
`open -n RA2.app --env NAME=value` passes them to the bundle.
