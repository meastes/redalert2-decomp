// score.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/score.json carries the addresses and the rows that
// stay in their own files.


#define RA2_VECTOR_UNSIGNED_INDEX  // migration holdout: unit not yet re-rolled to VectorClass::operator[](int) (docs/HANDOFF.md)
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/score.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/dsurface.hpp"
#include "shared/convert.hpp"
#include "shared/hicolor.hpp"
#include "shared/house.hpp"
#include "shared/house_type.hpp"
#include "shared/id_source.hpp"
#include "shared/keyboard.hpp"
#include "shared/mouse.hpp"
#include "shared/ms_engine.hpp"
#include "shared/palette.hpp"
#include "shared/play_movie.hpp"
#include "shared/rgb_class.hpp"
#include "shared/sound_available.hpp"
#include "shared/string_table.hpp"
#include "shared/theme.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/crt_string.hpp"
#include "shared/crt_memory.hpp"
#define FETCH(label, line) Fetch_Text(label, "D:\\Ra2\\Score.CPP", line)
#define PRINT(string, xp, yp, font) \
    Add_Anim(obj = new ScorePrintClass(string, xp, yp, font, false))
#define WAIT() Wait_For_Print(obj)
#define LOAD(var, name) \
    Open_Named(file, name); \
    ShapeFileStruct const *var = (ShapeFileStruct const *)Load_Alloc_Data(file); \
    file.Close();
#define WIPE(shape, xp, yp) \
    CC_Draw_Shape(unknown_surface_839954, Drawer, shape, i, Point2D(xp, yp), unknown_surface_839954->Get_Rect(), 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0))
#include "shared/comm_buffer.hpp"
#include "shared/queue.hpp"
#include "shared/serial_progress.hpp"
#include "shared/debug_printf.hpp"
#include "shared/win32_api.hpp"
#include "shared/win32_window.hpp"
#include "shared/score_anim.hpp"
#define SHAPE_FILE_EMPTY_RECT unknown_score_empty_rect_ab6f20
#include "shared/score_font.hpp"
#include "shared/surface.hpp"
#include "shared/msfont.hpp"


// Header-static dynamic initializers of the unit at 0x00663DF0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x00663DF0-0x00663FD1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00663E10, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_00663df0.cpp; no source of its own.


// Dynamic initializer (0x00663E40, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_00663df0.cpp; no source of its own.


// Dynamic initializer (0x00663E60, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_00663df0.cpp; no source of its own.


// Dynamic initializer (0x00663E80, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_00663df0.cpp; no source of its own.


// Dynamic initializer (0x00663EA0, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_00663df0.cpp; no source of its own.


// Dynamic initializer (0x00663EC0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_00663df0.cpp; no source of its own.


// Dynamic initializer (0x00663EE0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_00663df0.cpp; no source of its own.


// Dynamic initializer (0x00663F20, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_00663df0.cpp; no source of its own.


// Dynamic initializer (0x00663F50, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_00663df0.cpp; no source of its own.


// Dynamic initializer (0x00663F80, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_00663df0.cpp; no source of its own.


// Dynamic initializer (0x00663FB0, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_00663df0.cpp; no source of its own.


// Dynamic initializer (0x00663FC0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_00663df0.cpp; no source of its own.


// void ScoreClass::Presentation() (0x00663FE0, Score.CPP): the campaign score screen.

void *__fastcall Load_Alloc_Data(class FileClass &file);
void __fastcall Load_Title_Screen_00738190(char const *name, Surface *surface, PaletteClass *palette);
void unknown_481fb0();

extern "C" char *__cdecl _strdup(char const *text);
inline ScoreSoundClass::ScoreSoundClass(char const *name, char const *filename) :
    Name(0), unknown_04(0), unknown_08(false)
{
    if (unknown_check_406f30()) {
        Name = _strdup(name);
    }
}

inline ScoreSoundClass::~ScoreSoundClass()
{
    if (Name) {
        free(Name);
    }
}

// The file classes' open by name: names the file, then opens it (virtual calls).
static inline int Open_Named(FileClass &file, char const *name, int rights = FILE_RIGHTS_READ)
{
    file.Set_Name(name);
    return file.Open(rights);
}

// The string table entry for a label, typed as the score fonts take it.
static inline unsigned short const *Fetch_Text(char const *label, char const *file, int line)
{
    return (unsigned short const *)StringTable::Load_String(label, 0, file, line);
}

struct Fame {
    char name[0x68];
    int score;
    int level;
    int side;
};


void ScoreClass::Presentation()
{
    int i;
    int index;
    int total;
    int x, y;
    unsigned short const *str;
    ScoreAnimClass *obj;
    CCFileClass file;
    Fame hallfame[9];

    XOffset = (unknown_surface_839954->Get_Width() - 640) / 2;
    YOffset = (unknown_surface_839954->Get_Height() - 400) / 2;
    WWMouse->Hide_Mouse();
    WWKeyboard->Clear();
    Theme.Stop();
    Theme.Play_Song((ThemeType)Theme.From_Name("SCORE"));

    Open_Named(file, "SIDEFNT3.PAL");
    unsigned char *fontpal = (unsigned char *)Load_Alloc_Data(file);
    file.Close();
    for (i = 0; i < 256; i++) {
        unknown_palette_838710[i] = RGBClass(fontpal[i * 3] << 2, fontpal[i * 3 + 1] << 2, fontpal[i * 3 + 2] << 2);
    }
    Color = Build_Hicolor_Pixel(fontpal[204] << 2, fontpal[205] << 2, fontpal[206] << 2);
    delete fontpal;

    ConvertClass *drawer = new ConvertClass(unknown_palette_838710, unknown_palette_838710, unknown_surface_839950, 1, 0);
    SurfacePtr = new DSurface(unknown_surface_839954->Get_Width(), unknown_surface_839954->Get_Height(), false);
    SurfacePtr->Fill(0);
    Load_Title_Screen_00738190("SCORE.PCX", SurfacePtr, &unknown_palette_838710);

    LOAD(best01shape, "BEST01.SHP")
    LOAD(best02shape, "BEST02.SHP")
    unknown_481fb0();
    LOAD(logo01shape, "LOGO01.SHP")
    LOAD(efic01shape, "EFIC01.SHP")
    LOAD(time01shape, "TIME01.SHP")
    LOAD(casu01shape, "CASU01.SHP")
    unknown_481fb0();
    LOAD(casu02shape, "CASU02.SHP")
    LOAD(curr01shape, "CURR01.SHP")

    Sounds.Clear();
    Sounds.Add(new ScoreSoundClass("Wipe", "WIPE.AUD"));
    Sounds.Add(new ScoreSoundClass("Emblem", "EMBLEM.AUD"));
    Sounds.Add(new ScoreSoundClass("BestBox", "BESTBOX.AUD"));
    Sounds.Add(new ScoreSoundClass("Efficiency", "EFFICIEN.AUD"));
    Sounds.Add(new ScoreSoundClass("BarGraph", "BARGRAPH.AUD"));
    Sounds.Add(new ScoreSoundClass("Type", "TYPE.AUD"));
    Sounds.Add(new ScoreSoundClass("Back", "SCOLD8.AUD"));

    Font = new ScoreFullFontClass(drawer);
    unknown_font_10 = new ScoreBigFontClass(drawer);
    unknown_481fb0();
    Anims.Clear();
    WWKeyboard->Clear();
    Play_Sound("Wipe", 256);
    Play_Movie("SCORE.VQA", -1, false, false, true, false);
    unknown_surface_839954->Copy_From(*SurfacePtr, false, true);
    unknown_surface_839958->Copy_From(*SurfacePtr, false, true);

    Drawer = new ConvertClass(unknown_palette_838710, unknown_palette_838710, unknown_surface_839950, 1, 0);
    Add_Anim(new ScoreTimeClass(XOffset + 7, YOffset + 6, logo01shape, 60, 3, Drawer));

    Play_Sound("Efficiency", 128);
    for (i = 0; i < 10; i++) {
        WIPE(efic01shape, XOffset + 480, YOffset + 23);
        ScoreDelay(4);
    }
    Rect rect(XOffset + 480, YOffset + 23, 160, 120);
    SurfacePtr->Copy_From(rect, *unknown_surface_839954, rect, false, true);
    unknown_surface_839958->Copy_From(rect, *unknown_surface_839954, rect, false, true);
    str = FETCH("TXT_MISSION_EFFICIENCY", 506);
    Font->String_Pixel_Width(str);
    x = XOffset - Font->String_Pixel_Width(str) / 2 + 552;
    PRINT(str, x, YOffset + 13, Font);
    WAIT();
    WWKeyboard->Clear();

    total = Efficiency(unknown_player_a35db4);
    unsigned short dest[64];
    swprintf((wchar_t *)dest, L"%3d%%", total);
    PRINT(dest, XOffset + 520, YOffset + 60, unknown_font_10);
    WAIT();

    for (i = 0; i < 10; i++) {
        WIPE(curr01shape, XOffset + 481, YOffset + 148);
        ScoreDelay(4);
    }
    rect.Set(XOffset + 481, YOffset + 148, 144, 86);
    SurfacePtr->Copy_From(rect, *unknown_surface_839954, rect, false, true);
    unknown_surface_839958->Copy_From(rect, *unknown_surface_839954, rect, false, true);
    str = FETCH("TXT_CURRENCY", 543);
    Font->String_Pixel_Width(str);
    x = XOffset - Font->String_Pixel_Width(str) / 2 + 560;
    PRINT(str, x, YOffset + 193, Font);
    WAIT();
    rect.Set(XOffset + 481, YOffset + 148, 150, 120);
    SurfacePtr->Copy_From(rect, *unknown_surface_839954, rect, false, true);
    unknown_surface_839958->Copy_From(rect, *unknown_surface_839954, rect, false, true);
    Show_Credits();

    for (i = 0; i < 10; i++) {
        WIPE(time01shape, XOffset + 444, YOffset + 273);
        ScoreDelay(4);
    }
    rect.Set(XOffset + 444, YOffset + 273, 196, 118);
    SurfacePtr->Copy_From(rect, *unknown_surface_839954, rect, false, true);
    unknown_surface_839958->Copy_From(rect, *unknown_surface_839954, rect, false, true);
    str = FETCH("TXT_MISSION_TIME_LAPSE", 571);
    Font->String_Pixel_Width(str);
    x = XOffset - Font->String_Pixel_Width(str) / 2 + 542;
    PRINT(str, x, YOffset + 366, Font);
    WAIT();
    Print_Time(unknown_id_source_a3d290->ElapsedTime.Time() / 60);

    Play_Sound("Emblem", 256);
    for (i = 0; i < 10; i++) {
        WIPE(casu01shape, XOffset + 186, YOffset + 54);
        WIPE(casu02shape, XOffset + 186, YOffset + 254);
        ScoreDelay(4);
    }
    rect.Set(XOffset + 186, YOffset + 54, 262, 326);
    SurfacePtr->Copy_From(rect, *unknown_surface_839954, rect, false, true);
    unknown_surface_839958->Copy_From(rect, *unknown_surface_839954, rect, false, true);
    str = FETCH("TXT_CASUALTIES", 601);
    Font->String_Pixel_Width(str);
    x = XOffset - Font->String_Pixel_Width(str) / 2 + 316;
    PRINT(str, x, YOffset + 300, Font);
    WAIT();

    str = FETCH("TXT_STRUCTURES", 613);
    x = XOffset + 194;
    PRINT(str, x, YOffset + 247, Font);
    x = XOffset + 328;
    PRINT(str, x, YOffset + 247, Font);
    WAIT();
    str = FETCH("TXT_UNITS", 628);
    x = XOffset - Font->String_Pixel_Width(str) / 2 + 290;
    PRINT(str, x, YOffset + 247, Font);
    x = XOffset - Font->String_Pixel_Width(str) / 2 + 422;
    PRINT(str, x, YOffset + 247, Font);
    WAIT();
    Do_Graphs();
    while (unknown_score_print_ab6f9c) {
        ScoreDelay(1);
    }
    WWKeyboard->Clear();

    Play_Sound("BestBox", 256);
    for (i = 0; i < 10; i++) {
        WIPE(best01shape, XOffset, YOffset + 188);
        WIPE(best02shape, XOffset, YOffset + 388);
        ScoreDelay(4);
    }
    rect.Set(XOffset, YOffset + 188, 182, 212);
    SurfacePtr->Copy_From(rect, *unknown_surface_839954, rect, false, true);
    unknown_surface_839958->Copy_From(rect, *unknown_surface_839954, rect, false, true);
    str = FETCH("TXT_BEST_SCORES", 669);
    Font->String_Pixel_Width(str);
    x = XOffset - Font->String_Pixel_Width(str) / 2 + 84;
    PRINT(str, x, YOffset + 217, Font);

    memset(hallfame, 0, sizeof(hallfame));
    file.Close();
    file.Set_Name("HALLFAME.DAT");
    if (file.Is_Available() == true) {
        file.Read(hallfame, sizeof(hallfame));
        file.Close();
    }
    if (hallfame[8].score >= total) {
        hallfame[8].score = 0;
    }
    for (index = 0; index < 9; index++) {
        // Tests i, not index (as in the original): i is still 10 from the box wipe above.
        if (total > hallfame[i].score) {
            if (index < 8) {
                for (i = 8; i > index; i--) {
                    hallfame[i] = hallfame[i - 1];
                }
            }
            hallfame[index].name[0] = 0;
            hallfame[index].score = total;
            hallfame[index].level = unknown_id_source_a3d290->TechLevel;
            hallfame[index].side = unknown_player_a35db4->Type->Side;
            break;
        }
    }
    char maststr[9 * 32];
    for (i = 0; i < 9; i++) {
        PRINT(hallfame[i].name, XOffset + 7, YOffset + 242 + i * 16, Font);
        if (hallfame[i].score) {
            char *scorestr = maststr + i * 32;
            sprintf(scorestr, "%d%%", hallfame[i].score);
            PRINT(scorestr, XOffset + 109, YOffset + 242 + i * 16, Font);
            char *levelstr = scorestr + 16;
            sprintf(levelstr, "%02d", hallfame[i].level);
            PRINT(levelstr, XOffset + 151, YOffset + 242 + i * 16, Font);
            WAIT();
        }
        ScoreDelay(1);
    }
    while (unknown_score_print_ab6f9c) {
        ScoreDelay(1);
    }
    WWKeyboard->Clear();
    WWMouse->Show_Mouse();
    if (index < 9) {
        Input_Name(hallfame[index].name, XOffset + 7, YOffset + 242 + index * 16);
    } else {
        unsigned short const *str = FETCH("TXT_CLICK_CONTINUE", 749);
        x = XOffset + (640 - Font->String_Pixel_Width(str)) / 2;
        y = YOffset - Font->Height / 2 + 357;
        PRINT(str, x, y, Font);
        Cycle_Wait_Click(true);
    }
    WWKeyboard->Clear();
    if (Open_Named(file, "HALLFAME.DAT", FILE_RIGHTS_WRITE)) {
        file.Write(hallfame, sizeof(hallfame));
        file.Close();
    }
    Theme.Stop(true);

    for (i = 0; i < Sounds.Count(); i++) {
        if (Sounds[i]) {
            delete Sounds[i];
        }
    }
    Sounds.Clear();
    for (i = 0; i < Anims.Count(); i++) {
        if (Anims[i]) {
            delete Anims[i];
        }
    }
    Anims.Clear();
    if (Drawer != 0) {
        delete Drawer;
        Drawer = 0;
    }
    if (Font != 0) {
        delete Font;
        Font = 0;
    }
    if (unknown_font_10 != 0) {
        delete unknown_font_10;
        unknown_font_10 = 0;
    }
    if (SurfacePtr != 0) {
        delete SurfacePtr;
        SurfacePtr = 0;
    }
    delete drawer;
    unknown_surface_839954->Fill(0);
    Blit_Score();
    Theme.Queue_Song((ThemeType)-1);
}


// int ScoreClass::Add_Anim(ScoreAnimClass *anim) (0x00665E50)
// Lists a new animation for Update_Anims; nonzero when it was added.

int ScoreClass::Add_Anim(ScoreAnimClass *anim)
{
    return Anims.Add(anim);
}


// bool ScoreClass::Is_Anim_Done(ScoreAnimClass *anim) (0x00665EC0)
// True once Update_Anims has removed the (finished) animation.
bool ScoreClass::Is_Anim_Done(ScoreAnimClass *anim)
{
    bool done = Anims.ID(anim) == -1 ? true : false;
    return done;
}


// void ScoreClass::Cycle_Wait_Click(bool cycle) (0x00665EE0)
// Waits for a key or click (at least 20 delays), keeping a modem or
// null-modem link alive meanwhile (Westwood's Cycle_Wait_Click; cycle is
// unused).

void ScoreClass::Cycle_Wait_Click(bool cycle)
{
    int minclicks = 20;
    unsigned long timingtime = TickCount.Time();
    UnknownSerialPacket sendpacket;
    UnknownSerialPacket receivepacket;
    int packetlen;

    WWKeyboard->Clear();
    while (minclicks || !WWKeyboard->Check()) {
        if (unknown_game_mode_a3d298 == 2 || unknown_game_mode_a3d298 == 1) {
            if (TickCount.Time() - timingtime > 40) {
                memset(&sendpacket, 0, sizeof(sendpacket));
                sendpacket.Command = 0x6A;
                sendpacket.Timing.ResponseTime = serial_manager_00a40a28.Response_Time();
                sendpacket.ID = (unsigned char)serial_local_player_00a3fb38;
                serial_manager_00a40a28.Send_Message(&sendpacket, sizeof(sendpacket), 0);
                timingtime = TickCount.Time();
            }
            serial_manager_00a40a28.Get_Message(&receivepacket, &packetlen);
            serial_manager_00a40a28.Service();
        }
        ScoreDelay(1);
        if (minclicks) {
            minclicks--;
            WWKeyboard->Clear();
        }
    }
    WWKeyboard->Clear();
}


// Evaluate surviving assets and unspent income for the score screen, 0x00666040.


int ScoreClass::Efficiency(HouseClass *house)
{
    int remain = 0;
    int total = 0;
    for (int i = 0; i < Houses.Count(); i++) {
        HouseClass *other = Houses[i];
        if ((house->Is_Ally(other) && other->Is_Ally(house)) || house == other) {
            remain += other->unknown_5460.Sum();
            remain += other->unknown_5470.Sum();
            remain += other->unknown_5480.Sum();
            remain += other->unknown_5490.Sum();
            total += other->unknown_53b4;
            total += other->unknown_5360;
        }
    }
    total += remain;
    Debug_Printf("Remaining: %ld, Total: %ld\n", remain, total);
    float build = total > 0 ? (float)remain / total : 0.0f;
    Debug_Printf("BuildEconomy = %f\n", build);
    int const &harvested = house->unknown_220;
    Debug_Printf("Money: %ld, Harvested: %ld, Initial: %ld\n", house->Available_Money(), harvested, house->unknown_124);
    total = harvested + house->unknown_124;
    float money = total > 0 ? (float)house->Available_Money() / (float)total : 0.0f;
    Debug_Printf("MoneyEconomy = %f\n", money);
    int efficiency = (int)(100.0 * ((build + money) * 0.5));
    Debug_Printf("Efficiency: %ld\n", efficiency);
    return efficiency;
}


// void ScoreClass::Do_Graphs() (0x006661E0)
// Sums the units and buildings lost by the player's side (the player and
// mutual allies) and by everyone else, logs them, and draws the buildings
// graph at 227 and the units graph at 283, the player's side first unless
// the player's side index is non-zero.

void ScoreClass::Do_Graphs()
{
    int allyunits = 0;
    int allybuildings = 0;
    int enemyunits = 0;
    int enemybuildings = 0;
    for (int index = 0; index < Houses.Count(); index++) {
        HouseClass *house = Houses[index];
        Debug_Printf("Stats: %s - UnitsLost %ld, BuildingsLost %ld\n", (char const *)house->Type->ID,
                     house->unknown_5360, house->unknown_53b4);
        if ((unknown_player_a35db4->Is_Ally(house) && house->Is_Ally(unknown_player_a35db4)) || unknown_player_a35db4 == house) {
            allyunits += house->unknown_5360;
            allybuildings += house->unknown_53b4;
        } else {
            enemyunits += house->unknown_5360;
            enemybuildings += house->unknown_53b4;
        }
        Debug_Printf("AllyUnits %ld, AllyBuildings %ld, EnemyUnits %ld, EnemyBldgs %ld\n",
                     allyunits, allybuildings, enemyunits, enemybuildings);
    }
    if (unknown_player_a35db4->Type->Side == 0) {
        Do_Graph(allybuildings, enemybuildings, 227);
        Do_Graph(allyunits, enemyunits, 283);
    } else {
        Do_Graph(enemybuildings, allybuildings, 227);
        Do_Graph(enemyunits, allyunits, 283);
    }
}


// void ScoreClass::Do_Graph(int first, int second, int xpos) (0x00666320)
// Draws two translucent bars (gold for first, red for second) 140 pixels
// high at most, then grows them two pixels a tick with their counting
// values on top; the final bars and values also go to the background.

void ScoreClass::Do_Graph(int first, int second, int xpos)
{
    Rect firstrect(0, 0, 0, 0);
    Rect secondrect(0, 0, 0, 0);
    int max;
    xpos = XOffset + xpos;
    int bottom = YOffset + 244;
    int firstheight = first, secondheight = second;
    max = firstheight > secondheight ? firstheight : secondheight;
    if (!max) {
        max = 1;
    }
    firstheight = firstheight * 140 / max;
    secondheight = secondheight * 140 / max;
    if (max < 20) {
        firstheight = first * 5;
        secondheight = second * 5;
    }
    max = firstheight > secondheight ? firstheight : secondheight;
    if (!max) {
        max = 1;
    }
    Rect rect;
    rect.Set(xpos, bottom - firstheight, 4, firstheight);
    ((Surface *)SurfacePtr)->Fill_Rect_Trans(rect, RGBClass(0xFD, 0xB5, 0x1C), 25);
    rect.Set(xpos + 4, bottom - firstheight, 7, firstheight);
    ((Surface *)SurfacePtr)->Fill_Rect_Trans(rect, RGBClass(0xFD, 0xB5, 0x1C), 50);
    rect.Set(xpos + 11, bottom - firstheight, 4, firstheight);
    ((Surface *)SurfacePtr)->Fill_Rect_Trans(rect, RGBClass(0xFD, 0xB5, 0x1C), 25);
    rect.Set(xpos + 132, bottom - secondheight, 4, secondheight);
    ((Surface *)SurfacePtr)->Fill_Rect_Trans(rect, RGBClass(0xFA, 0x1C, 0x1C), 25);
    rect.Set(xpos + 132 + 4, bottom - secondheight, 7, secondheight);
    ((Surface *)SurfacePtr)->Fill_Rect_Trans(rect, RGBClass(0xFA, 0x1C, 0x1C), 50);
    rect.Set(xpos + 132 + 11, bottom - secondheight, 4, secondheight);
    ((Surface *)SurfacePtr)->Fill_Rect_Trans(rect, RGBClass(0xFA, 0x1C, 0x1C), 25);

    int firstheightcount = 1;
    int secondheightcount = 1;
    int secondcount = second;
    int firstcount = first;
    int secondstep = second * 2;
    int firststep = first * 2;
    for (int y = bottom - 1; firstheightcount <= (firstheight > secondheight ? firstheight : secondheight); ) {
        if (firstrect.Is_Valid()) {
            unknown_surface_839954->Copy_From(firstrect, *unknown_surface_839958, firstrect, false, true);
        }
        if (firstheightcount <= firstheight) {
            rect.Set(xpos, y, 15, firstheightcount);
            unknown_surface_839954->Copy_From(rect, *(Surface *)SurfacePtr, rect, false, true);
            firstrect = Count_Up_Print(unknown_surface_839954, L"%d", firstcount / max, first, xpos, y);
        } else {
            firstrect = Count_Up_Print(unknown_surface_839954, L"%d", first, first, xpos, bottom - firstheight);
        }
        if (secondrect.Is_Valid()) {
            unknown_surface_839954->Copy_From(secondrect, *unknown_surface_839958, secondrect, false, true);
        }
        if (secondheightcount <= secondheight) {
            rect.Set(xpos + 132, y, 15, secondheightcount);
            unknown_surface_839954->Copy_From(rect, *(Surface *)SurfacePtr, rect, false, true);
            secondrect = Count_Up_Print(unknown_surface_839954, L"%d", secondcount / max, second, xpos + 132, y);
        } else {
            secondrect = Count_Up_Print(unknown_surface_839954, L"%d", second, second, xpos + 132, bottom - secondheight);
        }
        Play_Sound("BarGraph", 96);
        ScoreDelay(1);
        firstheightcount += 2;
        secondheightcount += 2;
        firstcount += firststep;
        y -= 2;
        secondcount += secondstep;
    }

    if (firstrect.Is_Valid()) {
        unknown_surface_839954->Copy_From(firstrect, *unknown_surface_839958, firstrect, false, true);
    }
    rect.Set(xpos, bottom - firstheight, 15, firstheight);
    unknown_surface_839958->Copy_From(rect, *(Surface *)SurfacePtr, rect, false, true);
    Count_Up_Print(unknown_surface_839954, L"%d", first, first, xpos, bottom - firstheight);
    Count_Up_Print(unknown_surface_839958, L"%d", first, first, xpos, bottom - firstheight);
    if (secondrect.Is_Valid()) {
        unknown_surface_839954->Copy_From(secondrect, *unknown_surface_839958, secondrect, false, true);
    }
    rect.Set(xpos + 132, bottom - secondheight, 15, secondheight);
    unknown_surface_839958->Copy_From(rect, *(Surface *)SurfacePtr, rect, false, true);
    Count_Up_Print(unknown_surface_839954, L"%d", second, second, xpos + 132, bottom - secondheight);
    Count_Up_Print(unknown_surface_839958, L"%d", second, second, xpos + 132, bottom - secondheight);
    ScoreDelay(20);
}


// void ScoreClass::Show_Credits() (0x00666B60)
// Types the player's money, right-aligned at 590 (12 pixels a digit).

void ScoreClass::Show_Credits()
{
    char buffer[20];
    sprintf(buffer, "%d", unknown_player_a35db4->Available_Money());
    int x = 590 - strlen(buffer) * 12;
    ScorePrintClass *anim = new ScorePrintClass(buffer, XOffset + x, YOffset + 214, Font, false);
    Anims.Add(anim);
    while (!Is_Anim_Done(anim)) {
        ScoreDelay(1);
    }
}


// void ScoreClass::Print_Time(unsigned long seconds) (0x00666C70)
// Types the elapsed time as hh:mm:ss (at most 99:59:59), centred on 542.

void ScoreClass::Print_Time(unsigned long seconds)
{
    unsigned short buffer[20];
    if (seconds / 3600 > 99) {
        seconds = 359999;
    }
    unsigned long hours = seconds / 3600;
    seconds -= hours * 3600;
    unsigned long minutes = seconds / 60;
    seconds -= minutes * 60;
    swprintf((wchar_t *)buffer, L"%02ld:%02ld:%02ld", hours, minutes, seconds);
    int x = XOffset - unknown_font_10->String_Pixel_Width(buffer) / 2 + 542;
    ScorePrintClass *anim = new ScorePrintClass(buffer, x, YOffset + 322, unknown_font_10, false);
    Anims.Add(anim);
    while (Anims.ID(anim) != -1) {
        ScoreDelay(1);
    }
}


// Rect ScoreClass::Count_Up_Print(Surface *surface, unsigned short const *format, int value, int max, int x, int y) (0x00666DD0)
// Prints min(value, max) with format so its baseline is at y and returns
// the printed area.

Rect ScoreClass::Count_Up_Print(Surface *surface, unsigned short const *format, int value, int max, int x, int y)
{
    unsigned short buffer[64];
    swprintf((wchar_t *)buffer, (wchar_t const *)format, value < max ? value : max);
    y -= Font->Height;
    Font->Print(surface, buffer, x, y, 2);
    return Rect(x, y, Font->String_Pixel_Width(buffer), Font->Height);
}


// void ScoreClass::Input_Name(char *name, int x, int y) (0x00666E70)
// Reads the player's name from the keyboard: typed characters are drawn by
// a ScorePrintClass, backspace restores the background, return ends it.

void ScoreClass::Input_Name(char *name, int x, int y)
{
    int key = 0;
    int index = 0;
    int xpos = x;
    do {
        Wait_While_Inactive();
        if (WWKeyboard->Check()) {
            key = WWKeyboard->To_Char(WWKeyboard->Get()) & 0xFF;
            unknown_481fb0();
            if (xpos >= x + 90) {
                while (WWKeyboard->Check()) {
                    WWKeyboard->Get();
                }
            }
            if (key == 8 && xpos >= x + 84) {
                if (name[index] && name[index] != ' ') {
                    Rect rect(xpos, y + 1, Font->Char_Pixel_Width(name[index]) + 1, 16);
                    unknown_surface_839954->Copy_From(rect, *(Surface *)SurfacePtr, rect, false, true);
                    unknown_surface_839958->Copy_From(rect, *(Surface *)SurfacePtr, rect, false, true);
                    key = ' ';
                }
            }
            if (key == 8) {
                if (index) {
                    char letter = name[--index];
                    int width = Font->Char_Pixel_Width(letter) + 1;
                    xpos -= width;
                    Rect rect(xpos, y + 1, width, 16);
                    unknown_surface_839954->Copy_From(rect, *(Surface *)SurfacePtr, rect, false, true);
                    unknown_surface_839958->Copy_From(rect, *(Surface *)SurfacePtr, rect, false, true);
                    name[index] = 0;
                    Play_Sound("Back", 192);
                }
            } else if (key != 13 && key >= ' ') {
                int width = Font->Char_Pixel_Width(key) + 1;
                if (xpos + Font->Width <= x + 90) {
                    if (xpos + width >= x + 84) {
                        Rect rect(xpos, y + 1, Font->Char_Pixel_Width(name[index]) + 1, 16);
                        unknown_surface_839954->Copy_From(rect, *(Surface *)SurfacePtr, rect, false, true);
                        unknown_surface_839958->Copy_From(rect, *(Surface *)SurfacePtr, rect, false, true);
                    }
                    name[index] = key;
                    name[index + 1] = 0;
                    ScorePrintClass *anim = new ScorePrintClass(&name[index], xpos, y, Font, false);
                    Anims.Add(anim);
                    Play_Sound("Type", 128);
                    if (xpos < x + 87) {
                        index++;
                        xpos += Font->Char_Pixel_Width(key) + 1;
                    }
                }
            }
        }
        Draw_Cursor(xpos, y);
        ScoreDelay(1);
    } while (key != 13);
}


// void ScoreClass::Draw_Cursor(int x, int y) (0x006672A0)
// The name entry's blinking underscore: erases it where it was, draws it
// (11 pixels, 14 below y) or restores the background under it, and flips
// it every 5 ticks. The function-static timer's atexit destructor is
// 0x00667410.

void ScoreClass::Draw_Cursor(int x, int y)
{
    static CDTimerClass<SystemTimerClass> timer;
    static int lastx;
    static int blink;
    Rect rect;
    y += 14;
    if (x != lastx) {
        rect.Set(lastx, y, 11, 1);
        unknown_surface_839954->Copy_From(rect, *unknown_surface_839958, rect, false, true);
        lastx = x;
    }
    if (blink) {
        unknown_surface_839954->Draw_Line(Point2D(x, y), Point2D(x + 10, y), Color);
    } else {
        rect.Set(x, y, 11, 1);
        unknown_surface_839954->Copy_From(rect, *(Surface *)SurfacePtr, rect, false, true);
    }
    if (!timer) {
        blink ^= 1;
        timer = 5;
    }
}


// Guarded atexit destructor (0x00667410, _$E) of ScoreClass::Draw_Cursor's
// function-static timer, emitted by ScoreClass_Draw_Cursor_006672a0.cpp; no source of its own.


void ScoreClass::ScoreDelay(int ticks)
{
    CDTimerClass<SystemTimerClass> timer(1);
    do {
        Update_Anims();
        Blit_Score();
        do {
            Service_Game_IO_00481fb0();
            unknown_5b5ee0();
            if(!unknown_flag_a40d28) {
                timer.Stop();
                Wait_While_Inactive();
                timer.Start();
            }
            Sleep(0);
        } while(timer>0);
        timer=1;
        --ticks;
    } while(ticks>0);
}

// External references checked against complete original body operands.


// void ScoreClass::Update_Anims() (0x00667620)

void ScoreClass::Update_Anims()
{
    unknown_score_print_ab6f9c = false;
    if (unknown_redraw_8523dc) {
        unknown_redraw_8523dc = false;
        unknown_surface_839954->Fill(0);
        unknown_surface_839954->Copy_From(*unknown_surface_839958, false, true);
    }
    for (int index = 0; index < Anims.Count(); index++) {
        if (Anims[index]->Update(SurfacePtr) == true) {
            delete Anims[index];
            Anims.Delete(index);
        }
    }
}


// void ScoreClass::Blit_Score() (0x006676C0)
// Copies the 640x400 score area of the score surface to the screen at the
// window's client origin (as MSEngine::Blit_Rect 0x005B3490).

void ScoreClass::Blit_Score()
{
    RECT client;
    GetClientRect(MainWindow, &client);
    ClientToScreen(MainWindow, (POINT *)&client);
    Rect dest(XOffset + client.left, YOffset + client.top, 640, 400);
    Rect source(XOffset, YOffset, 640, 400);
    if (WWMouse != 0) {
        WWMouse->Draw_Mouse(unknown_surface_839954);
    }
    unknown_surface_839950->Copy_From(dest, *unknown_surface_839954, source, false, true);
    if (WWMouse != 0) {
        WWMouse->Erase_Mouse(unknown_surface_839954);
    }
}


// void ScoreClass::Wait_While_Inactive() (0x00667780)
// While the game window is inactive: pauses every animation, idles until
// it is active again, then resumes them (ScoreDelay inlines it).

void ScoreClass::Wait_While_Inactive()
{
    if (unknown_flag_a40d28 != true) {
        for (int index = 0; index < Anims.Count(); index++) {
            Anims[index]->Pause();
        }
        while (!unknown_flag_a40d28) {
            Sleep(500);
            unknown_5b5ee0();
        }
        for (int index2 = 0; index2 < Anims.Count(); index2++) {
            Anims[index2]->Resume();
        }
    }
}


// void ScoreClass::Play_Sound(char const *name, int volume) (0x006677F0)
// When a name is given and sound is available, walks Sounds while their
// names differ from it; nothing is played (the call is gone) and volume is
// unused. Input_Name and Presentation inline it.

void ScoreClass::Play_Sound(char const *name, int volume)
{
    if (name != 0 && unknown_check_406f30()) {
        for (int index = 0; index < Sounds.Count(); index++) {
            char const *soundname = Sounds[index]->Name;
            if (_stricmp(name, soundname) == 0) {
                break;
            }
        }
    }
}


ScoreTimeClass::ScoreTimeClass(int x, int y, void const *data, int maxStage, int delay, ConvertClass *drawer) :
    ScoreAnimClass(x,y,data),
    Stage(0),
    MaxStage(maxStage),
    DelayFrames(delay),
    Drawer(drawer)
{
}

// External references checked against complete original body operands.


// bool ScoreTimeClass::Update(Surface *surface) (0x00667F90, slot 3)
// Each time the timer runs out: restart it, advance the frame (wrapping at
// MaxStage) and draw it on both score surfaces (Westwood's SCORE.CPP).
// The unit's shape helpers fall back on Score.cpp's empty rectangle.



bool ScoreTimeClass::Update(Surface *surface)
{
    if (!Timer) {
        Timer = DelayFrames;
        if (++Stage >= MaxStage) {
            Stage = 0;
        }
        CC_Draw_Shape(unknown_surface_839954, Drawer, (ShapeFileStruct const *)DataPtr, Stage,
                      Point2D(XPos, YPos), unknown_surface_839954->Get_Rect(), 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
        CC_Draw_Shape(unknown_surface_839958, Drawer, (ShapeFileStruct const *)DataPtr, Stage,
                      Point2D(XPos, YPos), unknown_surface_839958->Get_Rect(), 0x400, 0, 0, 0, 1000, 0, 0, Point2D(0, 0));
    }
    return false;
}


// ScorePrintClass::ScorePrintClass(void const *string, int x, int y, ScoreFontClass *font, bool background) (0x006680A0)

ScorePrintClass::ScorePrintClass(void const *string, int x, int y, ScoreFontClass *font, bool background) :
    ScoreAnimClass(x, y, string),
    Stage(0),
    unknown_20(0),
    Background(background),
    Font(font)
{
}


// bool ScorePrintClass::Update(Surface *surface) (0x006680F0, slot 3)
// Types the string one more character each time the two-tick timer runs
// out: the characters typed so far are drawn, the newest two at the
// earlier fade frames, and each is also drawn into the background surface
// once settled. Returns true once the string has been shown for a while.

bool ScorePrintClass::Update(Surface *surface)
{
    unknown_score_print_ab6f9c = true;
    if (!Timer) {
        Timer = CDTimerClass<SystemTimerClass>(2);
        if ((unsigned)Stage > wcslen((unsigned short const *)DataPtr) && unknown_20 > 2) {
            return true;
        }
        if (!Background) {
            int width = Font->String_Pixel_Width((unsigned short const *)DataPtr);
            Rect rect(XPos - 8 > 0 ? XPos - 8 : 0, YPos, width + 10, Font->Height);
            unknown_surface_839954->Copy_From(rect, *unknown_surface_839958, rect, false, true);
        }
        int x = XPos;
        for (int i = 0; i <= Stage; i++) {
            char c = (char)((unsigned short const *)DataPtr)[i];
            if (c == 0) {
                unknown_20++;
                break;
            }
            int frame = Background == true ? 2 : (Stage - i < 2 ? Stage - i : 2);
            Font->Print_Char(unknown_surface_839954, c, x, YPos, frame, 1);
            if (frame == 2) {
                Font->Print_Char(unknown_surface_839958, c, x, YPos, 2, 0);
            }
            x += Font->Char_Pixel_Width(c) + 1;
        }
        Stage++;
    }
    return false;
}


// ScoreAnimClass::ScoreAnimClass(int x, int y, void const *data) (0x006682C0)

ScoreAnimClass::ScoreAnimClass(int x, int y, void const *data) :
    XPos(x),
    YPos(y),
    Timer(0),
    DataPtr(data)
{
}


// void ScoreAnimClass::Pause() -- 0x00668300 (slot 1)
// Purpose: stops the score animation's system-clock countdown at +0x0C: a
// running timer keeps the time left in DelayTime and goes back to -1.
// Assumptions: CDTimerClass<SystemTimerClass>::Stop with Value() expanded in
// place; only this spelling reproduces the cached Started load.

void ScoreAnimClass::Pause()
{
    if (Timer.Started != -1) {
        Timer.DelayTime = Timer.Value();
        Timer.Started = -1;
    }
}


// void *ScoreAnimClass::`scalar deleting destructor'(unsigned int flags) (0x00668370)
// Compiler-generated with the vtable. No source of its own.


// 0x00668390: function not yet identified
//
// Owner: ScoreTimeClass, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 32 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_00668390(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00668390 {

extern void  operator_delete_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_007a928c;

struct Owner_00668390 { unsigned int * FUN_00668390(unsigned char deletion_flags); };

unsigned int * Owner_00668390::FUN_00668390(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_007a928c);
    if ((deletion_flags & 1) != 0) {
        operator_delete_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_00668390


// void *ScorePrintClass::`scalar deleting destructor'(unsigned int flags) (0x006683B0)
// Compiler-generated with the vtable. No source of its own.


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<ScoreAnimClass *>::Add (0x006684F0).
// Layout and behaviour: shared/vector_class.hpp. The Score unit's out-of-line copy
// (ScoreClass::Anims; called six times from 0x006647F2-0x00664ECF).
