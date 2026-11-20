// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Changed to RA2 1.006's layout by RedAlert2 contributors, 2026.
#pragma once

#include "abstract.h"
#include "ftimer.h"
#include "timer.h"
#include "vector.h"

class HouseClass;
class TechnoClass;

EXTERN_C const CLSID CLSID_CaptureManagerClass; // RA2: 0x007A1DE0, the class identifier GetClassID returns

class CaptureManagerClass : public AbstractClass
{
		typedef AbstractClass BASECLASS;

	public:
		TechnoClass * Unit; // RA2: +0x24, the one unit under control (YR: ControlNode::Unit)
		HouseClass * OriginalOwner; // RA2: +0x28, its house before capture (YR: ControlNode::OriginalOwner)
		TechnoClass * Owner; // RA2: +0x2C

		CaptureManagerClass(void); // RA2: 0x00467E40, no owner
		CaptureManagerClass(TechnoClass * owner, int damage, bool infinite);
		CaptureManagerClass(TechnoClass * owner); // RA2: 0x467DB0
		CaptureManagerClass(NoInitClass const & x); // RA2: 0x00467ED0
		virtual ~CaptureManagerClass(void) override; // RA2: 0x00467F00, leaves the list

		bool Capture(TechnoClass * target); // RA2: 0x00467FF0, gives back the controlled unit, then takes the target over
		bool Detach_Target(AbstractClass const * target); // RA2: 0x468120, forgets the controlled unit when it is the target
		void FreeAll(void); // RA2: 0x4681E0, returns the controlled unit to its original house and forgets it (one unit only in 1.006)
		bool Detach_Unused(AbstractClass * target); // RA2: 0x468140, an empty function that returns true; nothing calls it
		bool Can_Capture(TechnoClass * target); // RA2: 0x00467F80, whether the unit can be mind controlled
		void Draw_Link(void) const; // RA2: 0x00468150, draws the link to the controlled unit

		virtual HRESULT STDMETHODCALLTYPE GetClassID(CLSID * retval) override;
		virtual HRESULT STDMETHODCALLTYPE Load(IStream * stream) override;
		virtual HRESULT STDMETHODCALLTYPE Save(IStream * stream, BOOL cleardirty) override;

		virtual RTTIType Fetch_RTTI(void) const override;
		virtual int Fetch_Object_Size(void) const override;
		virtual void Compute_CRC(CRCEngine & crc) const override;
};

extern DynamicVectorClass<CaptureManagerClass *> CaptureManagerArray;
