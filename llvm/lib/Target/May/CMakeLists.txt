add_llvm_component_group(May)

set(LLVM_TARGET_DEFINITIONS May.td)

tablegen(LLVM MayGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM MayGenInstrInfo.inc -gen-instr-info)

add_public_tablegen_target(MayCommonTableGen)

add_llvm_target(MayCodeGen
        MayTargetMachine.cpp
        MayRegisterInfo.cpp
        MayInstrInfo.cpp

        LINK_COMPONENTS
        CodeGen
        Core
        Support
        Target
        MayInfo

        ADD_TO_COMPONENT
        May
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
