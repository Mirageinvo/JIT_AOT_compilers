#include <iostream>
#include "IRBuilder.hpp"
#include "graph.hpp"

void GenerateFactorial() {
	try {
		IRGen::InstructionBuilder builder;
		IRGen::IRGenerator gen;
		gen.CreateGraph();
		IRGen::Graph *g = gen.GetGraph();
		g->SetName("Factorial");
		g->SetRetType(IRGen::InstrType::U64);
		g->SetParams({std::make_pair(IRGen::InstrType::U32, 0)});
		IRGen::BB *b1 = gen.CreateEmptyBB();
		b1->AddInstrBackward(builder.BuildMovI(IRGen::InstrType::U64, 0, uint64_t(1)));
		b1->AddInstrBackward(builder.BuildMovI(IRGen::InstrType::U64, 1, uint64_t(2)));
		b1->AddInstrBackward(builder.BuildCast(IRGen::InstrType::U64, 2, IRGen::InstrType::U32, 0));

		IRGen::BB *b2 = gen.CreateEmptyBB();
		IRGen::BB *b3 = gen.CreateEmptyBB();
		b2->AddInstrBackward(builder.BuildCmp(IRGen::InstrType::U64, 1, 2));
		b2->AddInstrBackward(builder.BuildJa(b3));
		b2->AddInstrBackward(builder.BuildMul(IRGen::InstrType::U64, 0, 0, 1));
		b2->AddInstrBackward(builder.BuildAddI(IRGen::InstrType::U64, 1, 1, uint64_t(1)));
		b2->AddInstrBackward(builder.BuildJump(b2));
		b3->AddInstrBackward(builder.BuildRet(IRGen::InstrType::U64, 0));

		g->Dump();
	} catch(const std::exception& e) {
    	std::cerr << "Exception: " << e.what() << std::endl;
	}
}

int main() {
	GenerateFactorial();
}
