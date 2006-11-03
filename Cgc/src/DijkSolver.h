/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#ifndef DIJKSOLVER_H
#define DIJKSOLVER_H

#include <Path.h>
#include <NodeCostComp.h>
#include <algorithm>
#include <iterator>

namespace Cgc
{
/** @brief Shortest single source to all sink solver.
    @ingroup Algorithms
    This solver uses a Dijkstra-inspired algorithm to find the shortest
    paths between a single source node and all other connected nodes.
    @li It can be stumped by negative costs.
    @li It requires the client Arc storage type to have a int getCost() method.
    @li It requires the client Node storage type to have a int getLabel() 
    @li No particular graph type requirements.
   
 */
template < class NetType , class NodeIterator >
class DijkSolver
{
    typedef typename NetType::Node NetNode;
    typedef typename NetType::Arc NetArc;
    typedef typename NetType::iterator NetIter;
    typedef typename NetNode::iterator NetNodeIter;

    typedef std::vector < NetIter > HeapType;
    HeapType dijkHeap;
      
public:
    /// Call this to solve the problem.
    int solve(NetType &net, NodeIterator &source);
    /// Call this to fetch out a path to a particular sink.
    void getPath(
                 const NetType &net, 
                 const NodeIterator &sink, 
                 Cgc::Path &pathToFill)const;
};

/** @note Technical detail: This MAXCOST is why the int data type is forced.
    This could be forced using an abstract class with a getMax() method.
    @todo do the conversion.
 */
#ifndef __DIJKMAXCOST
#define __DIJKMAXCOST INT_MAX
#endif

template < class NetType , class NodeIterator >
int DijkSolver<NetType, NodeIterator>::solve( NetType &net, NodeIterator &source)
{
    int oldCost;
    int newCost;
    int iterCnt=0;
    // clean out last use of heap
    dijkHeap.erase(dijkHeap.begin(),dijkHeap.end());
  
    // reset labels to 'inifinity'
    for(NodeIterator nodeIter=net.begin();nodeIter != net.end();nodeIter++)
        {
        NetNode & nRef= const_cast<NetNode &>(*nodeIter);
        (*nRef).setLabel(__DIJKMAXCOST, net.getNodeId(nodeIter) );
        }
    NetNode &node = const_cast<NetNode &>(*source);
    (*(node)).setLabel( 0, net.getNodeId(source));
    dijkHeap.push_back(source);
    while(dijkHeap.size())
        {
        ++iterCnt;
        if(iterCnt%10000==0)
            {
            iterCnt=0;
            std::cout<<"DijkSolver completed 10,000 iterations"<<std::endl;
            std::cout<<" heapSize="<<dijkHeap.size()<<std::endl;
            }
    
        NodeCostComp < NodeIterator > heapComparitor;
        std::pop_heap(dijkHeap.begin(),dijkHeap.end(),heapComparitor);
        NetIter thisNodeIter =  *(dijkHeap.rbegin());
        dijkHeap.pop_back();
        //dijkHeap.pop_back();
        // fix node label
        
        NetNodeIter stopper =  (*thisNodeIter).end();
        for(NetNodeIter arcIter = (*thisNodeIter).begin();
            arcIter != stopper; arcIter++)
            {
            if((*(*arcIter)).valid())
                {
                NetNode &nn = const_cast<NetNode &>((*(*arcIter).head()));
                oldCost = (*(nn)).getLabel();
                const NetArc &na = (*(arcIter));
                newCost = (*(*thisNodeIter)).getLabel() + (*(na)).getCost();
                if( newCost < oldCost )
                    {
                    // This indicates the the destination is not already in the arc heap.
                    if(oldCost ==__DIJKMAXCOST)
                        {
                        (*(nn)).setLabel(newCost,net.getNodeId(thisNodeIter));
                        NetArc *arcPnt = (NetArc *)(&(*arcIter));
                        dijkHeap.push_back(arcPnt->head());
                        std::push_heap(dijkHeap.begin(),dijkHeap.end(),heapComparitor);
                        //NodeCostComp < NodeIterator > heapComparitor;
                        //std::sort(dijkHeap.begin(),dijkHeap.end(),heapComparitor);
                        }
                    else
                        {
                        (*(nn)).setLabel(newCost,net.getNodeId(thisNodeIter));
                        //NodeCostComp < NodeIterator > heapComparitor;
                        std::make_heap(dijkHeap.begin(),dijkHeap.end(),heapComparitor);
                        }
                    }
                }
            }
        }
    return 0;
}

template < class NetType , class NodeIterator >
void DijkSolver< NetType, NodeIterator >::getPath(
                                                  const NetType &net,
                                                  const NodeIterator &sink,
                                                  Cgc::Path &pathToFill) const
{
    pathToFill.erase(pathToFill.begin(),pathToFill.end());
    NodeIterator thisNode = sink;
    NodeIterator prevNode = thisNode;
    Cgc::NodeId thisId = net.getNodeId(thisNode);
    pathToFill.setCost((*(*thisNode)).getLabel());
    do
        {
        thisNode = prevNode;
        pathToFill.push_back(thisId);
        Cgc::NodeId predId = (*(*thisNode)).getPred();
        NetType &nonConstNet = const_cast<NetType &>(net);
        prevNode = (NetIter ) nonConstNet.find(predId);
        thisId = predId;
        }
    while(prevNode != thisNode);
}
};
#endif
