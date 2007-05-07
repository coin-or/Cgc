/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef SPLABELCORRECTINGSOLVER_H
#define SPLABELCORRECTINGSOLVER_H
#include <iosfwd>
#include <set>
#include <NodeId.h>
namespace Cgc
{
    /** @brief implements the modified label-correcting shortest path algorithm
    @ingroup PublicInterface
        See "Network Flows"; Ahuja, Magnanti, and Orlin, Section 5.3, pgs 141
        LIST isn't implemented exactly as specified in the text.  The inner loop is unrolled and put into the outer loop.
        State of the loop counters are maintained in the solver and updated in the findArcToSet() method.
        This particular implementation doesn't do much fancy.  It doesn't detect negative cycles.  In this case it could be 
        deteremined based on how many updates are actually done.  If it gets past a certain number (m*n) then I believe a
        negative cycle is indicated.

        @note a deque implementation is relatively straightforward, just replace the findArcToSet() and associated data structures and a boolean to the node structure.

    */
    template < class NetType >
    class LabelCorrectingSolver
    {
    public:
        LabelCorrectingSolver(NetType &net)
            :mNet(net)
        {
        }
        void solve(Cgc::NodeId source)
        {
            typename NetType::iterator sourceIt = mNet.find(source);
            (*(*sourceIt)).setLabel(0);
            (*(*sourceIt)).setPred(Cgc::NodeId(source));
            LIST.clear();
            ARCLIST.clear();
            LIST.insert(source);

            NetType::arc_iterator arcToSet = findArcToSet();
            while(arcToSet != mNet.arc_end())
            {
                NetType::iterator headNodeIt = (*arcToSet).head();
                NetType::iterator tailNodeIt = (*arcToSet).tail();
                if((*(*headNodeIt)).getLabel() > (*(*tailNodeIt)).getLabel()+(*(*arcToSet)).getCost())
                {
                    //std::cout<<"Incrementing with arc from "<<mNet.getNodeId(tailNodeIt)<<" to "<<mNet.getNodeId(headNodeIt)<<std::endl;
                    //std::cout<<"    change node"<<mNet.getNodeId(headNodeIt)<<" cost from "<<(*(*headNodeIt)).getLabel()<<" to:"<<(*(*tailNodeIt)).getLabel()+(*(*arcToSet)).getCost()<<std::endl;
                    (*(*headNodeIt)).setLabel((*(*tailNodeIt)).getLabel()+(*(*arcToSet)).getCost());
                    (*(*headNodeIt)).setPred(mNet.getNodeId(tailNodeIt));
                }
                arcToSet = findArcToSet();
            }
        }
    protected:
        virtual typename NetType::arc_iterator findArcToSet()
        {
            // if we still have some arcs for the "current node" to process, do the next one.
            if(ARCLIST.size())
            {
                typename NetType::arc_iterator retVal = *(ARCLIST.rbegin());
                ARCLIST.pop_back();
                return retVal;
            }
            else
            {
                // nothing in arc list, see if there is still something in node list
                while(LIST.size())
                {
                    // while there is a node which has any arcs, 
                    // pick off the next one and add the arcs for it into the ARCLIST.
                    Cgc::NodeId newNode = *(LIST.begin());
                    typename NetType::iterator currentNode = mNet.find(newNode);
                    LIST.erase(newNode);
                    //
                    for(typename NetType::Node::iterator ntai = (*currentNode).begin();
                        ntai != (*currentNode).end();ntai++)
                    {
                        typename NetType::arc_iterator arcIter(mNet,currentNode,ntai);
                        ARCLIST.push_back(typename NetType::arc_iterator(arcIter));
                        LIST.insert(mNet.getNodeId((*ntai).head()));
                    }
                    if(ARCLIST.size())
                    {
                        // there was at least one arc on that node, pick it off and return.
                        typename NetType::arc_iterator retVal = *(ARCLIST.rbegin());
                        ARCLIST.pop_back();
                        return retVal;
                    }
                    // There weren't any arcs, so go to the next node and see what is found.
                }
            }
            return mNet.arc_end();
        }
    private:
        NetType &mNet;
        std::set<Cgc::NodeId> LIST;
        std::vector<typename NetType::arc_iterator> ARCLIST;
    };
}
#endif
