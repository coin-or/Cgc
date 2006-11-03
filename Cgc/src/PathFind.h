/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef PATHFIND_H
#define PATHFIND_H
#include <vector>
#include <list>
#include <stack>
#include <NodeId.h>
namespace Cgc
{
    /// A vector of NodeId s define the simple path.
    typedef std::vector<NodeId> JustAPath;
    /// A vector of simple paths.
    typedef std::vector<JustAPath> JustPaths;
    /** @brief Solver which finds all possible paths in a graph.
    @ingroup Algorithms
    This isn't a particularly useful method, but it does demonstrate out to iterate across a graph in a safe fashion.
    */
    template < class GraphClass > 
    class PathFind
    {
        GraphClass &net;
        typedef typename GraphClass::iterator NodeIterator;
        typedef typename GraphClass::Node::iterator ArcIterator;
        typedef NodeIterator Candidate;
        typedef std::stack< Candidate > CandidateBin;

        CandidateBin candidates;
        NodeIterator src;
        NodeIterator snk;
        NodeId snkId;
        JustAPath pathStack;
        /// workhorse of the method.  Can cause an infinite loop.  Privately called under the coveres.
        bool recurse(JustPaths &paths)
        {
            Candidate topCand = candidates.top();
            JustAPath p ;

            if(topCand==snk)
            {
                // path complete
                p = pathStack;
                paths.push_back(p);
                return true;
            }

            for(ArcIterator arcIt = (*topCand).begin();
                arcIt != ((*topCand).end()); arcIt++)
            {
                /* only add in arcs not already in stack */
                /* avoids infinite recursion */
                NodeId arcDestId = net.getNodeId((*arcIt).head());
                Candidate destNodeIt = (*arcIt).head();
                JustAPath::iterator psIt;
                for(psIt = pathStack.begin(); psIt != pathStack.end(); psIt++)
                {
                    if((*psIt)==arcDestId)
                    {
                        break;
                    }
                }
                if(psIt==pathStack.end())
                {
                    //std::cout<<"Inserting nodeId="<<net.getNodeId(destNodeIt)<<std::endl;
                    candidates.push(destNodeIt);
                    pathStack.push_back(net.getNodeId(destNodeIt));
                    //destId not already in stack, recurse on it
                    recurse(paths);
                    candidates.pop();
                    pathStack.pop_back();
                }
                // else do nothing, found it node already visited
            }
            return (paths.size()>0);
        }
    public:
        /// @brief constructor, takes a graph, and source, and a sink node.
        PathFind( GraphClass &graph, const NodeIterator source,
            const NodeIterator sink )
            :net(graph),
            src(source),
            snk(sink)
        {
            NodeIterator nodeIt=source;
            if((nodeIt == net.end()) || (*nodeIt).size()==0)
            {
                std::cerr<<"Graph empty!"<<std::endl;
            }
            else
            {
                //std::cout<<"Inserting nodeId="<<net.getNodeId(nodeIt)<<std::endl;
                candidates.push(nodeIt);
            }
        }

        /** @brief Returns the paths found by the solver.
            @see JustPaths
            @see JustAPath
        */
        bool getPaths(JustPaths &paths)
        {
            paths.erase(paths.begin(),paths.end());
            return recurse(paths);
        }
    };
};
std::ostream &operator<<(std::ostream &os, const Cgc::JustAPath &path)
{
    os<<"Path("<<path.size()<<")";
    bool first=true;
    for(Cgc::JustAPath::const_iterator cjapci = path.begin();cjapci!=path.end();cjapci++)
    {
        if(!first)
        {
            first=true;
            os<<':';
        }
        os<<(*cjapci);
    }
    return os;
}
std::ostream &operator<<(std::ostream &os, const Cgc::JustPaths &paths)
{
    os<<" Paths["<<paths.size()<<"]"<<std::endl;
    for(Cgc::JustPaths::const_iterator japci = paths.begin();
        japci!= paths.end();japci++)
    {
        os<<(*japci)<<std::endl;
    }
    return os;
}
#endif

