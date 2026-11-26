"""Structure census of the rebuild tree: tools/original_target/cleanup_census.py --structure pointed at
rebuild/ (tracked files under src and include, the rebuild catalog's rows, the units of
manifests/units.json). Line (c) counts src files that are not a unit's own file.
Exceptions are listed after the block: the RA2_NON_MATCHING draft guards and every address-suffixed
identifier with the file it is in (lane R).

Gate (lane R2, user 2026-11-17): `rebuild.py catalog` refuses to write the catalog when a structure
measure exceeds its budget in manifests/structure_budget.json: scope-qualified duplicate class
definitions, files outside a unit, free-vs-member rows and tool-data comment lines have budget zero;
address-suffixed names in headers may only fall. Per unit (rebuild_catalog.py): a unit is left out of the
catalog when its source has more stray externs or address-suffixed names than its budget entry (units
landed before the gate keep their counts as budgets, to be lowered; a new unit's budget is zero).
Duplicates are counted by qualified name: a class nested in another class or local to a function is
named by its enclosing scopes (OwnerDraw::CellData and MapGen::CellData are two classes); the bare-name
count of the shared census is printed beside it."""
import json, os, re, subprocess
from collections import Counter
from pathlib import Path
import rebuild as R
import catalog_io
import cleanup_census as C


DEFINITION_AT = re.compile(r'(?<![\w:])(?:typedef\s+)?(class|struct|union)\s+(?:__declspec\s*\([^)]*\)\s*)?([A-Za-z_]\w*)[^;{()]*\{')
SCOPE_NAME = re.compile(r'(?:(?:class|struct|union|namespace)\s+(?:__declspec\s*\([^)]*\)\s*)?([A-Za-z_]\w*)[^;{()]*|([A-Za-z_~][\w:~]*)\s*\([^;{}]*\)[^;{}()]*)$')


def qualified_definitions(code):
    """[qualified name] of the class/struct definitions in CODE (comments and literals blanked): each
    named by its enclosing class, namespace or function scopes."""
    found, stack, i = [], [], 0
    starts = {m.end() - 1: m for m in DEFINITION_AT.finditer(code)
              if not code[:m.start()].rstrip().endswith(('<', ','))}    # template parameters
    for i, ch in enumerate(code):
        if ch == '{':
            head = code[max(0, i - 300):i]
            head = head[max(head.rfind(';'), head.rfind('}'), head.rfind('{')) + 1:].strip()
            m = SCOPE_NAME.search(head)
            stack.append((m.group(1) or m.group(2) + '()') if m else '')
            if i in starts and not C.ENUM_CLASS.search(code[max(0, starts[i].start() - 8):starts[i].start()]):
                found.append('::'.join([s for s in stack[:-1] if s] + [starts[i].group(2)]))
        elif ch == '}' and stack:
            stack.pop()
    return found


def structure_measures(tree, read, result):
    """The gated measures: scope-qualified duplicates (with their files) and the shared census's
    counts of the other lines."""
    defs = {}
    for p in tree:
        if not p.endswith(C.SOURCE_SUFFIXES):
            continue
        for name in qualified_definitions(C.code_only(read(p) or '')):
            defs.setdefault(name, []).append(p)
    dups = {k: v for k, v in defs.items() if len(v) > 1}
    return dict(duplicates=len(dups), duplicate_list=dups,
                outside=result['files_outside']['outside'],
                free_vs_member=result['member_counterparts']['rows'] if 'rows' in result['member_counterparts'] else
                result['member_counterparts']['free'] + result['member_counterparts']['stand_in'],
                tool_data=result['ref_lines']['lines'],
                externs=result['externs']['lines'],
                address_suffixed=result['address_suffixed']['identifiers'],
                address_suffixed_headers=len({n for p in tree if p.startswith('include/')
                                              for n in C.address_suffixed(read(p) or '')}))


CHATTER = re.compile(r'\b[Ll]ane R\d*\b|\bcodex\d\b|\bWindows workers?\b|rebuild/(?:manifest|tools)|provenance\.csv|'
                     r'\bmanifest/|\btools/\w+\.py|rebuild trial|\(rebuild\)|\bold tree\b')


def placeholder_measures(tree, read):
    """Process chatter (budget zero), placeholder names and inherited constructs (budgets that only
    fall; review session, 2026-11-18)."""
    unknown, vt, field, fun = set(), set(), set(), set()
    chatter = guards = geometry = pragmas = extern_c = 0
    for p in tree:
        text = read(p) or ''
        chatter += sum(1 for line in text.splitlines() if CHATTER.search(line))
        code = C.code_only(text)
        unknown |= set(re.findall(r'\b[Uu]nknown\w*', code))
        vt |= set(re.findall(r'\bvt_entry_[0-9A-Fa-f]+\b', code))
        guards += len(re.findall(r'#ifdef RA2_NON_MATCHING', text))
        if p.startswith('include/'):
            field |= set(re.findall(r'\bfield_[0-9A-Fa-f]+\b', code))
            fun |= set(re.findall(r'\bFUN_[0-9A-Fa-f]+\b', code))
        if p.startswith('src/'):
            geometry += len(re.findall(r'^\s*#\s*define\s+GEOMETRY_H\b', text, re.M))
            pragmas += len(re.findall(r'^\s*#\s*pragma\s+auto_inline\s*\(\s*off', text, re.M))
            extern_c += len(re.findall(r'^[ \t]*extern\s+"C"', text, re.M))
    return dict(process_chatter=chatter, unknown_names=len(unknown), vt_entry_names=len(vt),
                field_names_headers=len(field), fun_names_headers=len(fun), draft_guards=guards,
                inherited_geometry_define=geometry, inherited_auto_inline_off=pragmas, extern_c_in_sources=extern_c)


# Gated keys (may only fall). draft_guards is reported only: the guards are the parked tail, and a new
# guarded draft is a legitimate parked attempt (owner ruling, 2026-11-18).
PLACEHOLDER_KEYS = ('declared_never_defined', 'helper_copies', 'inliner_statements', 'process_chatter', 'unknown_names', 'vt_entry_names', 'field_names_headers', 'fun_names_headers',
                    'inherited_geometry_define', 'inherited_auto_inline_off', 'extern_c_in_sources')


SHARED_INLINE = re.compile(r'^[ \t]*(?:static\s+)?inline\s+(?:static\s+)?[\w<>,\s\*&]+?\b([A-Za-z_]\w*_\w+)\s*\(', re.M)


def helper_copies(tree, read):
    """[(file, name, shared helper)]: unit-local copies of a shared inline helper (review session, 2026-11-19;
    budget that may not grow): an inline function defined in a unit source whose name holds, as whole
    underscore-separated words, the name of an inline free function a header defines (Adjacent_Cell,
    Move_Coord): Aircraft_Adjacent_Cell, Adjacent_Cell_L, Tunnel_Move_Coord, _Move_Coord. Names of one word (Distance)
    are not matched."""
    shared = set()
    for p in tree:
        if p.startswith('include/'):
            shared |= {n for n in SHARED_INLINE.findall(C.code_only(read(p) or '')) if '::' not in n}
    found = []
    evidenced = {(r['unit'], r['name']) for r in twin_helpers(read)}
    for p in tree:
        if not (p.startswith('src/') and p.endswith(('.cpp', '.c'))):
            continue
        for name in sorted(set(SHARED_INLINE.findall(C.code_only(read(p) or '')))):
            if (p.split('/')[-1].rsplit('.', 1)[0], name) in evidenced:
                continue
            for helper in sorted(shared):
                if name != helper and re.search(r'(?:^|_)%s(?:$|_)' % re.escape(helper), name):
                    found.append((p, name, helper))
                    break
    return found


TWIN_HELPERS = 'manifests/twin_helpers.csv'
TWIN_SRC = Path(__file__).resolve().parents[2] / 'private/reference/ra2-decomp/src'
# A helper adopted from another reference's text names it in twin_path with a prefix (lane O93, 2026-11-25:
# building's Silo_Storage_Frame and Refinery_Storage_Stage are mischa85's).
TWIN_ROOTS = {'mischa85:': TWIN_SRC.parents[1] / 'mischa85-RedAlert2/src',
              'robin:': TWIN_SRC.parents[1] / 'robin-ra2-1001-decomp/src/m85/src'}


def twin_file(twin_path):
    """The reference unit file a twin_helpers.csv twin_path names (ra2-decomp's src/ unless prefixed)."""
    for prefix, root in TWIN_ROOTS.items():
        if twin_path.startswith(prefix):
            return root / twin_path[len(prefix):]
    return TWIN_SRC / twin_path


def twin_helpers(read):
    """[{unit, name, twin_path, note}]: unit-local helpers evidenced by the twin (review session, 2026-11-20): listed
    in manifests/twin_helpers.csv, kept only while the unit source defines the name `static` (or `static inline`; the twin's spelling, owner 2026-11-22) and the
    ra2-decomp twin unit (twin_path, under private/reference/ra2-decomp; `mischa85:` or `robin:` before the path names
    those references' unit) defines the same name file-locally; a row with no twin_path records a flagged [helper]
    no reference has (listed, never evidenced);
    a listed helper either side lacks is not evidenced and counts as a copy again. Reported on its own census
    line, apart from the helper_copies budget (which may only fall)."""
    import csv, io
    rows = list(csv.DictReader(io.StringIO(read(TWIN_HELPERS) or '')))
    kept = []
    for r in rows:
        pat = re.compile(r'^[ \t]*static\s+(?:inline\s+)?[\w<>,\s\*&]+?\b%s\s*\(' % re.escape(r['name']), re.M)
        ours = read('src/%s.cpp' % r['unit']) or ''
        twin = twin_file(r['twin_path']) if r['twin_path'] else None
        if twin is not None and pat.search(C.code_only(ours)) and twin.exists() and twin_file_local(twin.read_text(errors='replace'), r['name']):
            kept.append(r)
    return kept


def twin_file_local(text, name):
    """True when the twin unit's text defines `name` file-locally: `static` (or `static inline`) at its definition,
    or `inline` at file scope (the unit's own text, as ownrdraw's Is_Flag_Control_* and netshare's Format_Slot), or
    declared `_STATIC` (ra2-decomp's file-local marker, vqalib) with a plain definition (lane C1)."""
    n = re.escape(name)
    if re.search(r'^[ \t]*static\s+(?:inline\s+)?[\w<>,\s\*&]+?\b%s\s*\(' % n, text, re.M):
        return True
    if re.search(r'^inline\s+[\w<>,\s\*&]+?\b%s\s*\(' % n, text, re.M):
        return True
    return bool(re.search(r'^[ \t]*_STATIC\s+[\w<>,\s\*&]+?\b%s\s*\(' % n, text, re.M)
                and re.search(r'^[\w<>,\s\*&]+?\b%s\s*\([^;]*\)\s*\{?\s*$' % n, text, re.M))


LARGE = 1024    # a parked function of 1 KB or more (the checkpoint measure: "parked large functions pass 500 KB")


def parked_no_row():
    """{entry: (body bytes, symbol)} of parked drafts with no row (series break, lane O96, 2026-11-26; owner after
    the review of lane O95's pool A dossier): the functions src-final carried as not exact (manifests/function_map.csv
    flags `non_exact`) and the image-derived bounds (manifests/image_bounds.csv, status bound) whose entry is neither a
    row of units.json nor a row of the catalog. Their guarded drafts sit in the sources, but before this break the
    parked line counted only units.json rows. Bytes: the image bound's body where there is one (it may run past
    src-final's size), else src-final's size."""
    import csv
    have = {r['entry'] for info in R.load_json(R.UNITS, {}).values() for r in info['rows']}
    have |= {'%08x' % r['entry'] for r in catalog_io.load(R.MANIFEST / 'catalog', references=False)['regions']}
    found = {}
    path = R.MANIFEST / 'function_map.csv'
    for r in (csv.DictReader(open(path)) if path.exists() else []):
        if r['flags'].startswith('non_exact'):
            found['%08x' % int(r['address'], 16)] = (int(r['size']), r['symbol'])
    path = R.MANIFEST / 'image_bounds.csv'
    for r in (csv.DictReader(open(path)) if path.exists() else []):
        if r['status'] == 'bound':
            body, symbol = int(r['body_bytes']), r['catalog_symbol']
            if r['entry'] in found:
                body, symbol = max(body, found[r['entry']][0]), found[r['entry']][1]
            found[r['entry']] = (body, symbol)
    return {e: v for e, v in found.items() if e not in have}


def parked_tail(no_row=True):
    """(rows, body bytes, rows of LARGE bytes or more, their bytes) not exact: the parked tail behind the
    guards. One definition for the census and progress.py (owner, 2026-11-18): every row of every unit in
    units.json that its check of the unit's current text does not find exact, and (series break, lane O96,
    2026-11-26) every parked draft with no row (parked_no_row); NO_ROW=False gives the definition before the
    break. A unit whose last check is missing or of older text is checked again first; a unit that does not
    compile counts all its rows."""
    import hashlib, io, contextlib
    rows = total = large = large_total = 0
    if no_row:
        for body, _symbol in parked_no_row().values():
            rows += 1; total += body
            if body >= LARGE:
                large += 1; large_total += body
    for unit, info in R.load_json(R.UNITS, {}).items():
        source = R.ROOT / info['source']
        last = R.last_check(unit, info)
        if last is None and source.exists():
            with contextlib.redirect_stdout(io.StringIO()):
                R.check(unit)
            last = R.last_check(unit, info)
        status = {r['entry']: r['status'] for r in last['results']} if last else {}
        sensitive = info.get('count_sensitive') or {}
        for r in info['rows']:
            if status.get(r['entry']) != 'exact' and r['entry'] not in sensitive:
                rows += 1; total += r['body']
                if r['body'] >= LARGE:
                    large += 1; large_total += r['body']
    return rows, total, large, large_total


def function_level(regions):
    """Two measures side by side (review session, 2026-11-20; no rule or credit change):
    whole-unit: rows the catalog holds (catalogued units, whole-file exact).
    function-level: each function checked on its own, as ra2-decomp reports: the catalog's rows, plus every
    row of a unit in units.json that the unit's current check finds exact (held units included), every
    count-sensitive row (exact in its probed window) and every row tailmap records as exact_when_open
    (exact with its guard open; manifests/tail_buckets.csv). Rows count once; bytes are body plus padding, and
    the body bytes alone are returned beside them (lane O41, 2026-11-22: C/C++ is counted in body bytes with
    padding on its own line, as on main).
    Returns dict(whole=(rows, bytes), function=(rows, bytes), parts={part: (rows, bytes)}, whole_body=body
    bytes, function_body=body bytes, whole_entries={entry: bytes}, function_entries={entry: bytes})."""
    import csv
    whole = {'%08x' % r['entry']: r['full_extent_bytes'] for r in regions}
    body = {'%08x' % r['entry']: r['body_bytes'] for r in regions}
    units = R.load_json(R.UNITS, {})
    extent = {}
    checked, sensitive = set(), set()
    for unit, info in units.items():
        last = R.last_check(unit, info)
        status = {r['entry']: r['status'] for r in last['results']} if last else {}
        for r in info['rows']:
            extent[r['entry']] = r['body'] + (r.get('padding') or 0)
            body.setdefault(r['entry'], r['body'])
            if status.get(r['entry']) == 'exact':
                checked.add(r['entry'])
        sensitive |= set(info.get('count_sensitive') or {})
    path = R.MANIFEST / 'tail_buckets.csv'
    when_open = {r['entry'] for r in csv.DictReader(open(path)) if r['bucket'] == 'exact_when_open'} \
        if path.exists() else set()
    parts, seen = {}, set(whole)
    for part, entries in (('exact in the unit check, not catalogued', checked), ('count-sensitive', sensitive),
                          ('exact with the guard open', when_open)):
        new = (entries & set(extent)) - seen
        parts[part] = (len(new), sum(extent[e] for e in new))
        seen |= new
    function_bytes = sum(whole.values()) + sum(b for _, b in parts.values())
    entries = dict(whole)
    entries.update((e, extent[e]) for e in seen - set(whole))
    return dict(whole=(len(whole), sum(whole.values())), function=(len(seen), function_bytes), parts=parts,
                whole_body=sum(body[e] for e in whole), function_body=sum(body[e] for e in seen),
                whole_entries=whole, function_entries=entries)


def count_sensitive():
    """(rows, body bytes) parked as "exact form, count-sensitive" (review ruling 2026-11-18, lane R20): rows
    whose exact text flips with the declaration count before them when an evidenced header changes. Each is
    listed in its unit's `count_sensitive` (entry -> windows, header, note) in units.json, guarded in the
    source, and kept apart from the parked tail; `countsens.py` tries them again after a header change."""
    rows = total = 0
    for info in R.load_json(R.UNITS, {}).values():
        body = {r['entry']: r['body'] for r in info['rows']}
        for entry in info.get('count_sensitive') or {}:
            rows += 1; total += body.get(entry, 0)
    return rows, total


def header_form_parked():
    """(rows, body bytes) of exact rows parked for evidenced header forms (review session, 2026-11-21): a row
    whose exact text is lost when evidenced header forms land (here AbstractClass::Is_Techno and Wstring::c_str
    inline, ra2-decomp's headers), parked as an ordinary draft with its last exact text kept in a dossier. Each
    is listed in its unit's `header_form_parked` (entry -> {header, score, dossier, note}) in units.json; the
    rows stay in the parked tail."""
    rows = total = 0
    for info in R.load_json(R.UNITS, {}).values():
        body = {r['entry']: r['body'] for r in info['rows']}
        for entry in info.get('header_form_parked') or {}:
            rows += 1; total += body.get(entry, 0)
    return rows, total


def judgment_calls(regions):
    """(rows, body bytes) of the accepted rows flagged as judgment calls in provenance.csv (AGENTS.md
    Decision policy: real computed or looked-up values kept and flagged, each with its evidence)."""
    import csv
    accepted = {'%08x' % r['entry'] for r in regions}
    path = R.MANIFEST / 'provenance.csv'
    flagged = [r for r in csv.DictReader(open(path)) if (r.get('judgment') or '').strip() and r['entry'] in accepted] \
        if path.exists() else []
    return len(flagged), sum(int(r['body_bytes']) for r in flagged)


def inliner_statements():
    """[(unit, entry, symbol)]: statements kept only to steer the inliner (review session, 2026-11-19; budget that
    may not grow): accepted or not, every provenance row whose judgment note starts with "inliner:" (an explicit
    trailing `return;` or a like statement that does no work and only stops or forces an expansion, allowed as a
    last resort after definition order, the caller's inline budget and the reference texts were checked; the note
    records which)."""
    import csv
    path = R.MANIFEST / 'provenance.csv'
    return [(r['unit'], r['entry'], r['symbol']) for r in csv.DictReader(open(path))
            if judgment_text(r.get('judgment')).lower().startswith('inliner:')] if path.exists() else []


def provisional_names():
    """[(unit, name)] from manifests/provisional_names.csv (review session, 2026-11-19): names the project coined
    (descriptive, not from TibSun, ra2-decomp, mischa85 or RTTI), counted in the structure line until an
    evidenced name replaces them. Reported, not gated."""
    import csv
    path = R.MANIFEST / 'provisional_names.csv'
    return [(r['unit'], r['name']) for r in csv.DictReader(open(path))] if path.exists() else []


HEX_ENTRY = re.compile(r'\b(?:0x)?(?=[0-9A-Fa-f]*[0-9])([0-9A-Fa-f]{6,8})\b')

# Judgment kinds (review session request, 2026-11-23, lane O63): every judgment note in provenance.csv and every
# header_judgments.csv form starts with its kind in brackets, e.g. "[iv-a] twin text ...", so strong evidence can be
# told from weak:
#   i     a reference form chosen by caller evidence (several reference forms of one inline; the one under which
#         every caller is exact; DECISIONS "Several reference forms of one shared inline")
#   ii    a sibling-image helper form (helper read off Yuri's Revenge's out-of-line copy; DECISIONS row of 2026-11-23)
#   iii   an author idiom unreferenced at the line (DECISIONS "An author idiom at a line where no reference has it")
#   iv-a  twin text or a reference-borrowed value that a reference has at this site or a sibling site
#   iv-b  a looked-up named local no reference has (main's own form or the lane's)
#   v     image-derived or frozen bounds only (the text itself is not a judgment)
#   vi    header-form dependents: rows with no note of their own that rest on a header_judgments.csv form
#         (a provenance note tagged [vi] points at such a form too)
#   joined statements from two references of the lineage joined at one site; strong only when the note says why
#         each side's form cannot stand whole (review spot-check, owner 2026-11-23; non-soft)
#   vii   anything else (coined helpers, composites, choices the image does not fix); held items not in the tree
#         belong in variation_candidates.csv, not header_judgments.csv (their dependents are not judgment rows)
# Flagged variation forms (AGENTS.md exception, user 2026-11-24; lane O78), each a choice no reference shows at the line:
#   helper             an inferred inline helper where the image's expansion depth shows a helper existed
#   spelling           an alternative spelling of a statement or condition
#   compiler-local     a named local chosen for the compiler (it holds a value the function uses)
#   readability-alias  a copy with a more meaningful name or type than what it copies, one copy per value, used
#                      from then on in place of the original, no copy of a copy
# Reference-borne forms (owner ruling, 2026-11-25, lane O93, after the independent review of 40 rows): adopted
# reference text (ra2-decomp, mischa85, robin) that carries a form we would refuse or flag had we coined it
# (once-used copies, dead tests or discarded calls the image performs, pass-through locals or wrappers,
# hand-expanded bodies, duplicated calls). The text stays (it matches); the row is flagged:
#   reference-borne    a refused-if-coined form taken from adopted reference text; the note names the form
# SOFT_KINDS rest on a choice and are left out of "exact with no soft judgment"; a header-form dependent is soft when
# any form it rests on is of a soft kind. An untagged note counts as vii (soft) and is listed as unclassified.
JUDGMENT_KINDS = ('i', 'ii', 'iii', 'iv-a', 'iv-b', 'v', 'vi', 'joined', 'vii',
                  'helper', 'spelling', 'compiler-local', 'readability-alias', 'reference-borne')
SOFT_KINDS = frozenset(('iii', 'iv-b', 'vii', 'helper', 'spelling', 'compiler-local', 'readability-alias',
                        'reference-borne'))
KIND_PREFIX = re.compile(r'\s*\[(%s)\]\s*' % '|'.join(re.escape(k) for k in sorted(JUDGMENT_KINDS, key=len, reverse=True)))


def judgment_kind(note):
    """The bracketed kind a judgment note starts with, or None (untagged or empty)."""
    m = KIND_PREFIX.match(note or '')
    return m.group(1) if m else None


def judgment_text(note):
    """The judgment note without its kind prefix."""
    note = note or ''
    m = KIND_PREFIX.match(note)
    return note[m.end():] if m else note.strip()


def classify_judgments(provenance_rows, header_rows, known):
    """({entry: (kind, soft)}, [unclassified (where, key)]) from provenance.csv rows and header_judgments.csv rows
    (dicts): a provenance row with a judgment note takes its own kind (untagged: vii); an entry listed in a flagged
    header form's dependents, known (a units.json row) and without a provenance note, takes kind vi, soft when any
    form it rests on is of a soft kind (or untagged)."""
    kinds, unclassified = {}, []
    for r in provenance_rows:
        note = (r.get('judgment') or '').strip()
        if not note:
            continue
        kind = judgment_kind(note)
        if kind is None:
            unclassified.append(('provenance', r['entry'])); kind = 'vii'
        kinds[r['entry']] = (kind, kind in SOFT_KINDS)
    forms = {}
    for r in header_rows:
        note = (r.get('judgment') or '').strip()
        if not note:
            continue
        kind = judgment_kind(note)
        if kind is None:
            unclassified.append(('header', r.get('member', ''))); kind = 'vii'
        for m in HEX_ENTRY.finditer(r.get('dependents') or ''):
            forms.setdefault(m.group(1).lower().zfill(8), set()).add(kind)
    for entry, form_kinds in forms.items():
        if entry in known and entry not in kinds:
            kinds[entry] = ('vi', bool(form_kinds & SOFT_KINDS))
    return kinds, unclassified


def judgment_kinds(units=None):
    """classify_judgments on the tree's manifests."""
    import csv
    units = R.load_json(R.UNITS, {}) if units is None else units
    known = {r['entry'] for info in units.values() for r in info['rows']}
    read = lambda name: list(csv.DictReader(open(R.MANIFEST / name))) if (R.MANIFEST / name).exists() else []
    return classify_judgments(read('provenance.csv'), read('header_judgments.csv'), known)


def kind_split(function, kinds, body):
    """({kind: (rows, body bytes)} over the function-level set FUNCTION, every kind listed; (rows, body bytes) of
    the soft rows) from KINDS ({entry: (kind, soft)}) and BODY ({entry: body bytes})."""
    split = {k: [0, 0] for k in JUDGMENT_KINDS}
    soft = [0, 0]
    for entry in set(function) & set(kinds):
        kind, is_soft = kinds[entry]
        split[kind][0] += 1; split[kind][1] += body.get(entry, 0)
        if is_soft:
            soft[0] += 1; soft[1] += body.get(entry, 0)
    return {k: tuple(v) for k, v in split.items()}, tuple(soft)


def format_kinds(split):
    """'i 1/362; iv-a 50/80,000; ...' (rows/body bytes per kind, kinds with no row left out)."""
    return '; '.join('%s %d/%d' % (k, split[k][0], split[k][1]) for k in JUDGMENT_KINDS if split.get(k, (0,))[0])


def judgment_entries(units=None):
    """(provenance entries, header-form dependent entries): every row flagged as a judgment call, accepted or not.
    Provenance: a non-empty judgment note in manifests/provenance.csv (this already takes the `bound: frozen_bound`
    and `bound: image-derived` rows, whose notes say so). Header forms: header_judgments.csv flags the member, not
    a row (review session, 2026-11-19); a row listed in that member's `dependents` column rests on the flagged form,
    so it counts as a judgment row too. Dependents are free text; an address in it (8 hex digits, or 0x and 6-8)
    counts when it is the entry of a row of units.json, so a parked or unknown address counts nothing."""
    import csv
    units = R.load_json(R.UNITS, {}) if units is None else units
    known = {r['entry'] for info in units.values() for r in info['rows']}
    path = R.MANIFEST / 'provenance.csv'
    provenance = {r['entry'] for r in csv.DictReader(open(path)) if (r.get('judgment') or '').strip()} \
        if path.exists() else set()
    path = R.MANIFEST / 'header_judgments.csv'
    header = set()
    for r in (csv.DictReader(open(path)) if path.exists() else []):
        if (r.get('judgment') or '').strip():
            header |= {m.group(1).lower().zfill(8) for m in HEX_ENTRY.finditer(r.get('dependents') or '')}
    return provenance, (header & known) - provenance


def clean_exact(whole, function, judgment, sensitive, asm_body, body):
    """Exact with no judgment call (user-approved process change, 2026-11-22, lane O37): the headline figures
    with every flagged row left out, so they cannot drift on flagged rows.
    WHOLE, FUNCTION: {entry: body+padding} of the whole-unit (catalogued) and function-level exact sets
    (function_level); JUDGMENT: entries flagged as judgment calls (judgment_entries, both kinds); SENSITIVE:
    count-sensitive entries (exact only in a probed declaration-count window); ASM_BODY: {entry: body} of the
    evidenced inline-asm rows (left out of the source share, as in source_share_of_text); BODY: {entry: body}.
    Returns dict(whole=(rows, bytes, source bytes), function=(rows, bytes, source bytes),
    judgment=(rows, body bytes), sensitive=(rows, body bytes)), the last two over the function-level set.
    Source bytes are C/C++ body bytes (lane O41, 2026-11-22): the kept rows' body bytes without padding and
    without evidenced inline asm, the measure of main's "Matched C/C++ body bytes"; bytes stay body plus padding."""
    flagged = set(judgment) | set(sensitive)
    out = {}
    for name, level in (('whole', whole), ('function', function)):
        kept = set(level) - flagged
        size = sum(level[e] for e in kept)
        out[name] = (len(kept), size, sum(body.get(e, 0) - asm_body.get(e, 0) for e in kept))
    j = set(function) & set(judgment)
    s = (set(function) & set(sensitive)) - j
    out['judgment'] = (len(j), sum(body.get(e, 0) for e in j))
    out['sensitive'] = (len(s), sum(body.get(e, 0) for e in s))
    return out


def clean_measures(regions, level=None):
    """clean_exact on the tree: the catalog's rows and units.json (see clean_exact)."""
    units = R.load_json(R.UNITS, {})
    level = level or function_level(regions)
    provenance, header = judgment_entries(units)
    sensitive = {e for info in units.values() for e in (info.get('count_sensitive') or {})}
    body = {r['entry']: r['body'] for info in units.values() for r in info['rows']}
    body.update(('%08x' % r['entry'], r['body_bytes']) for r in regions)
    asm_body = {'%08x' % r['entry']: r['body_bytes'] for r in regions if r.get('inline_asm') == 'evidenced'}
    out = clean_exact(level['whole_entries'], level['function_entries'], provenance | header, sensitive, asm_body,
                      body)
    fl = set(level['function_entries'])
    out['header'] = (len(header & fl), sum(body.get(e, 0) for e in header & fl))
    # Exact with no soft judgment (lane O63, 2026-11-23): only the judgment rows of a soft kind (iii, iv-b, vii, and
    # header-form dependents of a soft form) and the count-sensitive rows left out.
    kinds, out['unclassified'] = judgment_kinds(units)
    out['kinds'], _ = kind_split(fl, kinds, body)
    soft = {e for e, (_, is_soft) in kinds.items() if is_soft}
    free = clean_exact(level['whole_entries'], level['function_entries'], soft, sensitive, asm_body, body)
    out['soft_free_whole'], out['soft_free_function'], out['soft'] = free['whole'], free['function'], free['judgment']
    return out


def header_judgments():
    """[header member] flagged as judgment calls in header_judgments.csv (review session, 2026-11-19: a
    hypothesis in a shared class is flagged on the member, not on a row that depends on it)."""
    import csv
    path = R.MANIFEST / 'header_judgments.csv'
    return [r['member'] for r in csv.DictReader(open(path)) if (r.get('judgment') or '').strip()] \
        if path.exists() else []


def layout_and_pending(regions):
    """(layout symbols, pending ranges, pending bytes) of the catalogued units (DECISIONS, "Rebuild data no
    exact code refers to", review 2026-11-19): data placed by its layout position (data_bound_by: layout, the
    declaration a hypothesis in the flag note) and data ranges owned by parked functions (data_pending: the
    original's bytes, never defined, no credit)."""
    sources = {r['source'] for r in regions}
    layout = ranges = size = 0
    for info in R.load_json(R.UNITS, {}).values():
        if info['source'] not in sources:
            continue
        layout += sum(k == 'layout' for k in (info.get('data_bound_by') or {}).values())
        for item in info.get('data_pending') or []:
            ranges += 1
            size += item['size']
    return layout, ranges, size


def content_bound(regions):
    """(symbols, bytes) of catalogued units bound by their initialised bytes (data_bound_by: content; DECISIONS,
    review session 2026-11-20): pointer tables, every byte compared, the declaration a hypothesis in the flag
    note; zero .text credit by themselves. Bytes are each table's section size in its unit's last checked
    object (its string literals are not counted)."""
    import data_bindings
    return unplaced_objects(regions, data_bindings.CONTENT)


def evidenced_inline_asm(regions):
    """((rows, body bytes) of every catalogued row flagged inline_asm: evidenced, (rows, body bytes) of those
    whose blocks are reached through a file-static or inline helper of their unit (DECISIONS, review session
    and user 2026-11-20; canonical.module_inline_asm_blocks with helpers)). Evidenced inline assembly is its
    own ledger category: no C/C++ credit."""
    from canonical import module_inline_asm_blocks, _block_helper
    flagged = [r for r in regions if r.get('inline_asm') == 'evidenced']
    by_source = {}
    for r in regions:
        by_source.setdefault(r['source'], []).append(r)
    helper = []
    for source in sorted({r['source'] for r in flagged}):
        rows = by_source[source]
        if len(rows) < 2:
            continue
        text = (R.ROOT / source).read_text(errors='replace')
        attributed = module_inline_asm_blocks(text, rows, helpers=True)
        helper += [r for r in rows if any(_block_helper(b) for b in attributed[r['entry']])]
    return ((len(flagged), sum(r['body_bytes'] for r in flagged)),
            (len(helper), sum(r['body_bytes'] for r in helper)))


def asm_not_flagged(regions):
    """(rows, body bytes, entries) of catalogued rebuild rows main flags inline_asm: evidenced that the rebuild
    does not (review session, 2026-11-20): asm counted as C/C++ until header-template attribution flags them
    (blitblit.h BlitTrans<unsigned char>::BlitForward 0x775980, BlitTransXlat<unsigned short>::BlitForward 0x4861C0;
    rebuild_catalog.header_specialization_flags flags them, lane O16, pending the user's go-ahead)."""
    main = {r['entry']: r for r in R.main_catalog()['regions'] if r.get('inline_asm') == 'evidenced'}
    rows = [r for r in regions if r['entry'] in main and r.get('inline_asm') != 'evidenced']
    return len(rows), sum(r['body_bytes'] for r in rows), sorted(r['entry'] for r in rows)


# Byte forms of the distinctive string instructions an asm helper may hold (warning only).
DISTINCTIVE = {'rep stosd': b'\xf3\xab', 'rep stosb': b'\xf3\xaa', 'rep stosw': b'\x66\xf3\xab',
               'rep movsd': b'\xf3\xa5', 'rep movsb': b'\xf3\xa4', 'rep movsw': b'\x66\xf3\xa5',
               'repne scasb': b'\xf2\xae', 'repe cmpsb': b'\xf3\xa6'}


def helper_byte_warnings(regions):
    """Warning, not a gate (review session, 2026-11-20): in a unit with an asm helper, each unflagged row whose
    original bytes hold a distinctive instruction of the helper's blocks (`rep stosd` for surface_quick_fill)."""
    from canonical import module_inline_asm_blocks, _block_helper, inline_asm_blocks
    import rebuild_catalog
    by_source = {}
    for r in regions:
        by_source.setdefault(r['source'], []).append(r)
    warnings = []
    for source, rows in sorted(by_source.items()):
        text = (R.ROOT / source).read_text(errors='replace')
        if len(rows) < 2 or not inline_asm_blocks(text):
            continue
        try:
            attributed = module_inline_asm_blocks(text, rows, helpers=True)
        except ValueError:
            continue
        helper = [b for blocks in attributed.values() for b in blocks if _block_helper(b)]
        patterns = {i: DISTINCTIVE[i] for b in helper for i in b[1] if i in DISTINCTIVE}
        if not helper:
            continue
        if not patterns:
            warnings.append('%s: helper blocks hold no distinctive instruction to look for' % source)
            continue
        audit = rebuild_catalog._audit()
        for r in rows:
            if r.get('inline_asm') == 'evidenced':
                continue
            body = audit.original(r['entry'], r['body_bytes'])
            found = [i for i, pattern in patterns.items() if pattern in body]
            if found:
                warnings.append('%s %08x unflagged, original bytes hold %s' % (source, r['entry'], ', '.join(found)))
    return warnings


def unplaced_objects(regions, kind=None):
    """(symbols, bytes) of the zero-initialised objects catalogued units leave unplaced and unreferenced
    (data_bound_by: unplaced-unreferenced; DECISIONS, user 2026-11-18): no address, nothing earned. Bytes are
    each symbol's extent in its unit's last checked object. kind: another data_bound_by kind counted the same way."""
    import data_bindings
    from coff import Coff
    sources = {r['source'] for r in regions}
    count = size = 0
    for unit, info in R.load_json(R.UNITS, {}).items():
        names = [n for n, k in (info.get('data_bound_by') or {}).items() if k == (kind or data_bindings.UNPLACED)]
        last = R.load_json(R.WORK / 'last' / (unit + '.json'), None)
        if not names or info['source'] not in sources or not last:
            continue
        # The unit's last check must be of its current text; a stale or missing object is checked again
        # (the census never counts a symbol it cannot find in the object).
        import hashlib
        current = hashlib.sha256((R.ROOT / info['source']).read_bytes()).hexdigest()
        if last.get('source_sha256') != current or not Path(last['object']).exists():
            R.check(unit)
            last = R.load_json(R.WORK / 'last' / (unit + '.json'), None)
        coff = Coff(open(last['object'], 'rb').read())
        for name in names:
            try:
                index = data_bindings.defining_section(coff, name)
            except data_bindings.BindingRefused as error:
                print('  unplaced: %s: %s (not counted)' % (unit, error))
                continue
            symbols = data_bindings.section_symbols(coff, index)
            values = [v for v, _ in symbols] + [coff.sections[index - 1]['size']]
            position = [n for _, n in symbols].index(name)
            count += 1
            size += values[position + 1] - values[position]
    return count, size


def unit_faults(unit, source_text):
    """Per-unit structure faults over the unit's budget: (externs, budget), (suffixed, budget)."""
    budget = json.loads(BUDGET.read_text()) if BUDGET.exists() else {}
    import move_externs
    externs = len([n for n, l in C.extern_names(C.code_only(source_text)) if not move_externs.defines(source_text, n)])
    suffixed = len(set(C.address_suffixed(source_text)))
    faults = []
    if externs > budget.get('externs_by_unit', {}).get(unit, 0):
        faults.append('%d stray externs (budget %d)' % (externs, budget.get('externs_by_unit', {}).get(unit, 0)))
    if suffixed > budget.get('address_suffixed_by_unit', {}).get(unit, 0):
        faults.append('%d address-suffixed names (budget %d)' % (suffixed, budget.get('address_suffixed_by_unit', {}).get(unit, 0)))
    return faults


BUDGET = R.MANIFEST / 'structure_budget.json'


def gate(measures):
    """Violations of the budget (empty when the tree may land)."""
    budget = json.loads(BUDGET.read_text()) if BUDGET.exists() else {}
    return ['%s %d over budget %d' % (k, measures[k], budget.get(k, 0)) for k in
            ('duplicates', 'outside', 'free_vs_member', 'tool_data', 'address_suffixed_headers', 'address_suffixed')
            + PLACEHOLDER_KEYS + ('unformatted_new_lines',) if measures[k] > budget.get(k, 0)]


def image_bounds(units):
    """(frozen rows, frozen bytes, unit rows flagged `bound: image-derived`, [mismatch]) for the census line
    (review session ruling 2026-11-22, docs/DECISIONS.md "Image-derived bound"): a flagged unit row must equal a
    frozen row of manifests/image_bounds.csv (entry, body bytes, padding)."""
    import csv
    path = R.MANIFEST / 'image_bounds.csv'
    frozen = {r['entry']: r for r in csv.DictReader(open(path)) if r['status'] == 'bound'} if path.exists() else {}
    flagged, wrong = 0, []
    for unit, info in units.items():
        for row in info['rows']:
            if row.get('bound') != 'image-derived':
                continue
            flagged += 1
            f = frozen.get(row['entry'].lower())
            if f is None or (int(f['body_bytes']), int(f['padding_bytes'])) != (row.get('body'), row.get('padding')):
                wrong.append('%s %s' % (unit, row['entry']))
    return len(frozen), sum(int(r['body_bytes']) for r in frozen.values()), flagged, wrong


def main(quiet=False):
    files = subprocess.run(['git', '-C', str(R.ROOT), 'ls-files', 'src', 'include'],
                           capture_output=True, text=True).stdout.split()
    untracked = subprocess.run(['git', '-C', str(R.ROOT), 'ls-files', '--others', '--exclude-standard',
                                'src', 'include'], capture_output=True, text=True).stdout.split()
    tree = sorted(set(files) | set(untracked))
    cat = catalog_io.load(R.MANIFEST / 'catalog')
    units = R.load_json(R.UNITS, {})
    unit_map = {u: [r['entry'] for r in i['rows']] for u, i in units.items()}
    modules = {u: dict(source=i['source']) for u, i in units.items()}
    read = C.reader(R.ROOT)
    result = C.structure(R.ROOT, None, tree, cat['regions'], unit_map, modules, read)
    homes = {i['source'] for i in units.values()}
    srcs = [p for p in tree if p.startswith('src/') and p.endswith(('.cpp', '.c'))]
    outside = [p for p in srcs if p not in homes]
    result['files_outside'].update(unit_files=len([p for p in srcs if p in homes]), outside=len(outside),
                                   one_function=0, one_function_folders=0, one_function_top_folders=0,
                                   several_functions=0, no_row=len(outside))
    measures = structure_measures(tree, read, result)
    measures.update(placeholder_measures(tree, read))
    measures['judgment_rows'], measures['judgment_bytes'] = judgment_calls(cat['regions'])
    measures['judgment_members'] = header_judgments()
    copies = helper_copies(tree, read)
    measures['helper_copies'] = len(copies)
    # Declared, never defined (review session, 2026-11-19): declarations RA2 has no body for; budget only falls.
    import declared_never_defined
    never = [k for k, _ in declared_never_defined.census()[0]]
    measures['declared_never_defined'] = len(never)
    inliner = inliner_statements()
    provisional = provisional_names()
    measures['provisional_names'] = len(provisional)
    measures['inliner_statements'] = len(inliner)
    measures['unplaced_symbols'], measures['unplaced_bytes'] = unplaced_objects(cat['regions'])
    measures['layout_symbols'], measures['pending_ranges'], measures['pending_bytes'] = layout_and_pending(cat['regions'])
    measures['content_symbols'], measures['content_bytes'] = content_bound(cat['regions'])
    (measures['inline_asm_rows'], measures['inline_asm_bytes']), \
        (measures['helper_asm_rows'], measures['helper_asm_bytes']) = evidenced_inline_asm(cat['regions'])
    measures['asm_unflagged_rows'], measures['asm_unflagged_bytes'], unflagged = asm_not_flagged(cat['regions'])
    measures['count_sensitive_rows'], measures['count_sensitive_bytes'] = count_sensitive()
    measures['header_form_parked_rows'], measures['header_form_parked_bytes'] = header_form_parked()
    level = function_level(cat['regions'])
    measures['function_level_rows'], measures['function_level_bytes'] = level['function']
    measures['whole_body_bytes'], measures['function_level_body_bytes'] = level['whole_body'], level['function_body']
    clean = clean_measures(cat['regions'], level)
    measures['clean_whole_rows'], measures['clean_whole_bytes'], measures['clean_whole_source_bytes'] = clean['whole']
    measures['clean_function_rows'], measures['clean_function_bytes'], measures['clean_function_source_bytes'] = \
        clean['function']
    measures['judgment_any_rows'], measures['judgment_any_bytes'] = clean['judgment']
    measures['header_dependent_rows'], measures['header_dependent_bytes'] = clean['header']
    measures['clean_sensitive_rows'], measures['clean_sensitive_bytes'] = clean['sensitive']
    measures['judgment_kinds'] = clean['kinds']
    measures['judgment_unclassified'] = clean['unclassified']
    measures['soft_judgment_rows'], measures['soft_judgment_bytes'] = clean['soft']
    measures['soft_free_whole_rows'], measures['soft_free_whole_bytes'], measures['soft_free_whole_source_bytes'] = \
        clean['soft_free_whole']
    measures['soft_free_function_rows'], measures['soft_free_function_bytes'], \
        measures['soft_free_function_source_bytes'] = clean['soft_free_function']
    # House format of new lines (lane F1): the diff BASE..working tree; adopted text and manifests/format_exempt.csv
    # are not counted. Gated when RA2_FMT_BASE names the base (the owner's landing, after fmtnew.py); without it the
    # merge-base with rebuild-trial is measured and reported only, so a lane's catalog is never refused for it.
    import fmtnew
    fmt_gated = bool(os.environ.get('RA2_FMT_BASE'))
    fmt_count, fmt_note = fmtnew.unformatted(detail=not quiet)
    measures['unformatted_new_lines'] = fmt_count if fmt_gated else 0
    if quiet:
        return measures
    print(C.structure_block(result).replace('src/recovered/', 'src/'))
    guards = Counter()
    suffixed = []
    for p in tree:
        text = read(p) or ''
        guards[p] += len(re.findall(r'#ifdef RA2_NON_MATCHING', text))
        for name in sorted(set(C.address_suffixed(text))):
            suffixed.append('%s (%s)' % (name, p))
    print('Exceptions: RA2_NON_MATCHING draft guards %d in %s' % (
        sum(guards.values()), ', '.join('%s %d' % kv for kv in sorted(guards.items()) if kv[1])))
    print('  switches: ' + ', '.join('%s (%d files)' % (k, len(v)) for k, v in result['switches']['by_switch'].items()))
    print('  address-suffixed: ' + ('; '.join(suffixed[:60]) or 'none'))
    print('  multiply defined: ' + ', '.join('%s %s' % (t['name'], t['files']) for t in result['classes']['top']))
    print('Gate: duplicates by qualified name %d%s; externs in sources %d; address-suffixed %d; outside %d; '
          'free-vs-member %d; tool data %d' % (measures['duplicates'], (' ' + ', '.join('%s %s' % kv for kv in
          sorted(measures['duplicate_list'].items()))) if measures['duplicates'] else '', measures['externs'],
          measures['address_suffixed'], measures['outside'], measures['free_vs_member'], measures['tool_data']))
    print('Gate (placeholders, only fall): process chatter %d; Unknown* names %d; vt_entry_NN %d; field_NN in headers %d; '
          'FUN_ in headers %d' % (measures['process_chatter'], measures['unknown_names'], measures['vt_entry_names'],
                                  measures['field_names_headers'], measures['fun_names_headers']))
    print('Parked tail (reported, not gated): RA2_NON_MATCHING guards %d; %d rows not exact, %d body bytes; '
          '1 KB or more: %d rows, %d body bytes' % ((measures['draft_guards'],) + parked_tail()))
    no_row = parked_no_row()
    print('  of which parked drafts with no row (src-final non_exact rows and image-derived bounds that no units.json '
          'or catalog row holds; in the parked tail since lane O96): %d functions, %d body bytes' % (
              len(no_row), sum(b for b, _ in no_row.values())))
    print('Judgment calls (reported, not gated): %d rows, %d body bytes; header members %d (%s)' % (
        measures['judgment_rows'], measures['judgment_bytes'], len(measures['judgment_members']),
        ', '.join(measures['judgment_members']) or 'none'))
    print('Gate (may not grow): unit-local copies of a shared helper %d: %s' % (measures['helper_copies'], ', '.join(
        '%s %s (%s)' % (p.split('/')[-1][:-4], n, h) for p, n, h in copies) or 'none'))
    evidenced = twin_helpers(read)
    print('Report: unit-local helpers evidenced by the twin: %d%s' % (len(evidenced), ''.join(
        (': ' if i == 0 else ', ') + '%s %s (%s)' % (r['unit'], r['name'], r['twin_path'] if ':' in r['twin_path'] else
                                 'ra2-decomp ' + r['twin_path'])
        for i, r in enumerate(evidenced))))
    listed = declared_never_defined.count()
    print('Gate (may only fall): declared, never defined %d (manifests/declared_never_defined.csv%s)' % (
        measures['declared_never_defined'], '' if listed == len(never) else
        ', lists %d: run declared_never_defined.py --write' % listed))
    print('Gate (may not grow): statements kept only for the inliner %d: %s' % (measures['inliner_statements'], ', '.join(
        '%s %s' % (u, e) for u, e, _ in inliner) or 'none'))
    print('Provisional names (reported, not gated; manifests/provisional_names.csv): %d (%s)' % (
        measures['provisional_names'], ', '.join('%s %d' % kv for kv in sorted(Counter(u for u, _ in provisional).items()))
        or 'none'))
    print('Count-sensitive rows (exact form, parked by declaration count; not in the parked tail): %d rows, %d body bytes' % (
        measures['count_sensitive_rows'], measures['count_sensitive_bytes']))
    print('Exact rows parked for evidenced header forms (in the parked tail; last exact text in a dossier): '
          '%d rows, %d bytes' % (measures['header_form_parked_rows'], measures['header_form_parked_bytes']))
    print('Exact functions (reported, not gated; no credit change): whole-unit (catalogued, whole-file exact) %d rows, '
          '%d bytes; function-level (each function checked on its own, as ra2-decomp reports) %d rows, %d bytes (%s)' % (
              level['whole'] + level['function'] + ('; '.join('+%d rows %d B %s' % (n, b, k)
                                                              for k, (n, b) in level['parts'].items()),)))
    print('C/C++ body bytes of those (no padding; evidenced inline asm included, main\'s measure leaves it out): '
          'whole-unit %d, function-level %d' % (level['whole_body'], level['function_body']))
    print('Exact with no judgment call (reported, not gated; user 2026-11-22): whole-unit %d rows, %d bytes (C/C++ '
          'body %d); function-level %d rows, %d bytes (C/C++ body %d); left out of the function-level set: judgment calls %d rows, %d body '
          'bytes (provenance judgment notes, with %d rows %d B listed as dependents of a flagged header form), '
          'count-sensitive %d rows, %d body bytes' % (
              measures['clean_whole_rows'], measures['clean_whole_bytes'], measures['clean_whole_source_bytes'],
              measures['clean_function_rows'], measures['clean_function_bytes'], measures['clean_function_source_bytes'],
              measures['judgment_any_rows'], measures['judgment_any_bytes'],
              measures['header_dependent_rows'], measures['header_dependent_bytes'],
              measures['clean_sensitive_rows'], measures['clean_sensitive_bytes']))
    print('Judgment calls by kind over the function-level set (rows/body bytes; lane O63; kinds in rebuild_census.py): '
          '%s; unclassified notes %d%s' % (format_kinds(measures['judgment_kinds']) or 'none',
                                            len(measures['judgment_unclassified']), ''.join(
              (': ' if i == 0 else ', ') + '%s %s' % w for i, w in enumerate(measures['judgment_unclassified']))))
    print('Exact with no soft judgment (reported, not gated; left out: judgment kinds %s, header-form dependents of a '
          'form of those kinds, count-sensitive rows): whole-unit %d rows, %d bytes (C/C++ body %d); function-level %d '
          'rows, %d bytes (C/C++ body %d); soft judgment rows %d, %d body bytes' % (
              ', '.join(sorted(SOFT_KINDS)), measures['soft_free_whole_rows'], measures['soft_free_whole_bytes'],
              measures['soft_free_whole_source_bytes'], measures['soft_free_function_rows'],
              measures['soft_free_function_bytes'], measures['soft_free_function_source_bytes'],
              measures['soft_judgment_rows'], measures['soft_judgment_bytes']))
    print('Unplaced unreferenced objects (data_bound_by: unplaced-unreferenced; earn nothing): %d symbols, %d bytes' % (
        measures['unplaced_symbols'], measures['unplaced_bytes']))
    print('Data placed by layout (data_bound_by: layout; declarations a hypothesis): %d symbols; pending data owned by '
          'parked functions: %d ranges, %d bytes' % (measures['layout_symbols'], measures['pending_ranges'],
                                                      measures['pending_bytes']))
    print('Data bound by content (data_bound_by: content; pointer tables, every byte compared; declarations a '
          'hypothesis; zero .text credit by themselves): %d symbols, %d bytes' % (measures['content_symbols'],
                                                                                  measures['content_bytes']))
    print('Evidenced inline assembly (inline_asm: evidenced, cited TibSun __asm; its own ledger category, no C/C++ '
          'credit): %d rows, %d body bytes' % (measures['inline_asm_rows'], measures['inline_asm_bytes']))
    print('  of which reached through a file-static or inline helper of the unit (the archive\'s own helper): '
          '%d rows, %d body bytes' % (measures['helper_asm_rows'], measures['helper_asm_bytes']))
    print('  asm bytes not yet flagged in the rebuild (main flags them; header-template attribution to fix): '
          '%d rows, %d body bytes (%s)' % (measures['asm_unflagged_rows'], measures['asm_unflagged_bytes'],
                                           ', '.join('%08x' % e for e in unflagged) or 'none'))
    frozen_rows, frozen_bytes, flagged, wrong = image_bounds(units)
    print('Image-derived bounds (bound: image-derived; frozen in manifests/image_bounds.csv by image_bounds.py): '
          '%d rows, %d bytes frozen; unit rows using one %d%s' % (frozen_rows, frozen_bytes, flagged,
                                                                  ('; NOT the frozen bound: ' + ', '.join(wrong))
                                                                  if wrong else ''))
    for warning in helper_byte_warnings(cat['regions']):
        print('  warning (not a gate): ' + warning)
    print('Gate (inherited, to be evidenced): #define GEOMETRY_H in sources %d; #pragma auto_inline(off) %d; '
          'extern "C" in sources %d' % (measures['inherited_geometry_define'], measures['inherited_auto_inline_off'],
                                         measures['extern_c_in_sources']))
    print('%s: new lines not in house format %d (since %s)%s' % (
        'Gate' if fmt_gated else 'Report (gated at the landing with RA2_FMT_BASE)', fmt_count,
        fmtnew.default_base() or '-', (': ' + fmt_note) if fmt_note else ''))
    print('Gate: ' + ('; '.join(gate(measures)) or 'within budget'))
    return measures


if __name__ == '__main__':
    main()
