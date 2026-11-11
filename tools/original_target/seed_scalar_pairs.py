"""Local scalar/x87 conversion pairs, independent of unrelated helper calls.

Scratch source recovery only. Every producer must have a bounded double store,
linear cleanup/conversion, known float(double) source and pinned helper ABI.
"""
import re,hashlib
import capstone
from seed_callee_abi import graph,require,AbiRefusal
from seed_scalar_fp import FP,INT,expression_type,scalar_signature,plain_return,immediate_call,helper_contract,_CONTEXTS
X=capstone.x86_const

def source_pairs(body,params,callee,helper,globals_,seed):
 view=seed.code_view(body);name='FUN_%08x'%callee;consumer='FUN_%08x'%helper;sites=list(seed.call_sites(body,name));require(sites,'No scalar source calls')
 rows,comma=seed.scalar_declarations(view);require(not comma,'Ambiguous comma declarations');declarations=[name for kind,name,array in rows]+[seed.param_parts(p)[1] for p in params];require(len(declarations)==len(set(declarations)),'Shadowed source declarations');types=seed.variable_types(body,params);require(not any(name in types and types[name]!=kind for name,kind in globals_.items()),'Conflicting floating globals');types.update(globals_);pairs=[]
 for start,end,args,unused in sites:
  require(len(args)==1 and view[:start].rstrip().endswith(('{',';')),'Producer is not one sibling floating expression')
  match=re.match(r'\s*;\s*([A-Za-z_]\w*)\s*=\s*'+consumer+r'\s*\(\s*\)\s*;',view[end:]);require(match is not None,'No adjacent conversion assignment');stop=end+match.end()
  require(not re.search(r'/\*|//',body[start:stop]),'Rewrite would remove a comment');require(declarations.count(match[1])==1 and types.get(match[1]) in INT-{'bool'},'Unknown integer destination');require(expression_type(args[0],types,seed) in FP,'Unknown floating input expression');pairs.append((start,stop,match[1]+' = (longlong)'+body[start:end]+';'))
 require(all(a[1]<=b[0] for a,b in zip(pairs,pairs[1:])),'Overlapping source pairs');return pairs

def native_pairs(audit,inventory,caller,callee,body_hash,helper,count):
 cn,ce,cp,raw=graph(audit,inventory,callee);require(hashlib.sha256(raw).hexdigest()==body_hash,'Accepted scalar native body differs');returns=[i for i in cn.values() if i.group(capstone.CS_GRP_RET)];require(returns and all(plain_return(i) for i in returns),'Unknown scalar cleanup');first=cn[callee];require(first.mnemonic=='fld' and len(first.operands)==1,'Unknown scalar entry');op=first.operands[0];require(op.type==X.X86_OP_MEM and op.size==8 and op.mem.base==X.X86_REG_ESP and op.mem.disp==4 and not op.mem.index and not op.mem.segment,'Scalar does not load double argument')
 nodes,edges,pred,unused=graph(audit,inventory,caller);calls=sorted((i for i in nodes.values() if immediate_call(i,callee)),key=lambda i:i.address);require(len(calls)==count and count>0,'Native/source producer counts differ');proof=[]
 for call in calls:
  cleanup=nodes.get(call.address+call.size);require(cleanup and pred[cleanup.address]=={call.address} and cleanup.mnemonic=='add' and cleanup.op_str=='esp, 8','Unknown/bypassed cleanup');consumer=nodes.get(cleanup.address+cleanup.size);require(consumer and immediate_call(consumer,helper) and pred[consumer.address]=={cleanup.address},'No unique adjacent native conversion');prefix=[];at=call.address
  for unused in range(128):
   require(len(pred[at])==1,'Ambiguous argument ingress');before=next(iter(pred[at]));ins=nodes[before];require(before+ins.size==at,'Nonlinear argument setup');prefix.append(ins)
   if ins.mnemonic=='sub' and ins.op_str=='esp, 8':break
   require(ins.mnemonic in {'mov','lea','movsx','movzx','imul','add','sub','and','or','xor','neg','shl','shr','sar','cdq','fld','fild','fstp','fadd','faddp','fmul','fmulp','fsub','fsubp','fsubr','fsubrp','fdiv','fdivp','fdivr','fdivrp','fchs'},'Unknown operation in argument setup');require(not {X.X86_REG_ESP,X.X86_REG_SP}.intersection(ins.regs_access()[1]),'Unexpected argument stack change');at=before
  else:raise AbiRefusal('Missing bounded argument allocation')
  stores=[]
  for ins in prefix[:-1]:
   for index,op in enumerate(ins.operands):
    if op.type!=X.X86_OP_MEM or not (op.access&capstone.CS_AC_WRITE or ins.mnemonic=='fstp' and index==0):continue
    require(op.mem.base==X.X86_REG_ESP and not op.mem.index and not op.mem.segment and op.mem.disp>=0,'Unknown argument-memory alias')
    if op.mem.disp<8:require(ins.mnemonic=='fstp' and op.mem.disp==0 and op.size==8,'Partial/overlapping double argument');stores.append(ins.address)
  require(len(stores)==1,'Missing/repeated double argument store');proof.append(dict(producer=call.address,consumer=consumer.address,allocation=prefix[-1].address,argument_store=stores[0],cleanup=cleanup.address))
 return proof

def recover_pairs(repository,inventory,caller,callee,body,params,prototype,accepted,seed):
 ret,conv,old_params=prototype
 if callee not in accepted or ret.strip() not in FP or conv not in ('','__cdecl') or len(old_params)!=2 or any(seed.param_parts(p)[0] not in INT for p in old_params):return None
 evidence=(repository/'private/original-target-20261020/inventory').resolve();directory=next((p for p in inventory.passes if (p/'decompiled'/('%08x.c'%caller)).is_file()),None)
 if directory is None or not directory.resolve().is_relative_to(evidence):return None
 try:
  key=repository.resolve()
  if key not in _CONTEXTS:
   import catalog_io
   from verify_manifest import Audit
   cat=catalog_io.load(catalog_io.locate(repository),references=False);rows={}
   for row in cat['regions']:rows.setdefault(row['entry'],[]).append(row)
   _CONTEXTS[key]=(rows,Audit(repository,'private/original-target-20261020/inputs/game.exe'),cat)
  rows,audit,cat=_CONTEXTS[key];require(len(rows.get(callee,[]))==1,'Ambiguous accepted scalar');row=rows[callee][0];require(row['source']==accepted[callee] and row['source'].startswith('src/recovered/'),'Noncanonical scalar source');path=repository/row['source'];require(path.resolve().is_relative_to((repository/'src/recovered').resolve()),'Escaped scalar source');require(hashlib.sha256(path.read_bytes()).hexdigest()==row['source_sha256'],'Accepted source hash differs');scalar_signature(path.read_text(encoding='utf-8'),row['symbol'],seed)
  from diff_candidate import known_references
  helper=known_references(cat).get('__ftol');require(helper is not None,'Unknown conversion helper');globals_=seed.floating_operand_globals(body,inventory.function(caller)[1],params);pairs=source_pairs(body,params,callee,helper,globals_,seed);proof=native_pairs(audit,inventory,caller,callee,row['body_sha256'],helper,len(pairs));helper_hash=helper_contract(repository,audit,inventory,helper)
 except (AbiRefusal,OSError,KeyError,ValueError,TypeError):return None
 for first,last,replacement in reversed(pairs):body=body[:first]+replacement+body[last:]
 return dict(body=body,prototype=(ret,'__cdecl',['double input']),proof=dict(pairs=proof,helper_sha256=helper_hash),note='// Native double argument and adjacent ST0 conversions verified per call; unrelated conversion calls preserved.')
