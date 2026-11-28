#!/usr/bin/env python3
"""permute.py UNIT FUNC [options]: an unattended source-permutation search for one parked function (lane O108,
2026-11-27). It drives the rebuild's compile and compare (rebuild.py check's path: build.compile_units under the
compiler lock, rebuild.compare with the unit's bindings) and walks a beam of variants of the function's text, with
every compiled variant cached by its text.

FUNC is the function's name as the definition spells it (`Adjust_Control_Position`, `StripClass::Draw_It`,
`Tactical::Draw_Tiles(Cell`: text after the name selects an overload by its first parameter). The definition's
RA2_NON_MATCHING guard is opened in a scratch copy; the tree's file is never written.

Mutations (each keeps the same variables, values and side effects; none adds a statement that does no work):

  perm    order of adjacent independent sibling statements and local declarations (order_search.py's rule:
          no calls, no shared reads/writes, no aliasing writes); all orders of a run up to 5 at depth 0, adjacent
          swaps after
  incr    `x++` <-> `++x` as a statement or for step (order_search.py)
  loop    `for` <-> `while` (order_search.py)
  comm    operands of + * & | ^ == != swapped (both call-free and side-effect free; && and || too when neither
          operand dereferences, divides or calls); a left operand that would re-associate is parenthesised
  rel     `a < b` <-> `b > a` (and <=, >=), call-free operands
  not     `!x` <-> `x == 0`; in a test, `x != 0` <-> `x`
  andif   `if (A && B) {..}` <-> `if (A) { if (B) {..} }` (no else on either)
  tern    `v = c ? a : b;` <-> `if (c) { v = a; } else { v = b; }` (and returns)
  inline  a builtin or pointer local used once, its initialiser call-free and nothing between writes what it reads
          (or the use the next statement with no other call): the initialiser written at the use
  name    `if (COND) {` -> `bool test_N = COND; if (test_N) {` (a named condition; not after else)
  alt     --alt ALTS.json: [{"old","new","source"}] texts from a twin or the archive

Never: dead statements, dummy variables, empty inline sites, duplicate copies, __forceinline, header or include
changes, asm. A candidate counts only if every row of the unit that is exact in the base stays exact (the whole
unit is compared, not just the target).

Options: --entry ADDR (default from the unit's row or manifests/image_bounds.csv by --symbol), --symbol SUB
(substring of the target's mangled name; default ?Name@Class@), --ref NAME=ADDR (extra binding, repeatable; NAME
matches a symbol exactly or as a substring), --open TEXT (also open every guard
whose text holds TEXT, e.g. a guarded declaration the draft uses; repeatable),
--kinds perm,comm,... (default all but alt unless --alt), --beam K (6), --depth D (rounds; 8), --slack B (bytes above the
best a state may sit and still be expanded; 0), --budget N (compiles; 1500), --hours H (6), --batch N (24),
--work DIR (private/scratch/permute), --report FILE.md, --list (print the base's single mutations and stop),
--seed N, --source FILE (start from a draft of the unit instead of the tree's file).

Output: WORK/UNIT-ENTRY/cache.jsonl (text hash -> score, kept across runs: a rerun resumes), table.tsv, best-N.diff.
With --report, a markdown record: base and best scores, the whole-unit result, each exact hit's diff. A hit is a
candidate, not a landing: it lands only under the normal rules (catalog, flags, whole-file build).
"""
import csv
import difflib
import hashlib
import itertools
import json
import random
import re
import shutil
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import order_search as O    # noqa: E402

KINDS_ALL = ('perm', 'incr', 'loop', 'comm', 'rel', 'not', 'andif', 'tern', 'inline', 'name', 'alt')

# ---------------------------------------------------------------------------------------------------------------
# A small C expression parser (token spans), enough for statements of one line.

TOKEN = re.compile(r'\s*(?:(?P<str>L?"(?:\\.|[^"\\])*"|L?\'(?:\\.|[^\'\\])*\')|(?P<num>0[xX][0-9a-fA-F]+[uUlL]*|'
                   r'\d+\.\d*(?:[eE][-+]?\d+)?[fFlL]?|\.\d+(?:[eE][-+]?\d+)?[fFlL]?|\d+(?:[eE][-+]?\d+)?[uUlLfF]*)|'
                   r'(?P<id>[A-Za-z_]\w*(?:\s*::\s*~?[A-Za-z_]\w*)*)|'
                   r'(?P<op>->\*|->|\+\+|--|<<=|>>=|<<|>>|<=|>=|==|!=|&&|\|\||[-+*/%&|^]=|::|[-+*/%&|^!~<>=?:,.()\[\]{}]))')
BINARY = [('||',), ('&&',), ('|',), ('^',), ('&',), ('==', '!='), ('<', '>', '<=', '>='), ('<<', '>>'),
          ('+', '-'), ('*', '/', '%')]
PREC = {op: i for i, ops in enumerate(BINARY) for op in ops}
ASSIGN = {'=', '+=', '-=', '*=', '/=', '%=', '&=', '|=', '^=', '<<=', '>>='}
COMMUTATIVE = {'+', '*', '&', '|', '^', '==', '!=', '&&', '||'}
FLIP = {'<': '>', '>': '<', '<=': '>=', '>=': '<='}
TYPE_WORDS = {'int', 'unsigned', 'signed', 'char', 'short', 'long', 'bool', 'float', 'double', 'void', 'const',
              'BYTE', 'WORD', 'DWORD', 'UINT', 'LONG', 'HWND', 'HANDLE', 'LPARAM', 'WPARAM', 'LRESULT', 'BOOL',
              'size_t', '__int64', 'wchar_t'}


class ParseError(Exception):
    pass


def tokenize(text):
    out, i = [], 0
    while i < len(text):
        if text[i:].strip() == '':
            break
        m = TOKEN.match(text, i)
        if not m or m.end() == i:
            raise ParseError('token at %d' % i)
        kind = m.lastgroup
        out.append((kind, m.group(kind), m.start(kind), m.end(kind)))
        i = m.end()
    return out


class Node:
    def __init__(self, kind, a, b, op=None, kids=(), calls=False, effects=False, deref=False, divide=False):
        self.kind, self.a, self.b, self.op, self.kids = kind, a, b, op, list(kids)
        self.calls = calls or any(k.calls for k in self.kids)
        self.effects = effects or any(k.effects for k in self.kids)
        self.deref = deref or any(k.deref for k in self.kids)
        self.divide = divide or any(k.divide for k in self.kids)
        self.parent = None
        for k in self.kids:
            k.parent = self

    def walk(self):
        yield self
        for k in self.kids:
            yield from k.walk()


class Parser:
    def __init__(self, text, types=()):
        self.text, self.toks, self.i, self.types = text, tokenize(text), 0, set(types) | TYPE_WORDS

    def peek(self, k=0):
        return self.toks[self.i + k] if self.i + k < len(self.toks) else (None, None, len(self.text), len(self.text))

    def take(self, value=None):
        t = self.peek()
        if t[0] is None or (value is not None and t[1] != value):
            raise ParseError('expected %s at %s' % (value, t[1]))
        self.i += 1
        return t

    def parse_all(self):
        node = self.expression()
        if self.i != len(self.toks):
            raise ParseError('trailing %s' % (self.peek()[1],))
        return node

    def expression(self):
        node = self.assignment()
        while self.peek()[1] == ',':
            self.take()
            right = self.assignment()
            node = Node('bin', node.a, right.b, ',', (node, right))
        return node

    def assignment(self):
        left = self.conditional()
        if self.peek()[1] in ASSIGN:
            op = self.take()[1]
            right = self.assignment()
            return Node('assign', left.a, right.b, op, (left, right), effects=True)
        return left

    def conditional(self):
        cond = self.binary(0)
        if self.peek()[1] == '?':
            self.take()
            yes = self.assignment()
            self.take(':')
            no = self.assignment()
            return Node('cond', cond.a, no.b, '?', (cond, yes, no))
        return cond

    def binary(self, level):
        if level == len(BINARY):
            return self.unary()
        node = self.binary(level + 1)
        while self.peek()[1] in BINARY[level] and self.peek()[0] == 'op':
            op = self.take()[1]
            right = self.binary(level + 1)
            node = Node('bin', node.a, right.b, op, (node, right), divide=op in ('/', '%'))
        return node

    def is_cast(self):
        # (type words [*&]) followed by an operand
        j, saw = self.i + 1, False
        while j < len(self.toks):
            kind, value = self.toks[j][0], self.toks[j][1]
            if kind == 'id' and (value in self.types or value.split('::')[-1] in self.types or
                                 re.match(r'^(struct|class|enum|unsigned|signed|const)$', value)):
                saw = True
            elif kind == 'id' and saw is False and j == self.i + 1 and re.match(r'^[A-Z]\w*$', value) and \
                    j + 1 < len(self.toks) and self.toks[j + 1][1] in ('*', '&') and \
                    j + 2 < len(self.toks) and self.toks[j + 2][1] in ('*', '&', ')'):
                saw = True    # (ClassName *) and (ClassName &)
            elif value in ('*', '&') and saw:
                pass
            elif value == ')' and saw:
                nxt = self.toks[j + 1] if j + 1 < len(self.toks) else (None, None)
                return nxt[0] in ('id', 'num', 'str') or nxt[1] in ('(', '!', '~', '-', '*', '&', '+')
            else:
                return False
            j += 1
        return False

    def unary(self):
        t = self.peek()
        if t[1] in ('!', '~', '-', '+', '*', '&') and t[0] == 'op':
            self.take()
            operand = self.unary()
            return Node('un', t[2], operand.b, t[1], (operand,), deref=t[1] == '*')
        if t[1] in ('++', '--'):
            self.take()
            operand = self.unary()
            return Node('un', t[2], operand.b, t[1], (operand,), effects=True)
        if t[1] == '(' and self.is_cast():
            self.take()
            while self.peek()[1] != ')':
                self.take()
            self.take(')')
            operand = self.unary()
            return Node('cast', t[2], operand.b, None, (operand,))
        if t[0] == 'id' and t[1] in ('sizeof',):
            self.take()
            if self.peek()[1] == '(':
                depth = 0
                while True:
                    v = self.take()[1]
                    depth += v == '('
                    depth -= v == ')'
                    if depth == 0:
                        break
                return Node('atom', t[2], self.toks[self.i - 1][3])
            operand = self.unary()
            return Node('un', t[2], operand.b, 'sizeof', (operand,))
        if t[0] == 'id' and t[1] in ('new', 'delete', 'throw', 'operator', 'this') and t[1] != 'this':
            raise ParseError('keyword')
        return self.postfix()

    def postfix(self):
        t = self.peek()
        if t[1] == '(':
            self.take()
            inner = self.expression()
            close = self.take(')')
            node = Node('paren', t[2], close[3], None, (inner,))
        elif t[0] in ('id', 'num', 'str'):
            self.take()
            if t[0] == 'id' and self.peek()[1] == '<' and re.match(r'^(static_cast|reinterpret_cast|const_cast|'
                                                                  r'dynamic_cast)$', t[1]):
                raise ParseError('cast template')
            node = Node('atom', t[2], t[3])
        else:
            raise ParseError('operand %s' % (t[1],))
        while True:
            v = self.peek()[1]
            if v == '(':
                self.take()
                args = []
                if self.peek()[1] != ')':
                    args.append(self.assignment())
                    while self.peek()[1] == ',':
                        self.take()
                        args.append(self.assignment())
                close = self.take(')')
                node = Node('call', node.a, close[3], None, [node] + args, calls=True)
            elif v == '[':
                self.take()
                index = self.expression()
                close = self.take(']')
                node = Node('index', node.a, close[3], None, (node, index), deref=True)
            elif v in ('.', '->'):
                self.take()
                member = self.take()
                if member[0] != 'id':
                    raise ParseError('member')
                node = Node('member', node.a, member[3], v, (node,), deref=v == '->')
            elif v in ('++', '--'):
                end = self.take()[3]
                node = Node('post', node.a, end, v, (node,), effects=True)
            else:
                return node


def parse(text, types=()):
    try:
        return Parser(text, types).parse_all()
    except (ParseError, RecursionError):
        return None


def wrap_if_needed(node, text, parent_op):
    """The node's text, parenthesised when it is a binary of the same or lower precedence than parent_op."""
    s = text[node.a:node.b]
    if node.kind in ('bin', 'assign', 'cond') and (node.kind != 'bin' or PREC.get(node.op, -1) <= PREC[parent_op]):
        return '(' + s + ')'
    return s


def is_operand_atom(node):
    return node.kind in ('atom', 'member', 'index', 'paren', 'call', 'cast', 'post') or \
        (node.kind == 'un' and node.op in ('*', '-', '~'))


def expression_mutations(text, region_start, region_end, boolean_root, types=()):
    """[(kind, description, new text)] for the expression text[region_start:region_end]."""
    sub = text[region_start:region_end]
    root = parse(sub, types)
    if root is None:
        return []
    out = []

    def splice(a, b, new):
        return text[:region_start + a] + new + text[region_start + b:]

    def boolean_context(node):
        p = node.parent
        if p is None:
            return boolean_root
        if p.kind == 'paren':
            return boolean_context(p)
        return (p.kind == 'bin' and p.op in ('&&', '||')) or (p.kind == 'un' and p.op == '!') or \
            (p.kind == 'cond' and p.kids[0] is node)

    for node in root.walk():
        if node.kind == 'bin' and node.op in COMMUTATIVE:
            left, right = node.kids
            pure = not (left.calls or right.calls or left.effects or right.effects)
            if node.op in ('&&', '||'):
                pure = pure and not (left.deref or right.deref or left.divide or right.divide)
            if pure:
                new = wrap_if_needed(right, sub, node.op) + sub[left.b:right.a] + wrap_if_needed(left, sub, node.op)
                # the gap keeps the operator and its spaces; strip any parentheses the swap made needless
                out.append(('comm', '%s: %s' % (node.op, sub[node.a:node.b][:60]), splice(node.a, node.b, new)))
        if node.kind == 'bin' and node.op in FLIP:
            left, right = node.kids
            if not (left.calls or right.calls or left.effects or right.effects):
                gap = sub[left.b:right.a].replace(node.op, FLIP[node.op], 1)
                new = wrap_if_needed(right, sub, node.op) + gap + wrap_if_needed(left, sub, node.op)
                out.append(('rel', '%s: %s' % (node.op, sub[node.a:node.b][:60]), splice(node.a, node.b, new)))
        if node.kind == 'un' and node.op == '!' and is_operand_atom(node.kids[0]):
            inner = sub[node.kids[0].a:node.kids[0].b]
            new = inner + ' == 0'
            if node.parent is not None and node.parent.kind == 'bin' and PREC.get(node.parent.op, 99) >= PREC['==']:
                new = '(' + new + ')'
            out.append(('not', '!x -> x == 0: %s' % sub[node.a:node.b][:60], splice(node.a, node.b, new)))
        if node.kind == 'bin' and node.op in ('==', '!='):
            left, right = node.kids
            zero = None
            if sub[right.a:right.b] == '0' and is_operand_atom(left):
                zero = left
            elif sub[left.a:left.b] == '0' and is_operand_atom(right):
                zero = right
            if zero is not None:
                x = sub[zero.a:zero.b]
                if node.op == '==':
                    new = '!' + x
                    out.append(('not', 'x == 0 -> !x: %s' % sub[node.a:node.b][:60], splice(node.a, node.b, new)))
                elif boolean_context(node):
                    out.append(('not', 'x != 0 -> x: %s' % sub[node.a:node.b][:60], splice(node.a, node.b, x)))
        if is_operand_atom(node) and node.kind != 'paren' and boolean_context(node) and \
                not (node.parent is not None and node.parent.kind == 'un' and node.parent.op == '!'):
            x = sub[node.a:node.b]
            loose = node.parent is None or node.parent.kind == 'paren' or \
                (node.parent.kind == 'bin' and node.parent.op in ('&&', '||')) or node.parent.kind == 'cond'
            new = '%s != 0' % x if loose else '(%s != 0)' % x
            out.append(('not', 'x -> x != 0: %s' % x[:60], splice(node.a, node.b, new)))
    return out


# ---------------------------------------------------------------------------------------------------------------
# Line-level mutations on the function's lines [start, end).

HEAD = re.compile(r'^(\s*)(\}\s*else\s+)?(if|while|switch)\s*\((.*)\)\s*\{?\s*$')


def indent_of(line):
    return re.match(r'\s*', line).group(0)


def head_condition(code):
    """(offset, end) of the condition inside `if (...)`/`while (...)` on a code line, or None."""
    m = re.match(r'^(\s*(?:\}\s*)?(?:else\s+)?(?:if|while)\s*)\(', code)
    if not m:
        return None
    depth, i = 0, m.end() - 1
    for j in range(i, len(code)):
        if code[j] == '(':
            depth += 1
        elif code[j] == ')':
            depth -= 1
            if depth == 0:
                return i + 1, j
    return None


def statement_mutations(lines, start, end, kinds, types=()):
    """[(kind, description, touched lines, function(lines) -> lines)] for the new kinds."""
    out = []
    for i in range(start, end):
        line = lines[i]
        code = O.code_of(line)
        if not code.strip() or code.strip().startswith('#'):
            continue
        regions = []
        hc = head_condition(code)
        if hc:
            regions.append((hc[0], hc[1], True))
        elif code.rstrip().endswith(';') and not re.match(r'^\s*(for|case|default)\b', code):
            body = code.rstrip()[:-1]
            m = re.match(r'^(\s*return\s+)', body)
            if m:
                regions.append((m.end(), len(body), False))
            else:
                lead = len(body) - len(body.lstrip())
                decl = re.match(r'^\s*(?:(?:const|unsigned|signed|struct|class|static)\s+)*[A-Za-z_][\w:]*(?:<[^;=()]*>)?'
                                r'(?:\s*[*&]+\s*|\s+)(?:const\s+)?[A-Za-z_]\w*\s*=(?!=)', body)
                whole = O.DECL.match(body.strip() + ';')
                if decl and decl.group(0).split()[0] not in O.KEYWORDS:
                    regions.append((decl.end(), len(body), False))
                elif whole and whole.group(1) not in O.KEYWORDS and whole.group(5) is None:
                    pass    # a declaration with no initialiser: nothing to spell
                elif parse(body.strip(), types) is not None:
                    regions.append((lead, len(body), False))
                else:
                    eq = re.search(r'(?<![=!<>+\-*/%&|^])=(?!=)', body)
                    if eq and '(' not in body[:eq.start()]:
                        regions.append((eq.end(), len(body), False))
        for a, b, boolean in regions:
            for kind, desc, new in expression_mutations(code, a, b, boolean, types):
                if kind in kinds:
                    replaced = new + line[len(code):]
                    out.append((kind, 'line %d: %s' % (i + 1, desc), {i},
                                lambda ls, i=i, r=replaced: ls[:i] + [r] + ls[i + 1:]))
    if 'andif' in kinds:
        out += andif_mutations(lines, start, end, types)
    if 'tern' in kinds:
        out += ternary_mutations(lines, start, end, types)
    if 'inline' in kinds:
        out += inline_mutations(lines, start, end)
    if 'name' in kinds:
        out += name_mutations(lines, start, end, types)
    return out


def block_close(lines, a, end):
    """Index of the line holding the brace that closes the block opened on line a (that line ends with `{`)."""
    depth = 0
    for i in range(a, end):
        code = O.strip_literals(O.code_of(lines[i]))
        for c in code:
            if c == '{':
                depth += 1
            elif c == '}':
                depth -= 1
                if depth == 0:
                    return i
    return None


def followed_by_else(lines, close, end):
    code = O.code_of(lines[close]).strip()
    if re.match(r'^\}\s*else\b', code):
        return True
    for j in range(close + 1, end):
        c = O.code_of(lines[j]).strip()
        if c:
            return c.startswith('else')
    return False


def reindent(lines, delta, unit):
    out = []
    for l in lines:
        if delta > 0:
            out.append(unit + l if l.strip() else l)
        else:
            out.append(l[len(unit):] if l.startswith(unit) else l.lstrip('\t') if l.startswith('\t') else l)
    return out


def andif_mutations(lines, start, end, types):
    out = []
    for i in range(start, end):
        code = O.code_of(lines[i])
        m = re.match(r'^(\s*)if\s*\(', code)
        if not m or not code.rstrip().endswith('{'):
            continue
        hc = head_condition(code)
        if not hc:
            continue
        close = block_close(lines, i, end)
        if close is None or followed_by_else(lines, close, end) or O.code_of(lines[close]).strip() != '}':
            continue
        cond = code[hc[0]:hc[1]]
        root = parse(cond, types)
        if root is None:
            continue
        indent, unit = m.group(1), '\t'
        comment = lines[i][len(code):]
        if root.kind == 'bin' and root.op == '&&':
            left, right = root.kids
            a, b = cond[left.a:left.b], cond[right.a:right.b]
            body = lines[i + 1:close]

            def split_if(ls, i=i, close=close, a=a, b=b, indent=indent, comment=comment, body=body):
                return ls[:i] + [indent + 'if (%s) {%s' % (a, comment), indent + unit + 'if (%s) {' % b] + \
                    reindent(body, 1, unit) + [indent + unit + '}', ls[close]] + ls[close + 1:]
            out.append(('andif', 'line %d: if (A && B) -> nested' % (i + 1), set(range(i, close + 1)), split_if))
        # nested -> &&: the block holds exactly one if with no else
        inner = [j for j in range(i + 1, close) if O.code_of(lines[j]).strip()]
        if inner and re.match(r'^\s*if\s*\(', O.code_of(lines[inner[0]])) and \
                O.code_of(lines[inner[0]]).rstrip().endswith('{'):
            j = inner[0]
            jclose = block_close(lines, j, close + 1)
            if jclose is not None and jclose == inner[-1] and O.code_of(lines[jclose]).strip() == '}':
                ihc = head_condition(O.code_of(lines[j]))
                icond = O.code_of(lines[j])[ihc[0]:ihc[1]] if ihc else None
                iroot = parse(icond, types) if icond else None
                if iroot is not None:
                    a = '(%s)' % cond if root.kind in ('bin', 'cond', 'assign') and root.op in ('||', '?', ',') or \
                        root.kind in ('cond', 'assign') else cond
                    b = '(%s)' % icond if iroot.kind in ('cond', 'assign') or \
                        (iroot.kind == 'bin' and iroot.op in ('||', ',')) else icond

                    def join_if(ls, i=i, j=j, jclose=jclose, close=close, a=a, b=b, indent=indent, comment=comment):
                        return ls[:i] + [indent + 'if (%s && %s) {%s' % (a, b, comment)] + \
                            reindent(ls[j + 1:jclose], -1, unit) + ls[close:]
                    out.append(('andif', 'line %d: nested -> if (A && B)' % (i + 1), set(range(i, close + 1)),
                                join_if))
    return out


def ternary_mutations(lines, start, end, types):
    out = []
    for i in range(start, end):
        code = O.code_of(lines[i])
        m = re.match(r'^(\s*)(return\s+|([^=;]*?[^=!<>+\-*/%&|^])\s*=(?!=)\s*)(.*);\s*$', code)
        if m:
            indent, lhs_text, expr = m.group(1), m.group(3), m.group(4)
            root = parse(expr, types)
            if root is not None and root.kind == 'cond' and not root.kids[0].effects and \
                    re.match(r'^[\w.\->\[\]\s*]+$', (lhs_text or 'return').strip()) and \
                    not re.match(r'^\s*[A-Za-z_]\w*(?:\s*::\s*\w+)?[\s*&]+\w+\s*$', lhs_text or ''):
                c, yes, no = (expr[k.a:k.b] for k in root.kids)
                c = c[1:-1] if root.kids[0].kind == 'paren' else c
                lead = 'return ' if lhs_text is None else lhs_text.strip() + ' = '
                new = [indent + 'if (%s) {' % c, indent + '\t' + lead + yes + ';', indent + '} else {',
                       indent + '\t' + lead + no + ';', indent + '}']
                out.append(('tern', 'line %d: ?: -> if/else' % (i + 1), {i},
                            lambda ls, i=i, new=new: ls[:i] + new + ls[i + 1:]))
        m = re.match(r'^(\s*)if\s*\(', code)
        if m and code.rstrip().endswith('{'):
            hc = head_condition(code)
            close = block_close(lines, i, end)
            if not hc or close is None or close + 3 >= end:
                continue
            seq = [O.code_of(lines[k]).strip() for k in (i + 1, close, close + 1, close + 2)]
            if close != i + 2 or seq[1] != '} else {' or seq[3] != '}':
                continue
            a = re.match(r'^(return\s+|([\w.\->\[\]*]+)\s*=(?!=)\s*)(.*);$', seq[0])
            b = re.match(r'^(return\s+|([\w.\->\[\]*]+)\s*=(?!=)\s*)(.*);$', seq[2])
            if not a or not b or a.group(1).split('=')[0].strip() != b.group(1).split('=')[0].strip():
                continue
            cond = code[hc[0]:hc[1]]
            croot = parse(cond, types)
            if croot is None:
                continue
            wrap = lambda s, r: '(%s)' % s if r is not None and r.kind in ('bin', 'cond', 'assign') else s
            c = wrap(cond, croot)
            ya, nb = a.group(3), b.group(3)
            yr, nr = parse(ya, types), parse(nb, types)
            if yr is None or nr is None:
                continue
            ya = '(%s)' % ya if yr.kind in ('cond', 'assign') or (yr.kind == 'bin' and yr.op == ',') else ya
            nb = '(%s)' % nb if nr.kind in ('assign',) or (nr.kind == 'bin' and nr.op == ',') else nb
            lead = 'return ' if a.group(1).startswith('return') else a.group(2) + ' = '
            new = m.group(1) + lead + '%s ? %s : %s;' % (c, ya, nb)
            out.append(('tern', 'line %d: if/else -> ?:' % (i + 1), set(range(i, close + 3)),
                        lambda ls, i=i, close=close, new=new: ls[:i] + [new] + ls[close + 3:]))
    return out


def inline_mutations(lines, start, end):
    """A local used once, its initialiser written at the use."""
    units = O.split_units(lines, start, end)
    out = []
    for k, u in enumerate(units):
        d = u.decl
        if not d or not d['init'] or d['array'] or u.b != u.a + 1:
            continue
        if not (d['type'] in O.BUILTIN or '*' in d['ptr']) or '&' in d['ptr']:
            continue
        name = d['name']
        uses = [(m, v) for m, v in enumerate(units[k + 1:], k + 1) if re.search(r'\b%s\b' % re.escape(name), v.code)]
        if len(uses) != 1:
            continue
        m, v = uses[0]
        if v.b != v.a + 1 or len(re.findall(r'\b%s\b' % re.escape(name), v.code)) != 1:
            continue
        if v.depth < u.depth or name in v.writes or re.search(r'(\+\+|--)\s*%s\b|\b%s\s*(\+\+|--)|&\s*%s\b' % (
                (re.escape(name),) * 3), v.code):
            continue
        # no loop opened between the declaration and the use (the value would be computed once per pass)
        between = units[k + 1:m]
        if any(re.match(r'^(for|while|do)\b', w.code) for w in between) or \
                (v.depth > u.depth and any(re.match(r'^(for|while|do)\b', w.code) for w in units[k + 1:m])):
            continue
        init = d['init'].rstrip(';').strip()
        reads = set(O.IDENT.findall(O.strip_literals(init))) - O.KEYWORDS
        has_calls = bool(re.search(r'[A-Za-z_]\w*\s*\(', init)) and \
            any(c not in O.NOT_CALLS for c in re.findall(r'([A-Za-z_]\w*)\s*\(', init))
        if has_calls:
            if m != k + 1 or [c for c in v.calls] != []:
                continue
        else:
            deref = '->' in init or '[' in init or re.search(r'(^|[^\w)\]])\*\s*[A-Za-z_(]', init)
            if any(w.calls or (w.writes & reads) or (deref and (w.memwrites or w.calls)) or not w.simple and
                   not re.match(r'^(if|else|\}|\{)', w.code) for w in between):
                continue
            if any(w.writes & reads for w in between) or any(w.calls for w in between):
                continue
        root = parse(init)
        if root is None:
            continue
        cast = '' if d['type'] in ('int',) and not d['ptr'].strip() else ''
        text_init = init if root.kind in ('atom', 'member', 'index', 'call', 'paren') else '(%s)' % init

        def apply(ls, a=u.a, use=v.a, name=name, text_init=text_init, cast=cast):
            line = ls[use]
            code = O.code_of(line)
            new = re.sub(r'\b%s\b' % re.escape(name), lambda _m: cast + text_init, code, count=1)
            return ls[:a] + ls[a + 1:use] + [new + line[len(code):]] + ls[use + 1:]
        out.append(('inline', 'line %d: inline %s at line %d' % (u.a + 1, name, v.a + 1), {u.a, v.a}, apply))
    return out


def in_case_scope(lines, i, start):
    """Whether line i sits directly under a case or default label (no block of its own between)."""
    depth = 0
    for j in range(i - 1, start - 1, -1):
        code = O.strip_literals(O.code_of(lines[j]))
        depth += code.count('}') - code.count('{')
        if depth < 0:
            return False
        if depth == 0 and re.match(r'^\s*(case\b.*:|default\s*:)\s*$', code):
            return True
    return False


def name_mutations(lines, start, end, types):
    out = []
    for i in range(start, end):
        code = O.code_of(lines[i])
        m = re.match(r'^(\s*)if\s*\(', code)
        if not m or not code.rstrip().endswith('{'):
            continue
        hc = head_condition(code)
        if not hc:
            continue
        cond = code[hc[0]:hc[1]]
        root = parse(cond, types)
        if root is None or root.kind in ('atom',) or root.effects:
            continue
        if in_case_scope(lines, i, start):
            continue    # a new local there would be skipped by the next case label
        indent = m.group(1)
        name = 'test_%d' % (i + 1)

        def apply(ls, i=i, indent=indent, cond=cond, code=code, hc=hc, name=name):
            head = code[:hc[0]] + name + code[hc[1]:] + ls[i][len(code):]
            return ls[:i] + [indent + 'bool %s = %s;' % (name, cond), head] + ls[i + 1:]
        out.append(('name', 'line %d: bool %s = %s' % (i + 1, name, cond[:50]), {i}, apply))
    return out


# ---------------------------------------------------------------------------------------------------------------
# Locating the function and scoring.

def locate_function(text, func):
    """(definition start, open brace, body end) of FUNC's definition in text."""
    name, _, rest = func.partition('(')
    pattern = r'(?<![\w:~])' + re.escape(name).replace(r'::', r'\s*::\s*') + r'\s*\(' + \
        (r'[^;{]*?' + re.escape(rest) if rest else '')
    hits = []
    for m in re.finditer(r'^[^\n;{}#/]*' + pattern + r'(?:[^;{/]|//[^\n]*)*\{', text, re.M):
        hits.append(m)
    if len(hits) != 1:
        raise SystemExit('cannot locate %s (%d definitions)' % (func, len(hits)))
    m = hits[0]
    return m.start(), m.end() - 1, O.body_end(text, m.end() - 1)


def opened_text(text, func, also=()):
    from tailmap import guards, open_text
    d0, brace, d1 = locate_function(text, func)
    opened = {s for s, e, label in guards(text) if s < d0 < e or any(a in text[s:e] for a in also)}
    return open_text(text, opened)


def function_lines(text, func):
    """(lines, first body line, last body line) of the function in text."""
    d0, brace, d1 = locate_function(text, func)
    return text.split('\n'), text.count('\n', 0, brace) + 1, text.count('\n', 0, d1 - 1)


def mutations(text, func, kinds, alts, depth0, types=()):
    lines, first, last = function_lines(text, func)
    out = []
    for t in O.transforms(lines, first, last, alts if 'alt' in kinds else [], False):
        if t[0] not in kinds:
            continue
        if t[0] == 'perm' and not depth0:
            p = t[3].__defaults__[1]
            moved = [i for i, x in enumerate(p) if i != x]
            if len(moved) != 2 or moved[1] != moved[0] + 1:
                continue
        out.append(t)
    out += statement_mutations(lines, first, last, kinds, types)
    return [(k, d, '\n'.join(fn(lines))) for k, d, touched, fn in out]


def class_names():
    """Type names the headers declare (for the cast test)."""
    import rebuild as R
    names = set()
    for path in (R.ROOT / 'include').rglob('*.h'):
        for m in re.finditer(r'\b(?:class|struct|enum|typedef[^;]*?)\s+([A-Za-z_]\w*)\s*[;{:]',
                             path.read_text(encoding='latin-1')):
            names.add(m.group(1))
    return names | O.BUILTIN


class Scorer:
    def __init__(self, unit, func, entry, symbol, refs, work, batch):
        import rebuild as R
        from verify_manifest import Audit
        self.R, self.unit, self.func, self.work, self.batch = R, unit, func, work, batch
        self.info = R.load_json(R.UNITS, {})[unit]
        self.config = json.loads(R.CONFIG.read_text())
        self.audit = Audit(R.ROOT, self.config['executable'])
        base = R.resolver(unit, self.info)
        def resolve(n):
            if n in refs:
                return refs[n], 'extra'
            hits = [v for k, v in refs.items() if k in n]
            return (hits[0], 'extra') if len(hits) == 1 else base(n)
        self.resolve = resolve
        bounds = {r['entry']: r for r in csv.DictReader(open(R.ROOT / 'manifests/image_bounds.csv'))}
        row = next((r for r in self.info['rows'] if r['entry'] == entry), None) if entry else None
        if row is None and entry in bounds:
            row = dict(entry=entry, body=int(bounds[entry]['body_bytes']), padding=int(bounds[entry]['padding_bytes']),
                       symbol=None)
        if row is None:
            raise SystemExit('no row or image bound for %s' % entry)
        self.row, self.symbol_sub = row, symbol
        self.salt = json.dumps(sorted(refs.items())) + entry
        self.cache_path = work / 'cache.jsonl'
        self.cache = {}
        if self.cache_path.exists():
            for line in self.cache_path.read_text().splitlines():
                rec = json.loads(line)
                self.cache[rec['key']] = rec
        self.compiles = 0

    def target_symbol(self, coff):
        if self.row.get('symbol') and not self.symbol_sub:
            return self.row['symbol']
        names = [s['name'] for s in coff._functions if self.symbol_sub in s['name']]
        if len(names) != 1:
            raise SystemExit('symbol %r: %d functions %s' % (self.symbol_sub, len(names), names[:6]))
        return names[0]

    def score(self, data):
        from coff import Coff
        R = self.R
        coff = Coff(data)
        functions = {s['name'] for s in coff._functions}
        target = self.target_symbol(coff)
        st, diff, size, b, probs = R.compare(self.audit, coff, target, int(self.row['entry'], 16), self.row['body'],
                                             self.row.get('gap_padding', self.row['padding']), self.resolve)
        result = dict(diff=diff, status=st, size=size, digest=hashlib.sha1(R.compare.actual).hexdigest()[:10],
                      problems=probs[:6])
        missed = []
        for r in self.info['rows']:
            if r['entry'] == self.row['entry']:
                continue
            if r['symbol'] not in functions:
                missed.append(r['entry'])
                continue
            s2 = R.compare(self.audit, coff, r['symbol'], int(r['entry'], 16), r['body'],
                           r.get('gap_padding', r['padding']), self.resolve)[0]
            if s2 != 'exact':
                missed.append(r['entry'])
        result['other_rows_missed'] = missed
        return result

    def key(self, text):
        return hashlib.sha1((self.salt + '\n' + text).encode('latin-1', 'replace')).hexdigest()

    def run(self, texts):
        """{key: result} for texts (compiling only those not in the cache)."""
        import build
        from canonical import compiler_arguments
        todo, seen = [], set()
        for t in texts:
            k = self.key(t)
            if k not in self.cache and k not in seen:
                todo.append((k, t)); seen.add(k)
        for i in range(0, len(todo), self.batch):
            chunk = todo[i:i + self.batch]
            folder = self.work / 'c'
            shutil.rmtree(folder, ignore_errors=True)
            folder.mkdir(parents=True)
            units = []
            for j, (k, t) in enumerate(chunk):
                src = folder / ('%s.cpp' % self.unit) if len(chunk) == 1 else folder / ('v%02d' % j) / (
                    '%s.cpp' % self.unit)
                src.parent.mkdir(parents=True, exist_ok=True)
                src.write_text(t, encoding='latin-1')
                units.append(dict(cwd=src.parent, source=src, obj=src.with_suffix('.obj'), log=src.with_suffix('.log'),
                                  returncode=src.with_suffix('.rc'), args=compiler_arguments(self.info['options'])))
            build.compile_units(self.config, folder, units, 'permute', parallel=True, timeout=3600)
            with open(self.cache_path, 'a') as f:
                for (k, t), u in zip(chunk, units):
                    self.compiles += 1
                    if Path(u['obj']).exists():
                        try:
                            rec = self.score(Path(u['obj']).read_bytes())
                        except SystemExit as error:
                            rec = dict(diff=None, status='no-symbol', error=str(error)[:200])
                    else:
                        log = Path(u['log']).read_text(errors='replace') if Path(u['log']).exists() else ''
                        rec = dict(diff=None, status='error',
                                   error=' '.join(l.strip() for l in log.splitlines() if 'error' in l)[:300])
                    rec['key'] = k
                    self.cache[k] = rec
                    f.write(json.dumps(rec) + '\n')
            shutil.rmtree(folder, ignore_errors=True)
        return {self.key(t): self.cache[self.key(t)] for t in texts}


# ---------------------------------------------------------------------------------------------------------------

def main(argv):
    import rebuild as R
    args = [a for i, a in enumerate(argv) if not a.startswith('--') and (i == 0 or not argv[i - 1].startswith('--')
                                                                          or argv[i - 1] in ('--list',))]
    opts = {}
    multi = {'--ref': [], '--open': []}
    flags = {'--list'}
    i = 0
    positional = []
    while i < len(argv):
        a = argv[i]
        if a in flags:
            opts[a] = True; i += 1
        elif a.startswith('--'):
            if a in multi:
                multi[a].append(argv[i + 1])
            else:
                opts[a] = argv[i + 1]
            i += 2
        else:
            positional.append(a); i += 1
    del args
    unit, func = positional[0], positional[1]
    info = R.load_json(R.UNITS, {})[unit]
    tree_text = Path(opts.get('--source', str(R.ROOT / info['source']))).read_text(encoding='latin-1')
    base_text = opened_text(tree_text, func, multi['--open'])
    short = re.sub(r'\W+', '_', func.split('(')[0].split('::')[-1])
    entry = opts.get('--entry')
    entry = entry.lower().replace('0x', '').zfill(8) if entry else None
    name_part = func.split('(')[0].split('::')
    symbol = opts.get('--symbol') or ('?%s@%s@' % (name_part[-1], name_part[-2]) if len(name_part) > 1 else
                                      '?%s@' % name_part[-1])
    if entry is None:
        raise SystemExit('--entry is required')
    if any(r['entry'] == entry for r in info['rows']) and '--symbol' not in opts:
        symbol = None
    kinds = set(opts.get('--kinds', ','.join(k for k in KINDS_ALL if k != 'alt' or '--alt' in opts)).split(','))
    alts = json.load(open(opts['--alt'])) if '--alt' in opts else []
    types = class_names()
    if opts.get('--list'):
        for k, d, t in mutations(base_text, func, kinds, alts, True, types):
            print('  %-6s %s' % (k, d))
        return
    random.seed(int(opts.get('--seed', '1')))
    work = Path(opts.get('--work', str(R.ROOT / 'private/scratch/permute'))) / ('%s-%s' % (unit, entry))
    work.mkdir(parents=True, exist_ok=True)
    refs = {}
    for r in multi['--ref']:
        n, v = r.rsplit('=', 1)
        refs[n] = int(v, 16)
    scorer = Scorer(unit, func, entry, symbol, refs, work, int(opts.get('--batch', '24')))
    beam_width, max_depth = int(opts.get('--beam', '6')), int(opts.get('--depth', '8'))
    slack, budget = int(opts.get('--slack', '0')), int(opts.get('--budget', '1500'))
    deadline = time.time() + float(opts.get('--hours', '6')) * 3600
    started = time.time()
    base = scorer.run([base_text])[scorer.key(base_text)]
    if base.get('diff') is None:
        raise SystemExit('the base does not compile or the target is not emitted: %s' % base.get('error'))
    others = [r['entry'] for r in info['rows'] if r['entry'] != entry]
    must = set(others) - set(base['other_rows_missed'])
    print('%s %s %s: base %s B (%s), %d of %d other rows exact; problems %s' % (
        unit, entry, func, base['diff'], base['status'], len(must), len(others), base['problems']), flush=True)
    # state: (text, path of descriptions)
    states = {scorer.key(base_text): (base_text, [])}
    beam = [scorer.key(base_text)]
    expanded = set()
    hits, best_key = [], scorer.key(base_text)

    def valid(rec):
        return rec.get('diff') is not None and not must & set(rec['other_rows_missed'])

    def rank(k):
        rec = scorer.cache[k]
        return (rec['diff'], len(states[k][1]))

    for depth in range(max_depth):
        children = []
        for k in beam:
            expanded.add(k)
            text, path = states[k]
            for kind, desc, new in mutations(text, func, kinds, alts, depth == 0, types):
                nk = scorer.key(new)
                if nk not in states:
                    states[nk] = (new, path + ['%s %s' % (kind, desc)])
                    children.append(nk)
        if not children:
            break
        random.shuffle(children)
        left = budget - scorer.compiles
        todo = [c for c in children if c not in scorer.cache][:max(0, left)]
        print('depth %d: %d children, %d to compile (budget left %d)' % (depth + 1, len(children), len(todo), left),
              flush=True)
        for i in range(0, len(todo), scorer.batch * 4):
            if time.time() > deadline:
                break
            scorer.run([states[c][0] for c in todo[i:i + scorer.batch * 4]])
            done = [c for c in children if c in scorer.cache and valid(scorer.cache[c])]
            if done:
                b = min(done, key=rank)
                if rank(b) < rank(best_key):
                    best_key = b
                    print('  best %s B: %s' % (scorer.cache[b]['diff'], ' + '.join(states[b][1])[:200]), flush=True)
            for c in done:
                if scorer.cache[c]['diff'] == 0 and c not in hits:
                    hits.append(c)
                    print('  EXACT: %s' % ' + '.join(states[c][1]), flush=True)
            if hits and '--all-hits' not in opts:
                break
        if hits and '--all-hits' not in opts:
            break
        if scorer.compiles >= budget or time.time() > deadline:
            break
        # The next beam: unexpanded states within the slack, one per object first (best first), then further
        # texts of the same objects (a walk along a plateau of equal code).
        pool = sorted((c for c in states if c not in expanded and c in scorer.cache and valid(scorer.cache[c]) and
                       scorer.cache[c]['diff'] <= scorer.cache[best_key]['diff'] + slack), key=rank)
        first, rest, digests = [], [], set()
        for c in pool:
            (rest if scorer.cache[c]['digest'] in digests else first).append(c)
            digests.add(scorer.cache[c]['digest'])
        beam = (first + rest)[:beam_width]
        if not beam:
            break
        print('  beam %s' % ' '.join('%s' % scorer.cache[c]['diff'] for c in beam), flush=True)
    hours = (time.time() - started) / 3600
    # outputs
    ranked = sorted((k for k in states if k in scorer.cache and scorer.cache[k].get('diff') is not None),
                    key=lambda k: (not valid(scorer.cache[k]), scorer.cache[k]['diff']))
    with open(work / 'table.tsv', 'w') as f:
        f.write('diff\tstatus\tother_rows_ok\tdigest\tpath\n')
        for k in ranked:
            rec = scorer.cache[k]
            f.write('%s\t%s\t%s\t%s\t%s\n' % (rec['diff'], rec['status'], valid(rec), rec['digest'],
                                             ' + '.join(states[k][1])))
    base_lines = [l + '\n' for l in base_text.split('\n')]

    def diff_of(k):
        return ''.join(difflib.unified_diff(base_lines, [l + '\n' for l in states[k][0].split('\n')],
                                            'tree (guard opened)', 'variant', n=2))
    for n, k in enumerate([k for k in ranked if valid(scorer.cache[k])][:3]):
        (work / ('best-%d.diff' % n)).write_text(diff_of(k))
    errors = sum(1 for k in states if k in scorer.cache and scorer.cache[k].get('diff') is None)
    distinct = len({scorer.cache[k]['digest'] for k in states if k in scorer.cache and scorer.cache[k].get('digest')})
    summary = ('%d variants scored (%d compiled this run, %d failed to compile), %d distinct objects for the row, '
               '%.2f h' % (sum(1 for k in states if k in scorer.cache), scorer.compiles, errors, distinct, hours))
    print(summary)
    best = scorer.cache[best_key]
    print('best %s B (%s): %s' % (best['diff'], best['status'], ' + '.join(states[best_key][1]) or 'tree text'))
    if '--report' in opts:
        report = Path(opts['--report'])
        report.parent.mkdir(parents=True, exist_ok=True)
        out = ['# %s %s 0x%s: permute.py (lane O108, %s)' % (unit, func, entry.upper(), time.strftime('%Y-%m-%d')), '',
               '- Not a landing: a candidate list. Base (tree text, guard opened): %s B (%s); %d of %d other rows exact.' % (
                   base['diff'], base['status'], len(must), len(others)),
               '- Search: kinds %s; beam %d, depth %d, slack %d. %s.' % (','.join(sorted(kinds)), beam_width,
                                                                        max_depth, slack, summary),
               '- Best keeping every other row exact: %s B (%s), object %s, path: %s.' % (
                   best['diff'], best['status'], best.get('digest'), ' + '.join(states[best_key][1]) or 'tree text'),
               '- Exact hits: %d.' % len(hits), '- Calibration used: no.', '']
        top = {}
        for k in ranked:
            rec = scorer.cache[k]
            if valid(rec):
                top.setdefault(rec['diff'], []).append(k)
        out.append('## Score distribution (valid variants: diff -> count)')
        out.append('')
        out.append(', '.join('%s: %d' % (d, len(v)) for d, v in sorted(top.items())[:15]))
        out.append('')
        for k in hits:
            rec = scorer.cache[k]
            out += ['## Exact hit: %s' % ' + '.join(states[k][1]), '',
                    '- Target: exact (%d B, object %s). Whole unit: every row exact in the base stays exact (%d of %d '
                    'other rows exact).' % (scorer.row['body'], rec['digest'], len(others) - len(rec['other_rows_missed']),
                                            len(others)),
                    '', '```diff', diff_of(k), '```', '']
        if not hits and best_key != scorer.key(base_text):
            out += ['## Best variant diff', '', '```diff', diff_of(best_key), '```', '']
        report.write_text('\n'.join(out))
        print('report: %s' % report)


if __name__ == '__main__':
    main(sys.argv[1:])
