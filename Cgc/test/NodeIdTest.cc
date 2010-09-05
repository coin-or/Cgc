/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <iostream>
#include <NodeId.h>
#include "TestItem.h"
#include "TestBed.h"

#include <sstream>
#include <string.h>

static void test1()
{
  TestItem *item1 = new TestItem("NodeIdTest:1:Construct");
  NodeId nodeId1(1);
  std::cout<<"Id1 = "<<nodeId1<<std::endl;
  item1->passItem();
}

static void test2()
{
  TestItem *item2 = new TestItem("NodeIdTest:2:++pre and post++");
  NodeId nodeId1(1);
  NodeId nodeId2(2);
  NodeId nodeId3(3);
  if(nodeId1++ ==nodeId2)
    item2->failItem(__SPOT__);
  if(++nodeId2 ==nodeId3)
    item2->passItem();
}

static void test3()
{
  TestItem *item1 = new TestItem("NodeIdTest:3:Assignment");
  NodeId nodeId1(1);
  NodeId nodeId2(3);
  if(nodeId1 == nodeId2)
    item1->failItem(__SPOT__);

  nodeId2 = nodeId1;
  if(nodeId1 != nodeId2)
    item1->failItem(__SPOT__);

  item1->passItem();
}

static void test4()
{
  TestItem *ti = new TestItem("NodeIdTest:4:I/O 2 ways");
  char buffer[128];
  NodeId n20(20); 
  NodeId n31(31);
  NodeId n40(40);
  NodeId n21(21); 
  NodeId n32(32);
  NodeId n41(41);
  {
  std::ostringstream ostrm;
  ostrm<<n20<<n31<<n40<<std::ends;
  strcpy(buffer,ostrm.str().c_str());
  }
  {
  std::string s(buffer);
  std::istringstream istrm(s);
  istrm>>n21>>n32>>n41;
  }
  if(n20!=n21)
    ti->failItem(__SPOT__);
  if(n31!=n32)
    ti->failItem(__SPOT__);
  if(n40!=n41)
    ti->failItem(__SPOT__);
  ti->passItem();
}
  
int NodeIdTest(TestBed &myBed)
{
  TestItem::setBed(&myBed);

  test1(); 
  test2(); 
  test3(); 
  test4(); 
  return 0;
}
