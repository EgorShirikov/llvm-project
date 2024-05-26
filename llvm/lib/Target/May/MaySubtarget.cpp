#include "MaySubtarget.h"
#include "May.h"
#include "llvm/Target/TargetMachine.h"

using namespace llvm;

#define DEBUG_TYPE "may-subtarget"

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "MayGenSubtargetInfo.inc"

MaySubtarget::MaySubtarget(const StringRef &CPU, const StringRef &TuneCPU,
                             const StringRef &FS, const TargetMachine &TM)
    : MayGenSubtargetInfo(TM.getTargetTriple(), CPU, TuneCPU, FS) {
  MAY_DUMP_CYAN
}