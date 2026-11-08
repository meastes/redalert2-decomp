// NON-EXACT DRAFT 0x6F71E0 FUN_006f71e0
// entry 0x6F71E0; exact-match score 73.33 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout1030 (build/class-work/Scout1030/w/q1.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.

void __fastcall Unknown_006F71E0(char const *name)
{
    bool sound = unknown_sound_enabled_7d88d6;
    unknown_sound_enabled_7d88d6 = false;
    UnknownTest test = (UnknownTest)Unknown_006F7190;
    DynamicVectorClass<TechnoClass *> *list;
    if (unknown_flag_ac133c) {
        list = new DynamicVectorClass<TechnoClass *>(0, 0);
        int count = Technos.Count();
        for (int index = 0; index < count; index++) {
            TechnoClass *techno = Technos[index];
            if (test(techno, (int)name)) {
                list->Add(techno);
            }
        }
    } else {
        list = Unknown_006F69F0(test, (int)name);
    }
    int total = list->Count();
    for (int index = 0; index < total; index++) {
        (*list)[index]->unknown_slot80();
    }
    delete list;
    unknown_sound_enabled_7d88d6 = sound;
}
