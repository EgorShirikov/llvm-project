#ifndef LLVM_LIB_TARGET_MAY_MCTARGETDESC_MAYINFO_H
#define LLVM_LIB_TARGET_MAY_MCTARGETDESC_MAYINFO_H

#include "llvm/MC/MCInstrDesc.h"

namespace llvm {
namespace MayCC {
enum CondCode {
  EQ,
  NE,
  LE,
  GT,
  LEU,
  GTU,
  INVALID,
};

CondCode getOppositeBranchCondition(CondCode);

enum BRCondCode {
  BREQ = 0x0,
};
}

namespace MayOp {
enum OperandType : unsigned {
  OPERAND_SIMM16 = MCOI::OPERAND_FIRST_TARGET,
  OPERAND_UIMM16
};
} // namespace MayOp

} // end namespace llvm

#endif