#include "May.h"
#include "TargetInfo/MayTargetInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_REGINFO_MC_DESC
#include "MayGenRegisterInfo.inc"

static MCRegisterInfo *createMayMCRegisterInfo(const Triple &TT) {
  MAY_DUMP_MAGENTA
  MCRegisterInfo *X = new MCRegisterInfo();
  InitMayMCRegisterInfo(X, May::R0);
  return X;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMayTargetMC() {
  MAY_DUMP_MAGENTA
  Target &TheMayTarget = getTheMayTarget();
  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(TheMayTarget, createMayMCRegisterInfo);
}