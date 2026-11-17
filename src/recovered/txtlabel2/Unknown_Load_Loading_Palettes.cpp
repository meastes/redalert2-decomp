// void Unknown_Load_Loading_Palettes_006F24A0(int side) (0x006F24A0)
// Loads the loading-screen palettes once each: LDSCRN.PAL, the side's
// LDSCRNA.PAL or LDSCRNS.PAL, and SPLDBR.PAL (hypothesis: `side` nonzero
// selects the Soviet palette).
class PaletteClass;
class ConvertClass;

void __fastcall Unknown_Load_Palette_006F1FA0(char const *name, PaletteClass **palette, ConvertClass **drawer);
extern char const * const unknown_name_7f86f0;   // LDSCRN.PAL
extern char const * const unknown_name_7f86f4;   // LDSCRNA.PAL
extern char const * const unknown_name_7f86f8;   // LDSCRNS.PAL
extern char const * const unknown_name_7f86fc;   // SPLDBR.PAL
extern PaletteClass *unknown_palette_ac1140;
extern ConvertClass *unknown_drawer_ac1144;
extern PaletteClass *unknown_palette_ac1148;
extern ConvertClass *unknown_drawer_ac114c;
extern PaletteClass *unknown_palette_ac1150;
extern ConvertClass *unknown_drawer_ac1154;

void __fastcall Unknown_Load_Loading_Palettes_006F24A0(int side)
{
    if (unknown_drawer_ac1144 == 0) {
        Unknown_Load_Palette_006F1FA0(unknown_name_7f86f0, &unknown_palette_ac1140, &unknown_drawer_ac1144);
    }
    if (unknown_drawer_ac114c == 0) {
        if (side == 0) {
            Unknown_Load_Palette_006F1FA0(unknown_name_7f86f4, &unknown_palette_ac1148, &unknown_drawer_ac114c);
        } else {
            Unknown_Load_Palette_006F1FA0(unknown_name_7f86f8, &unknown_palette_ac1148, &unknown_drawer_ac114c);
        }
    }
    if (unknown_drawer_ac1154 == 0) {
        Unknown_Load_Palette_006F1FA0(unknown_name_7f86fc, &unknown_palette_ac1150, &unknown_drawer_ac1154);
    }
}
