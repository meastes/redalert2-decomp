// void Unknown_Release_Shell_Palettes_6F42D0() -- 0x006F42D0
// Releases the three art pairs 0x006F3E30 loads (hypothesis).

class UnknownArtData_006f2390 {
public:
    virtual ~UnknownArtData_006f2390();
};
extern void *unknown_art_ac1184;
extern UnknownArtData_006f2390 *unknown_art_ac1188;
extern void *unknown_art_ac118c;
extern UnknownArtData_006f2390 *unknown_art_ac1190;
extern void *unknown_art_ac1194;
extern UnknownArtData_006f2390 *unknown_art_ac1198;

void Unknown_Release_Shell_Palettes_6F42D0()
{
    if (unknown_art_ac1184 != 0) {
        operator delete(unknown_art_ac1184);
        unknown_art_ac1184 = 0;
    }
    if (unknown_art_ac1188 != 0) {
        delete unknown_art_ac1188;
        unknown_art_ac1188 = 0;
    }
    if (unknown_art_ac118c != 0) {
        operator delete(unknown_art_ac118c);
        unknown_art_ac118c = 0;
    }
    if (unknown_art_ac1190 != 0) {
        delete unknown_art_ac1190;
        unknown_art_ac1190 = 0;
    }
    if (unknown_art_ac1194 != 0) {
        operator delete(unknown_art_ac1194);
        unknown_art_ac1194 = 0;
    }
    if (unknown_art_ac1198 != 0) {
        delete unknown_art_ac1198;
        unknown_art_ac1198 = 0;
    }
}
