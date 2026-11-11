// NON-EXACT DRAFT 0x6F7360 FUN_006f7360
// entry 0x6F7360; exact-match score 54.2 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout1001 (build/class-work/Scout1001/keep/u_7360iN.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// @ref ??0?$VectorClass@PBD@@QAE@IPBQBD@Z=006f8b90
// @ref ?unknown_find_name_index@@YIHABV?$DynamicVectorClass@PBD@@PBD@Z=006f6c90
// @ref ?Add@?$DynamicVectorClass@PBD@@QAE_NABQBD@Z=006f8b40
// @ref ?Technos@@3V?$DynamicVectorClass@PAVTechnoClass@@@@A=00a40c20
// @ref ?Unknown_004F52B0@HouseClass@@QAE_NXZ=004f52b0
// @ref ?unknown_flag_ac133c@@3_NA=00ac133c
// @ref ?unknown_game_mode_a3d298@@3HA=00a3d298
// @ref ?unknown_objects_a40c60@@3V?$DynamicVectorClass@PAVObjectClass@@@@A=00a40c60
// @ref _wcscpy=00783689
// @ref ?unknown_int_a3d548@@3HA=00a3d548
// @ref ?unknown_state_ac132c@@3HA=00ac132c
// @ref ?unknown_selected_83996c@@3PAUUnknownSelection@@A=0083996c
// @ref ?Unknown_006AAD40@UnknownSelection@@QAEXXZ=006aad40
// @ref ?Unknown_006AAD70@UnknownSelection@@QAEXXZ=006aad70
// @ref ?Count@UnknownSelection@@QAEHXZ=006ac7b0
// @ref ?Get@UnknownSelection@@QAEPAVObjectClass@@H@Z=006ac7c0
// @ref ?Unknown_006F58E0@@YIXPBGH@Z=006f58e0
// @ref ?Add@?$DynamicVectorClass@PAVTechnoClass@@@@QAE_NABQAVTechnoClass@@@Z=004137b0
// @ref ??0?$DynamicVectorClass@PAVTechnoClass@@@@QAE@IPBQAVTechnoClass@@@Z=006f8a80
// @ref ??0?$VectorClass@PAVTechnoClass@@@@QAE@IPBQAVTechnoClass@@@Z=0040bc10
// @ref ??2@YAPAXI@Z=00782017
// @ref ??_7?$DynamicVectorClass@PAVTechnoClass@@@@6B@=0079a79c
// @ref ?unknown_flag_ac1330@@3_NA=00ac1330
// @ref ?unknown_matches_any_name@@YI_NPAVTechnoClass@@ABV?$DynamicVectorClass@PBD@@@Z=006f7610
// @ref ?Unknown_006F6790@@YIXPAG@Z=006f6790
#include "shared/uicmnds.hpp"
#include "shared/unknown_globals.hpp"

extern int unknown_state_ac132c;
bool __fastcall unknown_matches_any_name(TechnoClass *techno, DynamicVectorClass<char const *> const &names);
void __fastcall Unknown_006F58E0(wchar_t const *text, int delay);
void __fastcall Unknown_006F6790(wchar_t *buffer);
int __fastcall Unknown_006F6FE0(TechnoClass *techno, int argument);
DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F6930(UnknownTest test, int argument);
DynamicVectorClass<TechnoClass *> *__fastcall Unknown_006F69F0(UnknownTest test, intptr_t argument);   // NATIVE: pointer-sized argument (Scout1242)
DynamicVectorClass<char const *> *Unknown_006F76C0();

// Whether no techno of the list with one of the names is on the screen (+0x77 clear).
static bool None_Named_On_Screen(DynamicVectorClass<TechnoClass *> *list, DynamicVectorClass<char const *> *names)
{
    int count = list->Count();
    for (int index = 0; index < count; index++) {
        TechnoClass *techno = (*list)[index];
        if (unknown_matches_any_name(techno, *names) && !techno->unknown_77) {
            return false;
        }
    }
    return true;
}

// Calls slot 80 on the techno when its type is one of the names.
static void Select_If_Named(TechnoClass *techno, DynamicVectorClass<char const *> *names)
{
    if (unknown_matches_any_name(techno, *names)) {
        techno->unknown_slot80();
    }
}

void __fastcall Unknown_006F7360()
{
    if (unknown_int_a3d548 != 0) {
        return;
    }
    unknown_selected_83996c->Unknown_006AAD70();
    if (unknown_state_ac132c != 2) {
        unknown_flag_ac133c = false;
    }
    DynamicVectorClass<char const *> *names = Unknown_006F76C0();
    DynamicVectorClass<TechnoClass *> *list;
    if (unknown_flag_ac133c) {
        list = Unknown_006F6930((UnknownTest)Unknown_006F6FE0, 0);
    } else {
        list = Unknown_006F69F0((UnknownTest)Unknown_006F6FE0, 0);
        if (None_Named_On_Screen(list, names)) {
            unknown_flag_ac133c = true;
        }
        if (unknown_flag_ac133c) {
            delete list;
            list = Unknown_006F6930((UnknownTest)Unknown_006F6FE0, 0);
        }
    }
    int total = list->Count();
    for (int item = 0; item < total; item++) {
        Select_If_Named((*list)[item], names);
    }
    wchar_t buffer[256];
    Unknown_006F6790(buffer);
    Unknown_006F58E0(buffer, -1);
    unknown_state_ac132c = 2;
    unknown_flag_ac1330 = false;
    delete names;
    delete list;
}
