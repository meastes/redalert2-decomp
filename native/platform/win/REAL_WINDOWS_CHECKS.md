# Windows build: checks that need real Windows (codex4)

Lane C7 ran the Windows build under Wine on the Mac (played checks, the 24-mission
campaign sweep, ctest `native-census`, `native-enhancements-frames`,
`native-enhancements-settings`). Wine cannot answer the items below: a real desktop
window, real audio, real fullscreen and the system's DPI scaling. Run them on a Windows
10/11 x86_64 machine with the exe from the drop (`drop/native-win-<sha>/ra2-native.exe`,
console subsystem) and the installed Red Alert 2 (read only; never copied into the repo).

Set up once (PowerShell; adjust the game folder):

```
$RA2  = "C:\Westwood\RA2"                 # the installed game: ra2.mix, game.exe
$EXE  = "C:\ra2-native\ra2-native.exe"    # from the drop
$OUT  = "C:\ra2-native\checks"; mkdir $OUT -Force
Remove-Item -Recurse -Force "$env:APPDATA\RA2Native" -ErrorAction SilentlyContinue   # a first start
```

Report per item: pass/fail, the log named, a screenshot where asked, Windows version,
display resolution and scale (Settings > Display > Scale).

## 1. First start and windowed play (interactive)

```
& $EXE --app 2> "$OUT\app1.log"
```
- The setup question appears; Choose Folder opens the system folder picker (not a Mac
  path); pick `$RA2`. The text says settings are kept in your AppData folder.
- The main menu shows in a window (800x600 picture scaled; resizable; the window's close
  box quits). Settings land in `%APPDATA%\RA2Native\settings` (data_dir, fullscreen=0),
  games in `%APPDATA%\RA2Native\Game`, the log in `%LOCALAPPDATA%\RA2Native\Logs`.
- Start a skirmish (Single Player > Skirmish > Start Game), build a power plant and a
  barracks, select and move units with the mouse (left click select, right click
  deselect, drag box), scroll the map at the window's edges and with the arrow keys.
- Save (Options > Save), quit, start again with `& $EXE --app`, Load: the save plays on.
- Alt+Tab away and back: the game pauses while inactive and resumes.

## 2. Audio

```
& $EXE --app 2> "$OUT\audio.log"
```
- Menu music (the theme) plays; menu button clicks sound; in a skirmish, unit voices on
  selection and orders, building and weapon sounds. Options > Sound: the music and sound
  sliders change the volume.
- In the log: an audio device opened (no `dummy` driver); no "audio" errors.
- Unplug or switch the default output device while playing: the game keeps running.

## 3. Fullscreen

```
& $EXE --app 2> "$OUT\fullscreen.log"
```
- Alt+Return toggles fullscreen and back (borderless, the picture fitted with its aspect
  kept, letterbox bars on a wide display); the mouse stays inside the window in
  fullscreen; Alt+Tab out and in works.
- Options > ENHANCEMENTS > Fullscreen CHANGE > ACCEPT goes fullscreen; quit and start
  again: it starts fullscreen (settings `fullscreen=1`). ORIGINAL leaves fullscreen as is.
- Enhancements page, each row: Vertical sync On (no tearing while scrolling), Picture
  scaling Sharp/Nearest/Smooth (visible at a non-integer scale), Game interface size 1.5x
  and Map zoom 1.5x (both from the next game: start a skirmish). ACCEPT, quit, start
  again: the values are kept; ORIGINAL + ACCEPT sets all but Fullscreen back to
  Off/1x/1x/Sharp. Screenshot the page with the changed values.

## 4. DPI scaling

At display scale 100%, then 150% (and 125% if available), each with:
```
& $EXE --app 2> "$OUT\dpi_<scale>.log"
```
- The window opens at a sensible size for the scale (not tiny at 150%), the picture is
  sharp with Picture scaling Sharp (not blurred by the system's bitmap stretching), and
  mouse clicks land on the button under the pointer (check the menu's bottom button and
  a sidebar cameo). Screenshot the main menu at each scale.
- Drag the window between two monitors of different scales if available: the picture
  and the clicks stay right.

## 5. Test runs without Wine (for comparison with the Mac and Wine numbers)

```
$env:RA2_NATIVE_MOVIE_SKIP="1"; $env:RA2_NATIVE_IGNORE_FOCUS="1"; $env:RA2_NATIVE_CRASH_TRACE="1"
& $EXE --data-dir $RA2 --run-to=menu --run-seconds=120 --write-dir="$OUT\wd-menu" --game-args=-noaudio 2> "$OUT\menu.log"
& $EXE --data-dir $RA2 --run-to=frames --run-seconds=420 --write-dir="$OUT\wd-census" "--game-args=-noaudio -CD." --skirmish=#5 --skirmish-frames=600 --skirmish-order=all 2> "$OUT\census.log"
$env:RA2_NATIVE_INIT_TRACE="1"; & $EXE --data-dir $RA2 --run-to=menu --run-seconds=120 --write-dir="$OUT\wd-init" --game-args=-noaudio 2> "$OUT\inittrace.log"; Remove-Item Env:RA2_NATIVE_INIT_TRACE
```
- menu.log and census.log end with `stopped: target reached` and no `trap`, `signal` or
  `crash` line; inittrace.log's `initterm #N` lines count up from #0 to #3609 (358 lines;
  the order the Mac prints) and the last line says `the last starting at original entry #3609`.
