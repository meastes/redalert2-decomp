// TagClass: a live scenario tag (Tag.cpp, unit 0x006B3600; RTTI vtables
// 0x007AC700 primary, 0x007AC6E4 IRTTITypeInfo at +4, 0x007AC6DC
// INoticeSink, 0x007AC6D4 INoticeSource). Replaces the stand-in in
// object.hpp (ObjectClass keeps the count of objects carrying the tag in
// AttachCount).
//
// Layout (from the constructor 0x006B3870 and the destructor 0x006B39F0;
// member and field names from TibSun tag.h, names.csv agrees at every address):
//   +0x00  AbstractClass (0x24)
//   +0x24  TagTypeClass *Class
//   +0x28  TriggerClass *Trigger           (one per trigger type, chained)
//   +0x2C  int AttachCount                 (objects carrying the tag)
//   +0x30  Cell CellID
//   +0x34  bool IsToDie                    (queued for removal)
//   +0x35  bool IsCurrentlySprung          (springing; blocks reentry)
//   size 0x38
// Every tag is added to Tags (0x00ABFE48) and the abstract lists 0x00AC0CB0
// and 0x00AC0D48.
#ifndef RA2_SHARED_TAG_HPP
#define RA2_SHARED_TAG_HPP

#include "shared/tag_type.hpp"
#include "shared/trigger.hpp"

class ObjectClass;

extern "C" GUID const CLSID_TagClass;           // 0x007A1D30

class TagClass : public AbstractClass {
public:
    TagClass(TagTypeClass *type);                                       // 0x006B3870
    TagClass(NoInitClass const &noinit) : AbstractClass(noinit) {}
    virtual ~TagClass();                                                // 0x006B39F0

    virtual HRESULT __stdcall GetClassID(GUID *clsid);                  // 0x006B40D0 slot 3
    virtual HRESULT __stdcall Load(IStream *stream);                    // 0x006B4160 slot 5
    virtual HRESULT __stdcall Save(IStream *stream, BOOL clear_dirty);  // 0x006B41D0 slot 6
    virtual void Detach(AbstractClass *target, bool all = true);        // 0x006B4040 slot 10
    virtual int Kind_Of() const;                                        // 0x006B42D0 slot 11
    virtual int Size_Of() const;                                        // 0x006B42C0 slot 12
    virtual void Compute_CRC(WWCRCEngine &crc) const;                   // 0x006B4110 slot 13

    void Mark_To_Delete();                                              // 0x006B3CC0
    bool Is_Marked_To_Delete() const;                                   // 0x006B3D20
    Cell Get_Cell() const;                                        // 0x006B3D80
    bool Is_Cross_Vertical() const;                                     // 0x006B3D90 (Class->Is_Cross_Vertical)
    bool Is_Cross_Horizontal() const;                                   // 0x006B3DA0 (Class->Is_Cross_Horizontal)
    bool Is_Enters_Zone() const;                                        // 0x006B3DB0 (Class->Is_Enters_Zone)
    bool Is_Allow_Win() const;                                          // 0x006B3DC0 (Class->Is_Allow_Win)
    bool Is_Trigger_Attached(TriggerClass *trigger) const;              // 0x006B3DD0
    // Offers an event to the attached triggers (0x006B3DF0; TS's TagClass::Spring). source is the
    // event's originator (an object pointer the callers pass as an int).
    bool Spring(int event, ObjectClass *object, Cell cell, bool forced = false, int source = 0);
    void Set_Cell(Cell cell);                                     // 0x006B3F90
    void Timer_Global_Reset(int value);                                 // 0x006B3FD0
#ifdef RA2_TAG_UNIT_MEMBERS
    // Tag.cpp members declared for that unit only (tag.hpp is in object.hpp's closure).
    void Timer_Local_Reset(int value);                                  // 0x006B3FE0 (the 0x006ED650 reset of the chain)
    void Link(TriggerClass *trigger);                                   // 0x006B3FF0 (TibSun Link: chain head)
    bool Unlink(TriggerClass *trigger);                                 // 0x006B4000 (TibSun Unlink)
#endif
    bool Is_To_Inherit() const;                                         // 0x006B41F0
    bool Is_One_Of_A_Kind() const;                                      // 0x006B4280 (no other tag of this type)

    static TagClass *__fastcall Find_Or_Make(TagTypeClass *type);       // 0x006B3D30
    static void Clear_All();                                            // 0x006B3FA0
    static void __fastcall All_Timer_Global_Reset(int value);           // 0x006B4220
    static void __fastcall All_Timer_Local_Reset(int value);            // 0x006B4250

    TagTypeClass *Class;                    // +0x24
    TriggerClass *Trigger;                  // +0x28
    int AttachCount;                        // +0x2C
    Cell CellID;                      // +0x30
    bool IsToDie;                           // +0x34
    bool IsCurrentlySprung;                 // +0x35
};

// Every tag (0x00ABFE48; defined in this unit; TibSun Tags).
extern DynamicVectorClass<TagClass *> Tags;

#endif
