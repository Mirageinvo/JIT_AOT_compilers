#include <gtest/gtest.h>
#include "IRGenerator.hpp"
#include "graph.hpp"

namespace IRGen {
TEST(CFGTest, FirstExampleTest)
{
    IRGenerator generator;
    generator.CreateGraph();
    auto* graph = generator.GetGraph();
    auto* a = generator.CreateEmptyBB();
    auto* b = generator.CreateEmptyBB();
    auto* c = generator.CreateEmptyBB();
    auto* d = generator.CreateEmptyBB();
    auto* e = generator.CreateEmptyBB();
    auto* f = generator.CreateEmptyBB();
    auto* g = generator.CreateEmptyBB();
    a->AddSuccessor(b);
    b->AddSuccessor(c);
    b->AddSuccessor(f);
    c->AddSuccessor(d);
    f->AddSuccessor(e);
    f->AddSuccessor(g);
    e->AddSuccessor(d);
    g->AddSuccessor(d);
    graph->SetFirstBB(a);

    std::set<std::pair<uint64_t, uint64_t>> answer;
    answer.insert({0, 1});
    answer.insert({1, 2});
    answer.insert({1, 3});
    answer.insert({1, 5});
    answer.insert({5, 4});
    answer.insert({5, 6});

    auto dominatorTree = graph->BuildDominatorTree();
    std::set<std::pair<uint64_t, uint64_t>> treeEdges;
    for (const auto& pair : dominatorTree) {
        for (const auto& el : pair.second) {
            treeEdges.insert(std::make_pair(pair.first->GetId(), el->GetId()));
        }
    }
    ASSERT_EQ(treeEdges, answer);
}

TEST(CFGTest, SecondExampleTest)
{
    IRGenerator generator;
    generator.CreateGraph();
    auto* graph = generator.GetGraph();
    auto* a = generator.CreateEmptyBB();
    auto* b = generator.CreateEmptyBB();
    auto* c = generator.CreateEmptyBB();
    auto* d = generator.CreateEmptyBB();
    auto* e = generator.CreateEmptyBB();
    auto* f = generator.CreateEmptyBB();
    auto* g = generator.CreateEmptyBB();
    auto* h = generator.CreateEmptyBB();
    auto* i = generator.CreateEmptyBB();
    auto* j = generator.CreateEmptyBB();
    auto* k = generator.CreateEmptyBB();
    a->AddSuccessor(b);
    b->AddSuccessor(c);
    b->AddSuccessor(j);
    j->AddSuccessor(c);
    c->AddSuccessor(d);
    d->AddSuccessor(e);
    e->AddSuccessor(f);
    f->AddSuccessor(g);
    f->AddSuccessor(e);
    g->AddSuccessor(h);
    g->AddSuccessor(i);
    h->AddSuccessor(b);
    i->AddSuccessor(k);
    graph->SetFirstBB(a);

    std::set<std::pair<uint64_t, uint64_t>> answer;
    answer.insert({0, 1});
    answer.insert({1, 2});
    answer.insert({1, 9});
    answer.insert({2, 3});
    answer.insert({3, 4});
    answer.insert({4, 5});
    answer.insert({5, 6});
    answer.insert({6, 7});
    answer.insert({6, 8});
    answer.insert({8, 10});

    auto dominatorTree = graph->BuildDominatorTree();
    std::set<std::pair<uint64_t, uint64_t>> treeEdges;
    for (const auto& pair : dominatorTree) {
        for (const auto& el : pair.second) {
            treeEdges.insert(std::make_pair(pair.first->GetId(), el->GetId()));
        }
    }
    ASSERT_EQ(treeEdges, answer);
}

TEST(CFGTest, ThirdExampleTest)
{
    IRGenerator generator;
    generator.CreateGraph();
    auto* graph = generator.GetGraph();
    auto* a = generator.CreateEmptyBB();
    auto* b = generator.CreateEmptyBB();
    auto* c = generator.CreateEmptyBB();
    auto* d = generator.CreateEmptyBB();
    auto* e = generator.CreateEmptyBB();
    auto* f = generator.CreateEmptyBB();
    auto* g = generator.CreateEmptyBB();
    auto* h = generator.CreateEmptyBB();
    auto* i = generator.CreateEmptyBB();
    a->AddSuccessor(b);
    b->AddSuccessor(c);
    b->AddSuccessor(e);
    c->AddSuccessor(d);
    d->AddSuccessor(g);
    e->AddSuccessor(f);
    e->AddSuccessor(d);
    f->AddSuccessor(b);
    f->AddSuccessor(h);
    h->AddSuccessor(g);
    h->AddSuccessor(i);
    g->AddSuccessor(c);
    g->AddSuccessor(i);
    graph->SetFirstBB(a);

    std::set<std::pair<uint64_t, uint64_t>> answer;
    answer.insert({0, 1});
    answer.insert({1, 2});
    answer.insert({1, 3});
    answer.insert({1, 4});
    answer.insert({1, 6});
    answer.insert({1, 8});
    answer.insert({4, 5});
    answer.insert({5, 7});

    auto dominatorTree = graph->BuildDominatorTree();
    std::set<std::pair<uint64_t, uint64_t>> treeEdges;
    for (const auto& pair : dominatorTree) {
        for (const auto& el : pair.second) {
            treeEdges.insert(std::make_pair(pair.first->GetId(), el->GetId()));
        }
    }
    ASSERT_EQ(treeEdges, answer);
}
}  // namespace IRGen
