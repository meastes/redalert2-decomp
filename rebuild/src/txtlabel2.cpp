/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the art of the interface: the palettes, drawers and shapes the shell, the loading screens, the score
**	screens, the dialog frame and the sidebar draw with. They are loaded from the mix files when a screen needs
**	them, kept in the globals below and freed again. TibSun has no such file; the code is the executable's from
**	0x006F1A00.
*/

#include "always.h" // RA2: the includes this file needs
#include "rect.h" // RA2: the header statics, in the order of the unit's initializer table
#include "geometry.h"
#include "convert.h"
#include "data.h"
#include "draw.h"
#include "dsurface.h"
#include "goptions.h"
#include "palette.h"
#include "rect.h"
#include "rgb.h"
#include "shapeset.h"
#include "_rect.h"
#include "_surface.h"
#include "globals.h"
#include "tab.h"
#include "init.h"

RGBClass YellowColor1(255, 255, 0); // RA2: 0x00AC1124
RGBClass DarkRedColor1(72, 0, 0); // RA2: 0x00AC109C
RGBClass TealColor(0, 82, 117); // RA2: 0x00AC1008
RGBClass DarkRedColor2(72, 0, 0); // RA2: 0x00AC1100
RGBClass TooltipColorSide0(164, 210, 255); // RA2: 0x00AC0FE8
RGBClass TooltipColorSide1(255, 255, 0); // RA2: 0x00AC10F4
RGBClass TooltipColor(0, 0, 0); // RA2: 0x00AC1028


char const * DialogPalFile = "DIALOG.PAL"; // RA2: 0x007F86E8
char const * MainbttnPalFile = "MAINBTTN.PAL"; // RA2: 0x007F86EC
char const * LdscrnPalFile = "LDSCRN.PAL"; // RA2: 0x007F86F0
char const * LdscrnaPalFile = "LDSCRNA.PAL"; // RA2: 0x007F86F4
char const * LdscrnsPalFile = "LDSCRNS.PAL"; // RA2: 0x007F86F8
char const * SpldbrPalFile = "SPLDBR.PAL"; // RA2: 0x007F86FC
char const * MplsPalFile = "MPLS.PAL"; // RA2: 0x007F8700
char const * MplsobsPalFile = "MPLSOBS.PAL"; // RA2: 0x007F8704
char const * AscorePalFile = "ASCORE.PAL"; // RA2: 0x007F8708
char const * SscorePalFile = "SSCORE.PAL"; // RA2: 0x007F870C
char const * MpascrnPalFile = "MPASCRN.PAL"; // RA2: 0x007F8710
char const * MpsscrnPalFile = "MPSSCRN.PAL"; // RA2: 0x007F8714
char const * FsscrnPalFile = "FSSCRN.PAL"; // RA2: 0x007F8718
char const * ShellPalFile = "SHELL.PAL"; // RA2: 0x007F871C
char const * Shell2PalFile = "SHELL2.PAL"; // RA2: 0x007F8720
char const * SdbtnanmPalFile = "SDBTNANM.PAL"; // RA2: 0x007F8724
char const * SidebarPalFile = "SIDEBAR.PAL"; // RA2: 0x007F8728
char const * UibkgdPalFile = "UIBKGD.PAL"; // RA2: 0x007F872C
char const * Ttlbr640ShpFile = "TTLBR640.SHP"; // RA2: 0x007F8730
char const * Ttlbr800ShpFile = "TTLBR800.SHP"; // RA2: 0x007F8734
char const * SpldbrsShpFile = "SPLDBRS.SHP"; // RA2: 0x007F8738
char const * SpldbrlShpFile = "SPLDBRL.SHP"; // RA2: 0x007F873C
char const * PudlgbgnShpFile = "PUDLGBGN.SHP"; // RA2: 0x007F8740
char const * PudlgbgaShpFile = "PUDLGBGA.SHP"; // RA2: 0x007F8744
char const * PudlgbgsShpFile = "PUDLGBGS.SHP"; // RA2: 0x007F8748
char const * MnbttnShpFile = "MNBTTN.SHP"; // RA2: 0x007F874C
char const * AscrbksmShpFile = "ASCRBKSM.SHP"; // RA2: 0x007F8750
char const * AscrbkmdShpFile = "ASCRBKMD.SHP"; // RA2: 0x007F8754
char const * AscrtsmShpFile = "ASCRTSM.SHP"; // RA2: 0x007F8758
char const * AscrtmdShpFile = "ASCRTMD.SHP"; // RA2: 0x007F875C
char const * AscrasmShpFile = "ASCRASM.SHP"; // RA2: 0x007F8760
char const * AscramdShpFile = "ASCRAMD.SHP"; // RA2: 0x007F8764
char const * SscrbksmShpFile = "SSCRBKSM.SHP"; // RA2: 0x007F8768
char const * SscrbkmdShpFile = "SSCRBKMD.SHP"; // RA2: 0x007F876C
char const * SscrtsmShpFile = "SSCRTSM.SHP"; // RA2: 0x007F8770
char const * SscrtmdShpFile = "SSCRTMD.SHP"; // RA2: 0x007F8774
char const * SscrasmShpFile = "SSCRASM.SHP"; // RA2: 0x007F8778
char const * SscramdShpFile = "SSCRAMD.SHP"; // RA2: 0x007F877C
char const * MpascrnsShpFile = "MPASCRNS.SHP"; // RA2: 0x007F8780
char const * MpascrnlShpFile = "MPASCRNL.SHP"; // RA2: 0x007F8784
char const * MpsscrnsShpFile = "MPSSCRNS.SHP"; // RA2: 0x007F8788
char const * MpsscrnlShpFile = "MPSSCRNL.SHP"; // RA2: 0x007F878C
char const * FsasmShpFile = "FSASM.SHP"; // RA2: 0x007F8790
char const * FsalgShpFile = "FSALG.SHP"; // RA2: 0x007F8794
char const * FsbcsmShpFile = "FSBCSM.SHP"; // RA2: 0x007F8798
char const * FsbclgShpFile = "FSBCLG.SHP"; // RA2: 0x007F879C
char const * FsssmShpFile = "FSSSM.SHP"; // RA2: 0x007F87A0
char const * FsslgShpFile = "FSSLG.SHP"; // RA2: 0x007F87A4
char const * FsbkgdsmShpFile = "FSBKGDSM.SHP"; // RA2: 0x007F87A8
char const * FsbkgdlgShpFile = "FSBKGDLG.SHP"; // RA2: 0x007F87AC
char const * SdbtnanmShpFile = "SDBTNANM.SHP"; // RA2: 0x007F87B0
char const * SdmpbtnShpFile = "SDMPBTN.SHP"; // RA2: 0x007F87B4
char const * SdwrntmpShpFile = "SDWRNTMP.SHP"; // RA2: 0x007F87B8
char const * MnscrnsShpFile = "MNSCRNS.SHP"; // RA2: 0x007F87BC
char const * MnscrnlShpFile = "MNSCRNL.SHP"; // RA2: 0x007F87C0
char const * SdtpShpFile = "SDTP.SHP"; // RA2: 0x007F87C4
char const * SdbtnbkgdShpFile = "SDBTNBKGD.SHP"; // RA2: 0x007F87C8
char const * SdbtmShpFile = "SDBTM.SHP"; // RA2: 0x007F87CC
char const * LwscrnsShpFile = "LWSCRNS.SHP"; // RA2: 0x007F87D0
char const * LwscrnlShpFile = "LWSCRNL.SHP"; // RA2: 0x007F87D4
char const * SidebttnShpFile = "SIDEBTTN.SHP"; // RA2: 0x007F87D8
char const * BkgdsmShpFile = "BKGDSM.SHP"; // RA2: 0x007F87DC
char const * BkgdmdShpFile = "BKGDMD.SHP"; // RA2: 0x007F87E0
char const * BkgdlgShpFile = "BKGDLG.SHP"; // RA2: 0x007F87E4
char const * CreditsShpFile = "CREDITS.SHP"; // RA2: 0x007F87E8
char const * TopShpFile = "TOP.SHP"; // RA2: 0x007F87EC
char const * RadarShpFile = "RADAR.SHP"; // RA2: 0x007F87F0
char const * Side1ShpFile = "SIDE1.SHP"; // RA2: 0x007F87F4
char const * Side2ShpFile = "SIDE2.SHP"; // RA2: 0x007F87F8
char const * Side2bShpFile = "SIDE2B.SHP"; // RA2: 0x007F87FC
char const * Side3ShpFile = "SIDE3.SHP"; // RA2: 0x007F8800
char const * AddonShpFile = "ADDON.SHP"; // RA2: 0x007F8804
char const * LspacerShpFile = "LSPACER.SHP"; // RA2: 0x007F8808
char const * LendcapShpFile = "LENDCAP.SHP"; // RA2: 0x007F880C
char const * BttnbkgdShpFile = "BTTNBKGD.SHP"; // RA2: 0x007F8810
char const * RendcapShpFile = "RENDCAP.SHP"; // RA2: 0x007F8814

void * ArtPopupBack; // RA2: 0x00AC1218; PUDLGBGN.SHP, the popup dialog's backdrop
void * ArtPopupBackAllied; // RA2: 0x00AC121C; PUDLGBGA.SHP
void * ArtPopupBackSoviet; // RA2: 0x00AC1220; PUDLGBGS.SHP
void * ArtMainButton; // RA2: 0x00AC10C0; MNBTTN.SHP
bool ArtPopupBackAllocated; // RA2: 0x00AC1224; whether the loader allocated the buffer (the same for the flags that follow)
bool ArtPopupBackAlliedAllocated; // RA2: 0x00AC1225
bool ArtPopupBackSovietAllocated; // RA2: 0x00AC1226
bool ArtMainButtonAllocated; // RA2: 0x00AC1227
PaletteClass * DialogPalette; // RA2: 0x00AC1130; DIALOG.PAL
ConvertClass * DialogDrawer; // RA2: 0x00AC1134
PaletteClass * MainButtonPalette; // RA2: 0x00AC1138; MAINBTTN.PAL
ConvertClass * MainButtonDrawer; // RA2: 0x00AC113C
PaletteClass * LoadScreenPalette; // RA2: 0x00AC1140; LDSCRN.PAL
ConvertClass * LoadScreenDrawer; // RA2: 0x00AC1144
PaletteClass * LoadScreenSidePalette; // RA2: 0x00AC1148; LDSCRNA.PAL or LDSCRNS.PAL, by the side
ConvertClass * LoadScreenSideDrawer; // RA2: 0x00AC114C
PaletteClass * SplashBarPalette; // RA2: 0x00AC1150; SPLDBR.PAL
ConvertClass * SplashBarDrawer; // RA2: 0x00AC1154
PaletteClass * MPLoadPalette; // RA2: 0x00AC1158; MPLS.PAL or MPLSOBS.PAL
ConvertClass * MPLoadDrawer; // RA2: 0x00AC115C
int MPLoadPaletteKind; // RA2: 0x00AC1248; 0 nothing loaded, 1 MPLS.PAL, 2 MPLSOBS.PAL
PaletteClass * ScorePalette; // RA2: 0x00AC1160; ASCORE.PAL or SSCORE.PAL
ConvertClass * ScoreDrawer; // RA2: 0x00AC1164
bool ScoreArtLoaded; // RA2: 0x00AC1168
ShapeSet * Art_Chat_Back; // RA2: 0x00AC110C; ASCRBKSM.SHP, ASCRBKMD.SHP, SSCRBKSM.SHP or SSCRBKMD.SHP
void * ArtScoreTop; // RA2: 0x00AC10F0; ASCRTSM.SHP and the like
void * Art_Score_Frame; // RA2: 0x00AC1108; ASCRASM.SHP and the like
bool ArtChatBackAllocated; // RA2: 0x00AC1214
bool ArtScoreTopAllocated; // RA2: 0x00AC1215
bool ArtScoreFrameAllocated; // RA2: 0x00AC1216
PaletteClass * MPScorePalette; // RA2: 0x00AC116C; MPASCRN.PAL or MPSSCRN.PAL
ConvertClass * MPScoreDrawer; // RA2: 0x00AC1170
bool MPScoreArtLoaded; // RA2: 0x00AC1174
ShapeSet * Art_Score_Screen; // RA2: 0x00AC1104; MPASCRNS.SHP, MPASCRNL.SHP, MPSSCRNS.SHP or MPSSCRNL.SHP
bool ArtScoreScreenAllocated; // RA2: 0x00AC1217
PaletteClass * SideAnimationPalette; // RA2: 0x00AC1178; FSSCRN.PAL
ConvertClass * SideAnimationDrawer; // RA2: 0x00AC117C
void * Art_Side_Animation; // RA2: 0x00AC1120; FSASM.SHP or FSALG.SHP
void * Art_Button_Corner; // RA2: 0x00AC1040; FSBCSM.SHP or FSBCLG.SHP
void * Art_Side_Strip; // RA2: 0x00AC10FC; FSSSM.SHP or FSSLG.SHP
void * Art_Button_Back; // RA2: 0x00AC1074; FSBKGDSM.SHP or FSBKGDLG.SHP
bool ArtSideAnimationAllocated; // RA2: 0x00AC1228
bool ArtButtonCornerAllocated; // RA2: 0x00AC1229
bool ArtSideStripAllocated; // RA2: 0x00AC122A
bool ArtButtonBackAllocated; // RA2: 0x00AC122B
bool InterfaceArtLoaded; // RA2: 0x00AC119C; the dialog frame's art and the palettes of the shell
PaletteClass * ShellPalette; // RA2: 0x00AC1184; SHELL.PAL
ConvertClass * ShellDrawer; // RA2: 0x00AC1188
PaletteClass * Shell2Palette; // RA2: 0x00AC118C; SHELL2.PAL
ConvertClass * Shell2Drawer; // RA2: 0x00AC1190
PaletteClass * ButtonAnimPalette; // RA2: 0x00AC1194; SDBTNANM.PAL
ConvertClass * ButtonAnimDrawer; // RA2: 0x00AC1198
ShapeSet * Slide_Button_Shape; // RA2: 0x00AC10B8; SDBTNANM.SHP
ShapeSet * ArtSlideMPButton; // RA2: 0x00AC0FEC; SDMPBTN.SHP
ShapeSet * ArtSlideWarning; // RA2: 0x00AC10B4; SDWRNTMP.SHP
ShapeSet * Art_Dialog_Back640; // RA2: 0x00AC1128; MNSCRNS.SHP
ShapeSet * Art_Dialog_Back; // RA2: 0x00AC1010; MNSCRNL.SHP
ShapeSet * ArtSlideTop; // RA2: 0x00AC10E8; SDTP.SHP
ShapeSet * ArtSlideButtonBack; // RA2: 0x00AC107C; SDBTNBKGD.SHP
ShapeSet * ArtSlideBottom; // RA2: 0x00AC1044; SDBTM.SHP
ShapeSet * ArtLowerScreen640; // RA2: 0x00AC10D8; LWSCRNS.SHP
ShapeSet * ArtLowerScreen; // RA2: 0x00AC1060; LWSCRNL.SHP
bool SlideButtonAllocated; // RA2: 0x00AC122C
bool ArtSlideMPButtonAllocated; // RA2: 0x00AC122D
bool ArtSlideWarningAllocated; // RA2: 0x00AC122E
bool ArtDialogBack640Allocated; // RA2: 0x00AC122F
bool ArtDialogBackAllocated; // RA2: 0x00AC1230
bool ArtSlideTopAllocated; // RA2: 0x00AC1231
bool ArtSlideButtonBackAllocated; // RA2: 0x00AC1232
bool ArtSlideBottomAllocated; // RA2: 0x00AC1233
bool ArtLowerScreen640Allocated; // RA2: 0x00AC1234
bool ArtLowerScreenAllocated; // RA2: 0x00AC1235
Rect * SlideButtonRect; // RA2: 0x00AC11B4
Rect * SlideMPButtonRect; // RA2: 0x00AC11B8
Rect * SlideWarningRect; // RA2: 0x00AC11BC
Rect * DialogBackRect; // RA2: 0x00AC11C0
Rect * SlideTopRect; // RA2: 0x00AC11C4
Rect * Button_Strip_Top; // RA2: 0x00AC11C8
Rect * Button_Strip_Bottom; // RA2: 0x00AC11CC
Rect * LowerScreenRect; // RA2: 0x00AC11D0
int SlideButtonCount; // RA2: 0x00AC102C
PaletteClass * SidebarArtPalette; // RA2: 0x00AC11A0; SIDEBAR.PAL
ConvertClass * SidebarArtDrawer; // RA2: 0x00AC11A4
PaletteClass * UIBackgroundPalette; // RA2: 0x00AC11A8; UIBKGD.PAL
ConvertClass * UIBackgroundDrawer; // RA2: 0x00AC11AC
bool SidebarArtLoaded; // RA2: 0x00AC11B0
ShapeSet * Button_Strip_Shape; // RA2: 0x00AC0FF8; SIDEBTTN.SHP
ShapeSet * ArtSidebarBack640; // RA2: 0x00AC10C4; BKGDSM.SHP
ShapeSet * ArtSidebarBack800; // RA2: 0x00AC10BC; BKGDMD.SHP
ShapeSet * ArtSidebarBack1024; // RA2: 0x00AC105C; BKGDLG.SHP
ShapeSet const * TabBackShape; // RA2: 0x00AC10F8; CREDITS.SHP
ShapeSet const * RadarFrameShape; // RA2: 0x00AC0FF0; TOP.SHP
ShapeSet const * RadarAnimShape; // RA2: 0x00AC1070; RADAR.SHP
ShapeSet * ArtSide1; // RA2: 0x00AC1078; SIDE1.SHP
ShapeSet * ArtSide2; // RA2: 0x00AC10EC; SIDE2.SHP
ShapeSet * ArtSide2b; // RA2: 0x00AC100C; SIDE2B.SHP
ShapeSet * ArtSide3; // RA2: 0x00AC1094; SIDE3.SHP
ShapeSet * ArtAddon; // RA2: 0x00AC1058; ADDON.SHP
ShapeSet const * TabLeftShape; // RA2: 0x00AC1048; LSPACER.SHP
ShapeSet const * TabThumbShape; // RA2: 0x00AC1098; LENDCAP.SHP
ShapeSet const * TabTileShape; // RA2: 0x00AC10A4; BTTNBKGD.SHP
ShapeSet const * TabRightShape; // RA2: 0x00AC10B0; RENDCAP.SHP
bool ButtonStripShapeAllocated; // RA2: 0x00AC1236
bool ArtSidebarBack640Allocated; // RA2: 0x00AC1237
bool ArtSidebarBack800Allocated; // RA2: 0x00AC1238
bool ArtSidebarBack1024Allocated; // RA2: 0x00AC1239
bool TabBackShapeAllocated; // RA2: 0x00AC123A
bool RadarFrameShapeAllocated; // RA2: 0x00AC123B
bool RadarAnimShapeAllocated; // RA2: 0x00AC123C
bool ArtSide1Allocated; // RA2: 0x00AC123D
bool ArtSide2Allocated; // RA2: 0x00AC123E
bool ArtSide2bAllocated; // RA2: 0x00AC123F
bool ArtSide3Allocated; // RA2: 0x00AC1240
bool ArtAddonAllocated; // RA2: 0x00AC1241
bool TabLeftShapeAllocated; // RA2: 0x00AC1242
bool TabThumbShapeAllocated; // RA2: 0x00AC1243
bool TabTileShapeAllocated; // RA2: 0x00AC1244
bool TabRightShapeAllocated; // RA2: 0x00AC1245
Rect * SidebarBackRect; // RA2: 0x00AC11D4
Rect * TabBackRect; // RA2: 0x00AC11D8
Rect * RadarFrameRect; // RA2: 0x00AC11DC
Rect * RadarAnimRect; // RA2: 0x00AC11E0
Rect * RadarAreaRect; // RA2: 0x00AC11E4
Rect * Side1Rect; // RA2: 0x00AC11E8
Rect * Free_Button_Strip; // RA2: 0x00AC11EC
Rect * Default_Button_Y; // RA2: 0x00AC11F0
Rect * Default_Button_Strip; // RA2: 0x00AC11F4
Rect * SidebarButtonsRect; // RA2: 0x00AC11F8
Rect * TabBarRect; // RA2: 0x00AC11FC
Rect * TabLeftRect; // RA2: 0x00AC1200
Rect * ThumbOpenRect; // RA2: 0x00AC1204
Rect * ThumbClosedRect; // RA2: 0x00AC1208
Rect * ButtonSlotRect; // RA2: 0x00AC120C
Rect * TabEndRect; // RA2: 0x00AC1210
int SidebarStripCount; // RA2: 0x00AC10CC
int TabTileCount; // RA2: 0x00AC0FF4
int ThumbOpenX; // RA2: 0x00AC10C8
int ThumbClosedX; // RA2: 0x00AC10A0
int ThumbTravel; // RA2: 0x00AC1080

void __fastcall Load_Popup_Dialog_Art(void);
void __fastcall Free_Popup_Dialog_Art(void);
void __fastcall Load_Popup_Dialog_Shapes(void);
void __fastcall Load_Dialog_Palettes(void);
void __fastcall Free_Popup_Dialog_Shapes(void);
void __fastcall Free_Dialog_Palettes(void);
void __fastcall Free_MP_Load_Palette(void);
void __fastcall Load_Score_Shapes(int alternate);
void __fastcall Load_MP_Score_Shape(int alternate);
void __fastcall Load_Interface_Art(void);
void __fastcall Free_Interface_Art(void);
void __fastcall Free_Sidebar_Art(void);
void __fastcall Load_Interface_Shapes(void);
void __fastcall Load_Interface_Palettes(void);
void __fastcall Layout_Interface_Art(int width, int height);
void __fastcall Free_Interface_Shapes(void);
void __cdecl Free_Interface_Palettes(void);
void __fastcall Free_Interface_Rects(void);
void __fastcall Load_Sidebar_Shapes(void);
void __fastcall Load_Sidebar_Palettes(void);
void __fastcall Layout_Sidebar_Art(int width, int height);
void __fastcall Free_Sidebar_Shapes(void);
void __fastcall Free_Sidebar_Rects(void);

Point2D __fastcall Copy_Point(Rect const * source) // RA2: 0x006F1AE0; the rectangle's top-left point
{
	return Point2D(source->X, source->Y); // RA2: matches the rectangle accessor in the YR twin
}

/// <summary>
/// RA2: assigns one point to another.
/// </summary>
void __fastcall Assign_Point(Point2D * dest, Point2D const * source) // RA2: 0x006F1AF0
{
	*dest = *source;
}

/// <summary>
/// RA2: moves each edge of a rectangle in by an amount.
/// </summary>
void __fastcall Inflate_Rect(Rect & rect, int amount) // RA2: 0x006F1B00
{
	rect.X += amount;
	rect.Y += amount;
	rect.Width -= amount * 2;
	rect.Height -= amount * 2;
}

/// <summary>
/// RA2: moves the left and right edges of a rectangle in by one amount and the top and bottom edges by another.
/// </summary>
void __fastcall Inflate_Rect(Rect & rect, int dx, int dy) // RA2: 0x006F1B30
{
	rect.X += dx;
	rect.Y += dy;
	rect.Width -= dx * 2;
	rect.Height -= dy * 2;
}

/// <summary>
/// RA2: loads all the art: the popup dialogs' shapes and palettes and the dialog frame's art.
/// </summary>
void __fastcall Load_All_Art(void) // RA2: 0x006F1B60
{
	Load_Popup_Dialog_Art();
	Load_Interface_Art();
}

/// <summary>
/// RA2: frees all the art: the popup dialogs', the dialog frame's and the sidebar's.
/// </summary>
void __fastcall Free_All_Art(void) // RA2: 0x006F1D20
{
	Free_Popup_Dialog_Art();
	Free_Interface_Art();
	Free_Sidebar_Art();
}

/// <summary>
/// RA2: the area of the screen the tactical view and its title screens use: the screen without the sidebar.
/// </summary>
Rect __fastcall Get_Title_Screen_Area(void) // RA2: 0x006F1F30
{
	Rect rect = VisibleRect;
	rect.X = 0;
	rect.Y = 0;
	if (!Options.IsSidebarOnRight && !Debug_Map) {
		rect.X = 168; // RA2: the sidebar is on the left, so the area starts right of it
	}
	rect.Width -= 168;
	rect.Height -= 32; // RA2: the tab bar takes the bottom
	return rect;
}

/// <summary>
/// RA2: reads a palette file into a new palette and builds the drawer that goes with it.
/// </summary>
void __fastcall Load_Art_Palette(char const * name, PaletteClass * * palette, ConvertClass * * drawer) // RA2: 0x006F1FA0
{
	bool allocated = false;
	unsigned char const * data = (unsigned char const *)Load_Alloc_Data(name, allocated);
	*palette = new PaletteClass;
	for (int index = 0; index < 256; index++) {
		int offset = index * 3;
		RGBClass color(data[offset] << 2, data[offset + 1] << 2, data[offset + 2] << 2); // RA2: the file holds 6 bit colours
		(**palette)[index] = color;
	}
	if (allocated) {
		delete (void *)data;
	}
	*drawer = new ConvertClass(**palette, **palette, *AlternateSurface, 1, false);
}

/// <summary>
/// RA2: loads the popup dialogs' shapes and the dialog palettes.
/// </summary>
void __fastcall Load_Popup_Dialog_Art(void) // RA2: 0x006F20B0
{
	Load_Popup_Dialog_Shapes();
	Load_Dialog_Palettes();
}

/// <summary>
/// RA2: frees the popup dialogs' shapes and the dialog palettes.
/// </summary>
void __fastcall Free_Popup_Dialog_Art(void) // RA2: 0x006F2130
{
	Free_Popup_Dialog_Shapes();
	Free_Dialog_Palettes();
}

PaletteClass * __cdecl Get_Dialog_Palette(void) // RA2: 0x006F2220
{
	return DialogPalette;
}

ConvertClass * __cdecl Get_Dialog_Drawer(void) // RA2: 0x006F2230
{
	return DialogDrawer;
}

PaletteClass * __cdecl Get_Main_Button_Palette(void) // RA2: 0x006F2240
{
	return MainButtonPalette;
}

ConvertClass * __cdecl Get_Main_Button_Drawer(void) // RA2: 0x006F2250
{
	return MainButtonDrawer;
}

/// <summary>
/// RA2: loads the shapes of the popup dialogs and the main button.
/// </summary>
void __fastcall Load_Popup_Dialog_Shapes(void) // RA2: 0x006F2260
{
	ArtPopupBack = Load_Alloc_Data(PudlgbgnShpFile, ArtPopupBackAllocated);
	ArtPopupBackAllied = Load_Alloc_Data(PudlgbgaShpFile, ArtPopupBackAlliedAllocated);
	ArtPopupBackSoviet = Load_Alloc_Data(PudlgbgsShpFile, ArtPopupBackSovietAllocated);
	ArtMainButton = Load_Alloc_Data(MnbttnShpFile, ArtMainButtonAllocated);
}

/// <summary>
/// RA2: loads the dialog palette and the palette of the main buttons, with their drawers.
/// </summary>
void __fastcall Load_Dialog_Palettes(void) // RA2: 0x006F22C0
{
	Load_Art_Palette(DialogPalFile, &DialogPalette, &DialogDrawer);
	Load_Art_Palette(MainbttnPalFile, &MainButtonPalette, &MainButtonDrawer);
}

/// <summary>
/// RA2: frees the shapes of the popup dialogs and the main button.
/// </summary>
void __fastcall Free_Popup_Dialog_Shapes(void) // RA2: 0x006F22F0
{
	if (ArtPopupBackAllocated) {
		operator delete(ArtPopupBack);
		ArtPopupBackAllocated = false;
	}
	ArtPopupBack = NULL;
	if (ArtPopupBackAlliedAllocated) {
		operator delete(ArtPopupBackAllied);
		ArtPopupBackAlliedAllocated = false;
	}
	ArtPopupBackAllied = NULL;
	if (ArtPopupBackSovietAllocated) {
		operator delete(ArtPopupBackSoviet);
		ArtPopupBackSovietAllocated = false;
	}
	ArtPopupBackSoviet = NULL;
	if (ArtMainButtonAllocated) {
		operator delete(ArtMainButton);
		ArtMainButtonAllocated = false;
	}
	ArtMainButton = NULL;
}

/// <summary>
/// RA2: frees the dialog palette and the palette of the main buttons, with their drawers.
/// </summary>
void __fastcall Free_Dialog_Palettes(void) // RA2: 0x006F2390
{
	if (DialogPalette != NULL) {
		operator delete(DialogPalette);
		DialogPalette = NULL;
	}
	if (DialogDrawer != NULL) {
		delete[] DialogDrawer;
		DialogDrawer = NULL;
	}
	if (MainButtonPalette != NULL) {
		operator delete(MainButtonPalette);
		MainButtonPalette = NULL;
	}
	if (MainButtonDrawer != NULL) {
		delete[] MainButtonDrawer;
		MainButtonDrawer = NULL;
	}
}

/// <summary>
/// RA2: the title bar's file name for a screen width.
/// </summary>
char const * __fastcall Get_Title_Bar_File(int width, int, int) // RA2: 0x006F2400
{
	char const * file = Ttlbr640ShpFile;
	if (width != 640) {
		file = Ttlbr800ShpFile;
	}
	return file;
}

/// <summary>
/// RA2: the splash bar's file name for a screen width.
/// </summary>
char const * __fastcall Get_Splash_Bar_File(int width, int, int) // RA2: 0x006F2420
{
	char const * file = SpldbrsShpFile;
	if (width != 640) {
		file = SpldbrlShpFile;
	}
	return file;
}

PaletteClass * __cdecl Get_Load_Screen_Palette(void) // RA2: 0x006F2440
{
	return LoadScreenPalette;
}

ConvertClass * __cdecl Get_Load_Screen_Drawer(void) // RA2: 0x006F2450
{
	return LoadScreenDrawer;
}

PaletteClass * __cdecl Get_Load_Screen_Side_Palette(void) // RA2: 0x006F2460
{
	return LoadScreenSidePalette;
}

ConvertClass * __cdecl Get_Load_Screen_Side_Drawer(void) // RA2: 0x006F2470
{
	return LoadScreenSideDrawer;
}

PaletteClass * __cdecl Get_Splash_Bar_Palette(void) // RA2: 0x006F2480
{
	return SplashBarPalette;
}

ConvertClass * __cdecl Get_Splash_Bar_Drawer(void) // RA2: 0x006F2490
{
	return SplashBarDrawer;
}

/// <summary>
/// RA2: loads the palettes of the loading screens, the ones still missing, for a side.
/// </summary>
void __fastcall Load_Loading_Screen_Palettes(int side) // RA2: 0x006F24A0
{
	if (LoadScreenDrawer == NULL) {
		Load_Art_Palette(LdscrnPalFile, &LoadScreenPalette, &LoadScreenDrawer);
	}
	if (LoadScreenSideDrawer == NULL) {
		if (side == 0) {
			Load_Art_Palette(LdscrnaPalFile, &LoadScreenSidePalette, &LoadScreenSideDrawer);
		} else {
			Load_Art_Palette(LdscrnsPalFile, &LoadScreenSidePalette, &LoadScreenSideDrawer);
		}
	}
	if (SplashBarDrawer == NULL) {
		Load_Art_Palette(SpldbrPalFile, &SplashBarPalette, &SplashBarDrawer);
	}
}

/// <summary>
/// RA2: frees the palettes of the loading screens.
/// </summary>
void __fastcall Free_Loading_Screen_Palettes(void) // RA2: 0x006F2510
{
	if (LoadScreenPalette != NULL) {
		operator delete(LoadScreenPalette);
		LoadScreenPalette = NULL;
	}
	if (LoadScreenDrawer != NULL) {
		delete LoadScreenDrawer;
		LoadScreenDrawer = NULL;
	}
	if (LoadScreenSidePalette != NULL) {
		operator delete(LoadScreenSidePalette);
		LoadScreenSidePalette = NULL;
	}
	if (LoadScreenSideDrawer != NULL) {
		delete LoadScreenSideDrawer;
		LoadScreenSideDrawer = NULL;
	}
	if (SplashBarPalette != NULL) {
		operator delete(SplashBarPalette);
		SplashBarPalette = NULL;
	}
	if (SplashBarDrawer != NULL) {
		delete SplashBarDrawer;
		SplashBarDrawer = NULL;
	}
}

PaletteClass * __cdecl Get_MP_Load_Palette(void) // RA2: 0x006F25A0
{
	return MPLoadPalette;
}

ConvertClass * __cdecl Get_MP_Load_Drawer(void) // RA2: 0x006F25B0
{
	return MPLoadDrawer;
}

/// <summary>
/// RA2: loads the palette of the multiplayer loading screen, the observer's one when observing; a palette of the
/// other kind is freed first.
/// </summary>
void __fastcall Load_MP_Load_Palette(void) // RA2: 0x006F25C0
{
	if (MPLoadPaletteKind == 1 && ObserverMode != 0) {
		Free_MP_Load_Palette();
	}
	if (MPLoadPaletteKind == 2 && ObserverMode == 0) {
		Free_MP_Load_Palette();
	}
	if (MPLoadDrawer == NULL) {
		MPLoadPaletteKind = 0;
	}
	if (MPLoadPaletteKind == 0) {
		if (ObserverMode != 0) {
			Load_Art_Palette(MplsobsPalFile, &MPLoadPalette, &MPLoadDrawer);
			MPLoadPaletteKind = 2;
		} else {
			Load_Art_Palette(MplsPalFile, &MPLoadPalette, &MPLoadDrawer);
			MPLoadPaletteKind = 1;
		}
	}
}

/// <summary>
/// RA2: frees the palette of the multiplayer loading screen.
/// </summary>
void __fastcall Free_MP_Load_Palette(void) // RA2: 0x006F26D0
{
	if (MPLoadPalette != NULL) {
		delete MPLoadPalette;
		MPLoadPalette = NULL;
	}
	if (MPLoadDrawer != NULL) {
		delete MPLoadDrawer;
		MPLoadDrawer = NULL;
	}
}

/// <summary>
/// RA2: loads the score screens' art, the shapes and the palette, for the Allied or the Soviet side.
/// </summary>
void __fastcall Load_Score_Art(int alternate) // RA2: 0x006F2710
{
	if (!ScoreArtLoaded) {
		Load_Score_Shapes(alternate);
		char const * name = AscorePalFile;
		if (alternate) {
			name = SscorePalFile;
		}
		Load_Art_Palette(name, &ScorePalette, &ScoreDrawer);
		ScoreArtLoaded = true;
	}
}

/// <summary>
/// RA2: frees the score screens' art.
/// </summary>
void __cdecl Free_Score_Art(void) // RA2: 0x006F2750
{
	if (ScoreArtLoaded) {
		ScoreArtLoaded = false;
		if (ArtChatBackAllocated) {
			operator delete(Art_Chat_Back);
			ArtChatBackAllocated = false;
		}
		Art_Chat_Back = NULL;
		if (ArtScoreTopAllocated) {
			operator delete(ArtScoreTop);
			ArtScoreTopAllocated = false;
		}
		ArtScoreTop = NULL;
		if (ArtScoreFrameAllocated) {
			operator delete(Art_Score_Frame);
			ArtScoreFrameAllocated = false;
		}
		Art_Score_Frame = NULL;
		if (ScorePalette != NULL) {
			operator delete(ScorePalette);
			ScorePalette = NULL;
		}
		if (ScoreDrawer != NULL) {
			delete ScoreDrawer;
			ScoreDrawer = NULL;
		}
	}
}

bool __cdecl Is_Score_Art_Loaded(void) // RA2: 0x006F2810
{
	return ScoreArtLoaded;
}

PaletteClass * __cdecl Get_Score_Palette(void) // RA2: 0x006F2820
{
	return ScorePalette;
}

void * __fastcall Get_Chat_Art(void) // RA2: 0x006F2830
{
	return ScoreDrawer;
}

/// <summary>
/// RA2: loads the score screens' shapes, in the size that fits the screen, for the Allied or the Soviet side.
/// </summary>
void __fastcall Load_Score_Shapes(int alternate) // RA2: 0x006F2840
{
	if (!alternate) {
		if (VideoModeWidth == 640) {
			Art_Chat_Back = (ShapeSet *)Load_Alloc_Data(AscrbksmShpFile, ArtChatBackAllocated);
			ArtScoreTop = Load_Alloc_Data(AscrtsmShpFile, ArtScoreTopAllocated);
			Art_Score_Frame = Load_Alloc_Data(AscrasmShpFile, ArtScoreFrameAllocated);
		} else {
			Art_Chat_Back = (ShapeSet *)Load_Alloc_Data(AscrbkmdShpFile, ArtChatBackAllocated);
			ArtScoreTop = Load_Alloc_Data(AscrtmdShpFile, ArtScoreTopAllocated);
			Art_Score_Frame = Load_Alloc_Data(AscramdShpFile, ArtScoreFrameAllocated);
		}
	} else {
		if (VideoModeWidth == 640) {
			Art_Chat_Back = (ShapeSet *)Load_Alloc_Data(SscrbksmShpFile, ArtChatBackAllocated);
			ArtScoreTop = Load_Alloc_Data(SscrtsmShpFile, ArtScoreTopAllocated);
			Art_Score_Frame = Load_Alloc_Data(SscrasmShpFile, ArtScoreFrameAllocated);
		} else {
			Art_Chat_Back = (ShapeSet *)Load_Alloc_Data(SscrbkmdShpFile, ArtChatBackAllocated);
			ArtScoreTop = Load_Alloc_Data(SscrtmdShpFile, ArtScoreTopAllocated);
			Art_Score_Frame = Load_Alloc_Data(SscramdShpFile, ArtScoreFrameAllocated);
		}
	}
}

/// <summary>
/// RA2: loads the score screens' palette for the Allied or the Soviet side.
/// </summary>
void __fastcall Load_Score_Palette(int alternate) // RA2: 0x006F2960
{
	Load_Art_Palette(alternate == 0 ? AscorePalFile : SscorePalFile, &ScorePalette, &ScoreDrawer);
}

/// <summary>
/// RA2: frees the score screens' shapes.
/// </summary>
void __fastcall Free_Score_Shapes(void) // RA2: 0x006F2980
{
	if (ArtChatBackAllocated) {
		operator delete(Art_Chat_Back);
		ArtChatBackAllocated = false;
	}
	Art_Chat_Back = NULL;
	if (ArtScoreTopAllocated) {
		operator delete(ArtScoreTop);
		ArtScoreTopAllocated = false;
	}
	ArtScoreTop = NULL;
	if (ArtScoreFrameAllocated) {
		operator delete(Art_Score_Frame);
		ArtScoreFrameAllocated = false;
	}
	Art_Score_Frame = NULL;
}

/// <summary>
/// RA2: frees the score screens' palette.
/// </summary>
void __fastcall Free_Score_Palette(void) // RA2: 0x006F29F0
{
	if (ScorePalette != NULL) {
		operator delete(ScorePalette);
		ScorePalette = NULL;
	}
	if (ScoreDrawer != NULL) {
		delete ScoreDrawer;
		ScoreDrawer = NULL;
	}
}

/// <summary>
/// RA2: loads the multiplayer score screen's art for a side.
/// </summary>
void __fastcall Load_MP_Score_Art(int side) // RA2: 0x006F2A30
{
	if (MPScoreArtLoaded) {
		return;
	}
	Load_MP_Score_Shape(side);
	Load_Art_Palette(side == 0 ? MpascrnPalFile : MpsscrnPalFile, &MPScorePalette, &MPScoreDrawer);
	MPScoreArtLoaded = true;
}

/// <summary>
/// RA2: frees the multiplayer score screen's art.
/// </summary>
void __fastcall Free_MP_Score_Art(void) // RA2: 0x006F2AB0
{
	if (MPScoreArtLoaded) {
		MPScoreArtLoaded = false;
		if (ArtScoreScreenAllocated) {
			operator delete(Art_Score_Screen);
			ArtScoreScreenAllocated = false;
		}
		Art_Score_Screen = NULL;
		if (MPScorePalette != NULL) {
			operator delete(MPScorePalette);
			MPScorePalette = NULL;
		}
		if (MPScoreDrawer != NULL) {
			delete MPScoreDrawer;
			MPScoreDrawer = NULL;
		}
	}
}

bool __cdecl Is_MP_Score_Art_Loaded(void) // RA2: 0x006F2B20
{
	return MPScoreArtLoaded;
}

PaletteClass * __cdecl Get_MP_Score_Palette(void) // RA2: 0x006F2B30
{
	return MPScorePalette;
}

void * __fastcall Get_Score_Drawer(void) // RA2: 0x006F2B40
{
	return MPScoreDrawer;
}

/// <summary>
/// RA2: loads the multiplayer score screen's shape, in the size that fits the screen, for a side.
/// </summary>
void __fastcall Load_MP_Score_Shape(int alternate) // RA2: 0x006F2B50
{
	if (alternate == 0) {
		if (VideoModeWidth == 640) {
			Art_Score_Screen = (ShapeSet *)Load_Alloc_Data(MpascrnsShpFile, ArtScoreScreenAllocated);
		} else {
			Art_Score_Screen = (ShapeSet *)Load_Alloc_Data(MpascrnlShpFile, ArtScoreScreenAllocated);
		}
	} else {
		if (VideoModeWidth == 640) {
			Art_Score_Screen = (ShapeSet *)Load_Alloc_Data(MpsscrnsShpFile, ArtScoreScreenAllocated);
		} else {
			Art_Score_Screen = (ShapeSet *)Load_Alloc_Data(MpsscrnlShpFile, ArtScoreScreenAllocated);
		}
	}
}

/// <summary>
/// RA2: loads the multiplayer score screen's palette for a side.
/// </summary>
void __fastcall Load_MP_Score_Palette(int alternate) // RA2: 0x006F2BB0
{
	Load_Art_Palette(alternate == 0 ? MpascrnPalFile : MpsscrnPalFile, &MPScorePalette, &MPScoreDrawer);
}

/// <summary>
/// RA2: frees the multiplayer score screen's shape.
/// </summary>
void __fastcall Free_MP_Score_Shape(void) // RA2: 0x006F2BD0
{
	if (ArtScoreScreenAllocated) {
		operator delete(Art_Score_Screen);
		ArtScoreScreenAllocated = false;
	}
	Art_Score_Screen = NULL;
}

/// <summary>
/// RA2: frees the multiplayer score screen's palette.
/// </summary>
void __fastcall Free_MP_Score_Palette(void) // RA2: 0x006F2C00
{
	if (MPScorePalette != NULL) {
		operator delete(MPScorePalette);
		MPScorePalette = NULL;
	}
	if (MPScoreDrawer != NULL) {
		delete MPScoreDrawer;
		MPScoreDrawer = NULL;
	}
}

/// <summary>
/// RA2: loads the animation the side buttons of the campaign dialog show.
/// </summary>
void __fastcall Load_Side_Animation_Art(void) // RA2: 0x006F2C40
{
	if (VideoModeWidth == 640) {
		Art_Side_Animation = Load_Alloc_Data(FsasmShpFile, ArtSideAnimationAllocated);
		Art_Button_Corner = Load_Alloc_Data(FsbcsmShpFile, ArtButtonCornerAllocated);
		Art_Side_Strip = Load_Alloc_Data(FsssmShpFile, ArtSideStripAllocated);
		Art_Button_Back = Load_Alloc_Data(FsbkgdsmShpFile, ArtButtonBackAllocated);
	} else {
		Art_Side_Animation = Load_Alloc_Data(FsalgShpFile, ArtSideAnimationAllocated);
		Art_Button_Corner = Load_Alloc_Data(FsbclgShpFile, ArtButtonCornerAllocated);
		Art_Side_Strip = Load_Alloc_Data(FsslgShpFile, ArtSideStripAllocated);
		Art_Button_Back = Load_Alloc_Data(FsbkgdlgShpFile, ArtButtonBackAllocated);
	}
	Load_Art_Palette(FsscrnPalFile, &SideAnimationPalette, &SideAnimationDrawer);
}

/// <summary>
/// RA2: frees the animation the side buttons of the campaign dialog show.
/// </summary>
void __fastcall Free_Side_Animation_Art(void) // RA2: 0x006F2D00
{
	if (ArtSideAnimationAllocated) {
		operator delete(Art_Side_Animation);
		ArtSideAnimationAllocated = false;
	}
	Art_Side_Animation = NULL;
	if (ArtButtonCornerAllocated) {
		operator delete(Art_Button_Corner);
		ArtButtonCornerAllocated = false;
	}
	Art_Button_Corner = NULL;
	if (ArtSideStripAllocated) {
		operator delete(Art_Side_Strip);
		ArtSideStripAllocated = false;
	}
	Art_Side_Strip = NULL;
	if (ArtButtonBackAllocated) {
		operator delete(Art_Button_Back);
		ArtButtonBackAllocated = false;
	}
	Art_Button_Back = NULL;
	if (SideAnimationPalette != NULL) {
		operator delete(SideAnimationPalette);
		SideAnimationPalette = NULL;
	}
	if (SideAnimationDrawer != NULL) {
		delete SideAnimationDrawer;
		SideAnimationDrawer = NULL;
	}
}

PaletteClass * __cdecl Get_Side_Animation_Palette(void) // RA2: 0x006F2DD0
{
	return SideAnimationPalette;
}

void * __fastcall Get_Slot_Art(void) // RA2: 0x006F2DE0
{
	return SideAnimationDrawer;
}

/// <summary>
/// RA2: loads the shapes of the side animation, in the size that fits the screen.
/// </summary>
void __fastcall Load_Side_Animation_Shapes(void) // RA2: 0x006F2DF0
{
	if (VideoModeWidth == 640) {
		Art_Side_Animation = Load_Alloc_Data(FsasmShpFile, ArtSideAnimationAllocated);
		Art_Button_Corner = Load_Alloc_Data(FsbcsmShpFile, ArtButtonCornerAllocated);
		Art_Side_Strip = Load_Alloc_Data(FsssmShpFile, ArtSideStripAllocated);
		Art_Button_Back = Load_Alloc_Data(FsbkgdsmShpFile, ArtButtonBackAllocated);
	} else {
		Art_Side_Animation = Load_Alloc_Data(FsalgShpFile, ArtSideAnimationAllocated);
		Art_Button_Corner = Load_Alloc_Data(FsbclgShpFile, ArtButtonCornerAllocated);
		Art_Side_Strip = Load_Alloc_Data(FsslgShpFile, ArtSideStripAllocated);
		Art_Button_Back = Load_Alloc_Data(FsbkgdlgShpFile, ArtButtonBackAllocated);
	}
}

/// <summary>
/// RA2: loads the palette of the side animation.
/// </summary>
void __cdecl Load_Side_Animation_Palette(void) // RA2: 0x006F2EB0
{
	Load_Art_Palette(FsscrnPalFile, &SideAnimationPalette, &SideAnimationDrawer);
}

/// <summary>
/// RA2: frees the shapes of the side animation.
/// </summary>
void __fastcall Free_Side_Animation_Shapes(void) // RA2: 0x006F2ED0
{
	if (ArtSideAnimationAllocated) {
		operator delete(Art_Side_Animation);
		ArtSideAnimationAllocated = false;
	}
	Art_Side_Animation = NULL;
	if (ArtButtonCornerAllocated) {
		operator delete(Art_Button_Corner);
		ArtButtonCornerAllocated = false;
	}
	Art_Button_Corner = NULL;
	if (ArtSideStripAllocated) {
		operator delete(Art_Side_Strip);
		ArtSideStripAllocated = false;
	}
	Art_Side_Strip = NULL;
	if (ArtButtonBackAllocated) {
		operator delete(Art_Button_Back);
		ArtButtonBackAllocated = false;
	}
	Art_Button_Back = NULL;
}

/// <summary>
/// RA2: frees the palette of the side animation.
/// </summary>
void __fastcall Free_Side_Animation_Palette(void) // RA2: 0x006F2F70
{
	if (SideAnimationPalette != NULL) {
		operator delete(SideAnimationPalette);
		SideAnimationPalette = NULL;
	}
	if (SideAnimationDrawer != NULL) {
		delete SideAnimationDrawer;
		SideAnimationDrawer = NULL;
	}
}

/// <summary>
/// RA2: loads the dialog frame's art, the palettes of the shell and lays the frame out for the screen, unless
/// that is done already.
/// </summary>
void __fastcall Load_Interface_Art(void) // RA2: 0x006F2FB0
{
	if (!InterfaceArtLoaded) {
		Load_Neutral_Mix();
		Load_Interface_Shapes();
		Load_Art_Palette(ShellPalFile, &ShellPalette, &ShellDrawer);
		Load_Art_Palette(Shell2PalFile, &Shell2Palette, &Shell2Drawer);
		Load_Art_Palette(SdbtnanmPalFile, &ButtonAnimPalette, &ButtonAnimDrawer);
		Layout_Interface_Art(VideoModeWidth, VideoModeHeight);
		InterfaceArtLoaded = true;
	}
}

/// <summary>
/// RA2: frees the dialog frame's art and the palettes of the shell, when they are loaded.
/// </summary>
void __fastcall Free_Interface_Art(void) // RA2: 0x006F30F0
{
	if (InterfaceArtLoaded) {
		InterfaceArtLoaded = false;
		Free_Interface_Shapes();
		Free_Interface_Palettes();
		Free_Interface_Rects();
		Release_Neutral_Mix();
	}
}

/// <summary>
/// RA2: frees the dialog frame's art and loads it again, as at the end of a game.
/// </summary>
void __fastcall Reload_Art_Set(void) // RA2: 0x006F31B0
{
	Free_Interface_Art();
	Load_Interface_Art();
}

/// <summary>
/// RA2: lays the dialog frame out again for a screen size.
/// </summary>
void __fastcall Relayout_Interface_Art(int width, int height) // RA2: 0x006F33B0
{
	Free_Interface_Rects();
	Layout_Interface_Art(width, height);
}

bool __fastcall Is_Interface_Art_Loaded(void) // RA2: 0x006F3460
{
	return InterfaceArtLoaded;
}

PaletteClass * __cdecl Get_Shell_Palette(void) // RA2: 0x006F3470
{
	return ShellPalette;
}

void * __fastcall Get_Drawer(void) // RA2: 0x006F3480
{
	return ShellDrawer;
}

PaletteClass * __cdecl Get_Shell2_Palette(void) // RA2: 0x006F3490
{
	return Shell2Palette;
}

void * __fastcall Get_Listed_Drawer(void) // RA2: 0x006F34A0
{
	return Shell2Drawer;
}

PaletteClass * __cdecl Get_Button_Anim_Palette(void) // RA2: 0x006F34B0
{
	return ButtonAnimPalette;
}

ConvertClass * __cdecl Get_Button_Anim_Drawer(void) // RA2: 0x006F34C0
{
	return ButtonAnimDrawer;
}

/// <summary>
/// RA2: paints black over the screen outside the dialog frame when the screen is larger than 800 by 600.
/// </summary>
void __fastcall Draw_Interface_Borders(Surface * surface, Rect const & window) // RA2: 0x006F34D0
{
	if (VideoModeWidth >= 1024) {
		int left = DialogBackRect->X;
		int right = SlideTopRect->X + SlideTopRect->Width;
		int top = DialogBackRect->Y;
		int bottom = LowerScreenRect->Height + LowerScreenRect->Y;
		Rect rect(0, 0, left, top);
		surface->Fill_Rect(rect, window, 0);
		rect = Rect(left, 0, 800, top);
		surface->Fill_Rect(rect, window, 0);
		rect = Rect(right, 0, left, top);
		surface->Fill_Rect(rect, window, 0);
		rect = Rect(right, top, left, 600);
		surface->Fill_Rect(rect, window, 0);
		rect = Rect(right, bottom, left, 600);
		surface->Fill_Rect(rect, window, 0);
		rect = Rect(left, bottom, 800, top);
		surface->Fill_Rect(rect, window, 0);
		rect = Rect(0, bottom, left, top);
		surface->Fill_Rect(rect, window, 0);
		rect = Rect(0, top, left, 600);
		surface->Fill_Rect(rect, window, 0);
	}
}

/// <summary>
/// RA2: draws the dialog frame's strips and buttons.
/// </summary>
void __fastcall Draw_Interface_Frame(Surface * surface, Rect const & window, bool arg3) // RA2: 0x006F3650
{
	Load_Interface_Art(); // RA2: loads the art once; the nested expansion there leaves the shapes to a call
	Draw_Interface_Borders(surface, window);
	Point2D point(0, 0);
	Rect clip(window.X, window.Y, window.Width, window.Height);
	if (clip.Width > 800) {
		clip.Width = (clip.Width - 800) / 2 + 800;
	}
	if (clip.Height > 600) {
		clip.Height = (clip.Height - 600) / 2 + 600;
	}
	point.X = SlideTopRect->X;
	point.Y = SlideTopRect->Y;
	Draw_Shape(*surface, *ShellDrawer, ArtSlideTop, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	int index = 0;
	point.X = Button_Strip_Top->X;
	point.Y = Button_Strip_Top->Y;
	if (SlideButtonCount > 0) {
		do {
			Draw_Shape(*surface, *Shell2Drawer, ArtSlideButtonBack, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
			point.Y += Button_Strip_Top->Height;
			index++;
		} while (index < SlideButtonCount);
	}
	if (!arg3) {
		index = 0;
		point.X = SlideButtonRect->X;
		point.Y = SlideButtonRect->Y;
		if (SlideButtonCount > 0) {
			do {
				Draw_Shape(*surface, *ButtonAnimDrawer, Slide_Button_Shape, 10, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
				point.Y += SlideButtonRect->Height;
				index++;
			} while (index < SlideButtonCount);
		}
	}
	point.X = Button_Strip_Bottom->X;
	point.Y = Button_Strip_Bottom->Y;
	Draw_Shape(*surface, *ShellDrawer, ArtSlideBottom, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	point.X = LowerScreenRect->X;
	point.Y = LowerScreenRect->Y;
	if (VideoModeWidth == 640) {
		Draw_Shape(*surface, *ShellDrawer, ArtLowerScreen640, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	} else {
		Draw_Shape(*surface, *ShellDrawer, ArtLowerScreen, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	}
}

/// <summary>
/// RA2: draws the dialog's backdrop.
/// </summary>
void __fastcall Draw_Dialog_Backdrop(Surface * surface, Rect const & window, ConvertClass * drawer, ShapeSet * shape640, ShapeSet * shape) // RA2: 0x006F3930
{
	Point2D point(0, 0);
	Rect clip(window.X, window.Y, window.Width, window.Height);
	if (clip.Width > 800) {
		clip.Width = (clip.Width - 800) / 2 + 800;
	}
	if (clip.Height > 600) {
		clip.Height = (clip.Height - 600) / 2 + 600;
	}
	point.X = DialogBackRect->X;
	point.Y = DialogBackRect->Y;
	if (VideoModeWidth == 640) {
		Draw_Shape(*surface, *drawer, shape640, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	} else {
		Draw_Shape(*surface, *drawer, shape, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	}
}

/// <summary>
/// RA2: draws the dialog frame and the dialog's backdrop.
/// </summary>
void __fastcall Draw_Interface_Screen(Surface * surface, Rect const & rect, bool arg3) // RA2: 0x006F3A20
{
	Draw_Interface_Frame(surface, rect, arg3);
	Draw_Dialog_Backdrop(surface, rect, (ConvertClass *)Get_Drawer(), Art_Dialog_Back640, Art_Dialog_Back);
}

/// <summary>
/// RA2: draws the shape of the multiplayer button of the slide, at its place.
/// </summary>
void __fastcall Draw_Slide_MP_Button(Surface * surface, Rect const & window) // RA2: 0x006F3A60
{
	Point2D point(0, 0);
	point.X = SlideMPButtonRect->X;
	point.Y = SlideMPButtonRect->Y;
	Draw_Shape(*surface, *ShellDrawer, ArtSlideMPButton, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
}

/// <summary>
/// RA2: draws the second frame of the slide's top shape, at its place.
/// </summary>
void __fastcall Draw_Slide_Top(Surface * surface, Rect const & window) // RA2: 0x006F3AC0
{
	Point2D point(0, 0);
	point.X = SlideTopRect->X;
	point.Y = SlideTopRect->Y;
	Draw_Shape(*surface, *ShellDrawer, ArtSlideTop, 1, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
}

/// <summary>
/// RA2: draws the top of the score screens, at the dialog's place, moved right on wide screens.
/// </summary>
void __fastcall Draw_Score_Top(Surface * surface, Rect const & window) // RA2: 0x006F3B20
{
	Point2D point(0, 0);
	Point2D * source = (Point2D *)DialogBackRect;
	point.X = source->X;
	point.Y = source->Y;
	if (VideoModeWidth >= 800) {
		point.X += 80;
	}
	Draw_Shape(*surface, *ScoreDrawer, (ShapeSet *)ArtScoreTop, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
}

/// <summary>
/// RA2: draws the score screens' backdrop, at the dialog's place.
/// </summary>
void __fastcall Draw_Score_Back(Surface * surface, Rect const & window) // RA2: 0x006F3B90
{
	Point2D point(0, 0);
	point.X = DialogBackRect->X;
	point.Y = DialogBackRect->Y;
	Draw_Shape(*surface, *ScoreDrawer, Art_Chat_Back, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
}

/// <summary>
/// RA2: draws the dialog frame and the score screens' backdrop.
/// </summary>
void __fastcall Draw_Score_Screen(Surface * surface, Rect const & window) // RA2: 0x006F3BF0
{
	Draw_Interface_Borders(surface, window);
	Draw_Interface_Frame(surface, window, false);
	Point2D point(0, 0);
	point.X = DialogBackRect->X;
	point.Y = DialogBackRect->Y;
	Draw_Shape(*surface, *ScoreDrawer, Art_Chat_Back, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
}

/// <summary>
/// RA2: draws the multiplayer score screen's backdrop, at the dialog's place.
/// </summary>
void __fastcall Draw_MP_Score_Back(Surface * surface, Rect const & window) // RA2: 0x006F3C70
{
	Point2D point(0, 0);
	point.X = DialogBackRect->X;
	point.Y = DialogBackRect->Y;
	Draw_Shape(*surface, *MPScoreDrawer, Art_Score_Screen, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
}

/// <summary>
/// RA2: draws the dialog frame and the multiplayer score screen's backdrop.
/// </summary>
void __fastcall Draw_MP_Score_Screen(Surface * surface, Rect const & window) // RA2: 0x006F3CD0
{
	Draw_Interface_Borders(surface, window);
	Draw_Interface_Frame(surface, window, false);
	Point2D point(0, 0);
	point.X = DialogBackRect->X;
	point.Y = DialogBackRect->Y;
	Draw_Shape(*surface, *MPScoreDrawer, Art_Score_Screen, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
}

/// <summary>
/// RA2: loads the shapes of the dialog frame.
/// </summary>
void __fastcall Load_Interface_Shapes(void) // RA2: 0x006F3D50
{
	Slide_Button_Shape = (ShapeSet *)Load_Alloc_Data(SdbtnanmShpFile, SlideButtonAllocated);
	ArtSlideMPButton = (ShapeSet *)Load_Alloc_Data(SdmpbtnShpFile, ArtSlideMPButtonAllocated);
	ArtSlideWarning = (ShapeSet *)Load_Alloc_Data(SdwrntmpShpFile, ArtSlideWarningAllocated);
	Art_Dialog_Back640 = (ShapeSet *)Load_Alloc_Data(MnscrnsShpFile, ArtDialogBack640Allocated);
	Art_Dialog_Back = (ShapeSet *)Load_Alloc_Data(MnscrnlShpFile, ArtDialogBackAllocated);
	ArtSlideTop = (ShapeSet *)Load_Alloc_Data(SdtpShpFile, ArtSlideTopAllocated);
	ArtSlideButtonBack = (ShapeSet *)Load_Alloc_Data(SdbtnbkgdShpFile, ArtSlideButtonBackAllocated);
	ArtSlideBottom = (ShapeSet *)Load_Alloc_Data(SdbtmShpFile, ArtSlideBottomAllocated);
	ArtLowerScreen640 = (ShapeSet *)Load_Alloc_Data(LwscrnsShpFile, ArtLowerScreen640Allocated);
	ArtLowerScreen = (ShapeSet *)Load_Alloc_Data(LwscrnlShpFile, ArtLowerScreenAllocated);
}

/// <summary>
/// RA2: loads the palettes of the shell.
/// </summary>
void __fastcall Load_Interface_Palettes(void) // RA2: 0x006F3E30
{
	Load_Art_Palette(ShellPalFile, &ShellPalette, &ShellDrawer);
	Load_Art_Palette(Shell2PalFile, &Shell2Palette, &Shell2Drawer);
	Load_Art_Palette(SdbtnanmPalFile, &ButtonAnimPalette, &ButtonAnimDrawer);
}

/// <summary>
/// RA2: works out where the parts of the dialog frame go on a screen of this size: the frame is 800 by 600 and
/// centred on larger screens.
/// </summary>
void __fastcall Layout_Interface_Art(int width, int height) // RA2: 0x006F3E70
{
	int left = 0;
	int right = width;
	if (width >= 1024) {
		left = (width - 800) / 2;
		right = width - left;
	}
	int top = 0;
	int bottom = height;
	int span = height;
	if (height >= 768) {
		top = (height - 600) / 2;
		bottom = height - top;
		span = height - top * 2;
	}
	bool small_width = width == 640;
	bool small_height = height == 480;

	int w = small_width ? Art_Dialog_Back640->Get_Width() : Art_Dialog_Back->Get_Width();
	int h = small_height ? Art_Dialog_Back640->Get_Height() : Art_Dialog_Back->Get_Height();
	DialogBackRect = new Rect(left, top, w, h);

	int tw = ArtSlideTop->Get_Width();
	int th = ArtSlideTop->Get_Height();
	int x = right - tw;
	SlideTopRect = new Rect(x, top, tw, th);

	int wh = ArtSlideWarning->Get_Height();
	SlideWarningRect = new Rect(x, top, tw, wh);

	int by = SlideTopRect->Height + top;
	int bh = ArtSlideButtonBack->Get_Height();
	Button_Strip_Top = new Rect(x, by, tw, bh);

	int aw = Slide_Button_Shape->Get_Width();
	int ah = Slide_Button_Shape->Get_Height();
	int ax = Button_Strip_Top->Width - aw + Button_Strip_Top->X;
	int ay = Button_Strip_Top->Y;
	SlideButtonRect = new Rect(ax, ay, aw, ah);

	int mw = ArtSlideMPButton->Get_Width();
	int mh = ArtSlideMPButton->Get_Height();
	int my = Button_Strip_Top->Height + Button_Strip_Top->Y - mh;
	SlideMPButtonRect = new Rect(right - mw, my, mw, mh);

	SlideButtonCount = (span - SlideTopRect->Height) / Button_Strip_Top->Height;
	int offset = Button_Strip_Top->Height * SlideButtonCount;
	int used = SlideTopRect->Height + offset;
	int ly = Button_Strip_Top->Y + offset;
	int lw = ArtSlideBottom->Get_Width();
	Button_Strip_Bottom = new Rect(right - lw, ly, lw, span - used);

	int sw = small_width ? ArtLowerScreen640->Get_Width() : ArtLowerScreen->Get_Width();
	int sh = small_height ? ArtLowerScreen640->Get_Height() : ArtLowerScreen->Get_Height();
	LowerScreenRect = new Rect(left, bottom - sh, sw, sh);
}

/// <summary>
/// RA2: frees the shapes of the dialog frame.
/// </summary>
void __fastcall Free_Interface_Shapes(void) // RA2: 0x006F4160
{
	if (SlideButtonAllocated) {
		operator delete(Slide_Button_Shape);
		SlideButtonAllocated = false;
	}
	Slide_Button_Shape = NULL;
	if (ArtSlideMPButtonAllocated) {
		operator delete(ArtSlideMPButton);
		ArtSlideMPButtonAllocated = false;
	}
	ArtSlideMPButton = NULL;
	if (ArtSlideWarningAllocated) {
		operator delete(ArtSlideWarning);
		ArtSlideWarningAllocated = false;
	}
	ArtSlideWarning = NULL;
	if (ArtDialogBack640Allocated) {
		operator delete(Art_Dialog_Back640);
		ArtDialogBack640Allocated = false;
	}
	Art_Dialog_Back640 = NULL;
	if (ArtDialogBackAllocated) {
		operator delete(Art_Dialog_Back);
		ArtDialogBackAllocated = false;
	}
	Art_Dialog_Back = NULL;
	if (ArtSlideTopAllocated) {
		operator delete(ArtSlideTop);
		ArtSlideTopAllocated = false;
	}
	ArtSlideTop = NULL;
	if (ArtSlideButtonBackAllocated) {
		operator delete(ArtSlideButtonBack);
		ArtSlideButtonBackAllocated = false;
	}
	ArtSlideButtonBack = NULL;
	if (ArtSlideBottomAllocated) {
		operator delete(ArtSlideBottom);
		ArtSlideBottomAllocated = false;
	}
	ArtSlideBottom = NULL;
	if (ArtLowerScreen640Allocated) {
		operator delete(ArtLowerScreen640);
		ArtLowerScreen640Allocated = false;
	}
	ArtLowerScreen640 = NULL;
	if (ArtLowerScreenAllocated) {
		operator delete(ArtLowerScreen);
		ArtLowerScreenAllocated = false;
	}
	ArtLowerScreen = NULL;
}

/// <summary>
/// RA2: frees the palettes of the shell.
/// </summary>
void __cdecl Free_Interface_Palettes(void) // RA2: 0x006F42D0
{
	if (ShellPalette != NULL) {
		operator delete(ShellPalette);
		ShellPalette = NULL;
	}
	if (ShellDrawer != NULL) {
		delete ShellDrawer;
		ShellDrawer = NULL;
	}
	if (Shell2Palette != NULL) {
		operator delete(Shell2Palette);
		Shell2Palette = NULL;
	}
	if (Shell2Drawer != NULL) {
		delete Shell2Drawer;
		Shell2Drawer = NULL;
	}
	if (ButtonAnimPalette != NULL) {
		operator delete(ButtonAnimPalette);
		ButtonAnimPalette = NULL;
	}
	if (ButtonAnimDrawer != NULL) {
		delete ButtonAnimDrawer;
		ButtonAnimDrawer = NULL;
	}
}

/// <summary>
/// RA2: frees the areas the dialog frame was laid out in.
/// </summary>
void __fastcall Free_Interface_Rects(void) // RA2: 0x006F4360
{
	operator delete(SlideButtonRect);
	SlideButtonRect = NULL;
	operator delete(SlideMPButtonRect);
	SlideMPButtonRect = NULL;
	operator delete(SlideWarningRect);
	SlideWarningRect = NULL;
	operator delete(DialogBackRect);
	DialogBackRect = NULL;
	operator delete(SlideTopRect);
	SlideTopRect = NULL;
	operator delete(Button_Strip_Top);
	Button_Strip_Top = NULL;
	operator delete(Button_Strip_Bottom);
	Button_Strip_Bottom = NULL;
	operator delete(LowerScreenRect);
	LowerScreenRect = NULL;
}

/// <summary>
/// RA2: loads the sidebar's art and lays it out for the screen.
/// </summary>
void __fastcall Load_Sidebar_Art(void) // RA2: 0x006F4400
{
	Load_Sidebar_Shapes();
	Load_Sidebar_Palettes();
	Layout_Sidebar_Art(VideoModeWidth, VideoModeHeight);
	SidebarArtLoaded = true;
}

/// <summary>
/// RA2: frees the sidebar's art.
/// </summary>
void __fastcall Free_Sidebar_Art(void) // RA2: 0x006F4450
{
	Free_Sidebar_Shapes();
	if (SidebarArtPalette != NULL) {
		operator delete(SidebarArtPalette);
		SidebarArtPalette = NULL;
	}
	if (SidebarArtDrawer != NULL) {
		delete SidebarArtDrawer;
		SidebarArtDrawer = NULL;
	}
	if (UIBackgroundPalette != NULL) {
		operator delete(UIBackgroundPalette);
		UIBackgroundPalette = NULL;
	}
	if (UIBackgroundDrawer != NULL) {
		delete UIBackgroundDrawer;
		UIBackgroundDrawer = NULL;
	}
	Free_Sidebar_Rects();
	SidebarArtLoaded = false;
}

/// <summary>
/// RA2: frees the sidebar's art and loads it again.
/// </summary>
void __fastcall Reload_Sidebar_Art(void) // RA2: 0x006F44D0
{
	Free_Sidebar_Art();
	Load_Sidebar_Shapes();
	Load_Art_Palette(SidebarPalFile, &SidebarArtPalette, &SidebarArtDrawer);
	Load_Art_Palette(UibkgdPalFile, &UIBackgroundPalette, &UIBackgroundDrawer);
	Layout_Sidebar_Art(VideoModeWidth, VideoModeHeight);
	SidebarArtLoaded = true;
}

bool __fastcall Is_Sidebar_Art_Loaded(void) // RA2: 0x006F4590
{
	return SidebarArtLoaded;
}

/// <summary>
/// RA2: sets the outline colour of the tooltips for a side.
/// </summary>
void __fastcall Select_Tooltip_Color(int which) // RA2: 0x006F45A0
{
	if (which == 0) {
		TooltipColor = TooltipColorSide0;
		return;
	}
	TooltipColor = TooltipColorSide1;
}

PaletteClass * __fastcall Get_Game_Palette(void) // RA2: 0x006F45E0
{
	return SidebarArtPalette;
}

ConvertClass * __cdecl Get_Sidebar_Art_Drawer(void) // RA2: 0x006F45F0
{
	return SidebarArtDrawer;
}

PaletteClass * __cdecl Get_UI_Background_Palette(void) // RA2: 0x006F4600
{
	return UIBackgroundPalette;
}

ConvertClass * __cdecl Get_UI_Background_Drawer(void) // RA2: 0x006F4610
{
	return UIBackgroundDrawer;
}

/// <summary>
/// RA2: draws the sidebar's art on a surface: its backdrop, the radar's frame, the strips of buttons and the tab bar.
/// </summary>
void __fastcall Draw_Sidebar_Art(Surface * surface, Rect const & window, bool tabs) // RA2: 0x006F4620
{
	if (!SidebarArtLoaded) {
		Load_Sidebar_Shapes();
		Load_Sidebar_Palettes();
		Layout_Sidebar_Art(VideoModeWidth, VideoModeHeight);
		SidebarArtLoaded = true;
	}
	surface->Fill(0);
	Point2D point(0, 0);
	point.X = SidebarBackRect->X;
	point.Y = SidebarBackRect->Y;
	if (VideoModeWidth == 640) {
		Draw_Shape(*surface, *UIBackgroundDrawer, ArtSidebarBack640, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	} else if (VideoModeWidth == 800) {
		Draw_Shape(*surface, *UIBackgroundDrawer, ArtSidebarBack800, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	} else {
		Draw_Shape(*surface, *UIBackgroundDrawer, ArtSidebarBack1024, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	}
	point.X = TabBackRect->X;
	point.Y = TabBackRect->Y;
	Draw_Shape(*surface, *SidebarArtDrawer, TabBackShape, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	point.X = RadarFrameRect->X;
	point.Y = RadarFrameRect->Y;
	Draw_Shape(*surface, *SidebarArtDrawer, RadarFrameShape, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	point.X = RadarAnimRect->X;
	point.Y = RadarAnimRect->Y;
	Draw_Shape(*surface, *SidebarArtDrawer, RadarAnimShape, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	point.X = Side1Rect->X;
	point.Y = Side1Rect->Y;
	Draw_Shape(*surface, *SidebarArtDrawer, ArtSide1, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	int index = 0;
	point.X = Free_Button_Strip->X;
	point.Y = Free_Button_Strip->Y;
	if (SidebarStripCount > 0) {
		do {
			Draw_Shape(*surface, *SidebarArtDrawer, ArtSide2b, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
			point.Y += Free_Button_Strip->Height;
			index++;
		} while (index < SidebarStripCount);
	}
	point.X = Default_Button_Y->X;
	point.Y = Default_Button_Y->Y;
	Draw_Shape(*surface, *SidebarArtDrawer, ArtSide3, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	point.X = Default_Button_Strip->X;
	point.Y = Default_Button_Strip->Y;
	Draw_Shape(*surface, *SidebarArtDrawer, ArtAddon, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	Rect clip = window;
	clip.Width = Get_Title_Screen_Area().Width; // RA2: the tab bar is as wide as the area the tactical view uses
	point.X = TabLeftRect->X;
	point.Y = TabLeftRect->Y;
	Draw_Shape(*surface, *SidebarArtDrawer, TabLeftShape, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	if (tabs) {
		point.X = ThumbOpenRect->X;
		point.Y = ThumbOpenRect->Y;
		Draw_Shape(*surface, *SidebarArtDrawer, TabThumbShape, 2, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
		index = 0;
		point.X = ButtonSlotRect->X;
		point.Y = ButtonSlotRect->Y;
		if (TabTileCount > 0) {
			do {
				Draw_Shape(*surface, *SidebarArtDrawer, TabTileShape, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
				point.X += ButtonSlotRect->Width;
				index++;
			} while (index < TabTileCount);
		}
	} else {
		point.X = ThumbClosedRect->X;
		point.Y = ThumbClosedRect->Y;
		Draw_Shape(*surface, *SidebarArtDrawer, TabThumbShape, 2, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
	}
	point.X = TabEndRect->X;
	point.Y = TabEndRect->Y;
	Draw_Shape(*surface, *SidebarArtDrawer, TabRightShape, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(ZGRAD_GROUND), 1000, 0, 0, Point2D(0, 0));
}

/// <summary>
/// RA2: loads the shapes of the sidebar.
/// </summary>
void __fastcall Load_Sidebar_Shapes(void) // RA2: 0x006F4B00
{
	Button_Strip_Shape = (ShapeSet *)Load_Alloc_Data(SidebttnShpFile, ButtonStripShapeAllocated);
	ArtSidebarBack640 = (ShapeSet *)Load_Alloc_Data(BkgdsmShpFile, ArtSidebarBack640Allocated);
	ArtSidebarBack800 = (ShapeSet *)Load_Alloc_Data(BkgdmdShpFile, ArtSidebarBack800Allocated);
	ArtSidebarBack1024 = (ShapeSet *)Load_Alloc_Data(BkgdlgShpFile, ArtSidebarBack1024Allocated);
	TabBackShape = (ShapeSet *)Load_Alloc_Data(CreditsShpFile, TabBackShapeAllocated);
	RadarFrameShape = (ShapeSet *)Load_Alloc_Data(TopShpFile, RadarFrameShapeAllocated);
	RadarAnimShape = (ShapeSet *)Load_Alloc_Data(RadarShpFile, RadarAnimShapeAllocated);
	ArtSide1 = (ShapeSet *)Load_Alloc_Data(Side1ShpFile, ArtSide1Allocated);
	ArtSide2 = (ShapeSet *)Load_Alloc_Data(Side2ShpFile, ArtSide2Allocated);
	ArtSide2b = (ShapeSet *)Load_Alloc_Data(Side2bShpFile, ArtSide2bAllocated);
	ArtSide3 = (ShapeSet *)Load_Alloc_Data(Side3ShpFile, ArtSide3Allocated);
	ArtAddon = (ShapeSet *)Load_Alloc_Data(AddonShpFile, ArtAddonAllocated);
	TabLeftShape = (ShapeSet *)Load_Alloc_Data(LspacerShpFile, TabLeftShapeAllocated);
	TabThumbShape = (ShapeSet *)Load_Alloc_Data(LendcapShpFile, TabThumbShapeAllocated);
	TabTileShape = (ShapeSet *)Load_Alloc_Data(BttnbkgdShpFile, TabTileShapeAllocated);
	TabRightShape = (ShapeSet *)Load_Alloc_Data(RendcapShpFile, TabRightShapeAllocated);
}

/// <summary>
/// RA2: loads the palettes of the sidebar.
/// </summary>
void __fastcall Load_Sidebar_Palettes(void) // RA2: 0x006F4C60
{
	Load_Art_Palette(SidebarPalFile, &SidebarArtPalette, &SidebarArtDrawer);
	Load_Art_Palette(UibkgdPalFile, &UIBackgroundPalette, &UIBackgroundDrawer);
}

/// <summary>
/// RA2: works out where the parts of the sidebar and of the tab bar go on a screen of this size.
/// </summary>
void __fastcall Layout_Sidebar_Art(int width, int height) // RA2: 0x006F4C90
{
	ShapeSet * backdrop;
	if (VideoModeWidth == 640) {
		backdrop = ArtSidebarBack640;
	} else if (VideoModeWidth == 800) {
		backdrop = ArtSidebarBack800;
	} else {
		backdrop = ArtSidebarBack1024;
	}
	int w = backdrop->Get_Width();
	int h = backdrop->Get_Height();
	SidebarBackRect = new Rect(0, 0, w, h);

	w = TabBackShape->Get_Width();
	h = TabBackShape->Get_Height();
	int x = width - w;
	TabBackRect = new Rect(x, 0, w, h);
	int y = h;
	h = RadarFrameShape->Get_Height();
	RadarFrameRect = new Rect(x, y, w, h);
	y += h;
	h = RadarAnimShape->Get_Height();
	RadarAnimRect = new Rect(x, y, w, h);
	y += h;
	h = ArtSide1->Get_Height();
	Side1Rect = new Rect(x, y, w, h);
	y += h;
	h = ArtSide2->Get_Height();
	Free_Button_Strip = new Rect(x, y, w, h);
	h = ArtSide3->Get_Height();
	Default_Button_Y = new Rect(x, y, w, h);
	h = ArtAddon->Get_Height();
	Default_Button_Strip = new Rect(x, y, w, h);

	SidebarStripCount = (height - Default_Button_Y->Height - Side1Rect->Height - RadarAnimRect->Height
		- RadarFrameRect->Height - TabBackRect->Height) / Free_Button_Strip->Height;
	y += SidebarStripCount * Free_Button_Strip->Height;
	Default_Button_Y->Y = y;
	Default_Button_Strip->Y = Default_Button_Y->Height + y;

	y = height - 32;
	int bar_width = Get_Title_Screen_Area().Width;
	TabBarRect = new Rect(0, y, bar_width, 32);
	w = TabLeftShape->Get_Width();
	h = TabLeftShape->Get_Height();
	TabLeftRect = new Rect(0, y, w, h);
	w = TabThumbShape->Get_Width();
	h = TabThumbShape->Get_Height();
	ThumbOpenRect = new Rect(0, y, w, h);
	ThumbClosedRect = new Rect(0, y, w, h);
	w = TabTileShape->Get_Width();
	h = TabTileShape->Get_Height();
	ButtonSlotRect = new Rect(0, y, w, h);
	w = TabRightShape->Get_Width();
	h = TabRightShape->Get_Height();
	TabEndRect = new Rect(0, y, w, h);

	TabTileCount = (width - ThumbOpenRect->Width - TabBackRect->Width - TabEndRect->Width)
		/ ButtonSlotRect->Width;
	int bar_x = TabBackRect->X - TabEndRect->Width;
	TabEndRect->X = bar_x;
	bar_x -= ButtonSlotRect->Width * TabTileCount;
	ButtonSlotRect->X = bar_x;
	bar_x -= ThumbOpenRect->Width;
	ThumbOpenRect->X = bar_x;
	ThumbOpenX = bar_x;
	int tab_x = TabEndRect->X - ThumbClosedRect->Width;
	ThumbClosedRect->X = tab_x;
	ThumbClosedX = tab_x;
	ThumbTravel = TabEndRect->X - ThumbOpenRect->Width - ThumbOpenRect->X;

	int column_x = RadarFrameRect->X;
	int column_y = RadarFrameRect->Y;
	int column_width = RadarFrameRect->Width;
	int column_height = RadarAnimRect->Height + RadarFrameRect->Height;
	RadarAreaRect = new Rect(column_x, column_y, column_width, column_height);
	int strip_x = Side1Rect->X;
	int strip_y = Side1Rect->Y;
	int strip_width = Side1Rect->Width;
	int strip_height = SidebarStripCount * Free_Button_Strip->Height + Default_Button_Y->Height + Side1Rect->Height;
	SidebarButtonsRect = new Rect(strip_x, strip_y, strip_width, strip_height);
}

/// <summary>
/// RA2: frees the shapes of the sidebar.
/// </summary>
void __fastcall Free_Sidebar_Shapes(void) // RA2: 0x006F5150
{
	if (ButtonStripShapeAllocated) {
		delete Button_Strip_Shape;
		ButtonStripShapeAllocated = false;
	}
	Button_Strip_Shape = NULL;
	if (ArtSidebarBack640Allocated) {
		delete ArtSidebarBack640;
		ArtSidebarBack640Allocated = false;
	}
	ArtSidebarBack640 = NULL;
	if (ArtSidebarBack800Allocated) {
		delete ArtSidebarBack800;
		ArtSidebarBack800Allocated = false;
	}
	ArtSidebarBack800 = NULL;
	if (ArtSidebarBack1024Allocated) {
		delete ArtSidebarBack1024;
		ArtSidebarBack1024Allocated = false;
	}
	ArtSidebarBack1024 = NULL;
	if (TabBackShapeAllocated) {
		delete (ShapeSet *)TabBackShape;
		TabBackShapeAllocated = false;
	}
	TabBackShape = NULL;
	if (RadarFrameShapeAllocated) {
		delete (ShapeSet *)RadarFrameShape;
		RadarFrameShapeAllocated = false;
	}
	RadarFrameShape = NULL;
	if (RadarAnimShapeAllocated) {
		delete (ShapeSet *)RadarAnimShape;
		RadarAnimShapeAllocated = false;
	}
	RadarAnimShape = NULL;
	if (ArtSide1Allocated) {
		delete ArtSide1;
		ArtSide1Allocated = false;
	}
	ArtSide1 = NULL;
	if (ArtSide2Allocated) {
		delete ArtSide2;
		ArtSide2Allocated = false;
	}
	ArtSide2 = NULL;
	if (ArtSide2bAllocated) {
		delete ArtSide2b;
		ArtSide2bAllocated = false;
	}
	ArtSide2b = NULL;
	if (ArtSide3Allocated) {
		delete ArtSide3;
		ArtSide3Allocated = false;
	}
	ArtSide3 = NULL;
	if (ArtAddonAllocated) {
		delete ArtAddon;
		ArtAddonAllocated = false;
	}
	ArtAddon = NULL;
	if (TabLeftShapeAllocated) {
		delete (ShapeSet *)TabLeftShape;
		TabLeftShapeAllocated = false;
	}
	TabLeftShape = NULL;
	if (TabThumbShapeAllocated) {
		delete (ShapeSet *)TabThumbShape;
		TabThumbShapeAllocated = false;
	}
	TabThumbShape = NULL;
	if (TabTileShapeAllocated) {
		delete (ShapeSet *)TabTileShape;
		TabTileShapeAllocated = false;
	}
	TabTileShape = NULL;
	if (TabRightShapeAllocated) {
		delete (ShapeSet *)TabRightShape;
		TabRightShapeAllocated = false;
	}
	TabRightShape = NULL;
}

/// <summary>
/// RA2: frees the sidebar's palettes, when they are loaded.
/// </summary>
void __fastcall Free_Sidebar_Palettes(void) // RA2: 0x006F5390
{
	if (SidebarArtPalette != NULL) {
		delete SidebarArtPalette;
		SidebarArtPalette = NULL;
	}
	if (SidebarArtDrawer != NULL) {
		delete SidebarArtDrawer;
		SidebarArtDrawer = NULL;
	}
	if (UIBackgroundPalette != NULL) {
		delete UIBackgroundPalette;
		UIBackgroundPalette = NULL;
	}
	if (UIBackgroundDrawer != NULL) {
		delete UIBackgroundDrawer;
		UIBackgroundDrawer = NULL;
	}
}

/// <summary>
/// RA2: frees the areas the sidebar and the tab bar were laid out in.
/// </summary>
void __fastcall Free_Sidebar_Rects(void) // RA2: 0x006F5400
{
	operator delete(SidebarBackRect);
	SidebarBackRect = NULL;
	operator delete(TabBackRect);
	TabBackRect = NULL;
	operator delete(RadarFrameRect);
	RadarFrameRect = NULL;
	operator delete(RadarAnimRect);
	RadarAnimRect = NULL;
	operator delete(RadarAreaRect);
	RadarAreaRect = NULL;
	operator delete(Side1Rect);
	Side1Rect = NULL;
	operator delete(Free_Button_Strip);
	Free_Button_Strip = NULL;
	operator delete(Default_Button_Y);
	Default_Button_Y = NULL;
	operator delete(Default_Button_Strip);
	Default_Button_Strip = NULL;
	operator delete(SidebarButtonsRect);
	SidebarButtonsRect = NULL;
	operator delete(TabBarRect);
	TabBarRect = NULL;
	operator delete(TabLeftRect);
	TabLeftRect = NULL;
	operator delete(ThumbOpenRect);
	ThumbOpenRect = NULL;
	operator delete(ThumbClosedRect);
	ThumbClosedRect = NULL;
	operator delete(ButtonSlotRect);
	ButtonSlotRect = NULL;
	operator delete(TabEndRect);
	TabEndRect = NULL;
}

/// <summary>
/// RA2: frees the sidebar's shapes and loads them again.
/// </summary>
void __fastcall Reload_Sidebar_Shapes(void) // RA2: 0x006F5530
{
	Free_Sidebar_Shapes();
	Load_Sidebar_Shapes();
}

/// <summary>
/// RA2: frees the sidebar's palettes and loads them again.
/// </summary>
void __fastcall Reload_Sidebar_Palettes(void) // RA2: 0x006F5540
{
	if (SidebarArtPalette != NULL) {
		operator delete(SidebarArtPalette);
		SidebarArtPalette = NULL;
	}
	if (SidebarArtDrawer != NULL) {
		delete SidebarArtDrawer;
		SidebarArtDrawer = NULL;
	}
	if (UIBackgroundPalette != NULL) {
		operator delete(UIBackgroundPalette);
		UIBackgroundPalette = NULL;
	}
	if (UIBackgroundDrawer != NULL) {
		delete UIBackgroundDrawer;
		UIBackgroundDrawer = NULL;
	}
	Load_Art_Palette(SidebarPalFile, &SidebarArtPalette, &SidebarArtDrawer);
	Load_Art_Palette(UibkgdPalFile, &UIBackgroundPalette, &UIBackgroundDrawer);
}

/// <summary>
/// RA2: lays the sidebar out again for a screen size.
/// </summary>
void __fastcall Relayout_Sidebar_Art(int width, int height) // RA2: 0x006F55D0
{
	Free_Sidebar_Rects();
	Layout_Sidebar_Art(width, height);
}
