
// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFT (lane Y12), appended to the voxel animation unit: VoxelAnimClass::VoxelAnimClass
// (VoxelAnimTypeClass const *, Coord const &, HouseClass *), gamemd.exe 0x007493B0 (1,310 bytes, unmatched
// in the YR tree: voxel debris from bullets, the area damage and the meteor action create them; a
// link-closure trap). TibSun's text (TibSun archive code/vanim.cpp; SPDX GPL-3.0-or-later, Copyright 2025
// Electronic Arts Inc. and OpenTS contributors, EA's GPL section 7 terms; THIRD_PARTY.md), with
// gamemd.exe's changes: the two audio controllers are set up first; the start sound plays through the
// first controller (VocClass::PlayAt), else that controller stops looping, and the second is ended; the
// attached particle system holds this animation as its owner with no target offset. Behaviour code for the
// native build; never part of an exact build.
// ---------------------------------------------------------------------------------------------
VoxelAnimClass::VoxelAnimClass(VoxelAnimTypeClass const * type, Coord const & coord, HouseClass * house) :
	BASECLASS(),
	Class((VoxelAnimTypeClass *)type),
	Unused1(-1),
	AttachedParticleSys(NULL),
	House(house),
	IsToDie(false),
	IsInvisible(false),
	ECCounter(0)
{
	Create_ID();
	Audio3.FUN_00405BE0();
	Audio4.FUN_00405BE0();
	ECCounter = Class->Duration;
	VoxelAnims.Add(this);

	if (!Class->IsMeteor) {
		Unlimbo(coord);
		Coord center = Center_Coord() + Coord(0, 0, 10);
		int r1 = Scen->RandomNumber;
		int r2 = Scen->RandomNumber;
		int r3 = Scen->RandomNumber;
		int r4 = Scen->RandomNumber;
		float y = (abs(r3) % int(Class->MaxXYVel + Class->MaxXYVel)) - Class->MaxXYVel;
		float z = (abs(r2) % int(Class->MaxZVel - Class->MinZVel + 1.0)) + Class->MinZVel;
		float x = (abs(r4) % int(Class->MaxXYVel + Class->MaxXYVel)) - Class->MaxXYVel;
		Vector3 velocity(x, y, z);
		double rotation = (abs(r1) % int(Class->MaxAngularVelocity - Class->MinAngularVelocity + 1.0)) + Class->MinAngularVelocity;
		BounceClass::Init(center, Class->Elasticity, 1.4f, 0.0, velocity, rotation);
	} else {
		Vector3 velocity((abs(Scen->RandomNumber) % int(Class->MaxXYVel + Class->MaxXYVel)) - Class->MaxXYVel,
			(abs(Scen->RandomNumber) % int(Class->MaxXYVel + Class->MaxXYVel)) - Class->MaxXYVel, Class->MinZVel);
		if (velocity.X < -velocity.Y) {
			velocity.X = -velocity.X;
			velocity.Y = -velocity.Y;
		}
		ECCounter -= abs(Scen->RandomNumber % 20);
		int x = coord.X - ECCounter * velocity.X;
		int y = coord.Y - ECCounter * velocity.Y;
		int z = coord.Z - ECCounter * velocity.Z;
		Coord ucoord(x, y, z);
		Unlimbo(ucoord);
		int rnd = Scen->RandomNumber;
		BounceClass::Init(Center_Coord(), Class->Elasticity, 1.4f, 0.0, velocity,
			(abs(rnd) % int(Class->MaxAngularVelocity - Class->MinAngularVelocity + 1.0)) + Class->MinAngularVelocity);
	}

	if (Class->StartSound != VOC_NONE) {
		VocClass::PlayAt(Class->StartSound, coord, &Audio3);
	} else {
		Audio3.EndLooping();
	}
	Audio4.End();

	if (Class->AttachedSystem) {
		AttachedParticleSys = new ParticleSystemClass(Class->AttachedSystem, coord, &Map[coord], this, Coord(0, 0, 0), NULL);
	}
}
