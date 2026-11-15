
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

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y3): SidebarClass::AI (YR 0x006A7780, vtable slot AI) and
// SidebarClass::StripClass::AI (YR 0x006A8B30), both unmatched in ra2-decomp. Written from
// gamemd.exe's code with the RA2 tree's SidebarClass::AI (src/recovered/sidebar.cpp, 0x0067DC90)
// and TibSun's StripClass::AI (TibSun archive code/sidebar.cpp; GPL-3.0-or-later, EA's GPL
// section 7 terms; THIRD_PARTY.md) as the shape. The tab switch is this unit's SetTab, the
// scroll buttons are StripClass::Page (gamemd.exe inlines both); the observer's player-list
// refresh at the end of StripClass::AI (PlayerPtr == DAT_00AC1198, YR 0x006A9038..0x006A9251)
// is not written. Behaviour code for the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
extern int DAT_00B0B514;     // the strip's scroll step in pixels per frame (TibSun SCROLL_RATE)
void __fastcall FUN_00734250(BuildingClass * building);   // uicmnds.cpp: a finished building's placement

void SidebarClass::AI(KeyNumType & input, Point2D const & xy)
{
	Point2D point = xy - Point2D(SidebarRect.X, 0);

	if (!Unsorted::ArmageddonMode) {
		Activate(1);
		for (int index = 0; index < 4; index++) {
			Column[index].AI(input, point);
		}
	}

	if (IsSidebarActive) {
		if (unknown_5398 == 1) {
			unknown_5394++;
			if (unknown_5394 > ((ShapeSet const *)DAT_00B0B478)->Get_Count()) {
				unknown_5398 = 0;
				unknown_5394 = 0;
			}
		} else if (unknown_5398 == -1) {
			unknown_5394--;
			if (unknown_5394 < 0) {
				unknown_5398 = 0;
				unknown_5394 = 0;
			}
		}

		FUN_006A6290(PlayerPtr->CurBuildings > 0);

		if (input == 0x8066) {
			VocClass::PlayGlobal(Rule->GUIMainButtonSound, 1.0f, 0x2000);
			Sell_Mode_Control(-1);
		} else if (input == 0x8065) {
			VocClass::PlayGlobal(Rule->GUIMainButtonSound, 1.0f, 0x2000);
			Repair_Mode_Control(-1);
		} else if (input >= 0x80CB && input < 0x80CF) {
			SetTab(input - 0x80CB);
			VocClass::PlayGlobal(Rule->GUITabSound, 1.0f, 0x2000);
		} else if ((input & ~0x4000) == (Upgrade.ID | 0x8000)) {
			Upgrade.IsPressed = false;
			if (Column[ActiveTabIndex].Page(false)) {
				VocClass::PlayGlobal(Rule->GUITabSound, 1.0f, 0x2000);
			} else {
				VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000);
			}
		} else if ((input & ~0x4000) == (Waypoint.ID | 0x8000)) {
			Waypoint.IsPressed = false;
			if (Column[ActiveTabIndex].Page(true)) {
				VocClass::PlayGlobal(Rule->GUITabSound, 1.0f, 0x2000);
			} else {
				VocClass::PlayGlobal(Rule->ScoldSound, 1.0f, 0x2000);
			}
		}

		bool flashed = false;
		for (int tab = 0; tab < 4; tab++) {
			if (TabButtons[tab].AI()) {
				flashed = true;
			}
		}
		if (flashed) {
			IsToRedraw = true;
			IsToBlitSidebar = true;
			IsForceCompleteRedraw = true;
			Flag_To_Redraw();
		}
	}

	if (!IsSellMode && Power.IsOn) {
		Power.Turn_Off();
	}
	if (!IsRepairMode && Repair.IsOn) {
		Repair.Turn_Off();
	}
	PowerClass::AI(input, xy);
}

bool SidebarClass::StripClass::AI(KeyNumType & input, Point2D const & xy)
{
	bool redraw = false;

	/*
	**	Reflect the desired scroll into the scroll logic (TibSun's text; YR counts two cameos
	**	a row, and the observer's strip one player a row).
	*/
	if (!IsScrolling && Scroller) {
		int rows = Strip_Rows();
		int count = BuildableCount;
		int per_row = 2;
		int visible = rows * 2;
		if (PlayerPtr == DAT_00AC1198) {
			count = DAT_00884CF8;
			rows = rows / 2;
			per_row = 1;
			visible = rows;
		}
		if (count <= visible) {
			Scroller = 0;
		} else if (Scroller < 0) {
			if (!TopIndex) {
				Scroller = 0;
			} else {
				Scroller++;
				IsScrollingDown = false;
				IsScrolling = true;
				TopIndex--;
				Slid = 0;
			}
		} else if ((TopIndex + rows) * per_row >= count) {
			Scroller = 0;
		} else {
			Scroller--;
			Slid = DAT_00B0B500;
			IsScrollingDown = true;
			IsScrolling = true;
		}
	}

	if (IsScrolling) {
		if (IsScrollingDown) {
			Slid -= DAT_00B0B514;
			if (Slid <= 0) {
				IsScrolling = false;
				Slid = 0;
				TopIndex++;
			}
		} else {
			Slid += DAT_00B0B514;
			if (Slid >= DAT_00B0B500) {
				IsScrolling = false;
				Slid = 0;
			}
		}
		redraw = true;
	}

	if (Flasher != -1) {
		if (Graphic_Logic()) {
			redraw = true;
			if (Fetch_Stage() >= 7) {
				Set_Rate(0);
				Set_Stage(0);
				Flasher = -1;
			}
		}
	}

	if (Map.unknown_53A0 > Unsorted::CurrentFrame) {
		redraw = true;
	}

	/*
	**	The first two tabs (buildings and defences) flash while a building waits to be placed or
	**	a super weapon is ready.
	*/
	if (ID == 0 || ID == 1) {
		bool flash = false;
		for (int index = 0; index < BuildableCount; index++) {
			BuildType & build = Buildables[index];
			if (build.BuildableType == RTTI_SPECIAL) {
				SuperClass * super = PlayerPtr->SuperWeapon[build.BuildableID];
				if (super != NULL && super->ShouldFlashTab()) {
					flash = true;
					break;
				}
			} else if (build.Factory != NULL && build.Factory->Has_Completed()) {
				TechnoClass * pending = build.Factory->Get_Object();
				if (pending != NULL && pending->What_Am_I() == RTTI_BUILDING) {
					flash = true;
					break;
				}
			}
		}
		if (flash) {
			int frame = Unsorted::CurrentFrame;
			int delay = 10 - frame % 10;
			TabButtons[ID].StartFlashing(10, delay, ((frame + delay) / 10) % 2 == 0);
			Map.IsForceCompleteRedraw = true;
			Map.Flag_To_Redraw();
		} else {
			TabButtons[ID].StopFlashing();
		}
	}

	/*
	**	Handle the completion of what the strip's factories build.
	*/
	if (IsBuilding) {
		for (int index = 0; index < BuildableCount; index++) {
			FactoryClass * factory = Buildables[index].Factory;
			if (factory && factory->Has_Changed()) {
				redraw = true;
				if (factory->Has_Completed()) {
					TechnoClass * pending = factory->Get_Object();
					if (pending != NULL) {
						switch (pending->What_Am_I()) {
							case RTTI_UNIT:
							case RTTI_AIRCRAFT:
							case RTTI_INFANTRY:
								OutList.Add(EventClass(pending->Owner(), EventClass::PLACE, (RTTIType)pending->What_Am_I(), -1, pending->Techno_Type_Class()->Naval, Cell(0, 0)));
								Buildables[index].unknown_10 = 0;
								break;

							case RTTI_BUILDING:
								VoxClass::Play("EVA_ConstructionComplete", -1, -1);
								FUN_00734250((BuildingClass *)pending);
								break;

							default:
								break;
						}
					}
				}
			}
		}
	}

	/*
	**	The cameos' own build progress (gamemd.exe steps each buildable's Progress stage, to
	**	0x35, while its factory produces and is not suspended or on hold).
	*/
	for (int index = 0; index < BuildableCount; index++) {
		BuildType & build = Buildables[index];
		if (build.unknown_10 == 1) {
			FactoryClass * factory = build.Factory;
			if (factory == NULL || !factory->Is_Building() || factory->Is_On_Hold()) {
				build.Progress.Set_Rate(0);
				build.Progress.Set_Stage(0);
			}
			if (build.Progress.Graphic_Logic()) {
				redraw = true;
				if (build.Progress.Fetch_Stage() >= 0x35) {
					build.Progress.Set_Rate(0);
				}
			}
		}
	}

	if (redraw) {
		if (Map.ActiveTabIndex == ID) {
			((SidebarClass &)Map).IsToRedraw = true;
			Map.Column[Map.ActiveTabIndex].IsToRedraw = true;
			Map.Flag_To_Redraw();
		}
		IsToRedraw = true;
		Map.Flag_To_Redraw();
		Map.IsForceCompleteRedraw = true;
		IsToBlitSidebar = true;
	}

	return(redraw);
}

// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y3): SidebarClass::StripClass::Draw_It (YR 0x006A9540, 4,280
// bytes) is unmatched in ra2-decomp. The text is the RA2 tree's exact counterpart
// (src/recovered/sidebar.cpp, SidebarClass::StripClass::Unknown_0067F800, RA2 0x0067F800) with
// the YR tree's names and gamemd.exe's differences: the cameos' palette is FileSystem::CAMEO_PAL
// (0x0087F6B0), the clock GCLOCK2.SHP, the hover frame and the texts take the tooltip colour
// (DAT_00B0FA1C), the queue text is L"%d" through Fancy_Text_Print_Wide. The observer's player
// list (PlayerPtr == DAT_00AC1198, YR 0x006AA05B..0x006AA59B) is not written. Behaviour code for
// the native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
extern ConvertClass * DAT_0087F6B0;        // FileSystem::CAMEO_PAL, the cameos' drawer
extern RGBClass DAT_00B0FA1C;              // the tooltip/tab text colour (txtlabel2.cpp)
void __fastcall FUN_006AC480(wchar_t const * text, Point2D const & point, Rect const & window, int width);
void __fastcall FUN_00621B80(Rect * rect, Surface * surface, unsigned short color, int alpha);

static void Strip_Box_Text(wchar_t const * text, Point2D const & point, Rect const & cliprect, unsigned color, unsigned flags, int value)
{
	// The text over a darkened box (gamemd.exe: Drawing_GetTextDimensions, FUN_00621B80 at 0xAF).
	Rect box;
	Drawing_GetTextDimensions(&box, text, point, (unsigned short)flags, 2, 1);
	FUN_00621B80(&box, SidebarSurface, 0, 0xAF);
	Point2D drawn;
	Fancy_Text_Print_Wide(&drawn, text, SidebarSurface, &cliprect, &point, color, 0, TextPrintType(flags), value);
}

void SidebarClass::StripClass::Draw_It(bool complete)
{
	if (AllowedToDraw && (IsToRedraw || complete)) {
		IsToRedraw = false;
		IsToBlitSidebar = true;
		Rect cliprect(0, 0, SidebarSurface->Get_Width(), SidebarSurface->Get_Height());

		if (PlayerPtr == DAT_00AC1198) {
			LastSlid = Slid;	// NATIVE: the observer's player list is not written (see above)
			return;
		}

		bool busy = false;
		for (int i = 0; i < BuildableCount; i++) {
			if (Buildables[i].unknown_10 == 1 || Buildables[i].unknown_10 == 2) {
				busy = true;
				break;
			}
		}

		unsigned pixel = DSurface::Build_Hicolor_Pixel(DAT_00B0FA1C);
		int rows = Strip_Rows() + (IsScrolling ? 1 : 0);
		int columns = 2;
		for (int row = 0; row < rows; row++) {
			for (int col = 0; col < columns; col++) {
				bool draw = complete;
				int stage = 0;
				void const * shape = NULL;
				FactoryClass * factory = NULL;
				wchar_t const * state = NULL;
				int index = col + (TopIndex + row) * columns;
				bool production = false;
				bool completed = false;
				bool darken = false;
				bool hover = false;
				int x = DAT_00B0B4FC * col - SidebarRect.X + X;
				int y = DAT_00B0B500 * row + 1 + Y;
				SelectClass * button = &SelectButton[row * columns + ID * 60 + col];
				if (IsScrolling) {
					y += Slid - DAT_00B0B500;
				}
				wchar_t const * name = NULL;
				TechnoTypeClass const * obj = NULL;

				if (index < BuildableCount) {
					hover = button->FUN_006ABA20();
					if (Buildables[index].BuildableType != RTTI_SPECIAL) {
						obj = Fetch_Techno_Type(Buildables[index].BuildableType, Buildables[index].BuildableID);
						if (obj) {
							name = obj->UIName;
							bool isbusy = false;
							if (obj->What_Am_I() == RTTI_BUILDINGTYPE && busy) {
								isbusy = true;
							} else if (obj->Who_Can_Build_Me(true, true, true, PlayerPtr) == NULL) {
								isbusy = true;
							} else {
								TechnoTypeClass const * type = Fetch_Techno_Type(Buildables[index].BuildableType, Buildables[index].BuildableID);
								bool none = PlayerPtr->Can_Build(type, false, false) == -1;
								bool limited = PlayerPtr->FUN_0050B370(type);
								if (none || limited) {
									isbusy = true;
								}
							}
							shape = obj->Get_Cameo_Data();
							factory = Buildables[index].Factory;
							if (factory) {
								production = true;
								completed = factory->Has_Completed();
								if (completed) {
									state = StringTable::LoadString("TXT_READY", NULL, "D:\\ra2mdpost\\Sidebar.CPP", 0xDE5);
								} else {
									draw = true;
								}
								stage = FUN_006A94F0(index);
								darken = false;
							} else {
								production = false;
								darken = isbusy;
								switch (Buildables[index].unknown_10) {
									case 1:
										draw = true;
										production = true;
										stage = FUN_006A94F0(index);
										darken = false;
										break;
									case 3:
										production = true;
										completed = true;
										darken = false;
										break;
									case 2:
										production = true;
										completed = false;
										darken = true;
										stage = FUN_006A94F0(index);
										break;
									case 0:
										if (!complete) {
											continue;
										}
										break;
								}
							}
						} else {
							darken = false;
						}
					} else {
						int id = Buildables[index].BuildableID;
						SuperClass * super = PlayerPtr->SuperWeapon[id];
						name = SuperWeaponTypes[id]->UIName;
						shape = Get_Special_Cameo(SuperWeaponType(id));
						production = true;
						completed = !super->ShouldDrawProgress();
						state = super->NameReadiness();
						stage = super->Anim_Stage();
						darken = false;
						draw = true;
					}
				} else {
					production = false;
				}

				Rect window(0, 0, x + 60, y + 48);
				if (draw) {
					if (shape) {
						Draw_Shape(*SidebarSurface, *DAT_0087F6B0, (ShapeSet const *)shape, 0, Point2D(x, y), window, ShapeFlags_Type(0x400));
					}
					if (hover) {
						SidebarSurface->Draw_Rect(window, Rect(x, y, 60, 48), pixel);
					}
					if (darken) {
						Draw_Shape(*SidebarSurface, *FileSystem::SIDEBAR_PAL, (ShapeSet const *)FileSystem::DARKEN_SHP, 0, Point2D(x, y), window, ShapeFlags_Type(0x401));
					}
					if (Buildables[index].FlashEndFrame > Unsorted::CurrentFrame && Unsorted::CurrentFrame % 16 > 8) {
						Draw_Shape(*SidebarSurface, *FileSystem::SIDEBAR_PAL, (ShapeSet const *)FileSystem::DARKEN_SHP, 0, Point2D(x, y), window, ShapeFlags_Type(0x404));
					}
					if (name) {
						FUN_006AC480(name, Point2D(x, y + 36), cliprect, 60);
					}
					bool hasqueue = false;
					if (obj) {
						FactoryClass * queue = PlayerPtr->Fetch_Factory((RTTIType)obj->What_Am_I(), obj->Naval, 0);
						if (queue) {
							int total = queue->Total(obj);
							TechnoClass * making = queue->Get_Object();
							if (total > 1 || (total > 0 && (making == NULL || (making->Techno_Type_Class() && making->Techno_Type_Class() != obj)))) {
								wchar_t buffer[256];
								swprintf(buffer, L"%d", total);
								Strip_Box_Text(buffer, Point2D(x + 60, y + 1), cliprect, pixel, 0x242, total);
								hasqueue = true;
							}
						}
					}
					if (production) {
						if (state) {
							Strip_Box_Text(state, Point2D(x + 30, y + 1), cliprect, pixel, 0x142, 0);
						}
						if (!completed) {
							Draw_Shape(*SidebarSurface, *FileSystem::SIDEBAR_PAL, (ShapeSet const *)FileSystem::GCLOCK2_SHP, stage + 1, Point2D(x, y), window, ShapeFlags_Type(0x404));
							if ((factory && !factory->Is_Building()) || Buildables[index].unknown_10 == 2) {
								wchar_t const * hold = StringTable::LoadString("TXT_HOLD", NULL, "D:\\ra2mdpost\\Sidebar.CPP", 0xE34);
								if (hasqueue) {
									Strip_Box_Text(hold, Point2D(x + 2, y + 1), cliprect, pixel, 0x42, 0);
								} else {
									Strip_Box_Text(hold, Point2D(x + 30, y + 1), cliprect, pixel, 0x142, 0);
								}
							}
						}
					}
				}
			}
		}
		LastSlid = Slid;
	}
}

bool SidebarClass::StripClass::Recalc(void)
{
	int ok;

	if (Debug_Map || !BuildableCount) {
		return(false);
	}

	bool scroll = false;
	int max_visible = Strip_Rows() * 2;	// NATIVE: YR's strips show two cameos a row (the RA2 twin, RA2 0x006802D0)
	BuildType * unshifted = new BuildType[max_visible];

	for (int i = 0; i < max_visible; i++) {
		if (i + TopIndex * 2 < BuildableCount) {
			unshifted[i] = Buildables[i + TopIndex * 2];
		}
	}

	/*
	**	Sweep through all objects listed in the sidebar. If any of those object can
	**	not be created -- even in theory -- then they must be removed form the sidebar and
	**	any current production must be abandoned.
	*/
	bool redraw = false;
	for (int index = 0; index < BuildableCount; index++) {
		TechnoTypeClass const * tech = Fetch_Techno_Type(Buildables[index].BuildableType, Buildables[index].BuildableID);
		if (tech != NULL) {
			BuildingClass const * who = tech->Who_Can_Build_Me(true, false, false, PlayerPtr);
			ok = who != NULL && who->House->Can_Build(tech, true, true);
		} else {
			if ((unsigned)Buildables[index].BuildableID < (unsigned)PlayerPtr->SuperWeapon.Count()) {
				ok = PlayerPtr->SuperWeapon[Buildables[index].BuildableID]->Is_Present();
			} else {
				ok = false;
			}
		}

		if (!ok) {
			for (int i = 0; i < max_visible; i++) {
				if (unshifted[i] == Buildables[index]) {
					unshifted[i] = BuildType(0, RTTI_NONE);
				}
			}

			/*
			**	Removes this entry from the list.
			*/
			if (BuildableCount > 1 && index < BuildableCount-1) {
				memmove(&Buildables[index], &Buildables[index+1], sizeof(Buildables[0])*((BuildableCount-index)-1));
			}
			redraw = true;
			scroll = true;
			Buildables[BuildableCount-1].Factory = NULL;
			IsToRedraw = true;
			BuildableCount--;
			index--;
		}
	}

	if (scroll) {
		bool got_old = false;
		bool got_new = false;

		int oldpos;
		for (oldpos = 0; oldpos < max_visible; oldpos++) {
			if (unshifted[oldpos] != BuildType(0, RTTI_NONE)) {
				got_old = true;
				break;
			}
		}

		int newpos;
		if (got_old && BuildableCount != 0) {
			for (newpos = 0; newpos < BuildableCount; newpos++) {
				if (Buildables[newpos] == unshifted[oldpos]) {
					got_new = true;
					break;
				}
			}
		}

		if (got_old && got_new) {
			TopIndex = (newpos - oldpos) / 2;	// NATIVE: YR's TopIndex counts rows
			TopIndex = MAX(0, MIN(TopIndex, (BuildableCount - max_visible + 1) / 2));
		} else {
			TopIndex = 0;
		}
	}

	delete[] unshifted;
	return(redraw);
}
