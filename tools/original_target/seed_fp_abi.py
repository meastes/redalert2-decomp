"""Conservative source-backed ABI facts for accepted ordinary members.
No record layout or argument size is guessed. Unknown value types refuse.
"""
import re
from seed_fp_native import require

WORDS={'int','unsigned','unsigned int','signed','signed int','long','unsigned long','signed long','float','bool','char','signed char','unsigned char','short','short int','unsigned short','unsigned short int','signed short','signed short int','__int32','unsigned __int32','signed __int32'}
WIDE={'double','__int64','unsigned __int64','signed __int64','long long','unsigned long long','signed long long'}

def value_bytes(kind):
    kind=' '.join(kind.split())
    require(not any(ch in kind for ch in '()[]=,<>{}'),'Unknown compound/default argument type')
    if '*' in kind or '&' in kind:return 4
    unqualified=' '.join(word for word in kind.split() if word not in ('const','volatile'))
    require(unqualified in WORDS|WIDE,'Unknown by-value type width')
    return 8 if unqualified in WIDE else 4

def member_stack_arguments(proofs,entry,seed):
    proofs.provenance(entry)
    require(entry in proofs.rows,'Member has no accepted C++ source')
    row=proofs.rows[entry];symbol=row.get('symbol','')
    symbol_name=re.match(r'^\?([A-Za-z_]\w*)@([A-Za-z_]\w*)@@[QU][AB]E',symbol)
    require(symbol_name is not None and symbol.endswith('Z'),'Unsupported ordinary thiscall member symbol')
    name,owner=symbol_name.group(1,2)
    source=proofs._source_bytes(row['source'],row['source_sha256']).decode('utf-8');view=seed.code_view(source)
    require(not re.search(r'\b(?:__asm|asm)\b',view),'Opaque member contains assembly')
    pattern=r'^([^;{}\n]+?)\s*'+re.escape(owner)+r'::'+re.escape(name)+r'\s*\(([^()]*)\)\s*(?:const\s*)?\{'
    definitions=list(re.finditer(pattern,view,re.M))
    require(len(definitions)==1,'Missing or ambiguous ordinary member definition')
    result,parameters=definitions[0].group(1,2);result=result.strip()
    require('__' not in result.replace('__int64','').replace('__int32',''),'Explicit/nonstandard member ABI')
    # A record value result may add a hidden result pointer. Scalar and pointer/
    # reference results have no such uncounted argument under this ABI.
    if result!='void':value_bytes(result)
    args=[] if not parameters.strip() or parameters.strip()=='void' else parameters.split(',')
    kinds=[]
    for argument in args:
        require('...' not in argument,'Variadic member ABI')
        kind,param=seed.param_parts(argument.strip())
        require(bool(param) and re.fullmatch(r'[A-Za-z_]\w*',param),'Unknown parameter declaration')
        value_bytes(kind);kinds.append(kind)
    return dict(entry=entry,symbol=symbol,source=row['source'],source_sha256=row['source_sha256'],stack_argument_bytes=sum(value_bytes(kind) for kind in kinds),parameter_types=kinds,hidden_result_pointer=False,calling_convention='thiscall',return_type=result,owner=owner,member_name=name)
