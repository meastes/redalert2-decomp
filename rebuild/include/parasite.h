// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
#pragma once

#include "abstract.h"
#include "vector.h"
#include "ftimer.h"
#include "timer.h"

class AnimClass;
class FootClass;

class ParasiteClass;

EXTERN_C const CLSID CLSID_ParasiteClass;
extern DynamicVectorClass<ParasiteClass *> Parasites;

class ParasiteClass : public AbstractClass
{
		typedef AbstractClass BASECLASS;

	public:
		FootClass * Owner;
		FootClass * Victim;
		CDTimerClass<FrameTimerClass> SuppressionTimer;
		CDTimerClass<FrameTimerClass> DamageDeliveryTimer;
		AnimClass * GrappleAnim;
		int GrappleState;
		int GrappleAnimFrame;
		int GrappleAnimDelay;
		bool GrappleAnimGotInvalid;

		ParasiteClass(void);
		ParasiteClass(NoInitClass const & x);
		ParasiteClass(FootClass * owner);
		virtual ~ParasiteClass(void) override;

		virtual HRESULT STDMETHODCALLTYPE GetClassID(CLSID * retval) override;
		virtual HRESULT STDMETHODCALLTYPE Load(IStream * stream) override;
		virtual HRESULT STDMETHODCALLTYPE Save(IStream * stream, BOOL cleardirty) override;

		virtual RTTIType Fetch_RTTI(void) const override;
		virtual int Fetch_Object_Size(void) const override;
		virtual void Detach(AbstractClass const * target, bool all = true) override;
		virtual void Compute_CRC(CRCEngine & crc) const override;
		virtual void AI(void) override;

		void Clear_Grapple(void); // RA2: 0x006064C0; drops the grapple animation and resets the grapple state
		void Reset_Grapple_Counters(void); // RA2: 0x006066A0; restarts the grapple animation at its first frame
		Coord Get_Exit_Coord(void); // RA2: 0x00607C30; where the owner comes out when the victim goes away
		bool UpdateGrapple(void); // RA2: 0x006066B0
		void UpdateSquid(void); // RA2: 0x00606780; one frame of a squid holding a ship
		void ExitUnit(void); // RA2: 0x00607440; the owner leaves the victim
		bool CanInfect(FootClass * victim) const; // RA2: 0x006078F0
		void TryInfect(FootClass * target); // RA2: 0x00607980
		bool CanExistOnVictimCell(void) const; // RA2: 0x00607BA0; no caller (ExitUnit and Get_Exit_Coord expand the same test), the executable keeps the copy
};
