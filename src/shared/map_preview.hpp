// The scenario's map preview (unit 0x0061D240; its header statics start there):
// a pointer-sized object made by 0x0061D430 and freed by 0x0061D440, filled
// from the map by 0x0061D660 and written as the [Preview]/[PreviewPack]
// sections by 0x0061DC80 (Write_Scenario_INI). TibSun preview.h's MapPreviewClass; mischa85's names.csv
// names the class and members at these addresses.
#ifndef RA2_SHARED_MAP_PREVIEW_HPP
#define RA2_SHARED_MAP_PREVIEW_HPP

class CCINIClass;
class XSurface;
#ifdef MAP_PREVIEW_DECLARE_INTERNALS
class Cell;
struct Point2D;
#endif
typedef struct HWND__ *HWND;
class MapPreviewClass {
public:
    MapPreviewClass();                      // 0x0061D430 (clears the pointer)
    ~MapPreviewClass();                     // 0x0061D440 (deletes the object it holds)
    void unknown_005c7ef0(); // original ECX preview receiver/RET0; ignored result
    void Create_Preview();                        // 0x0061D660 (renders the preview from the map)
    bool Write_INI(CCINIClass &ini);       // 0x0061DC80 (true when a preview was stored)
    void Blit_Preview(HWND window);                // 0x0061D460 (paints the preview into the dialog)
    bool Read_PCX_Preview(char const *name);           // 0x0061E170 (loads a preview image file; the lobby passes RandMap.img)
    bool Read_INI_Preview(char const *filename);    // 0x0061E2A0 (makes the preview from a scenario file; ModemHost 0x00599920)
    unsigned short *Create_Paletted_Preview(int unknown, int *size);  // 0x0061E490 (the preview image and its size; ModemHost 0x0059C8B0)
    void Create_Preview_Surface(unsigned short const *image);    // 0x0061E930 (makes the preview from a decompressed image; ModemGuest 0x005989B0)
    bool Is_Present() const { return unknown_00 != 0; }   // inline (Write_Scenario_INI)
#ifdef MAP_PREVIEW_DECLARE_INTERNALS
    // Reads the [Preview] size and the [PreviewPack] block (0x0061DED0; TibSun Read_INI).
    bool Read_INI(CCINIClass const &ini);
    // The preview pixel of a cell (0x0061D5E0; TibSun Cell_To_Preview_Pixel; no callers).
    Point2D Cell_To_Preview_Pixel(Cell cell) const;
#endif

    XSurface *unknown_00;                           // +0x00 the preview surface (TibSun SurfacePtr)
};
// The loaded scenario's preview (0x00A6FEE4).
extern MapPreviewClass *unknown_preview_a6fee4;
// The map preview the skirmish and multiplayer setup dialogs draw (0x00A72CE8).
extern MapPreviewClass *unknown_dialog_preview_a72ce8;

#endif
