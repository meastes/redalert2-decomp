// Behaviour row (Scout1209 r5; NOT EXACT, native/behaviour_rewrites.json): selecting
// every unit of a type from the map.
#include "shared/uicmnds.hpp"
#include "shared/techno.hpp"

// 0x006F7020 (decompile and asm): the player's double click on one of his units
// (DisplayClass::Mouse_Left_Release 0x0049BB20, action 8, when the flag 0x00AC133D is
// set) selects every player-side techno of that type name (0x006F7190): from all technos
// (0x00A40C20) when the flag 0x00AC133C is set, else from the objects the selection list
// at 0x0083996C holds (flag +0x14 bit 0, as 0x006F69F0). Each one found gets slot 81
// (+0x144). The list is built, walked and deleted.
// NOT EXACT: the original expands 0x006F69F0's loop inline; the native body calls the
// test directly with the name pointer (calling it through UnknownTest, whose argument
// is an int, would cut the pointer on arm64).
void __fastcall Unknown_006F7020(char const *id)
{
    DynamicVectorClass<TechnoClass *> *list = new DynamicVectorClass<TechnoClass *>(0, 0);
    if (unknown_flag_ac133c) {
        int count = Technos.Count();
        for (int index = 0; index < count; index++) {
            TechnoClass *techno = Technos[index];
            if ((char)Unknown_006F7190(techno, id)) list->Add(techno);
        }
    } else {
        int count = unknown_selected_83996c->Count();
        for (int index = 0; index < count; index++) {
            ObjectClass *object = unknown_selected_83996c->Get(index);
            if (object == 0 || !object->AbstractFlags.IsTechno) continue;
            TechnoClass *techno = (TechnoClass *)object;
            if ((char)Unknown_006F7190(techno, id)) list->Add(techno);
        }
    }
    int total = list->Count();
    for (int index = 0; index < total; index++) (*list)[index]->unknown_slot81();
    delete list;
}

// 0x0069A430 under the name unknown_restart_timer 0x0069A160 gives it (its own local struct
// for the time source at 0x00ABD4C4): the system clock, timeGetTime() / 16
// (shared/system_timer.hpp). Same body as SystemTimerClass::operator() (Scout1209 r5).
#include "shared/system_timer.hpp"
struct UnknownTimeSource_0069a160 {
    int Value();                            // 0x0069A430
};
int UnknownTimeSource_0069a160::Value()
{
    return (int)(*reinterpret_cast<SystemTimerClass const *>(this))();
}
