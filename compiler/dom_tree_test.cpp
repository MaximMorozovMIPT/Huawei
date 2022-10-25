#include <cassert>

#include "graph.h"
#include "dom_tree.h"

void TestGraph1();

int main()
{
    TestGraph1();
}

void TestGraph1()
{
    Graph graph1;
    graph1.AddBBs(7);

    // Example 1
    // 0 = A, 1 = B, 2 = C, 3 = D, 4 = E, 5 = F, 6 = G
    graph1.AddInstruction(0, InstructionBlock(movi_u64, 0, 1)); 
    graph1.AddEdge(0, 1);

    graph1.AddInstruction(1, InstructionBlock(movi_u64, 0, 1)); 
    graph1.AddEdge(1, 2);
    graph1.AddEdge(1, 5);

    graph1.AddInstruction(2, InstructionBlock(movi_u64, 0, 1)); 
    graph1.AddEdge(2, 3);
    
    graph1.AddInstruction(3, InstructionBlock(movi_u64, 0, 1)); 
    graph1.AddInstruction(4, InstructionBlock(movi_u64, 0, 1)); 
    graph1.AddEdge(4, 3);

    graph1.AddInstruction(5, InstructionBlock(movi_u64, 0, 1));
    graph1.AddEdge(5, 4);
    graph1.AddEdge(5, 6);

    graph1.AddInstruction(6, InstructionBlock(movi_u64, 0, 1)); 
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