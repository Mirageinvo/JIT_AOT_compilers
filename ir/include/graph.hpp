#ifndef GENERATION_INCLUDE_GRAPH_HPP
#define GENERATION_INCLUDE_GRAPH_HPP

#include "basic_block.hpp"
#include "defines.hpp"
#include <cassert>
#include <vector>
#include <set>
#include <unordered_map>

namespace IRGen {
class Graph final {
public:
    Graph() = default;
    ~Graph() = default;
    NO_COPY_AND_MOVE_SEMANTICS(Graph);

    void SetName(const std::string& name)
    {
        name_ = name;
    }
    void SetReturnType(InstructionType retType)
    {
        returnType_ = retType;
    }
    BB* GetFirstBB()
    {
        return firstBB_;
    }
    BB* GetLastBB()
    {
        return lastBB_;
    }
    uint64_t GetBBCounter()
    {
        return BBCounter_;
    };
    void IncreaseBBCounter()
    {
        ++BBCounter_;
    };
    void SetFirstBB(BB* basicblock)
    {
        firstBB_ = basicblock;
    }
    void SetLastBB(BB* basicblock)
    {
        lastBB_ = basicblock;
    }
    void SetParams(const std::vector<std::pair<InstructionType, int>>& params)
    {
        params_ = params;
    }

    void AddBB(BB* bb, uint64_t id);
    void DeleteBB(BB* bb);
    void AddPredecessorBB(BB* newBB, BB* toBB);
    void AddSuccessorBB(BB* newBB, BB* toBB);
    void DeletePredecessors(BB* bb);
    void DeleteSuccessors(BB* bb);
    void Print() const;
    std::unordered_map<BB*, std::set<BB*>> BuildDominatorTree();

    static void Dfs(const BB* bb, std::set<uint64_t>& visited, BB* excludedBB = nullptr)
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

private:
    void CheckConsistency(BB* newBB, BB* toBB) const;

    uint64_t BBCounter_ = 0;
    BB* firstBB_;
    BB* lastBB_;
    std::vector<BB*> BBs_;  // TODO: consider list for faster deletion from the middle
    std::string name_;
    std::vector<std::pair<InstructionType, int>> params_;
    InstructionType returnType_;
};
}  // namespace IRGen

#endif  // GENERATION_INCLUDE_GRAPH_HPP