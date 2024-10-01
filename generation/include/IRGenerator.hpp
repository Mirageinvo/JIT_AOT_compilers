#ifndef GENERATION_INCLUDE_IRBUILDER_HPP
#define GENERATION_INCLUDE_IRBUILDER_HPP

#include "defines.hpp"
#include "instructions_impl.hpp"
#include <vector>

namespace IRGen {

class IRGenerator {
public:
    IRGenerator() {}
    ~IRGenerator()
    {
        Clear();
    }
    void CreateGraph();
    BB* CreateEmptyBB();
    Graph* GetGraph() const
    {
        return graph_;
    }
    void Clear();
    void PushBackInstr(BB* bb, Instruction* instr)
    {
        bb->AddInstrBack(instr);
    }
    void PushFrontInst(BB* bb, Instruction* instr)
    {
        bb->AddInstrFront(instr);
    }

    template <typename T>
    RegImmInstr<T>* BuildMovI(InstructionType type, int reg, T imm)
    {
        auto* inst = new RegImmInstr(InstructionOpcode::MOVI, type, reg, imm);
        instructions_.push_back(inst);
        return inst;
    }

    ThreeRegInstr* BuildMulI(InstructionType type, int reg1, int reg2, int reg3)
    {
        auto* inst = new ThreeRegInstr(InstructionOpcode::MULI, type, reg1, reg2, reg3);
        instructions_.push_back(inst);
        return inst;
    }

    template <typename T>
    TwoRegImmInstr<T>* BuildAddI(InstructionType type, int reg1, int reg2, T imm)
    {
        auto* inst = new TwoRegImmInstr(InstructionOpcode::ADDI, type, reg1, reg2, imm);
        instructions_.push_back(inst);
        return inst;
    }

    TwoRegInstr* BuildCmp(InstructionType type, int reg1, int reg2)
    {
        auto* inst = new TwoRegInstr(InstructionOpcode::CMP, type, reg1, reg2);
        instructions_.push_back(inst);
        return inst;
    }

    JumpInstr* BuildJump(BB* dest)
    {
        auto* inst = new JumpInstr(InstructionOpcode::JMP, InstructionType::U64, dest);
        instructions_.push_back(inst);
        return inst;
    }

    JumpInstr* BuildJa(BB* dest)
    {
        auto* inst = new JumpInstr(InstructionOpcode::JA, InstructionType::U64, dest);
        instructions_.push_back(inst);
        return inst;
    }

    OneRegInstr* BuildRet(InstructionType type, int reg)
    {
        auto* inst = new OneRegInstr(InstructionOpcode::RET, type, reg);
        instructions_.push_back(inst);
        return inst;
    }

    CastInstr* BuildCast(InstructionType type1, int reg1, InstructionType type2, int reg2)
    {
        auto* inst = new CastInstr(InstructionOpcode::CAST, type1, reg1, type2, reg2);
        instructions_.push_back(inst);
        return inst;
    }

private:
    std::vector<BB*> BBs_;
    std::vector<Instruction*> instructions_;
    Graph* graph_ = nullptr;
};
}  // namespace IRGen

#endif  // GENERATION_INCLUDE_IRBUILDER_HPP
