/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// Function comment blocks from mischa85/RedAlert2 392e904 src/keyboard.cpp (matching RA2 1.006 decomp), placed above our
// own notes; code unchanged.
// Parameter names from mischa85/RedAlert2 392e904 src/keyboard.cpp where both define the function (names only).
// Local variable names from mischa85/RedAlert2 392e904 src/keyboard.cpp where both functions declare the same local types (names only).

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


/***********************************************************************************************
 * WWKeyboardClass::WWKeyBoardClass -- Construction for Westwood Keyboard Class                *
 *                                                                                             *
 * INPUT:      none                                                                            *
 *                                                                                             *
 * OUTPUT:     none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/16/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
// (0x00532130)
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


/***********************************************************************************************
 * WWKeyboardClass::Buff_Get -- Lowlevel function to get a key from key buffer                 *
 *                                                                                             *
 * INPUT:      none                                                                            *
 *                                                                                             *
 * OUTPUT:     int      - the key value that was pulled from buffer (includes bits)            *
 *                                                                                             *
 * WARNINGS:   If the key was a mouse event MouseQX and MouseQY will be updated                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/17/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
// (0x00532160)
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


/***********************************************************************************************
 * WWKeyboardClass::Is_Mouse_Key -- Checks to see if specified key refers to the mouse.        *
 *                                                                                             *
 *    This checks the specified key code to see if it refers to the mouse buttons.             *
 *                                                                                             *
 * INPUT:   key   -- The key to check.                                                         *
 *                                                                                             *
 * OUTPUT:  bool; Is the key a mouse button key?                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00532290)
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


/***********************************************************************************************
 * WWKeyboardClass::Check -- Checks to see if a key is in the buffer                           *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/16/1995 PWG : Created.                                                                 *
 *   09/24/1996 JLB : Converted to new style keyboard system.                                  *
 *=============================================================================================*/
// (0x005322D0)
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


/***********************************************************************************************
 * WWKeyboardClass::Get -- Logic to get a metakey from the buffer                              *
 *                                                                                             *
 * INPUT:      none                                                                            *
 *                                                                                             *
 * OUTPUT:     int      - the meta key taken from the buffer.                                  *
 *                                                                                             *
 * WARNINGS:   This routine will not return until a keypress is received                       *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/16/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
// (0x00532320)
// Waits for a key and removes it (see Buff_Get).

KeyNumType WWKeyboardClass::Get()
{
    while (!Check()) {
    }
    return Buff_Get();
}


/***********************************************************************************************
 * WWKeyboardClass::Put -- Logic to insert a key into the keybuffer]                           *
 *                                                                                             *
 * INPUT:      int       - the key to insert into the buffer                                   *
 *                                                                                             *
 * OUTPUT:     bool      - true if key is sucessfuly inserted.                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/16/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
// (0x00532490)
// Queues a key; false when the queue is full.

bool WWKeyboardClass::Put(KeyNumType key)
{
    if (!Is_Buffer_Full()) {
        Put_Element(key);
        return true;
    }
    return false;
}


/***********************************************************************************************
 * WWKeyboardClass::Put_Key_Message -- Translates and inserts wParam into Keyboard Buffer      *
 *                                                                                             *
 * INPUT:                                                                                      *
 *                                                                                             *
 * OUTPUT:                                                                                     *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   10/16/1995 PWG : Created.                                                                 *
 *=============================================================================================*/
// (0x005324D0)
// Records the key state, adds the shift/control/alt bits of a keyboard key
// and the release bit, and queues it. Alt+tab is dropped; while the keyboard
// is ignored only VK_ESCAPE gets through.

bool WWKeyboardClass::Put_Key_Message(KeyNumType vk_key, bool release)
{
    if (IgnoreInput && vk_key != 0x1B) {
        return true;
    }
    Set_Key_State(vk_key, release);
    if (!Is_Mouse_Key(vk_key) && !Is_Char_Key(vk_key)) {
        if ((Get_Key_State(0x10) & 0x8000) != 0) {
            vk_key |= 0x100;
        }
        if ((Get_Key_State(0x11) & 0x8000) != 0) {
            vk_key |= 0x200;
        }
        if ((Get_Key_State(0x12) & 0x8000) != 0) {
            if (vk_key == 0x09) {
                return true;
            }
            vk_key |= 0x400;
        }
    }
    if (release) {
        vk_key |= 0x800;
    }
    return Put(vk_key);
}


/***********************************************************************************************
 * WWKeyboardClass::Put_Mouse_Message -- Stores a mouse type message into the keyboard buffer. *
 *                                                                                             *
 *    This routine will store the mouse type event into the keyboard buffer. It also checks    *
 *    to ensure that there is enough room in the buffer so that partial mouse events won't     *
 *    be recorded.                                                                             *
 *                                                                                             *
 * INPUT:   vk_key   -- The mouse key message itself.                                          *
 *                                                                                             *
 *          x,y      -- The mouse coordinates at the time of the event.                        *
 *                                                                                             *
 *          release  -- Is this a mouse button release?                                        *
 *                                                                                             *
 * OUTPUT:  bool; Was the event stored sucessfully into the keyboard buffer?                   *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   11/02/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x005325C0)
// Queues a mouse key and its position when three slots are free.

bool WWKeyboardClass::Put_Mouse_Message(KeyNumType vk_key, int x, int y, bool release)
{
    if (IgnoreInput) {
        return true;
    }
    if (Available() >= 3 && Is_Mouse_Key(vk_key)) {
        Put_Key_Message(vk_key, release);
        Put((KeyNumType)x);
        Put((KeyNumType)y);
        return true;
    }
    return false;
}


/// <summary>
/// RA2: queues a typed character: a wheel key (VK 0x0A) followed by the character's code, which Buff_Get stores in
/// MouseWheel for To_Char.
/// </summary>
// (0x005326A0)
// Queues the 0x0A marker key followed by a character when two slots are free.
// Assumes: name hypothesis; it has no callers (Message_Handler's WM_CHAR case
// queues the same two keys itself).

bool WWKeyboardClass::Put_Char(KeyNumType character)
{
    if (IgnoreInput) {
        return true;
    }
    if (Available() >= 2) {
        Put_Key_Message(0x0A);
        Put(character & 0xFF);
        return true;
    }
    return false;
}


/***********************************************************************************************
 * WWKeyboardClass::To_ASCII -- Convert the key value into an ASCII representation.            *
 *                                                                                             *
 *    This routine will convert the key code specified into an ASCII value. This takes into    *
 *    consideration the language and keyboard mapping of the host Windows system.              *
 *                                                                                             *
 * INPUT:   key   -- The key code to convert into ASCII.                                       *
 *                                                                                             *
 * OUTPUT:  Returns with the key converted into ASCII. If the key has no ASCII equivalent,     *
 *          then '\0' is returned.                                                             *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00532720)
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


/// <summary>
/// RA2: the character a key stands for: a typed character (wheel key 0x0A, from Put_Char) is MouseWheel's low byte
/// (a carriage return gives none), Home and End are backspace and backslash, and backspace, return and escape are
/// themselves.
/// </summary>
// (0x00532800)
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


/***********************************************************************************************
 * WWKeyboardClass::Down -- Checks to see if the specified key is being held down.             *
 *                                                                                             *
 *    This routine will examine the key specified to see if it is currently being held down.   *
 *                                                                                             *
 * INPUT:   key   -- The key to check.                                                         *
 *                                                                                             *
 * OUTPUT:  bool; Is the specified key currently being held down?                              *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00532890)
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
    return Get_Async_Key_State(key) != 0;
}


/***********************************************************************************************
 * WWKeyboardClass::Fetch_Element -- Extract the next element in the keyboard buffer.          *
 *                                                                                             *
 *    This routine will extract the next pending element in the keyboard queue. If there is    *
 *    no element available, then NULL is returned.                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the element extracted from the queue. An empty queue is signified     *
 *          by a 0 return value.                                                               *
 *                                                                                             *
 * WARNINGS:                                                                                   *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x005328E0)
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


/***********************************************************************************************
 * WWKeyboardClass::Peek_Element -- Fetches the next element in the keyboard buffer.           *
 *                                                                                             *
 *    This routine will examine and return with the next element in the keyboard buffer but    *
 *    it will not alter or remove that element. Use this routine to see what is pending in     *
 *    the keyboard queue.                                                                      *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  Returns with the next element in the keyboard queue. If the keyboard buffer is     *
 *          empty, then 0 is returned.                                                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00532920)
// The next queued element without removing it, 0 when empty.

KeyNumType WWKeyboardClass::Peek_Element()
{
    if (!Is_Buffer_Empty()) {
        return Buffer[Head];
    }
    return 0;
}


/***********************************************************************************************
 * WWKeyboardClass::Put_Element -- Put a keyboard data element into the buffer.                *
 *                                                                                             *
 *    This will put one keyboard data element into the keyboard buffer. Typically, this data   *
 *    is a key code, but it might be mouse coordinates.                                        *
 *                                                                                             *
 * INPUT:   val   -- The data element to add to the keyboard buffer.                           *
 *                                                                                             *
 * OUTPUT:  bool; Was the keyboard element added successfully? A failure would indicate that   *
 *                the keyboard buffer is full.                                                 *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00532940)
// Appends an element; false when the queue is full.

bool WWKeyboardClass::Put_Element(KeyNumType val)
{
    if (!Is_Buffer_Full()) {
        int temp = (Tail + 1) % 256;
        Buffer[Tail] = val;
        Tail = temp;
        return true;
    }
    return false;
}


/***********************************************************************************************
 * WWKeyboardClass::Is_Buffer_Full -- Determines if the keyboard buffer is full.               *
 *                                                                                             *
 *    This routine will examine the keyboard buffer to determine if it is completely           *
 *    full of queued keyboard events.                                                          *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the keyboard buffer completely full?                                      *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x00532980)
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


/***********************************************************************************************
 * WWKeyboardClass::Is_Buffer_Empty -- Checks to see if the keyboard buffer is empty.          *
 *                                                                                             *
 *    This routine will examine the keyboard buffer to see if it contains no events at all.    *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  bool; Is the keyboard buffer currently without any pending events queued?          *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x005329A0)
// Written as an if with two returns (see Is_Buffer_Full).

bool WWKeyboardClass::Is_Buffer_Empty()
{
    if (Head == Tail) {
        return true;
    }
    return false;
}


/***********************************************************************************************
 * WWKeyboardClass::Fill_Buffer_From_Syste -- Extract and process any queued windows messages. *
 *                                                                                             *
 *    This routine will extract and process any windows messages in the windows message        *
 *    queue. It is presumed that the normal message handler will call the keyboard             *
 *    message processing function.                                                             *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x005329C0)
// Unless the queue is full, lets the window messages add keys.

void WWKeyboardClass::Fill_Buffer_From_System()
{
    if (!Is_Buffer_Full()) {
        Unknown_005B5EE0();
    }
}


/***********************************************************************************************
 * WWKeyboardClass::Clear -- Clears the keyboard buffer.                                       *
 *                                                                                             *
 *    This routine will clear the keyboard buffer of all pending keyboard events.              *
 *                                                                                             *
 * INPUT:   none                                                                               *
 *                                                                                             *
 * OUTPUT:  none                                                                               *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// (0x005329F0)
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

/***********************************************************************************************
 * WWKeyboardClass::Message_Handler -- Process a windows message as it relates to the keyboard *
 *                                                                                             *
 *    This routine will examine the Windows message specified. If the message relates to an    *
 *    event that the keyboard input system needs to process, then it will be processed         *
 *    accordingly.                                                                             *
 *                                                                                             *
 * INPUT:   window   -- Handle to the window receiving the message.                            *
 *                                                                                             *
 *          message  -- The message number of this event.                                      *
 *                                                                                             *
 *          wParam   -- The windows specific word parameter (meaning depends on message).      *
 *                                                                                             *
 *          lParam   -- The windows specific long word parameter (meaning is message dependant)*
 *                                                                                             *
 * OUTPUT:  bool; Was this keyboard message recognized and processed? A 'false' return value   *
 *                means that the message should be processed normally.                         *
 *                                                                                             *
 * WARNINGS:   none                                                                            *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *   09/30/1996 JLB : Created.                                                                 *
 *=============================================================================================*/
// The Windows SDK values this handler uses (win32_api.hpp declares no message constants).

int WWKeyboardClass::Message_Handler(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
    bool processed = false;

    POINT point;
    point.x = LOWORD(lParam);
    point.y = HIWORD(lParam);
    ClientToScreen(window, &point);
    LONG x = point.x;
    LONG y = point.y;
    if (WWMouse != 0) WWMouse->Convert_Coordinate((int &)x, (int &)y);

    switch (message) {
        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
            if (wParam != VK_SCROLL && !(lParam & (1 << 30))) {
                Put_Key_Message((KeyNumType)wParam);
            }
            processed = true;
            break;

        case WM_SYSKEYUP:
        case WM_KEYUP:
            Put_Key_Message((KeyNumType)wParam, true);
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
                Put_Char((KeyNumType)wParam);
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
        DefWindowProcA(window, message, wParam, lParam);
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
