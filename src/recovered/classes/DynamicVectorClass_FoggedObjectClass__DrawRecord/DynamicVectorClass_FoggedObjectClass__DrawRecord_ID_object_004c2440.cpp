// DynamicVectorClass<FoggedObjectClass::DrawRecord>::ID_object (0x004C2440; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp. DrawRecord as shared/fogged_object.hpp declares it (that header's
// statics would add an initializer to this instance unit); the comparison is TibSun fog.h's (type, then frame).
#define VECTOR_IMPLEMENT_DYNAMIC_ID_OBJECT
#include "shared/vector_class.hpp"

class ObjectTypeClass;

class FoggedObjectClass {
public:
    struct DrawRecord {
        bool operator==(DrawRecord const &that) const
        {
            return Type == that.Type && unknown_04 == that.unknown_04;
        }
        ObjectTypeClass *Type;    // +0x00
        int unknown_04;
        unsigned char unknown_08;
        int unknown_0c;
    };
};

template class DynamicVectorClass<FoggedObjectClass::DrawRecord>;
