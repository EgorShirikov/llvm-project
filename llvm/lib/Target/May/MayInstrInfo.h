#ifndef LLVM_LIB_TARGET_MAY_MAYINSTRINFO_H
#define LLVM_LIB_TARGET_MAY_MAYINSTRINFO_H

#include "MayRegisterInfo.h"
#include "MCTargetDesc/MayInfo.h"
#include "llvm/CodeGen/TargetInstrInfo.h"

#define GET_INSTRINFO_HEADER
#include "MayGenInstrInfo.inc"

namespace llvm {

class MaySubtarget;

class MayInstrInfo : public MayGenInstrInfo {
public:
  MayInstrInfo();

};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_MAY_MAYINSTRINFO_H