// control.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/control.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/gadget.hpp"


// Header-static dynamic initializer of the unit at 0x00483410: the empty
// rectangle of shared/shape_file.hpp (0x00851130), the unit's first function.


// ControlClass::ControlClass(unsigned id, int x, int y, int w, int h, unsigned flags, bool sticky) (0x00483430)

ControlClass::ControlClass(unsigned id, int x, int y, int w, int h, unsigned flags, bool sticky) :
    GadgetClass(x, y, w, h, flags, sticky),
    ID(id),
    Peer(0)
{
}


// ControlClass::ControlClass(ControlClass const &control) (0x00483480)

ControlClass::ControlClass(ControlClass const &control) :
    GadgetClass(control),
    ID(control.ID),
    Peer(control.Peer)
{
}


// int ControlClass::Action(unsigned flags, unsigned &key) (0x004834B0, slot 31)
// Reports the event as button key ID | 0x8000 (| 0x4000 for event 0x40 on a
// gadget that takes event 0x10), tells the peer, then runs GadgetClass::Action.

int ControlClass::Action(unsigned flags, unsigned &key)
{
    if (flags) {
        if (ID) {
            key = ID | KN_BUTTON;
            if ((flags & 0x40) && (Flags & 0x10)) {
                key |= 0x4000;
            }
        } else {
            key = 0;
        }
    }
    if (Peer) {
        Peer->Peer_To_Peer(flags, key, *this);
    }
    return GadgetClass::Action(flags, key);
}


// void ControlClass::Make_Peer(GadgetClass &gadget) (0x00483510, slot 33)

void ControlClass::Make_Peer(GadgetClass &gadget)
{
    Peer = &gadget;
}


// unsigned ControlClass::Get_ID() const (0x00483520, slot 17)

unsigned ControlClass::Get_ID() const
{
    return ID;
}


// int ControlClass::Draw_Me(int forced) (0x00483530, slot 27)
// Draws the peer first.

int ControlClass::Draw_Me(int forced)
{
    if (Peer) {
        Peer->Draw_Me();
    }
    return GadgetClass::Draw_Me(forced);
}


// void GadgetClass::Peer_To_Peer(unsigned flags, unsigned &key, ControlClass &whom) (0x00483560)
// Compiler-emitted from shared/gadget.hpp in this unit (inline, empty); no source of its own.


// void *ControlClass::`scalar deleting destructor'(unsigned int) (0x00483570)
// Compiler-emitted from shared/gadget.hpp in this unit; no source of its own.
