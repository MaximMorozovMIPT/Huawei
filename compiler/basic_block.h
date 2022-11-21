#ifndef BASIC_BLOCK_H
#define BASIC_BLOCK_H

#include <list>
#include <set>
#include <cassert>
#include <memory>
#include <utility>

#include "instruction_block.h"

class Loop;

enum DFSColor
{
    white,
    grey,
    black
};

enum LSColor
{
    empty,
    green
};

class BasicBlock
{
public:
    BasicBlock(int id)
    {
        id_ = id;
        color = DFSColor::white;
        lscolor = LSColor::empty;
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

    void AddDominatesOver(std::vector<int> dom_succs)
    {
        dominates_over = dom_succs;
    }

    const std::vector<int> &GetDominatesOver()
    {
        return dominates_over;
    }

    void AddDominator(int bb_id)
    {
        dominators_vec.push_back(bb_id);
    }

    const std::vector<int> &GetDominators()
    {
        return dominators_vec;
    }

    void Execute()
    {
        VM::getVM()->logBB.emplace_back(this);
        for (auto & IB: ilist)
        {
            IB.CallInstr();
        }
    }

    void SetLoop(Loop *new_loop)
    {
        loop = new_loop;
    }

    Loop *GetLoop()
    {
        return loop;
    }

    int GetId()
    {
        return id_;
    }


private:
    DFSColor color;
    LSColor lscolor;
    std::list<InstructionBlock> ilist;
    std::vector<int> preds;
    std::vector<int> succs;
    std::vector<int> dominates_over;
    std::vector<int> dominators_vec;
    int id_;
    Loop *loop = nullptr;

    friend class DomTree;
    friend class LoopAnalizer;
};

#endif // BASIC_BLOCK_H