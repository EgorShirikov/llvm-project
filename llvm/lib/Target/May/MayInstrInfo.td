//===----------------------------------------------------------------------===//
// MAY profiles and nodes
//===----------------------------------------------------------------------===//

def may_ret      : SDNode<"MayISD::RET", SDTNone,
                            [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "MayInstrFormats.td"

//===----------------------------------------------------------------------===//
// MAY Operand, Complex Patterns and Transformations Definitions.
//===----------------------------------------------------------------------===//

class MImmAsmOperand<int width> : AsmOperandClass {
  let Name = "MImm" # width;
  let RenderMethod = "addImmOperands";
  let DiagnosticType = !strconcat("Invalid", Name);
}

def mimm16 : Operand<i32>, ImmLeaf<i32, [{return isInt<16>(Imm);}]> {
  let ParserMatchClass = MImmAsmOperand<16>;
  let EncoderMethod = "getImmOpValue";
  let DecoderMethod = "decodeMImmOperand<16>";
  let MCOperandPredicate = [{
    int64_t Imm;
    if (MCOp.evaluateAsConstantImm(Imm))
      return isInt<16>(Imm);
    return MCOp.isBareSymbolRef();
  }];
  let OperandType = "OPERAND_MayM16";
  let OperandNamespace = "MayOp";
}

//===----------------------------------------------------------------------===//
// MAY Instructions
//===----------------------------------------------------------------------===//

let hasSideEffects = 0, mayLoad = 0, mayStore = 0, isCall = 1, isBranch = 1, isTerminator = 1 in
def BR : MayInst<0x0, (outs), (ins GPR:$rs1), "BR $rs1",
                  [(brind GPR:$rs1)]>;

let isBarrier = 1, isReturn = 1, isTerminator = 1 in
def PseudoRET : MayPseudoInst<(outs), (ins), "# PseudoRET",
                [(may_ret)]>,
                PseudoInstExpansion<(BR R0)>;

// movli
let isAsCheapAsAMove = 1, isCodeGenOnly = 1, isReMaterializable = 1 in
def MOVLI : MayInst<0x0, (outs GPR:$rd), (ins mimm16:$imm),
                     "MOVli $rd $imm",
                     [(set GPR:$rd, mimm16:$imm)]>;