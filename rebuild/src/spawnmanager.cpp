/*
**	Command & Conquer Red Alert 2
**	SPAWNMANAGER.CPP: the spawn manager.
**
**	A carrier or a missile silo owns a spawn manager. It builds the aircraft (or missiles) the
**	owner launches, keeps a record for each, sends them at the owner's target, takes them back
**	when they dock, and rebuilds the ones that were lost.
*/

#include "always.h"
#include "geometry.h"
#include "rect.h"
#include "spawnmanager.h"
#include "aircraft.h"
#include "airctype.h"
#include "crc.h"
#include "globals.h"
#include "house.h"
#include "isun.h"
#include "kamikaze.h"
#include "_rules.h"
#include "rules.h"
#include "swizzle.h"
#include "techno.h"
#include "foot.h"
#include "iloco.h"
#include "_weapon.h"
#include "weapon.h"
#include "cell.h"
#include "_map.h"


DynamicVectorClass<SpawnManagerClass *> SpawnManagers;


/*
**	A spawn manager for an owner: one record per aircraft, each built at once, held in limbo
**	inside the owner until it is launched.
*/
SpawnManagerClass::SpawnManagerClass(TechnoClass * owner, AircraftTypeClass * type, int count, int regen, int reload)
{
	Owner = owner;
	SpawnType = type;
	SpawnCount = count;
	RegenRate = regen;
	ReloadRate = reload;
	for (int index = 0; index < SpawnCount; index++) {
		SpawnControl * node = new SpawnControl;
		if (node == NULL) break;

		node->Unit = (AircraftClass *)SpawnType->Create_One_Of(Owner->Owner_HouseClass());
		if (node->Unit != NULL) {
			if (SpawnType == Rule->V3Rocket.Type || SpawnType == Rule->DMisl.Type) {
				node->IsMissile = true;
			} else {
				node->IsMissile = false;
			}
			node->Unit->Limbo();
			node->Unit->SpawnOwner = Owner;
			node->Status = 0;
			node->Timer = 0;
			SpawnedNodes.Add(node);
		}
	}
	UpdateTimer = 20;
	Destination = NULL;
	Target = NULL;
	Status = 0;
	SpawnTimer = 0;
	SpawnManagers.Add(this);
}


SpawnManagerClass::SpawnManagerClass(void)
{
	Owner = NULL;
	SpawnType = NULL;
	SpawnCount = 0;
	UpdateTimer = 0;
	Destination = NULL;
	Target = NULL;
	Status = 0;
	SpawnTimer = 0;
	SpawnManagers.Add(this);
}


SpawnManagerClass::SpawnManagerClass(NoInitClass const & x) :
	AbstractClass(x)
{
}


SpawnManagerClass::~SpawnManagerClass(void)
{
	if (GameActive) {
		KillNodes();
	}
	for (int index = SpawnedNodes.Count() - 1; index >= 0; index--) {
		if (SpawnedNodes[index] != NULL) {
			delete SpawnedNodes[index];
		}
	}
	SpawnedNodes.Delete_All();
	SpawnManagers.Delete(this);
}


/*
**	Each record moves through its states; then the manager's own state follows the target.
*/
void SpawnManagerClass::AI(void)
{
	if (UpdateTimer != 0) return;
	UpdateTimer = 10;

	for (int index = 0; index < SpawnedNodes.Count(); index++) {
		bool burst = false;
		AircraftClass * unit = SpawnedNodes[index]->Unit;
		switch (SpawnedNodes[index]->Status) {

			/*
			**	Docked and ready: launch it at the destination.
			*/
			case 0:
			{
				if (Destination == NULL) break;
				if (SpawnTimer != 0) break;
				if (Status == 2) break;
				if (Owner->House->Ion_Sensitives_Should_Be_Offline()) break;
				if (SpawnedNodes[index]->IsMissile == 1) {
					if (((FootClass *)Owner)->Locomotion->Is_Moving()) break;
					if (((FootClass *)Owner)->Locomotion->Is_Moving_Now()) break;
				}
				if (Owner->Techno_Type_Class()->MissileSpawn) {
					SpawnTimer = 9;
				} else {
					SpawnTimer = 20;
				}

				if (SpawnedNodes[index]->IsMissile == 1) {
					WeaponTypeClass * weapon = Owner->Get_Class_Weapon_Data(0)->Weapon;
					if (weapon != NULL && weapon->Burst > 1) {
						burst = true;
						Owner->BurstIndex = index & 1;
					}
				}

				SpawnedNodes[index]->Status = 2;
				Coord coord = Owner->Fire_Coord(0);
				coord.Z += 10;
				unit->Unlimbo(coord, Owner->PrimaryFacing.Current().As_Dir256());
				if (burst) {
					Owner->BurstIndex = 0;
				}

				if (SpawnedNodes[index]->IsMissile == 1) {
					UpdateTarget();
					unit->Assign_Destination(Destination, true);
				} else {
					unit->Assign_Destination(&Map[Owner->Get_Cell()].Adjacent_Cell(FACING_N), true);
				}
				unit->QueueMission(MISSION_MOVE, false);
				break;
			}

			/*
			**	A missile in its launch: when its timer runs out it is on its own.
			*/
			case 1:
				if (SpawnedNodes[index]->Timer == 0) {
					UnlinkPointer(unit);
				}
				break;

			/*
			**	Taking off: head for the target, or home when there is none.
			*/
			case 2:
				if (SpawnedNodes[index]->IsMissile == 1) break;
				UpdateTarget();
				if (Destination == NULL) {
					unit->Assign_Destination(Owner, true);
					unit->Assign_Target(NULL);
					unit->QueueMission(MISSION_MOVE, false);
					SpawnedNodes[index]->Status = 4;
				} else {
					unit->Assign_Destination(&Map[Owner->Get_Cell()].Adjacent_Cell(FACING_S), true);
					unit->QueueMission(MISSION_MOVE, false);
				}
				break;

			/*
			**	Attacking: keep at the target while there is ammunition, else return.
			*/
			case 3:
				UpdateTarget();
				if (unit->Ammo == 0 || Destination == NULL) {
					unit->Assign_Destination(Owner, true);
					unit->Assign_Target(NULL);
					unit->QueueMission(MISSION_MOVE, false);
					SpawnedNodes[index]->Status = 4;
				} else {
					SpawnedNodes[index]->Unit->Assign_Target(Destination);
					SpawnedNodes[index]->Unit->QueueMission(MISSION_ATTACK, false);
				}
				break;

			/*
			**	Returning: attack again if there is a target and ammunition; dock once over
			**	the owner.
			*/
			case 4:
				UpdateTarget();
				if (unit->Ammo > 0 && Destination != NULL) {
					SpawnedNodes[index]->Status = 3;
					SpawnedNodes[index]->Unit->Assign_Target(Destination);
					SpawnedNodes[index]->Unit->QueueMission(MISSION_ATTACK, false);
					break;
				}
				if (unit->Get_Cell() == Owner->Get_Cell() && unit->Get_Coord().Z - Owner->Get_Coord().Z < 20) {
					unit->Limbo();
					SpawnedNodes[index]->Status = 6;
					SpawnedNodes[index]->Timer = ReloadRate;
				} else {
					unit->Assign_Destination(Owner, true);
					unit->Assign_Target(NULL);
					unit->QueueMission(MISSION_MOVE, false);
				}
				break;

			/*
			**	Docked and reloading.
			*/
			case 6:
				if (SpawnedNodes[index]->Timer == 0) {
					SpawnedNodes[index]->Status = 0;
					unit->Ammo = unit->Class->MaxAmmo;
					unit->EstimatedHealth = unit->Strength = unit->Class->MaxStrength;
				}
				break;

			/*
			**	Lost: build a new one when the timer runs out.
			*/
			case 7:
				if (SpawnedNodes[index]->Timer == 0) {
					SpawnedNodes[index]->Unit = (AircraftClass *)SpawnType->Create_One_Of(Owner->Owner_HouseClass());
					if (SpawnType == Rule->V3Rocket.Type || SpawnType == Rule->DMisl.Type) {
						SpawnedNodes[index]->IsMissile = true;
					} else {
						SpawnedNodes[index]->IsMissile = false;
					}
					SpawnedNodes[index]->Unit->Limbo();
					SpawnedNodes[index]->Unit->SpawnOwner = Owner;
					SpawnedNodes[index]->Status = 0;
				}
				break;
		}
	}

	bool reset = false;
	switch (Status) {

		/*
		**	Idle: a target in reach starts a launch.
		*/
		case 0:
			UpdateTarget();
			if (Destination != NULL) {
				if (!Owner->IsCloseEnoughToAttack(Destination)) {
					ResetTarget();
					return;
				}
				Status = 1;
			}
			break;

		/*
		**	Launching: once every aircraft is out (or lost), missiles go on alone and the rest
		**	attack.
		*/
		case 1:
		{
			if (Destination == NULL) {
				ResetTarget();
				return;
			}
			int index;
			for (index = 0; index < SpawnedNodes.Count(); index++) {
				if (SpawnedNodes[index]->Status != 2 && SpawnedNodes[index]->Status != 7) return;
			}
			for (index = 0; index < SpawnedNodes.Count(); index++) {
				if (SpawnedNodes[index]->Status == 2) {
					if (SpawnedNodes[index]->Unit->Class->MissileSpawn) {
						reset = true;
						Kamikazes.Add(SpawnedNodes[index]->Unit, Destination);
						Kamikazes.UpdateTimer = 2;
						if (SpawnedNodes[index]->IsMissile == 1) {
							SpawnedNodes[index]->Status = 1;
							if (SpawnType == Rule->V3Rocket.Type) {
								SpawnedNodes[index]->Timer = Rule->V3Rocket.PauseFrames + Rule->V3Rocket.TiltFrames;
							} else {
								SpawnedNodes[index]->Timer = Rule->DMisl.PauseFrames + Rule->DMisl.TiltFrames;
							}
						} else {
							UnlinkPointer(SpawnedNodes[index]->Unit);
						}
					} else {
						SpawnedNodes[index]->Status = 3;
						SpawnedNodes[index]->Unit->Assign_Target(Destination);
						SpawnedNodes[index]->Unit->QueueMission(MISSION_ATTACK, false);
					}
				}
			}
			if (reset) {
				ResetTarget();
			}
			Status = 2;
			break;
		}

		/*
		**	Out: back to idle once nothing is attacking or returning.
		*/
		case 2:
		{
			bool done = true;
			for (int index = 0; index < SpawnedNodes.Count(); index++) {
				if (SpawnedNodes[index]->Status == 3 || SpawnedNodes[index]->Status == 4) done = false;
			}
			if (done) {
				Status = 0;
			}
			break;
		}
	}
}


/*
**	Every aircraft is taken away: a docked one is deleted, a launching missile leaves the
**	kamikaze tracker and is deleted, one in flight goes on alone as a kamikaze at the
**	destination. Each record then waits to be rebuilt.
*/
void SpawnManagerClass::KillNodes(void)
{
	for (int index = SpawnedNodes.Count() - 1; index >= 0; index--) {
		int status = SpawnedNodes[index]->Status;
		if (status != 7) {
			if (status == 0 || status == 6) {
				SpawnedNodes[index]->Status = 7;
				SpawnedNodes[index]->Unit->Delete_Me();
				SpawnedNodes[index]->Unit = NULL;
				SpawnedNodes[index]->Timer = RegenRate;
			} else if (status == 1) {
				Kamikazes.Remove(SpawnedNodes[index]->Unit);
				SpawnedNodes[index]->Status = 7;
				SpawnedNodes[index]->Unit->Delete_Me();
				SpawnedNodes[index]->Unit = NULL;
				SpawnedNodes[index]->Timer = RegenRate;
			} else {
				SpawnedNodes[index]->Status = 7;
				Kamikazes.Add(SpawnedNodes[index]->Unit, Destination);
				SpawnedNodes[index]->Unit = NULL;
				SpawnedNodes[index]->Timer = RegenRate;
			}
		}
	}
}


/*
**	A new target, unless it is already the destination.
*/
void SpawnManagerClass::SetTarget(AbstractClass * target)
{
	if (target != Destination) {
		Target = target;
	}
}


/*
**	The target is given up: missiles in flight go on alone at the destination.
*/
void SpawnManagerClass::ResetTarget(void)
{
	for (int index = 0; index < SpawnedNodes.Count(); index++) {
		SpawnControl * node = SpawnedNodes[index];
		if (node->Status == 2 && node->Unit->Class->MissileSpawn) {
			Kamikazes.Add(node->Unit, Destination);
			Kamikazes.UpdateTimer = 2;
			UnlinkPointer(SpawnedNodes[index]->Unit);
		}
	}
	Status = 0;
	Target = NULL;
	Destination = NULL;
}


/*
**	The queued target becomes the destination.
*/
bool SpawnManagerClass::UpdateTarget(void)
{
	if (Target != NULL) {
		Destination = Target;
		Target = NULL;
		return(true);
	}
	return(false);
}


/*
**	An object is going away.
*/
void SpawnManagerClass::UnlinkPointer(AbstractClass * object)
{
	if (object == Destination) {
		Destination = NULL;
		if (Target == NULL) {
			ResetTarget();
		}
		return;
	}

	if (object == Target) {
		Target = NULL;
		return;
	}

	for (int index = SpawnedNodes.Count() - 1; index >= 0; index--) {
		if (object == SpawnedNodes[index]->Unit) {
			SpawnControl * node = SpawnedNodes[index];
			if (node->Unit->Strength <= 0 || node->Unit->IsKamikaze || node->IsMissile == 1) {
				node->Unit = NULL;
				SpawnedNodes[index]->Status = 7;
				SpawnedNodes[index]->Timer = RegenRate;
			}
			return;
		}
	}

	if (object == Owner) {
		KillNodes();
		ResetTarget();
	}
}


int SpawnManagerClass::CountAliveSpawns(void)
{
	int count = 0;
	for (int index = 0; index < SpawnedNodes.Count(); index++) {
		if (SpawnedNodes[index]->Status != 7) count++;
	}
	return(count);
}


int SpawnManagerClass::CountDockedSpawns(void)
{
	int count = 0;
	for (int index = 0; index < SpawnedNodes.Count(); index++) {
		if (SpawnedNodes[index]->Status == 6 || SpawnedNodes[index]->Status == 0) count++;
	}
	return(count);
}


int SpawnManagerClass::CountLaunchingSpawns(void)
{
	int count = 0;
	for (int index = 0; index < SpawnedNodes.Count(); index++) {
		if (SpawnedNodes[index]->Status == 1) count++;
	}
	return(count);
}


void SpawnManagerClass::Compute_CRC(CRCEngine & crc) const
{
	BASECLASS::Compute_CRC(crc);
	crc(Status);
	if (Target != NULL) crc(Target->Fetch_ID());
	if (Destination != NULL) crc(Destination->Fetch_ID());
	crc(SpawnTimer);
	crc(UpdateTimer);
	crc(SpawnedNodes.Count());
	crc(SpawnCount);
	if (SpawnType != NULL) crc(SpawnType->Fetch_ID());
	if (Owner != NULL) crc(Owner->Fetch_ID());
}


HRESULT STDMETHODCALLTYPE SpawnManagerClass::GetClassID(CLSID * classid)
{
	if (classid == NULL) {
		return(E_POINTER);
	}
	*classid = CLSID_SpawnManagerClass;
	return(S_OK);
}


HRESULT STDMETHODCALLTYPE SpawnManagerClass::Load(IStream * stream)
{
	HRESULT hr = BASECLASS::Load(stream);
	if (SUCCEEDED(hr)) {
		new (this) SpawnManagerClass(NoInitClass());

		int count;
		hr = stream->Read(&count, sizeof(count), NULL);
		if (SUCCEEDED(hr)) {
			new (&SpawnedNodes) DynamicVectorClass<SpawnControl *>;
			for (int index = 0; index < count; index++) {
				SpawnControl * node = new SpawnControl;
				hr = stream->Read(node, sizeof(SpawnControl), NULL);
				if (FAILED(hr)) return(hr);
				Swizzle_Pointer(&node->Unit);
				SpawnedNodes.Add(node);
			}

			Swizzle_Pointer(&Owner);
			Swizzle_Pointer(&SpawnType);
			Swizzle_Pointer(&Destination);
			Swizzle_Pointer(&Target);
		}
	}
	return(hr);
}


HRESULT STDMETHODCALLTYPE SpawnManagerClass::Save(IStream * stream, BOOL cleardirty)
{
	HRESULT hr = BASECLASS::Save(stream, cleardirty);
	if (FAILED(hr)) return(hr);

	int count = SpawnedNodes.Count();
	hr = stream->Write(&count, sizeof(count), NULL);
	if (FAILED(hr)) return(hr);

	for (int index = 0; index < count; index++) {
		hr = stream->Write(SpawnedNodes[index], sizeof(SpawnControl), NULL);
		if (FAILED(hr)) return(hr);
	}
	return(hr);
}


int SpawnManagerClass::Fetch_Object_Size(void) const
{
	return(sizeof(SpawnManagerClass));
}


RTTIType SpawnManagerClass::Fetch_RTTI(void) const
{
	return(RTTI_SPAWNMANAGER);
}
