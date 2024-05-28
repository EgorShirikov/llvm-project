let TargetPrefix = "may" in {  // All intrinsics start with "llvm.may.".

def int_may_putpixel: ClangBuiltin<"simPutPixel">,
  Intrinsic<[], [llvm_i32_ty, llvm_i32_ty, llvm_i32_ty], [IntrNoMem, IntrHasSideEffects]>;

def int_may_flush: ClangBuiltin<"simFlush">,
  Intrinsic<[], [], [IntrNoMem, IntrHasSideEffects]>;

def int_may_init: ClangBuiltin<"simInit">,
  Intrinsic<[], [], [IntrNoMem, IntrHasSideEffects]>;

def int_may_exit: ClangBuiltin<"simExit">,
  Intrinsic<[], [], [IntrNoMem, IntrHasSideEffects]>;

def int_may_event: ClangBuiltin<"simEvent">,
  Intrinsic<[], [], [IntrNoMem, IntrHasSideEffects]>;

} // TargetPrefix = "may"