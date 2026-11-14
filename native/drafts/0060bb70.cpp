// NON-EXACT DRAFT 0x60BB70 ParticleSystemClass::unknown_func_60bb70 (smoke system AI)
// entry 0x60BB70; exact-match score 54.23 (shape 93.54; Scout1127 keep/smoke_5423_m6.cpp, no Spread helper;
// from Scout1123 v15 49.78 and Scout588/Scout1000's 37.95 shape). Recovered source text by our lanes/workers;
// never exact-match credit. Not original bytes. Reached at frame ~3,300 of the M12 harness runs
// (ParticleSystemClass::AI, a smoking building or wreck). Text unchanged from the lane's (Scout1125, M15);
// Object_Flag_Set and Foot_Of are file-local statics (ra2-decomp Is_Foot shape, TibSun As_FootClass).
// void ParticleSystemClass::unknown_func_60bb70() (0x0060BB70)
// Behaviour 0 (smoke): the system follows its owner; particles run, and
// each finished particle splits into two of its type's next particle,
// spread sideways at random; every SpawnFrames frames a new particle rises
// near the system, slower the longer the system has been running.
#include "shared/particle_system.hpp"
#include "shared/foot.hpp"
#include "shared/cd_timer.hpp"
#include "shared/unknown_geometry.hpp"
// @ref ??0ParticleClass@@QAE@PAVParticleTypeClass@@ABUCoordStruct@@1PAVParticleSystemClass@@@Z=00608440
// @ref ?ParticleTypes@@3V?$DynamicVectorClass@PAVParticleTypeClass@@@@A=00a35e00
// @ref ??2@YAPAXI@Z=00782017
// @ref ?Behavior_AI@ParticleClass@@QAEXXZ=00609c90
// @ref ?Motion_AI@ParticleClass@@QAEXXZ=0060a420
// @ref ??RRandom2Class@@QAEHXZ=00638840
// @ref ?unknown_id_source_a3d290@@3PAVScenarioClass@@A=00a3d290

// @ref __real@4@3ffdb333330000000000=007a7f40
static inline bool Object_Flag_Set(ObjectClass const *object)
{
    if (object == 0) {
        return false;
    }
    unsigned char flag = object->AbstractFlags.IsObject;
    return flag != 0;
}
static inline FootClass *Foot_Of(ObjectClass *object)
{
    if (object != 0 && object->AbstractFlags.IsFoot) {
        return (FootClass *)object;
    }
    return 0;
}
void ParticleSystemClass::unknown_func_60bb70()
{
    if (Object_Flag_Set(Owner) && Owner->What_Am_I() != 6) {
        Set_Coord(Owner->Center_Coord() + unknown_9c);
    }

    int index;
    for (index = 0; index < Particles.Count(); index++) {
        Particles[index]->Behavior_AI();
    }

    for (index = Particles.Count() - 1; index >= 0; index--) {
        ParticleClass *particle = Particles[index];
        if (particle->unknown_121) {
            if (particle->Class->NextParticle != -1) {
                CoordStruct const &coord = particle->Coord;
                int x = coord.X;

                int xspread = particle->Class->Radius >> 3;
                int xrand = unknown_id_source_a3d290->Random()() % xspread;
                if (xrand <= 0) {
                    xrand = xrand - xspread;
                } else {
                    xrand = xspread + xrand;
                }

                ParticleTypeClass *type = particle->Class;
                int yspread = type->Radius >> 3;
                int yrand = unknown_id_source_a3d290->Random()() % yspread;
                if (yrand <= 0) {
                    yrand = yrand - yspread;
                } else {
                    yrand = yspread + yrand;
                }

                ParticleTypeClass *next = ParticleTypes[type->NextParticle];

                CoordStruct ncoord;
                ncoord.Y = coord.Y + yrand;
                ncoord.X = x + xrand;
                ncoord.Z = coord.Z;
                ParticleClass *npart = Spawn_Held_Particle(next, ncoord);
                if (npart != 0) {
                    npart->Velocity = particle->Velocity;
                    npart->unknown_11f = particle->unknown_11f + (unknown_id_source_a3d290->Random() % 6 != 0 ? 25 : 0);
                }

                CoordStruct pcoord;
                pcoord.Y = coord.Y - yrand;
                pcoord.X = x - xrand;
                pcoord.Z = coord.Z;
                ParticleClass *ppart = Spawn_Held_Particle(next, pcoord);
                if (ppart != 0) {
                    ppart->Velocity = particle->Velocity;
                    ppart->unknown_11f = particle->unknown_11f + (unknown_id_source_a3d290->Random() % 6 != 0 ? 25 : 0);
                }
            }
            particle->Delete_Me();
        } else {
            Particles[index]->Motion_AI();
        }
    }

    if (!unknown_e4 && unknown_7d && Frame % (int)unknown_d4 == 0) {
        FootClass *foot = Foot_Of(Owner);
        if (foot == 0 || foot->unknown_574 < 0) {
            Random2Class &random = unknown_id_source_a3d290->Random();
            int xrand = random;
            int yrand = random;
            int range = Class->SpawnRadius + 1;
            CoordStruct scoord = Position_Coord_004f3f40() + CoordStruct(xrand % range, yrand % range, 10);
            if (Class->HoldsWhat != -1) {
                ParticleClass *npart = new ParticleClass(ParticleTypes[Class->HoldsWhat], scoord, TargetCoord, this);
                if (npart != 0) {
                    Particles.Add(npart);
                    if (unknown_d4 > Class->SpawnTranslucencyCutoff) {
                        npart->unknown_11f += 25;
                    }
                    npart->Velocity = npart->Velocity - (unknown_d4 - Class->SpawnFrames) * 0.35f;
                    if (npart->Velocity < 2.0) {
                        npart->Velocity = 2.0;
                    }
                }
            }
        }
    }

    // NATIVE (Scout1274; codex4 r557): the original adds in the x87 register
    // (0x0060C01A fld/fadd), stores the sum rounded to float (0x0060C026 fst) and
    // compares the unrounded register value with SpawnCutoff (0x0060C02C fcomp).
    // Natively the sum is kept in double (53 bits, as the original's precision) and
    // the double is compared; the float store rounds as fst does (the game thread's
    // rounding mode, crt.cpp _controlfp).
    double sum = (double)Class->Slowdown + (double)unknown_d4;
    unknown_d4 = (float)sum;
    if (sum > (double)Class->SpawnCutoff) {
        unknown_e4 = true;
    }
}
