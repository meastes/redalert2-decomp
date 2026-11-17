"""Native capture witness for a marked, unaccepted scratch hypothesis.

The original graph and accepted ABI sources must all be independently pinned.
Value-root correspondence and storage-byte identity are proved separately.
Virtual object memory effects and full source equivalence remain unproved.
"""
import hashlib
from seed_guarded_cfg import X,PARTS,require,guarded_graph
from seed_capture_receivers import Receivers,input_contract,SEGMENTS


def frame_operand(op,size=None):
    return op.type==X.X86_OP_MEM and op.mem.base==X.X86_REG_ESP and not op.mem.index and op.mem.segment in SEGMENTS and (size is None or op.size==size)


def linear_next(graph,at):
    ins=graph['nodes'][at];targets=graph['edges'][at]
    require(len(targets)==1 and targets[0]==at+ins.size and graph['pred'][targets[0]]=={at},'Capture path branches or is bypassed')
    return targets[0]


def same_bytes(frames,left,lop,right,rop):
    result=frames.address(left,lop.mem.disp,relative_to=right,relative_displacement=rop.mem.disp)
    return result['proved'] and result['constant']=='0'


def source_frozen(body,params,inventory,entry,seed):
    text=inventory.decompiled(entry);ret,conv,name,original_params,span=seed.parse_header(text)
    conv,original_params=seed.fit_convention(conv,original_params,seed.stack_cleanup(inventory.function(entry)[1]),conv=='__thiscall')
    normalized=seed.nonnegative_tests(seed.reindent(text[span[1]:].strip()))
    require(body==normalized and params==original_params,'Capture source differs from current frozen export')
    return dict(export_sha256=hashlib.sha256(text.encode()).hexdigest(),source_body_sha256=hashlib.sha256(body.encode()).hexdigest())


def hidden_result_buffer(graph,frames,call,capture,capture_op):
    cursor=call;push=None
    for unused in range(16):
        require(len(graph['pred'][cursor])==1,'Ambiguous hidden-buffer argument ingress');parent=next(iter(graph['pred'][cursor]));ins=graph['nodes'][parent]
        require(parent+ins.size==cursor and graph['edges'][parent]==[cursor],'Nonlinear hidden-buffer setup')
        if ins.mnemonic=='push':push=ins;break
        require(ins.mnemonic not in ('call','pop','ret') and not {X.X86_REG_ESP,X.X86_REG_SP}.intersection(ins.regs_access()[1]),'Unmodeled hidden-buffer argument setup');cursor=parent
    require(push is not None and len(push.operands)==1 and push.operands[0].type==X.X86_OP_REG and push.operands[0].size==4,'No unique pushed result-buffer register');reg=push.operands[0].reg;cursor=push.address
    for unused in range(16):
        require(len(graph['pred'][cursor])==1,'Ambiguous hidden-buffer register ingress');parent=next(iter(graph['pred'][cursor]));ins=graph['nodes'][parent]
        require(parent+ins.size==cursor and graph['edges'][parent]==[cursor] and ins.mnemonic not in ('call','ret'),'Nonlinear/call-clobbered hidden-buffer register')
        writes={PARTS.get(r,r) for r in ins.regs_access()[1]}
        if reg in writes:
            require(ins.mnemonic=='lea' and len(ins.operands)==2 and ins.operands[0].reg==reg and ins.operands[0].size==4 and frame_operand(ins.operands[1]),'Hidden result buffer is not a known frame address')
            relative=frames.address(parent,ins.operands[1].mem.disp,relative_to=capture,relative_displacement=capture_op.mem.disp)
            require(relative['proved'] and int(relative['constant'])>=8,'Hidden result starts inside/below the captured double')
            return dict(call=call,push=push.address,address=parent,relative_to_capture=int(relative['constant']),qualified_by_nonnegative_normal_record_output_offsets=True)
        cursor=parent
    require(False,'No bounded hidden result-buffer origin')


def capture_candidate(graph,frames,incoming,machine,binding,at):
    nodes=graph['nodes'];load=nodes[at];op=load.operands[0]
    require(load.mnemonic=='fild' and frame_operand(op,4),'Not a signed DWORD frame load')
    # Find the unique full DWORD reaching store without crossing any possible
    # write, call, branch, or alternate predecessor. No operand is fabricated.
    cursor=at;store=None
    for unused in range(16):
        require(len(graph['pred'][cursor])==1,'Ambiguous integer spill ingress');parent=next(iter(graph['pred'][cursor]));ins=nodes[parent]
        require(parent+ins.size==cursor and graph['edges'][parent]==[cursor],'Nonlinear integer spill ingress')
        require(ins.mnemonic not in ('call','ret') and not ins.mnemonic.startswith('f'),'Unmodeled integer spill effect')
        writes=[o for o in ins.operands if o.type==X.X86_OP_MEM and o.access&2]
        for write in writes:
            require(frame_operand(write),'Unknown integer spill alias')
            relative=frames.address(parent,write.mem.disp,relative_to=at,relative_displacement=op.mem.disp);require(relative['proved'],'Unresolved integer spill alias');offset=int(relative['constant'])
            if offset<4 and offset+write.size>0:
                require(ins.mnemonic=='mov' and write.size==4 and offset==0 and len(ins.operands)==2 and ins.operands[1].type==X.X86_OP_REG and ins.operands[1].size==4,'Partial/unknown integer spill definition');store=ins;break
        if store:break
        cursor=parent
    require(store is not None,'No bounded integer spill definition')
    value=incoming[store.address].get(store.operands[1].reg)
    require(value and value[0]=='difference','Integer spill is not a returned-coordinate difference')
    getter_calls=[]
    for word,getter in zip(value[2:],binding['source_getters']):
        require(word[0]=='word' and word[2]==0 and word[1][0]=='return','Nonzero/unknown coordinate word')
        returned=word[1];root=returned[2]
        require(root[0]=='input' and root[1]==getter['parameter_index'] and returned[3]*4==getter['slot'],'Native/source getter roots or slots differ');getter_calls.append(returned[1])
    require(getter_calls[0]<getter_calls[1]<value[1]<=store.address<at,'Native getter/difference order differs')
    # The FILD result must be the next x87 value stored, unchanged as double.
    cursor=linear_next(graph,at);capture=None
    for unused in range(12):
        ins=nodes[cursor]
        if ins.mnemonic.startswith('f'):
            require(ins.mnemonic=='fstp' and len(ins.operands)==1 and frame_operand(ins.operands[0],8),'FILD value is modified or not stored as double');capture=ins;break
        require(ins.mnemonic not in ('call','ret'),'Call crossed before double capture');cursor=linear_next(graph,cursor)
    require(capture is not None,'No bounded double capture');capture_op=capture.operands[0]
    cursor=linear_next(graph,capture.address);calls=[];buffers=[];reload=None
    for unused in range(96):
        ins=nodes[cursor]
        if ins.mnemonic=='fld' and len(ins.operands)==1 and frame_operand(ins.operands[0],8) and same_bytes(frames,cursor,ins.operands[0],capture.address,capture_op):
            multiply=nodes[linear_next(graph,cursor)]
            require(multiply.mnemonic=='fmul' and len(multiply.operands)==1 and frame_operand(multiply.operands[0],8) and same_bytes(frames,multiply.address,multiply.operands[0],capture.address,capture_op),'Captured reload is not the same-byte square');reload=ins;break
        if ins.mnemonic=='call':
            family=machine.families.get(cursor);require(family and family['record_result'] and family['pop']==4,'Unknown call between capture and reload');calls.append(cursor);buffers.append(hidden_result_buffer(graph,frames,cursor,capture.address,capture_op))
        if ins.mnemonic in ('push','call'):
            address=frames.address(cursor,-4,relative_to=capture.address,relative_displacement=capture_op.mem.disp);require(address['proved'],'Unknown implicit push/call write');offset=int(address['constant']);require(offset>=8 or offset+4<=0,'Implicit push/call overwrites captured double')
        for write in ins.operands:
            if write.type!=X.X86_OP_MEM or not write.access&2:continue
            require(frame_operand(write),'Unknown explicit capture alias');relative=frames.address(cursor,write.mem.disp,relative_to=capture.address,relative_displacement=capture_op.mem.disp);require(relative['proved'],'Unknown capture/write byte relation');offset=int(relative['constant']);require(offset>=8 or offset+write.size<=0,'Double capture explicitly overwritten')
        require(ins.mnemonic!='ret','Capture path returns before reload');cursor=linear_next(graph,cursor)
    require(reload is not None,'No bounded same-byte double square reload')
    # Associate the square with the source-named, accepted scalar call using
    # the actual native instruction path. Other unrelated scalar sites remain.
    cursor=linear_next(graph,reload.address);scalar=None
    for unused in range(24):
        ins=nodes[cursor]
        if ins.mnemonic=='call':
            require(len(ins.operands)==1 and ins.operands[0].type==X.X86_OP_IMM and ins.operands[0].imm&0xffffffff==binding['scalar_callee'],'Square feeds a different/unknown call');scalar=ins;break
        require(ins.mnemonic!='ret','Square path returns before scalar call');cursor=linear_next(graph,cursor)
    require(scalar is not None,'No bounded scalar consumer');callee=machine.accepted(binding['scalar_callee'])
    return dict(integer_store=store.address,signed_dword_load=at,difference=value[1],getter_calls=getter_calls,capture_store=capture.address,capture_displacement=capture_op.mem.disp,reload=reload.address,reload_displacement=reload.operands[0].mem.disp,scalar_call=scalar.address,scalar_source=callee['source'],scalar_source_sha256=callee['source_sha256'],scalar_body_sha256=callee['body_sha256'],intervening_record_calls=calls,hidden_result_buffers=buffers)


def qualify_capture(repository,catalog,audit,inventory,entry,body,params,binding,seed):
    frozen=source_frozen(body,params,inventory,entry,seed)
    require(frozen['source_body_sha256']==binding['source_body_sha256'],'Native/source binding is stale')
    contract=input_contract(repository,catalog,audit,inventory,entry,params,seed);graph=guarded_graph(audit,inventory,entry)
    require(not any(ins.mnemonic in ('fild','fist','fistp','fisttp') and any(o.type==X.X86_OP_MEM and o.size==8 for o in ins.operands) for ins in graph['nodes'].values()),'Mixed QWORD integer/opaque FP storage refuses')
    machine=Receivers(repository,catalog,audit,inventory,graph,contract);incoming,frames,steps=machine.solve();matches=[];refusals={}
    for at,ins in graph['nodes'].items():
        if ins.mnemonic!='fild' or len(ins.operands)!=1 or not frame_operand(ins.operands[0],4):continue
        try:matches.append(capture_candidate(graph,frames,incoming,machine,binding,at))
        except ValueError as error:refusals['%08x'%at]=str(error)
    require(len(matches)==1,'No unique qualified native signed-DWORD/double capture')
    return dict(frozen,original_body_sha256=graph['body_sha256'],witness=matches[0],contract=contract,receiver_families={('%08x'%at):family for at,family in machine.families.items()},known_native_cleanups={('%08x'%at):pop for at,pop in machine.known.items()},signed_dword_capture_verified=True,same_eight_byte_reload_verified=True,source_roots_correspond=True,frozen_source_verified=True,source_emission_allowed=False,alias_closure_claimed=False,complete_source_correspondence_claimed=False,body_acceptance_claimed=False,refusals=refusals)


def optional_capture(body,params,entry,inventory,repository,seed):
    from pathlib import Path
    from seed_capture_source import bind_source,rewrite_capture
    try:binding=bind_source(body,params,seed)
    except ValueError:return body,[]
    repository=Path(repository);evidence=(repository/'private/original-target-20261020/inventory').resolve()
    directory=next((p for p in getattr(inventory,'passes',()) if (p/'decompiled'/('%08x.c'%entry)).is_file()),None)
    if directory is None or not directory.resolve().is_relative_to(evidence):return body,[]
    try:
        import catalog_io
        from verify_manifest import Audit
        catalog=catalog_io.load(catalog_io.locate(repository),references=False)
        audit=Audit(repository,'private/original-target-20261020/inputs/game.exe')
        proof=qualify_capture(repository,catalog,audit,inventory,entry,body,params,binding,seed)
        return rewrite_capture(body,binding,proof,seed)
    except (ValueError,KeyError,OSError,TypeError):return body,[]
