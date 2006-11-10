/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <iostream>
#include <StaticNet.h>
#include <StaticFBNet.h>
#include <DynNet.h>

#include <BFTraversal.h>
#include "TestBed.h"
#include <sstream>

typedef StaticNet<int,int> MyNetType;
typedef StaticFBNet<int,int> MyNetType2;
typedef DynNet<int,int> MyNetType3;

template < class MyNetType >
void buildNet(MyNetType &netToFill)
{
  typedef typename MyNetType::iterator NetIt;
  NetIt node0= netToFill.insert(1); //nodeId0
  NetIt node1= netToFill.insert(2); //nodeId1
  NetIt node2= netToFill.insert(4); //nodeId2
  NetIt node3= netToFill.insert(8); //nodeId3
  netToFill.arc_insert(node0,
		       16,
		       node1);
  netToFill.arc_insert(node0,
		       32,
		       node2);
  netToFill.arc_insert(node0,
		       64,
		       node3);
  netToFill.arc_insert(node1,
		       128,
		       node2);
  netToFill.arc_insert(node1,
		       256,
		       node3);
  netToFill.arc_insert(node2,
		       512,
		       node3);
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
void testBFT()
{
    std::stringstream testName;
    testName<<"BFT test:"<<i++;
    TestItem *ti = new TestItem(testName.str().c_str());
    NetType net(100,100);
    buildNet<NetType>(net);

    BFTraversal<NetType> traveler(net);

    for(traveler = net.begin();
        traveler != net.end(); traveler++)
    {
    typename NetType::iterator nodeIt = (*traveler);
    // @TODO this const_cast should not be necessary gcc3.x
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

int BFTraversalTest(TestBed &bed)
{
    TestItem::setBed(&bed);
    testBFT<MyNetType>();
    testBFT<MyNetType2>();
    testBFT<MyNetType3>();

  return 0;
}
