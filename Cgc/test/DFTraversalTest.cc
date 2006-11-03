/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <iostream>

#include <StaticNet.h>
#include <DynNet.h>
#include <DFTraversal.h>
#include "TestBed.h"

typedef DynNet<int,int> MyNetType;
typedef StaticNet<int,int> MyNetType3;

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


int DFTraversalTest(TestBed &myBed)
{
  std::cout<<"Testing DynNet"<<std::endl;
  MyNetType net(100,100);
  buildNet<MyNetType>(net);

  DFTraversal<MyNetType> traveler(net);

  for(traveler = net.begin();
      traveler != net.end(); traveler++)
    std::cout<<" at node "<<net.getNodeId(*traveler)<<std::endl;
	
/*
  std::cout<<"Testing StaticNet"<<std::endl;
  MyNetType2 net2(100,100);
  buildNet<MyNetType2>(net2);
  DFTraversal<MyNetType2> traveler2(net2);

  for(traveler2 = net2.begin();
      traveler2 != net2.end(); traveler2++)
    std::cout<<" at node "<<net2.getNodeId(*traveler2)<<std::endl;

*/

  std::cout<<"Testing StaticFBNet"<<std::endl;
  MyNetType3 net3(100,100);
  buildNet<MyNetType3>(net3);
  DFTraversal<MyNetType3> traveler3(net3);

  for(traveler3 = net3.begin();
      traveler3 != net3.end(); traveler3++)
    std::cout<<" at node "<<net3.getNodeId(*traveler3)<<std::endl;

  std::cout<<"Not a real test, but suffices"<<std::endl;
  return 0;
}
