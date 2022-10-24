#include <iostream>
#include <cstdint>
#include <list>
#include <vector>
#include <memory>
#include <algorithm>

#include "vm.h"
#include "graph.h"

VM *VM::p_instance;

// SSA ir to count factorial
// Registers 'i' for loop iteration, 'v' for data

/*
start:
    movi_u64, v0, 1
    movi_u64, i0, 2
    u32_to_u64, i1, fact_of
    jmp loopcond

loopcond:
    i2 = phi(i0, i3)
    v1 = phi(v0, v2)
    cmp_u64, i2, i1
    ja, done, loop

loop:
    mul_u64, v2, v1, i2
    addi_u64, i3, i2, v1
    jmp, loopcond

done:
    ret_u64 v1 // writes to reg 99
*/

void TestControlFlow(const std::vector<std::shared_ptr<BasicBlock>> &bbvec);
void TestGraph1(const Graph &test_graph);

int main()
{
    Graph graph;
    graph.AddBBs(4);

    int fact_of = 10;

    graph.AddInstruction(0, InstructionBlock(movi_u64, 0, 1));                                          // id = 0
    graph.AddInstruction(0, InstructionBlock(movi_u64, 10, 2));                                         // id = 1
    graph.AddInstruction(0, InstructionBlock(u32_to_u64, 11, fact_of));                                 // id = 2
    graph.AddInstruction(0, InstructionBlock(jmp, graph.GetBB(1)));                                     // id = 3

    graph.AddEdge(0, 1);

    graph.AddInstruction(1, InstructionBlock(phi, 12, {{10, graph.GetBB(0)}, {13, graph.GetBB(2)}}));   // id = 4
    graph.AddInstruction(1, InstructionBlock(phi, 1, {{0, graph.GetBB(0)}, {2, graph.GetBB(2)}}));      // id = 5
    graph.AddInstruction(1, InstructionBlock(cmp_u64, 12, 11));                                         // id = 6
    graph.AddInstruction(1, InstructionBlock(ja, graph.GetBB(3), graph.GetBB(2)));                      // id = 7
    graph.AddEdge(1, 2);
    graph.AddEdge(1, 3);

    graph.AddInstruction(2, InstructionBlock(mul_u64, 2, 1, 12));                                       // id = 8
    graph.AddInstruction(2, InstructionBlock(addi_u64, 13, 12, 1));                                     // id = 9
    graph.AddInstruction(2, InstructionBlock(jmp, graph.GetBB(1)));                                     // id = 10
    graph.AddEdge(2, 1);

    graph.AddInstruction(3, InstructionBlock(ret_u64, 1));                                              // id = 11

    graph.CreateDFG();

    TestGraph1(graph);

    // graph.AddStart(graph.GetBB(0));
    // graph.Execute();
    // assert(VM::getVM()->regs[99] == 3628800);
    // TestControlFlow({graph.GetBB(0), graph.GetBB(1), graph.GetBB(2), graph.GetBB(3)});

    return 0;
}

void TestGraph1(const Graph &test_graph)
{
    auto bb_vec = test_graph.GetBBVector();
    auto ib_vec = test_graph.GetIBVector();

    // ------------------
    // Test ID part
    // ------------------

    for(int i = 0; i < bb_vec.size(); ++i)
    {
        assert(bb_vec[i]->GetId() == i);
    }

    for(int i = 0; i < ib_vec.size(); ++i)
    {
        assert(ib_vec[i]->GetId() == i);
    }

    // ------------------
    // Test CFG part
    // ------------------

    // Test preds
    // Check if basic block bb_vec[i] contains basic block number j, where j 0-3 in predesessor vector
    assert(std::find(bb_vec[0]->GetPreds().begin(), bb_vec[0]->GetPreds().end(), 1) == bb_vec[0]->GetPreds().end());
    assert(std::find(bb_vec[0]->GetPreds().begin(), bb_vec[0]->GetPreds().end(), 2) == bb_vec[0]->GetPreds().end());
    assert(std::find(bb_vec[0]->GetPreds().begin(), bb_vec[0]->GetPreds().end(), 3) == bb_vec[0]->GetPreds().end());

    assert(std::find(bb_vec[1]->GetPreds().begin(), bb_vec[1]->GetPreds().end(), 0) != bb_vec[1]->GetPreds().end());
    assert(std::find(bb_vec[1]->GetPreds().begin(), bb_vec[1]->GetPreds().end(), 2) != bb_vec[1]->GetPreds().end());
    assert(std::find(bb_vec[1]->GetPreds().begin(), bb_vec[1]->GetPreds().end(), 3) == bb_vec[1]->GetPreds().end());

    assert(std::find(bb_vec[2]->GetPreds().begin(), bb_vec[2]->GetPreds().end(), 0) == bb_vec[2]->GetPreds().end());
    assert(std::find(bb_vec[2]->GetPreds().begin(), bb_vec[2]->GetPreds().end(), 1) != bb_vec[2]->GetPreds().end());
    assert(std::find(bb_vec[2]->GetPreds().begin(), bb_vec[2]->GetPreds().end(), 3) == bb_vec[2]->GetPreds().end());

    assert(std::find(bb_vec[3]->GetPreds().begin(), bb_vec[3]->GetPreds().end(), 0) == bb_vec[3]->GetPreds().end());
    assert(std::find(bb_vec[3]->GetPreds().begin(), bb_vec[3]->GetPreds().end(), 1) != bb_vec[3]->GetPreds().end());
    assert(std::find(bb_vec[3]->GetPreds().begin(), bb_vec[3]->GetPreds().end(), 2) == bb_vec[3]->GetPreds().end());

    // Test succs
    // Check if basic block bb_vec[i] contains basic block number j, where j 0-3 in successor vector
    assert(std::find(bb_vec[0]->GetSuccs().begin(), bb_vec[0]->GetSuccs().end(), 1) != bb_vec[0]->GetSuccs().end());
    assert(std::find(bb_vec[0]->GetSuccs().begin(), bb_vec[0]->GetSuccs().end(), 2) == bb_vec[0]->GetSuccs().end());
    assert(std::find(bb_vec[0]->GetSuccs().begin(), bb_vec[0]->GetSuccs().end(), 3) == bb_vec[0]->GetSuccs().end());

    assert(std::find(bb_vec[1]->GetSuccs().begin(), bb_vec[1]->GetSuccs().end(), 0) == bb_vec[1]->GetSuccs().end());
    assert(std::find(bb_vec[1]->GetSuccs().begin(), bb_vec[1]->GetSuccs().end(), 2) != bb_vec[1]->GetSuccs().end());
    assert(std::find(bb_vec[1]->GetSuccs().begin(), bb_vec[1]->GetSuccs().end(), 3) != bb_vec[1]->GetSuccs().end());

    assert(std::find(bb_vec[2]->GetSuccs().begin(), bb_vec[2]->GetSuccs().end(), 0) == bb_vec[2]->GetSuccs().end());
    assert(std::find(bb_vec[2]->GetSuccs().begin(), bb_vec[2]->GetSuccs().end(), 1) != bb_vec[2]->GetSuccs().end());
    assert(std::find(bb_vec[2]->GetSuccs().begin(), bb_vec[2]->GetSuccs().end(), 3) == bb_vec[2]->GetSuccs().end());

    assert(std::find(bb_vec[3]->GetSuccs().begin(), bb_vec[3]->GetSuccs().end(), 0) == bb_vec[3]->GetSuccs().end());
    assert(std::find(bb_vec[3]->GetSuccs().begin(), bb_vec[3]->GetSuccs().end(), 1) == bb_vec[3]->GetSuccs().end());
    assert(std::find(bb_vec[3]->GetSuccs().begin(), bb_vec[3]->GetSuccs().end(), 2) == bb_vec[3]->GetSuccs().end());

    // ------------------
    // Test DFG part
    // ------------------

    // Test succs
    assert(ib_vec[0]->GetSuccs().size() == 1);
    assert(ib_vec[1]->GetSuccs().size() == 1);
    assert(ib_vec[2]->GetSuccs().size() == 1);
    assert(ib_vec[3]->GetSuccs().size() == 0);
    assert(ib_vec[4]->GetSuccs().size() == 3);
    assert(ib_vec[5]->GetSuccs().size() == 2);
    assert(ib_vec[6]->GetSuccs().size() == 0);
    assert(ib_vec[7]->GetSuccs().size() == 0);
    assert(ib_vec[8]->GetSuccs().size() == 1);
    assert(ib_vec[9]->GetSuccs().size() == 1);
    assert(ib_vec[10]->GetSuccs().size() == 0);
    assert(ib_vec[11]->GetSuccs().size() == 0);

    // Check if instruction block ib_vec[i] contains instruction block number j,
    // where j can be 0-11 in successor vector
    assert(std::find(ib_vec[0]->GetSuccs().begin(), ib_vec[0]->GetSuccs().end(), 5) != ib_vec[0]->GetSuccs().end());
    assert(std::find(ib_vec[1]->GetSuccs().begin(), ib_vec[1]->GetSuccs().end(), 4) != ib_vec[1]->GetSuccs().end());
    assert(std::find(ib_vec[2]->GetSuccs().begin(), ib_vec[2]->GetSuccs().end(), 6) != ib_vec[2]->GetSuccs().end());
    assert(std::find(ib_vec[4]->GetSuccs().begin(), ib_vec[4]->GetSuccs().end(), 6) != ib_vec[4]->GetSuccs().end());
    assert(std::find(ib_vec[4]->GetSuccs().begin(), ib_vec[4]->GetSuccs().end(), 8) != ib_vec[4]->GetSuccs().end());
    assert(std::find(ib_vec[4]->GetSuccs().begin(), ib_vec[4]->GetSuccs().end(), 9) != ib_vec[4]->GetSuccs().end());
    assert(std::find(ib_vec[5]->GetSuccs().begin(), ib_vec[5]->GetSuccs().end(), 8) != ib_vec[5]->GetSuccs().end());
    assert(std::find(ib_vec[5]->GetSuccs().begin(), ib_vec[5]->GetSuccs().end(), 11) != ib_vec[5]->GetSuccs().end());
    assert(std::find(ib_vec[8]->GetSuccs().begin(), ib_vec[8]->GetSuccs().end(), 5) != ib_vec[8]->GetSuccs().end());
    assert(std::find(ib_vec[9]->GetSuccs().begin(), ib_vec[9]->GetSuccs().end(), 4) != ib_vec[9]->GetSuccs().end());

    // Test preds
    assert(ib_vec[0]->GetPreds().size() == 0);
    assert(ib_vec[1]->GetPreds().size() == 0);
    assert(ib_vec[2]->GetPreds().size() == 0);
    assert(ib_vec[3]->GetPreds().size() == 0);
    assert(ib_vec[4]->GetPreds().size() == 2);
    assert(ib_vec[5]->GetPreds().size() == 2);
    assert(ib_vec[6]->GetPreds().size() == 2);
    assert(ib_vec[7]->GetPreds().size() == 0);
    assert(ib_vec[8]->GetPreds().size() == 2);
    assert(ib_vec[9]->GetPreds().size() == 1);
    assert(ib_vec[10]->GetPreds().size() == 0);
    assert(ib_vec[11]->GetPreds().size() == 1);
}

// void TestControlFlow(const std::vector<std::shared_ptr<BasicBlock>> &bbvec)
// {
//     std::list<std::shared_ptr<BasicBlock>> check_control_flow;
//     check_control_flow.emplace_back(bbvec[0]);
//     for(int i = 0; i < 9; ++i)
//     {
//         check_control_flow.emplace_back(bbvec[1]);
//         check_control_flow.emplace_back(bbvec[2]);
//     }
//     check_control_flow.emplace_back(bbvec[1]);
//     check_control_flow.emplace_back(bbvec[3]);
//     assert(VM::getVM()->logBB.size() == check_control_flow.size());
//     auto lit1 = VM::getVM()->logBB.begin();
//     auto lit2 = check_control_flow.begin();
//     int i = 0;
//     for(; lit1 != VM::getVM()->logBB.end() && lit2 != check_control_flow.end(); lit1++, lit2++)
//     {
//         assert(*lit1 == *lit2);
//     }
// }