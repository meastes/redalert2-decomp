
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3), appended to the sidebar unit: SidebarClass::Reposition_Sidebar,
// YR 0x006ABD30 (1,235 bytes, vtable slot 34, unmatched in ra2-decomp). From the RA2 tree's twin
// (src/recovered/sidebar.cpp, 0x006819A0) with gamemd.exe's changes read from its code: the
// sidebar's x and the button anchors come from FUN_006A5130(false) (Init_IO's layout helper)
// instead of being computed here; the background shapes are SIDEGDI1-3 loaded through
// FileSystem::LoadFile; the button anchors are this unit's DAT_00B0B4DC..DAT_00B0B510; the
// select-button help regions 1000..1059 are all removed before the visible ones are added again;
// the sell help region is the Power button's (ID 102), the 0xC8/0xC9 regions are Waypoint's and
// Upgrade's. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
extern int DAT_00886F90;                          // 0x00886F90 SidebarRect.X
extern int DAT_00B0B4F4;                          // the select buttons' column x
extern int DAT_00B0B508;                          // Upgrade's position
extern int DAT_00B0B50C;
extern int DAT_00B0B510;                          // Waypoint's offset from Upgrade

void SidebarClass::Reposition_Sidebar(void)
{
	PowerClass::Reposition_Sidebar();
	FUN_006A5130(false);

	if (DAT_00B0B468 == NULL) {
		DAT_00B0B468 = (void *)FileSystem::LoadFile("SIDEGDI1.SHP", false);
		DAT_00B0B46C = (void *)FileSystem::LoadFile("SIDEGDI2.SHP", false);
		DAT_00B0B470 = (void *)FileSystem::LoadFile("SIDEGDI3.SHP", false);
	}

	Background.Set_Position(DAT_00886F90 + 0x10, DSurface::ViewBounds.Y);
	Background.Flag_To_Redraw();
	int top = RadarButton.Y + RadarButton.Height;
	if (Options.IsSidebarOnRight) {
		Background.Set_Position(DSurface::ViewBounds.X + DSurface::ViewBounds.Width, top);
	} else {
		Background.Set_Position(0, top);
	}
	Background.Set_Size(SidebarSurface->Get_Width(), SidebarSurface->Get_Height() - top);

	Repair.Set_Position(DAT_00B0B4DC, DAT_00B0B4E0);
	Repair.Flag_To_Redraw();
	Repair.DrawOffset.X = -DAT_00886F90;

	Power.Set_Position(Repair.X + DAT_00B0B4E4, Repair.Y);
	Power.Flag_To_Redraw();
	Power.DrawOffset.X = -DAT_00886F90;

	for (int tab = 0; tab < STRIPS; tab++) {
		TabButtons[tab].Set_Position(DAT_00B0B4F0 * tab + DAT_00B0B4E8, DAT_00B0B4EC);
		TabButtons[tab].Flag_To_Redraw();
		TabButtons[tab].DrawOffset.X = -DAT_00886F90;
	}

	Upgrade.Set_Position(DAT_00B0B508, DAT_00B0B50C);
	Upgrade.Flag_To_Redraw();
	Upgrade.DrawOffset.X = -DAT_00886F90;

	Waypoint.Set_Position(DAT_00B0B508 + DAT_00B0B510, DAT_00B0B50C);
	Waypoint.Flag_To_Redraw();
	Waypoint.DrawOffset.X = -DAT_00886F90;

	int rows = Max_Visible_Cameos();
	int count = rows * 2;
	for (int column = 0; column < STRIPS; column++) {
		StripClass::SelectClass *button = &StripClass::SelectButton[column * 60];
		for (int row = 0; row < rows; row++) {
			for (int side = 0; side < 2; side++, button++) {
				button->Set_Position(DAT_00B0B4FC * side + DAT_00B0B4F4, DAT_00B0B500 * row + DAT_00B0B4F8 + 1);
				button->Flag_To_Redraw();
			}
		}
	}

	if (ToolTips != NULL) {
		ToolTip tip;
		tip.Flag = false;
		for (int tab = 0; tab < STRIPS; tab++) {
			tip.ID = tab + 0xCB;
			tip.Text = 0;
			tip.Region = Rect(TabButtons[tab].X, TabButtons[tab].Y, TabButtons[tab].Width, TabButtons[tab].Height);
			ToolTips->Remove(tip.ID);
			ToolTips->Add(&tip);
		}
		for (int id = 0; id < 60; id++) {
			ToolTips->Remove(id + 1000);
		}
		for (int index = 0; index < count; index++) {
			StripClass::SelectClass &button = StripClass::SelectButton[index];
			tip.ID = index + 1000;
			tip.Region = Rect(button.X, button.Y, button.Width, button.Height);
			tip.Text = 0;
			tip.Flag = true;
			ToolTips->Add(&tip);
		}

		tip.Flag = false;
		tip.ID = 0x66;
		tip.Text = (intptr_t)"TXT_SELL_MODE";
		tip.Region = Rect(Power.X, Power.Y, Power.Width, Power.Height);
		ToolTips->Remove(tip.ID);
		ToolTips->Add(&tip);

		tip.ID = 0x65;
		tip.Text = (intptr_t)"TXT_REPAIR_MODE";
		tip.Region = Rect(Repair.X, Repair.Y, Repair.Width, Repair.Height);
		ToolTips->Remove(tip.ID);
		ToolTips->Add(&tip);

		tip.ID = 0xC8;
		tip.Text = 0;
		tip.Region = Rect(Waypoint.X, Waypoint.Y, Waypoint.Width, Waypoint.Height);
		ToolTips->Remove(tip.ID);
		ToolTips->Add(&tip);

		tip.ID = 0xC9;
		tip.Text = 0;
		tip.Region = Rect(Upgrade.X, Upgrade.Y, Upgrade.Width, Upgrade.Height);
		ToolTips->Remove(tip.ID);
		ToolTips->Add(&tip);
	}
}
