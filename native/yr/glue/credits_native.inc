
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3): CreditClass::Graphic_Logic (YR 0x004A2370) is unmatched in
// the YR tree. Written from gamemd.exe's code with TibSun's text (TibSun archive
// code/credits.cpp; GPL-3.0-or-later, EA's GPL section 7 terms; THIRD_PARTY.md) as the shape:
// the tick sound when the money changes, the credits tab, and the amount in the tab's text
// colour (DAT_00B0FA1C); an observer (PlayerPtr == DAT_00AC1198) sees the game time instead.
// YR has no mission-timer reminders here (the tactical view draws the timers). Behaviour code
// for the native build only.
// ---------------------------------------------------------------------------------------------
#include "stringtable.h"
extern RGBClass DAT_00B0FA1C;          // the credits tab's text colour (txtlabel2.cpp)
extern HouseClass * DAT_00AC1198;      // the observer's house
static wchar_t CreditTimeText[64];     // gamemd.exe's 0x0089F860

void CreditClass::Graphic_Logic(bool forced)
{
	if (forced || IsToRedraw) {
		int xx = SidebarSurface->Get_Width() / 2;
		Point2D point(xx, 2);
		unsigned color = DSurface::Build_Hicolor_Pixel(DAT_00B0FA1C);
		Rect rect = SidebarSurface->Get_Rect();
		Point2D drawn;

		if (PlayerPtr == DAT_00AC1198) {
			Map.Draw_Credits_Tab();
			int time = Current;
			if (time / 3600 > 99) {
				time = 359999;
			}
			int hours = time / 3600;
			time -= hours * 3600;
			int mins = time / 60;
			int secs = time - mins * 60;
			swprintf(CreditTimeText, StringTable::LoadString("TXT_TIME_FORMAT_HOURS", NULL, "D:\\ra2mdpost\\Credits.CPP", 0x6B), hours, mins, secs);
			Fancy_Text_Print_Wide(&drawn, CreditTimeText, SidebarSurface, &rect, &point, color, 0, TextPrintType(0x4108));
		} else {
			if (IsAudible && Rule->CreditTicks.Count() >= 2) {
				VocClass::PlayGlobal(IsUp ? Rule->CreditTicks[0] : Rule->CreditTicks[1], 0.5f, 0x2000);
			}
			Map.Draw_Credits_Tab();
			Fancy_Text_Print_Wide(&drawn, L"%ld", SidebarSurface, &rect, &point, color, 0, TextPrintType(0x4108), Current);
		}
		IsToRedraw = false;
		IsAudible = false;
	}
}
