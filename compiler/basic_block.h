#ifndef BASIC_BLOCK_H
#define BASIC_BLOCK_H

#include <list>
#include <set>
#include "instruction_block.h"
// #include "vm.h"

class BasicBlock
{
public:
    void AddInstruction(InstructionBlock instr)
    {
        ilist.emplace_back(instr);
    }

    void AddPred(BasicBlock *bb)
    {
        preds.insert(bb);
    }

    void AddSuccs(BasicBlock *bb)
    {
        succs.insert(bb);
    }

    void Execute()
    {
        VM::getVM()->logBB.emplace_back(this);
        for (auto & IB: ilist)
        {
            IB.CallInstr();
        }
    }

private:
    std::list<InstructionBlock> ilist;
    std::set<BasicBlock *> preds;
    std::set<BasicBlock *> succs;
};

#endif // BASIC_BLOCK_H