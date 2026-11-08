// NON-EXACT DRAFT 0x6F9790 FUN_006f9790
// entry 0x6F9790; exact-match score 82.59 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout909 (build/class-work/Scout909/keep/rtr1.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
#include "shared/string_table_file.hpp"
#include "shared/crt_string.hpp"
extern "C" char *__cdecl fgets(char *buffer, int count, void *file);                           // 0x00785547
extern "C" int __cdecl _stricmp(char const *left, char const *right);                          // 0x00781F20
int __cdecl Unknown_isspace_00784aa8(short character);                                         // 0x00784AA8
void __fastcall Unknown_006F92A0(void *file, char *source, char *text, char *label, int limit);   // 0x006F92A0
void __fastcall Reverse_String_006f9d50(char *start, char *end);                                 // 0x006F9D50
// @ref ?unknown_text_file_ad0430@@3PAXA=00ad0430
// @ref ?unknown_text_option_ad0450@@3HA=00ad0450
// @ref ?unknown_text_ac3378@@3PADA=00ac3378
// @ref ?unknown_label_ac4380@@3PADA=00ac4380
// @ref ?unknown_reversed_text_acd410@@3PADA=00acd410
// @ref ?Unknown_isspace_00784aa8@@YAHF@Z=00784aa8
// @ref ?Unknown_006F92A0@@YIXPAXPAD11H@Z=006f92a0
// @ref ?Reverse_String_006f9d50@@YIXPAD0@Z=006f9d50
// @ref _fgets=00785547
// @ref __stricmp=00781f20
// @ref ??_C@_03PIHE@END?$AA@=007f8e64

// Copies text to the reversal buffer with each run of letters reversed (format specifications and escaped
// characters stay in place).
static void Reverse_Letter_Runs(char *text)
{
    char *run = 0;
    char *reversed = unknown_reversed_text_acd410;
    char *next = text;
    char c = *next++;
    while (c != 0) {
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
        c = *next++;
    }
    if (run != 0) {
        Reverse_String_006f9d50(run, reversed - 1);
    }
    *reversed = 0;
}

// Converts a text-file string to wide characters, expanding its escapes; with the text option set, runs of
// letters are reversed first (format specifications and escaped characters stay in place).
static void Convert_String(wchar_t *wide, char *text)
{
    char *in = text;
    int escape = 0;
    wchar_t *out = wide;
    if (unknown_text_option_ad0450 != 0) {
        Reverse_Letter_Runs(text);
        in = unknown_reversed_text_acd410;
    }
    while (*in != 0) {
        if (escape == 1) {
            escape = 0;
            switch (*in) {
                case 0:
                    return;
                case '\\':
                    *out++ = '\\';
                    break;
                case '\'':
                    *out++ = '\'';
                    break;
                case '"':
                    *out++ = '"';
                    break;
                case '?':
                    *out++ = '?';
                    break;
                case 't':
                    *out++ = '\t';
                    break;
                case 'n':
                    *out++ = '\n';
                    break;
                default:
                    *out++ = *in & 0xFF;
                    break;
            }
        } else if (*in != '\\') {
            *out++ = *in & 0xFF;
        } else {
            escape = 1;
        }
        in++;
    }
    *out = 0;
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
        int duplicate = 0;
        for (int index = 0; index < label; index++) {
            if (_stricmp(unknown_labels_ad0440[index].Name, unknown_line_acf410) == 0) {
                duplicate = 1;
            }
        }
        if (duplicate) {
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
