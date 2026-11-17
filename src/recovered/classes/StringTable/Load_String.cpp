// wchar_t const *__fastcall StringTable::Load_String(char const *label, char *speech, char const *file, int line) (0x006F9EC0)
// Looks a label up (case-insensitively, by binary search over the 0x28-byte
// entries at 0x00AD0440) and answers its text (and, through speech, its
// speech name); a label that is missing answers a fresh "MISSING:'label'"
// text kept on the chunk list at 0x00AD0454 and logs it through the empty
// 0x004068F0. A table that never loaded answers the FATAL text.
#include "shared/string_table.hpp"

struct UnknownTextChunk {
    UnknownTextChunk *Next;
    wchar_t Text[1];
};
extern void *unknown_labels_ad0440;
extern int unknown_label_count_ad0438;
extern wchar_t **unknown_texts_ad0444;
extern char **unknown_speech_ad0448;
extern UnknownTextChunk *unknown_chunks_ad0454;
extern "C" void *__cdecl bsearch(void const *key, void const *base, unsigned int count, unsigned int width,
                                 int (__cdecl *compare)(void const *, void const *));
int __cdecl unknown_compare_781f20(void const *left, void const *right);
void __cdecl Unknown_004068F0(char const *format, ...);

struct UnknownLabelEntry {
    char Label[0x24];
    int Index;                          // +0x24
};

wchar_t const *__fastcall StringTable::Load_String(char const *label, char *speech, char const *file, int line)
{
    if (speech) {
        *(char **)speech = 0;
    }
    if (!unknown_labels_ad0440) {
        return L"***FATAL*** String Manager failed to initilaized properly";
    }
    UnknownLabelEntry *entry = (UnknownLabelEntry *)bsearch(label, unknown_labels_ad0440, unknown_label_count_ad0438, 0x28, unknown_compare_781f20);
    if (entry) {
        if (speech) {
            *(char **)speech = unknown_speech_ad0448[entry->Index];
        }
        return unknown_texts_ad0444[entry->Index];
    }
    UnknownTextChunk *chunk = (UnknownTextChunk *)::operator new(0x208);
    swprintf(chunk->Text, L"MISSING:'%hs'", label, file, line);
    Unknown_004068F0("***NO_STRING*** '%s' -> file:%s line:%d\n", label, file, line);
    chunk->Next = unknown_chunks_ad0454;
    unknown_chunks_ad0454 = chunk;
    return chunk->Text;
}
