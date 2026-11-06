// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void WaveClass::unknown_func_7238b0(CoordStruct const &source, CoordStruct const &target) -- 0x007238B0
// From ra2-decomp WaveClass::Draw_NonMagnetic (YR 0x00761640), without its YR-only
// fourth wave type; Scout801's draft for the rest.
// Purpose: places the wave between the two coordinates (shortened by the type's
// length factor) and builds its six-point outline polygon in screen pixels.
#include "shared/wave.hpp"
#include "shared/tactical.hpp"
#include "shared/matrix3d.hpp"
extern float const Wave_Length_Factors_007aec8c[3];
// @ref ?Wave_Length_Factors_007aec8c@@3QBMB=007aec8c

void WaveClass::unknown_func_7238b0(CoordStruct const &source, CoordStruct const &target)
{
    static Vector3 sizes[3][4] = {
        {Vector3(-30,-100,0),Vector3(-30,100,0),Vector3(30,-100,0),Vector3(30,100,0)},
        {Vector3(-34,-44,0),Vector3(-34,44,0),Vector3(34,-44,0),Vector3(34,44,0)},
        {Vector3(-27,-34,0),Vector3(-27,34,0),Vector3(27,-34,0),Vector3(27,34,0)}
    };
    CoordStruct start = source;
    CoordStruct end = Lerp(start,target,Wave_Length_Factors_007aec8c[Type]);
    if(Type==0)end.Z+=50;
    start=Lerp(target,start,Wave_Length_Factors_007aec8c[Type]);
    TPoint3D<float> planar_delta(start.X-end.X,start.Y-end.Y,0);
    int distance=Fast_Sqrt((double)planar_delta.Y*planar_delta.Y+(double)planar_delta.X*planar_delta.X);
    unknown_a0=start;
    unknown_slot103(unknown_a0);
    unknown_ac=end;
    Polygon_130.Count=6;
    Polygon_130.Points=&Polygon_130.InlinePoint0;
    unknown_tactical_83996c->Coord_To_Pixel(start,unknown_b8);
    unknown_tactical_83996c->Coord_To_Pixel(end,unknown_c0);
    Polygon_130.Points[1]=unknown_c0;
    Polygon_130.Points[4]=unknown_b8;
    double distance_f=distance;
    Vector3 end_left=sizes[Type][0]+Vector3(distance_f,0,0);
    Vector3 end_right=sizes[Type][1]+Vector3(distance_f,0,0);
    Vector3 start_left=sizes[Type][2]+Vector3(0,0,0);
    Vector3 start_right=sizes[Type][3]+Vector3(0,0,0);
    double dz=end.Z-start.Z;
    end_left.Z=end_left.X*dz/distance_f+start.Z;
    end_right.Z=end_right.X*dz/distance_f+start.Z;
    start_left.Z=start_left.X*dz/distance_f+start.Z;
    start_right.Z=start_right.X*dz/distance_f+start.Z;
    TPoint3D<float> heading(end.X-start.X,end.Y-start.Y,0);
    double dx=heading.X;
    double length=heading.Length();
    if(length<dx)dx=length;
    if(-dx>length)dx=-length;
    double theta=Fast_Asin(dx/length);
    if(start.Y>end.Y)theta=-theta;
    Matrix3D matrix;
    matrix.Make_Identity();
    matrix.Rotate_Z(theta);
    end_left=matrix*end_left;
    end_right=matrix*end_right;
    start_left=matrix*start_left;
    start_right=matrix*start_right;
    WaveEndLeftCoord=CoordStruct(start.X+end_left.X,start.Y+end_left.Y,end_left.Z);
    WaveEndRightCoord=CoordStruct(start.X+end_right.X,start.Y+end_right.Y,end_right.Z);
    WaveStartLeftCoord=CoordStruct(start.X+start_left.X,start.Y+start_left.Y,start_left.Z);
    WaveStartRightCoord=CoordStruct(start.X+start_right.X,start.Y+start_right.Y,start_right.Z);
    unknown_tactical_83996c->Coord_To_Pixel(WaveEndLeftCoord,unknown_c8);
    unknown_tactical_83996c->Coord_To_Pixel(WaveEndRightCoord,unknown_d0);
    unknown_tactical_83996c->Coord_To_Pixel(WaveStartLeftCoord,unknown_d8);
    unknown_tactical_83996c->Coord_To_Pixel(WaveStartRightCoord,unknown_e0);
    Polygon_130.Points[0]=unknown_c8;
    Polygon_130.Points[2]=unknown_d0;
    Polygon_130.Points[3]=unknown_e0;
    Polygon_130.Points[5]=unknown_d8;
    Facing=Owner->unknown_2f8;
}

// @ref _?$S43@?1??unknown_func_7238b0@WaveClass@@QAEXABUCoordStruct@@0@Z@4EA=00af9140

// @ref _?sizes@?1??unknown_func_7238b0@WaveClass@@QAEXABUCoordStruct@@0@Z@4PAY03UVector3@@A=00af9148

// @ref _$E44=007241f0

// @ref __real@8@3fff8000000000000000=0079a708

// @ref ??D@YI?AUVector3@@ABVMatrix3D@@ABU0@@Z=00591380
