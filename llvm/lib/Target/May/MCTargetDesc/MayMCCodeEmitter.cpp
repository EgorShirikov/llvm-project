//===-- MayMCCodeEmitter.cpp - Convert May code to machine code -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the MayMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include "../../../../../mlir/include/mlir/ExecutionEngine/SparseTensor/ErrorHandling.h"
#include "../../../../include/llvm/ADT/SmallVector.h"
#include "../../../../include/llvm/ADT/Statistic.h"
#include "../../../../include/llvm/MC/MCAsmInfo.h"
#include "../../../../include/llvm/MC/MCCodeEmitter.h"
#include "../../../../include/llvm/MC/MCContext.h"
#include "../../../../include/llvm/MC/MCExpr.h"
#include "../../../../include/llvm/MC/MCFixup.h"
#include "../../../../include/llvm/MC/MCInst.h"
#include "../../../../include/llvm/MC/MCInstrInfo.h"
#include "../../../../include/llvm/MC/MCObjectFileInfo.h"
#include "../../../../include/llvm/MC/MCRegisterInfo.h"
#include "../../../../include/llvm/MC/MCSubtargetInfo.h"
#include "../../../../include/llvm/MC/MCSymbol.h"
#include "../../../../include/llvm/Support/Casting.h"
#include "../../../../include/llvm/Support/Endian.h"
#include "../../../../include/llvm/Support/EndianStream.h"
#include "../../../../include/llvm/Support/raw_ostream.h"
#include "../../../../include/llvm/TargetParser/SubtargetFeature.h"
#include "../May.h"
#include "MayMCTargetDesc.h"
#include <cassert>
#include <cstdint>

using namespace llvm;

#define DEBUG_TYPE "mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");

namespace {

class MayMCCodeEmitter : public MCCodeEmitter {
  MCContext &Ctx;

public:
  MayMCCodeEmitter(const MCInstrInfo &, MCContext &ctx)
      : Ctx(ctx) {}
  MayMCCodeEmitter(const MayMCCodeEmitter &) = delete;
  MayMCCodeEmitter &operator=(const MayMCCodeEmitter &) = delete;
  ~MayMCCodeEmitter() override = default;

  void encodeInstruction(const MCInst &MI, SmallVectorImpl<char> &CB,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;

  // getBinaryCodeForInstr - TableGen'erated function for getting the
  // binary encoding for an instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  /// getMachineOpValue - Return binary encoding of operand. If the machine
  /// operand requires relocation, record the relocation and return zero.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;
  unsigned getMImm16OpValue(const MCInst &MI, unsigned OpNo,
                            SmallVectorImpl<MCFixup> &Fixups,
                            const MCSubtargetInfo &STI) const;
};

} // end anonymous namespace

void MayMCCodeEmitter::encodeInstruction(const MCInst &MI,
                                          SmallVectorImpl<char> &CB,
                                          SmallVectorImpl<MCFixup> &Fixups,
                                          const MCSubtargetInfo &STI) const {
  unsigned Bits = getBinaryCodeForInstr(MI, Fixups, STI);
  support::endian::write(CB, Bits, llvm::endianness::little);

  ++MCNumEmitted;  // Keep track of the # of mi's emitted.
}

unsigned MayMCCodeEmitter::
    getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                      SmallVectorImpl<MCFixup> &Fixups,
                      const MCSubtargetInfo &STI) const {
  if (MO.isReg())
    return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg());

  if (MO.isImm())
    return MO.getImm();

  assert(MO.isExpr());
  const MCExpr *Expr = MO.getExpr();

  int64_t Res;
  if (Expr->evaluateAsAbsolute(Res))
    return Res;

  llvm_unreachable("Unhandled expression!");
  return 0;
}

unsigned
MayMCCodeEmitter::getMImm16OpValue(const MCInst &MI, unsigned OpNo,
                                    SmallVectorImpl<MCFixup> &Fixups,
                                    const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);
  if (MO.isImm())
    return MO.getImm();

  assert(MO.isExpr() &&
         "getMImm16OpValue expects only expressions or an immediate");

  const MCExpr *Expr = MO.getExpr();

  // Constant value, no fixup is needed
  if (const MCConstantExpr *CE = dyn_cast<MCConstantExpr>(Expr))
    return CE->getValue();

  return 0;
}

#include "MayGenMCCodeEmitter.inc"

MCCodeEmitter *llvm::createMayMCCodeEmitter(const MCInstrInfo &MCII,
                                             MCContext &Ctx) {
  return new MayMCCodeEmitter(MCII, Ctx);
}