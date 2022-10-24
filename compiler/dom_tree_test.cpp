#include "graph.h"
#include "dom_tree.h"

int main()
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
    dt1.ExecDFS(0);
}