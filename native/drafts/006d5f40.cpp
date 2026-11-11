// NON-EXACT DRAFT 0x6D5F40 FUN_006d5f40
// entry 0x6D5F40; exact-match score 96.38 (misses_index best), this text 64.37; plan "draft as-is"
// origin lane: Scout767 (build/class-work/Scout767/w/keep/rk5.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// Native adaptation (Scout1069): VECTOR3_DECLARE_WWMATH inlines spelled out; each changed line is marked NATIVE.
// Rocking from an impact (TechnoClass slot 223, 0x006D5F40).
// NATIVE: VECTOR3_DECLARE_WWMATH is off in the host group (vector3.hpp is already included); its two inlines are spelled out below.
#include "shared/techno.hpp"
#include "shared/techno_type.hpp"
#include "shared/fast_math.hpp"
#include "shared/vector3.hpp"
#include "shared/crt_math.hpp"
#include "shared/unknown_pair_first.hpp"
// @ref __real@4@3fefd1b7170000000000=007ad008
// @ref __real@4@3ffaa3d70a0000000000=007ad004
// @ref __real@4@3ffacccccd0000000000=007ad000
inline Vector3 Normalize(Vector3 const &vector)
{
    float len=Fast_Sqrt(vector.X * vector.X + vector.Y * vector.Y + vector.Z * vector.Z);   // NATIVE: Length2()
    if (len!=0.0) {
        return Vector3(vector.X / len, vector.Y / len, vector.Z / len);   // NATIVE: operator/
    }
    return vector;
}
inline double Rock_Length(Vector3 const &v)
{
    double z = v.Z;
    double y = v.Y;
    double x = v.X;
    return Fast_Sqrt(x * x + y * y + z * z);
}
void TechnoClass::unknown_slot223(CoordStruct const &coord, float force)
{
    if (unknown_slot32()->unknown_b0.unknown_00 && !unknown_slot32()->unknown_b0.unknown_00->LoadFailed) {
        CoordStruct pos=Coord;
        Vector3 vec1;
        vec1.X=pos.X-coord.X;
        vec1.Y=coord.Y-pos.Y;
        vec1.Z=pos.Z-coord.Z;
        Vector3 vec2=vec1;

        float theta=unknown_2e0.Current().As_Radian();
        float sangle=Fast_Sin(theta);
        float cangle=Fast_Cos(theta);

        float dist1=Rock_Length(vec1);

        float scale=(0.04f-dist1*0.000025f)*force/unknown_slot32()->Weight;

        if (fabs(dist1)>=0.00002 && scale>=0.01f) {
            if (scale>0.05f) {
                scale=0.05f;
            }

            vec2.Z=0.0;
            vec1=Normalize(vec2);

            dist1=vec1.X*cangle+vec1.Y*sangle;
            vec2.X=-(vec1.Z*sangle);
            vec2.Y=+(vec1.Z*cangle);
            vec2.Z=(vec1.X*sangle)-(vec1.Y*cangle);
            double dist2=Rock_Length(vec2);
            if (fabs(dist1*cangle-dist2*sangle-vec1.X)>0.0002 || fabs(dist2*cangle+dist1*sangle-vec1.Y)>0.0002) {
                dist2=-dist2;
            }
            unknown_28c=dist1*scale/2.0f;
            unknown_288=-((float)dist2*scale);
        }
    }
}
