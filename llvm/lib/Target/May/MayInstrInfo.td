//===----------------------------------------------------------------------===//
// may profiles and nodes
//===----------------------------------------------------------------------===//

def SDT_MayCall         : SDTypeProfile<0, -1, [SDTCisVT<0, i32>]>;
def SDT_MayCallSeqStart : SDCallSeqStart<[ SDTCisVT<0, i32>,
                                            SDTCisVT<1, i32> ]>;
def SDT_MayCallSeqEnd   : SDCallSeqEnd<[ SDTCisVT<0, i32>,
                                          SDTCisVT<1, i32> ]>;

def SDT_MayBrCC : SDTypeProfile<0, 4, [ SDTCisSameAs<0, 1>,
                                         SDTCisVT<2, OtherVT>,
                                         SDTCisVT<3, OtherVT>]>;
//def Mayselectreg :
//SDTypeProfile<1, 4, [SDTCisSameAs<0, 1>, SDTCisSameAs<1, 2>, SDTCisVT<3, OtherVT>, SDTCisVT<4, OtherVT>]>;

def Mayselectreg :
SDTypeProfile<1, 2, []>;

// These are target-independent nodes, but have target-specific formats.
def callseq_start : SDNode<"ISD::CALLSEQ_START", SDT_MayCallSeqStart,
                           [SDNPHasChain, SDNPOutGlue]>;
def callseq_end   : SDNode<"ISD::CALLSEQ_END",   SDT_MayCallSeqEnd,
                           [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;

def may_ret      : SDNode<"MayISD::RET", SDTNone,
                            [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

def may_call     : SDNode<"MayISD::CALL", SDT_MayCall,
                            [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                             SDNPVariadic]>;

def may_brcc     :  SDNode<"MayISD::BR_CC", SDT_MayBrCC,
                            [SDNPHasChain]>;

def may_selectreg : SDNode<"MayISD::SELECT_REG", Mayselectreg, []>;


//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "MayInstrFormats.td"

//===----------------------------------------------------------------------===//
// Pseudo Instructions
//===----------------------------------------------------------------------===//

let Defs = [R2], Uses = [R2] in {
def ADJCALLSTACKDOWN : MayPseudoInst<(outs), (ins i32imm:$amt, i32imm:$amt2),
                               "# ADJCALLSTACKDOWN $amt, $amt2",
                               [(callseq_start timm:$amt, timm:$amt2)]>;
def ADJCALLSTACKUP : MayPseudoInst<(outs), (ins i32imm:$amt1, i32imm:$amt2),
                            "# ADJCALLSTACKUP $amt1",
                            [(callseq_end timm:$amt1, timm:$amt2)]>;
}

def IsOrAdd: PatFrag<(ops node:$A, node:$B), (or node:$A, node:$B), [{
  return isOrEquivalentToAdd(N);
}]>;

//===----------------------------------------------------------------------===//
// May Operand, Complex Patterns and Transformations Definitions.
//===----------------------------------------------------------------------===//

class ImmAsmOperand<string prefix, int width, string suffix> : AsmOperandClass {
  let Name = prefix # "Imm" # width # suffix;
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

class SImmAsmOperand<int width, string suffix = "">
    : ImmAsmOperand<"S", width, suffix> {
}

class UImmAsmOperand<int width, string suffix = "">
    : ImmAsmOperand<"U", width, suffix> {
}

def simm16 : Operand<i32>, ImmLeaf<i32, [{return isInt<16>(Imm);}]> {
  let ParserMatchClass = SImmAsmOperand<16>;
  let EncoderMethod = "getImm16OpValue";
  let DecoderMethod = "decodeSImmOperand<16>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<16>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_SIMM16";
  let OperandNamespace = "MayOp";
}


def uimm16 : Operand<i32>, ImmLeaf<i32, [{return isUInt<16>(Imm);}]> {
  let ParserMatchClass = UImmAsmOperand<16>;
  let EncoderMethod = "getImm16OpValue";
  let DecoderMethod = "decodeUImmOperand<16>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isUInt<16>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_UIMM16";
  let OperandNamespace = "MayOp";
}



def btarget16 : Operand<OtherVT> {
  let ParserMatchClass = SImmAsmOperand<16>;
  let PrintMethod = "printBranchOperand";
  let EncoderMethod = "getBranchTarget16OpValue";
  let DecoderMethod = "decodeSImmOperand<16>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<16>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_PCREL";
}

// TODO: sym methods
def ctarget16 : Operand<i32> {
  let ParserMatchClass = SImmAsmOperand<16>;
  let PrintMethod = "printBranchOperand";
  let EncoderMethod = "getBranchTarget16OpValue";
  let DecoderMethod = "decodeSImmOperand<16>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<16>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_PCREL";
}

def LI16 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() & 0xffff,
                                   SDLoc(N), MVT::i32);
}]>;

def HI16 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() >> 16, SDLoc(N),
                                   MVT::i32);
}]>;

// def LiImm16AsmOperand : AsmOperandClass { let Name = "LiImm16"; }
def i32li16u : Operand<i32>, PatLeaf<(i32 imm), [{
    return ((N->getZExtValue() & 0xFFFFUL) == N->getZExtValue());}], LI16> {
  // let ParserMatchClass = LiImm16AsmOperand;
}

// def HiImm16AsmOperand : AsmOperandClass { let Name = "HiImm16"; }
def i32hi16 : Operand<i32>, PatLeaf<(i32 imm), [{
    // i32hi16 predicate - true if the 32-bit immediate has only leftmost 16
    // bits set.
    return ((N->getZExtValue() & 0xFFFF0000UL) == N->getZExtValue());}], HI16> {
  // let ParserMatchClass = HiImm16AsmOperand;
  // let PrintMethod = "printHi16ImmOperand";
}

// Addressing modes
def AddrFI : ComplexPattern<iPTR, 1, "SelectAddrFI", [frameindex], []>;
def BaseAddr : ComplexPattern<iPTR, 1, "SelectBaseAddr">;

class PatGpr<SDPatternOperator OpNode, MayInst Inst>
    : Pat<(OpNode GPR:$r1), (Inst GPR:$r1)>;
class PatGprGpr<SDPatternOperator OpNode, MayInst Inst>
    : Pat<(OpNode GPR:$r1, GPR:$r2), (Inst GPR:$r1, GPR:$r2)>;
class PatGprImm<SDPatternOperator OpNode, MayInst Inst, ImmLeaf ImmType>
    : Pat<(i32 (OpNode (i32 GPR:$r1), ImmType:$r3_imm)),
          (Inst GPR:$r1, ImmType:$r3_imm)>;

class PatBcc<CondCode Cond, MayInst Inst>
    : Pat<(may_brcc GPR:$r1, GPR:$r2, Cond, bb:$r3_imm),
          (Inst GPR:$r1, GPR:$r2, btarget16:$r3_imm)>;

def RegCCOp : Operand<i16>;
let Constraints = "$rd = $r1",r2 = 0, r3_imm = 0 in
{
    def MOVR : MayInst<0x00, (outs GPR:$rd), (ins GPR:$r1, GPR:$r2),
    //,GPR: $r2,RegCCOp: $rcode, GPR:$r3_imm),
                           //!strconcat("movr$rcode","$r1"),
                           "MOVR $r1 $r2",
                           // [(set i32:$rd, (Mayselectreg maym32:imm , i32:$r2, $rcond, i32:$r4))]>;
                            [(set GPR:$rd, (may_selectreg GPR:$r1, GPR:$r2))]>;
}

//===----------------------------------------------------------------------===//
// Instructions specific format
//===----------------------------------------------------------------------===//

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isBranch = 1, isTerminator = 1 in
class BranchCC<bits<8> op, string opcodestr>
    : MayInst<op, (outs), (ins GPR:$r1, GPR:$r2, btarget16:$r3_imm),
              !strconcat(opcodestr, " $r1 $r2 $r3_imm"), []>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
class ArithmRR<bits<8> op, string opcodestr>
    : MayInst<op, (outs GPR:$r1), (ins GPR:$r2, GPR:$r3_imm),
              !strconcat(opcodestr, " $r1 $r2 $r3_imm"), []>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0 in
class ArithmRI<bits<8> op, string opcodestr>
    : MayInst<op, (outs GPR:$r1), (ins GPR:$r2, simm16:$r3_imm),
              !strconcat(opcodestr, " $r1 $r2 $r3_imm"), []>;


//===----------------------------------------------------------------------===//
// Instruction definition
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// May Instructions
//===----------------------------------------------------------------------===//

// TODO: check B* flags
let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isBarrier = 1, isBranch = 1, isTerminator = 1, r1 = 0, r2 = 0 in
def B : MayInst<0xB0, (outs), (ins btarget16:$r3_imm), "B $r3_imm",
                 [(br bb:$r3_imm)]>;
let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isCall = 1, isBranch = 1, isTerminator = 1, r2 = 0 in
def BL : MayInst<0xBA, (outs GPR:$r1), (ins ctarget16:$r3_imm), "BL $r1 $r3_imm",
                  []>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isCall = 1, isBranch = 1, isTerminator = 1, r2 = 0, r3_imm = 0 in
def BR : MayInst<0xBB, (outs), (ins GPR:$r1), "BR $r1",
                  [(brind GPR:$r1)]>;

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isCall = 1, isBranch = 1, isTerminator = 1, r3_imm = 0 in
def BLR : MayInst<0xBC, (outs GPR:$r1), (ins GPR:$r2),
                  "BLR $r1 $r2", []>;

let isBarrier = 1, isReturn = 1, isTerminator = 1 in
def PseudoRET : MayPseudoInst<(outs), (ins), "# PseudoRET",
                [(may_ret)]>,
                PseudoInstExpansion<(BR R0)>;

let isCall = 1, Defs = [R0] in
def PseudoCALL : MayPseudoInst<(outs), (ins ctarget16:$r3_imm), "# PseudoCALL",
                 [(may_call tglobaladdr:$r3_imm)]>,
                 PseudoInstExpansion<(BL R0, ctarget16:$r3_imm)>;

let isCall = 1, Defs = [R0] in
def PseudoCALLInd : MayPseudoInst<(outs), (ins GPR:$r1), "# PseudoCALLInd",
                    [(may_call GPR:$r1)]>,
                    PseudoInstExpansion<(BLR R0, GPR:$r1)>;

def BEQ  : BranchCC<0x70, "B.EQ">;
def BNE  : BranchCC<0x71, "B.NE">;
def BLE  : BranchCC<0x72, "B.LE">;
def BGT  : BranchCC<0x73, "B.GT">;
def BLEU  : BranchCC<0x74, "B.LEU">;
def BGTU  : BranchCC<0x75, "B.GTU">;

def : PatBcc<SETEQ, BEQ>;
def : PatBcc<SETNE, BNE>;
def : PatBcc<SETLE, BLE>;
def : PatBcc<SETGT, BGT>;
def : PatBcc<SETULE, BLEU>;
def : PatBcc<SETUGT, BGTU>;

// movli
let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1, r2 = 0 in
def MOVLI : MayInst<0xAA, (outs GPR:$r1), (ins i32li16u:$r3_imm),
                     "MOVli $r1 $r3_imm",
                     [(set GPR:$r1, i32li16u:$r3_imm)]>;

// movhi
let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1, r2 = 0  in
def MOVHI : MayInst<0xAF, (outs GPR:$r1), (ins i32hi16:$r3_imm),
                     "MOVhi $r1 $r3_imm",
                     [(set GPR:$r1, i32hi16:$r3_imm)]>;

// sti, but not SubtargetInfo!
let hasSideEffects = 0, mayLoad = 0, mayStore = 1 in
def STI_: MayInst<0xCA, (outs), (ins GPR:$r1, GPR:$r2, simm16:$r3_imm),
              "STi $r1 ${r2} ${r3_imm}",
              []>;
def : Pat<(store (i32 GPR:$r1), BaseAddr:$r2),
        (STI_ GPR:$r1, BaseAddr:$r2, 0)>;
def : Pat<(store (i32 GPR:$r1), (add BaseAddr:$r2, simm16:$r3_imm)),
        (STI_ GPR:$r1, BaseAddr:$r2, simm16:$r3_imm)>;
def : Pat<(store (i32 GPR:$r1), (IsOrAdd AddrFI:$r2, simm16:$r3_imm)),
        (STI_ GPR:$r1, AddrFI:$r2, simm16:$r3_imm)>;

// ldi
let hasSideEffects = 0, mayLoad = 1, mayStore = 0 in
def LDI: MayInst<0xCB, (outs GPR:$r1), (ins GPR:$r2, simm16:$r3_imm),
              "LDi $r1 ${r2} ${r3_imm}",
              []>;
def : Pat<(i32 (load BaseAddr:$r2)), (LDI BaseAddr:$r2, 0)>;
def : Pat<(i32 (load (add BaseAddr:$r2, simm16:$r3_imm))),
          (LDI BaseAddr:$r2, simm16:$r3_imm)>;
def : Pat<(i32 (load (IsOrAdd AddrFI:$r2, simm16:$r3_imm))),
          (LDI AddrFI:$r2, simm16:$r3_imm)>;

def ADD : ArithmRR<0x30, "ADD">;  def : PatGprGpr<add,  ADD>;
def SUB : ArithmRR<0x31, "SUB">;  def : PatGprGpr<sub,  SUB>;
def MUL : ArithmRR<0x32, "MUL">;  def : PatGprGpr<mul,  MUL>;
def DIV : ArithmRR<0x33, "DIV">;  def : PatGprGpr<sdiv, DIV>;
def OR  : ArithmRR<0x34, "OR">;   def : PatGprGpr<or,   OR>;
def AND : ArithmRR<0x35, "AND">;  def : PatGprGpr<and,  AND>;
def XOR : ArithmRR<0x36, "XOR">;  def : PatGprGpr<xor,  XOR>;
def SHL : ArithmRR<0x37, "SHL">;  def : PatGprGpr<shl,  SHL>;
def SRL : ArithmRR<0x38, "SRL">;  def : PatGprGpr<srl,  SRL>;

def ADDI : ArithmRI<0x40, "ADDi">; def : PatGprImm<add, ADDI, simm16>;
def MULI : ArithmRI<0x42, "MULi">; def : PatGprImm<mul, MULI, simm16>;
def ORI  : ArithmRI<0x44, "ORi">;  def : PatGprImm<mul, ORI,  simm16>;
//===----------------------------------------------------------------------===//
// May Other patterns
//===----------------------------------------------------------------------===//

// i16 imm
def : Pat<(i32 i32li16u:$r3_imm), (MOVLI imm:$r3_imm)>;
// i32 imm
def : Pat<(i32 imm:$r3_imm), (ORI (MOVHI (HI16 imm:$r3_imm)), (LI16 imm:$r3_imm))>;

def : Pat<(add (i32 AddrFI:$r1), simm16:$r3_imm),
          (ADDI (i32 AddrFI:$r1), simm16:$r3_imm)>;
def : Pat<(IsOrAdd (i32 AddrFI:$r1), simm16:$r3_imm),
          (ADDI (i32 AddrFI:$r1), simm16:$r3_imm)>;

//select
//def : Pat<(select (i32 GPR:$r1), (simm16:$r3_imm), (i32 GPR:$r2)),(ADD $r1, $r2)>;