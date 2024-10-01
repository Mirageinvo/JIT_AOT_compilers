#ifndef GENERATION_INCLUDE_GRAPH_HPP
#define GENERATION_INCLUDE_GRAPH_HPP

#include <vector>
#include <list>
#include "defines.hpp"
#include "basic_block.hpp"

namespace IRGen {
class Graph final {
public:
    Graph() = default;
    ~Graph() = default;
    NO_COPY_AND_MOVE_SEMANTICS(Graph);

	void SetName(const std::string &name) {name_ = name;}
	void SetReturnType(InstructionType retType) {returnType_ = retType;}
    BB *GetFirstBB() { return firstBB_; }
    BB *GetLastBB() { return lastBB_; }
    uint64_t GetBBCounter() { return BBCounter_; };
    void AddToBBCounter(uint64_t value) { BBCounter_ += value; };
    void SetFirstBB(BB *basicblock) { firstBB_ = basicblock; }
    void SetLastBB(BB *basicblock) { lastBB_ = basicblock; }
	// void SetParams(const std::vector<std::pair<InstrType, int>> &params) {params_ = params;}

    void AddBB(BB *bb, uint64_t id);
    void DeleteBB(BB *bb);
    void AddPredecessorBB(BB *newBB, BB *toBB);
    void AddSuccessorBB(BB *newBB, BB *toBB);
    void DeletePredecessors(BB *bb);
    void DeleteSuccessors(BB *bb);
	void Print();
    void CheckConsistency(BB *newBB, BB *toBB);

private:
    uint64_t BBCounter_ = 0;
    BB *firstBB_;
    BB *lastBB_;
    std::vector<BB *> BBs_; // TODO: consider list for faster deletion from the middle
	std::string name_;
	// std::vector<std::pair<InstrType, int>> params_;
	InstructionType returnType_;
};
}

#endif // GENERATION_INCLUDE_GRAPH_HPP