// wwmous.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wwmous.json carries the addresses and the rows that
// stay in their own files.


#include "shared/wwmouse.hpp"


// Dynamic initializer of unknown_empty_rect (0x007717A0, _$E2), the file-local
// empty Rect that shared/wwmouse.hpp defines in every unit including it.
// This first source of the unit also defines the file's globals (0x00B2AF30-0x00B2AF68).
// Unresolved: the original names of the empty rect and of the unknown_ flags.

MouseThreadInfoStruct MouseThreadInfo;
HANDLE MouseThread;
DWORD MouseThreadID;
bool unknown_flag_b2af58;
WWMouseClass *_Mouse;
HANDLE MouseMutex;
int MouseThreadActive;
bool unknown_flag_b2af68;


// void Process_Mouse_Unthreaded() (0x007717C0)
// Updates the mouse directly when the mouse thread is not running (called
// from 0x00406F00). Name is a hypothesis.

void Process_Mouse_Unthreaded()
{
    if (!MouseThreadActive && _Mouse != 0) {
        _Mouse->Process_Mouse();
    }
}


// void Start_Mouse_Thread() (0x007717E0)
// Creates the mouse mutex and the time-critical mouse thread (16 ms period);
// when the priority cannot be raised it reports the error and waits for the
// thread to finish. Name is a hypothesis (called from 0x0069003F).

void Start_Mouse_Thread()
{
    char buffer[1024];

    if (MouseMutex == 0) {
        MouseMutex = CreateMutexA(0, 0, 0);
    }
    if (!MouseThreadActive && MouseMutex != 0) {
        MouseThreadInfo.Quit = 0;
        MouseThreadInfo.Done = 0;
        MouseThreadInfo.Started = 0;
        MouseThreadInfo.Delay = 16;
        MouseThreadInfo.Count = 0;
        MouseThread = CreateThread(0, 0x1000, Mouse_Thread, &MouseThreadInfo, 0, &MouseThreadID);
        if (MouseThread != 0) {
            MouseThreadActive = 1;
            if (!SetThreadPriority(MouseThread, THREAD_PRIORITY_TIME_CRITICAL)) {
                FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, buffer, sizeof(buffer), 0);
                Debug_Printf("Unable to change the priority of the mouse thread - %s\n", buffer);
                while (!MouseThreadInfo.Done) {
                    Sleep(0);
                }
                WaitForSingleObject(MouseThread, 5000);
                CloseHandle(MouseThread);
                MouseThreadActive = 0;
            }
        }
    }
}


// DWORD WINAPI Mouse_Thread(LPVOID parameter) (0x00771900)
// Thread procedure: until asked to quit, processes the mouse under the mutex
// and sleeps for the configured delay. Name is a hypothesis.

DWORD WINAPI Mouse_Thread(LPVOID parameter)
{
    MouseThreadInfoStruct *info = (MouseThreadInfoStruct *)parameter;
    info->Started = 1;
    info->Done = 0;
    while (!info->Quit) {
        Block_Mouse();
        if (_Mouse != 0) {
            _Mouse->Process_Mouse();
        }
        Unblock_Mouse();
        Sleep((DWORD)info->Delay);
        info->Count++;
    }
    info->Done = 1;
    return 0;
}


// void __fastcall Kill_Mouse_Thread() (0x007719A0)
// Stops the mouse thread, waits for it and closes the thread and mutex
// handles. Name is a hypothesis (tail-called from 0x00691304).

void __fastcall Kill_Mouse_Thread()
{
    if (MouseThreadActive) {
        MouseThreadInfo.Quit = 1;
        while (!MouseThreadInfo.Done) {
            Sleep(0);
        }
        WaitForSingleObject(MouseThread, 5000);
        CloseHandle(MouseThread);
        MouseThreadActive = 0;
    }
    if (MouseMutex != 0) {
        CloseHandle(MouseMutex);
        MouseMutex = 0;
    }
}


// WWMouseClass::WWMouseClass(Surface *surface, HWND window) (0x00771A20)
// Empty state, hidden; confines the mouse to the window's client area, makes
// itself the thread's mouse and hides the Windows cursor.
// Unresolved: why the hotspot starts at the centre of the client area.

WWMouseClass::WWMouseClass(Surface *surface, HWND window)
    : MouseCursor(0),
      MouseFrame(0),
      MouseState(-1),
      IsCaptured(false),
      DrawX(0),
      DrawY(0),
      MouseX(0),
      MouseY(0),
      ScreenSurface(surface),
      Window(window),
      ConfiningRect(unknown_empty_rect),
      HotspotX(0),
      HotspotY(0),
      Background(0),
      BackgroundRect(unknown_empty_rect),
      SurfaceBackground(0),
      SurfaceBackgroundRect(unknown_empty_rect),
      SidebarBackground(0),
      SidebarBackgroundRect(unknown_empty_rect),
      unknown_80(unknown_empty_rect),
      unknown_90(-1),
      unknown_94(0)
{
    Block_Mouse();
    Calc_Confining_Rect();
    HotspotX = ConfiningRect.X + ConfiningRect.Width / 2;
    HotspotY = ConfiningRect.Y + ConfiningRect.Height / 2;
    _Mouse = this;
    Unblock_Mouse();
    ShowCursor(0);
}


// WWMouseClass::~WWMouseClass() (0x00771BC0)
// Detaches from the mouse thread and deletes the three save-under surfaces.

WWMouseClass::~WWMouseClass()
{
    Block_Mouse();
    if (_Mouse == this) {
        _Mouse = 0;
    }
    Unblock_Mouse();
    if (Background != 0) {
        delete Background;
        Background = 0;
    }
    if (SurfaceBackground != 0) {
        delete SurfaceBackground;
        SurfaceBackground = 0;
    }
    if (SidebarBackground != 0) {
        delete SidebarBackground;
        SidebarBackground = 0;
    }
}


// Mouse scalar deleting destructor (0x00773670, slot 0)
// Compiler generated with the Mouse vtable (the inline Mouse constructor in
// WWMouseClass's); stores the Mouse vtable (user-declared ~Mouse).
