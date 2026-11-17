# LAN game: iPhone with a Windows PC

Steps for a Red Alert 2 network game between the iPhone app and the Windows build on the same
home network. Both must be built from the same commit: a game between two different builds can
fall out of step.

## Before you start

1. On the Mac, build both from the same checkout:
   - Windows: `native/build.sh windows` makes `build/release-win/RA2Native.exe`. Copy that one
     file to the PC (a USB stick or a shared folder is fine). It needs nothing else.
   - iPhone: `RA2_IOS_TEAM=<your team id> native/build.sh ios device`, then install it:
     `xcrun devicectl device install app --device <iPhone udid> build/ios-one-device/RA2YR.app`.
2. Put the PC and the iPhone on the same Wi-Fi network (the PC may be on a cable to the same
   router). A guest network or "client isolation" on the router keeps them apart.
3. On the PC, make sure Windows calls that network a private network: Settings, Network &
   internet, your network, Network profile type: Private. On a public network Windows blocks
   the game's messages.

## First start on each

4. PC: start `RA2Native.exe`. The first time it asks for the Red Alert 2 folder; choose your
   installed game (the one with `game.exe` and `ra2.mix`). Windows may warn that the program is
   unrecognised (it is not code-signed): More info, Run anyway.
5. iPhone: open the app. If it has not been set up yet, it asks for the game folder once
   (pick the copied Red Alert 2 folder in Files).

## The game

6. PC: Network (on the main menu). The first time, Windows Defender Firewall asks whether
   RA2Native may use networks: tick Private networks and choose Allow. If you chose Cancel
   earlier, allow "RA2Native" in Windows Security, Firewall & network protection, Allow an app
   through firewall. The game uses UDP ports 26300 to 26307.
7. iPhone: Network. The first time, iOS asks to find devices on your local network: Allow.
   (If you said no: Settings, Privacy & Security, Local Network, turn the app on.)
8. Wait a few seconds: each lobby should list both players. Type a line in the chat box and press
   Return on each side to check that messages arrive.
9. Host on the PC first: New, pick a map, then on the iPhone select the PC's game, Join, Accept.
   (The iPhone cannot send network broadcasts, so it finds a game best when the other side is
   already in the lobby; hosting on the iPhone also works, the PC sees it once the iPhone has
   heard from the PC.)
10. PC: Start Game. Both load (a bar per player) and play.
11. Play at least ten minutes with orders on both sides: build, fight, use the sidebar, open and
    close the Options menu once on each side.

## What to report

- "Works" or what went wrong, and roughly when (minutes into the game).
- An "out of sync" message, a player dropped, or a crash: send both logs.
  - PC: `%LOCALAPPDATA%\RA2Native\Logs\ra2-native.log` (and `ra2-native.previous.log`).
  - iPhone: the Mac can pull it with
    `xcrun devicectl device copy from --device <udid> --domain-type appDataContainer --domain-identifier io.github.ra2native.RA2 --source Library/Logs/RA2Native/ra2-native.log --destination ./iphone-ra2-native.log`.
- If the lobbies never see each other: whether the chat test in step 8 worked, and which network
  each was on.
