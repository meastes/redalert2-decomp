/*******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2026 RedAlert2 contributors
 ******************************************************************************/

#pragma once

#include "vector.h"

class AbstractClass;
class BombClass;
class ObjectClass;
class TechnoClass;
struct IStream;

/*
**	RA2 keeps the bombs that Crazy Ivan plants in one list.
*/
class BombListClass
{
	public:
		void Update(void); // RA2: 0x004361F0; once a frame: removes finished bombs, keeps the ticking sounds going and (every 45th frame) decides which bombs the player sees
		void Reserved(void); // RA2: 0x00436460; does nothing, nothing calls it
		void Plant(TechnoClass * source, ObjectClass * target); // RA2: 0x00436470; an infantry unit plants a timed bomb on the target
		void Add_Bomb(BombClass * bomb); // RA2: 0x00436630
		void Add_Detector(TechnoClass * detector); // RA2: 0x00436680; a unit that can see bombs (BombSight)
		void Remove_Detector(TechnoClass * detector); // RA2: 0x004366D0
		void Clear(void); // RA2: 0x00436710
		void Detach(AbstractClass * target);
		HRESULT Save(IStream * stream); // RA2: 0x004367C0
		HRESULT Load(IStream * stream); // RA2: 0x00436860

		DynamicVectorClass<BombClass *> Planted; // RA2: +0x00; the bombs that are planted
		DynamicVectorClass<TechnoClass *> Detectors; // RA2: +0x18; the units that can see bombs
		int UpdateDelay; // RA2: +0x30; frames until the visibility pass runs again (45 when cleared, 1 after a plant)
};

extern BombListClass BombList;
