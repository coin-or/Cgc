/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#include <SPLabelCorrectingSolver.h>
#include <StaticFBNet.h>
#include <DynNet.h>
#include <limits.h>
#include <iostream>
#include "TestBed.h"
#include "TestItem.h"

/// arbitrary meaning of infinity for our convenience here.
int NODE_INFINITY = 10000000; 

namespace Cbc_LabelCorr
{
    class NodeLabel
    {
    public:
        NodeLabel()
            :val(NODE_INFINITY),
            pred(Cgc::NodeId(0))
        {}
        NodeLabel(int value)
        {
            val=value;
        }
        void setLabel(int value)
        {
            val=value;
        }
        void setPred(Cgc::NodeId newPred)
        {
            pred = newPred;
        }
        int getLabel()
        {
            return val;
        }
    private:
        int val;
        Cgc::NodeId pred;
        friend std::ostream &operator<<(std::ostream &os, const NodeLabel &ac);
    };
    std::ostream &operator<<(std::ostream &os, const NodeLabel &ac)
    {
        return os<<"nodeLabel:"<<ac.val<<" pred="<<ac.pred;
    }
    class ArcCost
    {
    protected:
        static const ArcCost MAXCOST;
    public:
        ArcCost()
            :cost(ArcCost::MAXCOST.cost)
        {
        }
        ArcCost(int initCost)
            :cost(initCost)
        {
        }
        int getCost()
        {
            return cost;
        }
        void setCost(int newCost)
        {
            cost=newCost;
        }
    private: 
        int cost;
        friend std::ostream &operator<<(std::ostream &os, const ArcCost &ac);
    };
    std::ostream &operator<<(std::ostream &os, const ArcCost &ac)
    {
        return os<<"arccost:"<<ac.cost;
    }

    const ArcCost ArcCost::MAXCOST(INT_MAX);


};
//#define DYNTEST
#define STATICFBTEST
typedef Cgc::StaticFBNet< Cbc_LabelCorr::NodeLabel, Cbc_LabelCorr::ArcCost > StaticSPFBNet;
typedef Cgc::DynNet< Cbc_LabelCorr::NodeLabel, Cbc_LabelCorr::ArcCost > DynSPFBNet;

using namespace Cgc;
using namespace Cbc_LabelCorr;
template<class MyNet>
void constructGraph(MyNet &net)
{
    int NUMNODES=5;
    for(int cnt=0;cnt<NUMNODES;cnt++)
    {
        net.insert(NodeLabel(NODE_INFINITY));
    }
    net.arc_insert(net.find(NodeId(0)),Cbc_LabelCorr::ArcCost(2), net.find(NodeId(1)));
    net.arc_insert(net.find(NodeId(0)),Cbc_LabelCorr::ArcCost(2), net.find(NodeId(2)));
    net.arc_insert(net.find(NodeId(1)),Cbc_LabelCorr::ArcCost(4), net.find(NodeId(3)));
    net.arc_insert(net.find(NodeId(1)),Cbc_LabelCorr::ArcCost(3), net.find(NodeId(4)));
    net.arc_insert(net.find(NodeId(2)),Cbc_LabelCorr::ArcCost(1), net.find(NodeId(4)));
    net.arc_insert(net.find(NodeId(3)),Cbc_LabelCorr::ArcCost(-3),net.find(NodeId(4)));
    std::cout<<"Net="<<net<<std::endl;
}
template<class MyNet>
void solve(MyNet &net)
{
    LabelCorrectingSolver<MyNet> solver(net);
    solver.solve(Cgc::NodeId(0));
    
}
template<class MyNet>
bool checkSolution(MyNet &net)
{
    if((*(*net.find(Cgc::NodeId(0)))).getLabel()!=0)
        return false;
    if((*(*net.find(Cgc::NodeId(1)))).getLabel()!=2)
        return false;
    if((*(*net.find(Cgc::NodeId(2)))).getLabel()!=2)
        return false;
    if((*(*net.find(Cgc::NodeId(3)))).getLabel()!=6)
        return false;
    if((*(*net.find(Cgc::NodeId(4)))).getLabel()!=3)
        return false;
    return true;
}
void SPLabelCorrTest(TestBed &bed)
{
#ifdef DYNTEST
    TestItem *ti1= new TestItem("DynNet Label Correcting SP test");
    DynSPFBNet net(5,6);
    constructGraph<DynSPFBNet>(net);
    solve<DynSPFBNet>(net);
    if(checkSolution<DynSPFBNet>(net))
    {
        ti1->passItem();
    }
    else
    {
        ti1->failItem(__SPOT__);
    }
#endif
#ifdef STATICFBTEST
    StaticSPFBNet net2(5,6);
    TestItem *ti2 = new TestItem("StaticFBNet Label Correcting SP test");
    constructGraph<StaticSPFBNet>(net2);
    solve<StaticSPFBNet>(net2);
    if(checkSolution(net2))
    {
        ti2->passItem();
    }
    else
    {
        ti2->failItem(__SPOT__);
    }
#endif

}
 
