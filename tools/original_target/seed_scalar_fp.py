"""Private scalar-FP source view; requires the R430 native graph utility.

One float(double) call and one following compiler conversion only. Preserve
source expressions and storage; never synthesize a missing FP expression.
"""
import re
import json
import hashlib
import capstone
from seed_callee_abi import graph, require, AbiRefusal
X=capstone.x86_const
FP={'float','double','float10'}
INT={'char','signed char','unsigned char','short','unsigned short','int','unsigned int','long','unsigned long','bool','byte','ushort','uint','undefined1','undefined2','undefined4'}

def expression_type(expression,types,seed,depth=0):
    """Read-only scalar grammar: no calls, mutations, wide values or volatile.

    Unlike seed_expression_type this recognizes FP arithmetic. It does not
    rewrite the tree or infer a packed/wide carrier's numerical meaning.
    """
    if depth>64 or re.search(r'\bvolatile\b|/\*|//|\+\+|--|[;{}?:=,]',expression):return None
    e=expression.strip()
    while e.startswith('('):
        try:closing=seed.matching_paren(e,0)
        except ValueError:return None
        if closing!=len(e)-1:break
        e=e[1:-1].strip()
    stack=[];ops=[];ranks={'|':0,'^':1,'&':2,'<<':3,'>>':3,'+':4,'-':4,'*':5,'/':5,'%':5};i=0
    while i<len(e):
        c=e[i]
        if c in '([':stack.append(c)
        elif c in ')]':
            if not stack or stack.pop()!=('(' if c==')' else '['):return None
        elif not stack:
            op=e[i:i+2] if e[i:i+2] in ('<<','>>') else c
            if op in ranks and not (op in ('+','-','*','&') and seed.unary_operator(e,i)):ops.append((ranks[op],i,op))
            if len(op)==2:i+=1
        i+=1
    if stack:return None
    if ops:
        _,at,op=min(ops,key=lambda x:(x[0],-x[1]));a=expression_type(e[:at],types,seed,depth+1);b=expression_type(e[at+len(op):],types,seed,depth+1)
        if a not in FP|INT or b not in FP|INT:return None
        if a in FP or b in FP:
            if op not in ('+','-','*','/'):return None
            return 'double' if a in ('double','float10') or b in ('double','float10') else 'float'
        return 'int'
    if re.fullmatch(r'[A-Za-z_]\w*',e):return types.get(e) if types.get(e) in FP|INT else None
    if re.fullmatch(r'(?:[0-9]+\.[0-9]*|[0-9]*\.[0-9]+)(?:[eE][+-]?[0-9]+)?[fF]?',e):return 'float' if e[-1:] in 'fF' else 'double'
    if re.fullmatch(r'(?:0[xX][0-9a-fA-F]+|[0-9]+)[uUlL]*',e):
        kind=seed.seed_expression_type(e,types);return kind if kind in INT else None
    load=re.match(r'^\*\s*\(\s*([A-Za-z_][\w ]*)\s*\*\s*\)\s*',e)
    if load:
        kind=' '.join(load[1].split());address=e[load.end():]
        # Existing seed address grammar includes increments, which are excluded
        # above. Validate all scalar reads, not just a cast's outer type.
        if kind not in FP|INT:return None
        k=seed.seed_expression_type(address,types)
        return kind if k and ('*' in k or k in INT) and not re.search(r'\b[A-Za-z_]\w*\s*\(',address) else None
    cast=re.match(r'^\(\s*([A-Za-z_][\w ]*)\s*\)\s*',e)
    if cast:
        kind=' '.join(cast[1].split());operand=e[cast.end():]
        if kind in FP|INT and expression_type(operand,types,seed,depth+1) in FP|INT:return kind
        # An explicit ordinary IA32 object-pointer-to-DWORD cast is already
        # present in source. Recognize its integral result without inventing a
        # dereference, treating its pointee as FP, or rewriting any expression.
        if kind in {'int','unsigned int','long','unsigned long','uint','undefined4'} and ordinary_pointer_leaf(operand,types,seed):return kind
        return None
    array=re.fullmatch(r'([A-Za-z_]\w*)\s*\[(.*)\]',e,re.S)
    if array:
        t=types.get(array[1],'');kind=t.rsplit('*',1)[0].strip() if '*' in t else None
        return kind if kind in FP|INT and expression_type(array[2],types,seed,depth+1) in INT else None
    if e.startswith('*'):
        name=e[1:].strip();t=types.get(name,'');kind=t.rsplit('*',1)[0].strip() if re.fullmatch(r'[A-Za-z_]\w*',name) and '*' in t else None
        return kind if kind in FP|INT else None
    if e.startswith(('+','-')):return expression_type(e[1:],types,seed,depth+1)
    return None

def ordinary_pointer_leaf(expression,types,seed):
    """Conservative explicit cast operand: one declared ordinary pointer read.

    MSVC's target is i386; nondefault pointer representations, function/code
    pointers, array/reference/qualified declarations and compound expressions
    remain unsupported. No layout or pointee numeric interpretation is used.
    """
    value=expression.strip()
    while value.startswith('('):
        try:end=seed.matching_paren(value,0)
        except ValueError:return False
        if end!=len(value)-1:break
        value=value[1:-1].strip()
    if not re.fullmatch(r'[A-Za-z_]\w*',value):return False
    kind=types.get(value,'')
    if not isinstance(kind,str) or re.search(r'\b(?:const|volatile|restrict|__restrict|__ptr32|__ptr64|__sptr|__uptr|far|near|__far|__near|code)\b',kind):return False
    # Explicit stars establish the representation. The pointee must itself
    # be a known object type, so a function typedef cannot pass as a record.
    match=re.fullmatch(r'([A-Za-z_]\w*(?:\s+[A-Za-z_]\w*)*)\s*(?:\*\s*)+',kind)
    if match is None:return False
    base=' '.join(match[1].split())
    if base in FP|INT|{'void','longlong','ulonglong','undefined8','__int32','unsigned __int32','__int64','unsigned __int64','signed int','signed long','long long','unsigned long long'}:return True
    if base.startswith(('struct ','class ')):base=base.split(' ',1)[1]
    if not re.fullmatch(r'[A-Za-z_]\w*',base):return False
    try:return base in seed.typed_schema().records
    except (ValueError,OSError,AttributeError):return False


def scalar_signature(source,symbol,seed):
    matches=list(re.finditer(r'^float __cdecl ([A-Za-z_]\w*)\(double ([A-Za-z_]\w*)\)\s*\{',seed.code_view(source),re.M))
    require(len(matches)==1,'Unknown float(double) definition');m=matches[0]
    require(symbol=='?'+m[1]+'@@YAMN@Z','Scalar symbol disagrees with definition')
    return m[1]

def source_pair(body,params,callee,helper,globals_,seed):
    view=seed.code_view(body);name='FUN_%08x'%callee;consumer='FUN_%08x'%helper
    sites=list(seed.call_sites(body,name));conversions=list(seed.call_sites(body,consumer))
    require(len(sites)==len(conversions)==1,'Ambiguous source producer/conversion')
    start,end,args,_=sites[0];require(len(args)==1,'Missing or split FP expression')
    require(view[:start].rstrip().endswith(('{',';')),'Producer is not an unconditional sibling statement')
    m=re.match(r'\s*;\s*([A-Za-z_]\w*)\s*=\s*'+consumer+r'\s*\(\s*\)\s*;',view[end:])
    require(m is not None,'No adjacent scalar conversion assignment');stop=end+m.end()
    require(not re.search(r'/\*|//',body[start:stop]),'Rewrite would remove a comment')
    declarations=[name for kind,name,array in seed.LOCAL.findall(view)]
    declarations += [seed.param_parts(p)[1] for p in params]
    require(len(declarations)==len(set(declarations)),'Shadowed source declarations')
    require(declarations.count(m[1])==1,'Destination declaration is not unique')
    types=seed.variable_types(body,params)
    require(not any(name in types and types[name]!=kind for name,kind in globals_.items()),'Conflicting FP global evidence')
    types.update(globals_)
    require(types.get(m[1]) in INT-{'bool'},'Unsupported conversion destination')
    require(expression_type(args[0],types,seed) in FP,'Unknown or wide FP input expression')
    return start,stop,m[1]+' = (longlong)'+body[start:end]+';'

def immediate_call(ins,target):
    return ins.mnemonic=='call' and len(ins.operands)==1 and ins.operands[0].type==X.X86_OP_IMM and ins.operands[0].imm&0xffffffff==target

def plain_return(ins):return ins.mnemonic=='ret' and (not ins.operands or ins.operands[0].imm==0)

def native_pair(audit,inventory,caller,callee,callee_hash,helper):
    cn,ce,cp,raw=graph(audit,inventory,callee)
    require(hashlib.sha256(raw).hexdigest()==callee_hash,'Accepted scalar native body differs')
    returns=[i for i in cn.values() if i.group(capstone.CS_GRP_RET)]
    require(returns and all(plain_return(i) for i in returns),'Scalar callee has nonzero/unknown cleanup')
    first=cn[callee]
    require(first.mnemonic=='fld' and len(first.operands)==1,'Unknown initial double argument load')
    o=first.operands[0]
    require(o.type==X.X86_OP_MEM and o.size==8 and o.mem.base==X.X86_REG_ESP and o.mem.disp==4 and not o.mem.index and not o.mem.segment,'Callee does not read a double argument')
    nodes,edges,pred,_=graph(audit,inventory,caller);producers=[i for i in nodes.values() if immediate_call(i,callee)];consumers=[i for i in nodes.values() if immediate_call(i,helper)]
    require(len(producers)==len(consumers)==1,'Ambiguous native producer/conversion')
    call=producers[0];cleanup=nodes.get(call.address+call.size)
    require(cleanup and pred[cleanup.address]=={call.address} and cleanup.mnemonic=='add' and cleanup.op_str=='esp, 8','Unknown or bypassed caller cleanup')
    consumer=consumers[0]
    require(cleanup.address+cleanup.size==consumer.address and pred[consumer.address]=={cleanup.address},'Intervening instruction or bypass before conversion')
    # Walk a unique linear prefix to its exact eight-byte allocation. Only
    # known integer/x87 operations; no calls, branches or hidden state changes.
    prefix=[];at=call.address
    for _ in range(128):
        require(len(pred[at])==1,'Ambiguous argument setup ingress');before=next(iter(pred[at]));i=nodes[before]
        require(before+i.size==at,'Nonlinear argument setup');prefix.append(i)
        if i.mnemonic=='sub' and i.op_str=='esp, 8':break
        require(i.mnemonic in {'mov','lea','movsx','movzx','imul','add','sub','and','or','xor','neg','shl','shr','sar','cdq','fld','fild','fstp','fadd','faddp','fmul','fmulp','fsub','fsubp','fsubr','fsubrp','fdiv','fdivp','fdivr','fdivrp','fchs'},'Unknown operation in argument setup')
        require(not {X.X86_REG_ESP,X.X86_REG_SP}.intersection(i.regs_access()[1]),'Unexpected stack change in argument setup')
        at=before
    else:raise AbiRefusal('Missing bounded argument allocation')
    stores=[]
    for i in prefix[:-1]:
        for j,o in enumerate(i.operands):
            write=o.type==X.X86_OP_MEM and (bool(o.access&capstone.CS_AC_WRITE) or i.mnemonic=='fstp' and j==0)
            if not write:continue
            require(o.mem.base==X.X86_REG_ESP and not o.mem.index and not o.mem.segment and o.mem.disp>=0,'Unknown argument-memory alias')
            if o.mem.disp<8:
                require(i.mnemonic=='fstp' and o.mem.disp==0 and o.size==8,'Partial or overlapping double argument store');stores.append(i.address)
    require(len(stores)==1,'Missing or repeated double argument store')
    return dict(producer=call.address,consumer=consumer.address,allocation=prefix[-1].address,argument_store=stores[0],cleanup=cleanup.address,returns=[i.address for i in returns])

def helper_contract(repository,audit,inventory,helper):
    ledger=json.loads((repository/'manifests/original_target/assembly.json').read_text())
    rows=[row for row in ledger['regions'] if int(row['entry'],16)==helper]
    require(len(rows)==1,'Missing/ambiguous compiler helper ledger');row=rows[0]
    require(row['symbol']=='__ftol' and row['source'].startswith('src/recovered/asm/'),'Unknown compiler conversion binding')
    require(hashlib.sha256((repository/row['source']).read_bytes()).hexdigest()==row['source_sha256'],'Helper source differs from accepted assembly')
    require(hashlib.sha256(audit.original(helper,row['bytes'])).hexdigest()==row['sha256'],'Helper native body differs from accepted assembly')
    nodes,edges,pred,raw=graph(audit,inventory,helper)
    returns=[i for i in nodes.values() if i.group(capstone.CS_GRP_RET)]
    require(len(raw)==row['bytes'] and returns and all(plain_return(i) for i in returns),'Unknown helper extent or cleanup')
    stores=[i for i in nodes.values() if i.mnemonic=='fistp']
    require(stores and all(i.operands[0].type==X.X86_OP_MEM and i.operands[0].size==8 for i in stores),'Unknown helper conversion width')
    return row['sha256']

_CONTEXTS={}
def recover(repository,inventory,caller,callee,body,params,prototype,accepted,seed):
    ret,conv,old_params=prototype
    if callee not in accepted or ret.strip() not in FP or conv not in ('','__cdecl') or len(old_params)!=2:return None
    if any(seed.param_parts(p)[0] not in INT for p in old_params):return None
    evidence=(repository/'private/original-target-20261020/inventory').resolve()
    directory=next((p for p in inventory.passes if (p/'decompiled'/('%08x.c'%caller)).is_file()),None)
    if directory is None or not directory.resolve().is_relative_to(evidence):return None
    try:
        key=repository.resolve()
        if key not in _CONTEXTS:
            import catalog_io
            from verify_manifest import Audit
            cat=catalog_io.load(catalog_io.locate(repository),references=False);rows={}
            for row in cat['regions']:rows.setdefault(row['entry'],[]).append(row)
            _CONTEXTS[key]=(rows,Audit(repository,'private/original-target-20261020/inputs/game.exe'),cat)
        rows,audit,cat=_CONTEXTS[key];require(len(rows.get(callee,[]))==1,'Ambiguous accepted scalar')
        row=rows[callee][0];require(row['source']==accepted[callee] and row['source'].startswith('src/recovered/'),'Noncanonical scalar source')
        path=repository/row['source'];require(path.resolve().is_relative_to((repository/'src/recovered').resolve()),'Escaped scalar source')
        require(hashlib.sha256(path.read_bytes()).hexdigest()==row['source_sha256'],'Scalar source differs from accepted source hash')
        scalar_signature(path.read_text(encoding='utf-8'),row['symbol'],seed)
        from diff_candidate import known_references
        refs=known_references(cat);helper=refs.get('__ftol');require(helper is not None,'Unbound compiler conversion helper')
        globals_=seed.floating_operand_globals(body,inventory.function(caller)[1],params)
        start,end,replacement=source_pair(body,params,callee,helper,globals_,seed)
        proof=native_pair(audit,inventory,caller,callee,row['body_sha256'],helper)
        proof['helper_sha256']=helper_contract(repository,audit,inventory,helper)
    except (AbiRefusal,OSError,KeyError,ValueError,TypeError):return None
    return dict(body=body[:start]+replacement+body[end:],prototype=(ret,'__cdecl',['double input']),proof=proof,note='// Native scalar double argument and unique ST0-to-signed64 compiler conversion verified; original FP expression retained.')
