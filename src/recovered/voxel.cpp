// voxel.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/voxel.json carries the addresses and the rows that
// stay in their own files.


#define RA2_MATRIX3D_INLINE_COPY
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/quaternion.hpp"
#include "shared/object_type.hpp"
#include "shared/facing.hpp"
#include "shared/fast_math.hpp"


// Header-static dynamic initializers of the unit at 0x00717CE0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x00717CE0-0x00717EC6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00717D10, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_00717ce0.cpp; no source of its own.


// Dynamic initializer (0x00717D30, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_00717ce0.cpp; no source of its own.


// Dynamic initializer (0x00717D50, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_00717ce0.cpp; no source of its own.


// Dynamic initializer (0x00717D70, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_00717ce0.cpp; no source of its own.


// Dynamic initializer (0x00717D90, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_00717ce0.cpp; no source of its own.


// Dynamic initializer (0x00717DB0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_00717ce0.cpp; no source of its own.


// Dynamic initializer (0x00717DF0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_00717ce0.cpp; no source of its own.


// Dynamic initializer (0x00717E20, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_00717ce0.cpp; no source of its own.


// Dynamic initializer (0x00717E50, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_00717ce0.cpp; no source of its own.


// Dynamic initializer (0x00717E80, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_00717ce0.cpp; no source of its own.


// Dynamic initializer (0x00717E90, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_00717ce0.cpp; no source of its own.


// Dynamic initializer (0x00717EB0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_00717ce0.cpp; no source of its own.


// The facing tables of the voxel draw (0x00717ED0-0x00719033): matrices and
// quaternions per facing and per ramp, built by Unknown_00718060 (0x00718060).
// The 8 class-type statics below are this unit's dynamic initializers
// 0x00717ED0-0x00717F80 in definition order (Matrix3D and Vector3 arrays
// have empty inline constructors: one-ret initializers).

Matrix3D ScreenSlopeMatrices[21];


Matrix3D SlopeMatrices[21];


Quaternion ScreenSlopeQuaternions[21];


Quaternion SlopeQuaternions[21];


Matrix3D IsometricViewMatrix;


Matrix3D BounceMatrices[4];


Vector3 unknown_vectors_af7930[256];


Vector3 unknown_vectors_af6768[256];


// Matrix3D __fastcall Get_Isometric_View_Matrix() -- 0x00717F90: the isometric view matrix (IsometricViewMatrix) by value
// (callers: BulletClass 0x004610D0, UnitClass 0x007005B0, AircraftClass 0x004141C0, BuildingClass 0x0043ABD0;
// the copy constructor is inline in this unit, RA2_MATRIX3D_INLINE_COPY in its first row).
Matrix3D __fastcall Get_Isometric_View_Matrix()
{
    return IsometricViewMatrix;
}


extern float VoxelLightAngle;
extern Vector3 unknown_839ab0;
extern Vector3 unknown_839a60;

void __stdcall Unknown_00717FB0(float angle)
{
    Matrix3D matrix;
    matrix.Make_Identity();
    VoxelLightAngle = angle;
    matrix.Rotate_Y(angle);
    unknown_839ab0 = matrix * Vector3(-0.70710599f, -0.70710599f, 0.0f);
    unknown_839a60 = Vector3(unknown_839ab0.X * -6.0f, 0.0f, 0.0f);
}


// void __fastcall Unknown_00718060() (0x00718060; Tiberian Sun Init_Voxel_Matrices, called by Init_Game 0x005116A0)
// Builds the per-facing and per-ramp voxel matrices and quaternions of this unit's tables. One body of 3,290 bytes
// to the ret at 0x00718D39 (the inventory row stops at 0x00718B80 inside the quaternion sequence). __fastcall: as
// __cdecl VC6 schedules the table/angle register arguments after the stack pushes (88.27).
void __fastcall Unknown_00718060()
{
    float slope_h = (float)unknown_geometry_h;
    float slope_i = (float)unknown_geometry_i;
    Vector3 diagonal1 = Matrix3D(Vector3(0.0f, 0.0f, 1.0f), 0.785375f) * Vector3(1.0f, 0.0f, 0.0f);
    Vector3 diagonal3 = Matrix3D(Vector3(0.0f, 0.0f, 1.0f), 2.3561251f) * Vector3(1.0f, 0.0f, 0.0f);
    Vector3 diagonal5 = Matrix3D(Vector3(0.0f, 0.0f, 1.0f), 3.9268751f) * Vector3(1.0f, 0.0f, 0.0f);
    Vector3 diagonal7 = Matrix3D(Vector3(0.0f, 0.0f, 1.0f), 5.4976249f) * Vector3(1.0f, 0.0f, 0.0f);
    Vector3 east(1.0f, 0.0f, 0.0f);
    Vector3 north(0.0f, 1.0f, 0.0f);
    Vector3 west(-1.0f, 0.0f, 0.0f);
    Vector3 south(0.0f, -1.0f, 0.0f);

    ScreenSlopeMatrices[0].Make_Identity();
    ScreenSlopeMatrices[1] = Matrix3D(3.9268751f, slope_i);
    ScreenSlopeMatrices[2] = Matrix3D(2.3561251f, slope_i);
    ScreenSlopeMatrices[3] = Matrix3D(0.785375f, slope_i);
    ScreenSlopeMatrices[4] = Matrix3D(5.4976249f, slope_i);
    ScreenSlopeQuaternions[1] = Axis_To_Quat(diagonal5, slope_i);
    ScreenSlopeQuaternions[2] = Axis_To_Quat(diagonal3, slope_i);
    ScreenSlopeQuaternions[3] = Axis_To_Quat(diagonal1, slope_i);
    ScreenSlopeQuaternions[4] = Axis_To_Quat(diagonal7, slope_i);
    ScreenSlopeMatrices[5] = Matrix3D(3.1415f, slope_h);
    ScreenSlopeMatrices[6] = Matrix3D(1.57075f, slope_h);
    ScreenSlopeMatrices[7] = Matrix3D(0.0f, slope_h);
    ScreenSlopeMatrices[8] = Matrix3D(4.7122502f, slope_h);
    ScreenSlopeQuaternions[5] = Axis_To_Quat(west, slope_h);
    ScreenSlopeQuaternions[6] = Axis_To_Quat(north, slope_h);
    ScreenSlopeQuaternions[7] = Axis_To_Quat(east, slope_h);
    ScreenSlopeQuaternions[8] = Axis_To_Quat(south, slope_h);
    ScreenSlopeMatrices[9] = Matrix3D(3.1415f, slope_h);
    ScreenSlopeMatrices[10] = Matrix3D(1.57075f, slope_h);
    ScreenSlopeMatrices[11] = Matrix3D(0.0f, slope_h);
    ScreenSlopeMatrices[12] = Matrix3D(4.7122502f, slope_h);
    ScreenSlopeQuaternions[9] = Axis_To_Quat(west, slope_h);
    ScreenSlopeQuaternions[10] = Axis_To_Quat(north, slope_h);
    ScreenSlopeQuaternions[11] = Axis_To_Quat(east, slope_h);
    ScreenSlopeQuaternions[12] = Axis_To_Quat(south, slope_h);
    ScreenSlopeMatrices[13] = Matrix3D(3.1415f, slope_i);
    ScreenSlopeMatrices[14] = Matrix3D(1.57075f, slope_i);
    ScreenSlopeMatrices[15] = Matrix3D(0.0f, slope_i);
    ScreenSlopeMatrices[16] = Matrix3D(4.7122502f, slope_i);
    ScreenSlopeQuaternions[13] = Axis_To_Quat(west, slope_i);
    ScreenSlopeQuaternions[14] = Axis_To_Quat(north, slope_i);
    ScreenSlopeQuaternions[15] = Axis_To_Quat(east, slope_i);
    ScreenSlopeQuaternions[16] = Axis_To_Quat(south, slope_i);
    ScreenSlopeMatrices[17].Make_Identity();
    ScreenSlopeMatrices[18].Make_Identity();
    ScreenSlopeMatrices[19].Make_Identity();
    ScreenSlopeMatrices[20].Make_Identity();
    ScreenSlopeQuaternions[17].Set(0.0f, 0.0f, 0.0f, 1.0f);
    ScreenSlopeQuaternions[18].Set(0.0f, 0.0f, 0.0f, 1.0f);
    ScreenSlopeQuaternions[19].Set(0.0f, 0.0f, 0.0f, 1.0f);
    ScreenSlopeQuaternions[20].Set(0.0f, 0.0f, 0.0f, 1.0f);

    for (int facing = 0; facing < 256; facing++) {
        Matrix3D matrix;
        matrix.Make_Identity();
        matrix.Rotate_Z(facing * 0.024542969f);
        unknown_vectors_af6768[facing] = matrix * Vector3(1.0f, 0.0f, 0.0f);
        unknown_vectors_af7930[facing] = Vector3::Cross_Product(unknown_vectors_af6768[facing], Vector3(0.0f, 0.0f, 1.0f));
        Matrix3D rotation(unknown_vectors_af7930[facing], VoxelLightAngle);
        unknown_vectors_af6768[facing] = rotation * unknown_vectors_af6768[facing];
    }

    SlopeMatrices[0].Make_Identity();
    SlopeMatrices[1] = Matrix3D(4.7122502f, slope_i);
    SlopeMatrices[2] = Matrix3D(3.1415f, slope_i);
    SlopeMatrices[3] = Matrix3D(1.57075f, slope_i);
    SlopeMatrices[4] = Matrix3D(0.0f, slope_i);
    SlopeMatrices[5] = Matrix3D(3.9268751f, slope_h);
    SlopeMatrices[6] = Matrix3D(2.3561251f, slope_h);
    SlopeMatrices[7] = Matrix3D(0.785375f, slope_h);
    SlopeMatrices[8] = Matrix3D(5.4976249f, slope_h);
    SlopeMatrices[9] = Matrix3D(3.9268751f, slope_h);
    SlopeMatrices[10] = Matrix3D(2.3561251f, slope_h);
    SlopeMatrices[11] = Matrix3D(0.785375f, slope_h);
    SlopeMatrices[12] = Matrix3D(5.4976249f, slope_h);
    SlopeMatrices[13] = Matrix3D(3.9268751f, slope_i);
    SlopeMatrices[14] = Matrix3D(2.3561251f, slope_i);
    SlopeMatrices[15] = Matrix3D(0.785375f, slope_i);
    SlopeMatrices[16] = Matrix3D(5.4976249f, slope_i);
    SlopeQuaternions[0].Set(0.0f, 0.0f, 0.0f, 1.0f);
    SlopeQuaternions[1] = Axis_To_Quat(south, slope_i);
    SlopeQuaternions[2] = Axis_To_Quat(west, slope_i);
    SlopeQuaternions[3] = Axis_To_Quat(north, slope_i);
    SlopeQuaternions[4] = Axis_To_Quat(east, slope_i);
    SlopeQuaternions[5] = Axis_To_Quat(diagonal5, slope_h);
    SlopeQuaternions[6] = Axis_To_Quat(diagonal3, slope_h);
    SlopeQuaternions[7] = Axis_To_Quat(diagonal1, slope_h);
    SlopeQuaternions[8] = Axis_To_Quat(diagonal7, slope_h);
    SlopeQuaternions[9] = Axis_To_Quat(diagonal5, slope_h);
    SlopeQuaternions[10] = Axis_To_Quat(diagonal3, slope_h);
    SlopeQuaternions[11] = Axis_To_Quat(diagonal1, slope_h);
    SlopeQuaternions[12] = Axis_To_Quat(diagonal7, slope_h);
    SlopeQuaternions[13] = Axis_To_Quat(diagonal5, slope_i);
    SlopeQuaternions[14] = Axis_To_Quat(diagonal3, slope_i);
    SlopeQuaternions[15] = Axis_To_Quat(diagonal1, slope_i);
    SlopeQuaternions[16] = Axis_To_Quat(diagonal7, slope_i);
    SlopeMatrices[17].Make_Identity();
    SlopeMatrices[18].Make_Identity();
    SlopeMatrices[19].Make_Identity();
    SlopeMatrices[20].Make_Identity();
    SlopeQuaternions[17].Set(0.0f, 0.0f, 0.0f, 1.0f);
    SlopeQuaternions[18].Set(0.0f, 0.0f, 0.0f, 1.0f);
    SlopeQuaternions[19].Set(0.0f, 0.0f, 0.0f, 1.0f);
    SlopeQuaternions[20].Set(0.0f, 0.0f, 0.0f, 1.0f);

    IsometricViewMatrix.Make_Identity();
    IsometricViewMatrix.Rotate_X((float)-unknown_geometry_angle60);
    IsometricViewMatrix.Rotate_Z((float)-unknown_geometry_angle45);

    for (int index = 0; index < 4; index++) {
        BounceMatrices[index].Make_Identity();
    }
    BounceMatrices[0].Row[1][1] = -1.0f;
    BounceMatrices[1].Row[0][0] = 0.0f;
    BounceMatrices[1].Row[0][1] = 1.0f;
    BounceMatrices[1].Row[1][0] = 1.0f;
    BounceMatrices[1].Row[1][1] = 0.0f;
    BounceMatrices[2].Row[0][0] = -1.0f;
    BounceMatrices[3].Row[0][0] = 0.0f;
    BounceMatrices[3].Row[0][1] = -1.0f;
    BounceMatrices[3].Row[1][0] = -1.0f;
    BounceMatrices[3].Row[1][1] = 0.0f;
}


void __fastcall Get_Screen_Slope_Matrix(int index, Matrix3D *matrix)
{
    *matrix = ScreenSlopeMatrices[index];
}


Matrix3D * __fastcall Get_Slope_Matrix(Matrix3D *matrix, int ramp)
{
    *matrix = SlopeMatrices[ramp];
    return matrix;
}


void __fastcall Get_Screen_Slope_Transition_Matrix(int from, int to, Matrix3D *matrix, double alpha)
{
    if (from == to) {
        *matrix = ScreenSlopeMatrices[to];
    } else {
        *matrix = Build_Matrix3D(Slerp(ScreenSlopeQuaternions[from], ScreenSlopeQuaternions[to], (float)alpha));
    }
}


Matrix3D * __fastcall Get_Slope_Transition_Matrix(Matrix3D *matrix, int from, int to, double alpha)
{
    if (from != to) {
        *matrix = Build_Matrix3D(Slerp(SlopeQuaternions[from], SlopeQuaternions[to], (float)alpha));
        return matrix;
    }
    *matrix = SlopeMatrices[to];
    return matrix;
}


// int __fastcall unknown_table_entry_7f9d9c(int index) -- 0x00718E70
// Purpose: entry `index` of the word table at 0x007F9D9C.
// Assumptions: what the table holds is unknown.

extern int SomeNumbers[];

int __fastcall unknown_table_entry_7f9d9c(int index)
{
    return SomeNumbers[index];
}


extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;
// The 256-step facing from one coordinate to another, measured from 0x20
// (the UnknownFacingDir(from, to) formula of 0x00423EF0 without its
// equal-coordinates case).
char __fastcall _Direction256(CoordStruct const &from, CoordStruct const &to)
{
    int x1 = from.X;
    int x2 = to.X;
    UnknownFacingDir dir;
    dir = UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y) - to.Y, double(x2) - x1) - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
    char facing = Dir_To_256(dir);
    facing = 0x20 - facing;
    return facing;
}


extern float ExtraLightTiltAngles[];

void __fastcall Apply_Light_Transform(Matrix3D *matrix, int index, int angle)
{
    Matrix3D rotation(unknown_vectors_af7930[index], ExtraLightTiltAngles[angle]);
    *matrix = *matrix * rotation;
    matrix->Translate(unknown_vectors_af6768[index] * -3.0f);
    matrix->Translate(0.0f, 0.0f, 3.0f);
}


// Vector3 __fastcall unknown_scaled_vector(int index, int arg2) -- 0x00718FA0
// Purpose: entry `index` of the vector table 0x00AF6768, scaled by 1.5
// (each component through a float temporary). arg2 is unused.

Vector3 __fastcall unknown_scaled_vector(int index, int arg2)
{
    Vector3 &entry = unknown_vectors_af6768[index];
    return Vector3(entry.X * 1.5f, entry.Y * 1.5f, entry.Z * 1.5f);
}


void Unknown_00719000()
{
    ObjectTypeClass::Clear_Voxel_Indexes();
}


Matrix3D * __fastcall Get_Bounce_Matrix(Matrix3D *matrix, int index)
{
    if (index >= 4) {
        index -= 4;
    }
    *matrix = BounceMatrices[index];
    return matrix;
}
