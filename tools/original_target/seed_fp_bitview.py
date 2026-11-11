"""Combine frozen source/native witnesses with automatic qualified alias closure."""
import hashlib
from seed_fp_native import X,require
from seed_fp_capture import discover_capture,caller_frame
from seed_fp_alias import PrefixMachine
from seed_fp_effects import direct_call
from seed_fp_virtual import virtual_call


def prove_raw_scalar(proofs,entry,text,seed):
    """Prove one raw two-DWORD scalar view, without emitting or scoring source.

    Qualified by valid normal MS i386 C++ pointer lifetime/call contracts,
    flat DS/ES/SS and clear DF. This is neither full-body acceptance nor a
    universal arbitrary-invalid-machine-input proof.
    """
    require(text==proofs.inventory.decompiled(entry),'Source is not the frozen inventory export')
    discovery=discover_capture(proofs,entry,text,seed)
    frame=caller_frame(proofs,entry)
    require(hashlib.sha256(frame['raw']).hexdigest()==discovery['original_sha256'],'Original changed between witnesses')
    machine=PrefixMachine(frame,entry,discovery['witness']['load'],discovery['capture'])
    def effect(state,ins):
        return direct_call(state,proofs,seed) if ins.operands[0].type==X.X86_OP_IMM else virtual_call(state,proofs,seed)
    aliases=machine.run(effect)
    require(aliases['native_alias_closure_complete'],'Missing qualified alias closure')
    return dict(entry=entry,frozen_source_sha256=hashlib.sha256(text.encode()).hexdigest(),discovery=discovery,alias_proof=aliases,qualified_guard_complete=True,source_emission_allowed=False,source_emitted=False,body_acceptance_claimed=False)


def rewrite_raw_scalar(proofs,entry,text,seed):
    """Emit one real local raw-bit view only after the combined guard passes."""
    import re
    proof=prove_raw_scalar(proofs,entry,text,seed)
    require(proof.get('qualified_guard_complete') and proof['frozen_source_sha256']==hashlib.sha256(text.encode()).hexdigest(),'Missing/current source proof')
    binding=proof['discovery']['source_binding'];low,high=binding['low'],binding['high'];start,end=binding['inner_view_span']
    pattern=r'\(\s*double\s*\)\s*CONCAT44\(\s*'+re.escape(high)+r'\s*,\s*'+re.escape(low)+r'\s*\)'
    require(re.fullmatch(pattern,text[start:end]) is not None,'Proven source span changed')
    header_end=seed.parse_header(text)[4][1];opening=re.match(r'\s*\{',seed.code_view(text[header_end:]))
    require(opening is not None,'No ordinary function body opening')
    insert=header_end+opening.end();require(insert<=start<end<=len(text),'Capture source coordinates escaped function')
    identifiers=set(re.findall(r'\b[A-Za-z_]\w*\b',seed.code_view(text)));name='captured_scalar_view';ordinal=0
    while name in identifiers:
        ordinal+=1;name='captured_scalar_view_%d'%ordinal
    declaration='\n    union { uint words[2]; double value; } '+name+';'
    expression='('+name+'.words[0] = (uint)'+low+', '+name+'.words[1] = (uint)'+high+', '+name+'.value)'
    result=text[:start]+expression+text[end:];result=result[:insert]+declaration+result[insert:]
    proof=dict(proof,source_emission_allowed=True,source_emitted=True,view_name=name,rewritten_source_sha256=hashlib.sha256(result.encode()).hexdigest())
    return result,proof


def _environment(repository,inventory):
    from pathlib import Path
    import catalog_io
    from verify_manifest import Audit
    from seed_fp_native import NativeProofs
    repository=Path(repository)
    audit=Audit(repository,'private/original-target-20261020/inputs/game.exe')
    catalog=catalog_io.load(catalog_io.locate(repository),references=False)
    return NativeProofs(repository,inventory,audit,catalog['regions'])


def optional_raw_scalar(entry,text,inventory,repository,parser):
    """Keep seeds byte-identical on absent/unknown evidence; no refusal churn."""
    from types import SimpleNamespace
    from seed_fp_source import VIEW
    from seed_fp_native import NativeRefusal
    if not VIEW.search(text):return text,[]
    seed=SimpleNamespace(**parser) if isinstance(parser,dict) else parser
    if not VIEW.search(seed.code_view(text)):return text,[]
    try:
        proofs=_environment(repository,inventory)
        result,proof=rewrite_raw_scalar(proofs,entry,text,seed)
    except (NativeRefusal,ValueError,OSError,KeyError):return text,[]
    return result,['//   Raw scalar word view verified against original frame and call effects.']
