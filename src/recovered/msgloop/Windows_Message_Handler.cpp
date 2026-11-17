// void Windows_Message_Handler() -- 0x005B5EE0 (Westwood msgloop.cpp; TS reconstruction name)
// Dispatches every pending window message: each goes to 0x004919B0 and the tooltips first, then to the
// modeless dialogs, the accelerators and the intercept handler; one none of them takes is translated and
// dispatched. Ends when the queue is empty or at WM_QUIT.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/movie.hpp"
#include "shared/tool_tip.hpp"
#include "shared/win32_window.hpp"
#include "shared/msgloop.hpp"

WIN32_IMPORT BOOL WINAPI IsDialogMessageA(HWND dialog, MSG *message);
WIN32_IMPORT int WINAPI TranslateAcceleratorA(HWND window, HACCEL accelerators, MSG *message);
extern CCToolTip *ToolTips;

void Windows_Message_Handler()
{
    if (MainWindow == 0) return;
    MSG msg;
    ++MessageLoopCount;
    while (Peek_Windows_Message(&msg, 0, 0, 0, 0)) {
        if (!Get_Windows_Message(&msg, 0, 0, 0)) {
            return;
        }
        DispatchLauncherMessage(&msg);
        if (ToolTips != 0) {
            ToolTips->Process_Message(&msg);
        }
        bool processed = false;
        for (int index = 0; index < _ModelessDialogs.Count(); index++) {
            if (IsDialogMessageA(_ModelessDialogs[index], &msg)) {
                processed = true;
                break;
            }
        }
        if (processed) continue;
        for (int aindex = 0; aindex < _Accelerators.Count(); aindex++) {
            if (TranslateAcceleratorA((HWND)_Accelerators[aindex].unknown_04, (HACCEL)_Accelerators[aindex].unknown_00, &msg)) {
                processed = true;
                break;
            }
        }
        if (processed) continue;
        if (Message_Intercept_Handler != 0) {
            processed = Message_Intercept_Handler(msg);
        }
        if (processed) continue;
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}
