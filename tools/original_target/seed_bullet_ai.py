"""Native-checked scratch repair for the frozen Bullet AI seed shape.

The post-render source contains locals reused as unrelated numeric values.
This binding recognizes one complete seed revision. The generated text is a
draft and the proof does not replace strict candidate or whole-file checks.
"""
import hashlib
import re
import struct
from pathlib import Path

from seed_callee_abi import reachable
from seed_capture_receivers import Receivers
from seed_fp_virtual import physical_family
from seed_frame_equations import aligned_frames
from seed_guarded_cfg import CFGRefusal, X, guarded_graph, require


ENTRY = 0x45f710
SEED_SHA256 = "898017dd0b0937e4e36c0fc0d20e6218172200057896888e2de882d7d0f97a25"
BODY_SHA256 = "97d12c5a8f9b4b124cf799d7b9a3580c8ff25fb7956de5475dee07e6ea3fd7d1"


def verify_native(repository, inventory):
    from verify_manifest import Audit
    import catalog_io

    audit = Audit(repository, "private/original-target-20261020/inputs/game.exe")
    graph = guarded_graph(audit, inventory, ENTRY)
    require(graph["body_sha256"] == BODY_SHA256, "Frozen Bullet extent differs")
    catalog = catalog_io.load(catalog_io.locate(repository), references=False)
    receivers = Receivers(repository, catalog, audit, inventory, graph, None)
    signatures = {
        0x437690: ("?Set_00437690@Vector3@@", 12),
        0x4376b0: ("?Scale_004376b0@Vector3@@", 4),
        0x590cd0: ("?Rotate_Vector@Matrix3D@@", 8),
        0x41b780: ("?Length@UnknownVector3@@", 0),
        0x4ba410: ("?Fast_Sqrt@@", 0),
        0x41b630: ("??0UnknownVector3@@", 12),
        0x434820: ("?Set_00434820@CoordStruct@@", 12),
        0x4642a0: ("??Y?$TVelocity3D@N@@", 4),
    }
    for address, (prefix, width) in signatures.items():
        require(receivers.accepted(address)["symbol"].startswith(prefix) and
                receivers.cleanup(address) == width, "Pinned Bullet helper ABI differs")

    initializer = receivers.accepted(0x6ec790)
    raw = audit.original(0x6ec790, initializer["body_bytes"])
    require(any(ins.mnemonic == "mov" and ins.op_str == "dword ptr [0xac0bf8], 0x7a177c"
                for ins in audit.decoder.disasm(raw, 0x6ec790)), "Bullet vector table initializer differs")
    index_target, = struct.unpack("<I", audit.original(0x7a177c + 0x10, 4))
    require(receivers.accepted(index_target)["symbol"].startswith("?ID@") and
            receivers.cleanup(index_target) == 4, "Bullet vector lookup ABI differs")
    targets = {table["target"] for table in physical_family(audit, "BulletClass", 60)}
    require(targets and all(receivers.accepted(target) and receivers.cleanup(target) == 0
                            for target in targets), "Bullet slot 60 ABI differs")

    known = {0x45f75e: 4, 0x45f7a8: 0}
    for address, ins in graph["nodes"].items():
        if ins.mnemonic == "call" and ins.operands[0].type == X.X86_OP_IMM:
            try:
                known[address] = receivers.cleanup(ins.operands[0].imm)
            except CFGRefusal:
                pass
    frames = aligned_frames(graph, known)
    slots = ((0x45fef8, 0, -424), (0x45ff03, 0, -424), (0x45fd2d, 0, -448),
             (0x45fd32, 0, -424), (0x45fd29, 0x3c, -388), (0x45fd22, 0xa8, -280),
             (0x45fd1c, 0x40, -376))
    require(all(frames.height(at) + disp == offset for at, disp, offset in slots),
            "Bullet Length or projectile argument homes differ")
    verify_height_receivers(graph, frames)
    for address in (0x45fbd7, 0x45fc22):
        operand = graph["nodes"][address].operands[0]
        require(operand.type == X.X86_OP_MEM and operand.mem.base == X.X86_REG_ESP and
                not operand.mem.index and all(frames.disjoint_addresses(
                    address, operand.mem.disp, operand.size, 0x45fef8, slot, 4)
                    for slot in (0x2c, 0x38, 0x4c)), "Bullet signed Z source is ambiguous")

    return graph, known, receivers


def verify_height_receivers(graph, frames):
    """Bind the two signed coordinate outputs surrounding the velocity add."""
    nodes = graph["nodes"]
    for site, target in ((0x4603f2, 0x41b630), (0x460403, 0x4642a0),
                         (0x460430, 0x434820), (0x46043f, 0x55a280),
                         (0x46045f, 0x548000), (0x46048a, 0x548000)):
        call = nodes[site]
        require(call.mnemonic == "call" and call.operands[0].type == X.X86_OP_IMM and
                call.operands[0].imm == target, "Height receiver call differs")

    accesses = (
        (0x4603eb, "lea", 0x0b4, -256), (0x4603f7, "lea", 0x090, -280),
        (0x4603fe, "lea", 0x078, -304), (0x46042c, "lea", 0x050, -356),
        (0x460435, "lea", 0x044, -356), (0x460456, "lea", 0x044, -356),
        (0x46047d, "lea", 0x0a8, -256),
        (0x460497, "mov", 0x04c, -348), (0x46049d, "mov", 0x0b0, -248),
    )
    for at, opcode, displacement, slot in accesses:
        ins = nodes[at]
        operand = ins.operands[1]
        require(ins.mnemonic == opcode and operand.type == X.X86_OP_MEM and
                operand.mem.base == X.X86_REG_ESP and not operand.mem.index and
                operand.mem.disp == displacement and frames.height(at) + displacement == slot,
                "Coordinate constructor or comparison slot differs")
    for loads in ((0x4603c7, 0x4603d3, 0x4603e0), (0x460408, 0x460414, 0x460421)):
        for at, slot in zip(loads, (-288, -296, -304)):
            ins = nodes[at]
            operand = ins.operands[0]
            require(ins.mnemonic == "fld" and operand.type == X.X86_OP_MEM and operand.size == 8 and
                    operand.mem.base == X.X86_REG_ESP and not operand.mem.index and
                    frames.height(at) + operand.mem.disp == slot, "Coordinate conversion load differs")
    for producer in (0x4603f2, 0x460430):
        require(0x460497 not in reachable(graph["edges"], [graph["entry"]], blocked=[producer]),
                "Height comparison can bypass a coordinate producer")

def verify_coordinate_calls(graph, known, receivers):
    known = dict(known)
    coordinate_calls = (
        ("AbstractClass", 22, 0x45fb5c, 0x45fb54),
        ("ObjectClass", 40, 0x45fb94, 0x45fb8c),
        ("BulletClass", 18, 0x45fc26, 0x45fc13),
    )
    for owner, slot, call, _ in coordinate_calls:
        family = receivers.family(("input", 0, owner), slot)
        require(family["pop"] == 4 and family["record_result"] == "CoordStruct",
                "Coordinate result ABI differs")
        known[call] = family["pop"]
    frames = aligned_frames(graph, known)
    verify_coordinate_returns(graph, frames, coordinate_calls)


def verify_coordinate_returns(graph, frames, calls):
    nodes = graph["nodes"]
    for _, slot, call, setup in calls:
        instruction = nodes[call]
        argument = nodes[setup].operands[1]
        require(instruction.mnemonic == "call" and instruction.operands[0].type == X.X86_OP_MEM
                and instruction.operands[0].mem.disp == slot * 4 and nodes[setup].mnemonic == "lea"
                and argument.type == X.X86_OP_MEM and argument.mem.base == X.X86_REG_ESP
                and not argument.mem.index and frames.height(setup) + argument.mem.disp == -256,
                "Coordinate result buffer differs")
    require(nodes[0x45fb6f].op_str == "ecx, dword ptr [ebp + 0xf4]" and
            nodes[0x45fb84].op_str == "byte ptr [ecx + 0x14], 2" and
            nodes[0x45fb88].mnemonic == "je" and graph["pred"][0x45fb8a] == {0x45fb88} and
            nodes[0x45fc20].op_str == "ecx, ebp", "Coordinate receiver root differs")
    ctor = nodes[0x45fc4a]
    require(ctor.mnemonic == "call" and ctor.operands[0].imm == 0x41b630 and
            nodes[0x45fc45].op_str == "ecx, [esp + 0x70]" and
            frames.height(0x45fc45) + 0x70 == -320,
            "Coordinate delta constructor differs")


def repair_speed_and_rotation(orig):
    start=orig.index("                    ((Callee_00437690*)")
    finish=orig.index("                }\n                else {",start)
    old=orig[start:finish]
    require(old.count("FUN_00590cd0")==2 and old.count("FUN_004376b0")==1 and old.count("FUN_00437690")==1, "Frozen Bullet seed shape changed")
    replacement="""                    Vector3 rotated;
                    rotated.Set_00437690((float)*((double const *)&local_120),
                                         -(float)*((double const *)&local_118),
                                         (float)*((double const *)&local_110));
                    rotated = reinterpret_cast<Matrix3D const *>(local_98)->Rotate_Vector(rotated);
                    rotated.Scale_004376b0((float)*((double const *)&local_158));
                    rotated.Z = -rotated.Z;
                    rotated = reinterpret_cast<Matrix3D const *>(local_c8)->Rotate_Vector(rotated);
                    *((double *)&local_120) = (double)rotated.X;
                    *((double *)&local_118) = (double)-rotated.Y;
                    *((double *)&local_110) = (double)rotated.Z;
"""
    text=orig[:start]+replacement+orig[finish:]
    start=text.index("        pdVar14 = (double *)(param_1 + 0x34);\n        local_1a0")
    finish=text.index("        *((double *)&(local_120)) = *pdVar14;",start)
    old=text[start:finish]
    repaired=old.replace("*((double *)&(local_148)) = (double)fVar24;","double speed = (double)fVar24;")
    repaired=repaired.replace("*((double const *)&(local_148))","speed").replace("*((double *)&(local_148))","speed")
    repaired=repaired.replace("local_148 = (undefined8)(local_148 - (double)(int)local_1a0);","speed -= (double)(int)local_1a0;")
    repaired=repaired.replace("local_148 = (undefined8)(0.0);","speed = 0.0;").replace("local_148","speed")
    repaired,count=re.subn(r"FUN_004ba410\(\(int\)\(([^;]*?)\)\)",r"Fast_Sqrt(\1)",repaired)
    require(count==2 and repaired.count("double speed =")==1, "Frozen Bullet seed shape changed")
    text=text[:start]+repaired+text[finish:]
    cut=re.search(r"^#include .+$",text,re.M).end()
    text=text[:cut]+'\n#include "shared/matrix3d.hpp"'+text[cut:]
    return text


def repair_signed_coordinates(old):
    text=old.replace('#include "shared/matrix3d.hpp"',
                     '#include "shared/matrix3d.hpp"\n#include "shared/coord.hpp"\n#include "shared/unknown_vector3.hpp"')
    text=text.replace("    float local_178;\n","").replace("    int iStack_140;\n","")
    for field in ("local_164","local_184","fVar10"):
        require(text.count(f"    float {field};")==1, "Frozen Bullet seed shape changed")
        text=text.replace(f"    float {field};",f"    int {field};")
    text=text.replace("(float)(*(float *)(iVar9 + 0x90))","*(int *)(iVar9 + 0x90)")
    text=text.replace("(float)param_1[0x24]","param_1[0x24]").replace("(float)FUN_0077f110()","(int)FUN_0077f110()")
    text=re.sub(r"\b(local_184|local_164|fVar10) = \(float\)(\([^;\n]+\));",r"\1 = \2;",text)
    text=text.replace("fVar10 = (float)(local_184),","fVar10 = local_184,")
    start=text.index("        *((double *)&(local_180)) = *pdVar7;\n        piVar8")
    stop=text.index("        local_1a0 = (undefined4)((int *)(iVar9 - iVar5));",start)
    old_span=text[start:stop]
    old_pair=old_span.index("        iStack_140 =")
    span=old_span[:old_pair]
    span=span.replace("*((double *)&(local_180)) = *pdVar7;","CoordStruct target = *reinterpret_cast<CoordStruct const *>(pdVar7);",1)
    span=span.replace("*((double *)&(local_180)) = *pdVar7;","target = *reinterpret_cast<CoordStruct const *>(pdVar7);",1)
    span=re.sub(r"(?m)^ *local_178 = \(float\)\(\*\(float \*\)\(pdVar7 \+ 1\)\);\n","",span)
    for old_expr,new_expr in [("(*(uint*)((char*)&local_180 + 4))","target.Y"),("(int)local_180","target.X"),("(int)local_178","target.Z"),("(float)(local_178)","target.Z"),("local_178","target.Z"),("(&local_180)","(&target)")]:
        span=span.replace(old_expr,new_expr)
    require("local_180" not in span and "local_178" not in span and span.count("reinterpret_cast<CoordStruct const *>")==2, "Frozen Bullet seed shape changed")
    pair="""        UnknownVector3 saved_delta;
        saved_delta.X = local_16c - iVar9;
        saved_delta.Y = local_168 - iVar5;
        saved_delta.Z = local_164 - target.Z;
        UnknownVector3 current_delta;
        current_delta.Z = local_184 - target.Z;
        current_delta.X = local_18c - iVar9;
        current_delta.Y = local_188 - iVar5;
        iVar5 = current_delta.Length();
        iVar9 = saved_delta.Length();
"""
    text=text[:start]+span+pair+text[stop:]
    require(text.count(".Length()")==2 and "local_178" not in text and "iStack_140" not in text, "Frozen Bullet seed shape changed")
    text=text.replace('#include "shared/unknown_vector3.hpp"',
                      '#include "shared/unknown_vector3.hpp"\n#define RA2_DIR_MISSILE_ANGLES\n#define RA2_BULLET_MISSILE_VELOCITY\n#include "shared/bullet_velocity.hpp"\n#undef RA2_BULLET_MISSILE_VELOCITY\n#undef RA2_DIR_MISSILE_ANGLES')
    text=text.replace("    int local_18c;\n    int local_188;\n    int local_184;", "    CoordStruct projected_coord;")
    for old_name,new_name in (("local_18c","projected_coord.X"),("local_188","projected_coord.Y"),("local_184","projected_coord.Z")):
        text=re.sub(r"\b"+old_name+r"\b",new_name,text)
    require(text.count("    CoordStruct projected_coord;")==1, "Frozen Bullet seed shape changed")
    old_decl="extern undefined4 __fastcall FUN_00593870(int, int, int, int, int, int);"
    new_decl="""extern int __fastcall Projectile_Motion(CoordStruct &, TVelocity3D<double> &,
                                         CoordStruct const &, DirType const &,
                                         bool, bool, bool, bool);"""
    require(text.count(old_decl)==1, "Frozen Bullet seed shape changed")
    text=text.replace(old_decl+"\n","")
    namespace_at=text.index("namespace seed_")
    text=text[:namespace_at]+new_decl+"\n\n"+text[namespace_at:]
    velocity_span="""        *((double *)&(local_120)) = *pdVar14;
        *((double *)&(local_118)) = *(double *)(param_1 + 0x36);
        *((double *)&(local_110)) = *(double *)(param_1 + 0x38);
"""
    target_start=text.index("        CoordStruct target =")
    velocity_start=text.rfind(velocity_span,0,target_start)
    require(0<target_start-velocity_start<800, "Frozen Bullet seed shape changed")
    velocity_end=velocity_start+len(velocity_span)
    text=text[:velocity_start]+"""        TVelocity3D<double> velocity;
        velocity.X = *pdVar14;
        velocity.Y = *(double *)(param_1 + 0x36);
        velocity.Z = *(double *)(param_1 + 0x38);
"""+text[velocity_end:]
    call=text.index("FUN_00593870(")
    call_start=text.rfind("\n        local_1a0 =",0,text.rfind("\n",0,call))+1
    call_end=text.index("\n",call)+1
    require("local_1a0._" not in text[call_start:call_end], "Frozen Bullet seed shape changed")
    require(text[call_start:call_end].count("local_1a0 =")==2, "Frozen Bullet seed shape changed")
    text=text[:call_start]+"""        DirType heading((int)bVar18 << 8);
        local_1a0 = (undefined4)Projectile_Motion(
            projected_coord, velocity, target, heading, uVar20 != 0,
            *(undefined1 *)(iVar5 + 0x264) != 0, *(undefined1 *)(iVar5 + 0x269) != 0,
            *(undefined1 *)(iVar5 + 0x26d) != 0);
"""+text[call_end:]
    copy_start=text.index("        local_fc[1] =",text.index("local_1a0 = (undefined4)Projectile_Motion"))
    copy_end=text.index("        *((double *)&(local_158)) = (double)(int)local_1a0;",copy_start)
    text=text[:copy_start]+"""        *pdVar14 = velocity.X;
        *(double *)(param_1 + 0x36) = velocity.Y;
        *(double *)(param_1 + 0x38) = velocity.Z;
"""+text[copy_end:]

    labels = r"(?m)^[ \t]*((?:LAB_|joined_r0x)\w+):"
    require(re.findall(labels,old)==re.findall(labels,text), "Frozen Bullet seed shape changed")
    return text


def repair_height_receivers(text):
    declaration = "    double local_138;\n    double local_130;\n    double local_128;"
    require(text.count(declaration) == 1 and len(re.findall(r"\blocal_100\b", text)) == 3,
            "Frozen coordinate lifetime changed")
    text = text.replace(declaration, "    TVelocity3D<double> next_position;")
    for local, field in (("local_138", "X"), ("local_130", "Y"), ("local_128", "Z")):
        text = re.sub(r"\b"+local+r"\b", "next_position."+field, text)

    start = text.index("            uVar20 = (undefined4)(FUN_0077f110());\n")
    stop = text.index("            local_160 = (int)(iVar5);", start)
    old = text[start:stop]
    require(old.count("FUN_0077f110(") == 6 and all(name in old for name in
            ("FUN_0041b630", "FUN_004642a0", "FUN_00434820", "FUN_0055a280")),
            "Frozen coordinate receiver source changed")
    replacement = """            const int old_z = (int)next_position.Z;
            const int old_y = (int)next_position.Y;
            const int old_x = (int)next_position.X;
            UnknownVector3 previous_position(old_x, old_y, old_z);
            TVelocity3D<double> frame_velocity;
            frame_velocity.X = *((double const *)&local_120);
            frame_velocity.Y = *((double const *)&local_118);
            frame_velocity.Z = *((double const *)&local_110);
            next_position += frame_velocity;
            const int next_z = (int)next_position.Z;
            const int next_y = (int)next_position.Y;
            const int next_x = (int)next_position.X;
            CoordStruct height_position;
            height_position.Set_00434820(next_x, next_y, next_z);
            local_1a0 = (undefined4)((Callee_0055a280*)(&DAT_008324e0))->FUN_0055a280(&height_position.X);
            iVar5 = (int)local_1a0 + DAT_00850338;
"""
    text = text[:start] + replacement + text[stop:]
    stop = text.index("            bVar2 = (bool)(false);", start)
    span = text[start:stop]
    require(span.count("(&local_16c)") == 1 and span.count("(local_108)") == 1 and
            span.count("(int)local_164") == 1 and span.count("local_100") == 2,
            "Frozen height comparisons changed")
    span = (span.replace("((int *)(&local_16c))", "(&height_position.X)")
            .replace("((int *)(local_108))", "(&previous_position.X)")
            .replace("(int)local_164", "height_position.Z").replace("local_100", "previous_position.Z"))
    return (text[:start] + span + text[stop:]).replace("    int local_100;\n", "")


def repair_coordinate_returns(original):
    text=original.replace('#include "shared/ghidra_types.hpp"', '#include "shared/bullet.hpp"\n#include "shared/dir_type.hpp"\nnamespace seed_0045f710 {\n#include "shared/ghidra_types.hpp"\n}')
    old_include='''#define RA2_DIR_MISSILE_ANGLES
#define RA2_BULLET_MISSILE_VELOCITY
#include "shared/bullet_velocity.hpp"
#undef RA2_BULLET_MISSILE_VELOCITY
#undef RA2_DIR_MISSILE_ANGLES
'''
    require(text.count(old_include)==1, "Frozen coordinate seed shape changed")
    text=text.replace(old_include,"")
    require(text.count("local_108")==3, "Frozen coordinate seed shape changed")
    text=text.replace("    undefined1 local_108 [8];\n","")
    start=text.index('        if ((uint)((int *)param_1[0x3d]) == (uint)((int *)0x0)) {\n            pdVar7')
    stop=text.index("        iVar5 = (int)((**(code **)(param_1[1] + 0x10))",start)
    text=text[:start]+'''        BulletClass *bullet = reinterpret_cast<BulletClass *>(param_1);
        AbstractClass *target_object = bullet->Target;
        CoordStruct target;
        if (target_object == 0) {
            target = *reinterpret_cast<CoordStruct const *>(&DAT_00850310);
        }
        else {
            target = target_object->unknown_slot22();
        }
        target_object = bullet->Target;
        if (target_object != 0 && target_object->AbstractFlags.IsObject) {
            target = static_cast<ObjectClass *>(target_object)->unknown_slot40();
        }
'''+text[stop:]
    start=text.index("        piVar8 = (int *)(**(code **)(*param_1 + 0x48))((int)&local_110 + 4);")
    stop=text.index("        if (iVar5 < 0x100)",start)
    span=text[start:stop]
    require(span.count("FUN_0041b630")==1 and span.count("FUN_004ba410")==1 and span.count("FUN_0077f110")==1, "Frozen coordinate seed shape changed")
    text=text[:start]+'''        CoordStruct center = bullet->Center_Coord();
        UnknownVector3 delta(center.X - target.X, center.Y - target.Y, center.Z - target.Z);
        double dx = delta.X;
        double dy = delta.Y;
        double dz = delta.Z;
        iVar5 = (int)Fast_Sqrt(dx * dx + (dy * dy + dz * dz));
'''+text[stop:]
    labels=r"(?m)^[ \t]*((?:LAB_|joined_r0x)\w+):"
    require(re.findall(labels,text)==re.findall(labels,original), "Frozen coordinate seed shape changed")
    return text


def optional_bullet_ai(text, entry, inventory, repository, accepted):
    if entry != ENTRY or entry in accepted:
        return text
    if hashlib.sha256(text.encode("utf-8")).hexdigest() != SEED_SHA256:
        return text
    try:
        graph, known, receivers = verify_native(Path(repository), inventory)
        result = repair_height_receivers(repair_signed_coordinates(repair_speed_and_rotation(text)))
    except (ValueError, TypeError, KeyError, OSError):
        return text
    try:
        verify_coordinate_calls(graph, known, receivers)
        result = repair_coordinate_returns(result)
    except (ValueError, TypeError, KeyError, OSError):
        # The previous repair is valid even if the separate virtual families
        # cannot be proved from the local metadata.
        pass
    return ("// Scratch hypothesis: separate floating and signed integer lifetimes in Bullet AI.\n"
            + result)
