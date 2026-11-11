"""Brace-consistent views of unknown external preprocessor choices.

This is a structural index aid, not a C preprocessor. Balanced alternatives
retain the index's existing combined view. Choices affecting enclosing brace
structure are projected consistently, and the index compares all projections.
Object-like constant/definedness flags are tracked without expanding C tokens.
Unsupported structural expressions or value-macro expansion refuse.
"""
import itertools
import re


def _blank(text):
    return ''.join('\n' if c == '\n' else ' ' for c in text)


def _condition(text):
    tokens=[];at=0
    while at<len(text):
        match=re.match(r'\s*(defined\b|[A-Za-z_]\w*|0[xX][0-9a-fA-F]+[uUlL]*|[0-9]+[uUlL]*|&&|\|\||[!()])',text[at:])
        if not match:
            if text[at:].strip():return None
            break
        tokens.append(match[1]);at+=match.end()
    pos=0
    def unary():
        nonlocal pos
        if pos>=len(tokens):raise ValueError()
        token=tokens[pos];pos+=1
        if token=='!':return ('not',unary())
        if token=='(':
            value=disjunction()
            if pos>=len(tokens) or tokens[pos]!=')':raise ValueError()
            pos+=1;return value
        if token=='defined':
            paren=pos<len(tokens) and tokens[pos]=='('
            if paren:pos+=1
            if pos>=len(tokens) or not re.fullmatch(r'[A-Za-z_]\w*',tokens[pos]):raise ValueError()
            name=tokens[pos];pos+=1
            if paren:
                if pos>=len(tokens) or tokens[pos]!=')':raise ValueError()
                pos+=1
            return ('defined',name)
        if re.fullmatch(r'[A-Za-z_]\w*',token):return ('value',token)
        if re.fullmatch(r'(?:0[xX][0-9a-fA-F]+|[0-9]+)[uUlL]*',token):
            numeric=re.sub('[uUlL]+$','',token);return ('constant',int(numeric,16 if numeric.lower().startswith('0x') else 10)!=0)
        raise ValueError()
    def conjunction():
        nonlocal pos
        value=unary()
        while pos<len(tokens) and tokens[pos]=='&&':pos+=1;value=('and',value,unary())
        return value
    def disjunction():
        nonlocal pos
        value=conjunction()
        while pos<len(tokens) and tokens[pos]=='||':pos+=1;value=('or',value,conjunction())
        return value
    try:
        value=disjunction()
        return value if pos==len(tokens) else None
    except ValueError:return None


def _atoms(expr):
    if expr is None:return None
    if expr[0] in ('defined','value'):return {expr}
    if expr[0]=='constant':return set()
    result=set()
    for child in expr[1:]:result.update(_atoms(child))
    return result


def _evaluate(expr,env):
    if expr is None:return None
    op=expr[0]
    if op=='constant':return expr[1]
    if op in ('defined','value'):
        if expr in env:return env[expr]
        if op=='value' and env.get(('defined',expr[1])) is False:return False
        if op=='defined' and env.get(('value',expr[1])) is True:return True
        return None
    if op=='not':
        value=_evaluate(expr[1],env);return None if value is None else not value
    a=_evaluate(expr[1],env);b=_evaluate(expr[2],env)
    if op=='and':return False if a is False or b is False else True if a is True and b is True else None
    return True if a is True or b is True else False if a is False and b is False else None


def structural_views(text,code_view,max_variants=128):
    """Return equal-length source views; never choose a unit's macro setting."""
    visible=code_view(text);lines=text.splitlines(True);views=visible.splitlines(True);root=[];current=root;stack=[];index=0
    while index<len(lines):
        raw=lines[index];view=views[index];match=re.match(r'^[ \t]*#\s*([A-Za-z_]\w*)\b(.*)',view)
        if not match:current.append(('text',raw,view));index+=1;continue
        logical=raw;payload=match[2];kind=match[1];index+=1
        while logical.rstrip('\r\n').endswith('\\') and index<len(lines):
            payload=payload.rstrip('\r\n')[:-1]+views[index];logical+=lines[index];index+=1
        current.append(('text',_blank(logical),_blank(logical)))
        payload=payload.strip()
        if kind in ('if','ifdef','ifndef'):
            expr=_condition(payload if kind=='if' else 'defined('+payload+')')
            if kind=='ifndef' and expr is not None:expr=('not',expr)
            node=['if',[(expr,[])],False];current.append(node);stack.append((current,node));current=node[1][-1][1]
        elif kind in ('elif','else'):
            if not stack or stack[-1][1][2]:raise ValueError('Malformed preprocessor alternative')
            parent,node=stack[-1]
            if kind=='else':node[2]=True;expr=('constant',True)
            else:expr=_condition(payload)
            node[1].append((expr,[]));current=node[1][-1][1]
        elif kind=='endif':
            if not stack:raise ValueError('Unmatched preprocessor endif')
            current,node=stack.pop()
        elif kind in ('define','undef'):
            name=re.match(r'[A-Za-z_]\w*',payload)
            if name:
                tail=payload[name.end():].strip()
                current.append(('flag',kind,name[0],tail))
    if stack:raise ValueError('Unterminated preprocessor conditional')
    structural=[]
    def summaries(nodes):
        possible={(0,0)}
        for node in nodes:
            if node[0]=='text':
                depth=0;minimum=0
                for c in node[2]:
                    depth+=(c=='{')-(c=='}');minimum=min(minimum,depth)
                choices={(depth,minimum)}
            elif node[0]=='flag':choices={(0,0)}
            else:
                choices=set()
                for expr,arm in node[1]:choices.update(summaries(arm))
                if not node[2]:choices.add((0,0))
                if any(net or minimum<0 for net,minimum in choices):structural.append(node)
            possible={(a+c,min(b,a+d)) for a,b in possible for c,d in choices}
        return possible
    summaries(root);atoms=set()
    for node in structural:
        for expr,arm in node[1]:
            found=_atoms(expr)
            if found is None:raise ValueError('Unsupported structural preprocessor expression')
            atoms.update(found)
    # A standalone index reads each guarded header once. Include guards are
    # not unit configuration choices, even when they enclose local flag defs.
    guards=set()
    for node in root:
        if node[0]=='text' and not node[2].strip():continue
        if node[0]=='if' and not node[2] and len(node[1])==1:
            expr,arm=node[1][0]
            first=next((child for child in arm if child[0]!='text' or child[2].strip()),None)
            if expr is not None and expr[0]=='not' and expr[1][0]=='defined' and first and first[0]=='flag' and first[1]=='define' and first[2]==expr[1][1]:guards.add(first[2])
        break
    local=[]
    def flag_paths(nodes,conditions=()):
        for node in nodes:
            if node[0]=='flag':local.append((node,conditions))
            elif node[0]=='if':
                # All branch tests are dependencies of an elif/else decision.
                tests=tuple(expr for expr,arm in node[1])
                for expr,arm in node[1]:flag_paths(arm,conditions+tests)
    flag_paths(root)
    while True:
        old=set(atoms)
        for node,conditions in local:
            if not any(name==node[2] for kind,name in atoms):continue
            for expr in conditions:
                found=_atoms(expr)
                if found is None:raise ValueError('Unsupported structural flag definition condition')
                atoms.update(found)
            if node[1]=='define' and ('value',node[2]) in atoms:
                value=_condition(node[3])
                if value is None or _atoms(value):raise ValueError('Unsupported structural value-macro expansion')
        if atoms==old:break
    tracked={name for kind,name in atoms}|guards
    atoms=sorted(atom for atom in atoms if atom[1] not in guards)
    kinds={}
    for kind,name in atoms:kinds.setdefault(name,set()).add(kind)
    initial_states=1
    for queried in kinds.values():initial_states*=3 if queried=={'defined','value'} else 2
    if initial_states>max_variants:raise ValueError('Too many structural preprocessor variants')
    def render(nodes,env,enabled=True):
        out=[]
        for node in nodes:
            if node[0]=='text':out.append(node[1] if enabled else _blank(node[1]));continue
            if node[0]=='flag':
                if enabled and node[2] in tracked:
                    name=node[2];defined=node[1]=='define';env[('defined',name)]=defined
                    expr=_condition(node[3]) if defined else ('constant',False)
                    env[('value',name)]=_evaluate(expr,{})
                continue
            possible=enabled;branches=[]
            for expr,arm in node[1]:
                truth=_evaluate(expr,env);active=possible and truth is not False;branch=dict(env);out.append(render(arm,branch,active))
                if active:branches.append(branch)
                if truth is True:possible=False
            if possible and not node[2]:branches.append(dict(env))
            if enabled and branches:
                keys=set().union(*(branch.keys() for branch in branches))
                for key in keys:
                    values=[branch.get(key) for branch in branches]
                    if all(value==values[0] for value in values):env[key]=values[0]
                    else:env.pop(key,None)
        return ''.join(out)
    result=[];seen=set()
    for values in itertools.product((False,True),repeat=len(atoms)):
        env=dict(zip(atoms,values));env.update({('defined',name):False for name in guards});env.update({('value',name):False for name in guards})
        if any(env.get(('value',name)) is True and env.get(('defined',name)) is False for kind,name in atoms):continue
        projected=render(root,env)
        if projected not in seen:seen.add(projected);result.append(projected)
    return result
