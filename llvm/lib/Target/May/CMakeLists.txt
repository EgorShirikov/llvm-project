add_llvm_component_group(May)

set(LLVM_TARGET_DEFINITIONS May.td)

tablegen(LLVM MayGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM MayGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM MayGenSubtargetInfo.inc -gen-subtarget)
tablegen(LLVM MayGenCallingConv.inc -gen-callingconv)
tablegen(LLVM MayGenMCPseudoLowering.inc -gen-pseudo-lowering)
tablegen(LLVM MayGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM MayGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM MayGenMCCodeEmitter.inc -gen-emitter)

add_public_tablegen_target(MayCommonTableGen)

add_llvm_target(MayCodeGen
        MayTargetMachine.cpp
        MayRegisterInfo.cpp
        MayInstrInfo.cpp
        MaySubtarget.cpp
        MayAsmPrinter.cpp
        MayFrameLowering.cpp
        MayISelDagToDag.cpp
        MayISelLowering.cpp
        MayMCInstrLower.cpp

        LINK_COMPONENTS
        Analysis
        AsmPrinter
        CodeGen
        Core
        Support
        Target
        MayInfo
        MC
        MayDesc
        SelectionDAG
        GlobalISel

        ADD_TO_COMPONENT
        May
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
