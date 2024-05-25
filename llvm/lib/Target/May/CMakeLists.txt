add_llvm_component_group(May)

set(LLVM_TARGET_DEFINITIONS May.td)

tablegen(LLVM MayGenRegisterInfo.inc -gen-register-info)

add_public_tablegen_target(MayCommonTableGen)

add_llvm_target(MayCodeGen
        MayTargetMachine.cpp
        MayRegisterInfo.cpp

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
