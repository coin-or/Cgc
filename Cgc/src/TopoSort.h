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
    template <class NetType>
    class TopoSort
    {
    public:
        TopoSort()
        {}

        bool solve(NetType &net,std::vector<int> &order)
        {
            std::list<NodeId> activeList;
            order = std::vector<int>(net.size());
            int nodeCntr=0;
            int *inDegree = new int[net.size()];
            memset(inDegree,0,sizeof(int)*net.size());
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
            for(int cnt=0;cnt<net.size();cnt++)
            {
                if(inDegree[cnt]==0)
                {
                    activeList.push_back(NodeId(cnt));
                }
            }
            if(activeList.size()==0)
                return false;
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