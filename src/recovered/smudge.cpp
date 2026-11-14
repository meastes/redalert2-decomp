// smudge.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/smudge.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/smudge.hpp"
#include "shared/crt_tokens.hpp"


// Header-static dynamic initializers of the unit at 0x006873D0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006873D0-0x006875B6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x00687400, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687420, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687440, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687460, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687480, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x006874A0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x006874E0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687510, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687540, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687570, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x00687580, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// Dynamic initializer (0x006875A0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006873d0.cpp; no source of its own.


// SmudgeClass::SmudgeClass(SmudgeTypeClass *type, CoordStruct const &coord, int house) (0x006875C0)
// Registers the smudge and places it at the coordinate (unless it is the
// zero coordinate) for the given house; a smudge that cannot be placed is
// removed again (slot 60).
// Assumptions: slot 53 places an object (Unlimbo); the house global at
// 0x007F4230 is read while placing.

SmudgeClass::SmudgeClass(SmudgeTypeClass *type, CoordStruct const &coord, int house) :
    Class(type)
{
    AbstractClass::Create_ID();
    Smudges.Add(this);
    if (coord.X != unknown_geometry_l.X || coord.Y != unknown_geometry_l.Y || coord.Z != unknown_geometry_l.Z) {
        unknown_placing_house_7f4230 = house;
        if (!ObjectClass::Unlimbo(&coord, 0)) {
            ObjectClass::Delete_Me();
            return;
        }
        unknown_placing_house_7f4230 = -1;
    }
}


// SmudgeClass::~SmudgeClass() (0x006876B0)
// Leaves the notice lists and the smudge list; leaves the map (ObjectClass
// slot 52) only when the 0x00A40958 flag is set.

SmudgeClass::~SmudgeClass()
{
    Unknown_006ECB90(this, true);
    Smudges.Delete(this);
    if (unknown_flag_a40958) {
        ObjectClass::Limbo();
    }
    Class = 0;
}


// bool SmudgeClass::Mark(int arg1) (0x00687750)
// After the base handling succeeds for modes 1 and 3, marks the smudge's
// cells (SmudgeTypeClass::Place, when the cells allow it or placement is
// forced) and removes the object (slot 60).

bool SmudgeClass::Mark(int arg1)
{
    if (ObjectClass::Mark(arg1) && (arg1 == 1 || arg1 == 3)) {
        Cell cell = Cell(unknown_slot104());
        if (unknown_a40774 > 0 || Debug_Map || Class->Can_Place(cell, true)) {
            Class->Place(cell);
        }
        unknown_74 = false;
        Delete_Me();
        return true;
    }
    return false;
}


// 0x006877e0: function not yet identified
//
// Body: 3 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void Disown_006877e0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006877e0 {

void __stdcall Disown_006877e0(int unused_0)
{
    return;
}

}  // namespace seed_006877e0


// static void __fastcall SmudgeClass::Read_INI(CCINIClass &ini) (0x006877F0)
// Creates the smudges listed in the [Smudge] section ("type,x,y,piece");
// only first pieces (piece 0) are created, the type marks its other cells.

void __fastcall SmudgeClass::Read_INI(CCINIClass &ini)
{
    int count = ini.Entry_Count(unknown_smudge_section_7ab558);
    for (int index = 0; index < count; index++) {
        char const *entry = ini.Get_Entry(unknown_smudge_section_7ab558, index);
        char buffer[128];
        ini.Get_String(unknown_smudge_section_7ab558, entry, 0, buffer, sizeof(buffer));
        int type = SmudgeTypeClass::From_Name(strtok(buffer, ","));
        if (type != -1) {
            char *token = strtok(0, ",");
            if (token != 0) {
                Cell cell;
                short x = atoi(token);
                cell.Y = atoi(strtok(0, ","));
                cell.X = x;
                token = strtok(0, ",");
                if (token == 0 || atoi(token) == 0) {
                    new SmudgeClass(SmudgeTypes[type], CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0), -1);
                }
            }
        }
    }
}


// static void __fastcall SmudgeClass::Write_INI(CCINIClass &ini) (0x00687930)
// Rewrites the [Smudge] section from the first piece of every smudged cell
// of the 512 x 512 map ("type,x,y,piece").

void __fastcall SmudgeClass::Write_INI(CCINIClass &ini)
{
    ini.Clear(unknown_smudge_section_7ab558);
    int index = 0;
    for (int y = 0; y < 512; y++) {
        for (int x = 0; x < 512; x++) {
            CellClass &cell = Map_Cell(Cell((short)x, (short)y));
            if (cell.SmudgeIndex != -1 && cell.SmudgeData == 0) {
                SmudgeTypeClass *type = SmudgeTypes[cell.SmudgeIndex];
                char entry[12];
                char value[128];
                sprintf(entry, "%d", index);
                sprintf(value, "%s,%d,%d,%d", (char const *)type->ID, x, y, cell.SmudgeData);
                ini.Put_String(unknown_smudge_section_7ab558, entry, value);
                index++;
            }
        }
    }
}


// HRESULT __stdcall SmudgeClass::Load(IStream *stream) (0x00687A00)
// Restores the smudge in place and swizzles its type.

HRESULT __stdcall SmudgeClass::Load(IStream *stream)
{
    HRESULT result = ObjectClass::Load(stream);
    if (SUCCEEDED(result)) {
        new (this) SmudgeClass(NoInitClass());
        Swizzler.Swizzle((void **)&Class);
    }
    return result;
}


// HRESULT __stdcall SmudgeClass::Save(IStream *stream, BOOL clear_dirty) (0x00687A60)

HRESULT __stdcall SmudgeClass::Save(IStream *stream, BOOL clear_dirty)
{
    return ObjectClass::Save(stream, clear_dirty);
}


// ObjectTypeClass *SmudgeClass::Class_Of() const (0x00687A80)
// Compiler-emitted from shared/smudge.hpp in this unit; no source of its own.


// int SmudgeClass::Size_Of() const (0x00687A90)
// Compiler-emitted from shared/smudge.hpp in this unit; no source of its own.


// int SmudgeClass::Kind_Of() const (0x00687AA0)
// Compiler-emitted from shared/smudge.hpp in this unit; no source of its own.


// HRESULT __stdcall SmudgeClass::GetClassID(GUID *clsid) (0x00687AB0)
// Compiler-emitted from shared/smudge.hpp in this unit; no source of its own.


// void SmudgeClass::unknown_slot67(Point2D const &point, Rect const &window) (0x00687AF0)
// Compiler-emitted from shared/smudge.hpp in this unit; no source of its own.
