#include <memory>
#include <algorithm>

#include "graph.h"
#include "dom_tree.h"

class LoopAnalizer
{
public:
    LoopAnalizer(Graph *graph)
    {
        graph_ = graph;
    }

    void BuildLoopTree()
    {
        // 1st stage
        DomTree dt(graph_);
        dt.BuildDomTree();

        // 2d stage
        ExecDFSForLoopAnalizer(0);
        ClearDFS();
    }

private:
    void ExecDFSForLoopAnalizer(const int head)
    {
        graph_->GetBB(head)->color = DFSColor::grey;
        for (const auto & bb_id: graph_->GetBB(head)->GetSuccs())
        {
            if (graph_->GetBB(bb_id)->color == DFSColor::white)
            {
                ExecDFSForLoopAnalizer(bb_id);
            }
            
            if (graph_->GetBB(bb_id)->color == DFSColor::grey)
            {
                // block bb_id now is a header of loop cause backedge is to it
                bool is_irreducible = (std::find(graph_->GetBB(bb_id)->GetDominatesOver().begin(),
                                                 graph_->GetBB(bb_id)->GetDominatesOver().end(),
                                                 head) == graph_->GetBB(bb_id)->GetDominatesOver().end());

                graph_->AddBackEdge(head, bb_id, is_irreducible); // From head to bb_id
            }
        }
        graph_->GetBB(head)->color = DFSColor::black;
    }

    void ClearDFS()
    {
        for(auto &bb: graph_->GetBBVector())
        {
            bb->color = DFSColor::white;
        }
    }

    Graph *graph_;
};
