// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Renamed for RA2 1.006 by RedAlert2 contributors, 2026.
#pragma once

#include "abstract.h"
#include "ftimer.h"
#include "timer.h"
#include "vector.h"

EXTERN_C const CLSID CLSID_TemporalClass;

class TemporalClass;
extern DynamicVectorClass<TemporalClass *> Temporals;

class SuperClass;
class TechnoClass;

class TemporalClass : public AbstractClass
{
		typedef AbstractClass BASECLASS;

	public:
		TechnoClass * Owner;
		TechnoClass * Target;
		CDTimerClass<FrameTimerClass> LifeTimer;
		void * Unused_38; // RA2: only ever set to NULL (and swizzled on load)
		SuperClass * SourceSW;
		TemporalClass * NextTemporal;
		TemporalClass * PrevTemporal;
		int WarpRemaining;
		int WarpPerStep;

		TemporalClass(void);
		TemporalClass(TechnoClass * owner);
		TemporalClass(NoInitClass const & x);
		virtual ~TemporalClass(void) override;

		virtual HRESULT STDMETHODCALLTYPE GetClassID(CLSID * retval) override;
		virtual HRESULT STDMETHODCALLTYPE Load(IStream * stream) override;
		virtual HRESULT STDMETHODCALLTYPE Save(IStream * stream, BOOL cleardirty) override;

		virtual RTTIType Fetch_RTTI(void) const override;
		virtual int Fetch_Object_Size(void) const override;
		virtual void Compute_CRC(CRCEngine & crc) const override;
		virtual void AI(void) override;

		int GetWarpPerStep(int helpercount = 0);
		void Detach(void);
		void Detach_Techno(AbstractClass const * target); // RA2: 0x6E24C0, forgets the target (or lets go of it) when it is the one detached
		void LetGo(void);
		void JustLetGo(void);
		void Release_Target(void); // RA2: 0x6E25F0, drops the target and clears its warp flags; nothing calls it
		void Restore_Target(void); // RA2: 0x6E2610, frees the target (a building is powered again) and redraws it; nothing calls it
		void Empty(void); // RA2: 0x6E2670, an empty function; nothing calls it
		void Clear_Links(void); // RA2: 0x6E26F0, forgets the target and both neighbours; nothing calls it
		bool CanWarpTarget(TechnoClass * target);
		void Fire(TechnoClass * target);
};
