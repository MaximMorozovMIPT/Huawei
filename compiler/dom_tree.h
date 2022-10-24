#include <memory>

#include "graph.h"

class DomTree
{
public:
    DomTree(Graph *graph)
    {
        graph_ = graph;
    }

    void ExecDFS(const int head)
    {
        graph_->GetBB(head)->color = DFSColor::grey;
        for (const auto & bb: graph_->GetBB(head)->GetSuccs())
        {
            if (graph_->GetBB(bb)->color == DFSColor::white)
            {
                ExecDFS(bb);
            }
            // std::cout << graph_->GetBB(bb)->GetId() << std::endl;
        }
        graph_->GetBB(head)->color = DFSColor::black;
        std::cout << graph_->GetBB(head)->GetId() << std::endl;
    }

private:
    Graph *graph_;
};