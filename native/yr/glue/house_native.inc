
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): functions unmatched in the YR tree, the text TibSun's
// (TibSun archive code/house.cpp; SPDX GPL-3.0-or-later, Copyright 2025 Electronic Arts Inc. and
// OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md); native changes are marked
// NATIVE. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------

void HouseClass::Update_Present_Super_Weapons(void)
{
	if (GameActive) {
		for (int s = 0; s < SuperWeapon.Count(); s++) {
			if (SuperWeapon[s]->Is_Present() && ((SuperWeapon[s]->NeedsBuilding && !SuperWeapon[s]->Is_One_Time()) || IsDefeated)) {
				bool present = false;
				bool enabled = false;
				if (!IsDefeated) {
					for (int j = 0; j < Buildings.Count(); j++) {
						BuildingClass * b = Buildings[j];
						if (!b->IsInLimbo && b->IsActive && b->House->Fetch_ID() == Fetch_ID()) {
							for (int k = 0; k < 3; k++) {
								BuildingTypeClass const * upgrade = b->Upgrades[k];
								if (upgrade != NULL && (upgrade->SuperWeapon == s || upgrade->SuperWeapon2 == s)) {
									present = true;
									if (!enabled) enabled = b->IsOn;
								}
							}
							if (b->Fetch_Super_Weapon() == s || b->Fetch_Super_Weapon2() == s) {
								present = true;
								if (enabled) break;
								enabled = b->IsOn;
							}
							if (enabled && present) break;
						}
					}
				}

				int power = Power;
				int drain = Drain;
				if (power < drain && drain != 0) {
					if (power == 0 || (double)power / (double)drain < 1.0) {
						enabled = false;
					}
				}

				if (present && !IsDefeated) {
					if (!enabled && SuperWeapon[s]->Is_Powered() && SuperWeapon[s]->SetOnHold(true)) {
						if (Fetch_ID() == PlayerPtr->Fetch_ID()) {
							if (Map.IsTargettingMode == s) {
								Map.IsTargettingMode = SUPER_NONE;
							}
							Map.Column[1].Flag_To_Redraw();
						}
						IsRecalcNeeded = true;
					}

					if (enabled && SuperWeapon[s]->SetOnHold(false)) {
						if (Fetch_ID() == PlayerPtr->Fetch_ID()) {
							if (s == Map.IsTargettingMode) {
								Map.IsTargettingMode = SUPER_NONE;
							}
							Map.Column[1].Flag_To_Redraw();
						}
						IsRecalcNeeded = true;
					}
				} else {
					if (SuperWeapon[s]->Lose() && PlayerPtr != NULL) {
						if (Fetch_ID() == PlayerPtr->Fetch_ID()) {
							if (Map.IsTargettingMode == s) {
								Map.IsTargettingMode = SUPER_NONE;
							}
							Map.Column[1].Flag_To_Redraw();
						}
						IsRecalcNeeded = true;
					}
				}
			}
		}
	}
}
