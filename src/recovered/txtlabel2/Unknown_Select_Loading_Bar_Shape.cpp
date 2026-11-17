// void *__fastcall Unknown_Select_Loading_Bar_Shape_6f2420(int width, int unused, int unused2) -- 0x006f2420
// Chooses between two art data pointers by the screen width (640 selects the first;
// hypothesis: the low-resolution variant).
extern void *unknown_art_7f8738;
extern void *unknown_art_7f873c;

void *__fastcall Unknown_Select_Loading_Bar_Shape_6f2420(int width, int unused, int unused2)
{
    void *art = unknown_art_7f8738;
    if (width != 0x280) {
        art = unknown_art_7f873c;
    }
    return art;
}
