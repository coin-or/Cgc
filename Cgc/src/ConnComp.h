/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef CONNCOMP_H
#define CONNCOMP_H

#ifndef COMPONENT_H
#include <Component.h>
#endif

#include <set>
#include <vector>
#include <list>

namespace Cgc
{
    template< class Net >
    class NodeIterNodeIdLess: public std::binary_function<typename Net::iterator,typename Net::iterator,bool>
    { 
    public:
        NodeIterNodeIdLess(const Net &_net)
            :net(_net)
        {}
        bool operator()(const typename Net::iterator &it1, const typename Net::iterator &it2)const
        {
            return net.getNodeId(it1) < net.getNodeId(it2);
        }
    private:
        const Net &net;
    };

    /** @brief Solver which finds bi-connected components in a graph.
        @ingroup Algorithms
    */
template < class Net>
class ConnComp
{
private:
    typedef typename Net::Node NodeType;
    typedef typename NodeType::iterator ArcIter;
    typedef typename Net::iterator NetIterator;

public:
    /** @brief finds connected components in a graph.
        Use the default construtor to build one.  Then call the solve method.
        @parm first is the source node.  Will only look from first to last
        in the graph.    This allows for larger than-work-area graphs.
        @parm net the graph
        @parm containerToFill output variable containing the components.
        @note One must be careful to make the last node achievable and
        have it stop the search.
    */
    bool solve( Cgc::CompCollection &containerToFill, Net &net,
                const NetIterator &first, 
                const NetIterator &last)
    {
        bool foundAny=false;
        NetIterator iter = first;
        NetIterator netEnd = net.end();
        std::list<NetIterator> nodeStack;
        std::list<NetIterator> pathStack;
        NodeIterNodeIdLess<Net> compObj(net);
        std::set<NetIterator,NodeIterNodeIdLess<Net> > visitedSet(compObj);
        nodeStack.push_back(iter);
        while(nodeStack.size())
        {
            iter = *(nodeStack.rbegin());
            nodeStack.pop_back();
            if(iter==netEnd)
            {
                visitedSet.erase(*(pathStack.rbegin()));
                pathStack.pop_back();
                // remove last node?
                // back off stack one.
                // depth=depth-1
                continue;
            }
            if(visitedSet.find(iter)==visitedSet.end())
            {
                pathStack.push_back(iter); // diving, so add to pathStack
                visitedSet.insert(iter);
                nodeStack.push_back(netEnd);
                //std::cout<<"Visiting "<<net.getNodeId(iter)<<" of "<<nodeStack.size()<<std::endl;
                //std::cout<<"   node has "<<(*iter).size()<<" arcs."<<std::endl;
                for(typename Net::Node::iterator arcIt = (*iter).begin();
                    arcIt!=(*iter).end();arcIt++)
                {
                    //std::cout<<"   +adding "<<net.getNodeId((*arcIt).head())<<std::endl;
                    nodeStack.push_back((*arcIt).head());
                }
            }
            else
            {
                //std::cout<<"Already visited node"<<net.getNodeId(iter)<<std::endl;
                foundAny=true;
                Cgc::Component newComp;
                for(typename std::list<NetIterator>::iterator lnii = pathStack.begin();lnii!=pathStack.end();lnii++)
                {
                    newComp.insert(net.getNodeId((*lnii)));
                    //std::cout<<" pathStack has"<<net.getNodeId((*lnii))<<std::endl;
                }
                containerToFill.insert(newComp);
            }
        }
        return foundAny;
    }
    private:

    void print(Net &net)
    {
        std::cout<<"Node stack["<<myStack.size()<<"]"<<std::endl;
        for(unsigned cnt=0;cnt<myStack.size();cnt++)
            {
            if(myStack[cnt]==netEnd)
                std::cout<<" *";
            else
                std::cout<<" "<<net.getNodeId(myStack[cnt]);
            }
            std::cout<<std::endl;
            std::cout<<" Path stack["<<pathStack.size()<<"]"<<std::endl;
            for(unsigned cnt2=0;cnt2<pathStack.size();cnt2++)
            {
                std::cout<<" "<<pathStack[cnt2];
            }
            std::cout<<std::endl;
            std::cout<<"depth="<<depth<<std::endl;
            std::cout<<"---------------------------"<<std::endl;
            for(NetIterator nci = net.begin();nci!=net.end();
                nci++)
            {
                std::cout<<"Node "<<net.getNodeId(nci)<<" has "
                    <<(*nci).size()<<" arcs with data="<<(*(*nci))<<std::endl;
                for(ArcIter ai = (*nci).begin();ai!=(*nci).end();
                    ai++)
                {
                    std::cout<<" Arc to "<<net.getNodeId((*ai).head())
                        <<" with arc data="<<(*(*ai))<<std::endl;
                }
            }
        }

        std::vector<NetIterator> myStack;
        NetIterator netEnd;
        std::vector<Cgc::NodeId> pathStack;
        int depth;
};

};


#endif
