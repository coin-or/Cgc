/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef TOPOSORT_H
#define TOPOSORT_H
#include <vector>
#include <list>
#include <NodeId.h>
namespace Cgc
{
    /** Solver to topologically sort a graph if it's possible.
        @ingroup Algorithms
        The solve method takes any graph.  It doesn't need any data on the graph or any
        special methods.  
        It returns true on success and the output order vector contains the order in the vector.  
        For instance, the 0th entry in the order vector is the NodeId Zero in the graph's sequence number.
        If false is returned there is a cycle in the graph.  Graphs with cycles cannot (in original form) 
        be topologically sorted.
        Basically, the algorithm creates a queue of in-degree-of-zero nodes.  It adds the zero-degree nodes to the 
        'sorted list'.   Then scans outward from the arcs on those nodes, decrementing head-node-in-degree for each 
        arc and checks to see if those then have zero degree.  if they do, then they go into the 
        in-degree-of-zero list.  Continue until every arc is traversed, or there are no in-degree-zero nodes.  
        If every arc is visited, then it will stop and will be complete.  Gets O(m) behavior -- vists every arc once.
        @note There can be multiple equivalent sequences, this doesn't try to find alternates.
    */
    template <class NetType>
    class TopoSort
    {
    public:
        TopoSort()
        {}
        
        bool solve(NetType &net,std::vector<int> &order)
        {
            // active list
            std::list<NodeId> activeList;
            // allocate space for output.
            order = std::vector<int>(net.size());
            int nodeCntr=0;
            // use in-degree to drive where we visit next.
            int *inDegree = new int[net.size()];
            // clean out the memory.
            memset(inDegree,0,sizeof(int)*net.size());
            // Compute the starting in-degrees
            for(NodeId nodeId = 0;nodeId < net.size();nodeId++)
            {
                NetType::iterator nodeIt = net.find(nodeId);
                for(NetType::Node::iterator arcIt = (*nodeIt).begin();
                    arcIt != (*nodeIt).end(); arcIt++)
                {
                    NodeId nid = net.getNodeId((*arcIt).head());
                    inDegree[nid.getNodeId()]++;
                }
            }
            // if we found inDegrees actually zero, put them on the active list.
            for(int cnt=0;cnt<net.size();cnt++)
            {
                if(inDegree[cnt]==0)
                {
                    activeList.push_back(NodeId(cnt));
                }
            }
            // If we found none, there are no nodes which aren't in a cycle.
            if(activeList.size()==0)
                return false;
            // Now, while we have zero degree entries, keep making the output list.
            while(activeList.size()>0)
            {
                NodeId currNode = activeList.front();
                activeList.pop_front();
                order[currNode.getNodeId()]=nodeCntr++;
                NetType::iterator currNodeIt = net.find(currNode);
                for(NetType::Node::iterator arcIt = (*currNodeIt).begin();
                    arcIt != (*currNodeIt).end();arcIt++)
                {
                    int location = net.getNodeId((*arcIt).head()).getNodeId();
                    inDegree[location]--;
                    if(inDegree[location]==0)
                    {
                        activeList.push_back(location);
                    }
                }
                // I don't think this should ever happen.
                if(currNode>net.size())
                    return false;
            }
            delete [] inDegree;
            if(nodeCntr>net.size())
            {
                // probably shouldn't happen.
                return false;
            }
            if(nodeCntr<net.size())
            {
                // didn't order them all.  Hit a cycle.
                return false;
            }
            return true;
        }
    };
}
#endif