// void LayerClass::Sort() (0x00534920)
// One pass of a bubble sort by ObjectClass slot 45 (Westwood's LAYER.CPP).
#include "shared/layer.hpp"
#include "shared/object.hpp"

void LayerClass::Sort()
{
    for (int index = 0; index < Count() - 1; index++) {
        ObjectClass *next = (*this)[index + 1];
        ObjectClass *object = (*this)[index];
        int value = next->Sort_Y();
        if (value < object->Sort_Y()) {
            (*this)[index + 1] = object;
            (*this)[index] = next;
        }
    }
}
