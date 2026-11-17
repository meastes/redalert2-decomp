
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3), appended to the progress unit: ProgressScreenClass::Draw_Player,
// YR 0x00643720 (955 bytes, unmatched in ra2-decomp; counterpart of the RA2 tree's matched
// ProgressScreenClass::Draw_Player, src/recovered/progress.cpp, RA2 0x0061FC20). Written from
// gamemd.exe's code: the row's height from Fit_Size, the bar centred in it (Bar_Size), the flag
// (FUN_004E3560 by country; observers -3) and the name (Draw_Label) after the bar shape's width,
// the flag drawn magenta-transparent on HiddenSurface. Behaviour code for the native build; never
// part of an exact build.
// ---------------------------------------------------------------------------------------------
void ProgressScreenClass::Draw_Player(ColorScheme * scheme, wchar_t const * name, Point2D * pos, int index, bool usedialog)
{
	double ratio = (index < PlayerCount) ? PlayerProgress[index] / MainProgress : MainProgress;

	int fit_width = 0;
	int fit_height = 0;
	Fit_Size(&fit_width, &fit_height, 1, usedialog);
	int y = fit_height * index + pos->Y;

	Rect bounds = ((SHPStruct *)Buffer)->GetFrameBounds(0);
	int dialog_width = 0;
	int dialog_height = 0;
	if (usedialog) {
		Dialog_Size(&dialog_width, &dialog_height);
	}
	int cell_width = 0;
	int cell_height = 0;
	Font_Cell_Size(&cell_width, &cell_height);
	int name_width = dialog_width + cell_width + bounds.Width + 0x1A + 0xA;
	if (Field78 != -1) {
		name_width = Field78;
	}

	bounds = ((SHPStruct *)Buffer)->GetFrameBounds(0);
	Point2D bar = *pos;
	Fit_Size(&fit_width, &fit_height, 1, usedialog);
	int bar_width = 0;
	int bar_height = 0;
	Bar_Size(&bar_width, &bar_height);
	bar.X += 5;
	bar.Y = (fit_height - bar_height) / 2 + y;

	int item_x = pos->X + bounds.Width + 0x15;
	Point2D flag(item_x, pos->Y);
	if (usedialog) {
		Fit_Size(&fit_width, &fit_height, 1, usedialog);
		Dialog_Size(&dialog_width, &dialog_height);
		flag.Y = (fit_height - dialog_height) / 2 + y;
	}
	int name_x = item_x;
	Fit_Size(&fit_width, &fit_height, 1, usedialog);
	Font_Cell_Size(&cell_width, &cell_height);
	int text_offset = (fit_height - cell_height) / 2;
	if (usedialog) {
		Surface *any = FUN_004E3560(0);
		int flag_width = any != NULL ? any->Get_Width() : 0;
		name_x = flag_width + item_x + 0xA;
	}
	int name_y = text_offset + y;

	Draw_Bar(bar, ratio, HiddenSurface, scheme);

	if (usedialog) {
		int country = Session.Players[index]->Get_Country();
		if ((Session.Type == GAME_IPX || Session.Type == GAME_INTERNET) && *(int *)((char *)Session.Players[index] + 0x6B) == -1) {
			country = -3;
		}
		Surface *image = FUN_004E3560(country);
		if (image != NULL) {
			Surface *any = FUN_004E3560(0);
			int width = any != NULL ? any->Get_Width() : 0;
			int height = any != NULL ? any->Get_Height() : 0;
			Rect rect(flag.X, flag.Y, width, height);
			int magenta = DSurface::Build_Hicolor_Pixel(0xFF, 0, 0xFF);
			if (HiddenSurface != NULL && !((DSurface *)HiddenSurface)->Is_Lost()) {
				SurfaceCache.DrawTrans(rect, *HiddenSurface, *image, (short)magenta);
			}
		}
	}
	Draw_Label(Point2D(name_x, name_y), name, scheme, pos->X - name_x + name_width - 3);
}
