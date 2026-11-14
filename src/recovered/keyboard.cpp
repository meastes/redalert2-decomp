// keyboard.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/keyboard.json carries the addresses and the rows that
// stay in their own files.


#include "shared/keyboard.hpp"
#include "shared/crt_memory.hpp"
#include "shared/mouse.hpp"
#define LOWORD(l) ((unsigned short)(l))
#define HIWORD(l) ((unsigned short)(((unsigned long)(l) >> 16) & 0xFFFF))
#define WM_KEYDOWN 0x0100
#define WM_KEYUP 0x0101
#define WM_CHAR 0x0102
#define WM_SYSKEYDOWN 0x0104
#define WM_SYSKEYUP 0x0105
#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP 0x0202
#define WM_LBUTTONDBLCLK 0x0203
#define WM_RBUTTONDOWN 0x0204
#define WM_RBUTTONUP 0x0205
#define WM_RBUTTONDBLCLK 0x0206
#define WM_MBUTTONDOWN 0x0207
#define WM_MBUTTONUP 0x0208
#define WM_MBUTTONDBLCLK 0x0209
#define VK_LBUTTON 0x01
#define VK_RBUTTON 0x02
#define VK_MBUTTON 0x04
#define VK_SCROLL 0x91


// 0x00532120: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void unknown_empty_00532120(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00532120 {

void  unknown_empty_00532120(void)
{
    return;
}

}  // namespace seed_00532120


// WWKeyboardClass::WWKeyboardClass() (0x00532130)
// Empty queue, no mouse position, all keys up.

WWKeyboardClass::WWKeyboardClass() :
    MouseQX(0),
    MouseQY(0),
    unknown_0C(0, 0),
    Head(0),
    Tail(0)
{
    memset(KeyState, 0, sizeof(KeyState));
}


// KeyNumType WWKeyboardClass::Buff_Get() (0x00532160)
// Waits for a key and removes it with the data queued behind it: the
// position of a mouse key, the character of a 0x0A key.

KeyNumType WWKeyboardClass::Buff_Get()
{
    while (!Check()) {
    }
    KeyNumType key = Fetch_Element();
    if (Is_Mouse_Key(key)) {
        MouseQX = Fetch_Element();
        MouseQY = Fetch_Element();
        unknown_0C = Point2D(MouseQX, MouseQY);
    } else if (Is_Char_Key(key)) {
        unknown_08 = Fetch_Element();
    }
    return key;
}


// int __fastcall WWKeyboardClass::Is_Mouse_Key(KeyNumType key) (0x00532290)
// Whether the key (low byte) is a mouse button: VK_LBUTTON, VK_MBUTTON or
// VK_RBUTTON.

int __fastcall WWKeyboardClass::Is_Mouse_Key(KeyNumType key)
{
    key &= 0xFF;
    return key == 1 || key == 4 || key == 2;
}


// int __fastcall WWKeyboardClass::Is_Char_Key(KeyNumType key) (0x005322C0)
// Whether the key is the 0x0A marker that a character follows in the queue.
// Assumes: name hypothesis.

int __fastcall WWKeyboardClass::Is_Char_Key(KeyNumType key)
{
    key &= 0xFF;
    return key == 0x0A;
}


// KeyNumType WWKeyboardClass::Check() (0x005322D0)
// Lets the window messages fill the queue, then returns the next key without
// removing it, or 0.

KeyNumType WWKeyboardClass::Check()
{
    Fill_Buffer_From_System();
    if (Is_Buffer_Empty()) {
        return 0;
    }
    return Peek_Element();
}


// KeyNumType WWKeyboardClass::Get() (0x00532320)
// Waits for a key and removes it (see Buff_Get).

KeyNumType WWKeyboardClass::Get()
{
    while (!Check()) {
    }
    return Buff_Get();
}


// bool WWKeyboardClass::Put(KeyNumType key) (0x00532490)
// Queues a key; false when the queue is full.

bool WWKeyboardClass::Put(KeyNumType key)
{
    if (!Is_Buffer_Full()) {
        Put_Element(key);
        return true;
    }
    return false;
}


// bool WWKeyboardClass::Put_Key_Message(KeyNumType key, bool release) (0x005324D0)
// Records the key state, adds the shift/control/alt bits of a keyboard key
// and the release bit, and queues it. Alt+tab is dropped; while the keyboard
// is ignored only VK_ESCAPE gets through.

bool WWKeyboardClass::Put_Key_Message(KeyNumType key, bool release)
{
    if (unknown_flag_a40d44 && key != 0x1B) {
        return true;
    }
    Unknown_005233A0(key, release);
    if (!Is_Mouse_Key(key) && !Is_Char_Key(key)) {
        if ((Unknown_005233D0(0x10) & 0x8000) != 0) {
            key |= 0x100;
        }
        if ((Unknown_005233D0(0x11) & 0x8000) != 0) {
            key |= 0x200;
        }
        if ((Unknown_005233D0(0x12) & 0x8000) != 0) {
            if (key == 0x09) {
                return true;
            }
            key |= 0x400;
        }
    }
    if (release) {
        key |= 0x800;
    }
    return Put(key);
}


// bool WWKeyboardClass::Put_Mouse_Message(KeyNumType key, int x, int y, bool release) (0x005325C0)
// Queues a mouse key and its position when three slots are free.

bool WWKeyboardClass::Put_Mouse_Message(KeyNumType key, int x, int y, bool release)
{
    if (unknown_flag_a40d44) {
        return true;
    }
    if (Available() >= 3 && Is_Mouse_Key(key)) {
        Put_Key_Message(key, release);
        Put((KeyNumType)x);
        Put((KeyNumType)y);
        return true;
    }
    return false;
}


// bool WWKeyboardClass::Put_Char(KeyNumType character) (0x005326A0)
// Queues the 0x0A marker key followed by a character when two slots are free.
// Assumes: name hypothesis; it has no callers (Message_Handler's WM_CHAR case
// queues the same two keys itself).

bool WWKeyboardClass::Put_Char(KeyNumType character)
{
    if (unknown_flag_a40d44) {
        return true;
    }
    if (Available() >= 2) {
        Put_Key_Message(0x0A);
        Put(character & 0xFF);
        return true;
    }
    return false;
}


// char WWKeyboardClass::To_ASCII(KeyNumType key) (0x00532720)
// The character a key produces (ToAscii with the key's own shift, control
// and alt bits in KeyState); 0 for releases and keys without one.
// VK_HOME and VK_END give backspace and backslash.
// The ToAscii buffer is a 10-byte char array (the 12-byte frame).

extern "C" __declspec(dllimport) unsigned int __stdcall MapVirtualKeyA(unsigned int code, unsigned int type);
extern "C" __declspec(dllimport) int __stdcall ToAscii(unsigned int key, unsigned int scan, unsigned char const *state,
                                                       unsigned short *buffer, unsigned int flags);

char WWKeyboardClass::To_ASCII(KeyNumType key)
{
    if (key & 0x800) {
        return 0;
    }
    if ((key & 0xFF) == 0x24) {
        return 8;
    }
    if ((key & 0xFF) == 0x23) {
        return '\\';
    }

    unsigned short shift = key & 0x100;
    if (shift) {
        KeyState[0x10] = 0x80;
    }
    unsigned short control = key & 0x200;
    if (control) {
        KeyState[0x11] = 0x80;
    }
    unsigned short alt = key & 0x400;
    if (alt) {
        KeyState[0x12] = 0x80;
    }

    char buffer[10];
    int scan = MapVirtualKeyA(key, 0);
    int result = ToAscii(key, scan, KeyState, (unsigned short *)buffer, 0);

    if (shift) {
        KeyState[0x10] = 0;
    }
    if (control) {
        KeyState[0x11] = 0;
    }
    if (alt) {
        KeyState[0x12] = 0;
    }
    if (result != 1) {
        return 0;
    }
    return buffer[0];
}


// char WWKeyboardClass::To_Char(KeyNumType key) (0x00532800)
// The character of a queued key for a text field: backspace, return and escape pass through,
// 0x0A gives the character queued after it (unknown_08, return as 0), 0x23 a backslash,
// 0x24 a backspace; released keys (0x800) and others give 0.

char WWKeyboardClass::To_Char(KeyNumType key)
{
    if ((key & 0x800) == 0) {
        KeyNumType ch = (KeyNumType)(key & 0xFF);
        switch (ch) {
            case 0x24:
                return 0x08;


            case 0x23:
                return '\\';

            case 0x0A:
            {
                char character = (char)unknown_08;
                if (character == 0x0D) {
                    character = 0;
                }
                unknown_08 = 0;
                return character;
            }

            case 0x08:
            case 0x0D:
            case 0x1B:
                return (char)ch;
        }
    }
    return 0;
}


// bool WWKeyboardClass::Down(KeyNumType key) (0x00532890)
// Whether the key is held now; the left and right mouse buttons are swapped
// when the system swaps them (SM_SWAPBUTTON).

extern "C" __declspec(dllimport) int __stdcall GetSystemMetrics(int index);

bool WWKeyboardClass::Down(KeyNumType key)
{
    key &= 0xFF;
    if (key == 1 || key == 2) {
        if (GetSystemMetrics(0x17) == 1) {
            key = key == 1 ? 2 : 1;
        }
    }
    return Unknown_005233F0(key) != 0;
}


// KeyNumType WWKeyboardClass::Fetch_Element() (0x005328E0)
// Removes and returns the next queued element, 0 when empty.

KeyNumType WWKeyboardClass::Fetch_Element()
{
    KeyNumType value = 0;
    if (Head != Tail) {
        value = Buffer[Head];
        Head = (Head + 1) % 256;
    }
    return value;
}


// KeyNumType WWKeyboardClass::Peek_Element() (0x00532920)
// The next queued element without removing it, 0 when empty.

KeyNumType WWKeyboardClass::Peek_Element()
{
    if (!Is_Buffer_Empty()) {
        return Buffer[Head];
    }
    return 0;
}


// bool WWKeyboardClass::Put_Element(KeyNumType key) (0x00532940)
// Appends an element; false when the queue is full.

bool WWKeyboardClass::Put_Element(KeyNumType key)
{
    if (!Is_Buffer_Full()) {
        int next = (Tail + 1) % 256;
        Buffer[Tail] = key;
        Tail = next;
        return true;
    }
    return false;
}


// bool WWKeyboardClass::Is_Buffer_Full() (0x00532980)
// Written as an if with two returns: the expression forms compile alike
// standalone but are cheaper to inline, and the original's callers
// (Buff_Get, Get) stop expanding at it.

bool WWKeyboardClass::Is_Buffer_Full()
{
    if ((Tail + 1) % 256 == Head) {
        return true;
    }
    return false;
}


// bool WWKeyboardClass::Is_Buffer_Empty() (0x005329A0)
// Written as an if with two returns (see Is_Buffer_Full).

bool WWKeyboardClass::Is_Buffer_Empty()
{
    if (Head == Tail) {
        return true;
    }
    return false;
}


// void WWKeyboardClass::Fill_Buffer_From_System() (0x005329C0)
// Unless the queue is full, lets the window messages add keys.

void WWKeyboardClass::Fill_Buffer_From_System()
{
    if (!Is_Buffer_Full()) {
        Unknown_005B5EE0();
    }
}


// void WWKeyboardClass::Clear() (0x005329F0)
// Discards everything queued, including keys the pending window messages add.

void WWKeyboardClass::Clear()
{
    Fill_Buffer_From_System();
    Head = Tail;
    Fill_Buffer_From_System();
    Head = Tail;
}


// int WWKeyboardClass::Message_Handler(HWND window, UINT message, WPARAM wparam, LPARAM lparam) (0x00532A60)
// Queues the keyboard and mouse-button messages (mouse keys with the converted screen position)
// and passes them to DefWindowProc; other messages give 0 (TibSun keyboard.cpp shape).

// The Windows SDK values this handler uses (win32_api.hpp declares no message constants).

int WWKeyboardClass::Message_Handler(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    bool processed = false;

    POINT point;
    point.x = LOWORD(lparam);
    point.y = HIWORD(lparam);
    ClientToScreen(window, &point);
    LONG x = point.x;
    LONG y = point.y;
    if (WWMouse != 0) WWMouse->Convert_Coordinate((int &)x, (int &)y);

    switch (message) {
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
            if (wparam != VK_SCROLL && !(lparam & (1 << 30))) {
                Put_Key_Message((KeyNumType)wparam);
            }
            processed = true;
            break;

        case WM_SYSKEYUP:
        case WM_KEYUP:
            Put_Key_Message((KeyNumType)wparam, true);
            processed = true;
            break;

        case WM_LBUTTONDOWN:
            Put_Mouse_Message(VK_LBUTTON, x, y);
            processed = true;
            break;

        case WM_LBUTTONUP:
            Put_Mouse_Message(VK_LBUTTON, x, y, true);
            processed = true;
            break;

        case WM_LBUTTONDBLCLK:
            Put_Mouse_Message(VK_LBUTTON, x, y);
            Put_Mouse_Message(VK_LBUTTON, x, y, true);
            processed = true;
            break;

        case WM_CHAR:
            if (unknown_flag_a3dc89) {
                Put_Char((KeyNumType)wparam);
            }
            processed = true;
            break;

        case WM_MBUTTONDOWN:
            Put_Mouse_Message(VK_MBUTTON, x, y);
            processed = true;
            break;

        case WM_MBUTTONUP:
            Put_Mouse_Message(VK_MBUTTON, x, y, true);
            processed = true;
            break;

        case WM_MBUTTONDBLCLK:
            Put_Mouse_Message(VK_MBUTTON, x, y);
            Put_Mouse_Message(VK_MBUTTON, x, y, true);
            processed = true;
            break;

        case WM_RBUTTONDOWN:
            Put_Mouse_Message(VK_RBUTTON, x, y);
            processed = true;
            break;

        case WM_RBUTTONUP:
            Put_Mouse_Message(VK_RBUTTON, x, y, true);
            processed = true;
            break;

        case WM_RBUTTONDBLCLK:
            Put_Mouse_Message(VK_RBUTTON, x, y);
            Put_Mouse_Message(VK_RBUTTON, x, y, true);
            processed = true;
            break;

        default:
            break;
    }

    if (processed) {
        DefWindowProcA(window, message, wparam, lparam);
        return true;
    }
    return false;
}


// int WWKeyboardClass::Available() (0x00532E60)
// The number of free slots in the queue.
// Assumes: not called out of line (the copy is the unit's own; every use in
// keyboard.cpp is expanded).

int WWKeyboardClass::Available()
{
    return 256 - abs(Tail - Head);
}
