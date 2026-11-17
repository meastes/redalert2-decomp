
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3): Attaches_To(TActionType) (gamemd 0x006E3EE0) is unmatched in
// the YR tree. TibSun's text (TibSun archive code/taction.cpp; SPDX GPL-3.0-or-later, Copyright
// 2025 Electronic Arts Inc. and OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md)
// with gamemd.exe's case list: actions 14, 32, 60, 61, 62, 91 and 111 attach to an object (YR's
// 111 is past TibSun's enumeration). Behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------
AttachType Attaches_To(TActionType event)
{
	AttachType attach = ATTACH_NONE;

	switch (event) {
		case TACTION_DESTROY_OBJECT:
		case TACTION_SELL_ATTACHED:
		case TACTION_TURN_OFF_ATTACHED:
		case TACTION_TURN_ON_ATTACHED:
		case TACTION_CHANGE_HOUSE:
		case TACTION_GO_BERZERK:
		case (TActionType)111:	// NATIVE: YR's action 111 (gamemd's table)
			attach = AttachType(attach | ATTACH_OBJECT);
			break;

		default:
			break;
	}
	return(attach);
}
