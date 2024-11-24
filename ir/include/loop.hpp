#ifndef IR_INCLUDE_LOOP_HPP
#define IR_INCLUDE_LOOP_HPP

#include <cassert>
#include <set>
#include "basic_block.hpp"
#include "defines.hpp"

namespace IRGen {
class Loop final {
public:
    explicit Loop(BB* header) : header_(header) {}
    ~Loop()
    {
        for (auto& loop : next_loops_) {
            delete loop;
        }
    }

    void SetHeader(BB* header)
    {
        assert(header_ == nullptr);
        assert(header != nullptr);
        header_ = header;
    }

    BB* GetHeader() const
    {
        return header_;
    }

    void AddLatch(BB* latch)
    {
        latches_.insert(latch);
    }

    const std::set<BB*>& GetLatches() const
    {
        return latches_;
    }

    void AddInnerBlock(BB* block)
    {
        inner_blocks_.emplace_back(block);
    }

    const std::vector<BB*>& GetInnerBlocks() const
    {
        return inner_blocks_;
    }

    void AddNextLoop(Loop* loop)
    {
        next_loops_.emplace_back(loop);
    }

    const std::vector<Loop*>& GetNextLoops() const
    {
        return next_loops_;
    }

    void SetHasAncestor(bool has_ancestor)
    {
        has_ancestor_ = has_ancestor;
    }

    bool HasAncestor() const
    {
        return has_ancestor_;
    }

private:
    BB* header_ {nullptr};
    bool has_ancestor_ {false};
    std::set<BB*> latches_;
    std::vector<BB*> inner_blocks_;
    std::vector<Loop*> next_loops_;
};
}  // namespace IRGen

#endif  // IR_INCLUDE_LOOP_HPP