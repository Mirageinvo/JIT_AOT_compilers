#ifndef GENERATION_INCLUDE_GRAPH_HPP
#define GENERATION_INCLUDE_GRAPH_HPP

#include "basic_block.hpp"
#include "defines.hpp"
#include "loop.hpp"
#include <cassert>
#include <vector>
#include <set>
#include <unordered_map>

namespace IRGen {
class Graph final {
public:
    enum class VERTEX_COLOR : int8_t { GRAY, BLACK };

    Graph() = default;
    ~Graph()
    {
        delete rootLoop_;
    }
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
    }
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
    Loop* BuildLoopTree();
    Loop* GetRootLoop() const
    {
        return rootLoop_;
    }
    static void Dfs(const BB* bb, std::set<uint64_t>& visited, BB* excludedBB = nullptr);
    static void CollectBackEdges(BB* bb, std::unordered_map<BB*, VERTEX_COLOR>& colors,
                                 std::unordered_map<BB*, std::set<BB*>>& latches, std::vector<BB*>& rpo_order,
                                 size_t* counter);
    static void PopulateLoops(const std::unordered_map<BB*, std::set<BB*>>& latches, const std::vector<BB*>& rpo_order,
                              std::vector<Loop*>& loops, std::unordered_map<BB*, Loop*>& bb_to_loop_map);
    static void ReverseTraverse(BB* cur_vertex, Loop* cur_loop, std::set<BB*>& visited,
                                std::unordered_map<BB*, Loop*>& bb_to_loop_map);

private:
    void CheckConsistency(BB* newBB, BB* toBB) const;
    Loop* rootLoop_ {nullptr};
    uint64_t BBCounter_ {0};
    uint64_t size_ {0};
    BB* firstBB_;
    BB* lastBB_;
    std::vector<BB*> BBs_;
    std::string name_;
    std::vector<std::pair<InstructionType, int>> params_;
    InstructionType returnType_;
};
}  // namespace IRGen

#endif  // GENERATION_INCLUDE_GRAPH_HPP
