/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <StaticNet.h>
#include <SSPSolver.h>
#include "TestItem.h"
#include "TestBed.h"

#include <iostream>
#include <set>

using namespace Cgc;

// Successive Shortest Path min cost flow -- Mangnanti Pg 321
// section 9.7

typedef StaticNet<MCFNodeData,ArcData> NetType;
typedef StaticNet<MCFNodeData,ArcData>::Node NodeType;
typedef StaticNet<MCFNodeData,ArcData>::Arc ArcType;
typedef SSPSolver<NetType > SolverType;

std::ostream & operator<<(std::ostream &os, const NetType &net)
{
    os<<"SSPNet|"<<net.size()<<"|:"<<std::endl;
    for(NetType::const_iterator nodeIt = net.begin();
        nodeIt!= net.end();++nodeIt)
        {
        os<<"Node["<<net.getNodeId(nodeIt)<<"]"<<*(*nodeIt)<<std::endl;
        for(NodeType::const_iterator arcIt = (*nodeIt).begin();
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

NetType *buildNet()
{

    //
    // Create a network.
    // Add nodes with source having positive , zero
    // add arcs BOTH DIRECTIONS: fwd ==> positive cost, and capacity
    //                           bwd ==> neg cost, zero capacity.
    //
    NetType *n = new NetType(4,4);
    n->insert(MCFNodeData(4,0));
    n->insert(MCFNodeData(0,0));
    n->insert(MCFNodeData(0,0));
    n->insert(MCFNodeData(-4,0));
    //NetType::arc_iterator ni =
    NetType::iterator nodeZero=n->find(0);
    NetType::iterator nodeOne=n->find(1);
    NetType::iterator nodeTwo=n->find(2);
    NetType::iterator nodeThree=n->find(3);
    // arc one forward
    n->arc_insert(nodeZero,ArcData(2,4,true),nodeOne);
    //arc2forward
    n->arc_insert(nodeZero,ArcData(2,2,true),nodeTwo);
    // arc one reverse
    n->arc_insert(nodeOne,ArcData(-2,0,false),nodeZero);
    //arc3forward
    n->arc_insert(nodeOne,ArcData(1,2,true),nodeTwo);
    //arc4forward
    n->arc_insert(nodeOne,ArcData(3,3,true),nodeThree);
    //arc2reverse
    n->arc_insert(nodeTwo,ArcData(-2,0,false),nodeZero);
    //arc3reverse
    n->arc_insert(nodeTwo,ArcData(-1,0,false),nodeOne);
    //arc5forward
    n->arc_insert(nodeTwo,ArcData(1,5,true),nodeThree);
    //arc4reverse
    n->arc_insert(nodeThree,ArcData(-3,0,false),nodeOne);
    //arc5reverse
    n->arc_insert(nodeThree,ArcData(-1,0,false),nodeTwo);
    return n;
}

typedef SolverType::Solution SolType;

std::ostream &operator<<(std::ostream &os, const SolType &sol)
{
    for(std::vector<SolType::FlowData>::const_iterator sci = sol.getFlowData().begin();
        sci!=sol.getFlowData().end();sci++)
        {
        const NetType::Arc &arc =*((*sci).arcIter);
        int flow=(*sci).flow;
        int origNode=(*sci).tail;
        int headNode=(*sci).head;
        os<<"Arc from "<<origNode<<":"<<*arc<<" to "<<headNode
          <<" has flow "<<flow<<std::endl;
        }
    return os;
}

static void test1()
{
    TestItem *ti=new TestItem("SSPSolverTest:01:build and solve one MCF problem");
    NetType *n = buildNet();
    SolverType solver;
    SolverType::Solution sol(*n);
    //cout<<*n<<std::endl;    
    std::cout<<"Starting solver"<<std::endl;
    solver.solve(*n);
    std::cout<<"Getting solution..."<<std::endl;
    int x=solver.getSolution(*n,sol);
    std::cout<<"Solver got min cost of "<<x<<std::endl;
    if(x!=14)
        {
        ti->failItem(__SPOT__);
        }
    ti->passItem();
}


int SSPSolverTest(TestBed &myBed)
{
    TestItem::setBed(&myBed);
    test1();
    return 0;
}


