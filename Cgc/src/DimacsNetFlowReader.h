/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#ifndef DIMACSNETFLOWREADER_H
#define DIMACSNETFLOWREADER_H
#include <stdio.h>
#include <list>
namespace Cgc
{
    /** @brief internal utility class used for intermediate storage.  
    @ingroup InternalUse
    CGC expects ordered arcs, SSPSolver expects arcs 
        in both directions, DIMACS gives only one direction.  Easiest solution is to construct a set of correctly 
        ordered direction arcs, then add them in a 2nd phase.
        */
        typedef struct {
                int first;
                int second;
                int third;
                int forth;
        } Quadruplet;
        /** internal ordering object 
        @ingroup InternalUse
        @see Quadruplet.
        */
        class QuadOrderOneTwo : public std::binary_function<const Quadruplet &, const Quadruplet &,bool>
        {
        public:
            bool operator()(const Quadruplet &q1, const Quadruplet &q2)const
            {
                if(q1.first!=q2.first)
                {
                    return q1.first < q2.first;
                }
                return q1.second < q2.second;
            }
        };

        /** @brief Reader for DIMACS Min Cost Flow Network format.  
        @ingroup PublicInterface
        The good news is that the format can be read.  
        <p> Protocol is to construct a reader handing it the istream, then call "constructGraph()" and the graph will be
        returned. pre-constructed from the DIMACS format data.
        */
    template <class NetType >
    class DIMACSNetFlowReader
    {
    public:
        DIMACSNetFlowReader(std::istream &inStream)
            :mInStream(inStream)
        {
        }
        NetType *constructGraph()
        {
            char buffer[BUFFERSZ];
            bool bFoundProblem=false;
            bool bFoundArcs=false;
            bool bFoundNodes=false;
            int numArcs=0;
            NetType *pNet=NULL;
            while(mInStream.getline(buffer,BUFFERSZ))
            {
                if(isCommentLine(buffer))
                    continue;
                if(!bFoundProblem)
                {
                    assert(!bFoundArcs);
                    assert(!bFoundNodes);
                    if(isProblemLine(buffer))
                    {
                        bFoundProblem=true;
                        int numNodes=0;
                        char nameBuff[BUFFERSZ];
                        char p;
                        int scanResult = sscanf(buffer,"%c %s %d %d",&p,nameBuff,&numNodes,&numArcs);
                        assert(scanResult==4);
                        assert(p=='p');
                        assert(numNodes > 0);
                        assert(numArcs > 0);
                        pNet = new NetType(numNodes,2*numArcs);
                        assert(pNet);
                        for(int cnt=0;cnt<numNodes;cnt++)
                        {
                            typename NetType::node_data nodeInfo;
                            pNet->insert(nodeInfo);
                        }
                    }
                }
                if(isNodeLine(buffer))
                {
                    assert(bFoundProblem);
                    assert(!bFoundArcs);
                    bFoundNodes=true;
                    int demand=0;
                    char n;
                    int nodeId;
                    int scanResult = sscanf(buffer,"%c %d %d",&n,&nodeId,&demand);
                    assert(scanResult==3);
                    assert(n=='n');
                    assert(demand!=0);
                    assert(nodeId>0);
                    assert((unsigned)nodeId<=pNet->size());
                    typename NetType::iterator netIt = pNet->find(NodeId(nodeId-1));
                    /// @TODO explore why the cost_cast is needed here. gcc3.x
                    typename NetType::Node &node = const_cast<typename NetType::Node &>(*netIt);
                    (*node).setDemand(demand);
                }
                if(isArcLine(buffer))
                {
                    assert(bFoundProblem);
                    assert(bFoundNodes);
                    bFoundArcs=true;
                    char a;
                    int tailNodeId;
                    int headNodeId;
                    int minFlow;
                    int maxFlow;
                    int flowCost;
                    int scanResult = sscanf(buffer,"%c %d %d %d %d %d",&a,&tailNodeId, &headNodeId, &minFlow, &maxFlow, &flowCost);
                    assert(scanResult==6);
                    assert(a=='a');
                    assert(tailNodeId>0&&tailNodeId<=pNet->size());
                    assert(headNodeId>0&&headNodeId<=pNet->size());
                    // add the right directioned arc first, then the back directioned arc.
                    // The second arc would be out of order, which is why the intermediate storage is used.
                    /** @todo write an alg which would take the min flow.  Or...alternatively, figure out if there
                        is a way to preprocess them away.  I know not which. 
                     */
                    Quadruplet q;
                    q.first=tailNodeId-1;
                    q.second=headNodeId-1;
                    q.third=maxFlow;
                    q.forth=flowCost;
                    mArcList.insert(q);
                    q.first=headNodeId-1;
                    q.second=tailNodeId-1;
                    q.third=0;
                    q.forth=-flowCost;
                    mArcList.insert(q);
                }
            }
            assert(bFoundArcs);
            // populate the graph from the intermediate storage for the arcs.
            for(ArcList::iterator ali = mArcList.begin();ali!=mArcList.end();ali++)
            {
                NodeId tailNode = (*ali).first;
                NodeId headNode = (*ali).second;
                typename NetType::iterator headIt = pNet->find(headNode);
                typename NetType::iterator tailIt = pNet->find(tailNode);
                typename NetType::arc_data arcData;
                arcData.setR((*ali).third);
                arcData.setC((*ali).forth);
                pNet->arc_insert(tailIt,arcData,headIt);
            }
            return pNet;
        }

    private:
        bool isCommentLine(const char *const line)const
        {
            return (line[0]=='c');
        }
        bool isProblemLine(const char *const line)const
        {
            return (line[0]=='p');
        }
        bool isNodeLine(const char *const line)const
        {
            return (line[0]=='n');
        }
        bool isArcLine(const char *const line)const
        {
            return (line[0]=='a');
        }
        typedef std::set<Quadruplet,QuadOrderOneTwo> ArcList;
        ArcList mArcList;
        std::istream &mInStream;
        enum { BUFFERSZ=1024 };
    };
}
#endif

