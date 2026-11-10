// HRESULT DisplayClass::Load(IStream *stream) (0x0049E810, slot 30)
// Loads the five display layers.
#include "shared/display.hpp"
// @ref ?Layer@DisplayClass@@2PAVLayerClass@@A=00852578
// @ref ?Load@LayerClass@@QAEJPAUIStream@@@Z=00534a80

HRESULT DisplayClass::Load(IStream *stream)
{
    HRESULT result;
    for (int index = 0; index < 5; index++) {
        result = Layer[index].Load(stream);
    }
    return result;
}
