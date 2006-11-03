/*
  Author: H. Philip Walton
  Copyright (C) 2006 H. Philip Walton
  All Rights Reserved.
*/
#include <PathFind.h>
#include <StaticNet.h>
#include <DynNet.h>

#include "TestItem.h"
#include "TestBed.h"

typedef int NodeType;
typedef int ArcType;
typedef StaticNet<NodeType,ArcType> MyNetType;
//typedef DynNet<NodeType,ArcType> MyNetType;
typedef PathFind<MyNetType> PathFinder;

std::ostream &operator<<(std::ostream &os, const MyNetType &net)
{
  os<<"My Net has "<<net.size()
      <<" nodes and "<<net.arc_size()<<" arcs."<<std::endl;
  return os;
}
/*
std::ostream &operator<<(std::ostream &os, const JustAPath &path)
{
  os<<'['<<path.size()<<"]:";
  for(JustAPath::const_iterator pathIt = path.begin();
      pathIt != path.end(); pathIt++)
    os<<(*pathIt)<<':';
  return os;
}
std::ostream &operator<<(std::ostream &os, const JustPaths &paths)
{
  os<<"Paths contains "<<paths.size()<<" paths"<<std::endl;
  for(JustPaths::const_iterator pathIt = paths.begin();
      pathIt != paths.end(); pathIt++)
      os<<(*pathIt)<<std::endl;
  return os;
}
*/

void buildSimpleNet(MyNetType &netToFill)
{
  netToFill.insert(0);
  netToFill.insert(1);
  netToFill.insert(2);
  netToFill.insert(3);
  netToFill.insert(4);
  netToFill.insert(5);
  netToFill.arc_insert(netToFill.find(0),0,netToFill.find(1));
  netToFill.arc_insert(netToFill.find(1),0,netToFill.find(2));
  netToFill.arc_insert(netToFill.find(1),0,netToFill.find(3));
  netToFill.arc_insert(netToFill.find(1),0,netToFill.find(4));
  netToFill.arc_insert(netToFill.find(2),0,netToFill.find(4));
  netToFill.arc_insert(netToFill.find(2),0,netToFill.find(5));
  netToFill.arc_insert(netToFill.find(3),0,netToFill.find(4));
  netToFill.arc_insert(netToFill.find(4),0,netToFill.find(5));
  std::cout<<netToFill<<std::endl;
}

static void test1()
{
  TestItem *ti = new TestItem("PathFindTest:1:Find all paths in a staticNet");
  MyNetType myNet(100,100);
  buildSimpleNet(myNet);
  MyNetType::iterator backOne = myNet.end();
  backOne--;
  PathFinder myFind(myNet,myNet.begin(),backOne);
  JustPaths paths;
  myFind.getPaths(paths);
  std::cout<<paths<<std::endl;
  if(paths.size()!=4)
    ti->failItem(__SPOT__);
  else
    ti->passItem();
}

int PathFindTest(TestBed &myBed)
{
  TestItem::setBed(&myBed);
  test1();
  return 0;
}
