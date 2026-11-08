// Behaviour-written locomotor rows the skirmish harness reaches when the
// tactical view draws the start units (Scout1091, native M8). NOT EXACT; never
// original code or bytes.
#include "shared/drive_locomotion.hpp"
#include "shared/matrix3d.hpp"
#include "shared/crt_math.hpp"
#include "shared/fast_math.hpp"
#include "shared/techno_type.hpp"
#include "shared/foot.hpp"

// 0x004A0050 DriveLocomotionClass::Draw_Matrix (ILocomotion; unrecovered,
// parked 95.91 in Scout907/w/d3n.cpp). From the decompile and that draft: the
// slope tilt matrix (0x0049FAA0 here: the ramp's, or the blend while the ramp
// changes); a level vehicle (0x0049FFC0) keys its cached matrix by the ramp
// (key * 64 + ramp) and returns tilt * base; otherwise the hull is lifted by
// its roll/pitch extents, shifted and rotated by the pitch (+0x280) and roll
// (+0x284), the key is -1, and the result is lift * tilt * base * rotation.
Matrix3D * __stdcall DriveLocomotionClass::Draw_Matrix(Matrix3D *matrix, int *key)
{
    Matrix3D tilt;
    unknown_49faa0(&tilt);
    Matrix3D transform;
    if (!Is_Level()) {
        Matrix3D rotation;
        transform.Make_Identity();
        rotation.Make_Identity();
        float length = (float)LinkedTo->unknown_slot32()->unknown_330;
        float width = (float)LinkedTo->unknown_slot32()->unknown_338;
        float roll_cos = Fast_Cos(LinkedTo->unknown_284);
        float roll_sin = Fast_Sin(LinkedTo->unknown_284);
        float pitch_cos = Fast_Cos(LinkedTo->unknown_280);
        float pitch_sin = Fast_Sin(LinkedTo->unknown_280);
        int lift = (int)(fabs(pitch_sin) * length + fabs(roll_sin) * width);
        int y_shift = (int)(width - (float)(int)(roll_cos * width));
        int y_offset = y_shift;
        int x_shift = (int)(length - (float)(int)(pitch_cos * length));
        if (LinkedTo->unknown_284 < 0.0f) {
            y_offset = -y_shift;
        }
        if (LinkedTo->unknown_280 > 0.0f) {
            x_shift = -x_shift;
        }
        transform.Translate_Z((float)lift);
        rotation.Translate_X((float)y_offset);
        rotation.Translate_Y((float)x_shift);
        rotation.Rotate_X(LinkedTo->unknown_280);
        rotation.Rotate_Y(LinkedTo->unknown_284);
        if (key != 0) {
            *key = -1;
        }
        Matrix3D temp;
        Matrix3D base(*LocomotionClass::Draw_Matrix(&temp, key));
        transform = transform * tilt * base * rotation;
    } else {
        if (key != 0 && *key != -1) {
            *key = (*key << 6) + unknown_1c;
        }
        Matrix3D temp;
        Matrix3D base(*LocomotionClass::Draw_Matrix(&temp, key));
        transform = tilt * base;
    }
    *matrix = transform;
    return matrix;
}
