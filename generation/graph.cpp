#include "graph.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

namespace IRGen {
void Graph::AddBB(BB* bb, uint64_t id)
{
    if (bb == nullptr) {
        throw std::runtime_error("BasicBlock is nullptr");
    }

    bb->SetId(id);
    bb->SetGraph(this);
    BBs_.push_back(bb);
}

void Graph::DeleteBB(BB* bb)
{
    if (bb == nullptr) {
        throw std::runtime_error("BasicBlock is nullptr");
    }

    auto bb_iter = std::find(BBs_.begin(), BBs_.end(), bb);
    if (bb_iter == BBs_.end()) {
        throw std::runtime_error("There is no such BasicBlock in current graph");
    }
    BBs_.erase(bb_iter);
    if (bb == lastBB_) {
        SetLastBB(nullptr);
    }
    // DeletePreds(bb);
    // DeleteSuccs(bb);
}

void Graph::AddPredecessorBB(BB* newBB, BB* toBB)
{
    CheckConsistency(newBB, toBB);

    newBB->SetGraph(this);
    toBB->AddPred(newBB);
    newBB->AddSucc(toBB);
}

void Graph::AddSuccessorBB(BB* newBB, BB* toBB)
{
    CheckConsistency(newBB, toBB);

    newBB->SetGraph(this);
    toBB->AddSucc(newBB);
    newBB->AddPred(toBB);
}

void Graph::Print()
{
    std::cout << name_ << " " << typeToNameConverter[static_cast<size_t>(returnType_)] << "(";
    for (const auto& param : params_) {
        std::cout << typeToNameConverter[static_cast<size_t>(param.first)] << " " << param.second << " ";
    }
    std::cout << ")" << std::endl;
    for (const auto& bb : BBs_) {
        bb->Print();
    }
}

void Graph::CheckConsistency(BB* newBB, BB* toBB)
{
    if (newBB == nullptr || toBB == nullptr) {
        throw std::runtime_error("BasicBlock is nullptr");
    }
    if ((newBB->GetGraph() && newBB->GetGraph() != this) || toBB->GetGraph() != this) {
        throw std::runtime_error(std::string("BasicBlocks' graph inconsistency"));
    }
}
}  // namespace IRGen
