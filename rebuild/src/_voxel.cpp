// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).

#include "always.h"

#include "rect.h" // RA2: RECT_NONE, the first initializer of the unit
#include "_voxel.h"

#include "matrix3d.h"
#include "stbuffer.h"

bool UseVoxelCache;
Vector3 VoxelShadowLightVector;
float VoxelLightAngle;
Vector3 VoxelLightSource;
float VoxelCameraAngle;
Matrix3D VoxelCameraMatrix;
StaticBufferClass VoxelStaticBuffer(4000000);
