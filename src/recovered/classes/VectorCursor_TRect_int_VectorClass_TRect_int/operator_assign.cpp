#define VECTOR_CURSOR_REFERENCE_FORM
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/trect.hpp"
#include "shared/vector_cursor.hpp"
typedef VectorCursor<TRect<int>, VectorClass<TRect<int> > > RectCursor;
template<> VectorCursor<TRect<int>, VectorClass<TRect<int> > >::VectorCursor(VectorClass<TRect<int> > &vector, int index);
template<> RectCursor &VectorCursor<TRect<int>, VectorClass<TRect<int> > >::operator=(RectCursor const &other)
{
    Vector = other.Vector;
    Index = other.Index;
    return *this;
}
