# Running a rebuilt image under Wine (the shift test)

Research helpers for docs/SOURCE_FORM_TRIAL.md; nothing here is part of the exact build.

What the game needs to start under the project's Wine host (private config `wine`,
`wine_prefix`):

1. A run folder with the game data (the test MIX files; the string table is not in that set,
   so the game stops at "String Manager failed to initialize", the same point as the native
   harness) and `binkw32.dll` beside the image (from the installed game, read only).
2. The launcher's mutex: `CopyProtection_Check_Already_Running` (0x4918F0) bails unless
   another process holds `48BC11BD-C4D7-466b-8A31-C6ABBAD47B3E`. `launcher_mutex.c` holds it;
   build it with the pinned VC6 (`CL /nologo /c /O1 launcher_mutex.c`, then
   `LINK /subsystem:console /entry:main /nodefaultlib /out:launcher_mutex.exe launcher_mutex.obj
   KERNEL32.LIB` from the VC98 media) and leave it running under the same prefix.
3. The launcher's registry value, `HKCU\\WChat\\SysID\\ID` = 0x10006 (`Validate_Launcher`
   compares it with `VerNum.Version_Number()`): `wine reg add "HKCU\\WChat\\SysID" /v ID /t
   REG_DWORD /d 65542 /f`. Remove it afterwards (`wine reg delete "HKCU\\WChat" /f`).

Then:

    run_wine.py build/<exact>/reference.exe RUN_DIR 45 RUN_DIR/exact.log
    run_wine.py build/<shift>/shifted.exe RUN_DIR 45 RUN_DIR/shift.log
    peek_object.py build/<shift>/shifted.exe RUN_DIR        # VerNum's vtable pointer

The two images should behave the same (with `+relay` and a keep regex, compare the traced
calls); `peek_object.py` shows the vtable pointer the constructor stored: the original copy
0x7A2ABC in the exact image, the module's relocated copy in the shifted one.
