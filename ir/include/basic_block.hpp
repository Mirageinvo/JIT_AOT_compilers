#ifndef GENERATION_INCLUDE_BASIC_BLOCK_HPP
#define GENERATION_INCLUDE_BASIC_BLOCK_HPP

#include "defines.hpp"
#include "instruction.hpp"
#include <vector>

namespace IRGen {
class Graph;

class BB {
public:
    BB(std::string name) : name_(name) {}
    explicit BB(Graph* graph, uint64_t id) : graph_(graph), id_(id) {};
    NO_COPY_AND_MOVE_SEMANTICS(BB);
    ~BB() = default;

    uint64_t GetId() const
    {
        return id_;
    }
    void SetId(uint64_t id)
    {
        id_ = id;
    }
    const std::string& GetName() const
    {
        return name_;
    }
    std::vector<BB*>& GetPredecessors()
    {
        return predecessors_;
    }
    std::vector<BB*> GetSuccessors() const
    {
        std::vector<BB*> successors;
        if (succesors_.firstSuccessor != nullptr) {
            successors.emplace_back(succesors_.firstSuccessor);
        }
        if (succesors_.secondSuccessor != nullptr) {
            successors.emplace_back(succesors_.secondSuccessor);
        }
        return successors;
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
    void AddPredecessor(BB* bb);
    void DeletePredecessor(BB* bb);
    void AddSuccessor(BB* bb);
    void DeleteSuccessor(BB* bb);
    void InsertInstrBefore(Instruction* insert, Instruction* anchor);
    void InsertInstrAfter(Instruction* insert, Instruction* anchor);
    void RemoveInstr(Instruction* instr);
    void AddInstrFront(Instruction* instr);
    void AddInstrBack(Instruction* instr);
    void Print();

private:
    std::vector<BB*> predecessors_;
    struct {
        BB* firstSuccessor {nullptr};
        BB* secondSuccessor {nullptr};
        size_t number {0};
    } succesors_;
    Instruction* beginBB_ = nullptr;
    Instruction* endBB_ = nullptr;
    Graph* graph_ = nullptr;
    uint64_t id_;
    std::string name_;
};
}  // namespace IRGen

#endif  // GENERATION_INCLUDE_BASIC_BLOCK_HPP