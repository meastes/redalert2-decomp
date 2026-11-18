/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 OpenTS contributors
 *
 * See LICENSE.md for applicable additional terms and warranty disclaimers.
 ******************************************************************************/

#include "always.h"

#include "rect.h"
#include "geoconst1.h" // RA2: this unit holds its own copy of the geometry constants (its first initializers)

#include "globals.h" // RA2: the includes this file needs
#include "wdtnet.h"
#include "geoconst2.h" // RA2: the slope and bridge constants (after globals.h)

using namespace WorldDominationTour;


/// <summary>
/// Creates a counted ownership snapshot of the tour at one tick of its cycle.
/// </summary>
/// <param name="tour">The tour state to take the snapshot from.</param>
/// <param name="tick">The point in the cycle's ownership history to look at.</param>
/// <returns>Returns with the new snapshot.</returns>
rc_ptr<State> __fastcall State::Create(rc_ptr<WDTState> const & tour, int tick) // RA2: a counted snapshot of the tour state at the tick, made by a static Create (0x00730700)
{
	return(rc_ptr<State>(new State(tour, tick))); // RA2: as above
}


/// <summary>
/// Creates a counted empty ownership snapshot.
/// </summary>
/// <returns>Returns with the new snapshot.</returns>
rc_ptr<State> __fastcall State::Create(void) // RA2: a counted empty snapshot, made by a static Create (0x00730790)
{
	return(rc_ptr<State>(new State())); // RA2: as above
}


/// <summary>
/// Creates an empty ownership snapshot.
/// The snapshot refers to no tour state at all, so every territory reports as unassigned
/// until it is assigned from a real one.
/// </summary>
State::State(void) : // RA2: the tour state is a counted pointer, empty by its own constructor (0x007307F0)
	Owners(NULL)
{
	//nothing
}


/// <summary>
/// Creates a snapshot of the tour at one tick of its cycle.
/// This routine is used by the tour history to hand out a view of who held what at some
/// earlier point in the current cycle. A tick the tour has no record for still yields a
/// usable snapshot, but every territory will report as unassigned.
/// </summary>
/// <param name="tour">The tour state to take the snapshot from.</param>
/// <param name="tick">The point in the cycle's ownership history to look at.</param>
State::State(rc_ptr<WDTState> const & tour, unsigned int tick) : // RA2: the tour state is a counted pointer (0x00730830)
	TourState(tour),
	Owners(NULL)
{
	if (TourState.Get() != NULL && tick < TourState.Get()->NumTicks) { // RA2: Get() because operator-> would not fold the second read (nor the copy of this body in Create 0x00730700)
		Owners = TourState.Get()->OwnerHistory[tick]; // RA2: as above
	}
}


/// <summary>
/// Fetches who holds a territory in this snapshot.
/// This routine is used by the tour map display to color each territory according to the
/// side that owned it at the moment the snapshot was taken.
/// </summary>
/// <param name="terr">Index of the territory to examine.</param>
/// <returns>Returns with the owning side of that territory. A territory nobody has claimed
/// yet, or an index this snapshot cannot answer for, comes back as 0.</returns>
int State::Owner_Of(unsigned int terr) const // RA2: what the snapshot records for a territory: 1 (disputed), 2 and 3 (the two sides) for the bytes 0 to 2, 0 for anything else (0x00730890)
{
	if (TourState.Get() != NULL && Owners != NULL && terr < TourState->NumTerritories) { // RA2: as above
		switch (Owners[terr]) { // RA2: as above
			case WDTState::CONTESTED: // RA2: as above
				return(1); // RA2: as above
			case WDTState::GDI: // RA2: as above
				return(2); // RA2: as above
			case WDTState::NOD: // RA2: as above
				return(3); // RA2: as above
		}
	}
	return(0); // RA2: as above
}


/// <summary>
/// Destroys the ownership snapshot.
/// </summary>
State::~State(void) // RA2: counted (0x007308E0)
{
	//nothing
}


/// <summary>
/// Counts the territories a side holds in this snapshot.
/// </summary>
/// <param name="owner">The side to count for, 2 or 3.</param>
/// <returns>Returns with the number of territories the side holds.</returns>
int State::Count_Owned(int owner) const // RA2: how many territories the snapshot gives the owner, 2 for the byte 1 and 3 for the byte 2, nothing for other owners (0x00730900)
{
	if (TourState.Get() != NULL && Owners != NULL) { // RA2: as above
		int count = 0; // RA2: as above
		unsigned char const * owners = Owners; // RA2: as above
		unsigned int i = TourState->NumTerritories; // RA2: as above
		while (i--) { // RA2: as above
			switch (*owners++) { // RA2: as above
				case WDTState::GDI: // RA2: as above
					if (owner == 2) { // RA2: as above
						count++; // RA2: as above
					}
					break;
				case WDTState::NOD: // RA2: as above
					if (owner == 3) { // RA2: as above
						count++; // RA2: as above
					}
					break;
			}
		}
		return(count); // RA2: as above
	}
	return(0); // RA2: as above
}
