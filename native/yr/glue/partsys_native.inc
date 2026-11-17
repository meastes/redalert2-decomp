// ---------------------------------------------------------------------------------------------
// NON-EXACT NATIVE DRAFTS (lane Y6): ParticleSystemClass::Smoke_AI (gamemd.exe 0x0062ED40, 1,255 B)
// and Spark_AI (0x0062E840, 1,277 B), unmatched in ra2-decomp. The text is the RA2 twins'
// (src/recovered/partsys.cpp: Spark_AI exact there, Smoke_AI a non-exact draft) in the YR tree's
// names, checked against gamemd.exe's code: YR makes the new particles with new ParticleClass,
// stops a smoke system by marking it for deletion, and a spark system only retires its dead
// particles (no motion step). Not compared byte for byte; behaviour code for the native build only.
// ---------------------------------------------------------------------------------------------

static inline double PartSys_Random_Unit(void)
{
	return(Scen->RandomNumber(0, 0x7FFFFFFE) / 2147483646.0);
}

static inline int PartSys_Spread(int spread)
{
	int value = Scen->RandomNumber() % spread;
	return(value <= 0 ? value - spread : value + spread);
}

void ParticleSystemClass::Smoke_AI(void)
{
	int i;

	ObjectClass * owner = (ObjectClass *)Source;
	if (owner != NULL && (owner->AbstractFlags & 2) && owner->Fetch_RTTI() != RTTI_BUILDING) {
		Set_Coord(owner->Center_Coord() + CoordOffset);
	}

	for (i = 0; i < SystemParticles.Count(); i++) {
		SystemParticles[i]->Behavior_AI();
	}

	for (i = SystemParticles.Count() - 1; i >= 0; i--) {
		ParticleClass * spart = SystemParticles[i];
		if (spart->IsToDie) {
			if (spart->Class->NextParticle != PARTICLE_NONE) {
				Coord coord = spart->Position;
				int spread = spart->Class->Radius >> 3;
				int xrand = spread > 0 ? PartSys_Spread(spread) : 0;	// NATIVE: a radius under 8 divides by zero in the original
				int yrand = spread > 0 ? PartSys_Spread(spread) : 0;
				ParticleTypeClass * next = ParticleTypes[spart->Class->NextParticle];

				ParticleClass * npart = new ParticleClass(next, Coord(coord.X + xrand, coord.Y + yrand, coord.Z), COORD_NONE, this);
				if (npart != NULL) {
					SystemParticles.Add(npart);
					npart->Speed = spart->Speed;
					npart->Translucency = spart->Translucency + ((Scen->RandomNumber() % 6) != 0 ? 25 : 0);
				}
				npart = new ParticleClass(next, Coord(coord.X - xrand, coord.Y - yrand, coord.Z), COORD_NONE, this);
				if (npart != NULL) {
					SystemParticles.Add(npart);
					npart->Speed = spart->Speed;
					npart->Translucency = spart->Translucency + ((Scen->RandomNumber() % 6) != 0 ? 25 : 0);
				}
			}
			spart->Delete_Me();
		} else {
			spart->Motion_AI();
		}
	}

	if (!IsMarkedForDeletion && IsActive) {
		int every = (int)SpawnFrames;
		if (every != 0 && Frame % every == 0) {	// NATIVE: guards the original's division
			FootClass * foot = (owner != NULL && (owner->AbstractFlags & 4)) ? (FootClass *)owner : NULL;
			if (foot == NULL || foot->CurrentTube < 0) {
				int xrand = Scen->RandomNumber();
				int range = Class->SpawnRadius + 1;
				int yrand = Scen->RandomNumber() % range;
				Coord scoord(Position.X + xrand % range, Position.Y + yrand, Position.Z + 10);
				if (Class->HoldsWhat != PARTICLE_NONE) {
					ParticleClass * npart = new ParticleClass(ParticleTypes[Class->HoldsWhat], scoord, SpawnCoord, this);
					if (npart != NULL) {
						SystemParticles.Add(npart);
						if (SpawnFrames > Class->SpawnTranslucencyCutoff) {
							npart->Translucency += 25;
						}
						npart->Speed = npart->Speed - (SpawnFrames - Class->SpawnFrames) * 0.35f;
						if (npart->Speed < 2.0) {
							npart->Speed = 2.0f;
						}
					}
				}
			}
		}
	}

	SpawnFrames = SpawnFrames + Class->Slowdown;
	if (SpawnFrames > Class->SpawnCutoff) {
		IsMarkedForDeletion = true;
	}
}

void ParticleSystemClass::Spark_AI(void)
{
	int i;

	if (SparkSpawnFrames > 0) {
		if (SparkSpawnFrames == 1 || PartSys_Random_Unit() <= Class->SpawnSparkPercentage) {
			int half = Class->ParticleCap / 2;
			int count = half + (half > 0 ? abs(Scen->RandomNumber()) % half : 0);	// NATIVE: guards the division
			ParticleTypeClass * held = Class->HoldsWhat != PARTICLE_NONE ? ParticleTypes[Class->HoldsWhat] : NULL;
			int rand1 = Scen->RandomNumber();
			int rand2 = Scen->RandomNumber();
			int rand3 = Scen->RandomNumber();
			Vector3 spark(0, 0, 0);
			if (held != NULL) {
				spark.X = (float)(rand2 % held->XVelocity);
				spark.Y = (float)(rand1 % held->YVelocity);
				spark.Z = (float)(rand3 % held->ZVelocityRange);
			}
			for (; count > 0 && held != NULL; count--) {	// NATIVE: the original reads a null particle without a held type
				ParticleClass * particle = new ParticleClass(held, Position, Position, this);
				if (particle == NULL) break;
				SystemParticles.Add(particle);
				ParticleTypeClass const * type = particle->Class;
				Vector3 & direction = particle->MovementDirection;
				direction.X = (float)(Scen->RandomNumber() % type->XVelocity);
				direction.Y = (float)(Scen->RandomNumber() % type->YVelocity);
				direction.Z = (float)(abs(Scen->RandomNumber()) % type->ZVelocityRange + type->MinZVelocity);
				float speed = (float)fastmath::sqrt(direction.X * direction.X + direction.Y * direction.Y + direction.Z * direction.Z);
				if (IsRandomSparkDirection) {
					direction = Vector3(spark.X + direction.X, spark.Y + direction.Y, spark.Z + direction.Z);
				} else {
					direction = Vector3(Class->SpawnDirection.X + direction.X, Class->SpawnDirection.Y + direction.Y, Class->SpawnDirection.Z + direction.Z);
				}
				double length = fastmath::sqrt(direction.X * direction.X + direction.Y * direction.Y + direction.Z * direction.Z);
				if (length != 0.0) {
					direction = Vector3((float)(direction.X / length), (float)(direction.Y / length), (float)(direction.Z / length));
				}
				direction = Vector3(speed * direction.X, speed * direction.Y, speed * direction.Z);
			}
			if (Options.DetailLevel == 2 && SparkSpawnFrames == Class->SparkSpawnFrames && Class->LightSize > 0 && !Class->OneFrameLight) {
				new SpotLightClass(Position, Class->LightSize);
			}
		}
		SparkSpawnFrames--;
		if (SparkSpawnFrames <= 0) {
			IsMarkedForDeletion = true;
		}
		double chance = PartSys_Random_Unit();
		if (chance < 0.3) {
			int radius = SparkRadius - 3;
			SparkRadius = radius <= 17 ? 17 : radius;
		} else if (chance < 0.6) {
			int radius = SparkRadius + 3;
			SparkRadius = radius >= 41 ? 41 : radius;
		}
	}

	for (i = 0; i < SystemParticles.Count(); i++) {
		SystemParticles[i]->Behavior_AI();
	}
	for (i = SystemParticles.Count() - 1; i >= 0; i--) {
		if (SystemParticles[i]->IsToDie) {
			SystemParticles[i]->Delete_Me();
		}
	}
}
