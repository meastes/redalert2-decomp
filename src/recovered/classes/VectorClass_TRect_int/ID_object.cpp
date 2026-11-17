// VectorClass<TRect<int>>::ID_object (0x0058E6C0; VectorClass<TRect<int>> vtable 0x007a5eb0 slot 4).
// Layout and behaviour: shared/vector_class.hpp. TRect<T> as TibSun rect.h declares it (fields and the member-wise
// comparison only; OpenTS, GPL-3.0-or-later with EA's terms, see THIRD_PARTY.md).
#define VECTOR_IMPLEMENT_VECTOR_ID_OBJECT
#include "shared/vector_class.hpp"

template <class T>
struct TRect {
    bool operator == (TRect<T> const & rvalue) const {return(X==rvalue.X && Y==rvalue.Y && Width==rvalue.Width && Height==rvalue.Height);}

    T X;
    T Y;
    T Width;
    T Height;
};

template class VectorClass<TRect<int> >;
