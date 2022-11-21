#include <cassert>

#include "graph.h"
#include "loop_analizer.h"

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

    LoopAnalizer la1(&graph1);
    la1.BuildLoopTree();

    assert(la1.GetLoopVector().size() == 1);
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

    LoopAnalizer la2(&graph2);
    la2.BuildLoopTree();
    assert(la2.GetLoopVector().size() == 4);

    assert(graph2.GetBB(0)->GetLoop() == la2.GetLoopVector()[0]);
    assert(graph2.GetBB(10)->GetLoop() == la2.GetLoopVector()[0]);

    assert(graph2.GetBB(2)->GetLoop() == la2.GetLoopVector()[1]);
    assert(graph2.GetBB(3)->GetLoop() == la2.GetLoopVector()[1]);

    assert(graph2.GetBB(4)->GetLoop() == la2.GetLoopVector()[2]);
    assert(graph2.GetBB(5)->GetLoop() == la2.GetLoopVector()[2]);

    assert(graph2.GetBB(1)->GetLoop() == la2.GetLoopVector()[3]);
    assert(graph2.GetBB(6)->GetLoop() == la2.GetLoopVector()[3]);
    assert(graph2.GetBB(7)->GetLoop() == la2.GetLoopVector()[3]);
    assert(graph2.GetBB(9)->GetLoop() == la2.GetLoopVector()[3]);
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

    LoopAnalizer la3(&graph3);
    la3.BuildLoopTree();
    assert(la3.GetLoopVector().size() == 3);


    assert(graph3.GetBB(0)->GetLoop() == la3.GetLoopVector()[0]);
    assert(graph3.GetBB(3)->GetLoop() == la3.GetLoopVector()[0]);
    assert(graph3.GetBB(7)->GetLoop() == la3.GetLoopVector()[0]);
    assert(graph3.GetBB(8)->GetLoop() == la3.GetLoopVector()[0]);

    // Irreducible loop so only 2 BBs inside
    assert(graph3.GetBB(2)->GetLoop() == la3.GetLoopVector()[1]);
    assert(graph3.GetBB(6)->GetLoop() == la3.GetLoopVector()[1]);

    assert(graph3.GetBB(1)->GetLoop() == la3.GetLoopVector()[2]);
    assert(graph3.GetBB(4)->GetLoop() == la3.GetLoopVector()[2]);
    assert(graph3.GetBB(5)->GetLoop() == la3.GetLoopVector()[2]);


}