// partsys.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/partsys.json carries the addresses and the rows that
// stay in their own files.


#include "shared/particle_system.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/techno.hpp"
#include "shared/spot_light.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/id_source.hpp"
#include "shared/options.hpp"
#include "shared/fast_math.hpp"
#include "shared/matrix3d.hpp"
#include "shared/laser_draw.hpp"
#include "shared/unknown_vector3.hpp"
#include "shared/rect.hpp"


// ParticleSystemClass::ParticleSystemClass(ParticleSystemTypeClass *type, CoordStruct const &coord, AbstractClass *target, ObjectClass *owner, CoordStruct const &target_coord) (0x0060AA70)
// A new system of a type at a coordinate, aimed at a target object (or a
// plain coordinate), optionally owned by an object; registered in the system
// list and the 0x00AC0D60 object list.

ParticleSystemClass::ParticleSystemClass(ParticleSystemTypeClass *type, CoordStruct const &coord, AbstractClass *target,
                                         ObjectClass *owner, CoordStruct const &target_coord) :
    Class(type),
    unknown_9c(0, 0, 0),
    TargetCoord(unknown_geometry_l),
    Owner(0),
    unknown_d0(0),
    unknown_d4((float)Class->SpawnFrames),
    Lifetime(Class->Lifetime),
    unknown_dc(Class->SparkSpawnFrames),
    unknown_e0(0x1d),
    unknown_e4(false),
    unknown_e5(false)
{
    AbstractClass::Create_ID();
    ParticleSystems.Add(this);
    Particles.Clear();
    if (target != 0) {
        TargetCoord = target->Center_Coord();
        if (Map[target->Center_Coord()].unknown_140 & 0x100) {
            if (target->Kind_Of() == 0xb) {
                TargetCoord.Z += unknown_geometry_j;
            }
        }
    } else {
        TargetCoord = target_coord;
    }
    CoordStruct place(coord.X, coord.Y, coord.Z);
    if (owner != 0) {
        Owner = owner;
        TechnoClass *techno = owner->As_Techno();
        if (techno != 0) {
            unknown_d0 = techno->unknown_228;
        }
    } else {
        Owner = 0;
        unknown_d0 = target;
    }
    ObjectClass::Unlimbo(&place, 0);
    if (Owner != 0) {
        CoordStruct center = Coord;
        unknown_9c = center - Owner->Center_Coord();
    }
    unknown_AbstractClass_list_00ac0d60.Add(this);
}


// ParticleSystemClass::ParticleSystemClass() (0x0060AD30)
// An empty system registered in the system list and the 0x00AC0D60 object
// list.

ParticleSystemClass::ParticleSystemClass() :
    Class(0),
    TargetCoord(unknown_geometry_l),
    Owner(0),
    unknown_d0(0),
    unknown_d4(0),
    Lifetime(0),
    unknown_dc(0),
    unknown_e0(0),
    unknown_e4(false),
    unknown_e5(false)
{
    Particles.Clear();
    ParticleSystems.Add(this);
    unknown_AbstractClass_list_00ac0d60.Add(this);
}


// ParticleSystemClass::~ParticleSystemClass() (0x0060AE80)
// Deletes its particles (also removing them from the 0x00AC0CD8 list) and
// leaves the system and object lists.

ParticleSystemClass::~ParticleSystemClass()
{
    Unknown_006ECB90(this, true);
    ObjectClass::Limbo();
    while (Particles.Count() != 0) {
        delete Particles[0];
        unknown_objects_ac0cd8.Delete(Particles[0]);
        Particles.Delete(Particles[0]);
    }
    Class = 0;
    ParticleSystems.Delete(this);
    unknown_objects_ac0cb0.Delete(this);
    unknown_AbstractClass_list_00ac0d60.Delete(this);
}


// void ParticleSystemClass::unknown_slot67(Point2D const &point, Rect const &window) (0x0060B090, slot 67)
// A one-frame light type flashes a spot light sized by how full the system
// is (the particle count over ParticleCap, between 0.4 and 1).

void ParticleSystemClass::unknown_slot67(Point2D const &point, Rect const &window)
{
    if (Class->OneFrameLight && Class->LightSize > 0 && Particles.Count() > 0) {
        // Westwood's Max(0.4, Min(1.0f, fill)) macros: each operand is
        // evaluated again where it is chosen.
        float ratio = (0.4 > ((1.0f < (float)Particles.Count() / Class->ParticleCap) ? 1.0f : (float)Particles.Count() / Class->ParticleCap))
            ? 0.4
            : ((1.0f < (float)Particles.Count() / Class->ParticleCap) ? 1.0f : (float)Particles.Count() / Class->ParticleCap);
        int size = ratio * Class->LightSize;
        SpotLightClass *light = new SpotLightClass(Coord, size);
        light->unknown_0c = unknown_e0;
        light->unknown_func_5ddb90();
        delete light;
    }
}


// ParticleClass *ParticleSystemClass::Spawn_Particle(CoordStruct const &coord, CoordStruct const &target) (0x0060B190)
// A particle of the type the system holds, added to the system.

ParticleClass *ParticleSystemClass::Spawn_Particle(CoordStruct const &coord, CoordStruct const &target)
{
    if (Class->HoldsWhat == -1) {
        return 0;
    }
    ParticleClass *particle = new ParticleClass(ParticleTypes[Class->HoldsWhat], coord, target, this);
    if (particle != 0) {
        Particles.Add(particle);
        return particle;
    }
    return 0;
}


// ParticleClass *ParticleSystemClass::Spawn_Held_Particle(ParticleTypeClass *type, CoordStruct const &coord) (0x0060B240)
// A particle of a given type at a coordinate (no target, no system), added
// to the system.

ParticleClass *ParticleSystemClass::Spawn_Held_Particle(ParticleTypeClass *type, CoordStruct const &coord)
{
    ParticleClass *particle = new ParticleClass(type, coord, unknown_geometry_l, 0);
    if (particle != 0) {
        Particles.Add(particle);
        return particle;
    }
    return 0;
}


// ParticleSystemClass range-limited trailing particle insertion, 0060B2D0.
extern "C" int __cdecl abs(int);
ParticleClass *ParticleSystemClass::Spawn_Particle(CoordStruct const &coord, CoordStruct const &target, int const range)
{
    if (Class->HoldsWhat == -1) return 0;
    ParticleClass *particle = new ParticleClass(ParticleTypes[Class->HoldsWhat], coord, target, this);
    if (particle == 0 || range <= 0) return 0;
    Particles.Add(particle);
    int count = Particles.Count();
    ParticleClass **data = &Particles[0];
    int limited_range = range;
    if (count < limited_range) limited_range = count;
    int offset = abs(unknown_id_source_a3d290->Random()()) % limited_range;
    int source = count - 2;
    int stop = count - offset - 2;
    int insert = count - offset;
    if (source > stop) {
        ParticleClass **position = data + source;
        int remaining = source - stop;
        do {
            position[1] = *position;
            --position;
        } while (--remaining != 0);
    }
    data[insert - 1] = particle;
    return particle;
}


// bool ParticleSystemClass::Remove_Particle(int index) (0x0060B3E0)
// Ends the particle at an index (its slot 60) and drops it from the system.

bool ParticleSystemClass::Remove_Particle(int index)
{
    if (index < Particles.Count()) {
        Particles[index]->Delete_Me();
        Particles.Delete(Particles[index]);
        return true;
    }
    return false;
}


// void ParticleSystemClass::Remove_All_Particles() (0x0060B460)
// Remove_Particle for every particle, last first.

void ParticleSystemClass::Remove_All_Particles()
{
    for (int index = Particles.Count() - 1; index >= 0; index--) {
        Remove_Particle(index);
    }
}


// void ParticleSystemClass::Gas_AI() (0x0060B4E0)
// Behaviour 1 (gas): every particle runs its behaviour, then each finished
// particle is replaced by its type's next particle (carrying velocity and
// step over) and removed; the others move.

void ParticleSystemClass::Gas_AI()
{
    int index;
    for (index = Particles.Count() - 1; index >= 0; index--) {
        Particles[index]->Behavior_AI();
    }
    for (index = Particles.Count() - 1; index >= 0; index--) {
        ParticleClass *particle = Particles[index];
        if (particle->unknown_121) {
            ParticleTypeClass *type = particle->Class;
            if (type->NextParticle != -1) {
                CoordStruct position = particle->Coord;
                ParticleTypeClass *next = ParticleTypes[type->NextParticle];
                CoordStruct coord = position + type->NextParticleOffset;
                ParticleClass *spawned = new ParticleClass(next, coord, unknown_geometry_l, 0);
                if (spawned != 0) {
                    Particles.Add(spawned);
                    spawned->Velocity = particle->Velocity;
                    spawned->unknown_b0 = particle->unknown_b0;
                }
            }
            particle->Delete_Me();
        } else {
            particle->Motion_AI();
        }
    }
}


extern "C" int __cdecl abs(int value);
#pragma intrinsic(abs)
inline double Random_Double(double low,double high)
{
    int number=unknown_id_source_a3d290->Random()(0,0x7ffffffe);
    return (high-low)*(number/2147483646.0)+low;
}
static inline Vector3 Normalize(Vector3 const &vector);
inline Vector3 operator*(float scale,Vector3 const &vector)
{
    return Vector3(scale*vector.X,scale*vector.Y,scale*vector.Z);
}
void ParticleSystemClass::Spark_AI()
{
    if (unknown_dc>0) {
        if (unknown_dc==1 || Random_Double(0.0,1.0)<=Class->SpawnSparkPercentage) {
            int random=unknown_id_source_a3d290->Random();
            int count=Class->ParticleCap/2+abs(random)%(Class->ParticleCap/2);
            ParticleTypeClass *held=ParticleTypes[Class->HoldsWhat];
            Random2Class &generator=unknown_id_source_a3d290->Random();
            int rand_y=generator;
            int rand_x=unknown_id_source_a3d290->Random();
            Vector3 spark;
            float vertical_velocity=(float)(int(generator)%held->ZVelocityRange);
            spark.X=(float)(rand_x%held->XVelocity);
            spark.Y=(float)(rand_y%held->YVelocity);
            spark.Z=vertical_velocity;
            while (count>0) {
                ParticleClass *particle=Spawn_Particle(Position_Coord_004f3f40(),Position_Coord_004f3f40());
                ParticleTypeClass const *type=particle->Class;
                Vector3 &direction=particle->unknown_fc;
                particle->unknown_fc.X=(float)(int(unknown_id_source_a3d290->Random())%type->XVelocity);
                particle->unknown_fc.Y=(float)(int(unknown_id_source_a3d290->Random())%type->YVelocity);
                particle->unknown_fc.Z=(float)(type->MinZVelocity+abs(int(unknown_id_source_a3d290->Random()))%type->ZVelocityRange);
                float speed=direction.Length();
                if (unknown_e5) particle->unknown_fc=spark+particle->unknown_fc;
                else { Vector3 &spawn_direction=(Vector3 &)Class->SpawnDirection;
                    particle->unknown_fc=spawn_direction+direction; }
                particle->unknown_fc=Normalize(particle->unknown_fc);
                particle->unknown_fc=speed*particle->unknown_fc;
                --count;
            }
            if (Options.DetailLevel==2) {
                if (unknown_dc==Class->SparkSpawnFrames && Class->LightSize>0 && !Class->OneFrameLight)
                    new SpotLightClass(Position_Coord_004f3f40(),Class->LightSize);
            }
        }
        --unknown_dc;
        if (unknown_dc<=0) unknown_e4=true;
        double chance=Random_Double(0.0,1.0);
        if (chance<0.3) {
            int radius;
            if(unknown_e0-3<=17) radius=17; else radius=unknown_e0-3;
            unknown_e0=radius;
        } else if (chance<0.6) {
            int radius=unknown_e0+3;
            if (radius>=41) radius=41;
            unknown_e0=radius;
        }
    }
    int index;
    for (index=0;index<Particles.Count();++index) Particles[index]->Behavior_AI();
    for (index=Particles.Count()-1;index>=0;--index) {
        ParticleClass *particle=Particles[index];
        if (particle->unknown_121) particle->Delete_Me();
    }
}


// Not exact (RA2_NON_MATCHING): ParticleSystemClass::unknown_func_60bb70 at 0x0060BB70, best score 54.23, 2026-11-13; differs: 346 instructions in the original, 351 in the draft (shape 93.54)
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x60BB70 ParticleSystemClass Smoke AI: 54.23 (shape 93.54), Scout1127 m6 (no Spread helper; from Scout1123 v15 49.78)
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
#include "shared/foot.hpp"
#include "shared/cd_timer.hpp"

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

    unknown_d4 = Class->Slowdown + unknown_d4;
    if (unknown_d4 > Class->SpawnCutoff) {
        unknown_e4 = true;
    }
}
#endif

// void ParticleSystemClass::Railgun_AI() (0x0060C050)
// Behaviour 4 (railgun): once, lays a spiral of particles along the line
// from the system to its target (with random position, direction and speed
// perturbation) and draws the laser when the type has one; then runs and
// retires the particles.
double __cdecl unknown_sin_4ba480(double angle);    // 0x004BA480
double __cdecl unknown_cos_4ba4d0(double angle);    // 0x004BA4D0
CoordStruct __fastcall Lerp(CoordStruct const &from, CoordStruct const &to, float t);   // 0x007220C0

static inline Vector3 Normalize(Vector3 const &v)
{
    double length = Fast_Sqrt(v.X * v.X + v.Y * v.Y + v.Z * v.Z);
    if (length != 0.0) {
        return Vector3((float)(v.X / length), (float)(v.Y / length), (float)(v.Z / length));
    }
    return v;
}

static inline Vector3 &operator+=(Vector3 &a, Vector3 const &b)
{
    a.X += b.X;
    a.Y += b.Y;
    a.Z += b.Z;
    return a;
}

// The limit is only ever compared as a value: read through this inline, VC6
// loads it before the compare (fld; fcompp) instead of folding it into fcomp.
static inline double Velocity_Limit(ParticleSystemTypeClass const *type)
{
    return type->VelocityPerturbationCoefficient;
}

void ParticleSystemClass::Railgun_AI()
{
    int i;
    if (!unknown_e4 && Particles.Count() == 0) {
        CoordStruct delta = TargetCoord - Position_Coord_004f3f40();
        int dx = delta.X;
        int dy = delta.Y;
        int dz = delta.Z;
        int total_dist = (int)Fast_Sqrt((double)dx * (double)dx + (double)dy * (double)dy + (double)dz * (double)dz);
        double dy_double = dy;
        double dx_double = dx;
        double horiz = dy_double * dy_double;
        int horiz_dist = (int)Fast_Sqrt(horiz + dx_double * dx_double);
        int clamped_z = dz;
        int clamped_x = dx;
        if (clamped_z >= total_dist) clamped_z = total_dist;
        if (clamped_z <= -total_dist) clamped_z = -total_dist;
        if (clamped_x >= horiz_dist) clamped_x = horiz_dist;
        if (clamped_x <= -horiz_dist) clamped_x = -horiz_dist;
        int num_particles = (int)((double)total_dist * Class->ParticlesPerCoord);
        float dist_f = (float)total_dist;
        float elevation = Fast_Asin_004ba550((double)clamped_z / dist_f);
        float azimuth = Fast_Acos_004ba580((double)clamped_x / horiz_dist);
        if (dy < 0) {
            azimuth = -azimuth;
        }
        Matrix3D mat;
        mat.Make_Identity();
        mat.Rotate_Z(azimuth);
        mat.Rotate_X(elevation);
        float velocity_accum = 0.0f;
        i = 0;
        if (num_particles > 0) {
            float num_particles_f = (float)num_particles;
            do {
                float frac = (float)i / num_particles_f;
                double angle = dist_f;
                angle *= frac;
                angle *= Class->SpiralDeltaPerCoord;
                double sine = unknown_sin_4ba480(angle);
                double cosine = unknown_cos_4ba4d0(angle);
                Vector3 spiral(0.0f, cosine, sine);
                spiral = mat * spiral;
                Vector3 scaled = spiral * Class->SpiralRadius;
                Vector3 tmp(((Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10) - 0.5) * Class->PositionPerturbationCoefficient,
                            ((Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10) - 0.5) * Class->PositionPerturbationCoefficient,
                            ((Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10) - 0.5) * Class->PositionPerturbationCoefficient);
                scaled += tmp;
                CoordStruct particle_pos = CoordStruct((int)scaled.X, (int)scaled.Y, (int)scaled.Z) + Lerp(Position_Coord_004f3f40(), TargetCoord, frac);
                ParticleClass *particle = Spawn_Particle(particle_pos, particle_pos);
                particle->unknown_fc = spiral;
                Vector3 move(((Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10) - 0.5) * Class->MovementPerturbationCoefficient,
                             ((Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10) - 0.5) * Class->MovementPerturbationCoefficient,
                             ((Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10) - 0.5) * Class->MovementPerturbationCoefficient);
                particle->unknown_fc += move;
                particle->unknown_fc = Normalize(particle->unknown_fc);
                float vp = ((Random_Pick(0, 0x7ffffffe) * 4.656612877414201e-10) + velocity_accum - 0.5)
                    * (Class->VelocityPerturbationCoefficient * 0.5);
                velocity_accum = (float)((((vp < Velocity_Limit(Class)) ? vp : Velocity_Limit(Class)) > -Class->MovementPerturbationCoefficient) ? ((vp < Velocity_Limit(Class)) ? vp : Velocity_Limit(Class)) : -Class->MovementPerturbationCoefficient);
                particle->Velocity = velocity_accum + particle->Class->Velocity;
                i++;
            } while (i < num_particles);
        }
        if (Class->Laser) {
            new LaserDrawClass(Position_Coord_004f3f40(), TargetCoord, 0, true, Class->LaserColor, RGBClass(0, 0, 0),
                               RGBClass(0, 0, 0), 10, false, true, 0.5f, 0.0f);
        }
        unknown_e4 = true;
    }
    for (i = 0; i < Particles.Count(); i++) {
        Particles[i]->Behavior_AI();
    }
    for (int j = Particles.Count() - 1; j >= 0; j--) {
        ParticleClass *particle = Particles[j];
        if (particle->unknown_121) {
            particle->Delete_Me();
        }
    }
}


// Fire particles track a rotating source and oscillate around its target.
extern "C" void * __cdecl memcpy(void *, void const *, unsigned int);
extern short Wave_Sine_Table_00af91e4[500];
extern int Particle_Fire_Step_X[8];
extern int Particle_Fire_Step_Y[8];
int __fastcall Wave_Direction_00721db0(Point2D const &start, Point2D const &end);
inline CoordStruct Particle_Coord_Along_Facing(UnknownFacingDir facing, int distance, CoordStruct coord)
{
    double angle = ((short)facing.Raw - 16383) * -9.587672516830327e-05;
    int z = coord.Z;
    int y = (int)(coord.Y - (double)Fast_Sin(angle) * distance);
    int x = (int)(coord.X + (double)Fast_Cos(angle) * distance);
    UnknownVector3 point(x, y, z);
    CoordStruct result;
    memcpy(&result, &point, sizeof result);
    return result;
}
void ParticleSystemClass::Fire_AI()
{
    int i;
    for (i = Particles.Count() - 1; i >= 0; --i) {
        ParticleClass *particle = Particles[i];
        particle->Behavior_AI();
        particle->Motion_AI();
    }
    for (i = Particles.Count() - 1; i >= 0; --i) {
        ParticleClass *particle = Particles[i];
        if (particle->unknown_121) particle->Delete_Me();
    }
    bool reaimed = false;
    TechnoClass *source = Owner->As_Techno();
    if (source) {
        if (source->unknown_7d) {
            if (source->unknown_228 && source->unknown_2e0.Is_Rotating()) {
                CoordStruct target = source->unknown_228->Center_Coord();
                CoordStruct delta = target - source->Position_Coord_004f3f40();
                int distance = (int)Fast_Sqrt((double)delta.X * delta.X + (double)delta.Y * delta.Y + (double)delta.Z * delta.Z);
                TargetCoord = Particle_Coord_Along_Facing(source->unknown_2e0.Current(), distance, source->Position_Coord_004f3f40());
                Set_Coord(source->unknown_slot43(0));
                reaimed = true;
            }
        }
        if (!source->unknown_7d) Delete_Me();
    } else Delete_Me();
    if (!unknown_e4 && (!(Frame % Class->SpawnFrames) || (!(Frame % 3) && reaimed))) {
        int index = Frame % 500;
        CoordStruct delta = Position_Coord_004f3f40() - TargetCoord;
        int squared = delta.X * delta.X + delta.Y * delta.Y + delta.Z * delta.Z;
        int divisor;
        if ((float)Fast_Sqrt((double)squared) < 200.0f) divisor = 3;
        else divisor = 1;
        int offset = (int)((double)(12 * Wave_Sine_Table_00af91e4[index]) / ((float)divisor * 3.0f));
        Point2D to(TargetCoord.X, TargetCoord.Y);
        CoordStruct position = Position_Coord_004f3f40();
        int facing = Wave_Direction_00721db0(Point2D(position.X, position.Y), to);
        CoordStruct spread = CoordStruct(Particle_Fire_Step_X[facing] * offset, Particle_Fire_Step_Y[facing] * offset, 0);
        Spawn_Particle(Position_Coord_004f3f40() + CoordStruct(0, 0, 1), TargetCoord + spread, 4);
    }
}


// void ParticleSystemClass::AI() (0x0060CB70)
// Runs the behaviour routine of the type, ends the system when its lifetime
// runs out, and once ended and empty leaves the map and queues itself in the
// 0x00AC0CD8 list.

void ParticleSystemClass::AI()
{
    switch (Class->BehavesLike) {
    case 0:
        unknown_func_60bb70();
        break;
    case 1:
        Gas_AI();
        break;
    case 2:
        Fire_AI();
        break;
    case 3:
        Spark_AI();
        break;
    case 4:
        Railgun_AI();
        break;
    }
    if (--Lifetime == 0) {
        Delete_Me();
    }
    if (unknown_7d && unknown_e4 && Particles.Count() == 0) {
        Limbo();
        unknown_7d = false;
        unknown_objects_ac0cd8.Add(this);
    }
}


// bool ParticleSystemClass::Is_Inactive() const (0x0060CC70)
// True once the system has ended and has no particles left.

bool ParticleSystemClass::Is_Inactive() const
{
    return unknown_e4 && Particles.Count() == 0;
}


// int ParticleSystemClass::In_Which_Layer() const (0x0060CC90)
// The layer (2).

int ParticleSystemClass::In_Which_Layer() const
{
    return 2;
}


// void ParticleSystemClass::Detach(AbstractClass *target, bool all) (0x0060CCA0)
// Drops a removed object: a particle, the type, unknown_d0 or the owner
// (losing the owner ends the system).

void ParticleSystemClass::Detach(AbstractClass *target, bool all)
{
    ObjectClass::Detach(target, all);
    Particles.Delete((ParticleClass *)target);
    if (target == Class) {
        Class = 0;
    }
    if (target == unknown_d0) {
        unknown_d0 = 0;
    }
    if (target == Owner) {
        unknown_e4 = true;
        Owner = 0;
    }
}


// HRESULT __stdcall ParticleSystemClass::Load(IStream *stream) (0x0060CD30)
// Restores the system in place, swizzles its pointers, then reads the
// particle count and pointers back into Particles.

HRESULT __stdcall ParticleSystemClass::Load(IStream *stream)
{
    HRESULT result = ObjectClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (this) ParticleSystemClass(NoInitClass());
    Swizzler.Swizzle((void **)&Class);
    Swizzler.Swizzle((void **)&unknown_d0);
    Swizzler.Swizzle((void **)&Owner);

    int count = 0;
    result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    while (count--) {
        ParticleClass *particle = 0;
        unsigned long read;
        result = stream->Read(&particle, sizeof(particle), &read);
        if (FAILED(result)) {
            return result;
        }
        Particles.Add(particle);
    }
    for (int index = 0; index < Particles.Count(); index++) {
        Swizzler.Swizzle((void **)&Particles[index]);
    }
    return S_OK;
}


// HRESULT __stdcall ParticleSystemClass::Save(IStream *stream, BOOL clear_dirty) (0x0060CEA0)
// AbstractClass::Save, then the particle count and the particle pointers.

HRESULT __stdcall ParticleSystemClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = AbstractClass::Save(stream, clear_dirty);
    if (SUCCEEDED(result)) {
        int count = Particles.Count();
        result = stream->Write(&count, sizeof(count), 0);
        if (SUCCEEDED(result)) {
            for (int index = 0; index < count; index++) {
                result = stream->Write(&Particles[index], sizeof(ParticleClass *), 0);
                if (FAILED(result)) {
                    break;
                }
            }
        }
    }
    return result;
}


// void ParticleSystemClass::Compute_CRC(WWCRCEngine &crc) const (0x0060CF10)

void ParticleSystemClass::Compute_CRC(WWCRCEngine &crc) const
{
    ObjectClass::Compute_CRC(crc);
    for (int index = 0; index < Particles.Count(); index++) {
        crc(Particles[index]->Fetch_ID());
    }
    if (Owner != 0) {
        crc(Owner->Fetch_ID());
    }
    if (unknown_d0 != 0) {
        crc(unknown_d0->Fetch_ID());
    }
    crc(unknown_d4);
    crc((long)Lifetime);
}


// HRESULT __stdcall ParticleSystemClass::GetClassID(GUID *clsid) (0x0060CFB0)
// Compiler-emitted from shared/particle_system.hpp in this unit; no source of its own.


// void ParticleSystemClass::Delete_Me() (0x0060CFF0)
// Compiler-emitted from shared/particle_system.hpp in this unit; no source of its own.


// int ParticleSystemClass::Size_Of() const (0x0060D000)
// Compiler-emitted from shared/particle_system.hpp in this unit; no source of its own.


// int ParticleSystemClass::Kind_Of() const (0x0060D010)
// Compiler-emitted from shared/particle_system.hpp in this unit; no source of its own.


// ObjectTypeClass *ParticleSystemClass::Class_Of() const (0x0060D020)
// Compiler-emitted from shared/particle_system.hpp in this unit; no source of its own.


// void *ParticleSystemClass::`scalar deleting destructor'(unsigned int) (0x0060D030)
// Compiler-emitted from shared/particle_system.hpp in this unit; no source of its own.


extern "C" unsigned char table_60d0a0[]; extern "C" void* __cdecl storage_60d0a0(unsigned int);
class Storage_60d0a0 { unsigned int table_word; unsigned int* items; unsigned int count; unsigned char valid; unsigned char owns; public: void* get(unsigned int requested,unsigned int* buffer); };
void* Storage_60d0a0::get(unsigned int requested,unsigned int* buffer) { items=0; count=requested; valid=1; owns=0; table_word=(unsigned int)table_60d0a0; if(requested) { if(buffer) { items=buffer; return this; } items=(unsigned int*)storage_60d0a0(requested << 2); owns=1; } return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<ParticleClass *>::Add (0x0060D050): emitted by this unit's use of its list.
