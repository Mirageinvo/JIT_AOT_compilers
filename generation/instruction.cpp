#include <string>
#include <stdexcept>
#include "instruction.hpp"
#include "basic_block.hpp"
#include "misc/defines.hpp"

namespace IRGen {
void Instruction::InsertBefore(Instruction *toInsert) {
	if (!parentBB_)
		throw std::runtime_error(std::string("Null BB in anchor instr ") + CUR_FUNC_NAME);

	parentBB_->InsertInstrBefore(toInsert, this);
}

void Instruction::InsertAfter(Instruction *toInsert) {
	if (!parentBB_)
        throw std::runtime_error(std::string("Null BB in anchor instr ") + CUR_FUNC_NAME);

	parentBB_->InsertInstrAfter(toInsert, this);
}

void Instruction::Finalization() {
	if (!parentBB_)
        throw std::runtime_error(std::string("Null BB in removing from BB") + CUR_FUNC_NAME);

	parentBB_->ExcludeInstr(this);
}
}
