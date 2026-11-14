// abstype.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/abstype.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/abstract_type.hpp"


// Header-static dynamic initializers of the unit at 0x004103B0:  the
// shared/unknown_geometry.hpp constants (0x004103B0-0x00410571); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x004103E0, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_004103b0.cpp; no source of its own.


// Dynamic initializer (0x00410400, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_004103b0.cpp; no source of its own.


// Dynamic initializer (0x00410420, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_004103b0.cpp; no source of its own.


// Dynamic initializer (0x00410440, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_004103b0.cpp; no source of its own.


// Dynamic initializer (0x00410460, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_004103b0.cpp; no source of its own.


// Dynamic initializer (0x00410480, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_004103b0.cpp; no source of its own.


// Dynamic initializer (0x004104C0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_004103b0.cpp; no source of its own.


// Dynamic initializer (0x004104F0, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_004103b0.cpp; no source of its own.


// Dynamic initializer (0x00410520, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_004103b0.cpp; no source of its own.


// Dynamic initializer (0x00410550, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_004103b0.cpp; no source of its own.


// Dynamic initializer (0x00410560, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_004103b0.cpp; no source of its own.


// AbstractTypeClass::AbstractTypeClass(char const *ini_name) (0x00410580)
// A type named ini_name (or its own address in hex); Name starts as the ID; registers it in AbstractTypes.

AbstractTypeClass::AbstractTypeClass(char const *ini_name)
{
    if (ini_name == 0) {
        char buffer[16];
        sprintf(buffer, "%08X", this);
        ID = buffer;
    } else {
        ID = ini_name;
    }
    Name = ID;
    UIName = unknown_empty_wide_839d4c;
    AbstractTypes.Add(this);
}


// AbstractTypeClass::AbstractTypeClass(NoInitClass const &noinit) (0x004106E0)
// Load path: keeps the loaded fields and refetches the UIName text.

AbstractTypeClass::AbstractTypeClass(NoInitClass const &noinit) :
    AbstractClass(noinit),
    ID(noinit),
    UINameLabel(noinit),
    Name(noinit)
{
    if (UINameLabel[0] != 0) {
        UIName = StringTable::Load_String(UINameLabel, 0, "D:\\Ra2\\AbsType.cpp", 127);
    } else {
        UIName = unknown_empty_wide_839d4c;
    }
}


// AbstractTypeClass::~AbstractTypeClass() (0x00410740)
// Removes the type from AbstractTypes.

AbstractTypeClass::~AbstractTypeClass()
{
    AbstractTypes.Delete(this);
}


// bool AbstractTypeClass::operator==(char const *ini_name) const (0x004107C0)
// Whether the ID matches ini_name, ignoring case.

bool AbstractTypeClass::operator==(char const *ini_name) const
{
    return _stricmp(ini_name, ID) == 0;
}


// bool AbstractTypeClass::Read_INI(CCINIClass &ini) (0x004107E0)
// Reads Name= and UIName= from the type's section; false without one.

bool AbstractTypeClass::Read_INI(CCINIClass &ini)
{
    char buffer[1024];

    if (ini.Find_Section(ID)) {
        ini.Get_String(ID, "Name", Name, buffer, sizeof(Name));
        Name = buffer;
        ini.Get_String(ID, "UIName", UINameLabel, buffer, sizeof(UINameLabel));
        UINameLabel = buffer;
        if (UINameLabel[0] != 0) {
            UIName = StringTable::Load_String(UINameLabel, 0, "D:\\Ra2\\AbsType.cpp", 205);
        } else {
            UIName = unknown_empty_wide_839d4c;
        }
        return true;
    }
    return false;
}


// bool AbstractTypeClass::Write_INI(CCINIClass &ini) const (0x00410910)
// Rewrites the section: Name=, and UIName= when set.

bool AbstractTypeClass::Write_INI(CCINIClass &ini) const
{
    ini.Clear(ID);
    ini.Put_String(ID, "Name", Name);
    if (UINameLabel[0] != 0) {
        ini.Put_String(ID, "UIName", UINameLabel);
    }
    return true;
}


// void AbstractTypeClass::Compute_CRC(WWCRCEngine &crc) const (0x00410960)
// Adds the AbstractClass data, ID, Name and UIName label to the CRC.

void AbstractTypeClass::Compute_CRC(WWCRCEngine &crc) const
{
    AbstractClass::Compute_CRC(crc);
    crc(ID);
    crc(Name);
    crc(UINameLabel);
}


// void AbstractTypeClass::unknown_slot24(int value) (0x004109A0)
// Virtual slot 24: nothing.

void AbstractTypeClass::unknown_slot24(int value)
{
}


// AbstractTypeClass scalar deleting destructor (0x004109B0)
// Compiler generated after the unit's last member: the inlined destructor
// (AbstractTypes.Delete, ~AbstractClass), then operator delete.
