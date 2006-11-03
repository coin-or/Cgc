/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/

#include <stdio.h>

#include <StaticNet.h>
#include <StaticFBNet.h>
#include <DynNet.h>
#include <DijkSolver.h>

#include "SPTestData.h"
#include "TestItem.h"
#include "TestBed.h"

typedef StaticNet< SPNodeLabel, SPArcCost > DynDijkSPNet1;
typedef StaticFBNet< SPNodeLabel, SPArcCost > DynDijkSPNet2;
typedef DynNet< SPNodeLabel, SPArcCost > DynDijkSPNet3;

static char labelString[1000];

static int t1obs=0;

template < class NetType >
void test1()
{
  sprintf(labelString,"Occurance[%d]",t1obs);
  t1obs++;
  strcat(labelString,"SPTest:1:Construction--compile time test");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();
  NetType mySPNet(1,1);

}

static int t2obs=0;
template < class DijkSPNetType >
void test2()
{
  sprintf(labelString,"Occurance[%d]",t2obs);
  t2obs++;
  strcat(labelString,"SPTest:2:Single Node Insert");
  TestItem *ti=new TestItem(labelString);

  DijkSPNetType mySPNet(1,1);
  SPNodeLabel tmp;
  
  typename DijkSPNetType::iterator nodeIt1 = mySPNet.insert( tmp );// NodeId0
  if(mySPNet.find(NodeId(0))!= nodeIt1)
    {
      std::cout<<"Iterator returned later is different!"<<std::endl;
      ti->failItem(__SPOT__);
    }

  if(*mySPNet.find(NodeId(0))!= *nodeIt1)
    {
      std::cout<<"*nodeIt="<<*(*nodeIt1)<<std::endl;
      std::cout<<"find returns "<<*(*mySPNet.find(NodeId(0)))<<std::endl;
      std::cout<<"Value returned later is different!"<<std::endl;
      ti->failItem(__SPOT__);
    }  
  ti->passItem();
}

static int t3obs=0;
template < class DijkSPNetType >
void test3()
{
  sprintf(labelString,"Occurance[%d]",t3obs);
  t3obs++;
  strcat(labelString,"SPTest:3:Multiple Node Insert");
  TestItem *ti=new TestItem(labelString);

  DijkSPNetType mySPNet(10,10);
  SPNodeLabel tmp;
  typename DijkSPNetType::iterator nodeIt1 = mySPNet.insert( tmp );// NodeId0

  if(*mySPNet.find(NodeId(0))!= *nodeIt1)
    {
      std::cout<<"There is some silly problem"<<std::endl;
      ti->failItem(__SPOT__);
    }


  mySPNet.insert( tmp ); // NodeId1
  mySPNet.insert( tmp ); // NodeId2
  mySPNet.insert( tmp ); // NodeId3
  mySPNet.insert( tmp ); // NodeId4

  typename DijkSPNetType::iterator orig1;
  typename DijkSPNetType::iterator dest1;
  orig1 = mySPNet.find(NodeId(0));
  dest1 = mySPNet.find(NodeId(1));
  mySPNet.arc_insert(orig1, SPArcCost(1), dest1);

  orig1 = mySPNet.find(NodeId(1));
  dest1 = mySPNet.find(NodeId(2));
  mySPNet.arc_insert( orig1, SPArcCost(2), dest1);

  orig1 = mySPNet.find(NodeId(1));
  dest1 = mySPNet.find(NodeId(4));
  mySPNet.arc_insert(orig1, SPArcCost(4),dest1);

  orig1 = mySPNet.find(NodeId(2));
  dest1 = mySPNet.find(NodeId(2));
  mySPNet.arc_insert(orig1, SPArcCost(2), dest1);

  orig1 = mySPNet.find(NodeId(1));
  dest1 = mySPNet.find(NodeId(3));
  mySPNet.arc_insert(orig1, SPArcCost(1),dest1);

  DijkSolver<DijkSPNetType, typename DijkSPNetType::iterator > myShortSolver1;
  Path pathToFill1;
  typename DijkSPNetType::iterator sourceNode1= mySPNet.begin();
  myShortSolver1.solve(mySPNet, sourceNode1);
  myShortSolver1.getPath(mySPNet, mySPNet.find(NodeId(2)),pathToFill1);
  if(pathToFill1.getCost()!=3)
      { std::cerr<<"ERROR"<<pathToFill1<<std::endl; ti->failItem(__SPOT__); return; }
  if(pathToFill1[0]!=NodeId(2))
      { ti->failItem(__SPOT__); return; }
  if(pathToFill1[1]!=NodeId(1))
      { ti->failItem(__SPOT__); return; }
  if(pathToFill1[2]!=NodeId(0))
      { ti->failItem(__SPOT__); return; }
  ti->passItem();
}

static int t4obs=0;
template < class DijkSPNetType >
void test4()
{
  sprintf(labelString,"Occurance[%d]",t4obs);
  t4obs++;
  strcat(labelString,"SPTest:4:Multiple Arc Inserts ");
  TestItem *ti=new TestItem(labelString);

  DijkSPNetType mySPNet(100,100);
  SPNodeLabel tmp;
  typename DijkSPNetType::iterator nodeIt = mySPNet.insert( tmp );// NodeId0
  if(mySPNet.find(NodeId(0))!= nodeIt)
    {
      std::cout<<"There is some silly problem"<<std::endl;
      ti->failItem(__SPOT__);
      return;
    }

  mySPNet.insert( tmp ); // NodeId1
  mySPNet.insert( tmp ); // NodeId2
  mySPNet.insert( tmp ); // NodeId3
  mySPNet.insert( tmp ); // NodeId4

  if(mySPNet.size()!= 5)
    {
      std::cout<<"Apparently has "<<mySPNet.size()<<" nodes."<<std::endl;
      ti->failItem(__SPOT__);
      return;
    }

  typename DijkSPNetType::iterator orig1;
  typename DijkSPNetType::iterator dest1;

  orig1 = mySPNet.find(NodeId(0));
  dest1 = mySPNet.find(NodeId(1));
  mySPNet.arc_insert(orig1, SPArcCost(1), dest1);

  orig1 = mySPNet.find(NodeId(1));
  dest1 = mySPNet.find(NodeId(2));
  mySPNet.arc_insert(orig1, SPArcCost(2), dest1);

  orig1 = mySPNet.find(NodeId(1));
  dest1 = mySPNet.find(NodeId(3));
  mySPNet.arc_insert(orig1, SPArcCost(1),dest1);

  orig1 = mySPNet.find(NodeId(1));
  dest1 = mySPNet.find(NodeId(4));
  mySPNet.arc_insert(orig1, SPArcCost(4), dest1);

  orig1 = mySPNet.find(NodeId(2));
  dest1 = mySPNet.find(NodeId(2));
  mySPNet.arc_insert(orig1, SPArcCost(2), dest1);
	
  if(mySPNet.arc_size()!= 5)
    {
      std::cout<<"Apparently has "<<mySPNet.arc_size()<<" arcs."<<std::endl;
      ti->failItem(__SPOT__);
      return;
    }
  ti->passItem();
}

static int tbobs=0;
template < class DijkSPNetType >
int buildTestNet(DijkSPNetType &netToFill)
{
  // build a 5 node 5 arc network
  sprintf(labelString,"Occurance[%d]",tbobs);
  tbobs++;
  strcat(labelString,"SPTest:aux:BuildTestNet");
  TestItem *ti=new TestItem(labelString);

  SPNodeLabel tmp;
  typename DijkSPNetType::iterator nodeIt = netToFill.insert( tmp );// NodeId0
  if(netToFill.find(NodeId(0))!= nodeIt)
    {
      std::cout<<"There is some silly problem"<<std::endl;
      ti->failItem(__SPOT__);
      return 1;
    }

  netToFill.insert( tmp ); // NodeId1
  netToFill.insert( tmp ); // NodeId2
  netToFill.insert( tmp ); // NodeId3
  netToFill.insert( tmp ); // NodeId4

  if(netToFill.size()!= 5)
    {
      std::cout<<"Apparently has "<<netToFill.size()<<" nodes."<<std::endl;
      ti->failItem(__SPOT__);
      return 1;
    }
	
  typename DijkSPNetType::iterator orig1;
  typename DijkSPNetType::iterator dest1;

  orig1 = netToFill.find(NodeId(0));
  dest1 = netToFill.find(NodeId(1));
  netToFill.arc_insert(orig1, SPArcCost(1), dest1);

  orig1 = netToFill.find(NodeId(1));
  dest1 = netToFill.find(NodeId(2));
  netToFill.arc_insert(orig1, SPArcCost(2), dest1);

  orig1 = netToFill.find(NodeId(1));
  dest1 = netToFill.find(NodeId(3));
  netToFill.arc_insert(orig1, SPArcCost(1),dest1);

  orig1 = netToFill.find(NodeId(1));
  dest1 = netToFill.find(NodeId(4));
  netToFill.arc_insert(orig1, SPArcCost(4), dest1);

  orig1 = netToFill.find(NodeId(2));
  dest1 = netToFill.find(NodeId(2));
  netToFill.arc_insert(orig1, SPArcCost(2), dest1);

  if(netToFill.arc_size()!= 5)
    {
      std::cout<<"Apparently has "<<netToFill.arc_size()<<" arcs."<<std::endl;
      ti->failItem(__SPOT__);
      return 1;
    }
  ti->passItem();
  return 0;
}

static int t5obs=0;
template <class DijkSPNetType >
void test5()
{
  sprintf(labelString,"Occurance[%d]",t5obs);
  t5obs++;
  strcat(labelString,"SPTest:5:testBuildTestNet");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();

  DijkSPNetType mySPNet(20,20);
  if(buildTestNet(mySPNet))
    ti->failItem(__SPOT__);
}

static int t6obs=0;
template <class DijkSPNetType >
void test6()
{
  sprintf(labelString,"Occurance[%d]",t6obs);
  t6obs++;
  strcat(labelString,"SPTest:6:SolverConstruct");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();

  DijkSPNetType mySPNet(20,20);
  if(buildTestNet(mySPNet))
    ti->failItem(__SPOT__);
  DijkSolver< DijkSPNetType , typename DijkSPNetType::iterator > net;
}

static int t7obs=0;
template <class DijkSPNetType >
void test7()
{
  typedef typename DijkSPNetType::Node DijkNodeType;
  sprintf(labelString,"Occurance[%d]",t7obs);
  t7obs++;
  strcat(labelString,"SPTest:7:Invalidate an arc");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();


  DijkSPNetType mySPNet(20,20);
  if(buildTestNet(mySPNet))
    ti->failItem(__SPOT__);
  // try to find and invalidate the head node
  typename DijkSPNetType::iterator  tailNode = mySPNet.find(NodeId(2));
  typename DijkSPNetType::iterator  headNode = mySPNet.find(NodeId(2));
  typename DijkNodeType::iterator arcIt = (*tailNode).begin();

  while(arcIt!= (*tailNode).end()&& (*arcIt).head()!=headNode)
      arcIt++;
  if(arcIt==(*tailNode).end())
    {
      ti->failItem(__SPOT__);
      return;
    }
  (*(*arcIt)).makeInvalid();
    
  DijkSolver< DijkSPNetType , typename DijkSPNetType::iterator > net;
}

static int t8obs=0;
template <class DijkSPNetType >
void test8()
{
  typedef typename DijkSPNetType::Node DijkNodeType;
  sprintf(labelString,"Occurance[%d]",t8obs);
  t8obs++;
  strcat(labelString,"SPTest:8:single arc graph");
  TestItem *ti=new TestItem(labelString);


  DijkSPNetType mySPNet(2,1);
  typename DijkSPNetType::iterator nodeIt1= mySPNet.insert(SPNodeLabel(0));
  typename DijkSPNetType::iterator nodeIt2 = mySPNet.insert(SPNodeLabel(0));
  mySPNet.arc_insert(nodeIt1, 3 , nodeIt2);
  DijkSolver<DijkSPNetType, typename DijkSPNetType::iterator > myShortSolver1;
  myShortSolver1.solve(mySPNet, nodeIt1);
  Path p;
  myShortSolver1.getPath(mySPNet,nodeIt2,p);
  std::cout<<"p="<<p<<std::endl;
  if(p.size()==0)
      ti->failItem(__SPOT__);
  if(p.size()!=2)
      ti->failItem(__SPOT__);
  ti->passItem();
}

int SPTest(TestBed &myBed)
{
  TestItem::setBed(&myBed);

#ifdef STATICTEST
  std::cout<<"Testing StaticNet"<<std::endl;
  test1<DynDijkSPNet1>();
  test2<DynDijkSPNet1>();
  test3<DynDijkSPNet1>();
  test4<DynDijkSPNet1>();
  test5<DynDijkSPNet1>();
  test6<DynDijkSPNet1>();
  test7<DynDijkSPNet1>();
  test8<DynDijkSPNet1>();

#endif
#ifdef STATICFBTEST
  std::cout<<"Testing StaticFBNet"<<std::endl;
  test1<DynDijkSPNet2>();
  test2<DynDijkSPNet2>();
  test3<DynDijkSPNet2>();
  test4<DynDijkSPNet2>();
  test5<DynDijkSPNet2>();
  test6<DynDijkSPNet2>();
  test7<DynDijkSPNet2>();
  test8<DynDijkSPNet2>();
#endif
#ifdef DYNTEST
  std::cout<<"Testing DynNet"<<std::endl;
  test1<DynDijkSPNet3>();
  test2<DynDijkSPNet3>();
  test3<DynDijkSPNet3>();
  test4<DynDijkSPNet3>();
  test5<DynDijkSPNet3>();
  test6<DynDijkSPNet3>();
  test7<DynDijkSPNet3>();
  test8<DynDijkSPNet3>();
#endif
  return 0;
}
