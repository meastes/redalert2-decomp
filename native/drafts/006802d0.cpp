// NON-EXACT DRAFT 0x6802D0 SidebarClass::StripClass::Unknown_006802D0 (Recalc)
// entry 0x6802D0; exact-match score 35.99 (misses_index best); plan "draft, verify"
// origin lane: Sidebar361 (build/class-work/Sidebar361/w/rc_0.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1089 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1089): the overlay w/hdr names replaced by HEAD's; each changed line is marked NATIVE.
// Best recorded 35.99 (Sidebar361, with its private header overlay w/hdr; later lanes 30.30/26.72 rebaselines); TS Recalc shape. Low score: behaviour follows TibSun sidebar.cpp StripClass::Recalc.
// bool SidebarClass::StripClass::Unknown_006802D0() (0x006802D0)
// Recalc: drops the icons the player can no longer build (abandoning their
// production), refreshes the tab, and keeps the first visible icon in view.
#include "shared/sidebar.hpp"
#include "shared/sidebar_screen.hpp"
#include "shared/event.hpp"
#include "shared/super.hpp"
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/factory_class_decl.hpp"
// @ref ?Unknown_004E3660@UnknownSidebarHouse@@QAEHPAVTechnoTypeClass@@HH@Z=004e3660
// @ref ?Unknown_004EB580@UnknownSidebarHouse@@QAEPAVFactoryClass@@H_NH@Z=004eb580
// @ref _memmove=00783290

extern "C" void *__cdecl memmove(void *dest, void const *source, unsigned int count);

// NATIVE: the overlay's UnknownBuildable30 operator== (TibSun sidebar.h BuildType::operator==:
// the same id and type).
static inline bool Same_Buildable(UnknownBuildable30 const &a, UnknownBuildable30 const &b)
{
    return a.unknown_00 == b.unknown_00 && a.unknown_04 == b.unknown_04;
}

bool SidebarClass::StripClass::Unknown_006802D0()
{
    if (Debug_Map || !BuildableCount) {
        return false;
    }
    bool scroll = false;
    int max_visible = Max_Visible_Rows() * 2;
    UnknownBuildable30 *unshifted = new UnknownBuildable30[max_visible];
    for (int i = 0; i < max_visible; i++) {
        if (i + unknown_40 * 2 < BuildableCount) {
            unshifted[i] = Buildables[i + unknown_40 * 2];
        }
    }

    bool redraw = false;
    for (int index = 0; index < BuildableCount; index++) {
        int ok;
        TechnoTypeClass *tech = (TechnoTypeClass *)Unknown_00482C00(Buildables[index].unknown_04, Buildables[index].unknown_00);
        if (tech) {
            TechnoClass *who = (TechnoClass *)tech->Who_Can_Build_Me(true, false, false, unknown_player_a35db4);
            ok = who && who->House->Can_Build(tech, false, true);   // NATIVE: the overlay's UnknownSidebarHouse::Unknown_004E3660 is HouseClass::Can_Build 0x004E3660
        } else {
            if ((unsigned)Buildables[index].unknown_00 < (unsigned)unknown_player_a35db4->Supers.Count()) {
                ok = unknown_player_a35db4->Supers[Buildables[index].unknown_00]->Is_Present();
            } else {
                ok = false;
            }
        }
        if (!ok) {
            if (Buildables[index].Factory) {
                OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 16, Buildables[index].unknown_04,
                    Buildables[index].unknown_00, tech->Naval));
                if (Buildables[index].unknown_04 == 7 || Buildables[index].unknown_04 == 6) {
                    Map.unknown_11a0 = 0;
                    Map.unknown_11a4 = 0;
                    Map.unknown_11a8 = -1;
                    Map.Unknown_00498F10(0);
                }
            }
            if (tech) {
                FactoryClass *factory = unknown_player_a35db4->unknown_func_4eb580(tech->Kind_Of(), tech->Naval, 0);   // NATIVE: 0x004EB580 under house_screen.hpp's name
                if (factory) {
                    for (int q = factory->Queue.Count() - 1; q >= 0; q--) {
                        if (factory->Queue[q] == tech) {
                            OutList.Add(EventClass(unknown_player_a35db4->ArrayIndex, 16, Buildables[index].unknown_04,
                                Buildables[index].unknown_00, tech->Naval));
                        }
                    }
                }
            }
            for (int i = 0; i < max_visible; i++) {
                if (Same_Buildable(unshifted[i], Buildables[index])) {   // NATIVE: the overlay's operator==
                    unshifted[i] = UnknownBuildable30();
                }
            }
            if (BuildableCount > 1 && index < BuildableCount - 1) {
                memmove(&Buildables[index], &Buildables[index + 1], sizeof(Buildables[0]) * ((BuildableCount - index) - 1));
            }
            redraw = true;
            scroll = true;
            Buildables[BuildableCount - 1].Factory = 0;
            Buildables[BuildableCount - 1].unknown_00 = 0;
            Buildables[BuildableCount - 1].unknown_04 = 0;
            Buildables[BuildableCount - 1].unknown_08 = 0;
            Buildables[BuildableCount - 1].unknown_10 = 0;
            Buildables[BuildableCount - 1].Stage.Set_Rate(0);
            Buildables[BuildableCount - 1].Stage.Value = 0;
            Buildables[BuildableCount - 1].unknown_2c = 0;
            unknown_38 = true;
            BuildableCount--;
            index--;
        }
    }

    Map.Unknown_0067CA50(unknown_34);

    if (scroll) {
        bool got_old = false;
        bool got_new = false;
        int oldpos;
        for (oldpos = 0; oldpos < max_visible; oldpos++) {
            if (unshifted[oldpos].unknown_00 != 0 || unshifted[oldpos].unknown_04 != 0) {
                got_old = true;
                break;
            }
        }
        int newpos;
        if (got_old && BuildableCount != 0) {
            for (newpos = 0; newpos < BuildableCount; newpos++) {
                if (Same_Buildable(Buildables[newpos], unshifted[oldpos])) {   // NATIVE
                    got_new = true;
                    break;
                }
            }
        }
        if (got_old && got_new) {
            unknown_40 -= (oldpos - newpos) / 2;
            int last = (BuildableCount - max_visible) / 2;
            unknown_40 = 0 > (unknown_40 < last ? unknown_40 : last) ? 0 : (unknown_40 < last ? unknown_40 : last);
        } else {
            unknown_40 = 0;
        }
        Map.unknown_4ee3 = true;
    }
    delete [] unshifted;
    return redraw;
}
