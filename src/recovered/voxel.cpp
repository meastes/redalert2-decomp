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

Matrix3D unknown_matrices_af73a0[21];


Matrix3D unknown_matrices_af8608[21];


Quaternion unknown_quaternions_af77d8[21];


Quaternion unknown_quaternions_af6618[21];


Matrix3D unknown_matrix_af77a8;


Matrix3D unknown_matrices_af8538[4];


Vector3 unknown_vectors_af7930[256];


Vector3 unknown_vectors_af6768[256];


// Matrix3D __fastcall Unknown_00717F90() -- 0x00717F90: the isometric view matrix (unknown_matrix_af77a8) by value
// (callers: BulletClass 0x004610D0, UnitClass 0x007005B0, AircraftClass 0x004141C0, BuildingClass 0x0043ABD0;
// the copy constructor is inline in this unit, RA2_MATRIX3D_INLINE_COPY in its first row).
Matrix3D __fastcall Unknown_00717F90()
{
    return unknown_matrix_af77a8;
}


extern float unknown_839aac;
extern Vector3 unknown_839ab0;
extern Vector3 unknown_839a60;

void __stdcall Unknown_00717FB0(float angle)
{
    Matrix3D matrix;
    matrix.Make_Identity();
    unknown_839aac = angle;
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

    unknown_matrices_af73a0[0].Make_Identity();
    unknown_matrices_af73a0[1] = Matrix3D(3.9268751f, slope_i);
    unknown_matrices_af73a0[2] = Matrix3D(2.3561251f, slope_i);
    unknown_matrices_af73a0[3] = Matrix3D(0.785375f, slope_i);
    unknown_matrices_af73a0[4] = Matrix3D(5.4976249f, slope_i);
    unknown_quaternions_af77d8[1] = Axis_To_Quat(diagonal5, slope_i);
    unknown_quaternions_af77d8[2] = Axis_To_Quat(diagonal3, slope_i);
    unknown_quaternions_af77d8[3] = Axis_To_Quat(diagonal1, slope_i);
    unknown_quaternions_af77d8[4] = Axis_To_Quat(diagonal7, slope_i);
    unknown_matrices_af73a0[5] = Matrix3D(3.1415f, slope_h);
    unknown_matrices_af73a0[6] = Matrix3D(1.57075f, slope_h);
    unknown_matrices_af73a0[7] = Matrix3D(0.0f, slope_h);
    unknown_matrices_af73a0[8] = Matrix3D(4.7122502f, slope_h);
    unknown_quaternions_af77d8[5] = Axis_To_Quat(west, slope_h);
    unknown_quaternions_af77d8[6] = Axis_To_Quat(north, slope_h);
    unknown_quaternions_af77d8[7] = Axis_To_Quat(east, slope_h);
    unknown_quaternions_af77d8[8] = Axis_To_Quat(south, slope_h);
    unknown_matrices_af73a0[9] = Matrix3D(3.1415f, slope_h);
    unknown_matrices_af73a0[10] = Matrix3D(1.57075f, slope_h);
    unknown_matrices_af73a0[11] = Matrix3D(0.0f, slope_h);
    unknown_matrices_af73a0[12] = Matrix3D(4.7122502f, slope_h);
    unknown_quaternions_af77d8[9] = Axis_To_Quat(west, slope_h);
    unknown_quaternions_af77d8[10] = Axis_To_Quat(north, slope_h);
    unknown_quaternions_af77d8[11] = Axis_To_Quat(east, slope_h);
    unknown_quaternions_af77d8[12] = Axis_To_Quat(south, slope_h);
    unknown_matrices_af73a0[13] = Matrix3D(3.1415f, slope_i);
    unknown_matrices_af73a0[14] = Matrix3D(1.57075f, slope_i);
    unknown_matrices_af73a0[15] = Matrix3D(0.0f, slope_i);
    unknown_matrices_af73a0[16] = Matrix3D(4.7122502f, slope_i);
    unknown_quaternions_af77d8[13] = Axis_To_Quat(west, slope_i);
    unknown_quaternions_af77d8[14] = Axis_To_Quat(north, slope_i);
    unknown_quaternions_af77d8[15] = Axis_To_Quat(east, slope_i);
    unknown_quaternions_af77d8[16] = Axis_To_Quat(south, slope_i);
    unknown_matrices_af73a0[17].Make_Identity();
    unknown_matrices_af73a0[18].Make_Identity();
    unknown_matrices_af73a0[19].Make_Identity();
    unknown_matrices_af73a0[20].Make_Identity();
    unknown_quaternions_af77d8[17].Set(0.0f, 0.0f, 0.0f, 1.0f);
    unknown_quaternions_af77d8[18].Set(0.0f, 0.0f, 0.0f, 1.0f);
    unknown_quaternions_af77d8[19].Set(0.0f, 0.0f, 0.0f, 1.0f);
    unknown_quaternions_af77d8[20].Set(0.0f, 0.0f, 0.0f, 1.0f);

    for (int facing = 0; facing < 256; facing++) {
        Matrix3D matrix;
        matrix.Make_Identity();
        matrix.Rotate_Z(facing * 0.024542969f);
        unknown_vectors_af6768[facing] = matrix * Vector3(1.0f, 0.0f, 0.0f);
        unknown_vectors_af7930[facing] = Vector3::Cross_Product(unknown_vectors_af6768[facing], Vector3(0.0f, 0.0f, 1.0f));
        Matrix3D rotation(unknown_vectors_af7930[facing], unknown_839aac);
        unknown_vectors_af6768[facing] = rotation * unknown_vectors_af6768[facing];
    }

    unknown_matrices_af8608[0].Make_Identity();
    unknown_matrices_af8608[1] = Matrix3D(4.7122502f, slope_i);
    unknown_matrices_af8608[2] = Matrix3D(3.1415f, slope_i);
    unknown_matrices_af8608[3] = Matrix3D(1.57075f, slope_i);
    unknown_matrices_af8608[4] = Matrix3D(0.0f, slope_i);
    unknown_matrices_af8608[5] = Matrix3D(3.9268751f, slope_h);
    unknown_matrices_af8608[6] = Matrix3D(2.3561251f, slope_h);
    unknown_matrices_af8608[7] = Matrix3D(0.785375f, slope_h);
    unknown_matrices_af8608[8] = Matrix3D(5.4976249f, slope_h);
    unknown_matrices_af8608[9] = Matrix3D(3.9268751f, slope_h);
    unknown_matrices_af8608[10] = Matrix3D(2.3561251f, slope_h);
    unknown_matrices_af8608[11] = Matrix3D(0.785375f, slope_h);
    unknown_matrices_af8608[12] = Matrix3D(5.4976249f, slope_h);
    unknown_matrices_af8608[13] = Matrix3D(3.9268751f, slope_i);
    unknown_matrices_af8608[14] = Matrix3D(2.3561251f, slope_i);
    unknown_matrices_af8608[15] = Matrix3D(0.785375f, slope_i);
    unknown_matrices_af8608[16] = Matrix3D(5.4976249f, slope_i);
    unknown_quaternions_af6618[0].Set(0.0f, 0.0f, 0.0f, 1.0f);
    unknown_quaternions_af6618[1] = Axis_To_Quat(south, slope_i);
    unknown_quaternions_af6618[2] = Axis_To_Quat(west, slope_i);
    unknown_quaternions_af6618[3] = Axis_To_Quat(north, slope_i);
    unknown_quaternions_af6618[4] = Axis_To_Quat(east, slope_i);
    unknown_quaternions_af6618[5] = Axis_To_Quat(diagonal5, slope_h);
    unknown_quaternions_af6618[6] = Axis_To_Quat(diagonal3, slope_h);
    unknown_quaternions_af6618[7] = Axis_To_Quat(diagonal1, slope_h);
    unknown_quaternions_af6618[8] = Axis_To_Quat(diagonal7, slope_h);
    unknown_quaternions_af6618[9] = Axis_To_Quat(diagonal5, slope_h);
    unknown_quaternions_af6618[10] = Axis_To_Quat(diagonal3, slope_h);
    unknown_quaternions_af6618[11] = Axis_To_Quat(diagonal1, slope_h);
    unknown_quaternions_af6618[12] = Axis_To_Quat(diagonal7, slope_h);
    unknown_quaternions_af6618[13] = Axis_To_Quat(diagonal5, slope_i);
    unknown_quaternions_af6618[14] = Axis_To_Quat(diagonal3, slope_i);
    unknown_quaternions_af6618[15] = Axis_To_Quat(diagonal1, slope_i);
    unknown_quaternions_af6618[16] = Axis_To_Quat(diagonal7, slope_i);
    unknown_matrices_af8608[17].Make_Identity();
    unknown_matrices_af8608[18].Make_Identity();
    unknown_matrices_af8608[19].Make_Identity();
    unknown_matrices_af8608[20].Make_Identity();
    unknown_quaternions_af6618[17].Set(0.0f, 0.0f, 0.0f, 1.0f);
    unknown_quaternions_af6618[18].Set(0.0f, 0.0f, 0.0f, 1.0f);
    unknown_quaternions_af6618[19].Set(0.0f, 0.0f, 0.0f, 1.0f);
    unknown_quaternions_af6618[20].Set(0.0f, 0.0f, 0.0f, 1.0f);

    unknown_matrix_af77a8.Make_Identity();
    unknown_matrix_af77a8.Rotate_X((float)-unknown_geometry_angle60);
    unknown_matrix_af77a8.Rotate_Z((float)-unknown_geometry_angle45);

    for (int index = 0; index < 4; index++) {
        unknown_matrices_af8538[index].Make_Identity();
    }
    unknown_matrices_af8538[0].Row[1][1] = -1.0f;
    unknown_matrices_af8538[1].Row[0][0] = 0.0f;
    unknown_matrices_af8538[1].Row[0][1] = 1.0f;
    unknown_matrices_af8538[1].Row[1][0] = 1.0f;
    unknown_matrices_af8538[1].Row[1][1] = 0.0f;
    unknown_matrices_af8538[2].Row[0][0] = -1.0f;
    unknown_matrices_af8538[3].Row[0][0] = 0.0f;
    unknown_matrices_af8538[3].Row[0][1] = -1.0f;
    unknown_matrices_af8538[3].Row[1][0] = -1.0f;
    unknown_matrices_af8538[3].Row[1][1] = 0.0f;
}


void __fastcall Unknown_00718D40(int index, Matrix3D *matrix)
{
    *matrix = unknown_matrices_af73a0[index];
}


Matrix3D * __fastcall Unknown_00718D60(Matrix3D *matrix, int ramp)
{
    *matrix = unknown_matrices_af8608[ramp];
    return matrix;
}


void __fastcall Unknown_00718D80(int from, int to, Matrix3D *matrix, double alpha)
{
    if (from == to) {
        *matrix = unknown_matrices_af73a0[to];
    } else {
        *matrix = Build_Matrix3D(Slerp(unknown_quaternions_af77d8[from], unknown_quaternions_af77d8[to], (float)alpha));
    }
}


Matrix3D * __fastcall Unknown_00718DF0(Matrix3D *matrix, int from, int to, double alpha)
{
    if (from != to) {
        *matrix = Build_Matrix3D(Slerp(unknown_quaternions_af6618[from], unknown_quaternions_af6618[to], (float)alpha));
        return matrix;
    }
    *matrix = unknown_matrices_af8608[to];
    return matrix;
}


// int __fastcall unknown_table_entry_7f9d9c(int index) -- 0x00718E70
// Purpose: entry `index` of the word table at 0x007F9D9C.
// Assumptions: what the table holds is unknown.

extern int unknown_table_7f9d9c[];

int __fastcall unknown_table_entry_7f9d9c(int index)
{
    return unknown_table_7f9d9c[index];
}


extern double const unknown_facing_scale_79b628;
extern double const unknown_facing_origin_79b630;
// The 256-step facing from one coordinate to another, measured from 0x20
// (the UnknownFacingDir(from, to) formula of 0x00423EF0 without its
// equal-coordinates case).
char __fastcall Unknown_00718E80(CoordStruct const &from, CoordStruct const &to)
{
    int x1 = from.X;
    int x2 = to.X;
    UnknownFacingDir dir;
    dir = UnknownFacingDir((unsigned short)(int)((Fast_Atan2(double(from.Y) - to.Y, double(x2) - x1) - unknown_facing_origin_79b630) * unknown_facing_scale_79b628));
    char facing = Dir_To_256(dir);
    facing = 0x20 - facing;
    return facing;
}


extern float unknown_angles_7f9d84[];

void __fastcall Unknown_00718EE0(Matrix3D *matrix, int index, int angle)
{
    Matrix3D rotation(unknown_vectors_af7930[index], unknown_angles_7f9d84[angle]);
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


Matrix3D * __fastcall Unknown_00719010(Matrix3D *matrix, int index)
{
    if (index >= 4) {
        index -= 4;
    }
    *matrix = unknown_matrices_af8538[index];
    return matrix;
}
