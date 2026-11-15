#!/usr/bin/env python3
"""Type-aware member renames for apply_rename (`member_renames`).

    member_rename.py CLASS OLD NEW [--mangled FULL] [--json]     (plan only; prints the edits)

A member of class X is renamed at every use whose static type resolves it to X's
declaration (X itself or a class derived from X that does not declare its own member of that
name), and nowhere else: another class's member, a local or a global of the same spelling
keeps its name. The resolution is clang's (libclang through ctypes, the Command Line Tools'
libclang.dylib, target i686-pc-windows-msvc with the MS extensions): each translation unit that
spells the name is parsed, every cursor that declares or references the member is matched by
its USR, and every identifier token of that spelling in the files it touches must be explained
by some cursor. A token no parse explains (an inactive #if branch no unit takes, a macro body, a
template that is never instantiated, code clang could not parse), one that two parses explain
differently, or a virtual member that a derived class overrides, refuses the rename: the tool
cannot tell, so it does not guess.

`--mangled FULL` selects one overload (its MSVC decorated name, as the catalog binds it); without
it every member of X with that spelling is renamed. apply_rename turns the plan into source
edits and catalog renames (the decorated names clang gives for the member, `?OLD@X@` becoming
`?NEW@X@`); an overload selected by name goes through `overload_renames` (full name and address).
Comments are not code: a comment token is renamed only where it is written `X::OLD`.
"""
import ctypes
import json
import re
import sys
from pathlib import Path

REPOSITORY = Path(__file__).resolve().parents[2]
LIBCLANG = '/Library/Developer/CommandLineTools/usr/lib/libclang.dylib'
CLANG_ARGS = ['-x', 'c++', '-target', 'i686-pc-windows-msvc', '-fms-extensions', '-fms-compatibility',
              '-fms-compatibility-version=12', '-fno-builtin', '-Wno-everything', '-ferror-limit=0', '-Isrc']

# cursor kinds (Index.h)
CXX_METHOD, FIELD_DECL, VAR_DECL, FUNCTION_TEMPLATE = 21, 6, 9, 30
MEMBER_REF, MEMBER_REF_EXPR, DECL_REF_EXPR, OVERLOADED_DECL_REF = 47, 102, 101, 49
CHILD_VISIT_RECURSE = 2
CXX_BASE_SPECIFIER = 44


class CXCursor(ctypes.Structure):
    _fields_ = [('kind', ctypes.c_int), ('xdata', ctypes.c_int), ('data', ctypes.c_void_p * 3)]


class CXString(ctypes.Structure):
    _fields_ = [('data', ctypes.c_void_p), ('flags', ctypes.c_uint)]


class CXSourceLocation(ctypes.Structure):
    _fields_ = [('ptr_data', ctypes.c_void_p * 2), ('int_data', ctypes.c_uint)]


VISITOR = ctypes.CFUNCTYPE(ctypes.c_int, CXCursor, CXCursor, ctypes.c_void_p)
_lib = None


def lib():
    global _lib
    if _lib is None:
        L = ctypes.CDLL(LIBCLANG)
        sig = {
            'clang_createIndex': (ctypes.c_void_p, [ctypes.c_int, ctypes.c_int]),
            'clang_parseTranslationUnit': (ctypes.c_void_p, [ctypes.c_void_p, ctypes.c_char_p,
                                           ctypes.POINTER(ctypes.c_char_p), ctypes.c_int, ctypes.c_void_p,
                                           ctypes.c_uint, ctypes.c_uint]),
            'clang_disposeTranslationUnit': (None, [ctypes.c_void_p]),
            'clang_getTranslationUnitCursor': (CXCursor, [ctypes.c_void_p]),
            'clang_visitChildren': (ctypes.c_uint, [CXCursor, VISITOR, ctypes.c_void_p]),
            'clang_getCursorReferenced': (CXCursor, [CXCursor]),
            'clang_getCanonicalCursor': (CXCursor, [CXCursor]),
            'clang_getCursorSemanticParent': (CXCursor, [CXCursor]),
            'clang_getCursorUSR': (CXString, [CXCursor]),
            'clang_getCursorSpelling': (CXString, [CXCursor]),
            'clang_Cursor_getMangling': (CXString, [CXCursor]),
            'clang_getCString': (ctypes.c_char_p, [CXString]),
            'clang_disposeString': (None, [CXString]),
            'clang_getCursorLocation': (CXSourceLocation, [CXCursor]),
            'clang_getSpellingLocation': (None, [CXSourceLocation, ctypes.POINTER(ctypes.c_void_p),
                                          ctypes.POINTER(ctypes.c_uint), ctypes.POINTER(ctypes.c_uint),
                                          ctypes.POINTER(ctypes.c_uint)]),
            'clang_getExpansionLocation': (None, [CXSourceLocation, ctypes.POINTER(ctypes.c_void_p),
                                           ctypes.POINTER(ctypes.c_uint), ctypes.POINTER(ctypes.c_uint),
                                           ctypes.POINTER(ctypes.c_uint)]),
            'clang_getFileName': (CXString, [ctypes.c_void_p]),
            'clang_Cursor_isNull': (ctypes.c_int, [CXCursor]),
            'clang_CXXMethod_isVirtual': (ctypes.c_uint, [CXCursor]),
            'clang_getOverriddenCursors': (None, [CXCursor, ctypes.POINTER(ctypes.POINTER(CXCursor)),
                                           ctypes.POINTER(ctypes.c_uint)]),
            'clang_disposeOverriddenCursors': (None, [ctypes.POINTER(CXCursor)]),
        }
        for name, (restype, argtypes) in sig.items():
            function = getattr(L, name)
            function.restype, function.argtypes = restype, argtypes
        _lib = L
    return _lib


def text_of(cxstring):
    L = lib()
    value = L.clang_getCString(cxstring)
    L.clang_disposeString(cxstring)
    return value.decode('latin-1') if value else ''


def location(cursor):
    """(file relative to the repository, byte offset) of the cursor's spelling, plus whether the
    spelling and expansion differ (a token written in a macro)."""
    L = lib()
    loc = L.clang_getCursorLocation(cursor)
    out = []
    for get in (L.clang_getSpellingLocation, L.clang_getExpansionLocation):
        f, line, col, off = ctypes.c_void_p(), ctypes.c_uint(), ctypes.c_uint(), ctypes.c_uint()
        get(loc, ctypes.byref(f), ctypes.byref(line), ctypes.byref(col), ctypes.byref(off))
        name = text_of(L.clang_getFileName(f)) if f.value else None
        out.append((name, off.value))
    (sfile, soff), (efile, eoff) = out
    if sfile is None:
        return None, None, False
    path = Path(sfile)
    path = (REPOSITORY / path).resolve() if not path.is_absolute() else path.resolve()
    try:
        rel = path.relative_to(REPOSITORY.resolve()).as_posix()
    except ValueError:
        rel = path.as_posix()
    return rel, soff, (sfile, soff) != (efile, eoff)


class Unit:
    """One parsed translation unit: every cursor spelled `name` that declares or references
    something, by location: {(file, offset): {USR, ...}}, plus the declarations of `name`."""
    def __init__(self, path, name, new=None, extra=()):
        L = lib()
        self.path, self.name = path, name
        import os
        os.chdir(REPOSITORY)
        index = L.clang_createIndex(0, 0)
        args = [a.encode() for a in list(CLANG_ARGS) + list(extra)]
        self.tu = L.clang_parseTranslationUnit(index, str(REPOSITORY / path).encode(),
                                               (ctypes.c_char_p * len(args))(*args), len(args), None, 0, 0x1)
        if not self.tu:
            raise SystemExit('member_rename: clang could not parse %s' % path)
        self.at, self.decls, self.macro = {}, {}, set()
        self.bases, self.new_owners, self.new_virtual = set(), set(), set()

        @VISITOR
        def visit(cursor, parent, data):
            if cursor.kind == CXX_BASE_SPECIFIER:
                base = L.clang_getCursorReferenced(cursor)
                if not L.clang_Cursor_isNull(base):
                    self.bases.add((text_of(L.clang_getCursorSpelling(parent)), text_of(L.clang_getCursorSpelling(base))))
                return CHILD_VISIT_RECURSE
            referenced = L.clang_getCursorReferenced(cursor)
            spelled = text_of(L.clang_getCursorSpelling(referenced)) if not L.clang_Cursor_isNull(referenced) else ''
            if new and spelled == new and cursor.kind in (CXX_METHOD, FIELD_DECL, VAR_DECL, FUNCTION_TEMPLATE):
                self.new_owners.add(text_of(L.clang_getCursorSpelling(L.clang_getCursorSemanticParent(cursor))))
                if cursor.kind == CXX_METHOD and L.clang_CXXMethod_isVirtual(cursor):
                    self.new_virtual.add(text_of(L.clang_getCursorSpelling(L.clang_getCursorSemanticParent(cursor))))
            if spelled == name:
                usr = text_of(L.clang_getCursorUSR(L.clang_getCanonicalCursor(referenced)))
                where, offset, in_macro = location(cursor)
                if where is not None and usr:
                    self.at.setdefault((where, offset), set()).add(usr)
                    if in_macro:
                        self.macro.add((where, offset))
                if cursor.kind in (CXX_METHOD, FIELD_DECL, VAR_DECL, FUNCTION_TEMPLATE) and usr:
                    owner = L.clang_getCursorSemanticParent(cursor)
                    self.decls.setdefault(usr, dict(
                        owner=text_of(L.clang_getCursorSpelling(owner)),
                        owner_usr=text_of(L.clang_getCursorUSR(owner)), kind=cursor.kind,
                        mangled=text_of(L.clang_Cursor_getMangling(cursor)),
                        virtual=bool(cursor.kind == CXX_METHOD and L.clang_CXXMethod_isVirtual(cursor)),
                        overrides=overridden_usrs(cursor)))
            return CHILD_VISIT_RECURSE
        L.clang_visitChildren(L.clang_getTranslationUnitCursor(self.tu), visit, None)
        L.clang_disposeTranslationUnit(self.tu)


def overridden_usrs(cursor):
    L = lib()
    if cursor.kind != CXX_METHOD:
        return []
    items, count = ctypes.POINTER(CXCursor)(), ctypes.c_uint()
    L.clang_getOverriddenCursors(cursor, ctypes.byref(items), ctypes.byref(count))
    usrs = [text_of(L.clang_getCursorUSR(L.clang_getCanonicalCursor(items[i]))) for i in range(count.value)]
    if count.value:
        L.clang_disposeOverriddenCursors(items)
    return usrs


TOKEN = re.compile(r'//[^\n]*|/\*.*?\*/|"(?:\\.|[^"\\\n])*"|\'(?:\\.|[^\'\\\n])*\'|[A-Za-z_]\w*', re.S)


def tokens(text, name):
    """(code offsets, comment offsets) of identifier `name` in C++ text: code tokens outside
    comments and literals; comment offsets only where the comment writes `X::name` (see plan)."""
    code, comments = [], []
    for match in TOKEN.finditer(text):
        token = match.group(0)
        if token == name:
            code.append(match.start())
        elif token.startswith(('//', '/*')):
            for inner in re.finditer(r'\b(\w+)::%s\b' % re.escape(name), token):
                comments.append((inner.group(1), match.start() + inner.start() + len(inner.group(1)) + 2))
    return code, comments


def includes(path, graph):
    """The src/shared headers `path` includes, transitively."""
    seen, stack = set(), [path]
    while stack:
        for name in graph.get(stack.pop(), ()):
            if name not in seen:
                seen.add(name); stack.append(name)
    return seen


def include_graph():
    pattern = re.compile(r'^\s*#\s*include\s+"([^"]+)"', re.M)
    graph = {}
    for root in ('src/recovered', 'src/shared'):
        for p in (REPOSITORY / root).rglob('*'):
            if p.suffix in ('.cpp', '.hpp', '.h'):
                graph[p.relative_to(REPOSITORY).as_posix()] = {'src/' + n for n in
                                                                pattern.findall(p.read_text(errors='replace'))}
    return graph


def plan(klass, old, new, mangled=None, files=None, allow_hiding=False):
    """{'edits': {path: [offset, ...]}, 'manglings': {old full: new full}, 'overload': bool,
    'units': n}: every token of `old` that resolves to klass's member, or SystemExit with the
    tokens the parses cannot explain."""
    if not re.fullmatch(r'[A-Za-z_]\w*', new) or not re.fullmatch(r'[A-Za-z_]\w*', old):
        raise SystemExit('member_rename: %s -> %s are not identifiers' % (old, new))
    graph = include_graph()
    texts = {p: (REPOSITORY / p).read_text(encoding='latin-1') for p in (files or graph)}
    spelled = {p for p, t in texts.items() if re.search(r'\b%s\b' % re.escape(old), t)}
    if not spelled:
        raise SystemExit('member_rename: no source spells %s' % old)
    # units: every .cpp that spells the name or includes a header that does
    headers = {p for p in spelled if p.startswith('src/shared/')}
    units = sorted(p for p in graph if p.endswith('.cpp') and (p in spelled or includes(p, graph) & headers))
    seen, decls, macro, bases, new_owners, new_virtual = {}, {}, set(), set(), set(), set()
    # Non-exact drafts sit under RA2_NON_MATCHING (the exact build never compiles them): a unit
    # that holds one is parsed a second time with it defined, so draft uses resolve by type too.
    passes = [(p, ()) for p in units]
    passes += [(p, ('-DRA2_NON_MATCHING',)) for p in units if 'RA2_NON_MATCHING' in texts.get(p, '') or
               'RA2_NON_MATCHING' in (REPOSITORY / p).read_text(encoding='latin-1')]
    for path, extra in passes:
        unit = Unit(path, old, new, extra)
        bases |= unit.bases
        new_owners |= unit.new_owners
        new_virtual |= unit.new_virtual
        for key, usrs in unit.at.items():
            seen.setdefault(key, set()).update(usrs)
        for usr, decl in unit.decls.items():
            decls.setdefault(usr, decl)
        macro |= unit.macro
    targets = {u for u, d in decls.items() if d['owner'] == klass and d['kind'] in (CXX_METHOD, FIELD_DECL, VAR_DECL)}
    if mangled:
        targets = {u for u in targets if decls[u]['mangled'] == mangled}
    if not targets:
        raise SystemExit('member_rename: %s has no member %s%s' % (klass, old, ' (%s)' % mangled if mangled else ''))
    # the new name may not already be a member of the class, a base or a derived class
    def closure(edges):
        found, stack = {klass}, [klass]
        while stack:
            current = stack.pop()
            for a, b in edges:
                if a == current and b not in found:
                    found.add(b); stack.append(b)
        return found
    ancestors, descendants = closure(bases) - {klass}, closure({(b, d) for d, b in bases})
    # `allow_hiding` (evidenced, e.g. TibSun's static TagClass::Init beside AbstractClass::Init):
    # the new member may hide a base's; never a member of the class itself or a derived class
    # a virtual base member is never hidden: clang (the native build) refuses a static or
    # non-virtual member that hides a virtual one, and a virtual one would override it
    clash = sorted(new_owners & (descendants | (ancestors & new_virtual) if allow_hiding else descendants | ancestors))
    if clash:
        raise SystemExit('member_rename: %s is already a member of %s (in %s::%s\'s hierarchy)' % (
            new, ', '.join(clash), klass, old))
    overriders = sorted({d['owner'] for u, d in decls.items() if set(d['overrides']) & targets})
    if overriders:
        raise SystemExit('member_rename: %s::%s is overridden in %s (the overrides would not follow)' % (
            klass, old, ', '.join(overriders)))
    if any(decls[u]['virtual'] and decls[u]['overrides'] for u in targets):
        raise SystemExit('member_rename: %s::%s overrides a base member (rename the base instead)' % (klass, old))
    edits, unexplained, conflicts = {}, [], []
    for path in sorted(spelled):
        code, comments = tokens(texts[path], old)
        for offset in code:
            usrs = seen.get((path, offset))
            if not usrs:
                unexplained.append((path, offset))
            elif usrs & targets and usrs - targets:
                conflicts.append((path, offset))
            elif usrs & targets:
                if (path, offset) in macro:
                    unexplained.append((path, offset))
                else:
                    edits.setdefault(path, []).append(offset)
        for owner, offset in comments:
            if owner == klass:
                edits.setdefault(path, []).append(offset)
    if unexplained or conflicts:
        def where(items):
            return ', '.join('%s:%d' % (p, texts[p].count('\n', 0, o) + 1) for p, o in items[:8])
        raise SystemExit('member_rename: %s::%s refused: %s%s' % (
            klass, old, ('%d token(s) no parse explains (%s)' % (len(unexplained), where(unexplained)))
            if unexplained else '', ('; %d token(s) two parses explain differently (%s)' % (
                len(conflicts), where(conflicts))) if conflicts else ''))
    manglings = {}
    for usr in targets:
        full = decls[usr]['mangled']
        prefix = '?%s@' % old
        if full.startswith(prefix):
            manglings[full] = '?%s@' % new + full[len(prefix):]
    siblings = {u for u, d in decls.items() if d['owner'] == klass and u not in targets and
                d['kind'] in (CXX_METHOD, VAR_DECL)}
    return dict(edits={p: sorted(set(o)) for p, o in edits.items()}, manglings=manglings,
                overload=bool(siblings), units=len(units), targets=sorted(targets))


def apply_edits(text, offsets, old, new):
    """`text` with `old` replaced at each byte offset (each must spell `old` there)."""
    out, last = [], 0
    for offset in sorted(offsets):
        if text[offset:offset + len(old)] != old:
            raise SystemExit('member_rename: the text moved (%r at %d)' % (text[offset:offset + len(old)], offset))
        out.append(text[last:offset]); out.append(new); last = offset + len(old)
    out.append(text[last:])
    return ''.join(out)


def main():
    import argparse
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('klass'); parser.add_argument('old'); parser.add_argument('new')
    parser.add_argument('--mangled')
    parser.add_argument('--allow-hiding', action='store_true', help='the new name may hide a base class member')
    parser.add_argument('--json', action='store_true')
    args = parser.parse_args()
    result = plan(args.klass, args.old, args.new, args.mangled, allow_hiding=args.allow_hiding)
    if args.json:
        print(json.dumps(result, indent=1))
    else:
        print('%s::%s -> %s: %d units parsed, %d edits in %d files; manglings %s%s' % (
            args.klass, args.old, args.new, result['units'], sum(map(len, result['edits'].values())),
            len(result['edits']), result['manglings'], ' (overloads exist)' if result['overload'] else ''))


if __name__ == '__main__':
    main()
