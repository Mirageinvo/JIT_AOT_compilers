#include "IRGenerator.hpp"
#include "graph.hpp"
#include <iostream>

void GenerateFactorial()
{
    try {
        IRGen::IRGenerator generator;
        generator.CreateGraph();
        auto* g = generator.GetGraph();
        g->SetName("Factorial");
        g->SetReturnType(IRGen::InstructionType::U64);
        g->SetParams({std::make_pair(IRGen::InstructionType::U32, 0)});
        auto* b1 = generator.CreateEmptyBB();
        auto* b2 = generator.CreateEmptyBB();
        auto* b3 = generator.CreateEmptyBB();

        b1->AddInstrBack(generator.BuildMovI(IRGen::InstructionType::U64, 0, uint64_t(1)));
        b1->AddInstrBack(generator.BuildMovI(IRGen::InstructionType::U64, 1, uint64_t(2)));
        b1->AddInstrBack(generator.BuildCast(IRGen::InstructionType::U64, 2, IRGen::InstructionType::U32, 0));

        b2->AddInstrBack(generator.BuildCmp(IRGen::InstructionType::U64, 1, 2));
        b2->AddInstrBack(generator.BuildJa(b3));
        b2->AddInstrBack(generator.BuildMulI(IRGen::InstructionType::U64, 0, 0, 1));
        b2->AddInstrBack(generator.BuildAddI(IRGen::InstructionType::U64, 1, 1, uint64_t(1)));
        b2->AddInstrBack(generator.BuildJump(b2));

        b3->AddInstrBack(generator.BuildRet(IRGen::InstructionType::U64, 0));

        g->Print();
    } catch (const std::exception& e) {
        std::cerr << "Caught an exception: " << e.what() << std::endl;
    }
}

int main()
{
    GenerateFactorial();
}
