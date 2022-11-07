#include <cassert>

#include "graph.h"
#include "dom_tree.h"

void TestGraph1();
void TestGraph2();
void TestGraph3();

int main()
{
    TestGraph1();
    TestGraph2();
    TestGraph3();
}

void TestGraph1()
{
    Graph graph1;
    graph1.AddBBs(7);

    // Example 1
    // 0 = A, 1 = B, 2 = C, 3 = D, 4 = E, 5 = F, 6 = G
    graph1.AddEdge(0, 1);

    graph1.AddEdge(1, 2);
    graph1.AddEdge(1, 5);

    graph1.AddEdge(2, 3);
    
    graph1.AddEdge(4, 3);

    graph1.AddEdge(5, 4);
    graph1.AddEdge(5, 6);

    graph1.AddEdge(6, 3);

    DomTree dt1(&graph1);
    dt1.BuildDomTree();

    // Test dominates over
    assert(graph1.GetBBVector()[0]->GetDominatesOver().size() == 6);
    assert(graph1.GetBBVector()[1]->GetDominatesOver().size() == 5);
    assert(graph1.GetBBVector()[2]->GetDominatesOver().size() == 0);
    assert(graph1.GetBBVector()[3]->GetDominatesOver().size() == 0);
    assert(graph1.GetBBVector()[4]->GetDominatesOver().size() == 0);
    assert(graph1.GetBBVector()[5]->GetDominatesOver().size() == 2);
    assert(graph1.GetBBVector()[6]->GetDominatesOver().size() == 0);

    // Test dominators
    assert(graph1.GetBBVector()[0]->GetDominators().size() == 0);
    assert(graph1.GetBBVector()[1]->GetDominators().size() == 1);
    assert(graph1.GetBBVector()[2]->GetDominators().size() == 2);
    assert(graph1.GetBBVector()[3]->GetDominators().size() == 2);
    assert(graph1.GetBBVector()[4]->GetDominators().size() == 3);
    assert(graph1.GetBBVector()[5]->GetDominators().size() == 2);
    assert(graph1.GetBBVector()[6]->GetDominators().size() == 3);
}


void TestGraph2()
{
    Graph graph2;
    graph2.AddBBs(11);

    // Example 1
    // 0 = A, 1 = B, 2 = C, 3 = D, 4 = E, 5 = F, 6 = G, 7 = H, 8 = I, 9 = J, 10 = K
    graph2.AddEdge(0, 1);

    graph2.AddEdge(1, 2);
    graph2.AddEdge(1, 9);

    graph2.AddEdge(2, 3);

    graph2.AddEdge(3, 2);
    graph2.AddEdge(3, 4);
    
    graph2.AddEdge(4, 5);

    graph2.AddEdge(5, 4);
    graph2.AddEdge(5, 6);

    graph2.AddEdge(6, 7);
    graph2.AddEdge(6, 8);

    graph2.AddEdge(7, 1);

    graph2.AddEdge(8, 10);

    graph2.AddEdge(9, 2);

    DomTree dt2(&graph2);
    dt2.BuildDomTree();

    // Test dominates over
    assert(graph2.GetBBVector()[0]->GetDominatesOver().size() == 10);
    assert(graph2.GetBBVector()[1]->GetDominatesOver().size() == 9);
    assert(graph2.GetBBVector()[2]->GetDominatesOver().size() == 7);
    assert(graph2.GetBBVector()[3]->GetDominatesOver().size() == 6);
    assert(graph2.GetBBVector()[4]->GetDominatesOver().size() == 5);
    assert(graph2.GetBBVector()[5]->GetDominatesOver().size() == 4);
    assert(graph2.GetBBVector()[6]->GetDominatesOver().size() == 3);
    assert(graph2.GetBBVector()[7]->GetDominatesOver().size() == 0);
    assert(graph2.GetBBVector()[8]->GetDominatesOver().size() == 1);
    assert(graph2.GetBBVector()[9]->GetDominatesOver().size() == 0);
    assert(graph2.GetBBVector()[10]->GetDominatesOver().size() == 0);

    // Test dominators
    assert(graph2.GetBBVector()[0]->GetDominators().size() == 0);
    assert(graph2.GetBBVector()[1]->GetDominators().size() == 1);
    assert(graph2.GetBBVector()[2]->GetDominators().size() == 2);
    assert(graph2.GetBBVector()[3]->GetDominators().size() == 3);
    assert(graph2.GetBBVector()[4]->GetDominators().size() == 4);
    assert(graph2.GetBBVector()[5]->GetDominators().size() == 5);
    assert(graph2.GetBBVector()[6]->GetDominators().size() == 6);
    assert(graph2.GetBBVector()[7]->GetDominators().size() == 7);
    assert(graph2.GetBBVector()[8]->GetDominators().size() == 7);
    assert(graph2.GetBBVector()[9]->GetDominators().size() == 2);
    assert(graph2.GetBBVector()[10]->GetDominators().size() == 8);
}

void TestGraph3()
{
    Graph graph3;
    graph3.AddBBs(9);

    // Example 1
    // 0 = A, 1 = B, 2 = C, 3 = D, 4 = E, 5 = F, 6 = G, 7 = H, 8 = I
    graph3.AddEdge(0, 1);

    graph3.AddEdge(1, 2);
    graph3.AddEdge(1, 4);

    graph3.AddEdge(2, 3);

    graph3.AddEdge(3, 6);

    graph3.AddEdge(4, 3);
    graph3.AddEdge(4, 5);

    graph3.AddEdge(5, 1);
    graph3.AddEdge(5, 7);

    graph3.AddEdge(6, 2);
    graph3.AddEdge(6, 8);

    graph3.AddEdge(7, 6);
    graph3.AddEdge(7, 8);


    DomTree dt3(&graph3);
    dt3.BuildDomTree();

    // Test dominates over
    assert(graph3.GetBBVector()[0]->GetDominatesOver().size() == 8);
    assert(graph3.GetBBVector()[1]->GetDominatesOver().size() == 7);
    assert(graph3.GetBBVector()[2]->GetDominatesOver().size() == 0);
    assert(graph3.GetBBVector()[3]->GetDominatesOver().size() == 0);
    assert(graph3.GetBBVector()[4]->GetDominatesOver().size() == 2);
    assert(graph3.GetBBVector()[5]->GetDominatesOver().size() == 1);
    assert(graph3.GetBBVector()[6]->GetDominatesOver().size() == 0);
    assert(graph3.GetBBVector()[7]->GetDominatesOver().size() == 0);
    assert(graph3.GetBBVector()[8]->GetDominatesOver().size() == 0);

    // Test dominators
    assert(graph3.GetBBVector()[0]->GetDominators().size() == 0);
    assert(graph3.GetBBVector()[1]->GetDominators().size() == 1);
    assert(graph3.GetBBVector()[2]->GetDominators().size() == 2);
    assert(graph3.GetBBVector()[3]->GetDominators().size() == 2);
    assert(graph3.GetBBVector()[4]->GetDominators().size() == 2);
    assert(graph3.GetBBVector()[5]->GetDominators().size() == 3);
    assert(graph3.GetBBVector()[6]->GetDominators().size() == 2);
    assert(graph3.GetBBVector()[7]->GetDominators().size() == 4);
    assert(graph3.GetBBVector()[8]->GetDominators().size() == 2);
}