
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3): FUN_00752760 (YR 0x00752760), the EVA speech service, is
// unmatched in the YR tree. Written from gamemd.exe's code: when audio runs, the speech stream
// is idle and its 500 ms gap since the last line has passed, it takes the next queued line (the
// two priority lists, then a held node, then the four type lists from the last), marks the
// previous and the new voice played (state 2), and streams the voice's file for the player's
// side (Files[2] side 0, Files[1] side 1, Files[0] otherwise) with ".WAV". Behaviour code for
// the native build only.
// ---------------------------------------------------------------------------------------------
void FUN_00752760(void)
{
	if (DAT_00A8ED64 || !FUN_00407000() || DAT_00B1D4CC == NULL || DAT_00B1D4CC->FUN_00408070()) {
		return;
	}
	if (FUN_004093B0() <= DAT_00B1D4CC->FUN_00408140() + DAT_00B1D4D0 || DAT_00B1D428) {
		return;
	}

	if (DAT_00B1D4C4 != NULL) {
		DAT_00B1D4C4->State = 2;
		DAT_00B1D4C4 = NULL;
	}

	VoxNode * node = (VoxNode *)DAT_00B1D3C8.FUN_004073C0();
	if (node == NULL) {
		node = (VoxNode *)DAT_00B1D3F0.FUN_004073C0();
	}
	if (node != NULL) {
		node->FUN_00407450();
		if (DAT_00B1D4B8 != NULL) {
			operator delete(DAT_00B1D4B8);
			DAT_00B1D4B8 = NULL;
		}
	} else if (DAT_00B1D4B8 != NULL) {
		node = DAT_00B1D4B8;
		DAT_00B1D4B8 = NULL;
	} else {
		for (int list = 3; list >= 0 && node == NULL; list--) {
			node = (VoxNode *)DAT_00B1D450[list].FUN_004073C0();
		}
		if (node == NULL) {
			return;
		}
		node->FUN_00407450();
	}

	VoxClass * vox = node->Vox;
	int type = node->Type;
	int priority = node->Priority;
	operator delete(node);
	if (vox == NULL) {
		return;
	}

	vox->State = 2;
	char const * file = DAT_00B1D4C8 == 0 ? vox->Files[2] : (DAT_00B1D4C8 == 1 ? vox->Files[1] : vox->Files[0]);
	char name[256];
	strcpy(name, file);
	strcat(name, ".WAV");
	if (FUN_00407B60(DAT_00B1D4CC, name, 1)) {
		DAT_00B1D4D0 = 500;
		DAT_00B1D4C4 = vox;
		DAT_00B1D3B8 = type;
		DAT_00B1D3E0 = priority;
		vox->State = 0;
	}
}
