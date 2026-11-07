// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool InfantryClass::unknown_slot289() -- 0x00508AB0
// TibSun/YR InfantryClass::JumpJet_To_Walk: a grounded jumpjet infantry with a short path left gets a walking
// locomotor that carries the jumpjet one as its piggyback. From ra2-decomp (YR 0x00521EB0): the comip
// `piggy != NULL` tests and Move_To taking the Center_Coord result directly (through CoordStruct's COM
// conversion under RA2_SHIP_MOVE_TO_VALUE). Those comip levels spend the inline budget that sends the
// temporary's release out of line (0x00509860) and keep `this` in ebx, as in the original.
// @ref ?Release_Held@?$UnknownComPtr@UIPiggyback@@$1?unknown_uuid_IPiggyback_7cfc30@@3UGUID@@B@@QAEXXZ=00509860
// @ref _CLSID_JumpjetLocomotionClass=007a2000
// @ref ??0UnknownPersistPtr@@QAE@ABV?$UnknownComPtr@UILocomotion@@$1?unknown_uuid_ILocomotion_7ce600@@3UGUID@@B@@@Z=00509870
// @ref ??1UnknownPersistPtr@@QAE@XZ=004a4d00
// @ref _unknown_Walk_clsid_007a1fa0=007a1fa0
// @ref ?_com_issue_error@@YGXJ@Z=00795940
// @ref ?Query@?$UnknownComPtr@UIPiggyback@@$1?unknown_uuid_IPiggyback_7cfc30@@3UGUID@@B@@QAEJABV?$UnknownComPtr@UILocomotion@@$1?unknown_uuid_ILocomotion_7ce600@@3UGUID@@B@@@Z=00454c70
// @ref ??4?$UnknownComPtr@UIPiggyback@@$1?unknown_uuid_IPiggyback_7cfc30@@3UGUID@@B@@QAEAAV0@PAUIPiggyback@@@Z=00453e70
#include "shared/infantry.hpp"
#include "shared/infantry_piggyback_ptr.hpp"

extern "C" GUID const unknown_Walk_clsid_007a1fa0;     // CLSID_WalkLocomotion
extern "C" GUID const CLSID_JumpjetLocomotionClass;

bool InfantryClass::unknown_slot289()
{
    int path_length = 0;
    if (unknown_494 != 0) {
        for (int i = 0; i < 24; i++) {
            if (Path[i] == -1) break;
            if (Path[i] == 8) break;
            path_length++;
        }
    }
    if (path_length >= 4) return false;

    if (unknown_508d00()) {
        UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30> piggy(Locomotor);
        if (piggy != 0 && !piggy.Checked()->Is_Piggybacking()) {
            UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> walk(unknown_Walk_clsid_007a1fa0);
            walk.Checked()->Link_To_Object(this);
            piggy = UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>(walk);
            if (piggy != 0) {
                Path[0] = -1;
                piggy.Checked()->Begin_Piggyback(Locomotor);
                Locomotor = walk;
                Locomotor.Checked()->Move_To(unknown_494->Center_Coord());
                return true;
            }
        }
    }

    return false;
}
