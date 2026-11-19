/*
**	Command & Conquer Red Alert 2
**	The spawn manager: the aircraft a carrier or a missile silo launches, docks and rebuilds.
*/
#pragma once

#include "abstract.h"
#include "ftimer.h"
#include "timer.h"
#include "vector.h"

class AircraftClass;
class AircraftTypeClass;
class TechnoClass;

/*
**	One spawned aircraft (the RTTI names it at namespace scope). A dead slot (status 7) waits
**	for its timer before a new aircraft is built into it.
*/
struct SpawnControl
{
		AircraftClass * Unit;
		int Status;
		CDTimerClass<FrameTimerClass> Timer;
		int IsMissile;
};


class SpawnManagerClass : public AbstractClass
{
		typedef AbstractClass BASECLASS;

	public:
		SpawnManagerClass(TechnoClass * owner, AircraftTypeClass * type, int count, int regen, int reload);
		SpawnManagerClass(void);
		SpawnManagerClass(NoInitClass const & x);
		virtual ~SpawnManagerClass(void) override;

		virtual HRESULT STDMETHODCALLTYPE GetClassID(CLSID * classid) override;
		virtual HRESULT STDMETHODCALLTYPE Load(IStream * stream) override;
		virtual HRESULT STDMETHODCALLTYPE Save(IStream * stream, BOOL cleardirty) override;

		virtual RTTIType Fetch_RTTI(void) const override;
		virtual int Fetch_Object_Size(void) const override;
		virtual void Compute_CRC(CRCEngine & crc) const override;
		virtual void AI(void) override;

		void KillNodes(void);
		void SetTarget(AbstractClass * target);
		void ResetTarget(void);
		bool UpdateTarget(void);
		void UnlinkPointer(AbstractClass * object);
		int CountAliveSpawns(void);
		int CountDockedSpawns(void);
		int CountLaunchingSpawns(void);

		TechnoClass * Owner;
		AircraftTypeClass * SpawnType;
		int SpawnCount;
		int RegenRate;
		int ReloadRate;
		DynamicVectorClass<SpawnControl *> SpawnedNodes;
		CDTimerClass<FrameTimerClass> UpdateTimer;
		CDTimerClass<FrameTimerClass> SpawnTimer;
		AbstractClass * Destination;
		AbstractClass * Target;
		int Status;
};

extern DynamicVectorClass<SpawnManagerClass *> SpawnManagers;
