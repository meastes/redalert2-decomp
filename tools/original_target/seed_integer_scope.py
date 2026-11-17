"""Discover an integer-first source lifetime hidden behind a NaN token.

Discovery never invents a NaN payload, native address or source rewrite. The
native field/value/control/alias proof must independently bind these witnesses.
"""
import re
from seed_guarded_cfg import require
WORDS={'int','unsigned int','signed int','uint','long','signed long','unsigned long','DWORD','ulong','undefined4'}

def bind_integer_scope(text,seed):
    ret,conv,name,params,(start,end)=seed.parse_header(text)
    original=text[end:];view=seed.code_view(original);types=seed.variable_types(view,params)
    decls=[(kind.strip(),variable,array) for kind,variable,array in seed.LOCAL.findall(view) if kind.strip().split()[0] not in ('return','goto','throw','delete','break','continue')]
    parameter_names={seed.param_parts(param)[1] for param in params}
    def declaration_count(variable):
        pattern=r'(?:^|[;{}])\s*([A-Za-z_]\w*(?:[\t ]+[A-Za-z_]\w*)*(?:[\t ]*\*+[\t ]*|[\t ]+))'+re.escape(variable)+r'\s*(?=[;=\[])'
        matches=[match for match in re.finditer(pattern,view,re.M) if match[1].strip().split()[0] not in ('return','goto','throw','delete','break','continue','else','case')]
        require(not re.search(r',\s*'+re.escape(variable)+r'\s*[;=\[]',view),'Ambiguous comma declaration of scope variable')
        return len(matches)

    full=seed.code_view(text)
    declared_nan=re.search(r'(?m)^\s*(?:(?:extern|static|const|volatile)\s+)*(?:float|double|int|uint|long)\s+\**NAN\b',full)
    require(not re.search(r'#\s*(?:define|undef)\s+NAN\b',full) and 'NAN' not in types and not declared_nan,'NAN names a real source declaration/macro')
    initializers=list(re.finditer(r'\b([A-Za-z_]\w*)\s*=\s*(-?)\s*NAN\s*;',view))
    require(len(initializers)==1,'Missing/ambiguous bare NaN lifetime')
    initial=initializers[0];variable=initial[1]
    require(types.get(variable)=='float' and variable not in parameter_names,'Carrier is not a float DWORD local')
    require(declaration_count(variable)==1 and [(kind,array) for kind,var,array in decls if var==variable]==[('float','')],'Carrier declaration is shadowed/array/unknown')
    assignments=list(re.finditer(r'\b'+re.escape(variable)+r'\s*=(?!=)\s*([^;]+);',view))
    require(len(assignments)>=3 and assignments[0].span()==initial.span(),'Missing ordered carrier lifetimes')
    update,reset=assignments[1:3];match=re.fullmatch(r'\(\s*float\s*\)\s*([A-Za-z_]\w*)',update[1].strip())
    require(match is not None,'Integer cache update is not a scalar float cast')
    counter=match[1];require(counter!=variable and types.get(counter) in WORDS,'Counter is not a known DWORD integer')
    counter_decls=[(kind,array) for kind,var,array in decls if var==counter]
    require(counter not in parameter_names and declaration_count(counter)==1 and len(counter_decls)==1 and not counter_decls[0][1],'Counter declaration is shadowed/unknown')
    require(not re.search(r'\b'+re.escape(variable)+r'\b',reset[1]),'Later float assignment still reads earlier carrier')
    reads=list(re.finditer(r'\b'+re.escape(counter)+r'\s*-\s*\(\s*int\s*\)\s*'+re.escape(variable)+r'\b',view[:update.start()]))
    require(len(reads)==1 and initial.end()<reads[0].start()<update.start()<reset.start(),'Missing unique ordered integer subtraction')
    comparison=re.match(r'\s*<\s*(-?(?:0x[0-9a-fA-F]+|[0-9]+))\b(?=\s*\))',view[reads[0].end():])
    require(comparison is not None,'Integer subtraction is not compared with an explicit less-than bound')
    token=comparison[1];limit=int(token,16 if 'x' in token.lower() else 10)
    require(-(1<<31)<limit<(1<<31),'Unsupported signed comparison limit')
    counter_updates=list(re.finditer(r'\b'+re.escape(counter)+r'\s*=\s*'+re.escape(counter)+r'\s*\+\s*(?:1|0x0*1)\s*;',view[initial.end():reset.start()]))
    require(counter_updates,'Missing supported first-lifetime counter increments')
    mutations=list(re.finditer(r'\b'+re.escape(counter)+r'\s*(?:=(?!=)|[+*/%&|^\-]=|\+\+|--)|(?:\+\+|--)\s*\b'+re.escape(counter)+r'\b',view[initial.end():reset.start()]))
    require(len(mutations)==len(counter_updates),'Unsupported counter mutation in first source lifetime')
    require(not re.search(r'&\s*'+re.escape(counter)+r'\b',view[:reset.start()]),'Counter escapes its first source lifetime')
    mentions=list(re.finditer(r'\b'+re.escape(variable)+r'\b',view[:reset.end()]))
    require(len(mentions)==5,'Carrier escapes/is mutated/read elsewhere before later lifetime')
    counter_zero=list(re.finditer(r'\b'+re.escape(counter)+r'\s*=\s*(?:0|0x0+)\s*;',view[:initial.start()]))
    require(len(counter_zero)==1 and len(re.findall(r'\b'+re.escape(counter)+r'\b',view[:initial.start()]))==2,'Counter initial state is unknown/escaped/mutated')
    for assignment in (initial,update,reset,counter_zero[0]):
        prefix=view[:assignment.start()].rstrip();require(not prefix or prefix[-1] in ';{}','Carrier/counter assignment is not a standalone local write')
    prefix=view[:reads[0].start()].rstrip();require(not prefix or prefix[-1] not in '.>:*&','Integer subtraction is not the declared counter value')
    for a,b in (initial.span(),reads[0].span(),update.span()):require(not re.search(r'/\*|//',original[a:b]),'Comment in selected lifetime expression')
    return dict(carrier=variable,counter=counter,counter_kind=types[counter],sign_bit=1 if initial[2] else 0,initializer_span=[end+initial.start(),end+initial.end()],counter_zero_span=[end+counter_zero[0].start(),end+counter_zero[0].end()],read_span=[end+reads[0].start(),end+reads[0].end()],update_span=[end+update.start(),end+update.end()],later_lifetime_span=[end+reset.start(),end+reset.end()],comparison_limit=limit,counter_increment_count=len(counter_updates),native_constant_known=False,native_frame_known=False,source_emission_allowed=False)
