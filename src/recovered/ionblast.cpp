// ionblast.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ionblast.json carries the addresses and the rows that
// stay in their own files.


// Header-static dynamic initializers of the unit at 0x00520FA0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00520FA0-0x00521186), then IonBlast.cpp's file data; the other
// initializer drafts of this plan are emitted by these includes and definitions.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
#include "shared/rect.hpp"
#include "shared/surface.hpp"
#include "shared/unknown_globals.hpp"
#define RA2_ION_BLAST_LIST
#include "shared/ion_blast.hpp"

extern "C" int __cdecl abs(int);
#pragma intrinsic(abs)

// IonBlast.cpp's file data (TibSun ionblast.cpp names; the wave constants'
// values match TibSun's): the spiral index to surface offset table and the
// stride it was built for, the 80 precalculated shockwave surfaces, the
// shockwave parameters and the list of blasts in progress. WaveSpeed
// (0x00521190), WavePhaseSpeed (0x005211B0) and IonBlasts (0x005211D0, which
// registers the guarded atexit destructor 0x00521210) have dynamic initializers.
int SpiralIndexToSurfaceLUT[289];
int BlastSurfaceStride = 0;
BSurface *BlastSurfaces[80];

static double const WaveStartDistance = 0.0;
static double const WaveBandWidth = 57.0;
static double const WaveFrequency = 0.11;
static double const WaveAmplitudeFalloff = 25.0;
static double const WaveRockScale = 6.283185307179586;     // 360 degrees in radians
static double const WaveSpeedScale = 0.0125;

static double WaveSpeed = ((256.0 - WaveStartDistance) + WaveBandWidth) * WaveSpeedScale;
static double WavePhaseSpeed = WaveSpeed;

DynamicVectorClass<IonBlastClass *> IonBlastClass::IonBlasts;

// TibSun's MAX (Point_To_Spiral_Index keeps the second operand unless the
// first is larger).
template <class T> inline T Max(T a, T b) { return (a > b) ? a : b; }

Point2D __fastcall Spiral_Index_To_Point(int index);
int __fastcall Point_To_Spiral_Index(Point2D point);
void __fastcall Calculate_Index_To_Surface_LUT(int stride);


// Dynamic initializer (0x00520FD0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer (0x00520FF0, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer (0x00521010, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer (0x00521030, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer (0x00521050, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer (0x00521070, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer (0x005210B0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer (0x005210E0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer (0x00521110, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer (0x00521140, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer (0x00521150, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer (0x00521170, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer of WaveSpeed (0x00521190, _$E) emitted by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer of WavePhaseSpeed (0x005211B0, _$E) emitted by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// Dynamic initializer of IonBlastClass::IonBlasts (0x005211D0, _$E) emitted by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// atexit destructor of IonBlastClass::IonBlasts (0x00521210, _$E) emitted by HeaderStatics_diagonal_init_00520fa0.cpp; no source of its own.


// IonBlastClass::IonBlastClass(CoordStruct) (0x00521270): a new blast at
// coord (TibSun ionblast.cpp) joins the list of blasts in progress.
IonBlastClass::IonBlastClass(CoordStruct coord) :
    Coord(coord),
    Lifetime(0),
    unknown_10(0)
{
    IonBlasts.Add(this);
}


// IonBlastClass::~IonBlastClass() (0x005212F0): leaves the list of blasts.
IonBlastClass::~IonBlastClass()
{
    IonBlasts.Delete(this);
}


// IonBlastClass::AI() (0x00521340): one frame of an ion blast; the shock wave
// rocks every foot occupier within 128 screen pixels (Scout571).
#define VECTOR3_DECLARE_WWMATH
extern "C" int __cdecl abs(int);
#pragma intrinsic(abs)
extern "C" double __cdecl fabs(double);
#pragma intrinsic(fabs)
#include "shared/foot.hpp"
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/object_type.hpp"
#include "shared/unknown_pair_first.hpp"
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
#include "shared/rules.hpp"
#include "shared/anim.hpp"
#include "shared/tactical.hpp"
#include "shared/facing.hpp"
#include "shared/vector3.hpp"
#include "shared/fast_math.hpp"
double __cdecl unknown_sin_4ba480(double angle);    // 0x004BA480
double __cdecl unknown_cos_4ba4d0(double angle);    // 0x004BA4D0
inline float Length552(Vector3 const &v) { return Fast_Sqrt((double)v.X * v.X + (double)v.Y * v.Y + (double)v.Z * v.Z); }
void IonBlastClass::AI()
{
    if (Lifetime >= 79) {
        delete this;
        return;
    }

    if (unknown_10 != 1) {
        Point2D point;
        unknown_tactical_83996c->Coord_To_Pixel(Coord, point);

        if (Lifetime == 0) {
            if (Map[Coord].unknown_ec == 2) {
                new AnimClass(Rules->SplashList[Rules->SplashList.Count() - 1], Coord + CoordStruct(0, 0, 5), 0, 1, 0x600, 0);
            } else {
                new AnimClass(Rules->IonBlast, Coord + CoordStruct(0, 0, 5), 0, 1, 0x600, 0);
            }
            new AnimClass(Rules->IonBeam, Coord + CoordStruct(0, 0, 5), 0, 1, 0x600, 0);

            if (Map[Coord].IsUnderBridge) {
                CoordStruct coord = CoordStruct(Coord.X, Coord.Y, unknown_geometry_j + Coord.Z);
                Unknown_0047E2C0(coord, Rules->IonCannonDamage, 0, Rules->IonCannonWarhead, 1);
            }

            Unknown_0047E2C0(Coord, Rules->IonCannonDamage, 0, Rules->IonCannonWarhead, 1);
            Unknown_0047F600(Coord, Rules->IonCannonDamage, Rules->IonCannonWarhead, 0, 0);
        }

        Cell center(Coord);

        for (int x_offset = -3; x_offset < 4; x_offset++) {
            for (int y_offset = -3; y_offset < 4; y_offset++) {
                CellClass &cell = Map[center + Cell(x_offset, y_offset)];
                ObjectClass *occupier = cell.OccupierPtr;

                while (occupier != 0) {
                    if (occupier->Kind_Of() == 0x0F || occupier->Kind_Of() == 1) {
                        FootClass *foot = (FootClass *)occupier;
                        Point2D occupier_point;
                        unknown_tactical_83996c->Coord_To_Pixel(occupier->Position_Coord_004f3f40(), occupier_point);

                        Point2D diff = occupier_point - point;
                        int screen_dist = (int)Fast_Sqrt((double)diff.Y * diff.Y + (double)diff.X * diff.X) + 8;

                        if (screen_dist < 128) {
                            char *displacement = (char *)BlastSurfaces[Lifetime]->Lock(Point2D(screen_dist + 128, 64));

                            if (*displacement > 0) {
                                foot->unknown_slot306(0.0);
                                int draw_offset = Spiral_Index_To_Point(*displacement).Y;
                                *(signed char *)&foot->unknown_59e = 2 * draw_offset;
                            }

                            VoxelLibrary *voxel;
                            if (foot->Techno_Type_Class()->unknown_b0.unknown_00 != 0 && (voxel = foot->Techno_Type_Class()->unknown_b0.unknown_00, !voxel->LoadFailed) && *displacement >= 0) {

                                CoordStruct blast_coord = Coord;
                                CoordStruct occupier_coord = occupier->Position_Coord_004f3f40();
                                Vector3 to_blast;
                                to_blast.X = (float)(blast_coord.X - occupier_coord.X);
                                to_blast.Y = (float)(occupier_coord.Y - blast_coord.Y);
                                to_blast.Z = (float)(blast_coord.Z - occupier_coord.Z);
                                Vector3 flat = to_blast;

                                double facing_rad = (double)((short)foot->unknown_2e0.Current().Raw - 0x3FFF) * -9.587672516830327e-05;
                                float facing_sin = (float)unknown_sin_4ba480(facing_rad);
                                Vector3 facing;
                                facing.X = (float)unknown_cos_4ba4d0(facing_rad);

                                float blast_dist = Fast_Sqrt((double)to_blast.X * to_blast.X + (double)to_blast.Y * to_blast.Y + (double)to_blast.Z * to_blast.Z);

                                if (fabs(blast_dist) > 0.00002) {
                                    flat.Z = 0.0;

                                    double flat_len = Fast_Sqrt(flat.X * flat.X + flat.Y * flat.Y);

                                    Vector3 normal;
                                    if (flat_len != 0.0) {
                                        normal = flat / flat_len;
                                    } else {
                                        normal = flat;
                                    }

                                    float forwards = normal.X * facing.X + normal.Y * facing_sin;
                                    float sideways = Length552(Vector3(-(normal.Z * facing_sin), normal.Z * facing.X, normal.X * facing_sin - normal.Y * facing.X));

                                    float c2 = facing.X;
                                    float s2 = facing_sin;
                                    float c3 = facing.X;
                                    float s3 = facing_sin;
                                    if (fabs(c2 * forwards - s2 * sideways - normal.X) > 0.0002
                                        || fabs(c3 * sideways + s3 * forwards - normal.Y) > 0.0002) {
                                        sideways = -sideways;
                                    }

                                    double decay = blast_dist + WaveAmplitudeFalloff;
                                    double height = (unknown_sin_4ba480((blast_dist - (double)Lifetime * WavePhaseSpeed + 38.0) * WaveFrequency) * 3.5 + 3.0) * WaveAmplitudeFalloff;
                                    double slope = ((unknown_cos_4ba4d0((blast_dist - (double)Lifetime * WavePhaseSpeed + 38.0) * WaveFrequency) * WaveFrequency * WaveAmplitudeFalloff * 3.5) * decay - height) / (decay * decay);

                                    foot->unknown_280 = (float)(sideways * slope * WaveRockScale);
                                    foot->unknown_284 = (float)(-(forwards * slope * WaveRockScale));
                                }
                            }
                        }
                    }
                    occupier = occupier->unknown_30;
                }
            }
        }
    }

    Lifetime++;
}


// IonBlastClass::Update_All() (0x00521A70): one frame of every blast, last
// first (a finished blast deletes itself from the list).
void IonBlastClass::Update_All()
{
    for (int i = IonBlasts.Count() - 1; i >= 0; i--) {
        IonBlasts[i]->AI();
    }
}


// IonBlastClass::Clear_All() (0x00521CB0): frees the shockwave surfaces.
void IonBlastClass::Clear_All()
{
    for (int i = 0; i < 80; i++) {
        delete BlastSurfaces[i];
        BlastSurfaces[i] = 0;
    }
}


// Not exact (RA2_NON_MATCHING): IonBlastClass::One_Time at 0x00521A90, best score 88.31, 2026-11-13; differs: same instruction count (154), shape 99.35: instruction choice and registers differ
#ifdef RA2_NON_MATCHING
// NON-EXACT DRAFT 0x521A90 IonBlastClass::One_Time
// entry 0x521A90; exact-match score 88.31 (/O2 /Ob2 /G5, shape 99.35; Scout1076); plan "draft, verify"
// origin lane: Scout1076 (written from the adopted TibSun ionblast.cpp text, THIRD_PARTY.md)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// native/drafts.json. Not original bytes.
// IonBlastClass::One_Time() (0x00521A90): builds the 80 shockwave displacement surfaces
// once (256x128, one byte per pixel, filled with -1). Each frame rasterises the ripple
// band [min_dist, max_dist] of one quadrant around the centre (0x4080) and mirrors it;
// a pixel holds the spiral index of the wave height. Both band edges then advance by
// WaveSpeed. The constants (0, 57, 38, 0.11, 3.5, 3, 25, 1, 0.5, 128, 1.2) were checked
// against the pinned target's .rdata; WavePhaseSpeed/WaveSpeed are IonBlast.cpp's statics.
double __cdecl unknown_sin_4ba480(double angle);    // 0x004BA480 (fast sine)

void IonBlastClass::One_Time()
{
    static bool one_time = false;
    if (one_time) {
        return;
    }

    double min_dist = WaveStartDistance - WaveBandWidth;
    double max_dist = WaveStartDistance;

    for (int frame = 0; frame < 80; frame++) {
        BlastSurfaces[frame] = new BSurface(256, 128, 1);
        BlastSurfaces[frame]->Fill(-1);

        char *center = (char *)BlastSurfaces[frame]->Lock() + 0x4080;
        for (int y = 63; y >= 0; y--) {
            int y_dist_sq = y * y * 4;
            for (int x = 127; x >= 0; x--) {
                double dist = unknown_sqrt_4ba410(x * x + y_dist_sq);
                if (dist >= min_dist && dist <= max_dist) {
                    double wave_height = (unknown_sin_4ba480((dist - frame * WavePhaseSpeed + 38.0) * WaveFrequency) * 3.5 + 3.0)
                                         / (dist / WaveAmplitudeFalloff + 1.0) + 0.5;
                    char spiral_index = Point_To_Spiral_Index(Point2D(0, wave_height));
                    int row = y << 8;
                    center[row + x] = spiral_index;
                    center[row - x] = spiral_index;
                    char *mirror = center - row;
                    mirror[x] = spiral_index;
                    mirror[-x] = spiral_index;
                }
            }
        }

        if (128.0 - WaveSpeed > max_dist) {
            max_dist += WaveSpeed;
        }

        min_dist = WaveSpeed * 1.2 + min_dist;

        if (min_dist > max_dist) {
            min_dist = max_dist;
        }
    }

    one_time = true;
}
#endif

// void IonBlastClass::Draw_It() (0x00521CE0; TibSun ionblast.cpp): warps the logical surface through the
// blast's shockwave surface for this lifetime frame, depth masked, at the highest detail level.
#include "shared/blit.hpp"
extern Rect unknown_rect_839630;        // 0x00839630 tactical X, Y, width, height
void IonBlastClass::Draw_It()
{
    Point2D point;
    if (unknown_a40b2c == 2 && unknown_tactical_83996c->Coord_To_Pixel(Coord, point)) {
        Surface *dest_surface = unknown_surface_83995c;
        Rect dcliprect = unknown_rect_839630;
        dcliprect.Height -= 7;
        Surface *source_surface = BlastSurfaces[Lifetime];
        int stride = dest_surface->Stride();
        bool overlapped = false;
        Rect srect(0, 0, 256, 128);
        Rect scliprect(0, 0, 256, 128);
        Rect drect(point.X - 128, point.Y - 64, 256, 128);
        char *sbuffer;
        short *dbuffer;

        if (Prep_For_Blit(*dest_surface, dcliprect, drect, *source_surface, scliprect, srect, overlapped, (void *&)dbuffer, (void *&)sbuffer)) {
            char *sptr;
            short *dest_row = dbuffer;
            short *dptr = dbuffer;
            char *source_row = sbuffer;
            sptr = sbuffer;

            int depth_z = Unknown_006A2970(Coord.Z);
            short base_z = (short)DepthBuffer->Get_Scroll() - depth_z;
            unsigned short draw_z = base_z;
            draw_z -= drect.Y;
            draw_z -= 3;

            unsigned short *zbuffer_base = DepthBuffer->Get_Buffer_Offset(Point2D(0, drect.Y));
            int height = srect.Height + 1;
            int surface_width = unknown_surface_83995c->Get_Width();
            int buffer_width = DepthBuffer->Width;

            if (zbuffer_base + surface_width + height * buffer_width < (unsigned short *)DepthBuffer->BufferTail) {
                unsigned short *zptr = zbuffer_base + drect.X;
                int x;
                for (int y = 0; y < srect.Height; ++y) {
                    for (x = 0; x < srect.Width; ++x) {
                        int index = *sptr++;
                        if (index > 0 && *zptr > draw_z) {
                            *dptr = dptr[SpiralIndexToSurfaceLUT[index]];
                        }
                        dptr++;
                        zptr++;
                    }
                    dest_row = (short *)((char *)dest_row + stride);
                    dptr = dest_row;
                    source_row += 256;
                    draw_z--;
                    sptr = source_row;
                    zptr += buffer_width - x;
                }
            } else {
                short *pixel = dptr;
                for (int y = 0; y < srect.Height; ++y) {
                    for (int x = 0; x < srect.Width; ++x) {
                        int index = *sptr++;
                        if (index > 0 && *DepthBuffer->Get_Buffer_Offset(Point2D(drect.X + x, drect.Y + y)) > draw_z) {
                            *pixel = pixel[SpiralIndexToSurfaceLUT[index]];
                        }
                        pixel++;
                    }
                    dest_row = (short *)((char *)dest_row + stride);
                    pixel = dest_row;
                    source_row += 256;
                    draw_z--;
                    sptr = source_row;
                }
            }

            dest_surface->Unlock();
            source_surface->Unlock();
        }
    }
}


// IonBlastClass::Draw_All() (0x00521FB0): rebuilds the offset table when the
// drawing surface's stride changed, then draws every blast's shockwave.
void IonBlastClass::Draw_All()
{
    if (unknown_surface_83995c->Stride() != BlastSurfaceStride) {
        Calculate_Index_To_Surface_LUT(unknown_surface_83995c->Stride());
    }
    for (int i = IonBlasts.Count() - 1; i >= 0; i--) {
        IonBlasts[i]->Draw_It();
    }
}


// Spiral_Index_To_Point(int) (0x00522040): the grid offset a spiral index
// stands for (index 0 is the centre; ring n holds 8n points).
Point2D __fastcall Spiral_Index_To_Point(int index)
{
    index--;
    int layer = 1;
    if (index >= 8) {
        for (int i = 8; index >= i; i += 8) {
            index -= i;
            layer++;
        }
    }
    if (index < 2 * layer + 1) {
        return Point2D(layer, index - layer);
    } else if (index < 4 * layer + 1) {
        return Point2D(3 * layer - index, layer);
    } else if (index < 6 * layer + 1) {
        return Point2D(-layer, 5 * layer - index);
    } else {
        return Point2D(index - 7 * layer, -layer);
    }
}


// Point_To_Spiral_Index(Point2D) (0x005220C0): the inverse of
// Spiral_Index_To_Point.
int __fastcall Point_To_Spiral_Index(Point2D point)
{
    if (point == Point2D(0, 0)) {
        return 0;
    }
    int layer = Max(abs(point.X), abs(point.Y));
    int index = 1;
    for (int i = 1; i < layer; i++) {
        index += 8 * i;
    }
    if (point.X == layer) {
        index += 1 * layer + point.Y;
    } else if (point.Y == layer) {
        index += 3 * layer - point.X;
    } else if (point.X == -layer) {
        index += 5 * layer - point.Y;
    } else {
        index += 7 * layer + point.X;
    }
    return index;
}


// Calculate_Index_To_Surface_LUT(int) (0x00522150): each spiral index's
// offset in a surface of the given stride (expanded in Draw_All).
void __fastcall Calculate_Index_To_Surface_LUT(int stride)
{
    SpiralIndexToSurfaceLUT[0] = 0;
    BlastSurfaceStride = stride;
    for (int index = 1; index < 289; index++) {
        Point2D point = Spiral_Index_To_Point(index);
        SpiralIndexToSurfaceLUT[index] = point.X + point.Y * stride;
    }
}


extern "C" unsigned char vtable_522460[]; extern "C" void __cdecl release_buffer_522460(void*); extern "C" void __cdecl release_object_522460(void*);
class Leaf_522460 { public: void* get(unsigned int flags); };
void* Leaf_522460::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_522460; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_522460((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_522460(this); return this; }


extern "C" unsigned char vtable_5224b0[]; extern "C" void __cdecl release_buffer_5224b0(void*); extern "C" void __cdecl release_object_5224b0(void*);
class Leaf_5224b0 { public: void* get(unsigned int flags); };
void* Leaf_5224b0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_5224b0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_5224b0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_5224b0(this); return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<IonBlastClass *>::Clear (0x005221A0; DynamicVectorClass<IonBlastClass *> vtable 0x007a45d4 slot 3).
// DynamicVectorClass<IonBlastClass *>::ID_pointer (0x005221D0; DynamicVectorClass<IonBlastClass *> vtable 0x007a45d4 slot 5).
// VectorClass<IonBlastClass *>::Fetch (0x005221F0; VectorClass<IonBlastClass *> vtable 0x007a45f4 slot 6).
// VectorClass<IonBlastClass *>::operator equal (0x00522200; VectorClass<IonBlastClass *> vtable 0x007a45f4 slot 1).
// VectorClass<IonBlastClass *>::Resize (0x00522240; VectorClass<IonBlastClass *> vtable 0x007a45f4 slot 2).
// VectorClass<IonBlastClass *>::Clear (0x005222F0; VectorClass<IonBlastClass *> vtable 0x007a45f4 slot 3).
// VectorClass<IonBlastClass *>::ID_object (0x00522320; VectorClass<IonBlastClass *> vtable 0x007a45f4 slot 4).
// VectorClass<IonBlastClass *>::ID_pointer (0x00522360; VectorClass<IonBlastClass *> vtable 0x007a45f4 slot 5).
// DynamicVectorClass<IonBlastClass *>::Resize (0x00522380; DynamicVectorClass<IonBlastClass *> vtable 0x007a45d4 slot 2).
// DynamicVectorClass<IonBlastClass *>::ID_object (0x00522430; DynamicVectorClass<IonBlastClass *> vtable 0x007a45d4 slot 4).
// bool DynamicVectorClass<IonBlastClass *>::Delete(int index) (0x00522500).
// Instantiated from shared/vector_class.hpp; called by the IonBlastClass destructor for the IonBlastClass list (0x00A51F88, vtable 0x007A45D4).
