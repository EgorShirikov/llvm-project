add_llvm_component_library(LLVMMayDesc
        MayMCTargetDesc.cpp
        MayInstPrinter.cpp
        MayMCAsmInfo.cpp

        LINK_COMPONENTS
        MC
        Support
        MayInfo

        ADD_TO_COMPONENT
        May
)