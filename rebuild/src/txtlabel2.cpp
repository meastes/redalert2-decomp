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
#include "convert.h" // RA2: as above
#include "data.h" // RA2: as above
#include "draw.h" // RA2: as above
#include "dsurface.h" // RA2: as above
#include "goptions.h" // RA2: as above
#include "palette.h" // RA2: as above
#include "rect.h" // RA2: as above
#include "rgb.h" // RA2: as above
#include "shapeset.h" // RA2: as above
#include "_rect.h" // RA2: as above
#include "_surface.h" // RA2: as above
#include "globals.h" // RA2: as above
#include "tab.h" // RA2: as above
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

Point2D __fastcall Copy_Point(Point2D const * source) // RA2: 0x006F1AE0; a copy of a point
{
	Point2D result(source->X, source->Y); // RA2: as above
	return result; // RA2: as above
}

/// <summary>
/// RA2: assigns one point to another.
/// </summary>
void __fastcall Assign_Point(Point2D * dest, Point2D const * source) // RA2: 0x006F1AF0
{
	*dest = *source; // RA2: as above
}

/// <summary>
/// RA2: moves each edge of a rectangle in by an amount.
/// </summary>
void __fastcall Inflate_Rect(Rect & rect, int amount) // RA2: 0x006F1B00
{
	rect.X += amount; // RA2: as above
	rect.Y += amount; // RA2: as above
	rect.Width -= amount * 2; // RA2: as above
	rect.Height -= amount * 2; // RA2: as above
}

/// <summary>
/// RA2: moves the left and right edges of a rectangle in by one amount and the top and bottom edges by another.
/// </summary>
void __fastcall Inflate_Rect(Rect & rect, int dx, int dy) // RA2: 0x006F1B30
{
	rect.X += dx; // RA2: as above
	rect.Y += dy; // RA2: as above
	rect.Width -= dx * 2; // RA2: as above
	rect.Height -= dy * 2; // RA2: as above
}

/// <summary>
/// RA2: loads all the art: the popup dialogs' shapes and palettes and the dialog frame's art.
/// </summary>
void __fastcall Load_All_Art(void) // RA2: 0x006F1B60
{
	Load_Popup_Dialog_Art(); // RA2: as above
	Load_Interface_Art(); // RA2: as above
}

/// <summary>
/// RA2: frees all the art: the popup dialogs', the dialog frame's and the sidebar's.
/// </summary>
void __fastcall Free_All_Art(void) // RA2: 0x006F1D20
{
	Free_Popup_Dialog_Art(); // RA2: as above
	Free_Interface_Art(); // RA2: as above
	Free_Sidebar_Art(); // RA2: as above
}

/// <summary>
/// RA2: the area of the screen the tactical view and its title screens use: the screen without the sidebar.
/// </summary>
Rect __fastcall Get_Title_Screen_Area(void) // RA2: 0x006F1F30
{
	Rect rect = VisibleRect; // RA2: as above
	rect.X = 0; // RA2: as above
	rect.Y = 0; // RA2: as above
	if (!Options.IsSidebarOnRight && !Debug_Map) { // RA2: as above
		rect.X = 168; // RA2: the sidebar is on the left, so the area starts right of it
	}
	rect.Width -= 168; // RA2: as above
	rect.Height -= 32; // RA2: the tab bar takes the bottom
	return rect; // RA2: as above
}

/// <summary>
/// RA2: reads a palette file into a new palette and builds the drawer that goes with it.
/// </summary>
void __fastcall Load_Art_Palette(char const * name, PaletteClass * * palette, ConvertClass * * drawer) // RA2: 0x006F1FA0
{
	bool allocated = false; // RA2: as above
	unsigned char const * data = (unsigned char const *)Load_Alloc_Data(name, allocated); // RA2: as above
	*palette = new PaletteClass; // RA2: as above
	for (int index = 0; index < 256; index++) { // RA2: as above
		int offset = index * 3; // RA2: as above
		RGBClass color(data[offset] << 2, data[offset + 1] << 2, data[offset + 2] << 2); // RA2: the file holds 6 bit colours
		(**palette)[index] = color; // RA2: as above
	}
	if (allocated) { // RA2: as above
		delete (void *)data; // RA2: as above
	}
	*drawer = new ConvertClass(**palette, **palette, *AlternateSurface, 1, false); // RA2: as above
}

/// <summary>
/// RA2: loads the popup dialogs' shapes and the dialog palettes.
/// </summary>
void __fastcall Load_Popup_Dialog_Art(void) // RA2: 0x006F20B0
{
	Load_Popup_Dialog_Shapes(); // RA2: as above
	Load_Dialog_Palettes(); // RA2: as above
}

/// <summary>
/// RA2: frees the popup dialogs' shapes and the dialog palettes.
/// </summary>
void __fastcall Free_Popup_Dialog_Art(void) // RA2: 0x006F2130
{
	Free_Popup_Dialog_Shapes(); // RA2: as above
	Free_Dialog_Palettes(); // RA2: as above
}

PaletteClass * __cdecl Get_Dialog_Palette(void) // RA2: 0x006F2220
{
	return DialogPalette; // RA2: as above
}

ConvertClass * __cdecl Get_Dialog_Drawer(void) // RA2: 0x006F2230
{
	return DialogDrawer; // RA2: as above
}

PaletteClass * __cdecl Get_Main_Button_Palette(void) // RA2: 0x006F2240
{
	return MainButtonPalette; // RA2: as above
}

ConvertClass * __cdecl Get_Main_Button_Drawer(void) // RA2: 0x006F2250
{
	return MainButtonDrawer; // RA2: as above
}

/// <summary>
/// RA2: loads the shapes of the popup dialogs and the main button.
/// </summary>
void __fastcall Load_Popup_Dialog_Shapes(void) // RA2: 0x006F2260
{
	ArtPopupBack = Load_Alloc_Data(PudlgbgnShpFile, ArtPopupBackAllocated); // RA2: as above
	ArtPopupBackAllied = Load_Alloc_Data(PudlgbgaShpFile, ArtPopupBackAlliedAllocated); // RA2: as above
	ArtPopupBackSoviet = Load_Alloc_Data(PudlgbgsShpFile, ArtPopupBackSovietAllocated); // RA2: as above
	ArtMainButton = Load_Alloc_Data(MnbttnShpFile, ArtMainButtonAllocated); // RA2: as above
}

/// <summary>
/// RA2: loads the dialog palette and the palette of the main buttons, with their drawers.
/// </summary>
void __fastcall Load_Dialog_Palettes(void) // RA2: 0x006F22C0
{
	Load_Art_Palette(DialogPalFile, &DialogPalette, &DialogDrawer); // RA2: as above
	Load_Art_Palette(MainbttnPalFile, &MainButtonPalette, &MainButtonDrawer); // RA2: as above
}

/// <summary>
/// RA2: frees the shapes of the popup dialogs and the main button.
/// </summary>
void __fastcall Free_Popup_Dialog_Shapes(void) // RA2: 0x006F22F0
{
	if (ArtPopupBackAllocated) { // RA2: as above
		operator delete(ArtPopupBack); // RA2: as above
		ArtPopupBackAllocated = false; // RA2: as above
	}
	ArtPopupBack = NULL; // RA2: as above
	if (ArtPopupBackAlliedAllocated) { // RA2: as above
		operator delete(ArtPopupBackAllied); // RA2: as above
		ArtPopupBackAlliedAllocated = false; // RA2: as above
	}
	ArtPopupBackAllied = NULL; // RA2: as above
	if (ArtPopupBackSovietAllocated) { // RA2: as above
		operator delete(ArtPopupBackSoviet); // RA2: as above
		ArtPopupBackSovietAllocated = false; // RA2: as above
	}
	ArtPopupBackSoviet = NULL; // RA2: as above
	if (ArtMainButtonAllocated) { // RA2: as above
		operator delete(ArtMainButton); // RA2: as above
		ArtMainButtonAllocated = false; // RA2: as above
	}
	ArtMainButton = NULL; // RA2: as above
}

/// <summary>
/// RA2: frees the dialog palette and the palette of the main buttons, with their drawers.
/// </summary>
void __fastcall Free_Dialog_Palettes(void) // RA2: 0x006F2390
{
	if (DialogPalette != NULL) { // RA2: as above
		operator delete(DialogPalette); // RA2: as above
		DialogPalette = NULL; // RA2: as above
	}
	if (DialogDrawer != NULL) { // RA2: as above
		delete [] DialogDrawer; // RA2: as above
		DialogDrawer = NULL; // RA2: as above
	}
	if (MainButtonPalette != NULL) { // RA2: as above
		operator delete(MainButtonPalette); // RA2: as above
		MainButtonPalette = NULL; // RA2: as above
	}
	if (MainButtonDrawer != NULL) { // RA2: as above
		delete [] MainButtonDrawer; // RA2: as above
		MainButtonDrawer = NULL; // RA2: as above
	}
}

/// <summary>
/// RA2: the title bar's file name for a screen width.
/// </summary>
char const * __fastcall Get_Title_Bar_File(int width, int, int) // RA2: 0x006F2400
{
	char const * file = Ttlbr640ShpFile; // RA2: as above
	if (width != 640) { // RA2: as above
		file = Ttlbr800ShpFile; // RA2: as above
	}
	return file; // RA2: as above
}

/// <summary>
/// RA2: the splash bar's file name for a screen width.
/// </summary>
char const * __fastcall Get_Splash_Bar_File(int width, int, int) // RA2: 0x006F2420
{
	char const * file = SpldbrsShpFile; // RA2: as above
	if (width != 640) { // RA2: as above
		file = SpldbrlShpFile; // RA2: as above
	}
	return file; // RA2: as above
}

PaletteClass * __cdecl Get_Load_Screen_Palette(void) // RA2: 0x006F2440
{
	return LoadScreenPalette; // RA2: as above
}

ConvertClass * __cdecl Get_Load_Screen_Drawer(void) // RA2: 0x006F2450
{
	return LoadScreenDrawer; // RA2: as above
}

PaletteClass * __cdecl Get_Load_Screen_Side_Palette(void) // RA2: 0x006F2460
{
	return LoadScreenSidePalette; // RA2: as above
}

ConvertClass * __cdecl Get_Load_Screen_Side_Drawer(void) // RA2: 0x006F2470
{
	return LoadScreenSideDrawer; // RA2: as above
}

PaletteClass * __cdecl Get_Splash_Bar_Palette(void) // RA2: 0x006F2480
{
	return SplashBarPalette; // RA2: as above
}

ConvertClass * __cdecl Get_Splash_Bar_Drawer(void) // RA2: 0x006F2490
{
	return SplashBarDrawer; // RA2: as above
}

/// <summary>
/// RA2: loads the palettes of the loading screens, the ones still missing, for a side.
/// </summary>
void __fastcall Load_Loading_Screen_Palettes(int side) // RA2: 0x006F24A0
{
	if (LoadScreenDrawer == NULL) { // RA2: as above
		Load_Art_Palette(LdscrnPalFile, &LoadScreenPalette, &LoadScreenDrawer); // RA2: as above
	}
	if (LoadScreenSideDrawer == NULL) { // RA2: as above
		if (side == 0) { // RA2: as above
			Load_Art_Palette(LdscrnaPalFile, &LoadScreenSidePalette, &LoadScreenSideDrawer); // RA2: as above
		} else {
			Load_Art_Palette(LdscrnsPalFile, &LoadScreenSidePalette, &LoadScreenSideDrawer); // RA2: as above
		}
	}
	if (SplashBarDrawer == NULL) { // RA2: as above
		Load_Art_Palette(SpldbrPalFile, &SplashBarPalette, &SplashBarDrawer); // RA2: as above
	}
}

/// <summary>
/// RA2: frees the palettes of the loading screens.
/// </summary>
void __fastcall Free_Loading_Screen_Palettes(void) // RA2: 0x006F2510
{
	if (LoadScreenPalette != NULL) { // RA2: as above
		operator delete(LoadScreenPalette); // RA2: as above
		LoadScreenPalette = NULL; // RA2: as above
	}
	if (LoadScreenDrawer != NULL) { // RA2: as above
		delete LoadScreenDrawer; // RA2: as above
		LoadScreenDrawer = NULL; // RA2: as above
	}
	if (LoadScreenSidePalette != NULL) { // RA2: as above
		operator delete(LoadScreenSidePalette); // RA2: as above
		LoadScreenSidePalette = NULL; // RA2: as above
	}
	if (LoadScreenSideDrawer != NULL) { // RA2: as above
		delete LoadScreenSideDrawer; // RA2: as above
		LoadScreenSideDrawer = NULL; // RA2: as above
	}
	if (SplashBarPalette != NULL) { // RA2: as above
		operator delete(SplashBarPalette); // RA2: as above
		SplashBarPalette = NULL; // RA2: as above
	}
	if (SplashBarDrawer != NULL) { // RA2: as above
		delete SplashBarDrawer; // RA2: as above
		SplashBarDrawer = NULL; // RA2: as above
	}
}

PaletteClass * __cdecl Get_MP_Load_Palette(void) // RA2: 0x006F25A0
{
	return MPLoadPalette; // RA2: as above
}

ConvertClass * __cdecl Get_MP_Load_Drawer(void) // RA2: 0x006F25B0
{
	return MPLoadDrawer; // RA2: as above
}

/// <summary>
/// RA2: loads the palette of the multiplayer loading screen, the observer's one when observing; a palette of the
/// other kind is freed first.
/// </summary>
void __fastcall Load_MP_Load_Palette(void) // RA2: 0x006F25C0
{
	if (MPLoadPaletteKind == 1 && ObserverMode != 0) { // RA2: as above
		Free_MP_Load_Palette(); // RA2: as above
	}
	if (MPLoadPaletteKind == 2 && ObserverMode == 0) { // RA2: as above
		Free_MP_Load_Palette(); // RA2: as above
	}
	if (MPLoadDrawer == NULL) { // RA2: as above
		MPLoadPaletteKind = 0; // RA2: as above
	}
	if (MPLoadPaletteKind == 0) { // RA2: as above
		if (ObserverMode != 0) { // RA2: as above
			Load_Art_Palette(MplsobsPalFile, &MPLoadPalette, &MPLoadDrawer); // RA2: as above
			MPLoadPaletteKind = 2; // RA2: as above
		} else {
			Load_Art_Palette(MplsPalFile, &MPLoadPalette, &MPLoadDrawer); // RA2: as above
			MPLoadPaletteKind = 1; // RA2: as above
		}
	}
}

/// <summary>
/// RA2: frees the palette of the multiplayer loading screen.
/// </summary>
void __fastcall Free_MP_Load_Palette(void) // RA2: 0x006F26D0
{
	if (MPLoadPalette != NULL) { // RA2: as above
		delete MPLoadPalette; // RA2: as above
		MPLoadPalette = NULL; // RA2: as above
	}
	if (MPLoadDrawer != NULL) { // RA2: as above
		delete MPLoadDrawer; // RA2: as above
		MPLoadDrawer = NULL; // RA2: as above
	}
}

/// <summary>
/// RA2: loads the score screens' art, the shapes and the palette, for the Allied or the Soviet side.
/// </summary>
void __fastcall Load_Score_Art(int alternate) // RA2: 0x006F2710
{
	if (!ScoreArtLoaded) { // RA2: as above
		Load_Score_Shapes(alternate); // RA2: as above
		char const * name = AscorePalFile; // RA2: as above
		if (alternate) { // RA2: as above
			name = SscorePalFile; // RA2: as above
		}
		Load_Art_Palette(name, &ScorePalette, &ScoreDrawer); // RA2: as above
		ScoreArtLoaded = true; // RA2: as above
	}
}

/// <summary>
/// RA2: frees the score screens' art.
/// </summary>
void __cdecl Free_Score_Art(void) // RA2: 0x006F2750
{
	if (ScoreArtLoaded) { // RA2: as above
		ScoreArtLoaded = false; // RA2: as above
		if (ArtChatBackAllocated) { // RA2: as above
			operator delete(Art_Chat_Back); // RA2: as above
			ArtChatBackAllocated = false; // RA2: as above
		}
		Art_Chat_Back = NULL; // RA2: as above
		if (ArtScoreTopAllocated) { // RA2: as above
			operator delete(ArtScoreTop); // RA2: as above
			ArtScoreTopAllocated = false; // RA2: as above
		}
		ArtScoreTop = NULL; // RA2: as above
		if (ArtScoreFrameAllocated) { // RA2: as above
			operator delete(Art_Score_Frame); // RA2: as above
			ArtScoreFrameAllocated = false; // RA2: as above
		}
		Art_Score_Frame = NULL; // RA2: as above
		if (ScorePalette != NULL) { // RA2: as above
			operator delete(ScorePalette); // RA2: as above
			ScorePalette = NULL; // RA2: as above
		}
		if (ScoreDrawer != NULL) { // RA2: as above
			delete ScoreDrawer; // RA2: as above
			ScoreDrawer = NULL; // RA2: as above
		}
	}
}

bool __cdecl Is_Score_Art_Loaded(void) // RA2: 0x006F2810
{
	return ScoreArtLoaded; // RA2: as above
}

PaletteClass * __cdecl Get_Score_Palette(void) // RA2: 0x006F2820
{
	return ScorePalette; // RA2: as above
}

void * __fastcall Get_Chat_Art(void) // RA2: 0x006F2830
{
	return ScoreDrawer; // RA2: as above
}

/// <summary>
/// RA2: loads the score screens' shapes, in the size that fits the screen, for the Allied or the Soviet side.
/// </summary>
void __fastcall Load_Score_Shapes(int alternate) // RA2: 0x006F2840
{
	if (!alternate) { // RA2: as above
		if (VideoModeWidth == 640) { // RA2: as above
			Art_Chat_Back = (ShapeSet *)Load_Alloc_Data(AscrbksmShpFile, ArtChatBackAllocated); // RA2: as above
			ArtScoreTop = Load_Alloc_Data(AscrtsmShpFile, ArtScoreTopAllocated); // RA2: as above
			Art_Score_Frame = Load_Alloc_Data(AscrasmShpFile, ArtScoreFrameAllocated); // RA2: as above
		} else {
			Art_Chat_Back = (ShapeSet *)Load_Alloc_Data(AscrbkmdShpFile, ArtChatBackAllocated); // RA2: as above
			ArtScoreTop = Load_Alloc_Data(AscrtmdShpFile, ArtScoreTopAllocated); // RA2: as above
			Art_Score_Frame = Load_Alloc_Data(AscramdShpFile, ArtScoreFrameAllocated); // RA2: as above
		}
	} else {
		if (VideoModeWidth == 640) { // RA2: as above
			Art_Chat_Back = (ShapeSet *)Load_Alloc_Data(SscrbksmShpFile, ArtChatBackAllocated); // RA2: as above
			ArtScoreTop = Load_Alloc_Data(SscrtsmShpFile, ArtScoreTopAllocated); // RA2: as above
			Art_Score_Frame = Load_Alloc_Data(SscrasmShpFile, ArtScoreFrameAllocated); // RA2: as above
		} else {
			Art_Chat_Back = (ShapeSet *)Load_Alloc_Data(SscrbkmdShpFile, ArtChatBackAllocated); // RA2: as above
			ArtScoreTop = Load_Alloc_Data(SscrtmdShpFile, ArtScoreTopAllocated); // RA2: as above
			Art_Score_Frame = Load_Alloc_Data(SscramdShpFile, ArtScoreFrameAllocated); // RA2: as above
		}
	}
}

/// <summary>
/// RA2: loads the score screens' palette for the Allied or the Soviet side.
/// </summary>
void __fastcall Load_Score_Palette(int alternate) // RA2: 0x006F2960
{
	Load_Art_Palette(alternate == 0 ? AscorePalFile : SscorePalFile, &ScorePalette, &ScoreDrawer); // RA2: as above
}

/// <summary>
/// RA2: frees the score screens' shapes.
/// </summary>
void __fastcall Free_Score_Shapes(void) // RA2: 0x006F2980
{
	if (ArtChatBackAllocated) { // RA2: as above
		operator delete(Art_Chat_Back); // RA2: as above
		ArtChatBackAllocated = false; // RA2: as above
	}
	Art_Chat_Back = NULL; // RA2: as above
	if (ArtScoreTopAllocated) { // RA2: as above
		operator delete(ArtScoreTop); // RA2: as above
		ArtScoreTopAllocated = false; // RA2: as above
	}
	ArtScoreTop = NULL; // RA2: as above
	if (ArtScoreFrameAllocated) { // RA2: as above
		operator delete(Art_Score_Frame); // RA2: as above
		ArtScoreFrameAllocated = false; // RA2: as above
	}
	Art_Score_Frame = NULL; // RA2: as above
}

/// <summary>
/// RA2: frees the score screens' palette.
/// </summary>
void __fastcall Free_Score_Palette(void) // RA2: 0x006F29F0
{
	if (ScorePalette != NULL) { // RA2: as above
		operator delete(ScorePalette); // RA2: as above
		ScorePalette = NULL; // RA2: as above
	}
	if (ScoreDrawer != NULL) { // RA2: as above
		delete ScoreDrawer; // RA2: as above
		ScoreDrawer = NULL; // RA2: as above
	}
}

/// <summary>
/// RA2: loads the multiplayer score screen's art for a side.
/// </summary>
void __fastcall Load_MP_Score_Art(int side) // RA2: 0x006F2A30
{
	if (MPScoreArtLoaded) { // RA2: as above
		return; // RA2: as above
	}
	Load_MP_Score_Shape(side); // RA2: as above
	Load_Art_Palette(side == 0 ? MpascrnPalFile : MpsscrnPalFile, &MPScorePalette, &MPScoreDrawer); // RA2: as above
	MPScoreArtLoaded = true; // RA2: as above
}

/// <summary>
/// RA2: frees the multiplayer score screen's art.
/// </summary>
void __fastcall Free_MP_Score_Art(void) // RA2: 0x006F2AB0
{
	if (MPScoreArtLoaded) { // RA2: as above
		MPScoreArtLoaded = false; // RA2: as above
		if (ArtScoreScreenAllocated) { // RA2: as above
			operator delete(Art_Score_Screen); // RA2: as above
			ArtScoreScreenAllocated = false; // RA2: as above
		}
		Art_Score_Screen = NULL; // RA2: as above
		if (MPScorePalette != NULL) { // RA2: as above
			operator delete(MPScorePalette); // RA2: as above
			MPScorePalette = NULL; // RA2: as above
		}
		if (MPScoreDrawer != NULL) { // RA2: as above
			delete MPScoreDrawer; // RA2: as above
			MPScoreDrawer = NULL; // RA2: as above
		}
	}
}

bool __cdecl Is_MP_Score_Art_Loaded(void) // RA2: 0x006F2B20
{
	return MPScoreArtLoaded; // RA2: as above
}

PaletteClass * __cdecl Get_MP_Score_Palette(void) // RA2: 0x006F2B30
{
	return MPScorePalette; // RA2: as above
}

void * __fastcall Get_Score_Drawer(void) // RA2: 0x006F2B40
{
	return MPScoreDrawer; // RA2: as above
}

/// <summary>
/// RA2: loads the multiplayer score screen's shape, in the size that fits the screen, for a side.
/// </summary>
void __fastcall Load_MP_Score_Shape(int alternate) // RA2: 0x006F2B50
{
	if (alternate == 0) { // RA2: as above
		if (VideoModeWidth == 640) { // RA2: as above
			Art_Score_Screen = (ShapeSet *)Load_Alloc_Data(MpascrnsShpFile, ArtScoreScreenAllocated); // RA2: as above
		} else {
			Art_Score_Screen = (ShapeSet *)Load_Alloc_Data(MpascrnlShpFile, ArtScoreScreenAllocated); // RA2: as above
		}
	} else {
		if (VideoModeWidth == 640) { // RA2: as above
			Art_Score_Screen = (ShapeSet *)Load_Alloc_Data(MpsscrnsShpFile, ArtScoreScreenAllocated); // RA2: as above
		} else {
			Art_Score_Screen = (ShapeSet *)Load_Alloc_Data(MpsscrnlShpFile, ArtScoreScreenAllocated); // RA2: as above
		}
	}
}

/// <summary>
/// RA2: loads the multiplayer score screen's palette for a side.
/// </summary>
void __fastcall Load_MP_Score_Palette(int alternate) // RA2: 0x006F2BB0
{
	Load_Art_Palette(alternate == 0 ? MpascrnPalFile : MpsscrnPalFile, &MPScorePalette, &MPScoreDrawer); // RA2: as above
}

/// <summary>
/// RA2: frees the multiplayer score screen's shape.
/// </summary>
void __fastcall Free_MP_Score_Shape(void) // RA2: 0x006F2BD0
{
	if (ArtScoreScreenAllocated) { // RA2: as above
		operator delete(Art_Score_Screen); // RA2: as above
		ArtScoreScreenAllocated = false; // RA2: as above
	}
	Art_Score_Screen = NULL; // RA2: as above
}

/// <summary>
/// RA2: frees the multiplayer score screen's palette.
/// </summary>
void __fastcall Free_MP_Score_Palette(void) // RA2: 0x006F2C00
{
	if (MPScorePalette != NULL) { // RA2: as above
		operator delete(MPScorePalette); // RA2: as above
		MPScorePalette = NULL; // RA2: as above
	}
	if (MPScoreDrawer != NULL) { // RA2: as above
		delete MPScoreDrawer; // RA2: as above
		MPScoreDrawer = NULL; // RA2: as above
	}
}

/// <summary>
/// RA2: loads the animation the side buttons of the campaign dialog show.
/// </summary>
void __fastcall Load_Side_Animation_Art(void) // RA2: 0x006F2C40
{
	if (VideoModeWidth == 640) { // RA2: as above
		Art_Side_Animation = Load_Alloc_Data(FsasmShpFile, ArtSideAnimationAllocated); // RA2: as above
		Art_Button_Corner = Load_Alloc_Data(FsbcsmShpFile, ArtButtonCornerAllocated); // RA2: as above
		Art_Side_Strip = Load_Alloc_Data(FsssmShpFile, ArtSideStripAllocated); // RA2: as above
		Art_Button_Back = Load_Alloc_Data(FsbkgdsmShpFile, ArtButtonBackAllocated); // RA2: as above
	} else {
		Art_Side_Animation = Load_Alloc_Data(FsalgShpFile, ArtSideAnimationAllocated); // RA2: as above
		Art_Button_Corner = Load_Alloc_Data(FsbclgShpFile, ArtButtonCornerAllocated); // RA2: as above
		Art_Side_Strip = Load_Alloc_Data(FsslgShpFile, ArtSideStripAllocated); // RA2: as above
		Art_Button_Back = Load_Alloc_Data(FsbkgdlgShpFile, ArtButtonBackAllocated); // RA2: as above
	}
	Load_Art_Palette(FsscrnPalFile, &SideAnimationPalette, &SideAnimationDrawer); // RA2: as above
}

/// <summary>
/// RA2: frees the animation the side buttons of the campaign dialog show.
/// </summary>
void __fastcall Free_Side_Animation_Art(void) // RA2: 0x006F2D00
{
	if (ArtSideAnimationAllocated) { // RA2: as above
		operator delete(Art_Side_Animation); // RA2: as above
		ArtSideAnimationAllocated = false; // RA2: as above
	}
	Art_Side_Animation = NULL; // RA2: as above
	if (ArtButtonCornerAllocated) { // RA2: as above
		operator delete(Art_Button_Corner); // RA2: as above
		ArtButtonCornerAllocated = false; // RA2: as above
	}
	Art_Button_Corner = NULL; // RA2: as above
	if (ArtSideStripAllocated) { // RA2: as above
		operator delete(Art_Side_Strip); // RA2: as above
		ArtSideStripAllocated = false; // RA2: as above
	}
	Art_Side_Strip = NULL; // RA2: as above
	if (ArtButtonBackAllocated) { // RA2: as above
		operator delete(Art_Button_Back); // RA2: as above
		ArtButtonBackAllocated = false; // RA2: as above
	}
	Art_Button_Back = NULL; // RA2: as above
	if (SideAnimationPalette != NULL) { // RA2: as above
		operator delete(SideAnimationPalette); // RA2: as above
		SideAnimationPalette = NULL; // RA2: as above
	}
	if (SideAnimationDrawer != NULL) { // RA2: as above
		delete SideAnimationDrawer; // RA2: as above
		SideAnimationDrawer = NULL; // RA2: as above
	}
}

PaletteClass * __cdecl Get_Side_Animation_Palette(void) // RA2: 0x006F2DD0
{
	return SideAnimationPalette; // RA2: as above
}

void * __fastcall Get_Slot_Art(void) // RA2: 0x006F2DE0
{
	return SideAnimationDrawer; // RA2: as above
}

/// <summary>
/// RA2: loads the shapes of the side animation, in the size that fits the screen.
/// </summary>
void __fastcall Load_Side_Animation_Shapes(void) // RA2: 0x006F2DF0
{
	if (VideoModeWidth == 640) { // RA2: as above
		Art_Side_Animation = Load_Alloc_Data(FsasmShpFile, ArtSideAnimationAllocated); // RA2: as above
		Art_Button_Corner = Load_Alloc_Data(FsbcsmShpFile, ArtButtonCornerAllocated); // RA2: as above
		Art_Side_Strip = Load_Alloc_Data(FsssmShpFile, ArtSideStripAllocated); // RA2: as above
		Art_Button_Back = Load_Alloc_Data(FsbkgdsmShpFile, ArtButtonBackAllocated); // RA2: as above
	} else {
		Art_Side_Animation = Load_Alloc_Data(FsalgShpFile, ArtSideAnimationAllocated); // RA2: as above
		Art_Button_Corner = Load_Alloc_Data(FsbclgShpFile, ArtButtonCornerAllocated); // RA2: as above
		Art_Side_Strip = Load_Alloc_Data(FsslgShpFile, ArtSideStripAllocated); // RA2: as above
		Art_Button_Back = Load_Alloc_Data(FsbkgdlgShpFile, ArtButtonBackAllocated); // RA2: as above
	}
}

/// <summary>
/// RA2: loads the palette of the side animation.
/// </summary>
void __cdecl Load_Side_Animation_Palette(void) // RA2: 0x006F2EB0
{
	Load_Art_Palette(FsscrnPalFile, &SideAnimationPalette, &SideAnimationDrawer); // RA2: as above
}

/// <summary>
/// RA2: frees the shapes of the side animation.
/// </summary>
void __fastcall Free_Side_Animation_Shapes(void) // RA2: 0x006F2ED0
{
	if (ArtSideAnimationAllocated) { // RA2: as above
		operator delete(Art_Side_Animation); // RA2: as above
		ArtSideAnimationAllocated = false; // RA2: as above
	}
	Art_Side_Animation = NULL; // RA2: as above
	if (ArtButtonCornerAllocated) { // RA2: as above
		operator delete(Art_Button_Corner); // RA2: as above
		ArtButtonCornerAllocated = false; // RA2: as above
	}
	Art_Button_Corner = NULL; // RA2: as above
	if (ArtSideStripAllocated) { // RA2: as above
		operator delete(Art_Side_Strip); // RA2: as above
		ArtSideStripAllocated = false; // RA2: as above
	}
	Art_Side_Strip = NULL; // RA2: as above
	if (ArtButtonBackAllocated) { // RA2: as above
		operator delete(Art_Button_Back); // RA2: as above
		ArtButtonBackAllocated = false; // RA2: as above
	}
	Art_Button_Back = NULL; // RA2: as above
}

/// <summary>
/// RA2: frees the palette of the side animation.
/// </summary>
void __fastcall Free_Side_Animation_Palette(void) // RA2: 0x006F2F70
{
	if (SideAnimationPalette != NULL) { // RA2: as above
		operator delete(SideAnimationPalette); // RA2: as above
		SideAnimationPalette = NULL; // RA2: as above
	}
	if (SideAnimationDrawer != NULL) { // RA2: as above
		delete SideAnimationDrawer; // RA2: as above
		SideAnimationDrawer = NULL; // RA2: as above
	}
}

/// <summary>
/// RA2: loads the dialog frame's art, the palettes of the shell and lays the frame out for the screen, unless
/// that is done already.
/// </summary>
void __fastcall Load_Interface_Art(void) // RA2: 0x006F2FB0
{
	if (!InterfaceArtLoaded) { // RA2: as above
		Load_Neutral_Mix(); // RA2: as above
		Load_Interface_Shapes(); // RA2: as above
		Load_Art_Palette(ShellPalFile, &ShellPalette, &ShellDrawer); // RA2: as above
		Load_Art_Palette(Shell2PalFile, &Shell2Palette, &Shell2Drawer); // RA2: as above
		Load_Art_Palette(SdbtnanmPalFile, &ButtonAnimPalette, &ButtonAnimDrawer); // RA2: as above
		Layout_Interface_Art(VideoModeWidth, VideoModeHeight); // RA2: as above
		InterfaceArtLoaded = true; // RA2: as above
	}
}

/// <summary>
/// RA2: frees the dialog frame's art and the palettes of the shell, when they are loaded.
/// </summary>
void __fastcall Free_Interface_Art(void) // RA2: 0x006F30F0
{
	if (InterfaceArtLoaded) { // RA2: as above
		InterfaceArtLoaded = false; // RA2: as above
		Free_Interface_Shapes(); // RA2: as above
		Free_Interface_Palettes(); // RA2: as above
		Free_Interface_Rects(); // RA2: as above
		Release_Neutral_Mix(); // RA2: as above
	}
}

/// <summary>
/// RA2: frees the dialog frame's art and loads it again, as at the end of a game.
/// </summary>
void __fastcall Reload_Art_Set(void) // RA2: 0x006F31B0
{
	Free_Interface_Art(); // RA2: as above
	Load_Interface_Art(); // RA2: as above
}

/// <summary>
/// RA2: lays the dialog frame out again for a screen size.
/// </summary>
void __fastcall Relayout_Interface_Art(int width, int height) // RA2: 0x006F33B0
{
	Free_Interface_Rects(); // RA2: as above
	Layout_Interface_Art(width, height); // RA2: as above
}

bool __fastcall Is_Interface_Art_Loaded(void) // RA2: 0x006F3460
{
	return InterfaceArtLoaded; // RA2: as above
}

PaletteClass * __cdecl Get_Shell_Palette(void) // RA2: 0x006F3470
{
	return ShellPalette; // RA2: as above
}

void * __fastcall Get_Drawer(void) // RA2: 0x006F3480
{
	return ShellDrawer; // RA2: as above
}

PaletteClass * __cdecl Get_Shell2_Palette(void) // RA2: 0x006F3490
{
	return Shell2Palette; // RA2: as above
}

void * __fastcall Get_Listed_Drawer(void) // RA2: 0x006F34A0
{
	return Shell2Drawer; // RA2: as above
}

PaletteClass * __cdecl Get_Button_Anim_Palette(void) // RA2: 0x006F34B0
{
	return ButtonAnimPalette; // RA2: as above
}

ConvertClass * __cdecl Get_Button_Anim_Drawer(void) // RA2: 0x006F34C0
{
	return ButtonAnimDrawer; // RA2: as above
}

/// <summary>
/// RA2: paints black over the screen outside the dialog frame when the screen is larger than 800 by 600.
/// </summary>
void __fastcall Draw_Interface_Borders(Surface * surface, Rect const & window) // RA2: 0x006F34D0
{
	if (VideoModeWidth >= 1024) { // RA2: as above
		int left = DialogBackRect->X; // RA2: as above
		int right = SlideTopRect->X + SlideTopRect->Width; // RA2: as above
		int top = DialogBackRect->Y; // RA2: as above
		int bottom = LowerScreenRect->Height + LowerScreenRect->Y; // RA2: as above
		Rect rect(0, 0, left, top); // RA2: as above
		surface->Fill_Rect(rect, window, 0); // RA2: as above
		rect = Rect(left, 0, 800, top); // RA2: as above
		surface->Fill_Rect(rect, window, 0); // RA2: as above
		rect = Rect(right, 0, left, top); // RA2: as above
		surface->Fill_Rect(rect, window, 0); // RA2: as above
		rect = Rect(right, top, left, 600); // RA2: as above
		surface->Fill_Rect(rect, window, 0); // RA2: as above
		rect = Rect(right, bottom, left, 600); // RA2: as above
		surface->Fill_Rect(rect, window, 0); // RA2: as above
		rect = Rect(left, bottom, 800, top); // RA2: as above
		surface->Fill_Rect(rect, window, 0); // RA2: as above
		rect = Rect(0, bottom, left, top); // RA2: as above
		surface->Fill_Rect(rect, window, 0); // RA2: as above
		rect = Rect(0, top, left, 600); // RA2: as above
		surface->Fill_Rect(rect, window, 0); // RA2: as above
	}
}

/// <summary>
/// RA2: draws the dialog frame's strips and buttons.
/// </summary>
void __fastcall Draw_Interface_Frame(Surface * surface, Rect const & window, bool arg3) // RA2: 0x006F3650
{
	Load_Interface_Art(); // RA2: loads the art once; the nested expansion there leaves the shapes to a call
	Draw_Interface_Borders(surface, window); // RA2: as above
	Point2D point(0, 0); // RA2: as above
	Rect clip(window.X, window.Y, window.Width, window.Height); // RA2: as above
	if (clip.Width > 800) { // RA2: as above
		clip.Width = (clip.Width - 800) / 2 + 800; // RA2: as above
	}
	if (clip.Height > 600) { // RA2: as above
		clip.Height = (clip.Height - 600) / 2 + 600; // RA2: as above
	}
	point.X = SlideTopRect->X; // RA2: as above
	point.Y = SlideTopRect->Y; // RA2: as above
	Draw_Shape(*surface, *ShellDrawer, ArtSlideTop, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	int index = 0; // RA2: as above
	point.X = Button_Strip_Top->X; // RA2: as above
	point.Y = Button_Strip_Top->Y; // RA2: as above
	if (SlideButtonCount > 0) { // RA2: as above
		do {
			Draw_Shape(*surface, *Shell2Drawer, ArtSlideButtonBack, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
			point.Y += Button_Strip_Top->Height; // RA2: as above
			index++; // RA2: as above
		} while (index < SlideButtonCount); // RA2: as above
	}
	if (!arg3) { // RA2: as above
		index = 0; // RA2: as above
		point.X = SlideButtonRect->X; // RA2: as above
		point.Y = SlideButtonRect->Y; // RA2: as above
		if (SlideButtonCount > 0) { // RA2: as above
			do {
				Draw_Shape(*surface, *ButtonAnimDrawer, Slide_Button_Shape, 10, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
				point.Y += SlideButtonRect->Height; // RA2: as above
				index++; // RA2: as above
			} while (index < SlideButtonCount); // RA2: as above
		}
	}
	point.X = Button_Strip_Bottom->X; // RA2: as above
	point.Y = Button_Strip_Bottom->Y; // RA2: as above
	Draw_Shape(*surface, *ShellDrawer, ArtSlideBottom, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	point.X = LowerScreenRect->X; // RA2: as above
	point.Y = LowerScreenRect->Y; // RA2: as above
	if (VideoModeWidth == 640) { // RA2: as above
		Draw_Shape(*surface, *ShellDrawer, ArtLowerScreen640, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	} else {
		Draw_Shape(*surface, *ShellDrawer, ArtLowerScreen, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	}
}

/// <summary>
/// RA2: draws the dialog's backdrop.
/// </summary>
void __fastcall Draw_Dialog_Backdrop(Surface * surface, Rect const & window, ConvertClass * drawer, ShapeSet * shape640, ShapeSet * shape) // RA2: 0x006F3930
{
	Point2D point(0, 0); // RA2: as above
	Rect clip(window.X, window.Y, window.Width, window.Height); // RA2: as above
	if (clip.Width > 800) { // RA2: as above
		clip.Width = (clip.Width - 800) / 2 + 800; // RA2: as above
	}
	if (clip.Height > 600) { // RA2: as above
		clip.Height = (clip.Height - 600) / 2 + 600; // RA2: as above
	}
	point.X = DialogBackRect->X; // RA2: as above
	point.Y = DialogBackRect->Y; // RA2: as above
	if (VideoModeWidth == 640) { // RA2: as above
		Draw_Shape(*surface, *drawer, shape640, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	} else {
		Draw_Shape(*surface, *drawer, shape, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	}
}

/// <summary>
/// RA2: draws the dialog frame and the dialog's backdrop.
/// </summary>
void __fastcall Draw_Interface_Screen(Surface * surface, Rect const & rect, bool arg3) // RA2: 0x006F3A20
{
	Draw_Interface_Frame(surface, rect, arg3); // RA2: as above
	Draw_Dialog_Backdrop(surface, rect, (ConvertClass *)Get_Drawer(), Art_Dialog_Back640, Art_Dialog_Back); // RA2: as above
}

/// <summary>
/// RA2: draws the shape of the multiplayer button of the slide, at its place.
/// </summary>
void __fastcall Draw_Slide_MP_Button(Surface * surface, Rect const & window) // RA2: 0x006F3A60
{
	Point2D point(0, 0); // RA2: as above
	point.X = SlideMPButtonRect->X; // RA2: as above
	point.Y = SlideMPButtonRect->Y; // RA2: as above
	Draw_Shape(*surface, *ShellDrawer, ArtSlideMPButton, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
}

/// <summary>
/// RA2: draws the second frame of the slide's top shape, at its place.
/// </summary>
void __fastcall Draw_Slide_Top(Surface * surface, Rect const & window) // RA2: 0x006F3AC0
{
	Point2D point(0, 0); // RA2: as above
	point.X = SlideTopRect->X; // RA2: as above
	point.Y = SlideTopRect->Y; // RA2: as above
	Draw_Shape(*surface, *ShellDrawer, ArtSlideTop, 1, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
}

/// <summary>
/// RA2: draws the top of the score screens, at the dialog's place, moved right on wide screens.
/// </summary>
void __fastcall Draw_Score_Top(Surface * surface, Rect const & window) // RA2: 0x006F3B20
{
	Point2D point(0, 0); // RA2: as above
	Point2D * source = (Point2D *)DialogBackRect; // RA2: as above
	point.X = source->X; // RA2: as above
	point.Y = source->Y; // RA2: as above
	if (VideoModeWidth >= 800) { // RA2: as above
		point.X += 80; // RA2: as above
	}
	Draw_Shape(*surface, *ScoreDrawer, (ShapeSet *)ArtScoreTop, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
}

/// <summary>
/// RA2: draws the score screens' backdrop, at the dialog's place.
/// </summary>
void __fastcall Draw_Score_Back(Surface * surface, Rect const & window) // RA2: 0x006F3B90
{
	Point2D point(0, 0); // RA2: as above
	point.X = DialogBackRect->X; // RA2: as above
	point.Y = DialogBackRect->Y; // RA2: as above
	Draw_Shape(*surface, *ScoreDrawer, Art_Chat_Back, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
}

/// <summary>
/// RA2: draws the dialog frame and the score screens' backdrop.
/// </summary>
void __fastcall Draw_Score_Screen(Surface * surface, Rect const & window) // RA2: 0x006F3BF0
{
	Draw_Interface_Borders(surface, window); // RA2: as above
	Draw_Interface_Frame(surface, window, false); // RA2: as above
	Point2D point(0, 0); // RA2: as above
	point.X = DialogBackRect->X; // RA2: as above
	point.Y = DialogBackRect->Y; // RA2: as above
	Draw_Shape(*surface, *ScoreDrawer, Art_Chat_Back, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
}

/// <summary>
/// RA2: draws the multiplayer score screen's backdrop, at the dialog's place.
/// </summary>
void __fastcall Draw_MP_Score_Back(Surface * surface, Rect const & window) // RA2: 0x006F3C70
{
	Point2D point(0, 0); // RA2: as above
	point.X = DialogBackRect->X; // RA2: as above
	point.Y = DialogBackRect->Y; // RA2: as above
	Draw_Shape(*surface, *MPScoreDrawer, Art_Score_Screen, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
}

/// <summary>
/// RA2: draws the dialog frame and the multiplayer score screen's backdrop.
/// </summary>
void __fastcall Draw_MP_Score_Screen(Surface * surface, Rect const & window) // RA2: 0x006F3CD0
{
	Draw_Interface_Borders(surface, window); // RA2: as above
	Draw_Interface_Frame(surface, window, false); // RA2: as above
	Point2D point(0, 0); // RA2: as above
	point.X = DialogBackRect->X; // RA2: as above
	point.Y = DialogBackRect->Y; // RA2: as above
	Draw_Shape(*surface, *MPScoreDrawer, Art_Score_Screen, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
}

/// <summary>
/// RA2: loads the shapes of the dialog frame.
/// </summary>
void __fastcall Load_Interface_Shapes(void) // RA2: 0x006F3D50
{
	Slide_Button_Shape = (ShapeSet *)Load_Alloc_Data(SdbtnanmShpFile, SlideButtonAllocated); // RA2: as above
	ArtSlideMPButton = (ShapeSet *)Load_Alloc_Data(SdmpbtnShpFile, ArtSlideMPButtonAllocated); // RA2: as above
	ArtSlideWarning = (ShapeSet *)Load_Alloc_Data(SdwrntmpShpFile, ArtSlideWarningAllocated); // RA2: as above
	Art_Dialog_Back640 = (ShapeSet *)Load_Alloc_Data(MnscrnsShpFile, ArtDialogBack640Allocated); // RA2: as above
	Art_Dialog_Back = (ShapeSet *)Load_Alloc_Data(MnscrnlShpFile, ArtDialogBackAllocated); // RA2: as above
	ArtSlideTop = (ShapeSet *)Load_Alloc_Data(SdtpShpFile, ArtSlideTopAllocated); // RA2: as above
	ArtSlideButtonBack = (ShapeSet *)Load_Alloc_Data(SdbtnbkgdShpFile, ArtSlideButtonBackAllocated); // RA2: as above
	ArtSlideBottom = (ShapeSet *)Load_Alloc_Data(SdbtmShpFile, ArtSlideBottomAllocated); // RA2: as above
	ArtLowerScreen640 = (ShapeSet *)Load_Alloc_Data(LwscrnsShpFile, ArtLowerScreen640Allocated); // RA2: as above
	ArtLowerScreen = (ShapeSet *)Load_Alloc_Data(LwscrnlShpFile, ArtLowerScreenAllocated); // RA2: as above
}

/// <summary>
/// RA2: loads the palettes of the shell.
/// </summary>
void __fastcall Load_Interface_Palettes(void) // RA2: 0x006F3E30
{
	Load_Art_Palette(ShellPalFile, &ShellPalette, &ShellDrawer); // RA2: as above
	Load_Art_Palette(Shell2PalFile, &Shell2Palette, &Shell2Drawer); // RA2: as above
	Load_Art_Palette(SdbtnanmPalFile, &ButtonAnimPalette, &ButtonAnimDrawer); // RA2: as above
}

/// <summary>
/// RA2: works out where the parts of the dialog frame go on a screen of this size: the frame is 800 by 600 and
/// centred on larger screens.
/// </summary>
void __fastcall Layout_Interface_Art(int width, int height) // RA2: 0x006F3E70
{
	int left = 0; // RA2: as above
	int right = width; // RA2: as above
	if (width >= 1024) { // RA2: as above
		left = (width - 800) / 2; // RA2: as above
		right = width - left; // RA2: as above
	}
	int top = 0; // RA2: as above
	int bottom = height; // RA2: as above
	int span = height; // RA2: as above
	if (height >= 768) { // RA2: as above
		top = (height - 600) / 2; // RA2: as above
		bottom = height - top; // RA2: as above
		span = height - top * 2; // RA2: as above
	}
	bool small_width = width == 640; // RA2: as above
	bool small_height = height == 480; // RA2: as above

	int w = small_width ? Art_Dialog_Back640->Get_Width() : Art_Dialog_Back->Get_Width(); // RA2: as above
	int h = small_height ? Art_Dialog_Back640->Get_Height() : Art_Dialog_Back->Get_Height(); // RA2: as above
	DialogBackRect = new Rect(left, top, w, h); // RA2: as above

	int tw = ArtSlideTop->Get_Width(); // RA2: as above
	int th = ArtSlideTop->Get_Height(); // RA2: as above
	int x = right - tw; // RA2: as above
	SlideTopRect = new Rect(x, top, tw, th); // RA2: as above

	int wh = ArtSlideWarning->Get_Height(); // RA2: as above
	SlideWarningRect = new Rect(x, top, tw, wh); // RA2: as above

	int by = SlideTopRect->Height + top; // RA2: as above
	int bh = ArtSlideButtonBack->Get_Height(); // RA2: as above
	Button_Strip_Top = new Rect(x, by, tw, bh); // RA2: as above

	int aw = Slide_Button_Shape->Get_Width(); // RA2: as above
	int ah = Slide_Button_Shape->Get_Height(); // RA2: as above
	int ax = Button_Strip_Top->Width - aw + Button_Strip_Top->X; // RA2: as above
	int ay = Button_Strip_Top->Y; // RA2: as above
	SlideButtonRect = new Rect(ax, ay, aw, ah); // RA2: as above

	int mw = ArtSlideMPButton->Get_Width(); // RA2: as above
	int mh = ArtSlideMPButton->Get_Height(); // RA2: as above
	int my = Button_Strip_Top->Height + Button_Strip_Top->Y - mh; // RA2: as above
	SlideMPButtonRect = new Rect(right - mw, my, mw, mh); // RA2: as above

	SlideButtonCount = (span - SlideTopRect->Height) / Button_Strip_Top->Height; // RA2: as above
	int offset = Button_Strip_Top->Height * SlideButtonCount; // RA2: as above
	int used = SlideTopRect->Height + offset; // RA2: as above
	int ly = Button_Strip_Top->Y + offset; // RA2: as above
	int lw = ArtSlideBottom->Get_Width(); // RA2: as above
	Button_Strip_Bottom = new Rect(right - lw, ly, lw, span - used); // RA2: as above

	int sw = small_width ? ArtLowerScreen640->Get_Width() : ArtLowerScreen->Get_Width(); // RA2: as above
	int sh = small_height ? ArtLowerScreen640->Get_Height() : ArtLowerScreen->Get_Height(); // RA2: as above
	LowerScreenRect = new Rect(left, bottom - sh, sw, sh); // RA2: as above
}

/// <summary>
/// RA2: frees the shapes of the dialog frame.
/// </summary>
void __fastcall Free_Interface_Shapes(void) // RA2: 0x006F4160
{
	if (SlideButtonAllocated) { // RA2: as above
		operator delete(Slide_Button_Shape); // RA2: as above
		SlideButtonAllocated = false; // RA2: as above
	}
	Slide_Button_Shape = NULL; // RA2: as above
	if (ArtSlideMPButtonAllocated) { // RA2: as above
		operator delete(ArtSlideMPButton); // RA2: as above
		ArtSlideMPButtonAllocated = false; // RA2: as above
	}
	ArtSlideMPButton = NULL; // RA2: as above
	if (ArtSlideWarningAllocated) { // RA2: as above
		operator delete(ArtSlideWarning); // RA2: as above
		ArtSlideWarningAllocated = false; // RA2: as above
	}
	ArtSlideWarning = NULL; // RA2: as above
	if (ArtDialogBack640Allocated) { // RA2: as above
		operator delete(Art_Dialog_Back640); // RA2: as above
		ArtDialogBack640Allocated = false; // RA2: as above
	}
	Art_Dialog_Back640 = NULL; // RA2: as above
	if (ArtDialogBackAllocated) { // RA2: as above
		operator delete(Art_Dialog_Back); // RA2: as above
		ArtDialogBackAllocated = false; // RA2: as above
	}
	Art_Dialog_Back = NULL; // RA2: as above
	if (ArtSlideTopAllocated) { // RA2: as above
		operator delete(ArtSlideTop); // RA2: as above
		ArtSlideTopAllocated = false; // RA2: as above
	}
	ArtSlideTop = NULL; // RA2: as above
	if (ArtSlideButtonBackAllocated) { // RA2: as above
		operator delete(ArtSlideButtonBack); // RA2: as above
		ArtSlideButtonBackAllocated = false; // RA2: as above
	}
	ArtSlideButtonBack = NULL; // RA2: as above
	if (ArtSlideBottomAllocated) { // RA2: as above
		operator delete(ArtSlideBottom); // RA2: as above
		ArtSlideBottomAllocated = false; // RA2: as above
	}
	ArtSlideBottom = NULL; // RA2: as above
	if (ArtLowerScreen640Allocated) { // RA2: as above
		operator delete(ArtLowerScreen640); // RA2: as above
		ArtLowerScreen640Allocated = false; // RA2: as above
	}
	ArtLowerScreen640 = NULL; // RA2: as above
	if (ArtLowerScreenAllocated) { // RA2: as above
		operator delete(ArtLowerScreen); // RA2: as above
		ArtLowerScreenAllocated = false; // RA2: as above
	}
	ArtLowerScreen = NULL; // RA2: as above
}

/// <summary>
/// RA2: frees the palettes of the shell.
/// </summary>
void __cdecl Free_Interface_Palettes(void) // RA2: 0x006F42D0
{
	if (ShellPalette != NULL) { // RA2: as above
		operator delete(ShellPalette); // RA2: as above
		ShellPalette = NULL; // RA2: as above
	}
	if (ShellDrawer != NULL) { // RA2: as above
		delete ShellDrawer; // RA2: as above
		ShellDrawer = NULL; // RA2: as above
	}
	if (Shell2Palette != NULL) { // RA2: as above
		operator delete(Shell2Palette); // RA2: as above
		Shell2Palette = NULL; // RA2: as above
	}
	if (Shell2Drawer != NULL) { // RA2: as above
		delete Shell2Drawer; // RA2: as above
		Shell2Drawer = NULL; // RA2: as above
	}
	if (ButtonAnimPalette != NULL) { // RA2: as above
		operator delete(ButtonAnimPalette); // RA2: as above
		ButtonAnimPalette = NULL; // RA2: as above
	}
	if (ButtonAnimDrawer != NULL) { // RA2: as above
		delete ButtonAnimDrawer; // RA2: as above
		ButtonAnimDrawer = NULL; // RA2: as above
	}
}

/// <summary>
/// RA2: frees the areas the dialog frame was laid out in.
/// </summary>
void __fastcall Free_Interface_Rects(void) // RA2: 0x006F4360
{
	operator delete(SlideButtonRect); // RA2: as above
	SlideButtonRect = NULL; // RA2: as above
	operator delete(SlideMPButtonRect); // RA2: as above
	SlideMPButtonRect = NULL; // RA2: as above
	operator delete(SlideWarningRect); // RA2: as above
	SlideWarningRect = NULL; // RA2: as above
	operator delete(DialogBackRect); // RA2: as above
	DialogBackRect = NULL; // RA2: as above
	operator delete(SlideTopRect); // RA2: as above
	SlideTopRect = NULL; // RA2: as above
	operator delete(Button_Strip_Top); // RA2: as above
	Button_Strip_Top = NULL; // RA2: as above
	operator delete(Button_Strip_Bottom); // RA2: as above
	Button_Strip_Bottom = NULL; // RA2: as above
	operator delete(LowerScreenRect); // RA2: as above
	LowerScreenRect = NULL; // RA2: as above
}

/// <summary>
/// RA2: loads the sidebar's art and lays it out for the screen.
/// </summary>
void __fastcall Load_Sidebar_Art(void) // RA2: 0x006F4400
{
	Load_Sidebar_Shapes(); // RA2: as above
	Load_Sidebar_Palettes(); // RA2: as above
	Layout_Sidebar_Art(VideoModeWidth, VideoModeHeight); // RA2: as above
	SidebarArtLoaded = true; // RA2: as above
}

/// <summary>
/// RA2: frees the sidebar's art.
/// </summary>
void __fastcall Free_Sidebar_Art(void) // RA2: 0x006F4450
{
	Free_Sidebar_Shapes(); // RA2: as above
	if (SidebarArtPalette != NULL) { // RA2: as above
		operator delete(SidebarArtPalette); // RA2: as above
		SidebarArtPalette = NULL; // RA2: as above
	}
	if (SidebarArtDrawer != NULL) { // RA2: as above
		delete SidebarArtDrawer; // RA2: as above
		SidebarArtDrawer = NULL; // RA2: as above
	}
	if (UIBackgroundPalette != NULL) { // RA2: as above
		operator delete(UIBackgroundPalette); // RA2: as above
		UIBackgroundPalette = NULL; // RA2: as above
	}
	if (UIBackgroundDrawer != NULL) { // RA2: as above
		delete UIBackgroundDrawer; // RA2: as above
		UIBackgroundDrawer = NULL; // RA2: as above
	}
	Free_Sidebar_Rects(); // RA2: as above
	SidebarArtLoaded = false; // RA2: as above
}

/// <summary>
/// RA2: frees the sidebar's art and loads it again.
/// </summary>
void __fastcall Reload_Sidebar_Art(void) // RA2: 0x006F44D0
{
	Free_Sidebar_Art(); // RA2: as above
	Load_Sidebar_Shapes(); // RA2: as above
	Load_Art_Palette(SidebarPalFile, &SidebarArtPalette, &SidebarArtDrawer); // RA2: as above
	Load_Art_Palette(UibkgdPalFile, &UIBackgroundPalette, &UIBackgroundDrawer); // RA2: as above
	Layout_Sidebar_Art(VideoModeWidth, VideoModeHeight); // RA2: as above
	SidebarArtLoaded = true; // RA2: as above
}

bool __fastcall Is_Sidebar_Art_Loaded(void) // RA2: 0x006F4590
{
	return SidebarArtLoaded; // RA2: as above
}

/// <summary>
/// RA2: sets the outline colour of the tooltips for a side.
/// </summary>
void __fastcall Select_Tooltip_Color(int which) // RA2: 0x006F45A0
{
	if (which == 0) { // RA2: as above
		TooltipColor = TooltipColorSide0; // RA2: as above
		return; // RA2: as above
	}
	TooltipColor = TooltipColorSide1; // RA2: as above
}

PaletteClass * __fastcall Get_Game_Palette(void) // RA2: 0x006F45E0
{
	return SidebarArtPalette; // RA2: as above
}

ConvertClass * __cdecl Get_Sidebar_Art_Drawer(void) // RA2: 0x006F45F0
{
	return SidebarArtDrawer; // RA2: as above
}

PaletteClass * __cdecl Get_UI_Background_Palette(void) // RA2: 0x006F4600
{
	return UIBackgroundPalette; // RA2: as above
}

ConvertClass * __cdecl Get_UI_Background_Drawer(void) // RA2: 0x006F4610
{
	return UIBackgroundDrawer; // RA2: as above
}

/// <summary>
/// RA2: draws the sidebar's art on a surface: its backdrop, the radar's frame, the strips of buttons and the tab bar.
/// </summary>
void __fastcall Draw_Sidebar_Art(Surface * surface, Rect const & window, bool tabs) // RA2: 0x006F4620
{
	if (!SidebarArtLoaded) { // RA2: as above
		Load_Sidebar_Shapes(); // RA2: as above
		Load_Sidebar_Palettes(); // RA2: as above
		Layout_Sidebar_Art(VideoModeWidth, VideoModeHeight); // RA2: as above
		SidebarArtLoaded = true; // RA2: as above
	}
	surface->Fill(0); // RA2: as above
	Point2D point(0, 0); // RA2: as above
	point.X = SidebarBackRect->X; // RA2: as above
	point.Y = SidebarBackRect->Y; // RA2: as above
	if (VideoModeWidth == 640) { // RA2: as above
		Draw_Shape(*surface, *UIBackgroundDrawer, ArtSidebarBack640, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	} else if (VideoModeWidth == 800) { // RA2: as above
		Draw_Shape(*surface, *UIBackgroundDrawer, ArtSidebarBack800, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	} else {
		Draw_Shape(*surface, *UIBackgroundDrawer, ArtSidebarBack1024, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	}
	point.X = TabBackRect->X; // RA2: as above
	point.Y = TabBackRect->Y; // RA2: as above
	Draw_Shape(*surface, *SidebarArtDrawer, TabBackShape, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	point.X = RadarFrameRect->X; // RA2: as above
	point.Y = RadarFrameRect->Y; // RA2: as above
	Draw_Shape(*surface, *SidebarArtDrawer, RadarFrameShape, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	point.X = RadarAnimRect->X; // RA2: as above
	point.Y = RadarAnimRect->Y; // RA2: as above
	Draw_Shape(*surface, *SidebarArtDrawer, RadarAnimShape, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	point.X = Side1Rect->X; // RA2: as above
	point.Y = Side1Rect->Y; // RA2: as above
	Draw_Shape(*surface, *SidebarArtDrawer, ArtSide1, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	int index = 0; // RA2: as above
	point.X = Free_Button_Strip->X; // RA2: as above
	point.Y = Free_Button_Strip->Y; // RA2: as above
	if (SidebarStripCount > 0) { // RA2: as above
		do {
			Draw_Shape(*surface, *SidebarArtDrawer, ArtSide2b, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
			point.Y += Free_Button_Strip->Height; // RA2: as above
			index++; // RA2: as above
		} while (index < SidebarStripCount); // RA2: as above
	}
	point.X = Default_Button_Y->X; // RA2: as above
	point.Y = Default_Button_Y->Y; // RA2: as above
	Draw_Shape(*surface, *SidebarArtDrawer, ArtSide3, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	point.X = Default_Button_Strip->X; // RA2: as above
	point.Y = Default_Button_Strip->Y; // RA2: as above
	Draw_Shape(*surface, *SidebarArtDrawer, ArtAddon, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	Rect clip = window; // RA2: as above
	clip.Width = Get_Title_Screen_Area().Width; // RA2: the tab bar is as wide as the area the tactical view uses
	point.X = TabLeftRect->X; // RA2: as above
	point.Y = TabLeftRect->Y; // RA2: as above
	Draw_Shape(*surface, *SidebarArtDrawer, TabLeftShape, 0, point, clip, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	if (tabs) { // RA2: as above
		point.X = ThumbOpenRect->X; // RA2: as above
		point.Y = ThumbOpenRect->Y; // RA2: as above
		Draw_Shape(*surface, *SidebarArtDrawer, TabThumbShape, 2, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
		index = 0; // RA2: as above
		point.X = ButtonSlotRect->X; // RA2: as above
		point.Y = ButtonSlotRect->Y; // RA2: as above
		if (TabTileCount > 0) { // RA2: as above
			do {
				Draw_Shape(*surface, *SidebarArtDrawer, TabTileShape, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
				point.X += ButtonSlotRect->Width; // RA2: as above
				index++; // RA2: as above
			} while (index < TabTileCount); // RA2: as above
		}
	} else {
		point.X = ThumbClosedRect->X; // RA2: as above
		point.Y = ThumbClosedRect->Y; // RA2: as above
		Draw_Shape(*surface, *SidebarArtDrawer, TabThumbShape, 2, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
	}
	point.X = TabEndRect->X; // RA2: as above
	point.Y = TabEndRect->Y; // RA2: as above
	Draw_Shape(*surface, *SidebarArtDrawer, TabRightShape, 0, point, window, ShapeFlags_Type(SHAPE_WIN_REL), 0, 0, ZGradientType(0), 1000, 0, 0, Point2D(0, 0)); // RA2: as above
}

/// <summary>
/// RA2: loads the shapes of the sidebar.
/// </summary>
void __fastcall Load_Sidebar_Shapes(void) // RA2: 0x006F4B00
{
	Button_Strip_Shape = (ShapeSet *)Load_Alloc_Data(SidebttnShpFile, ButtonStripShapeAllocated); // RA2: as above
	ArtSidebarBack640 = (ShapeSet *)Load_Alloc_Data(BkgdsmShpFile, ArtSidebarBack640Allocated); // RA2: as above
	ArtSidebarBack800 = (ShapeSet *)Load_Alloc_Data(BkgdmdShpFile, ArtSidebarBack800Allocated); // RA2: as above
	ArtSidebarBack1024 = (ShapeSet *)Load_Alloc_Data(BkgdlgShpFile, ArtSidebarBack1024Allocated); // RA2: as above
	TabBackShape = (ShapeSet *)Load_Alloc_Data(CreditsShpFile, TabBackShapeAllocated); // RA2: as above
	RadarFrameShape = (ShapeSet *)Load_Alloc_Data(TopShpFile, RadarFrameShapeAllocated); // RA2: as above
	RadarAnimShape = (ShapeSet *)Load_Alloc_Data(RadarShpFile, RadarAnimShapeAllocated); // RA2: as above
	ArtSide1 = (ShapeSet *)Load_Alloc_Data(Side1ShpFile, ArtSide1Allocated); // RA2: as above
	ArtSide2 = (ShapeSet *)Load_Alloc_Data(Side2ShpFile, ArtSide2Allocated); // RA2: as above
	ArtSide2b = (ShapeSet *)Load_Alloc_Data(Side2bShpFile, ArtSide2bAllocated); // RA2: as above
	ArtSide3 = (ShapeSet *)Load_Alloc_Data(Side3ShpFile, ArtSide3Allocated); // RA2: as above
	ArtAddon = (ShapeSet *)Load_Alloc_Data(AddonShpFile, ArtAddonAllocated); // RA2: as above
	TabLeftShape = (ShapeSet *)Load_Alloc_Data(LspacerShpFile, TabLeftShapeAllocated); // RA2: as above
	TabThumbShape = (ShapeSet *)Load_Alloc_Data(LendcapShpFile, TabThumbShapeAllocated); // RA2: as above
	TabTileShape = (ShapeSet *)Load_Alloc_Data(BttnbkgdShpFile, TabTileShapeAllocated); // RA2: as above
	TabRightShape = (ShapeSet *)Load_Alloc_Data(RendcapShpFile, TabRightShapeAllocated); // RA2: as above
}

/// <summary>
/// RA2: loads the palettes of the sidebar.
/// </summary>
void __fastcall Load_Sidebar_Palettes(void) // RA2: 0x006F4C60
{
	Load_Art_Palette(SidebarPalFile, &SidebarArtPalette, &SidebarArtDrawer); // RA2: as above
	Load_Art_Palette(UibkgdPalFile, &UIBackgroundPalette, &UIBackgroundDrawer); // RA2: as above
}

/// <summary>
/// RA2: works out where the parts of the sidebar and of the tab bar go on a screen of this size.
/// </summary>
void __fastcall Layout_Sidebar_Art(int width, int height) // RA2: 0x006F4C90
{
	ShapeSet * backdrop; // RA2: as above
	if (VideoModeWidth == 640) { // RA2: as above
		backdrop = ArtSidebarBack640; // RA2: as above
	} else if (VideoModeWidth == 800) { // RA2: as above
		backdrop = ArtSidebarBack800; // RA2: as above
	} else {
		backdrop = ArtSidebarBack1024; // RA2: as above
	}
	int w = backdrop->Get_Width(); // RA2: as above
	int h = backdrop->Get_Height(); // RA2: as above
	SidebarBackRect = new Rect(0, 0, w, h); // RA2: as above

	w = TabBackShape->Get_Width(); // RA2: as above
	h = TabBackShape->Get_Height(); // RA2: as above
	int x = width - w; // RA2: as above
	TabBackRect = new Rect(x, 0, w, h); // RA2: as above
	int y = h; // RA2: as above
	h = RadarFrameShape->Get_Height(); // RA2: as above
	RadarFrameRect = new Rect(x, y, w, h); // RA2: as above
	y += h; // RA2: as above
	h = RadarAnimShape->Get_Height(); // RA2: as above
	RadarAnimRect = new Rect(x, y, w, h); // RA2: as above
	y += h; // RA2: as above
	h = ArtSide1->Get_Height(); // RA2: as above
	Side1Rect = new Rect(x, y, w, h); // RA2: as above
	y += h; // RA2: as above
	h = ArtSide2->Get_Height(); // RA2: as above
	Free_Button_Strip = new Rect(x, y, w, h); // RA2: as above
	h = ArtSide3->Get_Height(); // RA2: as above
	Default_Button_Y = new Rect(x, y, w, h); // RA2: as above
	h = ArtAddon->Get_Height(); // RA2: as above
	Default_Button_Strip = new Rect(x, y, w, h); // RA2: as above

	SidebarStripCount = (height - Default_Button_Y->Height - Side1Rect->Height - RadarAnimRect->Height
		- RadarFrameRect->Height - TabBackRect->Height) / Free_Button_Strip->Height; // RA2: as above
	y += SidebarStripCount * Free_Button_Strip->Height; // RA2: as above
	Default_Button_Y->Y = y; // RA2: as above
	Default_Button_Strip->Y = Default_Button_Y->Height + y; // RA2: as above

	y = height - 32; // RA2: as above
	int bar_width = Get_Title_Screen_Area().Width; // RA2: as above
	TabBarRect = new Rect(0, y, bar_width, 32); // RA2: as above
	w = TabLeftShape->Get_Width(); // RA2: as above
	h = TabLeftShape->Get_Height(); // RA2: as above
	TabLeftRect = new Rect(0, y, w, h); // RA2: as above
	w = TabThumbShape->Get_Width(); // RA2: as above
	h = TabThumbShape->Get_Height(); // RA2: as above
	ThumbOpenRect = new Rect(0, y, w, h); // RA2: as above
	ThumbClosedRect = new Rect(0, y, w, h); // RA2: as above
	w = TabTileShape->Get_Width(); // RA2: as above
	h = TabTileShape->Get_Height(); // RA2: as above
	ButtonSlotRect = new Rect(0, y, w, h); // RA2: as above
	w = TabRightShape->Get_Width(); // RA2: as above
	h = TabRightShape->Get_Height(); // RA2: as above
	TabEndRect = new Rect(0, y, w, h); // RA2: as above

	TabTileCount = (width - ThumbOpenRect->Width - TabBackRect->Width - TabEndRect->Width)
		/ ButtonSlotRect->Width; // RA2: as above
	int bar_x = TabBackRect->X - TabEndRect->Width; // RA2: as above
	TabEndRect->X = bar_x; // RA2: as above
	bar_x -= ButtonSlotRect->Width * TabTileCount; // RA2: as above
	ButtonSlotRect->X = bar_x; // RA2: as above
	bar_x -= ThumbOpenRect->Width; // RA2: as above
	ThumbOpenRect->X = bar_x; // RA2: as above
	ThumbOpenX = bar_x; // RA2: as above
	int tab_x = TabEndRect->X - ThumbClosedRect->Width; // RA2: as above
	ThumbClosedRect->X = tab_x; // RA2: as above
	ThumbClosedX = tab_x; // RA2: as above
	ThumbTravel = TabEndRect->X - ThumbOpenRect->Width - ThumbOpenRect->X; // RA2: as above

	int column_x = RadarFrameRect->X; // RA2: as above
	int column_y = RadarFrameRect->Y; // RA2: as above
	int column_width = RadarFrameRect->Width; // RA2: as above
	int column_height = RadarAnimRect->Height + RadarFrameRect->Height; // RA2: as above
	RadarAreaRect = new Rect(column_x, column_y, column_width, column_height); // RA2: as above
	int strip_x = Side1Rect->X; // RA2: as above
	int strip_y = Side1Rect->Y; // RA2: as above
	int strip_width = Side1Rect->Width; // RA2: as above
	int strip_height = SidebarStripCount * Free_Button_Strip->Height + Default_Button_Y->Height + Side1Rect->Height; // RA2: as above
	SidebarButtonsRect = new Rect(strip_x, strip_y, strip_width, strip_height); // RA2: as above
}

/// <summary>
/// RA2: frees the shapes of the sidebar.
/// </summary>
void __fastcall Free_Sidebar_Shapes(void) // RA2: 0x006F5150
{
	if (ButtonStripShapeAllocated) { // RA2: as above
		delete Button_Strip_Shape; // RA2: as above
		ButtonStripShapeAllocated = false; // RA2: as above
	}
	Button_Strip_Shape = NULL; // RA2: as above
	if (ArtSidebarBack640Allocated) { // RA2: as above
		delete ArtSidebarBack640; // RA2: as above
		ArtSidebarBack640Allocated = false; // RA2: as above
	}
	ArtSidebarBack640 = NULL; // RA2: as above
	if (ArtSidebarBack800Allocated) { // RA2: as above
		delete ArtSidebarBack800; // RA2: as above
		ArtSidebarBack800Allocated = false; // RA2: as above
	}
	ArtSidebarBack800 = NULL; // RA2: as above
	if (ArtSidebarBack1024Allocated) { // RA2: as above
		delete ArtSidebarBack1024; // RA2: as above
		ArtSidebarBack1024Allocated = false; // RA2: as above
	}
	ArtSidebarBack1024 = NULL; // RA2: as above
	if (TabBackShapeAllocated) { // RA2: as above
		delete (ShapeSet *)TabBackShape; // RA2: as above
		TabBackShapeAllocated = false; // RA2: as above
	}
	TabBackShape = NULL; // RA2: as above
	if (RadarFrameShapeAllocated) { // RA2: as above
		delete (ShapeSet *)RadarFrameShape; // RA2: as above
		RadarFrameShapeAllocated = false; // RA2: as above
	}
	RadarFrameShape = NULL; // RA2: as above
	if (RadarAnimShapeAllocated) { // RA2: as above
		delete (ShapeSet *)RadarAnimShape; // RA2: as above
		RadarAnimShapeAllocated = false; // RA2: as above
	}
	RadarAnimShape = NULL; // RA2: as above
	if (ArtSide1Allocated) { // RA2: as above
		delete ArtSide1; // RA2: as above
		ArtSide1Allocated = false; // RA2: as above
	}
	ArtSide1 = NULL; // RA2: as above
	if (ArtSide2Allocated) { // RA2: as above
		delete ArtSide2; // RA2: as above
		ArtSide2Allocated = false; // RA2: as above
	}
	ArtSide2 = NULL; // RA2: as above
	if (ArtSide2bAllocated) { // RA2: as above
		delete ArtSide2b; // RA2: as above
		ArtSide2bAllocated = false; // RA2: as above
	}
	ArtSide2b = NULL; // RA2: as above
	if (ArtSide3Allocated) { // RA2: as above
		delete ArtSide3; // RA2: as above
		ArtSide3Allocated = false; // RA2: as above
	}
	ArtSide3 = NULL; // RA2: as above
	if (ArtAddonAllocated) { // RA2: as above
		delete ArtAddon; // RA2: as above
		ArtAddonAllocated = false; // RA2: as above
	}
	ArtAddon = NULL; // RA2: as above
	if (TabLeftShapeAllocated) { // RA2: as above
		delete (ShapeSet *)TabLeftShape; // RA2: as above
		TabLeftShapeAllocated = false; // RA2: as above
	}
	TabLeftShape = NULL; // RA2: as above
	if (TabThumbShapeAllocated) { // RA2: as above
		delete (ShapeSet *)TabThumbShape; // RA2: as above
		TabThumbShapeAllocated = false; // RA2: as above
	}
	TabThumbShape = NULL; // RA2: as above
	if (TabTileShapeAllocated) { // RA2: as above
		delete (ShapeSet *)TabTileShape; // RA2: as above
		TabTileShapeAllocated = false; // RA2: as above
	}
	TabTileShape = NULL; // RA2: as above
	if (TabRightShapeAllocated) { // RA2: as above
		delete (ShapeSet *)TabRightShape; // RA2: as above
		TabRightShapeAllocated = false; // RA2: as above
	}
	TabRightShape = NULL; // RA2: as above
}

/// <summary>
/// RA2: frees the sidebar's palettes, when they are loaded.
/// </summary>
void __fastcall Free_Sidebar_Palettes(void) // RA2: 0x006F5390
{
	if (SidebarArtPalette != NULL) { // RA2: as above
		delete SidebarArtPalette; // RA2: as above
		SidebarArtPalette = NULL; // RA2: as above
	}
	if (SidebarArtDrawer != NULL) { // RA2: as above
		delete SidebarArtDrawer; // RA2: as above
		SidebarArtDrawer = NULL; // RA2: as above
	}
	if (UIBackgroundPalette != NULL) { // RA2: as above
		delete UIBackgroundPalette; // RA2: as above
		UIBackgroundPalette = NULL; // RA2: as above
	}
	if (UIBackgroundDrawer != NULL) { // RA2: as above
		delete UIBackgroundDrawer; // RA2: as above
		UIBackgroundDrawer = NULL; // RA2: as above
	}
}

/// <summary>
/// RA2: frees the areas the sidebar and the tab bar were laid out in.
/// </summary>
void __fastcall Free_Sidebar_Rects(void) // RA2: 0x006F5400
{
	operator delete(SidebarBackRect); // RA2: as above
	SidebarBackRect = NULL; // RA2: as above
	operator delete(TabBackRect); // RA2: as above
	TabBackRect = NULL; // RA2: as above
	operator delete(RadarFrameRect); // RA2: as above
	RadarFrameRect = NULL; // RA2: as above
	operator delete(RadarAnimRect); // RA2: as above
	RadarAnimRect = NULL; // RA2: as above
	operator delete(RadarAreaRect); // RA2: as above
	RadarAreaRect = NULL; // RA2: as above
	operator delete(Side1Rect); // RA2: as above
	Side1Rect = NULL; // RA2: as above
	operator delete(Free_Button_Strip); // RA2: as above
	Free_Button_Strip = NULL; // RA2: as above
	operator delete(Default_Button_Y); // RA2: as above
	Default_Button_Y = NULL; // RA2: as above
	operator delete(Default_Button_Strip); // RA2: as above
	Default_Button_Strip = NULL; // RA2: as above
	operator delete(SidebarButtonsRect); // RA2: as above
	SidebarButtonsRect = NULL; // RA2: as above
	operator delete(TabBarRect); // RA2: as above
	TabBarRect = NULL; // RA2: as above
	operator delete(TabLeftRect); // RA2: as above
	TabLeftRect = NULL; // RA2: as above
	operator delete(ThumbOpenRect); // RA2: as above
	ThumbOpenRect = NULL; // RA2: as above
	operator delete(ThumbClosedRect); // RA2: as above
	ThumbClosedRect = NULL; // RA2: as above
	operator delete(ButtonSlotRect); // RA2: as above
	ButtonSlotRect = NULL; // RA2: as above
	operator delete(TabEndRect); // RA2: as above
	TabEndRect = NULL; // RA2: as above
}

/// <summary>
/// RA2: frees the sidebar's shapes and loads them again.
/// </summary>
void __fastcall Reload_Sidebar_Shapes(void) // RA2: 0x006F5530
{
	Free_Sidebar_Shapes(); // RA2: as above
	Load_Sidebar_Shapes(); // RA2: as above
}

/// <summary>
/// RA2: frees the sidebar's palettes and loads them again.
/// </summary>
void __fastcall Reload_Sidebar_Palettes(void) // RA2: 0x006F5540
{
	if (SidebarArtPalette != NULL) { // RA2: as above
		operator delete(SidebarArtPalette); // RA2: as above
		SidebarArtPalette = NULL; // RA2: as above
	}
	if (SidebarArtDrawer != NULL) { // RA2: as above
		delete SidebarArtDrawer; // RA2: as above
		SidebarArtDrawer = NULL; // RA2: as above
	}
	if (UIBackgroundPalette != NULL) { // RA2: as above
		operator delete(UIBackgroundPalette); // RA2: as above
		UIBackgroundPalette = NULL; // RA2: as above
	}
	if (UIBackgroundDrawer != NULL) { // RA2: as above
		delete UIBackgroundDrawer; // RA2: as above
		UIBackgroundDrawer = NULL; // RA2: as above
	}
	Load_Art_Palette(SidebarPalFile, &SidebarArtPalette, &SidebarArtDrawer); // RA2: as above
	Load_Art_Palette(UibkgdPalFile, &UIBackgroundPalette, &UIBackgroundDrawer); // RA2: as above
}

/// <summary>
/// RA2: lays the sidebar out again for a screen size.
/// </summary>
void __fastcall Relayout_Sidebar_Art(int width, int height) // RA2: 0x006F55D0
{
	Free_Sidebar_Rects(); // RA2: as above
	Layout_Sidebar_Art(width, height); // RA2: as above
}
