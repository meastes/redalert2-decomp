// wstring.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/wstring.json carries the addresses and the rows that
// stay in their own files.


#include "shared/wstring.hpp"
#include "shared/wide_string.hpp"
#include "shared/bit_codec.hpp"


// Wstring::Wstring() : (0x0076D910)
// Empty (null) string.

Wstring::Wstring() :
    Buffer(0)
{
}


// Wstring::Wstring(char const *string) : (0x0076D920)
// Copy of a C string (an empty string for null).

Wstring::Wstring(char const *string) :
    Buffer(0)
{
    *this = string;
}


// Wstring::Wstring(Wstring const &other) : (0x0076D980)
// Copy constructor; a null source stays null.

Wstring::Wstring(Wstring const &other) :
    Buffer(0)
{
    if (other.Buffer) {
        Buffer = new char[strlen(other.Buffer) + 1];
        strcpy(Buffer, other.Buffer);
    }
}


// Wstring::~Wstring() (0x0076D9E0)
// Frees the buffer.

Wstring::~Wstring()
{
    Release();
}


// bool Wstring::operator==(char const *string) const (0x0076DA00)
// Null-aware string equality comparison (strcmp).

bool Wstring::operator==(char const *string) const
{
    if (Buffer == 0 && string == 0) {
        return true;
    }
    if (Buffer == 0 || string == 0) {
        return false;
    }
    if (strcmp(Buffer, string) == 0) {
        return true;
    }
    return false;
}


// bool Wstring::operator==(Wstring const &other) const (0x0076DA70)
// Null-aware string equality comparison (strcmp).

bool Wstring::operator==(Wstring const &other) const
{
    if (Buffer == 0 && other.Buffer == 0) {
        return true;
    }
    if (Buffer == 0 || other.Buffer == 0) {
        return false;
    }
    if (strcmp(Buffer, other.Buffer) == 0) {
        return true;
    }
    return false;
}


// bool Wstring::operator!=(char const *string) const (0x0076DAE0)
// Null-aware string inequality comparison (strcmp).

bool Wstring::operator!=(char const *string) const
{
    if (Buffer == 0 && string == 0) {
        return false;
    }
    if (Buffer == 0 || string == 0) {
        return true;
    }
    if (strcmp(Buffer, string) != 0) {
        return true;
    }
    return false;
}


// bool Wstring::operator!=(Wstring const &other) const (0x0076DB50)
// Null-aware string inequality comparison (strcmp).

bool Wstring::operator!=(Wstring const &other) const
{
    if (Buffer == 0 && other.Buffer == 0) {
        return false;
    }
    if (Buffer == 0 || other.Buffer == 0) {
        return true;
    }
    if (strcmp(Buffer, other.Buffer) != 0) {
        return true;
    }
    return false;
}


// Wstring &Wstring::operator=(char const *string) (0x0076DBC0)
// Replaces the contents with a copy of a C string (empty for null).

Wstring &Wstring::operator=(char const *string)
{
    Release();
    Buffer = new char[string ? strlen(string) + 1 : 1];
    Buffer[0] = '\0';
    if (string) {
        strcpy(Buffer, string);
    }
    return *this;
}


// Wstring &Wstring::operator=(Wstring const &other) (0x0076DC30)
// Assignment; skipped when the strings already compare equal.

Wstring &Wstring::operator=(Wstring const &other)
{
    if (*this == other) {
        return *this;
    }
    *this = other.Get();
    return *this;
}


// bool Wstring::Concatenate(char const *string) (0x0076DD00)
// Appends a C string.

bool Wstring::Concatenate(char const *string)
{
    if (!string) {
        return true;
    }
    char *old = Buffer;
    unsigned size = strlen(string) + 1;
    if (old) {
        size += strlen(old);
    }
    Buffer = new char[size];
    if (old) {
        strcpy(Buffer, old);
    } else {
        Buffer[0] = '\0';
    }
    strcat(Buffer, string);
    if (old) {
        delete[] old;
    }
    return true;
}


// bool Wstring::Concatenate(unsigned length, char const *string) (0x0076DDB0)
// Appends at most `length` characters (strncat).

bool Wstring::Concatenate(unsigned length, char const *string)
{
    char *old = Buffer;
    unsigned size = length + 1;
    if (old) {
        size += strlen(old);
    }
    Buffer = new char[size];
    if (old) {
        strcpy(Buffer, old);
    } else {
        Buffer[0] = '\0';
    }
    if (string) {
        strncat(Buffer, string, length);
    }
    if (old) {
        delete[] old;
    }
    return true;
}


// bool Wstring::Concatenate(Wstring const &other) (0x0076DE40)
// Appends another Wstring.

bool Wstring::Concatenate(Wstring const &other)
{
    return Concatenate(other.Peek_Buffer());
}


// Wstring &Wstring::operator+=(char const *string) (0x0076DF10)
// Appends a C string.

Wstring &Wstring::operator+=(char const *string)
{
    Concatenate(string);
    return *this;
}


// Wstring &Wstring::operator+=(Wstring const &other) (0x0076DFC0)
// Appends another Wstring.

Wstring &Wstring::operator+=(Wstring const &other)
{
    Concatenate(other);
    return *this;
}


// Wstring Wstring::operator+(char const *string) const (0x0076E090)
// New string: this followed by a C string.

Wstring Wstring::operator+(char const *string) const
{
    Wstring result(*this);
    result.Concatenate(string);
    return result;
}


// Wstring Wstring::operator+(Wstring const &other) const (0x0076E1C0)
// New string: this followed by another Wstring.

Wstring Wstring::operator+(Wstring const &other) const
{
    Wstring result(*this);
    result.Concatenate(other);
    return result;
}


// bool Wstring::Remove(int position, int count) (0x0076E310)
// Deletes a clamped window of characters.

bool Wstring::Remove(int position, int count)
{
    int length = strlen(Buffer);
    // As observed: an overlong window moves its start back, not its end.
    if (position + count > length) {
        position = length - count;
    }
    if (position < 0) {
        count += position;
        position = 0;
    }
    if (count <= 0) {
        return false;
    }
    char *result = new char[length - count + 1];
    Buffer[position] = '\0';
    Buffer[position + count - 1] = '\0';
    strcpy(result, Buffer);
    strcat(result, Buffer + position + count);
    delete[] Buffer;
    Buffer = result;
    return true;
}


// bool Wstring::Remove_Character(char character) (0x0076E3E0)
// Deletes every occurrence of a character, then reallocates to fit.

bool Wstring::Remove_Character(char character)
{
    bool removed = false;
    if (!Buffer) {
        return false;
    }
    int length = strlen(Buffer);
    char *found;
    while ((found = strchr(Buffer, character)) != 0) {
        removed = true;
        memmove(found, found + 1, Buffer - found + length - 1);
        length--;
        Buffer[length] = '\0';
    }
    if (removed) {
        char *result = new char[strlen(Buffer) + 1];
        strcpy(result, Buffer);
        delete[] Buffer;
        Buffer = result;
    }
    return removed;
}


// void Wstring::Remove_Spaces() (0x0076E4A0)
// Deletes all spaces and tabs.

void Wstring::Remove_Spaces()
{
    Remove_Character(' ');
    Remove_Character('\t');
}


// void Wstring::Trim_Leading(Wstring const &characters) (0x0076E4C0)
// Deletes leading characters found in `characters`.

void Wstring::Trim_Leading(Wstring const &characters)
{
    int count = 0;
    while (count < (int)Length() && strchr(characters.Get(), Buffer[count])) {
        count++;
    }
    if (count > 0) {
        Remove(0, count);
    }
}


// void Wstring::Trim_Trailing(Wstring const &characters) (0x0076E520)
// Deletes trailing characters found in `characters`.

void Wstring::Trim_Trailing(Wstring const &characters)
{
    unsigned index = Length();
    while (index-- && strchr(characters.Peek_Buffer(), Buffer[index])) {
    }
    index++;
    char const *source = "";
    if (Buffer) {
        source = Buffer;
    }
    Wstring result;
    result.Buffer = new char[index + 1];
    result.Buffer[0] = '\0';
    if (source) {
        strncpy(result.Buffer, source, index);
    }
    result.Buffer[index] = '\0';
    *this = result.Peek_Buffer();
}


// void Wstring::Release() (0x0076E630)
// Frees the buffer and leaves the string null.

void Wstring::Release()
{
    if (Buffer) {
        delete[] Buffer;
    }
    Buffer = 0;
}


// void Wstring::Set_Size(int size) (0x0076E650)
// Replaces the contents with `size` zero bytes.

void Wstring::Set_Size(int size)
{
    Release();
    if (size >= 0) {
        Buffer = new char[size];
        memset(Buffer, 0, size);
    }
}


// void Wstring::Copy_Padded(char *buffer, unsigned size) const (0x0076E6A0)
// Copies into a space-padded fixed field of `size` characters.

void Wstring::Copy_Padded(char *buffer, unsigned size) const
{
    unsigned length;
    strncpy(buffer, Buffer, size);
    length = strlen(Buffer);
    for (unsigned index = length; index < size; index++) {
        buffer[index] = ' ';
    }
    buffer[size] = '\0';
}


// char const *Wstring::Get() const (0x0076E6F0)
// The text, or the empty string when null.

char const *Wstring::Get() const
{
    return Buffer ? Buffer : "";
}


// char Wstring::Get_Char(unsigned index) const (0x0076E700)
// Character at `index`, or 0 past the end.

char Wstring::Get_Char(unsigned index) const
{
    if (index < strlen(Buffer)) {
        return Buffer[index];
    }
    return '\0';
}


// char const *Wstring::Peek_Buffer() const (0x0076E730)
// The text, or the empty string when null: a second copy of Get (0x0076E6F0).
// Formerly declared as operator char const *; the original passes string
// literals to INIClass's Wstring overloads, which an implicit conversion to
// char const * would make ambiguous, so the accessor is a named call. The name
// is a hypothesis.

char const *Wstring::Peek_Buffer() const
{
    return (Buffer == 0) ? "" : Buffer;
}


// char Wstring::operator[](unsigned index) const (0x0076E740)
// Character at `index`, or 0 past the end.

char Wstring::operator[](unsigned index) const
{
    if (index < strlen(Buffer)) {
        return Buffer[index];
    }
    return '\0';
}


// char Wstring::operator()(unsigned index) const (0x0076E770)
// Character at `index`, or 0 past the end.

char Wstring::operator()(unsigned index) const
{
    if (index < strlen(Buffer)) {
        return Buffer[index];
    }
    return '\0';
}


// unsigned Wstring::Length() const (0x0076E7A0)
// Length in characters (0 when null).

unsigned Wstring::Length() const
{
    if (!Buffer) {
        return 0;
    }
    return strlen(Buffer);
}


// bool Wstring::Insert(char const *string, unsigned position) (0x0076E7C0)
// Inserts a C string at position (clamped to the length); a null string
// just takes the text.

bool Wstring::Insert(char const *string, unsigned position)
{
    if (!Buffer) {
        *this = string;
        return true;
    }
    if (position > strlen(Buffer)) {
        position = strlen(Buffer);
    }
    char *result = new char[strlen(Buffer) + strlen(string) + 1];
    memset(result, 0, strlen(Buffer) + strlen(string) + 1);
    strcpy(result, Buffer);
    memmove(result + position + strlen(string), result + position, strlen(Buffer) - position + 1);
    memmove(result + position, string, strlen(string));
    delete[] Buffer;
    Buffer = result;
    return true;
}


// bool Wstring::Insert(char character, unsigned position) (0x0076E930)
// Inserts one character at position (clamped to the length); a null string
// just takes the character.

bool Wstring::Insert(char character, unsigned position)
{
    char text[2];
    if (!Buffer) {
        text[0] = character;
        text[1] = '\0';
        *this = text;
        return true;
    }
    unsigned length = strlen(Buffer);
    if (position > length) {
        position = length;
    }
    char *result = new char[length + 2];
    text[1] = '\0';
    char saved = Buffer[position];
    Buffer[position] = '\0';
    strcpy(result, Buffer);
    Buffer[position] = saved;
    text[0] = character;
    strcat(result, text);
    strcat(result, Buffer + position);
    delete[] Buffer;
    Buffer = result;
    return true;
}


// bool Wstring::Replace(char const *find, char const *with) (0x0076EA90)
// Replaces every occurrence of find with `with` (the narrow form of
// WideString::Replace 0x007706E0).

bool Wstring::Replace(char const *find, char const *with)
{
    Wstring result;
    char const *current = Get();
    while (current != 0 && *current != 0) {
        char const *found = strstr(current, find);
        if (found == 0) {
            result.Concatenate(current);
            break;
        }
        int length = found - current;
        if (length != 0) {
            char *old = result.Buffer;
            unsigned size = length + 1;
            if (old != 0) {
                size += strlen(old);
            }
            result.Buffer = new char[size];
            if (old != 0) {
                strcpy(result.Buffer, old);
            } else {
                result.Buffer[0] = 0;
            }
            strncat(result.Buffer, current, length);
            if (old != 0) {
                delete old;
            }
        }
        if (with != 0) {
            result.Concatenate(with);
        }
        current = found + strlen(find);
        if (current == 0) {
            break;
        }
    }
    *this = result.Peek_Buffer();
    return true;
}


// bool Wstring::Set(char const *string) (0x0076ED30)
// Replaces the contents with a copy of a C string (empty for null); true.

bool Wstring::Set(char const *string)
{
    *this = string;
    return true;
}


// bool Wstring::Set_Char(char character, unsigned index) (0x0076EDA0)
// Replaces the character at index; false when index is past the end.

bool Wstring::Set_Char(char character, unsigned index)
{
    if (index >= strlen(Buffer)) {
        return false;
    }
    Buffer[index] = character;
    return true;
}


// bool Wstring::Set(unsigned length, char const *string) (0x0076EDD0)
// Replaces the contents with at most length characters of string (strncpy
// into length + 1 bytes, always terminated); true.

bool Wstring::Set(unsigned length, char const *string)
{
    Release();
    Buffer = new char[length + 1];
    Buffer[0] = '\0';
    if (string) {
        strncpy(Buffer, string, length);
    }
    Buffer[length] = '\0';
    return true;
}


// void Wstring::To_Lower() (0x0076EE20): lowercases A-Z in place.

void Wstring::To_Lower()
{
    for (unsigned index = 0; index < Length(); index++) {
        if (Buffer[index] >= 'A' && Buffer[index] <= 'Z') {
            Buffer[index] = (char)tolower(Buffer[index]);
        }
    }
}


// Wstring Wstring::As_Lower() const (0x0076EE70): a lowercased copy.

Wstring Wstring::As_Lower() const
{
    Wstring copy(*this);
    copy.To_Lower();
    return copy;
}


// void Wstring::To_Upper() (0x0076EF40): uppercases a-z in place.

void Wstring::To_Upper()
{
    for (unsigned index = 0; index < Length(); index++) {
        if (Buffer[index] >= 'a' && Buffer[index] <= 'z') {
            Buffer[index] = (char)toupper(Buffer[index]);
        }
    }
}


// Wstring Wstring::As_Upper() const (0x0076EF90): a uppercased copy.

Wstring Wstring::As_Upper() const
{
    Wstring copy(*this);
    copy.To_Upper();
    return copy;
}


// bool Wstring::Truncate(unsigned length) (0x0076F060)
// Keeps the first length characters, through a temporary; true.

bool Wstring::Truncate(unsigned length)
{
    Wstring part;
    part.Set(length, Peek_Buffer());
    *this = part.Peek_Buffer();
    return true;
}


// bool Wstring::Truncate_At(char character) (0x0076F120)
// Keeps the text before the first occurrence of character; false when there is none.

bool Wstring::Truncate_At(char character)
{
    if (!Buffer) {
        return false;
    }
    char *found = strchr(Buffer, character);
    if (!found) {
        return false;
    }
    return Truncate(found - Buffer);
}


// int Wstring::Get_Token(int start, char const *separators, Wstring &token) const (0x0076F200)
// Skips separators from start, copies the run up to the next separator into
// token and returns the position after it; -1 for a negative start or when
// only separators remain.

int Wstring::Get_Token(int start, char const *separators, Wstring &token) const
{
    if (start < 0) {
        return -1;
    }
    while (start < (int)Length() && strchr(separators, Buffer[start])) {
        start++;
    }
    if (start >= (int)Length()) {
        return -1;
    }
    int first = start;
    while (start < (int)Length() && !strchr(separators, Buffer[start])) {
        start++;
    }
    int last = start - 1;
    token = Buffer + first;
    token.Truncate(last - first + 1);
    return last + 1;
}


// int Wstring::Get_Line(int start, Wstring &line) const (0x0076F3D0)
// Copies the text from start up to its line end (CR, LF, or CR LF kept in
// the line) and returns the position after it; -1 past the end.

int Wstring::Get_Line(int start, Wstring &line) const
{
    int position = start;
    if (start >= (int)Length()) {
        return -1;
    }
    while (position < (int)Length() && !strchr("\r\n", Buffer[position])) {
        position++;
    }
    int end = position;
    if (Buffer[position] == '\r' && Buffer[position + 1] == '\n') {
        position++;
        end = position;
    }
    line = Buffer + start;
    line.Truncate(end - start + 1);
    return end + 1;
}


// int Wstring::As_Integer() const (0x0076F570): atoi of the text.

int Wstring::As_Integer() const
{
    return atoi(Buffer);
}


// Wstring __fastcall Wstring_From_Integer(int value) (0x0076F580)
// The decimal text of value.

Wstring __fastcall Wstring_From_Integer(int value)
{
    char buffer[16];
    sprintf(buffer, "%d", value);
    return Wstring(buffer);
}


// Wstring __fastcall Wstring_From_Integer(int value, int width) (0x0076F610)
// The decimal text of value, zero-padded to width (the format is built in
// the output buffer and used in place, as in the original).

Wstring __fastcall Wstring_From_Integer(int value, int width)
{
    char buffer[16];
    sprintf(buffer, "%%0%dd", width);
    sprintf(buffer, buffer, value);
    return Wstring(buffer);
}


// Wstring __fastcall operator+(char const *string, Wstring const &other) (0x0076F6B0)
// A C string followed by a Wstring.

Wstring __fastcall operator+(char const *string, Wstring const &other)
{
    return Wstring(string) + other;
}


// Wstring __fastcall Wstring_Join(Wstring const &left, Wstring const &right) (0x0076F7D0)
// left followed by right (the member operator+ with Concatenate expanded;
// callers 0x007286B3, 0x0072ADA0, ...). The name is a hypothesis: a free
// operator+ would be ambiguous with the member one.

Wstring __fastcall Wstring_Join(Wstring const &left, Wstring const &right)
{
    Wstring result(left);
    result.Concatenate(right);
    return result;
}


// bool Wstring::Contains_Any(char const *characters) const (0x0076F920)
// Whether the text contains any of characters (strcspn stops before the
// end); false for no characters or no text.

bool Wstring::Contains_Any(char const *characters) const
{
    if (characters && Buffer) {
        unsigned length = strlen(Buffer);
        return strcspn(Buffer, characters) < length;
    }
    return false;
}


// bool Wstring::Contains_Any(Wstring const &characters) const (0x0076F960)

bool Wstring::Contains_Any(Wstring const &characters) const
{
    return Contains_Any(characters.Peek_Buffer());
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

WideString::WideString() :
    Buffer(0)
{
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

WideString::WideString(unsigned short const *string) :
    Buffer(0)
{
    *this = string;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

WideString::WideString(WideString const &other) :
    Buffer(0)
{
    if (other.Buffer) {
        Buffer = new unsigned short[wcslen(other.Buffer) + 1];
        wcscpy(Buffer, other.Buffer);
    }
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

WideString::~WideString()
{
    if (Buffer) {
        delete Buffer;
    }
    Buffer = 0;
}


// bool WideString::operator==(unsigned short const *string) const (0x0076FA70)

extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);

bool WideString::operator==(unsigned short const *string) const
{
    if (Buffer == 0 && string == 0) {
        return true;
    }
    if (Buffer != 0 && string != 0) {
        return wcscmp(Buffer, string) == 0 ? true : false;
    }
    return false;
}


// bool WideString::operator==(WideString const &other) const (0x0076FAB0)

extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);

bool WideString::operator==(WideString const &other) const
{
    if (Buffer == 0 && other.Buffer == 0) {
        return true;
    }
    if (Buffer != 0 && other.Buffer != 0) {
        return wcscmp(Buffer, other.Buffer) == 0 ? true : false;
    }
    return false;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);

bool WideString::operator!=(unsigned short const *string) const
{
    if (Buffer == 0 && string == 0) {
        return false;
    }
    if (Buffer == 0 || string == 0) {
        return true;
    }
    if (wcscmp(Buffer, string) != 0) {
        return true;
    }
    return false;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

bool WideString::operator!=(WideString const &other) const
{
    if (Buffer == 0 && other.Buffer == 0) {
        return false;
    }
    if (Buffer == 0 || other.Buffer == 0) {
        return true;
    }
    if (wcscmp(Buffer, other.Buffer) != 0) {
        return true;
    }
    return false;
}


// WideString &WideString::operator=(unsigned short const *string) (0x0076FB70)
// Replaces the text with an owned copy (an empty string for null).

extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);

WideString &WideString::operator=(unsigned short const *string)
{
    if (Buffer != 0) {
        delete Buffer;
    }
    Buffer = 0;
    int length = string != 0 ? wcslen(string) + 1 : 1;
    Buffer = new unsigned short[length];
    Buffer[0] = 0;
    if (string != 0) {
        wcscpy(Buffer, string);
    }
    return *this;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

WideString &WideString::operator=(WideString const &other)
{
    if (Buffer == 0 && other.Buffer == 0) {
        return *this;
    }
    if (Buffer != 0 && other.Buffer != 0 && wcscmp(Buffer, other.Buffer) == 0) {
        return *this;
    }
    *this = other.Get();
    return *this;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

bool WideString::Concatenate(unsigned short const *string)
{
    if (!string) {
        return true;
    }
    unsigned short *old = Buffer;
    unsigned size = wcslen(string) + 1;
    if (old) {
        size += wcslen(old);
    }
    Buffer = new unsigned short[size * 2];
    if (old) {
        wcscpy(Buffer, old);
    } else {
        Buffer[0] = 0;
    }
    wcscat(Buffer, string);
    if (old) {
        delete old;
    }
    return true;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

bool WideString::Concatenate(unsigned length, unsigned short const *string)
{
    unsigned short *old = Buffer;
    unsigned size = length + 1;
    if (old) {
        size += wcslen(old);
    }
    Buffer = new unsigned short[size * 2];
    if (old) {
        wcscpy(Buffer, old);
    } else {
        Buffer[0] = 0;
    }
    if (string) {
        wcsncat(Buffer, string, length);
    }
    if (old) {
        delete old;
    }
    return true;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

bool WideString::Concatenate(WideString const &other)
{
    return Concatenate(other.Get());
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

WideString &WideString::operator+=(unsigned short const *string)
{
    Concatenate(string);
    return *this;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

WideString &WideString::operator+=(WideString const &other)
{
    Concatenate(other);
    return *this;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

WideString WideString::operator+(unsigned short const *string) const
{
    WideString result(*this);
    result.Concatenate(string);
    return result;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

WideString WideString::operator+(WideString const &other)
{
    WideString result(*this);
    result.Concatenate(other);
    return result;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

short WideString::Remove(int position, int count)
{
    int length = wcslen(Buffer);
    if (position + count > length) {
        position = length - count;
    }
    if (position < 0) {
        count += position;
        position = 0;
    }
    if (count <= 0) {
        return 0;
    }
    unsigned short *result = new unsigned short[length - count + 1];
    Buffer[position] = 0;
    Buffer[position + count - 1] = 0;
    wcscpy(result, Buffer);
    wcscat(result, Buffer + position + count);
    delete Buffer;
    Buffer = result;
    return 1;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

bool WideString::Remove_Character(unsigned short character)
{
    bool removed = false;
    if (!Buffer) {
        return false;
    }
    int length = wcslen(Buffer);
    unsigned short *found;
    while ((found = wcschr(Buffer, character)) != 0) {
        removed = true;
        memmove(found, found + 1, length * 2 - (found - Buffer) - 2);
        length--;
        Buffer[length] = 0;
    }
    if (removed) {
        unsigned short *result = new unsigned short[wcslen(Buffer) + 1];
        wcscpy(result, Buffer);
        delete Buffer;
        Buffer = result;
    }
    return removed;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

void WideString::Trim_Leading(WideString const &characters)
{
    unsigned count = 0;
    while (count < Length() && wcschr(characters.Get(), Buffer[count])) {
        count++;
    }
    if (count > 0) {
        Remove(0, count);
    }
}


// void WideString::Trim_Trailing(WideString const &characters) (0x00770230)
// Drops trailing characters found in characters (the counterpart of Trim_Leading
// 0x007701D0); the kept part goes through the inline Truncate.
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);

void WideString::Trim_Trailing(WideString const &characters)
{
    unsigned index = Length();
    while (index--) {
        if (!wcschr(characters.Get(), Buffer[index])) {
            break;
        }
    }
    Truncate(index + 1);
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

void WideString::Set_Size(int size)
{
    if (Buffer) {
        delete Buffer;
    }
    Buffer = 0;
    if (size >= 0) {
        Buffer = new unsigned short[size];
        memset(Buffer, 0, size * 2);
    }
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

void WideString::Copy_Padded(unsigned short *buffer, unsigned size) const
{
    unsigned length;
    wcsncpy(buffer, Buffer, size);
    length = wcslen(Buffer);
    for (unsigned index = length; index < size; index++) {
        buffer[index] = L' ';
    }
    buffer[size] = 0;
}


// 0x00770330: function not yet identified
//
// Body: 26 bytes (frozen inventory bound).
// Referenced from 3 sites (unconditional call).
// Ghidra prototype: void __fastcall FUN_00770330(int *param_1)
// Callees with accepted source:
//   FUN_00781d3b -> src/recovered/00781d3b.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00770330 {

extern void  operator_delete_00781d3b(unsigned int allocation_address);

void __fastcall FUN_00770330(int *string_words)
{
    if (*string_words != 0) {
        operator_delete_00781d3b((unsigned int)(*string_words));
    }
    *string_words = 0;
    return;
}

}  // namespace seed_00770330


// unsigned short const *WideString::Get() const (0x007703F0)
// The text, or an empty string when null.

unsigned short const *WideString::Get() const
{
    return Buffer != 0 ? Buffer : (unsigned short const *)L"";
}


// 0x00770400: function not yet identified
//
// Body: 39 bytes (frozen inventory bound).
// Referenced from 1 site (unconditional call).
// Ghidra prototype: undefined2 __thiscall FUN_00770400(int *param_1,uint param_2)
// Callees with accepted source:
//   FUN_00783605 -> src/recovered/00783605.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00770400 {

extern unsigned int  FUN_00783605(short *text_buffer);

struct Owner_00770400 { unsigned short FUN_00770400(unsigned int character_index); };

unsigned short Owner_00770400::FUN_00770400(unsigned int character_index)
{
    int * string_words = (int *)this;
    unsigned int length;

    length = (unsigned int)(FUN_00783605((short *)(*string_words)));
    if (character_index < length) {
        return *(unsigned short *)(*string_words + character_index * 2);
    }
    return 0;
}

}  // namespace seed_00770400


class Leaf_770430 { public: unsigned int get(); };
unsigned int Leaf_770430::get() { unsigned int string_address=(unsigned int)this, text_address=0, unused_value=0; unsigned char unused_byte=0; text_address = *(unsigned int*)(string_address); if ((text_address & text_address) != 0) goto L77043b; text_address = 0x839d4c; L77043b:; return text_address; }


// 0x00770440: function not yet identified
//
// Body: 39 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined2 __thiscall FUN_00770440(int *param_1,uint param_2)
// Callees with accepted source:
//   FUN_00783605 -> src/recovered/00783605.cpp
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00770440 {

extern unsigned int  FUN_00783605(short *text_buffer);

struct Owner_00770440 { unsigned short FUN_00770440(unsigned int character_index); };

unsigned short Owner_00770440::FUN_00770440(unsigned int character_index)
{
    int * string_words = (int *)this;
    unsigned int length;

    length = (unsigned int)(FUN_00783605((short *)(*string_words)));
    if (character_index < length) {
        return *(unsigned short *)(*string_words + character_index * 2);
    }
    return 0;
}

}  // namespace seed_00770440


// unsigned short WideString::Get_Char(unsigned index) const (0x00770470):
// the character at index, 0 past the end. (A plain `return 0;` compiles to
// xor ax,ax; the original's xor eax,eax comes from the 16-bit typed zero of
// the conditional.)

extern "C" unsigned int __cdecl wcslen(unsigned short const *string);

unsigned short WideString::Get_Char(unsigned index) const
{
    return index < wcslen(Buffer) ? Buffer[index] : (unsigned short)0;
}


// unsigned WideString::Length() const (0x007704A0): wcslen; 0 for no text.

extern "C" unsigned int __cdecl wcslen(unsigned short const *string);

unsigned WideString::Length() const
{
    if (Buffer == 0) {
        return 0;
    }
    return wcslen(Buffer);
}


// bool WideString::Insert(unsigned short const *string, unsigned position) (0x007704C0)
// Inserts string at position (clamped to the length); with no text it just takes the
// string (TibSun Wstring::insert shape: the moved tail is counted in characters).
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);

bool WideString::Insert(unsigned short const *string, unsigned position)
{
    if (Buffer == 0) {
        *this = string;
        return true;
    }
    if (position > wcslen(Buffer)) {
        position = wcslen(Buffer);
    }
    unsigned short *buffer = new unsigned short[wcslen(Buffer) + wcslen(string) + 1];
    memset(buffer, 0, (wcslen(Buffer) + wcslen(string) + 1) * 2);
    wcscpy(buffer, Buffer);
    int count = wcslen(Buffer) + 1 - position;
    memmove(buffer + position + wcslen(string), buffer + position, count * 2);
    memmove(buffer + position, string, wcslen(string) * 2);
    delete Buffer;
    Buffer = buffer;
    return true;
}


// bool WideString::Insert(unsigned short character, unsigned position) (0x007705E0)
// Inserts one character at position (clamped to the length).
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);

bool WideString::Insert(unsigned short character, unsigned position)
{
    unsigned short text[2];
    if (Buffer == 0) {
        text[0] = character;
        text[1] = 0;
        *this = text;
        return true;
    }
    unsigned length = wcslen(Buffer);
    if (position > length) {
        position = length;
    }
    unsigned short *buffer = new unsigned short[length + 2];
    text[0] = Buffer[position];
    Buffer[position] = 0;
    text[1] = 0;
    wcscpy(buffer, Buffer);
    Buffer[position] = text[0];
    text[0] = character;
    wcscat(buffer, text);
    wcscat(buffer, Buffer + position);
    delete Buffer;
    Buffer = buffer;
    return true;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsstr(unsigned short const *string, unsigned short const *match);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

bool WideString::Replace(unsigned short const *find, unsigned short const *with)
{
    WideString result;
    unsigned short const *current = Get();
    while (current != 0 && *current != 0) {
        unsigned short const *found = wcsstr(current, find);
        if (found == 0) {
            result.Concatenate(current);
            break;
        }
        int length = (int)found - (int)current;
        if (length != 0) {
            unsigned short *old = result.Buffer;
            unsigned size = length + 1;
            if (old != 0) {
                size += wcslen(old);
            }
            result.Buffer = new unsigned short[size * 2];
            if (old != 0) {
                wcscpy(result.Buffer, old);
            } else {
                result.Buffer[0] = 0;
            }
            wcsncat(result.Buffer, current, length);
            if (old != 0) {
                delete old;
            }
        }
        if (with != 0) {
            result.Concatenate(with);
        }
        current = (unsigned short const *)((int)found + wcslen(find) * 2);
        if (current == 0) {
            break;
        }
    }
    *this = (unsigned short const *)result;
    return true;
}


// short WideString::Set(unsigned short const *string) (0x007708F0)
// As operator=(string) (0x0076FB70), returning a 16-bit 1 (`mov ax, 1`).

extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);

short WideString::Set(unsigned short const *string)
{
    if (Buffer != 0) {
        delete Buffer;
    }
    Buffer = 0;
    int length = string != 0 ? wcslen(string) + 1 : 1;
    Buffer = new unsigned short[length];
    Buffer[0] = 0;
    if (string != 0) {
        wcscpy(Buffer, string);
    }
    return 1;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

bool WideString::Set_Char(unsigned short character, unsigned index)
{
    if (index >= wcslen(Buffer)) {
        return false;
    }
    Buffer[index] = character;
    return true;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

short WideString::Set(unsigned length, unsigned short const *string)
{
    if (Buffer) {
        delete Buffer;
    }
    Buffer = 0;
    Buffer = new unsigned short[length + 1];
    Buffer[0] = 0;
    if (string) {
        wcsncpy(Buffer, string, length);
    }
    Buffer[length] = 0;
    return 1;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

void WideString::To_Lower()
{
    for (unsigned index = 0; index < Length(); index++) {
        if (Buffer[index] >= 'A' && Buffer[index] <= 'Z') {
            Buffer[index] = tolower(Buffer[index]);
        }
    }
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

WideString WideString::As_Lower() const
{
    WideString copy(*this);
    copy.To_Lower();
    return copy;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

void WideString::To_Upper()
{
    for (unsigned index = 0; index < Length(); index++) {
        if (Buffer[index] >= 'a' && Buffer[index] <= 'z') {
            Buffer[index] = toupper(Buffer[index]);
        }
    }
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

WideString WideString::As_Upper() const
{
    WideString copy(*this);
    copy.To_Upper();
    return copy;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

bool WideString::Truncate(unsigned length)
{
    WideString part;
    part.Set(length, *this);
    *this = (unsigned short const *)part;
    return true;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

bool WideString::Truncate_At(unsigned short character)
{
    if (!Buffer) {
        return false;
    }
    unsigned short *found = wcschr(Buffer, character);
    if (!found) {
        return false;
    }
    return Truncate(found - Buffer);
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcschr(unsigned short const *string, unsigned short character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

int WideString::Get_Token(int start, unsigned short const *separators, WideString &token) const
{
    if (start < 0) {
        return -1;
    }
    while (start < (int)Length() && wcschr(separators, Buffer[start])) {
        start++;
    }
    if (start >= (int)Length()) {
        return -1;
    }
    int first = start;
    while (start < (int)Length() && !wcschr(separators, Buffer[start])) {
        start++;
    }
    int last = start - 1;
    token = Buffer + first;
    token.Truncate(last - first + 1);
    return last + 1;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" char *__cdecl strchr(char const *string, int character);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

int WideString::Get_Line(int start, WideString &line) const
{
    int position = start;
    if (start >= (int)Length()) {
        return -1;
    }
    while (position < (int)Length() && !strchr("\r\n", Buffer[position])) {
        position++;
    }
    int end = position;
    if (Buffer[position] == L'\r' && Buffer[position + 1] == L'\n') {
        position++;
        end = position;
    }
    line = Buffer + start;
    line.Truncate(end - start + 1);
    return end + 1;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

extern "C" long __cdecl wcstol(unsigned short const *string, unsigned short **end, int base);

int WideString::As_Integer() const
{
    return wcstol(Buffer, 0, 10);
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

bool WideString::Is_Narrow() const
{
    if (Buffer != 0) {
        int length = wcslen(Buffer);
        for (int index = 0; index < length; index++) {
            if (Buffer[index] >= 0x100) {
                return false;
            }
        }
    }
    return true;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);

WideString __fastcall unknown_number_text_7710c0(int number)
{
    unsigned short buffer[32];
    swprintf(buffer, L"%d", number);
    return WideString(buffer);
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

extern "C" int __cdecl swprintf(unsigned short *buffer, unsigned short const *format, ...);

WideString __fastcall unknown_number_text_width_771130(int number, int width)
{
    unsigned short buffer[32];
    swprintf(buffer, L"%%0%dd", width);
    swprintf(buffer, buffer, number);
    return WideString(buffer);
}


// WideString __fastcall Wstring_To_Wide(Wstring const &string) (0x007711C0)
// The wide copy of a narrow string: the text is widened into a scratch buffer
// (0x006FA0C0), copied into the result and the buffer freed.
// The same function and mangled name as shared/campaign.hpp (wchar_t there is unsigned short); this unit does not include it.
void __fastcall Char_To_Wide_006fa0c0(unsigned short *buffer, char const *text, int length);

WideString __fastcall Wstring_To_Wide(Wstring const &string)
{
    int length = (string.Buffer == 0) ? 0 : strlen(string.Buffer);
    unsigned short *buffer = new unsigned short[length + 1];
    Char_To_Wide_006fa0c0(buffer, (string.Buffer == 0) ? "" : string.Buffer, length);
    WideString wide(buffer);
    delete buffer;
    return wide;
}


// Wstring __fastcall Narrow_Copy_00771280(WideString const &wide) (0x00771280)
void __fastcall WideToCharString_006fa0f0(char *dst, unsigned short const *src, int limit);

Wstring __fastcall Narrow_Copy_00771280(WideString const &wide)
{
    int length = wide.Length();
    char *buffer = new char[length + 1];
    WideToCharString_006fa0f0(buffer, wide, length);
    Wstring narrow(buffer);
    delete buffer;
    return narrow;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

WideString __fastcall operator+(unsigned short const *string, WideString const &other)
{
    return WideString(string).operator+(other);
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

WideString __fastcall operator+(WideString const &left, WideString const &right)
{
    WideString sum(left);
    sum.Concatenate((unsigned short const *)right);
    return sum;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

extern "C" unsigned int __cdecl wcscspn(unsigned short const *string, unsigned short const *characters);

bool WideString::Contains_Any(unsigned short const *characters) const
{
    if (characters != 0 && Buffer != 0) {
        unsigned int length = wcslen(Buffer);
        return wcscspn(Buffer, characters) < length;
    }
    return false;
}


extern "C" int __cdecl wcscmp(unsigned short const *left, unsigned short const *right);
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" void *__cdecl memmove(void *destination, void const *source, unsigned int count);
extern "C" unsigned short *__cdecl wcsncpy(unsigned short *dest, unsigned short const *source, unsigned int count);
extern "C" void *__cdecl memset(void *destination, int value, unsigned int count);
extern "C" int __cdecl tolower(int character);
extern "C" int __cdecl toupper(int character);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);

bool WideString::Contains_Any(WideString const &characters) const
{
    return Contains_Any((unsigned short const *)characters);
}


// Wstring __fastcall Wide_To_7_Bit(WideString const &wide) (0x007715D0)

Wstring __fastcall Wide_To_7_Bit(WideString const &wide)
{
    int bits = (wide.Length() + 1) * 16;
    unsigned char *buffer = new unsigned char[(bits + 6) / 7];
    Bit_Expand_7((unsigned char const *)(unsigned short const *)wide, buffer, bits);
    Wstring encoded((char const *)buffer);
    delete buffer;
    return encoded;
}


// WideString __fastcall Wide_From_7_Bit(Wstring const &string) (0x007716D0)

WideString __fastcall Wide_From_7_Bit(Wstring const &string)
{
    int bytes = (string.Length() + 1) * 7 / 8;
    int count = (bytes + 1) / 2;
    unsigned short *buffer = new unsigned short[count + 1];
    Bit_Compress_7_String((unsigned char const *)((string.Buffer == 0) ? "" : string.Buffer), buffer, count);
    WideString decoded(buffer);
    delete buffer;
    return decoded;
}
