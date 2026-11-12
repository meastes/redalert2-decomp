// RadioClass: MissionClass with radio contact (Radio.cpp, 0x00636640-
// 0x0063702F; RTTI vtables 0x007A88E8 primary (153 slots), 0x007A88CC/
// 0x007A88C4/0x007A88BC). Layout (constructor 0x00636830): +0xBC Old, the
// last three messages (TibSun Old; cleared), +0xC8 VectorClass<TechnoClass *> RadioLinks (1 entry).
// Names from mischa85 include/radio.h; slots 149-151 are its Transmit_Message overloads (modules.json radio).
#ifndef RA2_SHARED_RADIO_HPP
#define RA2_SHARED_RADIO_HPP

#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
#include "shared/mission.hpp"

class TechnoClass;

// The message argument passed by the broadcast slots (0x00A40BE0; TibSun RadioClass::LParam).
// Radio message arguments carry objects (the destination, the caller, a cell).
extern AbstractClass *unknown_radio_param_a40be0;

class RadioClass : public MissionClass {
public:
    RadioClass();                                               // 0x00636830
    RadioClass(NoInitClass const &noinit);                      // 0x006368C0

    bool HasAnyLink() const;                                    // 0x00636F10 any radio link in use
    bool Unknown_00636EA0() const;                              // 0x00636EA0 a radio link slot is free (template HasFreeLink)
    TechnoClass *Contact_With_Whom(int index) const;            // 0x00636E20 (code identical to Contact; template Radio_Link const)
    bool Unknown_00636ED0(TechnoClass *techno) const;           // 0x00636ED0 a link is free or held by techno (template HasFreeLink)
    int FindLinkIndex(TechnoClass *techno) const;               // 0x00636E70 the techno's link index, or -1
    bool ContainsLink(TechnoClass *techno) const;               // 0x00636E30 techno is one of RadioLinks
    virtual ~RadioClass() {}

    virtual HRESULT __stdcall Load(IStream *stream);            // slot 5 0x00636C60
    virtual HRESULT __stdcall Save(IStream *stream, BOOL clear_dirty);  // slot 6 0x00636D20
    virtual void Detach(AbstractClass *target, bool all = true);    // slot 10 0x00636BA0
    virtual void Compute_CRC(WWCRCEngine &crc) const;           // slot 13 0x00636BF0
    virtual int Receive_Message(RadioClass *radio, int message, AbstractClass *&param);   // slot 95 0x00636900

    virtual bool Limbo();                               // slot 52 0x00636B60
    virtual int unknown_slot149(int message);                   // slot 149 0x00636D90 to the first contact (template Transmit_Message)
    virtual int unknown_slot150(int message, TechnoClass *techno);  // slot 150 0x00636B80 (template Transmit_Message)
    virtual int unknown_slot151(int message, AbstractClass **param, TechnoClass *techno);  // slot 151 0x00636A50 transmit (template Transmit_Message)
    virtual void SendToEachLink(int message);                   // slot 152 0x00636DC0 to every contact

    void SetLinkCount(int count);                               // 0x00636F40 grows RadioLinks, clearing new entries
    TechnoClass *Contact(int index) const;                      // 0x00636E10
#ifdef RA2_RADIO_CONTACT_WITH_WHOM
    // The first contact (TibSun radio.h Contact_With_Whom; with several links it reads link 0).
    TechnoClass *Contact_With_Whom() const { return Contact(0); }
#endif

    int Old[3];                             // +0xBC the last three messages received, newest first (TibSun Old)
    VectorClass<TechnoClass *> RadioLinks;  // +0xC8
};

#endif
