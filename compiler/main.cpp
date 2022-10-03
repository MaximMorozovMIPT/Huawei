#include <iostream>
#include <cstdint>
#include <list>
#include <vector>
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

void TestControlFlow(std::vector<BasicBlock*>);

int main()
{
    Graph graph;
    BasicBlock *bb1 = new BasicBlock;
    BasicBlock *bb2 = new BasicBlock;
    BasicBlock *bb3 = new BasicBlock;
    BasicBlock *bb4 = new BasicBlock;

    int fact_of = 10;

    bb1->AddInstruction(InstructionBlock(movi_u64, 0, 1));
    bb1->AddInstruction(InstructionBlock(movi_u64, 10, 2));
    bb1->AddInstruction(InstructionBlock(u32_to_u64, 11, fact_of));
    bb1->AddInstruction(InstructionBlock(jmp, bb2));
    graph.AddEdge(bb1, bb2);

    bb2->AddInstruction(InstructionBlock(phi, 12, 10, 13, bb1, bb3));
    bb2->AddInstruction(InstructionBlock(phi, 1, 0, 2, bb1, bb3));
    bb2->AddInstruction(InstructionBlock(cmp_u64, 12, 11));
    bb2->AddInstruction(InstructionBlock(ja, bb4, bb3));
    graph.AddEdge(bb2, bb3);
    graph.AddEdge(bb2, bb4);

    bb3->AddInstruction(InstructionBlock(mul_u64, 2, 1, 12));
    bb3->AddInstruction(InstructionBlock(addi_u64, 13, 12, 1));
    bb3->AddInstruction(InstructionBlock(jmp, bb2));
    graph.AddEdge(bb3, bb2);

    bb4->AddInstruction(InstructionBlock(ret_u64, 1));
    graph.AddStart(bb1);
    graph.Execute();

    assert(VM::getVM()->regs[99] == 3628800);

    TestControlFlow({bb1, bb2, bb3, bb4});

    delete bb1;
    delete bb2;
    delete bb3;
    delete bb4;

    return 0;
}

void TestControlFlow(std::vector<BasicBlock*> bbvec)
{
    std::list<BasicBlock *> check_control_flow;
    check_control_flow.emplace_back(bbvec[0]);
    for(int i = 0; i < 9; ++i)
    {
        check_control_flow.emplace_back(bbvec[1]);
        check_control_flow.emplace_back(bbvec[2]);
    }
    check_control_flow.emplace_back(bbvec[1]);
    check_control_flow.emplace_back(bbvec[3]);


    assert(VM::getVM()->logBB.size() == check_control_flow.size());

    auto lit1 = VM::getVM()->logBB.begin();
    auto lit2 = check_control_flow.begin();
    int i = 0;
    for(; lit1 != VM::getVM()->logBB.end() && lit2 != check_control_flow.end(); lit1++, lit2++)
    {
        assert(*lit1 == *lit2);
    }
}