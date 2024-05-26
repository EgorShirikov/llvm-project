include "llvm/Target/Target.td"
include "MayRegisterInfo.td"
include "MayInstrInfo.td"

class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : Proc<"generic", []>;

def MayInstrInfo : InstrInfo;

def May : Target{
    let InstructionSet = MayInstrInfo;
}