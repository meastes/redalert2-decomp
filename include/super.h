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

/* $Header: /CounterStrike/SUPER.H 1     3/03/97 10:25a Joe_bostic $ */
/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Command & Conquer                                            *
 *                                                                                             *
 *                    File Name : SUPER.H                                                      *
 *                                                                                             *
 *                   Programmer : Joe L. Bostic                                                *
 *                                                                                             *
 *                   Start Date : 07/28/95                                                     *
 *                                                                                             *
 *                  Last Update : July 28, 1995 [JLB]                                          *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#pragma once

#include "abstract.h"
#include "super.hh"	// RA2: SuperWeaponType for CurrentSuperWeaponType
#include "ftimer.h"
#include "timer.h"

class SuperWeaponTypeClass;
class HouseClass;

// RA2: the three fields at +0x3C of SuperClass; what they hold is not known (Enable clears the first, Load the other two)
struct SuperBlinkInfo
{
	SuperBlinkInfo(void) {Data0 = 0; State = false; Data1 = 0;}
	SuperBlinkInfo(NoInitClass const &) {}

	bool State; // RA2: +0x00
	int Data0; // RA2: +0x04
	int Data1; // RA2: +0x08
};

class SuperClass : public AbstractClass
{
		typedef AbstractClass BASECLASS;

	public:
		SuperClass(void);
		SuperClass(SuperWeaponTypeClass * type, HouseClass * owner);
		SuperClass(NoInitClass const & x) : Control(x), BlinkInfo(x) {}; // RA2: BlinkInfo is left alone as well
		virtual ~SuperClass(void) override;

		virtual HRESULT STDMETHODCALLTYPE GetClassID(CLSID * retval) override;
		virtual HRESULT STDMETHODCALLTYPE Load(IStream * stream) override;
		virtual HRESULT STDMETHODCALLTYPE Save(IStream * stream, BOOL cleardirty) override;

		virtual int STDMETHODCALLTYPE What_Am_I(void) const override {return(SuperClass::Fetch_RTTI());};

		virtual RTTIType Fetch_RTTI(void) const override {return(RTTI_SUPERWEAPON);}
		virtual int Fetch_Object_Size(void) const override {return(sizeof(*this));} // RA2: no argument

		virtual void Compute_CRC(CRCEngine & crc) const override;

		void Detach(AbstractClass const * target, bool all);
		void Detach_Animation(void); // RA2: 0x0069CA70; drops the animation (stops its loops) and the animation list entry
		void Create_Chrono_Anim(Coord coord); // RA2: 0x0069CB30; replaces the animation with the chrono placement marker at coord
		void Reset(void); // RA2: 0x0069EBB0; drops the animation, clears the ready flag and sets the timer to the full charge time

		bool Suspend(bool on);
		bool Enable(bool onetime = false, bool player=false, bool quiet=false);
		void Forced_Charge(bool player=false);
		void Place(Cell const & cell, bool player);
		void Deactivate_Firestorm(int, bool player) const; // RA2: 0x0069E980; the body is empty
		bool AI(bool player=false);
		bool Remove(void);
		void Set_Ready(bool ready); // RA2: 0x0069CFB0; sets the ready flag, nothing else (YR: SetReadiness)
		void Impatient_Click(void) const;
		int Anim_Stage(void) const;
		bool Discharged(bool player, Cell const & cell);
		bool Can_Place(void); // RA2: 0x0069D820; not const
		bool Is_Charging(void) const;
		bool Is_Powered(void) const;
		bool Is_Ready(void) const {return(IsReady);}
		bool Is_Present(void) const {return(IsPresent);}
		bool Is_One_Time(void) const {return(IsOneTime && IsPresent);}

		wchar_t const * State_String(void) const; // RA2: 0x0069D770; the text is wide

	public:
		/*
		 * This points to the type class that describes this super weapon -- its recharge
		 * time, its cameo, and the action it performs when it is unleashed.
		 */
		SuperWeaponTypeClass *Class;

		bool Recharge(bool player=false);

		/*
		 * This is the house that owns this super weapon and that is credited as the
		 * attacker for whatever the weapon unleashes.
		 */
		HouseClass * House;

		CDTimerClass<FrameTimerClass> Control;

		wchar_t const * UIName; // RA2: +0x38
		SuperBlinkInfo BlinkInfo; // RA2: +0x3C, 12 bytes (a bool, then two ints; the constructors clear them in the order +4, +0, +8, Load clears the two ints again); the class stays 0x68 bytes

	public:
		/*
		 * If this super weapon lasts only as long as the house owns a building that grants
		 * it, then this flag will be true. A trigger action can clear it to hand the weapon
		 * over permanently.
		 */
		bool NeedsBuilding;

		Cell ChronoMapCoords; // RA2: +0x4A
		AnimClass * Animation; // RA2: +0x50
		bool AnimationGotInvalid; // RA2: +0x54
	private:
		bool IsPresent;
		bool IsOneTime;
		bool IsReady;
		friend class Tactical; // RA2: Render reads the suspended flag for the super weapon timer.
		bool IsSuspended;

		int OldStage;

		/*
		 * This is where a charge drain weapon (such as the firestorm defense) sits in its
		 * cycle. Such a weapon charges to full, stays ready until it is switched on, and
		 * then drains back down while it is running instead of discharging in one shot.
		 */
		enum {
			SUSPENDED = -1,
			CHARGING,
			READY,
			FIRESTORM_ON
		} ChargeDrainState;

		int Unused_64; // RA2: +0x64; no code reads it, but the executable allocates 0x68 bytes per super weapon (replaces TibSun's ANIMATION_STAGES enum, which nothing used)
};

extern SuperWeaponType CurrentSuperWeaponType; // RA2: 0x00833694; the type waiting for a target (SUPER_CHRONO_WARP, while the destination is chosen), else -1
extern DynamicVectorClass<SuperClass *> TimedSuperWeapons; // RA2: 0x00A35DB8; the super weapons whose timer shows (ShowTimer) while the house has them
