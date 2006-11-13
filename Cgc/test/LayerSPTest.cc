/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <stdio.h>

#include <StaticFBNet.h>
#include <DynNet.h>

#include "SPTestData.h"
#include <LayerSPSolver.h>
#include "TestItem.h"
#include "TestBed.h"
#include <iostream>

#define STATICFBTEST
#define DYNTEST

typedef StaticFBNet< SPNodeLabel, SPArcCost > DynLayerSPNet2;
typedef StaticFBNet< SPNodeLabel, SPArcCost >::Node DynLayerSPNetNode;
typedef DynNet< SPNodeLabel, SPArcCost > DynLayerSPNet3;

char labelString[1000];

static int t1obs=0;

template < class NetType >
void test1()
{
  sprintf(labelString,"Occurance[%d]",t1obs);
  t1obs++;
  strcat(labelString,"LayerSPTest:1:Construction--compile time test");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();
}

template < class LayerSPNetType >
void test2()
{
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"LayerSPTest:2:Single Node Insert");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();

  LayerSPNetType mySPNet(1,1);
  SPNodeLabel tmp;
  
  typename LayerSPNetType::iterator nodeIt1 = mySPNet.insert( tmp );// NodeId0
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
}

template < class LayerSPNetType >
void test3()
{
  typedef typename LayerSPNetType::Node LayerNetNode;
  typedef typename LayerSPNetType::Arc LayerNetArc;
  typedef LayerNetArc * LayerNetArcPnt;
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"LayerSPTest:3:Multiple Node Insert");
  TestItem *ti=new TestItem(labelString);

  ti->passItem();

  LayerSPNetType mySPNet(10,10);
  SPNodeLabel tmp;
  typename LayerSPNetType::iterator nodeIt1 = mySPNet.insert( tmp );// NodeId0

  if(*mySPNet.find(NodeId(0))!= *nodeIt1)
    {
      std::cout<<"There is some silly problem"<<std::endl;
      ti->failItem(__SPOT__);
    }


  mySPNet.insert( tmp ); // NodeId1
  mySPNet.insert( tmp ); // NodeId2
  mySPNet.insert( tmp ); // NodeId3
  mySPNet.insert( tmp ); // NodeId4

  /* HPW */

  typename LayerSPNetType::iterator orig1;
  typename LayerSPNetType::iterator dest1;
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

  LayerSPSolver<LayerSPNetType, typename LayerSPNetType::iterator > myShortSolver1;
  ArcPath<LayerNetArcPnt> pathToFill1;
  typename LayerSPNetType::iterator sourceNode1= mySPNet.begin();
  myShortSolver1.solve(mySPNet, sourceNode1);
  myShortSolver1.getPath(mySPNet, mySPNet.find(NodeId(2)),pathToFill1);
}

template < class LayerSPNetType >
void test4()
{
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"LayerSPTest:4:Multiple Arc Inserts ");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();

  LayerSPNetType mySPNet(100,100);
  SPNodeLabel tmp;
  typename LayerSPNetType::iterator nodeIt = mySPNet.insert( tmp );// NodeId0
  if(mySPNet.find(NodeId(0))!= nodeIt)
    {
      std::cout<<"There is some silly problem"<<std::endl;
      ti->failItem(__SPOT__);
    }

  mySPNet.insert( tmp ); // NodeId1
  mySPNet.insert( tmp ); // NodeId2
  mySPNet.insert( tmp ); // NodeId3
  mySPNet.insert( tmp ); // NodeId4

  if(mySPNet.size()!= 5)
    {
      std::cout<<"Apparently has "<<mySPNet.size()<<" nodes."<<std::endl;
      ti->failItem(__SPOT__);
    }

  typename LayerSPNetType::iterator orig1;
  typename LayerSPNetType::iterator dest1;

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
    }
}

template < class LayerSPNetType >
int buildTestNet(LayerSPNetType &netToFill)
{
  // build a 5 node 5 arc network
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"aux:BuildTestNet");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();

  SPNodeLabel tmp;
  typename LayerSPNetType::iterator nodeIt = netToFill.insert( tmp );// NodeId0
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
	
  typename LayerSPNetType::iterator orig1;
  typename LayerSPNetType::iterator dest1;

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
  return 0;
}

template < class LayerSPNetType >
int buildComplexTestNet(LayerSPNetType &netToFill)
{
  // build a 7 node, many-arc network
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"LayerSPTest:Complex network builder");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();
  
  SPNodeLabel tmp;
  typename LayerSPNetType::iterator nodeIt = netToFill.insert( tmp );// NodeId0
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
  netToFill.insert( tmp ); // NodeId5
  netToFill.insert( tmp ); // NodeId6

  if(netToFill.size()!= 7)
    {
      std::cout<<"ERROR: size ="<<netToFill.size()<<std::endl;
      ti->failItem(__SPOT__);
      return 1;
    }
	
  typename LayerSPNetType::iterator orig1;
  typename LayerSPNetType::iterator dest1;
  SPArcCost tmpArcCost;

  orig1 = netToFill.find(NodeId(0));
  dest1 = netToFill.find(NodeId(1));
  tmpArcCost.makeValid();
  tmpArcCost.makeBackValid();
  tmpArcCost.setCost(1);
  netToFill.arc_insert(orig1, tmpArcCost, dest1);

  orig1 = netToFill.find(NodeId(1));
  dest1 = netToFill.find(NodeId(2));
  tmpArcCost.setCost(2);
  tmpArcCost.makeBackInvalid();
  netToFill.arc_insert(orig1, tmpArcCost, dest1);

  orig1 = netToFill.find(NodeId(1));
  dest1 = netToFill.find(NodeId(3));
  tmpArcCost.setCost(3);
  tmpArcCost.makeInvalid();
  tmpArcCost.makeBackValid();
  netToFill.arc_insert(orig1, tmpArcCost,dest1);

  orig1 = netToFill.find(NodeId(1));
  dest1 = netToFill.find(NodeId(4));
  tmpArcCost.setCost(4);
  tmpArcCost.makeValid();
  tmpArcCost.makeBackValid();
  netToFill.arc_insert(orig1, tmpArcCost, dest1);

  orig1 = netToFill.find(NodeId(3));
  dest1 = netToFill.find(NodeId(2));
  tmpArcCost.setCost(1);
  tmpArcCost.makeInvalid();
  tmpArcCost.makeBackValid();
  netToFill.arc_insert(orig1, tmpArcCost, dest1);


  orig1 = netToFill.find(NodeId(2));
  dest1 = netToFill.find(NodeId(2));
  tmpArcCost.setCost(2);
  tmpArcCost.makeValid();
  tmpArcCost.makeBackValid();
  netToFill.arc_insert(orig1, tmpArcCost, dest1);

  orig1 = netToFill.find(NodeId(2));
  dest1 = netToFill.find(NodeId(5));
  tmpArcCost.setCost(3);
  tmpArcCost.makeValid();
  tmpArcCost.makeBackInvalid();
  netToFill.arc_insert(orig1, tmpArcCost, dest1);

  orig1 = netToFill.find(NodeId(3));
  dest1 = netToFill.find(NodeId(6));
  tmpArcCost.setCost(1);
  tmpArcCost.makeValid();
  tmpArcCost.makeBackValid();
  netToFill.arc_insert(orig1, tmpArcCost, dest1);

  orig1 = netToFill.find(NodeId(3));
  dest1 = netToFill.find(NodeId(5));
  tmpArcCost.setCost(10);
  tmpArcCost.makeValid();
  tmpArcCost.makeBackInvalid();
  netToFill.arc_insert(orig1, tmpArcCost, dest1);

  orig1 = netToFill.find(NodeId(4));
  dest1 = netToFill.find(NodeId(5));
  tmpArcCost.setCost(3);
  tmpArcCost.makeValid();
  tmpArcCost.makeBackInvalid();
  netToFill.arc_insert(orig1, tmpArcCost, dest1);

  orig1 = netToFill.find(NodeId(5));
  dest1 = netToFill.find(NodeId(6));
  tmpArcCost.setCost(16);
  tmpArcCost.makeValid();
  tmpArcCost.makeBackInvalid();
  netToFill.arc_insert(orig1, tmpArcCost, dest1);

  return 0;
}

template <class LayerSPNetType >
void test5()
{
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"LayerSPTest:5:testBuildTestNet");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();

  LayerSPNetType mySPNet(20,20);
  if(buildTestNet(mySPNet))
    ti->failItem(__SPOT__);
}

template <class LayerSPNetType >
void test6()
{
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"LayerSPTest:6:SolverConstruct");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();

  LayerSPNetType mySPNet(20,20);
  if(buildTestNet(mySPNet))
    ti->failItem(__SPOT__);
  LayerSPSolver< LayerSPNetType , typename LayerSPNetType::iterator > net;
}

template <class LayerSPNetType >
void test7()
{
  typedef typename LayerSPNetType::Node LayerNodeType;
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  obs++;
  strcat(labelString,"LayerSPTest:7:Invalidate an arc");
  TestItem *ti=new TestItem(labelString);
  ti->passItem();


  LayerSPNetType mySPNet(20,20);
  if(buildTestNet(mySPNet))
    ti->failItem(__SPOT__);
  // try to find and invalidate the head node
  typename LayerSPNetType::iterator  tailNode = mySPNet.find(NodeId(2));
  typename LayerSPNetType::iterator  headNode = mySPNet.find(NodeId(2));
  typename LayerNodeType::iterator arcIt = (*tailNode).begin();

  while(arcIt!= (*tailNode).end()&& (*arcIt).head()!=headNode)
      arcIt++;
  if(arcIt==(*tailNode).end())
    {
      ti->failItem(__SPOT__);
      return;
    }
  (*(*arcIt)).makeInvalid();
    
  LayerSPSolver< LayerSPNetType , typename LayerSPNetType::iterator > solver;
}

template <class LayerSPNetType >
void test8()
{
  typedef typename LayerSPNetType::Node LayerNodeType;
  static int obs=0;
  sprintf(labelString,"Occurance[%d]",obs);
  strcat(labelString,"LayerSPTest:08:Complicated Layered Shortest Path test");
  TestItem *ti=new TestItem(labelString);
  LayerSPNetType mySPNet(7,20);
  if(buildComplexTestNet(mySPNet))
    ti->failItem(__SPOT__);
  else
    ti->passItem();
  LayerSPSolver< LayerSPNetType , typename LayerSPNetType::iterator > solver;
  ArcPath< typename LayerSPNetType::Arc * > pathToFill;
  typename LayerSPNetType::iterator sourceIt = mySPNet.find(NodeId(1));
  solver.solve(mySPNet,sourceIt);
  solver.getPath(mySPNet,mySPNet.find(NodeId(6)),pathToFill);
  if(pathToFill.getCost()!=4)
    {
      std::cout<<"Got value="<<pathToFill.getCost()<<std::endl;
      ti->failItem(__SPOT__);
    }
//  std::cout<<pathToFill<<std::endl;
  mySPNet.find(NodeId(3));
}


int LayerSPTest(TestBed &myBed)
{
  TestItem::setBed(&myBed);

#ifdef STATICFBTEST
  test1<DynLayerSPNet2>();
  test2<DynLayerSPNet2>();
  test3<DynLayerSPNet2>();
  test4<DynLayerSPNet2>();
  test5<DynLayerSPNet2>();
  test6<DynLayerSPNet2>();
  test7<DynLayerSPNet2>();
#else
TestItem *ti = new TestItem("StaticFBNet with LayeredSP");
ti->failItem(__SPOT__);
#endif
#ifdef DYNTEST
  test1<DynLayerSPNet3>();
  test2<DynLayerSPNet3>();
  test3<DynLayerSPNet3>();
  test4<DynLayerSPNet3>();
  test5<DynLayerSPNet3>();
  test6<DynLayerSPNet3>();
  test7<DynLayerSPNet3>();
#endif
  test8<DynLayerSPNet3>();
  return 0;
}
