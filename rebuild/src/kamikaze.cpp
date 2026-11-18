/*
**	Command & Conquer Red Alert 2
**	KAMIKAZE.CPP: the kamikaze tracker.
**
**	A launcher's spawned missiles are handed to the tracker, which keeps steering each one at
**	its cell until the missile is gone. A missile type that is not a spawned missile simply
**	crashes.
*/

#include "always.h"
#include "geometry.h"
#include "rect.h"
#include "kamikaze.h"
#include "aircraft.h"
#include "airctype.h"
#include "cell.h"
#include "globals.h"
#include "swizzle.h"
#include "_map.h"


KamikazeClass Kamikazes;


KamikazeClass::KamikazeClass(void)
{
	UpdateTimer = 100;
}


/*
**	Takes a missile into the tracker, aimed at the target's cell, or at the cell ahead of the
**	missile when there is no target. A missile type that does not spawn crashes at once.
*/
void KamikazeClass::Add(AircraftClass * missile, AbstractClass * target)
{
	if (!missile->Class->MissileSpawn) {
		missile->Crash(NULL);
		return;
	}

	KamikazeControl * node = new KamikazeControl;
	node->Unit = missile;
	if (target == NULL) {
		node->Cell = &missile->Get_Cell_Ptr()->Adjacent_Cell(missile->PrimaryFacing.Current().As_Dir8());
	} else {
		node->Cell = &Map[Cell(target->Center_Coord())];
	}
	missile->IsKamikaze = true;
	missile->Ammo = 1;
	Nodes.Add(node);
}


/*
**	Every thirty frames each missile is aimed at its cell again.
*/
#ifdef RA2_NON_MATCHING // not exact: 50 of 187 bytes differ (the null-cell branch should push its own argument)
void KamikazeClass::Update(void)
{
	if (UpdateTimer != 0) return;
	UpdateTimer = 30;

	for (int index = 0; index < Nodes.Count(); index++) {
		KamikazeControl * node = Nodes[index];
		CellClass * cell = node->Cell;
		AircraftClass * missile = node->Unit;
		missile->Ammo = 1;
		missile->Assign_Target(cell != NULL ? cell : &missile->Get_Cell_Ptr()->Adjacent_Cell(missile->PrimaryFacing.Current().As_Dir8()));
		missile->QueueMission(MISSION_ATTACK, false);
	}
}
#endif


/*
**	An object is going away: a missile leaves the tracker; a missile aimed at it is aimed at
**	the object's cell instead.
*/
void KamikazeClass::Remove(AbstractClass * target)
{
	for (int index = Nodes.Count() - 1; index >= 0; index--) {
		KamikazeControl * node = Nodes[index];
		if (target == node->Unit) {
			delete Nodes[index];
			Nodes.Delete_Index(index);
			return;
		}
		if (target == node->Cell) {
			CellClass * cell = &Map[Cell(target->Center_Coord())];
			Nodes[index]->Unit->Assign_Target(cell);
			Nodes[index]->Unit->QueueMission(MISSION_ATTACK, false);
			Nodes[index]->Cell = cell;
			return;
		}
	}
}


KamikazeClass::~KamikazeClass(void)
{
	for (int index = Nodes.Count() - 1; index >= 0; index--) {
		delete Nodes[index];
	}
	Nodes.Delete_All();
}


void KamikazeClass::Clear(void)
{
	for (int index = Nodes.Count() - 1; index >= 0; index--) {
		delete Nodes[index];
	}
	Nodes.Delete_All();
	UpdateTimer = 1;
}


/*
**	The record count, then each record as it is in memory.
*/
HRESULT KamikazeClass::Save(IStream * stream)
{
	int count = Nodes.Count();
	HRESULT hr = stream->Write(&count, sizeof(count), NULL);
	if (FAILED(hr)) return(hr);

	for (int index = 0; index < count; index++) {
		hr = stream->Write(Nodes[index], sizeof(KamikazeControl), NULL);
		if (FAILED(hr)) return(hr);
	}
	return(hr);
}


/*
**	Reads the records Save wrote; their pointers are swizzled.
*/
HRESULT KamikazeClass::Load(IStream * stream)
{
	int count;
	HRESULT hr = stream->Read(&count, sizeof(count), NULL);
	if (FAILED(hr)) return(hr);

	for (int index = 0; index < count; index++) {
		KamikazeControl * node = new KamikazeControl;
		hr = stream->Read(node, sizeof(KamikazeControl), NULL);
		if (FAILED(hr)) return(hr);
		Swizzle_Pointer(&node->Unit);
		Swizzle_Pointer(&node->Cell);
		Nodes.Add(node);
	}
	return(hr);
}
