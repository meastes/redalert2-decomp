// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y12): Show_Who_Was_Responsible, gamemd.exe 0x004C3E30 (2,924 bytes;
// the tree declares it in egos.h and calls it as FUN_004C3E30 from the main menu's Credits, and Do_Win
// calls it after a campaign's last mission: a link-closure trap at both). Written from behaviour: Red
// Alert 2's draft (src/recovered/classes/EgoClass/Show_Who_Was_Responsible.cpp, game.exe 0x004B37C0) in
// the YR tree's names, with gamemd.exe's changes: CREDITSMD.TXT, the tree's EgoClass (Scroll by two
// pixels, Render onto the credits surface DAT_008A0F58), OD_Blend_Color for the fades. Behaviour code for
// the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
#include "stringtable.h"
namespace Game { bool __cdecl CallBack(void); }
class InputManagerClass {
	public:
		void DoSomething(void);
		static InputManagerClass * Instance;
};

static wchar_t Credits_Line[512];      // gamemd.exe 0x008A0F5C: the line with its labels expanded
static int const Credits_Scroll_Ticks = 2;    // gamemd.exe 0x00820770

// "{Label}" in a credits line is the string table's text for Label (spaces and tabs left out).
static wchar_t * Credits_Expand_Labels(char const * text)
{
	wchar_t * out = Credits_Line;
	wchar_t * const end = Credits_Line + (sizeof(Credits_Line) / sizeof(Credits_Line[0])) - 1;
	*out = 0;
	while (*text && out < end) {
		if (*text == '{') {
			++text;
			char const * start = text;
			unsigned length = 0;
			while (*text && *text != '}') { ++length; ++text; }
			wchar_t const * replacement;
			text = start;
			if (length == 0) {
				replacement = L"{Missing Label}";
			} else if (length > 31) {
				replacement = L"{Bad Label}";
			} else {
				char label[32];
				char * lptr = label;
				while (*text && *text != '}') {
					if (*text != ' ' && *text != '\t') *lptr++ = *text;
					++text;
				}
				*lptr = 0;
				replacement = StringTable::LoadString(label, NULL, "D:\\ra2mdpost\\Egos.CPP", 0x11C);
			}
			while (*text && *text != '}') ++text;
			if (*text == '}') ++text;
			if (replacement != NULL) {
				size_t n = wcslen(replacement);
				if (n > (size_t)(end - out)) n = end - out;
				memcpy(out, replacement, n * sizeof(wchar_t));
				out += n;
			}
		} else {
			*out++ = (unsigned char)*text++;
		}
	}
	*out = 0;
	return(Credits_Line);
}

void Show_Who_Was_Responsible(void)
{
	LogicalSurface = AlternateSurface;
	CCFileClass file("CREDITSMD.TXT");
	if (!file.Is_Available()) {
		return;
	}
	int size = file.Size();
	char * credits = new char[size + 1];
	file.Read(credits, size);
	credits[size] = 0;
	GadgetClass::CurrentColorScheme = "Green";
	Point2D ret;
	Rect bounds = LogicalSurface->Get_Rect();
	Point2D origin(0, 0);
	Fancy_Text_Print_Wide(&ret, L" ", LogicalSurface, &bounds, &origin, Fetch_Scheme_By_Name("LightGold", 1), 0, (TextPrintType)0x148);

	// Each run of text becomes an EgoClass line below the screen: the column picks the alignment
	// (1-3 left, 4-7 centred, 8 on right), a centred line after a gap is a heading.
	int length = size;
	int line = 0;
	int column = 0;
	int y = HiddenSurface->Get_Height() + 2;
	int lastline = -2;
	char * cursor = credits;
	while (length > 0) {
		char ch = *cursor++;
		--length;
		switch (ch) {
			case '\r': ++line; y += 16; column = 0; break;
			case '\n': break;
			case ' ': ++column; break;
			case '\t': column = (column + 8) & ~7; break;
			default: {
				char * start = cursor - 1;
				char * parse = start;
				char * const stop = cursor + length;
				char last = ch;
				bool done = false;
				do {
					ch = *parse++;
					if (ch == '\t' || ch == '\n' || ch == '\r') done = true;
					else if (ch == ' ' && last == ' ') done = true;
					if (parse >= stop) done = true;
					last = ch;
				} while (!done);
				if (parse >= stop) {
					length = 0;
					break;
				}
				if (parse[-2] == ' ') --parse;
				unsigned flags = 0x48;
				if (column > 3 && column < 8) flags = 0x148;
				else if (column > 8) flags = 0x248;
				if (column > 3 && column < 8 && line - lastline > 1) flags |= 0x80;
				else flags |= 0x20;
				lastline = line;
				char saved = parse[-1];
				parse[-1] = 0;
				EgoList.Add(new EgoClass(0, y, Credits_Expand_Labels(start), (TextPrintType)flags));
				parse[-1] = saved;
				column += parse - start;
				length -= parse - start - 1;
				cursor = parse;
				if (ch == '\r') { ++line; y += 16; column = 0; }
				else if (ch == '\t') column = (column + 7) & ~7;
				break;
			}
		}
	}

	fprintf(stderr, "credits: %d lines\n", EgoList.Count());   // NATIVE: the run log's note
	Theme.Stop();
	HiddenSurface->Fill(0);
	Update_Visible_Surface(true, HiddenSurface);
	DAT_008A0F58 = new DSurface(Drawing::RenderWidth, Drawing::RenderHeight, false, false);
	DAT_008A0F58->Blit_From(*HiddenSurface, false, true);
	InputManagerClass::Instance->DoSomething();
	float oldvolume = Options.ScoreVolume;
	LogicalSurface = HiddenSurface;
	if (oldvolume == 0.0f) {
		Options.Set_Score_Volume(0.4f, false);
	}
	Theme.Queue_Song(Theme.From_Name("CREDITS"));

	int start_time = TickCount;
	int frame = 0;
	int picture_frame = 0;
	int slide = 0;
	bool fresh = false;
	MouseCursor->Hide_Mouse();
	while (EgoList.Count() > 0) {
		++frame;
		if (SurfacesRestored) {
			SurfacesRestored = false;
			fresh = true;
			if (!WindowedMode) {
				VisibleSurface->Fill(0);
				DAT_008A0F58->Fill(0);
			}
			MouseCursor->Show_Mouse();
			MouseCursor->Hide_Mouse();
		}
		if (++picture_frame > 200 && slide < 16) {
			++slide;
			picture_frame = 0;
		}
		for (int i = EgoList.Count() - 1; i >= 0; --i) {
			if (EgoList[i]->Scroll(2)) {
				delete EgoList[i];
				EgoList.Delete_Index(i);
				break;
			}
		}
		Rect dest(0, 32, Drawing::RenderWidth, Drawing::RenderHeight - 66);
		Rect source(0, 34, Drawing::RenderWidth, Drawing::RenderHeight - 66);
		Rect top(0, 0, Drawing::RenderWidth, 34);
		Rect bottom(0, Drawing::RenderHeight - 34, Drawing::RenderWidth, 34);
		DAT_008A0F58->Fill_Rect(top, 0);
		DAT_008A0F58->Fill_Rect(bottom, 0);
		for (int r = EgoList.Count() - 1; r >= 0; --r) {
			EgoList[r]->Render(fresh);
		}
		// The top and bottom 32 rows fade to black.
		unsigned short * pixels = (unsigned short *)DAT_008A0F58->Lock();
		if (pixels != NULL) {
			int step = DAT_008A0F58->Stride() / 2;
			for (int row = 0; row < 32; ++row) {
				int alpha = 256 - row * 8;
				if (alpha > 255) alpha = 255;
				unsigned short * first = pixels + step * row;
				unsigned short * last = pixels + step * (Drawing::RenderHeight - row - 1);
				for (int x = 0; x < Drawing::RenderWidth; ++x) {
					first[x] = OD_Blend_Color(first[x], 0, (unsigned char)alpha);
					last[x] = OD_Blend_Color(last[x], 0, (unsigned char)alpha);
				}
			}
			DAT_008A0F58->Unlock();
		}
		if (Game::IsFocused) {
			if (fresh) {
				VisibleSurface->Blit_From(*DAT_008A0F58, false, true);
			} else {
				VisibleSurface->Blit_From(top, *DAT_008A0F58, top, false, true);
				VisibleSurface->Blit_From(dest, *VisibleSurface, source, false, true);
				VisibleSurface->Blit_From(bottom, *DAT_008A0F58, bottom, false, true);
			}
		}
		fresh = false;
		Game::CallBack();
		// NATIVE: the wait services the game's callback too (the port's events and sound).
		while ((int)TickCount - start_time < frame * Credits_Scroll_Ticks && !Keyboard->Check()) {
			Game::CallBack();
		}
		if (Keyboard->Check() && Keyboard->Get() == 27) {
			break;
		}
	}
	while (picture_frame <= 187) {
		if (picture_frame < 150 && picture_frame > 47) picture_frame = 150;
		++frame;
		++picture_frame;
		Game::CallBack();
		while ((int)TickCount - start_time < frame * Credits_Scroll_Ticks && !Keyboard->Check()) {
			Game::CallBack();
		}
	}
	HiddenSurface->Fill(0);
	Update_Visible_Surface(true, HiddenSurface);
	MouseCursor->Show_Mouse();
	GadgetClass::CurrentColorScheme = "Blue";
	Theme.Stop();
	Options.Set_Score_Volume(oldvolume, false);
	delete DAT_008A0F58;
	DAT_008A0F58 = NULL;
	delete [] credits;
	while (EgoList.Count() > 0) {
		delete EgoList[0];
		EgoList.Delete_Index(0);
	}
	EgoList.Clear();
}

