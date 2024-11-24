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
    auto root_children = answer->GetNextLoops();
    ASSERT_EQ(root_children.size(), 1);
    auto next_children = root_children[0]->GetNextLoops();
    ASSERT_EQ(next_children.size(), 2);
}

TEST(LoopAnalyzerTest, NoLoopExampleTest)
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
    auto root_children = answer->GetNextLoops();
    ASSERT_EQ(root_children.size(), 0);
}

}  // namespace IRGen
