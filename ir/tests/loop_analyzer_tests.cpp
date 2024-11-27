#include <gtest/gtest.h>
#include "IRGenerator.hpp"
#include "graph.hpp"

namespace IRGen {
TEST(LoopAnalyzerTest, FirstExampleTest)
{
    IRGenerator generator;
    generator.CreateGraph();
    auto* graph = generator.GetGraph();
    auto* a = generator.CreateEmptyBB("a");
    auto* b = generator.CreateEmptyBB("b");
    auto* c = generator.CreateEmptyBB("c");
    auto* d = generator.CreateEmptyBB("d");
    auto* e = generator.CreateEmptyBB("e");
    auto* f = generator.CreateEmptyBB("f");
    auto* g = generator.CreateEmptyBB("g");
    auto* h = generator.CreateEmptyBB("h");
    auto* i = generator.CreateEmptyBB("i");
    auto* k = generator.CreateEmptyBB("k");
    auto* l = generator.CreateEmptyBB("l");
    auto* o = generator.CreateEmptyBB("o");
    auto* q = generator.CreateEmptyBB("q");
    auto* r = generator.CreateEmptyBB("r");
    auto* t = generator.CreateEmptyBB("t");
    auto* v = generator.CreateEmptyBB("v");
    auto* w = generator.CreateEmptyBB("w");
    auto* x = generator.CreateEmptyBB("x");

    a->AddSuccessor(b);
    a->AddPredecessor(w);
    a->AddPredecessor(i);
    a->AddPredecessor(k);

    b->AddSuccessor(c);
    b->AddPredecessor(a);
    b->AddPredecessor(r);

    c->AddSuccessor(d);
    c->AddSuccessor(r);
    c->AddPredecessor(b);

    r->AddSuccessor(b);
    r->AddPredecessor(c);

    d->AddSuccessor(e);
    d->AddPredecessor(c);

    e->AddSuccessor(f);
    e->AddSuccessor(l);
    e->AddPredecessor(d);
    e->AddPredecessor(o);

    l->AddSuccessor(i);
    l->AddPredecessor(e);

    i->AddSuccessor(a);
    i->AddPredecessor(l);

    f->AddSuccessor(g);
    f->AddSuccessor(h);
    f->AddPredecessor(e);

    g->AddSuccessor(q);
    g->AddPredecessor(f);

    q->AddSuccessor(w);
    q->AddPredecessor(g);

    w->AddSuccessor(a);
    w->AddPredecessor(q);

    h->AddSuccessor(o);
    h->AddSuccessor(t);
    h->AddPredecessor(f);

    o->AddSuccessor(e);
    o->AddPredecessor(h);

    t->AddSuccessor(v);
    t->AddPredecessor(h);

    v->AddSuccessor(k);
    v->AddSuccessor(x);
    v->AddPredecessor(t);

    k->AddSuccessor(a);
    k->AddPredecessor(v);

    x->AddPredecessor(v);

    graph->SetFirstBB(a);

    Loop* answer = graph->BuildLoopTree();
    ASSERT_NE(answer, nullptr);
    ASSERT_EQ(answer, graph->GetRootLoop());
    auto root_blocks = answer->GetInnerBlocks();
    std::set<std::string> blocks_names;
    std::set<std::string> blocks_names_answer({"x"});
    for (const auto& el : root_blocks) {
        blocks_names.insert(el->GetName());
    }
    ASSERT_EQ(blocks_names, blocks_names_answer);
    auto root_children = answer->GetNextLoops();
    ASSERT_EQ(root_children.size(), 1);
    auto next_children = root_children[0]->GetNextLoops();
    ASSERT_EQ(next_children.size(), 2);

    blocks_names = {};
    blocks_names_answer = {"f", "h"};
    for (const auto& el : next_children[0]->GetInnerBlocks()) {
        blocks_names.insert(el->GetName());
    }
    ASSERT_EQ(blocks_names, blocks_names_answer);

    blocks_names = {};
    blocks_names_answer = {"c"};
    for (const auto& el : next_children[1]->GetInnerBlocks()) {
        blocks_names.insert(el->GetName());
    }
    ASSERT_EQ(blocks_names, blocks_names_answer);
}

TEST(LoopAnalyzerTest, SecondExampleTest)
{
    IRGenerator generator;
    generator.CreateGraph();
    auto* graph = generator.GetGraph();
    auto* a = generator.CreateEmptyBB("a");
    auto* b = generator.CreateEmptyBB("b");
    auto* c = generator.CreateEmptyBB("c");
    auto* d = generator.CreateEmptyBB("d");
    auto* e = generator.CreateEmptyBB("e");
    auto* f = generator.CreateEmptyBB("f");
    auto* g = generator.CreateEmptyBB("g");

    a->AddSuccessor(b);

    b->AddSuccessor(c);
    b->AddSuccessor(f);
    b->AddPredecessor(a);

    c->AddSuccessor(d);
    c->AddPredecessor(b);

    f->AddSuccessor(e);
    f->AddSuccessor(g);
    f->AddPredecessor(b);

    e->AddSuccessor(d);
    e->AddPredecessor(f);

    g->AddSuccessor(d);
    g->AddPredecessor(f);

    d->AddPredecessor(c);
    d->AddPredecessor(e);
    d->AddPredecessor(g);

    graph->SetFirstBB(a);

    Loop* answer = graph->BuildLoopTree();
    ASSERT_NE(answer, nullptr);
    ASSERT_EQ(answer, graph->GetRootLoop());
    auto root_blocks = answer->GetInnerBlocks();
    std::set<std::string> blocks_names;
    std::set<std::string> blocks_names_answer({"a", "b", "c", "d", "e", "f", "g"});
    for (const auto& el : root_blocks) {
        blocks_names.insert(el->GetName());
    }
    ASSERT_EQ(blocks_names, blocks_names_answer);
    auto root_children = answer->GetNextLoops();
    ASSERT_EQ(root_children.size(), 0);
}

TEST(LoopAnalyzerTest, ThirdExampleTest)
{
    IRGenerator generator;
    generator.CreateGraph();
    auto* graph = generator.GetGraph();
    auto* a = generator.CreateEmptyBB("a");
    auto* b = generator.CreateEmptyBB("b");
    auto* c = generator.CreateEmptyBB("c");
    auto* d = generator.CreateEmptyBB("d");
    auto* e = generator.CreateEmptyBB("e");

    a->AddSuccessor(b);

    b->AddSuccessor(c);
    b->AddSuccessor(d);
    b->AddPredecessor(a);
    b->AddPredecessor(e);

    c->AddPredecessor(b);

    d->AddSuccessor(e);
    d->AddPredecessor(b);

    e->AddSuccessor(b);
    e->AddPredecessor(d);

    graph->SetFirstBB(a);

    Loop* answer = graph->BuildLoopTree();
    ASSERT_NE(answer, nullptr);
    ASSERT_EQ(answer, graph->GetRootLoop());
    auto root_blocks = answer->GetInnerBlocks();
    std::set<std::string> blocks_names;
    std::set<std::string> blocks_names_answer({"a", "c"});
    for (const auto& el : root_blocks) {
        blocks_names.insert(el->GetName());
    }
    ASSERT_EQ(blocks_names, blocks_names_answer);
    auto root_children = answer->GetNextLoops();
    ASSERT_EQ(root_children.size(), 1);
}

TEST(LoopAnalyzerTest, FourthExampleTest)
{
    IRGenerator generator;
    generator.CreateGraph();
    auto* graph = generator.GetGraph();
    auto* a = generator.CreateEmptyBB("a");
    auto* b = generator.CreateEmptyBB("b");
    auto* c = generator.CreateEmptyBB("c");
    auto* d = generator.CreateEmptyBB("d");
    auto* e = generator.CreateEmptyBB("e");
    auto* f = generator.CreateEmptyBB("f");

    a->AddSuccessor(b);

    b->AddSuccessor(c);
    b->AddPredecessor(a);
    b->AddPredecessor(e);

    c->AddSuccessor(d);
    c->AddSuccessor(f);
    c->AddPredecessor(b);

    d->AddSuccessor(e);
    d->AddSuccessor(f);
    d->AddPredecessor(c);

    e->AddSuccessor(b);
    e->AddPredecessor(d);

    f->AddPredecessor(c);
    f->AddPredecessor(d);

    graph->SetFirstBB(a);

    Loop* answer = graph->BuildLoopTree();
    ASSERT_NE(answer, nullptr);
    ASSERT_EQ(answer, graph->GetRootLoop());
    auto root_blocks = answer->GetInnerBlocks();
    std::set<std::string> blocks_names;
    std::set<std::string> blocks_names_answer({"a", "f"});
    for (const auto& el : root_blocks) {
        blocks_names.insert(el->GetName());
    }
    ASSERT_EQ(blocks_names, blocks_names_answer);
    auto root_children = answer->GetNextLoops();
    ASSERT_EQ(root_children.size(), 1);

    blocks_names = {};
    blocks_names_answer = {"c", "d"};
    for (const auto& el : root_children[0]->GetInnerBlocks()) {
        blocks_names.insert(el->GetName());
    }
    ASSERT_EQ(blocks_names, blocks_names_answer);
}

TEST(LoopAnalyzerTest, FifthExampleTest)
{
    IRGenerator generator;
    generator.CreateGraph();
    auto* graph = generator.GetGraph();
    auto* a = generator.CreateEmptyBB("a");
    auto* b = generator.CreateEmptyBB("b");
    auto* c = generator.CreateEmptyBB("c");
    auto* d = generator.CreateEmptyBB("d");
    auto* e = generator.CreateEmptyBB("e");
    auto* f = generator.CreateEmptyBB("f");
    auto* g = generator.CreateEmptyBB("g");
    auto* h = generator.CreateEmptyBB("h");

    a->AddSuccessor(b);
    a->AddPredecessor(h);

    b->AddSuccessor(c);
    b->AddSuccessor(d);
    b->AddPredecessor(a);
    b->AddPredecessor(g);

    c->AddSuccessor(e);
    c->AddSuccessor(f);
    c->AddPredecessor(b);

    d->AddSuccessor(f);
    d->AddPredecessor(b);

    e->AddPredecessor(c);

    f->AddSuccessor(g);
    f->AddPredecessor(c);
    f->AddPredecessor(d);

    g->AddSuccessor(b);
    g->AddSuccessor(h);
    g->AddPredecessor(f);

    h->AddSuccessor(a);
    h->AddPredecessor(g);

    graph->SetFirstBB(a);

    Loop* answer = graph->BuildLoopTree();
    ASSERT_NE(answer, nullptr);
    ASSERT_EQ(answer, graph->GetRootLoop());
    auto root_blocks = answer->GetInnerBlocks();
    std::set<std::string> blocks_names;
    std::set<std::string> blocks_names_answer({"e"});
    for (const auto& el : root_blocks) {
        blocks_names.insert(el->GetName());
    }
    ASSERT_EQ(blocks_names, blocks_names_answer);
    auto root_children = answer->GetNextLoops();
    ASSERT_EQ(root_children.size(), 1);

    blocks_names = {};
    blocks_names_answer = {};
    for (const auto& el : root_children[0]->GetInnerBlocks()) {
        blocks_names.insert(el->GetName());
    }
    ASSERT_EQ(blocks_names, blocks_names_answer);

    root_children = root_children[0]->GetNextLoops();
    blocks_names = {};
    blocks_names_answer = {"c", "d", "f"};
    for (const auto& el : root_children[0]->GetInnerBlocks()) {
        blocks_names.insert(el->GetName());
    }
    ASSERT_EQ(blocks_names, blocks_names_answer);
}

TEST(LoopAnalyzerTest, SixthExampleTest)
{
    IRGenerator generator;
    generator.CreateGraph();
    auto* graph = generator.GetGraph();
    auto* a = generator.CreateEmptyBB("a");
    auto* b = generator.CreateEmptyBB("b");
    auto* c = generator.CreateEmptyBB("c");
    auto* d = generator.CreateEmptyBB("d");
    auto* e = generator.CreateEmptyBB("e");
    auto* f = generator.CreateEmptyBB("f");

    a->AddSuccessor(b);

    b->AddSuccessor(c);
    b->AddSuccessor(d);
    b->AddPredecessor(a);
    b->AddPredecessor(f);

    c->AddSuccessor(e);
    c->AddSuccessor(f);
    c->AddPredecessor(b);

    d->AddSuccessor(f);
    d->AddPredecessor(b);

    e->AddPredecessor(c);

    f->AddSuccessor(b);
    f->AddPredecessor(c);
    f->AddPredecessor(d);

    graph->SetFirstBB(a);

    Loop* answer = graph->BuildLoopTree();
    ASSERT_NE(answer, nullptr);
    ASSERT_EQ(answer, graph->GetRootLoop());
    auto root_blocks = answer->GetInnerBlocks();
    std::set<std::string> blocks_names;
    std::set<std::string> blocks_names_answer({"a", "e"});
    for (const auto& el : root_blocks) {
        blocks_names.insert(el->GetName());
    }
    ASSERT_EQ(blocks_names, blocks_names_answer);
    auto root_children = answer->GetNextLoops();
    ASSERT_EQ(root_children.size(), 1);

    blocks_names = {};
    blocks_names_answer = {"c", "d"};
    for (const auto& el : root_children[0]->GetInnerBlocks()) {
        blocks_names.insert(el->GetName());
    }
    ASSERT_EQ(blocks_names, blocks_names_answer);
}

}  // namespace IRGen
