#include "May.h"
#include "TargetInfo/MayTargetInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "MayMCTargetDesc.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "MayMCAsmInfo.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/Support/ErrorHandling.h"
#include "MayInfo.h"
#include "MayInstPrinter.h"
#include "MayTargetStreamer.h"


using namespace llvm;

#define GET_REGINFO_MC_DESC
#include "MayGenRegisterInfo.inc"

#define GET_INSTRINFO_MC_DESC
#include "MayGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "MayGenSubtargetInfo.inc"

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

static MCSubtargetInfo *createMayMCSubtargetInfo(const Triple &TT,
                                                  StringRef CPU, StringRef FS) {
  MAY_DUMP_MAGENTA
  return createMayMCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
}

static MCAsmInfo *createMayMCAsmInfo(const MCRegisterInfo &MRI,
                                     const Triple &TT,
                                     const MCTargetOptions &Options) {
  MAY_DUMP_MAGENTA
  MCAsmInfo *MAI = new MayELFMCAsmInfo(TT);
  unsigned SP = MRI.getDwarfRegNum(May::R1, true);
  MCCFIInstruction Inst = MCCFIInstruction::cfiDefCfa(nullptr, SP, 0);
  MAI->addInitialFrameState(Inst);
  return MAI;
}

static MCInstPrinter *createMayMCInstPrinter(const Triple &T,
                                             unsigned SyntaxVariant,
                                             const MCAsmInfo &MAI,
                                             const MCInstrInfo &MII,
                                             const MCRegisterInfo &MRI) {
  return new MayInstPrinter(MAI, MII, MRI);
}

MayTargetStreamer::MayTargetStreamer(MCStreamer &S) : MCTargetStreamer(S) {}
MayTargetStreamer::~MayTargetStreamer() = default;

static MCTargetStreamer *createTargetAsmStreamer(MCStreamer &S,
                                                 formatted_raw_ostream &OS,
                                                 MCInstPrinter *InstPrint,
                                                 bool isVerboseAsm) {
  return new MayTargetStreamer(S);
}


extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeMayTargetMC() {
  MAY_DUMP_MAGENTA
  Target &TheMayTarget = getTheMayTarget();
  RegisterMCAsmInfoFn X(TheMayTarget, createMayMCAsmInfo);
  // Register the MC register info.
  TargetRegistry::RegisterMCRegInfo(TheMayTarget, createMayMCRegisterInfo);
  TargetRegistry::RegisterMCInstrInfo(TheMayTarget, createMayMCInstrInfo);
  TargetRegistry::RegisterMCSubtargetInfo(TheMayTarget, createMayMCSubtargetInfo);
  // Register the MCInstPrinter
  TargetRegistry::RegisterMCInstPrinter(TheMayTarget, createMayMCInstPrinter);
  TargetRegistry::RegisterAsmTargetStreamer(TheMayTarget,
                                            createTargetAsmStreamer);
}


