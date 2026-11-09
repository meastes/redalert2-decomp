// NON-EXACT DRAFT 0x60BB70 ParticleSystemClass::unknown_func_60bb70 (smoke system AI)
// entry 0x60BB70; exact-match score 37.95 (Scout588/Scout1000; misses_index best 53.12 by Scout1005, text not kept);
// plan "draft, verify". Origin: build/class-work/Scout1000/w/s0.cpp (Scout588's shape).
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by Scout1111
// (native/drafts.json). Not original bytes. Reached at frame ~3,300 of the M12 harness runs
// (ParticleSystemClass::AI, a smoking building or wreck). Text unchanged.
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
// @ref ?unknown_func_609c90@ParticleClass@@QAEXXZ=00609c90
// @ref ?unknown_func_60a420@ParticleClass@@QAEXXZ=0060a420
// @ref ??RRandom2Class@@QAEHXZ=00638840
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290

// @ref __real@4@3ffdb333330000000000=007a7f40
void ParticleSystemClass::unknown_func_60bb70()
{
    bool is_object;
    if (Owner != 0) {
        is_object = Owner->AbstractFlags.IsObject;
    }
    if (Owner != 0 && is_object && Owner->What_Am_I() != 6) {
        unknown_slot103(Owner->Center_Coord() + unknown_9c);
    }

    int index;
    for (index = 0; index < Particles.Count(); index++) {
        Particles[index]->unknown_func_609c90();
    }

    for (index = Particles.Count() - 1; index >= 0; index--) {
        ParticleClass *particle = Particles[index];
        if (particle->unknown_121) {
            if (particle->Class->NextParticle != -1) {
                CoordStruct coord = particle->Coord;
                int x = coord.X;
                int y = coord.Y;
                int z = coord.Z;

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
                ncoord.Y = y + yrand;
                ncoord.X = x + xrand;
                ncoord.Z = z;
                ParticleClass *npart = Spawn_Held_Particle(next, ncoord);
                if (npart != 0) {
                    npart->Velocity = particle->Velocity;
                    npart->unknown_11f = particle->unknown_11f + (unknown_id_source_a3d290->Random()() % 6 != 0 ? 25 : 0);
                }

                CoordStruct pcoord;
                pcoord.Y = y - yrand;
                pcoord.X = x - xrand;
                pcoord.Z = z;
                ParticleClass *ppart = Spawn_Held_Particle(next, pcoord);
                if (ppart != 0) {
                    ppart->Velocity = particle->Velocity;
                    ppart->unknown_11f = particle->unknown_11f + (unknown_id_source_a3d290->Random()() % 6 != 0 ? 25 : 0);
                }
            }
            particle->unknown_slot60();
        } else {
            particle->unknown_func_60a420();
        }
    }

    if (!unknown_e4 && unknown_7d && Frame % (int)unknown_d4 == 0) {
        if (Owner == 0 || !Owner->AbstractFlags.IsFoot || ((FootClass *)Owner)->unknown_574 < 0) {
            int xrand = unknown_id_source_a3d290->Random()();
            int range = Class->SpawnRadius + 1;
            int yrand = unknown_id_source_a3d290->Random()();
            CoordStruct scoord = Coord + CoordStruct(xrand % range, yrand % range, 10);
            ParticleClass *npart = Spawn_Particle(scoord, TargetCoord);
            if (npart != 0) {
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

    unknown_d4 = Class->Slowdown + unknown_d4;
    if (unknown_d4 > Class->SpawnCutoff) {
        unknown_e4 = true;
    }
}
