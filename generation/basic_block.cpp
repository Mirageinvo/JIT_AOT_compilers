#include <string>
#include <stdexcept>
#include <iostream>
#include "basic_block.hpp"

namespace IRGen {
void BB::InsertInstrBefore(Instruction *insert, Instruction *anchor) {
	if(!insert) {
        throw std::runtime_error(std::string("Null insert instr in ") + CUR_FUNC_NAME);
    }
	if(!anchor) {
        throw std::runtime_error(std::string("Null anchor instr in ") + CUR_FUNC_NAME);
    }
	if(anchor->GetBB() != this) {
        throw std::runtime_error(std::string("Anchor BB diff from cur BB in ") + CUR_FUNC_NAME);
    }

	insert->SetBB(this);
	auto prev = anchor->GetPrevInstr();
	insert->SetPrevInstr(prev);
	insert->SetNextInstr(anchor);
	prev->SetNextInstr(insert);
	anchor->SetPrevInstr(insert);
	if(!prev) {
		beginBB_ = insert;
    }
}

void BB::InsertInstrAfter(Instruction *to_insert, Instruction *where_to_insert) {
	if (to_insert == nullptr || where_to_insert == nullptr) {
        throw std::runtime_error("At least one instruction is nullptr");
    }
	if (where_to_insert->GetBB() != to_insert) {
        throw std::runtime_error(std::string("Anchor BB diff from cur BB in ") + CUR_FUNC_NAME);
    }
	
	to_insert->SetBB(this);
	auto next = where_to_insert->GetNextInstr();
	to_insert->SetNextInstr(next);
	to_insert->SetPrevInstr(where_to_insert);
	next->SetPrevInstr(to_insert);
	where_to_insert->SetNextInstr(to_insert);
	if(next == nullptr) {
		endBB_ = to_insert;
    }
}

void BB::RemoveInstr(Instruction *instr) {
    if (instr == nullptr || instr->GetBB() != this) {
        throw std::runtime_error("Bad instruction");
    }

    // First of all we need to remove links to previous and next instructions
    instr->SetBB(nullptr);
    auto prev = instr->GetPrevInstr();
    auto next = instr->GetNextInstr();
    instr->SetPrevInstr(nullptr);
    instr->SetNextInstr(nullptr);

    if (prev == nullptr) { // instr was beginBB_
        beginBB_ = next;
    } else {
        prev->SetNextInstr(next);
    }

    if (next == nullptr) { // instr was endBB_
        endBB_ = prev;
    } else {
        next->SetPrevInstr(prev);

    }
}

void BB::AddPred(BB *bb) {
    if (!bb)
        throw std::runtime_error(std::string("Null BB passed to ") + CUR_FUNC_NAME);

    preds_.push_back(bb);
}

void BB::DeletePred(BB *bb) {
    if (!bb)
        throw std::runtime_error(std::string("Null BB passed to ") + CUR_FUNC_NAME);

    auto it = std::find(preds_.begin(), preds_.end(), bb);
    if (it != preds_.end()) {
        preds_.erase(it);
    } else {
        throw std::runtime_error(std::string("No find predecessor in ") + CUR_FUNC_NAME);
    }
}

void BB::AddSucc(BB *bb) {
    if (!bb) {
        throw std::runtime_error(std::string("Null BB passed to ") + CUR_FUNC_NAME);
    }
    if (succs_.size() > 1) {
        throw std::runtime_error(std::string("Attempt to add more than 2 successor ") + CUR_FUNC_NAME);
    }

    succs_.push_back(bb);
}

void BB::DeleteSucc(BB *bb) {
    if (!bb) {
        throw std::runtime_error(std::string("Null BB passed to ") + CUR_FUNC_NAME);
    }
    auto it = std::find(succs_.begin(), succs_.end(), bb);
    if (it != succs_.end()) {
        succs_.erase(it);
    } else {
        throw std::runtime_error(std::string("No successor found in ") + CUR_FUNC_NAME);
    }
}

void BB::AddInstrForward(Instruction *instr) {
	if (!instr || instr->GetBB() || instr->GetPrevInstr() || instr->GetNextInstr()) {
        throw std::runtime_error(std::string("Wrong instr ") + CUR_FUNC_NAME);
    }

    instr->SetBB(this);

    if (!beginBB_) {
        beginBB_ = instr;
        endBB_ = instr;
    } else {
        instr->SetNextInstr(beginBB_);
        beginBB_->SetPrevInstr(instr);
        beginBB_ = instr;
    }
}

void BB::AddInstrBackward(Instruction *instr) {
	if (!instr || instr->GetBB() || instr->GetPrevInstr() || instr->GetNextInstr()) {
        throw std::runtime_error(std::string("Wrong instr ") + CUR_FUNC_NAME);
    }
    instr->SetBB(this);

    if (!beginBB_) {
        beginBB_ = instr;
        endBB_ = instr;
    } else {
        instr->SetPrevInstr(endBB_);
        endBB_->SetNextInstr(instr);
        endBB_ = instr;
    }
}

void BB::Print() {
	auto *cur = beginBB_;
	if (!cur) {
		std::cout << "Empty" << std::endl;
		return;
	}
	std::cout << bbId_ << ":" << std::endl;
	while(cur) {
		std::cout << "\t" << cur->toString() << std::endl;
		cur = cur->GetNextInstr();
	}
	std::cout << std::endl;
}
}
