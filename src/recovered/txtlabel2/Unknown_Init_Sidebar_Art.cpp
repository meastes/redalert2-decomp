// void Unknown_Init_Sidebar_Art_006F4400() (0x006F4400)
// Loads the sidebar art unconditionally: the art buffers (0x006F4B00), the
// SIDEBAR/UIBKGD palettes of 0x006F4C60 expanded inline and the layout for the
// screen size (0x006F4C90), then marks the art loaded (the block that
// 0x006F4620 runs when the flag is clear).

void __fastcall Unknown_Load_Art_Buffers_006F4B00();
void __fastcall Unknown_Load_Art_Pairs_006F4C60();
void __fastcall Unknown_Layout_Sidebar_006F4C90(int width, int height);
extern bool unknown_flag_ac11b0;
extern int VideoModeWidth;                 // screen width
extern int VideoModeHeight;                 // screen height

void __fastcall Unknown_Init_Sidebar_Art_006F4400()
{
    Unknown_Load_Art_Buffers_006F4B00();
    Unknown_Load_Art_Pairs_006F4C60();
    Unknown_Layout_Sidebar_006F4C90(VideoModeWidth, VideoModeHeight);
    unknown_flag_ac11b0 = true;
}
