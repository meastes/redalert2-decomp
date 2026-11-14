// wwmouse.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wwmouse.json carries the addresses and the rows that
// stay in their own files.


#include "shared/wwmouse.hpp"


// void WWMouseClass::Calc_Confining_Rect() (0x00771C50)
// ConfiningRect = the window's client area in screen coordinates (name from
// the debug string). The explicit return keeps the constructor from expanding
// it inline (without it VC6 inlines it into 0x00771A20).

void WWMouseClass::Calc_Confining_Rect()
{
    RECT rect;
    GetClientRect(Window, &rect);
    POINT topleft;
    topleft.x = rect.left;
    topleft.y = rect.top;
    ClientToScreen(Window, &topleft);
    POINT bottomright;
    bottomright.x = rect.right;
    bottomright.y = rect.bottom;
    ClientToScreen(Window, &bottomright);
    ConfiningRect = Rect(topleft.x, topleft.y, bottomright.x - topleft.x, bottomright.y - topleft.y);
    Debug_Printf("Calc_Confining_Rect(%d,%d,%d,%d)\n", topleft.x, topleft.y, bottomright.x - topleft.x,
                 bottomright.y - topleft.y);
    return;
}


// int WWMouseClass::Get_Mouse_State() const (0x00771CE0, slot 10)
// The hide nesting state (0 shown, negative hidden).

int WWMouseClass::Get_Mouse_State() const
{
    return MouseState;
}


// void WWMouseClass::Set_Cursor(Point2D const &hotspot, ShapeFileStruct const *cursor, int frame) (0x00771CF0, slot 1)
// Selects the cursor shape, frame and hotspot; a captured, visible cursor is
// erased and redrawn around the change.

void WWMouseClass::Set_Cursor(Point2D const &hotspot, ShapeFileStruct const *cursor, int frame)
{
    if (cursor != 0) {
        if (Is_Captured()) {
            Block_Mouse();
            if (!Is_Hidden()) {
                Low_Hide_Mouse();
            }
            MouseCursor = cursor;
            MouseFrame = frame;
            HotspotX = hotspot.X;
            HotspotY = hotspot.Y;
            if (!Is_Hidden()) {
                Low_Show_Mouse();
            }
            Unblock_Mouse();
        } else {
            MouseCursor = cursor;
            MouseFrame = frame;
            HotspotX = hotspot.X;
            HotspotY = hotspot.Y;
        }
    }
}


// bool WWMouseClass::Has_Cursor() const (0x00771F40)
// A cursor shape is set. Never called (inlined into Validate_Backgrounds);
// name is a hypothesis. The conditional form gives setne without a cleared eax.

bool WWMouseClass::Has_Cursor() const
{
    return MouseCursor != 0 ? true : false;
}


// bool WWMouseClass::Validate_Backgrounds() (0x00771F50)
// Replaces save-under surfaces smaller than the cursor shape and creates the
// missing ones in the screen's pixel format; true when all three exist.
// Name is a hypothesis.

bool WWMouseClass::Validate_Backgrounds()
{
    if (Has_Cursor()) {
    if (Background != 0
        && (MouseCursor->Width > Background->Get_Width() || MouseCursor->Height > Background->Get_Height())) {
        delete Background;
        Background = 0;
    }
    if (SurfaceBackground != 0
        && (MouseCursor->Width > SurfaceBackground->Get_Width()
            || MouseCursor->Height > SurfaceBackground->Get_Height())) {
        delete SurfaceBackground;
        SurfaceBackground = 0;
    }
    if (SidebarBackground != 0
        && (MouseCursor->Width > SidebarBackground->Get_Width()
            || MouseCursor->Height > SidebarBackground->Get_Height())) {
        delete SidebarBackground;
        SidebarBackground = 0;
    }
    if (ScreenSurface != 0) {
        if (Background == 0) {
            Background = new BSurface(MouseCursor->Get_Width(), MouseCursor->Get_Height(), ScreenSurface->Bytes_Per_Pixel());
        }
        if (SurfaceBackground == 0) {
            SurfaceBackground
                = new BSurface(MouseCursor->Get_Width(), MouseCursor->Get_Height(), ScreenSurface->Bytes_Per_Pixel());
        }
        if (SidebarBackground == 0) {
            SidebarBackground
                = new BSurface(MouseCursor->Get_Width(), MouseCursor->Get_Height(), ScreenSurface->Bytes_Per_Pixel());
        }
    }
    return Background != 0 && SurfaceBackground != 0 && SidebarBackground != 0;
    }
    return false;
}


// Rect WWMouseClass::Get_Cursor_Rect() (0x00772170)
// Rectangle of the current cursor frame placed at the drawing position, relative
// to ConfiningRect (empty without a cursor). Name is a hypothesis.

Rect WWMouseClass::Get_Cursor_Rect()
{
    Rect rect(0, 0, 0, 0);
    if (MouseCursor != 0) {
        Block_Mouse();
        rect = MouseCursor->Get_Frame_Rect(MouseFrame);
        rect.X += DrawX - HotspotX;
        rect.Y += DrawY - HotspotY;
        Unblock_Mouse();
    }
    return rect;
}


// void WWMouseClass::Save_Background() (0x00772250)
// Records the cursor rectangle and copies the screen pixels under it into
// Background. Name is a hypothesis.

void WWMouseClass::Save_Background()
{
    if (Validate_Backgrounds() && ScreenSurface != 0) {
        BackgroundRect = Get_Cursor_Rect();
        Rect rect = BackgroundRect;
        rect.X += ConfiningRect.X;
        rect.Y += ConfiningRect.Y;
        Background->Copy_From(Rect(0, 0, rect.Width, rect.Height), *ScreenSurface, rect, false, true);
    }
}


// void WWMouseClass::Restore_Background() (0x00772390)
// Copies Background back to the screen over the last cursor rectangle.
// Name is a hypothesis.

void WWMouseClass::Restore_Background()
{
    if (BackgroundRect.Is_Valid() && ScreenSurface != 0 && Background != 0) {
        Rect toarea = BackgroundRect;
        toarea.X += ConfiningRect.X;
        toarea.Y += ConfiningRect.Y;
        ScreenSurface->Copy_From(toarea, *Background, Rect(0, 0, toarea.Width, toarea.Height), false,
                                 true);
    }
}


// void WWMouseClass::Draw_Mouse(Surface *surface, bool issidebar) (0x00772410, slot 15)
// Draws the captured cursor onto another surface (the sidebar one, or a
// surface offset by the sidebar width), saving what it covers first.
// Unresolved: the globals giving the offsets (0x00839638, 0x00839648,
// 0x00839948 a surface, flag 0x00A40B30).

void WWMouseClass::Draw_Mouse(Surface *surface, bool issidebar)
{
    if (Is_Hidden() || !Is_Captured() || surface == 0 || surface == ScreenSurface) {
        return;
    }
    int xoffset;
    BSurface *background;
    Rect *rect;
    if (issidebar) {
        xoffset = unknown_flag_a40b30 ? -unknown_int_839638 : 0;
        background = SidebarBackground;
        rect = &SidebarBackgroundRect;
    } else {
        xoffset = unknown_flag_a40b30 ? 0 : (-unknown_surface_839948->Get_Width() > surface->Get_Width() - unknown_int_839648
                ? -unknown_surface_839948->Get_Width()
                : surface->Get_Width() - unknown_int_839648);
        background = SurfaceBackground;
        rect = &SurfaceBackgroundRect;
    }
    if (background == 0 || Background == 0) {
        rect->Width = 0;
        return;
    }
    Block_Mouse();
    *rect = BackgroundRect;
    rect->X += xoffset;
    background->Copy_From(Rect(0, 0, rect->Width, rect->Height), *surface, *rect, false, true);
    if (issidebar && rect->X < 0 && -rect->X < Background->Get_Width()) {
        Background->Copy_From(Rect(-rect->X, 0, rect->Width, rect->Height), *surface,
                              Rect(0, rect->Y, rect->Width, rect->Height), false, true);
    } else {
        Background->Copy_From(Rect(0, 0, rect->Width, rect->Height), *surface, *rect, false, true);
    }
    Draw_Cursor(surface, xoffset, 0);
    Unblock_Mouse();
}


// void WWMouseClass::Erase_Mouse(Surface *surface, bool issidebar) (0x00772620, slot 16)
// Restores the pixels Draw_Mouse saved on that surface.

void WWMouseClass::Erase_Mouse(Surface *surface, bool issidebar)
{
    if (Is_Hidden() || !Is_Captured() || surface == 0 || surface == ScreenSurface || SurfaceBackground == 0
        || SidebarBackground == 0) {
        return;
    }
    BSurface *background;
    Rect rect;
    if (issidebar) {
        background = SidebarBackground;
        rect = SidebarBackgroundRect;
    } else {
        background = SurfaceBackground;
        rect = SurfaceBackgroundRect;
    }
    if (background != 0) {
        Block_Mouse();
        surface->Copy_From(rect, *background, Rect(0, 0, rect.Width, rect.Height), false, true);
        Unblock_Mouse();
    }
}


// void WWMouseClass::Draw_Cursor(Surface *surface, int xoffset, int yoffset) (0x00772740)
// Wraps the cursor frame's pixels in a temporary 8-bit BSurface and draws it
// through unknown_convert_8323c8 at the cursor rectangle (offset), clipped to
// the confining rectangle on the screen or to the surface otherwise.
// Unresolved: the drawing function 0x0049F3A0 and its trailing arguments (0, 0, 0, 0, 1000, 0).

void WWMouseClass::Draw_Cursor(Surface *surface, int xoffset, int yoffset)
{
    if (MouseCursor == 0 || surface == 0 || unknown_convert_8323c8 == 0) {
        return;
    }
    Rect rect = BackgroundRect;
    rect.X += xoffset;
    rect.Y += yoffset;
    BSurface cursor(rect.Width, rect.Height, 1, (void *)MouseCursor->Get_Frame_Data(MouseFrame));
    if (surface == ScreenSurface) {
        unknown_blit_49f3a0(*surface, unknown_convert_8323c8, cursor, Rect(0, 0, rect.Width, rect.Height), RectPoint2D(rect.X, rect.Y),
                            ConfiningRect, 0, 0, 0, 0, 1000, 0);
    } else {
        unknown_blit_49f3a0(*surface, unknown_convert_8323c8, cursor, Rect(0, 0, rect.Width, rect.Height), RectPoint2D(rect.X, rect.Y),
                            surface->Get_Rect(), 0, 0, 0, 0, 1000, 0);
    }
}


// void WWMouseClass::Low_Show_Mouse() (0x007728A0)
// Saves the background and draws the cursor on the screen under the mutex.
// Never called (inlined everywhere); name is a hypothesis.

void WWMouseClass::Low_Show_Mouse()
{
    Block_Mouse();
    Save_Background();
    Draw_Cursor(ScreenSurface, 0, 0);
    Unblock_Mouse();
}


// void WWMouseClass::Low_Hide_Mouse() (0x00772A00)
// Erases the cursor from the screen under the mutex. Never called (inlined
// everywhere); name is a hypothesis.

void WWMouseClass::Low_Hide_Mouse()
{
    Block_Mouse();
    Restore_Background();
    Unblock_Mouse();
}


// void WWMouseClass::Show_Mouse() (0x00772AC0, slot 4)
// Undoes one Hide_Mouse; at state 0 draws the captured cursor or shows the
// Windows cursor.

void WWMouseClass::Show_Mouse()
{
    if (MouseState != 0) {
        Block_Mouse();
        MouseState++;
        if (MouseState == 0) {
            if (Is_Captured()) {
                Low_Show_Mouse();
            } else {
                while (ShowCursor(1) < 0) {
                }
            }
        }
        if (MouseState > 0) {
            MouseState = 0;
        }
        Unblock_Mouse();
    }
}


// void WWMouseClass::Hide_Mouse() (0x00772CA0, slot 3)
// Hides the cursor (nesting): at state 0 erases the captured cursor or hides
// the Windows cursor.

void WWMouseClass::Hide_Mouse()
{
    Block_Mouse();
    if (MouseState == 0) {
        if (Is_Captured()) {
            Low_Hide_Mouse();
        } else {
            while (ShowCursor(0) >= 0) {
            }
        }
    }
    MouseState--;
    Unblock_Mouse();
}


// void WWMouseClass::Capture_Mouse() (0x00772DD0, slot 6)
// Switches to the software cursor: draws it if visible and hides the Windows
// cursor (name from the debug string).

void WWMouseClass::Capture_Mouse()
{
    if (this != 0 && !Is_Captured()) {
        Debug_Printf("Capture_Mouse()\n");
        Block_Mouse();
        IsCaptured = true;
        if (!Is_Hidden()) {
            Low_Show_Mouse();
        }
        ShowCursor(0);
        Unblock_Mouse();
    }
}


// void WWMouseClass::Release_Mouse() (0x00772FA0, slot 5)
// Returns to the Windows cursor: erases the software cursor and shows the
// Windows one if visible (name from the debug string).

void WWMouseClass::Release_Mouse()
{
    if (this != 0 && Is_Captured()) {
        Debug_Printf("Release_Mouse()\n");
        Block_Mouse();
        IsCaptured = false;
        if (!Is_Hidden()) {
            Low_Hide_Mouse();
            while (ShowCursor(1) < 0) {
            }
        }
        Unblock_Mouse();
    }
}


// void WWMouseClass::Conditional_Hide_Mouse(Rect region) (0x007730E0, slot 8)
// Hides the mouse regardless of the region.

void WWMouseClass::Conditional_Hide_Mouse(Rect)
{
    Hide_Mouse();
}


// void WWMouseClass::Conditional_Show_Mouse() (0x007730F0, slot 9)
// Shows the mouse (tail call of Show_Mouse).

void WWMouseClass::Conditional_Show_Mouse()
{
    Show_Mouse();
}


// void WWMouseClass::Convert_Coordinate(int &x, int &y) const (0x00773100, slot 17)
// Screen to client coordinates clamped to the confining rectangle (negative y
// is kept when unknown_flag_b2af58 is set).

void WWMouseClass::Convert_Coordinate(int &x, int &y) const
{
    x -= ConfiningRect.X;
    y -= ConfiningRect.Y;
    if (x < 0) {
        x = 0;
    }
    if (y < 0 && !unknown_flag_b2af58) {
        y = 0;
    }
    if (x >= ConfiningRect.Width) {
        x = ConfiningRect.Width - 1;
    }
    if (y >= ConfiningRect.Height) {
        y = ConfiningRect.Height - 1;
    }
}


// void WWMouseClass::Get_Cursor_Position(int &x, int &y) (0x00773160)
// The Windows cursor position in client coordinates. Never called (inlined into
// Process_Mouse); name is a hypothesis.

void WWMouseClass::Get_Cursor_Position(int &x, int &y)
{
    POINT point;
    GetCursorPos(&point);
    x = point.x;
    y = point.y;
    Convert_Coordinate(x, y);
}


// void WWMouseClass::Move_Cursor(int x, int y, bool force) (0x007731A0)
// Moves the drawn cursor to x, y (redrawing a captured, visible cursor when
// drawing is enabled). Never called (inlined into Process_Mouse); name is a
// hypothesis. Unresolved: the flag 0x00A40D28 (also tested by DSurface::Lock).

void WWMouseClass::Move_Cursor(int x, int y, bool force)
{
    Block_Mouse();
    if (x != DrawX || y != DrawY || force) {
        if (Is_Captured() && !Is_Hidden() && unknown_flag_a40d28) {
            Low_Hide_Mouse();
        }
        DrawX = x;
        DrawY = y;
        if (Is_Captured() && !Is_Hidden() && unknown_flag_a40d28) {
            Low_Show_Mouse();
        }
    }
    Unblock_Mouse();
}


// void WWMouseClass::Process_Mouse() (0x00773400)
// Mouse thread callback (TibSun WWMouseClass::Process_Mouse): reads the
// Windows cursor (unless 0x00522E80 holds the position), then moves the
// drawn cursor; while the screen surface is unavailable it only records the
// position and marks the next move as forced.

void WWMouseClass::Process_Mouse()
{
    if (ScreenSurface != 0) {
        Block_Mouse();

        int x;
        int y;
        if (unknown_function_522e80()) {
            x = MouseX;
            y = MouseY;
        } else {
            Get_Cursor_Position(x, y);
            MouseX = x;
            MouseY = y;
        }
        if (!ScreenSurface->Unknown_Slot26() && !unknown_flag_b2af68) {
            DrawX = x;
            DrawY = y;
            unknown_flag_b2af68 = true;
        } else {
            Move_Cursor(x, y, unknown_flag_b2af68);
            unknown_flag_b2af68 = false;
        }

        Unblock_Mouse();
    }
}


// void Block_Mouse() (0x00773630)
// Takes the mouse mutex, warning after 10 seconds (the warning names line 1277
// of the original file, hence the literal). VC6 expands it into the earlier
// callers; Show/Capture_Mouse call this copy.

void Block_Mouse()
{
    if (WaitForSingleObject(MouseMutex, 10000) == WAIT_TIMEOUT) {
        Debug_Printf("Warning: Probable deadlock occurred on MouseMutex. %s, line %d\n", "D:\\Ra2\\wwmous.cpp", 1277);
    }
}


// void Unblock_Mouse() (0x00773660)
// Releases the mouse mutex (always expanded inline; this copy is never called).

void Unblock_Mouse()
{
    ReleaseMutex(MouseMutex);
}


// bool WWMouseClass::Is_Hidden() const (0x00773690, slot 2)
// Inline in shared/wwmouse.hpp: MouseState < 0.


// bool WWMouseClass::Is_Captured() const (0x007736A0, slot 7)
// Inline in shared/wwmouse.hpp.


// int WWMouseClass::Get_Mouse_X() const (0x007736B0, slot 11)
// Inline in shared/wwmouse.hpp.


// int WWMouseClass::Get_Mouse_Y() const (0x007736C0, slot 12)
// Inline in shared/wwmouse.hpp.


// Point2D WWMouseClass::Get_Mouse_XY() const (0x007736D0, slot 13)
// Inline in shared/wwmouse.hpp.


// void WWMouseClass::Set_Mouse_XY(int x, int y) (0x007736F0, slot 14)
// Inline in shared/wwmouse.hpp.


// WWMouseClass scalar deleting destructor (0x00773710, slot 0)
// Compiler generated with the WWMouseClass vtable; calls the destructor.
