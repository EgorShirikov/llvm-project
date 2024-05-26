#ifndef LLVM_LIB_TARGET_MAY_MCTARGETDESC_MAYMCASMINFO_H
#define LLVM_LIB_TARGET_MAY_MCTARGETDESC_MAYMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {

class Triple;

class MayELFMCAsmInfo : public MCAsmInfoELF {
public:
  explicit MayELFMCAsmInfo(const Triple &TheTriple);
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_MAY_MCTARGETDESC_MAYMCASMINFO_H