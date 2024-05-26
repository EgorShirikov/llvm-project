#ifndef LLVM_LIB_TARGET_MAY_MAYTARGETSTREAMER_H
#define LLVM_LIB_TARGET_MAY_MAYTARGETSTREAMER_H

#include "llvm/MC/MCStreamer.h"

namespace llvm {

class MayTargetStreamer : public MCTargetStreamer {
public:
  MayTargetStreamer(MCStreamer &S);
  ~MayTargetStreamer() override;
};

} // end namespace llvm

#endif // LLVM_LIB_TARGET_MAY_MAYTARGETSTREAMER_H