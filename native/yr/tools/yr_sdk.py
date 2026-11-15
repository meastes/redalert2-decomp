#!/usr/bin/env python3
"""The Win32/DirectX headers for the native YR build (lane Y1), generated at configure time.

ra2-decomp's tree includes the system headers (<windows.h>, <ddraw.h>, <dsound.h>, <comdef.h>, ...).
The native build takes them from the mingw-w64 headers of the local llvm-mingw toolchain (the one
the Windows build uses; read at build time, never copied into the repository), rewritten for
the VC6 widths on LP64 arm64 (long -> RA2_LONG, as the recovered trees) and with the C runtime
pieces that would clash with the host's C library replaced by the host's own (native/yr/sdk
holds those replacements, searched first). The C library itself is the host's.

    python3 native/yr/tools/yr_sdk.py --mingw <llvm-mingw>/generic-w64-mingw32/include --out build/yr-native/mingw
"""
import argparse
import re
from pathlib import Path

RULES = [(re.compile(r'\blong\s+long\b'), 'RA2_LL_PH'), (re.compile(r'\blong\s+double\b'), 'RA2_LD_PH'),
         (re.compile(r'\bunsigned\s+long(\s+int)?\b'), 'RA2_ULONG'), (re.compile(r'\bsigned\s+long(\s+int)?\b'), 'RA2_LONG'),
         (re.compile(r'\blong\s+int\b'), 'RA2_LONG'), (re.compile(r'\blong\b'), 'RA2_LONG')]


# Headers whose `long` is an overload of `int` (comutil.h's _variant_t/_bstr_t constructors and
# conversions): rewritten, the two overloads would be one. They keep `long` (64-bit, unused as data).
KEEP_LONG = {'comutil.h'}
# Winsock's BSD types under their own names (the host's <sys/types.h> has a 64-bit u_long and its
# own fd_set and timeval): the Winsock structures keep their 32-bit layout and the port's wsock32
# sees them. The YR tree itself does not name these types.
TOKENS = [(re.compile(r'\bu_long\b'), 'ra2ws_u_long'), (re.compile(r'\bfd_set\b'), 'ra2ws_fd_set'),
          (re.compile(r'\btimeval\b'), 'ra2ws_timeval')]
# The host's <arpa/inet.h> defines the byte-order functions as macros; Winsock declares them.
PREPEND = {name: '#undef htonl\n#undef htons\n#undef ntohl\n#undef ntohs\n' for name in ('winsock.h', 'winsock2.h')}


# After the rewrites, per header:
# - corecrt.h: mbstate_t is the host's (its <wchar.h> declares it first).
# - comip.h: a smart pointer compared with NULL (the host's __null, a long on LP64) has int and
#   long long overloads to choose from; VC6 had int. A long overload decides it as int would.
# - winuser.h: DLGPROC returns BOOL as the YR tree's dialog procedures do (the Win64 header has
#   INT_PTR; an int return zero-extends into the register on arm64, so the port reads it whole).
# - _mingw.h, rpcndr.h, winnt.h: COM interfaces carry their IIDs as __declspec(uuid) and
#   __uuidof is clang's own (-fms-extensions), as under MSVC: the YR tree declares its interfaces
#   with MIDL_INTERFACE("...") and takes __uuidof of them, and comdef.h's smart-pointer typedefs
#   (comdefsp.h) are only made where __uuidof is real.
POST = {
    '_mingw.h': [('#ifdef _MSC_VER\n#define USE___UUIDOF\t1', '#if defined(_MSC_VER) || defined(__clang__)\n#define USE___UUIDOF\t1')],
    'rpcndr.h': [('#define MIDL_INTERFACE(x) struct\n', '#define MIDL_INTERFACE(x) struct __declspec(uuid(x)) __declspec(novtable)\n')],
    'winnt.h': [('#ifndef DECLSPEC_UUID\n#define DECLSPEC_UUID(x)\n', '#ifndef DECLSPEC_UUID\n#define DECLSPEC_UUID(x) __declspec(uuid(x))\n')],
    'corecrt.h': [('#ifndef __CYGWIN__ /* Cygwin defines mbstate_t themselves */',
                   '#if !defined(__CYGWIN__) && !defined(__APPLE__) /* the host defines mbstate_t */')],
    'comip.h': [('bool operator!=(int null) { return !(operator==(null)); }',
                 'bool operator!=(int null) { return !(operator==(null)); }\n'
                 '  bool operator==(__typeof__(0L) null) { return operator==((int)null); }\n'
                 '  bool operator!=(__typeof__(0L) null) { return !(operator==((int)null)); }'),
                ('  _com_ptr_t(int null) : m_pInterface(NULL) {',
                 '  _com_ptr_t(__typeof__(0L) null) : m_pInterface(NULL) { if(null!=0) { _com_issue_error(E_POINTER); } }\n'
                 '  _com_ptr_t(int null) : m_pInterface(NULL) {')],
    'winuser.h': [('typedef INT_PTR (CALLBACK *DLGPROC) (HWND, UINT, WPARAM, LPARAM);',
                   'typedef BOOL (CALLBACK *DLGPROC) (HWND, UINT, WPARAM, LPARAM);')],
}


def rewrite(text, name=''):
    for rx, rep in TOKENS:
        text = rx.sub(rep, text)
    text = PREPEND.get(name, '') + text
    for old, new in POST.get(name, ()):
        if old not in text:
            raise SystemExit('yr_sdk: %s: patch text not found: %r' % (name, old[:60]))
        text = text.replace(old, new)
    if name == 'winuser.h':
        # Win64 headers withdraw the Win32 window-data names (DWL_USER, GWL_USERDATA, ...): the YR
        # tree uses them and the port rewrites send those calls to pointer-sized storage.
        text = re.sub(r'^[ \t]*#[ \t]*undef[ \t]+(?:DWL|GWL)_\w+[ \t]*$', '', text, flags=re.M)
    if name in KEEP_LONG:
        # its __LONG32 overloads are unsigned int on LP64, the same as the int ones beside them
        return text.replace('__LONG32', 'long')
    for rx, rep in RULES:
        text = rx.sub(rep, text)
    return text.replace('RA2_LL_PH', 'long long').replace('RA2_LD_PH', 'long double')


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('--mingw', required=True)
    ap.add_argument('--out', required=True)
    a = ap.parse_args()
    src, out = Path(a.mingw), Path(a.out)
    n = 0
    for f in src.rglob('*'):
        if not f.is_file():
            continue
        target = out / f.relative_to(src)
        text = rewrite(f.read_text(encoding='latin-1'), f.relative_to(src).as_posix())
        target.parent.mkdir(parents=True, exist_ok=True)
        if not target.exists() or target.read_text(encoding='latin-1') != text:
            target.write_text(text, encoding='latin-1')
        n += 1
    print('yr_sdk: %d headers -> %s' % (n, out))


if __name__ == '__main__':
    main()
