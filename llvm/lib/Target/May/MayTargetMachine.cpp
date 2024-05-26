//===----------------------------------------------------------------------===//
//
// Implements the info about May target spec.
//
//===----------------------------------------------------------------------===//

#include "MayTargetMachine.h"
#include "May.h"
#include "TargetInfo/MayTargetInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/Attributes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Target/TargetOptions.h"

#define DEBUG_TYPE "May"

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMayTarget() {
  // Register the target.
  MAY_DUMP_CYAN
  RegisterTargetMachine<MayTargetMachine> A(getTheMayTarget());
}

MayTargetMachine::MayTargetMachine(const Target &T, const Triple &TT,
                                     StringRef CPU, StringRef FS,
                                     const TargetOptions &Options,
                                     std::optional<Reloc::Model> RM,
                                     std::optional<CodeModel::Model> CM,
                                     CodeGenOptLevel OL, bool JIT)
    : LLVMTargetMachine(T, "e-m:e-p:32:32-i8:8:32-i16:16:32-i64:64-n32", TT,
                        CPU, FS, Options, Reloc::Static,
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, std::string(CPU), std::string(FS), *this) {
  initAsmInfo();
}

MayTargetMachine::~MayTargetMachine() = default;

namespace {

/// May Code Generator Pass Configuration Options.
class MayPassConfig : public TargetPassConfig {
public:
  MayPassConfig(MayTargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  MayTargetMachine &getMayTargetMachine() const {
    return getTM<MayTargetMachine>();
  }

  bool addInstSelector() override;
};

} // end anonymous namespace

TargetPassConfig *MayTargetMachine::createPassConfig(PassManagerBase &PM) {
  return new MayPassConfig(*this, PM);
}

bool MayPassConfig::addInstSelector() {
  addPass(createMayISelDag(getMayTargetMachine()));
  return false;
}