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
    ++BBCounter_;
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

/* static */
void Graph::Dfs(const BB* bb, std::set<uint64_t>& visited, BB* excludedBB)
{
    if (bb == nullptr || bb == excludedBB) {
        return;
    }
    visited.insert(bb->GetId());
    for (const auto* successor : bb->GetSuccessors()) {
        assert(successor != nullptr);
        if (visited.find(successor->GetId()) == visited.end()) {
            Dfs(successor, visited, excludedBB);
        }
    }
}

Loop* Graph::BuildLoopTree()
{
    std::unordered_map<BB*, VERTEX_COLOR> colors;
    std::unordered_map<BB*, std::set<BB*>> latches;
    size_t blocks_count = GetBBCounter();
    std::vector<BB*> rpo_order(blocks_count);
    CollectBackEdges(firstBB_, colors, latches, rpo_order, &blocks_count);
    assert(blocks_count == 0);
    rootLoop_ = new Loop(nullptr);
    std::vector<Loop*> loops;
    std::unordered_map<BB*, Loop*> bb_to_loop_map;
    PopulateLoops(latches, rpo_order, loops, bb_to_loop_map);
    // All loops in "loops" vector that don't have an ancestor
    // should have "rootLoop_" as an ancestor
    for (auto* loop : loops) {
        if (!loop->HasAncestor()) {
            rootLoop_->AddNextLoop(loop);
            loop->SetHasAncestor(true);
        }
    }
    // Vertexes that are not assigned to any loop are going to be added to rootLoop_
    for (const auto& bb : rpo_order) {
        if (bb_to_loop_map.find(bb) == bb_to_loop_map.end()) {
            rootLoop_->AddInnerBlock(bb);
        }
    }
    return rootLoop_;
}

/* static */
void Graph::PopulateLoops(const std::unordered_map<BB*, std::set<BB*>>& latches, const std::vector<BB*>& rpo_order,
                          std::vector<Loop*>& loops, std::unordered_map<BB*, Loop*>& bb_to_loop_map)
{
    // Reverse RPO
    for (auto iter = rpo_order.rbegin(); iter != rpo_order.rend(); ++iter) {
        auto header_iter = latches.find(*iter);
        if (header_iter != latches.end()) {
            // Found a loop header
            Loop* new_loop = new Loop(header_iter->first);
            assert(bb_to_loop_map.find(header_iter->first) == bb_to_loop_map.end());
            bb_to_loop_map[header_iter->first] = new_loop;
            for (const auto& bb : header_iter->second) {
                assert(bb_to_loop_map.find(bb) == bb_to_loop_map.end());
                new_loop->AddLatch(bb);
                bb_to_loop_map[bb] = new_loop;
            }
            std::set<BB*> visited;
            for (const auto& bb : header_iter->second) {
                ReverseTraverse(bb, new_loop, visited, bb_to_loop_map);
            }
            loops.emplace_back(new_loop);
        }
    }
}

/* static */
void Graph::ReverseTraverse(BB* cur_vertex, Loop* cur_loop, std::set<BB*>& visited,
                            std::unordered_map<BB*, Loop*>& bb_to_loop_map)
{
    visited.insert(cur_vertex);
    if (cur_vertex == cur_loop->GetHeader()) {
        return;
    }
    auto latches = cur_loop->GetLatches();
    auto iter_in_bb_to_loop_map = bb_to_loop_map.find(cur_vertex);
    if (latches.find(cur_vertex) == latches.end() && iter_in_bb_to_loop_map == bb_to_loop_map.end()) {
        cur_loop->AddInnerBlock(cur_vertex);
        bb_to_loop_map[cur_vertex] = cur_loop;
    } else if (iter_in_bb_to_loop_map != bb_to_loop_map.end()) {
        Loop* loop_of_vertex = iter_in_bb_to_loop_map->second;
        if (loop_of_vertex != cur_loop && !loop_of_vertex->HasAncestor()) {
            cur_loop->AddNextLoop(loop_of_vertex);
            loop_of_vertex->SetHasAncestor(true);
        }
    }
    for (auto& vertex : cur_vertex->GetPredecessors()) {
        if (visited.find(vertex) == visited.end()) {
            ReverseTraverse(vertex, cur_loop, visited, bb_to_loop_map);
        }
    }
}

/* static */
void Graph::CollectBackEdges(BB* bb, std::unordered_map<BB*, VERTEX_COLOR>& colors,
                             std::unordered_map<BB*, std::set<BB*>>& latches, std::vector<BB*>& rpo_order,
                             size_t* blocks_count)
{
    assert(bb != nullptr);
    // We haven't seen this vertex (assume its color is white)
    // So, color it with gray
    colors[bb] = VERTEX_COLOR::GRAY;
    for (auto* successor : bb->GetSuccessors()) {
        assert(successor != nullptr);
        auto color_iter = colors.find(successor);
        if (color_iter == colors.end()) {
            CollectBackEdges(successor, colors, latches, rpo_order, blocks_count);
        } else if (color_iter->second == VERTEX_COLOR::GRAY) {
            auto latches_iter = latches.find(successor);
            if (latches_iter == latches.end()) {
                latches[successor] = {bb};
            } else {
                latches_iter->second.insert(bb);
            }
        }
    }
    colors[bb] = VERTEX_COLOR::BLACK;
    assert(blocks_count != nullptr && *blocks_count > 0);
    rpo_order[--(*blocks_count)] = bb;
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
