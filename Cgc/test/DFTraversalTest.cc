/*
Author: H. Philip Walton
Copyright (C) 2006 H. Philip Walton
All Rights Reserved.
*/
#include <iostream>
#include <sstream>
#include <StaticNet.h>
#include <StaticFBNet.h>
#include <DynNet.h>
#include <DFTraversal.h>
#include "TestBed.h"

typedef DynNet<int,int> MyNetType;
typedef StaticNet<int,int> MyNetType2;
typedef StaticFBNet<int,int> MyNetType3;

template < class MyNetType >
void buildNet(MyNetType &netToFill)
{
    typedef typename MyNetType::iterator NetIter;
    NetIter nit0=netToFill.insert(1); //nodeId0
    NetIter nit1=netToFill.insert(2); //nodeId1
    NetIter nit2=netToFill.insert(4); //nodeId2
    NetIter nit3=netToFill.insert(8); //nodeId3
    netToFill.arc_insert(nit0,
        16,
        nit1);
    netToFill.arc_insert(nit0,
        32,
        nit2);
    netToFill.arc_insert(nit0,
        64,
        nit3);
    netToFill.arc_insert(nit1,
        128,
        nit2);
    netToFill.arc_insert(nit1,
        256,
        nit3);
    netToFill.arc_insert(nit2,
        512,
        nit3);
}

template < class MyNetType >
bool checkResult(MyNetType &traversedNet)
{
    for(typename MyNetType::iterator nodeIt = traversedNet.begin();
        nodeIt!=traversedNet.end();nodeIt++)
    {
        switch(traversedNet.getNodeId(nodeIt).getNodeId())
        {
        case 0:
            if(*(*nodeIt)!=2)
                return false;
            break;
        case 1:
            if(*(*nodeIt)!=3)
                return false;
            break;
        case 2:
            if(*(*nodeIt)!=6)
                return false;
            break;
        case 3:
            if(*(*nodeIt)!=12)
                return false;
            break;
        default:
            return false;
        }
    }
    return true;
}

static int i;
template < class NetType >
void testDFT()
{
    std::stringstream testName;
    testName<<"DFT test:"<<i++;
    TestItem *ti = new TestItem(testName.str().c_str());
    NetType net(100,100);
    buildNet<NetType>(net);

    DFTraversal<NetType> traveler(net);

    for(traveler = net.begin();
        traveler != net.end(); traveler++)
    {
    typename NetType::iterator nodeIt = (*traveler);
    // @todo the previous const_cast shouldn't be necessary gcc3.x
    typename NetType::Node &node = const_cast<typename NetType::Node  &>(*nodeIt);
    int &i = *node;
    i++;
    }
    if(!checkResult(net))
    {
        ti->failItem(__SPOT__);
    }
    ti->passItem();
}	

int DFTraversalTest(TestBed &myBed)
{
    TestItem::setBed(&myBed);
    testDFT<MyNetType>();
    testDFT<MyNetType2>();
    testDFT<MyNetType3>();

    return 0;
}
