"""declared_never_defined.py [--write] [--not-carried]: functions the rebuild tree declares that RA2 never
defines (review session, 2026-11-19; lane R45 found In_Radio_Contact, both Sound_Effect overloads and Speak
declared in adopted Tiberian Sun headers with no RA2 body). A declaration with no RA2 body invites new text
written against a routine the game does not have; the census counts the list (budget that may only fall) and
a declaration is removed from its header when its last user is replaced by the image's calls.

A function (scope and name; overloads share one entry) is listed when:
  - it is declared without a body in rebuild/include, or at file or class scope of a rebuild/src file;
  - no file of rebuild/ defines it (in a class body, out of class, or behind a draft guard);
  - no symbol the image is known to hold names it: main's catalog and population tables, main's symbol
    database (manifests/symbols.json, function_map.csv), mischa85 names.csv, rebuild symbols.csv and every
    row of units.json (decorated names reduced to scope and name; a constructor is its class's name);
  - it is not an import (manifests/imports.json) or a CRT/library name, not a pure virtual, not an operator,
    and not compiler-generated (a constructor, destructor or copy assignment the class only declares).
A function main's src/ defines, or whose declaration cites its image address ("RA2: 0x6F45F0"), but the
rebuild does not define yet is not listed: --not-carried prints those (not yet carried, a different list).
Code under #if 0 is not read.

Users: rebuild sources that name it in a call (by name: a member call is not resolved to its class), each
`exact` (active code) or `parked` (behind #ifdef RA2_NON_MATCHING). `instead` is what the image calls in its
place where a lane has found it (b_log_r45.csv and the notes kept in INSTEAD below).

Without --write the list is compared with rebuild/manifest/declared_never_defined.csv (exit 1 when it
differs)."""
import csv, json, re, subprocess, sys
from pathlib import Path
import rebuild as R
import cleanup_census as C

OUT = R.MANIFEST / 'declared_never_defined.csv'
FIELDS = ['function', 'kind', 'header', 'line', 'users', 'instead']

# What the image calls where the adopted text calls a routine RA2 lacks (lane R45, b_log_r45.csv; main's text).
INSTEAD = {
    'RadioClass::In_Radio_Contact': 'RadioClass::HasAnyLink 0x636F10',
    'Sound_Effect': 'VocClass::PlayGlobal 0x713DB0 (no coordinate), VocClass::PlayAt (a coordinate)',
    'Speak': 'Queue_Speech',
}

KEYWORDS = {'if', 'while', 'for', 'switch', 'return', 'sizeof', 'catch', 'defined', 'throw', 'new', 'delete',
            'case', 'do', 'else', 'typedef', 'using', '__declspec', '__pragma', 'decltype', 'static_cast',
            'reinterpret_cast', 'const_cast', 'dynamic_cast', 'operator'}
TYPE_WORDS = {'void', 'int', 'char', 'bool', 'short', 'long', 'float', 'double', 'unsigned', 'signed', 'const',
              'static', 'virtual', 'inline', 'extern', 'friend', '__cdecl', '__stdcall', '__fastcall', '__thiscall',
              'explicit', 'struct', 'class', 'enum', 'volatile', 'WINAPI', 'PASCAL', 'CALLBACK', 'APIENTRY',
              '__inline', '__forceinline', 'register'}
SCOPE_HEAD = re.compile(r'(?:^|[\s;}])(?:template\s*<[^{};]*>\s*)?(class|struct|union|namespace)\s+'
                        r'(?:__declspec\s*\([^)]*\)\s*)?([A-Za-z_]\w*)?[^;{()]*$')
CITED = re.compile(r'RA2:\s*(0x[0-9A-Fa-f]{6,8})')     # the declaration's comment names its image address
EXTERN_C = re.compile(r'extern\s+"C"\s*$')


def blank_preprocessor(code):
    """CODE with preprocessor lines (and their continuations) blanked, keeping lines."""
    out, cont = [], False
    for line in code.split('\n'):
        directive = cont or line.lstrip().startswith('#')
        cont = directive and line.rstrip().endswith('\\')
        out.append('' if directive else line)
    return '\n'.join(out)


def guard_lines(text):
    """(parked, dead): sets of line numbers inside #ifdef RA2_NON_MATCHING and inside #if 0."""
    parked, dead, stack = set(), set(), []
    for n, line in enumerate(text.split('\n'), 1):
        s = line.strip()
        if re.match(r'#\s*if', s):
            stack.append('parked' if re.match(r'#\s*ifdef\s+RA2_NON_MATCHING\b', s)
                         else 'dead' if re.match(r'#\s*if\s*\(?\s*0\s*\)?\s*$', s) else '')
        elif re.match(r'#\s*el', s) and stack:
            stack[-1] = ''          # the #else of a draft guard is the original-byte side; of #if 0, live code
        elif re.match(r'#\s*endif', s) and stack:
            stack.pop()
        if 'parked' in stack:
            parked.add(n)
        if 'dead' in stack:
            dead.add(n)
    return parked, dead


def split_call(head):
    """(name, qualifier, before, after) of the first top-level call in HEAD, or None."""
    depth_angle = 0
    for i, ch in enumerate(head):
        if ch == '<':
            depth_angle += 1
        elif ch == '>':
            depth_angle = max(0, depth_angle - 1)
        elif ch == '(' and depth_angle == 0:
            m = re.search(r'((?:[A-Za-z_]\w*(?:<[^<>()]*>)?\s*::\s*)*)(~?\s*[A-Za-z_]\w*)\s*$', head[:i])
            if not m:
                return None
            depth, j = 0, i
            while j < len(head):
                depth += {'(': 1, ')': -1}.get(head[j], 0)
                if depth == 0:
                    break
                j += 1
            qual = re.sub(r'<[^<>()]*>|\s', '', m.group(1)).rstrip(':')
            return re.sub(r'\s', '', m.group(2)), qual, head[:m.start()], head[j + 1:]
    return None


def scan(code, lines_of):
    """[(kind, scope, name, line, head, end line)] over CODE (comments, literals, directives blanked):
    function declarations and definitions at namespace and class scope. kind is 'decl' or 'def'; end line
    is the last line of a definition's body or of a declaration."""
    found, stack, start = [], [], 0      # stack entries: (kind, name) kind in class/namespace/body/other
    i = 0
    n = len(code)
    while i < n:
        ch = code[i]
        if ch in ';{}':
            head = code[start:i].strip()
            in_body = any(k in ('body', 'other') for k, _ in stack)
            scope = '::'.join(name for k, name in stack if k in ('class', 'namespace') and name)
            at_class = bool(stack) and stack[-1][0] == 'class'
            if ch == '{':
                m = SCOPE_HEAD.search(code[start:i]) if not in_body else None
                if m and '(' not in head and '=' not in head:
                    stack.append(('class' if m.group(1) != 'namespace' else 'namespace', m.group(2) or ''))
                elif not in_body and EXTERN_C.search(head):
                    stack.append(('namespace', ''))
                else:
                    call = split_call(head) if not in_body and head else None
                    if call and is_function_head(call, at_class):
                        name, qual, before, after = call
                        found.append(['def', '::'.join(p for p in (scope, qual) if p), name,
                                      lines_of(start + len(code[start:i]) - len(code[start:i].lstrip())), head, 0])
                        stack.append(('body', len(found) - 1))
                    else:
                        stack.append(('other', ''))
            elif ch == ';':
                if not in_body and head and '(' in head:
                    call = split_call(head)
                    if call and is_function_head(call, at_class) and not re.search(r'\b(typedef|using)\b', head):
                        name, qual, before, after = call
                        if '=' not in before:
                            found.append(['decl', '::'.join(p for p in (scope, qual) if p), name,
                                          lines_of(start + len(code[start:i]) - len(code[start:i].lstrip())), head,
                                          lines_of(i)])
            else:
                if stack:
                    kind, item = stack.pop()
                    if kind == 'body':
                        found[item][5] = lines_of(i)
            start = i + 1
        i += 1
    return [tuple(f) for f in found]


def is_function_head(call, at_class):
    name, qual, before, after = call
    if name in KEYWORDS or name.lstrip('~') in KEYWORDS or re.search(r'\boperator\b', before + ' ' + name):
        return False
    if re.search(r'\(\s*\*|\(\s*__\w+\s*\*', before) or before.rstrip().endswith(('(', ',', '=', '.', '->', '&&', '||')):
        return False
    rest = after.strip()
    if rest and not re.match(r'^(const\b|throw\s*\(|=\s*0\s*$|:|volatile\b|PURE\b)', rest):
        return False
    words = re.findall(r'[A-Za-z_]\w*', before)
    if not words and not qual and not at_class and not name.startswith('~'):
        return False        # a macro invocation at namespace scope
    return True


def line_index(code):
    starts = [0] + [m.end() for m in re.finditer(r'\n', code)]
    import bisect
    return lambda offset: bisect.bisect_right(starts, offset)


def analyse(path_text):
    """[(kind, scope, name, line, head, end line)] of one file's text."""
    _, dead = guard_lines(path_text)
    code = blank_preprocessor(C.code_only(path_text))
    if dead:
        code = '\n'.join('' if n in dead else line for n, line in enumerate(code.split('\n'), 1))
    return scan(code, line_index(code))


def demangled_key(symbol):
    """(scope, name) of a decorated or C symbol, or None."""
    if symbol.startswith('?'):
        if symbol.startswith('??'):
            m = re.match(r'\?\?[01]\??\$?([A-Za-z_]\w*)@', symbol)
            return (m.group(1), m.group(1)) if m else None
        m = re.match(r'\?([A-Za-z_]\w*)@(?:\?\$)?([A-Za-z_]\w*)?@', symbol)
        if not m:
            return None
        return (m.group(2) or '', m.group(1))
    m = re.match(r'^[_@]?([A-Za-z_]\w*?)(?:@\d+)?$', symbol)
    return ('', m.group(1)) if m else None


def image_keys():
    """{(scope, name)} of every symbol the image is known to hold."""
    names = set()
    catalog = R.main_catalog()
    for table in catalog['population_references'].values():
        names.update(table)
    names.update(r['symbol'] for r in catalog['regions'])
    names.update(R.mischa_names())
    names.update(R.extra_symbols())
    for r in csv.DictReader(open(R.ROOT / 'manifests/function_map.csv')):
        names.add(r['symbol'])
    for s in json.load(open(R.ROOT / 'manifests/symbols.json'))['symbols']:
        names.add(s.get('mangled_prefix') or '')
        q = s.get('name') or ''
        if '::' in q:
            scope, _, name = q.rpartition('::')
            names.add('?%s@%s@@' % (name, scope.split('::')[-1]))
        elif q:
            names.add('_' + q)
    for info in R.load_json(R.UNITS, {}).values():
        for r in info['rows']:
            names.update(n for n in (r.get('symbol'), r.get('main_symbol')) if n)
        names.update(info.get('references') or {})
    keys = {k for k in map(demangled_key, names) if k}
    # main's address-suffixed names (Draw_Chat_Back_005E1234) name the same function
    keys |= {(scope, re.sub(r'_[0-9A-Fa-f]{8}$', '', name)) for scope, name in keys}
    return keys


def library_names():
    names = {i['name'] for i in json.load(open(R.ROOT / 'manifests/imports.json'))['imports']}
    names |= {s['name'] for s in json.load(open(R.ROOT / 'manifests/symbols.json'))['symbols']
              if s.get('kind') in ('crt', 'import', 'library')}
    return names


def tree_files(prefixes):
    out = subprocess.run(['git', '-C', str(R.ROOT), 'ls-files'] + list(prefixes), capture_output=True,
                         text=True).stdout.split()
    return [p for p in out if p.endswith(('.h', '.hpp', '.cpp', '.c', '.inl'))]


def short(scope):
    return scope.split('::')[-1] if scope else ''


def defined_keys(files, read):
    keys = set()
    for p in files:
        for kind, scope, name, line, head, end in analyse(read(p) or ''):
            if kind == 'def':
                keys.add((short(scope), name))
    return keys


def census():
    read = C.reader(R.ROOT)
    files = tree_files(['rebuild/include', 'rebuild/src'])
    scanned = {p: analyse(read(p) or '') for p in files}
    decls, defs, scopes = {}, set(), {}
    for p, found in scanned.items():
        for kind, scope, name, line, head, end in found:
            key = (short(scope), name)
            scopes.setdefault(name, set()).add(key[0])
            if kind == 'def':
                defs.add(key)
            elif not re.search(r'=\s*0\s*$', head) and not re.search(r'\bPURE\s*$', head):
                decls.setdefault(key, []).append((p, line, scope, head))
    image = image_keys()
    libs = library_names()
    main_defs = None
    listed, not_carried = [], []
    for key, places in sorted(decls.items()):
        scope, name = key
        if key in defs or key in image:
            continue
        if name.startswith('~') or name == scope.split('<')[0]:
            continue        # constructor or destructor the class only declares
        if not scope and name in libs:
            continue
        if any(re.search(r'__declspec\s*\(\s*dllimport|\bWINAPI\b|\bAPIENTRY\b|\bSTDAPI', h) for _, _, _, h in places):
            continue
        if main_defs is None:
            main_defs = defined_keys(tree_files(['src']), read)
        cited = [m.group(1) for p, l, _, _ in places for m in [CITED.search((read(p) or '').split('\n')[l - 1])] if m]
        if re.search(r'_[0-9A-Fa-f]{8}$', name):
            cited.append(name)          # a name holding its image address (FUN_004085A0)
        if key in main_defs or cited:
            not_carried.append((key, places))
            continue
        listed.append((key, places))
    return listed, not_carried, files, read, scanned, scopes


def row_states():
    """{source: {(scope, name): set of row states}} from each unit's last check of its current text."""
    states = {}
    for unit, info in R.load_json(R.UNITS, {}).items():
        last = R.last_check(unit, info)
        sensitive = info.get('count_sensitive') or {}
        status = {r['entry']: r['status'] for r in last['results']} if last else {}
        for r in info['rows']:
            key = demangled_key(r['symbol'])
            if not key:
                continue
            state = 'count-sensitive' if r['entry'] in sensitive else \
                'exact' if status.get(r['entry']) == 'exact' else 'parked' if last else 'unchecked'
            states.setdefault(info['source'], {}).setdefault(key, set()).add(state)
    return states


def users_of(name, places, files, read, scanned, states):
    """'file (states)' for each rebuild file that calls NAME outside its declarations: each call is the state
    of the row that holds it (exact, parked, count-sensitive; parked when behind a draft guard; header for
    an inline body in a header)."""
    decl_lines = {(p, l) for p, l, _, _ in places}
    pattern = re.compile(r'(?<![\w~])%s\s*\(' % re.escape(name))
    users = []
    for p in files:
        text = read(p) or ''
        if name not in text:
            continue
        code = C.code_only(text)
        parked, dead = guard_lines(text)
        index = line_index(code)
        bodies = [(line, end, (short(scope), fn)) for kind, scope, fn, line, head, end in scanned[p] if kind == 'def']
        found = set()
        for m in pattern.finditer(code):
            line = index(m.start())
            if (p, line) in decl_lines or line in dead:
                continue
            if line in parked:
                found.add('parked')
                continue
            if p.startswith('rebuild/include/'):
                found.add('header')
                continue
            holder = [k for a, b, k in bodies if a <= line <= b]
            row = states.get(p, {}).get(holder[-1], set()) if holder else set()
            found |= row or {'not a row'}
        if found:
            users.append('%s (%s)' % (p.replace('rebuild/', ''), '/'.join(sorted(found))))
    return users


def rows():
    listed, not_carried, files, read, scanned, scopes = census()
    states = row_states()
    out = []
    for (scope, name), places in listed:
        qualified = '%s::%s' % (scope, name) if scope else name
        p, line, _, _ = places[0]
        shared = len(scopes[name]) > 1
        users = 'name shared by %d scopes, calls not resolved' % len(scopes[name]) if shared else \
            '; '.join(users_of(name, places, files, read, scanned, states))
        out.append(dict(function=qualified, kind='member' if scope else 'free', header=p.replace('rebuild/', ''),
                        line=line, users=users, instead=INSTEAD.get(qualified, '')))
    return out, not_carried


def count():
    """Rows in the committed list (the census line reads this)."""
    return sum(1 for _ in csv.DictReader(open(OUT))) if OUT.exists() else 0


def main():
    found, not_carried = rows()
    if '--not-carried' in sys.argv:
        for (scope, name), places in not_carried:
            print('not yet carried: %s (%s:%d)' % ('%s::%s' % (scope, name) if scope else name,
                                                  places[0][0], places[0][1]))
    if '--write' in sys.argv:
        with open(OUT, 'w', newline='') as f:
            w = csv.DictWriter(f, fieldnames=FIELDS)
            w.writeheader(); w.writerows(found)
        print('%d declared, never defined; %d defined in main only (not yet carried) -> %s' % (
            len(found), len(not_carried), OUT.relative_to(R.ROOT)))
        return 0
    old = list(csv.DictReader(open(OUT))) if OUT.exists() else []
    same = [dict((k, str(v)) for k, v in r.items()) for r in found] == old
    print('%d declared, never defined (list %s); %d not yet carried' % (
        len(found), 'current' if same else 'differs: run --write', len(not_carried)))
    return 0 if same else 1


if __name__ == '__main__':
    sys.exit(main())
