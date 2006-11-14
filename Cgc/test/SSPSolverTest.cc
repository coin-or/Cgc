/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <iostream>
#include <StaticNet.h>
#include <DynNet.h>
#include <StaticFBNet.h>
#include <SSPSolver.h>
#include "TestItem.h"
#include "TestBed.h"
#include <NodeId.h>
#include <set>
#include <sstream>
using namespace Cgc;

// Successive Shortest Path min cost flow -- Mangnanti Pg 321
// section 9.7

typedef StaticNet<MCFNodeData,MCFArcData>  MyNetType;
typedef DynNet<MCFNodeData,MCFArcData>     MyNetType2;
typedef StaticFBNet<MCFNodeData,MCFArcData> MyNetType3;
/*
template <class NetType>
std::ostream & operator<<(std::ostream &os, const NetType &net)
{
    os<<"SSPNet|"<<net.size()<<"|:"<<std::endl;
    for(typename NetType::const_iterator nodeIt = net.begin();
        nodeIt!= net.end();++nodeIt)
        {
        os<<"Node["<<net.getNodeId(nodeIt)<<"]"<<*(*nodeIt)<<std::endl;
        for(StaticNet<MCFNodeData,ArcData>::Node::const_iterator arcIt = (*nodeIt).begin();
            arcIt!=(*nodeIt).end();arcIt++)
            {
            os<<"  ArcTo["<<net.getNodeId((*arcIt).head())<<"]"<<
                *(*arcIt);
            if((*(*arcIt)).valid())
                os<<"*"<<std::endl;
            else
                os<<"XXX"<<std::endl;
            }
        }
    return os;
}
*/
static int instance=0;
template <class NetType>
NetType *buildNet(std::string &testname)
{
    TestItem *ti = new TestItem(testname.c_str());
    //
    // Create a network.
    // Add nodes with source having positive , zero
    // add arcs BOTH DIRECTIONS: fwd ==> positive cost, and capacity
    //                           bwd ==> neg cost, zero capacity.
    //
    NetType *n = new NetType(4,10);
    typename NetType::iterator it0=n->insert(MCFNodeData(4,0));
    typename NetType::iterator it1=n->insert(MCFNodeData(0,0));
    typename NetType::iterator it2=n->insert(MCFNodeData(0,0));
    typename NetType::iterator it3=n->insert(MCFNodeData(-4,0));

    typename NetType::iterator nodeZero=n->find(0);
    typename NetType::iterator nodeOne=n->find(1);
    typename NetType::iterator nodeTwo=n->find(2);
    typename NetType::iterator nodeThree=n->find(3);
    if(it0!=nodeZero)
        ti->failItem(__SPOT__);
    if(it1!=nodeOne)
        ti->failItem(__SPOT__);
    if(it2!=nodeTwo)
        ti->failItem(__SPOT__);
    if(it3!=nodeThree)
        ti->failItem(__SPOT__);
    // arc one forward
    n->arc_insert(nodeZero,MCFArcData(2,4,true),nodeOne);
    //arc2forward
    n->arc_insert(nodeZero,MCFArcData(2,2,true),nodeTwo);
    // arc one reverse
    n->arc_insert(nodeOne,MCFArcData(-2,0,false),nodeZero);
    //arc3forward
    n->arc_insert(nodeOne,MCFArcData(1,2,true),nodeTwo);
    //arc4forward
    n->arc_insert(nodeOne,MCFArcData(3,3,true),nodeThree);
    //arc2reverse
    n->arc_insert(nodeTwo,MCFArcData(-2,0,false),nodeZero);
    //arc3reverse
    n->arc_insert(nodeTwo,MCFArcData(-1,0,false),nodeOne);
    //arc5forward
    n->arc_insert(nodeTwo,MCFArcData(1,5,true),nodeThree);
    //arc4reverse
    n->arc_insert(nodeThree,MCFArcData(-3,0,false),nodeOne);
    //arc5reverse
    n->arc_insert(nodeThree,MCFArcData(-1,0,false),nodeTwo);
    ti->passItem();
    return n;
}

template < class NetType >
std::ostream &operator<<(std::ostream &os, const typename SSPSolver<NetType >::Solution &sol)
{
    for(typename std::vector<typename SolutionType<NetType>::FlowData>::const_iterator sci = sol.getFlowData().begin();
        sci!=sol.getFlowData().end();sci++)
        {
        const typename NetType::Arc &arc =*((*sci).arcIter);
        int flow=(*sci).flow;
        int origNode=(*sci).tail;
        int headNode=(*sci).head;
        os<<"Arc from "<<origNode<<":"<<*arc<<" to "<<headNode
          <<" has flow "<<flow<<std::endl;
        }
    return os;
}


template < class NetType >
void SSPSolverTest1(const std::string &testName)
{
    TestItem *ti=new TestItem(testName.c_str());
    std::string buildTestName=testName+"build";
    NetType *n = buildNet<NetType>(buildTestName);
    SSPSolver<NetType > solver;
    typename SSPSolver<NetType >::Solution sol(*n);
    //cout<<*n<<std::endl;    
    solver.solve(*n);
    int x=solver.getSolution(*n,sol);
    if(x!=14)
        {
        ti->failItem(__SPOT__);
        }
    typedef std::vector< typename SSPSolver<NetType>::Solution::FlowData > FlowVect;
    const FlowVect &flowVect= sol.getFlowData();
    int flowId=0;
    for(typename FlowVect::const_iterator fi = flowVect.begin();fi!=flowVect.end();fi++)
    {
        switch(flowId)
        {
        case 0:
            if((*fi).tail!=0 || (*fi).head!=1||(*fi).flow!=2)
            {
                ti->failItem(__SPOT__);
            }
            break;
        case 1:
            if((*fi).tail!=0 || (*fi).head!=2||(*fi).flow!=2)
            {
                ti->failItem(__SPOT__);
            }
            break;
        case 2:
            if((*fi).tail!=1 || (*fi).head!=2||(*fi).flow!=2)
            {
                ti->failItem(__SPOT__);
            }
            break;
        case 3:
            if((*fi).tail!=2 || (*fi).head!=3||(*fi).flow!=4)
            {
                ti->failItem(__SPOT__);
            }
            break;
        default:
            ti->failItem(__SPOT__);
            break;
        };
        flowId++;
    }
    ti->passItem();
}


int SSPSolverTest(TestBed &myBed)
{
    TestItem::setBed(&myBed);
    SSPSolverTest1<MyNetType>("SSPSolverTest:01:build and solve one MCF problem:StaticNet");
    SSPSolverTest1<MyNetType2>("SSPSolverTest:01:build and solve one MCF problem:DynNet");
    SSPSolverTest1<MyNetType3>("SSPSolverTest:01:build and solve one MCF problem:StaticFBNet");
    return 0;
}


