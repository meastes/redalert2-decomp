// void BuildingClass::Sell_Back(int mode) (0x004437A0)
// Sells the building (mission 0x13; hypothesis: Sell_Back): -1 toggles,
// 0 cancels (nothing unless selling) and 1 starts it (not while selling or
// while unknown_5af is set); the sell sound (the rules' +0x62C) plays for a
// player-controlled owner. A building that cannot be sold (unknown_5b9
// clear) only reacts when it is a firestorm wall: its type's slot 33 for the
// owner, the owner's 0x004F52F0, then slots 52 and 60.
#include "shared/building.hpp"
#include "shared/house.hpp"
// @ref ?Rules@@3PAVRulesClass@@A=00839848
// @ref ?Is_Player_Control@HouseClass@@QAE_NXZ=004f52b0
// @ref ?Unknown_004F52F0@HouseClass@@QAE_NXZ=004f52f0
// @ref ?Voc_Play@@YIPAUAudioEvent@@HMHPAUAudioEventHandle@@@Z=00713db0

void BuildingClass::Sell_Back(int mode)
{
    if (unknown_5b9) {
        bool sell = false;
        switch (mode) {
        case -1:
            sell = Get_Mission() != 0x13;
            break;
        case 0:
            if (Get_Mission() != 0x13) {
                return;
            }
            break;
        case 1:
            if (Get_Mission() == 0x13) {
                return;
            }
            if (unknown_5af) {
                return;
            }
            sell = true;
            break;
        }
        if (sell) {
            QueueMission(0x13, false);
            Commence();
        }
        if (House->Is_Player_Control()) {
            Voc_Play(Rules->unknown_62c, 1.0f, 0x2000, 0);
        }
    } else if (Type->FirestormWall) {
        Type->Cost_Of(House);
        House->Unknown_004F52F0();
        Limbo();
        Delete_Me();
    }
}
