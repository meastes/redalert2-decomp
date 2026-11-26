/*
**	Command & Conquer Red Alert 2
**	The kamikaze tracker: missiles spawned by a launcher fly at a cell until they hit it.
*/
#pragma once

#include "win.h"
#include "timer.h"
#include "ftimer.h"
#include "vector.h"

class AbstractClass;
class AircraftClass;
class CellClass;
struct IStream;

/*
**	One missile and the cell it flies at.
*/
struct KamikazeControl
{
		AircraftClass * Unit;
		CellClass * Cell;
};


class KamikazeClass
{
	public:
		KamikazeClass(void);
		~KamikazeClass(void);

		void Add(AircraftClass * missile, AbstractClass * target);
		void Update(void);
		void Remove(AbstractClass * target);
		void Clear(void);

		HRESULT Save(IStream * stream);
		HRESULT Load(IStream * stream);

		/*
		**	The missiles are steered again whenever this runs out (the spawn manager
		**	shortens it when it lets its missiles go).
		*/
		CDTimerClass<FrameTimerClass> UpdateTimer;

		DynamicVectorClass<KamikazeControl *> Nodes;
};

extern KamikazeClass Kamikazes;
