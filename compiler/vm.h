#ifndef VM_H
#define VM_H

#include <vector>
#include <cstdint>
#include <list>
#include <memory>

class BasicBlock;
class InstructionBlock;

class VM
{
public:
    static VM *getVM()
    {
        if(!p_instance)
        {         
            p_instance = new VM();
        }
        return p_instance;
    }

    std::vector<uint64_t> regs;
    std::shared_ptr<BasicBlock>currbb;
    std::list<std::shared_ptr<BasicBlock>> logBB;
    int cmp_res;

private:
    static VM *p_instance;

    VM()
    {
        regs.resize(100);
    }
};

#endif // VM_H