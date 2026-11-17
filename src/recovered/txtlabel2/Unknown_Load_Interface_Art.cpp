// void Unknown_Load_Interface_Art_006F1B60() (0x006F1B60)
// Loads the interface art (hypothesis): four shapes by name into their slots,
// two blocks with their objects, and, once (flag 0x00AC119C), ten more shapes,
// three more blocks and the final 0x006F3E70 call with the two values at 0x008523E0.
// Anything loaded before is released by 0x006F5390 / 0x006F5540.

struct UnknownOwnedObject {
    virtual ~UnknownOwnedObject();
};
void *__fastcall Unknown_00495270(char const *name, bool *found);
void __fastcall Unknown_006F1FA0(char const *name, void **shape, void **data);   // 0x006F1FA0
void __fastcall Unknown_0051A2B0();
void __fastcall Unknown_006F3E70(int arg1, int arg2);
extern bool unknown_flag_ac119c;
extern int VideoModeWidth;
extern int VideoModeHeight;
extern UnknownOwnedObject *unknown_object_ac1134;
extern UnknownOwnedObject *unknown_object_ac113c;
extern UnknownOwnedObject *unknown_object_ac1188;
extern UnknownOwnedObject *unknown_object_ac1190;
extern UnknownOwnedObject *unknown_object_ac1198;
extern bool unknown_flag_ac1224;
extern bool unknown_flag_ac1225;
extern bool unknown_flag_ac1226;
extern bool unknown_flag_ac1227;
extern bool unknown_flag_ac122c;
extern bool unknown_flag_ac122d;
extern bool unknown_flag_ac122e;
extern bool unknown_flag_ac122f;
extern bool unknown_flag_ac1230;
extern bool unknown_flag_ac1231;
extern bool unknown_flag_ac1232;
extern bool unknown_flag_ac1233;
extern bool unknown_flag_ac1234;
extern bool unknown_flag_ac1235;
extern char const *unknown_name_7f86e8;
extern char const *unknown_name_7f86ec;
extern char const *unknown_name_7f871c;
extern char const *unknown_name_7f8720;
extern char const *unknown_name_7f8724;
extern char const *unknown_name_7f8740;
extern char const *unknown_name_7f8744;
extern char const *unknown_name_7f8748;
extern char const *unknown_name_7f874c;
extern char const *unknown_name_7f87b0;
extern char const *unknown_name_7f87b4;
extern char const *unknown_name_7f87b8;
extern char const *unknown_name_7f87bc;
extern char const *unknown_name_7f87c0;
extern char const *unknown_name_7f87c4;
extern char const *unknown_name_7f87c8;
extern char const *unknown_name_7f87cc;
extern char const *unknown_name_7f87d0;
extern char const *unknown_name_7f87d4;
extern void *unknown_art_ac0fec;
extern void *unknown_art_ac1010;
extern void *unknown_art_ac1044;
extern void *unknown_art_ac1060;
extern void *unknown_art_ac107c;
extern void *unknown_art_ac10b4;
extern void *unknown_art_ac10b8;
extern void *unknown_art_ac10c0;
extern void *unknown_art_ac10d8;
extern void *unknown_art_ac10e8;
extern void *unknown_art_ac1128;
extern void *unknown_art_ac1218;
extern void *unknown_art_ac121c;
extern void *unknown_art_ac1220;
extern void *unknown_block_ac1130;
extern void *unknown_block_ac1138;
extern void *unknown_block_ac1184;
extern void *unknown_block_ac118c;
extern void *unknown_block_ac1194;

void __fastcall Unknown_Load_Interface_Art_006F1B60()
{
    unknown_art_ac1218 = Unknown_00495270(unknown_name_7f8740, &unknown_flag_ac1224);
    unknown_art_ac121c = Unknown_00495270(unknown_name_7f8744, &unknown_flag_ac1225);
    unknown_art_ac1220 = Unknown_00495270(unknown_name_7f8748, &unknown_flag_ac1226);
    unknown_art_ac10c0 = Unknown_00495270(unknown_name_7f874c, &unknown_flag_ac1227);
    Unknown_006F1FA0(unknown_name_7f86e8, &unknown_block_ac1130, (void **)&unknown_object_ac1134);
    Unknown_006F1FA0(unknown_name_7f86ec, &unknown_block_ac1138, (void **)&unknown_object_ac113c);
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
        Unknown_006F1FA0(unknown_name_7f871c, &unknown_block_ac1184, (void **)&unknown_object_ac1188);
        Unknown_006F1FA0(unknown_name_7f8720, &unknown_block_ac118c, (void **)&unknown_object_ac1190);
        Unknown_006F1FA0(unknown_name_7f8724, &unknown_block_ac1194, (void **)&unknown_object_ac1198);
        Unknown_006F3E70(VideoModeWidth, VideoModeHeight);
        unknown_flag_ac119c = true;
    }
}
