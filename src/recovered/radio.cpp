// Westwood radio.cpp in one source (twins: mischa85/RedAlert2 src/radio.cpp on this target, TibSun radio.cpp):
// the accepted bodies in the unit's order. Names follow the template where taking them changes no other
// unit; the rest are recorded in modules.json radio.

#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/radio.hpp"
#include "shared/techno.hpp"


// The unit's first functions (0x00636640-0x00636810) are the dynamic initializers of the
// shared/unknown_geometry.hpp constants and of shared/unknown_static_rect.hpp's rectangle,
// emitted by the includes above.


// RadioClass::RadioClass() (0x00636830): no contact (one empty radio link)
// and no messages remembered.

RadioClass::RadioClass() :
    RadioLinks(1)
{
    RadioLinks[0] = 0;
    for (int i = 0; i < 3; i++) {
        Old[i] = 0;
    }
}


// RadioClass::RadioClass(NoInitClass const &noinit) (0x006368C0)

RadioClass::RadioClass(NoInitClass const &noinit) :
    MissionClass(noinit),
    RadioLinks(noinit)
{
}


// int RadioClass::Receive_Message(RadioClass *radio, int message, AbstractClass *&param) (0x00636900, slot 95)
// Receives a radio message: remembers the last three message kinds in
// Old; message 3 hangs up a contact and message 2 accepts one (0xA
// when the other house is no ally or no link is free); anything else is
// ObjectClass's.

int RadioClass::Receive_Message(RadioClass *radio, int message, AbstractClass *&param)
{
    if (message != Old[0]) {
        Old[2] = Old[1];
        Old[1] = Old[0];
        Old[0] = message;
    }
    if (message == 3) {
        for (int index = 0; index < RadioLinks.Length(); index++) {
            if (radio == RadioLinks[index]) {
                MissionClass::Receive_Message(radio, 3, param);
                RadioLinks[index] = 0;
                return 1;
            }
        }
    } else if (message == 2 && Strength != 0) {
        if (!((TechnoClass *)radio)->House->Is_Ally((TechnoClass *)this)) {
            return 0xA;
        }
        if (this != 0 && AbstractFlags.IsTechno && !((TechnoClass *)this)->House->Is_Ally((TechnoClass *)radio)) {
            return 0xA;
        }
        if (radio != 0 && FindLinkIndex((TechnoClass *)radio) != -1) {
            return 1;
        }
        for (int index = 0; index < RadioLinks.Length(); index++) {
            if (RadioLinks[index] == 0) {
                RadioLinks[index] = (TechnoClass *)radio;
                return 1;
            }
        }
        return 0xA;
    }
    return MissionClass::Receive_Message(radio, message, param);
}


// int RadioClass::unknown_slot151(int message, AbstractClass **param, TechnoClass *to) (0x00636A50, slot 151)
// Sends a radio message (3 hangs up, 2 makes contact) to a recipient
// (the first contact when none is given) through the recipient's slot 95.
// Contact needs a free link: when all are taken the first is hung up.
// 0xA is the answer when the recipient does not accept.

int RadioClass::unknown_slot151(int message, AbstractClass **param, TechnoClass *to)
{
    if (to == 0) {
        to = RadioLinks[0];
        if (to == 0) {
            return 0;
        }
    }
    if (message == 3) {
        for (int index = 0; index < RadioLinks.Length(); index++) {
            if (RadioLinks[index] == to) {
                RadioLinks[index] = 0;
            }
        }
    } else if (message == 2) {
        int free_link = -1;
        for (int index = 0; index < RadioLinks.Length(); index++) {
            if (free_link == -1 && RadioLinks[index] == 0) {
                free_link = index;
            }
            if (RadioLinks[index] == to) {
                return 1;
            }
        }
        if (free_link == -1) {
            unknown_slot150(3, RadioLinks[0]);
            free_link = 0;
        }
        if (to->Receive_Message(As_Techno(), 2, *param) == 1) {
            RadioLinks[free_link] = to;
            return 1;
        }
        return 0xA;
    }
    return to->Receive_Message(As_Techno(), message, *param);
}


// bool RadioClass::Limbo() (0x00636B60, slot 52)
// Sends message 3 to every contact first unless unknown_76 is set.

bool RadioClass::Limbo()
{
    if (!unknown_76) {
        SendToEachLink(3);
    }
    return ObjectClass::Limbo();
}


// int RadioClass::unknown_slot150(int message, TechnoClass *techno) (0x00636B80, slot 150)

int RadioClass::unknown_slot150(int message, TechnoClass *techno)
{
    return unknown_slot151(message, &unknown_radio_param_a40be0, techno);
}


// void RadioClass::Detach(AbstractClass *target, bool all) (0x00636BA0, slot 10)
// MissionClass's part, then forgets the target among the radio contacts
// (only when all).

void RadioClass::Detach(AbstractClass *target, bool all)
{
    MissionClass::Detach(target, all);
    for (int index = 0; index < RadioLinks.Length(); index++) {
        if (RadioLinks[index] == (TechnoClass *)target && all) {
            RadioLinks[index] = 0;
        }
    }
}


// void RadioClass::Compute_CRC(WWCRCEngine &crc) const (0x00636BF0, slot 13)
// MissionClass's part, the contact count and each contact's ID and kind.

void RadioClass::Compute_CRC(WWCRCEngine &crc) const
{
    MissionClass::Compute_CRC(crc);
    crc((long)RadioLinks.Length());
    for (int index = 0; index < RadioLinks.Length(); index++) {
        if (RadioLinks[index] != 0) {
            crc((long)RadioLinks[index]->Fetch_ID());
            crc((long)RadioLinks[index]->Kind_Of());
        }
    }
}


// HRESULT __stdcall RadioClass::Load(IStream *stream) (0x00636C60, slot 5)
// MissionClass's part, then the contact vector: rebuilt empty, resized to
// the saved count and filled with the saved pointers, which are swizzled.

HRESULT __stdcall RadioClass::Load(IStream *stream)
{
    HRESULT result = MissionClass::Load(stream);
    if (FAILED(result)) {
        return result;
    }
    new (&RadioLinks) VectorClass<TechnoClass *>;
    int count;
    result = stream->Read(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    RadioLinks.Resize(count);
    TechnoClass *link;
    for (int index = 0; index < count; index++) {
        result = stream->Read(&link, sizeof(link), 0);
        if (FAILED(result)) {
            return result;
        }
        RadioLinks[index] = link;
    }
    for (int fix = 0; fix < count; fix++) {
        Swizzler.Swizzle((void **)&RadioLinks[fix]);
    }
    return S_OK;
}


// HRESULT __stdcall RadioClass::Save(IStream *stream, BOOL clear_dirty) (0x00636D20, slot 6)
// MissionClass's part, then the contact vector's size and its pointers.

HRESULT __stdcall RadioClass::Save(IStream *stream, BOOL clear_dirty)
{
    HRESULT result = MissionClass::Save(stream, clear_dirty);
    if (FAILED(result)) {
        return result;
    }
    int count = RadioLinks.Length();
    result = stream->Write(&count, sizeof(count), 0);
    if (FAILED(result)) {
        return result;
    }
    for (int index = 0; index < count; index++) {
        result = stream->Write(&RadioLinks[index], sizeof(RadioLinks[index]), 0);
        if (FAILED(result)) {
            return result;
        }
    }
    return S_OK;
}


// int RadioClass::unknown_slot149(int message) (0x00636D90, slot 149): to the first contact, if any.

int RadioClass::unknown_slot149(int message)
{
    if (RadioLinks[0] != 0) {
        return unknown_slot151(message, &unknown_radio_param_a40be0, RadioLinks[0]);
    }
    return 0;
}


// void RadioClass::SendToEachLink(int message) (0x00636DC0, slot 152): to every contact.

void RadioClass::SendToEachLink(int message)
{
    for (int index = 0; index < RadioLinks.Length(); index++) {
        if (RadioLinks[index] != 0) {
            unknown_slot151(message, &unknown_radio_param_a40be0, RadioLinks[index]);
        }
    }
}


// TechnoClass *RadioClass::Contact(int index) const (0x00636E10): the radio link.

TechnoClass *RadioClass::Contact(int index) const
{
    return RadioLinks[index];
}


// TechnoClass *RadioClass::Contact_With_Whom(int index) const (0x00636E20): the radio link.

TechnoClass *RadioClass::Contact_With_Whom(int index) const
{
    return RadioLinks[index];
}


bool RadioClass::ContainsLink(TechnoClass *techno) const {
    return FindLinkIndex(techno) != -1;
}


int RadioClass::FindLinkIndex(TechnoClass *techno) const {
    if (techno) {
        for (int index = 0; index < RadioLinks.Length(); ++index) {
            if (RadioLinks[index] == techno) return index;
        }
    }
    return -1;
}


// bool RadioClass::Unknown_00636EA0() const (0x00636EA0): whether a radio link slot is free (mischa85 HasFreeLink).
bool RadioClass::Unknown_00636EA0() const {
    for (int index = 0; index < RadioLinks.Length(); ++index) {
        if (!RadioLinks[index]) return true;
    }
    return false;
}


bool RadioClass::Unknown_00636ED0(TechnoClass *techno) const {
    for (int index = 0; index < RadioLinks.Length(); ++index) {
        TechnoClass *contact = RadioLinks[index];
        if (!contact || contact == techno) return true;
    }
    return false;
}


bool RadioClass::HasAnyLink() const {
    for (int index = 0; index < RadioLinks.Length(); ++index) {
        if (RadioLinks[index]) return true;
    }
    return false;
}


void RadioClass::SetLinkCount(int count) {
    int previous = RadioLinks.Length();
    if (count > previous) {
        RadioLinks.Resize(count);
        for (int index = previous; index < count; ++index) RadioLinks[index] = 0;
    }
}


// RadioClass scalar deleting destructor at00636F90, emitted from the
// existing inline virtual destructor in shared/radio.hpp. This unit already
// requires it through the Radio noinit constructor/vtable; no extra caller.
// Primary original Radio RTTI vtable slot8; body extent verified separately.
