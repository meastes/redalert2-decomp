"""Narrow ABI provenance for identified compiler C-runtime functions.

A matched archive/symbol is not a semantic allocation/read/write summary.
In particular, identical-section name assignments may be ambiguous. Only the
normal MS i386 cdecl nonvolatile-register contract is exported here.
"""
from pathlib import Path
import hashlib
import re
from seed_guarded_cfg import guarded_graph,require


def normal_c_runtime_abi(proofs,entry,ledger,archives):
    rows=[row for row in ledger['regions'] if int(row['entry'],16)==entry]
    require(len(rows)==1,'Missing/ambiguous linked library function provenance');row=rows[0]
    require(row.get('archive')=='LIBCMT.LIB' and row.get('archive_role')=='VC6 SP3','Not the pinned compiler C-runtime domain')
    require(row.get('section')=='.text' and row.get('symbols') and all(re.fullmatch(r'_[A-Za-z_]\w*',symbol) for symbol in row['symbols']),'Not an ordinary cdecl code symbol')
    pins=[pin for pin in ledger['archives'] if pin['name']==row['archive'] and pin['sha256']==row['archive_sha256']]
    require(len(pins)==1 and row['archive'] in archives,'Missing matching C-runtime archive pin')
    path=Path(archives[row['archive']]);require(path.is_file(),'Pinned C-runtime archive is unavailable')
    require(hashlib.sha256(path.read_bytes()).hexdigest()==row['archive_sha256'],'C-runtime archive hash differs')
    from library_identity import archive_members
    from coff import Coff
    import struct
    members=enumerate(archive_members(path));selected=next(((name,obj) for index,(name,obj) in members if index==row['member_index']),None)
    require(selected is not None and selected[0].split('\\')[-1]==row['member'],'Pinned library member identity differs')
    member,obj=selected;coff=Coff(obj)
    compilers=[symbol['value'] for symbol in coff.symbols.values() if symbol['name']=='@comp.id']
    require(len(compilers)==1 and compilers[0]>>16 in (10,11) and (compilers[0]&65535) in (8168,8447),'Not an evidenced VC6 C/C++ compiler object')
    definitions={symbol['name'] for symbol in coff.symbols.values() if symbol['type']&0x20 and symbol['storage']==2 and symbol['section']>0}
    require(set(row['symbols'])<=definitions,'Library symbol is not an external COFF function')
    sections=[(index,section) for index,section in enumerate(coff.sections) if section['flags']&0x20 and section['data']]
    require(0<=row['section_index']<len(sections),'Unknown library code section ordinal')
    index,section=sections[row['section_index']];source=section['data']
    symbols=[symbol['name'] for symbol in coff.symbols.values() if symbol['section']==index+1 and symbol['storage']==2 and symbol['value']==0]
    require(section['name']==row['section'] and len(source)==row['bytes'] and set(symbols)==set(row['symbols']),'Library producer section differs')
    patches=row.get('relocations',[])
    require(sorted((offset,typ) for offset,symbol,typ in section['relocs'])==sorted((patch['offset'],patch['type']) for patch in patches),'Library relocation set/type differs')
    linked=bytearray(source)
    for patch in patches:
        offset=patch['offset'];require(patch['type'] in (6,20) and 0<=offset and offset+4<=len(linked),'Unsupported library relocation')
        linked[offset:offset+4]=struct.pack('<I',patch['resolved']&0xffffffff)
    graph=guarded_graph(proofs.audit,proofs.inventory,entry)
    require(len(graph['raw'])==row['bytes'] and hashlib.sha256(graph['raw']).hexdigest()==row['sha256'],'Library native body/extent differs')
    require(bytes(linked)==graph['raw'],'Fully relocated library producer bytes differ')
    from seed_guarded_cfg import X,PARTS
    saved={X.X86_REG_EBX,X.X86_REG_EBP,X.X86_REG_ESI,X.X86_REG_EDI}
    require(not any(saved.intersection(PARTS.get(reg,reg) for reg in ins.regs_access()[1]) for ins in graph['nodes'].values()),'Library ABI thin wrapper directly changes a nonvolatile register')
    from seed_frame_equations import AffineFrames
    frame=AffineFrames(graph,{})
    for at,ins in graph['nodes'].items():
        if ins.mnemonic=='ret':require(frame.height(at)==0,'C-runtime return stack is not normal')
    returns=[ins for ins in graph['nodes'].values() if ins.mnemonic=='ret']
    require(returns and all(not ins.operands or ins.operands[0].imm==0 for ins in returns),'C-runtime function is not callee-cleanup zero')
    return dict(entry=entry,archive=row['archive'],archive_sha256=row['archive_sha256'],member=row['member'],symbols=row['symbols'],native_body_sha256=row['sha256'],coff_compiler_id=compilers[0],coff_external_function=True,fully_relocated_producer_exact=True,direct_nonvolatile_writes=False,normal_ms_i386_cdecl_abi=True,nonvolatile_registers_preserved=True,name_assignment=row.get('assignment'),memory_effects_proved=False,allocation_semantics_inferred=False)
