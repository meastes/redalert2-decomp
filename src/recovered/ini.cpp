// ini.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/ini.json carries the addresses and the rows that
// stay in their own files.


#include "shared/ini_class.hpp"
#include "shared/buffer_straw.hpp"
#include "shared/base64_straw.hpp"
#include "shared/buffer_pipe.hpp"
#include "shared/base64_pipe.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/com_api.hpp"
#include "shared/win32_api.hpp"
#include "shared/wide_string.hpp"
#include "shared/crt_tokens.hpp"
#include "shared/string_table.hpp"
#include "shared/pkey.hpp"


class Leaf_50b8b0 { public: void get(); };
void Leaf_50b8b0::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int zero_value; unsigned int unused_value; zero_value = 0; *(unsigned int*)(0xa411b8) = zero_value; *(unsigned int*)(0xa411bc) = zero_value; *(unsigned int*)(0xa411c0) = zero_value; *(unsigned int*)(0xa411c4) = zero_value; }


// INIClass::~INIClass (0x0050B8D0)
// Releases every section, entry and comment line, then the section index and
// section list (member destructors).

INIClass::~INIClass()
{
    Clear();
}


// INIClass::Clear (0x0050B9A0)
// With no section: deletes all sections (and their entries), empties the
// section index and frees the comment lines kept from the file head. With a
// section only: removes that section. With both: removes that entry from the
// section. Missing sections or entries are ignored; always returns true.

bool INIClass::Clear(char const *section, char const *entry)
{
    if (section == 0) {
        SectionList.Delete();
        SectionIndex.Clear();
        while (LineComments != 0) {
            free(LineComments->Value);
            INIComment *next = LineComments->Next;
            delete LineComments;
            LineComments = next;
        }
    } else {
        INISection *secptr = Find_Section(section);
        if (secptr != 0) {
            if (entry != 0) {
                INIEntry *entptr = secptr->Find_Entry(entry);
                if (entptr != 0) {
                    secptr->EntryIndex.Remove_Index(entptr->Index_ID());
                    delete entptr;
                }
            } else {
                SectionIndex.Remove_Index(secptr->Index_ID());
                delete secptr;
            }
        }
    }
    return true;
}


// INIClass::Load (0x0050BBE0)
// Loads the INI text of a file (see Load(Straw &)) by reading it
// through a FileStraw.

int INIClass::Load(FileClass &file, bool keep_comments)
{
    FileStraw straw(file);
    return Load(straw, keep_comments);
}


// INIClass::Load (0x0050BC30)
// Parses INI text from a straw (read through a 4 KB CacheStraw, lines of up
// to 511 bytes).
//
// An empty database is built directly: each "[name]" line starts a section,
// "key=value" lines become entries (key and value trimmed; lines without a
// key or value, or starting with ';' or '=', are skipped) and an empty
// section is dropped. With keep_comments every raw line is also kept as a
// comment line: the lines above a section header belong to the section, the
// lines above an entry to the entry (the entry's own line is blanked), the
// entry's trailing comment and column positions are recorded, and lines
// after the last entry become LineComments.
//
// A database that already has sections is merged instead: comments are not
// kept and every entry goes through Put_String, replacing existing values.
//
// Returns false when the text has no section at all (without keep_comments)
// or when memory runs out (the database is then cleared, except in a merge).

int INIClass::Load(Straw &file, bool keep_comments)
{
    bool end_of_file = false;
    INIComment *comment_head = 0;
    INIComment *comment_tail = 0;
    INIComment *comment = 0;
    bool merge = false;
    if (Section_Count() > 0) {
        merge = true;
        keep_comments = false;
    }

    CacheStraw ffile;
    ffile.Get_From(file);

    char buffer[512];

    // Comment lines before the first section.
    while (!end_of_file) {
        Read_Line(ffile, buffer, sizeof(buffer), end_of_file);
        if (end_of_file) {
            if (keep_comments) {
                LineComments = comment_head;
                return true;
            }
            while (comment_head != 0) {
                free(comment_head->Value);
                INIComment *next = comment_head->Next;
                delete comment_head;
                comment_head = next;
            }
            return false;
        }
        if (Is_Section_Header(buffer)) {
            break;
        }
        if (keep_comments) {
            comment = new INIComment;
            if (comment == 0) {
                keep_comments = false;
            }
        }
        if (keep_comments) {
            if (comment_tail != 0) {
                comment_tail->Next = comment;
                comment_tail = comment;
            } else {
                comment_tail = comment;
                comment_head = comment;
            }
            comment_tail->Value = _strdup(buffer);
        }
    }

    if (merge) {
        while (!end_of_file) {
            char section[512];
            section[0] = '\0';
            strtrim(buffer);
            if (buffer[0] == '[') {
                char *end = strchr(buffer, ']');
                if (end != 0) {
                    *end = '\0';
                }
                strcpy(section, &buffer[1]);
            }

            while (!end_of_file) {
                int length = Read_Line(ffile, buffer, sizeof(buffer), end_of_file);
                if (Is_Section_Header(buffer)) {
                    break;
                }
                Strip_Comments(buffer);
                if (length == 0 || buffer[0] == ';' || buffer[0] == '=') {
                    continue;
                }
                char *divider = strchr(buffer, '=');
                if (divider != 0) {
                    *divider++ = '\0';
                    strtrim(buffer);
                    if (strlen(buffer) == 0) {
                        continue;
                    }
                    strtrim(divider);
                    if (strlen(divider) == 0) {
                        continue;
                    }
                    if (!Put_String(section, buffer, divider)) {
                        return false;
                    }
                }
            }
        }
    } else {
        while (LineComments != 0) {
            free(LineComments->Value);
            INIComment *next = LineComments->Next;
            delete LineComments;
            LineComments = next;
        }

        while (!end_of_file) {
            strtrim(buffer);
            if (buffer[0] == '[') {
                char *end = strchr(buffer, ']');
                if (end != 0) {
                    *end = '\0';
                }
            } else {
                buffer[0] = '\0';
                buffer[1] = '\0';
            }

            INISection *secptr = new INISection(_strdup(&buffer[1]), comment_head);
            if (secptr == 0) {
                while (comment_head != 0) {
                    free(comment_head->Value);
                    INIComment *next = comment_head->Next;
                    delete comment_head;
                    comment_head = next;
                }
                Clear();
                return false;
            }
            comment_tail = 0;
            comment_head = 0;

            while (!end_of_file) {
                int length = Read_Line(ffile, buffer, sizeof(buffer), end_of_file);
                if (end_of_file) {
                    break;
                }
                if (Is_Section_Header(buffer)) {
                    break;
                }
                if (keep_comments) {
                    comment = new INIComment;
                    if (comment == 0) {
                        keep_comments = false;
                    }
                }
                if (keep_comments) {
                    if (comment_tail != 0) {
                        comment_tail->Next = comment;
                        comment_tail = comment;
                    } else {
                        comment_tail = comment;
                        comment_head = comment;
                    }
                    comment_tail->Value = _strdup(buffer);
                }

                int equals_column = 0;
                int value_column = 0;
                int comment_column = 0;
                char *comment_string = 0;
                if (keep_comments) {
                    comment_string = Locate_Line_Comment(buffer, equals_column, value_column, comment_column);
                    if (comment_string != 0) {
                        comment_string = _strdup(comment_string);
                    }
                }
                Strip_Comments(buffer);
                if (length == 0 || buffer[0] == ';' || buffer[0] == '=') {
                    free(comment_string);
                    continue;
                }
                char *divider = strchr(buffer, '=');
                if (divider == 0) {
                    free(comment_string);
                    continue;
                }
                *divider++ = '\0';
                strtrim(buffer);
                if (strlen(buffer) == 0) {
                    free(comment_string);
                    continue;
                }
                strtrim(divider);
                if (strlen(divider) == 0) {
                    free(comment_string);
                    continue;
                }

                // The entry's own line is not a comment of the entry.
                if (keep_comments && comment_tail != 0 && comment_tail->Value != 0) {
                    free(comment_tail->Value);
                    comment_tail->Value = 0;
                }

                INIEntry *entryptr = new INIEntry(_strdup(buffer), _strdup(divider), comment_head, comment_string,
                                                  comment_column, equals_column, value_column);
                if (entryptr == 0) {
                    free(comment_string);
                    while (comment_head != 0) {
                        free(comment_head->Value);
                        INIComment *next = comment_head->Next;
                        delete comment_head;
                        comment_head = next;
                    }
                    delete secptr;
                    Clear();
                    return false;
                }
                comment_tail = 0;
                comment_head = 0;

                secptr->EntryIndex.Add_Index(entryptr->Index_ID(), entryptr);
                secptr->EntryList.Add_Tail(entryptr);
            }

            if (!keep_comments && !secptr->EntryList.First()->Is_Valid()) {
                while (comment_head != 0) {
                    free(comment_head->Value);
                    INIComment *next = comment_head->Next;
                    delete comment_head;
                    comment_head = next;
                }
                delete secptr;
            } else {
                SectionIndex.Add_Index(secptr->Index_ID(), secptr);
                SectionList.Add_Tail(secptr);
            }
        }
    }

    LineComments = comment_head;
    return true;
}


// INIClass::Save (0x0050C5E0)
// Writes the database as INI text to a file through a FilePipe.

int INIClass::Save(FileClass &file) const
{
    FilePipe pipe(file);
    return Save(pipe);
}


// INIClass::Save (0x0050C620)
// Writes the database as INI text (CRLF line ends) and ends the pipe.
// Sections are separated by a blank line unless they have comment lines
// above them. Kept comment lines are written back above their section or
// entry, entries are padded with spaces so that '=', the value and a
// trailing ";comment" return to their loaded columns (at most 256 spaces
// each), and the LineComments follow the last section. Returns the byte
// count reported by the pipe.

int INIClass::Save(Pipe &pipe) const
{
    int total = 0;
    char spaces[512];
    memset(spaces, ' ', sizeof(spaces));
    spaces[sizeof(spaces) - 1] = '\0';

    INISection *secptr = SectionList.First();
    while (secptr != 0 && secptr->Is_Valid()) {
        if (total > 0 && secptr->Comments == 0) {
            total += pipe.Put("\r\n", strlen("\r\n"));
        }
        for (INIComment *comment = secptr->Comments; comment != 0; comment = comment->Next) {
            if (comment->Value != 0) {
                total += pipe.Put(comment->Value, strlen(comment->Value));
                total += pipe.Put("\r\n", strlen("\r\n"));
            }
        }

        total += pipe.Put("[", 1);
        total += pipe.Put(secptr->Name, strlen(secptr->Name));
        total += pipe.Put("]", 1);
        total += pipe.Put("\r\n", strlen("\r\n"));

        INIEntry *entryptr = secptr->EntryList.First();
        while (entryptr != 0 && entryptr->Is_Valid()) {
            for (INIComment *comment = entryptr->Comments; comment != 0; comment = comment->Next) {
                if (comment->Value != 0) {
                    total += pipe.Put(comment->Value, strlen(comment->Value));
                    total += pipe.Put("\r\n", strlen("\r\n"));
                }
            }

            int key_length = strlen(entryptr->Key);
            int value_length = strlen(entryptr->Value);
            int padding = entryptr->EqualsColumn - key_length;
            int padded = 0;
            if (padding > 256) {
                padding = 256;
            }
            total += pipe.Put(entryptr->Key, key_length);
            if (padding > 0) {
                total += pipe.Put(spaces, padding);
                padded = padding;
            }
            total += pipe.Put("=", 1);

            padding = entryptr->ValueColumn - (key_length + padded + 1);
            if (padding > 256) {
                padding = 256;
            }
            if (padding > 0) {
                total += pipe.Put(spaces, padding);
                padded += padding;
            }
            total += pipe.Put(entryptr->Value, value_length);

            if (entryptr->CommentString != 0) {
                padding = entryptr->CommentColumn - (key_length + value_length + padded + 1);
                if (padding > 256) {
                    padding = 256;
                }
                if (padding > 0) {
                    total += pipe.Put(spaces, padding);
                }
                total += pipe.Put(";", 1);
                total += pipe.Put(entryptr->CommentString, strlen(entryptr->CommentString));
            }
            total += pipe.Put("\r\n", strlen("\r\n"));

            entryptr = entryptr->Next();
        }

        secptr = secptr->Next();
    }

    for (INIComment *comment = LineComments; comment != 0; comment = comment->Next) {
        if (comment->Value != 0) {
            total += pipe.Put(comment->Value, strlen(comment->Value));
            total += pipe.Put("\r\n", strlen("\r\n"));
        }
    }

    total += pipe.End();
    return total;
}


// INIClass::Find_Section (0x0050C9B0)
// Returns the section with this name, or null (also for a null name).

INIClass::INISection *INIClass::Find_Section(char const *section) const
{
    if (section != 0) {
        long crc = CRCEngine()(section, strlen(section));
        if (SectionIndex.Is_Present(crc)) {
            return SectionIndex[crc];
        }
    }
    return 0;
}


// INIClass::Section_Count (0x0050CAF0)
// Number of sections.

int INIClass::Section_Count() const
{
    return SectionIndex.Count();
}


// INIClass::Entry_Count (0x0050CB00)
// Number of entries in the named section; 0 when the section is missing.

int INIClass::Entry_Count(char const *section) const
{
    INISection *secptr = Find_Section(section);
    if (secptr != 0) {
        return secptr->EntryIndex.Count();
    }
    return 0;
}


// INIClass::Entry_Count (0x0050CBD0)
// Wstring overload of Entry_Count.

int INIClass::Entry_Count(Wstring const &section) const
{
    return Entry_Count(section.Peek_Buffer());
}


// INIClass::Find_Entry (0x0050CCA0)
// Returns the entry with this key in the named section, or null.

INIClass::INIEntry *INIClass::Find_Entry(char const *section, char const *entry) const
{
    INISection *secptr = Find_Section(section);
    if (secptr != 0) {
        return secptr->Find_Entry(entry);
    }
    return 0;
}


// INIClass::Get_Entry (0x0050CE00)
// Key of the index'th entry (in file order) of the named section, or null
// when the section is missing or index is past its last entry.

char const *INIClass::Get_Entry(char const *section, int index) const
{
    INISection *secptr = Find_Section(section);
    if (secptr != 0 && index < secptr->EntryIndex.Count()) {
        INIEntry *entptr = secptr->EntryList.First();
        while (entptr != 0 && entptr->Is_Valid()) {
            if (index == 0) {
                return entptr->Key;
            }
            index--;
            entptr = entptr->Next();
        }
    }
    return 0;
}


// INIClass::Get_Entry (0x0050CED0)
// Wstring overload of Get_Entry; returns the key as a new Wstring (empty
// when there is no such entry).

Wstring INIClass::Get_Entry(Wstring const &section, int index) const
{
    return Get_Entry(section.Peek_Buffer(), index);
}


// INIClass::Put_UUBlock (0x0050CFC0)
// Replaces the section with a binary block, Base64 encoded in lines of up
// to 70 characters stored as entries "1", "2", ... False for a null section
// or block or an empty length.

bool INIClass::Put_UUBlock(char const *section, void const *block, int length)
{
    if (section == 0 || block == 0 || length < 1) return false;

    Clear(section);

    BufferStraw straw((void *)block, length);
    Base64Straw bstraw(Base64Straw::ENCODE);
    bstraw.Get_From(straw);

    int counter = 1;

    for (;;) {
        char buffer[71];
        char sbuffer[32];

        int count = bstraw.Get(buffer, sizeof(buffer) - 1);
        buffer[count] = '\0';
        if (count == 0) break;

        sprintf(sbuffer, "%d", counter);
        Put_String(section, sbuffer, buffer);
        counter++;
    }
    return true;
}


// INIClass::Get_UUBlock (0x0050D0F0)
// Decodes the section's Base64 lines (stored by Put_UUBlock) into block, at
// most length bytes; returns the decoded length.

int INIClass::Get_UUBlock(char const *section, void *block, int length) const
{
    if (section == 0) return 0;

    Base64Pipe b64pipe(Base64Pipe::DECODE);
    BufferPipe bpipe(block, length);

    b64pipe.Put_To(&bpipe);

    int total = 0;
    int counter = Entry_Count(section);
    for (int index = 0; index < counter; index++) {
        char buffer[128];

        int count = Get_String(section, Get_Entry(section, index), "=", buffer, sizeof(buffer));
        int outcount = b64pipe.Put(buffer, count);
        total += outcount;
    }
    total += b64pipe.End();
    return total;
}


// INIClass::Put_TextBlock (0x0050D310)
// Replaces the section with the text split into entries "1", "2", ...: each
// holds at most 75 characters, a longer piece broken back at a whitespace
// byte, trimmed. Always true once the section is named.

bool INIClass::Put_TextBlock(char const *section, char const *text)
{
    if (section == 0) return false;

    Clear(section);

    int index = 1;
    while (text != 0 && *text != '\0') {

        char buffer[128];

        strncpy(buffer, text, 75);
        buffer[75] = '\0';

        char b[32];
        sprintf(b, "%d", index);

        int count = strlen(buffer);
        if (count > 0) {
            if (count >= 75) {
                while (count) {
                    char c = buffer[count];

                    if (c != '\0' && (unsigned char)c <= ' ') break;
                    count--;
                }

                if (count == 0) {
                    break;
                } else {
                    buffer[count] = '\0';
                }
            }

            strtrim(buffer);
            Put_String(section, b, buffer);
            index++;
            text = text + count;
        } else {
            break;
        }
    }
    return true;
}


// INIClass::Get_TextBlock (0x0050D400)
// Joins the section's entries (in order, separated by one space) into
// buffer, stopping when it is full; returns the length written.

int INIClass::Get_TextBlock(char const *section, char *buffer, int length) const
{
    if (length <= 0) return 0;

    buffer[0] = '\0';
    if (length <= 1) return 0;

    int elen = Entry_Count(section);
    int total = 0;
    for (int index = 0; index < elen; index++) {

        if (index > 0) {
            *buffer++ = ' ';
            length--;
            total++;
        }

        Get_String(section, Get_Entry(section, index), unknown_string_83c55c, buffer, length);

        int partial = strlen(buffer);
        total += partial;
        buffer += partial;
        length -= partial;
        if (length <= 1) break;
    }
    return total;
}


// INIClass::Put_Int (0x0050D6E0)
// Stores an integer as text: format 1 writes hex with an 'h' suffix
// ("1Fh"), format 2 hex with a '$' prefix ("$1F"), anything else decimal.

bool INIClass::Put_Int(char const *section, char const *entry, int number, int format)
{
    char buffer[512];

    switch (format) {
        default:
        case 0:
            sprintf(buffer, "%d", number);
            break;

        case 1:
            sprintf(buffer, "%Xh", number);
            break;

        case 2:
            sprintf(buffer, "$%X", number);
            break;
    }
    return Put_String(section, entry, buffer);
}


// INIClass::Put_Int (0x0050D760)
// Wstring overload of Put_Int.

bool INIClass::Put_Int(Wstring const &section, Wstring const &entry, int number, int format)
{
    return Put_Int(section.Peek_Buffer(), entry.Peek_Buffer(), number, format);
}


// INIClass::Get_Int (0x0050D7F0)
// Reads an integer entry. "$1F" and "1Fh" are hexadecimal, anything else is
// decimal (atoi). Returns defvalue when the section, entry or value is
// missing.

int INIClass::Get_Int(char const *section, char const *entry, int defvalue) const
{
    if (section == 0 || entry == 0) {
        return defvalue;
    }

    INIEntry *entptr = Find_Entry(section, entry);
    if (entptr != 0 && entptr->Value != 0) {
        if (*entptr->Value == '$') {
            sscanf(entptr->Value, "$%x", &defvalue);
        } else if (tolower(entptr->Value[strlen(entptr->Value) - 1]) == 'h') {
            sscanf(entptr->Value, "%xh", &defvalue);
        } else {
            defvalue = atoi(entptr->Value);
        }
    }
    return defvalue;
}


// INIClass::Get_Int (0x0050D9D0)
// Wstring overload of Get_Int.

int INIClass::Get_Int(Wstring const &section, Wstring const &entry, int defvalue) const
{
    return Get_Int(section.Peek_Buffer(), entry.Peek_Buffer(), defvalue);
}


// INIClass::Get_UUID (0x0050DA00)
// Reads a class ID written by Put_UUID ("{...}"): the text is widened and
// parsed with CLSIDFromString; defvalue when the entry is missing or empty or
// the text does not parse.

WIN32_IMPORT HRESULT __stdcall CLSIDFromString(unsigned short *text, GUID *clsid);

GUID INIClass::Get_UUID(char const *section, char const *entry, GUID defvalue) const
{
    char buffer[128];

    if (Get_String(section, entry, unknown_string_83c55c, buffer, sizeof(buffer))) {
        unsigned short wide[128];
        GUID value;

        MultiByteToWideChar(0, 1, buffer, -1, wide, 128);
        if (SUCCEEDED(CLSIDFromString(wide, &value))) {
            return value;
        }
    }
    return defvalue;
}


// INIClass::Put_UUID (0x0050DC20)
// Stores a class ID as its registry text ("{...}"): StringFromCLSID, then
// narrowed with WideCharToMultiByte; the wide text is released with
// SysFreeString.

WIN32_IMPORT HRESULT __stdcall StringFromCLSID(GUID const &clsid, unsigned short **text);
WIN32_IMPORT int __stdcall WideCharToMultiByte(unsigned int codepage, unsigned long flags, unsigned short const *wide, int wide_length,
                                              char *narrow, int narrow_length, char const *default_char, int *used_default);
WIN32_IMPORT void __stdcall SysFreeString(unsigned short *string);

bool INIClass::Put_UUID(char const *section, char const *entry, GUID const &value)
{
    unsigned short *wide = 0;
    char buffer[128];

    StringFromCLSID(value, &wide);
    if (!WideCharToMultiByte(0, 0, wide, -1, buffer, sizeof(buffer), 0, 0)) {
        GetLastError();
    }
    SysFreeString(wide);
    return Put_String(section, entry, buffer);
}


// INIClass::Put_Rect (0x0050DCA0)
// Stores a rectangle as "x,y,width,height".

bool INIClass::Put_Rect(char const *section, char const *entry, INIRect const &value)
{
    char buffer[64];

    sprintf(buffer, "%d,%d,%d,%d", value.X, value.Y, value.Width, value.Height);
    return Put_String(section, entry, buffer);
}


// INIClass::Put_Rect (0x0050DCF0)
// Wstring overload of Put_Rect.

bool INIClass::Put_Rect(Wstring const &section, Wstring const &entry, INIRect const &value)
{
    return Put_Rect(section.Peek_Buffer(), entry.Peek_Buffer(), value);
}


// INIClass::Get_Rect (0x0050DD50)
// Reads a "x,y,width,height" rectangle; defvalue when the entry is missing or empty
// (the text is read through Get_String into a 64-byte buffer).

INIRect INIClass::Get_Rect(char const *section, char const *entry, INIRect const &defvalue) const
{
    char buffer[64];

    if (Get_String(section, entry, "0,0,0,0", buffer, sizeof(buffer)) != 0) {
        INIRect value = defvalue;
        sscanf(buffer, "%d,%d,%d,%d", &value.X, &value.Y, &value.Width, &value.Height);
        return value;
    }
    return defvalue;
}


// INIClass::Get_Rect (0x0050DF70)
// Wstring overload of Get_Rect.

INIRect INIClass::Get_Rect(Wstring const &section, Wstring const &entry, INIRect const &defvalue) const
{
    return Get_Rect(section.Peek_Buffer(), entry.Peek_Buffer(), defvalue);
}


// INIClass::Put_Hex (0x0050E180)
// Stores an integer as unprefixed upper-case hex ("1F").

bool INIClass::Put_Hex(char const *section, char const *entry, int number)
{
    char buffer[512];

    sprintf(buffer, "%X", number);
    return Put_String(section, entry, buffer);
}


// INIClass::Put_Hex (0x0050E1D0)
// Wstring overload of Put_Hex.

bool INIClass::Put_Hex(Wstring const &section, Wstring const &entry, int number)
{
    return Put_Hex(section.Peek_Buffer(), entry.Peek_Buffer(), number);
}


// INIClass::Get_Hex (0x0050E230)
// Reads an entry as unprefixed hexadecimal ("%x"); defvalue when the
// section, entry or value is missing.

int INIClass::Get_Hex(char const *section, char const *entry, int defvalue) const
{
    if (section == 0 || entry == 0) {
        return defvalue;
    }

    INIEntry *entptr = Find_Entry(section, entry);
    if (entptr != 0 && entptr->Value != 0) {
        sscanf(entptr->Value, "%x", &defvalue);
    }
    return defvalue;
}


// INIClass::Get_Double (0x0050E3A0)
// Reads an entry as a number (scanned as a float with "%f"); a value that
// contains '%' is a percentage and is scaled by 0.01. Returns defvalue when
// the section, entry or value is missing. The '%' test passes the
// multi-character constant '%%' (0x2525) to strchr, which only looks at its
// low byte.

double INIClass::Get_Double(char const *section, char const *entry, double defvalue) const
{
    if (section == 0 || entry == 0) {
        return defvalue;
    }

    INIEntry *entptr = Find_Entry(section, entry);
    if (entptr != 0 && entptr->Value != 0) {
        float value;
        sscanf(entptr->Value, "%f", &value);
        defvalue = value;
        if (strchr(entptr->Value, '%%') != 0) {
            defvalue = defvalue * 0.01;
        }
    }
    return defvalue;
}


// INIClass::Put_Double (0x0050E540)
// Stores a number with "%f" (six decimals). The value passes through a
// float conversion that /O2 keeps in the FPU (fld/fstp qword, no rounding);
// a plain double argument is copied as two dwords instead.

bool INIClass::Put_Double(char const *section, char const *entry, double number)
{
    char buffer[512];

    sprintf(buffer, "%f", (float)number);
    return Put_String(section, entry, buffer);
}


// INIClass::Put_Double (0x0050E590)
// Wstring overload of Put_Double.

bool INIClass::Put_Double(Wstring const &section, Wstring const &entry, double number)
{
    return Put_Double(section.Peek_Buffer(), entry.Peek_Buffer(), number);
}


// INIClass::Put_String (0x0050E5F0)
// Sets section/entry to a copy of string, creating the section if needed.
// An existing entry is replaced; the comment lines and trailing comment it
// carried (and its three column positions) move to the new entry. A null or
// empty string removes the entry. Returns false on a null section/entry or
// when memory runs out.

bool INIClass::Put_String(char const *section, char const *entry, char const *string)
{
    if (section == 0 || entry == 0) {
        return false;
    }

    INISection *secptr = Find_Section(section);
    if (secptr == 0) {
        secptr = new INISection(_strdup(section));
        if (secptr == 0) {
            return false;
        }
        SectionList.Add_Tail(secptr);
        SectionIndex.Add_Index(secptr->Index_ID(), secptr);
    }

    INIComment *comments = 0;
    char *comment_string = 0;
    int comment_column = 0;
    int equals_column = 0;
    int value_column = 0;
    INIEntry *entptr = secptr->Find_Entry(entry);
    if (entptr != 0) {
        comment_string = entptr->CommentString;
        comment_column = entptr->CommentColumn;
        comments = entptr->Comments;
        equals_column = entptr->EqualsColumn;
        value_column = entptr->ValueColumn;
        entptr->Comments = 0;
        entptr->CommentString = 0;
        secptr->EntryIndex.Remove_Index(entptr->Index_ID());
        delete entptr;
    }

    if (string != 0 && strlen(string) > 0) {
        entptr = new INIEntry(_strdup(entry), _strdup(string), comments, comment_string,
                              comment_column, equals_column, value_column);
        if (entptr == 0) {
            free(comment_string);
            while (comments != 0) {
                free(comments->Value);
                INIComment *next = comments->Next;
                delete comments;
                comments = next;
            }
            return false;
        }
        secptr->EntryList.Add_Tail(entptr);
        secptr->EntryIndex.Add_Index(entptr->Index_ID(), entptr);
    }
    return true;
}


// INIClass::Put_String (0x0050E970)
// Wstring overload of Put_String.

bool INIClass::Put_String(Wstring const &section, Wstring const &entry, Wstring const &string)
{
    return Put_String(section.Peek_Buffer(), entry.Peek_Buffer(), string.Peek_Buffer());
}


// INIClass::Get_String (0x0050E9A0)
// Copies the entry's value (or defvalue when the entry is missing) into
// buffer, truncated to size - 1 characters and trimmed, and returns its
// length. With no value and no default the buffer becomes empty. Returns 0
// for a null section/entry/buffer or a size below 2.

int INIClass::Get_String(char const *section, char const *entry, char const *defvalue, char *buffer, int size) const
{
    if (buffer == 0 || size < 2 || section == 0 || entry == 0) {
        return 0;
    }

    INIEntry *entptr = Find_Entry(section, entry);
    if (entptr != 0 && entptr->Value != 0) {
        defvalue = entptr->Value;
    }

    if (defvalue == 0) {
        buffer[0] = '\0';
        return 0;
    }
    strncpy(buffer, defvalue, size);
    buffer[size - 1] = '\0';
    strtrim(buffer);
    return strlen(buffer);
}


// INIClass::Get_String (0x0050EB40)
// Wstring overload of Get_String: reads into a 1023-character buffer, which
// is then assigned to result (empty when there is no value and no default).
// Returns the length.

int INIClass::Get_String(Wstring const &section, Wstring const &entry, Wstring const &defvalue, Wstring &result) const
{
    char buffer[1024];
    int length = Get_String(section.Peek_Buffer(), entry.Peek_Buffer(), defvalue.Peek_Buffer(), buffer,
                            sizeof(buffer) - 1);
    result = buffer;
    return length;
}


// INIClass::Put_Unicode (0x0050ECF0)
// Stores a wide string as comma-terminated hexadecimal character codes
// ("41,42,"); the text is built in a heap buffer of eight bytes per
// character. Name hypothesis (the reader is Get_Unicode 0x0050EDF0).

extern "C" char *__cdecl _itoa(int value, char *buffer, int radix);
extern "C" char *__cdecl strcat(char *destination, char const *source);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
#pragma intrinsic(strcat)

bool INIClass::Put_Unicode(char const *section, char const *entry, unsigned short const *string)
{
    bool result = false;

    if (string != 0) {
        int length = wcslen(string) * 8;
        char *buffer = new char[length + 1];
        if (buffer != 0) {
            char *ptr = buffer;
            *ptr = 0;
            for (; *string != 0; string++) {
                _itoa(*string, ptr, 16);
                strcat(ptr, ",");
                ptr = buffer + strlen(buffer);
            }
            result = Put_String(section, entry, buffer);
            delete [] buffer;
        }
    }
    return result;
}


// INIClass::Put_Unicode (0x0050EDC0)
// Wstring overload of Put_Unicode (name hypothesis, like the char form).

bool INIClass::Put_Unicode(Wstring const &section, Wstring const &entry, WideString const &string)
{
    return Put_Unicode(section.Peek_Buffer(), entry.Peek_Buffer(), string.unknown_text_770430());
}


// INIClass::Get_Unicode (0x0050EDF0)
// Reads a wide string written by Put_Unicode (comma-separated hexadecimal
// character codes) into buffer, at most size characters, null terminated;
// copies defvalue when the entry is missing or empty. Returns the length.
// Name hypothesis.

extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);

int INIClass::Get_Unicode(char const *section, char const *entry, unsigned short const *defvalue, unsigned short *buffer, int size) const
{
    char text[0x5000];

    int length = Get_String(section, entry, 0, text, sizeof(text));
    if (length == 0) {
        wcsncpy(buffer, defvalue, size);
        return wcslen(buffer);
    }

    char *token = strtok(text, ",");
    unsigned short *ptr = buffer;
    while (token != 0 && size != 0) {
        int value;
        sscanf(token, "%x", &value);
        *ptr++ = (unsigned short)value;
        token = strtok(0, ",");
        size--;
    }
    *ptr = 0;
    return wcslen(buffer);
}


// INIClass::Get_Wide_String (0x0050F010)
// Reads the entry as a string table label (TXT_...) and copies its localized
// text into buffer, at most size characters, null terminated; returns the
// length. Name from CampaignClass::Read_INI's view (shared/campaign.hpp).

int INIClass::Get_Wide_String(char const *section, char const *entry, wchar_t *buffer, int size) const
{
    char label[1024];

    Get_String(section, entry, 0, label, sizeof(label));
    wcsncpy(buffer, StringTable::Load_String(label, 0, "D:\\Ra2\\Ini.CPP", 1827), size);
    buffer[size - 1] = 0;
    return wcslen(buffer);
}


// INIClass::Get_Wide_String (0x0050F1B0)
// As Get_Wide_String 0x0050F010, with a default label: the text of the label
// the entry names, or of defvalue when the entry is missing or empty.

int INIClass::Get_Wide_String(char const *section, char const *entry, wchar_t *buffer, char const *defvalue,
                              int size) const
{
    char label[1024];

    Get_String(section, entry, 0, label, sizeof(label));
    if (strlen(label) == 0) {
        wcsncpy(buffer, StringTable::Load_String(defvalue, 0, "D:\\Ra2\\Ini.CPP", 1841), size);
    } else {
        wcsncpy(buffer, StringTable::Load_String(label, 0, "D:\\Ra2\\Ini.CPP", 1843), size);
    }
    buffer[size - 1] = 0;
    return wcslen(buffer);
}


// INIClass::Put_Bool (0x0050F380)
// Stores a flag as "yes" or "no".

bool INIClass::Put_Bool(char const *section, char const *entry, bool value)
{
    if (value) {
        return Put_String(section, entry, "yes");
    }
    return Put_String(section, entry, "no");
}


// INIClass::Put_Bool (0x0050F3C0)
// Wstring overload of Put_Bool.

bool INIClass::Put_Bool(Wstring const &section, Wstring const &entry, bool value)
{
    return Put_Bool(section.Peek_Buffer(), entry.Peek_Buffer(), value);
}


// INIClass::Get_Bool (0x0050F410)
// Reads a flag from the first character of the value: Y, T or 1 is true,
// N, F or 0 is false (case-insensitive); anything else, or a missing
// section/entry/value, gives defvalue.

bool INIClass::Get_Bool(char const *section, char const *entry, bool defvalue) const
{
    if (section == 0 || entry == 0) {
        return defvalue;
    }

    INIEntry *entptr = Find_Entry(section, entry);
    if (entptr != 0 && entptr->Value != 0) {
        switch (toupper(*entptr->Value)) {
            case 'Y':
            case 'T':
            case '1':
                return true;

            case 'N':
            case 'F':
            case '0':
                return false;
        }
    }
    return defvalue;
}


// INIClass::Put_Point (0x0050F5D0)
// Stores a 2D point as "x,y".

bool INIClass::Put_Point(char const *section, char const *entry, INIPoint2D const &value)
{
    char buffer[56];

    sprintf(buffer, "%d,%d", value.X, value.Y);
    return Put_String(section, entry, buffer);
}


// INIClass::Put_Point (0x0050F610)
// Wstring overload of Put_Point.

bool INIClass::Put_Point(Wstring const &section, Wstring const &entry, INIPoint2D const &value)
{
    return Put_Point(section.Peek_Buffer(), entry.Peek_Buffer(), value);
}


// INIClass::Get_Point (0x0050F670)
// Reads an "x,y" point; defvalue when the entry is missing or empty (the
// text is read through Get_String into a 64-byte buffer).

INIPoint2D INIClass::Get_Point(char const *section, char const *entry, Point2D const &defvalue) const
{
    char buffer[64];

    if (Get_String(section, entry, unknown_string_83c55c, buffer, sizeof(buffer))) {
        int x, y;
        sscanf(buffer, "%d,%d", &x, &y);
        return INIPoint2D(x, y);
    }
    return (INIPoint2D const &)defvalue;
}


// INIClass::Get_Point (0x0050F800)
// Wstring overload of Get_Point.

INIPoint2D INIClass::Get_Point(Wstring const &section, Wstring const &entry, Point2D const &defvalue) const
{
    return Get_Point(section.Peek_Buffer(), entry.Peek_Buffer(), defvalue);
}


// INIClass::Put_Point (0x0050F990)
// Stores a 3D integer point as "x,y,z".

bool INIClass::Put_Point(char const *section, char const *entry, INIPoint3D const &value)
{
    char buffer[56];

    sprintf(buffer, "%d,%d,%d", value.X, value.Y, value.Z);
    return Put_String(section, entry, buffer);
}


// INIClass::Put_Point (0x0050F9E0)
// Wstring overload of Put_Point.

bool INIClass::Put_Point(Wstring const &section, Wstring const &entry, INIPoint3D const &value)
{
    return Put_Point(section.Peek_Buffer(), entry.Peek_Buffer(), value);
}


// INIClass::Get_Point (0x0050FA40)
// Reads a "x,y,z" integer point; defvalue when the entry is missing or empty
// (the text is read through Get_String into a 64-byte buffer).

INIPoint3D INIClass::Get_Point(char const *section, char const *entry, INIPoint3D const &defvalue) const
{
    char buffer[64];

    if (Get_String(section, entry, unknown_string_83c55c, buffer, sizeof(buffer)) != 0) {
        int x, y, z;
        sscanf(buffer, "%d,%d,%d", &x, &y, &z);
        return INIPoint3D(x, y, z);
    }
    return defvalue;
}


// INIClass::Get_Point (0x0050FBF0)
// Wstring overload of Get_Point.

INIPoint3D INIClass::Get_Point(Wstring const &section, Wstring const &entry, INIPoint3D const &defvalue) const
{
    return Get_Point(section.Peek_Buffer(), entry.Peek_Buffer(), defvalue);
}


// INIClass::Put_Vector (0x0050FD90)
// Stores three floats as "x,y,z" ("%f" each).

bool INIClass::Put_Vector(char const *section, char const *entry, INIVector3 const &value)
{
    char buffer[56];

    sprintf(buffer, "%f,%f,%f", value.X, value.Y, value.Z);
    return Put_String(section, entry, buffer);
}


// INIClass::Put_Vector (0x0050FDF0)
// Wstring overload of Put_Vector.

bool INIClass::Put_Vector(Wstring const &section, Wstring const &entry, INIVector3 const &value)
{
    return Put_Vector(section.Peek_Buffer(), entry.Peek_Buffer(), value);
}


// INIClass::Get_Vector (0x0050FE60)
// Reads a "x,y,z" float triple; defvalue when the entry is missing or empty
// (the text is read through Get_String into a 64-byte buffer).

INIVector3 INIClass::Get_Vector(char const *section, char const *entry, INIVector3 const &defvalue) const
{
    char buffer[64];

    if (Get_String(section, entry, unknown_string_83c55c, buffer, sizeof(buffer)) != 0) {
        float x, y, z;
        sscanf(buffer, "%f,%f,%f", &x, &y, &z);
        return INIVector3(x, y, z);
    }
    return defvalue;
}


// INIClass::Get_Vector (0x00510020)
// Wstring overload of Get_Vector.

INIVector3 INIClass::Get_Vector(Wstring const &section, Wstring const &entry, INIVector3 const &defvalue) const
{
    return Get_Vector(section.Peek_Buffer(), entry.Peek_Buffer(), defvalue);
}


// INIClass::INISection::Find_Entry (0x005101D0)
// Out-of-line copy of the inline member (shared/ini_class.hpp): Get_UUBlock
// calls it instead of expanding it.


// INIClass::Put_PKey (0x00510310)
// Stores an RSA key: the DER modulus as [PublicKey] and the DER exponent as
// [PrivateKey], each uuencoded.

bool INIClass::Put_PKey(PKey const &key)
{
    char buffer[512];

    int length = key.Encode_Modulus(buffer);
    Put_UUBlock("PublicKey", buffer, length);

    length = key.Encode_Exponent(buffer);
    Put_UUBlock("PrivateKey", buffer, length);
    return true;
}


// INIClass::Get_PKey (0x00510370)
// Reads an RSA key stored by Put_PKey. The fast key's exponent is the known
// constant 65537, DER encoded here instead of read from [PrivateKey].

PKey INIClass::Get_PKey(bool fast) const
{
    PKey key;
    char buffer[512];

    if (fast) {
        BigInt exponent(0x10001);
        exponent.DEREncode((unsigned char *)buffer);
        key.Decode_Exponent(buffer);
    } else {
        Get_UUBlock("PrivateKey", buffer, sizeof(buffer));
        key.Decode_Exponent(buffer);
    }

    Get_UUBlock("PublicKey", buffer, sizeof(buffer));
    key.Decode_Modulus(buffer);

    return key;
}


// INIClass::Get_Time (0x00510460)
// Reads a duration written by Put_Time ("hh:mm:ss") back as 1/60 s ticks;
// defvalue when the entry is missing.

// "%d:%d:%d" (0x007D4EF8): the literal lives in an earlier unit's pool
// (CoopCampaignClass::Next_Map 0x0048E360 formats with it too).
extern char const time_format_007d4ef8[];

int INIClass::Get_Time(char const *section, char const *entry, int defvalue) const
{
    if (section == 0 || entry == 0) return defvalue;

    INIEntry *entptr = Find_Entry(section, entry);
    if (entptr != 0 && entptr->Value != 0) {
        int hours = 0;
        int minutes = 0;
        int seconds = 0;
        sscanf(entptr->Value, time_format_007d4ef8, &hours, &minutes, &seconds);
        return ((hours * 60 + minutes) * 60 + seconds) * 60;
    }
    return defvalue;
}


// INIClass::Put_Time (0x00510610)
// Stores a duration given in 1/60 s ticks as "hh:mm:ss". The name is a
// hypothesis; the unit follows from the three divisions by 60.

bool INIClass::Put_Time(char const *section, char const *entry, int ticks)
{
    char buffer[32];

    int seconds = ticks / 60;
    int minutes = seconds / 60;
    int hours = minutes / 60;
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes - hours * 60, seconds - minutes * 60);
    return Put_String(section, entry, buffer);
}


// INIClass::Locate_Line_Comment (0x005106B0)
// Scans an entry line and records the columns of its first '=', of the first
// visible character after that '=' and of its ';' (tabs advance to the next
// multiple of 8; missing items give column 0). Returns the comment text after
// the ';', or null when the line has no comment.

char *__fastcall INIClass::Locate_Line_Comment(char *line, int &equals_column, int &value_column,
                                               int &comment_column)
{
    equals_column = -1;
    value_column = -1;
    comment_column = -1;
    char *comment = 0;
    int column = 0;
    while (*line != '\0') {
        if (equals_column >= 0 && value_column < 0 && (unsigned char)*line > ' ') {
            value_column = column;
        }
        if (*line == ';') {
            comment_column = column;
            comment = line + 1;
            break;
        }
        switch (*line) {
            case '\t':
                column = (column & ~7) + 8;
                break;

            case '=':
                if (equals_column < 0) {
                    equals_column = column;
                }
                column++;
                break;

            default:
                column++;
                break;
        }
        line++;
    }
    equals_column = equals_column < 0 ? 0 : equals_column;
    value_column = value_column < 0 ? 0 : value_column;
    comment_column = comment_column < 0 ? 0 : comment_column;
    return comment;
}


// INIClass::Strip_Comments (0x00510760)
// Cuts a line at its first ';' and trims what remains.

void __fastcall INIClass::Strip_Comments(char *buffer)
{
    if (buffer != 0) {
        char *comment = strchr(buffer, ';');
        if (comment != 0) {
            *comment = '\0';
            strtrim(buffer);
        }
    }
}


// INIClass::Is_Section_Header (0x00510790)
// True when the line, after leading whitespace, starts with '[' and contains
// a ']'.

bool __fastcall INIClass::Is_Section_Header(char const *line)
{
    if (line != 0) {
        while (*line != '\0' && (unsigned char)*line <= ' ') {
            line++;
        }
        if (*line == '[' && strchr(line, ']') != 0) {
            return true;
        }
    }
    return false;
}


// INIClass::INIEntry::~INIEntry (0x005107C0)
// Frees the key, value, comment lines and trailing comment; the Node base
// destructor then unlinks the entry from its section's list.

INIClass::INIEntry::~INIEntry()
{
    free(Key);
    Key = 0;
    free(Value);
    Value = 0;
    while (Comments != 0) {
        free(Comments->Value);
        INIComment *next = Comments->Next;
        delete Comments;
        Comments = next;
    }
    free(CommentString);
    CommentString = 0;
}


// INIClass::INISection::~INISection (0x00510850)
// Frees the name, deletes every entry, empties the entry index and frees the
// comment lines above the section header; member destructors then empty the
// index again, unlink any remaining list nodes and unlink the section.

INIClass::INISection::~INISection()
{
    free(Name);
    Name = 0;
    EntryList.Delete();
    EntryIndex.Clear();
    while (Comments != 0) {
        free(Comments->Value);
        INIComment *next = Comments->Next;
        delete Comments;
        Comments = next;
    }
}


// INIClass::INIEntry scalar deleting destructor (0x005109B0, vtable slot 0)
// Compiler generated from the virtual ~INIEntry, which it inlines.


// INIClass::INISection scalar deleting destructor (0x00510A60, vtable slot 0)
// Compiler generated from the virtual ~INISection (0x00510850), which it calls.


// Node<INIClass::INISection *> scalar deleting destructor (0x00510A80)
// Compiler generated for the Node base of INISection (vtable 0x007A3BB4):
// unlinks the node (inline GenericNode destructor), then optionally frees it.


// CacheStraw scalar deleting destructor (0x00510AD0)
// Compiler generated from CacheStraw's inline virtual destructor, emitted by the CacheStraw local of
// INIClass::Load (0x0050BC30); no source of its own.


// Base64Straw scalar deleting destructor (0x00510B00)
// Compiler generated with the vtable (0x007A3BCC) where Put_UUBlock
// constructs a Base64Straw: the inline Straw destructor, then an optional free.


// CRCEngine::CRCEngine (0x00510B20)
// Out-of-line copy of the inline constructor in shared/ini_class.hpp,
// emitted where this unit did not inline it (for example in Put_String).


// Base64Pipe scalar deleting destructor (0x00510B40)
// Compiler generated with the vtable (0x007A3BDC) where Get_UUBlock
// constructs a Base64Pipe: the inline Pipe destructor, then an optional free.


// INIClass::INIEntry::INIEntry (0x00510B60)
// Takes ownership of the key/value strings, the comment lines above the
// entry and its trailing comment, and records the entry's column positions.

INIClass::INIEntry::INIEntry(char *key, char *value, INIComment *comments, char *comment_string,
                             int comment_column, int equals_column, int value_column) :
    Key(key),
    Value(value),
    Comments(comments),
    CommentString(comment_string),
    EqualsColumn(equals_column),
    ValueColumn(value_column),
    CommentColumn(comment_column)
{
}


// List<INIClass::INIEntry *>::List (0x00510BB0)
// Out-of-line copy of the inline list constructor (links the head sentinel
// to the tail sentinel), emitted for INISection's constructor.


// List<INIClass::INIEntry *> scalar deleting destructor (0x00510C20)
// Compiler generated: unlinks (does not delete) every member, then the
// sentinels, then optionally frees the list.


// IndexClass<int, INIEntry *>::IndexClass (0x00510CE0)
// Empty table, no cached record.
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.


// IndexClass<int, INIEntry *>::Add_Index (0x00510D00)
// Appends a record, growing the table (inlined Increase_Table_Size).
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.


// IndexClass<int, INIEntry *>::Is_Present (0x00510DD0)
// Cached-record check, then lazy sort and binary search; caches a hit.
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.


// IndexClass<int, INIEntry *>::operator[] (0x00510E70)
// Value slot for an ID, or the shared zero slot (0x00B2B630).
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.


// IndexClass<int, INISection *>::Remove_Index (0x00510F10)
// Removes a record and closes the gap.
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.


// IndexClass<int, INISection *>::Is_Present (0x00510FF0)
// Cached-record check, then lazy sort and binary search; caches a hit.
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.


// IndexClass<int, INISection *>::operator[] (0x00511090)
// Value slot for an ID, or the shared zero slot (0x00B2B62C).
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.


// IndexClass<int, INIEntry *>::Is_Archive_Same (0x00511130)
// True when the cached record has this ID.
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses (Get_Rect 0x0050DF70) instantiate it.


// IndexClass<int, INIEntry *>::Search_For_Node (0x00511150)
// Lazy qsort, then binary search.
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.


// IndexClass<int, INISection *>::Increase_Table_Size (0x005111D0)
// Reallocates the table with room for amount more records.
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.


// IndexClass<int, INISection *>::Is_Archive_Same (0x00511260)
// True when the cached record has this ID.
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.


// IndexClass<int, INISection *>::Search_For_Node (0x00511280)
// Lazy qsort, then binary search.
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.


// IndexClass<int, INIEntry *>::search_compfunc (0x00511300)
// qsort comparator on the signed ID.
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.


// Binary_Search<IndexClass<int, INIEntry *>::NodeElement> (0x00511330)
// Fastcall binary search over sorted index records.
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses (INISection::Find_Entry 0x005101D0) instantiate it.


// IndexClass<int, INISection *>::search_compfunc (0x00511380)
// qsort comparator on the signed ID.
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.


// Binary_Search<IndexClass<int, INISection *>::NodeElement> (0x005113B0)
// Fastcall binary search over sorted index records.
// Template instantiation; the definition is in shared/ini_class.hpp and
// this unit's uses instantiate it.
