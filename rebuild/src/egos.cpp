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

/* $Header: /counterstrike/EGOS.CPP 2     3/10/97 3:19p Steve_tall $ */
/*************************************************************************************
 **   C O N F I D E N T I A L --- W E S T W O O D    S T U D I O S                  **
 *************************************************************************************
 *                                                                                   *
 *                 Project Name : Command & Conquer - Red Alert                      *
 *                                                                                   *
 *                    File Name : EGOS.CPP                                           *
 *                                                                                   *
 *                   Programmer : Steve Tall                                         *
 *                                                                                   *
 *                   Start Date : September 4th, 1996                                *
 *                                                                                   *
 *                  Last Update : September 4th, 1996 [ST]                           *
 *                                                                                   *
 *-----------------------------------------------------------------------------------*
 * Overview:                                                                         *
 *                                                                                   *
 *   Scrolling movie style credits.                                                  *
 *                                                                                   *
 *-----------------------------------------------------------------------------------*
 * Functions:                                                                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#include "always.h"
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table

#include "egos.h"
#include "bitfont.h" // RA2: Wipe measures with BitFont

#include "_keyboar.h"
#include "_palette.h"
#include "_rect.h"
#include "_surface.h"
#include "_timer.h"
#include "addon.h"
#include "ccfile.h"
#include "conquer.h"
#include "dialog.h"
#include "dsurface.h"
#include "font.h"
#include "gadget.h"
#include "globals.h"
#include "goptions.h"
#include "gscreen.h"
#include "language\language.h"
#include "misc.h"
#include "ownrdraw.h"
#include "scheme.h"
#include "stringtable.h" // RA2: StringTable::LoadString for {LABEL} in credits lines
#include "theme.h"
#include "vector.h"
#include "windlg.h"

#include "color.hh"
#include "dialog.hh"

#include <cstring>



/*
**	List of Ego Class instances
**	There will be one instance for each line of text.
*/
DynamicVectorClass<EgoClass *> EgoList;

/*
**	Number of slideshow pictures
*/

#define NUM_SLIDES 17

/*
**	Length of time frame is displayed for
*/
#define FRAME_DELAY	150

/*
**	Number of frames that palete fade occurs over
*/
#define FADE_DELAY		37

Surface * CreditsSurface; // RA2: the credits' surface (0x853118), set by Show_Credits (TibSun: CreditsSurface, the copy the text is undrawn from)

wchar_t CreditsText[5120]; // RA2: 0x85311C, the current credits line as wide text; its size is not shown, the next variable lies 10260 bytes on


/***********************************************************************************************
 * EC::EgoClass -- EgoClass constructor                                                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    x position of text                                                                *
 *           y position of text                                                                *
 *           ptr to text string                                                                *
 *           flags to print text with                                                          *
 *                                                                                             *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/9/96 11:53PM ST : Created                                                              *
 *=============================================================================================*/
EgoClass::EgoClass (int x, int y, unsigned short *text, TextPrintType flags) // RA2: wide text
{
	XPos = x;
	YPos = y;
	Flags= flags;
	Text = new unsigned short [wcslen ((wchar_t *)text)+1]; // RA2: wide
	wcscpy ((wchar_t *)Text, (wchar_t *)text); // RA2: wide
}


/***********************************************************************************************
 * EC::~EgoClass -- EgoClass destructor                                                        *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/9/96 11:54PM ST : Created                                                              *
 *=============================================================================================*/
EgoClass::~EgoClass(void)
{
	delete [] Text;
}


/***********************************************************************************************
 * EC::Scroll -- Apply the given distance to the y position of the text.                       *
 *               A positive distance scrolls up.                                               *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    distance in pixels to scroll up                                                   *
 *                                                                                             *
 * OUTPUT:   true if text scrolled beyond the top of the screen                                *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/9/96 11:55PM ST : Created                                                              *
 *=============================================================================================*/
bool EgoClass::Scroll(int distance)
{
	YPos -= distance;
	if (YPos < -20) {
		return(true);
	}else{
		return(false);
	}
}


/***********************************************************************************************
 * EC::Render -- Draws the text to the logic page                                              *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/9/96 11:57PM ST : Created                                                              *
 *=============================================================================================*/
void EgoClass::Render(bool fresh)
{
	if ((YPos < LogicalSurface->Get_Height() && YPos > LogicalSurface->Get_Height() - 52) || YPos >= -16 && YPos <= 32 || fresh) {
		Rect textrect; // RA2: Render at 0x4B3690 has no HFONT, HDC, GetDC and 2 more
		textrect.X = (VideoModeWidth - 520) / 2; // RA2: a 520-wide column centred on the screen
		textrect.Width = 520; // RA2: as above
		textrect.Y = YPos;
		textrect.Height = VideoModeHeight; // RA2: the screen height

		int alignment = 0;
		if (Flags & TPF_CENTER) {
			alignment = 1; // RA2: the wide text call's centre value
		}else{
			if (Flags & TPF_RIGHT){
				alignment = 4; // RA2: its right value
			}
		}

		if (GameInFocus) {
			OD_Draw_Text(0x80FFFF, NULL, textrect, (wchar_t *)Text, wcslen((wchar_t *)Text), alignment, 0, CreditsSurface, 0); // RA2: wide, BitFont, the credits surface
		}
	}
}


/***********************************************************************************************
 * EC::Wipe -- Wipes the previously rendered text by blitting a rectangle from the given       *
 *             background screen.                                                              *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    ptr to screen containing original background                                      *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/9/96 11:58PM ST : Created                                                              *
 *=============================================================================================*/
void EgoClass::Wipe (Surface *background)
{
	BitFont * font = Font_From_TPF(Flags); // RA2: Wipe at 0x4B3760 matches only so
	font->String_Pixel_Width(Text, 0); // RA2: as above

	Rect r(0, YPos, VideoModeWidth, 18);
	LogicalSurface->Blit_From(r, *background, r);
}


/*
**	RA2: converts one credits line to wide text in CreditsText; {LABEL} is looked up in the string table.
**	The executable has no copy of this function, it is expanded into Show_Credits.
*/
inline wchar_t * Credits_Wide_Text (const char *strstart) // RA2: lines are wide text and {LABEL}s are looked up in the string table
{
	const char *src = strstart; // RA2: as above
	wchar_t *dst = CreditsText; // RA2: as above
	*dst = 0; // RA2: as above
	while (*src) { // RA2: as above
		if (*src == '{') { // RA2: as above
			unsigned len = 0; // RA2: as above
			src++; // RA2: as above
			const char *label = src; // RA2: as above
			while (*src && *src != '}') { // RA2: as above
				len++; // RA2: as above
				src++; // RA2: as above
			} // RA2: as above
			const wchar_t *text; // RA2: as above
			if (len == 0) { // RA2: as above
				text = L"{Missing Label}"; // RA2: as above
			} else if (len > 31) { // RA2: as above
				text = L"{Bad Label}"; // RA2: as above
			} else { // RA2: as above
				char name[32]; // RA2: as above
				src = label; // RA2: as above
				char *np = name; // RA2: as above
				while (*src && *src != '}') { // RA2: as above
					if (*src == ' ' || *src == '\t') { // RA2: as above
						src++; // RA2: as above
						continue; // RA2: as above
					} // RA2: as above
					*np++ = *src++; // RA2: as above
				} // RA2: as above
				*np = 0; // RA2: as above
				if (*src == '}') src++; // RA2: as above
				text = StringTable::LoadString(name, NULL, "D:\\Ra2\\Egos.CPP", 284); // RA2: as above
			} // RA2: as above
			if (text) { // RA2: as above
				wcscpy(dst, text); // RA2: as above
				dst += wcslen(text); // RA2: as above
			} // RA2: as above
		} else { // RA2: as above
			*dst++ = *src & 0xFF; // RA2: as above
			src++; // RA2: as above
		} // RA2: as above
	} // RA2: as above
	*dst = 0; // RA2: as above
	return CreditsText; // RA2: as above
}


/***********************************************************************************************
 * Show_Credits -- Main function to print the credits.                             *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:    Nothing                                                                           *
 *                                                                                             *
 * OUTPUT:   Nothing                                                                           *
 *                                                                                             *
 * WARNINGS: None                                                                              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *    9/10/96 0:20AM ST : Created                                                              *
 *=============================================================================================*/
#ifdef RA2_NON_MATCHING // not exact: no independent bound yet (004b37c0 has no row)
void Show_Credits (void) // RA2: 0x4B37C0 (TibSun: Show_Who_Was_Responsible)
{

	int	i;

	/*
	**	Deault speed of credits scolling. This is the frame delay between pixel scrolls.
	*/
	static int speed = 2;

	LogicalSurface = AlternateSurface;

	/*
	**	Read in the credits file to be displayed
	**
	**	Lines of text in CREDITS.TXT are treated as follows....
	**
	**	If the text starts and ends to the left of column 40 then text will be right justified.
	**	If the text starts before column 40 and ends after it then it will be centered.
	**	If the text starts after column 40 it will be right justified.
	*/
	CCFileClass creditsfile ("CREDITS.TXT"); // RA2: file name
	if (creditsfile.Is_Available()) { // RA2: the exe jumps to the common cleanup instead of returning here
	char *credits = new char [creditsfile.Size()+1];
	creditsfile.Read (credits, creditsfile.Size());

	/*
	**	Initialise the text printing system.
	*/
	GadgetClass::Set_Color_Scheme("Green");
	Fancy_Text_Print(L" ", *LogicalSurface, LogicalSurface->Get_Rect(), Point2D(0, 0), Fetch_Scheme_By_Name("LightGold"), // RA2: as above
		TBLACK, TextPrintType(TPF_CENTER|TPF_METAL12|TPF_FULLSHADOW)); // RA2: wide text " " (TibSun: TXT_NONE)


	/*
	**	Miscellaneous stuff for parsing the credits text file.
	*/
	int 				length = creditsfile.Size();
	int 				line   = 0;
	int 				column = 0;
	char				*cptr = credits;
	char				ch, lastchar, oldchar;
	char				*strstart, *strparse;
	bool				gotendstr;
	int				startcolumn, endcolumn; // RA2: no x, the exe stores 0
	int				y=HiddenSurface->Get_Height()+2;
	int 				lastline = -2;
	EgoClass 		*ego;
	TextPrintType 	flags;


	/*
	**	Search through the text file and extract the strings, using each string to create
	**	a new EgoClass
	*/
	do {
		/*
		**	Search for text
		*/
		ch = *cptr++;
		length --;

		/*
		**	Look for a non whitespace character.
		*/
		switch ( ch ){

			case 13:
				/*
				**	Char was carriage return. Go on to the next line starting at column 0.
				*/
				line++;
				y += 16;
				column = 0;
				break;


			case 10:
				/*
				**	Ignore line feed. CR does both.
				*/
				break;

				/*
				**	Space character. Just advance the cursor and move on.
				*/
			case 32:
				column++;
				break;

				/*
				**	Tab char. Advance to the next tab column. Tabs are every 8 columns.
				*/
			case 9:
				column += 8;
				column &= 0xfffffff8;
				break;

			default:
				/*
				**	Found new string. Work out where it ends so we know how to treat it.
				*/
				lastchar = ch;
				strstart = cptr-1;
				strparse = cptr-1;
				endcolumn = startcolumn = column;
				gotendstr = false;

				do	{
					ch = *strparse++;
					switch ( ch ){
						case 9:
						case 10:
						case 13:
							gotendstr = true;
							break;

						case 32:
							if (lastchar == 32) gotendstr = true;
							//endcolumn++;
							break;

						default:
							//endcolumn++;
							break;
					}
					if (strparse >= cptr+length) gotendstr = true;

					lastchar = ch;
				}while (!gotendstr);


				if (strparse >= cptr+length) break;

				/*
				**	Strip off any trailing space.
				*/
				if (*(strparse-2) == 32){
					strparse--;
					//endcolumn -= 2;
				}


				/*
				**	If string straddles the center column then center it.
				**
				**	If string is on the left hand side then right justify it.
				**
				**	If string is on the right hand side then left justify it.
				*/
				flags = TextPrintType(TPF_METAL12 | TPF_FULLSHADOW );

				if (startcolumn > 3 && endcolumn < 8){
					flags = TextPrintType(flags | TPF_CENTER);
				}else{
					if (startcolumn > 8){ // RA2: the exe tests > 8 here (TibSun: < 3)
						flags = TextPrintType(flags | TPF_RIGHT);
					}
				}
	// RA2: no x is computed, the exe stores 0 (the column is placed by Render)

				if (startcolumn > 3 && endcolumn < 8 && line - lastline > 1) {
					flags = TextPrintType(flags | TPF_LIGHTSHADOW);
				} else {
					flags = TextPrintType(flags | TPF_DROPSHADOW);
				}
				lastline = line;

				/*
				**	Temporarily terminate the string.
				*/
				oldchar = *(strparse-1);
				*(strparse-1) = 0;

				/*
				**	Create the new class and add it to our list.
				*/
				ego = new EgoClass (0, y, (unsigned short *)Credits_Wide_Text(strstart), flags); // RA2: x is 0, the text is wide

				EgoList.Add (ego);

				/*
				**	Restore the character that was lost when we added the terminator.
				*/
				*(strparse-1) = oldchar;

				/*
				**	Fix up our outer loop parsing variables.
				*/
				cptr = strparse;
				column += strparse - strstart;
				length -= strparse - strstart-1;

				if (ch == 13) {
					line++;
					y += 16;
					column = 0;
				}else{
					if (ch == 9){
						column += 7;
						column &= 0xfffffff8;
					}
				}
				break;
		}

	} while ( length>0 );

	/*
	**	Stop the music.
	*/
	Theme.Stop();

	/*
	**	Clear the Seen Page since we will not be blitting to all of it.
	*/
	HiddenSurface->Fill(TBLACK);
	Update_Visible_Surface();

	/*
	**	Create a new graphic buffer to restore the background from. Initialise it to black so
	**	we can start scrolling before the first slideshow picture is blitted.
	*/
	CreditsSurface = new DSurface(VisibleRect.Width, VisibleRect.Height); // RA2: the credits surface (0x853118) takes the place of TibSun's BackgroundSurface
	CreditsSurface->Blit_From(*HiddenSurface); // RA2: as above

	/*
	**	Go away nasty keyboard.
	*/
	Keyboard->Clear();

	LogicalSurface = HiddenSurface;

	/*
	**	Start any old song.
	*/
	float oldvolume = Options.ScoreVolume;
	if ((double)oldvolume == 0) {
		Options.Set_Score_Volume(0.4f, false);
	}
	Theme.Queue_Song(Theme.From_Name("CREDITS")); // RA2: the credits theme (TibSun: MADRAP)

	/*
	**	Init misc timing variables.
	*/
	int time  = TickCount;
	int frame = 0;
	int picture_frame = 0;
	int slide_number = 0;
	bool fresh = false;

	Hide_Mouse();

	/*
	**	Main scrolling loop.
	**	Keeps going until all the EgoClass objects are deleted or esc is pressed.
	*/
	while ( EgoList.Count() ){

		frame++;

		if (SurfacesRestored) {
			SurfacesRestored = false;
			fresh = true;
			if (!WindowedMode) {
				VisibleSurface->Fill(TBLACK);
				CreditsSurface->Fill(TBLACK); // RA2: as above
			}
			Show_Mouse();
			Hide_Mouse();
		}

		/*
		**	Update the slideshow frame and switch to the next picture if its time.
		*/
		picture_frame++;

		if (picture_frame > FRAME_DELAY+50){
			if (slide_number <NUM_SLIDES-1){
				slide_number++;
				picture_frame = 0;
			}else{
				//slide_number = 0;
				//picture_frame = 0;
			}
		}

		/*
		**	Scroll the text. If any text goes off the top then delete that object.
		*/
		for (i=EgoList.Count()-1 ; i>=0 ; i--){
			//EgoList[i]->Wipe(CreditsSurface);
			if ( EgoList[i]->Scroll(2) ){
				delete EgoList[i];
				EgoList.Delete_Index(i);
				break;
			}
		}

		/*
		 * The scroll is performed by copying the middle of the visible surface two
		 * scan lines upward. The top and bottom bands lie outside that copy, so they
		 * are cleared and then refreshed from the background surface every frame.
		 */
		Rect scroll_dest(0, 32, VideoModeWidth, VideoModeHeight - 66);
		Rect scroll_source(0, 34, VideoModeWidth, VideoModeHeight - 66);
		Rect top_band(0, 0, VideoModeWidth, 34);
		Rect bottom_band(0, VideoModeHeight - 34, VideoModeWidth, 34);
		CreditsSurface->Fill_Rect(top_band, TBLACK); // RA2: as above
		CreditsSurface->Fill_Rect(bottom_band, TBLACK); // RA2: as above

		/*
		**	Render all the text strings in their new positions.
		*/
		for (i=EgoList.Count()-1 ; i>=0 ; i--){
			EgoList[i]->Render(fresh);
		}

		unsigned short *bsurf = (unsigned short *)CreditsSurface->Lock(); // RA2: as above
		if (bsurf) {
			int step = CreditsSurface->Stride() / 2; // RA2: step is local to the locked block
			int fade_x;
			int fade_y; // RA2: the fade runs over 32 rows
			int xidx;
			int yidx;
			for (fade_y = 0; fade_y < 32; fade_y++) { // RA2: the fade has 32 rows (TibSun: none)
				int alpha = min(256 - fade_y * 8, 255); // RA2: alpha falls 8 per row from 256, at most 255
				xidx = step * fade_y;
				yidx = step * (VideoModeHeight - fade_y - 1);
				for (fade_x = 0; fade_x < VideoModeWidth; fade_x++) {
					(bsurf + xidx)[fade_x] = OwnerDraw::Blend_Color((bsurf + xidx)[fade_x], 0, alpha); // RA2: as above
					(bsurf + yidx)[fade_x] = OwnerDraw::Blend_Color((bsurf + yidx)[fade_x], 0, alpha); // RA2: as above
				}
			}
			CreditsSurface->Unlock(); // RA2: as above
		}

		if (GameInFocus) {
			if (fresh) {
				VisibleSurface->Blit_From(*CreditsSurface); // RA2: as above
			} else {
				VisibleSurface->Blit_From(top_band, *CreditsSurface, top_band); // RA2: as above
				VisibleSurface->Blit_From(scroll_dest, *VisibleSurface, scroll_source);
				VisibleSurface->Blit_From(bottom_band, *CreditsSurface, bottom_band); // RA2: as above
			}
		}

		fresh = false;

		/*
		**	Stop calling Theme.AI after a while so a different song doesnt start playing
		*/
		Call_Back();

		/*
		**	Kill any spare time before blitting the hid page forward.
		*/
		while (TickCount - time < frame * speed && !Keyboard->Check()) {}

		/*
		**	If user hits escape then break.
		*/
		if (Keyboard->Check()){ // RA2: no key variable, no Theme.Fade_Out/palette fade after escape
			if (Keyboard->Get() == KN_ESC){ // RA2: the key is not kept, the exe compares the Get result
				break;
			}
		}

	}

	/*
	**	Wait for the picture to fade down
	*/
	while (picture_frame <= FADE_DELAY+FRAME_DELAY){ // RA2: runs after escape too
		if (picture_frame < FRAME_DELAY && picture_frame > 10+FADE_DELAY){ // RA2: as above
			picture_frame = FRAME_DELAY; // RA2: as above
		}
		frame++; // RA2: as above
		picture_frame++; // RA2: as above

		Call_Back(); // RA2: as above

		/*
		**	Kill any spare time
		*/
		while (TickCount - time < frame * speed && !Keyboard->Check()) {} // RA2: as above

	}

	/*
	**	Tidy up.
	*/
	HiddenSurface->Fill(TBLACK);
	Update_Visible_Surface();

	Show_Mouse();

	GadgetClass::Set_Color_Scheme("Blue");

	Theme.Stop();
	Options.Set_Score_Volume(oldvolume, false);

	delete CreditsSurface; // RA2: as above

	delete [] credits;

	while (EgoList.Count() > 0) {
		delete EgoList[0];
		EgoList.Delete_Index(0);
	}
	EgoList.Clear();
	} // RA2: as above
}
#endif

