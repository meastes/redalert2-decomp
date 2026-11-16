// Cell tube lookup: typed existing ECX fastcall ABI; member/free spelling remains a hypothesis.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/map_cell.hpp"
#include "shared/vector_class.hpp"
class TubeClass;
extern DynamicVectorClass<TubeClass *> Tubes;
// @ref ?Tubes@@=008661c8
TubeClass *__fastcall Cell_Tube_0047a130(CellClass const *cell)
{
    short index=cell->Tube;
    if (index>=0 && index<Tubes.Count()) return Tubes[index];
    return 0;
}
