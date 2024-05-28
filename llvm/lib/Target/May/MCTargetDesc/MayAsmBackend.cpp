//===-- MayAsmBackend.cpp - May Assembler Backend ---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/MayMCTargetDesc.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCValue.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/EndianStream.h"
#include "MCTargetDesc/MayFixupKinds.h"

using namespace llvm;

namespace {
class MayAsmBackend : public MCAsmBackend {
protected:
  const Target &TheTarget;

public:
  MayAsmBackend(const Target &T)
      : MCAsmBackend(llvm::endianness::little), TheTarget(T) {}

  unsigned getNumFixupKinds() const override {
    return May::NumTargetFixupKinds;
  }
  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override {

    const static MCFixupKindInfo InfosLE[May::NumTargetFixupKinds] = {
        // name                offset bits  flags
        { "fixup_May_PC16",     0,     16,  MCFixupKindInfo::FKF_IsPCRel },
    };

    // Fixup kinds from .reloc directive are like R_SPARC_NONE. They do
    // not require any extra processing.
    if (Kind >= FirstLiteralRelocationKind)
      return MCAsmBackend::getFixupKindInfo(FK_NONE);

    if (Kind < FirstTargetFixupKind)
      return MCAsmBackend::getFixupKindInfo(Kind);

    assert(unsigned(Kind - FirstTargetFixupKind) < getNumFixupKinds() &&
           "Invalid kind!");
    return InfosLE[Kind - FirstTargetFixupKind];
  }

  /// fixupNeedsRelaxation - Target specific predicate for whether a given
  /// fixup requires the associated instruction to be relaxed.
  bool fixupNeedsRelaxation(const MCFixup &Fixup, uint64_t Value,
                            const MCRelaxableFragment *DF,
                            const MCAsmLayout &Layout) const override {
    llvm_unreachable("fixupNeedsRelaxation() unimplemented");
    return false;
  }

  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override {
    // Cannot emit NOP with size not multiple of 32 bits.
    OS.write_zeros(Count);
    return true;
  }
};

class ELFMayAsmBackend : public MayAsmBackend {
  Triple::OSType OSType;

public:
  ELFMayAsmBackend(const Target &T, Triple::OSType OSType)
      : MayAsmBackend(T), OSType(OSType) {}

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override {
    unsigned NumBytes = 0;
    switch (Fixup.getKind()) {
    default:
      return;
    case May::fixup_May_PC16:
      // Forcing a signed division because Value can be negative.
      Value /= 4;
      NumBytes = 2;
      break;
    }

    unsigned Offset = Fixup.getOffset();
    // For each byte of the fragment that the fixup touches, mask in the bits
    // from the fixup value. The Value has been "split up" into the
    // appropriate bitfields above.
    for (unsigned I = 0; I != NumBytes; ++I) {
      Data[Offset + I] |= uint8_t((Value >> (I * 8)) & 0xff);
    }
    return;
  }

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override {
    uint8_t OSABI = MCELFObjectTargetWriter::getOSABI(OSType);
    return createMayELFObjectWriter(false, OSABI);
  }
};

} // end anonymous namespace

MCAsmBackend *llvm::createMayAsmBackend(const Target &T,
                                         const MCSubtargetInfo &STI,
                                         const MCRegisterInfo &MRI,
                                         const MCTargetOptions &Options) {
  return new ELFMayAsmBackend(T, STI.getTargetTriple().getOS());
}