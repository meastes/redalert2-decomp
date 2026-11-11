"""Read-only source witness for one two-word scalar FP capture.
No native fact is inferred from a Ghidra local's name or suffix.
"""
import re
from seed_fp_native import NativeRefusal
WORD32={'undefined4','uint','int','unsigned int','signed int','long','unsigned long','DWORD','ulong'}
VIEW=re.compile(r'\(\s*float\s*\)\s*(?P<inner>\(\s*double\s*\)\s*CONCAT44\(\s*(?P<high>[A-Za-z_]\w*)\s*,\s*(?P<low>[A-Za-z_]\w*)\s*\))')
def need(condition,reason):
 if not condition:raise NativeRefusal(reason)

def bind_capture(text,callee_name,seed):
 ret,convention,name,params,(start,end)=seed.parse_header(text)
 need(convention in ('__fastcall','__thiscall') and len(params)==1,'Unknown incoming ECX source parameter')
 root_kind,root=seed.param_parts(params[0]);need('*' in root_kind or root_kind in WORD32,'Incoming root is not a known DWORD value')
 original=text[end:];body=seed.code_view(original);types=seed.variable_types(body,params);declarations=[n for t,n,a in seed.LOCAL.findall(body) if t.strip().split()[0] not in ('return','goto','delete','throw','break','continue')];need(len(declarations)==len(set(declarations)),'Shadowed local declarations');need(root not in declarations,'Incoming root parameter is shadowed')
 sites=list(seed.call_sites(body,callee_name));witnesses=[]
 for ordinal,(at,stop,args,unused) in enumerate(sites):
  if len(args)!=1:continue
  match=VIEW.fullmatch(args[0].strip())
  if match:witnesses.append((ordinal,at,stop,args[0],match))
 need(len(witnesses)==1,'Missing or ambiguous raw floating argument')
 ordinal,call_start,call_end,argument,match=witnesses[0];high,low=match['high'],match['low'];need(high!=low and root not in (high,low),'Overlapping capture names')
 need(types.get(high) in WORD32 and types.get(low) in WORD32,'Capture storage is not exactly two DWORD scalars')
 need(declarations.count(high)==declarations.count(low)==1,'Capture declarations are not unique locals')
 need(not re.search(r'/\*|//',original[call_start:call_end]),'Comment in selected argument')
 word=r'(?:undefined4|uint|int|unsigned\s+int|signed\s+int|long|unsigned\s+long|DWORD|ulong)'
 low_re=re.compile(r'\b'+re.escape(low)+r'\s*=\s*\*\s*\(\s*'+word+r'\s*\*\s*\)\s*'+re.escape(root)+r'\s*;')
 high_re=re.compile(r'\b'+re.escape(high)+r'\s*=\s*\*\s*\(\s*'+word+r'\s*\*\s*\)\s*\(\s*\(\s*(?:int|uint|unsigned\s+int)\s*\)\s*'+re.escape(root)+r'\s*\+\s*(?:4|0x0*4)\s*\)\s*;')
 low_init=list(low_re.finditer(body));high_init=list(high_re.finditer(body));need(len(low_init)==len(high_init)==1,'Missing exact byte-adjacent root word captures')
 a,b=low_init[0],high_init[0];need(a.start()<b.start()<call_start,'Capture read order or source lifetime differs')
 prefix=body[:b.end()]
 need(not re.search(r'\b(?:if|for|while|switch|goto|return|throw|try|catch|__asm|asm|code)\b',prefix),'Capture prefix is conditional or unverified')
 need(not re.search(r'\b[A-Za-z_]\w*\s*\(',prefix),'Call before capture completes')
 need(not re.search(r'\b'+re.escape(root)+r'\s*(?:\+\+|--)|(?:\+\+|--)\s*\b'+re.escape(root)+r'\b',prefix),'Incoming root mutation before capture')
 for statement in prefix.split(';'):
  if '=' in statement:
   parts=statement.strip().lstrip('{').strip().split('=');need(len(parts)==2 and re.fullmatch(r'[A-Za-z_]\w*',parts[0].strip()) and parts[0].strip()!=root,'Non-local/root write before capture completes')
 # Declaration, initialization and the selected read only. A later address use
 # begins another lifetime and is retained; it cannot affect this earlier read.
 for variable in (low,high):
  mentions=list(re.finditer(r'\b'+re.escape(variable)+r'\b',body[:call_end]));need(len(mentions)==3,'Capture read, mutation or address escape before use')
 arg_at=body.find(argument,call_start,call_end);need(arg_at>=0,'Ambiguous argument coordinates')
 return dict(root=root,root_kind=root_kind,low=low,high=high,root_byte_offsets=[0,4],callee_name=callee_name,source_call_count=len(sites),source_call_ordinal=ordinal,low_initializer=[a.start(),a.end()],high_initializer=[b.start(),b.end()],inner_view_span=[end+arg_at+match.start('inner'),end+arg_at+match.end('inner')],field_capture_lifetime_end=end+call_end)
