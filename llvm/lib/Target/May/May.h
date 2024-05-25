#ifndef LLVM_LIB_TARGET_May_May_H
#define LLVM_LIB_TARGET_May_May_H

#include "MCTargetDesc/MayMCTargetDesc.h"
#include "llvm/Support/raw_ostream.h"

#define MAY_DUMP(Color)                                                        \
  {                                                                            \
    llvm::errs().changeColor(Color)                                            \
        << __func__ << "\n\t\t" << __FILE__ << ":" << __LINE__ << "\n";        \
    llvm::errs().changeColor(llvm::raw_ostream::WHITE);                        \
  }
// #define MAY_DUMP(Color) {}
#define MAY_DUMP_RED MAY_DUMP(llvm::raw_ostream::RED)
#define MAY_DUMP_GREEN MAY_DUMP(llvm::raw_ostream::GREEN)
#define MAY_DUMP_YELLOW MAY_DUMP(llvm::raw_ostream::YELLOW)
#define MAY_DUMP_CYAN MAY_DUMP(llvm::raw_ostream::CYAN)
#define MAY_DUMP_MAGENTA MAY_DUMP(llvm::raw_ostream::MAGENTA)
#define MAY_DUMP_WHITE MAY_DUMP(llvm::raw_ostream::WHITE)

#endif // LLVM_LIB_TARGET_May_May_H