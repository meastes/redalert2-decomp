// void Unknown_Load_Dialog_Palettes_006F22C0() -- 0x006F22C0
// Loads the shell art pairs (shape and companion data) named at 0x007F86E8 and 0x007F86EC (hypothesis).
void __fastcall unknown_load_art_6f1fa0(char const *name, void **shape, void **data);
extern char const * const unknown_name_7f86e8;
extern char const * const unknown_name_7f86ec;
extern void *unknown_art_ac1130;
extern void *unknown_art_ac1134;
extern void *unknown_art_ac1138;
extern void *unknown_art_ac113c;
void __fastcall Unknown_Load_Dialog_Palettes_006F22C0()
{
    unknown_load_art_6f1fa0(unknown_name_7f86e8, &unknown_art_ac1130, &unknown_art_ac1134);
    unknown_load_art_6f1fa0(unknown_name_7f86ec, &unknown_art_ac1138, &unknown_art_ac113c);
}
