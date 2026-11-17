// void Unknown_Load_MP_Score_Art_006F2A30(int side) (0x006F2A30)
// Loads the multiplayer score screen backdrop (small or large by screen
// width) and its palette once: MPASCRN for the Allies, MPSSCRN when `side`
// is nonzero (hypothesis); the backdrop load is 0x006F2B50, expanded inline.
class PaletteClass;
class ConvertClass;

void const *__fastcall Unknown_Load_Shape_00495270(char const *filename, bool &allocated);
void __fastcall Unknown_Load_MP_Score_Backdrop_006F2B50(int alternate);
void __fastcall Unknown_Load_Palette_006F1FA0(char const *name, PaletteClass **palette, ConvertClass **drawer);
extern bool unknown_flag_ac1174;
extern int VideoModeWidth;                 // screen width
extern bool unknown_flag_ac1217;
extern void const *unknown_shape_ac1104;
extern char const * const unknown_name_7f8780;   // MPASCRNS.SHP
extern char const * const unknown_name_7f8784;   // MPASCRNL.SHP
extern char const * const unknown_name_7f8788;   // MPSSCRNS.SHP
extern char const * const unknown_name_7f878c;   // MPSSCRNL.SHP
extern char const * const unknown_name_7f8710;   // MPASCRN.PAL
extern char const * const unknown_name_7f8714;   // MPSSCRN.PAL
extern PaletteClass *unknown_palette_ac116c;
extern ConvertClass *unknown_drawer_ac1170;

void __fastcall Unknown_Load_MP_Score_Art_006F2A30(int side)
{
    if (unknown_flag_ac1174) {
        return;
    }
    Unknown_Load_MP_Score_Backdrop_006F2B50(side);
    Unknown_Load_Palette_006F1FA0(side == 0 ? unknown_name_7f8710 : unknown_name_7f8714, &unknown_palette_ac116c, &unknown_drawer_ac1170);
    unknown_flag_ac1174 = true;
}
