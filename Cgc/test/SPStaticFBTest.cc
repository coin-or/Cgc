/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <StaticFBNet.h>
#include <DynNet.h>
#include <DijkSolver.h>
#include "SPTestData.h"
#include "TestItem.h"
#include "TestBed.h"

typedef StaticFBNet< SPNodeLabel, SPArcCost > StaticDijkSPNet;
typedef DynNet< SPNodeLabel, SPArcCost > DynDijkSPNet;

void constructSimpleNet(StaticDijkSPNet &net)
{
  static TestItem *ti = new TestItem("aux:Fill Simple Net");
  SPNodeLabel tmp;
  StaticDijkSPNet::iterator nodeIt = net.insert( tmp ); // NodeId0
  if(net.find(NodeId(0))!= nodeIt)
    ti->failItem(__SPOT__);
  ti->passItem();
  
  net.insert( tmp ); // NodeId1
  net.insert( tmp ); // NodeId2
  net.insert( tmp ); // NodeId3
  net.insert( tmp ); // NodeId4
  
  net.arc_insert(net.find(NodeId(0)),
		 SPArcCost(1),
		 net.find(NodeId(1)));
  net.arc_insert(net.find(NodeId(1)),
		 SPArcCost(2),
		 net.find(NodeId(2)));
  net.arc_insert(net.find(NodeId(1)),
		 SPArcCost(1),
		 net.find(NodeId(3)));
  net.arc_insert(net.find(NodeId(1)),
		 SPArcCost(6),
		 net.find(NodeId(4)));
  net.arc_insert(net.find(NodeId(2)),
		 SPArcCost(2),
		 net.find(NodeId(3)));
  net.arc_insert(net.find(NodeId(3)),
		 SPArcCost(3),
		 net.find(NodeId(4)));
  
}

static void test1()
{
  TestItem *ti = new TestItem("1:Construct and solve");
  StaticDijkSPNet myStaticSPNet(100,100);
  
  constructSimpleNet(myStaticSPNet);
  
  DijkSolver<StaticDijkSPNet, StaticDijkSPNet::iterator > myShortSolver;
  Path pathToFill;
  StaticDijkSPNet::iterator start=myStaticSPNet.begin();
  myShortSolver.solve(myStaticSPNet, start );
  //std::cout<<"Solve done"<<std::endl;
  StaticDijkSPNet::iterator start2 = myStaticSPNet.find(NodeId(2));
  myShortSolver.getPath(myStaticSPNet, start2,pathToFill);
  //std::cout<<"Path gotten"<<std::endl;
  //std::cout<<pathToFill<<std::endl;
  //std::cout<<"Path printed"<<std::endl;
  if(pathToFill.size()!=3)
    ti->failItem(__SPOT__);
  ti->passItem();
}

static void test2()
{
  TestItem *ti = new TestItem("2:getAllPaths");
  StaticDijkSPNet myStaticSPNet(100,100);
  
  constructSimpleNet(myStaticSPNet);
  
  DijkSolver<StaticDijkSPNet, StaticDijkSPNet::iterator > myShortSolver;
  Path pathToFill;
  StaticDijkSPNet::iterator start = myStaticSPNet.begin();
  myShortSolver.solve(myStaticSPNet, start );
  NodeId tmpId;
  for( StaticDijkSPNet::iterator nodeIt = myStaticSPNet.begin();
       nodeIt !=myStaticSPNet.end();nodeIt++)
    {
      pathToFill.erase(pathToFill.begin(),pathToFill.end());
      tmpId = myStaticSPNet.getNodeId(nodeIt);
      myShortSolver.getPath(myStaticSPNet,myStaticSPNet.find(tmpId),pathToFill);
      //std::cout<<"Path for nodeId "<<tmpId<<" = "<<pathToFill<<std::endl;
    }
  ti->passItem();
}


int SPStaticFBTest(TestBed &myBed)
{
  TestItem::setBed(&myBed);
  test1();
  test2();
  return 0;
}
