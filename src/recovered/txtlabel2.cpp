// txtlabel2.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/txtlabel2.json carries the addresses and the rows that
// stay in their own files.


#include "shared/rgb_class.hpp"
#include "shared/string_table.hpp"


// void unknown_init_color_ac1124() -- 0x006F1A00
// Sets the colour at 0x00AC1124 to (0xFF, 0xFF, 0).
extern RGBClass unknown_color_ac1124;

void __fastcall unknown_init_color_ac1124()
{
    unknown_color_ac1124.Red = 0xff;
    unknown_color_ac1124.Green = 0xff;
    unknown_color_ac1124.Blue = 0;
}


// void unknown_init_color_ac1028() -- 0x006F1AC0
// Sets the colour at 0x00AC1028 to black.
extern RGBClass unknown_color_ac1028;

void __fastcall unknown_init_color_ac1028()
{
    unknown_color_ac1028.Red = 0;
    unknown_color_ac1028.Green = 0;
    unknown_color_ac1028.Blue = 0;
}


// void Unknown_Free_All_Art_006F1D20() -- 0x006F1D20
// Frees every art resource the shell loaded: the four flagged buffers
// (0x00AC1218..0x00AC1220 and 0x00AC10C0), the two art pairs 0x006F20B0 loads
// (0x00AC1130..0x00AC113C), then, when the second set is loaded (0x00AC119C),
// the second set's pairs and blocks, and finally the third set (hypothesis).

class UnknownArtData_006f2390 {
public:
    virtual ~UnknownArtData_006f2390();
};
extern void *unknown_art_ac1218;
extern void *unknown_art_ac121c;
extern void *unknown_art_ac1220;
extern void *unknown_art_ac10c0;
extern bool unknown_flag_ac1224;
extern bool unknown_flag_ac1225;
extern bool unknown_flag_ac1226;
extern bool unknown_flag_ac1227;
extern void *unknown_art_ac1130;
extern UnknownArtData_006f2390 *unknown_art_ac1134;
extern void *unknown_art_ac1138;
extern UnknownArtData_006f2390 *unknown_art_ac113c;
extern bool unknown_flag_ac119c;
extern void *unknown_art_ac11b4;
extern void *unknown_art_ac11b8;
extern void *unknown_art_ac11bc;
extern void *unknown_art_ac11c0;
extern void *unknown_art_ac11c4;
extern void *unknown_art_ac11c8;
extern void *unknown_art_ac11cc;
extern void *unknown_art_ac11d0;
extern void *unknown_art_ac11a0;
extern UnknownArtData_006f2390 *unknown_art_ac11a4;
extern void *unknown_art_ac11a8;
extern UnknownArtData_006f2390 *unknown_art_ac11ac;
extern bool unknown_flag_ac11b0;
void __fastcall Unknown_006F4160();
void __fastcall Unknown_6F42D0();
void __fastcall Unknown_0051A270();
void __fastcall Unknown_006F5150();
void __fastcall Unknown_6F5400();

void __fastcall Unknown_Free_All_Art_006F1D20()
{
    if (unknown_flag_ac1224) {
        operator delete(unknown_art_ac1218);
        unknown_flag_ac1224 = false;
    }
    unknown_art_ac1218 = 0;
    if (unknown_flag_ac1225) {
        operator delete(unknown_art_ac121c);
        unknown_flag_ac1225 = false;
    }
    unknown_art_ac121c = 0;
    if (unknown_flag_ac1226) {
        operator delete(unknown_art_ac1220);
        unknown_flag_ac1226 = false;
    }
    unknown_art_ac1220 = 0;
    if (unknown_flag_ac1227) {
        operator delete(unknown_art_ac10c0);
        unknown_flag_ac1227 = false;
    }
    unknown_art_ac10c0 = 0;
    if (unknown_art_ac1130 != 0) {
        operator delete(unknown_art_ac1130);
        unknown_art_ac1130 = 0;
    }
    if (unknown_art_ac1134 != 0) {
        delete [] unknown_art_ac1134;
        unknown_art_ac1134 = 0;
    }
    if (unknown_art_ac1138 != 0) {
        operator delete(unknown_art_ac1138);
        unknown_art_ac1138 = 0;
    }
    if (unknown_art_ac113c != 0) {
        delete [] unknown_art_ac113c;
        unknown_art_ac113c = 0;
    }
    if (unknown_flag_ac119c) {
        unknown_flag_ac119c = false;
        Unknown_006F4160();
        Unknown_6F42D0();
        operator delete(unknown_art_ac11b4);
        unknown_art_ac11b4 = 0;
        operator delete(unknown_art_ac11b8);
        unknown_art_ac11b8 = 0;
        operator delete(unknown_art_ac11bc);
        unknown_art_ac11bc = 0;
        operator delete(unknown_art_ac11c0);
        unknown_art_ac11c0 = 0;
        operator delete(unknown_art_ac11c4);
        unknown_art_ac11c4 = 0;
        operator delete(unknown_art_ac11c8);
        unknown_art_ac11c8 = 0;
        operator delete(unknown_art_ac11cc);
        unknown_art_ac11cc = 0;
        operator delete(unknown_art_ac11d0);
        unknown_art_ac11d0 = 0;
        Unknown_0051A270();
    }
    Unknown_006F5150();
    if (unknown_art_ac11a0 != 0) {
        operator delete(unknown_art_ac11a0);
        unknown_art_ac11a0 = 0;
    }
    if (unknown_art_ac11a4 != 0) {
        delete unknown_art_ac11a4;
        unknown_art_ac11a4 = 0;
    }
    if (unknown_art_ac11a8 != 0) {
        operator delete(unknown_art_ac11a8);
        unknown_art_ac11a8 = 0;
    }
    if (unknown_art_ac11ac != 0) {
        delete unknown_art_ac11ac;
        unknown_art_ac11ac = 0;
    }
    Unknown_6F5400();
    unknown_flag_ac11b0 = false;
}


// void Unknown_Free_Art_Sets_006F2130() -- 0x006F2130
// Frees the four flagged buffers and the two art pairs of the shell (hypothesis).

extern bool unknown_flag_ac1224;
extern bool unknown_flag_ac1225;
extern bool unknown_flag_ac1226;
extern bool unknown_flag_ac1227;
extern void * unknown_art_ac1218;
extern void * unknown_art_ac121c;
extern void * unknown_art_ac1220;
extern void * unknown_art_ac10c0;
extern void * unknown_art_ac1130;
extern UnknownArtData_006f2390 * unknown_art_ac1134;
extern void * unknown_art_ac1138;
extern UnknownArtData_006f2390 * unknown_art_ac113c;

void __fastcall Unknown_Free_Art_Sets_006F2130()
{
    if (unknown_flag_ac1224) {
        operator delete(unknown_art_ac1218);
        unknown_flag_ac1224 = false;
    }
    unknown_art_ac1218 = 0;
    if (unknown_flag_ac1225) {
        operator delete(unknown_art_ac121c);
        unknown_flag_ac1225 = false;
    }
    unknown_art_ac121c = 0;
    if (unknown_flag_ac1226) {
        operator delete(unknown_art_ac1220);
        unknown_flag_ac1226 = false;
    }
    unknown_art_ac1220 = 0;
    if (unknown_flag_ac1227) {
        operator delete(unknown_art_ac10c0);
        unknown_flag_ac1227 = false;
    }
    unknown_art_ac10c0 = 0;
    if (unknown_art_ac1130 != 0) {
        operator delete(unknown_art_ac1130);
        unknown_art_ac1130 = 0;
    }
    if (unknown_art_ac1134 != 0) {
        delete [] unknown_art_ac1134;
        unknown_art_ac1134 = 0;
    }
    if (unknown_art_ac1138 != 0) {
        operator delete(unknown_art_ac1138);
        unknown_art_ac1138 = 0;
    }
    if (unknown_art_ac113c != 0) {
        delete [] unknown_art_ac113c;
        unknown_art_ac113c = 0;
    }
}


// void Unknown_Load_Art_Buffers_006F2260() -- 0x
// Loads the four flagged buffers of the shell (hypothesis).

void *__fastcall Unknown_00495270(char const *name, bool *allocated);

extern char const * const unknown_name_7f8740;
extern char const * const unknown_name_7f8744;
extern char const * const unknown_name_7f8748;
extern char const * const unknown_name_7f874c;
extern bool unknown_flag_ac1224;
extern bool unknown_flag_ac1225;
extern bool unknown_flag_ac1226;
extern bool unknown_flag_ac1227;
extern void *unknown_art_ac1218;
extern void *unknown_art_ac121c;
extern void *unknown_art_ac1220;
extern void *unknown_art_ac10c0;

void __fastcall Unknown_Load_Art_Buffers_006F2260()
{
    unknown_art_ac1218 = Unknown_00495270(unknown_name_7f8740, &unknown_flag_ac1224);
    unknown_art_ac121c = Unknown_00495270(unknown_name_7f8744, &unknown_flag_ac1225);
    unknown_art_ac1220 = Unknown_00495270(unknown_name_7f8748, &unknown_flag_ac1226);
    unknown_art_ac10c0 = Unknown_00495270(unknown_name_7f874c, &unknown_flag_ac1227);
}


// void Unknown_Free_Art_Buffers_006F22F0() -- 0x006F22F0
// Frees the four flagged buffers of the shell (hypothesis).

extern bool unknown_flag_ac1224;
extern bool unknown_flag_ac1225;
extern bool unknown_flag_ac1226;
extern bool unknown_flag_ac1227;
extern void * unknown_art_ac1218;
extern void * unknown_art_ac121c;
extern void * unknown_art_ac1220;
extern void * unknown_art_ac10c0;

void __fastcall Unknown_Free_Art_Buffers_006F22F0()
{
    if (unknown_flag_ac1224) {
        operator delete(unknown_art_ac1218);
        unknown_flag_ac1224 = false;
    }
    unknown_art_ac1218 = 0;
    if (unknown_flag_ac1225) {
        operator delete(unknown_art_ac121c);
        unknown_flag_ac1225 = false;
    }
    unknown_art_ac121c = 0;
    if (unknown_flag_ac1226) {
        operator delete(unknown_art_ac1220);
        unknown_flag_ac1226 = false;
    }
    unknown_art_ac1220 = 0;
    if (unknown_flag_ac1227) {
        operator delete(unknown_art_ac10c0);
        unknown_flag_ac1227 = false;
    }
    unknown_art_ac10c0 = 0;
}


// void Unknown_Free_Art_Sets_006F2510()
// Frees the three art pairs 0x006F24A0 loads (hypothesis).

extern void *unknown_art_ac1140;
extern UnknownArtData_006f2390 *unknown_art_ac1144;
extern void *unknown_art_ac1148;
extern UnknownArtData_006f2390 *unknown_art_ac114c;
extern void *unknown_art_ac1150;
extern UnknownArtData_006f2390 *unknown_art_ac1154;

void __fastcall Unknown_Free_Art_Sets_006F2510()
{
    if (unknown_art_ac1140 != 0) {
        operator delete(unknown_art_ac1140);
        unknown_art_ac1140 = 0;
    }
    if (unknown_art_ac1144 != 0) {
        delete unknown_art_ac1144;
        unknown_art_ac1144 = 0;
    }
    if (unknown_art_ac1148 != 0) {
        operator delete(unknown_art_ac1148);
        unknown_art_ac1148 = 0;
    }
    if (unknown_art_ac114c != 0) {
        delete unknown_art_ac114c;
        unknown_art_ac114c = 0;
    }
    if (unknown_art_ac1150 != 0) {
        operator delete(unknown_art_ac1150);
        unknown_art_ac1150 = 0;
    }
    if (unknown_art_ac1154 != 0) {
        delete unknown_art_ac1154;
        unknown_art_ac1154 = 0;
    }
}


// void Unknown_Free_Art_Buffers_006F2980() -- 0x006F2980
// Frees the three flagged art buffers (0x00AC110C, 0x00AC10F0, 0x00AC1108; each freed only when
// it was allocated) and clears their pointers: the body of 0x006F2750 without its loaded test.

extern bool unknown_flag_ac1214;
extern bool unknown_flag_ac1215;
extern bool unknown_flag_ac1216;
extern void *unknown_art_ac110c;
extern void *unknown_art_ac10f0;
extern void *unknown_art_ac1108;

void __fastcall Unknown_Free_Art_Buffers_006F2980()
{
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
}


// void Unknown_006F2AB0() -- 0x006F2AB0
// Once loaded (flag 0x00AC1174): clears the flag, releases the backdrop shape at 0x00AC1104
// (freed only when it was allocated, flag 0x00AC1217) and the art pair at
// 0x00AC116C/0x00AC1170 that 0x006F2A30 loads (hypothesis).

extern bool unknown_flag_ac1174;
extern bool unknown_flag_ac1217;
extern void *unknown_art_ac1104;
extern void *unknown_art_ac116c;
extern UnknownArtData_006f2390 *unknown_art_ac1170;

void __fastcall Unknown_006F2AB0()
{
    if (unknown_flag_ac1174) {
        unknown_flag_ac1174 = false;
        if (unknown_flag_ac1217) {
            operator delete(unknown_art_ac1104);
            unknown_flag_ac1217 = false;
        }
        unknown_art_ac1104 = 0;
        if (unknown_art_ac116c != 0) {
            operator delete(unknown_art_ac116c);
            unknown_art_ac116c = 0;
        }
        if (unknown_art_ac1170 != 0) {
            delete unknown_art_ac1170;
            unknown_art_ac1170 = 0;
        }
    }
}


// void Unknown_Free_Backdrop_006F2BD0() -- 0x006F2BD0
// Frees the backdrop shape 0x00AC1104 when it was allocated (flag 0x00AC1217) and clears it.

extern bool unknown_flag_ac1217;
extern void *unknown_art_ac1104;

void __fastcall Unknown_Free_Backdrop_006F2BD0()
{
    if (unknown_flag_ac1217) {
        operator delete(unknown_art_ac1104);
        unknown_flag_ac1217 = false;
    }
    unknown_art_ac1104 = 0;
}


// void Unknown_006F2C00() -- 0x006F2C00
// Frees the art block at 0x00AC116C (operator delete) and the companion object at
// 0x00AC1170 (delete through its virtual destructor), clearing each pointer
// (hypothesis; the same pair 0x006F2A30 loads).

extern void *unknown_art_ac116c;
extern UnknownArtData_006f2390 *unknown_art_ac1170;

void __fastcall Unknown_006F2C00()
{
    if (unknown_art_ac116c != 0) {
        operator delete(unknown_art_ac116c);
        unknown_art_ac116c = 0;
    }
    if (unknown_art_ac1170 != 0) {
        delete unknown_art_ac1170;
        unknown_art_ac1170 = 0;
    }
}


// void Unknown_006F2C40() -- 0x006F2C40
// Loads four shapes into 0x00AC1120, 0x00AC1040, 0x00AC10FC and 0x00AC1074, named for the screen
// width (640 or not), recording in the flags at 0x00AC1228 whether each was allocated and the art pair at 0x00AC1178 (hypothesis).

void *__fastcall Unknown_00495270(char const *name, bool *allocated);
extern int unknown_value_8523e0;
void __fastcall unknown_load_art_6f1fa0(char const *name, void **shape, void **data);   // 0x006F1FA0
extern char const * const unknown_name_7f8718;
extern void *unknown_art_ac1178;
extern void *unknown_art_ac117c;
extern char const * const unknown_name_7f8790;
extern char const * const unknown_name_7f8798;
extern char const * const unknown_name_7f87a0;
extern char const * const unknown_name_7f87a8;
extern char const * const unknown_name_7f8794;
extern char const * const unknown_name_7f879c;
extern char const * const unknown_name_7f87a4;
extern char const * const unknown_name_7f87ac;
extern bool unknown_flag_ac1228;
extern bool unknown_flag_ac1229;
extern bool unknown_flag_ac122a;
extern bool unknown_flag_ac122b;
extern void *unknown_art_ac1120;
extern void *unknown_art_ac1040;
extern void *unknown_art_ac10fc;
extern void *unknown_art_ac1074;

void __fastcall Unknown_006F2C40()
{
    if (unknown_value_8523e0 == 0x280) {
        unknown_art_ac1120 = Unknown_00495270(unknown_name_7f8790, &unknown_flag_ac1228);
        unknown_art_ac1040 = Unknown_00495270(unknown_name_7f8798, &unknown_flag_ac1229);
        unknown_art_ac10fc = Unknown_00495270(unknown_name_7f87a0, &unknown_flag_ac122a);
        unknown_art_ac1074 = Unknown_00495270(unknown_name_7f87a8, &unknown_flag_ac122b);
    } else {
        unknown_art_ac1120 = Unknown_00495270(unknown_name_7f8794, &unknown_flag_ac1228);
        unknown_art_ac1040 = Unknown_00495270(unknown_name_7f879c, &unknown_flag_ac1229);
        unknown_art_ac10fc = Unknown_00495270(unknown_name_7f87a4, &unknown_flag_ac122a);
        unknown_art_ac1074 = Unknown_00495270(unknown_name_7f87ac, &unknown_flag_ac122b);
    }
    unknown_load_art_6f1fa0(unknown_name_7f8718, &unknown_art_ac1178, (void **)&unknown_art_ac117c);
}


// void Unknown_006F2DF0() -- 0x006F2DF0
// Loads four shapes into 0x00AC1120, 0x00AC1040, 0x00AC10FC and 0x00AC1074, named for the screen
// width (640 or not), recording in the flags at 0x00AC1228 whether each was allocated (hypothesis).

void *__fastcall Unknown_00495270(char const *name, bool *allocated);
extern int unknown_value_8523e0;
extern char const * const unknown_name_7f8790;
extern char const * const unknown_name_7f8798;
extern char const * const unknown_name_7f87a0;
extern char const * const unknown_name_7f87a8;
extern char const * const unknown_name_7f8794;
extern char const * const unknown_name_7f879c;
extern char const * const unknown_name_7f87a4;
extern char const * const unknown_name_7f87ac;
extern bool unknown_flag_ac1228;
extern bool unknown_flag_ac1229;
extern bool unknown_flag_ac122a;
extern bool unknown_flag_ac122b;
extern void *unknown_art_ac1120;
extern void *unknown_art_ac1040;
extern void *unknown_art_ac10fc;
extern void *unknown_art_ac1074;

void __fastcall Unknown_006F2DF0()
{
    if (unknown_value_8523e0 == 0x280) {
        unknown_art_ac1120 = Unknown_00495270(unknown_name_7f8790, &unknown_flag_ac1228);
        unknown_art_ac1040 = Unknown_00495270(unknown_name_7f8798, &unknown_flag_ac1229);
        unknown_art_ac10fc = Unknown_00495270(unknown_name_7f87a0, &unknown_flag_ac122a);
        unknown_art_ac1074 = Unknown_00495270(unknown_name_7f87a8, &unknown_flag_ac122b);
    } else {
        unknown_art_ac1120 = Unknown_00495270(unknown_name_7f8794, &unknown_flag_ac1228);
        unknown_art_ac1040 = Unknown_00495270(unknown_name_7f879c, &unknown_flag_ac1229);
        unknown_art_ac10fc = Unknown_00495270(unknown_name_7f87a4, &unknown_flag_ac122a);
        unknown_art_ac1074 = Unknown_00495270(unknown_name_7f87ac, &unknown_flag_ac122b);
    }
}


// void Unknown_Free_Art_Buffers_006F2ED0() -- 0x006F2ED0
// Frees the four flagged buffers of the shell (hypothesis).

extern bool unknown_flag_ac1228;
extern bool unknown_flag_ac1229;
extern bool unknown_flag_ac122a;
extern bool unknown_flag_ac122b;
extern void * unknown_art_ac1120;
extern void * unknown_art_ac1040;
extern void * unknown_art_ac10fc;
extern void * unknown_art_ac1074;

void __fastcall Unknown_Free_Art_Buffers_006F2ED0()
{
    if (unknown_flag_ac1228) {
        operator delete(unknown_art_ac1120);
        unknown_flag_ac1228 = false;
    }
    unknown_art_ac1120 = 0;
    if (unknown_flag_ac1229) {
        operator delete(unknown_art_ac1040);
        unknown_flag_ac1229 = false;
    }
    unknown_art_ac1040 = 0;
    if (unknown_flag_ac122a) {
        operator delete(unknown_art_ac10fc);
        unknown_flag_ac122a = false;
    }
    unknown_art_ac10fc = 0;
    if (unknown_flag_ac122b) {
        operator delete(unknown_art_ac1074);
        unknown_flag_ac122b = false;
    }
    unknown_art_ac1074 = 0;
}


// void Unknown_006F2FB0() -- 0x006F2FB0
// Once (flag 0x00AC119C): loads ten shapes, three art pairs and runs 0x006F3E70 with the two
// values at 0x008523E0 (hypothesis).

void *__fastcall Unknown_00495270(char const *name, bool *allocated);
void __fastcall unknown_load_art_6f1fa0(char const *name, void **shape, void **data);   // 0x006F1FA0
void __fastcall Unknown_0051A2B0();
void __fastcall Unknown_006F3E70(int arg1, int arg2);
extern bool unknown_flag_ac119c;
extern int unknown_value_8523e0;
extern int unknown_value_8523e4;
extern char const * const unknown_name_7f87b0;
extern bool unknown_flag_ac122c;
extern void *unknown_art_ac10b8;
extern char const * const unknown_name_7f87b4;
extern bool unknown_flag_ac122d;
extern void *unknown_art_ac0fec;
extern char const * const unknown_name_7f87b8;
extern bool unknown_flag_ac122e;
extern void *unknown_art_ac10b4;
extern char const * const unknown_name_7f87bc;
extern bool unknown_flag_ac122f;
extern void *unknown_art_ac1128;
extern char const * const unknown_name_7f87c0;
extern bool unknown_flag_ac1230;
extern void *unknown_art_ac1010;
extern char const * const unknown_name_7f87c4;
extern bool unknown_flag_ac1231;
extern void *unknown_art_ac10e8;
extern char const * const unknown_name_7f87c8;
extern bool unknown_flag_ac1232;
extern void *unknown_art_ac107c;
extern char const * const unknown_name_7f87cc;
extern bool unknown_flag_ac1233;
extern void *unknown_art_ac1044;
extern char const * const unknown_name_7f87d0;
extern bool unknown_flag_ac1234;
extern void *unknown_art_ac10d8;
extern char const * const unknown_name_7f87d4;
extern bool unknown_flag_ac1235;
extern void *unknown_art_ac1060;
extern char const * const unknown_name_7f871c;
extern void *unknown_art_ac1184;
extern void *unknown_art_ac1188;
extern char const * const unknown_name_7f8720;
extern void *unknown_art_ac118c;
extern void *unknown_art_ac1190;
extern char const * const unknown_name_7f8724;
extern void *unknown_art_ac1194;
extern void *unknown_art_ac1198;

void __fastcall Unknown_006F2FB0()
{
    if (!unknown_flag_ac119c) {
        Unknown_0051A2B0();
        unknown_art_ac10b8 = Unknown_00495270(unknown_name_7f87b0, &unknown_flag_ac122c);
        unknown_art_ac0fec = Unknown_00495270(unknown_name_7f87b4, &unknown_flag_ac122d);
        unknown_art_ac10b4 = Unknown_00495270(unknown_name_7f87b8, &unknown_flag_ac122e);
        unknown_art_ac1128 = Unknown_00495270(unknown_name_7f87bc, &unknown_flag_ac122f);
        unknown_art_ac1010 = Unknown_00495270(unknown_name_7f87c0, &unknown_flag_ac1230);
        unknown_art_ac10e8 = Unknown_00495270(unknown_name_7f87c4, &unknown_flag_ac1231);
        unknown_art_ac107c = Unknown_00495270(unknown_name_7f87c8, &unknown_flag_ac1232);
        unknown_art_ac1044 = Unknown_00495270(unknown_name_7f87cc, &unknown_flag_ac1233);
        unknown_art_ac10d8 = Unknown_00495270(unknown_name_7f87d0, &unknown_flag_ac1234);
        unknown_art_ac1060 = Unknown_00495270(unknown_name_7f87d4, &unknown_flag_ac1235);
        unknown_load_art_6f1fa0(unknown_name_7f871c, &unknown_art_ac1184, (void **)&unknown_art_ac1188);
        unknown_load_art_6f1fa0(unknown_name_7f8720, &unknown_art_ac118c, (void **)&unknown_art_ac1190);
        unknown_load_art_6f1fa0(unknown_name_7f8724, &unknown_art_ac1194, (void **)&unknown_art_ac1198);
        Unknown_006F3E70(unknown_value_8523e0, unknown_value_8523e4);
        unknown_flag_ac119c = true;
    }
}


// void Unknown_Free_Art_Set_006F30F0() -- 0x006F30F0
// Frees the second shell set loaded by 0x006F2FB0 (flag 0x00AC119C) (hypothesis).

void __fastcall Unknown_006F4160();
void __fastcall Unknown_6F42D0();
void __fastcall Unknown_0051A270();
extern bool unknown_flag_ac119c;
extern void *unknown_art_ac11b4;
extern void *unknown_art_ac11b8;
extern void *unknown_art_ac11bc;
extern void *unknown_art_ac11c0;
extern void *unknown_art_ac11c4;
extern void *unknown_art_ac11c8;
extern void *unknown_art_ac11cc;
extern void *unknown_art_ac11d0;

void __fastcall Unknown_Free_Art_Set_006F30F0()
{
    if (unknown_flag_ac119c) {
        unknown_flag_ac119c = false;
        Unknown_006F4160();
        Unknown_6F42D0();
        operator delete(unknown_art_ac11b4);
        unknown_art_ac11b4 = 0;
        operator delete(unknown_art_ac11b8);
        unknown_art_ac11b8 = 0;
        operator delete(unknown_art_ac11bc);
        unknown_art_ac11bc = 0;
        operator delete(unknown_art_ac11c0);
        unknown_art_ac11c0 = 0;
        operator delete(unknown_art_ac11c4);
        unknown_art_ac11c4 = 0;
        operator delete(unknown_art_ac11c8);
        unknown_art_ac11c8 = 0;
        operator delete(unknown_art_ac11cc);
        unknown_art_ac11cc = 0;
        operator delete(unknown_art_ac11d0);
        unknown_art_ac11d0 = 0;
        Unknown_0051A270();
    }
}


// void Unknown_Reload_Art_Set_006F31B0() -- 0x006F31B0
// Frees the second shell set when loaded, then loads it again (hypothesis).
// Once (flag 0x00AC119C): loads ten shapes, three art pairs and runs 0x006F3E70 with the two
// values at 0x008523E0 (hypothesis).

void *__fastcall Unknown_00495270(char const *name, bool *allocated);
void __fastcall unknown_load_art_6f1fa0(char const *name, void **shape, void **data);   // 0x006F1FA0
void __fastcall Unknown_0051A2B0();
void __fastcall Unknown_006F3E70(int arg1, int arg2);
extern bool unknown_flag_ac119c;
extern int unknown_value_8523e0;
extern int unknown_value_8523e4;
extern char const * const unknown_name_7f87b0;
extern bool unknown_flag_ac122c;
extern void *unknown_art_ac10b8;
extern char const * const unknown_name_7f87b4;
extern bool unknown_flag_ac122d;
extern void *unknown_art_ac0fec;
extern char const * const unknown_name_7f87b8;
extern bool unknown_flag_ac122e;
extern void *unknown_art_ac10b4;
extern char const * const unknown_name_7f87bc;
extern bool unknown_flag_ac122f;
extern void *unknown_art_ac1128;
extern char const * const unknown_name_7f87c0;
extern bool unknown_flag_ac1230;
extern void *unknown_art_ac1010;
extern char const * const unknown_name_7f87c4;
extern bool unknown_flag_ac1231;
extern void *unknown_art_ac10e8;
extern char const * const unknown_name_7f87c8;
extern bool unknown_flag_ac1232;
extern void *unknown_art_ac107c;
extern char const * const unknown_name_7f87cc;
extern bool unknown_flag_ac1233;
extern void *unknown_art_ac1044;
extern char const * const unknown_name_7f87d0;
extern bool unknown_flag_ac1234;
extern void *unknown_art_ac10d8;
extern char const * const unknown_name_7f87d4;
extern bool unknown_flag_ac1235;
extern void *unknown_art_ac1060;
extern char const * const unknown_name_7f871c;
extern void *unknown_art_ac1184;
extern void *unknown_art_ac1188;
extern char const * const unknown_name_7f8720;
extern void *unknown_art_ac118c;
extern void *unknown_art_ac1190;
extern char const * const unknown_name_7f8724;
extern void *unknown_art_ac1194;
extern void *unknown_art_ac1198;
void __fastcall Unknown_006F4160();
void __fastcall Unknown_6F42D0();
void __fastcall Unknown_0051A270();
extern void *unknown_art_ac11b4;
extern void *unknown_art_ac11b8;
extern void *unknown_art_ac11bc;
extern void *unknown_art_ac11c0;
extern void *unknown_art_ac11c4;
extern void *unknown_art_ac11c8;
extern void *unknown_art_ac11cc;
extern void *unknown_art_ac11d0;

void __fastcall Unknown_Reload_Art_Set_006F31B0()
{
    if (unknown_flag_ac119c) {
        unknown_flag_ac119c = false;
        Unknown_006F4160();
        Unknown_6F42D0();
        operator delete(unknown_art_ac11b4);
        unknown_art_ac11b4 = 0;
        operator delete(unknown_art_ac11b8);
        unknown_art_ac11b8 = 0;
        operator delete(unknown_art_ac11bc);
        unknown_art_ac11bc = 0;
        operator delete(unknown_art_ac11c0);
        unknown_art_ac11c0 = 0;
        operator delete(unknown_art_ac11c4);
        unknown_art_ac11c4 = 0;
        operator delete(unknown_art_ac11c8);
        unknown_art_ac11c8 = 0;
        operator delete(unknown_art_ac11cc);
        unknown_art_ac11cc = 0;
        operator delete(unknown_art_ac11d0);
        unknown_art_ac11d0 = 0;
        Unknown_0051A270();
    }
    if (!unknown_flag_ac119c) {
        Unknown_0051A2B0();
        unknown_art_ac10b8 = Unknown_00495270(unknown_name_7f87b0, &unknown_flag_ac122c);
        unknown_art_ac0fec = Unknown_00495270(unknown_name_7f87b4, &unknown_flag_ac122d);
        unknown_art_ac10b4 = Unknown_00495270(unknown_name_7f87b8, &unknown_flag_ac122e);
        unknown_art_ac1128 = Unknown_00495270(unknown_name_7f87bc, &unknown_flag_ac122f);
        unknown_art_ac1010 = Unknown_00495270(unknown_name_7f87c0, &unknown_flag_ac1230);
        unknown_art_ac10e8 = Unknown_00495270(unknown_name_7f87c4, &unknown_flag_ac1231);
        unknown_art_ac107c = Unknown_00495270(unknown_name_7f87c8, &unknown_flag_ac1232);
        unknown_art_ac1044 = Unknown_00495270(unknown_name_7f87cc, &unknown_flag_ac1233);
        unknown_art_ac10d8 = Unknown_00495270(unknown_name_7f87d0, &unknown_flag_ac1234);
        unknown_art_ac1060 = Unknown_00495270(unknown_name_7f87d4, &unknown_flag_ac1235);
        unknown_load_art_6f1fa0(unknown_name_7f871c, &unknown_art_ac1184, (void **)&unknown_art_ac1188);
        unknown_load_art_6f1fa0(unknown_name_7f8720, &unknown_art_ac118c, (void **)&unknown_art_ac1190);
        unknown_load_art_6f1fa0(unknown_name_7f8724, &unknown_art_ac1194, (void **)&unknown_art_ac1198);
        Unknown_006F3E70(unknown_value_8523e0, unknown_value_8523e4);
        unknown_flag_ac119c = true;
    }
}


// void __fastcall Unknown_Reset_Art_006F33B0(int width, int height) -- 0x006F33B0
// Frees the eight blocks 0x006F3E70 builds and rebuilds them for the new size (hypothesis).

void __fastcall Unknown_006F3E70(int width, int height);
extern void *unknown_art_ac11b4;
extern void *unknown_art_ac11b8;
extern void *unknown_art_ac11bc;
extern void *unknown_art_ac11c0;
extern void *unknown_art_ac11c4;
extern void *unknown_art_ac11c8;
extern void *unknown_art_ac11cc;
extern void *unknown_art_ac11d0;

void __fastcall Unknown_Reset_Art_006F33B0(int width, int height)
{
    operator delete(unknown_art_ac11b4);
    unknown_art_ac11b4 = 0;
    operator delete(unknown_art_ac11b8);
    unknown_art_ac11b8 = 0;
    operator delete(unknown_art_ac11bc);
    unknown_art_ac11bc = 0;
    operator delete(unknown_art_ac11c0);
    unknown_art_ac11c0 = 0;
    operator delete(unknown_art_ac11c4);
    unknown_art_ac11c4 = 0;
    operator delete(unknown_art_ac11c8);
    unknown_art_ac11c8 = 0;
    operator delete(unknown_art_ac11cc);
    unknown_art_ac11cc = 0;
    operator delete(unknown_art_ac11d0);
    unknown_art_ac11d0 = 0;
    Unknown_006F3E70(width, height);
}


// bool Unknown_Get_Art_006f3460() -- 0x006F3460
// Returns the art data at 0x00AC119C (hypothesis).
extern bool unknown_flag_ac119c;

bool Unknown_Get_Art_006f3460()
{
    return unknown_flag_ac119c;
}


// void * Unknown_Get_Art_006f3470() -- 0x006F3470
// Returns the art data at 0x00AC1184 (hypothesis).
extern void * unknown_art_ac1184;

void * Unknown_Get_Art_006f3470()
{
    return unknown_art_ac1184;
}


// void * Unknown_Get_Art_006f3480() -- 0x006F3480
// Returns the art data at 0x00AC1188 (hypothesis).
extern void * unknown_art_ac1188;

void * Unknown_Get_Art_006f3480()
{
    return unknown_art_ac1188;
}


// void * Unknown_Get_Art_006f3490() -- 0x006F3490
// Returns the art data at 0x00AC118C (hypothesis).
extern void * unknown_art_ac118c;

void * Unknown_Get_Art_006f3490()
{
    return unknown_art_ac118c;
}


// void * Unknown_Get_Art_006f34a0() -- 0x006F34A0
// Returns the art data at 0x00AC1190 (hypothesis).
extern void * unknown_art_ac1190;

void * Unknown_Get_Art_006f34a0()
{
    return unknown_art_ac1190;
}


// void * Unknown_Get_Art_006f34b0() -- 0x006F34B0
// Returns the art data at 0x00AC1194 (hypothesis).
extern void * unknown_art_ac1194;

void * Unknown_Get_Art_006f34b0()
{
    return unknown_art_ac1194;
}


// void * Unknown_Get_Art_006f34c0() -- 0x006F34C0
// Returns the art data at 0x00AC1198 (hypothesis).
extern void * unknown_art_ac1198;

void * Unknown_Get_Art_006f34c0()
{
    return unknown_art_ac1198;
}


// void Unknown_006F3D50() -- 0x006F3D50
// Loads the ten flagged buffers (flags 0x00AC122C..0x00AC1235) of the second shell set (hypothesis).

void *__fastcall Unknown_00495270(char const *name, bool *allocated);
extern char const * const unknown_name_7f87b0;
extern bool unknown_flag_ac122c;
extern void *unknown_art_ac10b8;
extern char const * const unknown_name_7f87b4;
extern bool unknown_flag_ac122d;
extern void *unknown_art_ac0fec;
extern char const * const unknown_name_7f87b8;
extern bool unknown_flag_ac122e;
extern void *unknown_art_ac10b4;
extern char const * const unknown_name_7f87bc;
extern bool unknown_flag_ac122f;
extern void *unknown_art_ac1128;
extern char const * const unknown_name_7f87c0;
extern bool unknown_flag_ac1230;
extern void *unknown_art_ac1010;
extern char const * const unknown_name_7f87c4;
extern bool unknown_flag_ac1231;
extern void *unknown_art_ac10e8;
extern char const * const unknown_name_7f87c8;
extern bool unknown_flag_ac1232;
extern void *unknown_art_ac107c;
extern char const * const unknown_name_7f87cc;
extern bool unknown_flag_ac1233;
extern void *unknown_art_ac1044;
extern char const * const unknown_name_7f87d0;
extern bool unknown_flag_ac1234;
extern void *unknown_art_ac10d8;
extern char const * const unknown_name_7f87d4;
extern bool unknown_flag_ac1235;
extern void *unknown_art_ac1060;

void __fastcall Unknown_006F3D50()
{
    unknown_art_ac10b8 = Unknown_00495270(unknown_name_7f87b0, &unknown_flag_ac122c);
    unknown_art_ac0fec = Unknown_00495270(unknown_name_7f87b4, &unknown_flag_ac122d);
    unknown_art_ac10b4 = Unknown_00495270(unknown_name_7f87b8, &unknown_flag_ac122e);
    unknown_art_ac1128 = Unknown_00495270(unknown_name_7f87bc, &unknown_flag_ac122f);
    unknown_art_ac1010 = Unknown_00495270(unknown_name_7f87c0, &unknown_flag_ac1230);
    unknown_art_ac10e8 = Unknown_00495270(unknown_name_7f87c4, &unknown_flag_ac1231);
    unknown_art_ac107c = Unknown_00495270(unknown_name_7f87c8, &unknown_flag_ac1232);
    unknown_art_ac1044 = Unknown_00495270(unknown_name_7f87cc, &unknown_flag_ac1233);
    unknown_art_ac10d8 = Unknown_00495270(unknown_name_7f87d0, &unknown_flag_ac1234);
    unknown_art_ac1060 = Unknown_00495270(unknown_name_7f87d4, &unknown_flag_ac1235);
}


// void Unknown_Load_Art_Pairs_006F3E30() -- 0x006F3E30
// Loads the three shell art pairs of the second set (hypothesis).
void __fastcall unknown_load_art_6f1fa0(char const *name, void **shape, void **data);   // 0x006F1FA0
extern char const * const unknown_name_7f871c;
extern char const * const unknown_name_7f8720;
extern char const * const unknown_name_7f8724;
extern void *unknown_art_ac1184;
extern void *unknown_art_ac1188;
extern void *unknown_art_ac118c;
extern void *unknown_art_ac1190;
extern void *unknown_art_ac1194;
extern void *unknown_art_ac1198;

void __fastcall Unknown_Load_Art_Pairs_006F3E30()
{
    unknown_load_art_6f1fa0(unknown_name_7f871c, &unknown_art_ac1184, &unknown_art_ac1188);
    unknown_load_art_6f1fa0(unknown_name_7f8720, &unknown_art_ac118c, &unknown_art_ac1190);
    unknown_load_art_6f1fa0(unknown_name_7f8724, &unknown_art_ac1194, &unknown_art_ac1198);
}


// void Unknown_006F4160() -- 0x006F4160
// Frees the ten flagged buffers 0x006F2FB0 loads (hypothesis).

extern bool unknown_flag_ac122c;
extern void *unknown_art_ac10b8;
extern bool unknown_flag_ac122d;
extern void *unknown_art_ac0fec;
extern bool unknown_flag_ac122e;
extern void *unknown_art_ac10b4;
extern bool unknown_flag_ac122f;
extern void *unknown_art_ac1128;
extern bool unknown_flag_ac1230;
extern void *unknown_art_ac1010;
extern bool unknown_flag_ac1231;
extern void *unknown_art_ac10e8;
extern bool unknown_flag_ac1232;
extern void *unknown_art_ac107c;
extern bool unknown_flag_ac1233;
extern void *unknown_art_ac1044;
extern bool unknown_flag_ac1234;
extern void *unknown_art_ac10d8;
extern bool unknown_flag_ac1235;
extern void *unknown_art_ac1060;

void __fastcall Unknown_006F4160()
{
    if (unknown_flag_ac122c) {
        operator delete(unknown_art_ac10b8);
        unknown_flag_ac122c = false;
    }
    unknown_art_ac10b8 = 0;
    if (unknown_flag_ac122d) {
        operator delete(unknown_art_ac0fec);
        unknown_flag_ac122d = false;
    }
    unknown_art_ac0fec = 0;
    if (unknown_flag_ac122e) {
        operator delete(unknown_art_ac10b4);
        unknown_flag_ac122e = false;
    }
    unknown_art_ac10b4 = 0;
    if (unknown_flag_ac122f) {
        operator delete(unknown_art_ac1128);
        unknown_flag_ac122f = false;
    }
    unknown_art_ac1128 = 0;
    if (unknown_flag_ac1230) {
        operator delete(unknown_art_ac1010);
        unknown_flag_ac1230 = false;
    }
    unknown_art_ac1010 = 0;
    if (unknown_flag_ac1231) {
        operator delete(unknown_art_ac10e8);
        unknown_flag_ac1231 = false;
    }
    unknown_art_ac10e8 = 0;
    if (unknown_flag_ac1232) {
        operator delete(unknown_art_ac107c);
        unknown_flag_ac1232 = false;
    }
    unknown_art_ac107c = 0;
    if (unknown_flag_ac1233) {
        operator delete(unknown_art_ac1044);
        unknown_flag_ac1233 = false;
    }
    unknown_art_ac1044 = 0;
    if (unknown_flag_ac1234) {
        operator delete(unknown_art_ac10d8);
        unknown_flag_ac1234 = false;
    }
    unknown_art_ac10d8 = 0;
    if (unknown_flag_ac1235) {
        operator delete(unknown_art_ac1060);
        unknown_flag_ac1235 = false;
    }
    unknown_art_ac1060 = 0;
}


// void Unknown_6F4360() -- 0x006F4360
// Frees the eight art blocks at 0x00AC11B4 and clears their pointers (hypothesis).

extern void *unknown_art_acac11b4;
extern void *unknown_art_acac11b8;
extern void *unknown_art_acac11bc;
extern void *unknown_art_acac11c0;
extern void *unknown_art_acac11c4;
extern void *unknown_art_acac11c8;
extern void *unknown_art_acac11cc;
extern void *unknown_art_acac11d0;

void __fastcall Unknown_6F4360()
{
    operator delete(unknown_art_acac11b4);
    unknown_art_acac11b4 = 0;
    operator delete(unknown_art_acac11b8);
    unknown_art_acac11b8 = 0;
    operator delete(unknown_art_acac11bc);
    unknown_art_acac11bc = 0;
    operator delete(unknown_art_acac11c0);
    unknown_art_acac11c0 = 0;
    operator delete(unknown_art_acac11c4);
    unknown_art_acac11c4 = 0;
    operator delete(unknown_art_acac11c8);
    unknown_art_acac11c8 = 0;
    operator delete(unknown_art_acac11cc);
    unknown_art_acac11cc = 0;
    operator delete(unknown_art_acac11d0);
    unknown_art_acac11d0 = 0;
}


// void Unknown_6F4450() -- 0x006F4450
// Releases the two art pairs 0x006F4C60 loads and the blocks 0x006F5400 frees (hypothesis).

void __fastcall Unknown_006F5150();
void __fastcall Unknown_006F5400();
extern bool unknown_flag_ac11b0;
extern void *unknown_art_ac11a0;
extern UnknownArtData_006f2390 *unknown_art_ac11a4;
extern void *unknown_art_ac11a8;
extern UnknownArtData_006f2390 *unknown_art_ac11ac;

void __fastcall Unknown_6F4450()
{
    Unknown_006F5150();
    if (unknown_art_ac11a0 != 0) {
        operator delete(unknown_art_ac11a0);
        unknown_art_ac11a0 = 0;
    }
    if (unknown_art_ac11a4 != 0) {
        delete unknown_art_ac11a4;
        unknown_art_ac11a4 = 0;
    }
    if (unknown_art_ac11a8 != 0) {
        operator delete(unknown_art_ac11a8);
        unknown_art_ac11a8 = 0;
    }
    if (unknown_art_ac11ac != 0) {
        delete unknown_art_ac11ac;
        unknown_art_ac11ac = 0;
    }
    Unknown_006F5400();
    unknown_flag_ac11b0 = false;
}


// void Unknown_Reload_Art_006F44D0() -- 0x006F44D0
// Releases the two art pairs 0x006F4C60 loads and the blocks 0x006F5400 frees (hypothesis).

void __fastcall Unknown_006F5150();
void __fastcall Unknown_006F4B00();
void __fastcall Unknown_006F4C90(int arg1, int arg2);
extern int unknown_value_8523e0;
extern int unknown_value_8523e4;
void __fastcall unknown_load_art_6f1fa0(char const *name, void **shape, void **data);   // 0x006F1FA0
extern char const * const unknown_name_7f8728;
extern char const * const unknown_name_7f872c;
void __fastcall Unknown_006F5400();
extern bool unknown_flag_ac11b0;
extern void *unknown_art_ac11a0;
extern UnknownArtData_006f2390 *unknown_art_ac11a4;
extern void *unknown_art_ac11a8;
extern UnknownArtData_006f2390 *unknown_art_ac11ac;

void __fastcall Unknown_Reload_Art_006F44D0()
{
    Unknown_006F5150();
    if (unknown_art_ac11a0 != 0) {
        operator delete(unknown_art_ac11a0);
        unknown_art_ac11a0 = 0;
    }
    if (unknown_art_ac11a4 != 0) {
        delete unknown_art_ac11a4;
        unknown_art_ac11a4 = 0;
    }
    if (unknown_art_ac11a8 != 0) {
        operator delete(unknown_art_ac11a8);
        unknown_art_ac11a8 = 0;
    }
    if (unknown_art_ac11ac != 0) {
        delete unknown_art_ac11ac;
        unknown_art_ac11ac = 0;
    }
    Unknown_006F5400();
    unknown_flag_ac11b0 = false;
    Unknown_006F4B00();
    unknown_load_art_6f1fa0(unknown_name_7f8728, &unknown_art_ac11a0, (void **)&unknown_art_ac11a4);
    unknown_load_art_6f1fa0(unknown_name_7f872c, &unknown_art_ac11a8, (void **)&unknown_art_ac11ac);
    Unknown_006F4C90(unknown_value_8523e0, unknown_value_8523e4);
    unknown_flag_ac11b0 = true;
}


// bool Unknown_Get_Art_006f4590() -- 0x006F4590
// Returns the art data at 0x00AC11B0 (hypothesis).
extern bool unknown_flag_ac11b0;

bool Unknown_Get_Art_006f4590()
{
    return unknown_flag_ac11b0;
}


// void * Unknown_Get_Art_006f45e0() -- 0x006F45E0
// Returns the art data at 0x00AC11A0 (hypothesis).
extern void * unknown_art_ac11a0;

void * Unknown_Get_Art_006f45e0()
{
    return unknown_art_ac11a0;
}


// void * Unknown_Get_Art_006f4600() -- 0x006F4600
// Returns the art data at 0x00AC11A8 (hypothesis).
extern void * unknown_art_ac11a8;

void * Unknown_Get_Art_006f4600()
{
    return unknown_art_ac11a8;
}


// void Unknown_006F4B00() -- 0x006F4B00
// Loads the sixteen flagged buffers (flags 0x00AC1236..0x00AC1245) of the second shell set (hypothesis).

void *__fastcall Unknown_00495270(char const *name, bool *allocated);
extern char const * const unknown_name_7f87d8;
extern bool unknown_flag_ac1236;
extern void *unknown_art_ac0ff8;
extern char const * const unknown_name_7f87dc;
extern bool unknown_flag_ac1237;
extern void *unknown_art_ac10c4;
extern char const * const unknown_name_7f87e0;
extern bool unknown_flag_ac1238;
extern void *unknown_art_ac10bc;
extern char const * const unknown_name_7f87e4;
extern bool unknown_flag_ac1239;
extern void *unknown_art_ac105c;
extern char const * const unknown_name_7f87e8;
extern bool unknown_flag_ac123a;
extern void *unknown_art_ac10f8;
extern char const * const unknown_name_7f87ec;
extern bool unknown_flag_ac123b;
extern void *unknown_art_ac0ff0;
extern char const * const unknown_name_7f87f0;
extern bool unknown_flag_ac123c;
extern void *unknown_art_ac1070;
extern char const * const unknown_name_7f87f4;
extern bool unknown_flag_ac123d;
extern void *unknown_art_ac1078;
extern char const * const unknown_name_7f87f8;
extern bool unknown_flag_ac123e;
extern void *unknown_art_ac10ec;
extern char const * const unknown_name_7f87fc;
extern bool unknown_flag_ac123f;
extern void *unknown_art_ac100c;
extern char const * const unknown_name_7f8800;
extern bool unknown_flag_ac1240;
extern void *unknown_art_ac1094;
extern char const * const unknown_name_7f8804;
extern bool unknown_flag_ac1241;
extern void *unknown_art_ac1058;
extern char const * const unknown_name_7f8808;
extern bool unknown_flag_ac1242;
extern void *unknown_art_ac1048;
extern char const * const unknown_name_7f880c;
extern bool unknown_flag_ac1243;
extern void *unknown_art_ac1098;
extern char const * const unknown_name_7f8810;
extern bool unknown_flag_ac1244;
extern void *unknown_art_ac10a4;
extern char const * const unknown_name_7f8814;
extern bool unknown_flag_ac1245;
extern void *unknown_art_ac10b0;

void __fastcall Unknown_006F4B00()
{
    unknown_art_ac0ff8 = Unknown_00495270(unknown_name_7f87d8, &unknown_flag_ac1236);
    unknown_art_ac10c4 = Unknown_00495270(unknown_name_7f87dc, &unknown_flag_ac1237);
    unknown_art_ac10bc = Unknown_00495270(unknown_name_7f87e0, &unknown_flag_ac1238);
    unknown_art_ac105c = Unknown_00495270(unknown_name_7f87e4, &unknown_flag_ac1239);
    unknown_art_ac10f8 = Unknown_00495270(unknown_name_7f87e8, &unknown_flag_ac123a);
    unknown_art_ac0ff0 = Unknown_00495270(unknown_name_7f87ec, &unknown_flag_ac123b);
    unknown_art_ac1070 = Unknown_00495270(unknown_name_7f87f0, &unknown_flag_ac123c);
    unknown_art_ac1078 = Unknown_00495270(unknown_name_7f87f4, &unknown_flag_ac123d);
    unknown_art_ac10ec = Unknown_00495270(unknown_name_7f87f8, &unknown_flag_ac123e);
    unknown_art_ac100c = Unknown_00495270(unknown_name_7f87fc, &unknown_flag_ac123f);
    unknown_art_ac1094 = Unknown_00495270(unknown_name_7f8800, &unknown_flag_ac1240);
    unknown_art_ac1058 = Unknown_00495270(unknown_name_7f8804, &unknown_flag_ac1241);
    unknown_art_ac1048 = Unknown_00495270(unknown_name_7f8808, &unknown_flag_ac1242);
    unknown_art_ac1098 = Unknown_00495270(unknown_name_7f880c, &unknown_flag_ac1243);
    unknown_art_ac10a4 = Unknown_00495270(unknown_name_7f8810, &unknown_flag_ac1244);
    unknown_art_ac10b0 = Unknown_00495270(unknown_name_7f8814, &unknown_flag_ac1245);
}


// void Unknown_006F5150() (0x006F5150)
// Frees the 16 buffers (0x00AC0FF8, 0x00AC10C4, ...) that the loader
// 0x006F4C90's owners flagged (flags 0x00AC1236..0x00AC1245) and clears
// every pointer. Called from 4 sites.

extern bool unknown_owned_ac1236;
extern void *unknown_buffer_ac0ff8;
extern bool unknown_owned_ac1237;
extern void *unknown_buffer_ac10c4;
extern bool unknown_owned_ac1238;
extern void *unknown_buffer_ac10bc;
extern bool unknown_owned_ac1239;
extern void *unknown_buffer_ac105c;
extern bool unknown_owned_ac123a;
extern void *unknown_buffer_ac10f8;
extern bool unknown_owned_ac123b;
extern void *unknown_buffer_ac0ff0;
extern bool unknown_owned_ac123c;
extern void *unknown_buffer_ac1070;
extern bool unknown_owned_ac123d;
extern void *unknown_buffer_ac1078;
extern bool unknown_owned_ac123e;
extern void *unknown_buffer_ac10ec;
extern bool unknown_owned_ac123f;
extern void *unknown_buffer_ac100c;
extern bool unknown_owned_ac1240;
extern void *unknown_buffer_ac1094;
extern bool unknown_owned_ac1241;
extern void *unknown_buffer_ac1058;
extern bool unknown_owned_ac1242;
extern void *unknown_buffer_ac1048;
extern bool unknown_owned_ac1243;
extern void *unknown_buffer_ac1098;
extern bool unknown_owned_ac1244;
extern void *unknown_buffer_ac10a4;
extern bool unknown_owned_ac1245;
extern void *unknown_buffer_ac10b0;

void __fastcall Unknown_006F5150()
{
    if (unknown_owned_ac1236) {
        delete unknown_buffer_ac0ff8;
        unknown_owned_ac1236 = false;
    }
    unknown_buffer_ac0ff8 = 0;
    if (unknown_owned_ac1237) {
        delete unknown_buffer_ac10c4;
        unknown_owned_ac1237 = false;
    }
    unknown_buffer_ac10c4 = 0;
    if (unknown_owned_ac1238) {
        delete unknown_buffer_ac10bc;
        unknown_owned_ac1238 = false;
    }
    unknown_buffer_ac10bc = 0;
    if (unknown_owned_ac1239) {
        delete unknown_buffer_ac105c;
        unknown_owned_ac1239 = false;
    }
    unknown_buffer_ac105c = 0;
    if (unknown_owned_ac123a) {
        delete unknown_buffer_ac10f8;
        unknown_owned_ac123a = false;
    }
    unknown_buffer_ac10f8 = 0;
    if (unknown_owned_ac123b) {
        delete unknown_buffer_ac0ff0;
        unknown_owned_ac123b = false;
    }
    unknown_buffer_ac0ff0 = 0;
    if (unknown_owned_ac123c) {
        delete unknown_buffer_ac1070;
        unknown_owned_ac123c = false;
    }
    unknown_buffer_ac1070 = 0;
    if (unknown_owned_ac123d) {
        delete unknown_buffer_ac1078;
        unknown_owned_ac123d = false;
    }
    unknown_buffer_ac1078 = 0;
    if (unknown_owned_ac123e) {
        delete unknown_buffer_ac10ec;
        unknown_owned_ac123e = false;
    }
    unknown_buffer_ac10ec = 0;
    if (unknown_owned_ac123f) {
        delete unknown_buffer_ac100c;
        unknown_owned_ac123f = false;
    }
    unknown_buffer_ac100c = 0;
    if (unknown_owned_ac1240) {
        delete unknown_buffer_ac1094;
        unknown_owned_ac1240 = false;
    }
    unknown_buffer_ac1094 = 0;
    if (unknown_owned_ac1241) {
        delete unknown_buffer_ac1058;
        unknown_owned_ac1241 = false;
    }
    unknown_buffer_ac1058 = 0;
    if (unknown_owned_ac1242) {
        delete unknown_buffer_ac1048;
        unknown_owned_ac1242 = false;
    }
    unknown_buffer_ac1048 = 0;
    if (unknown_owned_ac1243) {
        delete unknown_buffer_ac1098;
        unknown_owned_ac1243 = false;
    }
    unknown_buffer_ac1098 = 0;
    if (unknown_owned_ac1244) {
        delete unknown_buffer_ac10a4;
        unknown_owned_ac1244 = false;
    }
    unknown_buffer_ac10a4 = 0;
    if (unknown_owned_ac1245) {
        delete unknown_buffer_ac10b0;
        unknown_owned_ac1245 = false;
    }
    unknown_buffer_ac10b0 = 0;
}


// void Unknown_006F5390() (0x006F5390)
// Frees the two loaded art slots (hypothesis): a raw block (0x00AC11A0, 0x00AC11A8)
// and an owned object (0x00AC11A4, 0x00AC11AC) each, clearing the pointers.

struct UnknownOwnedObject {
    virtual ~UnknownOwnedObject();
};
extern void *unknown_block_ac11a0;
extern UnknownOwnedObject *unknown_object_ac11a4;
extern void *unknown_block_ac11a8;
extern UnknownOwnedObject *unknown_object_ac11ac;

void __fastcall Unknown_006F5390()
{
    if (unknown_block_ac11a0 != 0) {
        delete unknown_block_ac11a0;
        unknown_block_ac11a0 = 0;
    }
    if (unknown_object_ac11a4 != 0) {
        delete unknown_object_ac11a4;
        unknown_object_ac11a4 = 0;
    }
    if (unknown_block_ac11a8 != 0) {
        delete unknown_block_ac11a8;
        unknown_block_ac11a8 = 0;
    }
    if (unknown_object_ac11ac != 0) {
        delete unknown_object_ac11ac;
        unknown_object_ac11ac = 0;
    }
}


// void Unknown_6F5400() -- 0x006F5400
// Frees the sixteen art blocks at 0x00AC11D4 and clears their pointers (hypothesis).

extern void *unknown_art_acac11d4;
extern void *unknown_art_acac11d8;
extern void *unknown_art_acac11dc;
extern void *unknown_art_acac11e0;
extern void *unknown_art_acac11e4;
extern void *unknown_art_acac11e8;
extern void *unknown_art_acac11ec;
extern void *unknown_art_acac11f0;
extern void *unknown_art_acac11f4;
extern void *unknown_art_acac11f8;
extern void *unknown_art_acac11fc;
extern void *unknown_art_acac1200;
extern void *unknown_art_acac1204;
extern void *unknown_art_acac1208;
extern void *unknown_art_acac120c;
extern void *unknown_art_acac1210;

void __fastcall Unknown_6F5400()
{
    operator delete(unknown_art_acac11d4);
    unknown_art_acac11d4 = 0;
    operator delete(unknown_art_acac11d8);
    unknown_art_acac11d8 = 0;
    operator delete(unknown_art_acac11dc);
    unknown_art_acac11dc = 0;
    operator delete(unknown_art_acac11e0);
    unknown_art_acac11e0 = 0;
    operator delete(unknown_art_acac11e4);
    unknown_art_acac11e4 = 0;
    operator delete(unknown_art_acac11e8);
    unknown_art_acac11e8 = 0;
    operator delete(unknown_art_acac11ec);
    unknown_art_acac11ec = 0;
    operator delete(unknown_art_acac11f0);
    unknown_art_acac11f0 = 0;
    operator delete(unknown_art_acac11f4);
    unknown_art_acac11f4 = 0;
    operator delete(unknown_art_acac11f8);
    unknown_art_acac11f8 = 0;
    operator delete(unknown_art_acac11fc);
    unknown_art_acac11fc = 0;
    operator delete(unknown_art_acac1200);
    unknown_art_acac1200 = 0;
    operator delete(unknown_art_acac1204);
    unknown_art_acac1204 = 0;
    operator delete(unknown_art_acac1208);
    unknown_art_acac1208 = 0;
    operator delete(unknown_art_acac120c);
    unknown_art_acac120c = 0;
    operator delete(unknown_art_acac1210);
    unknown_art_acac1210 = 0;
}


// void Unknown_Reload_Art_Buffers_006F5530() -- 0x006F5530
// Frees the flagged buffers and loads the second set (hypothesis).

void __fastcall Unknown_006F5150();
void __fastcall Unknown_006F4B00();

void __fastcall Unknown_Reload_Art_Buffers_006F5530()
{
    Unknown_006F5150();
    Unknown_006F4B00();
}


// void __fastcall Unknown_Reset_Art_006F55D0(int width, int height) -- 0x006F55D0
// Frees the art blocks and rebuilds the layout for the new size (hypothesis).

void __fastcall Unknown_6F5400();
void __fastcall Unknown_006F4C90(int width, int height);

void __fastcall Unknown_Reset_Art_006F55D0(int width, int height)
{
    Unknown_6F5400();
    Unknown_006F4C90(width, height);
}
