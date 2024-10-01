#include "basic_block.hpp"
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

namespace IRGen {
void BB::InsertInstrBefore(Instruction* insert, Instruction* where_to_insert)
{
    if (insert == nullptr || where_to_insert == nullptr) {
        throw std::runtime_error("At least one instruction is nullptr");
    }
    if (where_to_insert->GetLinkedBB() != this) {
        throw std::runtime_error("Different BasicBlocks");
    }

    insert->SetLinkedBB(this);
    auto prev = where_to_insert->GetPrevInstr();
    if (prev == nullptr) {
        beginBB_ = insert;
    }
    insert->SetPrevInstr(prev);
    insert->SetNextInstr(where_to_insert);
    if (prev != nullptr) {
        prev->SetNextInstr(insert);
    }
    where_to_insert->SetPrevInstr(insert);
}

void BB::InsertInstrAfter(Instruction* to_insert, Instruction* where_to_insert)
{
    if (to_insert == nullptr || where_to_insert == nullptr) {
        throw std::runtime_error("At least one instruction is nullptr");
    }
    if (where_to_insert->GetLinkedBB() != to_insert->GetLinkedBB()) {
        throw std::runtime_error("Different BasicBlocks");
    }

    to_insert->SetLinkedBB(this);
    auto next = where_to_insert->GetNextInstr();
    if (next == nullptr) {
        endBB_ = to_insert;
    }
    to_insert->SetNextInstr(next);
    to_insert->SetPrevInstr(where_to_insert);
    if (next != nullptr) {
        next->SetPrevInstr(to_insert);
    }
    where_to_insert->SetNextInstr(to_insert);
}

void BB::RemoveInstr(Instruction* instr)
{
    if (instr == nullptr || instr->GetLinkedBB() != this) {
        throw std::runtime_error("Bad instruction");
    }

    // First of all we need to remove links to previous and next instructions
    instr->SetLinkedBB(nullptr);
    auto prev = instr->GetPrevInstr();
    auto next = instr->GetNextInstr();
    instr->SetPrevInstr(nullptr);
    instr->SetNextInstr(nullptr);

    if (prev == nullptr) {  // instr was beginBB_
        beginBB_ = next;
    } else {
        prev->SetNextInstr(next);
    }

    if (next == nullptr) {  // instr was endBB_
        endBB_ = prev;
    } else {
        next->SetPrevInstr(prev);
    }
}

void BB::AddPredecessor(BB* bb)
{
    if (bb == nullptr) {
        throw std::runtime_error("BasicBlock is nullptr");
    }
    predecessors_.push_back(bb);
}

void BB::DeletePredecessor(BB* bb)
{
    if (bb == nullptr) {
        throw std::runtime_error("BasicBlock is nullptr");
    }
    auto it = std::find(predecessors_.begin(), predecessors_.end(), bb);
    if (it != predecessors_.end()) {
        predecessors_.erase(it);
    } else {
        throw std::runtime_error("Predecessor is not found");
    }
}

void BB::AddSuccessor(BB* bb)
{
    assert(succesors_.number <= 2);
    if (bb == nullptr) {
        throw std::runtime_error("BasicBlock is nullptr");
    }
    if (succesors_.number == 2) {
        throw std::runtime_error("Attempt to add more than 2 successors");
    }

    if (succesors_.number == 0) {
        succesors_.firstSuccessor = bb;
    } else {
        succesors_.secondSuccessor = bb;
    }
    succesors_.number++;
}

void BB::DeleteSuccessor(BB* bb)
{
    if (bb == nullptr) {
        throw std::runtime_error("BasicBlock is nullptr");
    }
    if (succesors_.number == 0) {
        assert(succesors_.firstSuccessor == nullptr);
        assert(succesors_.secondSuccessor == nullptr);
        throw std::runtime_error("This BasicBlock doesn't have any successors");
    }

    if (bb == succesors_.secondSuccessor) {
        succesors_.secondSuccessor = nullptr;
        assert(succesors_.number >= 1);
        succesors_.number--;
    } else if (bb == succesors_.firstSuccessor) {
        succesors_.firstSuccessor = nullptr;
        assert(succesors_.number >= 1);
        succesors_.number--;
        std::swap(succesors_.firstSuccessor, succesors_.secondSuccessor);
    } else {
        throw std::runtime_error("This BasicBlock doesn't have such successor");
    }
}

void BB::AddInstrFront(Instruction* instr)
{
    if (instr == nullptr || instr->GetLinkedBB() || instr->GetPrevInstr() || instr->GetNextInstr()) {
        throw std::runtime_error("Bad instruction");
    }
    instr->SetLinkedBB(this);
    if (beginBB_ == nullptr) {
        beginBB_ = instr;
        endBB_ = instr;
    } else {
        instr->SetNextInstr(beginBB_);
        beginBB_->SetPrevInstr(instr);
        beginBB_ = instr;
    }
}

void BB::AddInstrBack(Instruction* instr)
{
    if (!instr || instr->GetLinkedBB() || instr->GetPrevInstr() || instr->GetNextInstr()) {
        throw std::runtime_error("Bad instruction");
    }
    instr->SetLinkedBB(this);

    if (beginBB_ == nullptr) {
        beginBB_ = instr;
        endBB_ = instr;
    } else {
        instr->SetPrevInstr(endBB_);
        endBB_->SetNextInstr(instr);
        endBB_ = instr;
    }
}

void BB::Print()
{
    auto* current = beginBB_;
    if (current == nullptr) {
        std::cout << "No instructions in BasicBlock with id: " << id_ << std::endl;
        return;
    }
    std::cout << id_ << ":" << std::endl;
    while (current != nullptr) {
        std::cout << "\t" << current->infoToString() << std::endl;
        current = current->GetNextInstr();
    }
    std::cout << std::endl;
}
}  // namespace IRGen
