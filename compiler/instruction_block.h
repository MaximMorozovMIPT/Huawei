#ifndef INSTRUCTION_BLOCK_H
#define INSTRUCTION_BLOCK_H

#include <cstdint>
#include "vm.h"

class BasicBlock;

enum Instrs
{
    movi_u64,
    u32_to_u64,
    cmp_u64,
    mul_u64,
    addi_u64,
    ja,
    jmp,
    ret_u64,
    phi
};

class InstructionBlock
{
public:
    InstructionBlock(Instrs instr, BasicBlock* bb1)
    {
        instr_ = instr;
        bb1_ = bb1;
    }

    InstructionBlock(Instrs instr, BasicBlock* bb1, BasicBlock* bb2)
    {
        instr_ = instr;
        bb1_ = bb1;
        bb2_ = bb2;
    }

    InstructionBlock(Instrs instr, uint64_t val1)
    {
        instr_ = instr;
        val1_ = val1;
    }

    InstructionBlock(Instrs instr, uint64_t val1, uint64_t val2)
    {
        instr_ = instr;
        val1_ = val1;
        val2_ = val2;
    }

    InstructionBlock(Instrs instr, uint64_t val1, uint64_t val2, uint64_t val3)
    {
        instr_ = instr;
        val1_ = val1;
        val2_ = val2;
        val3_ = val3;
    }

    InstructionBlock(Instrs instr, uint64_t val1, uint64_t val2, uint64_t val3, BasicBlock* bb1, BasicBlock* bb2)
    {
        instr_ = instr;
        val1_ = val1;
        val2_ = val2;
        val3_ = val3;
        bb1_ = bb1;
        bb2_ = bb2;
    }

    void CallInstr()
    {
        std::cout << "CallInstr ";
        switch (instr_)
        {
            case movi_u64:
                Instr_movi_u64(val1_, val2_);
                break;
            case u32_to_u64:
                Instr_u32_to_u64(val1_, val2_);
                break;
            case cmp_u64:
                Instr_cmp_u64(val1_, val2_);
                break;
            case mul_u64:
                Instr_mul_u64(val1_, val2_, val3_);
                break;
            case addi_u64:
                Instr_addi_u64(val1_, val2_, val3_);
                break;
            case ja:
                Instr_ja(bb1_, bb2_);
                break;
            case jmp:
                Instr_jmp(bb1_);
                break;
            case ret_u64:
                Instr_ret_u64(val1_);
                break;
            case phi:
                // val1_ = src reg, val2_ reg from bb1, val3_ reg from bb2_
                Instr_phi(val1_, val2_, val3_, bb1_, bb2_);
                break;
            default:
                assert(false);
        }
    }
    
    Instrs instr_;
    BasicBlock *bb1_ = nullptr;
    BasicBlock *bb2_ = nullptr;
    uint64_t val1_ = 0;
    uint64_t val2_ = 0;
    uint64_t val3_ = 0;

private:
    // Basic
    void Instr_movi_u64(uint64_t reg, uint64_t value)
    {
        std::cout << "Instr_movi_u64" << std::endl;
        VM::getVM()->regs[reg] = value;
    }

    void Instr_u32_to_u64(uint64_t reg, uint64_t value)
    {
        std::cout << "Instr_u32_to_u64" << std::endl;
        VM::getVM()->regs[reg] = value;
    }

    void Instr_cmp_u64(uint64_t reg1, uint64_t reg2)
    {
        std::cout << "Instr_cmp_u64" << std::endl;
        VM::getVM()->cmp_res = VM::getVM()->regs[reg1] > VM::getVM()->regs[reg2] ? 1 : VM::getVM()->regs[reg1] == VM::getVM()->regs[reg2] ? 0 : -1;
    }

    // Arithmetic
    void Instr_mul_u64(uint64_t reg_dst, uint64_t reg_src1, uint64_t reg_src2)
    {
        std::cout << "Instr_mul_u64" << std::endl;
        VM::getVM()->regs[reg_dst] = VM::getVM()->regs[reg_src1] * VM::getVM()->regs[reg_src2];
    }

    void Instr_addi_u64(uint64_t reg_dst, uint64_t reg_src1, uint64_t imm)
    {
        std::cout << "Instr_addi_u64" << std::endl;
        VM::getVM()->regs[reg_dst] = VM::getVM()->regs[reg_src1] + imm;
    }

    // Control
    void Instr_ja(BasicBlock* success, BasicBlock* failure)
    {
        std::cout << "Instr_ja" << std::endl;
        if (VM::getVM()->cmp_res == 1)
        {
            VM::getVM()->currbb = success;
            return;
        }
        VM::getVM()->currbb = failure;
    }

    void Instr_jmp(BasicBlock* pc)
    {
        std::cout << "Instr_jmp" << std::endl;
        VM::getVM()->currbb = pc;
    }

    void Instr_ret_u64(uint64_t ret_reg)
    {
        std::cout << "Instr_ret_u64" << std::endl;
        // std::cout << VM::getVM()->regs[ret_reg] << std::endl;
        VM::getVM()->regs[99] = VM::getVM()->regs[ret_reg];
        VM::getVM()->currbb = nullptr;
    }

    void Instr_phi(uint64_t val1, uint64_t val2, uint64_t val3, BasicBlock* bb1, BasicBlock* bb2)
    {
        std::cout << "Instr_phi" << std::endl;
        int true_bb = 0;
        for(auto revit = VM::getVM()->logBB.rbegin(); revit != VM::getVM()->logBB.rend(); revit++)
        {
            if (*revit == bb1)
            {
                VM::getVM()->regs[val1] = VM::getVM()->regs[val2];
                return;
            }
            else if(*revit == bb2)
            {
                VM::getVM()->regs[val1] = VM::getVM()->regs[val3];
                return;
            }
        }
        assert(false);
    }
};

#endif // INSTRUCTION_BLOCK_H