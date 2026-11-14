// wdtvoice.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wdtvoice.json carries the addresses and the rows that
// stay in their own files.


#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#define WDT_VOICES_DECLARE_SET_NAME
#include "shared/vector_class.hpp"
#include "shared/wdt_voices.hpp"


// Header-static dynamic initializers of the unit at 0x007312D0: the shape_file.hpp empty rect and the
// shared/unknown_geometry.hpp constants (0x007312D0-0x007314B1); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x007312F0, _$E) of diagonal from the header(s) included
// by HeaderStatics_empty_rect_init_007312d0.cpp; no source of its own.


// Dynamic initializer (0x00731320, _$E) of b from the header(s) included
// by HeaderStatics_empty_rect_init_007312d0.cpp; no source of its own.


// Dynamic initializer (0x00731340, _$E) of c from the header(s) included
// by HeaderStatics_empty_rect_init_007312d0.cpp; no source of its own.


// Dynamic initializer (0x00731360, _$E) of angle45 from the header(s) included
// by HeaderStatics_empty_rect_init_007312d0.cpp; no source of its own.


// Dynamic initializer (0x00731380, _$E) of angle60 from the header(s) included
// by HeaderStatics_empty_rect_init_007312d0.cpp; no source of its own.


// Dynamic initializer (0x007313A0, _$E) of angle90 from the header(s) included
// by HeaderStatics_empty_rect_init_007312d0.cpp; no source of its own.


// Dynamic initializer (0x007313C0, _$E) of g from the header(s) included
// by HeaderStatics_empty_rect_init_007312d0.cpp; no source of its own.


// Dynamic initializer (0x00731400, _$E) of h from the header(s) included
// by HeaderStatics_empty_rect_init_007312d0.cpp; no source of its own.


// Dynamic initializer (0x00731430, _$E) of i from the header(s) included
// by HeaderStatics_empty_rect_init_007312d0.cpp; no source of its own.


// Dynamic initializer (0x00731460, _$E) of j from the header(s) included
// by HeaderStatics_empty_rect_init_007312d0.cpp; no source of its own.


// Dynamic initializer (0x00731490, _$E) of k from the header(s) included
// by HeaderStatics_empty_rect_init_007312d0.cpp; no source of its own.


// Dynamic initializer (0x007314A0, _$E) of l from the header(s) included
// by HeaderStatics_empty_rect_init_007312d0.cpp; no source of its own.


// WorldDominationTour::Voices::Voices(int faction) (0x007314C0)
// Names the six voice groups after their INI key prefixes.
// First unit of the Voices population: the container member guards of the
// unit (see Load 0x007317A0) and the group name setter are defined here,
// before shared/vector_class.hpp and shared/wdt_voices.hpp are first included.

namespace WorldDominationTour {

Voices::Voices(int faction) :
    Faction(faction),
    Idle(true),
    Pending(0),
    Stream(0)
{
    Audio_Get_Device();
    Groups[0].Set_Name("OldCycle");
    Groups[1].Set_Name("History");
    Groups[2].Set_Name("Status");
    Groups[3].Set_Name("Territory");
    Groups[4].Set_Name("Game");
    Groups[5].Set_Name("Evacuate");
}

}


// WorldDominationTour::Voices::~Voices() (0x00731680)

namespace WorldDominationTour {

Voices::~Voices()
{
    Stop();
    if (Stream != 0) {
        Audio_Stream_Destroy(Stream);
    }
}

}


// void WorldDominationTour::Voices::Load(INIClass &ini, Wstring const &section) (0x007317A0)
// Every group's Win/Lose/Draw lines and the Startup and TerritorySelect
// lists of the faction.
// Container members: only the ones this unit expands (the Group line
// arrays' constructor, Add, Delete(int) and the accessors). vector_class.hpp
// is included first, before wdt_voices.hpp pulls in shared/ms_engine.hpp,
// whose VECTOR_IMPLEMENT_ALL would otherwise compile every
// DynamicVectorClass<Wstring> virtual member and emit a ??_EWstring that
// the original unit never had.

namespace WorldDominationTour {

void Voices::Load(INIClass &ini, Wstring const &section)
{
    for (int group = 0; group < 6; group++) {
        Groups[group].Load(ini, section, Faction);
    }
    {
        Wstring key("Startup");
        Load_List(ini, section, 0, key);
    }
    {
        Wstring key("TerritorySelect");
        Load_List(ini, section, 1, key);
    }
}

}


// void WorldDominationTour::Voices::Load_List(INIClass &ini, Wstring const &section, int index, Wstring const &key) (0x00731AA0)
// The comma separated line numbers of <faction name><key>.

namespace WorldDominationTour {

void Voices::Load_List(INIClass &ini, Wstring const &section, int index, Wstring const &key)
{
    Wstring entry = Wstring_Join(Faction_Name(Faction), key);
    Wstring value;
    {
        Wstring empty("");
        ini.Get_String(section, entry, empty, value);
    }
    Parse_List(value, Lists[index]);
}

}


// bool WorldDominationTour::Voices::Pick(int group, int kind, Wstring &name) const (0x00731BC0)

namespace WorldDominationTour {

bool Voices::Pick(int group, int kind, Wstring &name) const
{
    return Groups[group].Pick(Faction, kind, name);
}

}


// bool WorldDominationTour::Voices::Pick_Emphasis(int group, int kind, Wstring &name) const (0x00731CA0)

namespace WorldDominationTour {

bool Voices::Pick_Emphasis(int group, int kind, Wstring &name) const
{
    return Groups[group].Pick_Emphasis(Faction, kind, name);
}

}


// bool WorldDominationTour::Voices::Pick_List(int index, Wstring &name) const (0x00731D80)

namespace WorldDominationTour {

bool Voices::Pick_List(int index, Wstring &name) const
{
    return Pick(Faction, Lists[index], name);
}

}


// bool __fastcall WorldDominationTour::Voices::Pick(int faction, DynamicVectorClass<unsigned> const &lines, Wstring &name) (0x00731E60)
// The speech name of a random line of the list (false, name released, for
// an empty list).

namespace WorldDominationTour {

bool __fastcall Voices::Pick(int faction, DynamicVectorClass<unsigned> const &lines, Wstring &name)
{
    name.Release();
    if (lines.Count() <= 0) {
        return false;
    }
    int index = lines.Count() == 1 ? 0 : unknown_random_839218(0, lines.Count() - 1);
    return Make_Name(faction, lines[index], name);
}

}


// bool __fastcall WorldDominationTour::Voices::Make_Name(int faction, int line, Wstring &name) (0x00731F30)
// "WWD-Soviet<NN>" for faction 3, else "WWD_Allied<NN>".

namespace WorldDominationTour {

bool __fastcall Voices::Make_Name(int faction, int line, Wstring &name)
{
    if (faction == 3) {
        name = "WWD-Soviet" + Wstring_From_Integer(line, 2);
    } else {
        name = "WWD_Allied" + Wstring_From_Integer(line, 2);
    }
    return true;
}

}


// void __fastcall WorldDominationTour::Voices::Parse_List(Wstring const &text, DynamicVectorClass<unsigned> &lines) (0x00731FC0)
// Replaces lines with the comma separated numbers of text.

namespace WorldDominationTour {

void __fastcall Voices::Parse_List(Wstring const &text, DynamicVectorClass<unsigned> &lines)
{
    lines.Clear();
    Wstring token;
    int position = text.Get_Token(0, ",", token);
    while (position != -1) {
        lines.Add(token.As_Integer());
        position = text.Get_Token(position, ",", token);
    }
}

}


// WorldDominationTour::Voices::Group::Group() (0x00732060)

namespace WorldDominationTour {

Voices::Group::Group()
{
    Name = "";
}

}


// void WorldDominationTour::Voices::Group::Load(INIClass &ini, Wstring const &section, int faction) (0x007320C0)

namespace WorldDominationTour {

void Voices::Group::Load(INIClass &ini, Wstring const &section, int faction)
{
    {
        Wstring key("Win");
        Load(ini, section, faction, 0, key);
    }
    {
        Wstring key("Lose");
        Load(ini, section, faction, 1, key);
    }
    {
        Wstring key("Draw");
        Load(ini, section, faction, 2, key);
    }
}

}


// bool WorldDominationTour::Voices::Group::Pick(int faction, int kind, Wstring &name) const (0x00732160)

namespace WorldDominationTour {

bool Voices::Group::Pick(int faction, int kind, Wstring &name) const
{
    return Voices::Pick(faction, Lines[kind * 2], name);
}

}


// bool WorldDominationTour::Voices::Group::Pick_Emphasis(int faction, int kind, Wstring &name) const (0x00732230)

namespace WorldDominationTour {

bool Voices::Group::Pick_Emphasis(int faction, int kind, Wstring &name) const
{
    return Voices::Pick(faction, Lines[kind * 2 + 1], name);
}

}


// void WorldDominationTour::Voices::Group::Load(INIClass &ini, Wstring const &section, int faction, int kind, Wstring const &key) (0x00732300)
// <faction name><group name><key> and the same + "Emphasis".

namespace WorldDominationTour {

void Voices::Group::Load(INIClass &ini, Wstring const &section, int faction, int kind, Wstring const &key)
{
    Wstring entry = Wstring_Join(Faction_Name(faction), Name) + key;
    Wstring value;
    {
        Wstring empty("");
        ini.Get_String(section, entry, empty, value);
    }
    Voices::Parse_List(value, Lines[kind * 2]);
    entry += "Emphasis";
    {
        Wstring empty("");
        ini.Get_String(section, entry, empty, value);
    }
    Voices::Parse_List(value, Lines[kind * 2 + 1]);
}

}


// void WorldDominationTour::Voices::Say(int group, int kind, bool emphasis, bool counted, int delay) (0x00732510)

namespace WorldDominationTour {

void Voices::Say(int group, int kind, bool emphasis, bool counted, int delay)
{
    Trim();
    Wstring name;
    if (Pick(group, kind, name)) {
        if (!Speech_Is_Playing()) {
            Timer = delay;
        }
        Queue.Add(name);
        if (counted) {
            Pending++;
        }
        if (emphasis && Pick_Emphasis(group, kind, name)) {
            Queue.Add(name);
            if (counted) {
                Pending++;
            }
        }
    }
}

}


// void WorldDominationTour::Voices::Say_List(int index, bool counted, int delay) (0x00732700)

namespace WorldDominationTour {

void Voices::Say_List(int index, bool counted, int delay)
{
    Trim();
    Wstring name;
    if (Pick_List(index, name)) {
        if (!Speech_Is_Playing()) {
            Timer = delay;
        }
        Queue.Add(name);
        if (counted) {
            Pending++;
        }
    }
}

}


// void WorldDominationTour::Voices::Update() (0x00732870)
// Once the delay ran out and nothing is playing: drops the line just said
// and plays the next queued one.

namespace WorldDominationTour {

void Voices::Update()
{
    if (Timer.Value() == 0 && Queue.Count() > 0 && !Speech_Is_Playing()) {
        if (!Idle) {
            Queue.Delete(0);
            if (Pending > 0) {
                Pending--;
            }
        }
        if (Queue.Count() > 0) {
            Speech_Play(Unknown_00716640(Queue[0].Get()), -1, -1);
            Idle = false;
        } else {
            Pending = 0;
            Idle = true;
        }
    }
}

}


// void WorldDominationTour::Voices::Trim() (0x00732960)
// Drops the queued lines past the Pending ones.

namespace WorldDominationTour {

void Voices::Trim()
{
    int index = Queue.Count();
    while (index-- > Pending) {
        Queue.Delete(index);
    }
    if (Pending == 0) {
        Idle = true;
    }
}

}


// 0x007329d0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_007329d0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_007329d0 {

void  FUN_007329d0(void)
{
    return;
}

}  // namespace seed_007329d0


// 0x007329e0: function not yet identified
//
// Body: 1 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void FUN_007329e0(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_007329e0 {

void  FUN_007329e0(void)
{
    return;
}

}  // namespace seed_007329e0


// void WorldDominationTour::Voices::Stop() (0x007329F0)

namespace WorldDominationTour {

void Voices::Stop()
{
    if (Queue.Count() > 0) {
        Speech_Stop(true);
        Queue.Clear();
    }
    Timer = CDTimerClass<SystemTimerClass>(0);
}

}


// bool WorldDominationTour::Voices::Is_Speaking() (0x00732A40)

namespace WorldDominationTour {

bool Voices::Is_Speaking()
{
    return Speech_Is_Playing();
}

}


// WorldDominationTour::Voices::Anim *WorldDominationTour::Voices::Create_Anim() (0x00732A50)
// A new animation that plays this object's queue.

namespace WorldDominationTour {

Voices::Anim *Voices::Create_Anim()
{
    return new Anim(this);
}

}


// Voices::Anim::Anim(Voices *owner) (0x00732AA0)

namespace WorldDominationTour {

Voices::Anim::Anim(Voices *owner) :
    Owner(owner)
{
}

}


// Voices::Anim::~Anim() (0x00732AE0)

namespace WorldDominationTour {

Voices::Anim::~Anim()
{
}

}


// void Voices::Anim::Set_Unknown_0C(bool value) (0x00732AF0)
// Clearing the flag stops the voices.

namespace WorldDominationTour {

void Voices::Anim::Set_Unknown_0C(bool value)
{
    unknown_0C = value;
    if (!value) {
        Owner->Stop();
    }
}

}


// void Voices::Anim::Pause() (0x00732B50)

namespace WorldDominationTour {

void Voices::Anim::Pause()
{
    Timer.Stop();
}

}


// void Voices::Anim::Resume() (0x00732BA0)

namespace WorldDominationTour {

void Voices::Anim::Resume()
{
    Timer.Start();
}

}


// bool Voices::Anim::Advance(Surface *surface, Rect *dirty) (0x00732BC0)
// Plays the next queued line when due; never asks the engine to delete it.

namespace WorldDominationTour {

bool Voices::Anim::Advance(Surface *surface, Rect *dirty)
{
    Owner->Update();
    return false;
}

}


// void Voices::Anim::Draw(Surface *surface, Rect const *clip) (0x00732BD0)

namespace WorldDominationTour {

void Voices::Anim::Draw(Surface *surface, Rect const *clip)
{
    Owner->Update();
}

}


// Rect Voices::Anim::Get_Rect() (0x00732BE0)
// An unset rectangle.

namespace WorldDominationTour {

Rect Voices::Anim::Get_Rect()
{
    Rect rect;
    return rect;
}

}


// bool Voices::Anim::Has_Finished() (0x00732C10)
// True once nothing is being said.

namespace WorldDominationTour {

bool Voices::Anim::Has_Finished()
{
    return !Speech_Is_Playing();
}

}


// void Voices::Anim::Restore(int unknown1) (0x00732C20)

namespace WorldDominationTour {

void Voices::Anim::Restore(int unknown1)
{
    Owner->Update();
}

}


// 0x00732c30: function not yet identified
//
// Owner: DynamicVectorClass<unsigned int>, stores the DynamicVectorClass<unsigned int> vtable (constructor or destructor) (from RTTI/vtables).
// Body: 32 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: void __fastcall FUN_00732c30(undefined4 *param_1)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00732c30 {

extern "C" void* PTR_FUN_0079c4dc;

void __fastcall FUN_00732c30(unsigned int *vector_words)
{
    *(unsigned char *)(vector_words + 3) = 1;
    vector_words[1] = 0;
    vector_words[2] = 0;
    *(unsigned char *)((int)vector_words + 0xd) = 0;
    *vector_words = (unsigned int)(&PTR_FUN_0079c4dc);
    vector_words[5] = 10;
    vector_words[4] = 0;
    return;
}

}  // namespace seed_00732c30


// 0x00732c80: function not yet identified
//
// Owner: WorldDominationTour::Voices::Anim, virtual slot 0 (vtable +0x0) (from RTTI/vtables).
// Body: 36 bytes (frozen inventory bound).
// Referenced from 1 site (data).
// Ghidra prototype: undefined4 * __thiscall FUN_00732c80(undefined4 *param_1,byte param_2)
// Callees with accepted source:
//   FUN_005ac690 -> src/recovered/classes/MSAnim/005ac690.cpp
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00732c80 {

extern void __fastcall FUN_005ac690(void);
extern void  FUN_00781d3b(unsigned int allocation_address);
extern "C" void* PTR_FUN_007af40c;

struct Owner_00732c80 { unsigned int * FUN_00732c80(unsigned char deletion_flags); };

unsigned int * Owner_00732c80::FUN_00732c80(unsigned char deletion_flags)
{
    unsigned int * object_words = (unsigned int *)this;
    *object_words = (unsigned int)(&PTR_FUN_007af40c);
    FUN_005ac690();
    if ((deletion_flags & 1) != 0) {
        FUN_00781d3b((unsigned int)(object_words));
    }
    return object_words;
}

}  // namespace seed_00732c80
