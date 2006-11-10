/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <MSTSolver.h>
#include "MSTData.h"

#include <StaticFBNet.h>
#include <DynNet.h>

#include "TestItem.h"
#include "TestBed.h"
#include <stdio.h>

#define DYNTEST
#define STATICFBTEST
typedef StaticFBNet< MSTNodeLabel, MSTArcCost > StaticMSTNet1;
typedef DynNet< MSTNodeLabel, MSTArcCost > DynMSTNet2;

static char labelString[1000];

template < class MSTNet >
void test1()
{
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString, "MSTTest1:Construction--compile test");
  TestItem *ti=new TestItem(labelString);

  MSTNet myStaticNet(100,100);
  ti->passItem();
  
}

template < class MSTNet >
void test2()
{
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"MSTTest2:Single Node Insert");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();
  
  MSTNet myStaticNet(10,10);
  MSTNodeLabel tmp;
  typename MSTNet::iterator nodeIt = myStaticNet.insert( tmp );// NodeId0
  if(myStaticNet.find(NodeId(0) ) != nodeIt )
    {
      std::cerr<<"There is some silly problem"<<std::endl;
      ti->failItem(__SPOT__);
    }

}

template <class MSTNet >
void printGraph(MSTNet &myStaticNet)
{
  typename MSTNet::iterator nodeIt;
  typedef typename MSTNet::Node MSTNode;
  typedef typename MSTNode::iterator MSTNodeIter;

  std::cout<<"Graph:"<<std::endl;
  for(nodeIt = myStaticNet.begin();
      nodeIt != myStaticNet.end();nodeIt++)
    {
      std::cout<<"At node:"<<myStaticNet.getNodeId(nodeIt)<<*(*nodeIt)<<std::endl;
      std::cout<<"Node has "<<(*nodeIt).size()<<" arcs"<<std::endl;
      for(MSTNodeIter arcIt = (*nodeIt).begin();
	  arcIt != (*nodeIt).end();arcIt++)
	std::cout<<" has arc = "<<*(*arcIt)<<" from "
	    <<myStaticNet.getNodeId((*arcIt).tail())
	    <<" to "
	    <<myStaticNet.getNodeId((*arcIt).head())<<std::endl;
    }
}

template <class MSTNet ,class MSTNodeIter >
void dumpArcInfo(MSTNet &net, MSTNodeIter arcIt )
{
  std::cout<<"Arc from "
      << net.getNodeId((*arcIt).tail())<<" to "
      << net.getNodeId((*arcIt).head())<<" with cost="<<(*(*arcIt)).getCost()<<std::endl;
}
  
template <class MSTNet>
void test3()
{
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"MSTTest3:Multiple Node Insert");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();
  
  typedef typename MSTNet::Node MSTNode;
  typedef typename MSTNode::iterator MSTNodeIter;
  MSTNet myStaticNet(100,100);
  MSTNodeLabel tmp;
  typename MSTNet::iterator node0 = myStaticNet.insert( tmp );// NodeId0
  if( myStaticNet.find(NodeId(0) ) != node0 )
    {
      std::cout<<"There is some silly problem"<<std::endl;
      ti->failItem(__SPOT__);
    }
  
  typename MSTNet::iterator node1=myStaticNet.insert( tmp ); // NodeId1
  typename MSTNet::iterator node2=myStaticNet.insert( tmp ); // NodeId2
  typename MSTNet::iterator node3=myStaticNet.insert( tmp ); // NodeId3
  typename MSTNet::iterator node4=myStaticNet.insert( tmp ); // NodeId4
  
  /* HPW */
  myStaticNet.arc_insert(node0,
			 MSTArcCost(1),
			 node1);
  myStaticNet.arc_insert(node1,
			 MSTArcCost(2),
			 node2);
  myStaticNet.arc_insert(node1,
			   MSTArcCost(4),
			   node4);
  myStaticNet.arc_insert(node2,
			   MSTArcCost(2),
			   node3);
  myStaticNet.arc_insert(node2,
			   MSTArcCost(1),
			   node4);

  printGraph(myStaticNet);
  
  MSTSolver<MSTNet> myTreeSolver;

  //std::cout<<"Calling solver now"<<std::endl;
  myTreeSolver.solve(myStaticNet );
  //std::cout<<"Solver done"<<std::endl;
  printGraph(myStaticNet);

  typedef typename MSTSolver<MSTNet>::ArcIndex MSTArcList;
  //MSTSolver<MSTNet>::ArcIndex arcList;
  MSTArcList arcList;

  myTreeSolver.getArcIndex(arcList);
  for(typename MSTArcList::iterator arcIt = arcList.begin();arcIt != arcList.end();
      arcIt++)
      dumpArcInfo<MSTNet> (myStaticNet,*arcIt);
}
template < class MSTNet>
void test4()
{
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"MSTTest4:Multiple Arc Inserts ");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();
  
  MSTNet myStaticNet(100,100);
  MSTNodeLabel tmp;
  typename MSTNet::iterator node0 = myStaticNet.insert( tmp );// NodeId0
  if(myStaticNet.find(NodeId(0))!= node0)
    {
      std::cout<<"There is some silly problem"<<std::endl;
      ti->failItem(__SPOT__);
    }
  
  typename MSTNet::iterator node1=myStaticNet.insert( tmp ); // NodeId1
  typename MSTNet::iterator node2=myStaticNet.insert( tmp ); // NodeId2
  typename MSTNet::iterator node3=myStaticNet.insert( tmp ); // NodeId3
  typename MSTNet::iterator node4=myStaticNet.insert( tmp ); // NodeId4
  
  if(myStaticNet.size()!= 5)
    {
      std::cout<<"Apparently has "<<myStaticNet.size()<<" nodes."<<std::endl;
      ti->failItem(__SPOT__);
    }
  
  myStaticNet.arc_insert(node0,
			   MSTArcCost(1),
			   node1);
  myStaticNet.arc_insert(node1,
			   MSTArcCost(2),
			   node2);
  myStaticNet.arc_insert(node1,
			   MSTArcCost(1),
			   node3);
  myStaticNet.arc_insert(node1,
			   MSTArcCost(4),
			   node4);
  myStaticNet.arc_insert(node2,
			   MSTArcCost(2),
			   node2);
  
  if(myStaticNet.arc_size()!= 5)
    {
      std::cout<<"Apparently has "<<myStaticNet.arc_size()<<" arcs."<<std::endl;
      ti->failItem(__SPOT__);
    }
}

template <class MSTNet>
int buildTestNet(MSTNet &netToFill)
{
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"MSTTestaux:BuildTestNet");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();
  
  MSTNodeLabel tmp;
  typename MSTNet::iterator node0 = netToFill.insert( tmp );// NodeId0
  if(netToFill.find(NodeId(0))!= node0)
    {
      std::cout<<"There is some silly problem"<<std::endl;
      ti->failItem(__SPOT__);
      return 1;
    }
  
  typename MSTNet::iterator node1=netToFill.insert( tmp ); // NodeId1
  typename MSTNet::iterator node2=netToFill.insert( tmp ); // NodeId2
  typename MSTNet::iterator node3=netToFill.insert( tmp ); // NodeId3
  typename MSTNet::iterator node4=netToFill.insert( tmp ); // NodeId4
  
  if(netToFill.size()!= 5)
    {
      std::cout<<"Apparently has "<<netToFill.size()<<" nodes."<<std::endl;
      ti->failItem(__SPOT__);
      return 1;
    }
  
  netToFill.arc_insert(node0,
		       MSTArcCost(1),
		       node1);
  netToFill.arc_insert(node1,
		       MSTArcCost(2),
		       node2);
  netToFill.arc_insert(node1,
		       MSTArcCost(1),
		       node3);
  netToFill.arc_insert(node1,
		       MSTArcCost(4),
		       node4);
  netToFill.arc_insert(node2,
		       MSTArcCost(2),
		       node2);
  
  if(netToFill.arc_size()!= 5)
    {
      std::cout<<"Apparently has "<<netToFill.arc_size()<<" arcs."<<std::endl;
      ti->failItem(__SPOT__);
      return 1;
    }
  return 0;
}

template <class MSTNet >
void test5()
{
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"MSTTest5:testBuildTestNet");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();
  
  MSTNet myStaticNet(100,100);
  if(buildTestNet(myStaticNet))
    ti->failItem(__SPOT__);
}

template <class MSTNet>
void test6()
{
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"MSTTest6:SolverConstruct");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();
  
  MSTNet myStaticNet(100,100);
  if(buildTestNet(myStaticNet))
    ti->failItem(__SPOT__);
  MSTSolver< MSTNet > net;
}

template <class MSTNet>
void stuffArc(MSTNet &net, 
	      unsigned short tl, 
	      int cost, 
	      unsigned short head)
{
  
  typename MSTNet::iterator nodeTail = net.find(NodeId(tl));
  typename MSTNet::iterator nodeHead = net.find(NodeId(head));
  net.arc_insert(nodeTail,
		 MSTArcCost(cost),
		 nodeHead);
}

template <class MSTNet>
void test7()
{
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"MSTTest7:Solving a 11 node problem");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();
  MSTNodeLabel lab;
  
  MSTNet myStaticNet(100,100);
  myStaticNet.insert(lab); // id 0
  myStaticNet.insert(lab); // id 1
  myStaticNet.insert(lab); // id 2
  myStaticNet.insert(lab); // id 3
  myStaticNet.insert(lab); // id 4
  myStaticNet.insert(lab); // id 5
  myStaticNet.insert(lab); // id 6
  myStaticNet.insert(lab); // id 7
  myStaticNet.insert(lab); // id 8
  myStaticNet.insert(lab); // id 9
  myStaticNet.insert(lab); // id 10
  stuffArc<MSTNet>(myStaticNet,0,19,1);
  stuffArc<MSTNet>(myStaticNet,0,20,2);
  stuffArc<MSTNet>(myStaticNet,1,6,2);
  stuffArc<MSTNet>(myStaticNet,1,7,3);
  stuffArc<MSTNet>(myStaticNet,2,8,4);
  stuffArc<MSTNet>(myStaticNet,3,9,4);
  stuffArc<MSTNet>(myStaticNet,3,11,5);
  stuffArc<MSTNet>(myStaticNet,3,12,6);
  stuffArc<MSTNet>(myStaticNet,4,10,5);
  stuffArc<MSTNet>(myStaticNet,5,13,6);
  stuffArc<MSTNet>(myStaticNet,5,14,7);
  stuffArc<MSTNet>(myStaticNet,6,15,7);
  stuffArc<MSTNet>(myStaticNet,7,6,8);
  stuffArc<MSTNet>(myStaticNet,7,7,9);
  stuffArc<MSTNet>(myStaticNet,8,8,9);
  stuffArc<MSTNet>(myStaticNet,8,20,10);
  stuffArc<MSTNet>(myStaticNet,9,19,10);

  MSTSolver<MSTNet> myTreeSolver;
  myTreeSolver.solve(myStaticNet );
  //std::cout<<"Solver done"<<std::endl;
  printGraph(myStaticNet);

  typedef typename MSTSolver<MSTNet>::ArcIndex MSTArcList;
  MSTArcList arcList;
  
  myTreeSolver.getArcIndex(arcList);

  int cost=0;
  for(typename MSTArcList::iterator arcIt = arcList.begin();arcIt != arcList.end();
      arcIt++)
  {
      dumpArcInfo(myStaticNet,*arcIt);
      cost += (*(*(*arcIt))).getCost();
  }
  if(cost != 108) // known answer to this problem
    {
      std::cout<<"ERROR: Cost is:"<<cost<<std::endl;
      ti->failItem(__SPOT__);
    }
  std::cout<<"Total cost is:"<<cost<<std::endl;
}


int MSTTest(TestBed &myBed)
{
  TestItem::setBed(&myBed);
  
#ifdef STATICFBTEST
  std::cout<<"Testing StaticMSTNet"<<std::endl;
  test1<StaticMSTNet1>();
  test2<StaticMSTNet1>();
  test3<StaticMSTNet1>();
  test4<StaticMSTNet1>();
  test5<StaticMSTNet1>();
  test6<StaticMSTNet1>();
  test7<StaticMSTNet1>();
#endif
#ifdef DYNTEST
  std::cout<<"Testing DynNet"<<std::endl;
  test1<DynMSTNet2>();
  test2<DynMSTNet2>();
  test3<DynMSTNet2>();
  test4<DynMSTNet2>();
  test5<DynMSTNet2>();
  test6<DynMSTNet2>();
  test7<DynMSTNet2>();
#endif
  return 0;
}
