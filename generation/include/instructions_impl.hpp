#ifndef GENERATION_INCLUDE_INSTRUCTION_DEF_HPP
#define GENERATION_INCLUDE_INSTRUCTION_DEF_HPP

#include <string>
#include "instruction.hpp"
#include "basic_block.hpp"

namespace IRGen {
template<typename T>
class RegImmInstr: public Instruction {
public:
	RegImmInstr(InstructionOpcode op, InstructionType type, int reg, T imm): Instruction(op, type), reg_(reg), imm_(imm) {
	}
	std::string AsString() {
		return Instruction::toString() + " " + RegToString() + std::to_string(reg_) + " " + std::to_string(imm_);
	}

private:
	int reg_;
	T imm_;
};

class OneRegInstr: public Instruction {
public:
	OneRegInstr(InstructionOpcode op, InstructionType type, int reg): Instruction(op, type), reg_(reg) {
	}

	std::string AsString() {
		return Instruction::toString() + " " + RegToString() + std::to_string(reg_);
	}

private:
	int reg_;
};


class CastInstr: public Instruction {
public:
	CastInstr(InstructionOpcode op, InstructionType type1, int reg1, InstructionType type2, int reg2): Instruction(op, type1), reg1_(reg1)
	, reg2_(reg2), type2_(type2) {
	}

	std::string AsString() {
		return Instruction::toString() + "from" + IRGen::toString(type2_) + " " + RegToString() + std::to_string(reg1_) + 
			" " + IRGen::typeToReg(type2_) + std::to_string(reg2_);
	}

private:
	int reg1_;
	int reg2_;
	InstructionType type2_;
};

class TwoRegInstr: public Instruction {
public:
	TwoRegInstr(InstructionOpcode op, InstructionType type, int reg1, int reg2): Instruction(op, type), reg1_(reg1), reg2_(reg2) {
	}
	std::string AsString() {
		return Instruction::toString() + " " + RegToString() + std::to_string(reg1_) + " " + RegToString() + std::to_string(reg2_);
	}

private:
	int reg1_;
	int reg2_;
};


class ThreeRegInstr: public Instruction {
public:
	ThreeRegInstr(InstructionOpcode op, InstructionType type, int reg1, int reg2, int reg3): Instruction(op, type), 
		reg1_(reg1), reg2_(reg2), reg3_(reg3) {
	}
	std::string AsString() {
		return Instruction::toString() + " " + RegToString() + std::to_string(reg1_) + " " + RegToString() + std::to_string(reg2_) +" " + RegToString() + std::to_string(reg3_);
	}

private:
	int reg1_;
	int reg2_;
	int reg3_;
};

template<typename T>
class TwoRegImmInstr: public Instruction {
public:
	TwoRegImmInstr(InstructionOpcode op, InstructionType type, int reg1, int reg2, T imm): Instruction(op, type), 
		reg1_(reg1), reg2_(reg2), imm_(imm) {
	}
	
	std::string AsString() {
		return Instruction::toString() + " " + RegToString() + std::to_string(reg1_) + " " + RegToString() + std::to_string(reg2_) + " " + std::to_string(imm_);
	}

private:
	int reg1_;
	int reg2_;
	T imm_;
};

class JumpInstr: public Instruction {
public:
	JumpInstr(InstructionOpcode op, InstructionType type, BB *out): Instruction(op, type), out_(out){
	}

	std::string AsString() {
		return IRGen::toString(InstructionOpcode_) + std::string(" ") + std::to_string(out_->GetId());
	}

private:
	BB *out_;
};
}

#endif // GENERATION_INCLUDE_INSTRUCTION_DEF_HPP
