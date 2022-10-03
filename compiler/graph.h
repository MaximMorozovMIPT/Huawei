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
// #include "vm.h"

class Graph
{
public:
    Graph() {}
    ~Graph(){}
    
    void AddStart(BasicBlock *v)
    {
        start = v;
    }

    void AddEdge(BasicBlock *v, BasicBlock *w)
    {
        adjacentVertices[v].insert(w);
        v->AddSuccs(w);
        w->AddPred(v);
    }

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

private:
    std::map<BasicBlock *, std::set<BasicBlock *>> adjacentVertices;
    BasicBlock* start = nullptr;
};

#endif // GRAPH_H