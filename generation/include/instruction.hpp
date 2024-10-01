#ifndef GENERATION_INCLUDE_INSTRUCTION_HPP
#define GENERATION_INCLUDE_INSTRUCTION_HPP

#include "instruction_info.hpp"
#include "defines.hpp"

namespace IRGen {
class BB;

class Instruction {
public:
    Instruction(InstructionOpcode opcode, InstructionType type)
        : opcode_(opcode), type_(type) {}

    NO_COPY_AND_MOVE_SEMANTICS(Instruction);
    virtual ~Instruction() = 0;

    Instruction *GetPrevInstr() const { return prev_; }
    Instruction *GetNextInstr() const { return next_; }
    BB *GetLinkedBB() const { return linkedBB_; }
    InstructionOpcode GetOpcode() const { return opcode_; }

    void SetPrevInstr(Instruction *instr) { prev_ = instr; }
    void SetNextInstr(Instruction *instr) { next_ = instr; }
    void SetLinkedBB(BB *bb) { linkedBB_ = bb; }
    void Finalization();
    void InsertBefore(Instruction *toInsert);
    void InsertAfter(Instruction *toInsert);

	// virtual std::string toString() { return /*IRGen::toString(opcode_) + std::string(".") + IRGen::toString(type_);*/""; };
	std::string RegToString() { return /*IRGen::typeToReg(type_);*/"";}

protected:
    InstructionOpcode opcode_;
    InstructionType type_;

private:
    Instruction *prev_ = nullptr;
    Instruction *next_ = nullptr;
    BB *linkedBB_ = nullptr;
};
}

#endif // GENERATION_INCLUDE_INSTRUCTION_HPP
