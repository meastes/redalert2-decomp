// MP_Score_Dialog_Proc_005AAE40 (0x005AAE40): the score dialog's procedure. After the common dialog
// handler: 0x497 plays the score theme and fills the title, game, time and one row per score (name, kills,
// lost, built and score in the player's colour); WM_PAINT frames the rows; button 0x6D1 sets the caller's
// done flag.

int __stdcall MP_Score_Dialog_Proc_005AAE40(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
    int result = Default_Dialog_Proc(window, message, wparam, lparam);
    if (result != 0) {
        return result;
    }
    int *done = (int *)GetWindowLongA(window, 8);
    switch (message) {
    case 0x0F: {
        RECT first;
        RECT last;
        HWND item = GetDlgItem(window, 0x6D2);
        Window_Rect_In_Main_Client(item, &first);
        int id = unknown_mp_score_controls_7e5518[unknown_session_a3d298.ScoreCount - 1][4];
        item = GetDlgItem(window, id);
        Window_Rect_In_Main_Client(item, &last);
        Rect area(first.left, first.top, last.right - first.left, last.bottom - first.top);
        Unknown_Rect_Shrink_006F1B30(&area, -4, -8);
        Unknown_005feb60(&area, AlternateSurface, 0, 0x9F);
        break;
    }
    case 0x111:
        if ((unsigned short)wparam == 0x6D1) {
            *done = 1;
            break;
        }
        break;
    case 0x497: {
        wchar_t text[32];
        Theme.Play_Song((ThemeType)Theme.From_Name("SCORE"));
        if (unknown_session_a3d298.Type == 4) {
            swprintf(text, StringTable::Load_String("TXT_GAME", 0, "D:\\Ra2\\MPScore.cpp", 284), unknown_b2a7fc);
        } else {
            swprintf(text, StringTable::Load_String("TXT_GAME", 0, "D:\\Ra2\\MPScore.cpp", 286), unknown_a3f58c);
        }
        wchar_t const *title;
        if (unknown_session_a3d298.Type == 5) {
            title = StringTable::Load_String("GUI:SkirmishScore", 0, "D:\\Ra2\\MPScore.cpp", 290);
        } else {
            title = StringTable::Load_String("GUI:MultiplayerScore", 0, "D:\\Ra2\\MPScore.cpp", 292);
        }
        HWND item = GetDlgItem(window, 0x694);
        SendMessageA(item, 0x4B2, 0, (LPARAM)title);
        item = GetDlgItem(window, 0x6D2);
        SendMessageA(item, 0x4B2, 0, (LPARAM)text);
        MP_Format_Time_005AAAE0(text, Scen->ElapsedTime.Time() / 60);
        item = GetDlgItem(window, 0x3EA);
        SendMessageA(item, 0x4B2, 0, (LPARAM)text);
        MP_Tally_Scores_005AABD0();
        Unknown_005AADD0();
        for (int i = 0; i < unknown_session_a3d298.ScoreCount; i++) {
            ColorScheme *scheme = ColorSchemes[unknown_sorted_scores_a715f0[i]->Scheme];
            RGBClass color = scheme->BaseColor.To_RGB();
            unsigned long rgb = color.Red | (color.Green << 8) | (color.Blue << 16);
            int id = unknown_mp_score_controls_7e5518[i][0];
            wchar_t const *name = unknown_sorted_scores_a715f0[i]->Name;
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x4B2, 0, (LPARAM)name);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            id = unknown_mp_score_controls_7e5518[i][1];
            MP_Set_Dialog_Item_Number_005AAB80(window, id, unknown_sorted_scores_a715f0[i]->Kills);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            id = unknown_mp_score_controls_7e5518[i][2];
            MP_Set_Dialog_Item_Number_005AAB80(window, id, unknown_sorted_scores_a715f0[i]->Lost);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            id = unknown_mp_score_controls_7e5518[i][3];
            MP_Set_Dialog_Item_Number_005AAB80(window, id, unknown_sorted_scores_a715f0[i]->Built);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
            id = unknown_mp_score_controls_7e5518[i][4];
            MP_Set_Dialog_Item_Number_005AAB80(window, id, unknown_sorted_scores_a715f0[i]->Score);
            item = GetDlgItem(window, id);
            SendMessageA(item, 0x498, 0, rgb);
        }
        break;
    }
    }
    return 0;
}
