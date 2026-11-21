/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/

/*
**	RA2: the bombs that Crazy Ivan plants (BombClass) and the list that keeps them (BombListClass).
**	TibSun has no such file; the code is the executable's from 0x00435970.
*/

#define INCLUDE_COM // RA2: IStream
#include "geometry.h" // RA2: the geometry constants come before the rectangle of rect.h (the executable initializes them first)
#include "always.h" // RA2: the includes this file needs
#include "_map.h" // RA2: the includes this file needs
#include "_rules.h" // RA2: the includes this file needs
#include "anim.h" // RA2: the includes this file needs
#include "bomb.h" // RA2: the includes this file needs
#include "bomblist.h" // RA2: the includes this file needs
#include "builtype.h" // RA2: the includes this file needs
#include "building.h" // RA2: the includes this file needs
#include "cell.h" // RA2: the includes this file needs
#include "combat.h" // RA2: the includes this file needs
#include "crc.h" // RA2: the includes this file needs
#include "globals.h" // RA2: the includes this file needs
#include "house.h" // RA2: the includes this file needs
#include "isotype.h" // RA2: the includes this file needs
#include "map.h" // RA2: the includes this file needs
#include "noinit.h" // RA2: the includes this file needs
#include "rect.h" // RA2: the includes this file needs
#include "rules.h" // RA2: the includes this file needs
#include "swizzle.h" // RA2: the includes this file needs
#include "techno.h" // RA2: the includes this file needs
#include "techtype.h" // RA2: the includes this file needs
#include "vector.h" // RA2: the includes this file needs
#include "voc.h" // RA2: the includes this file needs



DynamicVectorClass<BombClass *> Bombs; // RA2: 0x0084EB78; every BombClass object, added by its constructor and removed by its destructor


/// <summary>
/// Creates a bomb that is not planted yet.
/// The bomb takes the ticking sound from the rules and registers itself in the list of all
/// bomb objects.
/// </summary>
BombClass::BombClass(void) : // RA2: 0x00435BE0
	BASECLASS(),
	ShouldPlayTickingSound(false) // RA2: the sound handle is not playing yet
{
	OwnerHouse = NULL; // RA2: a new bomb has no owner house and no target until it is planted
	Target = NULL; // RA2: as above
	Harmless = false; // RA2: as above
	Audio.Init(); // RA2: AudioController::Init (0x00405BF0) clears the sound handle
	TickSound = Rule->BombTickingSound; // RA2: the ticking sound comes from the rules
	Bombs.Add(this); // RA2: the bomb registers itself in the list of every bomb object
}


/// <summary>
/// Destroys the bomb.
/// The sound handle is released and the bomb leaves the list of all bomb objects.
/// </summary>
BombClass::~BombClass(void) // RA2: 0x00435C80
{
	Audio.Release(); // RA2: AudioController::Release (0x00405C10)
	Bombs.Delete(this); // RA2: the bomb leaves the list of every bomb object
}


/// <summary>
/// Creates a bomb for the save game loader.
/// The object is filled in afterwards from the stream, so nothing else is initialized.
/// </summary>
/// <param name="x">The no init marker.</param>
BombClass::BombClass(NoInitClass const & x) : // RA2: 0x00435D00
	BASECLASS(x)
{
}


/// <summary>
/// Blows the bomb up on its target.
/// The area around the target takes the Ivan damage and an explosion animation is shown. A
/// bridge repair hut that blows up takes the nearby bridge with it. Afterwards the bomb is
/// harmless and no longer belongs to anyone.
/// </summary>
void BombClass::Detonate(void) // RA2: 0x00435D30
{
	if (Target != NULL && !Harmless) { // RA2: a bomb without a target, or one that already went off or was disarmed, does nothing
		bool limbo = Target->IsInLimbo; // RA2: read before the bomb is detached from its target
		Target->AttachedBomb = NULL; // RA2: the target forgets the bomb
		if (limbo) { // RA2: an object in limbo is not on the map, so only the bomb goes away
			Target->BombVisible = false; // RA2: as above
			Harmless = true; // RA2: as above
		} else {
			Target->BombVisible = false; // RA2: as above
			Harmless = true; // RA2: as above
			Coord coord = Target->Position; // RA2: the blast is centred on the target
			Explosion_Damage(coord, Rule->IvanDamage, Owner, Rule->IvanWarhead, true); // RA2: the rules' Ivan damage and warhead, credited to the planter
			new AnimClass(Combat_Anim(Rule->IvanDamage, Rule->IvanWarhead, Map[coord.As_Cell()].Land_Type(), coord), coord, 0, 1, ShapeFlags_Type(SHAPE_CENTER | SHAPE_WIN_REL | SHAPE_ZGRAD), Get_Explosion_Z(coord)); // RA2: the explosion animation for the Ivan warhead on the land under the target; flags 0x2600 as the executable passes
			if (Target != NULL && Target->Fetch_RTTI() == RTTI_BUILDING && ((BuildingClass *)Target)->Class->IsBridgeRepairHut) { // RA2: a bridge repair hut takes the bridge next to it
				bool found = false; // RA2: whether a bridge lies within two cells of the hut
				for (int y = -2; y < 3; y++) { // RA2: as above
					for (int x = -2; x < 3; x++) { // RA2: as above
						IsometricTileType tile = Map[Target->Get_Cell() + Cell(x, y)].ITType; // RA2: as above
						OverlayType overlay = Map[Target->Get_Cell() + Cell(x, y)].Overlay; // RA2: as above
						if ((tile >= IsometricTileTypeClass::TrainBridgeSet && tile < IsometricTileTypeClass::TrainBridgeSet + 16) || (overlay >= OVERLAY_LOWBRIDGE_FIRST && overlay <= OVERLAY_LOWBRIDGE_LAST)) { // RA2: a train bridge tile or a low bridge overlay
							found = true; // RA2: as above
						}
					}
				}
				if (found) { // RA2: as above
					Map.Destroy_Low_Bridge_Near(Target->Get_Cell()); // RA2: 0x00557090
				} else {
					Map.Destroy_Bridge2_Near(Target->Get_Cell()); // RA2: 0x00556520
				}
			}
		}
		Owner = NULL; // RA2: the bomb lets go of everything it referred to
		Target = NULL; // RA2: as above
		OwnerHouse = NULL; // RA2: as above
		Audio.End(); // RA2: AudioController::End (0x00405FE0) stops the ticking
		ShouldPlayTickingSound = false; // RA2: as above
	}
}


/// <summary>
/// Defuses the bomb.
/// The target forgets the bomb, the bomb turns harmless and nothing owns it any more. The
/// bomb object itself stays until the list sweeps it up.
/// </summary>
void BombClass::Disarm(void) // RA2: 0x00435FB0
{
	if (Target != NULL) { // RA2: a disarmed bomb may already have lost its target
		Target->AttachedBomb = NULL; // RA2: the target forgets the bomb and stops showing it
		Target->BombVisible = false; // RA2: as above
	}
	Harmless = true; // RA2: as above
	Owner = NULL; // RA2: the bomb lets go of everything it referred to, as after a detonation
	Target = NULL; // RA2: as above
	OwnerHouse = NULL; // RA2: as above
	Audio.End(); // RA2: as above
	ShouldPlayTickingSound = false; // RA2: as above
}


/// <summary>
/// Tells whether this bomb was put on its target by death rather than by a planting infantry unit.
/// </summary>
/// <returns>True for a death bomb.</returns>
BOOL BombClass::Is_Death_Bomb(void) const // RA2: 0x00435FF0
{
	return(DeathBomb); // RA2: as above
}


/// <summary>
/// Gets the frame of the bomb icon that is drawn on the target.
/// The icon steps through the stages while the fuse burns down and flickers between two
/// frames at the rules' flicker rate. A death bomb shows the last frame.
/// </summary>
/// <returns>The icon frame, 0 to 12.</returns>
int BombClass::Get_Current_Flicker_Frame(void) const // RA2: 0x00436000
{
	if (DeathBomb == 1) return(12); // RA2: a death bomb always shows the last frame
	int rate = Rule->IvanIconFlickerRate; // RA2: frames between two flicker steps
	int stage = (Frame - PlantingFrame) / (Rule->IvanTimedDelay / 6); // RA2: the fuse is divided into six stages
	stage *= 2; // RA2: as above
	if (Frame % (rate * 2) >= rate) stage++; // RA2: as above
	if (stage >= 12) stage = 11; // RA2: as above
	return(stage); // RA2: as above
}


/// <summary>
/// Tells whether the fuse has burnt down.
/// </summary>
/// <returns>True when the bomb is a timed one, still live, and its detonation frame has passed.</returns>
bool BombClass::Time_To_Explode(void) const // RA2: 0x00436070
{
	if (DeathBomb == 0 && Frame > DetonationFrame && !Harmless) return(true); // RA2: death bombs never time out
	return(false); // RA2: as above
}


/// <summary>
/// Adds the state of this bomb to the running game checksum.
/// This routine is used by the multiplayer sync check to prove that every machine holds
/// an identical copy of this object.
/// </summary>
/// <param name="crc">The checksum engine to submit the object state to.</param>
void BombClass::Compute_CRC(CRCEngine & crc) const // RA2: 0x00436090
{
	BASECLASS::Compute_CRC(crc); // RA2: the base class first, then the ids of the owner house, owner and target that exist, then the harmless flag
	if (OwnerHouse != NULL) crc(OwnerHouse->Fetch_ID()); // RA2: as above
	if (Owner != NULL) crc(Owner->Fetch_ID()); // RA2: as above
	if (Target != NULL) crc(Target->Fetch_ID()); // RA2: as above
	crc(Harmless); // RA2: as above
}


/// <summary>
/// Fetches the class identifier used to persist this object.
/// </summary>
/// <param name="retval">Pointer to the buffer that will receive the class identifier.</param>
/// <returns>Returns with S_OK, or E_POINTER if no buffer was supplied.</returns>
HRESULT STDMETHODCALLTYPE BombClass::GetClassID(CLSID * retval) // RA2: 0x00436100
{
	if (retval == NULL) return(E_POINTER); // RA2: as above
	*retval = CLSID_BombClass; // RA2: as above
	return(S_OK); // RA2: as above
}


/// <summary>
/// Loads this bomb from the save game stream.
/// The object is rebuilt in place, the sound handle starts out idle and the owner house,
/// owner and target pointers are handed to the swizzler.
/// </summary>
/// <param name="stream">The stream to load from.</param>
/// <returns>Returns with S_OK on success.</returns>
HRESULT STDMETHODCALLTYPE BombClass::Load(IStream * stream) // RA2: 0x00436140
{
	HRESULT result = BASECLASS::Load(stream); // RA2: the base class reads its part of the stream first
	if (SUCCEEDED(result)) { // RA2: as above
		new (this) BombClass(NoInitClass()); // RA2: restores the vtables over the loaded data
		Audio.Init(); // RA2: as above
		ShouldPlayTickingSound = false; // RA2: as above
		Swizzle_Pointer(&OwnerHouse); // RA2: as above
		Swizzle_Pointer(&Owner); // RA2: as above
		Swizzle_Pointer(&Target); // RA2: as above
	}
	return(result); // RA2: as above
}


/// <summary>
/// Saves this bomb to the save game stream.
/// </summary>
/// <param name="stream">The stream to save to.</param>
/// <param name="cleardirty">Whether the dirty flag is cleared by the save.</param>
/// <returns>Returns with S_OK on success.</returns>
HRESULT STDMETHODCALLTYPE BombClass::Save(IStream * stream, BOOL cleardirty) // RA2: 0x004361D0
{
	return(BASECLASS::Save(stream, cleardirty)); // RA2: the bomb saves nothing beyond its base class; the pointers travel as they are
}


/// <summary>
/// Runs the bombs once a frame.
/// Bombs without a target are deleted, ticking sounds are started or moved along with their
/// targets, and every 45th frame each target is marked as showing or hiding its bomb: the
/// player's own bombs are always shown, other bombs only within sight of a detector that
/// belongs to the player.
/// </summary>
void BombListClass::Update(void) // RA2: 0x004361F0
{
	int i; // RA2: as above
	for (i = Planted.Count() - 1; i >= 0; i--) { // RA2: first pass, from the end: drop empty entries and delete bombs that lost their target
		BombClass * bomb = Planted[i]; // RA2: as above
		if (bomb == NULL) { // RA2: as above
			Planted.Delete_Index(i); // RA2: as above
		} else if (bomb->Target == NULL) { // RA2: as above
			delete bomb; // RA2: as above
			Planted.Delete_Index(i); // RA2: as above
		}
	}

	for (i = Planted.Count() - 1; i >= 0; i--) { // RA2: second pass: the ticking sounds
		BombClass * bomb = Planted[i]; // RA2: as above
		if (bomb->TickSound != -1) { // RA2: as above
			if (bomb->Target->IsInLimbo) { // RA2: as above
				bomb->Audio.Stop(); // RA2: as above
				bomb->ShouldPlayTickingSound = false; // RA2: as above
			} else if (!bomb->ShouldPlayTickingSound) { // RA2: as above
				Coord coord = bomb->Target->Position; // RA2: as above
				VocClass::PlayAt(bomb->TickSound, coord, &bomb->Audio); // RA2: as above
				bomb->ShouldPlayTickingSound = true; // RA2: as above
			} else {
				Coord coord = bomb->Target->Position; // RA2: as above
				VocClass::UpdateAt(coord, &bomb->Audio); // RA2: VocClass::UpdateAt (0x007141B0) moves the playing sound with the target
			}
		}
	}

	if (UpdateDelay > 0) { // RA2: the visibility pass only runs when the delay has run out
		UpdateDelay--; // RA2: as above
		return; // RA2: as above
	}
	UpdateDelay = 45; // RA2: and then every 45th frame

	for (int k = Planted.Count() - 1; k >= 0; k--) { // RA2: third pass: which bombs the player sees
		BombClass * bomb = Planted[k]; // RA2: as above
		bool wasvisible = bomb->Target->BombVisible; // RA2: as above
		bool visible = false; // RA2: as above
		if (bomb->OwnerHouse->Is_Player_Control()) { // RA2: as above
			visible = true; // RA2: as above
		} else {
			for (int d = 0; d < Detectors.Count(); d++) { // RA2: as above
				TechnoClass * detector = Detectors[d]; // RA2: as above
				if (detector->Owner_HouseClass()->Is_Player_Control()) { // RA2: as above
					Coord diff = detector->Center_Coord() - bomb->Target->Center_Coord(); // RA2: as above
					int dist = (int)fastmath::sqrt((double)diff.X * diff.X + (double)diff.Y * diff.Y + (double)diff.Z * diff.Z); // RA2: as above
					if (dist < ((TechnoTypeClass *)detector->Techno_Type_Class())->BombSight << 8) { // RA2: BombSight is in cells, 256 leptons each
						visible = true; // RA2: as above
					}
				}
				if (visible) break; // RA2: as above
			}
		}
		bomb->Target->BombVisible = visible; // RA2: as above
		if (visible != wasvisible) { // RA2: as above
			bomb->Target->IsToDisplay = true; // RA2: as above
		}
	}
}


/// <summary>
/// Does nothing. Nothing in the executable calls it.
/// </summary>
void BombListClass::Reserved(void) // RA2: 0x00436460
{
}


/// <summary>
/// Plants a timed bomb on the target.
/// Only infantry plant bombs, and only on objects that carry no bomb yet. The new bomb
/// belongs to the planter's house and goes off after the Ivan delay. The visibility pass
/// runs on the next frame, and the player hears the attach sound when it is the player's
/// own unit.
/// </summary>
/// <param name="source">The infantry unit that plants the bomb.</param>
/// <param name="target">The object that receives the bomb.</param>
void BombListClass::Plant(TechnoClass * source, ObjectClass * target) // RA2: 0x00436470
{
	if (source != NULL && source->Fetch_RTTI() == RTTI_INFANTRY) { // RA2: only infantry plant bombs
		if (target != NULL && target->AttachedBomb == NULL) { // RA2: a target carries at most one bomb
			BombClass * bomb = new BombClass; // RA2: as above
			bomb->OwnerHouse = source->Owner_HouseClass(); // RA2: as above
			bomb->Owner = source; // RA2: as above
			bomb->Target = target; // RA2: as above
			bomb->PlantingFrame = Frame; // RA2: as above
			bomb->DetonationFrame = Rule->IvanTimedDelay + bomb->PlantingFrame; // RA2: as above
			bomb->DeathBomb = false; // RA2: as above
			target->AttachedBomb = bomb; // RA2: as above
			Planted.Add(bomb); // RA2: as above
			UpdateDelay = 1; // RA2: the visibility pass runs again on the next frame
			if (Rule->BombAttachSound != -1 && source->House->Is_Player_Control()) { // RA2: the attach sound plays for the player's own planters
				Coord coord = target->Position; // RA2: as above
				VocClass::PlayAt(Rule->BombAttachSound, coord, NULL); // RA2: as above
			}
		}
	}
}


/// <summary>
/// Puts a bomb into the list of planted bombs.
/// </summary>
/// <param name="bomb">The bomb to add.</param>
void BombListClass::Add_Bomb(BombClass * bomb) // RA2: 0x00436630
{
	Planted.Add(bomb); // RA2: as above
}


/// <summary>
/// Adds a unit that can see bombs to the list of detectors.
/// </summary>
/// <param name="detector">The unit to add.</param>
void BombListClass::Add_Detector(TechnoClass * detector) // RA2: 0x00436680
{
	Detectors.Add(detector); // RA2: as above
}


/// <summary>
/// Takes a unit out of the list of detectors.
/// </summary>
/// <param name="detector">The unit to remove.</param>
void BombListClass::Remove_Detector(TechnoClass * detector) // RA2: 0x004366D0
{
	Detectors.Delete(detector); // RA2: as above
}


/// <summary>
/// Empties both lists and restarts the visibility timer.
/// </summary>
void BombListClass::Clear(void) // RA2: 0x00436710
{
	UpdateDelay = 45; // RA2: the delay restarts at its full length
	Planted.Delete_All(); // RA2: each list is emptied but keeps its size
	Detectors.Delete_All(); // RA2: as above
}


/// <summary>
/// Called when an object is about to disappear.
/// A bomb that is the object leaves the list, and a bomb that the object planted forgets its owner.
/// </summary>
/// <param name="target">The object that is going away.</param>
void BombListClass::Detach(AbstractClass * target) // RA2: 0x00436750
{
	Planted.Delete((BombClass *)target); // RA2: the object may itself be a bomb
	for (int index = 0; index < Planted.Count(); index++) { // RA2: as above
		BombClass * bomb = Planted[index]; // RA2: as above
		if (bomb != NULL && bomb->Owner == target) { // RA2: as above
			bomb->Owner = NULL; // RA2: as above
		}
	}
}


/// <summary>
/// Saves the list to the save game stream.
/// Both lists are written as a count followed by the pointers; the objects themselves are
/// saved elsewhere.
/// </summary>
/// <param name="stream">The stream to save to.</param>
/// <returns>Returns with S_OK on success.</returns>
HRESULT BombListClass::Save(IStream * stream) // RA2: 0x004367C0
{
	int count = Planted.Count(); // RA2: the planted bombs are written as a count and the pointers
	HRESULT result = stream->Write(&count, sizeof(count), NULL); // RA2: as above
	if (SUCCEEDED(result)) { // RA2: as above
		for (int index = 0; index < count; index++) { // RA2: as above
			result = stream->Write(&Planted[index], sizeof(BombClass *), NULL); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
		}
		count = Detectors.Count(); // RA2: then the detectors the same way
		result = stream->Write(&count, sizeof(count), NULL); // RA2: as above
		if (SUCCEEDED(result)) { // RA2: as above
			for (int index = 0; index < count; index++) { // RA2: as above
				result = stream->Write(&Detectors[index], sizeof(TechnoClass *), NULL); // RA2: as above
				if (FAILED(result)) { // RA2: as above
					return(result); // RA2: as above
				}
			}
		}
	}
	return(result); // RA2: as above
}


/// <summary>
/// Loads the list from the save game stream.
/// The pointers are read as saved and handed to the swizzler to be fixed up once every
/// object has been loaded.
/// </summary>
/// <param name="stream">The stream to load from.</param>
/// <returns>Returns with S_OK on success.</returns>
HRESULT BombListClass::Load(IStream * stream) // RA2: 0x00436860
{
	int count; // RA2: the first count is the planted bombs, the second the detectors
	int index; // RA2: as above
	BombClass * bomb; // RA2: as above
	TechnoClass * detector; // RA2: as above
	HRESULT result = stream->Read(&count, sizeof(count), NULL); // RA2: as above
	if (FAILED(result)) { // RA2: as above
		return(result); // RA2: as above
	}
	for (index = 0; index < count; index++) { // RA2: as above
		result = stream->Read(&bomb, sizeof(bomb), NULL); // RA2: as above
		if (FAILED(result)) { // RA2: as above
			return(result); // RA2: as above
		}
		Planted.Add(bomb); // RA2: as above
	}
	for (index = 0; index < count; index++) { // RA2: as above
		Swizzle_Pointer(&Planted[index]); // RA2: as above
	}

	result = stream->Read(&count, sizeof(count), NULL); // RA2: as above
	if (SUCCEEDED(result)) { // RA2: as above
		for (int item = 0; item < count; item++) { // RA2: as above
			result = stream->Read(&detector, sizeof(detector), NULL); // RA2: as above
			if (FAILED(result)) { // RA2: as above
				return(result); // RA2: as above
			}
			Detectors.Add(detector); // RA2: as above
		}
		for (index = 0; index < count; index++) { // RA2: as above
			Swizzle_Pointer(&Detectors[index]); // RA2: as above
		}
	}
	return(result); // RA2: as above
}


/// <summary>
/// Fetches the size of this object in bytes.
/// </summary>
/// <returns>Returns with the byte size of this object.</returns>
int BombClass::Fetch_Object_Size(void) const // RA2: 0x004369D0
{
	return(sizeof(*this)); // RA2: as above
}


/// <summary>
/// Fetches the run time type identifier of this object.
/// </summary>
/// <returns>Returns with RTTI_BOMB.</returns>
RTTIType BombClass::Fetch_RTTI(void) const // RA2: 0x004369E0
{
	return(RTTI_BOMB); // RA2: as above
}
