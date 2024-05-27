#ifndef LLVM_LIB_TARGET_MAY_MCTARGETDESC_MAYMCTARGETDESC_H
#define LLVM_LIB_TARGET_MAY_MCTARGETDESC_MAYMCTARGETDESC_H

// Defines symbolic names for May registers.  This defines a mapping from
// register name to register number.
//
#define GET_REGINFO_ENUM
#include "MayGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "MayGenInstrInfo.inc"

#endif

#include <memory>

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class MCTargetOptions;
class Target;

MCCodeEmitter *createMayMCCodeEmitter(const MCInstrInfo &MCII, MCContext &Ctx);
MCAsmBackend *createMayAsmBackend(const Target &T, const MCSubtargetInfo &STI,
                                   const MCRegisterInfo &MRI,
                                   const MCTargetOptions &Options);
std::unique_ptr<MCObjectTargetWriter> createMayELFObjectWriter(bool Is64Bit,
                                                                uint8_t OSABI);
} // namespace llvm
