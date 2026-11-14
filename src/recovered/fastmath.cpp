// fastmath.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/fastmath.json carries the addresses and the rows that
// stay in their own files.


#include "shared/fast_math.hpp"
#include "shared/crt_memory.hpp"


// float Fast_Sqrt(double value) (0x004BA410)
// The double face of the table square root (TibSun fastmath::sqrt): no more
// precision than the float function it expands.

float __cdecl Fast_Sqrt(double value)
{
    return (float)Fast_Sqrt_Float_004BA830((float)value);
}


float __cdecl Fast_Asin_004ba550(double value)
{
    float number = (float)value;
    int index = (int)((number + 1.0f) * -2048.0f);
    return unknown_asin_table_80bdd4[-index];
}


float __cdecl Fast_Acos_004ba580(double value)
{
    float number = (float)value;
    int index = (int)((number + 1.0f) * -2048.0f);
    return (float)(1.5707963267948966 - unknown_asin_table_80bdd4[-index]);
}


double __cdecl Fast_Sin(double angle)
{
    float value = (float)angle;
    int doubled = (int)(value * unknown_sin_scale_7d8048);
    bool round_up = (doubled & 1) != 0;
    int index = (doubled / 2) % 8192;
    if (index < 0) index += 8192;
    if (round_up) {
        if (index < 8191) ++index;
    }
    double sample=unknown_sin_table_801dc4[index];
    // Preserve the original single-precision result store before returning it.
    return (float)sample;
}


double __cdecl Fast_Cos(double angle)
{
    float value = (float)angle;
    int doubled = (int)(value * unknown_cos_scale_7d804c);
    bool round_up = (doubled & 1) != 0;
    int index = (doubled / 2) % 8192;
    if (index < 0) index += 10240;
    else index += 2048;
    if (round_up) {
        if (index < 10239) ++index;
    }
    double sample=unknown_sin_table_801dc4[index];
    // Preserve the original single-precision result store before returning it.
    return (float)sample;
}


float __cdecl Fast_Sin_4096(double angle)
{
    int index=(int)(angle*651.8986469044033)%4096;
    if(index<0) index+=4096;
    double sample=unknown_sin_table_80fde4[index];
    return sample;
}


// ?Fast_Atan@@YAMN@Z (0x004BA5B0)
// Arctangent from the 0x1001-entry table (pi/2 beyond it), sign from value.

float __cdecl Fast_Atan(double value)
{
    float number = (float)value;
    int index = abs((int)(number / unknown_atan_step_817df8));
    float result;
    if (index < 0x1001) {
        result = unknown_atan_table_813df4[index];
    } else {
        result = 1.5707964f;
    }
    if (number < 0.0f) {
        result = -result;
    }
    return result;
}


// ?Fast_Atan2@@YAMNN@Z (0x004BA600)
// Arctangent of y / x over the full circle (0 for the origin).

float __cdecl Fast_Atan2(double y, double x)
{
    float fx = (float)x;
    float fy = (float)y;
    if (fx == 0.0f) {
        if (fy == 0.0f) {
            return 0.0f;
        }
        if (fy > 0.0f) {
            return 1.5707964f;
        }
        return -1.5707964f;
    }
    int index = abs((int)(fy / fx / unknown_atan_step_817df8));
    float result;
    if (index < 0x1001) {
        result = unknown_atan_table_813df4[index];
    } else {
        result = 1.5707964f;
    }
    if (fx < 0.0f) {
        result = (float)(3.141592653589793 - result);
    }
    if (fy < 0.0f) {
        result = -result;
    }
    return result;
}


// 0x004ba6c0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void fastmath_blank_1_004ba6c0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004ba6c0 {

void  fastmath_blank_1_004ba6c0(void)
{
    return;
}

}  // namespace seed_004ba6c0


// 0x004ba6d0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void fastmath_blank_3_004ba6d0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004ba6d0 {

void  fastmath_blank_3_004ba6d0(void)
{
    return;
}

}  // namespace seed_004ba6d0


// 0x004ba6e0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void fastmath_blank_2_004ba6e0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004ba6e0 {

void  fastmath_blank_2_004ba6e0(void)
{
    return;
}

}  // namespace seed_004ba6e0


// void Init_Sqrt_Table_004ba6f0() (0x004BA6F0)
// Builds the square-root mantissa table 0x00817DFC (TibSun fastmath.cpp
// init_sqrt_tab, the Graphics Gems III two-half build loop retargeted to
// float): the first half for an even exponent, the second for an odd one.


void Init_Sqrt_Table_004ba6f0()
{
    short i;
    float value;
    unsigned int *fi = (unsigned int *)&value;

    for (i = 0; i != 0x2000; i++) {
        value = 0;
        *fi = (i << 10) | (127 << 23);
        value = Fast_Sqrt(value);
        unknown_sqrt_table_817dfc[i] = *fi & 0x7fffff;

        value = 0;
        *fi = (i << 10) | (128 << 23);
        value = Fast_Sqrt(value);
        unknown_sqrt_table_817dfc[i + 0x2000] = *fi & 0x7fffff;
    }
}


// The callee pops one 4-byte float argument. Original C++ declaration
// and non-stack calling-convention details remain unresolved.
union RootFloatBits { float Value; unsigned int Bits; };
float __stdcall Fast_Sqrt_Float_004BA830(float value)
{
    if (value == 0.0f) return 0.0f;
    float absolute = value <= 0.0f ? value * -1.0 : value;
    RootFloatBits &number = *(RootFloatBits *)&absolute;
    short exponent = (short)((number.Bits >> 23) - 127);
    unsigned int mantissa = number.Bits & 0x7fffff;
    if (exponent & 1) mantissa |= 0x800000;
    exponent >>= 1;
    number.Bits = (exponent + 127) * 0x800000 + unknown_sqrt_table_817dfc[mantissa >> 10];
    return absolute;
}


float __fastcall Fast_Sin_Float_004ba920(float angle);
float __fastcall Fast_Asin_Float_004baa30(float value);
extern double const unknown_sine_step_007a0f98;
extern double const unknown_asin_step_007a0fa0;
void __cdecl Remap_Sine_Table_004ba8b0()
{
 for(int i=0;i!=10241;++i) unknown_sin_table_801dc4[i]=Fast_Sin_Float_004ba920((float)(i*unknown_sine_step_007a0f98));
}


inline int Nearest_Trig_Index(int doubled_index, int quarter_offset)
{
    bool round_up = (doubled_index & 1) != 0;
    int index = (doubled_index / 2) % 8192;
    if(index<0) index+=8192+quarter_offset;
    else index+=quarter_offset;
    if (round_up) {
        if (index < 8191 + quarter_offset) index++;
    }
    return index;
}

float __fastcall Fast_Sin_Float_004ba920(float angle)
{
    return unknown_sin_table_801dc4[Nearest_Trig_Index((int)(angle * unknown_sin_scale_7d8048), 0)];
}


float __fastcall Fast_Cos_Float_004ba970(float angle)
{
    return unknown_sin_table_801dc4[Nearest_Trig_Index((int)(angle * unknown_cos_scale_7d804c), 2048)];
}


void __cdecl Remap_Asin_Table_004ba9d0()
{
 for(int i=0;i!=4097;++i) unknown_asin_table_80bdd4[i]=Fast_Asin_Float_004baa30((float)(i*unknown_asin_step_007a0fa0-1.0));
}


float __fastcall Fast_Asin_Float_004baa30(float value)
{
 int index=(int)((value+1.0f)*-2048.0f);
 return unknown_asin_table_80bdd4[-index];
}


float __fastcall Fast_Acos_Float_004baa60(float value)
{
 int index=(int)((value+1.0f)*-2048.0f);
 return (float)(1.5707963267948966-unknown_asin_table_80bdd4[-index]);
}


extern double unknown_sin4096_scale_007a0f80;
extern double const unknown_sin4096_step_007a0fa8;
float __fastcall Fast_Sin4096_Float_004baaf0(float angle);
void __cdecl Remap_Sin4096_Table_004baa90()
{
 for(int i=0;i!=4097;++i) unknown_sin_table_80fde4[i]=Fast_Sin4096_Float_004baaf0((float)(i*unknown_sin4096_step_007a0fa8));
}


float __fastcall Fast_Sin4096_Float_004baaf0(float angle)
{
 int index=(int)(angle*unknown_sin4096_scale_007a0f80)%4096;
 if(index<0) index+=4096;
 return unknown_sin_table_80fde4[index];
}


float __fastcall Fast_Atan_Float_004bac50(float value);
extern float const unknown_atan_sample_00810dc8;
float __cdecl Remap_Atan_Table_004bab30()
{
 float limit=unknown_atan_sample_00810dc8;
 unknown_atan_step_817df8=limit*0.000244140625f;
 float value=0.0f;
 for(int i=0;i!=4097;++i) {
  unknown_atan_table_813df4[i]=Fast_Atan_Float_004bac50(value);
  value+=unknown_atan_step_817df8;
 }
 return limit;
}


float __fastcall Fast_Atan2_Float_004baba0(float y,float x)
{
    float fx = (float)x;
    float fy = (float)y;
    if (fx == 0.0f) {
        if (fy == 0.0f) {
            return 0.0f;
        }
        if (fy > 0.0f) {
            return 1.5707964f;
        }
        return -1.5707964f;
    }
    int index = abs((int)(fy / fx / unknown_atan_step_817df8));
    float result;
    if (index < 0x1001) {
        result = unknown_atan_table_813df4[index];
    } else {
        result = 1.5707964f;
    }
    if (fx < 0.0f) {
        result = (float)(3.141592653589793 - result);
    }
    if (fy < 0.0f) {
        result = -result;
    }
    return result;
}


float __fastcall Fast_Atan_Float_004bac50(float value)
{
    float number = (float)value;
    int index = abs((int)(number / unknown_atan_step_817df8));
    float result;
    if (index < 0x1001) {
        result = unknown_atan_table_813df4[index];
    } else {
        result = 1.5707964f;
    }
    if (number < 0.0f) {
        result = -result;
    }
    return result;
}
