#include <memory>
#include <algorithm>

#include "graph.h"
#include "dom_tree.h"


class Loop
{
public:
    Loop(int header, int backedge_src, bool is_irreducible)
    {
        header_ = header;
        backedge_src_ = backedge_src;
        is_irreducible_ = is_irreducible;
    }

    int GetHeader()
    {
        return header_;
    }

    int GetBackEdgeSource()
    {
        return backedge_src_;
    }

    void SetOuterLoop(Loop *outer_loop)
    {
        outer_loop_ = outer_loop;
    }

    Loop *GetOuterLoop()
    {
        return outer_loop_;
    }

    std::vector<Loop *> GetInnerLoops()
    {
        return inner_loops_;
    }

    void AddInnerLoop(Loop *loop)
    {
        inner_loops_.push_back(loop);
    }

    bool IsIrreducible()
    {
        return is_irreducible_;
    }

private:
    bool is_irreducible_;
    int header_;
    int backedge_src_;
    Loop *outer_loop_ = nullptr;
    std::vector<Loop *> inner_loops_;
};

class LoopAnalizer
{
public:
    LoopAnalizer(Graph *graph)
    {
        graph_ = graph;
    }

    void BuildLoopTree()
    {
        // Init loop vector
        Loop *root = new Loop(-1, -1, false);
        loop_vec.push_back(root);

        // 1st stage
        DomTree dt(graph_);
        dt.BuildDomTree();

        // 2d stage
        ExecDFSForLoopAnalizer(0);
        ClearDFS();

        // 3d stage
        PopulateLoopsTraversal(0);
        ClearDFS();

        // 4th stage
        CompleteLoopTree();
    }

    std::vector<Loop *> GetLoopVector()
    {
        return loop_vec;
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

                Loop *tmp_loop = new Loop(bb_id, head, is_irreducible);
                loop_vec.push_back(tmp_loop);
                graph_->GetBB(bb_id)->SetLoop(tmp_loop); // Set here only header
            }
        }
        graph_->GetBB(head)->color = DFSColor::black;
    }

    void PopulateLoopsTraversal(const int head)
    {
        graph_->GetBB(head)->color = DFSColor::grey;
        for (const auto & bb_id: graph_->GetBB(head)->GetSuccs())
        {
            if (graph_->GetBB(bb_id)->color == DFSColor::white)
            {
                PopulateLoopsTraversal(bb_id);
            }
        }
        if (graph_->GetBB(head)->GetLoop() == nullptr)
        {
            graph_->GetBB(head)->color = DFSColor::black;
            return;
        }

        if (graph_->GetBB(head)->GetLoop()->GetHeader() == head && !(graph_->GetBB(head)->GetLoop()->IsIrreducible()))
        {
            graph_->GetBB(head)->lscolor = LSColor::green;
            PopulateLoopsDFS(graph_->GetBB(head)->GetLoop()->GetBackEdgeSource(), graph_->GetBB(head)->GetLoop());
            ClearPLDFS();
        }

        if (graph_->GetBB(head)->GetLoop()->GetHeader() == head && graph_->GetBB(head)->GetLoop()->IsIrreducible())
        {
            graph_->GetBB(graph_->GetBB(head)->GetLoop()->GetBackEdgeSource())->SetLoop(graph_->GetBB(head)->GetLoop());
            ClearPLDFS();
        }

        graph_->GetBB(head)->color = DFSColor::black;
    }

    void PopulateLoopsDFS(const int head, Loop *current_loop)
    {
        graph_->GetBB(head)->lscolor = LSColor::green;
        for (const auto & bb_id: graph_->GetBB(head)->GetPreds())
        {
            if (graph_->GetBB(bb_id)->lscolor != LSColor::green)
            {
                PopulateLoopsDFS(bb_id, current_loop);
            }
        }

        if (graph_->GetBB(head)->GetLoop() == nullptr)
        {
            graph_->GetBB(head)->SetLoop(current_loop);
        }
        if (graph_->GetBB(head)->GetLoop() != nullptr && graph_->GetBB(head)->GetLoop()->GetOuterLoop() == nullptr)
        {
            graph_->GetBB(head)->GetLoop()->SetOuterLoop(current_loop);
            current_loop->AddInnerLoop(graph_->GetBB(head)->GetLoop());
        }
    }

    void ClearDFS()
    {
        for(auto &bb: graph_->GetBBVector())
        {
            bb->color = DFSColor::white;
        }
    }

    void ClearPLDFS()
    {
        for(auto &bb: graph_->GetBBVector())
        {
            bb->lscolor = LSColor::empty;
        }
    }

    void CompleteLoopTree()
    {
        for(int i = 1; i < loop_vec.size(); ++i)
        {
            if (loop_vec[i]->GetOuterLoop() == nullptr)
            {
                loop_vec[i]->SetOuterLoop(loop_vec[0]);
                loop_vec[0]->AddInnerLoop(loop_vec[i]);
            }
        }
        for(auto &bb: graph_->GetBBVector())
        {
            if (bb->GetLoop() == nullptr)
            {
                bb->SetLoop(loop_vec[0]);
            }
        }
    }

    std::vector<Loop *> loop_vec;
    Graph *graph_;
};
