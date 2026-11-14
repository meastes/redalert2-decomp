// toggle.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/toggle.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/gadget.hpp"


// Header-static dynamic initializer of the unit at 0x006EB100: the empty
// rectangle of shared/shape_file.hpp (0x00AC0BD8), the unit's first function.


// ToggleClass::ToggleClass(unsigned id, int x, int y, int w, int h) (0x006EB120)
// A sticky, released, switched-off button.

ToggleClass::ToggleClass(unsigned id, int x, int y, int w, int h) :
    ControlClass(id, x, y, w, h, LEFTPRESS | LEFTRELEASE, true),
    IsPressed(false),
    IsOn(false),
    ToggleType(0)
{
}


// void ToggleClass::Turn_On() (0x006EB160)

void ToggleClass::Turn_On()
{
    IsOn = true;
    Flag_To_Redraw();
}


// void ToggleClass::Turn_Off() (0x006EB170)

void ToggleClass::Turn_Off()
{
    IsOn = false;
    Flag_To_Redraw();
}
