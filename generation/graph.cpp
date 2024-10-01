#include <stdexcept>
#include <string>
#include <iostream>
#include "graph.hpp"

namespace IRGen {
void Graph::AddBB(BB *bb, uint64_t id) {
    if (bb == nullptr) {
        throw std::runtime_error("BasicBlock is nullptr");
    }

    bb->SetId(id);
    BBs_.push_back(bb);
    bb->SetGraph(this);
}

void Graph::DeleteBB(BB *bb) {
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
    //DeletePreds(bb);
    //DeleteSuccs(bb);
}

void Graph::AddPredecessorBB(BB *newBB, BB *toBB) {
	CheckConsistency(newBB, toBB);

    newBB->SetGraph(this);
    toBB->AddPred(newBB);
	newBB->AddSucc(toBB);
}

void Graph::AddSuccessorBB(BB *newBB, BB *toBB) {
	CheckConsistency(newBB, toBB);

    newBB->SetGraph(this);
    toBB->AddSucc(newBB);
	newBB->AddPred(toBB);
}

void Graph::Print() {
	// std::cout << IRGen::toString(retType_) << " " << name_ << "(";
	// std::for_each(params_.begin(), params_.end(), [](const auto &p){
	// 	std::cout << IRGen::toString(p.first) << " " << IRGen::typeToReg(p.first) << p.second << " ";
	// });
	// std::cout << ")" << std::endl;
	// std::for_each(BBs_.begin(), BBs_.end(), [](auto *b){b->Dump();});
}

void Graph::CheckConsistency(BB *newBB, BB *toBB) {
    if (!newBB || !toBB) {
        throw std::runtime_error("BasicBlock is nullptr");
    }
    if ((newBB->GetGraph() && newBB->GetGraph() != this) || toBB->GetGraph() != this) {
        throw std::runtime_error(std::string("BasicBlocks' graph inconsistency"));
    }
}
}
