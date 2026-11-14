// subtitle.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/subtitle.json carries the addresses and the rows that
// stay in their own files.


#define VECTOR_IMPLEMENT_ALL
#include "shared/subtitle.hpp"
#include "shared/vector_class.hpp"
#include "shared/debug_printf.hpp"


// The movie subtitle parser unit (0x0069B680-0x0069C3AF; RA2 only, no static initializers): SubTitleParser reads
// a movie's section of the UTF-16 subtitle script into SubTitle objects (shared/subtitle.hpp), with the
// keyword handlers and the DynamicVectorClass<SubTitle *> instances.
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);                                    // 0x00783605
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);       // 0x00783AC6
extern "C" unsigned short *__cdecl wcspbrk(unsigned short const *string, unsigned short const *set);     // 0x00785509
extern "C" int __cdecl _wcsicmp(unsigned short const *left, unsigned short const *right);                // 0x00796318
extern "C" int __cdecl _wcsnicmp(unsigned short const *left, unsigned short const *right, unsigned int count);   // 0x007970A0
extern "C" unsigned int __cdecl mbstowcs(unsigned short *wide, char const *string, unsigned int count);  // 0x007853AC
unsigned short *__fastcall wcstrim(unsigned short *buffer);                                              // 0x006EEF80

bool __fastcall Read_Line(Straw &input, unsigned short *buffer, int count);   // 0x0069C100

// Script keywords and their handlers (0x007F62F0; null-terminated).
struct SubTitleHook {
    unsigned short const *Token;
    void (__fastcall *Handler)(unsigned short *param, SubTitle *subtitle);
};
extern SubTitleHook unknown_subtitle_hooks_7f62f0[];

// SubTitleParser::SubTitleParser(Straw &input) (0x0069B680): checks the UTF-16 byte order mark.
SubTitleParser::SubTitleParser(Straw &input) : Input(input), LineNumber(0)
{
    unsigned short mark = 0;
    input.Get(&mark, sizeof(mark));
    if (mark != 0xFEFF) {
        Debug_Printf("Error: Subtitle control file is not unicode!\n");
    }
}


// SubTitleParser::~SubTitleParser() (0x0069B6C0).
SubTitleParser::~SubTitleParser()
{
}


// DynamicVectorClass<SubTitle *> *SubTitleParser::Get_Sub_Titles(char const *moviename) (0x0069B6D0): the
// "<...>" entries of the movie's section up to EndMovie; 0 when there is no section or it is empty.
DynamicVectorClass<SubTitle *> *SubTitleParser::Get_Sub_Titles(char const *moviename)
{
    DynamicVectorClass<SubTitle *> *subtitles = 0;
    if (Find_Movie_Entry(moviename) == true) {
        subtitles = new DynamicVectorClass<SubTitle *>;
        if (subtitles != 0) {
            for (;;) {
                unsigned short *string = Get_Next_Line();
                if (string == 0 || wcslen(string) <= 0) {
                    continue;
                }
                if (string[0] == '<' && string[wcslen(string) - 1] == '>') {
                    unsigned short *entry = string + 1;
                    entry[wcslen(entry) - 1] = 0;
                    wcstrim(entry);
                    if (wcslen(entry) == 0) {
                        continue;
                    }
                    SubTitle *subtitle = new SubTitle;
                    if (subtitle == 0) {
                        Debug_Printf("***** Failed to create SubTitle!\n");
                        break;
                    }
                    if (Parse_Sub_Title(entry, subtitle) == true) {
                        subtitles->Add(subtitle);
                    } else {
                        delete subtitle;
                    }
                } else if (_wcsnicmp(string, L"EndMovie", wcslen(L"EndMovie")) == 0) {
                    break;
                }
            }
            if (subtitles->Count() == 0) {
                delete subtitles;
                subtitles = 0;
            }
        }
    }
    return subtitles;
}


// bool SubTitleParser::Find_Movie_Entry(char const *moviename) (0x0069B9A0): skips to the line
// "BeginMovie <moviename>".
bool SubTitleParser::Find_Movie_Entry(char const *moviename)
{
    unsigned short name[32];
    mbstowcs(name, moviename, 32);
    unsigned short *string;
    do {
        string = Get_Next_Line();
        if (string != 0 && _wcsnicmp(string, L"BeginMovie", wcslen(L"BeginMovie")) == 0) {
            unsigned short *ptr = wcschr(string, ' ');
            if (ptr != 0) {
                wcstrim(ptr);
                if (_wcsicmp(ptr, name) == 0) {
                    Debug_Printf("Found movie entry %s\n", moviename);
                    return true;
                }
            }
        }
    } while (string != 0);
    return false;
}


// bool SubTitleParser::Parse_Sub_Title(unsigned short *string, SubTitle *subtitle) (0x0069BAD0): the
// entry's "token = value" pairs (values quoted or up to a comma or blank), each passed to its handler.
bool SubTitleParser::Parse_Sub_Title(unsigned short *string, SubTitle *subtitle)
{
    for (;;) {
        unsigned short *separator = wcschr(string, '=');
        if (separator == 0) {
            Debug_Printf("Error on line %d: syntax error\n", LineNumber);
            return false;
        }
        *separator = 0;
        unsigned short *token = string;
        wcstrim(token);
        unsigned short *param = separator + 1;
        wcstrim(param);
        if (*param == '"') {
            param++;
            unsigned short *ptr = wcschr(param, '"');
            if (ptr == 0) {
                Debug_Printf("Error on line %d: mismatched quotes\n", LineNumber);
                return false;
            }
            *ptr++ = 0;
            wcstrim(ptr);
            if (*ptr == ',') {
                ptr++;
            }
            string = ptr;
        } else {
            unsigned short *ptr = wcspbrk(param, L", ");
            if (ptr != 0) {
                *ptr = 0;
                string = ptr + 1;
            } else {
                string = L"";
            }
        }
        if (wcslen(token) == 0) {
            Debug_Printf("Error on line %d: missing token\n", LineNumber);
            return false;
        }
        Parse_Token(token, param, subtitle);
        wcstrim(string);
        if (wcslen(string) == 0) {
            return true;
        }
    }
}


// void SubTitleParser::Parse_Token(unsigned short *token, unsigned short *param, SubTitle *subtitle)
// (0x0069BC60): calls the keyword's handler.
void SubTitleParser::Parse_Token(unsigned short *token, unsigned short *param, SubTitle *subtitle)
{
    if (token != 0) {
        for (int index = 0; unknown_subtitle_hooks_7f62f0[index].Token != 0; index++) {
            SubTitleHook *hook = &unknown_subtitle_hooks_7f62f0[index];
            if (_wcsicmp(hook->Token, token) == 0) {
                hook->Handler(param, subtitle);
                return;
            }
        }
    }
}


// unsigned short *SubTitleParser::Get_Next_Line() (0x0069BCB0): the next line that is not blank or a
// ';' comment, trimmed; 0 at the end of the input.
unsigned short *SubTitleParser::Get_Next_Line()
{
    bool eof = false;
    while (eof == false) {
        eof = Read_Line(Input, Buffer, 0x400);
        LineNumber++;
        unsigned short *string = wcstrim(Buffer);
        if (wcslen(string) > 0 && *string != ';') {
            return string;
        }
    }
    return 0;
}


// void __fastcall unknown_set_timecode(unsigned short const *text, UnknownTimed_0069be40 *target) -- 0x0069BE40
// Purpose: stores the time parsed from a "H:M:S:F" string
// (unknown_parse_timecode, 0x0069BD60) in target +0x04 unless it is 0.
// Assumptions: owner of +0x04 unidentified.

unsigned __fastcall unknown_parse_timecode(unsigned short const *text);

struct UnknownTimed_0069be40 {
    int unknown_00;
    unsigned Time;                          // +0x04
};

void __fastcall unknown_set_timecode(unsigned short const *text, UnknownTimed_0069be40 *target)
{
    unsigned time = unknown_parse_timecode(text);
    if (time > 0) {
        target->Time = time;
    }
}


// void __fastcall unknown_subtitle_parse_position(unsigned short *value, SubTitle *subtitle) (0x0069BE60):
// the "Position" handler: "[line:]alignment" with the alignment one of Left, Right, Center (default).
struct SubTitleAlignmentName {
    unsigned short const *Name;
    int Alignment;
};
extern SubTitleAlignmentName unknown_subtitle_alignments_7f6320[];
extern "C" long __cdecl wcstol(unsigned short const *string, unsigned short **end, int base);   // 0x00784BB8

void __fastcall unknown_subtitle_parse_position(unsigned short *value, SubTitle *subtitle)
{
    unsigned short *colon = wcschr(value, ':');
    if (colon != 0) {
        *colon = 0;
        subtitle->Set_Line_Position(wcstol(value, 0, 0));
        value = colon + 1;
    }
    int alignment = 2;
    for (int index = 0; unknown_subtitle_alignments_7f6320[index].Name != 0; index++) {
        if (_wcsicmp(value, unknown_subtitle_alignments_7f6320[index].Name) == 0) {
            alignment = unknown_subtitle_alignments_7f6320[index].Alignment;
            break;
        }
    }
    subtitle->Set_Alignment(alignment);
}


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// DynamicVectorClass<SubTitle *>::Clear (0x0069BF80; DynamicVectorClass<SubTitle *> vtable 0x007ac194 slot 3).
// DynamicVectorClass<SubTitle *>::ID_pointer (0x0069BFB0; DynamicVectorClass<SubTitle *> vtable 0x007ac194 slot 5).
// VectorClass<SubTitle *>::Fetch (0x0069BFD0; VectorClass<SubTitle *> vtable 0x007ac1b4 slot 6).
// VectorClass<SubTitle *>::operator equal (0x0069BFE0; VectorClass<SubTitle *> vtable 0x007ac1b4 slot 1).
// DynamicVectorClass<SubTitle *>::Resize (0x0069C020; DynamicVectorClass<SubTitle *> vtable 0x007ac194 slot 2).
// DynamicVectorClass<SubTitle *>::ID_object (0x0069C0D0; DynamicVectorClass<SubTitle *> vtable 0x007ac194 slot 4).


// bool __fastcall Read_Line(Straw &input, unsigned short *buffer, int count) (0x0069C100): one line of the
// script: up to count characters, carriage returns dropped; true at the end of the input.
bool __fastcall Read_Line(Straw &input, unsigned short *buffer, int count)
{
    *buffer = 0;
    bool eof = false;
    int length = 0;
    while (length < count) {
        unsigned short character;
        if (input.Get(&character, sizeof(character)) != sizeof(character)) {
            eof = true;
            break;
        }
        if (character == '\n') {
            break;
        }
        if (character != '\r') {
            buffer[length] = character;
            length++;
        }
    }
    buffer[length] = 0;
    return eof;
}


extern "C" unsigned char vtable_69c180[]; extern "C" void __cdecl release_buffer_69c180(void*); extern "C" void __cdecl release_object_69c180(void*);
class Leaf_69c180 { public: void* get(unsigned int flags); };
void* Leaf_69c180::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_69c180; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_69c180((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_69c180(this); return this; }


// Container members this unit emits by its own use of its lists (bodies: shared/vector_class.hpp):
// VectorClass<SubTitle *>::Resize (0x0069C1D0; VectorClass<SubTitle *> vtable 0x007ac1b4 slot 2).
// VectorClass<SubTitle *>::Clear (0x0069C280; VectorClass<SubTitle *> vtable 0x007ac1b4 slot 3).
// VectorClass<SubTitle *>::ID_object (0x0069C2B0; VectorClass<SubTitle *> vtable 0x007ac1b4 slot 4).
// VectorClass<SubTitle *>::ID_pointer (0x0069C2F0; VectorClass<SubTitle *> vtable 0x007ac1b4 slot 5).


extern "C" unsigned char vtable_69c310[]; extern "C" void __cdecl release_buffer_69c310(void*); extern "C" void __cdecl release_object_69c310(void*);
class Leaf_69c310 { public: void* get(unsigned int flags); };
void* Leaf_69c310::get(unsigned int flags) { unsigned int pointer = *(unsigned int*)((char*)this + 4); *(unsigned int*)this=(unsigned int)vtable_69c310; if(pointer && *(unsigned char*)((char*)this + 13)){ release_buffer_69c310((void*)pointer); *(unsigned int*)((char*)this + 4)=0; } *(unsigned char*)((char*)this + 13)=0; *(unsigned int*)((char*)this + 8)=0; if(flags & 1) release_object_69c310(this); return this; }


// VectorClass<SubTitle *>::VectorClass(unsigned size, SubTitle * const *array) (0x0069C360): this unit's copy,
// emitted by shared/vector_class.hpp for Get_Sub_Titles's `new DynamicVectorClass<SubTitle *>`.
