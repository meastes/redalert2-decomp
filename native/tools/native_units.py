#!/usr/bin/env python3
"""Generate the native (arm64, clang) view of the recovered tree.

The exact build compiles each catalog compile group (population + options, in
source order) as ONE translation unit: the group's sources are concatenated,
so later files rely on declarations made by earlier ones. The native build
keeps that unit structure. This tool never edits src/; it writes:

  OUT/overlay/shared/*.hpp   every src/shared header after the port rewrites
  OUT/units/<group>.cpp      one native TU per selected compile group
  OUT/units.cmake            RA2_NATIVE_UNITS list for CMake
  OUT/report.json            groups, sources, skipped rows and why

Port rewrites (mechanical, semantics-preserving for VC6 x86 -> LP64 arm64):
  * long / unsigned long        -> RA2_LONG / RA2_ULONG (32-bit, as on VC6)
  * integer literal suffix L    -> dropped (a VC6 long literal is 32-bit)
  * operator new(unsigned int   -> operator new(RA2_SIZE_T  (size_t on LP64)
  * layout size checks          -> disabled (they assert the 32-bit layout)
Rows whose source carries __asm (evidenced inline assembly) are skipped and
listed: they need a native behaviour implementation in native/port.

    python3 native/tools/native_units.py --out build/native/gen --subsystem core
"""
import argparse
import json
import re
import sys
from collections import OrderedDict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / 'tools/original_target'))
import catalog_io                    # noqa: E402  (pure Python, read-only)
from source_paths import source_path  # noqa: E402

# Subsystems: a compile group is selected when any of its sources lives under
# one of these prefixes (the whole group is taken, as the exact build does).
SUBSYSTEMS = {
    'core': [
        'src/recovered/classes/Random/', 'src/recovered/classes/RandomStraw/',
        'src/recovered/classes/CRCEngine/', 'src/recovered/classes/WWCRCEngine/',
        'src/recovered/classes/Matrix3D/', 'src/recovered/classes/Quaternion/',
        'src/recovered/classes/Vector3/', 'src/recovered/classes/Rect/',
        'src/recovered/unowned/Fast_',
    ],
}

LONG_PROTECT = [(re.compile(r'\blong(\s+)long\b'), 'RA2_LL_PLACEHOLDER'),
                (re.compile(r'\blong(\s+)double\b'), 'RA2_LD_PLACEHOLDER')]
RULES = [
    # COM class factories (TClassFactory<X>::CreateInstance, 74 accepted rows)
    # allocate X through a light in-file view padded to the VC6 size; the real
    # constructor then fills the arm64 layout, which is larger (pointers and
    # vectors double). The native view allocates through ra2port::factory_tag
    # (prelude: twice the light size, the same LP64 bound data_globals uses);
    # the constructor call and its arguments are unchanged.
    (re.compile(r'(TClassFactory<(\w+)>::CreateInstance\b(?:(?!TClassFactory<)[\s\S])*?)\bnew \2\('),
     r'\1new (ra2port::factory_alloc) \2('),
    # Game objects made through a light view (a stand-in class padded to the
    # VC6 size, e.g. unit_type.hpp's UnitClass, infantry_type.hpp's
    # InfantryClass: UnitTypeClass/InfantryTypeClass::Create_One_Of, Create_Units
    # 0x00660320) get the VC6 size from `new`, and the real constructor then
    # fills the larger arm64 layout (Scout1091: FootClass's constructor wrote
    # past a 0x5D8-byte InfantryClass). Every `new <Name>Class(` allocates
    # through the same tag as the factories (twice the size the unit sees);
    # the constructor call and its arguments are unchanged.
    (re.compile(r'(?<![\w:])new\s+([A-Z]\w*Class)\s*\('), r'new (ra2port::factory_alloc) \1('),
    # The depth/alpha rings' Wrap_Overflow/Wrap_Underflow (shared/blit.hpp) fold
    # raw addresses; their 80 callers pass `(unsigned int)pointer`, which drops
    # the upper half on arm64. The native view passes uintptr_t (the helpers
    # take it there: port/fixups.json blit.hpp). Same meaning.
    (re.compile(r'\bWrap_(Overflow|Underflow)\(\s*\(\s*unsigned(?:\s+int)?\s*\)'), r'Wrap_\1((uintptr_t)'),
    # Bit_Blit 0x00434B40 and RLE_Blit 0x004350C0 keep the depth/alpha ring
    # positions in `unsigned int` locals (they fold through Wrap_Overflow/
    # Wrap_Underflow above) and pass them on as `(void *)`: pointer width natively.
    (re.compile(r'\bunsigned int (depth_pointer|alpha_pointer) = 0;'), r'uintptr_t \1 = 0;'),
    (re.compile(r'\(unsigned int\)([AZ]BufferPtr->Get_Buffer_Ptr)'), r'(uintptr_t)\1'),
    # The sight-scan RadiusCount table (0x007A5908, 12 ints) is split by the
    # data tables at its second element (unknown_cell_count_7a590c names
    # 0x007A590C), so natively the unsized array is a 4-byte storage and
    # Sight_From read past it (Scout1091: the reveal landed on the wrong cells).
    # The native view reads the whole stored table (link/behaviour_map_m8.cpp).
    (re.compile(r'\bextern\s+int\s+const\s+(unknown_radius_(?:count|starts)_7a5908)\s*\[\s*\]\s*;'),
     r'int const *ra2port_radius_count_7a5908();   /* native: the whole stored table */\n#define \1 (ra2port_radius_count_7a5908())'),
    # VC6 variadic forwarding: `vswprintf(buffer, format, (char *)(&flags + 1))`
    # passes the address after the last named parameter as the va_list (the x86
    # stack holds the variadic arguments there). On arm64 that address is a
    # spilled register copy, so the formatter read garbage (Scout1098: the
    # sidebar credits through Plain_Text_Print 0x00497A60 showed 0). The native
    # view builds a real va_list from the same parameter (Plain_Text_Print,
    # Fancy_Text_Printf, MonoClass::Printf, JShell_Fatal, Add_Chat_Line, ...).
    (re.compile(r'\b(v\w*printf)\(([^;{}]*?),\s*\(\s*char\s*\*\s*\)\s*\(\s*&(\w+)\s*\+\s*1\s*\)\s*\);'),
     r'{ __builtin_va_list ra2_ap; __builtin_va_start(ra2_ap, \3); \1(\2, ra2_ap); __builtin_va_end(ra2_ap); }   /* NATIVE: was (char *)(&\3 + 1) */'),
    # An audio event's waiting source (AudioEvent::unknown_148, pointer-sized
    # natively: port/fixups.json audio.hpp) is stored with `(int)` casts and
    # read back into `int source` (Audio_Event_Begin 0x004054B0).
    (re.compile(r'(->unknown_148\s*=\s*)\(int\)'), r'\1(intptr_t)'),
    (re.compile(r'\bint(\s+source\s*=\s*event->unknown_148\s*;)'), r'intptr_t\1'),
    # Audio sample data addresses kept in ints (AudioSourceInfo/AudioSource/
    # AudioChannel members, pointer-sized natively: port/fixups.json audio.hpp):
    # the stores `= (int)data`, `= (int)operator new(size)`, `= (int)stream->Drain->Pointer`.
    (re.compile(r'(\b(?:unknown_0c|unknown_00|unknown_170)\s*=\s*)\(int\)(\s*(?:data\b|operator\s+new\s*\(|stream->Drain->Pointer\b))'), r'\1(intptr_t)\2'),
    # The dialog's DWL_USER slot (index 8) carries a pointer (`(LONG)&result`,
    # `(LONG)this`; read back as `(int *)GetWindowLongA(window, 8)`). A 32-bit
    # LONG truncates it on LP64: the native view stores and reads the pointer
    # through pointer-sized port calls (port/src/user32.cpp; Win64's
    # Set/GetWindowLongPtr(DWLP_USER) shape). Same meaning.
    (re.compile(r'\bSetWindowLongA\(\s*([^,()]+?)\s*,\s*(?:8|DWL_USER)\s*,\s*\(\s*(?:LONG|long|int)\s*\)\s*'),
     r'ra2port_set_dialog_user(\1, '),
    (re.compile(r'\bGetWindowLongA\(\s*([^,()]+?)\s*,\s*(?:8|DWL_USER)\s*\)'), r'ra2port_get_dialog_user(\1)'),
    # GWL_WNDPROC (-4) and GWL_USERDATA (-21) carry pointers too (the owner-draw
    # subclassing 0x005ED180 stores its control procedure and gets the original
    # one back; 0x005ECB00/0x005EDD30 keep a record pointer in GWL_USERDATA):
    # pointer-sized port calls (Win64's Set/GetWindowLongPtr). Scout1119 (M14).
    (re.compile(r'\bSetWindowLongA\(\s*([^,()]+?)\s*,\s*(-4|-21|GWL_WNDPROC|GWL_USERDATA)\s*,\s*\(\s*(?:LONG|long|int)\s*\)\s*'),
     r'ra2port_set_window_ptr(\1, \2, (intptr_t)'),
    (re.compile(r'\bGetWindowLongA\(\s*([^,()]+?)\s*,\s*(-4|-21|GWL_WNDPROC|GWL_USERDATA)\s*\)'), r'ra2port_get_window_ptr(\1, \2)'),
    (re.compile(r'\bunsigned\s+long(\s+int)?\b'), 'RA2_ULONG'),
    (re.compile(r'\bsigned\s+long(\s+int)?\b'), 'RA2_LONG'),
    (re.compile(r'\blong\s+int\b'), 'RA2_LONG'),
    (re.compile(r'\blong\b'), 'RA2_LONG'),
    (re.compile(r'\b((?:0[xX][0-9a-fA-F]+)|(?:\d+))([uU]?)[lL]([uU]?)\b'), r'\1\2\3'),
    (re.compile(r'operator\s+new(\s*\[\s*\])?\s*\(\s*unsigned\s+int\b'), r'operator new\1(RA2_SIZE_T'),
    (re.compile(r'^([ \t]*typedef[ \t]+char[ \t]+\w+[ \t]*\[.*\?[ \t]*1[ \t]*:[ \t]*-1[ \t]*\][ \t]*;)', re.M),
     r'/* native: 32-bit layout check */ // \1'),
    # VC6 CRT wide-string functions take 16-bit characters; libc's take 32-bit
    # wchar_t. The port supplies UTF-16 versions under ra2_ names.
    (re.compile(r'(?<![\w.])(_?wcs\w+|_?wcsicmp|_?wcsnicmp|vswprintf|swprintf|_vsnwprintf|_snwprintf|swscanf|mbstowcs|wcstombs|_putws|towupper|towlower|iswspace|iswdigit|iswalpha|_wtoi|wtoi)\b(?=\s*\()'),
     r'ra2_\1'),
    # Win32 imports and VC6 CRT functions whose names libc also defines with a
    # different contract: WSOCK32's sockaddr has no sin_len and SOCKET is an
    # unsigned int; VC6 rand() is its own LCG; VC6 fopen takes DOS paths; VC6
    # time_t is 32 bits; VC6 printf/scanf %ld is 32 bits (LP64 libc reads 64);
    # VC6 strcpy/strcat copy forward a byte at a time, and recovered code
    # shifts strings in place with them (strtrim 0x006EEF10: strcpy(s, s + 1)),
    # which libc leaves undefined for overlapping ranges; VC6's /O2 intrinsic
    # memcpy is rep movsd/movsb forward, and recovered code fills arrays by
    # copying onto themselves one element on (WarheadTypeClass 0x00720100:
    # memcpy(verses + 1, verses, ...) sets every Verses to 1.0). The
    # port supplies the Win32/VC6 behaviour under ra2_ names, so the process's
    # own libc (and SDL) keep theirs.
    (re.compile(r'(?<![\w.>:])(socket|bind|sendto|recvfrom|setsockopt|getsockopt|gethostname|gethostbyname|'
                r'inet_ntoa|htonl|htons|ntohl|ntohs|rand|srand|fopen|time|ctime|localtime|gmtime|mktime|'
                r'sprintf|vsprintf|_snprintf|_vsnprintf|sscanf|fprintf|vfprintf|printf|strcpy|strcat|strncpy|memcpy)\b(?=\s*\()'),
     r'ra2_\1'),
]


# VC6 (/Ze) puts a for-init declaration in the enclosing scope, and recovered
# sources rely on it. Hoist it there: `{ for(int i=0;` -> `{ int i=0; for(;`.
# Only when the for follows ; { or } so an unbraced if/else body is untouched.
FOR_INIT = re.compile(r'([;{}][ \t]*\n?[ \t]*)for[ \t]*\([ \t]*'
                      r'((?:const[ \t]+)?(?:unsigned[ \t]+|signed[ \t]+)?[A-Za-z_][\w:]*(?:<[^;()]*?>)?[ \t\*&]+'
                      r'[A-Za-z_]\w*[ \t]*=[^;()]*(?:\([^;()]*\)[^;()]*)*);')


# VC6 gives every enum the underlying type int, so an opaque `enum X;` is a
# complete type there. ISO C++ only allows that with a fixed underlying type, and
# then every declaration must repeat it: for each enum declared opaquely anywhere
# in the tree, `enum X;` -> `enum X : int;` and `enum X {` -> `enum X : int {`
# (int is exactly VC6's representation, so layouts and values are unchanged).
OPAQUE_ENUM = re.compile(r'\benum[ \t]+([A-Za-z_]\w*)[ \t]*;')
OPAQUE_ENUMS = set()


def scan_opaque_enums(texts):
    for text in texts:
        OPAQUE_ENUMS.update(OPAQUE_ENUM.findall(text))


def fix_enums(text):
    if not OPAQUE_ENUMS or 'enum' not in text:
        return text
    def opaque(m):
        return 'enum %s : int;' % m.group(1) if m.group(1) in OPAQUE_ENUMS else m.group(0)
    text = OPAQUE_ENUM.sub(opaque, text)
    def body(m):
        return 'enum %s : int%s' % (m.group(1), m.group(2)) if m.group(1) in OPAQUE_ENUMS else m.group(0)
    return re.sub(r'\benum[ \t]+([A-Za-z_]\w*)(\s*\{)', body, text)


def _balanced(text, i, open_ch, close_ch):
    """Index just past the bracket group starting at text[i] == open_ch, or -1."""
    depth = 0
    for j in range(i, len(text)):
        c = text[j]
        if c == open_ch:
            depth += 1
        elif c == close_ch:
            depth -= 1
            if depth == 0:
                return j + 1
        elif c in ';{}' and open_ch == '(':
            return -1
    return -1


def _postfix_end(text, i):
    """End of the postfix-expression operand of a C-style cast at text[i]
    (identifier / qualified name or parenthesised expression, then calls,
    subscripts, . -> members); returns (end, has_call_or_parens) or (-1, False)."""
    n = len(text)
    while i < n and text[i] in ' \t':
        i += 1
    called = False
    if i < n and text[i] == '(':
        i = _balanced(text, i, '(', ')')
        if i < 0:
            return -1, False
        called = True
    else:
        m = re.compile(r'(?:::)?[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*').match(text, i)
        if not m or m.group(0) in ('const', 'volatile', 'throw', 'override', 'return', 'sizeof'):
            return -1, False
        i = m.end()
    while i < n:
        if text[i] == '(':
            i = _balanced(text, i, '(', ')')
            if i < 0:
                return -1, False
            called = True
        elif text[i] == '[':
            i = _balanced(text, i, '[', ']')
            if i < 0:
                return -1, False
        elif text.startswith('->', i) or (text[i] == '.' and i + 1 < n and (text[i + 1].isalpha() or text[i + 1] == '_')):
            i += 2 if text[i] == '-' else 1
            m = re.compile(r'[A-Za-z_]\w*').match(text, i)
            if not m:
                return -1, False
            i = m.end()
        else:
            break
    return i, called


# VC6 lets a cast to a reference type take an rvalue (it reinterprets a
# temporary copy); ISO C++ does not. Wrap the operand in ra2port::as_lvalue
# (prelude), which binds the temporary for the full expression and is the
# identity on lvalues, so the cast reads the same bytes VC6's did.
REINTERPRET_REF = re.compile(r'\breinterpret_cast[ \t]*<')
CSTYLE_REF = re.compile(r'\((?:[ \t]*const[ \t]+)?[ \t]*[A-Za-z_][\w:]*(?:[ \t]*<[^()<>;]*(?:<[^()<>;]*>[^()<>;]*)*>)?'
                        r'(?:[ \t]+const)?[ \t]*&[ \t]*\)')


def fix_reference_casts(text):
    if '&' not in text:
        return text
    out = []
    pos = 0
    for m in REINTERPRET_REF.finditer(text):
        if m.start() < pos:
            continue
        close = _balanced(text, m.end() - 1, '<', '>')
        if close < 0 or '&' not in text[m.end():close]:
            continue
        j = close
        while j < len(text) and text[j] in ' \t':
            j += 1
        if j >= len(text) or text[j] != '(':
            continue
        end = _balanced(text, j, '(', ')')
        if end < 0:
            continue
        out.append(text[pos:j + 1])
        out.append('ra2port::as_lvalue(%s))' % text[j + 1:end - 1])
        pos = end
    out.append(text[pos:])
    text = ''.join(out)
    out = []
    pos = 0
    for m in CSTYLE_REF.finditer(text):
        if m.start() < pos:
            continue
        k = m.start() - 1
        while k >= 0 and text[k] in ' \t':
            k -= 1
        prev_word = re.search(r'([A-Za-z_]\w*)$', text[:k + 1])
        if k >= 0 and (text[k].isalnum() or text[k] in '_)]') and not (prev_word and prev_word.group(1) == 'return'):
            continue   # f(Type &) is a declarator or call, not a cast
        line_start = text.rfind('\n', 0, m.start()) + 1
        if '//' in text[line_start:m.start()] or re.search(r'operator\s*\S{1,3}\s*$', text[line_start:m.start()]):
            continue   # a comment, or operator@(Type &) declarator
        end, called = _postfix_end(text, m.end())
        if end < 0 or not called:
            continue
        operand = text[m.end():end]
        out.append(text[pos:m.end()])
        out.append('ra2port::as_lvalue(%s)' % operand.strip())
        pos = end
    out.append(text[pos:])
    return ''.join(out)


# Win32 message parameters and the *_PTR integers carry pointers (SendMessage
# lParam, DialogBoxParam init values, SetWindowLong user data, list-box item
# data). On Win32 they are 32 bits; the port implements that API on LP64, so
# they become pointer-sized as on Win64: WPARAM/UINT_PTR/ULONG_PTR/DWORD_PTR/
# SIZE_T -> uintptr_t, LPARAM/LRESULT/INT_PTR/LONG_PTR -> intptr_t. Every
# typedef of these names (win32_api.hpp, ghidra_types.hpp, seed prologues) is
# rewritten, so all units agree on the native API's argument widths. Values
# below 2^31 are unchanged; a pointer is no longer truncated.
# On in the CMake build (RA2_NATIVE_WIN64_PARAMS): fix_message_params respells
# the (HWND, unsigned, unsigned, long) procedure shape and drops conflicting
# Win32 redeclarations; port/fixups.json retypes the remaining callbacks/casts.
WIN64_PARAMS = False
POINTER_WIDTH = {'WPARAM': 'uintptr_t', 'UINT_PTR': 'uintptr_t', 'ULONG_PTR': 'uintptr_t',
                 'DWORD_PTR': 'uintptr_t', 'SIZE_T': 'uintptr_t', 'LPARAM': 'intptr_t',
                 'LRESULT': 'intptr_t', 'INT_PTR': 'intptr_t', 'LONG_PTR': 'intptr_t'}
TYPEDEF_LIST = re.compile(r'^([ \t]*)typedef[ \t]+([A-Za-z_][\w \t]*?)[ \t]+'
                          r'([A-Za-z_]\w*(?:[ \t]*,[ \t]*[A-Za-z_]\w*)*)[ \t]*;', re.M)


# The message-procedure shape (HWND, UINT, WPARAM, LPARAM) is spelled with the
# underlying VC6 types in many recovered prototypes, procedures, function-pointer
# typedefs and by-reference message filters: (HWND, unsigned [int], unsigned
# [int], long) with optional names and '&'. On Win32 these are the same types;
# with pointer-sized params they must be spelled with the Win32 names again, or
# the procedure no longer converts to DLGPROC/WNDPROC and redeclarations overload.
MSG_PARAMS = re.compile(r'\(\s*((?:struct\s+)?HWND(?:__)?\s*\*?\s*\w*)\s*,'
                        r'\s*(?:unsigned\s+int|unsigned|UINT)\b(\s*&?\s*\w*)\s*,'
                        r'\s*(?:unsigned\s+int|unsigned|UINT)\b(\s*&?\s*\w*)\s*,'
                        r'\s*(?:long|LPARAM)\b(\s*&?\s*\w*)\s*\)')
# Source-level redeclarations of Win32 APIs that win32_api.hpp already declares
# (with WPARAM/LPARAM); with pointer-sized params they conflict, so they go.
WIN32_REDECL = re.compile(r'^[ \t]*WIN32_IMPORT[^;\n]*\b(?:SendMessageA|SendDlgItemMessageA|EnumChildWindows)[ \t]*\((?=[^;]*\b(?:unsigned|long)\b)[^;]*\);[ \t]*$', re.M)
WNDENUMPROC_REDEF = re.compile(r'^[ \t]*typedef[ \t]+BOOL[ \t]*\([ \t]*(?:WINAPI|__stdcall)[ \t]*\*[ \t]*WNDENUMPROC[ \t]*\)(?=[^;]*\blong\b)[^;]*;[ \t]*$', re.M)


def fix_message_params(text):
    if not WIN64_PARAMS or 'HWND' not in text:
        return text
    text = WIN32_REDECL.sub('/* native: Win32 redeclaration dropped (win32_api.hpp) */', text)
    text = MSG_PARAMS.sub(lambda m: '(%s, unsigned int%s, uintptr_t%s, intptr_t%s)' % m.groups(), text)
    return WNDENUMPROC_REDEF.sub('/* native: WNDENUMPROC from win32_api.hpp */', text)


def fix_pointer_width_typedefs(text):
    text = fix_message_params(text)
    if not WIN64_PARAMS or not any(n in text for n in POINTER_WIDTH):
        return text
    def split(m):
        names = [n.strip() for n in m.group(3).split(',')]
        wide = [n for n in names if n in POINTER_WIDTH]
        if not wide or m.group(2).strip() in ('struct', 'class', 'enum', 'union'):
            return m.group(0)
        rest = [n for n in names if n not in POINTER_WIDTH]
        out = []
        if rest:
            out.append('%stypedef %s %s;' % (m.group(1), m.group(2), ', '.join(rest)))
        out.append(' '.join('typedef %s %s;' % (POINTER_WIDTH[n], n) for n in wide)
                   + ' /* native: pointer-sized as on Win64 */')
        return (m.group(1) if not rest else ' ').join(out) if rest else m.group(1) + out[0]
    return TYPEDEF_LIST.sub(split, text)


# VC6 CRT functions whose count/size parameter is size_t: the recovered
# declarations spell it `unsigned int` (VC6's size_t). On arm64 a 32-bit
# argument's upper register half is unspecified, and libc reads all 64 bits
# (memset(p, 0, w) with w from a packed Rect faulted). Declare them with
# size_t: the value is the same 32-bit count, zero-extended at the call.
CRT_SIZE_DECL = re.compile(r'((?:^|(?<=[;{}]))[ \t]*(?:extern[ \t]+"C"[ \t]*)?(?:__declspec\(\w+\)[ \t]*)?(?:const[ \t]+)?'
                           r'(?:void|char|int|unsigned(?:[ \t]+int)?|size_t)[ \t]*\*?[ \t]*(?:__cdecl[ \t]+)?'
                           r'(?:memset|memcpy|memmove|memcmp|memchr|_memicmp|strncpy|strncmp|strncat|_strnicmp|strnicmp|'
                           r'malloc|calloc|realloc|qsort|bsearch|fread|fwrite)[ \t]*)\(([^;{}]*?)\)(?=[ \t]*(?:;|throw|\{|\n))',
                           re.M)


def fix_crt_sizes(text):
    return CRT_SIZE_DECL.sub(lambda m: m.group(1) + '(' + re.sub(r'\bunsigned(?:\s+int)?\b(?!\s*(?:char|short|long))',
                                                                 'RA2_SIZE_T', m.group(2)) + ')', text)


def port(text):
    text = fix_pointer_width_typedefs(text)
    text = fix_crt_sizes(text)
    text = fix_enums(text)
    text = fix_reference_casts(text)
    text = FOR_INIT.sub(lambda m: '%s%s; for(;' % (m.group(1), m.group(2)), text)
    for rx, ph in LONG_PROTECT:
        text = rx.sub(ph, text)
    for rx, rep in RULES:
        text = rx.sub(rep, text)
    return text.replace('RA2_LL_PLACEHOLDER', 'long long').replace('RA2_LD_PLACEHOLDER', 'long double')


def groups(catalog):
    separate = {p['id'] for p in catalog.get('populations', []) if p.get('separate_units')}
    out = OrderedDict()
    for row in sorted(catalog['regions'], key=lambda r: r['source_order']):
        key = (row['population'], tuple(row['options']))
        if row['population'] in separate:
            key += (row['source'],)
        out.setdefault(key, []).append(row)
    return out


def load_fixups():
    data = json.loads((ROOT / 'native/port/fixups.json').read_text())
    fixups = {}
    for f in data['fixups']:
        fixups.setdefault(f['source'], []).append(f)
    renames = [(re.compile(r'(?<![\w:])%s\b' % re.escape(r['from'])), r['to']) for r in data.get('renames', [])]
    return fixups, renames


def load_view_layouts():
    """port/view_layouts.json (tools/view_audit.py --write): {source: [(view, {pad: arm64 size})]}."""
    path = ROOT / 'native/port/view_layouts.json'
    out = {}
    if path.exists():
        for v in json.loads(path.read_text())['views']:
            if v['pads'] or v.get('bytes'):
                out.setdefault(v['source'], []).append((v['view'], v['pads'], v.get('bytes', [])))
    return out


VIEW_LAYOUTS = None


def respell_views(path, text):
    """Padded views of real classes (light stand-ins) at the real class's arm64
    offsets: each listed padding member's size becomes RA2_NATIVE_PAD(arm64, vc6)
    (port prelude), the VC6 size on i686 (Scout1111; view_audit.py measures them)."""
    global VIEW_LAYOUTS
    if VIEW_LAYOUTS is None:
        VIEW_LAYOUTS = load_view_layouts()
    for view, pads, byte_members in VIEW_LAYOUTS.get(path, []):
        found = False
        for m in list(re.finditer(r'^(?:class|struct)\s+(?:__declspec\(\w+\)\s+)?%s\b[^;{]*\{' % re.escape(view),
                                  text, re.M)):
            end = text.find('\n};', m.end())
            body = text[m.end():end]
            for pad, size in pads.items():
                body, n = re.subn(r'\b((?:unsigned )?char %s)\[([^\]]+)\];' % re.escape(pad),
                                  lambda x: '%s[RA2_NATIVE_PAD(0x%X, %s)];' % (x.group(1), size, x.group(2)), body)
                if n < 1:   # (a member under #if defined(__LP64__) / #else: both spellings)
                    raise SystemExit('view_layouts.json: %s %s has no padding member %s' % (path, view, pad))
            # A view bool over a real byte (Scout1142): unsigned char, so a test reads
            # the whole byte as VC6 does (clang takes a bool's byte as 0 or 1).
            for member in byte_members:
                body, n = re.subn(r'\bbool(\s+%s\s*;)' % re.escape(member), r'unsigned char\1', body)
                if n < 1:
                    raise SystemExit('view_layouts.json: %s %s has no bool member %s' % (path, view, member))
            text = text[:m.end()] + body + text[end:]
            found = True
        if not found:
            raise SystemExit('view_layouts.json: %s defines no %s' % (path, view))
    return text


def apply_fixups(path, text, fixups, renames, used):
    for f in fixups.get(path, []):
        if f['old'] not in text:
            raise SystemExit('stale fixup for %s: %s' % (path, f['old']))
        text = text.replace(f['old'], f['new'])
        used.append(path)
    for rx, to in renames:
        text = rx.sub(to, text)
    return respell_views(path, text)


def emit(target, text):
    if not target.exists() or target.read_text(errors='replace') != text:
        target.write_text(text)


def load_drafts(catalog, report):
    """Unmatched launch-path functions carried as their best parked draft
    (native/drafts.json; never exact, each flagged with its score). A draft is
    appended to the compile group of its host_source, as the exact-tree join
    puts a candidate into its unit. A draft whose entry has since been accepted
    as a real function (a catalog region that is not a seed leaf: Leaf_/seed_
    owners only hold the original bytes' shape) is left out: the exact source
    supersedes it."""
    accepted = {int(r['entry']) for r in catalog['regions']
                if not re.search(r'@Leaf_[0-9a-f]+@@|@seed_[0-9a-f]+@@', r['symbol'])}
    by_host = OrderedDict()
    for d in json.loads((ROOT / 'native/drafts.json').read_text())['drafts']:
        if int(d['entry'], 16) in accepted:
            report.setdefault('superseded_drafts', []).append(d['entry'])
            continue
        by_host.setdefault(d['host_source'], []).append(d)
    return by_host


def draft_part(d, fixups, renames, used, manifest, unit):
    row = dict(entry=d['entry'], name=d.get('name'), plan=d['plan'], score=d['score'],
               last_lane_score=d.get('last_lane_score'), draft=d.get('draft'), origin=d.get('origin'),
               author=d.get('author'), draft_found_by=d.get('draft_found_by'),
               raw_seed=d.get('raw_seed', False), exact=False, unit=unit)
    manifest.append(row)
    if d.get('native', 'compiles') != 'compiles':
        row['status'] = d['native']
        return ''
    # Included drafts are tracked text (native/drafts/<entry>.cpp); the build
    # reads nothing from ignored lane scratch.
    if not d.get('draft') or not d['draft'].startswith('native/drafts/'):
        raise SystemExit('draft %s: an included draft must be tracked under native/drafts/' % d['entry'])
    path = ROOT / d['draft']
    if not path.is_file():
        raise SystemExit('draft %s: %s missing' % (d['entry'], d['draft']))
    text = path.read_text(errors='replace')
    if '__asm' in text or '_asm ' in text:
        row['status'] = 'skipped: inline assembly'
        return ''
    row['status'] = 'included'
    text = apply_fixups(d['draft'], text, fixups, renames, used)
    return ('// NON-EXACT DRAFT %s (%s, score %s): %s\n#line 1 "%s"\n%s\n'
            % (d['entry'], d['plan'], d['score'], d['draft'], path.as_posix(), port(text)))


# Static initialisers. The original's CRT runs _initterm over .CRT$XCA..XCZ
# before WinMain (native/initterm.json, from initterm_manifest.py). Natively the
# compiler emits each unit's initialisers into __mod_init_func and the linker
# keeps input order, so units are listed in the order of their first original
# initialiser (units without one first, in source order), and the explicit
# entries (named init functions the table points at) are called from small
# generated units at their original positions. Seed/scaffold entries
# (transliterated absolute addresses or truncated function pointers) cannot run
# natively and are skipped and reported.
EXPLICIT_DECL = [
    (re.compile(r'^\?([A-Za-z_]\w*)@@Y[AGI]XXZ$'), lambda m: ('void %s();' % m.group(1), '%s();' % m.group(1))),
    (re.compile(r'^\?([A-Za-z_]\w*)@([A-Za-z_]\w*)@@Y[AGI]XXZ$'),
     lambda m: ('namespace %s { void %s(); }' % (m.group(2), m.group(1)), '%s::%s();' % (m.group(2), m.group(1)))),
    (re.compile(r'^_([A-Za-z_]\w*)$'), lambda m: ('extern "C" void %s();' % m.group(1), '%s();' % m.group(1))),
]


def native_rewrites():
    """Entry addresses with a native rewrite (native/link/initterm_rewrites.cpp)."""
    path = ROOT / 'native/link/initterm_rewrites.cpp'
    if not path.exists():
        return set()
    text = path.read_text()
    found = set(re.findall(r'^void Initterm_([0-9A-F]{8})\(\)', text, re.M))
    found |= set(re.findall(r'^RA2_ZERO_STATIC\(([0-9A-F]{8}),', text, re.M))
    return found


REWRITES = None


def explicit_call(entry, text):
    """(declaration, call) for an explicit initialiser, or (None, reason)."""
    global REWRITES
    if REWRITES is None:
        REWRITES = native_rewrites()
    address = entry['entry'][2:].upper().rjust(8, '0')
    if address in REWRITES:
        return 'void Initterm_%s();' % address, 'Initterm_%s();  // native rewrite' % address
    if 'seed-generated' in text or entry['symbol'].startswith(('?get@Leaf_', '_recover_')):
        return None, 'seed/scaffold source (absolute addresses or truncated pointers): needs a native rewrite'
    for rx, make in EXPLICIT_DECL:
        m = rx.match(entry['symbol'])
        if m:
            return make(m)
    return None, 'symbol shape not handled: %s' % entry['symbol']


MARK_PREFIX = 'extern "C" int ra2port_initterm_mark(int);'
MARK = MARK_PREFIX + ' static int ra2_initterm_mark_ = ra2port_initterm_mark(%d);\n'


def initterm_order(names, unit_of, texts, out, unit_texts):
    """Reorder unit names for the original initialiser order; write explicit
    initialiser units. Returns (ordered names, report)."""
    manifest = json.loads((ROOT / 'native/initterm.json').read_text())
    entries = manifest['entries']
    placed, items, deviations, skipped, called = set(), [], [], [], []
    pending, first = [], {}

    def flush():
        if not pending:
            return
        name = 'units/initterm_%04d.cpp' % pending[0][0]['index']
        decls = '\n'.join(sorted({d for _, d, _ in pending}))
        calls = '\n'.join('    %s  // %s %s %s' % (c, e['index'], e['entry'], e['source']) for e, _, c in pending)
        emit(out / name, '// generated by native/tools/native_units.py: explicit _initterm entries %s-%s\n'
             '// (native/initterm.json), called here so they run at their original position.\n%s\n'
             '%snamespace {\nstruct InitTerm { InitTerm() {\n%s\n} } initterm;\n}\n'
             % (pending[0][0]['index'], pending[-1][0]['index'], decls, MARK % pending[0][0]['index'], calls))
        items.append(name)
        del pending[:]

    for e in entries:
        unit = unit_of.get(e['source'])
        if e['kind'] == 'explicit':
            decl, call = explicit_call(e, texts.get(e['source'], ''))
            if decl is None:
                skipped.append(dict(index=e['index'], entry=e['entry'], symbol=e['symbol'], source=e['source'], reason=call))
            else:
                pending.append((e, decl, call))
                called.append(e['index'])
            continue
        if unit is None:
            skipped.append(dict(index=e['index'], entry=e['entry'], symbol=e['symbol'], source=e['source'],
                                reason='source not in this native view'))
            continue
        if unit in placed:
            if pending or (items and items[-1] != unit):
                deviations.append(dict(index=e['index'], entry=e['entry'], source=e['source'], unit=unit,
                                       after=items[-1]))
            continue
        flush()
        placed.add(unit)
        items.append(unit)
        first[unit] = e['index']
    flush()
    # Progress marker (diagnostic): the first dynamic initialiser of each unit
    # records the original _initterm index the unit starts at, so a crash or
    # trap during start-up names how far the table got (ra2port_initterm_mark,
    # RA2_NATIVE_INIT_TRACE=1 prints each; the host reports the last at main).
    for unit, index in first.items():
        if unit in unit_texts:
            head, _, rest = unit_texts[unit].partition('\n')
            unit_texts[unit] = head + '\n' + MARK % index + rest
    ordered = [n for n in names if n not in placed] + items
    return ordered, dict(entries=len(entries), units=len(placed), explicit_called=len(called),
                         skipped=skipped, out_of_order=deviations)


def place_member_aliases(unit_texts):
    """data/member_aliases.tsv (tools/member_aliases.py): a name inside a
    class-typed global is an assembler alias of the owner's symbol plus the
    member's native offset, emitted in the unit that defines the owner (one
    assembly file, so the offset resolves); owners with BSS storage get theirs
    in data/bss_globals.cpp (tools/data_globals.py)."""
    tsv = ROOT / 'native/data/member_aliases.tsv'
    if not tsv.exists():
        return {}
    by_owner = OrderedDict()
    for line in tsv.read_text().split('\n'):
        if line and not line.startswith(('#', 'symbol\t')):
            r = line.split('\t')
            by_owner.setdefault((r[1], r[6]), []).append(r)
    placed = {}
    for (owner_symbol, owner), rows in by_owner.items():
        rx = re.compile(r'^(?!extern\b|static\b|typedef\b|return\b)[A-Za-z_][^;(){}=\n]*?[\s*&]%s\s*(?:;|=|\(|\{)'
                        % re.escape(owner), re.M)
        for unit, text in unit_texts.items():
            if rx.search(text):
                unit_texts[unit] = text + '\n// native member aliases (data/member_aliases.tsv)\n' + ''.join(
                    '__asm__(".globl %s\\n.set %s, %s + %s\\n");  // %s: %s %s\n'
                    % (r[0], r[0], owner_symbol, r[2], r[5], r[4], r[7][-60:]) for r in rows)
                placed[owner] = dict(unit=unit, names=len(rows))
                break
    return placed



def bss_symbols():
    """Original address -> link symbol of the zero storage data/bss_globals.cpp
    defines for a global the recovered code names (one storage per address)."""
    out = {}
    path = Path(__file__).resolve().parent.parent / 'data/bss_globals.cpp'
    if path.exists():
        for line in path.read_text().splitlines():
            m = re.match(r'^alignas\(\d+\) unsigned char \w+\[\d+\] __asm__\("(_\w+)"\);\s*// (0x[0-9A-F]{8}) ', line)
            if m:
                out[int(m.group(2), 16)] = m.group(1)
    return out


HEADER_STATIC_COPIES = [0]


def header_static_copies(text, bss_by_address):
    """Header and file statics (the per-unit copies of a shared header's file statics, e.g.
    unknown_geometry.hpp's unknown_geometry_j) live at one original address that
    other units also read through an extern name (unknown_bridge_height_ad057c,
    0x00AD057C). Natively the extern name is its own zeroed storage
    (data/bss_globals.cpp); after the header-statics row's own initialisers, copy
    each static's value there (Scout1103: UnitClass slot 59 cleared the bridge
    level's vehicle bit because the bridge height read 0, so a deploying MCV
    still blocked its own cell). Same value at the same _initterm position."""
    lines = []
    for m in re.finditer(r'^// @ref _([A-Za-z]\w*)=([0-9a-fA-F]{8})\s*$', text, re.M):
        name, address = m.group(1), int(m.group(2), 16)
        symbol = bss_by_address.get(address)
        if symbol is None or not re.search(r'\bstatic\b[^;\n]*\b%s\b' % re.escape(name), text + header_static_text(text)):
            continue
        HEADER_STATIC_COPIES[0] += 1
        lines.append('extern "C" unsigned char native_hs_%x[] __asm__("%s");\n'
                     'static int const native_hs_copy_%d = (__builtin_memcpy(native_hs_%x, &%s, sizeof %s), 0);\n'
                     % (address, symbol, HEADER_STATIC_COPIES[0], address, name, name))
    if not lines:
        return ''
    return '// NATIVE (native_units.py header_static_copies): header statics to their extern names\n' + ''.join(lines)


def header_static_text(text):
    """The src/shared headers a header-statics row includes (where its statics are defined)."""
    out = ''
    for inc in re.findall(r'^#include "shared/([\w.]+)"', text, re.M):
        path = ROOT / 'src/shared' / inc
        if path.exists():
            out += path.read_text(errors='replace')
    return out


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--out', type=Path, required=True)
    ap.add_argument('--subsystem', default='core', choices=sorted(SUBSYSTEMS) + ['all'])
    ap.add_argument('--drafts', action='store_true', help='append the native/drafts.json drafts to their host groups')
    ap.add_argument('--win64-params', action='store_true',
                    help='pointer-sized WPARAM/LPARAM/LRESULT/*_PTR, as on Win64 (on in the CMake build)')
    args = ap.parse_args()
    out = args.out.resolve()
    global WIN64_PARAMS
    WIN64_PARAMS = args.win64_params
    (out / 'overlay/shared').mkdir(parents=True, exist_ok=True)
    (out / 'units').mkdir(parents=True, exist_ok=True)
    catalog = catalog_io.load(catalog_io.locate(ROOT), references=False)
    fixups, renames = load_fixups()
    used = []
    headers = {h: h.read_text(errors='replace') for h in sorted((ROOT / 'src/shared').glob('*.hpp'))}
    texts = {r['source']: source_path(ROOT, r['source']).read_text(errors='replace') for r in catalog['regions']}
    scan_opaque_enums(list(headers.values()) + list(texts.values()))
    for h, text in headers.items():
        rel = h.relative_to(ROOT).as_posix()
        emit(out / 'overlay/shared' / h.name, port(apply_fixups(rel, text, fixups, renames, used)))
    prefixes = None if args.subsystem == 'all' else SUBSYSTEMS[args.subsystem]
    report = dict(subsystem=args.subsystem, groups=[], skipped=[])
    names = []
    by_host = load_drafts(catalog, report) if args.drafts else {}
    manifest = []
    unit_texts = OrderedDict()
    bss_by_address = bss_symbols()
    for index, (key, rows) in enumerate(groups(catalog).items()):
        sources = [r['source'] for r in rows]
        if prefixes is not None and not any(s.startswith(p) for s in sources for p in prefixes):
            continue
        parts = []
        for r in rows:
            # Fixups first: a fixup may replace an evidenced __asm block with its
            # native meaning (Main_Game's exception-return capture).
            text = apply_fixups(r['source'], texts[r['source']], fixups, renames, used)
            if '__asm' in text or '_asm ' in text:
                report['skipped'].append(dict(source=r['source'], reason='inline assembly: native implementation needed'))
                continue
            parts.append('#line 1 "%s"\n%s\n' % ((ROOT / r['source']).as_posix(), port(text)))
            # File statics too (Scout1115): Tactical.CPP's unknown_scale_abe490
            # (0x00ABE490, pixels per lepton of height) is read elsewhere through
            # the extern unknown_abe490; zero there, every object's Z adjust lost
            # its height and the depth test hid units and infantry.
            parts.append(header_static_copies(text, bss_by_address))
        if not parts:
            continue
        name = 'g%04d_%s.cpp' % (index, key[0].replace('population-', 'p'))
        for source in sources:
            for d in by_host.get(source, []):
                parts.append(draft_part(d, fixups, renames, used, manifest, 'units/' + name))
        unit_texts['units/' + name] = (
            '// generated by native/tools/native_units.py: %s %s\n' % (key[0], ' '.join(key[1])) + ''.join(parts))
        names.append('units/' + name)
        report['groups'].append(dict(unit=name, population=key[0], options=list(key[1]), sources=sources))
    # Drafts with no host group (host_source null) form their own unit: they
    # need a header context no accepted group provides (e.g. WinMain, whose
    # Startup.CPP unit has no accepted row). Linked after every initialiser unit.
    own_units = []
    for d in (by_host.get(None, []) if prefixes is None else []):
        name = 'units/draft_%s.cpp' % d['entry'].lower().replace('0x', '')
        part = draft_part(d, fixups, renames, used, manifest, name)
        if part:
            unit_texts[name] = '// generated by native/tools/native_units.py: own-unit draft %s\n%s' % (d['entry'], part)
            own_units.append(name)
    if args.drafts:
        (out / 'drafts.json').write_text(json.dumps(dict(
            about='Non-exact drafts in this native tree (from native/drafts.json): no exact-match credit.',
            drafts=manifest), indent=1))
    unused = sorted(set(fixups) - set(used))
    if unused and args.subsystem == 'all':
        raise SystemExit('fixups for sources not in the tree: %s' % ', '.join(unused))
    report['fixups'] = sorted(set(used))
    report['opaque_enums'] = sorted(OPAQUE_ENUMS)
    if args.subsystem == 'all':
        unit_of = {}
        for g in report['groups']:
            for source in g['sources']:
                unit_of[source] = 'units/' + g['unit']
        names, report['initterm'] = initterm_order(names, unit_of, texts, out, unit_texts)
    names += own_units
    report['member_aliases'] = place_member_aliases(unit_texts)
    for name, text in unit_texts.items():
        emit(out / name, text)
    (out / 'units.cmake').write_text('set(RA2_NATIVE_UNITS\n%s)\n' % ''.join(
        '  "${RA2_NATIVE_GEN}/%s"\n' % n for n in names))
    (out / 'report.json').write_text(json.dumps(report, indent=1))
    print('%d units, %d sources, %d skipped, %d fixed-up files' % (
        len(names), sum(len(g['sources']) for g in report['groups']), len(report['skipped']), len(set(used))))


if __name__ == '__main__':
    main()
