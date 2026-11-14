// pixelfx.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/pixelfx.json carries the addresses and the rows that
// stay in their own files.


#include "shared/pixel_fx.hpp"
#include "shared/crt_memory.hpp"


// void PixelFXClass::Init(int type) (0x0060EB20)
// Picks the colours of the given type (Bright darkened by random jitter bits),
// a random screen offset, blend rate and lifetime; restarts the phase.

void PixelFXClass::Init(int type)
{
    PixelFXTypeStruct *fx = &PixelFXTypes[type];
    unsigned random = rand();
    XOffset = (random & 0x3F) - 0x1F;
    YOffset = ((random >> 5) & 0x1F) - 0x0F;

    Bright.Red = fx->Bright.Red;
    Bright.Green = fx->Bright.Green;
    Bright.Blue = fx->Bright.Blue;
    if (fx->JitterBits != 0) {
        unsigned mask = (1 << fx->JitterBits) - 1;
        random = rand();
        Bright.Red -= mask & random;
        random >>= fx->JitterBits;
        Bright.Green -= mask & random;
        random >>= fx->JitterBits;
        Bright.Blue -= mask & random;
    }

    Current.Red = Dark.Red = fx->Dark.Red;
    Current.Green = Dark.Green = fx->Dark.Green;
    Current.Blue = Dark.Blue = fx->Dark.Blue;
    Phase = 0;
    unsigned range = fx->MaxRate - fx->MinRate + 1;
    Rate = rand() % range + fx->MinRate;
    Life = rand() & fx->LifeMask;
}


// PixelFXClass::PixelFXClass() (0x0060EBF0)
// A sparkle of the first type.

PixelFXClass::PixelFXClass()
{
    Init(0);
}


// PixelFXClass::PixelFXClass(int type) (0x0060EC10)
// A sparkle of the given type.

PixelFXClass::PixelFXClass(int type)
{
    Init(type);
}


// void PixelFXClass::Update(int frames) (0x0060EC30)
// Advances the phase and blends the current colour: Dark at phase 0 and
// 0x2000, Bright at 0x1000 (12-bit fixed point). The phase stops at 0x2000.

void PixelFXClass::Update(int frames)
{
    Phase += Rate * frames;
    if (Phase > 0x2000) {
        Phase = 0x2000;
    }

    int blend = Phase & 0xFFF;
    if (Phase & 0x1000) {
        blend = 0x1000 - blend;
    }

    Current.Red = (Dark.Red * (0x1000 - blend) + Bright.Red * blend) >> 12;
    Current.Green = (Dark.Green * (0x1000 - blend) + Bright.Green * blend) >> 12;
    Current.Blue = (Dark.Blue * (0x1000 - blend) + Bright.Blue * blend) >> 12;
}


// bool PixelFXClass::Reduce_Life(int frames) (0x0060ECC0)
// Counts the lifetime down; true (and clamped to 0) once it has run out.

bool PixelFXClass::Reduce_Life(int frames)
{
    Life -= frames;
    if (Life <= 0) {
        Life = 0;
        return true;
    }
    return false;
}


// void PixelFXClass::Set_Offset(int x, int y) (0x0060ECF0)
// Places the sparkle relative to its owner.

void PixelFXClass::Set_Offset(int x, int y)
{
    XOffset = x;
    YOffset = y;
}


// PixelFXClass::~PixelFXClass() (0x0060ED10)
// Nothing to release.

PixelFXClass::~PixelFXClass()
{
}


// PixelFXClass::`scalar deleting destructor'(unsigned int) (0x0060ED20)
// Compiler-generated from the virtual destructor; emitted with the vtable.
