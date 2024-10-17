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
    toBB->AddPredecessor(newBB);
    newBB->AddSuccessor(toBB);
}

void Graph::AddSuccessorBB(BB* newBB, BB* toBB)
{
    CheckConsistency(newBB, toBB);

    newBB->SetGraph(this);
    toBB->AddSuccessor(newBB);
    newBB->AddPredecessor(toBB);
}

void Graph::Print() const
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

std::unordered_map<BB*, std::set<BB*>> Graph::BuildDominatorTree()
{
    std::set<uint64_t> visited;
#ifndef NDEBUG
    Dfs(firstBB_, visited);
    if (visited.size() != BBs_.size()) {
        throw std::runtime_error("Graph is inconsistent. Some BasicBlocks are unreachable");
    }
#endif

    std::unordered_map<BB*, std::set<BB*>> dominatorTree;
    for (auto* dominator_bb : BBs_) {
        assert(dominator_bb != nullptr);
        visited.clear();
        Dfs(firstBB_, visited, dominator_bb);
        std::set<BB*> dominated;
        for (auto* bb : BBs_) {
            if (bb != dominator_bb && visited.find(bb->GetId()) == visited.end()) {
                dominated.insert(bb);
            }
        }
        dominatorTree[dominator_bb] = dominated;
    }

    // Now we need to remove all unnecessary edges
    for (auto& pair : dominatorTree) {
        const std::set<BB*>& dominated = pair.second;
        std::set<BB*> saved_bbs = dominated;
        for (const auto& dominated_bb : dominated) {
            const std::set<BB*>& dominated_by_dominated = dominatorTree[dominated_bb];
            for (const auto& block_to_check : dominated_by_dominated) {
                if (saved_bbs.find(block_to_check) != saved_bbs.end()) {
                    saved_bbs.erase(block_to_check);
                }
            }
        }
        pair.second = saved_bbs;
    }

    return dominatorTree;
}

void Graph::CheckConsistency(BB* newBB, BB* toBB) const
{
    if (newBB == nullptr || toBB == nullptr) {
        throw std::runtime_error("BasicBlock is nullptr");
    }
    if ((newBB->GetGraph() && newBB->GetGraph() != this) || toBB->GetGraph() != this) {
        throw std::runtime_error("BasicBlocks' graph inconsistency");
    }
}
}  // namespace IRGen
