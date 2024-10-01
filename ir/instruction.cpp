#include "instruction.hpp"
#include "basic_block.hpp"
#include "defines.hpp"
#include <stdexcept>
#include <string>

namespace IRGen {
void Instruction::InsertBefore(Instruction* toInsert)
{
    if (linkedBB_ == nullptr)
        throw std::runtime_error("Linked BasicBlock is nullptr");

    linkedBB_->InsertInstrBefore(toInsert, this);
}

void Instruction::InsertAfter(Instruction* toInsert)
{
    if (linkedBB_ == nullptr)
        throw std::runtime_error("Linked BasicBlock is nullptr");

    linkedBB_->InsertInstrAfter(toInsert, this);
}

void Instruction::Finalization()
{
    if (linkedBB_ == nullptr)
        throw std::runtime_error("Linked BasicBlock is nullptr");

    linkedBB_->RemoveInstr(this);
}
}  // namespace IRGen
