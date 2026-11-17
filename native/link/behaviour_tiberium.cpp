// Behaviour-written exit-path rows over the tiberium types (see
// behaviour_startup.cpp). NOT EXACT.
#define VECTOR_IMPLEMENT_ALL
#include "shared/vector_class.hpp"
#include "shared/tiberium.hpp"

// 0x006E9670 / 0x006EA130 (exit): every tiberium type's spread / growth queue
// emptied and freed with its data and map: the rows' seeds walk
// Tiberiums and do what TiberiumClass::Clear_Spread
// and Clear_Growth (shared/tiberium.hpp) do.
void Unknown_006E9670()
{
    for (int i = 0; i < Tiberiums.Count(); i++) {
        Tiberiums[i]->Clear_Spread();
    }
}

void __fastcall Unknown_006EA130()
{
    for (int i = 0; i < Tiberiums.Count(); i++) {
        Tiberiums[i]->Clear_Growth();
    }
}
