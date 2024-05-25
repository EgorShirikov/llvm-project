#ifndef LLVM_LIB_TARGET_MAY_MAYREGISTERINFO_H
#define LLVM_LIB_TARGET_MAY_MAYREGISTERINFO_H

#define GET_REGINFO_HEADER
#include "MayGenRegisterInfo.inc"

namespace llvm {

struct MayRegisterInfo : public MayGenRegisterInfo {
public:
  MayRegisterInfo();
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_MAY_MAYREGISTERINFO_H