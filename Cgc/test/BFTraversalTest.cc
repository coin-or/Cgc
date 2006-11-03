/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <iostream>
#include <StaticNet.h>
#include <DynNet.h>

#include <BFTraversal.h>
#include "TestBed.h"

typedef StaticNet<int,int> MyNetType;
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


int BFTraversalTest(TestBed &bed)
{
  std::cout<<"Testing StaticNet"<<std::endl;
  MyNetType net(100,100);
  buildNet<MyNetType>(net);
  BFTraversal<MyNetType> traveler(net);

  for(traveler = net.begin();
       traveler != net.end(); traveler++)
      std::cout<<" at node "<<net.getNodeId(*traveler)<<std::endl;
	
/*
  cout<<"Testing StaticFBNet"<<endl;
  MyNetType2 net2(100,100);
  buildNet<MyNetType2>(net2);
  BFTraversal<MyNetType2> traveler2(net2);

  for(traveler2 = net2.begin();
       traveler2 != net2.end(); traveler2++)
      cout<<" at node "<<net2.getNodeId(*traveler2)<<endl;

*/
  std::cout<<"Testing DynNet"<<std::endl;
  MyNetType3 net3(100,100);
  buildNet<MyNetType3>(net3);
  BFTraversal<MyNetType3> traveler3(net3);

  for(traveler3 = net3.begin();
       traveler3 != net3.end(); traveler3++)
      std::cout<<" at node "<<net3.getNodeId(*traveler3)<<std::endl;

	std::cout<<"not a real test...but suffices"<<std::endl;
  return 0;
}
