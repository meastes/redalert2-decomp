
// ---------------------------------------------------------------------------------------------
// Native glue (lane Y2), appended to the wwmous unit: WinMain (native/yr/glue/startup_winmain.inc)
// makes the mouse here, where gamemd.exe's WWMouseClass (wwmous.h) is the one in scope.
// 0x006BDEF9: new WWMouseClass(VisibleSurface, MainWindow), then its Hide_Mouse (slot 3).
// ---------------------------------------------------------------------------------------------
struct VirtualObject;

VirtualObject *yr_new_wwmouse(DSurface *surface, HWND window)
{
	return (VirtualObject *)new WWMouseClass(surface, window);
}

void yr_wwmouse_hide(VirtualObject *mouse)
{
	((WWMouseClass *)mouse)->Hide_Mouse();
}

// Init_Game's mouse shapes (0x0052C35F): Hide_Mouse, then Set_Cursor(Point2D(0, 0), MOUSE.SHA, 0).
void yr_wwmouse_set_cursor(VirtualObject *mouse, void const *cursor)
{
	((WWMouseClass *)mouse)->Set_Cursor(Point2D(0, 0), (ShapeSet const *)cursor, 0);
}
