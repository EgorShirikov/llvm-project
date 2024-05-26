#ifndef LLVM_LIB_TARGET_MAY_MCTARGETDESC_MAYINFO_H
#define LLVM_LIB_TARGET_MAY_MCTARGETDESC_MAYINFO_H

#include "../../../../include/llvm/MC/MCInstrDesc.h"

namespace llvm {

namespace MayOp {
enum OperandType : unsigned {
  OPERAND_MayM16 = MCOI::OPERAND_FIRST_TARGET,
};
} // namespace MayOp

} // end namespace llvm

#endif