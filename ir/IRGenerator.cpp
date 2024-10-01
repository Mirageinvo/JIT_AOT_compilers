#include "IRGenerator.hpp"
#include "basic_block.hpp"
#include "graph.hpp"

namespace IRGen {

void IRGenerator::CreateGraph()
{
    if (graph_ == nullptr) {
        graph_ = new Graph();
    }
}
BB* IRGenerator::CreateEmptyBB()
{
    auto* bb = new BB();
    BBs_.push_back(bb);
    if (graph_ != nullptr) {
        graph_->AddBB(bb, graph_->GetBBCounter());
        graph_->IncreaseBBCounter();  // increasing number of created basic blocks
    }
    return bb;
}
void IRGenerator::Clear()
{
    for (auto* bb : BBs_) {
        delete bb;
    }
    BBs_.clear();

    for (auto* instr : instructions_) {
        delete instr;
    }
    instructions_.clear();

    delete graph_;
    graph_ = nullptr;
}

}  // namespace IRGen