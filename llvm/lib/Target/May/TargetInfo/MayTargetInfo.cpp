#include "May.h"
#include "TargetInfo/MayTargetInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheMayTarget() {
  MAY_DUMP_YELLOW;
  static Target TheMayTarget;
  return TheMayTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMayTargetInfo() {
  RegisterTarget<Triple::may> X(getTheMayTarget(), "may", "May32",
                                 "MAY");
}