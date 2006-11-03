/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <DynNet.h>
#include <DijkSolver.h>
#include "SPTestData.h"

#include "TestItem.h"
#include "TestBed.h"

typedef DynNet< SPNodeLabel, SPArcCost > DynDijkSPNet;

void constructSimpleNet(DynDijkSPNet &net)
{
  static TestItem *ti = new TestItem("SPDynTest:aux:Fill Simple Net");
  SPNodeLabel tmp;
  DynDijkSPNet::iterator node0 = net.insert( tmp ); // NodeId0
  if(*net.find(NodeId(0))!= *node0)
    {
      std::cout<<"ERROR: constructingSimpleNet, nodeid should be zero is:";
      std::cout<<net.getNodeId(node0)<<std::endl;
      std::cout<<"....find returned:"<<*net.find(NodeId(0))<<std::endl;
      ti->failItem(__SPOT__);
      return;
    }
  
  DynDijkSPNet::iterator node1 = net.insert(tmp);
  DynDijkSPNet::iterator node2 = net.insert(tmp);
  DynDijkSPNet::iterator node3 = net.insert(tmp);
  DynDijkSPNet::iterator node4 = net.insert(tmp);
  std::cout<<"tmp = "<<tmp<<std::endl;
  std::cout<<"inserting nodeId = "<<net.getNodeId(node1)<<std::endl; // Id1
  std::cout<<"inserting nodeId = "<<net.getNodeId(node2)<<std::endl; // Id2
  std::cout<<"inserting nodeId = "<<net.getNodeId(node3)<<std::endl; // Id3
  std::cout<<"inserting nodeId = "<<net.getNodeId(node4)<<std::endl; // Id4
  int count=0;
  for( DynDijkSPNet::iterator nodeIt1 = net.begin();
       nodeIt1 != net.end(); nodeIt1++)
    {
      count++;
      if(net.getNodeId(nodeIt1)!= NodeId(count))
	{
	  std::cout<<"WARN:Node id does not match node count, id=";
	  std::cout<<net.getNodeId(nodeIt1)<<" , count="<<NodeId(count)<<std::endl;
	  //ti->failItem(__SPOT__);
	  //return;
	}
    }
  ti->passItem();
  DynDijkSPNet::iterator nodeIt2 = net.find(NodeId(0));
  if(nodeIt2 == net.end())
    {
      ti->failItem(__SPOT__);
      return;
    }
  if(net.getNodeId(nodeIt2)!= NodeId(0))
    {
      std::cout<<"nodeId zero was "<<net.getNodeId(net.find(NodeId(0)));
      ti->failItem(__SPOT__);
      return ;
    }
  if(net.getNodeId(net.find(NodeId(1)))!= NodeId(1))
    {
      std::cout<<"nodeId one was "<<net.getNodeId(net.find(NodeId(1)));
      ti->failItem(__SPOT__);
      return ;
    }
  
  
  DynDijkSPNet::iterator first = net.find(NodeId(0));
  DynDijkSPNet::iterator second = net.find(NodeId(1));
  net.arc_insert(first,
		 SPArcCost(1),
		 second);
  first = net.find(NodeId(1));
  second = net.find(NodeId(2));
  net.arc_insert(first,
		 SPArcCost(2),
		 second);
  first = net.find(NodeId(1));
  second = net.find(NodeId(3));
  net.arc_insert(first,
		 SPArcCost(1),
		 second);
  first = net.find(NodeId(1));
  second = net.find(NodeId(4));
  net.arc_insert(first,
		 SPArcCost(6),
		 second);
  first = net.find(NodeId(2));
  second = net.find(NodeId(3));
  net.arc_insert(first,
		 SPArcCost(2),
		 second);
  first = net.find(NodeId(3));
  second = net.find(NodeId(4));
  net.arc_insert(first,
		 SPArcCost(3),
		 second);
  
}

static void test1()
{
  TestItem *ti = new TestItem("SPDynTest:1:Construct and solve");
  DynDijkSPNet myDynSPNet(20,20);
  
  constructSimpleNet(myDynSPNet);
  
  DijkSolver<DynDijkSPNet, DynDijkSPNet::iterator > myShortSolver;
  Path pathToFill;
  std::cout<<"SOLVING WITH SOURCE ="<<*myDynSPNet.find(NodeId(0))<<std::endl;
  
  DynDijkSPNet::iterator sourceIt=myDynSPNet.find(NodeId(0));
  myShortSolver.solve(myDynSPNet, sourceIt);
  std::cout<<"Solve done"<<std::endl;
  sourceIt=myDynSPNet.find(NodeId(2));
  myShortSolver.getPath(myDynSPNet, sourceIt,pathToFill);
  std::cout<<"Path gotten"<<std::endl;
  std::cout<<pathToFill<<std::endl;
  std::cout<<"Path printed"<<std::endl;
  if(pathToFill.size()!=3)
    {
      std::cout<<"Path size = "<<pathToFill.size()<<std::endl;
      ti->failItem(__SPOT__);
    }
  ti->passItem();
}

static void test2()
{
  TestItem *ti = new TestItem("SPDynTest:2:getAllPaths");
  DynDijkSPNet myDynSPNet(0,0);
  
  constructSimpleNet(myDynSPNet);
  
  DijkSolver<DynDijkSPNet, DynDijkSPNet::iterator > myShortSolver;
  Path pathToFill;
  DynDijkSPNet::iterator sourceIt = myDynSPNet.find(NodeId(0));
  myShortSolver.solve(myDynSPNet, sourceIt);
  NodeId tmpId;
  for( DynDijkSPNet::iterator nodeIt = myDynSPNet.begin();
       nodeIt !=myDynSPNet.end();nodeIt++)
    {
      pathToFill.erase(pathToFill.begin(),pathToFill.end());
      tmpId = myDynSPNet.getNodeId(nodeIt);
      myShortSolver.getPath(myDynSPNet,myDynSPNet.find(tmpId),pathToFill);
      std::cout<<"Path for nodeId "<<tmpId<<" = "<<pathToFill<<std::endl;
    }
  ti->passItem();
}


int SPDynTest(TestBed &myBed)
{
  TestItem::setBed(&myBed);
  test1();
  test2();
  return 0;
}
