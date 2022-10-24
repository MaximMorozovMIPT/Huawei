#ifndef BASIC_BLOCK_H
#define BASIC_BLOCK_H

#include <list>
#include <set>
#include <cassert>
#include <memory>

#include "instruction_block.h"

enum DFSColor
{
    white,
    grey,
    black
};

class BasicBlock
{
public:
    BasicBlock(int id)
    {
        id_ = id;
        color = DFSColor::white;
    }

    void AddInstruction(InstructionBlock instr)
    {
        ilist.emplace_back(instr);
    }

    void AddPred(int bb)
    {
        preds.push_back(bb);
    }

    const std::vector<int> &GetPreds()
    {
        return preds;
    }

    void AddSuccs(int bb)
    {
        succs.push_back(bb);
    }

    const std::vector<int> &GetSuccs()
    {
        return succs;
    }

    void Execute()
    {
        VM::getVM()->logBB.emplace_back(this);
        for (auto & IB: ilist)
        {
            IB.CallInstr();
        }
    }

    int GetId()
    {
        return id_;
    }

private:
    DFSColor color;
    std::list<InstructionBlock> ilist;
    std::vector<int> preds;
    std::vector<int> succs;
    int id_;

    friend class DomTree;
};

#endif // BASIC_BLOCK_H