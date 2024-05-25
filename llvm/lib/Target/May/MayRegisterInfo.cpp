#include "MayRegisterInfo.h"
#include "May.h"
#include "MayFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

using namespace llvm;

#define GET_REGINFO_TARGET_DESC
#include "MayGenRegisterInfo.inc"

MayRegisterInfo::MayRegisterInfo() : MayGenRegisterInfo(May::R0) {
  MAY_DUMP_GREEN
}