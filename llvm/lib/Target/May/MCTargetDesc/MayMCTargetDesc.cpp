#include "May.h"
#include "TargetInfo/MayTargetInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "MCTargetDesc/MayInfo.h"
#include "llvm/MC/MCInstrInfo.h"

using namespace llvm;

#define GET_REGINFO_MC_DESC
#include "MayGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "MayGenInstrInfo.inc"

static MCRegisterInfo *createMayMCRegisterInfo(const Triple &TT) {
  MAY_DUMP_MAGENTA
  MCRegisterInfo *X = new MCRegisterInfo();
  InitMayMCRegisterInfo(X, May::R0);
  return X;
}

static MCInstrInfo *createMayMCInstrInfo() {
  MAY_DUMP_MAGENTA
  MCInstrInfo *X = new MCInstrInfo();
  InitMayMCInstrInfo(X);
  return X;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMayTargetMC() {
  MAY_DUMP_MAGENTA
  Target &TheMayTarget = getTheMayTarget();
  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(TheMayTarget, createMayMCRegisterInfo);
  TargetRegistry::RegisterMCInstrInfo(TheMayTarget, createMayMCInstrInfo);
}