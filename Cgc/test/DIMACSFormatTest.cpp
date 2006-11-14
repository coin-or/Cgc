/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#include <iostream>
#include <sstream>
#include <StaticNet.h>
#include <DynNet.h>
#include <StaticFBNet.h>
#include <DimacsNetFlowReader.h>
#include <SSPSolver.h>
#include "TestItem.h"
using namespace Cgc;

std::string buildInput()
{
    std::stringstream ss;
    ss<<"c"<<std::endl;
    ss<<"c        This is a simple example file to demonstrate the"<<std::endl;
    ss<<"c     DIMACS input file format for minimum-cost flow problems."<<std::endl;
    ss<<"c "<<std::endl;
    ss<<"c problem line :"<<std::endl;
    ss<<"p min 4 5"<<std::endl;
    ss<<"c"<<std::endl;
    ss<<"c node descriptor lines :"<<std::endl;
    ss<<"n 1 4"<<std::endl;
    ss<<"n 4 -4"<<std::endl;
    ss<<"c"<<std::endl;
    ss<<"c arc descriptor lines :"<<std::endl;
    ss<<"a 1 2 0 4 2"<<std::endl;
    ss<<"a 1 3 0 2 2"<<std::endl;
    ss<<"a 2 3 0 2 1"<<std::endl;
    ss<<"a 2 4 0 3 3"<<std::endl;
    ss<<"a 3 4 0 5 1"<<std::endl;
    return ss.str();
}

class NodeInfo : public MCFNodeData
{
public:
    NodeInfo()
    {}
    void setDemand(int demand)
    {
        setE(demand);
    }
};

class ArcInfo : public MCFArcData
{
public:
    ArcInfo()
        :minFlow(0)
    {}
    ArcInfo(int minF,
        int maxF,
        int fCost)
        :MCFArcData(fCost,maxF,true),
        minFlow(minF)
    {}
    int getMinFlow()const
    {
        return minFlow;
    }
    void setMinFlow( int newVal)
    {
        minFlow = newVal;
    }
private:
    int minFlow;
};

typedef StaticNet<NodeInfo,ArcInfo> MyNetType;
typedef DynNet<NodeInfo,ArcInfo> MyNetType2;
typedef StaticFBNet<NodeInfo,ArcInfo> MyNetType3;

template <class NetType>
void test1(const std::string &testName)
{
    TestItem *ti = new TestItem(testName.c_str());
    std::string inNet(buildInput());
    std::stringstream ss(inNet);

    DIMACSNetFlowReader<NetType> reader(ss);
    NetType *net = reader.constructGraph();
    if(net==NULL)
        ti->failItem(__SPOT__);
    if(net->arc_size()!=10)
        ti->failItem(__SPOT__);
    if(net->size()!=4)
        ti->failItem(__SPOT__);
    ti->passItem();
}
template <class NetType >
void test2(const std::string &testName)
{
    TestItem *ti = new TestItem(testName.c_str());
    std::string inNet(buildInput());
    std::stringstream ss(inNet);

    DIMACSNetFlowReader<NetType> reader(ss);
    NetType *net = reader.constructGraph();
    //std::cout<<*net<<std::endl;
    SSPSolver<NetType> sspSolver;
    sspSolver.solve(*net);
    typename SSPSolver<NetType>::Solution solution(*net);
    sspSolver.getSolution(*net,solution);
    typedef std::vector<typename SSPSolver<NetType>::Solution::FlowData> MyFlowDataVect;
    const MyFlowDataVect &flows = solution.getFlowData();
    if(flows[0].tail!=0)
        ti->failItem(__SPOT__);
    if(flows[0].head!=1)
        ti->failItem(__SPOT__);
    if(flows[0].flow!=3)
        ti->failItem(__SPOT__);
    if(flows[1].tail!=0)
        ti->failItem(__SPOT__);
    if(flows[1].head!=2)
        ti->failItem(__SPOT__);
    if(flows[1].flow!=1)
        ti->failItem(__SPOT__);

    if(flows[2].tail!=1)
        ti->failItem(__SPOT__);
    if(flows[2].head!=3)
        ti->failItem(__SPOT__);
    if(flows[2].flow!=3)
        ti->failItem(__SPOT__);

    if(flows[3].tail!=2)
        ti->failItem(__SPOT__);
    if(flows[3].head!=3)
        ti->failItem(__SPOT__);
    if(flows[3].flow!=1)
        ti->failItem(__SPOT__);
    /*
    for(MyFlowDataVect::const_iterator fci= flows.begin(); fci!=flows.end();fci++)
    {
        // back to original node id
        std::cout<<" flow "<< (*fci).tail+1<<" -> "<<(*fci).head+1<<" "<<(*fci).flow<<std::endl;
    }
    */
    ti->passItem();
}


void ReadDimacsTest(TestBed &myBed)
{
    TestItem::setBed(&myBed);
    test1<MyNetType>("DimacsFormat: staticnet");
    test1<MyNetType2>("DimacsFormat: dynnet");
    test1<MyNetType3>("DimacsFormat: staticfbnet");
    test2<MyNetType>("DimacsFormat: staticnet sspSolverCall");
    test2<MyNetType2>("DimacsFormat: dynnet sspSolverCall");
    test2<MyNetType3>("Dimacsformat: staticfbnet sspSolverCall");
}
