// palette.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/palette.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_ALL
#define RGB_DECLARE_DEFAULT_CONSTRUCTOR
#include "shared/color_scheme.hpp"
#include "shared/crt_string.hpp"
#define RA2_HASH_STRING_UNIT
#include "shared/palette_name.hpp"
#include "shared/scheme_table.hpp"
#include "shared/theater_type.hpp"
#include "shared/convert.hpp"
#include "shared/crt_memory.hpp"


// The per-palette colour scheme lists of the palette-name loader (unit 0x00602FD0 continued; RA2 only): a named
// list of the colour schemes built for one palette file (0x104 bytes; the value type of the scheme hash table,
// HashObject<HashString, SchemeNode *>). Names are hypotheses.

class SchemeNode {
public:
    // A node for the palette file name and its scheme list (0x00603200).
    SchemeNode(char const *name, DynamicVectorClass<ColorScheme *> *schemes);
    // Frees the schemes and the list (0x00603240: the destructor, Free expanded inline).
    ~SchemeNode();
    // Deletes every scheme and the list (0x00603320, out of line where a caller's budget runs out).
    void unknown_603320();
    // Frees the current list and takes another (0x006032A0; the free expands inline).
    void unknown_6032a0(DynamicVectorClass<ColorScheme *> *schemes);

    char Name[0x100];                               // +0x000
    DynamicVectorClass<ColorScheme *> *Schemes;     // +0x100
};

SchemeNode::SchemeNode(char const *name, DynamicVectorClass<ColorScheme *> *schemes)
{
    strcpy(Name, name);
    Schemes = schemes;
}


// SchemeNode::~SchemeNode() (0x00603240): frees the scheme list (0x00603320 expanded inline; the destructor
// itself expands wherever a node is deleted).

SchemeNode::~SchemeNode()
{
    unknown_603320();
}


// SchemeNode::unknown_6032a0 (0x6032A0)
void SchemeNode::unknown_6032a0(DynamicVectorClass<ColorScheme *> *schemes)
{
    unknown_603320();
    Schemes = schemes;
}


// SchemeNode::unknown_603320 (0x603320)
void SchemeNode::unknown_603320()
{
    if (Schemes != 0) {
        int count = Schemes->Count();
        for (int index = 0; index < count; index++) {
            ColorScheme *scheme = (*Schemes)[index];
            if (scheme != 0) {
                delete scheme;
            }
        }
        delete Schemes;
        Schemes = 0;
    }
}


// Resolves the palette name to its cached list of colour schemes.

void *__fastcall PaletteName::Load()
{
    SchemeTable *table = Scheme_Table();
    DynamicVectorClass<ColorScheme *> *schemes = table->Find_Schemes(Buffer);
    if (schemes != 0)
        return schemes;
    unknown_scheme_table_a76414->Add_Node(Buffer);
    return unknown_scheme_table_a76414->Find_Schemes(Buffer);
}


// int __fastcall Unknown_Hash_00603610(HashString const &key) (0x00603610): the scheme table's hash,
// (c + hash * 26) % 31 over the key's text.

int __fastcall Unknown_Hash_00603610(HashString const &key)
{
    char const *text = key.Text;
    int hash = 0;
    for (char c = *text; c != '\0'; c = *++text) {
        hash = (c + hash * 26) % 31;
    }
    return hash;
}


// DynamicVectorClass<ColorScheme *> *UnknownSchemeListIterator::Next() (0x00603640; Set_Scheme_Lighting
// 0x00520A80 and Set_Lighting 0x00520B00 walk the table with it): the scheme list of the table's next non-empty
// key, null at the end. The table's Next and Find expand inline; Find goes through the global again.

DynamicVectorClass<ColorScheme *> *UnknownSchemeListIterator::Next()
{
    HashTableClass<HashString, SchemeNode *> *table = unknown_scheme_table_a76414->Table;
    HashString *key = table->Next(*this);
    if (key == 0) {
        return 0;
    }
    SchemeNode **node = unknown_scheme_table_a76414->Table->Find(*key);
    if (node == 0) {
        return 0;
    }
    return (*node)->Schemes;
}


// Not exact (RA2_NON_MATCHING): Unknown_00603750 at 0x00603750, best score 70.35, 2026-11-13; differs: same instruction count (172), shape 98.84: instruction choice and registers differ
#ifdef RA2_NON_MATCHING
// void __fastcall Unknown_00603750(int theater) (0x00603750; Init_Theater 0x00519E70): makes the table on first
// use and switches it to the theater (nothing when it is unchanged).

void __fastcall Unknown_00603750(int theater)
{
    SchemeTable *table = Scheme_Table();
    if (table->unknown_00 != theater) {
        table->Set_Theater(theater);
    }
}
#endif

// void Unknown_00603950() (0x00603950; RulesClass::Process 0x006433A0): frees the extra palette names and empties
// the name and colour lists (the table is made on first use).

void Unknown_00603950()
{
    int count = Scheme_Table()->Names->Count();
    for (int index = 0; index < count; index++) {
        delete (*unknown_scheme_table_a76414->Names)[index];
    }
    unknown_scheme_table_a76414->Names->Clear();
    unknown_scheme_table_a76414->Colors->Clear();
}


// void __fastcall unknown_add_color_603a60(char const *name, HSVClass const &color) (0x00603A60; RulesClass::Colors
// 0x00647E60): adds an extra palette name (a 256-byte copy) and its colour (the table is made on first use).

void __fastcall unknown_add_color_603a60(char const *name, HSVClass const &color)
{
    Scheme_Table();
    char *copy = new char[0x100];
    strcpy(copy, name);
    unknown_scheme_table_a76414->Names->Add(copy);
    unknown_scheme_table_a76414->Colors->Add(color);
}


// int Unknown_00603C10() (0x00603C10; Set_Scheme_Lighting 0x00520A80): the number of extra palette names (the
// table is made on first use).

int Unknown_00603C10()
{
    return Scheme_Table()->Names->Count();
}


// void Unknown_00603FA0() (0x00603FA0; Prog_End and 0x006910E5): deletes the per-palette scheme table.

void Unknown_00603FA0()
{
    if (unknown_scheme_table_a76414 != 0) {
        delete unknown_scheme_table_a76414;
        unknown_scheme_table_a76414 = 0;
    }
}


// void Unknown_00603FD0() (0x00603FD0): makes the scheme table unless it exists.

void Unknown_00603FD0()
{
    if (unknown_scheme_table_a76414 == 0) {
        unknown_scheme_table_a76414 = new SchemeTable;
    }
}


// SchemeTable::SchemeTable() (0x00604120): the out-of-line copy of the inline constructor (shared/scheme_table.hpp)
// that the destructor's lazy make calls.


// SchemeTable::~SchemeTable() (0x00604250; Unknown_00603FA0): frees the extra palettes and both lists, every
// node and its schemes, then the table.

SchemeTable::~SchemeTable()
{
    if (Table != 0) {
        Unknown_00603950();
        delete Names;
        delete Colors;
        UnknownSchemeListIterator it;
        HashString *key = Table->Next(it);
        while (key != 0) {
            SchemeNode **node = Table->Find(*key);
            delete *node;
            key = Table->Next(it);
        }
        Table->Clear();
        delete Table;
    }
}


// DynamicVectorClass<ColorScheme *> *SchemeTable::Find_Schemes(char const *name) (0x006044A0; no direct callers):
// the scheme list filed under the palette name, null when there is none (the table's Find expands inline; the
// key is a named HashString local).

DynamicVectorClass<ColorScheme *> *SchemeTable::Find_Schemes(char const *name)
{
    HashString key(name);
    SchemeNode **node = Table->Find(key);
    if (node == 0) {
        return 0;
    }
    return (*node)->Schemes;
}


// Build the pair of colour schemes for every registered remap colour,00604540.
extern "C" char *__cdecl _strupr(char *text);
// The physical static MixFileClass::Retrieve entry: filename in ECX, pointer result in EAX.
void const *__fastcall Retrieve_Palette_Data_005956c0(char const *filename);
extern PaletteClass unknown_palette_837e10;
extern char const Scheme_Palette_Dot_007dbe44[];
extern char const *Scheme_Palette_Extension_007eb5e4;

DynamicVectorClass<ColorScheme *> *SchemeTable::Build_Schemes(char const *name)
{
    if (unknown_00 == -1) return 0;
    char filename[256];
    strcpy(filename, name);
    strcat(filename, Theaters[unknown_00].Suffix);
    strcat(filename, Scheme_Palette_Dot_007dbe44);
    strcat(filename, Scheme_Palette_Extension_007eb5e4);
    _strupr(filename);
    PaletteClass palette;
    void const *data = Retrieve_Palette_Data_005956c0(filename);
    if (!data) return 0;
    memcpy(&palette, data, sizeof(palette));
    RGBClass color(0, 0, 0);
    for (int index = 0; index < 256; ++index) {
        color = palette[index];
        palette[index].Set(color.Red << 2, color.Green << 2, color.Blue << 2);
    }
    int count = Colors->Count();
    DynamicVectorClass<ColorScheme *> *schemes = new DynamicVectorClass<ColorScheme *>(20);
    for (int scheme_index = 0; scheme_index < count; ++scheme_index) {
        char const *scheme_name = (*Names)[scheme_index];
        HSVClass color = (*Colors)[scheme_index];
        ColorScheme *normal = new ColorScheme(scheme_name, color, &palette, &unknown_palette_837e10, 1, false);
        normal->Index = scheme_index * 2;
        schemes->Add(normal);
        ColorScheme *dark = new ColorScheme(scheme_name, color, &palette, &unknown_palette_837e10, 53, false);
        dark->Index = scheme_index * 2 + 1;
        schemes->Add(dark);
    }
    return schemes;
}


// void SchemeTable::Add_Node(char const *name) (0x00604870): files a node with a freshly built list under the name.

void SchemeTable::Add_Node(char const *name)
{
    DynamicVectorClass<ColorScheme *> *schemes = Build_Schemes(name);
    SchemeNode *node = new SchemeNode(name, schemes);
    Table->Add(name, node);
}


// void SchemeTable::Rebuild_Node(char const *name) (0x006049C0): replaces the named node's list with a fresh
// one (the node must exist).

void SchemeTable::Rebuild_Node(char const *name)
{
    SchemeNode *node = *Table->Find(name);
    DynamicVectorClass<ColorScheme *> *schemes = Build_Schemes(name);
    node->unknown_603320();
    node->Schemes = schemes;
}


// void SchemeTable::Set_Theater(int theater) (0x00604AD0): stores the theater and rebuilds every node's list.

void SchemeTable::Set_Theater(int theater)
{
    unknown_00 = theater;
    UnknownSchemeListIterator it;
    HashString *key = Table->Next(it);
    while (key != 0) {
        Rebuild_Node(key->Text);
        key = Table->Next(it);
    }
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<ColorScheme *>::Clear (0x00604CD0; DynamicVectorClass<ColorScheme *> vtable 0x007a7b98 slot 3).
// DynamicVectorClass<ColorScheme *>::ID_pointer (0x00604D00; DynamicVectorClass<ColorScheme *> vtable 0x007a7b98 slot 5).
// VectorClass<ColorScheme *>::Fetch (0x00604D20; VectorClass<ColorScheme *> vtable 0x007a7bb8 slot 6).


// ?Clear@?$DynamicVectorClass@VHSVClass@@@@UAEXXZ (0x604D30): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?ID@?$DynamicVectorClass@VHSVClass@@@@UAEHPBVHSVClass@@@Z (0x604D60): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?Fetch@?$VectorClass@VHSVClass@@@@UBE?AVHSVClass@@H@Z (0x604D90): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<ColorScheme *>::operator equal (0x00604DC0; VectorClass<ColorScheme *> vtable 0x007a7bb8 slot 1).
// VectorClass<ColorScheme *>::Resize (0x00604E00; VectorClass<ColorScheme *> vtable 0x007a7bb8 slot 2).
// VectorClass<ColorScheme *>::Clear (0x00604EB0; VectorClass<ColorScheme *> vtable 0x007a7bb8 slot 3).
// VectorClass<ColorScheme *>::ID_object (0x00604EE0; VectorClass<ColorScheme *> vtable 0x007a7bb8 slot 4).
// VectorClass<ColorScheme *>::ID_pointer (0x00604F20; VectorClass<ColorScheme *> vtable 0x007a7bb8 slot 5).
// DynamicVectorClass<ColorScheme *>::Resize (0x00604F40; DynamicVectorClass<ColorScheme *> vtable 0x007a7b98 slot 2).
// DynamicVectorClass<ColorScheme *>::ID_object (0x00604FF0; DynamicVectorClass<ColorScheme *> vtable 0x007a7b98 slot 4).


// ??0?$HashTableClass@VHashString@@PAVSchemeNode@@@@QAE@HHP6IHABVHashString@@@Z@Z (0x605020): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


SchemeNode **HashTableClass<HashString, SchemeNode *>::Find(HashString const &key)
{
        DynamicVectorClass<HashObject<HashString, SchemeNode *> > &bucket = Buckets[Hash(key) % BucketCount];
        for (int index = bucket.Count() - 1; index >= 0; index--) {
            HashObject<HashString, SchemeNode *> &object = bucket[index];
            if (object.Key.operator==(key)) {
                return &object.Value;
            }
        }
        return 0;
    }


HashString *HashTableClass<HashString, SchemeNode *>::Next(UnknownSchemeListIterator &it)
{
        if (it.Done) {
            return 0;
        }
        while (it.Bucket < BucketCount) {
            DynamicVectorClass<HashObject<HashString, SchemeNode *> > &bucket = Buckets[it.Bucket];
            while (it.Index < bucket.Count()) {
                HashObject<HashString, SchemeNode *> &object = bucket[it.Index];
                it.Index++;
                if (!object.Key.Is_Empty()) {
                    return &object.Key;
                }
            }
            it.Bucket++;
            it.Index = 0;
        }
        it.Done = true;
        return 0;
    }


// ??8?$VectorClass@VHSVClass@@@@UBE_NABV0@@Z (0x6051A0): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?Resize@?$VectorClass@VHSVClass@@@@UAE_NHPBVHSVClass@@@Z (0x605290): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?Clear@?$VectorClass@VHSVClass@@@@UAEXXZ (0x605390): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?ID@?$VectorClass@VHSVClass@@@@UAEHABVHSVClass@@@Z (0x6053C0): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?ID@?$VectorClass@VHSVClass@@@@UAEHPBVHSVClass@@@Z (0x605420): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?Resize@?$DynamicVectorClass@VHSVClass@@@@UAE_NHPBVHSVClass@@@Z (0x605450): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?ID@?$DynamicVectorClass@VHSVClass@@@@UAEHABVHSVClass@@@Z (0x605550): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


extern "C" unsigned char vtable_6055a0[]; extern "C" void __cdecl release_buffer_6055a0(void*); extern "C" void __cdecl release_object_6055a0(void*);
class Leaf_6055a0 { public: void* get(unsigned int flags); };
void* Leaf_6055a0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6055a0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6055a0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6055a0(this); return this; }


extern "C" unsigned char vtable_6055f0[]; extern "C" void __cdecl release_buffer_6055f0(void*); extern "C" void __cdecl release_object_6055f0(void*);
class Leaf_6055f0 { public: void* get(unsigned int flags); };
void* Leaf_6055f0::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_6055f0; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_6055f0((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_6055f0(this); return this; }


// ??_E?$DynamicVectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@UAEPAXI@Z (0x605640): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ??_F?$DynamicVectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@QAEXXZ (0x6056F0): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ??_G?$VectorClass@VHSVClass@@@@UAEPAXI@Z (0x605710): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ??_G?$DynamicVectorClass@VHSVClass@@@@UAEPAXI@Z (0x605760): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?Clear@?$DynamicVectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@UAEXXZ (0x6057B0): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?ID@?$DynamicVectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@UAEHPBU?$HashObject@VHashString@@PAVSchemeNode@@@@@Z (0x6057E0): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?Fetch@?$VectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@UBE?AU?$HashObject@VHashString@@PAVSchemeNode@@@@H@Z (0x605810): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ??8?$VectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@UBE_NABV0@@Z (0x605850): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?Resize@?$VectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@UAE_NHPBU?$HashObject@VHashString@@PAVSchemeNode@@@@@Z (0x6058A0): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?Clear@?$VectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@UAEXXZ (0x6059F0): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?ID@?$VectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@UAEHABU?$HashObject@VHashString@@PAVSchemeNode@@@@@Z (0x605A20): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?ID@?$VectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@UAEHPBU?$HashObject@VHashString@@PAVSchemeNode@@@@@Z (0x605A80): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ??0?$DynamicVectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@QAE@IPBU?$HashObject@VHashString@@PAVSchemeNode@@@@@Z (0x605AB0): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ?Resize@?$DynamicVectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@UAE_NHPBU?$HashObject@VHashString@@PAVSchemeNode@@@@@Z (0x605B60): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<HashObject<HashString, SchemeNode *>>::Add (0x00605CC0).
// Layout and behaviour: shared/vector_class.hpp, element shared/hash_string.hpp
// (0x00603577 adds a color scheme's hash entry).


// ?ID@?$DynamicVectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@UAEHABU?$HashObject@VHashString@@PAVSchemeNode@@@@@Z (0x605D30): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


class Call_605d80_0 {public: unsigned int target(unsigned int p0);};
class Leaf_605d80 { public: unsigned int get(unsigned int a1, unsigned int a2); };
unsigned int Leaf_605d80::get(unsigned int a1, unsigned int a2) { unsigned int r0 = ((Call_605d80_0*)((unsigned int)this))->target((unsigned int)(a1)); *(unsigned int*)(((unsigned int)this) + 0x100) = *(unsigned int*)((a2)); return (unsigned int)this; }


// ??_G?$VectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@UAEPAXI@Z (0x605E30): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).


// ??0?$VectorClass@U?$HashObject@VHashString@@PAVSchemeNode@@@@@@QAE@IPBU?$HashObject@VHashString@@PAVSchemeNode@@@@@Z (0x605E80): template instance emitted by the colour-scheme table unit from its shared headers
// (vector_class.hpp, hash_table.hpp, hash_object.hpp).
