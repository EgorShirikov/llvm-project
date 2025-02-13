//===-- MayELFObjectWriter.cpp - May ELF Writer -----------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/MayMCTargetDesc.h"
#include "May.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
class MayELFObjectWriter : public MCELFObjectTargetWriter {
public:
  MayELFObjectWriter(bool Is64Bit, uint8_t OSABI)
      : MCELFObjectTargetWriter(Is64Bit, OSABI, ELF::EM_MAY,
                                /*HasRelocationAddend*/ true) {}

  ~MayELFObjectWriter() override = default;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override;

  bool needsRelocateWithSymbol(const MCValue &Val, const MCSymbol &Sym,
                               unsigned Type) const override;
};
} // namespace

unsigned MayELFObjectWriter::getRelocType(MCContext &Ctx, const MCValue &Target,
                                           const MCFixup &Fixup,
                                           bool IsPCRel) const {
  MCFixupKind Kind = Fixup.getKind();
  if (Kind >= FirstLiteralRelocationKind)
    return Kind - FirstLiteralRelocationKind;

  llvm_unreachable("Unimplemented fixup -> relocation");
}

bool MayELFObjectWriter::needsRelocateWithSymbol(const MCValue &,
                                                  const MCSymbol &,
                                                  unsigned Type) const {
  return false;
}

std::unique_ptr<MCObjectTargetWriter>
llvm::createMayELFObjectWriter(bool Is64Bit, uint8_t OSABI) {
  return std::make_unique<MayELFObjectWriter>(Is64Bit, OSABI);
}