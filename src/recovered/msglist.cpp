// msglist.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/msglist.json carries the addresses and the rows that
// stay in their own files.


#define RA2_MESSAGE_LIST_UNIT
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ADD
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_OBJECT
#define VECTOR_IMPLEMENT_DYNAMIC_DELETE_INDEX
#include "shared/vector_class.hpp"
#include "shared/message_list.hpp"
#include "shared/text_label.hpp"
#include "shared/text_print.hpp"
#include "shared/comm_buffer.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/crt_memory.hpp"
#include "shared/rules.hpp"
#include "shared/sound_play.hpp"
#include "shared/message_gadget_layout.hpp"
#include "shared/keyboard.hpp"
#include "shared/color_scheme.hpp"
#include "shared/mouse.hpp"


// MessageListClass::MessageListClass() -- 0x005B4BD0
// Clears the list, the geometry and the selection range; the seven trailing words become 0x10001.
// The DynamicVectorClass members this unit's lists expand, defined before any other header includes the
// vector header (the unit's first source; shared/vector_class.hpp).
// MsgList.cpp members declared for this unit only (shared/message_list.hpp).

MessageListClass::MessageListClass()
{
    First = 0;
    X = 0;
    Y = 0;
    MaxMessages = 0;
    MaxChars = 0;
    Height = 0;
    unknown_18 = false;
    IsEdit = false;
    AdjustEdit = false;
    EditX = 0;
    EditY = 0;
    EditLabel = 0;
    EditCurPos = 0;
    EditInitPos = 0;
    EditCursor = 0;
    Selection_First = 0;
    Selection_Last = 0;
    TextWidth = 0;
    EditBuf[0] = 0;
    OverflowBuf[0] = 0;
    for (int i = 0; i < 7; i++) {
        unknown_1480[i] = 0x10001;
    }
}


// void MessageListClass::Reset() -- 0x005B4C40
// Init with everything cleared and the default width of 640.

void MessageListClass::Reset()
{
    Init(0, 0, 0, 0, 0, 0, 0, false, 0, 0, 0x280);
}


// void MessageListClass::Init(int x, int y, int max_messages, int max_chars, int unknown_arg5, int x2, int y2,
//     bool unknown_flag, int first, int last, int width) -- 0x005B4C60
// Deletes the messages, resets the list and stores the geometry (message and character limits clamped to 14
// and 112), the second position (or the first when either is -1) and the selection range.

void MessageListClass::Init(int x, int y, int max_messages, int max_chars, int unknown_arg5, int x2, int y2,
    bool unknown_flag, int first, int last, int width)
{
    TextWidth = width - 8;
    Debug_Printf("MessageListClass::Init(Width: %d)\n", width - 8);
    TextLabelClass *node = First;
    while (node) {
        First = (TextLabelClass *)node->Remove();
        if (node) {
            delete node;
        }
        node = First;
    }
    for (int i = 0; i < 7; i++) {
        unknown_1480[i] = 0x10001;
    }
    if (IsEdit) {
        if (EditLabel) {
            delete EditLabel;
        }
        EditLabel = 0;
    }
    First = 0;
    X = x;
    Y = y;
    MaxMessages = max_messages;
    if (max_messages > 14) {
        MaxMessages = 14;
    }
    MaxChars = max_chars;
    if (max_chars > 112) {
        MaxChars = 112;
    }
    Height = 19;
    unknown_18 = unknown_flag;
    IsEdit = false;
    if (x2 == -1 || y2 == -1) {
        AdjustEdit = true;
        EditX = x;
        EditY = y;
    } else {
        AdjustEdit = false;
        EditX = x2;
        EditY = y2;
    }
    EditLabel = 0;
    EditBuf[0] = 0;
    OverflowBuf[0] = 0;
    EditCurPos = 0;
    EditInitPos = 0;
    EditCursor = 0;
    Selection_First = first;
    Selection_Last = last;
    if (Selection_Last >= MaxChars) {
        Selection_Last = MaxChars - 1;
    }
    if (Selection_First >= Selection_Last) {
        Selection_First = Selection_Last - 1;
    }
}


// MessageListClass::Add_Message -- 0x005B4DA0
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcsncat(unsigned short *dest, unsigned short const *source, unsigned int count);
extern unsigned short const unknown_prefix_separator_7e5e0c[];

TextLabelClass *MessageListClass::Add_Message(unsigned short const *prefix, int identifier,
    unsigned short const *text, int color, unsigned style, int delay, bool unknown_flag)
{
    BitFontClass *font = Font_For_Style(style);
    if (text == 0) return 0;
    if (font == 0) return 0;
    unsigned short message[162];
    message[0] = 0;
    if (prefix != 0) {
        wcscpy(message, prefix);
        wcscat(message, unknown_prefix_separator_7e5e0c);
    }
    int prefix_length = wcslen(message);
    int prefix_width = font->String_Pixel_Width((wchar_t const *)message, 0);
    int text_length = wcslen(text);
    int width = TextWidth - prefix_width - 8;
    if (width <= 0) return 0;
    int chars = font->Chars_In_Width((wchar_t const *)text, width, 111, 1);
    if (chars < 0) return 0;
    wcsncat(message, text, chars);
    message[prefix_length + chars] = 0;
    if (MaxMessages > 0 && Num_Messages() + 1 > MaxMessages) {
        TextLabelClass *old = First;
        if (old == 0) return 0;
        First = (TextLabelClass *)old->Remove();
        for (int slot = 0; slot < 14; slot++) {
            if (old->Text == (wchar_t const *)Texts[slot]) Slot_Free[slot] = 1;
        }
        delete old;
    }
    TextLabelClass *label = new TextLabelClass((wchar_t const *)message, X, Y, color, style | 0x8000);
    if (unknown_game_mode_a3d298 == 4) {
        label->IsRevealed = !unknown_flag && unknown_flag_a3f209;
    } else if (unknown_game_mode_a3d298 == 3) {
        label->IsRevealed = !unknown_flag && unknown_flag_a3f208;
    } else {
        label->IsRevealed = !unknown_flag;
    }
    if (delay == -1) label->UserData1 = 0;
    else label->UserData1 = TickCount.Time() + delay;
    label->UserData2 = identifier;
    label->PixWidth = TextWidth;
    int found = 0;
    for (int i = 0; i < 14; i++) {
        if (Slot_Free[i]) {
            Slot_Free[i] = 0;
            memset(Texts[i], 0, 162);
            wcscpy(Texts[i], message);
            label->Text = (wchar_t const *)Texts[i];
            found = 1;
            break;
        }
    }
    if (!found) {
        delete label;
        return 0;
    }
    if (!unknown_flag) Voc_Play(Rules->IncomingMessage, 1.0f, 0x2000, 0);
    if (First) label->Add_Tail(*First);
    else First = label;
    int y;
    if (IsEdit && AdjustEdit) y = Height + Y;
    else y = Y;
    TextLabelClass *node = First;
    while (node) {
        node->Y = y;
        node = (TextLabelClass *)node->Get_Next();
        y += Height;
    }
    if (chars < text_length) {
        unsigned short const *remaining = text + chars;
        while (*remaining != 0 && *remaining < 0x20) remaining++;
        if (*remaining != 0) Add_Message(prefix, identifier, remaining, color, style, delay, true);
    }
    return label;
}


// unsigned short *MessageListClass::Get_Message(int identifier) -- 0x005B5160
// The text of the message with the identifier, or none (inlined everywhere in the unit).

unsigned short *MessageListClass::Get_Message(int identifier)
{
    for (TextLabelClass *node = First; node != 0; node = (TextLabelClass *)node->Get_Next()) {
        if (node->UserData2 == identifier) {
            return (unsigned short *)node->Text;
        }
    }
    return 0;
}


// TextLabelClass *MessageListClass::Get_Label(int identifier) -- 0x005B5190
// The message with the identifier, or none (inlined everywhere in the unit).

TextLabelClass *MessageListClass::Get_Label(int identifier)
{
    for (TextLabelClass *node = First; node != 0; node = (TextLabelClass *)node->Get_Next()) {
        if (node->UserData2 == identifier) {
            return node;
        }
    }
    return 0;
}


// MessageListClass::Concat_Message (0x005B51C0): append to a matching live message.
// Appends text to the message with this identifier and prefix, trimming the front of the message when it would
// no longer fit, and renews its expiry.
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern "C" unsigned short *__cdecl wcscat(unsigned short *dest, unsigned short const *source);

int MessageListClass::Concat_Message(unsigned short const *prefix, int identifier, unsigned short const *text, int delay)
{
    if (prefix == 0 || text == 0 || !unknown_18) return 0;
    TextLabelClass *node = 0;
    int found = 0;
    if (First) {
        node = First;
        while (node) {
            if (node->UserData2 == identifier && memcmp(node->Text, prefix, wcslen(prefix) * 2) == 0) {
                found = 1;
                break;
            }
            node = (TextLabelClass *)node->Get_Next();
        }
    }
    if (!found) return 0;
    unsigned short *tail = (unsigned short *)node->Text + wcslen(prefix) + 1;
    unsigned int max = MaxChars;
    if (wcslen(tail) + wcslen(text) < max) {
        unsigned short *buffer = (unsigned short *)operator new(max * 2 + 2);
        BitFontClass *font = Font_For_Style(node->Style);
        int extra = font->String_Pixel_Width(node->Text, 0) - font->String_Pixel_Width((wchar_t const *)tail, 0);
        wcscpy(buffer, tail);
        wcscat(buffer, text);
        while (font->String_Pixel_Width((wchar_t const *)buffer, 0) + extra >= TextWidth) {
            int length = wcslen(tail);
            if (length < 10) {
                length = 10;
            }
            Trim_Message(0, tail, 10, length, 0);
            wcscpy(buffer, tail);
            wcscat(buffer, text);
        }
        delete buffer;
        wcscat(tail, text);
    } else {
        int length = wcslen(tail) - MaxChars + wcslen(text);
        int most = wcslen(tail);
        if (most < length) {
            most = length;
        }
        Trim_Message(0, tail, length, most, 0);
        wcscat(tail, text);
    }
    if (delay == -1) {
        node->UserData1 = 0;
        return 1;
    }
    node->UserData1 = TickCount.Time() + delay;
    return 1;
}


// void MessageListClass::Set_Edit_Focus() -- 0x005B53E0
// Gives the open edit line the focus (inlined everywhere in the unit).

void MessageListClass::Set_Edit_Focus()
{
    if (IsEdit) {
        EditLabel->Set_Focus();
    }
}


// bool MessageListClass::Has_Edit_Focus() (0x005B53F0)
// Whether the open edit line has the focus (Westwood's Has_Edit_Focus).

bool MessageListClass::Has_Edit_Focus()
{
    if (IsEdit) {
        return EditLabel->Has_Focus();
    } else {
        return false;
    }
}


// TextLabelClass *MessageListClass::Add_Edit(color, style, text, cursor, width) -- 0x005B5410
// Opens the edit line (an open one only takes the focus again). With AdjustEdit the edit line is the
// last row: the oldest message goes when the list is full, the messages move down a row and the list
// holds one message less.
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);
extern "C" unsigned short *__cdecl wcscpy(unsigned short *dest, unsigned short const *source);
extern unsigned short const unknown_empty_wide_839d4c[];

TextLabelClass *MessageListClass::Add_Edit(int color, unsigned style, unsigned short const *text, char cursor, int width)
{
    if (IsEdit) {
        EditLabel->Set_Focus();
        return 0;
    }
    if (AdjustEdit) {
        int count = 0;
        for (TextLabelClass *node = First; node != 0; node = (TextLabelClass *)node->Get_Next()) {
            count++;
        }
        if (IsEdit && AdjustEdit) {
            count++;
        }
        if (count + 1 > MaxMessages) {
            TextLabelClass *oldest = First;
            First = (TextLabelClass *)oldest->Remove();
            for (int i = 0; i < 14; i++) {
                if (oldest->Text == Texts[i]) {
                    Slot_Free[i] = 1;
                }
            }
            delete oldest;
        }
    }
    if (text == 0) {
        text = unknown_empty_wide_839d4c;
    }
    EditCursor = cursor;
    memset(EditBuf, 0, sizeof(EditBuf));
    wcscpy(EditBuf, text);
    OverflowBuf[0] = 0;
    EditCurPos = EditInitPos = wcslen(text);
    EditLabel = new TextLabelClass(EditBuf, EditX, EditY, color, style | 0x8000);
    if (width > 0) {
        EditLabel->PixWidth = width;
    } else {
        EditLabel->PixWidth = TextWidth;
    }
    if (EditLabel) {
        IsEdit = true;
        EditLabel->Set_Focus();
    } else {
        IsEdit = false;
    }
    if (AdjustEdit) {
        Restack_Messages();
        MaxMessages--;
    }
    return EditLabel;
}


// void MessageListClass::Remove_Edit() -- 0x005B5580
// Closes the open edit line; with AdjustEdit the messages move back up and the list holds one more.

void MessageListClass::Remove_Edit()
{
    if (IsEdit) {
        IsEdit = false;
        delete EditLabel;
        Unknown_00738100();
        if (AdjustEdit) {
            Restack_Messages();
            MaxMessages++;
        }
    }
}


class Leaf_5b55e0 { public: unsigned int Get_Edit_Buf_005b55e0(); };
unsigned int Leaf_5b55e0::Get_Edit_Buf_005b55e0() { unsigned int message_list_address = (unsigned int)this; unsigned int buffer_value; unsigned int unused_value; buffer_value = *(unsigned int*)(message_list_address + 0x2b4); buffer_value = message_list_address + buffer_value*2 + 0x28; return buffer_value; }


// 0x005b55f0: function not yet identified
//
// Body: 20 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __thiscall FUN_005b55f0(int param_1,undefined4 param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_005b55f0 {

struct Owner_005b55f0 { void Set_Edit_Color_005b55f0(unsigned int color); };

void Owner_005b55f0::Set_Edit_Color_005b55f0(unsigned int color)
{
    int message_list_address = (int)this;
    if (*(char *)(message_list_address + 0x19) != '\0') {
        *(unsigned int *)(*(int *)(message_list_address + 0x24) + 0x34) = color;
    }
    return;
}

}  // namespace seed_005b55f0


// int MessageListClass::Update_Messages() (0x005B5610)
// Drops every message whose time has come (freeing its text slot), and when any went restacks the rest from the top.

int MessageListClass::Update_Messages()
{
    int removed = 0;
    TextLabelClass *node = First;
    if (node != 0) {
        do {
            if (node->UserData1 != 0 && TickCount.Time() > node->UserData1) {
                TextLabelClass *next = (TextLabelClass *)node->Get_Next();
                First = (TextLabelClass *)node->Remove();
                for (int i = 0; i < 14; i++) {
                    if (node->Text == Texts[i]) {
                        Slot_Free[i] = 1;
                    }
                }
                if (node != 0) {
                    delete node;
                }
                removed = 1;
                node = next;
            } else {
                node = (TextLabelClass *)node->Get_Next();
            }
        } while (node != 0);
        if (removed != 0) {
            int y;
            if (IsEdit && AdjustEdit) {
                y = Height + Y;
            } else {
                y = Y;
            }
            node = First;
            while (node != 0) {
                node->Y = y;
                node = (TextLabelClass *)node->Get_Next();
                y += Height;
            }
        }
    }
    return removed;
}


// int MessageListClass::Input(int &input, unsigned short key) -- 0x005B56F0
// The edit line's input: a mouse click (with the right button) closes it; Escape closes it, Return closes it
// (sent when it holds text, which gets a trailing space), Backspace removes the last character, and other
// printable characters are added while there is room (MaxChars and the pixel width); with unknown_18 set
// a full line moves its start to OverflowBuf. Keys without a character come through WWKeyboard.

int MessageListClass::Input(int &input, unsigned short key)
{
    int retcode = 0;
    if (key == 0) {
        if (input == 0) {
            return 0;
        }
        int button = input & ~0x800;
        if (button == 1 || button == 2) {
            if ((input & 2) && IsEdit) {
                Remove_Edit();
                return 2;
            }
            return 0;
        }
    }
    if (IsEdit) {
        int ascii;
        if (key == 0) {
            ascii = WWKeyboard->To_Char((KeyNumType)input) & 0xff;
            if ((input & 0x1000) && ascii >= '0' && ascii <= '9') {
                input &= ~0x1000;
            } else if ((!(input & 0x1000) && !(input & 0x8000) && ascii >= ' ') || (input & 0xff) == 13
                || (input & 0xff) == 8 || (input & 0xff) == 27) {
            } else {
                input = 0;
                return 0;
            }
        } else {
            ascii = key;
        }
        switch (ascii) {
        case 27:
            Remove_Edit();
            retcode = 2;
            input = 0;
            break;
        case 13:
            if (EditCurPos < EditInitPos) {
                EditCurPos = EditInitPos;
            }
            while (EditBuf[EditCurPos] != 0 && EditCurPos - EditInitPos < MaxChars - 1) {
                EditCurPos++;
            }
            if (EditCurPos == EditInitPos) {
                Remove_Edit();
                retcode = 2;
                input = 0;
                break;
            }
            if (EditCurPos - EditInitPos < MaxChars - 1) {
                EditBuf[EditCurPos] = ' ';
                EditCurPos++;
                EditBuf[EditCurPos] = 0;
            }
            Remove_Edit();
            retcode = 3;
            input = 0;
            break;
        case 8:
            if (EditCurPos > EditInitPos) {
                EditCurPos--;
                EditBuf[EditCurPos] = 0;
                retcode = 2;
            }
            input = 0;
            EditLabel->Set_Focus();
            break;
        default:
            EditLabel->Set_Focus();
            if (ascii > 0x1f && ascii < 0x10000) {
                bool overflow = false;
                while (EditBuf[EditCurPos] != 0 && EditCurPos - EditInitPos < MaxChars - 1) {
                    EditCurPos++;
                }
                if (EditCurPos - EditInitPos < MaxChars - 1) {
                    EditBuf[EditCurPos] = ascii;
                    EditCurPos++;
                    EditBuf[EditCurPos] = 0;
                    retcode = 1;
                    BitFontClass *font = Font_For_Style(EditLabel->Style);
                    if (font->String_Pixel_Width((wchar_t const *)EditBuf, 0) >= TextWidth - 10) {
                        EditCurPos--;
                        EditBuf[EditCurPos] = 0;
                        overflow = true;
                    }
                } else {
                    overflow = true;
                }
                if (overflow) {
                    if (unknown_18) {
                        int count = Trim_Message(OverflowBuf, &EditBuf[EditInitPos], Selection_First, Selection_Last, 1);
                        EditCurPos -= count;
                        EditBuf[EditCurPos] = ascii;
                        EditCurPos++;
                        EditBuf[EditCurPos] = 0;
                        retcode = 4;
                    } else {
                        retcode = 0;
                    }
                }
            }
            input = 0;
            break;
        }
    }
    return retcode;
}


// void MessageListClass::Draw() -- 0x005B5B30
// Draws the open edit line, with its cursor character after the text while it has the focus and the
// text is shorter than MaxChars, then the messages.

void MessageListClass::Draw()
{
    unsigned short cursor[2];
    cursor[0] = 0;
    cursor[1] = 0;
    if (IsEdit) {
        WWMouse->Hide_Mouse();
        EditLabel->Draw_Me(true);
        BitFontClass *font = Font_For_Style(EditLabel->Style);
        int width = font->String_Pixel_Width(EditLabel->Text, 0);
        if (EditCursor != 0 && EditCurPos - EditInitPos < MaxChars - 1 && EditLabel->Has_Focus()) {
            cursor[0] = EditCursor;
            Point2D at(EditLabel->X + width, EditLabel->Y);
            Fancy_Text_Printf((wchar_t const *)cursor, unknown_surface_83995c, &unknown_surface_83995c->Get_Rect(), &at,
                ColorSchemes[EditLabel->Color], 0, EditLabel->Style);
        }
        WWMouse->Show_Mouse();
    }
    if (First) {
        First->Draw_All(true);
    }
}


// int MessageListClass::Num_Messages() -- 0x005B5C30
// The number of messages; the edit line counts when it is the list's last row.

int MessageListClass::Num_Messages()
{
    int count = 0;
    for (TextLabelClass *node = First; node != 0; node = (TextLabelClass *)node->Get_Next()) {
        count++;
    }
    if (IsEdit && AdjustEdit) {
        count++;
    }
    return count;
}


// void MessageListClass::Set_Width(int width) -- 0x005B5C60
// The list's text width (8 less than the width) for the list, every message and the edit line.

void MessageListClass::Set_Width(int width)
{
    width -= 8;
    TextWidth = width;
    Debug_Printf("MessageListClass::Set_Width(%d)\n", width);
    TextLabelClass *node = First;
    while (node != 0) {
        node->PixWidth = width;
        node = (TextLabelClass *)node->Get_Next();
    }
    if (IsEdit) {
        EditLabel->PixWidth = width;
    }
}


// int MessageListClass::Trim_Message(dest, src, min_chars, max_chars, scan_start) -- 0x005B5CB0
// Moves the first characters of src (at least min_chars, at most max_chars, broken after a space found
// scanning forward from min_chars, or backward from max_chars when scan_start is set) into dest when it
// is given; src keeps the rest. The number of characters taken (0 when min_chars is not positive).
extern "C" unsigned int __cdecl wcslen(unsigned short const *string);

int MessageListClass::Trim_Message(unsigned short *dest, unsigned short *src, int min_chars, int max_chars, int scan_start)
{
    if (min_chars <= 0) {
        return 0;
    }
    int len = wcslen(src);
    if (max_chars > len) {
        max_chars = len;
    }
    int i;
    bool found = false;
    if (scan_start == 0) {
        for (i = min_chars; i <= max_chars; i++) {
            if (src[i - 1] != 0 && src[i - 1] <= ' ') {
                found = true;
                break;
            }
        }
    } else {
        for (i = max_chars; i >= min_chars; i--) {
            if (src[i - 1] != 0 && src[i - 1] <= ' ') {
                found = true;
                break;
            }
        }
    }
    if (!found) {
        i = min_chars;
    }
    if (dest) {
        memcpy(dest, src, i * 2);
        dest[i] = 0;
    }
    memmove(src, &src[i], (len - i + 1) * 2);
    return i;
}
