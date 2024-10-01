#ifndef GENERATION_INCLUDE_BASIC_BLOCK_HPP
#define GENERATION_INCLUDE_BASIC_BLOCK_HPP

#include "defines.hpp"
#include "instruction.hpp"
#include <vector>

namespace IRGen {
class Graph;

class BB {
public:
    BB() = default;
    explicit BB(Graph* graph, uint64_t id) : id_(id), graph_(graph) {};
    NO_COPY_AND_MOVE_SEMANTICS(BB);
    ~BB() = default;

    uint64_t GetId()
    {
        return id_;
    }
    void SetId(uint64_t id)
    {
        id_ = id;
    }
    std::vector<BB*>& GetPredecessors()
    {
        return predecessors_;
    }
    auto& GetSuccessors()
    {
        return succesors_;
    }
    Instruction* GetBeginBB()
    {
        return beginBB_;
    }
    Instruction* GetEndBB()
    {
        return endBB_;
    }
    Graph* GetGraph()
    {
        return graph_;
    }
    void SetGraph(Graph* newGraph)
    {
        graph_ = newGraph;
    }
    void AddPred(BB* bb);
    void DeletePred(BB* bb);
    void AddSucc(BB* bb);
    void DeleteSucc(BB* bb);
    void InsertInstrBefore(Instruction* insert, Instruction* anchor);
    void InsertInstrAfter(Instruction* insert, Instruction* anchor);
    void RemoveInstr(Instruction* instr);
    void AddInstrFront(Instruction* instr);
    void AddInstrBack(Instruction* instr);
    void Print();

private:
    std::vector<BB*> predecessors_;
    struct {
        BB* firstSuccessor = nullptr;
        BB* secondSuccessor = nullptr;
        size_t number = 0;
    } succesors_;
    Instruction* beginBB_ = nullptr;
    Instruction* endBB_ = nullptr;
    Graph* graph_ = nullptr;
    uint64_t id_;
};
}  // namespace IRGen

#endif  // GENERATION_INCLUDE_BASIC_BLOCK_HPP