// void __fastcall Unknown_Load_Art_Flagged_006F2840(int alternate) -- 0x006F2840
// Loads three flagged buffers, chosen by the argument and by the screen width (640 selects the first of each pair; hypothesis).

class UnknownArtData_006f2390 {
public:
    virtual ~UnknownArtData_006f2390();
};
struct UnknownArtOut {
    UnknownArtOut(void **pointer) : Pointer(pointer) {}
    void **Pointer;
};
void __fastcall unknown_load_art_6f1fa0(UnknownArtOut data, char const *name, void **shape);
void *__fastcall Unknown_00495270(char const *name, bool *allocated);
extern int VideoModeWidth;
extern bool unknown_flag_ac1214;
extern bool unknown_flag_ac1215;
extern bool unknown_flag_ac1216;
extern void *unknown_art_ac110c;
extern void *unknown_art_ac10f0;
extern void *unknown_art_ac1108;
extern char const * const unknown_name_7f8750;
extern char const * const unknown_name_7f8754;
extern char const * const unknown_name_7f8758;
extern char const * const unknown_name_7f875c;
extern char const * const unknown_name_7f8760;
extern char const * const unknown_name_7f8764;
extern char const * const unknown_name_7f8768;
extern char const * const unknown_name_7f876c;
extern char const * const unknown_name_7f8770;
extern char const * const unknown_name_7f8774;
extern char const * const unknown_name_7f8778;
extern char const * const unknown_name_7f877c;

void __fastcall Unknown_Load_Art_Flagged_006F2840(int alternate)
{
    if (!alternate) {
        if (VideoModeWidth == 0x280) {
            unknown_art_ac110c = Unknown_00495270(unknown_name_7f8750, &unknown_flag_ac1214);
            unknown_art_ac10f0 = Unknown_00495270(unknown_name_7f8758, &unknown_flag_ac1215);
            unknown_art_ac1108 = Unknown_00495270(unknown_name_7f8760, &unknown_flag_ac1216);
        } else {
            unknown_art_ac110c = Unknown_00495270(unknown_name_7f8754, &unknown_flag_ac1214);
            unknown_art_ac10f0 = Unknown_00495270(unknown_name_7f875c, &unknown_flag_ac1215);
            unknown_art_ac1108 = Unknown_00495270(unknown_name_7f8764, &unknown_flag_ac1216);
        }
    } else {
        if (VideoModeWidth == 0x280) {
            unknown_art_ac110c = Unknown_00495270(unknown_name_7f8768, &unknown_flag_ac1214);
            unknown_art_ac10f0 = Unknown_00495270(unknown_name_7f8770, &unknown_flag_ac1215);
            unknown_art_ac1108 = Unknown_00495270(unknown_name_7f8778, &unknown_flag_ac1216);
        } else {
            unknown_art_ac110c = Unknown_00495270(unknown_name_7f876c, &unknown_flag_ac1214);
            unknown_art_ac10f0 = Unknown_00495270(unknown_name_7f8774, &unknown_flag_ac1215);
            unknown_art_ac1108 = Unknown_00495270(unknown_name_7f877c, &unknown_flag_ac1216);
        }
    }
}
