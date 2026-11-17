"""Generate an explicitly provisional scratch integer-lifetime hypothesis.

The seeder's public contract is hypothesis generation, never recovered source.
Native value/control and source-scope evidence gate this hypothesis. Alias
closure remains a separate, stronger proof; no acceptance is granted here.
"""
import hashlib,re
from seed_guarded_cfg import guarded_graph,X,require
from seed_frame_equations import AffineFrames
from seed_integer_scope import bind_integer_scope
from seed_integer_native import bind_native_integer


def discover_integer_candidate(proofs,entry,text,seed,header_pins,library=None,archives=None):
    require(text==proofs.inventory.decompiled(entry),'Source is not the frozen inventory export')
    binding=bind_integer_scope(text,seed);graph=guarded_graph(proofs.audit,proofs.inventory,entry);known={};normal=set()
    for at,ins in graph['nodes'].items():
        if ins.mnemonic!='call' or ins.operands[0].type!=X.X86_OP_IMM:continue
        target=ins.operands[0].imm&0xffffffff
        try:
            child=guarded_graph(proofs.audit,proofs.inventory,target)
            pops={i.operands[0].imm if i.operands else 0 for i in child['nodes'].values() if i.mnemonic=='ret'}
            if len(pops)==1:known[at]=next(iter(pops))
        except (ValueError,KeyError):pass
        if target in proofs.rows:
            try:
                proofs.provenance(target);row=proofs.rows[target];source=proofs._source_bytes(row['source'],row['source_sha256']).decode('utf-8')
                require(not re.search(r'\b(?:__asm|_asm|naked)\b',seed.code_view(source)),'Nonordinary source ABI');normal.add(at)
            except (ValueError,KeyError):pass
        if at not in normal and library is not None:
            from seed_library_abi import normal_c_runtime_abi
            try:normal_c_runtime_abi(proofs,target,library,archives or {});normal.add(at)
            except (ValueError,KeyError,OSError):pass
    from seed_global_leaf_abi import global_leaf_cleanup
    for at,ins in graph['nodes'].items():
        if ins.mnemonic!='call' or ins.operands[0].type==X.X86_OP_IMM:continue
        try:contract=global_leaf_cleanup(proofs,graph,at,seed,header_pins);known[at]=contract['pop'];normal.add(at)
        except (ValueError,KeyError,OSError):pass
    frames=AffineFrames(graph,known);native=bind_native_integer(graph,frames,binding,normal)
    return dict(entry=entry,frozen_source_sha256=hashlib.sha256(text.encode()).hexdigest(),native_body_sha256=graph['body_sha256'],source=binding,native=native,candidate_hypothesis_only=True,scratch_candidate_emission_allowed=True,alias_effect_closure_complete=False,accepted_source_emission_allowed=False,body_acceptance_claimed=False)


def emit_integer_candidate(text,proof,seed):
    require(proof.get('candidate_hypothesis_only') is True and proof.get('scratch_candidate_emission_allowed') is True,'Missing scratch hypothesis proof')
    require(proof.get('body_acceptance_claimed') is False and proof.get('accepted_source_emission_allowed') is False,'Scratch emitter cannot grant source acceptance')
    require(proof['frozen_source_sha256']==hashlib.sha256(text.encode()).hexdigest(),'Frozen candidate source changed')
    source=bind_integer_scope(text,seed);require(source==proof['source'],'Source binding changed');native=proof['native']
    require(native.get('native_source_roles_verified') is True,'Missing native value/control roles')
    word=native['word'];require(isinstance(word,int) and 0<=word<=0xffffffff and word&0x7f800000==0x7f800000 and word&0x7fffff and word>>31==source['sign_bit'],'Native word is not the signed NaN-labelled artifact')
    value=word-(1<<32) if word>>31 else word;require(value==native['signed_value'],'Native word/signed value disagree')
    identifiers=set(re.findall(r'\b[A-Za-z_]\w*\b',seed.code_view(text)));stem='integer_cache_%08x'%native['initializer'];name=stem;index=0
    while name in identifiers:index+=1;name=stem+'_%d'%index
    header_end=seed.parse_header(text)[4][1];opening=re.match(r'\s*\{',seed.code_view(text[header_end:]));require(opening is not None,'No ordinary candidate body')
    insert=header_end+opening.end();counter=source['counter']
    edits=[(*source['initializer_span'],name+' = '+str(value)+';'),(*source['read_span'],'(int)((uint)'+counter+' - (uint)'+name+')'),(*source['update_span'],name+' = '+counter+';'),(insert,insert,'\n    int '+name+';')]
    require(all(insert<=a<=b<=len(text) for a,b,replacement in edits),'Candidate edit escaped function')
    ordered=sorted(edits);require(all(left[1]<=right[0] for left,right in zip(ordered,ordered[1:])),'Candidate edits overlap')
    result=text
    for a,b,replacement in sorted(edits,reverse=True):result=result[:a]+replacement+result[b:]
    report=dict(proof,integer_name=name,rewritten_source_sha256=hashlib.sha256(result.encode()).hexdigest(),source_emitted_as_scratch_hypothesis=True,alias_effect_closure_complete=False,accepted_source_emission_allowed=False,body_acceptance_claimed=False)
    return result,report


def draft_integer_candidate(proofs,entry,text,seed,header_pins,library=None,archives=None):
    proof=discover_integer_candidate(proofs,entry,text,seed,header_pins,library,archives)
    return emit_integer_candidate(text,proof,seed)


def _environment(repository,inventory):
    from pathlib import Path
    import json,os
    import catalog_io
    from verify_manifest import Audit
    from seed_fp_native import NativeProofs
    repository=Path(repository)
    config_path=Path(os.environ.get('RA2_CONFIG',str(repository/'private/original-target.local.json')))
    if not config_path.is_absolute():config_path=repository/config_path
    config=json.loads(config_path.read_text())
    audit=Audit(repository,config['executable']);catalog=catalog_io.load(catalog_io.locate(repository),references=False)
    proofs=NativeProofs(repository,inventory,audit,catalog['regions']);pins={row['path']:row['sha256'] for row in catalog['headers']}
    library=json.loads((repository/'manifests/original_target/library.json').read_text());archives={}
    expected={row['archive_sha256'] for row in library['regions'] if row.get('archive')=='LIBCMT.LIB' and row.get('archive_role')=='VC6 SP3'}
    # The actual archive hash in each function row remains the decisive pin.
    directories=list(config.get('libraries',[]))+[repository/'private/original-target-20261020/toolchain/media-sp3/VC98/LIB',repository/'private/original-target-20261020/toolchain/media/VC98/LIB']
    for directory in directories:
        directory=Path(directory)
        if not directory.is_absolute():directory=repository/directory
        path=directory/'LIBCMT.LIB'
        if path.is_file() and hashlib.sha256(path.read_bytes()).hexdigest() in expected:archives['LIBCMT.LIB']=path;break
    return proofs,pins,library,archives


def optional_integer_candidate(entry,text,inventory,repository,parser):
    """Generate a seed hypothesis only; unavailable evidence leaves bytes alone."""
    from types import SimpleNamespace
    if not re.search(r'\bNAN\b',text):return text,[]
    seed=SimpleNamespace(**parser) if isinstance(parser,dict) else parser
    try:
        bind_integer_scope(text,seed)
        proofs,pins,library,archives=_environment(repository,inventory)
        result,proof=draft_integer_candidate(proofs,entry,text,seed,pins,library,archives)
    except (ValueError,OSError,KeyError):return text,[]
    return result,['//   Scratch hypothesis: integer-first cache from native value/control roles; alias closure unproved.']
