#ifndef LLVM_LIB_TARGET_MAY_MCTARGETDESC_MAYFIXUPKINDS_H
#define LLVM_LIB_TARGET_MAY_MCTARGETDESC_MAYFIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

namespace llvm {
namespace May {
// Although most of the current fixup types reflect a unique relocation
// one can have multiple fixup types for a given relocation and thus need
// to be uniquely named.
//
// This table *must* be in the same order of
// MCFixupKindInfo Infos[May::NumTargetFixupKinds]
// in MayAsmBackend.cpp.
//
enum Fixups {
  fixup_May_PC16 = FirstTargetFixupKind,
  // Marker
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};
} // namespace May
} // namespace llvm


#endif