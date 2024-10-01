#ifndef GENERATION_INCLUDE_BASIC_BLOCK_HPP
#define GENERATION_INCLUDE_BASIC_BLOCK_HPP

#include <vector>
#include "defines.hpp"
#include "instruction.hpp"

namespace IRGen {
class Graph;

class BB {
public:
    BB() = default;
    explicit BB(Graph *graph, uint64_t id): id_(id), graph_(graph) {};

    NO_COPY_AND_MOVE_SEMANTICS(BB);
    ~BB() = default;

    uint64_t GetId() { return id_; }
    std::vector<BB *> &GetPredecessors() { return predecessors_; }
    auto& GetSuccs() { return succesors_; }
    Instruction *GetBeginBB() { return beginBB_; }
    Instruction *GetEndBB() { return endBB_; }
    Graph *GetGraph() { return graph_; }

    void SetId(uint64_t id) { id_ = id; }
    void AddPred(BB *bb);
    void DeletePred(BB *bb);
    void AddSucc(BB *bb);
    void DeleteSucc(BB *bb);
    void InsertInstrBefore(Instruction *insert, Instruction *anchor);
    void InsertInstrAfter(Instruction *insert, Instruction *anchor);
    void SetGraph(Graph *newGraph) { graph_ = newGraph; }
    void ExcludeInstr(Instruction *instr);
    void AddInstrFront(Instruction *instr);
    void AddInstrBack(Instruction *instr);
	void Print();

private:
    std::vector<BB *> predecessors_;
    struct {
        BB *firstSuccessor;
        BB *secondSuccessor;
        size_t successorsNumber;
    } succesors_;
    Instruction *beginBB_ = nullptr;
    Instruction *endBB_ = nullptr;
    Graph *graph_ = nullptr;
    uint64_t id_;
};
}

#endif // GENERATION_INCLUDE_BASIC_BLOCK_HPP