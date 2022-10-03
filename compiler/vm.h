#ifndef VM_H
#define VM_H

#include <vector>
#include <cstdint>
#include <list>

class BasicBlock;

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
    int cmp_res;
    BasicBlock *currbb;
    std::list<BasicBlock *> logBB;

private:
    static VM *p_instance;

    VM()
    {
        regs.resize(100);
    }
};

#endif // VM_H