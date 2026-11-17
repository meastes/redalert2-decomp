// void Unknown_Free_Art_Flagged_006F2750() -- 0x006F2750
// Frees the flagged buffers and the art pair 0x006F2710 loaded (hypothesis).

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
void __fastcall Unknown_Load_Art_Flagged_006F2840(bool alternate);
extern bool unknown_flag_ac1168;
extern bool unknown_flag_ac1214;
extern bool unknown_flag_ac1215;
extern bool unknown_flag_ac1216;
extern void *unknown_art_ac110c;
extern void *unknown_art_ac10f0;
extern void *unknown_art_ac1108;
extern void *unknown_art_ac1160;
extern UnknownArtData_006f2390 *unknown_art_ac1164;

void Unknown_Free_Art_Flagged_006F2750()
{
    if (unknown_flag_ac1168) {
        unknown_flag_ac1168 = false;
        if (unknown_flag_ac1214) {
            operator delete(unknown_art_ac110c);
            unknown_flag_ac1214 = false;
        }
        unknown_art_ac110c = 0;
        if (unknown_flag_ac1215) {
            operator delete(unknown_art_ac10f0);
            unknown_flag_ac1215 = false;
        }
        unknown_art_ac10f0 = 0;
        if (unknown_flag_ac1216) {
            operator delete(unknown_art_ac1108);
            unknown_flag_ac1216 = false;
        }
        unknown_art_ac1108 = 0;
        if (unknown_art_ac1160 != 0) {
            operator delete(unknown_art_ac1160);
            unknown_art_ac1160 = 0;
        }
        if (unknown_art_ac1164 != 0) {
            delete unknown_art_ac1164;
            unknown_art_ac1164 = 0;
        }
    }
}
