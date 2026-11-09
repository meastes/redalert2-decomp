// NON-EXACT DRAFT 0x676010 ShipLocomotionClass::Draw_Matrix
// entry 0x676010; exact-match score 97.08 (misses_index best)
// origin lane: Scout592 (build/class-work/Scout592/w/dm_best_97.08.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1133 (native/drafts.json, M16). Not original bytes.
// Matrix3D * __stdcall ShipLocomotionClass::Draw_Matrix(Matrix3D *matrix, int *key) (0x00676010, ILocomotion 9)
#include "shared/ship_locomotion.hpp"
#include "shared/matrix3d.hpp"
#include "shared/crt_math.hpp"
#include "shared/fast_math.hpp"
#include "shared/techno_type.hpp"
// @ref ?Make_Identity@Matrix3D@@QAEXXZ=00590060
// @ref ?Unknown_00718D60@@YIPAVMatrix3D@@PAV1@H@Z=00718d60
// @ref ?Unknown_00718DF0@@YIPAVMatrix3D@@PAV1@HHN@Z=00718df0
// @ref ??D@YI?AVMatrix3D@@ABV0@0@Z=00591180
// @ref ??0Matrix3D@@QAE@ABV0@@Z=0058fe10
// @ref ?Draw_Matrix@LocomotionClass@@UAGPAVMatrix3D@@PAV2@PAH@Z=0053d030
// @ref ?Translate_X@Matrix3D@@QAEXM@Z=00590180
// @ref ?Translate_Y@Matrix3D@@QAEXM@Z=005901b0
// @ref ?Translate_Z@Matrix3D@@QAEXM@Z=005901e0
// @ref ?Rotate_X@Matrix3D@@QAEXM@Z=00590760
// @ref ?Rotate_Y@Matrix3D@@QAEXM@Z=00590880
// @ref ?Fast_Sin@@YAMN@Z=004ba480
// @ref ?Fast_Cos@@YAMN@Z=004ba4d0
// @ref ?Value@?$CDTimerClass@VFrameTimerClass@@@@QBEJXZ=004a4d20
// @ref __real@8@3fff8000000000000000=0079a708
// @ref __real@8@3ff7a3d70a3d70a3d800=0079d170

Matrix3D * __stdcall ShipLocomotionClass::Draw_Matrix(Matrix3D *matrix, int *key)
{
    Matrix3D transform;
    Matrix3D rotation;
    Matrix3D tilt;
    if (!Is_Level()) {
        transform.Make_Identity();
        rotation.Make_Identity();
        float length = LinkedTo->unknown_slot32()->unknown_330;
        float width = LinkedTo->unknown_slot32()->unknown_338;
        float roll_cos = Fast_Cos(LinkedTo->unknown_284);
        float roll_sin = Fast_Sin(LinkedTo->unknown_284);
        float pitch_cos = Fast_Cos(LinkedTo->unknown_280);
        float pitch_sin = Fast_Sin(LinkedTo->unknown_280);
        int lift = fabs(pitch_sin) * length + fabs(roll_sin) * width;
        int y_shift = width - (int)(roll_cos * width);
        int x_shift = length - (int)(pitch_cos * length);
        if (LinkedTo->unknown_284 < 0.0f) {
            y_shift = -y_shift;
        }
        if (LinkedTo->unknown_280 > 0.0f) {
            x_shift = -x_shift;
        }
        transform.Translate_Z((float)lift);
        rotation.Translate_X((float)y_shift);
        rotation.Translate_Y((float)x_shift);
        rotation.Rotate_X(LinkedTo->unknown_280);
        rotation.Rotate_Y(LinkedTo->unknown_284);
        if (key != 0) {
            *key = -1;
        }
        Matrix3D temp;
        Matrix3D body(*LocomotionClass::Draw_Matrix(&temp, key));
        transform = transform * *unknown_675a60(&tilt) * body * rotation;
    } else {
        if (key != 0) {
            if (*key != -1) {
                int keybase = *key << 6;
                *key = keybase;
                *key += unknown_1c;
            }
        }
        Matrix3D temp;
        Matrix3D body(*LocomotionClass::Draw_Matrix(&temp, key));
        transform = *unknown_675a60(&tilt) * body;
    }
    *matrix = transform;
    return matrix;
}

