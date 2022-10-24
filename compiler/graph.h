#ifndef GRAPH_H
#define GRAPH_H

#include <set>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <random>
#include <map>
#include <cassert>

#include "basic_block.h"

class Graph
{
public:
    Graph()
    {
        bb_counter = 0;
        inst_counter = 0;
        reg_assigment_to_instr_vector.resize(100);

    }
    ~Graph(){}

    // ---------------------------
    // Basic block functions
    // ---------------------------
    void AddBB()
    {
        BB_vec.push_back(std::make_shared<BasicBlock>(GetBBId()));
        assert(BB_vec[bb_counter - 1]->GetId() == bb_counter - 1);
    }

    void AddBBs(int num)
    {
        for(int i = 0; i < num; ++i)
        {
            AddBB();
        }
    }

    const std::shared_ptr<BasicBlock> &GetBB(int i)
    {
        return BB_vec[i];
    }

    void AddEdge(int v, int w)
    {
        BB_vec[v]->AddSuccs(w);
        BB_vec[w]->AddPred(v);
    }

    const std::vector<std::shared_ptr<BasicBlock>> &GetBBVector()
    {
        return BB_vec;
    }

    // ---------------------------
    // Instruction block functions
    // ---------------------------

    void AddInstruction(int bb_index, InstructionBlock instr)
    {
        instr.SetId(GetInstId());
        GetBB(bb_index)->AddInstruction(instr);
        IB_vec.push_back(std::make_shared<InstructionBlock>(instr));
        switch (instr.instr_)
        {
            case Instrs::movi_u64:
            case Instrs::u32_to_u64:
            case Instrs::mul_u64:
            case Instrs::addi_u64:
            case Instrs::phi:
                // val1_ assigned this id
                reg_assigment_to_instr_vector[instr.val1_] = instr.GetId();
                break;
            default:
                break;
        }
    }

    const std::vector<std::shared_ptr<InstructionBlock>> &GetIBVector()
    {
        return IB_vec;
    }

    void CreateDFG()
    {
        for(const auto &instr: IB_vec)
        {
            switch (instr->instr_)
            {
                // One input
                case ret_u64:
                    instr->preds.push_back(reg_assigment_to_instr_vector[instr->val1_]);
                    IB_vec[reg_assigment_to_instr_vector[instr->val1_]]->succs.push_back(instr->GetId());
                    break;
                case addi_u64:
                    instr->preds.push_back(reg_assigment_to_instr_vector[instr->val2_]);
                    IB_vec[reg_assigment_to_instr_vector[instr->val2_]]->succs.push_back(instr->GetId());
                    break;
                // Two inputs
                case cmp_u64:
                    instr->preds.push_back(reg_assigment_to_instr_vector[instr->val1_]);
                    IB_vec[reg_assigment_to_instr_vector[instr->val1_]]->succs.push_back(instr->GetId());
                    instr->preds.push_back(reg_assigment_to_instr_vector[instr->val2_]);
                    IB_vec[reg_assigment_to_instr_vector[instr->val2_]]->succs.push_back(instr->GetId());
                    break;
                case mul_u64:
                    instr->preds.push_back(reg_assigment_to_instr_vector[instr->val2_]);
                    IB_vec[reg_assigment_to_instr_vector[instr->val2_]]->succs.push_back(instr->GetId());
                    instr->preds.push_back(reg_assigment_to_instr_vector[instr->val3_]);
                    IB_vec[reg_assigment_to_instr_vector[instr->val3_]]->succs.push_back(instr->GetId());
                    break;
                // Multiple input
                case phi:
                    for(const auto &phiStruct: instr->phi_)
                    {
                        instr->preds.push_back(reg_assigment_to_instr_vector[phiStruct.val]);
                        IB_vec[reg_assigment_to_instr_vector[phiStruct.val]]->succs.push_back(instr->GetId());
                    }
                default:
                    break;
            }
        }
    }

    // ---------------------------
    // Interpreter functions
    // ---------------------------

    void Execute()
    {
        assert(start != nullptr);
        VM::getVM()->currbb = start;
        start->Execute();

        while(VM::getVM()->currbb != nullptr)
        {
            VM::getVM()->currbb->Execute();
        }
    }

    void AddStart(std::shared_ptr<BasicBlock> v)
    {
        start = v;
    }

private:
    int GetBBId()
    {
        ++bb_counter;
        return bb_counter - 1;
    }

    int GetInstId()
    {
        ++inst_counter;
        return inst_counter - 1;
    }

    std::shared_ptr<BasicBlock> start;
    std::vector<std::shared_ptr<BasicBlock>> BB_vec;
    std::vector<std::shared_ptr<InstructionBlock>> IB_vec;
    std::vector<int> reg_assigment_to_instr_vector;

    int bb_counter;
    int inst_counter;
};

#endif // GRAPH_H