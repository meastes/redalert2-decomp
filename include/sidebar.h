/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 * Copyright 2026 ra2-decomp contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/* $Header: /CounterStrike/SIDEBAR.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SIDEBAR.H                                                    *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : October 20, 1994                                             *
 *                                                                                             *
 *                  Last Update : October 20, 1994   [JLB]                                     *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#pragma once

#include "control.h"
#include "gadget.h"
#include "power.h"
#include "shapebtn.h"
#include "stage.h"

#include "rtti.hh"
#include "super.hh"


class InitClass {};
class FactoryClass;

class SidebarClass : public PowerClass
{
		typedef PowerClass BASECLASS;

	public:
		virtual HRESULT Load(IStream * stream) override;
		virtual HRESULT Save(IStream * stream) override;
		virtual void DisposeOfArt(void) override; // RA2: 0x67BFC0; TabClass::DisposeOfArt calls it first

	public:
		/*
		**	These constants are used to control the sidebar rendering. They are instantiated
		**	as enumerations since C++ cannot use "const" in this context.
		*/
		enum SideBarClassEnums {
			SIDE_Y=148,						// The Y position of sidebar upper left corner.
			SIDE_WIDTH=168,					// Width of the entire sidebar (in pixels).
			SIDE_BODY_Y=138,				/// Y position where the sidebar body (below the radar) begins.
			CREDITS_HEIGHT=16,				/// Height of the credits readout area at the top of the sidebar.

			COLUMN_ONE_X=24,				// Sidestrip upper left coordinates...
			COLUMN_ONE_Y=26,
			COLUMN_TWO_X=92,
			COLUMN_TWO_Y=26,

			BUTTON_ONE_X=31,				// Left button X coordinate.
			BUTTON_ONE_Y=-9,				// Left button Y coordinate.
			BUTTON_SPACING=27,				/// Spacing between the top row buttons (in pixels).

			GADGET_CAMEO=1000,				/// Tooltip ID of the first cameo slot.

			COLUMNS=2,						// Number of side strips on sidebar.
			STRIPS=4 // RA2: four strips, one per tab
		};

		/*
		**	The sidebar's layout, computed by Recalc_Layout and Recalc_Horizontal_Layout.
		*/
		static int TopButtonX; // RA2: 0xABCD48; the left of the first button of the top row (upgrade/repair)
		static int TopButtonY; // RA2: 0xABCD4C
		static int TopButtonSpacing; // RA2: 0xABCD50; from the first button of the top row to the next
		static int TabButtonX; // RA2: 0xABCD54; the left of the first tab button
		static int TabButtonY; // RA2: 0xABCD58
		static int TabButtonSpacing; // RA2: 0xABCD5C; between the tab buttons
		static int StripLeft; // RA2: 0xABCD60; where the strips start
		static int StripTop; // RA2: 0xABCD64; where the strips start (the sidebar height less the strips' share)
		static int CameoWidth; // RA2: 0xABCD68; 63 for the Allies, 64 for the Soviets
		static int CameoHeight; // RA2: 0xABCD6C; the step from one row of cameos to the next
		static int PowerBarLeft; // RA2: 0xABCD74
		static int SidebarBottomY; // RA2: 0xABCD78; the power bar height plus 234
		static int SidebarEdgeWidth; // RA2: 0xABCD7C
		static int SidebarEdgeStep; // RA2: 0xABCD80

		/*
		**	The sidebar's art. Each shape is allocated by Init_For_House or retrieved from a mix file; the
		**	allocated ones are freed by DisposeOfArt.
		*/
		enum SidebarShapeType {
			SBSHAPE_TOP, // SIDE1.SHP, the top of the sidebar backdrop
			SBSHAPE_MIDDLE, // SIDE2.SHP, stacked once for every visible row of cameos
			SBSHAPE_BOTTOM, // SIDE3.SHP
			SBSHAPE_ADDON, // ADDON.SHP, drawn below the bottom cap
			SBSHAPE_SHUTTER_BACK, // the shutter animation that covers the strips while no tab has anything
			SBSHAPE_SHUTTER_MIDDLE,
			SBSHAPE_SHUTTER_FRONT,
			SBSHAPE_CLOCK, // GCLOCK2.SHP, the production clock
			SBSHAPE_COUNT
		};
		static ShapeSet const * SidebarShapes[SBSHAPE_COUNT]; // RA2: 0xABCD20
		static bool SidebarShapeAllocated[SBSHAPE_COUNT]; // RA2: 0xABCD40

		SidebarClass(void);
		SidebarClass(NoInitClass const & x);

		/*
		**	Initialization
		*/
		virtual void One_Time(void) override;                       // One-time inits
		virtual void Init_Clear(void) override;                     // Clears all to known state
		virtual void Init_IO(void) override;                        // Inits button list
		virtual void Init_For_House(void) override;
		void Reload_Sidebar(void);							// Loads house-specific sidebar art
		void Set_Cameo_Text(bool state); // RA2: 0x67B880; the body is empty (the caption flag is a global)
		void Recalc_Layout(void); // RA2: 0x67B910; the sidebar's rectangle and the sizes of its parts
		static void Recalc_Horizontal_Layout(void); // RA2: 0x67BA60; the left edge of the sidebar and of its controls
		void Request_Redraw(void); // RA2: 0x67C100; the active strip is redrawn and the sidebar for two more frames
		void Notice_Object_Change(ObjectClass * object); // RA2: 0x67C150
		void Request_Column_Redraw(int column); // RA2: 0x67C310; as Request_Redraw, if the strip is the active one
		void Update_Build_Buttons(void); // RA2: 0x67C850; the scroll buttons are on while the active strip holds more than the sidebar shows
		void Enable_Column_Button(int column); // RA2: 0x67C910
		void Update_Column_Availability(int column); // RA2: 0x67CA50; the tab of an emptied strip is disabled and another tab selected
		void Activate_Tab(int tab); // RA2: 0x67D7E0
		void Start_Tab_Flash(int tab); // RA2: 0x67DC00
		bool Stop_Tab_Flash(int tab); // RA2: 0x67DC70
		int First_Used_Column(void); // RA2: 0x681980; the first strip that holds something, or -1
		int Max_Visible(void); // RA2: 0x6820A0; the rows of cameos that fit
		int Get_Doubled_Row_Count(void); // RA2: 0x6820F0; two cameos to a row
		static int __fastcall Cameos_Width(int count); // RA2: 0x67E640
		static int __fastcall Cameos_Height(int count); // RA2: 0x67E650

		virtual void AI(KeyNumType & input, Point2D const & xy) override;
		virtual void Draw_It(bool complete) override;
		virtual void Reposition_Sidebar(void) override;
		virtual char const * Help_Text(int id) override;

		void Zoom_Mode_Control(void);
		bool Abandon_Production(RTTIType type, FactoryClass * factory);
		virtual bool Activate(int control); // RA2: virtual
		bool Add(RTTIType type, int ID);
		static int Which_Tab(RTTIType type, int id, bool flag); // RA2: 0x6818D0, the sidebar tab an object is built from
		void Flag_Tab_To_Redraw(int tab); // RA2: 0x67C2D0
		void Select_Tab(int tab); // RA2: 0x67D990
		void Flash_Cameo(TechnoTypeClass const * type, int frames); // RA2: 0x6822B0
		bool Abandon_Production(RTTIType type, int id, FactoryClass * factory); // RA2: 0x681740 takes the object id too (HouseClass::Abandon_Production passes it); replaces TibSun's Sidebar_Click declaration, which nothing defines or calls
		void Recalc(void);
		bool Factory_Link(FactoryClass * factory, RTTIType type, int id);

		void Blit_Sidebar(bool complete);

		/*
		**	Each side strip is managed by this class. It handles all strip specific
		**	actions.
		*/
		class StripClass : public StageClass
		{
			public:
			class SelectClass : public ControlClass
			{
				public:
					SelectClass(void);
					SelectClass(NoInitClass const & x) : ControlClass(x) {};

					void Set_Owner(StripClass & strip, int index);
					bool Is_Hovered(void); // RA2: the cameo is highlighted and accepts input

					StripClass * Strip;
					int Index;
					bool IsHovered; // RA2: +0x34, the mouse is over the button; the button is 0x38 bytes

					virtual void Mouse_Enter(void) override; // RA2: 0x681600
					virtual void Mouse_Leave(void) override; // RA2: 0x681650

				protected:
					virtual int Action(unsigned flags, KeyNumType & key) override;
			};

			public:
				StripClass(void) {}
				StripClass(InitClass const &);
				StripClass(NoInitClass const & ) {};

				bool Add(RTTIType type, int ID);
				bool Abandon_Production(FactoryClass const * factory);
				bool Scroll(bool up);
				bool Scroll(bool up, int amount); // RA2: 0x67EE60; the strip moves by this many rows
				bool Step(bool up); // RA2: 0x67EEF0; one row
				bool Page(bool up);
				bool AI(KeyNumType & input, Point2D const & xy);
				wchar_t const * Help_Text(int id); // RA2: 0x67F580; the cameo name and cost (wide text)
				void Draw_It(bool complete);
				void One_Time(int id);
				void Clear_Buildables(void); // RA2: 0x67E770; the scroll state and every entry are cleared
				void Init_Buttons(int id); // RA2: 0x67E7E0; numbers the strip and places its cameo buttons
				void Add_Buttons(void); // RA2: 0x67E8F0
				void Remove_Buttons(void); // RA2: 0x67E9A0
				bool Is_Before(RTTIType type, int id, RTTIType othertype, int otherid); // RA2: 0x67EA20; the order of the cameos
				void Insert_Buildable(RTTIType type, int id); // RA2: 0x67ED10
				int Get_Progress(int index); // RA2: 0x67F7C0; how far an entry's production has come
				void Set_Flash(int id, int delay); // RA2: 0x682400; the entry flashes until this many frames from now
				bool Recalc(void);
				void Activate(void);
				bool Has_Buildables(void) const; // RA2: 0x681730, any cameos on this strip
				void Deactivate(void);
				void Flag_To_Redraw(void);
				bool Factory_Link(FactoryClass * factory, RTTIType type, int id);
				ShapeSet const * Get_Special_Cameo(SuperWeaponType type);

				/*
				**	Working numbers used when rendering and processing the side strip.
				*/
				enum SideBarGeneralEnums {
					BUTTON_UP=200,
					BUTTON_DOWN=210,
					BUTTON_SELECT=220,
					MAX_BUILDABLES=75,				// Maximum number of object types in sidebar.
					OBJECT_HEIGHT=51,				// Pixel height of each buildable object.
					OBJECT_WIDTH=64,				// Pixel width of each buildable object.
					MAX_VISIBLE=4,					// Number of object slots visible at any one time.
					MAX_SLOTS_PER_STRIP=60,			// RA2: the cameo buttons of a strip in SelectButton
					MAX_SLOTS=20,					/// Maximum number of object slots at any resolution.
					SCROLL_RATE=OBJECT_HEIGHT,		// The pixel jump while scrolling (larger is faster).
					UP_X_OFFSET=5,					// Scroll up arrow coordinates.
					UP_Y_OFFSET=25,					/// Y offset of the scroll arrows below the last object slot.
					DOWN_X_OFFSET=34,				// Scroll down arrow coordinates.
					DOWN_Y_OFFSET=UP_Y_OFFSET,
					TEXT_X_OFFSET=30,				// X offset to print "ready" text.
					TEXT_Y_OFFSET=2,				// Y offset to print "ready" text.
					CAMEO_TEXT_Y_OFFSET=41,			/// Y offset to print the cameo name text.
					QUEUE_COUNT_X_OFFSET=60			/// X offset to print the queued production count.
				};

				/*
				**	This is the coordinate of the upper left corner that this side strip
				**	uses for rendering.
				*/
				bool AllowedToDraw; // RA2: before X,Y; X at +0x1C
				int X,Y;

				/*
				 * This is the area, in sidebar coordinates, covered by the strip's visible
				 * object slots, recomputed whenever the slot count changes.
				 */
				Rect ObjectRect;

				/*
				**	This is a unique identifier for the sidebar strip. Using this identifier,
				**	it is possible to differentiate the button messages that arrive from the
				**	common input button list.  It >MUST< be equal to the strip's index into
				**	the Column[] array, because the strip uses it to access the stripclass
				**	buttons.
				*/
				int ID;

				/*
				**	Shape numbers for the shapes in the STRIP.SHP file.
				*/
				enum SideBarStipShapeEnums {
					SB_BLANK,			// The blank rectangle to use if there are no objects present.
					SB_FRAME
				};

				/*
				**	If this particular side strip needs to be redrawn, then this flag
				**	will be true.
				*/
				bool IsToRedraw;

				/*
				**	If construction is in progress (no other objects in this strip can
				**	be started), then this flag will be true. It will be cleared when
				**	the strip is free to start production again.
				*/
				bool IsBuilding;

				/*
				**	This controls the sidebar slide direction. If this is true, then the sidebar
				**	will scroll downward -- revealing previous objects.
				*/
				bool IsScrollingDown;

				/*
				**	If the sidebar is scrolling, then this flag is true. Otherwise it is false.
				*/
				bool IsScrolling;

				/*
				**	This is the object (sidebar slot) that is flashing. Only one slot can be flashing
				**	at any one instant. This is usually the result of a click on the slot and construction
				**	has commenced.
				*/
				unsigned Flasher;

				/*
				**	As the sidebar scrolls up and down, this variable holds the index for the topmost
				**	visible sidebar slot.
				*/
				int TopIndex;

				/*
				**	This is the queued scroll direction and amount. The sidebar
				**	will scroll the number of slots indicated by this value. This
				**	value is set according to the scroll buttons.
				*/
				int Scroller;

				/*
				**	The sidebar has smooth scrolling. This is the number of pixels the sidebar
				**	has slide down. Thus, if this value were 5, then there would be 5 pixels of
				**	the TopIndex-1 sidebar object visible. When the Slid value reaches 24, then
				**	the value resets to zero and the TopIndex is decremented. For sliding in the
				**	opposite direction, change the IsScrollingDown flag.
				*/
				int Slid;

				/*
				**	The value of Slid the last time we rendered the sidebar.
				*/
				int LastSlid;

				/*
				**	This is the count of the number of sidebar slots that are active.
				*/
				int BuildableCount;

				/*
				**	This is the array of buildable object types. This array is sorted in the order
				**	that it is to be displayed. This array keeps track of which objects are building
				**	and ready to be placed. The very nature of this method precludes simultaneous
				**	construction of the same object type.
				*/
				struct BuildType {
					BuildType(void) {}
					BuildType(int id, RTTIType type, FactoryClass *factory=NULL) :
						BuildableID(id),
						BuildableType(type),
						IsAlt(0),
						Factory(factory)
					{}

					bool operator==(const BuildType & other) const {
						return(BuildableID == other.BuildableID && BuildableType == other.BuildableType);
					}

					bool operator!=(const BuildType & other) const {
						return(BuildableID != other.BuildableID || BuildableType != other.BuildableType);
					}

					int BuildableID;
					RTTIType BuildableType;
					BOOL IsAlt; // RA2: +0x08, set for building types
					FactoryClass * Factory;								// Production manager.
					int Unknown_10; // RA2: +0x10, cleared by StripClass::Add
					StageClass Progress; // RA2: +0x14
					int FlashEndFrame; // RA2: +0x2C; an entry is 0x30 bytes
				};
				BuildType Buildables[MAX_BUILDABLES];

				/*
				**	Pointer to the shape data for small versions of the logos. These are used as
				**	placeholder pieces on the side bar.
				*/
				static ShapeSet * LogoShapes; // RA2: declared, never defined (nor are ClockShapes and RechargeClockShapes); the initializer numbering of every unit that includes this header counts the three declarations

				/*
				**	This points to the animation sequence of frames used to mark the passage of time
				**	as an object is undergoing construction.
				*/
				static ShapeSet const * ClockShapes;
				static ShapeSet const * RechargeClockShapes;
				static ShapeSet const * DarkenShapes;

				static SelectClass SelectButton[STRIPS * MAX_SLOTS_PER_STRIP]; // RA2: 0xAB9740; the cameo buttons of all the strips, 60 to a strip

		} Column[STRIPS]; // RA2: four strips of 0xE64 bytes from +0x1540

		int ShutterFrame; // RA2: +0x4ED0; frame of the animation drawn over the sidebar as tabs empty or fill
		int ShutterStep; // RA2: +0x4ED4; +1 or -1
		int ActiveTabIndex; // RA2: +0x4ED8
		int RedrawUntilFrame; // RA2: +0x4EDC; the active strip redraws until this frame

		/*
		 * If the player has asked for cameo text, then this flag will be true. With it
		 * set, every build cameo is captioned with its name and cost, so the pop-up help
		 * for a cameo mentions only the cost.
		 */
		bool IsCameoText;

		/*
		**	If the sidebar is active then this flag is true.
		*/
		bool IsSidebarActive;

		/*
		**	This flag tells the rendering system that the sidebar needs to be redrawn.
		*/
		bool IsToRedraw;

		/*
		 * If something outside of the sidebar's own rendering has drawn over the
		 * sidebar -- a tooltip, for instance -- then this flag will be true. It forces
		 * the next render to rebuild the sidebar in full rather than just the parts
		 * that changed, and is cleared once that render has taken place.
		 */
		bool IsForceCompleteRedraw;

		/*
		 * If the only thing that changed is the credits readout, then this flag will be
		 * true. It lets the blitter copy just the credits strip at the top of the
		 * sidebar to the visible page rather than the whole sidebar.
		 */
		bool IsToRedrawCredits;

		class SBGadgetClass: public GadgetClass {
			public:
				SBGadgetClass(void);

			protected:
				virtual int Action(unsigned flags, KeyNumType & key) override;
		};

		/*
		**	This is the button that is used to collapse and expand the sidebar.
		**	These buttons must be available to derived classes, for Save/Load.
		*/
		static ShapeButtonClass Repair;
		static ShapeButtonClass Upgrade;
		static ShapeButtonClass TabButton[STRIPS]; // RA2: 0xAB9508; the four tab buttons (IDs 203 to 206)
		static ShapeButtonClass ScrollDownButton; // RA2: 0xABCBE8; ID 201, R-DN.SHP
		static ShapeButtonClass ScrollUpButton; // RA2: 0xABCCC0; ID 200, R-UP.SHP
		static Rect DirtyRect; // RA2: 0x8336E8; the part of the sidebar surface a strip drew
		static bool IsSidebarRedrawPending; // RA2: 0x8373C3; set whenever a button or a strip changed
		static SBGadgetClass Background;

		/*
		 * Set whenever anything rasterizes onto the sidebar surface; tells
		 * Blit_Sidebar that the surface must be copied to the visible page.
		 */
		static bool IsToBlitSidebar;

		/*
		 * Set after Blit_Sidebar draws the mouse onto the sidebar surface;
		 * GScreenClass::Render erases it from the surface next frame.
		 */
		static bool IsToEraseSidebarMouse;

		bool Scroll(bool up, int column);
		bool Page(bool up, int column);

	private:
		bool Activate_Repair(int control);
		bool Activate_Upgrade(int control);
		bool Activate_Demolish(int control);
		int Which_Column(RTTIType type);

		bool IsDemolishActive; // RA2: +0x4EE5
		bool IsRepairActive; // RA2: +0x4EE6; RA2 has no upgrade mode
};
