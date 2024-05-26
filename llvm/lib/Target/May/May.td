include "llvm/Target/Target.td"
include "MayRegisterInfo.td"
include "MayInstrInfo.td"

def MayInstrInfo : InstrInfo;

def May : Target{
    let InstructionSet = MayInstrInfo;
}