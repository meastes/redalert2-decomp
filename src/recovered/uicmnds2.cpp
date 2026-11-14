// uicmnds2.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/uicmnds2.json carries the addresses and the rows that
// stay in their own files.


#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/string_table_file.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/crt_string.hpp"
#include "shared/debug_printf.hpp"
#include "shared/session_state.hpp"
#include "shared/map_seed.hpp"


// Header-static dynamic initializers of the unit at 0x006F8BE0:  the
// shared/unknown_geometry.hpp constants and unknown_static_rect.hpp
// (0x006F8BE0-0x006F8DC6); the other
// drafts of this plan are emitted by these includes.


// Dynamic initializer (0x006F8C10, _$E) of b from the header(s) included
// by HeaderStatics_diagonal_init_006f8be0.cpp; no source of its own.


// Dynamic initializer (0x006F8C30, _$E) of c from the header(s) included
// by HeaderStatics_diagonal_init_006f8be0.cpp; no source of its own.


// Dynamic initializer (0x006F8C50, _$E) of angle45 from the header(s) included
// by HeaderStatics_diagonal_init_006f8be0.cpp; no source of its own.


// Dynamic initializer (0x006F8C70, _$E) of angle60 from the header(s) included
// by HeaderStatics_diagonal_init_006f8be0.cpp; no source of its own.


// Dynamic initializer (0x006F8C90, _$E) of angle90 from the header(s) included
// by HeaderStatics_diagonal_init_006f8be0.cpp; no source of its own.


// Dynamic initializer (0x006F8CB0, _$E) of g from the header(s) included
// by HeaderStatics_diagonal_init_006f8be0.cpp; no source of its own.


// Dynamic initializer (0x006F8CF0, _$E) of h from the header(s) included
// by HeaderStatics_diagonal_init_006f8be0.cpp; no source of its own.


// Dynamic initializer (0x006F8D20, _$E) of i from the header(s) included
// by HeaderStatics_diagonal_init_006f8be0.cpp; no source of its own.


// Dynamic initializer (0x006F8D50, _$E) of j from the header(s) included
// by HeaderStatics_diagonal_init_006f8be0.cpp; no source of its own.


// Dynamic initializer (0x006F8D80, _$E) of k from the header(s) included
// by HeaderStatics_diagonal_init_006f8be0.cpp; no source of its own.


// Dynamic initializer (0x006F8D90, _$E) of l from the header(s) included
// by HeaderStatics_diagonal_init_006f8be0.cpp; no source of its own.


// Dynamic initializer (0x006F8DB0, _$E) of static_rect from the header(s) included
// by HeaderStatics_diagonal_init_006f8be0.cpp; no source of its own.


// UnknownTableEntry_7f8cf0 *__fastcall unknown_table_find_6f8dd0(int id) (0x006F8DD0): the language table entry
// with this id; null when there is none.
UnknownTableEntry_7f8cf0 *__fastcall unknown_table_find_6f8dd0(int id)
{
    for (LanguageInfo *entry = unknown_languages_7f8cf0; entry->ID != 10; entry++) {
        if (entry->ID == id) {
            return entry;
        }
    }
    return 0;
}


// char const *__fastcall unknown_table_name_6f8e00(int id) (0x006F8E00): the name of a language; "unknown" when it
// is not in the table.
char const *__fastcall unknown_table_name_6f8e00(int id)
{
    LanguageInfo *entry = unknown_table_find_6f8dd0(id);
    if (entry != 0) {
        return entry->Name;
    }
    return "unknown";
}


// The string table unit (0x006F8BE0-0x006FA1DF; RA2 only): loads the localized strings from <name>.csf (or,
// failing that, the quoted entries of the text file <name>) into a label index searched by
// StringTable::Load_String (0x006F9EC0).

extern "C" void *__cdecl fopen(char const *name, char const *mode);                           // 0x00783A45
extern "C" char *__cdecl fgets(char *buffer, int count, void *file);                           // 0x00785547
extern "C" int __cdecl fclose(void *file);                                                     // 0x0078395B
extern "C" void __cdecl qsort(void *base, unsigned int count, unsigned int width,
                              int (__cdecl *compare)(void const *, void const *));             // 0x00781D46
extern "C" char *__cdecl strchr(char const *string, int character);                           // 0x00784130
extern "C" int __cdecl _stricmp(char const *left, char const *right);                          // 0x00781F20
int __cdecl Unknown_isspace_00784aa8(short character);                                         // 0x00784AA8
extern SessionClass unknown_session_a3d298;
extern MapSeedClass unknown_map_seed_a6fd68;


void __fastcall Unknown_006F92A0(void *file, char *source, char *text, char *label, int limit);   // 0x006F92A0
int __fastcall Read_CSF_File_006f9400(char const *name);                                        // 0x006F9400
int __fastcall Read_Text_File_006f9790();                                                                   // 0x006F9790

// Opens a text file for reading.
static void *Open_File(char const *name)
{
    return fopen(name, "r");
}

// Closes a text file.
static void Close_File(void *file)
{
    fclose(file);
}

// The table sizes and language from the header of a .csf file; false when it is not one.
static int Read_CSF_Header(char const *name)
{
    CCFileClass file(name);
    int ok = 0;
    if (file.Is_Available() && file.Open(FILE_RIGHTS_READ)) {
        CSFHeader header;
        if (file.Read(&header, sizeof(header)) == sizeof(header) && header.ID == 'CSF ') {
            unknown_label_count_ad0438 = header.LabelCount;
            unknown_string_count_ad043c = header.StringCount;
            unknown_language_7f8da0 = header.Version < 2 ? 0 : header.Language;
            ok = 1;
        }
        return ok;
    }
    return 0;
}

// Drops leading and trailing white space.
static void Strip_Line(char *line)
{
    char *in = line;
    char c = *in;
    while (c != 0 && Unknown_isspace_00784aa8(c)) {
        c = *++in;
    }
    char *out = line;
    while ((*out++ = *in++) != 0) {
    }
    out -= 2;
    while (out > line && *out != 0 && Unknown_isspace_00784aa8(*out)) {
        out--;
    }
    out[1] = 0;
}

// int __fastcall String_Table_Init_006f8e30(char const *filename) (0x006F8E30): loads the table once; 1 when it
// holds strings. The body runs to 0x006F929D (the frozen inventory row stopped at 0x006F91F5, inside it).
int __fastcall String_Table_Init_006f8e30(char const *filename)
{
    if (unknown_initialized_ad044c != 0) {
        return 1;
    }
    unknown_initialized_ad044c = 1;
    unknown_max_label_length_ad0428 = 0;
    unknown_file_name_ad0434 = filename;
    if (filename == 0 || *filename == 0) {
        return 0;
    }
    strcpy(unknown_csf_name_ac5388, filename);
    char *dot = strchr(unknown_csf_name_ac5388, '.');
    if (dot != 0) {
        *dot = 0;
    }
    strcat(unknown_csf_name_ac5388, ".csf");
    int csf;
    if (Read_CSF_Header(unknown_csf_name_ac5388)) {
        csf = 1;
    } else {
        if (unknown_allow_text_7f8ce8 == 0) {
            return 0;
        }
        unknown_text_file_ad0430 = fopen(filename, "r");
        if (unknown_text_file_ad0430 == 0) {
            return 0;
        }
        unknown_label_count_ad0438 = 0;
        unknown_string_count_ad043c = 0;
        while (true) {
            if (fgets(unknown_line_acf410, 0xFFF, unknown_text_file_ad0430) == 0) {
                break;
            }
            Strip_Line(unknown_line_acf410);
            if (unknown_line_acf410[0] == '"') {
                int length = strlen(unknown_line_acf410);
                unknown_line_acf410[length] = '\n';
                unknown_line_acf410[length + 1] = 0;
                Unknown_006F92A0(unknown_text_file_ad0430, &unknown_line_acf410[1], unknown_text_ac3378, unknown_label_ac4380, 0x1000);
                unknown_string_count_ad043c++;
                continue;
            }
            if (_stricmp(unknown_line_acf410, "END") == 0) {
                unknown_label_count_ad0438++;
            }
        }
        unknown_label_count_ad0438 += 500;
        unknown_string_count_ad043c += 1000;
        Close_File(unknown_text_file_ad0430);
        csf = 0;
    }
    if (unknown_string_count_ad043c == 0 || unknown_label_count_ad0438 == 0) {
        return 0;
    }
    unknown_labels_ad0440 = (StringLabel *)::operator new(unknown_label_count_ad0438 * sizeof(StringLabel));
    if (unknown_labels_ad0440 == 0) {
        return 0;
    }
    memset(unknown_labels_ad0440, 0, unknown_label_count_ad0438 * sizeof(StringLabel));
    unknown_texts_ad0444 = (wchar_t **)::operator new(unknown_string_count_ad043c * sizeof(wchar_t *));
    if (unknown_texts_ad0444 == 0) {
        return 0;
    }
    memset(unknown_texts_ad0444, 0, unknown_string_count_ad043c * sizeof(wchar_t *));
    unknown_speech_ad0448 = (char **)::operator new(unknown_string_count_ad043c * sizeof(char *));
    if (unknown_speech_ad0448 == 0) {
        return 0;
    }
    memset(unknown_speech_ad0448, 0, unknown_string_count_ad043c * sizeof(char *));
    if (csf == 0) {
        unknown_text_file_ad0430 = Open_File(filename);
        if (unknown_text_file_ad0430 == 0) {
            return 0;
        }
        if (!Read_Text_File_006f9790()) {
            Close_File(unknown_text_file_ad0430);
            return 0;
        }
        Close_File(unknown_text_file_ad0430);
    } else {
        Read_CSF_File_006f9400(unknown_csf_name_ac5388);
    }
    qsort(unknown_labels_ad0440, unknown_label_count_ad0438, sizeof(StringLabel), (int (__cdecl *)(void const *, void const *))_stricmp);
    Debug_Printf("MaxLabelLen = %d\n", unknown_max_label_length_ad0428);
    Debug_Printf("Language: %s\n", unknown_table_name_6f8e00(unknown_language_7f8da0));
    unknown_session_a3d298.Unknown_0066EAA0();
    unknown_map_seed_a6fd68.Set_Default_Description_00576fe0();
    return 1;
}


// The .csf reader of the string table unit (0x006F8BE0-0x006FA1DF).

// Drops repeated spaces and the spaces at the ends of lines.
static inline void Collapse_White_Space(wchar_t *string)
{
    wchar_t *in = string;
    wchar_t previous = 0;
    int line_start = 1;
    wchar_t *out = string;
    for (wchar_t c = *in++; c != 0; c = *in++) {
        if (c == ' ') {
            if (previous == c || line_start) {
                continue;
            }
        } else if (c == '\n' || c == '\t') {
            if (previous == ' ') {
                out--;
            }
            *out = c;
            line_start = 1;
            previous = c;
            out++;
            continue;
        }
        *out++ = c;
        previous = c;
        line_start = 0;
    }
    if (previous == ' ') {
        out--;
    }
    *out = 0;
}

// Undoes the .csf string encoding (every character inverted).
static void Decode_String(wchar_t *string)
{
    for (; *string != 0; string++) {
        *string = ~*string;
    }
}

// int __fastcall Read_CSF_File_006f9400(char const *name) (0x006F9400): reads the labels and strings of a .csf
// file into the tables Init sized from its header; 0 when the file is short or malformed.
int __fastcall Read_CSF_File_006f9400(char const *name)
{
    CCFileClass file(name);
    int string_index = 0;
    if (!file.Is_Available() || !file.Open(FILE_RIGHTS_READ)) {
        return 0;
    }
    CSFHeader header;
    if (file.Read(&header, sizeof(header)) != sizeof(header)) {
        return 0;
    }
    int id;
    int count;
    int length;
    int label = 0;
    while (file.Read(&id, 4) == 4) {
        if (id != 'LBL ') {
            return 0;
        }
        file.Read(&count, 4);
        file.Read(&length, 4);
        if (length != 0) {
            file.Read(unknown_line_acf410, length);
        }
        unknown_line_acf410[length] = 0;
        strcpy(unknown_labels_ad0440[label].Name, unknown_line_acf410);
        if (length > unknown_max_label_length_ad0428) {
            unknown_max_label_length_ad0428 = length;
        }
        unknown_labels_ad0440[label].FirstString = string_index;
        unknown_labels_ad0440[label].StringCount = count;
        for (int index = 0; index < count; index++) {
            file.Read(&id, 4);
            if (id != 'STR ' && id != 'STRW') {
                return 0;
            }
            file.Read(&length, 4);
            if (length != 0) {
                file.Read(unknown_wide_text_ac93fc, length * 2);
            }
            unknown_wide_text_ac93fc[length] = 0;
            Decode_String(unknown_wide_text_ac93fc);
            Collapse_White_Space(unknown_wide_text_ac93fc);
            unknown_texts_ad0444[string_index + index] = (wchar_t *)::operator new((wcslen(unknown_wide_text_ac93fc) + 1) * sizeof(wchar_t));
            wcscpy(unknown_texts_ad0444[string_index + index], unknown_wide_text_ac93fc);
            if (id == 'STRW') {
                file.Read(&length, 4);
                if (length != 0) {
                    file.Read(unknown_line_acf410, length);
                }
                unknown_line_acf410[length] = 0;
                if (length != 0) {
                    unknown_speech_ad0448[string_index + index] = (char *)::operator new(length + 1);
                    strcpy(unknown_speech_ad0448[string_index + index], unknown_line_acf410);
                }
            }
        }
        label++;
        string_index += count;
    }
    return 1;
}


// void __fastcall Reverse_String_006f9d50(char *start, char *end) (0x006F9D50): reverses the characters from
// start to end in place; when the first character is upper case and the last lower case, their cases swap.
void __fastcall Reverse_String_006f9d50(char *start, char *end)
{
    int first = 1;
    while (true) {
        if (start >= end) {
            return;
        }
        char left = *start;
        char right = *end;
        if (first) {
            if (left >= 'A' && left <= 'Z' && right >= 'a' && right <= 'z') {
                left += 'a' - 'A';
                right -= 'a' - 'A';
            }
            first = 0;
        }
        *end-- = left;
        *start = right;
        start++;
    }
}

// Not exact (RA2_NON_MATCHING): Read_Text_File_006f9790 at 0x006F9790, best score 71.31, 2026-11-13; differs: 440 instructions in the original, 564 in the draft (shape 87.05)
#ifdef RA2_NON_MATCHING
extern "C" char *__cdecl fgets(char *buffer, int count, void *file);                           // 0x00785547
extern "C" int __cdecl _stricmp(char const *left, char const *right);                          // 0x00781F20
int __cdecl Unknown_isspace_00784aa8(short character);                                         // 0x00784AA8
void __fastcall Unknown_006F92A0(void *file, char *source, char *text, char *label, int limit);   // 0x006F92A0
void __fastcall Reverse_String_006f9d50(char *start, char *end);                                 // 0x006F9D50

// Converts a text-file string to wide characters, expanding its escapes; with the text option set, runs of
// letters are reversed first (format specifications and escaped characters stay in place).
static void Convert_String(wchar_t *wide, char *text)
{
    int escape = 0;
    if (unknown_text_option_ad0450 != 0) {
        char *run = 0;
        char *reversed = unknown_reversed_text_acd410;
        char *next = text;
        char c;
        while ((c = *next++) != 0) {
            if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
                if (run == 0) {
                    run = reversed;
                }
                *reversed++ = c;
            } else {
                if (run != 0) {
                    Reverse_String_006f9d50(run, reversed - 1);
                    run = 0;
                }
                *reversed++ = c;
                if (c == '\\') {
                    *reversed++ = *next++;
                } else if (c == '%') {
                    char f = *next++;
                    while (f != 0 && !((f >= 'a' && f <= 'z') || (f >= 'A' && f <= 'Z'))) {
                        *reversed++ = f;
                        f = *next++;
                    }
                    *reversed++ = f;
                }
            }
        }
        if (run != 0) {
            Reverse_String_006f9d50(run, reversed - 1);
        }
        *reversed = 0;
        text = unknown_reversed_text_acd410;
    }
    while (*text != 0) {
        if (escape == 1) {
            escape = 0;
            switch (*text) {
                case 0:
                    return;
                case '\\':
                    *wide++ = '\\';
                    break;
                case '\'':
                    *wide++ = '\'';
                    break;
                case '"':
                    *wide++ = '"';
                    break;
                case '?':
                    *wide++ = '?';
                    break;
                case 't':
                    *wide++ = '\t';
                    break;
                case 'n':
                    *wide++ = '\n';
                    break;
                default:
                    *wide++ = *text & 0xFF;
                    break;
            }
        } else if (*text != '\\') {
            *wide++ = *text & 0xFF;
        } else {
            escape = 1;
        }
        text++;
    }
    *wide = 0;
}

// int Read_Text_File_006f9790() (0x006F9790): reads the labels of the open text file (a label line, its quoted
// strings, then END) into the tables Init sized; 0 when the file ends inside a label.
int __fastcall Read_Text_File_006f9790()
{
    int label = 0;
    int string_index = 0;
    while (true) {
        if (fgets(unknown_line_acf410, 0x1000, unknown_text_file_ad0430) == 0) {
            break;
        }
        Strip_Line(unknown_line_acf410);
        if (*(unsigned short *)unknown_line_acf410 == '//') {
            continue;
        }
        if (unknown_line_acf410[0] == 0) {
            continue;
        }
        for (int index = 0; index < label; index++) {
            if (_stricmp(unknown_labels_ad0440[index].Name, unknown_line_acf410) == 0) {
            }
        }
        strcpy(unknown_labels_ad0440[label].Name, unknown_line_acf410);
        int length = strlen(unknown_line_acf410);
        if (length > unknown_max_label_length_ad0428) {
            unknown_max_label_length_ad0428 = length;
        }
        unknown_labels_ad0440[label].FirstString = string_index;
        int count = 0;
        while (true) {
            if (fgets(unknown_line_acf410, 0xFFF, unknown_text_file_ad0430) == 0) {
                return 0;
            }
            Strip_Line(unknown_line_acf410);
            if (unknown_line_acf410[0] == '"') {
                int line_length = strlen(unknown_line_acf410);
                unknown_line_acf410[line_length] = '\n';
                unknown_line_acf410[line_length + 1] = 0;
                Unknown_006F92A0(unknown_text_file_ad0430, &unknown_line_acf410[1], unknown_text_ac3378, unknown_label_ac4380, 0x1000);
                Convert_String(unknown_wide_text_ac93fc, unknown_text_ac3378);
                Collapse_White_Space(unknown_wide_text_ac93fc);
                int wide_length = wcslen(unknown_wide_text_ac93fc) + 1;
                unknown_texts_ad0444[string_index + count] = new wchar_t[wide_length];
                wcscpy(unknown_texts_ad0444[string_index + count], unknown_wide_text_ac93fc);
                int speech_length = strlen(unknown_label_ac4380) + 1;
                if (speech_length > 1) {
                    unknown_speech_ad0448[string_index + count] = new char[speech_length];
                    strcpy(unknown_speech_ad0448[string_index + count], unknown_label_ac4380);
                }
                count++;
                continue;
            }
            if (_stricmp(unknown_line_acf410, "END") == 0) {
                unknown_labels_ad0440[label].StringCount = count;
                label++;
                string_index += count;
                break;
            }
        }
    }
    return 1;
}
#endif


// void Unknown_006F9D90() (0x006F9D90)
// Frees the string manager's data (hypothesis: the text manager of 0x006F9EC0): the chunk
// list at 0x00AD0458, the two pointer tables (0x00AD0444, 0x00AD0448) of
// 0x00AD043C entries with their entries, the table 0x00AD0440 and the chunk
// list at 0x00AD0454; the counter 0x00AD044C resets.

struct UnknownTextChunk {
    UnknownTextChunk *Next;
};
extern UnknownTextChunk *unknown_chunks_ad0458;
extern void **unknown_table_ad0444;
extern void **unknown_table_ad0448;
extern void *unknown_index_ad0440;
extern UnknownTextChunk *unknown_chunks_ad0454;
extern int unknown_count_ad043c;
extern int unknown_total_ad044c;

void Unknown_006F9D90()
{
    while (unknown_chunks_ad0458) {
        UnknownTextChunk *chunk = unknown_chunks_ad0458;
        unknown_chunks_ad0458 = chunk->Next;
        delete chunk;
    }
    unknown_chunks_ad0458 = 0;
    if (unknown_table_ad0444) {
        for (int index = 0; index < unknown_count_ad043c; index++) {
            if (unknown_table_ad0444[index]) {
                delete unknown_table_ad0444[index];
            }
        }
        delete unknown_table_ad0444;
        unknown_table_ad0444 = 0;
    }
    if (unknown_table_ad0448) {
        for (int index = 0; index < unknown_count_ad043c; index++) {
            if (unknown_table_ad0448[index]) {
                delete unknown_table_ad0448[index];
            }
        }
        delete unknown_table_ad0448;
        unknown_table_ad0448 = 0;
    }
    if (unknown_index_ad0440) {
        delete unknown_index_ad0440;
        unknown_index_ad0440 = 0;
    }
    UnknownTextChunk *chunk = unknown_chunks_ad0454;
    while (chunk) {
        UnknownTextChunk *next = chunk->Next;
        delete chunk;
        chunk = next;
    }
    unknown_chunks_ad0454 = 0;
    unknown_total_ad044c = 0;
}


// 0x006f9f80: function not yet identified
//
// Body: 44 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: bool FUN_006f9f80(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//
// Adjusted from the Ghidra seed by permute_seeds.py (signedness, comparison order, return width) to
// match the original; edit freely, this file is no longer regenerated.

namespace seed_006f9f80 {

extern unsigned int  stricmp_00781f20(unsigned char *first_buffer, unsigned char *second_buffer);
extern "C" unsigned int DAT_007f8da0;
extern "C" unsigned int DAT_007f8e34;

int  FUN_006f9f80(void)
{
    int entry_or_comparison_result;

    entry_or_comparison_result = (int)(*(int *)(DAT_007f8da0 * 0x10 + 0x7f8cfc));
    if (entry_or_comparison_result == 0) {
        return true;
    }
    entry_or_comparison_result = (int)(stricmp_00781f20((unsigned char *)(entry_or_comparison_result), (unsigned char *)(&DAT_007f8e34)));
    return entry_or_comparison_result == 0;
}

}  // namespace seed_006f9f80


// void __fastcall Set_Text_Option_006f9fb0(int value) -- 0x006F9FB0
// Purpose: when the text option at 0x00AD0450 differs from value, frees the text
// tables (0x006F9D90), stores value and has the object at 0x00AD0434 run 0x006F8E30.
// Parse_Command_Line sets it from the -jabber switch with the value in ecx.
// Assumptions: the option's meaning and the observer class are unidentified.

struct UnknownObserver_006f9fb0 {
    void unknown_6f8e30();                  // 0x006F8E30
};
extern UnknownObserver_006f9fb0 *unknown_observer_ad0434;

extern int unknown_text_option_ad0450;       // 0x00AD0450
void Unknown_006F9D90();                    // 0x006F9D90 frees the text tables

void __fastcall Set_Text_Option_006f9fb0(int value)
{
    if (unknown_text_option_ad0450 != value) {
        Unknown_006F9D90();
        unknown_text_option_ad0450 = value;
        unknown_observer_ad0434->unknown_6f8e30();
    }
}


// 0x006fa010: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 FUN_006fa010(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006fa010 {

extern "C" unsigned int DAT_00ad0450;

unsigned int  FUN_006fa010(void)
{
    return DAT_00ad0450;
}

}  // namespace seed_006fa010


// 0x006fa020: function not yet identified
//
// Body: 6 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 FUN_006fa020(void)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_006fa020 {

extern "C" unsigned int DAT_007f8ce8;

unsigned int  FUN_006fa020(void)
{
    return DAT_007f8ce8;
}

}  // namespace seed_006fa020


class Leaf_6fa030 { public: unsigned int get(); };
unsigned int Leaf_6fa030::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int global_value; unsigned int unused_value; global_value = *(unsigned int*)(0xad044c); return global_value; }
