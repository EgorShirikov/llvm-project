//===----------------------------------------------------------------------===//
//  Describe May instructions format
//
//  CPU INSTRUCTION FORMATS
//
//  FF FF FF FF
//        ^^ ^^ - r3_imm - third reg or imm
//      ^       - r2     - second reg
//     ^        - r1     - first reg
//  ^^          - opcode - operation code
//
//===----------------------------------------------------------------------===//

// Generic  Format
class MayInst<bits<8> op, dag outs, dag ins, string asmstr, list<dag> pattern>: Instruction
{
  // Inst and Size: for tablegen(... -gen-emitter) and
  // tablegen(... -gen-disassembler) in CMakeLists.txt
  let Namespace = "May";
  let DecoderNamespace = "May";
  let Size = 4;

  field bits<32> Inst;

  bits<8> Opcode = op;
  bits<4>  r1;
  bits<4>  r2;
  bits<16> r3_imm;

  let Inst{31-24} = Opcode;
  let Inst{23-20} = r1;
  let Inst{19-16} = r2;
  let Inst{15-0}  = r3_imm;

  let OutOperandList = outs;
  let InOperandList  = ins;
  let AsmString   = asmstr;
  let Pattern     = pattern;

}

// Pseudo instructions format
class MayPseudoInst<dag outs, dag ins, string asmstr, list<dag> pattern>
   : MayInst<0, outs, ins, asmstr, pattern> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

//intrinsics

def PUTPIXEL : MayInst<0xF0, (outs), (ins GPR:$r1, GPR:$r2, GPR:$r3_imm),
                      "PUTPIXEL $r1 $r2 $r3_imm", [(int_may_putpixel GPR:$r1, GPR:$r2, GPR:$r3_imm)]>;

let r1 = 0, r2 = 0, r3_imm = 0 in
def FLUSH : MayInst<0xF1, (outs), (ins), "FLUSH", [(int_may_flush)]>;

let r1 = 0, r2 = 0, r3_imm = 0 in
def INIT  : MayInst<0xF4, (outs), (ins), "INIT", [(int_may_init)]>;

let r1 = 0, r2 = 0, r3_imm = 0 in
def EXIT  : MayInst<0xF4, (outs), (ins), "EXIT", [(int_may_exit)]>;

let r1 = 0, r2 = 0, r3_imm = 0 in
def EVENT  : MayInst<0xF4, (outs), (ins), "EVENT", [(int_may_event)]>;