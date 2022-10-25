#include <memory>

#include "graph.h"

class DomTree
{
public:
    DomTree(Graph *graph)
    {
        graph_ = graph;
    }

    void BuildDomTree()
    {
        if (graph_->GetBBVector().empty())
        {
            return;
        }

        std::vector<int> head_dominated_over;

        for(int i = 1; i < graph_->GetBBVector().size(); ++i)
        {
            head_dominated_over.push_back(i);
            ExecDFS(0, i);
            graph_->ResolveDomination(i, FindWhite(i));
            ClearDFS();
        }
        graph_->ResolveDomination(0, head_dominated_over);
    }

private:
    void ExecDFS(const int head, const int exclude = -1)
    {
        graph_->GetBB(head)->color = DFSColor::grey;
        for (const auto & bb_id: graph_->GetBB(head)->GetSuccs())
        {
            if (bb_id == exclude)
            {
                continue;
            }

            if (graph_->GetBB(bb_id)->color == DFSColor::white)
            {
                ExecDFS(bb_id, exclude);
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

    std::vector<int> FindWhite(int exclude = -1)
    {
        std::vector<int> white_bb;
        for(auto &bb: graph_->GetBBVector())
        {
            if (bb->color == DFSColor::white && bb->GetId() != exclude)
            {
                white_bb.push_back(bb->GetId());
            }
        }
        return white_bb;
    }

    Graph *graph_;

};