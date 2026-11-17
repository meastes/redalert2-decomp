// SPDX-License-Identifier: GPL-3.0-or-later
// RA2: not in TibSun. The electric bolt drawn by IsElectricBolt weapons (names from YRpp's EBolt.h).
#pragma once

#include "coord.h"
#include "vector.h"

class UnitClass;
class TechnoClass;

class EBolt
{
	public:
		Coord Point1;
		Coord Point2;
		int Unknown_18;
		int Random;
		TechnoClass * Owner;
		int WeaponSlot;
		int Lifetime;
		bool AlternateColor; // RA2: +0x2C

		EBolt(void); // RA2: 0x4B17F0
		void Clear_Owner(void); // RA2: 0x4B1830 (YR name ClearOwner); clears Owner's ElectricBolt pointer and Owner
		void Draw_Line(Coord start, Coord end, int color, int start_depth, int end_depth); // RA2: 0x4B1850
		void Draw_Bolt(Coord start, Coord end, int color); // RA2: 0x4B1900
		void Draw(void); // RA2: 0x4B2170
		static void __fastcall Update_All(void); // RA2: 0x4B2210
		static void __cdecl Delete_All(void); // RA2: 0x4B23B0
		void Set_Owner(TechnoClass * owner, int which); // RA2: 0x4B2580
		void Fire(Coord source, Coord target, int zadjust); // RA2: 0x4B2430
		Coord Owner_Coord(void) const; // RA2: 0x4B2510
		~EBolt(void); // RA2: 0x4B25C0

		static DynamicVectorClass<EBolt *> Bolts; // RA2: 0x853050
};
