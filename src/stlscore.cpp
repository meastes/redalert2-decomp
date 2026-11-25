/*
**	Command & Conquer Red Alert 2
**	STLSCORE.CPP: the score screen of a single player mission.
**
**	The dialog lists the mission's par time for the difficulty played, the time the mission
**	took (with the under or over par title and message the scenario names), and the number
**	of enemy objects the player's houses destroyed.
*/

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"
#include "stlscore.h"
#include "audioctl.h"
#include "gscreen.h"
#include "house.h"
#include "misc.h"
#include "ownrdraw.h"
#include "rules.h"
#include "_rules.h"
#include "scenario.h"
#include "theme.h"
#include "voc.h"
#include "globals.h"
#include <stdio.h>
#include <windows.h>
#undef LoadString
#include "stringtable.h"

void __fastcall Load_Score_Art(int side); // RA2: 0x006F2710 (txtlabel2.cpp)
void __cdecl Free_Score_Art(void); // RA2: 0x006F2750 (txtlabel2.cpp)
void __fastcall Draw_Score_Screen(Surface * surface, Rect const & window); // RA2: 0x006F3BF0 (txtlabel2.cpp)
int __stdcall Score_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam);

#define STLSCORE_DIALOG 0x103 // RA2: the dialog resource of the score screen
#define STLSCORE_DONE 0x423 // RA2: the button that closes the screen
#define STLSCORE_PAR_TIME 0x6F1 // RA2: the label of the par time
#define STLSCORE_TIME 0x6F2 // RA2: the label of the time the mission took
#define STLSCORE_TITLE 0x6F3 // RA2: the label of the under or over par title
#define STLSCORE_MESSAGE 0x6F4 // RA2: the label of the under or over par message
#define STLSCORE_KILLS 0x6F6 // RA2: the label of the kills
#define STLSCORE_ANIMATION 0x72B // RA2: the animated control of the dialog
#define STLSCORE_SET_TEXT (WM_USER + 178) // RA2: 0x4B2, sets the text of an owner drawn control
#define STLSCORE_SHOWN (WM_USER + 151) // RA2: 0x497, the dialog has been shown
#define STLSCORE_START (WM_USER + 215) // RA2: 0x4D7, posted to the animated control to start it
#define STLSCORE_STARTED (WM_USER + 216) // RA2: 0x4D8, the animated control has started
#define STLSCORE_STEP (WM_USER + 236) // RA2: 0x4EC, the animated control has moved on

static AudioController ScoreAudio; // RA2: 0x00ABD6A0, the sound of the counting
static int ScoreAudioOn; // RA2: 0x00ABD6D4, set while the counting sound is held


STLScoreScreen::STLScoreScreen(void)
{
}


/// <summary>
/// Shows the score screen and waits until the player closes it.
/// </summary>
void STLScoreScreen::Present(void)
{
	Load_Score_Art(Scen->IsGDI == false);
	Rect rect(0, 0, VideoModeWidth, VideoModeHeight);
	Draw_Score_Screen(HiddenSurface, rect);
	Update_Visible_Surface();
	Dialog_Box(STLSCORE_DIALOG, Score_Dialog_Proc, false);
	Draw_Score_Screen(HiddenSurface, rect);
	Update_Visible_Surface();
	Free_Score_Art();
}


/// <summary>
/// Sets a label of the dialog to a time given in seconds, as hours, minutes and seconds.
/// </summary>
void Set_Time_Text(HWND dialog, int id, int seconds)
{
	wchar_t buffer[32];
	int minutes = seconds / 60;
	int hours = minutes / 60;
	swprintf(buffer, L"%02d:%02d:%02d", hours, minutes - hours * 60, seconds - minutes * 60);
	HWND control = GetDlgItem(dialog, id);
	SendMessageA(control, STLSCORE_SET_TEXT, 0, (LPARAM)buffer);
}


/// <summary>
/// Sets a label of the dialog to a number.
/// </summary>
void Set_Number_Text(HWND dialog, int id, int number)
{
	wchar_t buffer[32];
	swprintf(buffer, L"%d", number);
	HWND control = GetDlgItem(dialog, id);
	SendMessageA(control, STLSCORE_SET_TEXT, 0, (LPARAM)buffer);
}


/// <summary>
/// The message handler of the score screen's dialog.
/// </summary>
int __stdcall Score_Dialog_Proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	int result = OwnerDraw::Default_Dialog_Proc(window, message, wparam, lparam);
	if (result != 0) {
		return(result);
	}

	int * done = (int *)GetWindowLongA(window, DWL_USER);

	switch (message) {
		case STLSCORE_SHOWN: {
			ScoreAudio.Init();
			ScoreAudioOn = true;

			int par = 0;
			if (Scen->Difficulty == DIFF_EASY) {
				par = Scen->ParTimeEasy;
			} else if (Scen->Difficulty == DIFF_NORMAL) {
				par = Scen->ParTimeMedium;
			} else if (Scen->Difficulty == DIFF_HARD) {
				par = Scen->ParTimeDifficult;
			}
			Set_Time_Text(window, STLSCORE_PAR_TIME, par / 60);

			int elapsed = Scen->ElapsedTimer;
			Set_Time_Text(window, STLSCORE_TIME, elapsed / 60);

			char const * title;
			char const * text;
			if (elapsed / 60 <= par / 60) {
				title = Scen->UnderParTitle;
				text = Scen->UnderParMessage;
			} else {
				title = Scen->OverParTitle;
				text = Scen->OverParMessage;
			}
			wchar_t const * titletext = StringTable::LoadString(title, NULL, "D:\\Ra2\\stlscore.cpp", 137);
			wchar_t const * messagetext = StringTable::LoadString(text, NULL, "D:\\Ra2\\stlscore.cpp", 138);
			HWND control = GetDlgItem(window, STLSCORE_TITLE);
			SendMessageA(control, STLSCORE_SET_TEXT, 0, (LPARAM)titletext);
			control = GetDlgItem(window, STLSCORE_MESSAGE);
			SendMessageA(control, STLSCORE_SET_TEXT, 0, (LPARAM)messagetext);

			int kills = 0;
			int count = Houses.Count();
			for (int index = 0; index < count; index++) {
				HouseClass * house = Houses[index];
				if (house->IsPlayerControl) {
					kills += house->DestroyedAircraft.Get_Unit_Total();
					kills += house->DestroyedInfantry.Get_Unit_Total();
					kills += house->DestroyedUnits.Get_Unit_Total();
					kills += house->DestroyedBuildings.Get_Unit_Total();
				}
			}
			control = GetDlgItem(window, STLSCORE_ANIMATION);
			PostMessageA(control, STLSCORE_START, 0, (LPARAM)window);
			Set_Number_Text(window, STLSCORE_KILLS, kills);

			Theme.Play_Song(Theme.From_Name("SCORE"));
			return(0);
		}

		case WM_DESTROY:
			if (ScoreAudioOn) {
				ScoreAudio.End();
				ScoreAudio.Release();
				ScoreAudioOn = false;
				return(0);
			}
			break;

		case WM_COMMAND:
			if (LOWORD(wparam) == STLSCORE_DONE) {
				*done = true;
				ScoreAudio.End();
				return(0);
			}
			break;

		case STLSCORE_STARTED:
			if (wparam == 0) {
				SendDlgItemMessageA(window, STLSCORE_ANIMATION, 0x4D5, 0, 0x1D);
			}
			break;

		case STLSCORE_STEP:
			ScoreAudio.Stop();
			ScoreAudio.SetEvent((AudioEvent *)VocClass::PlayGlobal(Rule->ScoreAnimSound, 1.0f, 0x2000), NULL);
			SendDlgItemMessageA(window, STLSCORE_ANIMATION, 0x4D3, 0, 0);
			break;
	}
	return(0);
}
